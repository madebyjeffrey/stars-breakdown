"""Creation/refresh/terminal cleanup compared with original startup code."""
import ctypes as C
import json
import random
import struct
import unittest
from tools.build_core import ROOT
from tools.startup_machine import StartupMachine
from tests import test_common_machine as common_tests
from tests.test_race_ui import UiHost
from tests.test_race_runtime import Runtime, Platform, PTR
from tests.test_memory_machine import Memory, HeapApi


class StartupHost(UiHost):
    def __init__(self, lib, machine):
        super().__init__(lib, machine)
        self.images = {s: (C.c_ubyte * 65536).from_buffer_copy(bytes(machine.cpu.mem_read(s * 16, 65536)))
                       for s in [0x8000, 0xa000, 0xb000, 0xc000]}
        self.heap_callbacks = []
        self.heap_api = HeapApi()
        for name in ['unlock', 'free']:
            operation = {'unlock': 'GlobalUnlock', 'free': 'GlobalFree'}[name]
            def call(_, h, operation=operation):
                result, _ = self.world.call(operation, h)
                return result & 65535
            cb = C.CFUNCTYPE(C.c_uint16, C.c_void_p, C.c_uint16)(call)
            self.heap_callbacks.append(cb)
            setattr(self.heap_api, name, C.cast(cb, C.c_void_p).value)
        cb = C.CFUNCTYPE(C.c_void_p, C.c_void_p, C.c_uint16)(lambda _, s: C.addressof(self.images[s]))
        self.heap_callbacks.append(cb)
        self.heap_api.resolve = C.cast(cb, C.c_void_p).value
        self.memory = Memory(C.pointer(self.runtime), C.pointer(self.heap_api), None)

    def do_memory(self, seg, off):
        if seg == 0x3000:
            return C.addressof(self.data) + off
        return C.addressof(self.images[seg]) + off


class StartupMachineTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        common_tests.CommonMachineTests.setUpClass.__func__(cls)
        cls.lib.stars_runtime_init.argtypes = [C.POINTER(Runtime), PTR, PTR, C.POINTER(Platform),
                                              C.c_void_p, C.c_uint16, C.c_uint16]
        for name, restype, args in [
                ('brush_get', C.c_uint16, [C.POINTER(Runtime), C.c_uint32]),
                ('brush_free', None, [C.POINTER(Runtime), C.c_uint16]),
                ('system_colors', C.c_int16, [C.POINTER(Runtime)]),
                ('create_planet_brushes', None, [C.POINTER(Runtime)]),
                ('create_fonts', C.c_int16, [C.POINTER(Runtime), C.c_uint16]),
                ('init_tiles', None, [PTR]),
                ('free_stuff', None, [C.POINTER(Memory)])]:
            f = getattr(cls.lib, 'stars_' + name)
            f.restype, f.argtypes = restype, args

    @classmethod
    def tearDownClass(cls):
        cls.temp.cleanup()

    def setUp(self):
        self.machine = StartupMachine()
        self.machine.put(0xd34, bytes(48))
        self.machine.put(0x13ac, b'\0\0')
        self.machine.put(0x9a0, bytes(4))
        self.machine.put(0xaf8, bytes(4))
        self.host = None

    def start(self):
        self.host = StartupHost(self.lib, self.machine)

    def call(self, name, *args):
        if self.host is None:
            self.start()
        host = self.host
        host.world.events.clear()
        self.machine.world.events.clear()
        offsets = {'brush_get': (9, 0x4448, '<I'), 'brush_free': (9, 0x4532, '<H'),
                   'system_colors': (4, 0x8d2, ''), 'create_fonts': (1, 0xab2, '<H'),
                   'init_tiles': (1, 0xeaa, ''), 'free_stuff': (4, 0xbae, '')}
        segment, offset, fmt = offsets[name]
        target = host.data if name == 'init_tiles' else C.byref(host.memory if name == 'free_stuff' else host.runtime)
        result = getattr(self.lib, 'stars_' + name)(target, *args)
        original = self.machine.call(offset, struct.pack(fmt, *args) if fmt else b'', segment=segment, shared_stack=True)
        self.assertFalse(host.errors)
        if result is not None:
            self.assertEqual(result & 65535, original & 65535)
        self.assertEqual(host.world.events, self.machine.world.events, name)
        self.assertEqual(bytes(host.data[:0xe000]), self.machine.get(0, 0xe000), name)
        for selector, data in host.images.items():
            self.assertEqual(bytes(data), bytes(self.machine.cpu.mem_read(selector * 16, 65536)), (name, selector))
        return result

    def test_brush_cache_reuse_last_hole_full_cache_and_failures(self):
        first = self.call('brush_get', 0x123456)
        self.assertEqual(first, self.call('brush_get', 0x123456))
        self.call('brush_free', first)
        self.assertFalse(self.host.world.events)
        self.call('brush_free', first)
        handles = [self.call('brush_get', c) for c in range(33)]
        self.assertEqual(struct.unpack_from('<H', bytes(self.host.data), 0x13ac)[0], 32)
        self.call('brush_free', handles[32])  # uncached
        for i in [2, 8, 5]:
            self.call('brush_free', handles[i])
        self.call('brush_get', 0xdeadbeef)  # the last free slot, eight
        self.assertEqual(self.host.data[0x2518 + 8], 1)
        self.host.world.fail_brush = self.machine.world.fail_brush = True
        self.assertEqual(self.call('brush_get', 0xabcdef), 0)
        self.call('brush_free', 0)
        self.call('brush_free', 0xffff)

    def test_planet_color_startup_slice_and_cached_ownership(self):
        from unicorn import UC_HOOK_CODE
        from tools.startup_machine import reg
        colors=[0x7f0000,0xff0000,0x7f,0xff,0x7f00,0xff00]
        for case in ['fresh','shared','failed_allocation']:
            self.setUp()
            if case=='shared':
                self.call('brush_get',colors[0]);self.call('brush_get',colors[4])
            if self.host is None:self.start()
            if case=='failed_allocation':
                self.host.world.fail_brush=self.machine.world.fail_brush=True
            self.host.world.events.clear();self.machine.world.events.clear()
            self.lib.stars_create_planet_brushes(C.byref(self.host.runtime))
            # This self-contained original slice balances its own stack. Exit
            # through an existing RETF after its sixth store, before unrelated
            # FCreateStuff work. No game call is replaced with a stub.
            def stop(cpu,address,size,user):
                if address==0x702af:cpu.reg_write(reg.UC_X86_REG_IP,0xab1)
            hook=self.machine.cpu.hook_add(UC_HOOK_CODE,stop)
            self.machine.call(0x23d,b'',segment=1,shared_stack=True)
            self.machine.cpu.hook_del(hook)
            self.assertEqual(self.host.errors,[])
            self.assertEqual(self.host.world.events,self.machine.world.events)
            self.assertEqual(bytes(self.host.data[:0xe000]),self.machine.get(0,0xe000))
            if case=='fresh':
                self.assertEqual([e[4] for e in self.host.world.events if e[0]=='CreateSolidBrush'],colors)
            # FreeStuff already releases these same six references. Check
            # sharing and failure behavior against the original FreeHbr here.
            handles=struct.unpack('<6H',bytes(self.host.data[0x5680:0x568c]))
            self.assertEqual(all(handles),case!='failed_allocation')
            for h in handles:self.call('brush_free',h)

    def test_brush_reference_count_wrap(self):
        first = self.call('brush_get', 0x76543210)
        for _ in range(255):
            self.call('brush_get', 0x76543210)
        self.assertEqual(self.host.data[0x2518], 0)
        self.call('brush_free', first)
        self.call('brush_get', 0x76543210)

    def test_system_color_refresh_palette_bytes_and_failures(self):
        for resources in [False, True]:
            self.setUp()
            if resources:
                self.machine.put(0xc82, struct.pack('<H', 55))
                self.machine.put(0x598c, struct.pack('<H', 56))
                self.machine.put(0xc000, b'\xa5' * 1200)
            self.call('system_colors')
            self.call('system_colors')  # frees/reacquires shared brushes
            self.host.world.colors = self.machine.world.colors = {i: 0x123456 for i in range(32)}
            self.call('system_colors')
            self.host.world.colors = self.machine.world.colors = {i: i + 1000 for i in range(32)}
            self.host.world.fail_brush = self.machine.world.fail_brush = True
            self.assertEqual(self.call('system_colors'), 1)

    def test_fonts_default_custom_faces_dpi_and_failure(self):
        for dpi, faces, fail in [(96, False, False), (120, True, False), (32767, True, False), (96, False, True)]:
            self.setUp()
            self.machine.world.caps[90] = dpi
            self.machine.world.fail_font = fail
            self.machine.put(0x6a6, bytes(128))
            if faces:
                for i in range(4):
                    self.machine.put(0x6a6 + i * 32, ('Face%d' % i).encode() + b'\0')
            self.assertEqual(self.call('create_fonts', 500), 1)
            self.assertEqual(sum(e[0] == 'CreateFontIndirect' for e in self.host.world.events), 9)
            self.assertEqual(self.host.world.selected[500], 1)

    def test_tile_arrays_group_boundaries_and_overflow(self):
        rng = random.Random(2700301)
        for n in range(100):
            self.setUp()
            self.machine.put(0x23fa, struct.pack('<H', rng.randrange(65536)))
            for base, count in [(0x7fc, 6), (0x90e, 7)]:
                self.machine.put(base, bytes(rng.randrange(256) for _ in range(count * 16)))
            self.call('init_tiles')

    def test_full_cleanup_after_fonts_and_colors(self):
        self.call('create_fonts', 500)
        self.call('system_colors')
        self.call('free_stuff')
        self.assertEqual(sum(e[0] == 'FreeResource' for e in self.host.world.events), 24)
        self.assertEqual(sum(e[0] == 'DestroyCursor' for e in self.host.world.events), 11)

    def test_terminal_cleanup_populated_handles_heap_payloads_and_optional_fields(self):
        globals_ = json.loads((ROOT / 'analysis/debug/globals.json').read_text())
        for populated in [False, True]:
            self.setUp()
            for entry in globals_:
                n, off = entry['name'], entry.get('ne_offset')
                if off is None or entry.get('ne_segment') != 37:
                    continue
                if n.startswith(('hbr', 'hbmp', 'hcur', 'hpen', 'hdib', 'hrgn', 'hicon', 'vhpal')):
                    self.machine.put(off, struct.pack('<H', ((off ^ 0x5555) or 1) if populated else 0))
                elif n.startswith('lpfn'):
                    self.machine.put(off, struct.pack('<I', (0x77770000 | off) if populated else 0))
            # Three synthetic Windows allocations, with two live game payloads.
            for selector, ht, handle in [(0x8000, 4, 101), (0xa000, 2, 102), (0xb000, 11, 103)]:
                p = selector << 16 | 32
                header = struct.pack('<4HIHBB', 86, 128, 86, 42, 0, handle, ht, 0xa5)
                self.machine.cpu.mem_write(selector * 16 + 32, header + struct.pack('<H', 24) + b'\xa5' * 24)
                self.machine.put(0xd34 + ht * 4, struct.pack('<I', p))
                if ht in [4, 2]:
                    self.machine.put(0x9a0 if ht == 4 else 0xaf8, struct.pack('<I', p + 18))
            self.call('free_stuff')
            self.assertEqual(bytes(self.host.data[0x9a0:0x9a4]), bytes(4))
            self.assertEqual(bytes(self.host.data[0xaf8:0xafc]), bytes(4))
            self.assertEqual([e[1] for e in self.host.world.events if e[0] == 'GlobalFree'], [102, 101, 103])


if __name__ == '__main__':
    unittest.main()
