"""Strict-C89 check of Macinti's marked attack-production increment."""

import pathlib
import subprocess
import tempfile
import unittest


ROOT = pathlib.Path(__file__).resolve().parents[1]


class MacintiAttackPressureQueueE1Tests(unittest.TestCase):
    def test_attack_pressure_queue(self):
        with tempfile.TemporaryDirectory() as temporary:
            executable = pathlib.Path(temporary) / "probe"
            subprocess.run(
                ["cc", "-std=c89", "-pedantic-errors", "-Wall", "-Wextra",
                 "-Werror", "-I", str(ROOT / "reconstructed/include"),
                 str(ROOT / "tests/do_macinti_attack_pressure_queue_e1_probe.c"),
                 str(ROOT / "reconstructed/src/do_macinti_ai_turn.c"),
                 str(ROOT / "reconstructed/src/ai_plord.c"),
                 "-o", str(executable)],
                check=True, cwd=ROOT)
            result = subprocess.run([str(executable)], check=True,
                                    capture_output=True, text=True)
            self.assertIn("CHECKS=19 FAILURES=0 RESULT=PASS", result.stdout)


if __name__ == "__main__":
    unittest.main()
