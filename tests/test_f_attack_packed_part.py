import subprocess
import tempfile
import unittest
from pathlib import Path

from tools.ne_layout import NE

ROOT = Path(__file__).resolve().parents[1]


class FAttackPackedPartTests(unittest.TestCase):
    def test_raw_part_lookup_uses_original_beam_initiative(self):
        with tempfile.TemporaryDirectory() as temp:
            exe = Path(temp) / "f_attack_packed_part"
            segment_path = Path(temp) / "SEG2.BIN"
            segment_path.write_bytes(bytes(NE(ROOT / "target/stars.exe").raw[1]))
            subprocess.run([
                "cc", "-std=c89", "-pedantic", "-Wall", "-Wextra", "-Werror",
                "-I", str(ROOT / "reconstructed/include"),
                str(ROOT / "tests/f_attack_packed_part_probe.c"),
                str(ROOT / "reconstructed/src/f_attack.c"),
                str(ROOT / "tests/f_attack_lpshdef_stub.c"),
                str(ROOT / "reconstructed/src/c_torp_hit.c"),
                str(ROOT / "reconstructed/src/lookup_part.c"),
                "-o", str(exe),
            ], check=True)
            result = subprocess.run([str(exe), str(segment_path)], check=False, capture_output=True,
                                    text=True)
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertIn(
            "F_ATTACK_PACKED_PART_CHECKS=24 FAILURES=0 RESULT=PASS",
            result.stdout,
        )


if __name__ == "__main__":
    unittest.main()
