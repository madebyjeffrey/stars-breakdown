"""Strict-C89 host check of Macinti's first transport target repair."""

import pathlib
import subprocess
import tempfile
import unittest


ROOT = pathlib.Path(__file__).resolve().parents[1]


class MacintiFirstTransportE2Tests(unittest.TestCase):
    def test_first_transport(self):
        with tempfile.TemporaryDirectory() as temporary:
            executable = pathlib.Path(temporary) / "probe"
            subprocess.run(
                ["cc", "-std=c89", "-pedantic-errors", "-Wall", "-Wextra",
                 "-Werror", "-I", str(ROOT / "reconstructed/include"),
                 str(ROOT / "tests/do_macinti_first_transport_e2_probe.c"),
                 str(ROOT / "reconstructed/src/do_macinti_ai_turn.c"),
                 str(ROOT / "reconstructed/src/ai_plord.c"),
                 "-o", str(executable)],
                check=True, cwd=ROOT)
            result = subprocess.run([str(executable)], check=True,
                                    capture_output=True, text=True)
            self.assertIn("CHECKS=10 FAILURES=0 RESULT=PASS", result.stdout)


if __name__ == "__main__":
    unittest.main()
