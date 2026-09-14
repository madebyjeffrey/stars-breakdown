"""Acceptance fixture generation, comparison boundaries and transfer integrity."""
import contextlib
import io
import json
from pathlib import Path
import struct
import tempfile
import unittest
import zipfile

from tools.package_race_acceptance import generate, publish, review
from tools.race_file_compare import Inspector, compare
from tools.race_scenarios import catalog


class RaceAcceptanceTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        # Independent original/C wizard paths, including dynamic mouse input
        # and a sequence whose correct result is a failed checksum on reload.
        selected = [c for c in catalog() if c['id'] in ['B000', 'G000', 'U020', 'I000']]
        with contextlib.redirect_stdout(io.StringIO()):
            cls.report, cls.files, cls.labels = generate(selected)
            cls.other, cls.other_files, _ = generate([catalog()[0]], tick=991)
        cls.inspector = Inspector()

    def test_clock_changes_ciphertext_but_not_decoded_race(self):
        a = self.report['cases'][0]['sessions'][0]['file']
        b = self.other['cases'][0]['sessions'][0]['file']
        result = compare(a, b)
        self.assertEqual(result['status'], 'PASS')
        self.assertFalse(result['exact_file'])
        self.assertEqual(set(result['bof_differences']), {'salt'})
        self.assertTrue(result['plaintext_equal'])

    def test_header_changes_are_reported_and_corruption_fails(self):
        raw = self.files['MODEL/B000.R1']
        expected = self.inspector.inspect(raw)
        changed = bytearray(raw)
        changed[-1] ^= 1
        bad = self.inspector.inspect(changed)
        self.assertFalse(bad['checksum']['valid'])
        self.assertEqual(compare(expected, bad)['status'], 'FAIL')
        changed = bytearray(raw)
        # The file RNG only uses low game-id bits. This change leaves its
        # plaintext intact, but is NOT a permitted clock/salt variation.
        struct.pack_into('<I', changed, 6, expected['bof']['game_id']+4)
        changed_header = self.inspector.inspect(changed)
        self.assertEqual(compare(expected, changed_header)['status'], 'REVIEW_HEADER')
        for truncated in [raw[:1], raw[:19], raw[:-1], raw+b'\0']:
            with self.assertRaises(ValueError):
                self.inspector.inspect(truncated)

    def test_expected_rejection_is_preserved(self):
        u020 = next(c for c in self.report['cases'] if c['case']['id']=='U020')
        self.assertEqual([s['load_result'] for s in u020['sessions']], [1, 0])
        damaged = u020['sessions'][1]['file']
        self.assertFalse(damaged['checksum']['valid'])
        self.assertEqual(compare(damaged, damaged)['status'], 'PASS')
        self.assertEqual(u020['sessions'][1]['fields']['singular'], 'X')
        growth = next(c for c in self.report['cases'] if c['case']['id']=='G000')
        self.assertEqual(growth['sessions'][0]['fields']['growth'], 1)
        rejected = next(c for c in self.report['cases'] if c['case']['id']=='I000')['sessions'][0]
        self.assertLess(rejected['advantage_points'], 0)
        self.assertIsNone(rejected['file'])

    def test_transfer_ascii_dos_names_integrity_and_preserves_returns(self):
        with tempfile.TemporaryDirectory() as directory:
            package = Path(directory)/'RACEACC'
            archive = publish(package, self.report, self.files, self.labels)
            with zipfile.ZipFile(archive) as source:
                for name in source.namelist():
                    for part in Path(name).parts:
                        self.assertRegex(part, r'^[A-Z0-9_]{1,8}(\.[A-Z0-9_]{1,3})?$')
                    payload = source.read(name)
                    if Path(name).suffix in ['.TXT', '.JSN']:
                        payload.decode('ascii')
                        self.assertNotIn(b'\0', payload)
                        self.assertNotIn(b'\n', payload.replace(b'\r\n', b''))
            marker = package/'RETURN/OBSERVE.TXT'
            marker.write_bytes(b'live observations\r\n')
            with self.assertRaises(ValueError):
                publish(package, self.report, self.files, self.labels)
            self.assertEqual(marker.read_bytes(), b'live observations\r\n')
            result = review(package, package/'MODEL')
            self.assertEqual(set(r['status'] for r in result['files']), {'PASS', 'OBSERVATION_NEEDED'})
            # No file does not count as evidence that a rejection was observed.
            result = review(package, package/'RETURN')
            self.assertIn('MISSING', [r['status'] for r in result['files']])
            expected = package/'EXPECT.JSN'
            expected.write_bytes(expected.read_bytes()+b' ')
            with self.assertRaisesRegex(ValueError, 'integrity manifest'):
                review(package, package/'MODEL')


if __name__ == '__main__':
    unittest.main()
