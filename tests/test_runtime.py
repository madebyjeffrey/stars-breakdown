"""Ensure comparisons detect changes and enforce explicit, intact fixture inputs."""
import json
from pathlib import Path
import tempfile
import unittest
from unittest.mock import patch
from tools.runtime import compare as runtime
from tools import baseline

class RuntimeChecks(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        self.source = self.root/'source'
        self.source.mkdir()
        self.patch = patch.object(runtime, 'ROOT', self.root)
        self.patch.start()
        self.addCleanup(self.patch.stop)

    def snapshot(self, name, data):
        (self.source/'TEST.HST').write_bytes(data)
        dest = self.root/'runs'/name
        runtime.capture(self.source, ['TEST.HST'], dest)
        return dest

    def test_equality_and_changed_byte_plus_length(self):
        a = self.snapshot('a', b'abc')
        b = self.snapshot('b', b'abc')
        self.assertTrue(runtime.compare(a,b)['equal'])
        c = self.snapshot('c', b'axcd')
        row = runtime.compare(a,c)['files'][0]
        self.assertFalse(row['equal'])
        self.assertEqual(row['first_difference'],1)
        self.assertEqual(row['differing_bytes'],2)

    def test_tampering_and_empty_manifest_rejected(self):
        a = self.snapshot('a',b'abc')
        (a/'TEST.HST').write_bytes(b'xyz')
        with self.assertRaises(ValueError): runtime.verified_snapshot(a)
        (a/'manifest.json').write_text('{"normalization":"none","files":[]}')
        with self.assertRaises(ValueError): runtime.verified_snapshot(a)

    def test_paths_and_unrelated_extensions_rejected(self):
        for name in ('../TEST.HST','/TEST.HST','TEST.HST.txt','notes.txt','TEST.M17'):
            with self.assertRaises(ValueError): runtime.game_file(self.source,name)
        with self.assertRaises(ValueError): runtime.capture(self.source,[],self.root/'runs/empty')
        with self.assertRaises(ValueError): runtime.capture(self.source,['TEST.HST'],self.root/'outside')

    def test_missing_file_is_mismatch(self):
        a = self.snapshot('a', b'abc')
        (self.source/'TEST.M1').write_bytes(b'player')
        b = self.root/'runs/b'
        runtime.capture(self.source,['TEST.M1'],b)
        self.assertFalse(runtime.compare(a,b)['equal'])

    def test_baseline_never_discovers_extra_inputs(self):
        with patch.object(Path,'rglob',side_effect=AssertionError('Recursive discovery forbidden')):
            self.assertTrue(baseline.inventory()['files'])

if __name__ == '__main__': unittest.main()
