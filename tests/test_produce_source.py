import subprocess
import tempfile
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]


class ProduceSourceTests(unittest.TestCase):
    def test_four_resource_ship_progress_is_written_in_place(self):
        with tempfile.TemporaryDirectory() as temp:
            exe = Path(temp) / "produce_source"
            subprocess.run([
                "cc", "-std=c89", "-pedantic", "-Wall", "-Wextra",
                "-Werror", "-I", str(ROOT / "reconstructed/include"),
                str(ROOT / "tests/produce_source_probe.c"),
                str(ROOT / "reconstructed/src/produce.c"),
                str(ROOT / "reconstructed/src/c_build_prod_item.c"),
                "-o", str(exe),
            ], check=True)
            result = subprocess.run([str(exe)], check=False,
                                    capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertEqual(result.stdout.strip(), "PRODUCE_SOURCE=PASS")


if __name__ == "__main__":
    unittest.main()
