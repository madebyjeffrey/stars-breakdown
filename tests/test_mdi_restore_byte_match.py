"""Regression check for the exact RestoreSelection byte comparison."""
import hashlib
import json
import unittest

from tools.compare_byte_match import object_module
from tools.compare_mdi_restore_match import ROOT, compare


class MdiRestoreByteMatchTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.report = json.loads((ROOT / 'analysis/compat/mdi-restore-byte-match.json').read_text())
        cls.fixture = json.loads((ROOT / 'tests/fixtures/mdi-restore-byte-match.json').read_text())

    def test_exact_match_and_current_source(self):
        row = self.report['functions'][0]
        self.assertTrue(self.report['all_functions_match'])
        self.assertEqual((row['original_bytes'], row['generated_bytes'], row['differing_bytes']),
                         (603, 603, 0))
        self.assertEqual(hashlib.sha256((ROOT / 'compat/mdi-restore-match/MDI.C').read_bytes()).hexdigest(),
                         self.report['source_sha256'])

    def test_fixture_reproduces_match(self):
        module, _ = object_module(bytes.fromhex(self.fixture['object_hex']))
        self.assertIn('RESTORESELECTION', module.publics[0]['name'].upper())
        self.assertTrue(compare(bytes.fromhex(self.fixture['object_hex']))['all_functions_match'])


if __name__ == '__main__':
    unittest.main()
