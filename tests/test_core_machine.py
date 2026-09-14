"""Differential checks against original x86 instructions (requires Unicorn)."""
import ctypes as C
from pathlib import Path
import random
import struct
import subprocess
import tempfile
import unittest

from tools.core_machine import OriginalCore, ROOT


class Seeds(C.Structure):
    _fields_ = [('first', C.c_uint32), ('second', C.c_uint32)]

    def pair(self):
        return self.first, self.second


class Rect(C.Structure):
    _fields_ = [(name, C.c_uint16) for name in ['left', 'top', 'right', 'bottom']]


class CoreMachineTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp = tempfile.TemporaryDirectory()
        library = Path(cls.temp.name) / 'core.so'
        subprocess.run(['cc', '-std=c89', '-pedantic', '-Wall', '-Wextra',
                        '-Werror', '-shared', '-fPIC', '-O2', '-I',
                        str(ROOT / 'reconstructed/include'),
                        str(ROOT / 'reconstructed/src/utilgen.c'),
                        str(ROOT / 'reconstructed/src/race.c'),
                        str(ROOT / 'reconstructed/src/near_state.c'), '-o', str(library)],
                       check=True)
        cls.lib = C.CDLL(str(library))
        for name in ['stars_randomize', 'stars_randomize2']:
            getattr(cls.lib, name).argtypes = [C.POINTER(Seeds), C.c_uint32]
        cls.lib.stars_random.argtypes = [C.POINTER(Seeds), C.c_int16]
        cls.lib.stars_random.restype = C.c_int16
        cls.lib.stars_next_file_xor.argtypes = [C.POINTER(Seeds)]
        cls.lib.stars_next_file_xor.restype = C.c_uint32
        cls.lib.stars_set_file_xor_stream.argtypes = [C.POINTER(Seeds), C.c_uint32,
                                                     *([C.c_int16] * 4)]
        cls.lib.stars_distance_squared.argtypes = [C.c_int16] * 4
        cls.lib.stars_distance_squared.restype = C.c_uint32
        cls.lib.stars_get_race_stat.argtypes = [C.POINTER(C.c_ubyte), C.c_uint16, C.c_int16]
        cls.lib.stars_get_race_stat.restype = C.c_int16
        cls.lib.stars_set_file_seeds.argtypes = [C.POINTER(Seeds), C.c_uint32, C.c_uint32]
        cls.lib.stars_get_file_seeds.argtypes = [C.POINTER(Seeds), C.POINTER(C.c_uint32),
                                                C.POINTER(C.c_uint32)]
        cls.lib.stars_compare_long.argtypes = [C.c_uint32, C.c_uint32]
        cls.lib.stars_compare_long.restype = C.c_int16
        for name in ['stars_expand_rect', 'stars_offset_rect']:
            getattr(cls.lib, name).argtypes = [C.POINTER(Rect), C.c_int16, C.c_int16]
        cls.lib.stars_push_random.argtypes = [C.POINTER(C.c_ubyte), C.c_uint32, C.c_uint32]
        cls.lib.stars_pop_random.argtypes = [C.POINTER(C.c_ubyte)]
        cls.lib.stars_xor_file_buffer.argtypes = [C.POINTER(C.c_ubyte), C.c_uint16, C.c_int16]

    @classmethod
    def tearDownClass(cls):
        cls.temp.cleanup()

    def setUp(self):
        self.machine = OriginalCore()

    def test_random_seed_index_space(self):
        # Exhaust both effective input spaces, including equal-index branches.
        for address, name, limit in [(0x15ea, 'stars_randomize', 4096),
                                     (0x165a, 'stars_randomize2', 16384)]:
            function = getattr(self.lib, name)
            for seed in range(limit):
                state = Seeds()
                function(C.byref(state), seed | 0xffff0000)
                self.machine.call(address, struct.pack('<I', seed | 0xffff0000))
                self.assertEqual(state.pair(), self.machine.seeds(), (name, seed))

    def test_random_and_file_xor(self):
        rng = random.Random(27003)
        edges = [0, 1, 53667, 53668, 52774, 2147483562,
                 0x7fffffff, 0x80000000, 0xffffffff]
        pairs = [(a, b) for a in edges for b in edges]
        pairs += [(rng.getrandbits(32), rng.getrandbits(32)) for _ in range(200)]
        for pair in pairs:
            for count in [-32768, -1, 0, 1, 2, 17, 2000, 32767]:
                state = Seeds(*pair)
                actual = self.lib.stars_random(C.byref(state), count)
                self.machine.seeds(value=pair)
                original = self.machine.call(0x16d2, struct.pack('<h', count)) & 0xffff
                self.assertEqual((actual, state.pair()),
                                 (original, self.machine.seeds()), (pair, count))
            state = Seeds(*pair)
            actual = self.lib.stars_next_file_xor(C.byref(state))
            self.machine.seeds(file=True, value=pair)
            original = self.machine.call(0x1b54)
            self.assertEqual((actual, state.pair()),
                             (original, self.machine.seeds(file=True)), pair)

    def test_file_stream_initialization(self):
        rng = random.Random(27003)
        cases = [(rng.getrandbits(32), salt, rng.randrange(-32768, 32768),
                  rng.randrange(-32768, 32768), 0) for salt in range(2048)]
        cases += [(3, 1024, 3, 3, extra) for extra in [-32768, -65, -64, -63, 32767]]
        for args in cases:
            state = Seeds()
            self.lib.stars_set_file_xor_stream(C.byref(state), *args)
            self.machine.call(0x1aa6, struct.pack('<Ihhhh', *args))
            self.assertEqual(state.pair(), self.machine.seeds(file=True), args)

    def test_distance_wrapping(self):
        import itertools
        for coords in itertools.product([-32768, -1, 0, 1, 32767], repeat=4):
            actual = self.lib.stars_distance_squared(*coords)
            original = self.machine.call(0x685c, struct.pack('<hhhh', *coords))
            self.assertEqual(actual, original, coords)

    def test_race_signed_byte_and_near_wrap(self):
        memory = (C.c_ubyte * 65536)(*(i & 255 for i in range(65536)))
        self.machine.put(0, bytes(memory))
        for player in [0, 0xff00, 0xffff]:
            for index in list(range(-128, 128)) + [-32768, 32767]:
                actual = self.lib.stars_get_race_stat(memory, player, index)
                original = C.c_int16(self.machine.call(
                    0x30d2, struct.pack('<Hh', player, index), segment=29) & 0xffff).value
                self.assertEqual(actual, original, (player, index))

    def test_file_seed_transfers_and_output_alias(self):
        for pair in [(0, 0), (0xffffffff, 0x80000000), (17, 41)]:
            state = Seeds()
            self.lib.stars_set_file_seeds(C.byref(state), *pair)
            self.machine.call(0x1a7c, struct.pack('<II', *pair))
            self.assertEqual(state.pair(), self.machine.seeds(file=True))
            for second in [0x2804, 0x2800]:
                first_out, second_out = C.c_uint32(), C.c_uint32()
                output = first_out if second == 0x2800 else second_out
                self.lib.stars_get_file_seeds(C.byref(state), C.byref(first_out), C.byref(output))
                self.machine.call(0x1a4e, struct.pack('<HH', 0x2800, second))
                self.assertEqual(first_out.value, struct.unpack('<I', self.machine.get(0x2800, 4))[0])
                self.assertEqual(output.value, struct.unpack('<I', self.machine.get(second, 4))[0])

    def test_long_comparison_truncates(self):
        import itertools
        for pair in itertools.product([0, 1, 32767, 32768, 65535, 65536, 0x7fffffff,
                                       0x80000000, 0xffffffff], repeat=2):
            self.machine.put(0x2800, struct.pack('<II', *pair))
            original = C.c_int16(self.machine.call(0x1d74, struct.pack('<HH', 0x2800, 0x2804))).value
            self.assertEqual(self.lib.stars_compare_long(*pair), original, pair)

    def test_rectangle_arithmetic(self):
        import itertools
        for address, name in [(0x2f0c, 'stars_expand_rect'), (0x2f3e, 'stars_offset_rect')]:
            for dx, dy in itertools.product([-32768, -1, 0, 1, 32767], repeat=2):
                rect = Rect(0, 32768, 65535, 32767)
                self.machine.put(0x2800, bytes(rect))
                getattr(self.lib, name)(C.byref(rect), dx, dy)
                self.machine.call(address, struct.pack('<Hhh', 0x2800, dx, dy))
                self.assertEqual(bytes(rect), self.machine.get(0x2800, 8), (name, dx, dy))

    def test_random_stack_aliasing_and_index_wrap(self):
        rng = random.Random(27003)
        baseline = rng.randbytes(65536)
        # Exhaust effective index residues, including writes over the index and
        # RNG globals. High index bits affect the retained counter, not address.
        for index in range(8192):
            memory = bytearray(baseline)
            struct.pack_into('<H', memory, 0x1378, index | 0xe000)
            self.machine.put(0, bytes(memory))
            host = (C.c_ubyte * 65536).from_buffer(memory)
            self.lib.stars_push_random(host, 0x12345678, 0x9abcdef0)
            self.machine.call(0x1440, struct.pack('<II', 0x12345678, 0x9abcdef0))
            self.assertEqual(bytes(memory), self.machine.get(0, 65536), ('push', index))
            self.lib.stars_pop_random(host)
            self.machine.call(0x14a2)
            self.assertEqual(bytes(memory), self.machine.get(0, 65536), ('pop', index))
        # Explicit underflow from an empty stack.
        struct.pack_into('<H', memory, 0x1378, 0)
        self.machine.put(0, bytes(memory))
        self.lib.stars_pop_random(host)
        self.machine.call(0x14a2)
        self.assertEqual(bytes(memory), self.machine.get(0, 65536))

    def test_file_buffer_xor_tails_aliasing_and_end_wrap(self):
        rng = random.Random(27003)
        baseline = rng.randbytes(65536)
        cases = [(start, count) for start in [0x2800, 0x2270, 0x2272, 0x2274, 0x2276]
                 for count in range(20)]
        cases += [(0xfffc, count) for count in range(4, 12)]
        cases += [(0x2800, count) for count in [-4, -3, -2, -1]]
        for start, count in cases:
            memory = bytearray(baseline)
            struct.pack_into('<II', memory, 0x2272, 17, 41)
            self.machine.put(0, bytes(memory))
            host = (C.c_ubyte * 65536).from_buffer(memory)
            self.lib.stars_xor_file_buffer(host, start, count)
            self.machine.call(0x1cc4, struct.pack('<Hh', start, count))
            self.assertEqual(bytes(memory), self.machine.get(0, 65536), (start, count))


if __name__ == '__main__':
    unittest.main()
