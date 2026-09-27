"""Compose the Rototill caller with the reconstructed packed queue writer."""

from pathlib import Path
import subprocess
import sys
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]


class RototillQueueWriterE1Tests(unittest.TestCase):
    def test_packed_items_and_relocated_allocation(self):
        with tempfile.TemporaryDirectory() as temp:
            exe = Path(temp) / "rototill_queue_writer"
            subprocess.run([
                "cc", "-std=c89", "-pedantic", "-Wall", "-Wextra",
                "-Werror", "-ffunction-sections",
                "-Wl,-dead_strip" if sys.platform == "darwin"
                else "-Wl,--gc-sections",
                "-I", str(ROOT / "reconstructed/include"),
                str(ROOT / "tests/do_rototill_queue_writer_e1_probe.c"),
                str(ROOT / "reconstructed/src/do_rototill_ai_turn.c"),
                str(ROOT / "reconstructed/src/ai.c"),
                "-o", str(exe),
            ], check=True)
            result = subprocess.run([str(exe)], capture_output=True,
                                    text=True, check=False)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertIn("ROTOTILL_QUEUE_WRITER_E1_CHECKS=11 FAILURES=0 RESULT=PASS",
                      result.stdout)


if __name__ == "__main__":
    unittest.main()
