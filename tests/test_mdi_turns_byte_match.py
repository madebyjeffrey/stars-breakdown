"""Regression check for the exact CFindTurnsOutstanding byte comparison."""
import hashlib
import json
import unittest

from tools.compare_byte_match import object_module
from tools.compare_mdi_turns_match import ROOT, compare


class MdiTurnsByteMatchTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.report = json.loads((ROOT / 'analysis/compat/mdi-turns-byte-match.json').read_text())
        cls.fixture = json.loads((ROOT / 'tests/fixtures/mdi-turns-byte-match.json').read_text())

    def test_exact_match_and_current_source(self):
        row = self.report['functions'][0]
        self.assertTrue(self.report['all_functions_match'])
        self.assertEqual((row['original_bytes'], row['generated_bytes'], row['differing_bytes']),
                         (495, 495, 0))
        self.assertEqual(hashlib.sha256((ROOT / 'compat/mdi-turns-match/MDI.C').read_bytes()).hexdigest(),
                         self.report['source_sha256'])

    def test_fixture_reproduces_match(self):
        module, _ = object_module(bytes.fromhex(self.fixture['object_hex']))
        self.assertIn('CFINDTURNSOUTSTANDING', module.publics[0]['name'].upper())
        self.assertTrue(compare(bytes.fromhex(self.fixture['object_hex']))['all_functions_match'])


if __name__ == '__main__':
    unittest.main()
