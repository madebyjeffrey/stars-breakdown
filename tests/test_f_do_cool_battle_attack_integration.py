import subprocess
import tempfile
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]


class CoolBattleAttackIntegrationTests(unittest.TestCase):
    def test_attack_context_is_seeded_from_battle_token(self):
        with tempfile.TemporaryDirectory() as temp:
            exe = Path(temp) / "cool_battle_attack_integration"
            subprocess.run([
                "cc", "-std=c89", "-pedantic", "-Wall", "-Wextra",
                "-Werror", "-I", str(ROOT / "reconstructed/include"),
                str(ROOT / "tests/f_do_cool_battle_attack_integration_probe.c"),
                str(ROOT / "reconstructed/src/f_do_cool_battle.c"),
                str(ROOT / "reconstructed/src/f_attack.c"),
                str(ROOT / "reconstructed/src/lookup_part.c"),
                str(ROOT / "reconstructed/src/c_torp_hit.c"),
                str(ROOT / "reconstructed/src/dxy_move_tok_to.c"),
                str(ROOT / "reconstructed/src/dz_move_range_to_consider.c"),
                str(ROOT / "reconstructed/src/initialize_board.c"),
                str(ROOT / "reconstructed/src/battle.c"),
                str(ROOT / "reconstructed/src/md_target.c"),
                "-o", str(exe),
            ], check=True)
            result = subprocess.run([str(exe)], check=True,
                                    capture_output=True, text=True)
        self.assertIn(
            "F_DO_COOL_BATTLE_ATTACK_INTEGRATION_CHECKS=44 "
            "FAILURES=0 RESULT=PASS",
            result.stdout,
        )


if __name__ == "__main__":
    unittest.main()
