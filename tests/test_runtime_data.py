"""CRT data identity and negative controls, separate from code-byte matching."""
import copy
import json
import unittest
from unittest.mock import patch
from tools.review_runtime_data import ROOT,review
from tools.omf import library

class RuntimeDataTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.directory=ROOT/'runs/step4/guest/MSVC/LIB'
        if not (cls.directory/'MLIBCEW.LIB').is_file():
            raise unittest.SkipTest('Local VC runtime library required')
        cls.members=library((cls.directory/'MLIBCEW.LIB').read_bytes())
        cls.report=review(cls.directory)
    def test_audit_preserves_unresolved_linker_boundaries(self):
        self.assertEqual(json.loads(json.dumps(self.report)),json.loads((ROOT/'analysis/reconstruction/runtime-data.json').read_text()))
        self.assertEqual(self.report['summary'],dict(modules=108,records_without_code=7,data_segments=44,
            initialized_literal_bytes=2199,data_fixups=99,unresolved=4))
        data_only=[r for r in self.report['modules'] if not r['original_code_bytes']]
        self.assertEqual(sum(r['status']=='verified_data_mapping' for r in data_only),6)
        for r in self.report['modules']:
            for d in r['data']:
                self.assertLessEqual(len(d.get('candidate_examples',[])),8)
                if d['name'] in ['PAD','EPAD']:self.assertEqual(d['status'],'unresolved_placement')
    def test_initialized_data_mutation_is_detected(self):
        members=copy.deepcopy(self.members)
        member=next(m for m in members if m.offset==39120)
        segment=next(s for s in member.segments if s and s.size==257)
        segment.data[70]^=1
        with patch('tools.review_runtime_data.library',return_value=members):changed=review(self.directory)
        row=next(r for r in changed['modules'] if r['member_offset']==39120)
        self.assertEqual(row['status'],'unresolved')
        self.assertEqual(row['data'][0]['literal_mismatches'],[70])
    def test_data_pointer_target_mutation_is_detected(self):
        members=copy.deepcopy(self.members)
        member=next(m for m in members if m.offset==38848)
        segment=next(s for s in member.segments if s and s.fixups)
        segment.fixups[0]['displacement']+=1
        with patch('tools.review_runtime_data.library',return_value=members):changed=review(self.directory)
        row=next(r for r in changed['modules'] if r['member_offset']==38848)
        self.assertEqual(row['status'],'unresolved')
        self.assertEqual(row['data'][0]['fixups'][0]['status'],'pointer_mismatch')

if __name__=='__main__':unittest.main()
