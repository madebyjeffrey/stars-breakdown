import hashlib
import json
import tempfile
import unittest
from pathlib import Path

from tools.compare_all_byte_match import ROOT, compare


class AllByteMatchTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.report = json.loads(
            (ROOT / 'analysis/compat/all-byte-match.json').read_text()
        )

    def test_all_625_exact_and_current_sources(self):
        self.assertEqual(len(self.report['functions']), 625)
        self.assertTrue(self.report['all_functions_match'])
        for row in self.report['functions']:
            self.assertTrue(row['exact_after_address_resolution'])
            self.assertEqual(row['differing_bytes'], 0)
            self.assertEqual(row['original_bytes'], row['generated_bytes'])
            self.assertEqual(
                hashlib.sha256((ROOT / row['source']).read_bytes()).hexdigest(),
                row['source_sha256'],
            )

    def test_fixture_reproduces_match(self):
        with tempfile.TemporaryDirectory() as directory:
            fixture = Path(directory)
            for index, (_, blob) in enumerate(self.report['objects'].items()):
                (fixture / f'F{index:03d}.OBJ').write_bytes(bytes.fromhex(blob))
            result = compare(fixture)
            self.assertTrue(result['all_functions_match'])


if __name__ == '__main__':
    unittest.main()
