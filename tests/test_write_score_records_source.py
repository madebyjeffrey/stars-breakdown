"""Compile and exercise the production type-45 score-record writer."""

import subprocess
import tempfile
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]


class WriteScoreRecordsSourceTests(unittest.TestCase):
    def test_player_file_selection_and_raw_record_bytes(self):
        with tempfile.TemporaryDirectory() as temp:
            executable = Path(temp) / "write_score_records_source"
            subprocess.run([
                "cc", "-std=c89", "-pedantic", "-Wall", "-Wextra",
                "-Werror", "-ffunction-sections", "-fdata-sections",
                "-Wl,-dead_strip", "-I",
                str(ROOT / "reconstructed/include"),
                str(ROOT / "tests/write_score_records_source_probe.c"),
                str(ROOT / "reconstructed/src/file.c"),
                "-o", str(executable),
            ], check=True)
            result = subprocess.run([str(executable)], check=False,
                                    capture_output=True, text=True)
        self.assertEqual(result.returncode, 0,
                         result.stdout + result.stderr)
        self.assertEqual(result.stdout.strip(),
                         "WRITE_SCORE_CHECKS=6 FAILURES=0 RESULT=PASS")


if __name__ == "__main__":
    unittest.main()
