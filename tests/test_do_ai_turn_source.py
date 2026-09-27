import subprocess
import tempfile
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]


class DoAiTurnSourceTests(unittest.TestCase):
    def test_retained_selector_one_dispatches_for_owner_one(self):
        with tempfile.TemporaryDirectory() as temp:
            exe = Path(temp) / "do_ai_turn_source"
            subprocess.run([
                "cc", "-std=c89", "-pedantic", "-Wall", "-Wextra",
                "-Werror", "-I", str(ROOT / "reconstructed/include"),
                str(ROOT / "tests/do_ai_turn_source_probe.c"),
                str(ROOT / "reconstructed/src/do_ai_turn.c"),
                "-o", str(exe),
            ], check=True)
            result = subprocess.run([str(exe)], check=False,
                                    capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertEqual(result.stdout.strip(), "DO_AI_TURN_SOURCE=PASS")


if __name__ == "__main__":
    unittest.main()
