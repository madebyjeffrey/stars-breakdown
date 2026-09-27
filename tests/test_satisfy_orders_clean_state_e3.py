"""Compare the clean original turn's scoped fleet/order fields with production."""

import json
from pathlib import Path
import re
import subprocess
import tempfile
import unittest

from tools.satisfy_orders_original_oracle import decode_compact_order, decode_owner_fleet


ROOT = Path(__file__).resolve().parents[1]
ORACLE = ROOT / "analysis/compat/satisfy-orders-original-m1-stable-e3.json"


class SatisfyOrdersCleanStateTests(unittest.TestCase):
    def test_scoped_original_fields_match_four_pass_production(self):
        oracle = json.loads(ORACLE.read_text())
        fleets = [decode_owner_fleet(bytes.fromhex(body))
                  for body in oracle["fleet_bodies_hex"]]
        order = decode_compact_order(bytes.fromhex(oracle["order_body_hex"]))
        self.assertEqual(order["task"], 0)
        self.assertEqual(order["object_kind"], 1)
        self.assertEqual(order["flags"], 0x1100)
        self.assertEqual([fleet["saved_flags"] for fleet in fleets],
                         [0x2907] * 6)
        with tempfile.TemporaryDirectory() as temp:
            exe = Path(temp) / "clean_state_probe"
            subprocess.run([
                "cc", "-std=c89", "-pedantic", "-Wall", "-Wextra", "-Werror",
                "-I", str(ROOT / "reconstructed/include"),
                str(ROOT / "tests/satisfy_orders_clean_state_e3_probe.c"),
                str(ROOT / "reconstructed/src/do_orders.c"),
                str(ROOT / "reconstructed/src/satisfy_orders.c"),
                "-o", str(exe),
            ], check=True)
            result = subprocess.run([str(exe)], capture_output=True,
                                    text=True, check=False)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        lines = result.stdout.splitlines()
        self.assertEqual(lines[0], "PASSES=1,2,3,4 DONE=0 MESSAGES=0")
        self.assertEqual(len(lines), 7)
        pattern = re.compile(
            r"FLEET=(\d+) OWNER=(\d+) PLANET=(\d+) FUEL=(\d+) "
            r"ORDER=(\d+) ACTIVE=(\d+)")
        for line, original in zip(lines[1:], fleets):
            match = pattern.fullmatch(line)
            self.assertIsNotNone(match, line)
            identity, owner, planet, fuel, task, active = map(int, match.groups())
            self.assertEqual((identity, owner, planet, fuel, task, active),
                             (original["id"], original["owner"],
                              original["planet"], original["cargo"][4],
                              order["task"], order["valid_task"]))


if __name__ == "__main__":
    unittest.main()
