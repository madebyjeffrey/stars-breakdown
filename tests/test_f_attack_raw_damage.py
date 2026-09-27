import subprocess
import tempfile
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]


class FAttackRawDamageTests(unittest.TestCase):
    def test_raw_damage_rescans_live_target_state(self):
        with tempfile.TemporaryDirectory() as temp:
            exe = Path(temp) / "f_attack_raw_damage"
            subprocess.run([
                "cc", "-std=c89", "-pedantic", "-Wall", "-Wextra", "-Werror",
                "-I", str(ROOT / "reconstructed/include"),
                str(ROOT / "tests/f_attack_raw_damage_probe.c"),
                str(ROOT / "reconstructed/src/f_attack.c"),
                str(ROOT / "reconstructed/src/f_damage_tok.c"),
                str(ROOT / "tests/f_attack_lpshdef_stub.c"),
                str(ROOT / "reconstructed/src/lookup_part.c"),
                "-o", str(exe),
            ], check=True)
            result = subprocess.run([str(exe)], check=False, capture_output=True,
                                    text=True)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertIn(
            "F_ATTACK_RAW_DAMAGE_CHECKS=15 FAILURES=0 RESULT=PASS",
            result.stdout,
        )


if __name__ == "__main__":
    unittest.main()
