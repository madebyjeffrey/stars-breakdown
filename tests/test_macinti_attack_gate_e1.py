"""Strict-C89 check of the bounded Macinti inner attack-production gate."""

from pathlib import Path
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]


class MacintiAttackGateE1Tests(unittest.TestCase):
    def test_inner_attack_gate(self):
        with tempfile.TemporaryDirectory() as temp:
            exe = Path(temp) / "macinti_attack_gate"
            subprocess.run([
                "cc", "-std=c89", "-pedantic-errors", "-Wall", "-Wextra",
                "-Werror", "-I", str(ROOT / "reconstructed/include"),
                str(ROOT / "tests/do_macinti_attack_gate_e1_probe.c"),
                str(ROOT / "reconstructed/src/do_macinti_ai_turn.c"),
                str(ROOT / "reconstructed/src/ai_plord.c"),
                "-o", str(exe),
            ], check=True)
            result = subprocess.run([str(exe)], capture_output=True,
                                    text=True, check=True)
            self.assertIn("CHECKS=39 FAILURES=0 RESULT=PASS", result.stdout)


if __name__ == "__main__":
    unittest.main()
