"""Whole random-race creation with real RNG, scoring, strings and CRT callees."""
import ctypes as C
from pathlib import Path
import struct
import unittest
from tests import test_core_machine as core_tests
from tools.core_machine import OriginalCore, reg
from tools.ne_layout import NE
from unicorn import UC_HOOK_CODE

ROOT=Path(__file__).resolve().parents[1]

class RandomRaceMachineTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        core_tests.CoreMachineTests.setUpClass.__func__(cls)
        cls.strings=(C.c_ubyte*65536).from_buffer_copy(bytes(NE(ROOT/'target/stars.exe').raw[2]).ljust(65536,b'\0'))
        cls.lib.stars_create_random_race.argtypes=[C.POINTER(C.c_ubyte)]*2+[C.c_uint16]
        cls.lib.stars_create_random_race.restype=None
        cls.lib.stars_copy_game_string.argtypes=[C.POINTER(C.c_ubyte)]*2+[C.c_int16,C.c_uint16]
        cls.lib.stars_race_advantage_points.argtypes=[C.POINTER(C.c_ubyte),C.c_uint16]
        cls.lib.stars_race_advantage_points.restype=C.c_int16

    @classmethod
    def tearDownClass(cls):cls.temp.cleanup()

    def setUp(self):self.machine=OriginalCore()

    def compare(self,seed,player=0x3000,placeholder=False,seed_pair=None):
        memory=bytearray(self.machine.get(0,65536))
        record=bytearray(memory[0xda2:0xe62])
        record[128:160]=b'Synthetic race\0'.ljust(32,b'\0')
        record[160:192]=bytes([0x6a])*32
        memory[player:player+192]=record
        memory[0x1484]=255
        state=core_tests.Seeds()
        self.lib.stars_randomize(C.byref(state),seed)
        struct.pack_into('<II',memory,0x1370,*(seed_pair if seed_pair is not None else (state.first,state.second)))
        host=(C.c_ubyte*65536).from_buffer(memory)
        if placeholder:self.lib.stars_copy_game_string(host,self.strings,0x532,player+128)
        self.machine.put(0,bytes(memory))
        paths=set()
        counts={}
        def entered(cpu,at,size,unused):
            point=at-0x40000
            paths.add(point)
            counts[point]=counts.get(point,0)+1
        self.last_counts=counts
        hooks=[]
        for point in [0x5b31,0x5b97,0x5bfd,0x5ccb,0x5d54,0x5d9f,0x5dea,
                      0x6029,0x606a,0x60a0,0x60da,0x6115,0x622c,0x62da,0x6397,0x647c,0x6539,
                      0x6198,0x620b,0x62bd,0x636f,0x6449,0x6506,0x65ba]:
            address=0x40000+point
            hooks.append(self.machine.cpu.hook_add(UC_HOOK_CODE,
                          entered,begin=address,end=address))
        self.lib.stars_create_random_race(host,self.strings,player)
        for name,v in [('FPCW',0x37f),('FPSW',0),('FPTAG',0xffff)]:self.machine.cpu.reg_write(getattr(reg,'UC_X86_REG_'+name),v)
        self.machine.call(0x5b08,struct.pack('<H',player),segment=29,shared_stack=True,instruction_budget=5000000000)
        for hook in hooks:self.machine.cpu.hook_del(hook)
        for offset,size in [(player,192),(0x59a2,192),(0xda2,192),(0x1370,8),(0x1484,1),(0x547a,256)]:
            self.assertEqual(bytes(memory[offset:offset+size]),self.machine.get(offset,size),(seed,hex(offset)))
        if not placeholder:
            self.assertEqual(memory[player+128:player+143],b'Synthetic race\0')
        return paths,self.lib.stars_race_advantage_points(host,player)

    def test_seeded_generation_and_name_paths(self):
        paths=set()
        for seed in range(12):
            hit,score=self.compare(seed,0x59a2 if seed%3==0 else 0x3000,seed%2==0)
            paths |= hit
            if 0x60a0 not in hit:self.assertTrue(0<=score<=50, (seed,score))
        self.assertTrue({0x6029,0x6115,0x622c,0x62da}.issubset(paths),paths)

    def test_remaining_initial_branches(self):
        # Seed 13 starts mode 7; seed 21 starts mode 11 (single immunity).
        paths=set()
        for seed in [13,21]:
            hit,score=self.compare(seed,placeholder=True);paths |= hit
            if 0x60a0 not in hit:self.assertTrue(0<=score<=50)
        self.assertTrue({0x5bfd,0x5d54}.issubset(paths),paths)

    def test_stalled_rng_fallback_preserves_name(self):
        # Both zero seeds remain zero, forcing repeated choices. No original
        # callee is patched: this exercises the actual 251-attempt limit.
        hit,_=self.compare(0,seed_pair=(0,0))
        self.assertIn(0x60a0,hit)
        self.assertEqual(self.last_counts[0x60da],251)

if __name__=='__main__':unittest.main()
