"""Strict-C89 Macinti packed fleet binding and observerless dispatch."""

from pathlib import Path
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]


class MacintiPackedFleetE2Tests(unittest.TestCase):
    def test_packed_fleet_action_chain(self):
        with tempfile.TemporaryDirectory() as temp:
            exe = Path(temp) / "macinti_packed_fleet"
            subprocess.run([
                "cc", "-std=c89", "-pedantic", "-Wall", "-Wextra", "-Werror",
                "-I", str(ROOT / "reconstructed/include"),
                str(ROOT / "tests/do_macinti_packed_fleet_e2_probe.c"),
                str(ROOT / "reconstructed/src/do_macinti_ai_turn.c"),
                str(ROOT / "reconstructed/src/ai_plord.c"),
                "-o", str(exe),
            ], check=True)
            result = subprocess.run([str(exe)], capture_output=True,
                                    text=True, check=False)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertIn("MACINTI_PACKED_FLEET_E2_CHECKS=12 FAILURES=0 RESULT=PASS",
                      result.stdout)


if __name__ == "__main__":
    unittest.main()
