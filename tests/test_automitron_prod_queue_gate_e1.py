"""Strict-C89 packed PLPROD bypass scan for Automitron AU-E1."""

import subprocess
import tempfile
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]


class AutomitronProdQueueGateTests(unittest.TestCase):
    def test_packed_bypass_and_single_scan(self):
        with tempfile.TemporaryDirectory() as temp:
            exe = Path(temp) / "automitron_prod_gate"
            subprocess.run([
                "cc", "-std=c89", "-pedantic", "-Wall", "-Wextra", "-Werror",
                "-I", str(ROOT / "reconstructed/include"),
                str(ROOT / "tests/do_automitron_prod_queue_gate_e1_probe.c"),
                str(ROOT / "reconstructed/src/do_automitron_ai_turn.c"),
                str(ROOT / "reconstructed/src/ai_plord.c"),
                "-o", str(exe),
            ], check=True)
            result = subprocess.run([str(exe)], capture_output=True,
                                    text=True, check=False)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertIn("AUTOMITRON_PROD_GATE_CHECKS=13 FAILURES=0 RESULT=PASS",
                      result.stdout)


if __name__ == "__main__":
    unittest.main()
