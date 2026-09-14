"""Complete memory.obj: stateful differential tests with nonzero heap images."""
import ctypes as C
from pathlib import Path
import random
import struct
import subprocess
import tempfile
import unittest
from tools.build_core import ROOT, SOURCES, FOUNDATION_SOURCES
from tools.memory_machine import OriginalMemory, HeapServices, MemoryAbort
from tests.test_race_runtime import Runtime, Platform, WindowCall

PTR = C.POINTER(C.c_ubyte)
HEAP_TYPES = [('resolve', C.c_void_p, C.c_uint16),
              ('alloc', C.c_uint16, C.c_uint16, C.c_uint32),
              ('realloc', C.c_uint16, C.c_uint16, C.c_uint32, C.c_uint16),
              ('lock', C.c_uint32, C.c_uint16),
              ('unlock', C.c_uint16, C.c_uint16), ('free', C.c_uint16, C.c_uint16)]


class HeapApi(C.Structure):
    _fields_ = [(n, C.c_void_p) for n, *_ in HEAP_TYPES]


class Memory(C.Structure):
    _fields_ = [('runtime', C.POINTER(Runtime)), ('api', C.POINTER(HeapApi)), ('context', C.c_void_p)]


class MemoryMachineTests(unittest.TestCase):
    visited = set()
    comparisons = 0

    @classmethod
    def setUpClass(cls):
        cls.temp = tempfile.TemporaryDirectory()
        path = Path(cls.temp.name) / 'memory.so'
        subprocess.run(['cc', '-std=c89', '-pedantic', '-Wall', '-Wextra', '-Werror',
                        '-O2', '-shared', '-fPIC', '-I', str(ROOT / 'reconstructed/include'),
                        *[str(ROOT / 'reconstructed/src' / n) for n in SOURCES + FOUNDATION_SOURCES],
                        str(ROOT / 'tests/memory_abort_bridge.c'), '-o', str(path)], check=True)
        cls.lib = C.CDLL(str(path))
        cls.lib.stars_runtime_init.argtypes = [C.POINTER(Runtime), PTR, PTR, C.POINTER(Platform),
                                              C.c_void_p, C.c_uint16, C.c_uint16]
        cls.lib.memory_test_run.argtypes = [C.POINTER(Memory), C.c_int, C.c_uint32, C.c_uint16, C.c_int16]

    @classmethod
    def tearDownClass(cls):
        cls.temp.cleanup()

    def setUp(self):
        self.reset()

    def reset(self, base=32, small=False):
        self.machine = OriginalMemory(base)
        self.machine.put(0xd34, bytes(48))
        self.machine.put(0x6e, struct.pack('<H', 0x2800))
        self.machine.put(0x229a, b'\0\0')
        if small:
            self.machine.put(0xd64, struct.pack('<12H', *([128] * 12)))
        self.data = (C.c_ubyte * 65536).from_buffer_copy(self.machine.get(0, 65536))
        self.strings = (C.c_ubyte * 65536).from_buffer_copy(self.machine.read_heap(0xd000, 0, 65536))
        self.images = {s: (C.c_ubyte * 65536)() for s in HeapServices.SELECTORS}
        self.heap = HeapServices(self.read, self.write, base)
        self.errors = []
        self.callbacks = []
        self.api = HeapApi()
        self.platform = Platform()
        for name, result, *args in HEAP_TYPES:
            fn = self.resolve if name == 'resolve' else getattr(self.heap, name)
            def checked(_, *values, fn=fn):
                try:
                    return fn(*values)
                except BaseException as e:
                    self.errors.append(e)
                    return 0
            callback = C.CFUNCTYPE(result, C.c_void_p, *args)(checked)
            self.callbacks.append(callback)
            setattr(self.api, name, C.cast(callback, C.c_void_p).value)
        def window(_, p):
            c = p.contents
            if c.operation == b'GetFocus':
                self.heap.events.append(('focus', 0x1234))
                return 0x1234
            if c.operation == b'MessageBox':
                self.heap.events.append(('message', c.window, C.string_at(c.p), C.string_at(c.q), c.a))
                return 1
            self.errors.append(AssertionError(c.operation))
            return 0
        callback = C.CFUNCTYPE(C.c_uint32, C.c_void_p, C.POINTER(WindowCall))(window)
        self.callbacks.append(callback)
        self.platform.window = C.cast(callback, C.c_void_p).value
        self.runtime = Runtime()
        self.lib.stars_runtime_init(C.byref(self.runtime), self.data, self.strings,
                                    C.byref(self.platform), None, 0xe000, 0xff00)
        self.memory = Memory(C.pointer(self.runtime), C.pointer(self.api), None)

    def read(self, selector, offset, count):
        return bytes(self.images[selector][offset:offset + count])

    def write(self, selector, offset, value):
        self.images[selector][offset:offset + len(value)] = value

    def resolve(self, selector):
        return C.addressof(self.images[selector])

    def configure(self, **values):
        for key, value in values.items():
            setattr(self.heap, key, value)
            setattr(self.machine.heap, key, value)

    def put(self, p, value):
        self.write(p >> 16, p & 65535, value)
        self.machine.write_heap(p >> 16, p & 65535, value)

    def word(self, p, off=0):
        return struct.unpack('<H', self.read(p >> 16, (p + off) & 65535, 2))[0]

    def run_op(self, op, p=0, a=0, b=0, abort=False):
        self.heap.events.clear()
        self.machine.events.clear()
        caught = self.lib.memory_test_run(C.byref(self.memory), op, p, a, b)
        result = C.c_uint32.in_dll(self.lib, 'memory_test_result').value
        if caught:
            self.heap.events.append(('abort', C.c_uint16.in_dll(self.lib, 'memory_test_environment').value,
                                     C.c_int16.in_dll(self.lib, 'memory_test_value').value))
        args = {0: lambda: struct.pack('<Hh', a, b), 1: lambda: struct.pack('<I', p),
                2: lambda: struct.pack('<I', p), 3: lambda: struct.pack('<h', b),
                4: lambda: struct.pack('<Hh', a, b), 5: lambda: struct.pack('<Ih', p, b),
                6: lambda: struct.pack('<IHh', p, a, b), 7: lambda: struct.pack('<Ih', p, b),
                8: lambda: struct.pack('<IH', p, a), 9: lambda: struct.pack('<HHh', p, a, b),
                10: lambda: struct.pack('<I', p)}[op]()
        original_caught = False
        try:
            original = self.machine.call([0, 0x108, 0x2d8, 0x348, 0x3b2, 0x58c, 0x660,
                                          0x7a8, 0x836, 0x88c, 0x918][op], args, segment=13,
                                         shared_stack=True)
        except MemoryAbort:
            original_caught = True
            original = None
        self.visited.update(self.machine.visited)
        type(self).comparisons += 1
        self.assertFalse(self.errors)
        self.assertEqual(bool(caught), original_caught)
        self.assertEqual(bool(caught), abort)
        if op not in [2, 3, 7, 10] and not caught:
            self.assertEqual(result, original, (op, p, a, b))
        self.assertEqual(self.heap.events, self.machine.events)
        self.assertEqual(self.heap.blocks, self.machine.heap.blocks)
        # Exclude only the documented scratch/stack arena, not any game globals.
        self.assertEqual(bytes(self.data[:0xe000]), self.machine.get(0, 0xe000))
        self.assertEqual(self.heap.touched, self.machine.heap.touched)
        for selector in self.heap.touched:
            self.assertEqual(self.read(selector, 0, 65536), self.machine.read_heap(selector, 0, 65536),
                             (op, hex(selector)))
        return result

    def test_original_heap_sizes_headers_and_strict_owner_bounds(self):
        for ht in range(12):
            for base in [0, 32]:
                self.reset(base)
                p = self.run_op(0, a=0, b=ht)
                for q in [0, p, p + 1, p + 15, p + 16, p + self.word(p, 2) - 1,
                          p + self.word(p, 2), p - 1, p ^ 0x10000]:
                    self.run_op(5, p=q, b=ht)
                for invalid in [-32768, -1, 12, 32767]:
                    self.run_op(5, p=p + 18, b=invalid)
                self.run_op(3, b=ht)
                self.run_op(2, p=p)
        for cb in [1, 15, 4095, 4096, 65000, 65500, 65519, 65520, 65535]:
            self.reset(base=0)
            self.run_op(0, a=cb, b=1)

    def test_nulls_empty_reset_and_wrapping_request(self):
        for op in [1, 2, 7, 10]:
            self.run_op(op)
        for ht in range(12):
            self.run_op(3, b=ht)
        for count in [0, 1, 2, 3, 65533, 65534, 65535]:
            self.reset(small=True)
            self.run_op(4, a=count, b=1)

    def test_zero_size_allocation_at_block_end_has_no_owner(self):
        self.reset(small=True)
        self.run_op(4, a=108, b=1)
        p = self.run_op(4, a=0, b=1)
        self.assertEqual(p & 65535, 160)
        self.assertEqual(self.run_op(5, p=p, b=1), 0)
        # The original FreeLp would dereference a null owner. Do not manufacture
        # a valid pointer for it or use this case in valid-lifetime sequences.

    def test_fragmentation_coalescing_and_whole_hole_consumption(self):
        self.reset(small=True)
        pointers = [self.run_op(4, a=n, b=1) for n in [10, 14, 30, 48]]
        # 10 + 14 coalesce to 26 payload bytes, all consumed for a 20-byte request.
        for p in pointers[:2]:
            self.run_op(7, p=p, b=1)
        p = self.run_op(4, a=20, b=1)
        self.assertEqual(p, pointers[0])
        self.assertEqual(self.word(p, -2), 26)
        # Free tail does not merge the preceding hole until a later scan/reset.
        self.run_op(7, p=pointers[2], b=1)
        self.run_op(7, p=pointers[3], b=1)
        self.run_op(4, a=46, b=1)
        self.run_op(3, b=1)
        self.run_op(4, a=100, b=1)

    def test_fragmentation_exhausts_scan_then_uses_another_block(self):
        self.reset(small=True)
        pointers = [self.run_op(4, a=20, b=1) for _ in range(5)]
        for i in [0, 2, 4]:
            self.run_op(7, p=pointers[i], b=1)
        self.run_op(4, a=40, b=1)
        # A too-small coalesced hole is still rewritten before the next block.
        self.run_op(4, a=80, b=1)
        self.run_op(3, b=1)
        head = struct.unpack_from('<I', bytes(self.data), 0xd38)[0]
        self.run_op(2, p=head)

    def test_realloc_same_tail_copy_shrink_and_payload(self):
        self.reset(small=True)
        p = self.run_op(4, a=12, b=1)
        self.put(p, b'HELLO WORLD!')
        self.assertEqual(self.run_op(6, p=p, a=11, b=1), p)
        self.assertEqual(self.run_op(6, p=p, a=20, b=1), p)
        q = self.run_op(4, a=12, b=1)
        self.put(q, b'next object!')
        r = self.run_op(6, p=p, a=60, b=1)
        self.assertNotEqual(r, p)
        self.assertEqual(self.read(r >> 16, r & 65535, 12), b'HELLO WORLD!')
        self.run_op(6, p=q, a=65535, b=1)  # rounded size wraps to zero; no shrink

    def test_special_heaps_repeat_growth_and_relocation(self):
        for ht in [3, 10]:
            for move in [False, True]:
                self.reset(small=True)
                self.configure(move=move, change_handle=move)
                p = self.run_op(4, a=100, b=ht)
                self.put(p, b'original payload')
                q = self.run_op(6, p=p, a=700, b=ht)
                self.assertEqual(self.read(q >> 16, q & 65535, 16), b'original payload')
                self.run_op(7, p=q, b=ht)

    def test_heap_relocation_head_middle_tail_and_limit(self):
        for index in [0, 1, 2]:
            self.reset(small=True)
            pointers = [self.run_op(0, a=80, b=1) for _ in range(3)]
            self.configure(move=True, change_handle=True)
            self.run_op(1, p=pointers[index])
            self.run_op(3, b=1)
            head = struct.unpack_from('<I', bytes(self.data), 0xd38)[0]
            self.run_op(2, p=head)
        self.reset()
        p = self.run_op(0, a=65000, b=1)
        p = self.run_op(1, p=p)
        self.assertEqual(self.word(p, 2), 0xffdc)
        self.run_op(1, p=p, abort=True)

    def test_global_allocation_failure_and_reallocation_failure(self):
        self.configure(fail_alloc=True)
        self.run_op(4, a=123, b=1, abort=True)
        self.assertIn(('abort', 0x2800, -1), self.heap.events)
        self.reset(small=True)
        p = self.run_op(4, a=100, b=3)
        self.configure(fail_realloc=True)
        self.run_op(6, p=p, a=200, b=3, abort=True)
        self.assertFalse(any(e[0] == 'GlobalLock' for e in self.heap.events))

    def test_lists_packed_fields_growth_truncation_and_reuse(self):
        for ht in [0, 1, 3, 7]:
            self.reset(small=True)
            p = self.run_op(9, p=7, a=10, b=ht)
            self.put(p + 3, b'\x06')  # retain iMac across movement
            self.put(p + 4, b'items!')
            p = self.run_op(8, p=p, a=20)
            self.run_op(10, p=p)
            self.run_op(9, p=0x107, a=0, b=ht)
        for item, count in [(0, 256), (1, 257), (256, 257), (65535, 0), (2, 32768)]:
            self.reset()
            self.run_op(9, p=item, a=count, b=1)

    def test_deterministic_allocation_lifetimes(self):
        rng = random.Random(2700313)
        for sequence in range(12):
            self.reset(small=True)
            live = []
            for step in range(80):
                if live and (len(live) > 8 or rng.randrange(3) == 0):
                    pointer = live.pop(rng.randrange(len(live)))
                    owner = self.run_op(5, p=pointer, b=1)
                    self.assertNotEqual(owner, 0, (sequence, step, hex(pointer)))
                    self.run_op(7, p=pointer, b=1)
                else:
                    live.append(self.run_op(4, a=rng.randrange(1, 36), b=1))
            self.run_op(3, b=1)
            head = struct.unpack_from('<I', bytes(self.data), 0xd38)[0]
            self.run_op(2, p=head)


if __name__ == '__main__':
    unittest.main()
