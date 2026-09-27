"""Regression checks for the exact stars.obj FHandleChar comparison."""
import hashlib
import json
import unittest

from tools.compare_byte_match import object_module
from tools.compare_stars_small_match import ROOT, compare


class StarsSmallByteMatchTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.report = json.loads((ROOT / 'analysis/compat/stars-small-byte-match.json').read_text())
        cls.fixture = json.loads((ROOT / 'tests/fixtures/stars-small-byte-match.json').read_text())

    def test_exact_match_and_current_source(self):
        row = self.report['functions'][0]
        self.assertTrue(self.report['all_functions_match'])
        self.assertTrue(row['exact_after_address_resolution'])
        self.assertEqual(row['original_bytes'], 124)
        self.assertEqual(row['generated_bytes'], 124)
        self.assertEqual(row['differing_bytes'], 0)
        self.assertEqual(hashlib.sha256((ROOT / 'compat/stars-small-match/STARS.C').read_bytes()).hexdigest(),
                         self.report['source_sha256'])

    def test_fixture_reproduces_match(self):
        module, _ = object_module(bytes.fromhex(self.fixture['object_hex']))
        self.assertIn('FHANDLECHAR', module.publics[0]['name'].upper())
        result = compare(bytes.fromhex(self.fixture['object_hex']))
        self.assertTrue(result['all_functions_match'])


if __name__ == '__main__':
    unittest.main()
