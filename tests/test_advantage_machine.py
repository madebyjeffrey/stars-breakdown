"""Complete race scoring, repair side effects, and growth against original code."""
import ctypes as C
import itertools
import random
import struct
import unittest
from tests import test_core_machine as core_tests
from tools.core_machine import OriginalCore, reg


class AdvantageMachineTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        core_tests.CoreMachineTests.setUpClass.__func__(cls)
        cls.lib.stars_race_advantage_points.argtypes = [C.POINTER(C.c_ubyte), C.c_uint16]
        cls.lib.stars_race_advantage_points.restype = C.c_int16
        cls.lib.stars_true_max_growth.argtypes = [C.POINTER(C.c_ubyte), C.c_int16]
        cls.lib.stars_true_max_growth.restype = C.c_int16

    @classmethod
    def tearDownClass(cls): cls.temp.cleanup()

    def setUp(self):
        self.machine = OriginalCore()
        self.initial = self.machine.get(0,65536)

    def record(self, immune=True):
        r = bytearray(192)
        r[16:25] = bytes([255]*9 if immune else [50]*3+[25]*3+[75]*3)
        r[25] = 15
        r[62:78] = bytes([10,10,10,10,10,3,10,0,1,1,1,1,1,1,0,0])
        r[84:86] = b'\x81\xa5'
        return r

    def compare(self, record, player=0x3000):
        memory = bytearray(self.initial)
        memory[0x59a2:0x5a62] = bytes((i*13+7)&255 for i in range(192))
        memory[player:player+192] = record
        before = bytes(memory[0x59a2:0x5a62])
        self.machine.put(0,bytes(memory))
        for name, v in [('FPCW',0x37f),('FPSW',0),('FPTAG',0xffff)]:
            self.machine.cpu.reg_write(getattr(reg,'UC_X86_REG_'+name),v)
        actual = self.lib.stars_race_advantage_points((C.c_ubyte*65536).from_buffer(memory),player)
        original = C.c_int16(self.machine.call(0x444c,struct.pack('<H',player),segment=29,shared_stack=True)&65535).value
        self.assertEqual(actual,original,(record.hex(),player))
        self.assertEqual(bytes(memory[player:player+192]),self.machine.get(player,192))
        self.assertEqual(bytes(memory[0x59a2:0x5a62]),self.machine.get(0x59a2,192))
        if player != 0x59a2: self.assertEqual(bytes(memory[0x59a2:0x5a62]),before)
        return actual, memory[player:player+192]

    def test_growth_all_signed_bytes_and_repair(self):
        for raw in range(256):
            record = self.record();record[25] = raw
            _, repaired = self.compare(record)
            expected = 1 if raw == 0 or raw >= 128 else min(raw,20)
            self.assertEqual(repaired[25],expected)
            self.assertEqual(repaired[84],0x81 if 1<=raw<=20 else 0x91)

    def test_stat_boundaries_and_factory_thresholds(self):
        for index in range(16):
            for raw in list(range(27))+[127,128,254,255]:
                record = self.record();record[62+index] = raw
                self.compare(record)
        for a,b,c in itertools.product([5,10,13,15],[5,10,25],[5,10,16,17,21,22,24,25]):
            record=self.record();record[63:66]=bytes([a,b,c]);self.compare(record)

    def test_research_all_combinations(self):
        for values in itertools.product(range(3),repeat=6):
            record=self.record();record[70:76]=bytes(values)
            record[62]=9 if sum(values)%2 else 10
            record[76]=8 if values[0]==2 else 0
            self.compare(record)

    def test_trait_counts_and_primary_interactions(self):
        rng=random.Random(270019)
        masks=[0,0x3fff,0xffffffff]+[1<<i for i in range(32)]
        masks += [rng.getrandbits(14) for _ in range(200)]
        for i,flags in enumerate(masks):
            record=self.record();record[76]=i%10
            record[78:82]=struct.pack('<I',flags);self.compare(record)
        for primary,flags in itertools.product(range(10),[1<<10,1<<29,1<<31,(1<<31)|(1<<29)|(1<<10)]):
            record=self.record();record[76]=primary;record[78:82]=struct.pack('<I',flags)
            self.compare(record,0x59a2)

    def test_mixed_environments_and_random_production(self):
        rng=random.Random(270019)
        for i in range(100):
            record=self.record(False)
            for axis in range(3):
                low=rng.randrange(99); high=rng.randrange(low+2,101)
                if i%8 & (1<<axis): low=high=255
                record[19+axis],record[22+axis]=low,high
                record[16+axis]=rng.randrange(256)  # bounds check must recompute
            record[25]=rng.randrange(1,21)
            record[62:78]=bytes(rng.randrange(26) for _ in range(16))
            record[78:82]=struct.pack('<I',rng.getrandbits(32))
            self.compare(record,0x59a2 if i%3==0 else 0x3000)

    def test_true_growth_signed_bytes_and_player_addressing(self):
        memory=bytearray(self.initial)
        for index in [0,1,15,-1,32767,-32768]:
            player=(0x59a2+index*192)&65535
            for primary in [0,1,8,255]:
                for raw in range(256):
                    memory[(player+25)&65535]=raw
                    memory[(player+76)&65535]=primary
                    self.machine.put(0,bytes(memory))
                    actual=self.lib.stars_true_max_growth((C.c_ubyte*65536).from_buffer(memory),index)
                    original=C.c_int16(self.machine.call(0x65d4,struct.pack('<h',index),segment=29)&65535).value
                    self.assertEqual(actual,original,(index,primary,raw))
                    self.assertEqual(bytes(memory),self.machine.get(0,65536))

    def test_scoring_degenerate_range_preserves_original_fault(self):
        from unicorn import UcError, UC_ERR_EXCEPTION
        record=self.record(False);record[19]=record[22]=50
        self.machine.put(0x3000,bytes(record))
        with self.assertRaises(UcError) as error:
            self.machine.call(0x444c,struct.pack('<H',0x3000),segment=29,shared_stack=True)
        self.assertEqual(error.exception.errno,UC_ERR_EXCEPTION)
        self.assertEqual(self.machine.cpu.reg_read(reg.UC_X86_REG_CS),0x8000)
        self.assertEqual(self.machine.cpu.reg_read(reg.UC_X86_REG_IP),0x6f35)

if __name__=='__main__':unittest.main()
