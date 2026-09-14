"""Shared numeric formatting/geometry against original x86 and wsprintf model."""
import ctypes as C
from pathlib import Path
import random
import struct
import subprocess
import tempfile
import unittest
from tools.build_core import ROOT, SOURCES, FOUNDATION_SOURCES
from tools.core_machine import OriginalCore, reg
from unicorn import UC_HOOK_CODE


class CommonMachine(OriginalCore):
    SEGMENTS = dict(OriginalCore.SEGMENTS, **{})
    SEGMENTS[8] = 0xa000

    def __init__(self):
        super().__init__({'USER!#420': (0xb000, 0)})
        self.cpu.mem_map(0xb0000, 4096)
        self.cpu.mem_write(0xb0000, b'\xcb')
        self.cpu.hook_add(UC_HOOK_CODE, self._format, begin=0xb0000, end=0xb0000)

    def _format(self, cpu, address, size, unused):
        stack = cpu.reg_read(reg.UC_X86_REG_SS) * 16 + cpu.reg_read(reg.UC_X86_REG_SP)
        dst, ds, fmt, fs = struct.unpack('<4H', bytes(cpu.mem_read(stack + 4, 8)))
        text = bytes(cpu.mem_read(fs * 16 + fmt, 8)).split(b'\0')[0]
        assert text in [b'%d', b'%ld'], text
        n = struct.unpack('<h' if text == b'%d' else '<i',
                          bytes(cpu.mem_read(stack + 12, 2 if text == b'%d' else 4)))[0]
        payload = str(n).encode('ascii')
        cpu.mem_write(ds * 16 + dst, payload + b'\0')
        cpu.reg_write(reg.UC_X86_REG_AX, len(payload))


class CommonMachineTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp = tempfile.TemporaryDirectory()
        path = Path(cls.temp.name) / 'common.so'
        subprocess.run(['cc', '-std=c89', '-pedantic', '-Wall', '-Wextra', '-Werror',
                        '-O2', '-shared', '-fPIC', '-I', str(ROOT / 'reconstructed/include'),
                        *[str(ROOT / 'reconstructed/src' / n) for n in SOURCES + FOUNDATION_SOURCES],
                        '-o', str(path)], check=True)
        cls.lib = C.CDLL(str(path))
        ptr = C.POINTER(C.c_ubyte)
        for name, integer in [('int', C.c_int16), ('long', C.c_int32), ('scaled', C.c_int32)]:
            fn = getattr(cls.lib, 'stars_number_' + name)
            fn.argtypes = [ptr, integer, C.c_uint16]
            fn.restype = C.c_uint16
        cls.lib.stars_number_commas.argtypes = [ptr, C.c_uint16, C.c_int32]
        cls.lib.stars_number_commas.restype = C.c_uint16
        cls.lib.stars_bound_points.argtypes = [ptr, C.c_uint16, C.c_uint16, C.c_int16]
        cls.lib.stars_pack_long.argtypes = [C.c_uint32]
        cls.lib.stars_pack_long.restype = C.c_uint16

    @classmethod
    def tearDownClass(cls):
        cls.temp.cleanup()

    def setUp(self):
        self.machine = CommonMachine()

    def number(self, operation, value, count=0x3000, initial_count=1, poison=0xa5):
        data = bytearray(self.machine.get(0, 65536))
        data[0x590e:0x592e] = bytes([poison]) * 32
        struct.pack_into('<H', data, count, initial_count)
        self.machine.put(0, bytes(data))
        host = (C.c_ubyte * 65536).from_buffer(data)
        if operation == 'commas':
            result = self.lib.stars_number_commas(host, count, value)
            args = struct.pack('<Hi', count, value)
        else:
            result = getattr(self.lib, 'stars_number_' + operation)(host, value, count)
            args = struct.pack('<hH' if operation == 'int' else '<iH', value, count)
        original = self.machine.call({'int': 0x2274, 'long': 0x22b6, 'scaled': 0x22fe,
                                      'commas': 0x2440}[operation], args, shared_stack=True) & 65535
        self.assertEqual(result, original)
        self.assertEqual(bytes(data[:0xe000]), self.machine.get(0, 0xe000), (operation, value, count))
        return data

    def test_numeric_boundaries_and_seeded_values(self):
        values = [-2147483648, -1000001, -1000000, -999999, -10000, -1000, -999, -100,
                  -1, 0, 1, 99, 100, 999, 1000, 9999, 10000, 10499, 10500, 999499,
                  999500, 999999, 1000000, 1499999, 1500000, 999499999, 999500000,
                  2146983647, 2146983648, 2147483647]
        rng = random.Random(2700309)
        values += [rng.randrange(-2147483648, 2147483648) for _ in range(150)]
        for operation in ['long', 'scaled', 'commas']:
            for value in values:
                self.number(operation, value)
        for value in [-32768, -9999, -1, 0, 1, 9999, 32767]:
            self.number('int', value)

    def test_count_pointer_value_and_alias_quirks(self):
        for operation in ['int', 'long', 'scaled']:
            for count in [0, 0x3000, 0x590e, 0x5910]:
                for initial in [0, 1, 0xffff]:
                    self.number(operation, 123, count, initial)
        data = self.number('long', 12345, initial_count=0)
        self.assertEqual(struct.unpack_from('<H', data, 0x3000)[0], 0)
        data = self.number('scaled', 12345)
        self.assertEqual(data[0x590e:0x5913], b'12k\xa5\xa5')

    def test_bounding_rect_signed_extremes_empty_and_aliasing(self):
        rng = random.Random(2700329)
        cases = [[(-32768, -32768)], [(32767, 32767)], [(0, 0)],
                 [(-32768, 32767), (32767, -32768)], [(9, 4), (9, 4)]]
        cases += [[(rng.randrange(-32768, 32768), rng.randrange(-32768, 32768))
                   for _ in range(rng.randrange(1, 30))] for _ in range(100)]
        for points in cases:
            for rect, count in [(0x3000, len(points)), (0x3200, len(points)), (0x3202, 0), (0x3000, -1)]:
                data = bytearray(self.machine.get(0, 65536))
                data[0x3200:0x3200 + len(points) * 4] = b''.join(struct.pack('<hh', *p) for p in points)
                self.machine.put(0, bytes(data))
                host = (C.c_ubyte * 65536).from_buffer(data)
                self.lib.stars_bound_points(host, rect, 0x3200, count)
                self.machine.call(0x2f70, struct.pack('<HHh', rect, 0x3200, count))
                self.assertEqual(bytes(data), self.machine.get(0, 65536))

    def test_pack_long_exponent_boundaries_and_negative_bit_patterns(self):
        values = set(range(0, 65536, 37)) | {0xffffffff, 0x80000000, 0x7fffffff}
        for shift in range(0, 32, 2):
            values.update(v & 0xffffffff for v in [(8191 << shift), (8192 << shift), (8192 << shift) - 1])
        for value in sorted(values):
            result = self.lib.stars_pack_long(value)
            original = self.machine.call(0x4ba2, struct.pack('<I', value), segment=8) & 65535
            self.assertEqual(result, original, value)


if __name__ == '__main__':
    unittest.main()
