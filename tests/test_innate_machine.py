"""Synthetic player records compared with unchanged Win16/x87 execution."""
import ctypes as C
import itertools
import random
import struct
import unittest
from tests import test_core_machine as core_tests
from tools.core_machine import OriginalCore, reg


class InnateMachineTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        core_tests.CoreMachineTests.setUpClass.__func__(cls)
        cls.lib.stars_innate_race_habitability.argtypes = [C.POINTER(C.c_ubyte), C.c_uint16]
        cls.lib.stars_innate_race_habitability.restype = C.c_int32

    @classmethod
    def tearDownClass(cls): cls.temp.cleanup()

    def setUp(self): self.machine = OriginalCore()

    def compare(self, centers, lows, highs, trait=False, player=0x3000):
        memory = bytearray(self.machine.get(0, 65536))
        memory[0x59a2:0x5a62] = bytes((i * 17 + 3) & 255 for i in range(192))
        memory[player:player+192] = bytes((i * 11 + 7) & 255 for i in range(192))
        for offset, values in [(16, centers), (19, lows), (22, highs)]:
            memory[player+offset:player+offset+3] = bytes(v & 255 for v in values)
        memory[player+78:player+82] = struct.pack('<I', 2 if trait else 0)
        memory[player+84] = 0x81
        before = bytes(memory[0x59a2:0x5a62])
        self.machine.put(0, bytes(memory))
        self.machine.cpu.reg_write(reg.UC_X86_REG_FPCW, 0x37f)
        self.machine.cpu.reg_write(reg.UC_X86_REG_FPSW, 0)
        self.machine.cpu.reg_write(reg.UC_X86_REG_FPTAG, 0xffff)
        host = (C.c_ubyte * 65536).from_buffer(memory)
        actual = self.lib.stars_innate_race_habitability(host, player)
        original = C.c_int32(self.machine.call(0x4c6e, struct.pack('<H', player),
                                              segment=29, shared_stack=True)).value
        self.assertEqual(actual, original, (centers, lows, highs, trait, player))
        self.assertEqual(bytes(memory[player:player+192]), self.machine.get(player, 192))
        self.assertEqual(bytes(memory[0x59a2:0x5a62]), self.machine.get(0x59a2, 192))
        if player != 0x59a2: self.assertEqual(bytes(memory[0x59a2:0x5a62]), before)
        return actual, bytes(memory[player:player+192])

    def test_immunity_and_trait_combinations(self):
        for immune in itertools.product([False, True], repeat=3):
            for trait in [False, True]:
                c = [-1 if flag else 50 for flag in immune]
                lo = [-1 if flag else 25 for flag in immune]
                hi = [-1 if flag else 75 for flag in immune]
                result, _ = self.compare(c, lo, hi, trait)
                if all(immune): self.assertEqual(result, 23958000)

    def test_symmetric_narrow_and_clipped_ranges(self):
        for center, width in [(50,1),(50,25),(50,50),(1,1),(99,1),(15,15),(85,15)]:
            for trait in [False, True]:
                self.compare([center]*3, [center-width]*3, [center+width]*3, trait)

    def test_asymmetric_ranges(self):
        rng = random.Random(270074)
        for i in range(120):
            c = [rng.randrange(1,100) for _ in range(3)]
            lo = [rng.randrange(v) for v in c]
            hi = [rng.randrange(v+1,101) for v in c]
            self.compare(c, lo, hi, bool(i % 2), 0x59a2 if i % 7 == 0 else 0x3000)

    def test_rounding_boundary_candidates(self):
        # Selected by a deterministic 300,000-record host scan for totals near
        # 10*n+5; checked against the original, not an algebraic substitute.
        self.compare([34, 93, 40], [22, 48, 7], [76, 95, 42], True)
        self.compare([31, 81, 93], [0, 18, 20], [40, 90, 96], False)
        self.compare([18, 75, 37], [8, 59, 32], [67, 84, 75], False)
        # Native binary64 accumulation incorrectly returned 1,043,305.
        self.assertEqual(self.compare([10, 20, 15], [4, 9, 6], [22, 25, 88], True)[0], 1043306)
        self.compare([32, 56, 27], [29, 22, 22], [69, 67, 57], False)
        self.compare([94, 23, 74], [88, 10, 2], [98, 71, 89], True)
        self.compare([88, 62, 34], [22, 2, 12], [97, 98, 98], True)
        self.compare([37, 29, 19], [6, 25, 9], [58, 67, 73], False)
        self.compare([28, 66, 83], [19, 21, 2], [75, 71, 98], False)

    def test_invalid_triples_repair_and_restore(self):
        for player in [0x3000, 0x59a2]:
            for invalid in [-128, -2, -1, 101, 127]:
                for field in range(3):
                    triples = [[50]*3, [25]*3, [75]*3]
                    triples[field][1] = invalid
                    _, record = self.compare(*triples, player=player)
                    self.assertEqual([record[17],record[20],record[23]], [255]*3)
                    self.assertEqual(record[84], 0x91)


if __name__ == '__main__': unittest.main()
