"""Strict-C89 probe for Rototill's live packed fleet view and flag sweep."""

from pathlib import Path
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]


class RototillPackedFleetE2Tests(unittest.TestCase):
    def test_live_fleet_fields_and_flag_sweep(self):
        with tempfile.TemporaryDirectory() as temp:
            exe = Path(temp) / "rototill_fleet"
            subprocess.run([
                "cc", "-std=c89", "-pedantic", "-Wall", "-Wextra", "-Werror",
                "-I", str(ROOT / "reconstructed/include"),
                str(ROOT / "tests/do_rototill_packed_fleet_e2_probe.c"),
                str(ROOT / "reconstructed/src/do_rototill_ai_turn.c"),
                "-o", str(exe),
            ], check=True)
            result = subprocess.run([str(exe)], capture_output=True,
                                    text=True, check=False)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertIn("ROTOTILL_PACKED_FLEET_E2_CHECKS=17 FAILURES=0 RESULT=PASS",
                      result.stdout)


if __name__ == "__main__":
    unittest.main()
