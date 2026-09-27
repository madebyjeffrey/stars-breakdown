"""Exercise source position rings, including the original cache boundary."""
from pathlib import Path
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]


class LinkFleetsSourceTests(unittest.TestCase):
    def test_position_rings(self):
        with tempfile.TemporaryDirectory() as directory:
            executable = Path(directory) / "probe"
            subprocess.run([
                "cc", "-std=c89", "-pedantic", "-Wall", "-Wextra", "-Werror",
                "-fsanitize=address", "-I", str(ROOT / "reconstructed/include"),
                str(ROOT / "tests/link_fleets_source_probe.c"),
                str(ROOT / "reconstructed/src/link_fleets.c"), "-o", str(executable),
            ], check=True)
            result = subprocess.run([str(executable)], check=True,
                                    capture_output=True, text=True)
            self.assertEqual(result.stdout.strip(), "LINK_FLEETS_SOURCE=PASS")
