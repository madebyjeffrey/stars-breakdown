"""Rototill second-sweep colonizer marker source boundary."""

from pathlib import Path
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]


class RototillColonizerMarkerE3Tests(unittest.TestCase):
    def test_selected_fleet_colonizer_marker(self):
        with tempfile.TemporaryDirectory() as temp:
            exe = Path(temp) / "rototill_colonizer_marker"
            subprocess.run([
                "cc", "-std=c89", "-pedantic", "-Wall", "-Wextra",
                "-Werror", "-I", str(ROOT / "reconstructed/include"),
                str(ROOT / "tests/do_rototill_colonizer_marker_e3_probe.c"),
                str(ROOT / "reconstructed/src/do_rototill_ai_turn.c"),
                "-o", str(exe),
            ], check=True)
            result = subprocess.run([str(exe)], capture_output=True,
                                    text=True, check=False)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertIn(
            "ROTOTILL_COLONIZER_MARKER_E3_CHECKS=9 FAILURES=0 RESULT=PASS",
            result.stdout)


if __name__ == "__main__":
    unittest.main()
