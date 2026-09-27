"""Check the fresh Win16 package layout before guest transfer."""

from pathlib import Path
import tempfile
import unittest

from tools.package_automitron_e1_native import package


class PackageAutomitronE1NativeTests(unittest.TestCase):
    def test_fresh_package_is_dos_text_and_refuses_overwrite(self):
        with tempfile.TemporaryDirectory() as temp:
            target = Path(temp) / "AUE1N1"
            package(target, "C:\\AUE1N1")
            self.assertEqual({item.name for item in target.iterdir()}, {
                "CORE.H", "PLD.C", "AUT.C", "CO.C", "AI.C", "AT.C",
                "SUP.C", "CEN.C", "ERQ.C", "HUL.C", "QGT.C", "LFC.C", "PVL.C",
                "FLG.C", "RTE.C", "SCT.C",
                "WINSTUB.EXE", "AUE1.C", "AUE1.DEF", "AUE1.LNK",
                "BUILD.BAT", "RUNBLD.BAT", "RUN.BAT"})
            for name in ("BUILD.BAT", "RUNBLD.BAT", "RUN.BAT", "AUE1.C"):
                body = (target / name).read_bytes()
                self.assertIn(b"\r\n", body)
                self.assertNotIn(b"\n", body.replace(b"\r\n", b""))
            for name in ("BUILD.BAT", "AUE1.LNK"):
                self.assertTrue(all(len(line) <= 127 for line in
                                    (target / name).read_bytes().splitlines()))
            self.assertIn(b"/Dmain=colonizer_probe", (target / "BUILD.BAT").read_bytes())
            self.assertIn(b"/Dmain=ai_planet_probe", (target / "BUILD.BAT").read_bytes())
            self.assertIn(b"/Dmain=ai_planet_table_probe", (target / "BUILD.BAT").read_bytes())
            self.assertIn(b"/Dmain=support_queue_probe", (target / "BUILD.BAT").read_bytes())
            self.assertIn(b"/Dmain=planet_census_probe", (target / "BUILD.BAT").read_bytes())
            self.assertIn(b"/Dmain=early_queue_probe", (target / "BUILD.BAT").read_bytes())
            self.assertIn(b"/Dmain=hull_batches_probe", (target / "BUILD.BAT").read_bytes())
            self.assertIn(b"/Dmain=prod_queue_gate_probe", (target / "BUILD.BAT").read_bytes())
            self.assertIn(b"/Dmain=production_lifecycle_probe", (target / "BUILD.BAT").read_bytes())
            self.assertIn(b"/Dmain=planet_values_probe", (target / "BUILD.BAT").read_bytes())
            self.assertIn(b"/Dmain=fleet_flags_probe", (target / "BUILD.BAT").read_bytes())
            self.assertIn(b"/Dmain=packed_route_probe", (target / "BUILD.BAT").read_bytes())
            self.assertIn(b"/Dmain=scout_fuel_probe", (target / "BUILD.BAT").read_bytes())
            self.assertIn(b"C:\\AUE1N1", (target / "RUNBLD.BAT").read_bytes())
            with self.assertRaisesRegex(ValueError, "exists"):
                package(target, "C:\\AUE1N1")


if __name__ == "__main__":
    unittest.main()
