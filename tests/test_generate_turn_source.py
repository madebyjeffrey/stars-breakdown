"""Strict-C89 regression for the FGenerateTurn player-log shuffle."""

from pathlib import Path
import subprocess
import tempfile
import unittest


ROOT = Path(__file__).resolve().parents[1]


class GenerateTurnSourceTests(unittest.TestCase):
    def test_player_shuffle_draws_and_log_order(self):
        with tempfile.TemporaryDirectory() as temporary:
            executable = Path(temporary) / "generate_turn_source_probe"
            subprocess.run([
                "cc", "-std=c89", "-pedantic", "-Wall", "-Wextra",
                "-Werror", "-I", str(ROOT / "reconstructed/include"),
                str(ROOT / "tests/generate_turn_source_probe.c"),
                str(ROOT / "reconstructed/src/generate_turn.c"),
                "-o", str(executable),
            ], check=True)
            result = subprocess.run([str(executable)], check=False,
                                    capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertEqual(result.stdout.strip(), "GENERATE_TURN_SOURCE=PASS")


if __name__ == "__main__":
    unittest.main()
