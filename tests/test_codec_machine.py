"""Original-instruction comparisons for the user-string codec."""
import ctypes as C
import random
import struct
import unittest
from tests import test_core_machine as core_tests
from tools.core_machine import OriginalCore


class CodecMachineTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        core_tests.CoreMachineTests.setUpClass.__func__(cls)
        bytep = C.POINTER(C.c_ubyte)
        cls.lib.stars_nybble_from_char.argtypes = [bytep, C.c_ubyte]
        cls.lib.stars_nybble_from_char.restype = C.c_uint16
        cls.lib.stars_char_from_nybble.argtypes = [bytep, C.c_int16]
        cls.lib.stars_char_from_nybble.restype = C.c_ubyte
        cls.lib.stars_compress_user_string.argtypes = [bytep, bytep, bytep, C.POINTER(C.c_int16)]
        cls.lib.stars_decompress_user_string.argtypes = [bytep, bytep, C.c_int16, bytep, C.POINTER(C.c_int16)]

    @classmethod
    def tearDownClass(cls):
        cls.temp.cleanup()

    def setUp(self):
        self.machine = OriginalCore()
        self.data = (C.c_ubyte * 65536).from_buffer_copy(self.machine.get(0, 65536))

    def test_character_codes_exhaustive(self):
        for ch in range(256):
            original = self.machine.call(0x4880, struct.pack('<H', ch)) & 0xffff
            self.assertEqual(self.lib.stars_nybble_from_char(self.data, ch), original, ch)
        for code in range(-32768, 32768):
            original = self.machine.call(0x49ea, struct.pack('<h', code)) & 255
            self.assertEqual(self.lib.stars_char_from_nybble(self.data, code), original, code)

    def test_known_encodings_and_round_trip(self):
        for text, encoded in [(b'a', b'\x1f'), (b'Stars!', bytes.fromhex('c2a189e7')),
                              (b'\xff', b'\xff\xff')]:
            incoming = (C.c_ubyte * (len(text) + 1)).from_buffer_copy(text + b'\0')
            output = (C.c_ubyte * 64)()
            capacity = C.c_int16(64)
            self.assertEqual(self.lib.stars_compress_user_string(self.data, incoming,
                                                                output, C.byref(capacity)), 1)
            self.assertEqual(bytes(output)[:capacity.value], encoded)
            restored = (C.c_ubyte * 64)()
            limit = C.c_int16(63)
            self.assertEqual(self.lib.stars_decompress_user_string(self.data, output,
                              capacity.value, restored, C.byref(limit)), 1)
            self.assertEqual(bytes(restored)[:len(text) + 1], text + b'\0')
            self.compare_compress(text, 64)
            self.compare_decompress(encoded, len(encoded), 63)

    def compare_compress(self, payload, capacity):
        incoming = (C.c_ubyte * (len(payload) + 1)).from_buffer_copy(payload + b'\0')
        output = (C.c_ubyte * 2200)(*([0xcc] * 2200))
        limit = C.c_int16(capacity)
        actual = self.lib.stars_compress_user_string(self.data, incoming, output, C.byref(limit))
        self.machine.put(0x3000, payload + b'\0')
        self.machine.put(0x5000, b'\xcc' * 2200)
        self.machine.put(0x2900, struct.pack('<h', capacity))
        original = self.machine.call(0x45a0, struct.pack('<HHHHH', 0x3000, 0x3000,
                                                        0x5000, 0x3000, 0x2900), shared_stack=True) & 0xffff
        self.assertEqual(actual, original, (len(payload), capacity))
        self.assertEqual(bytes(output), self.machine.get(0x5000, 2200))
        self.assertEqual(limit.value, struct.unpack('<h', self.machine.get(0x2900, 2))[0])

    def test_compression_categories_and_limits(self):
        rng = random.Random(27004)
        payloads = [b'', b'a', b'ab', b'Stars! 2.70j', bytes(range(1, 256))]
        payloads += [bytes(rng.randrange(1, 256) for _ in range(n)) for n in range(80)]
        payloads += [b'a' * n for n in [2045, 2046, 2047, 2048, 2049]]
        for payload in payloads:
            for capacity in [-1, 0, 1, 17, 1023, 1024, 32767]:
                self.compare_compress(payload, capacity)

    def compare_decompress(self, payload, length, capacity):
        # Deliberate accessible lookahead to observe the original's truncated
        # escape behavior; no out-of-object host reads are permitted.
        padded = payload + b'\xab\xcd\xef\x00'
        incoming = (C.c_ubyte * len(padded)).from_buffer_copy(padded)
        output = (C.c_ubyte * 2200)(*([0xcc] * 2200))
        limit = C.c_int16(capacity)
        actual = self.lib.stars_decompress_user_string(self.data, incoming, length, output, C.byref(limit))
        self.machine.put(0x3000, padded)
        self.machine.put(0x5000, b'\xcc' * 2200)
        self.machine.put(0x2900, struct.pack('<h', capacity))
        original = self.machine.call(0x46e8, struct.pack('<HHhHHH', 0x3000, 0x3000,
                                      length, 0x5000, 0x3000, 0x2900), shared_stack=True) & 0xffff
        self.assertEqual(actual, original, (payload.hex(), length, capacity))
        self.assertEqual(bytes(output), self.machine.get(0x5000, 2200))
        self.assertEqual(limit.value, struct.unpack('<h', self.machine.get(0x2900, 2))[0])

    def test_decompression_padding_truncation_and_capacity(self):
        for byte in range(256):
            for capacity in [-1, 0, 1, 2, 3, 1023]:
                self.compare_decompress(bytes([byte]), 1, capacity)
        for length in [-32768, -1, 0]:
            self.compare_decompress(b'\xff', length, -1)
        rng = random.Random(27004)
        for n in range(100):
            payload = rng.randbytes(n)
            for capacity in [0, 17, 1023]:
                self.compare_decompress(payload, n, capacity)
        # 1023 decoded characters fit with their terminator; capacity does not
        # include that terminator. Avoid original stack overflow at 1024.
        self.compare_decompress(b'\x11' * 511 + b'\x1f', 512, 1023)


if __name__ == '__main__':
    unittest.main()
