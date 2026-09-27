"""Check Automitron's direct packed FMoveAiFleet fallback."""

import subprocess
import tempfile
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]


class AutomitronPackedRouteTests(unittest.TestCase):
    def test_move_colonize_capacity_and_callback_precedence(self):
        with tempfile.TemporaryDirectory() as temp:
            exe = Path(temp) / "automitron_packed_route"
            subprocess.run([
                "cc", "-std=c89", "-pedantic", "-Wall", "-Wextra", "-Werror",
                "-I", str(ROOT / "reconstructed/include"),
                str(ROOT / "tests/do_automitron_packed_route_e3_probe.c"),
                str(ROOT / "reconstructed/src/do_automitron_ai_turn.c"),
                str(ROOT / "reconstructed/src/ai_plord.c"),
                "-o", str(exe),
            ], check=True)
            result = subprocess.run([str(exe)], capture_output=True,
                                    text=True, check=False)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertIn("AUTOMITRON_PACKED_ROUTE_CHECKS=10 FAILURES=0 RESULT=PASS",
                      result.stdout)


if __name__ == "__main__":
    unittest.main()
