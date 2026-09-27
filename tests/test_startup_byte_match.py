"""Regression checks for the exact VC152 InitInstance comparison."""
import hashlib
import json
import unittest
from pathlib import Path

from tools.compare_byte_match import object_module
from tools.compare_startup_match import ROOT, compare


class StartupByteMatchTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.report = json.loads((ROOT / 'analysis/compat/startup-byte-match.json').read_text())
        cls.fixture = json.loads((ROOT / 'tests/fixtures/startup-byte-match.json').read_text())

    def test_exact_match_and_current_source(self):
        row = self.report['functions'][0]
        self.assertTrue(self.report['all_functions_match'])
        self.assertTrue(row['exact_after_address_resolution'])
        self.assertEqual(row['original_bytes'], 314)
        self.assertEqual(row['generated_bytes'], 314)
        self.assertEqual(row['differing_bytes'], 0)
        self.assertEqual(hashlib.sha256((ROOT / 'compat/startup-match/INIT.C').read_bytes()).hexdigest(),
                         self.report['source_sha256'])

    def test_fixture_reproduces_match(self):
        module, _ = object_module(bytes.fromhex(self.fixture['object_hex']))
        self.assertEqual(module.publics[0]['name'], '_InitInstance')
        result = compare(bytes.fromhex(self.fixture['object_hex']), self.fixture['listing'])
        self.assertTrue(result['all_functions_match'])


if __name__ == '__main__':
    unittest.main()
