import subprocess
import tempfile
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]


class ExecuteReportChainTests(unittest.TestCase):
    def test_report_hit_test_to_click_and_popup_cases(self):
        with tempfile.TemporaryDirectory() as temp:
            exe = Path(temp) / "report_chain_probe"
            subprocess.run([
                "cc", "-std=c89", "-pedantic", "-Wall", "-Wextra", "-Werror",
                "-I", str(ROOT / "reconstructed/include"),
                str(ROOT / "tests/execute_report_click_report_chain_probe.c"),
                str(ROOT / "reconstructed/src/report_dlg.c"),
                str(ROOT / "reconstructed/src/execute_report_click.c"),
                "-o", str(exe),
            ], check=True)
            result = subprocess.run([str(exe)], check=False,
                                    capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertIn(
            "EXECUTE_REPORT_CLICK_REPORT_CHAIN_CHECKS=33 FAILURES=0 RESULT=PASS",
            result.stdout,
        )


if __name__ == "__main__":
    unittest.main()
