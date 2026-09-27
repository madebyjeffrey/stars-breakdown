"""Strict-C89 Rototill packed production and AI-planet source probe."""

import subprocess
import tempfile
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]


class RototillProductionTests(unittest.TestCase):
    def test_queue_branches_and_ai_planet_writes(self):
        with tempfile.TemporaryDirectory() as temp:
            exe = Path(temp) / "rototill_production"
            subprocess.run([
                "cc", "-std=c89", "-pedantic", "-Wall", "-Wextra", "-Werror",
                "-I", str(ROOT / "reconstructed/include"),
                str(ROOT / "tests/do_rototill_production_e1_probe.c"),
                str(ROOT / "reconstructed/src/do_rototill_ai_turn.c"),
                "-o", str(exe),
            ], check=True)
            result = subprocess.run([str(exe)], capture_output=True,
                                    text=True, check=False)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertIn("ROTOTILL_PRODUCTION_CHECKS=20 FAILURES=0 RESULT=PASS",
                      result.stdout)


if __name__ == "__main__":
    unittest.main()
