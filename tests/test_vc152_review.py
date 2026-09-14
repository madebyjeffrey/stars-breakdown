"""A native compatibility pass needs both reports, stages and intact inputs."""
import copy
import json
from pathlib import Path
import unittest

from tools.review_vc152 import assess_return, fields

ROOT = Path(__file__).resolve().parents[1]
MANIFEST = 'WIN16_RUNTIME_CHECKS 563\nWIN16_LAYOUT_CHECKS 1491\n'


class VC152ReviewTests(unittest.TestCase):
    def setUp(self):
        self.evidence = json.loads((ROOT / 'analysis/compat/vc152-return2.json').read_text())

    def test_retained_return_passes_with_both_native_executables(self):
        result = assess_return(self.evidence, MANIFEST)
        self.assertTrue(result['native_compatibility_pass'])
        for name in ('BLDSTAT.TXT', 'RESULTS.TXT', 'LAYRES.TXT'):
            info = self.evidence['artifacts'][name]['text_bytes']
            self.assertTrue(info['ascii_only'])
            self.assertEqual(info['nul_bytes'], 0)
            self.assertEqual(info['lone_lf_count'], 0)

    def test_partial_or_host_results_cannot_complete_native_check(self):
        cases = [
            ('LAYRES.TXT', 'MODE=WIN16_MEDIUM', 'MODE=HOST_ONLY'),
            ('LAYRES.TXT', 'CHECKS=1491', 'CHECKS=0'),
            ('LAYRES.TXT', 'FAILURES=0', 'FAILURES=1'),
            ('BLDSTAT.TXT', 'LAYLINK=PASS', 'LAYLINK=FAIL'),
            ('RESULTS.TXT', 'RESULT=PASS', 'RESULT=FAIL'),
        ]
        for file, before, after in cases:
            with self.subTest(file=file, replacement=after):
                evidence = copy.deepcopy(self.evidence)
                self.assertIn(before, evidence['logs'][file])
                evidence['logs'][file] = evidence['logs'][file].replace(before, after)
                self.assertFalse(assess_return(evidence, MANIFEST)['native_compatibility_pass'])
        self.evidence['package_inputs'][0]['unchanged'] = False
        self.assertFalse(assess_return(self.evidence, MANIFEST)['native_compatibility_pass'])

    def test_missing_executable_or_duplicate_result_is_not_a_pass(self):
        self.evidence.pop('layout_debug_summary')
        self.assertFalse(assess_return(self.evidence, MANIFEST)['native_compatibility_pass'])
        with self.assertRaises(ValueError):
            fields('RESULT=FAIL\nRESULT=PASS\n')


if __name__ == '__main__':
    unittest.main()
