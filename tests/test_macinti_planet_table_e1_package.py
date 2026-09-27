"""Static safety checks for the Macinti Win16 packet."""

from pathlib import Path
import tempfile
import unittest

from tools.package_macinti_planet_table_e1_native import package


class MacintiPlanetTableE1PackageTests(unittest.TestCase):
    def test_fresh_short_dos_packet(self):
        with tempfile.TemporaryDirectory() as temp:
            output = Path(temp) / "MAE1N1"
            package(output, "C:\\MAE1N1")
            names = {item.name for item in output.iterdir()}
            self.assertEqual(names, {
                "CORE.H", "MAI.C", "PLORD.C", "PROBE.C", "FLPROBE.C",
                "FSPROBE.C", "BUDPROBE.C", "PRSPROBE.C",
                "WPPROBE.C", "EOPROBE.C", "GATPROBE.C",
                "MAE1.C",
                "MAE1.DEF", "MAE1.LNK", "BUILD.BAT", "RUNBLD.BAT",
                "RUN.BAT", "WINSTUB.EXE",
            })
            for item in output.iterdir():
                self.assertLessEqual(len(item.stem), 8)
                if item.suffix == ".BAT":
                    content = item.read_bytes()
                    self.assertNotIn(b"\n", content.replace(b"\r\n", b""))
                    self.assertLess(max(map(len, content.split(b"\r\n"))), 127)
            self.assertIn(b"if exist BLDSTAT.TXT goto stale",
                          (output / "BUILD.BAT").read_bytes())
            with self.assertRaises(ValueError):
                package(output, "C:\\MAE1N1")


if __name__ == "__main__":
    unittest.main()
