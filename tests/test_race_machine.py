"""Race access, bounds repair and checksum comparisons with original x86."""
import ctypes as C
import random
import struct
import unittest
from tests import test_core_machine as core_tests
from tools.core_machine import OriginalCore


class RaceMachineTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        core_tests.CoreMachineTests.setUpClass.__func__(cls)
        for name, count in [('stars_set_race_stat', 2), ('stars_get_race_trait', 1),
                            ('stars_set_race_trait', 2), ('stars_race_checksum', 0),
                            ('stars_bounds_check_player', 0)]:
            fn = getattr(cls.lib, name)
            fn.argtypes = [C.POINTER(C.c_ubyte), C.c_uint16] + [C.c_int16] * count
            fn.restype = C.c_uint16

    @classmethod
    def tearDownClass(cls): cls.temp.cleanup()

    def setUp(self): self.machine = OriginalCore()

    def compare(self, memory, name, address, player, args=(), returns=True):
        self.machine.put(0, bytes(memory))
        actual = getattr(self.lib, name)((C.c_ubyte * 65536).from_buffer(memory), player, *args)
        original = self.machine.call(address, struct.pack('<H' + 'h' * len(args), player, *args), segment=29) & 65535
        if returns: self.assertEqual(actual, original, (name, player, args))
        self.assertEqual(bytes(memory), self.machine.get(0, 65536), (name, player, args))

    def test_stat_setter_all_byte_values_and_signed_extremes(self):
        for index in range(16):
            for value in list(range(-128, 256)) + [-32768, 32767]:
                self.compare(bytearray(65536), 'stars_set_race_stat', 0x3114, 0xffd0, (index, value))

    def test_trait_getter_all_shift_counts(self):
        memory = bytearray(65536)
        struct.pack_into('<I', memory, 0x304e, 0xa5a5a5a5)
        self.machine.put(0, bytes(memory))
        host = (C.c_ubyte * 65536).from_buffer(memory)
        for index in range(-32768, 32768):
            actual = self.lib.stars_get_race_trait(host, 0x3000, index)
            original = self.machine.call(0x3176, struct.pack('<Hh', 0x3000, index), segment=29) & 65535
            self.assertEqual(actual, original, index)

    def test_trait_setter_preserves_other_bits(self):
        for flags in [0, 0xffffffff, 0xa5a5a5a5]:
            for index in list(range(260)) + [-32768, -256, -1, 32767]:
                for enabled in [0, 1, -1]:
                    memory = bytearray(65536)
                    struct.pack_into('<I', memory, 0x304e, flags)
                    self.compare(memory, 'stars_set_race_trait', 0x31b8, 0x3000, (index, enabled), False)

    def test_checksum_full_record_and_wrap(self):
        rng = random.Random(27006)
        for player in [0x3000, 0xff80]:
            for i in range(192):
                memory = bytearray(65536)
                memory[(player + i) & 65535] = 1 << (i % 8)
                self.compare(memory, 'stars_race_checksum', 0x5888, player)
            self.compare(bytearray(rng.randbytes(65536)), 'stars_race_checksum', 0x5888, player)

    def test_bounds_repair_random_records_and_near_wrap(self):
        rng = random.Random(27006)
        records = [bytes([value]) * 192 for value in range(256)]
        records += [rng.randbytes(192) for _ in range(1000)]
        for i, record in enumerate(records):
            player = 0x3000 if i % 2 else 0xff80
            memory = bytearray(65536)
            for offset, byte in enumerate(record): memory[(player + offset) & 65535] = byte
            self.compare(memory, 'stars_bounds_check_player', 0x40ae, player, returns=False)

    def test_bounds_repair_immunity_and_endpoint_edges(self):
        import itertools
        for low, high, center in itertools.product([0, 1, 99, 100, 101, 128, 254, 255], repeat=3):
            memory = bytearray(65536)
            memory[0x3010], memory[0x3013], memory[0x3016] = center, low, high
            self.compare(memory, 'stars_bounds_check_player', 0x40ae, 0x3000, returns=False)


if __name__ == '__main__': unittest.main()
