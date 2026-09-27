"""Strict-C89 source probe for Automitron's two original hull batches."""

import subprocess
import tempfile
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]


class AutomitronHullBatchesTests(unittest.TestCase):
    def test_live_two_batch_resource_flow(self):
        with tempfile.TemporaryDirectory() as temp:
            exe = Path(temp) / "automitron_hull_batches"
            subprocess.run([
                "cc", "-std=c89", "-pedantic", "-Wall", "-Wextra", "-Werror",
                "-I", str(ROOT / "reconstructed/include"),
                str(ROOT / "tests/do_automitron_hull_batches_e1_probe.c"),
                str(ROOT / "reconstructed/src/do_automitron_ai_turn.c"),
                str(ROOT / "reconstructed/src/ai_plord.c"),
                "-o", str(exe),
            ], check=True)
            result = subprocess.run([str(exe)], capture_output=True,
                                    text=True, check=False)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertIn("AUTOMITRON_HULL_BATCH_CHECKS=18 FAILURES=0 RESULT=PASS",
                      result.stdout)


if __name__ == "__main__":
    unittest.main()
