"""Retained live-original GlobalPD bytes for the Phase-2 ER-E1 gate."""

import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
RETURNS = ROOT / "runs" / "vmctl-gp-live-preflight"
EVIDENCE = ROOT / "analysis" / "compat" / (
    "execute-report-click-original-globalpd-e1.md"
)
CAPTURES = {
    "STARBASE-DIFF": "0B001200E71901000000000000000000000000000000",
    "PLANET-LIVE": "07000A00E71901000000000000000000000000000000",
    "RESOURCE-LIVE": "0C000A0023001E000000000000000000000000000000",
    "DEFENSE-LIVE": "0900008009139E183711000000000000000000000000",
    "FLEET-LIVE": "03008C019F1901000000FF0000000000000000000000",
    "MINERAL-LIVE": "01000000000001000000D50100004400000006000000",
    "MINE-LIVE": "08000A00B0040A001900000000004400000006000000",
    "FACTORY-LIVE": "08000A00B0040A001900010000004400000006000000",
}


class OriginalGlobalPdCaptureTests(unittest.TestCase):
    def test_isolated_original_capture_bytes_and_reports(self):
        evidence = EVIDENCE.read_text(encoding="utf-8")
        for name, expected in CAPTURES.items():
            with self.subTest(name=name):
                self.assertIn(f"`CAPTURE-{name}`", evidence)
                self.assertIn(f"`{expected}`", evidence)
                captured = RETURNS / f"CAPTURE-{name}"
                # runs/ is a local return archive, not a checked-in fixture.
                # Verify it when present without making a clean checkout fail.
                if not RETURNS.exists():
                    continue
                self.assertTrue(captured.with_suffix(".BIN").exists())
                self.assertTrue(captured.with_suffix(".TXT").exists())
                payload = captured.with_suffix(".BIN").read_bytes()
                report = captured.with_suffix(".TXT").read_text(
                    encoding="ascii"
                )
                self.assertEqual(len(payload), 22)
                self.assertEqual(payload.hex().upper(), expected)
                self.assertIn("PATH=C:\\GPLIVE1\\STARS.EXE", report)
                self.assertIn("READ=22", report)
                self.assertIn(f"GLOBALPD_HEX={expected}", report)
                self.assertIn("STATUS=PASS", report)
                self.assertIn("POPUP_RELEASED=1", report)


if __name__ == "__main__":
    unittest.main()
