"""Cyber caller PLORD and AI-table composition from the recovered branch."""

from pathlib import Path
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]


class CyberColonizerSecondOrderE3Tests(unittest.TestCase):
    def test_caller_writes_second_order_marker(self):
        with tempfile.TemporaryDirectory() as temp:
            exe = Path(temp) / "cyber_second_order"
            subprocess.run([
                "cc", "-std=c89", "-pedantic", "-Wall", "-Wextra",
                "-Werror", "-I", str(ROOT / "reconstructed/include"),
                str(ROOT / "tests/do_cyber_colonizer_second_order_e3_probe.c"),
                str(ROOT / "reconstructed/src/do_cyber_ai_turn.c"),
                str(ROOT / "reconstructed/src/ai_plord.c"),
                "-o", str(exe),
            ], check=True)
            result = subprocess.run([str(exe)], capture_output=True,
                                    text=True, check=False)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertIn("CYBER_SECOND_ORDER_E3_CHECKS=200 FAILURES=0 RESULT=PASS",
                      result.stdout)


if __name__ == "__main__":
    unittest.main()
