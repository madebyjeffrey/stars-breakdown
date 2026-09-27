"""Bind original HST resource inputs to the production task-5 services."""

from pathlib import Path
import subprocess
import tempfile
import unittest

from tools.race_file_compare import Inspector
from tools.stars_game_stream import decode_stream
from tools.satisfy_orders_design_cost_oracle import recover as design_oracle
from tools.satisfy_orders_planet10_oracle import decode_planet10
from tools.satisfy_orders_racebit5_oracle import recover as resource_oracle


ROOT = Path(__file__).resolve().parents[1]


class SatisfyOrdersVerifiedResourceTests(unittest.TestCase):
    def test_original_input_bound_to_production_resource_service(self):
        rows = decode_stream(ROOT / "runs/syorig-racebit5-bundle-1/CLEAN.HST",
                             Inspector())
        player = rows[1]["body"]
        self.assertEqual(list(player[0x3e:0x4d]),
                         [10, 10, 10, 10, 10, 5, 10, 0, 1, 1, 1, 1, 1, 1, 9])
        planet = decode_planet10(rows[13]["body"])
        self.assertEqual(planet["minerals"], [469, 893, 376, 250])
        self.assertEqual(planet["improvements"]["factories"], 10)
        self.assertEqual(design_oracle()["owner_design_cost"],
                         [24, 12, 8, 7])
        original = resource_oracle()
        self.assertEqual(original["resource_scrap_minus_clean_minerals"],
                         [10, 7, 6, 0])
        self.assertEqual(original["resource_message_ids"][:2],
                         [0x5d, 0x143])
        with tempfile.TemporaryDirectory() as directory:
            exe = Path(directory) / "verified_resource"
            subprocess.run([
                "cc", "-std=c89", "-pedantic", "-Wall", "-Wextra", "-Werror",
                "-I", str(ROOT / "reconstructed/include"),
                str(ROOT / "tests/satisfy_orders_task5_verified_resource_probe.c"),
                str(ROOT / "reconstructed/src/satisfy_orders.c"),
                str(ROOT / "reconstructed/src/planet.c"),
                "-lm", "-o", str(exe),
            ], check=True)
            output = subprocess.check_output([str(exe)], text=True).strip()
        self.assertEqual(output,
                         "RESOURCE_BOUND_CHECKS=22 FAILURES=0 RESULT=PASS "
                         "BASE=35 ORE=479,900,382 EXTRA=24 MSG=93,323")


if __name__ == "__main__":
    unittest.main()
