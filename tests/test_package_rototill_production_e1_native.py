"""Check the fresh Rototill E1 Win16 package before guest transfer."""

from pathlib import Path
import tempfile
import unittest

from tools.package_rototill_production_e1_native import package


class PackageRototillProductionE1Tests(unittest.TestCase):
    def test_fresh_package_layout_and_overwrite_guard(self):
        with tempfile.TemporaryDirectory() as temp:
            target = Path(temp) / "RTE1N15"
            package(target, "C:\\RTE1N15")
            self.assertEqual({item.name for item in target.iterdir()}, {
                "CORE.H", "ROT.C", "PROBE.C", "FLPROBE.C", "LQPROBE.C",
                "MOPROBE.C", "RRPROBE.C", "FKPROBE.C", "SCPROBE.C",
                "CMPROBE.C",
                "WINSTUB.EXE",
                "RTE1.C", "RTE1.DEF", "RTE1.LNK", "BUILD.BAT",
                "RUNBLD.BAT", "RUN.BAT"})
            for name in ("BUILD.BAT", "RUNBLD.BAT", "RUN.BAT", "RTE1.C"):
                body = (target / name).read_bytes()
                self.assertIn(b"\r\n", body)
                self.assertNotIn(b"\n", body.replace(b"\r\n", b""))
            for name in ("BUILD.BAT", "RTE1.LNK"):
                self.assertTrue(all(len(line) <= 127 for line in
                                    (target / name).read_bytes().splitlines()))
            self.assertIn(b"/Dmain=rototill_production_probe",
                          (target / "BUILD.BAT").read_bytes())
            self.assertIn(b"/Dmain=rototill_packed_fleet_probe",
                          (target / "BUILD.BAT").read_bytes())
            self.assertIn(b"/Dmain=rototill_live_queue_probe",
                          (target / "BUILD.BAT").read_bytes())
            self.assertIn(b"/Dmain=rototill_multi_order_probe",
                          (target / "BUILD.BAT").read_bytes())
            self.assertIn(b"/Dmain=rototill_reroute_probe",
                          (target / "BUILD.BAT").read_bytes())
            self.assertIn(b"/Dmain=rototill_first_kind_probe",
                          (target / "BUILD.BAT").read_bytes())
            self.assertIn(b"/Dmain=rototill_scout_probe",
                          (target / "BUILD.BAT").read_bytes())
            self.assertIn(b"/Dmain=rototill_colonizer_marker_probe",
                          (target / "BUILD.BAT").read_bytes())
            self.assertIn(b"C:\\RTE1N15", (target / "RUNBLD.BAT").read_bytes())
            self.assertIn(b"int printf(const char *format, ...)",
                          (target / "RTE1.C").read_bytes())
            with self.assertRaisesRegex(ValueError, "exists"):
                package(target, "C:\\RTE1N15")


if __name__ == "__main__":
    unittest.main()
