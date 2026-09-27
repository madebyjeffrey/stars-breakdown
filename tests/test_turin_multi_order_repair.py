import subprocess
import tempfile
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]


class TurinMultiOrderRepairTests(unittest.TestCase):
    def test_original_repair_uses_packed_kind_and_preserves_queue(self):
        with tempfile.TemporaryDirectory() as temp:
            exe = Path(temp) / "turin_multi_order_repair"
            subprocess.run([
                "cc", "-std=c89", "-pedantic", "-Wall", "-Wextra", "-Werror",
                "-I", str(ROOT / "reconstructed/include"),
                str(ROOT / "tests/do_turin_drone_ai_turn_multi_order_repair_probe.c"),
                str(ROOT / "reconstructed/src/do_turin_drone_ai_turn.c"),
                str(ROOT / "reconstructed/src/ai_plord.c"),
                "-o", str(exe),
            ], check=True)
            result = subprocess.run([str(exe)], check=False, capture_output=True,
                                    text=True)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertIn(
            "TURIN_MULTI_ORDER_REPAIR_CHECKS=18 FAILURES=0 RESULT=PASS",
            result.stdout,
        )


if __name__ == "__main__":
    unittest.main()
