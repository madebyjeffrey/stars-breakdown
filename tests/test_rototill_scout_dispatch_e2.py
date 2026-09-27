"""Check Rototill's source-gated second-sweep scout dispatch."""

from pathlib import Path
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]


class RototillScoutDispatchE2Tests(unittest.TestCase):
    def test_source_gates_selection_and_low_fuel_fallback(self):
        with tempfile.TemporaryDirectory() as temp:
            exe = Path(temp) / "rototill_scout"
            subprocess.run([
                "cc", "-std=c89", "-pedantic", "-Wall", "-Wextra",
                "-Werror", "-I", str(ROOT / "reconstructed/include"),
                str(ROOT / "tests/do_rototill_scout_dispatch_e2_probe.c"),
                str(ROOT / "reconstructed/src/do_rototill_ai_turn.c"),
                "-o", str(exe),
            ], check=True)
            result = subprocess.run([str(exe)], capture_output=True,
                                    text=True, check=False)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertIn("ROTOTILL_SCOUT_E2_CHECKS=10 FAILURES=0 RESULT=PASS",
                      result.stdout)


if __name__ == "__main__":
    unittest.main()
