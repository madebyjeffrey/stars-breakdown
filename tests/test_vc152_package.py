"""Transfer integrity and executable host checks for the manual Win16 experiment."""
import copy
import json
from pathlib import Path
import re
import subprocess
import tempfile
import unittest
import zipfile

from tools.package_vc152 import SOURCE, export, payload, render_vectors


class VC152PackageTests(unittest.TestCase):
    def test_transfer_is_dos_compatible_complete_and_reproducible(self):
        files = payload()
        self.assertEqual(files, payload())
        for name, data in files.items():
            self.assertRegex(name, r'^[A-Z0-9_]{1,8}\.[A-Z0-9_]{1,3}$')
            self.assertNotIn(b'\n', data.replace(b'\r\n', b''), name)
            self.assertNotIn(b'\r', data.replace(b'\r\n', b''), name)
            text = data.decode('ascii')
            for include in re.findall(r'#include\s+"([^"]+)"', text):
                self.assertIn(include, files, (name, include))
        self.assertNotIn('PROBE.EXE', files)
        # LINK uses exactly five response fields; blank option-only lines and
        # extra commas would advance its prompts and break the native build.
        responses = files['PROBE.LNK'].decode('ascii').splitlines()
        self.assertEqual(len(responses), 5)
        self.assertIn('PROBE.OBJ', responses[0])
        self.assertEqual(responses[1:3], ['PROBE.EXE', 'PROBE.MAP'])
        self.assertIn('MLIBCEW.LIB', responses[3])
        self.assertEqual(responses[4], 'PROBE.DEF')
        with tempfile.TemporaryDirectory() as tmp:
            first = export(Path(tmp) / 'FIRST')
            second = export(Path(tmp) / 'SECOND')
            self.assertEqual(first.read_bytes(), second.read_bytes())
            with zipfile.ZipFile(first) as archive:
                self.assertEqual(set(archive.namelist()), set(files))
                self.assertEqual({name: archive.read(name) for name in files}, files)
            # User return artifacts must survive accidental regeneration.
            marker = Path(tmp) / 'FIRST/RESULTS.TXT'
            marker.write_bytes(b'guest results\r\n')
            with self.assertRaises(FileExistsError):
                export(Path(tmp) / 'FIRST')
            self.assertEqual(marker.read_bytes(), b'guest results\r\n')

    def test_exported_c89_runs_frozen_oracle_vectors_and_detects_mismatch(self):
        document = json.loads((SOURCE / 'vectors.json').read_text())
        self.assertEqual(len(document['cases']), 113)
        _, vector_checks = render_vectors(document)
        self.assertEqual(vector_checks, 288)
        with tempfile.TemporaryDirectory() as tmp:
            directory = Path(tmp) / 'PACKAGE'
            export(directory)
            command = ['cc', '-std=c89', '-pedantic', '-Wall', '-Wextra', '-Werror',
                       '-DHOST_TEST', '-x', 'c', '-I', str(directory),
                       *[str(directory / name) for name in
                         ('PROBE.C', 'ABI.C', 'VECTEST.C', 'UTILGEN.C')],
                       '-o', str(directory / 'host-probe')]
            subprocess.run(command, check=True, capture_output=True, text=True)
            result = subprocess.run([str(directory / 'host-probe')], cwd=directory)
            self.assertEqual(result.returncode, 0)
            report = (directory / 'RESULTS.TXT').read_text()
            self.assertIn('MODE=HOST_ONLY', report)
            self.assertIn('CHECKS=552\nFAILURES=0\nRESULT=PASS', report)
            # Compile the actual transferred generated headers in host-storage
            # mode too; it does not pretend to exercise STARS_NATIVE16.
            # Recovered unspecified-parameter function types deliberately use
            # C89's (), not a guessed (void); retain that metadata unchanged.
            subprocess.run(['cc', '-std=c89', '-pedantic', '-Wall', '-Wextra', '-Werror',
                            '-Wno-strict-prototypes',
                            '-x', 'c', '-c', str(directory / 'LAYOUT.C'),
                            '-o', str(directory / 'layout-host.o')],
                           check=True, capture_output=True, text=True)
            bad = copy.deepcopy(document)
            bad['cases'][0]['state'][0] ^= 1
            source, _ = render_vectors(bad)
            (directory / 'VECTEST.C').write_text(source)
            subprocess.run(command, check=True, capture_output=True, text=True)
            result = subprocess.run([str(directory / 'host-probe')], cwd=directory)
            self.assertEqual(result.returncode, 1)
            report = (directory / 'RESULTS.TXT').read_text()
            self.assertIn('FAIL randomize.first[0]', report)
            self.assertIn('CHECKS=552\nFAILURES=1\nRESULT=FAIL', report)


if __name__ == '__main__':
    unittest.main()
