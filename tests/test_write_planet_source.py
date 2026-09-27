"""Compile and exercise the packed planet writer reconstruction."""

import subprocess
import tempfile
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]


class WritePlanetSourceTests(unittest.TestCase):
    def test_routing_mineral_and_optional_tail_records(self):
        with tempfile.TemporaryDirectory() as temp:
            executable = Path(temp) / "write_planet_source"
            subprocess.run([
                "cc", "-std=c89", "-pedantic", "-Wall", "-Wextra",
                "-Werror", "-ffunction-sections", "-fdata-sections",
                "-Wl,-dead_strip", "-I",
                str(ROOT / "reconstructed/include"),
                str(ROOT / "tests/write_planet_source_probe.c"),
                str(ROOT / "reconstructed/src/file.c"),
                "-o", str(executable),
            ], check=True)
            result = subprocess.run([str(executable)], check=False,
                                    capture_output=True, text=True)
        self.assertEqual(result.returncode, 0,
                         result.stdout + result.stderr)
        self.assertEqual(result.stdout.strip(), "WRITE_PLANET_SOURCE=PASS")


if __name__ == "__main__":
    unittest.main()
