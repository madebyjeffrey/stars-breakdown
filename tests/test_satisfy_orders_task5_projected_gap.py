"""Pin the task-5 original/production divergence at the projected fleet boundary.

This is a characterization of an OPEN compatibility gap, not an acceptance
test for the full SatisfyOrders branch. The typed adapter cannot yet ingest
the original ship-design, planet-resource, and message-service state.
"""

import hashlib
from pathlib import Path
import re
import subprocess
import tempfile
import unittest

from tools.race_file_compare import Inspector
from tools.satisfy_orders_original_oracle import decode_owner_fleet
from tools.stars_game_stream import decode_stream


ROOT = Path(__file__).resolve().parents[1]
RUN = ROOT / "runs/vmctl-syact3"


class SatisfyOrdersTask5ProjectedGapTests(unittest.TestCase):
    def test_original_task5_planet_mineral_delta(self):
        inspector = Inspector()
        control = decode_stream(ROOT / "runs/vmctl-syorig1/CLEAN.M1", inspector)
        action = decode_stream(RUN / "CLEAN.M1", inspector)

        def planet(records):
            matches = [record["body"] for record in records
                       if record["kind"] == 13 and record["turn"] == 1
                       and record["body"][:2] == b"\x0a\x00"]
            self.assertEqual(len(matches), 1)
            return matches[0]

        before, after = planet(control), planet(action)
        self.assertEqual(len(before), len(after))
        minerals_before = [int.from_bytes(before[i:i + 2], "little")
                           for i in (17, 19, 21)]
        minerals_after = [int.from_bytes(after[i:i + 2], "little")
                          for i in (17, 19, 21)]
        self.assertEqual([after - before for before, after in
                          zip(minerals_before, minerals_after)], [9, 5, 5])

    def test_original_task5_message_prefix_decodes_raw_arguments(self):
        inspector = Inspector()
        records = decode_stream(RUN / "CLEAN.M1", inspector)
        messages = [record["body"] for record in records
                    if record["kind"] == 12 and record["turn"] == 1]
        self.assertEqual(len(messages), 1)
        body = messages[0]
        self.assertEqual(body[:16].hex(),
                         "5a000a000014000a41010a000014000a")
        self.assertEqual(body[16:].hex(), "3f000a000a")
        first, second = body[:8], body[8:16]
        self.assertEqual((int.from_bytes(first[:2], "little"),
                          int.from_bytes(first[2:4], "little"),
                          list(first[4:])), (0x5a, 10, [0, 20, 0, 10]))
        self.assertEqual((int.from_bytes(second[:2], "little"),
                          int.from_bytes(second[2:4], "little"),
                          list(second[4:])), (0x141, 10, [0, 20, 0, 10]))

    def test_packed_task5_accounting_probe(self):
        with tempfile.TemporaryDirectory() as temp:
            exe = Path(temp) / "task5_accounting_probe"
            subprocess.run([
                "cc", "-std=c89", "-pedantic", "-Wall", "-Wextra", "-Werror",
                "-I", str(ROOT / "reconstructed/include"),
                str(ROOT / "tests/satisfy_orders_task5_accounting_probe.c"),
                str(ROOT / "reconstructed/src/satisfy_orders.c"),
                "-o", str(exe),
            ], check=True)
            result = subprocess.run([str(exe)], capture_output=True,
                                    text=True, check=False)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertEqual(result.stdout.strip(),
                         "TASK5_ACCOUNTING_CHECKS=19 FAILURES=0 RESULT=PASS")

    def test_packed_task5_service_probe(self):
        with tempfile.TemporaryDirectory() as temp:
            exe = Path(temp) / "task5_services_probe"
            subprocess.run([
                "cc", "-std=c89", "-pedantic", "-Wall", "-Wextra", "-Werror",
                "-I", str(ROOT / "reconstructed/include"),
                str(ROOT / "tests/satisfy_orders_task5_services_probe.c"),
                str(ROOT / "reconstructed/src/satisfy_orders.c"),
                "-o", str(exe),
            ], check=True)
            result = subprocess.run([str(exe)], capture_output=True,
                                    text=True, check=False)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertEqual(result.stdout.strip(),
                         "TASK5_SERVICES_CHECKS=30 FAILURES=0 RESULT=PASS")

    def test_packed_task5_resource_conversion_probe(self):
        with tempfile.TemporaryDirectory() as temp:
            exe = Path(temp) / "task5_resource_probe"
            subprocess.run([
                "cc", "-std=c89", "-pedantic", "-Wall", "-Wextra", "-Werror",
                "-I", str(ROOT / "reconstructed/include"),
                str(ROOT / "tests/satisfy_orders_task5_resource_probe.c"),
                str(ROOT / "reconstructed/src/satisfy_orders.c"),
                str(ROOT / "reconstructed/src/mark_techs.c"),
                "-o", str(exe),
            ], check=True)
            result = subprocess.run([str(exe)], capture_output=True,
                                    text=True, check=False)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertEqual(result.stdout.strip(),
                     "TASK5_RESOURCE_CHECKS=76 FAILURES=0 RESULT=PASS")

    def test_packed_task5_scrap_service_retires_original_order(self):
        with tempfile.TemporaryDirectory() as temp:
            exe = Path(temp) / "task5_retirement_probe"
            subprocess.run([
                "cc", "-std=c89", "-pedantic", "-Wall", "-Wextra", "-Werror",
                "-I", str(ROOT / "reconstructed/include"),
                str(ROOT / "tests/satisfy_orders_task5_retirement_probe.c"),
                str(ROOT / "reconstructed/src/satisfy_orders.c"),
                "-o", str(exe),
            ], check=True)
            result = subprocess.run([str(exe)], capture_output=True,
                                    text=True, check=False)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertEqual(result.stdout.strip(),
                         "TASK5_RETIREMENT_CHECKS=7 FAILURES=0 RESULT=PASS")

    def test_original_task5_fleet_retirement_requires_scrap_binding(self):
        self.assertEqual(
            hashlib.sha256((RUN / "BACKUP/CLEAN.HST").read_bytes()).hexdigest(),
            "aa6161bb6c95c8276b7a5d12e5a12d9999e3dea0fa242a3be89a94a05c45df3c",
        )
        inspector = Inspector()
        host = decode_stream(RUN / "BACKUP/CLEAN.HST", inspector)
        order = host[45]
        self.assertEqual((order["turn"], order["kind"], order["body"].hex()),
                         (0, 19, "680414050a00051100000000000000000000"))
        turn = decode_stream(RUN / "CLEAN.M1", inspector)
        original_ids = [decode_owner_fleet(record["body"])["id"]
                        for record in turn
                        if record["turn"] == 1 and record["kind"] == 16]
        self.assertEqual(original_ids, [1, 2, 3, 4, 5])

        with tempfile.TemporaryDirectory() as temp:
            exe = Path(temp) / "task5_projected_probe"
            subprocess.run([
                "cc", "-std=c89", "-pedantic", "-Wall", "-Wextra", "-Werror",
                "-I", str(ROOT / "reconstructed/include"),
                str(ROOT / "tests/satisfy_orders_clean_state_e3_probe.c"),
                str(ROOT / "reconstructed/src/do_orders.c"),
                str(ROOT / "reconstructed/src/satisfy_orders.c"),
                "-o", str(exe),
            ], check=True)
            result = subprocess.run([str(exe), "--task5"], capture_output=True,
                                    text=True, check=False)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertEqual(result.stdout.splitlines()[0],
                         "PASSES=1,2,3,4 DONE=0 MESSAGES=0")
        match = re.fullmatch(
            r"FLEET=0 OWNER=0 PLANET=10 FUEL=50 ORDER=5 ACTIVE=1",
            result.stdout.splitlines()[1],
        )
        self.assertIsNotNone(match, result.stdout)
        self.assertNotIn(0, original_ids)


if __name__ == "__main__":
    unittest.main()
