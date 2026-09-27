import subprocess
import tempfile
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]


class TurinSeedFinishTests(unittest.TestCase):
    def test_original_turn_zero_and_finish_order(self):
        with tempfile.TemporaryDirectory() as temp:
            exe = Path(temp) / "turin_seed_finish"
            subprocess.run([
                "cc", "-std=c89", "-pedantic", "-Wall", "-Wextra", "-Werror",
                "-I", str(ROOT / "reconstructed/include"),
                str(ROOT / "tests/do_turin_drone_ai_turn_seed_finish_probe.c"),
                str(ROOT / "reconstructed/src/do_turin_drone_ai_turn.c"),
                str(ROOT / "reconstructed/src/ai_plord.c"),
                "-o", str(exe),
            ], check=True)
            result = subprocess.run([str(exe)], check=False, capture_output=True,
                                    text=True)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertIn(
            "TURIN_SEED_FINISH_CHECKS=24 FAILURES=0 RESULT=PASS",
            result.stdout,
        )


if __name__ == "__main__":
    unittest.main()
