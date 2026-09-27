"""Regression checks for exact small mdi.obj byte comparisons."""
import hashlib
import json
import unittest

from tools.compare_byte_match import object_module
from tools.compare_mdi_small_match import ROOT, compare


class MdiSmallByteMatchTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.report = json.loads((ROOT / 'analysis/compat/mdi-small-byte-match.json').read_text())
        cls.fixture = json.loads((ROOT / 'tests/fixtures/mdi-small-byte-match.json').read_text())

    def test_exact_matches_and_current_source(self):
        self.assertTrue(self.report['all_functions_match'])
        self.assertEqual([f['name'] for f in self.report['functions']],
                         ['DrawHostOptions', 'GetASubMenu', 'GetWindowRc', 'SetWindowIniString'])
        self.assertEqual(sum(f['original_bytes'] for f in self.report['functions']), 305)
        self.assertEqual(hashlib.sha256((ROOT / 'compat/mdi-small-match/MDI.C').read_bytes()).hexdigest(),
                         self.report['source_sha256'])

    def test_fixture_reproduces_matches(self):
        module, _ = object_module(bytes.fromhex(self.fixture['object_hex']))
        self.assertIn('DRAWHOSTOPTIONS', module.publics[0]['name'].upper())
        result = compare(bytes.fromhex(self.fixture['object_hex']))
        self.assertTrue(result['all_functions_match'])


if __name__ == '__main__':
    unittest.main()
