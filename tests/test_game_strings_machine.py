"""All embedded string ids and cache/copy behavior against original code."""
import ctypes as C
from pathlib import Path
import struct
import unittest
from tests import test_core_machine as core_tests
from tools.core_machine import OriginalCore
from tools.ne_layout import NE

ROOT=Path(__file__).resolve().parents[1]

class GameStringsMachineTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        core_tests.CoreMachineTests.setUpClass.__func__(cls)
        cls.strings=(C.c_ubyte*65536).from_buffer_copy(bytes(NE(ROOT/'target/stars.exe').raw[2]).ljust(65536,b'\0'))
        for name, extra in [('stars_get_compressed_string',[]),('stars_copy_game_string',[C.c_uint16])]:
            fn=getattr(cls.lib,name);fn.argtypes=[C.POINTER(C.c_ubyte)]*2+[C.c_int16]+extra;fn.restype=C.c_uint16

    @classmethod
    def tearDownClass(cls):cls.temp.cleanup()

    def setUp(self):self.machine=OriginalCore()

    def compare(self,id,output=None,marker=255,cached=b'old cache\0'):
        memory=bytearray([0xa5])*65536
        memory[0x1484]=marker
        memory[0x547a:0x547a+len(cached)]=cached
        self.machine.put(0,bytes(memory))
        host=(C.c_ubyte*65536).from_buffer(memory)
        if output is None:
            actual=self.lib.stars_get_compressed_string(host,self.strings,id)
            original=self.machine.call(0x743a,struct.pack('<h',id),segment=3)&65535
        else:
            actual=self.lib.stars_copy_game_string(host,self.strings,id,output)
            original=self.machine.call(0x221e,struct.pack('<hH',id,output),segment=9)&65535
        self.assertEqual(actual,original,(id,output,marker))
        self.assertEqual(bytes(memory),self.machine.get(0,65536),(id,output,marker))
        self.assertEqual(memory[0x1484],marker)
        return memory,actual

    def test_all_1414_strings_and_copy_lengths(self):
        for id in range(1414):
            self.compare(id)
            self.compare(id,0x3000)

    def test_signed_byte_cache_hit_and_non_update(self):
        for id in [-128,-1,0,1,63,127]:
            memory,result=self.compare(id,marker=id&255)
            self.assertEqual(result,0x547a)
            self.assertEqual(memory[0x547a:0x5484],b'old cache\0')
        for id in [128,255,256,1340,1413]:self.compare(id,marker=id&255)

    def test_forward_aliasing_and_wrapped_destination(self):
        for id in [0,15,63,64,0x532,0x56e,1413]:
            for output in [0x547a,0x5479,0xfff8]:self.compare(id,output)

if __name__=='__main__':unittest.main()
