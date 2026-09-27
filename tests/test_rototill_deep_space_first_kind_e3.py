"""Rototill fleet fallback changes only first PLORD kind."""

from pathlib import Path
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]


class RototillDeepSpaceFirstKindE3Tests(unittest.TestCase):
    def test_three_order_preservation_and_guards(self):
        with tempfile.TemporaryDirectory() as temp:
            exe = Path(temp) / "rototill_first_kind"
            subprocess.run([
                "cc", "-std=c89", "-pedantic", "-Wall", "-Wextra",
                "-Werror", "-I", str(ROOT / "reconstructed/include"),
                str(ROOT / "tests/do_rototill_deep_space_first_kind_e3_probe.c"),
                str(ROOT / "reconstructed/src/do_rototill_ai_turn.c"),
                "-o", str(exe),
            ], check=True)
            result = subprocess.run([str(exe)], capture_output=True,
                                    text=True, check=False)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertIn("ROTOTILL_FIRST_KIND_E3_CHECKS=19 FAILURES=0 RESULT=PASS",
                      result.stdout)


if __name__ == "__main__":
    unittest.main()
