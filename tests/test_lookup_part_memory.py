import subprocess
import tempfile
import unittest
from pathlib import Path

from tools.ne_layout import NE


ROOT = Path(__file__).resolve().parents[1]


class LookupPartMemoryTests(unittest.TestCase):
    def test_resolves_original_segment_records_and_weapon_fields(self):
        with tempfile.TemporaryDirectory() as temp:
            temp_path = Path(temp)
            segment_path = temp_path / "SEG2.BIN"
            segment_path.write_bytes(bytes(NE(ROOT / "target/stars.exe").raw[1]))
            exe = temp_path / "lookup_part_memory"
            subprocess.run([
                "cc", "-std=c89", "-pedantic", "-Wall", "-Wextra", "-Werror",
                "-I", str(ROOT / "reconstructed/include"),
                str(ROOT / "tests/lookup_part_memory_probe.c"),
                str(ROOT / "reconstructed/src/lookup_part.c"),
                "-o", str(exe),
            ], check=True)
            result = subprocess.run([str(exe), str(segment_path)], check=False,
                                    capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertIn(
            "LOOKUP_PART_MEMORY_CHECKS=129 FAILURES=0 RESULT=PASS",
            result.stdout,
        )


if __name__ == "__main__":
    unittest.main()
