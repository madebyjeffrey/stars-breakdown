"""Independent original-instruction comparisons for the startup milestone."""
import ctypes as C
import struct
import unittest
from tests import test_common_machine as common_tests
from tests.test_race_ui import UiHost, Rect
from tests.test_race_runtime import Runtime, Platform, PTR
from tools.title_machine import TitleMachine
from tools.race_ui_machine import words_bytes
from unicorn import UC_HOOK_CODE
from tools.core_machine import reg


class CreateWindow(C.Structure):
    _fields_ = [('class_name', C.c_void_p), ('text', C.c_void_p), ('geometry', Rect),
                ('instance', C.c_uint16), ('menu', C.c_uint16)]


class Profile(C.Structure):
    _fields_ = [(n, C.c_void_p) for n in ['section', 'key', 'fallback', 'file']]


class TitleHost(UiHost):
    def do_memory(self, seg, off):
        if seg == 0xc000:
            return C.addressof((C.c_ubyte * len(self.world.huge)).from_buffer(self.world.huge)) + off
        return super().do_memory(seg, off)

    def do_window(self, cp):
        c = cp.contents; n = c.operation.decode()
        p = q = None
        if n == 'CreatePalette':
            count = C.c_uint16.from_address(c.p + 2).value
            p = C.string_at(c.p, 4 + count * 4)
        elif n == 'CreateWindow':
            req = C.cast(c.p, C.POINTER(CreateWindow)).contents
            p = (C.string_at(req.class_name), C.string_at(req.text), *struct.unpack('<4H', bytes(req.geometry)), req.instance, req.menu)
        elif n == 'InflateRect': p = struct.unpack('<4H', C.string_at(c.p, 8))
        elif n == 'GetPrivateProfileString':
            req = C.cast(c.q, C.POINTER(Profile)).contents
            q = tuple(C.string_at(getattr(req, name)) for name, _ in Profile._fields_)
        elif n not in {n for n, _ in TitleMachine.EXTRA.values()}:
            return super().do_window(cp)
        result, output = self.world.call(n, c.window, c.a, c.b, c.value, p, q)
        if output is not None:
            payload = words_bytes(output) if isinstance(output, tuple) else output
            C.memmove(c.p, payload, len(payload))
        return result


class TitleTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        common_tests.CommonMachineTests.setUpClass.__func__(cls)
        rp = C.POINTER(Runtime); u = C.c_uint16; s = C.c_int16; l = C.c_uint32
        cls.lib.stars_runtime_init.argtypes = [rp, PTR, PTR, C.POINTER(Platform), C.c_void_p, u, u]
        for n, result, args in [
            ('read_big_block', s, [rp,u,l,l]), ('load_big_resource', u, [rp,u]),
            ('palette_from_dib', u, [rp,u]), ('ini_window_rect', None, [rp,u,u,s,u]),
            ('draw_stars', None, [rp,u,C.POINTER(Rect),l]), ('title_create', None, [rp,u]),
            ('title_paint', None, [rp,u,l]), ('title_palette', s, [rp,u,u,u]),
            ('title_release', None, [rp]), ('title_read_fonts', None, [rp,PTR])]:
            f = getattr(cls.lib, 'stars_' + n); f.restype = result; f.argtypes = args

    @classmethod
    def tearDownClass(cls): cls.temp.cleanup()

    def setUp(self):
        self.m = TitleMachine()
        for offset, value in [(0x5310,7),(0x23fa,15),(0x430,0),(0x42e,0),(0x1378,0),(0x5312,8)]:
            self.m.put(offset, words_bytes([value]))
        self.m.put(0x56a2, b'\0')

    def host(self): return TitleHost(self.lib, self.m)

    def compare(self, h, *, stack=False):
        self.assertEqual(h.errors, [])
        self.assertEqual(h.world.events, self.m.world.events)
        self.assertEqual(h.world.huge, self.m.world.huge)
        a, b = bytes(h.data[:0xe000]), self.m.get(0, 0xe000)
        self.assertEqual([(hex(i), x, y) for i, (x, y) in enumerate(zip(a,b)) if x != y][:20], [])

    def test_huge_reads_zero_boundaries_selector_wrap_and_short_reads(self):
        for size in [0,1,29999,30000,30001,65535,65536,100000,481088]:
            for offset in [0,50000]:
                self.setUp(); self.m.world.resource = bytes((i*7)&255 for i in range(size))
                h = self.host(); pointer = 0xc0000000 | offset
                actual = self.lib.stars_read_big_block(C.byref(h.runtime),7,pointer,size)
                expected = self.m.call(0x5310,struct.pack('<HII',7,pointer,size),shared_stack=True)&65535
                self.assertEqual(actual,expected); self.compare(h)
        for short in range(4):
            self.setUp(); self.m.world.short_read_at = short; h = self.host()
            actual = self.lib.stars_read_big_block(C.byref(h.runtime),7,0xc0000000,100000)
            expected = self.m.call(0x5310,struct.pack('<HII',7,0xc0000000,100000),shared_stack=True)&65535
            self.assertEqual(actual,expected); self.assertEqual(actual,0); self.compare(h)

    def test_resource_load_success_and_every_failure_exit(self):
        for failure in [None,'FindResource','AllocResource','AccessResource','LockResource','short']:
            self.setUp(); self.m.world.failure = failure
            if failure == 'short': self.m.world.short_read_at = 2
            h = self.host(); actual = self.lib.stars_load_big_resource(C.byref(h.runtime),449)
            expected = self.m.call(0x5220,words_bytes([449]),shared_stack=True)&65535
            self.assertEqual(actual,expected); self.assertEqual(actual,60 if failure is None else 0); self.compare(h)

    def dib(self, colors=256, core=False):
        data = bytearray((i*17)&255 for i in range(2048))
        struct.pack_into('<I',data,0,12 if core else 40)
        struct.pack_into('<H',data,10 if core else 14,8)
        struct.pack_into('<I',data,32,colors)
        self.m.world.huge[:len(data)] = data
        self.m.cpu.mem_write(0xc0000,bytes(data))

    def test_palette_header_variants_limits_reserved_bytes_and_failures(self):
        for count, core, fail in [(0,False,None),(2,False,None),(16,False,None),(256,False,None),
                                  (257,False,None),(0,True,None),(256,False,'LockResource'),(256,False,'CreatePalette')]:
            self.setUp(); self.dib(count,core); self.m.world.failure=fail; h=self.host()
            actual=self.lib.stars_palette_from_dib(C.byref(h.runtime),60)
            expected=self.m.call(0x4b50,words_bytes([60]),shared_stack=True)&65535
            self.assertEqual(actual,expected); self.compare(h)

    def test_ini_window_format_valid_malformed_and_flag_preservation(self):
        for raw in [b'',b'R0001000208000600',b'M-001002003000400',b'I12-300-4----1234',
                    b'M00010002080006000',b'X0001000208000600',b'R0001000a08000600',b'R0001000208000600\xff']:
            for id in [0x94,0xb1]:
                self.setUp(); self.m.put(0xc000,b'Test\0'); self.m.put(0xc100,b'STARSREC.INI\0'); self.m.put(0xc208,words_bytes([0xabc7]))
                # Get the key from the independent original compressed-string routine.
                self.m.call(0x221e,words_bytes([id,0xc300]),shared_stack=True)
                key=self.m.get(0xc300,16).split(b'\0')[0]
                self.m.world.profile[(b'Test',key)] = raw
                h=self.host(); self.lib.stars_ini_window_rect(C.byref(h.runtime),0xc000,0xc100,id,0xc200)
                self.m.call(0x1020,words_bytes([0xc000,0xc100,id,0xc200]),segment=1,shared_stack=True)
                self.compare(h)

    def test_starfield_all_pixels_and_seed_restore(self):
        for box in [(0,0,640,480),(0,0,800,600),(2,3,1026,771),(0,0,7,7)]:
            for si in [0,0x1234,0x7fff]:
                self.setUp(); self.m.put(0xc100,words_bytes(box)); h=self.host(); rect=Rect(*box)
                self.lib.stars_draw_stars(C.byref(h.runtime),500,C.byref(rect),(si<<16)|0x5678)
                self.m.call(0x5fe2,words_bytes([500,0xc100]),segment=8,shared_stack=True,entry_si=si)
                self.compare(h)

    def test_title_create_layout_resolution_edges_and_palette_failures(self):
        for width,height in [(640,480),(800,500),(800,501),(800,649),(800,650),(1024,768)]:
            for depth, failure in [(4,None),(8,None),(24,'FindResource')]:
                self.setUp(); self.dib(); self.m.world.resource=bytes(self.m.world.huge[:2048])
                self.m.world.client=(0,0,width,height); self.m.world.failure=failure
                self.m.put(0x5312,words_bytes([depth])); h=self.host()
                self.lib.stars_title_create(C.byref(h.runtime),100)
                self.m.dialog_call(0x9126,100,1,segment=5)
                self.compare(h)

    def test_title_paint_bitmap_fallback_iconic_and_palette_refresh(self):
        for depth, dib, iconic in [(8,60,0),(4,0,0),(24,0,0),(8,60,1)]:
            self.setUp(); self.dib(); self.m.world.iconic=iconic
            self.m.put(0x5312,words_bytes([depth])); self.m.put(0x430,words_bytes([dib]))
            # Capture the two implicit register arguments at DrawABunchOfStars,
            # before its prologue. No instructions or game calls are replaced.
            captured=[]
            def entry(cpu,addr,size,_):
                captured.append(cpu.reg_read(reg.UC_X86_REG_SI)<<16|cpu.reg_read(reg.UC_X86_REG_DI))
            hook=self.m.cpu.hook_add(UC_HOOK_CODE,entry,begin=0xb5fe2,end=0xb5fe2)
            h=self.host(); self.m.dialog_call(0x9126,100,15,segment=5)
            self.lib.stars_title_paint(C.byref(h.runtime),100,captured[0] if captured else 0)
            self.compare(h); self.m.cpu.hook_del(hook)
        for msg,wp,depth,changed in [(0x30f,0,8,1),(0x30f,0,8,0),(0x311,100,8,1),(0x311,101,8,1),(0x30f,0,4,1)]:
            self.setUp(); self.m.world.realized=changed; self.m.put(0x5312,words_bytes([depth])); h=self.host()
            actual=self.lib.stars_title_palette(C.byref(h.runtime),100,msg,wp)
            expected=self.m.dialog_call(0x9126,100,msg,wp,segment=5)
            self.assertEqual(actual,expected); self.compare(h)

    def test_title_continue_file_presence_and_release(self):
        for access in [-1,0]:
            self.setUp();self.m.put(0x5312,words_bytes([4]));self.m.put(0x56a2,b'TEST.M1\0')
            self.m.access_results=[access];h=self.host()
            self.lib.stars_title_create(C.byref(h.runtime),100)
            self.m.dialog_call(0x9126,100,1,segment=5)
            self.compare(h);self.assertEqual(h.events,self.m.events)
            self.assertEqual(sum(e[0]=='EnableWindow' for e in h.world.events),int(access==-1))
        for dib in [0,60]:
            self.setUp();self.m.put(0x430,words_bytes([dib]));self.m.put(0x354,words_bytes([1]));h=self.host()
            self.lib.stars_title_release(C.byref(h.runtime))
            self.m.dialog_call(0x9126,100,2,segment=5)
            self.assertEqual(self.m.world.events.pop(),('DefWindowProc',100,2,0,0,None,None))
            self.compare(h)

    def test_font_configuration_slice_original_length_boundaries(self):
        for faces in [[b'',b'Face',b'Arial',b'A'*31],[b'A'*32,b'Times New Roman',b'Courier New',b'B'*90]]:
            self.setUp()
            self.m.put(0xc100,b'STARSREC.INI\0')
            self.m.call(0x221e,words_bytes([0xc4,0xc300]),shared_stack=True)
            section=self.m.get(0xc300,40).split(b'\0')[0]
            for i,face in enumerate(faces):
                self.m.call(0x221e,words_bytes([0xc5+i,0xc300]),shared_stack=True)
                key=self.m.get(0xc300,40).split(b'\0')[0]
                self.m.world.profile[(section,key)]=face
            h=self.host()
            self.lib.stars_title_read_fonts(C.byref(h.runtime),C.cast(C.byref(h.data,0xc100),PTR))
            # Execute just the identified source slice inside ReadIniSettings,
            # with its parent frame's existing filename local populated.
            cpu=self.m.cpu
            for name,value in dict(CS=0x7000,IP=0x249d,DS=0x3000,ES=0x3000,SS=0x3000,
                                   SP=0xee00,BP=0xef00,SI=0x1234,DI=0x5678,EFLAGS=2).items():
                cpu.reg_write(getattr(reg,'UC_X86_REG_'+name),value)
            self.m.put(0xef00-0x32,b'STARSREC.INI\0')
            stopped=[]
            def stop(cpu,addr,size,_): stopped.append(addr);cpu.emu_stop()
            hook=cpu.hook_add(UC_HOOK_CODE,stop,begin=0x72544,end=0x72544)
            cpu.emu_start(0x7249d,0x72545,count=1000000)
            self.assertEqual(stopped,[0x72544]);cpu.hook_del(hook)
            self.compare(h)


if __name__ == '__main__': unittest.main()
