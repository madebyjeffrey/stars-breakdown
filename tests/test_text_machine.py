"""Compare text utility side effects and returns against original instructions."""
import ctypes as C
import random
import struct
import unittest
from tests import test_core_machine as core_tests
from tools.core_machine import OriginalCore


class TextMachineTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        core_tests.CoreMachineTests.setUpClass.__func__(cls)
        bytep = C.POINTER(C.c_ubyte)
        for name in ['stars_add_back_trailing_spaces', 'stars_chop_trailing_spaces',
                     'stars_chop_last_word']:
            getattr(cls.lib, name).argtypes = [bytep, C.c_uint16, C.c_uint16]
        cls.lib.stars_int_to_roman.argtypes = [bytep, C.c_int16, C.c_uint16]
        cls.lib.stars_parse_numbers.argtypes = [bytep, C.c_uint16, C.c_uint16, C.c_int16]
        cls.lib.stars_parse_numbers.restype = C.c_int16
        cls.lib.stars_get_line.argtypes = [bytep, C.c_uint16]
        cls.lib.stars_get_line.restype = C.c_uint32

    @classmethod
    def tearDownClass(cls):
        cls.temp.cleanup()

    def setUp(self):
        self.machine = OriginalCore()

    def compare(self, memory, name, args, address, packed, result=False):
        self.machine.put(0, bytes(memory))
        host = (C.c_ubyte * 65536).from_buffer(memory)
        actual = getattr(self.lib, name)(host, *args)
        original = self.machine.call(address, packed)
        if result == 'word': original = C.c_int16(original & 0xffff).value
        if result: self.assertEqual(actual, original, (name, args))
        self.assertEqual(bytes(memory), self.machine.get(0, 65536), (name, args))

    def test_space_pointer_operations(self):
        rng = random.Random(27005)
        texts = [b'', b' ', b'abc', b'abc   def  ', b'\t \n', b'\xff \0 ']
        texts += [bytes(rng.choice(b' ab\t\0\xff') for _ in range(rng.randrange(40)))
                  for _ in range(400)]
        for text in texts:
            for name, address, forward in [('stars_add_back_trailing_spaces', 0x280c, True),
                                          ('stars_chop_trailing_spaces', 0x28c6, False),
                                          ('stars_chop_last_word', 0x2842, False)]:
                memory = bytearray(65536)
                memory[0x3000:0x3000 + len(text)] = text
                start, end, slot = 0x3000, 0x3000 + len(text), 0x2800
                struct.pack_into('<H', memory, slot, start if forward else end)
                args = (slot, end) if forward else (start, slot)
                self.compare(memory, name, args, address, struct.pack('<HH', *args))

    def test_space_pointer_alias_and_unsigned_bounds(self):
        for cursor in [0, 0x2020, 0x2021, 0x2022, 0xfffe]:
            for bound in [0, 0x201f, 0x2026, 0xffff]:
                for name, address, forward in [('stars_add_back_trailing_spaces', 0x280c, True),
                                              ('stars_chop_trailing_spaces', 0x28c6, False),
                                              ('stars_chop_last_word', 0x2842, False)]:
                    memory = bytearray(b'a' * 65536)
                    memory[0x201f:0x2027] = b' ' * 8
                    struct.pack_into('<H', memory, 0x2020, cursor)
                    args = (0x2020, bound) if forward else (bound, 0x2020)
                    self.compare(memory, name, args, address, struct.pack('<HH', *args))

    def test_roman_nonpositive_large_and_offset_wrap(self):
        for value in list(range(-10, 201)) + [-32768, 32767, 400, 999, 1000]:
            for output in [0x3000, 0xfffc]:
                self.compare(bytearray(b'?' * 65536), 'stars_int_to_roman', (value, output),
                             0x5830, struct.pack('<hH', value, output))

    def test_parse_numbers_limits_invalid_and_overflow(self):
        texts = [b'', b' ', b'0', b'123 456 789', b'1 2x 3', b'+1', b'-1',
                 b'1\t2', b'1\xff2', b'4294967295 4294967296 4294967297', b'9' * 1000,
                 b'  17    20   ', b'1 \xff', b'1 2 3 4 5 6 7']
        rng = random.Random(27005)
        texts += [bytes(rng.choice(b'0123456789  x\t') for _ in range(80)) for _ in range(100)]
        for text in texts:
            for maximum in [-32768, -1, 0, 1, 2, 5, 32767]:
                memory = bytearray(b'\xcc' * 65536)
                memory[0x3000:0x3000 + len(text) + 1] = text + b'\0'
                self.compare(memory, 'stars_parse_numbers', (0x3000, 0x4000, maximum),
                             0x6986, struct.pack('<HHHh', 0x3000, 0x3000, 0x4000, maximum), 'word')

    def test_parse_numbers_input_output_alias_and_wrap(self):
        for start in [0x3000, 0xfff8]:
            for output in [start, (start + 4) & 0xffff, 0x2800, 0xfffc]:
                memory = bytearray(65536)
                for i, ch in enumerate(b'12 34 56 78\0'):
                    memory[(start + i) & 0xffff] = ch
                self.compare(memory, 'stars_parse_numbers', (start, output, 8), 0x6986,
                             struct.pack('<HHHh', start, 0x3000, output, 8), 'word')

    def test_get_line_endings_leading_spaces_and_wrap(self):
        for text in [b'', b' ', b'  abc\r\ndef', b'abc\ndef', b'abc\rdef',
                     b'\tabc\n', b'\r\n', b'abc\0def', b'  \xff  \n']:
            for start in [0x3000, 0xfffd]:
                memory = bytearray(65536)
                for i, ch in enumerate(text + b'\0'):
                    memory[(start + i) & 0xffff] = ch
                struct.pack_into('<HH', memory, 0x2800, start, 0x3000)
                self.compare(memory, 'stars_get_line', (0x2800,), 0x68ba,
                             struct.pack('<H', 0x2800), 'long')

    def test_get_line_pointer_slot_alias(self):
        for slot, start in [(0x3000, 0x3000), (0x2020, 0x2020), (0x3000, 0x2fff)]:
            memory = bytearray(65536)
            memory[0x2fff] = ord('a')
            struct.pack_into('<HH', memory, slot, start, 0x3000)
            self.compare(memory, 'stars_get_line', (slot,), 0x68ba,
                         struct.pack('<H', slot), 'long')


if __name__ == '__main__':
    unittest.main()
