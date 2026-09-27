"""Strict-C89 Macinti far second-waypoint mutation."""

from pathlib import Path
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]


class MacintiFarWaypointE3Tests(unittest.TestCase):
    def test_far_waypoint(self):
        with tempfile.TemporaryDirectory() as temp:
            exe = Path(temp) / "macinti_waypoint"
            subprocess.run([
                "cc", "-std=c89", "-pedantic", "-Wall", "-Wextra", "-Werror",
                "-I", str(ROOT / "reconstructed/include"),
                str(ROOT / "tests/do_macinti_far_waypoint_e3_probe.c"),
                str(ROOT / "reconstructed/src/do_macinti_ai_turn.c"),
                str(ROOT / "reconstructed/src/ai_plord.c"),
                "-o", str(exe),
            ], check=True)
            result = subprocess.run([str(exe)], capture_output=True,
                                    text=True, check=False)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertIn("MACINTI_FAR_WAYPOINT_E3_CHECKS=8 FAILURES=0 RESULT=PASS",
                      result.stdout)


if __name__ == "__main__":
    unittest.main()
