"""Regression checks for the exact FFindSomethingAndSelectIt byte comparison."""
import hashlib
import json
import unittest

from tools.compare_byte_match import object_module
from tools.compare_mdi_find_match import ROOT, compare


class MdiFindByteMatchTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.report = json.loads((ROOT / 'analysis/compat/mdi-find-byte-match.json').read_text())
        cls.fixture = json.loads((ROOT / 'tests/fixtures/mdi-find-byte-match.json').read_text())

    def test_exact_match_and_current_source(self):
        self.assertTrue(self.report['all_functions_match'])
        self.assertEqual(self.report['functions'][0]['name'], 'FFindSomethingAndSelectIt')
        self.assertEqual(self.report['functions'][0]['original_bytes'], 349)
        self.assertEqual(hashlib.sha256((ROOT / 'compat/mdi-find-match/MDI.C').read_bytes()).hexdigest(),
                         self.report['source_sha256'])

    def test_fixture_reproduces_match(self):
        module, _ = object_module(bytes.fromhex(self.fixture['object_hex']))
        self.assertIn('FFINDSOMETHINGANDSELECTIT', module.publics[0]['name'].upper())
        self.assertTrue(compare(bytes.fromhex(self.fixture['object_hex']))['all_functions_match'])


if __name__ == '__main__':
    unittest.main()
