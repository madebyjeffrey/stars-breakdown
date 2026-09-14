"""Exact byte comparison must reject both instruction and relocation changes."""
import json
from pathlib import Path
import struct
import unittest
from tools.compare_byte_match import ROOT, object_module, compare


def alter_record(blob, kind, mutate):
    out=bytearray();pos=0;changed=False
    while pos<len(blob):
        typ,n=struct.unpack_from('<BH',blob,pos);raw=blob[pos:pos+3+n]
        if typ==kind and not changed:
            body=mutate(raw[3:-1])
            if body is not None:
                raw=bytes([typ])+struct.pack('<H',len(body)+1)+body
                raw+=bytes([-sum(raw)&255]);changed=True
        out.extend(raw);pos+=3+n
    assert changed
    return bytes(out)


class ByteMatchTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.fixture=json.loads((ROOT/'tests/fixtures/byte-match-probe.json').read_text())
        cls.blob=bytes.fromhex(cls.fixture['object_hex'])

    def test_compiler_backpatches_and_extents(self):
        m,patches=object_module(self.blob)
        self.assertTrue(patches)
        self.assertEqual(m.segments[1].data[:9],bytes.fromhex('558bec81ec02005657'))
        self.assertEqual(len(m.segments[1].fixups),21)
        self.assertEqual([p['offset'] for p in m.publics],[0,66,164])

    def test_reject_truncation_and_checksum_corruption(self):
        with self.assertRaises(ValueError):object_module(self.blob[:-1])
        b=bytearray(self.blob);b[5]^=1
        # Compiler records can use the OMF checksum=0 convention; make this
        # record nonzero and demonstrably invalid rather than relying on it.
        n=int.from_bytes(b[1:3],'little');b[n+2]=1
        if sum(b[:n+3])&255==0:b[n+2]=2
        with self.assertRaisesRegex(ValueError,'checksum'):object_module(bytes(b))

    @unittest.skipUnless((ROOT/'target/stars.exe').exists(),'original required')
    def test_matches_original_and_rejects_changed_instruction(self):
        result=compare(self.blob,self.fixture['listing'])
        self.assertTrue(result['all_functions_match'])
        self.assertTrue(result['stat_tables_match'])
        def change(body):
            # LEDATA segment 1 offset 0 starts with the getter PUSH BP.
            if body[:3]!=b'\x01\0\0':return None
            self.assertEqual(body[3],0x55)
            return body[:3]+b'\x54'+body[4:]
        changed=alter_record(self.blob,0xa0,change)
        result=compare(changed,self.fixture['listing'])
        self.assertFalse(result['all_functions_match'])
        self.assertEqual(result['functions'][0]['nonaddress_byte_differences'][0]['offset'],0)

    @unittest.skipUnless((ROOT/'target/stars.exe').exists(),'original required')
    def test_relocation_identity_is_checked_not_masked(self):
        def change(body):
            if b'_lSeed1' not in body:return None
            return body.replace(b'_lSeed1',b'_lSeed2')
        result=compare(alter_record(self.blob,0x8c,change),self.fixture['listing'])
        self.assertFalse(result['all_functions_match'])
        self.assertFalse(result['functions'][2]['exact_after_address_resolution'])

if __name__=='__main__':unittest.main()
