"""Strict-C89 check for the post-InitProduction live PLPROD binding."""

from pathlib import Path
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]


class RototillLiveProdQueueE1Tests(unittest.TestCase):
    def test_post_init_queue_supersedes_stale_snapshot(self):
        with tempfile.TemporaryDirectory() as temp:
            exe = Path(temp) / "rototill_live_queue"
            subprocess.run([
                "cc", "-std=c89", "-pedantic", "-Wall", "-Wextra", "-Werror",
                "-I", str(ROOT / "reconstructed/include"),
                str(ROOT / "tests/do_rototill_live_prod_queue_e1_probe.c"),
                str(ROOT / "reconstructed/src/do_rototill_ai_turn.c"),
                "-o", str(exe),
            ], check=True)
            result = subprocess.run([str(exe)], capture_output=True,
                                    text=True, check=False)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertIn("ROTOTILL_LIVE_QUEUE_E1_CHECKS=12 FAILURES=0 RESULT=PASS",
                      result.stdout)


if __name__ == "__main__":
    unittest.main()
