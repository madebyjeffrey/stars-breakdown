import subprocess
import tempfile
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]


class FAttackBeamScaleTests(unittest.TestCase):
    def test_raw_beam_scaling_matches_instruction_order(self):
        with tempfile.TemporaryDirectory() as temp:
            exe = Path(temp) / "f_attack_beam_scale"
            subprocess.run([
                "cc", "-std=c89", "-pedantic", "-Wall", "-Wextra", "-Werror",
                "-I", str(ROOT / "reconstructed/include"),
                str(ROOT / "tests/f_attack_beam_scale_probe.c"),
                str(ROOT / "reconstructed/src/f_attack.c"),
                str(ROOT / "tests/f_attack_lpshdef_stub.c"),
                str(ROOT / "reconstructed/src/lookup_part.c"),
                "-o", str(exe),
            ], check=True)
            result = subprocess.run([str(exe)], check=True, capture_output=True,
                                    text=True)
        self.assertIn(
            "F_ATTACK_BEAM_SCALE_CHECKS=86 FAILURES=0 RESULT=PASS",
            result.stdout,
        )


if __name__ == "__main__":
    unittest.main()
