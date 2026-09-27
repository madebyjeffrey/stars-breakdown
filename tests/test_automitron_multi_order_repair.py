import subprocess
import tempfile
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]


class AutomitronMultiOrderRepairTests(unittest.TestCase):
    def test_original_repair_preserves_queued_orders(self):
        with tempfile.TemporaryDirectory() as temp:
            exe = Path(temp) / "automitron_multi_order_repair"
            subprocess.run([
                "cc", "-std=c89", "-pedantic", "-Wall", "-Wextra", "-Werror",
                "-I", str(ROOT / "reconstructed/include"),
                str(ROOT / "tests/do_automitron_ai_turn_multi_order_repair_probe.c"),
                str(ROOT / "reconstructed/src/do_automitron_ai_turn.c"),
                str(ROOT / "reconstructed/src/ai_plord.c"),
                "-o", str(exe),
            ], check=True)
            result = subprocess.run([str(exe)], check=False, capture_output=True,
                                    text=True)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertIn(
            "AUTOMITRON_MULTI_ORDER_REPAIR_CHECKS=22 FAILURES=0 RESULT=PASS",
            result.stdout,
        )


if __name__ == "__main__":
    unittest.main()
