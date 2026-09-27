import subprocess
import tempfile
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]


class CBuildProdItemSourceTests(unittest.TestCase):
    def test_partial_progress_and_completion_spend_incremental_costs(self):
        with tempfile.TemporaryDirectory() as temp:
            exe = Path(temp) / "c_build_prod_item_source"
            subprocess.run([
                "cc", "-std=c89", "-pedantic", "-Wall", "-Wextra",
                "-Werror", "-I", str(ROOT / "reconstructed/include"),
                str(ROOT / "tests/c_build_prod_item_source_probe.c"),
                str(ROOT / "reconstructed/src/c_build_prod_item.c"),
                "-o", str(exe),
            ], check=True)
            result = subprocess.run([str(exe)], check=False,
                                    capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertEqual(result.stdout.strip(), "C_BUILD_PROD_ITEM_SOURCE=PASS")


if __name__ == "__main__":
    unittest.main()
