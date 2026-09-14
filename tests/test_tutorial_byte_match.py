"""Reproduce tutorial byte credit and reject instruction, target and extent changes."""
import copy
import hashlib
import json
from pathlib import Path
import unittest
from unittest.mock import patch
from tools.compare_tutorial_match import ROOT, compare, original_image, symbols, extent
from tests.test_byte_match import alter_record

FIXTURE = ROOT/'tests/fixtures/tutorial-byte-match.json'
REPORT = ROOT/'analysis/compat/tutorial-byte-match.json'


class TutorialByteMatchTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.fixture=json.loads(FIXTURE.read_text())
        cls.report=json.loads(REPORT.read_text())
        cls.procs={p['name']:p for p in json.loads((ROOT/'analysis/debug/procedures.json').read_text())}

    def test_evidence_is_bound_to_current_sources_and_objects(self):
        fixtures={f['stem']:f for f in self.fixture['objects']}
        for row in self.report['functions']:
            with self.subTest(name=row['name']):
                self.assertEqual(hashlib.sha256((ROOT/row['source']).read_bytes()).hexdigest(),row['source_sha256'])
                self.assertEqual(hashlib.sha256((ROOT/'compat/tutorial-match/TYPES.H').read_bytes()).hexdigest(),row['header_sha256'])
                blob=bytes.fromhex(fixtures[row['stem']]['object_hex'])
                self.assertEqual(hashlib.sha256(blob).hexdigest(),row['object_sha256'])
                self.assertEqual(extent(fixtures[row['stem']]['listing'],row['name'])[1],row['generated_bytes'])

    @unittest.skipUnless((ROOT/'target/stars.exe').exists(),'original required')
    def test_reproduce_every_recorded_outcome(self):
        image=original_image();known=symbols()
        for row, fixture in zip(self.report['functions'],self.fixture['objects']):
            with self.subTest(name=row['name']):
                self.assertEqual(row['stem'],fixture['stem'])
                actual,_,_=compare(bytes.fromhex(fixture['object_hex']),fixture['listing'],self.procs[row['name']],image,known)
                for key in actual:
                    self.assertEqual(actual[key],row[key],key)

    @unittest.skipUnless((ROOT/'target/stars.exe').exists(),'original required')
    def test_no_masking_of_instructions_or_relocation_targets(self):
        row=next(r for r in self.report['functions'] if r['name']=='ShowTutor')
        fixture=next(f for f in self.fixture['objects'] if f['stem']==row['stem'])
        blob=bytes.fromhex(fixture['object_hex']);image=original_image();known=symbols();p=self.procs['ShowTutor']
        def change(body):
            if body[:3]!=b'\x01\0\0':return None
            self.assertEqual(body[3],0x55)
            return body[:3]+b'\x54'+body[4:]
        actual,_,_=compare(alter_record(blob,0xa0,change),fixture['listing'],p,image,known)
        self.assertFalse(actual['exact_after_address_resolution'])
        self.assertEqual(actual['differences'][0]['offset'],0)
        moved=copy.deepcopy(known)
        moved['_tutor']={v+2 for v in moved['_tutor']}
        actual,_,_=compare(blob,fixture['listing'],p,image,moved)
        self.assertFalse(actual['exact_after_address_resolution'])
        del moved['_tutor']
        with self.assertRaisesRegex(ValueError,'Unknown/ambiguous external'):
            compare(blob,fixture['listing'],p,image,moved)
        shortened=dict(p,size=p['size']-1)
        actual,_,_=compare(blob,fixture['listing'],shortened,image,known)
        self.assertFalse(actual['exact_after_address_resolution'])
        self.assertEqual(actual['length_difference'],1)

    def test_todo_byte_credit_is_separate_and_hash_checked(self):
        from tools.render_todo5 import byte_matches
        actual=byte_matches()
        for row in self.report['functions']:
            identity=(row['ne_segment'],row['original_offset'])
            self.assertEqual(identity in actual,row['exact_after_address_resolution'])

    def test_stale_source_evidence_cannot_receive_todo_credit(self):
        from tools import render_todo5
        original_read=render_todo5.read
        def stale_read(path):
            result=original_read(path)
            if path=='analysis/compat/tutorial-byte-match.json':
                result['functions'][0]['source_sha256']='0'*64
            return result
        with patch.object(render_todo5,'read',side_effect=stale_read):
            with self.assertRaisesRegex(AssertionError,'Stale byte evidence'):
                render_todo5.byte_matches()

if __name__=='__main__':unittest.main()
