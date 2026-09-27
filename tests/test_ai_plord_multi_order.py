import subprocess
import tempfile
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]


class AiPlordMultiOrderTests(unittest.TestCase):
    def test_multi_order_mutation_matches_fmoveai_fleet_contract(self):
        with tempfile.TemporaryDirectory() as temp:
            exe = Path(temp) / "ai_plord_multi_order"
            subprocess.run([
                "cc", "-std=c89", "-pedantic", "-Wall", "-Wextra", "-Werror",
                "-I", str(ROOT / "reconstructed/include"),
                str(ROOT / "tests/ai_plord_multi_order_probe.c"),
                str(ROOT / "reconstructed/src/ai_plord.c"),
                "-o", str(exe),
            ], check=True)
            result = subprocess.run([str(exe)], check=False, capture_output=True,
                                    text=True)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertIn(
            "AI_PLORD_MULTI_ORDER_CHECKS=15 FAILURES=0 RESULT=PASS",
            result.stdout,
        )


if __name__ == "__main__":
    unittest.main()
