"""Structural invariants plus independent byte-level anchors for this NE image."""
import hashlib
import json
from pathlib import Path
import struct
import unittest
from tools.ne_layout import NE,chain_sites,patch

ROOT=Path(__file__).resolve().parents[1]

class LayoutChecks(unittest.TestCase):
    @classmethod
    def setUpClass(cls):cls.ne=NE(ROOT/'target/stars.exe')

    def test_whole_file_partition(self):
        cursor=0
        for r in self.ne.regions:
            self.assertEqual(r['start'],cursor)
            cursor=r['end_exclusive']
            self.assertNotEqual(r['classification'],'unclassified nonzero gap')
        self.assertEqual(cursor,len(self.ne.b))

    def test_header_and_stub_independent_bytes(self):
        b=self.ne.b
        self.assertEqual(b[0x414:0x41c].hex(),'1a00240000002500')
        self.assertEqual((self.ne.header['cs'],self.ne.header['ip']),(36,26))
        # DOS CALL reaches POP DX, then sets DS=CS, prints message, exits with 1.
        self.assertEqual(b[0x200:0x203].hex(),'e85300')
        self.assertEqual(b[0x256:0x262].hex(),'5a0e1fb409cd21b8014ccd21')
        self.assertEqual(b[0x203:0x22f],b'This program requires Microsoft Windows.\r\n$ ')

    def test_cross_segment_far_call(self):
        # ReadIniSettings -> FSerialAndEnvFromSz, verified against debug names.
        self.assertEqual(self.ne.b[16152:16160].hex(),'0200d21705000000')
        raw=self.ne.raw[0];mem=self.ne.memory[0]
        self.assertEqual(raw[0x17cf:0x17d4].hex(),'9aec2a4109')
        self.assertEqual(mem[0x17cf:0x17d4].hex(),'9aec2a2810')
        fs=json.loads((ROOT/'analysis/functions.json').read_text())
        self.assertTrue(any(f['name']=='FSerialAndEnvFromSz' and (f['ne_segment'],f['ne_offset'])==(5,0x2aec) for f in fs))

    def test_import_chain_messagebox(self):
        r=next(r for r in self.ne.relocations if r['segment']==5 and r.get('import_key')=='USER!#1')
        self.assertEqual(self.ne.b[118825:118833].hex(),'0301297304000100')
        self.assertEqual(r['sites'],[29481,16395])
        self.assertEqual(struct.unpack_from('<H',self.ne.raw[4],29481)[0],16395)
        self.assertEqual(struct.unpack_from('<H',self.ne.raw[4],16395)[0],65535)
        for p in r['sites']:self.assertEqual(self.ne.memory[4][p:p+4].hex(),'01002080')
        self.assertEqual(self.ne.imports['USER!#1']['name'],'MessageBox')

    def test_all_relocations_resolve_and_match_image(self):
        occupied=set()
        for r in self.ne.relocations:
            if r['target_kind']=='os_fpu_fixup':
                p=r['source_offset'];self.assertEqual(self.ne.raw[r['segment']-1][p:p+2],self.ne.memory[r['segment']-1][p:p+2]);continue
            width={0:1,2:2,3:4,5:2}[r['address_kind']]
            selector=r['synthetic_target']['selector'];off=r['synthetic_target']['offset']
            expected={0:bytes([off&255]),2:struct.pack('<H',selector),3:struct.pack('<HH',off,selector),5:struct.pack('<H',off)}[r['address_kind']]
            self.assertFalse(r['additive'])  # Baseline has additive FPU records only.
            for p in r['sites']:
                for j in range(width):
                    key=(r['segment'],p+j);self.assertNotIn(key,occupied);occupied.add(key)
                self.assertEqual(bytes(self.ne.memory[r['segment']-1][p:p+width]),expected)
        self.assertEqual(len(self.ne.relocations),2783)
        self.assertEqual(len(self.ne.imports),197)

    def test_zero_fill_and_translations(self):
        self.assertEqual(self.ne.raw[36][8818:],bytes(17236))
        self.assertIsNone(self.ne.ne_to_file(37,8818))
        self.assertEqual(self.ne.cv_to_ne(38,0),(36,0))
        self.assertEqual(self.ne.file_to_ne(835538),(29,0x30d2))
        self.assertEqual(self.ne.selector_to_ne(0x10e8,0x30d2),(29,0x30d2))

    def test_global_data_address_from_machine_instruction(self):
        # SetFileSeeds stores the two 32-bit globals at DS:2272 and DS:2276.
        self.assertEqual(self.ne.cv_to_ne(69,0),(37,0x2272))
        self.assertEqual(self.ne.cv_to_ne(69,4),(37,0x2276))
        self.assertIsNone(self.ne.ne_to_file(37,0x2272))
        fs=json.loads((ROOT/'analysis/functions.json').read_text())
        f=next(f for f in fs if f['name']=='SetFileSeeds')
        raw=self.ne.b[f['file_offset']:f['file_offset']+f['size']]
        self.assertIn(bytes.fromhex('a3722289167422'),raw)
        self.assertIn(bytes.fromhex('a3762289167822'),raw)

    def test_resources_and_references(self):
        rs=self.ne.resources
        self.assertEqual(len(rs),122)
        about=next(r for r in rs if (r['type'],r['id'])==(5,90))
        self.assertEqual(about['decoded']['caption'],'About Stars!')
        self.assertEqual(about['decoded']['control_count'],10)
        for r in rs:
            data=self.ne.b[r['file_offset']:r['file_offset']+r['bytes']]
            self.assertEqual(hashlib.sha256(data).hexdigest(),r['sha256'])
            if r['type'] in (12,14):
                typ=1 if r['type']==12 else 3
                for item in r['decoded']['items']:
                    image=next(x for x in rs if (x['type'],x['id'])==(typ,item['id']))
                    self.assertLessEqual(item['size'],image['bytes'])
                    self.assertLess(image['bytes']-item['size'],64)
        image_names={r['id'].upper() if isinstance(r['id'],str) else r['id'] for r in rs if r['type']==14}
        for r in rs:
            if r['type']==5:
                for c in r['decoded']['controls']:
                    if c['window_class']=='STATIC' and c['style']&31==3:
                        name=c['text'];name=name['ordinal'] if isinstance(name,dict) else name
                        self.assertIn(name.upper() if isinstance(name,str) else name,image_names)

class FixupEdgeCases(unittest.TestCase):
    def test_additive_pointer_preserves_offset_addend_only(self):
        mem=bytearray.fromhex('feff3412');patch(mem,0,3,0x1028,5,True)
        self.assertEqual(mem.hex(),'03002810')
        self.assertEqual(chain_sites(bytes.fromhex('03002810'),0,True,4),[0])

    def test_bad_chains_rejected(self):
        with self.assertRaises(ValueError):chain_sites(bytes.fromhex('0000'),0,False,2)
        with self.assertRaises(ValueError):chain_sites(bytes.fromhex('0300'),0,False,2)

if __name__=='__main__':unittest.main()
