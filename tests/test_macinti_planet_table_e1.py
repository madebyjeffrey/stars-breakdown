"""Strict-C89 Macinti live planet/AI-table selection probe."""

from pathlib import Path
import subprocess
import sys
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]


class MacintiPlanetTableE1Tests(unittest.TestCase):
    def test_live_planet_table_selection(self):
        with tempfile.TemporaryDirectory() as temp:
            exe = Path(temp) / "macinti_planet_table"
            subprocess.run([
                "cc", "-std=c89", "-pedantic", "-Wall", "-Wextra", "-Werror",
                "-ffunction-sections",
                "-Wl,-dead_strip" if sys.platform == "darwin"
                else "-Wl,--gc-sections",
                "-I", str(ROOT / "reconstructed/include"),
                str(ROOT / "tests/do_macinti_planet_table_e1_probe.c"),
                str(ROOT / "reconstructed/src/do_macinti_ai_turn.c"),
                str(ROOT / "reconstructed/src/ai.c"),
                str(ROOT / "reconstructed/src/ai_plord.c"),
                "-o", str(exe),
            ], check=True)
            result = subprocess.run([str(exe)], capture_output=True,
                                    text=True, check=False)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertIn("MACINTI_PLANET_TABLE_E1_CHECKS=41 FAILURES=0 RESULT=PASS",
                      result.stdout)


if __name__ == "__main__":
    unittest.main()
