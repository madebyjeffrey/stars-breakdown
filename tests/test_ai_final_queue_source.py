"""Source regressions required by the SO-E3 final AI queue binding."""

from pathlib import Path
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]


class AiFinalQueueSourceTests(unittest.TestCase):
    def test_queue_reconciliation_and_budget_boundary(self):
        for name, source, expected in (
            ("destroy_cur_game", "file", "DESTROY_CUR_GAME_SOURCE=PASS"),
            ("id_nearest_colonizable_planet", "ai",
             "ID_NEAREST_COLONIZABLE_CHECKS=5 FAILURES=0 RESULT=PASS"),
            ("c_build_prod_item", "c_build_prod_item", "C_BUILD_PROD_ITEM_SOURCE=PASS"),
            ("estimate_item_prod_sched", "estimate_item_prod_sched",
             "ESTIMATE_ITEM_PROD_SCHED_SOURCE=PASS"),
            ("find_ideal_warp", "find_ideal_warp",
             "FIND_IDEAL_WARP_CHECKS=15 FAILURES=0 RESULT=PASS"),
            ("init_production", "init_production", "INIT_PRODUCTION_SOURCE=PASS"),
            ("fill_prod_mines_and_factories", "fill_prod_mines_and_factories",
             "FILL_PROD_MF_CHECKS=6 FAILURES=0 RESULT=PASS"),
            ("queue_ai_defenses", "ai", "QUEUE_AI_DEFENSES_CHECKS=5 FAILURES=0 RESULT=PASS"),
            ("fuel_use_to_waypoint", "fuel_use_to_waypoint",
             "FUEL_USE_TO_WAYPOINT_SOURCE=PASS"),
            ("iwarp_best_for_waypoint", "iwarp_best_for_waypoint",
             "IWARP_BEST_CHECKS=8 FAILURES=0 RESULT=PASS"),
        ):
            with self.subTest(service=name), tempfile.TemporaryDirectory() as tmp:
                executable = Path(tmp) / "probe"
                subprocess.run([
                    "cc", "-std=c89", "-pedantic", "-Wall", "-Wextra", "-Werror", "-ffunction-sections", "-fdata-sections", "-Wl,-dead_strip",
                    "-I", str(ROOT / "reconstructed/include"),
                    str(ROOT / f"tests/{name}_source_probe.c"),
                    str(ROOT / f"reconstructed/src/{source}.c"),
                    "-o", str(executable),
                ], check=True)
                result = subprocess.run([str(executable)], capture_output=True,
                                        text=True, check=False)
                self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
                self.assertEqual(result.stdout.strip(), expected)


if __name__ == "__main__":
    unittest.main()
