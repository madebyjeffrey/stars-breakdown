"""Differential validation of game-creation victory-setting dependencies."""
import ctypes as C
import struct
import unittest
from tests import test_core_machine as core_tests
from tools.core_machine import OriginalCore


class CreateMachineTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        core_tests.CoreMachineTests.setUpClass.__func__(cls)
        for name, count in [('stars_set_victory_check', 2), ('stars_get_victory_check', 1),
                            ('stars_get_victory_value', 2), ('stars_set_victory_value', 2)]:
            function = getattr(cls.lib, name)
            function.argtypes = [C.POINTER(C.c_ubyte), C.c_uint16] + [C.c_int16] * count
            function.restype = C.c_int16
        cls.lib.stars_init_battle_plan.argtypes = [C.POINTER(C.c_ubyte), C.c_uint16, C.c_int16, C.c_int16]
        cls.lib.stars_computer_player_pointer.argtypes = [C.c_uint16, C.c_int16, C.c_int16]
        cls.lib.stars_computer_player_pointer.restype = C.c_uint32
        cls.lib.stars_init_new_game3.argtypes = []

    @classmethod
    def tearDownClass(cls):
        cls.temp.cleanup()

    def setUp(self):
        self.machine = OriginalCore()

    def compare(self, memory, name, address, game, index, extra=None, returns=True):
        self.machine.put(0, bytes(memory))
        host = (C.c_ubyte * 65536).from_buffer(memory)
        args = (game, index) if extra is None else (game, index, extra)
        packed = struct.pack('<Hh' + ('' if extra is None else 'h'), *args)
        actual = getattr(self.lib, name)(host, *args)
        original = C.c_int16(self.machine.call(address, packed, segment=16) & 0xffff).value
        if returns: self.assertEqual(actual, original, (name, args))
        self.assertEqual(bytes(memory), self.machine.get(0, 65536), (name, args))

    def test_check_bits_all_bytes_and_nonboolean_inputs(self):
        for byte in range(256):
            for index in [0, 8, 9, -32768, 32767]:
                memory = bytearray(65536)
                at = (0xfff0 + 20 + index) & 0xffff
                memory[at] = byte
                self.compare(memory, 'stars_get_victory_check', 0xb60c, 0xfff0, index)
                for enabled in [-32768, -1, 0, 1, 32767]:
                    memory[at] = byte
                    self.compare(memory, 'stars_set_victory_check', 0xb5bc, 0xfff0, index, enabled, False)

    def test_value_conversion_all_bytes_and_dispatch(self):
        for index in list(range(10)) + [-1, 10, 32767]:
            for byte in range(256):
                for raw in [0, 1, -1]:
                    memory = bytearray(65536)
                    memory[0x84:0x8c] = b'\x80' * 8
                    memory[(0x3000 + 20 + index) & 0xffff] = byte
                    self.compare(memory, 'stars_get_victory_value', 0xb710, 0x3000, index, raw)

    def test_set_value_clamping_and_flag_preservation(self):
        limits = [16, 18, 4, 19, 28, 49, 29, 87, 7, 47]
        for index, limit in enumerate(limits):
            for value in [-32768, -1, 0, 1, limit - 1, limit, limit + 1, 127, 255, 32767]:
                for byte in [0, 127, 128, 255]:
                    memory = bytearray(65536)
                    memory[0x84:0x8c] = b'\x80' * 8
                    memory[0x3014 + index] = byte
                    self.compare(memory, 'stars_set_victory_value', 0xb644, 0x3000, index, value)

    def test_condition_count_uses_global_game_and_omits_field_two(self):
        for flags in range(256):
            for game in [0x70, 0x3000]:
                memory = bytearray(65536)
                memory[0x3014:0x301c] = b'\x80' * 8
                for i in range(8): memory[0x84 + i] = 128 if flags & (1 << i) else 0
                memory[game + 28] = 255
                self.compare(memory, 'stars_get_victory_value', 0xb710, game, 8, 0)
                self.compare(memory, 'stars_set_victory_value', 0xb644, game, 8, 7)
                self.assertEqual(memory[game + 28] & 127, (flags & ~4).bit_count())

    def test_battle_plan_templates_ids_and_global_aliasing(self):
        for template in range(5):
            for plan_id in [-32768, -1, 0, 1, 15, 16, 32767]:
                for flags in [0, 4, 255]:
                    for output in [0x3000, 0x70, 0x80, 0xfff0]:
                        memory = bytearray(65536)
                        memory[0x80] = flags
                        self.machine.put(0, bytes(memory))
                        self.lib.stars_init_battle_plan((C.c_ubyte * 65536).from_buffer(memory),
                                                       output, template, plan_id)
                        self.machine.call(0xc0, struct.pack('<HHhh', output, 0x3000, template, plan_id), segment=16)
                        self.assertEqual(bytes(memory), self.machine.get(0, 65536), (template, plan_id, flags, output))

    def test_computer_player_pointer_offset_wrap(self):
        for race in list(range(-1, 8)) + [-32768, 32767]:
            for level in list(range(-1, 6)) + [-32768, 32767]:
                actual = self.lib.stars_computer_player_pointer(0x7000, race, level)
                original = self.machine.call(0xb570, struct.pack('<hh', race, level), segment=16)
                self.assertEqual(actual, original, (race, level))

    def test_original_empty_initializer(self):
        before = self.machine.get(0, 65536)
        self.lib.stars_init_new_game3()
        self.machine.call(0x74f8, segment=16)
        self.assertEqual(before, self.machine.get(0, 65536))


if __name__ == '__main__':
    unittest.main()
