"""Verify the matching native deep-space salvage probe and returned report."""

from pathlib import Path
import subprocess
import tempfile
import unittest


ROOT = Path(__file__).resolve().parents[1]
EXPECTED = (b"SMOKE_SALVAGE_CHECKS=21 FAILURES=0 RESULT=PASS "
            b"ORE=5,0,2 THING=0022fc046405ff8305000000020002000000 "
            b"MSG=91 FLEET=DEAD\n")


class SatisfyOrdersSmokeDeepSpaceNativeTests(unittest.TestCase):
    def test_host_and_native(self):
        with tempfile.TemporaryDirectory() as directory:
            executable = Path(directory) / "probe"
            subprocess.run([
                "cc", "-std=c89", "-Wall", "-Wextra", "-Werror",
                "-Ireconstructed/include",
                "reconstructed/src/satisfy_orders.c",
                "reconstructed/src/drop_salvage.c",
                "reconstructed/src/thing_decay.c",
                "tests/satisfy_orders_smoke_deepspace_probe.c",
                "-o", str(executable),
            ], cwd=ROOT, check=True, capture_output=True)
            result = subprocess.run([str(executable)], cwd=ROOT, check=True,
                                    capture_output=True)
        self.assertEqual(result.stdout, EXPECTED)
        returned = ROOT / "runs/vmctl-soe2q"
        status = (returned / "SYOSTA.TXT").read_bytes()
        self.assertIn(b"COMPILE=PASS", status)
        self.assertIn(b"LINK=PASS", status)
        self.assertEqual((returned / "SOE2QRES.TXT").read_bytes().replace(
            b"\r\n", b"\n"), EXPECTED)


if __name__ == "__main__":
    unittest.main()
