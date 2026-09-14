"""Compare integer habitability reconstruction with original x87/CRT execution."""
import ctypes as C
import itertools
import random
import struct
import unittest
from tests import test_core_machine as core_tests
from tools.core_machine import OriginalCore, reg


class HabitabilityMachineTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        core_tests.CoreMachineTests.setUpClass.__func__(cls)
        cls.lib.stars_planet_desirability.argtypes = [C.POINTER(C.c_ubyte), C.c_uint16, C.c_int16]
        cls.lib.stars_planet_desirability.restype = C.c_int16

    @classmethod
    def tearDownClass(cls): cls.temp.cleanup()

    def setUp(self):
        self.machine = OriginalCore()
        self.memory = bytearray(self.machine.get(0, 65536))

    def compare(self, centers, lows, highs, environment, index=0, planet=0x3000):
        player = (0x59a2 + index * 192) & 65535
        for offset, values in [(player + 16, centers), (player + 19, lows),
                               (player + 22, highs), (planet + 12, environment)]:
            for i, value in enumerate(values): self.memory[(offset + i) & 65535] = value & 255
        self.machine.put(0, bytes(self.memory))
        # Explicit ordinary x87 initialization; original sqrt and conversion
        # helpers execute unchanged. Private CRT scratch/stack writes are not
        # application-record writes and are excluded from record comparison.
        self.machine.cpu.reg_write(reg.UC_X86_REG_FPCW, 0x37f)
        self.machine.cpu.reg_write(reg.UC_X86_REG_FPSW, 0)
        self.machine.cpu.reg_write(reg.UC_X86_REG_FPTAG, 0xffff)
        host = (C.c_ubyte * 65536).from_buffer(self.memory)
        actual = self.lib.stars_planet_desirability(host, planet, index)
        original = C.c_int16(self.machine.call(0x6e1e, struct.pack('<HHh', planet, 0x3000, index),
                                               segment=10, shared_stack=True) & 65535).value
        self.assertEqual(actual, original, (centers, lows, highs, environment, index))
        self.assertEqual(bytes(self.memory[player:player + 192]), self.machine.get(player, 192))
        for i in range(3):
            at = (planet + 12 + i) & 65535
            self.assertEqual(self.memory[at], self.machine.get(at, 1)[0])
        return actual

    def test_center_edges_and_hostile_penalties(self):
        self.assertEqual(self.compare([50]*3, [25]*3, [75]*3, [50]*3), 100)
        self.assertEqual(self.compare([50]*3, [25]*3, [75]*3, [0]*3), -45)
        # Regression for the binary's 0.9 bias (0.5 incorrectly returns 13).
        self.assertEqual(self.compare([50]*3, [25]*3, [75]*3, [25,25,49]), 14)
        for environment in itertools.product([0, 24, 25, 26, 37, 49, 50, 51, 63, 74, 75, 76, 100], repeat=3):
            self.compare([50]*3, [25]*3, [75]*3, environment)

    def test_immunity_combinations(self):
        for immune in itertools.product([False, True], repeat=3):
            centers = [-1 if flag else 50 for flag in immune]
            lows = [-1 if flag else 25 for flag in immune]
            highs = [-1 if flag else 75 for flag in immune]
            for environment in itertools.product([0, 50, 100], repeat=3):
                self.compare(centers, lows, highs, environment)
        # Only the signed HIGH endpoint controls immunity in this routine.
        self.assertEqual(self.compare([0]*3, [0]*3, [-128]*3, [127]*3), 100)

    def test_asymmetric_ranges_random_planets_and_player_slots(self):
        rng = random.Random(27007)
        for i in range(2000):
            centers = [rng.randrange(1, 100) for _ in range(3)]
            lows = [rng.randrange(center) for center in centers]
            highs = [rng.randrange(center + 1, 101) for center in centers]
            environment = [rng.randrange(101) for _ in range(3)]
            self.compare(centers, lows, highs, environment, i % 16)

    def test_in_range_rounding_and_narrow_ranges(self):
        rng = random.Random(27007)
        for _ in range(2000):
            centers = [rng.randrange(1, 100) for _ in range(3)]
            lows = [rng.randrange(center) for center in centers]
            highs = [rng.randrange(center + 1, 101) for center in centers]
            environment = [rng.randrange(low, high + 1) for low, high in zip(lows, highs)]
            self.compare(centers, lows, highs, environment)
        for environment in itertools.product([49, 50, 51], repeat=3):
            self.compare([50]*3, [49]*3, [51]*3, environment, planet=0xfff2)

    def test_degenerate_range_traps_in_original(self):
        from unicorn import UcError, UC_ERR_EXCEPTION
        self.machine.put(0x59b2, bytes([50,50,50,25,25,25,50,75,75]))
        self.machine.put(0x300c, bytes([50,50,50]))
        # Do not invoke the host adapter outside its documented nonzero-width
        # contract. Preserve evidence that the original also divides by zero.
        with self.assertRaises(UcError) as error:
            self.machine.call(0x6e1e, struct.pack('<HHh',0x3000,0x3000,0), segment=10, shared_stack=True)
        self.assertEqual(error.exception.errno, UC_ERR_EXCEPTION)
        self.assertEqual(self.machine.cpu.reg_read(reg.UC_X86_REG_IP), 0x6f35)


if __name__ == '__main__': unittest.main()
