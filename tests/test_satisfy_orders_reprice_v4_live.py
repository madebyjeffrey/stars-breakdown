"""Bind original high-offer task-5 inputs to the reconstructed live path."""

from pathlib import Path
import subprocess
import tempfile
import unittest


ROOT = Path(__file__).resolve().parents[1]
EXPECTED = (
    "V4_LIVE_TASK5_CHECKS=74 FAILURES=0 RESULT=PASS "
    "LOOKUPS=12 REPRICE=4 COST=1930,494,681,261 "
    "EVENTS=KUUUUMTMRD MESSAGES=90,317 RECIPIENTS=1,0 "
    "MINERALS=864,1437,584 TECH=1 RESEARCH0=0 FLEET_DEAD=1\n"
)


class SatisfyOrdersRepriceV4LiveTests(unittest.TestCase):
    def test_original_high_offer_inputs_reprice_in_packed_task5(self):
        with tempfile.TemporaryDirectory() as directory:
            executable = Path(directory) / "v4-live"
            subprocess.run([
                "cc", "-std=c89", "-pedantic", "-Wall", "-Wextra",
                "-Werror", "-Ireconstructed/include", "-Itests",
                "reconstructed/src/reprice_shdef.c",
                "reconstructed/src/lookup_part.c",
                "reconstructed/src/true_part_cost.c",
                "reconstructed/src/mark_techs.c",
                "reconstructed/src/satisfy_orders.c",
                "reconstructed/src/itech_learn_a_tech.c",
                "tests/satisfy_orders_reprice_v4_live_probe.c",
                "-o", str(executable),
            ], cwd=ROOT, check=True, capture_output=True)
            result = subprocess.run([str(executable)], cwd=ROOT, check=True,
                                    capture_output=True, text=True)
        self.assertEqual(result.stdout, EXPECTED)


if __name__ == "__main__":
    unittest.main()
