import subprocess
import tempfile
import unittest
from pathlib import Path

from tools.ne_layout import NE


ROOT = Path(__file__).resolve().parents[1]


class FAttackRawTorpedoDispatchTests(unittest.TestCase):
    def test_original_torpedo_fields_drive_initiative_and_range(self):
        with tempfile.TemporaryDirectory() as temp:
            exe = Path(temp) / "f_attack_raw_torpedo_dispatch"
            segment_path = Path(temp) / "SEG2.BIN"
            segment_path.write_bytes(bytes(NE(ROOT / "target/stars.exe").raw[1]))
            subprocess.run([
                "cc", "-std=c89", "-pedantic", "-Wall", "-Wextra", "-Werror",
                "-I", str(ROOT / "reconstructed/include"),
                str(ROOT / "tests/f_attack_raw_torpedo_dispatch_probe.c"),
                str(ROOT / "reconstructed/src/f_attack.c"),
                str(ROOT / "tests/f_attack_lpshdef_stub.c"),
                str(ROOT / "reconstructed/src/lookup_part.c"),
                str(ROOT / "reconstructed/src/c_torp_hit.c"),
                "-o", str(exe),
            ], check=True)
            result = subprocess.run([str(exe), str(segment_path)],
                                    capture_output=True, text=True)
            self.assertEqual(result.returncode, 0,
                             result.stdout + result.stderr)
        self.assertIn(
            "F_ATTACK_RAW_TORPEDO_DISPATCH_CHECKS=17 FAILURES=0 RESULT=PASS",
            result.stdout,
        )


if __name__ == "__main__":
    unittest.main()
