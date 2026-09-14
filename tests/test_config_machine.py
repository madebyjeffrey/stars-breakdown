"""Configuration state and boundary comparisons with original instructions."""
import ctypes as C
import random
import struct
import unittest
from tests import test_title_machine as title_tests
from tests.test_title_machine import TitleHost,Profile
from tests.test_race_runtime import Runtime,PTR
from tools.config_machine import ConfigMachine,words_bytes,reg
from unicorn import UC_HOOK_CODE

class ConfigHost(TitleHost):
    def do_window(self,cp):
        c=cp.contents;n=c.operation.decode()
        if n not in ['GetWindowPlacement','IsZoomed','GetPrivateProfileInt','WritePrivateProfileString']:
            return super().do_window(cp)
        p=q=None
        if n in ['GetPrivateProfileInt','WritePrivateProfileString']:
            profile=C.cast(c.q,C.POINTER(Profile)).contents
            q=(C.string_at(profile.section),C.string_at(profile.key),None,C.string_at(profile.file))
            if n=='WritePrivateProfileString':p=C.string_at(c.p)
        result,out=self.world.call(n,c.window,c.a,c.b,c.value,p,q)
        if out is not None:C.memmove(c.p,out,len(out))
        return result

class ConfigTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        title_tests.TitleTests.setUpClass.__func__(cls)
        cls.lib.stars_read_ini_tiles.argtypes=[PTR,C.c_uint16,C.c_uint16,C.c_int16]
        cls.lib.stars_window_rect.argtypes=[C.POINTER(Runtime),C.c_uint16,C.c_uint16]
        cls.lib.stars_window_ini_string.argtypes=[C.POINTER(Runtime),C.c_uint16,C.c_uint16]
        cls.lib.stars_read_ui_config.argtypes=[C.POINTER(Runtime),PTR]
        cls.lib.stars_write_ui_config.argtypes=[C.POINTER(Runtime),PTR,C.c_uint16]
    @classmethod
    def tearDownClass(cls):cls.temp.cleanup()
    def test_tile_order_case_stars_unknown_and_duplicate_letters(self):
        rng=random.Random(270032)
        cases=[b'',b'abcde',b'*****ABc',b'ffaAAzz*',b'c*bA',b'\xff\x80*\x7f']
        cases += [''.join(rng.choice('abcdefABCDEF*** xyz') for _ in range(30)).encode() for _ in range(80)]
        for text in cases:
            m=ConfigMachine();m.put(0xc000,text+b'\0');h=ConfigHost(self.lib,m)
            self.lib.stars_read_ini_tiles(h.data,0xc000,0x7fc,6)
            m.call(0x2b8a,words_bytes([0xc000,0x7fc,6]),segment=1,shared_stack=True)
            self.assertEqual(bytes(h.data[:0xe000]),m.get(0,0xe000),text)
    def test_normal_window_placement_and_unused_output_argument(self):
        for normal in [(30,40,630,440),(-300,-40,700,760),(32760,-32760,-32760,32760)]:
            for zoom,iconic in [(0,0),(1,0),(0,1),(1,1)]:
                m=ConfigMachine();m.world.normal=normal;m.world.zoomed=zoom;m.world.iconic=iconic
                h=ConfigHost(self.lib,m)
                self.lib.stars_window_rect(C.byref(h.runtime),100,0xc100)
                m.call(0x79ac,words_bytes([100,0xc100]),segment=5,shared_stack=True)
                self.assertEqual(bytes(h.data[:0xe000]),m.get(0,0xe000));self.assertEqual(h.world.events,m.world.events)
                self.lib.stars_window_ini_string(C.byref(h.runtime),0xc500,100)
                m.call(0x79f6,words_bytes([0xc500,100]),segment=5,shared_stack=True)
                self.assertEqual(h.errors,[]);self.assertEqual(h.world.events,m.world.events)
                self.assertEqual(bytes(h.data[:0xe000]),m.get(0,0xe000))
    def test_numeric_settings_slice_original_signed_clamps(self):
        # Enter the full original reader with a code hook that restricts this
        # comparison to 1431..176a (layout, six pane sizes, scanner flip flag).
        for value in [-32768,-1,0,1,2,3,9,10,11,1999,2000,2001,32767]:
            m=ConfigMachine()
            for id in [0x90,*range(0xa3,0xac)]:
                m.call(0x221e,words_bytes([id,0xc300]),shared_stack=True)
                text=m.get(0xc300,32).split(b'\0')[0]
                if id==0x90:section=text
                else:m.world.numbers[(section,text)]=value
            h=ConfigHost(self.lib,m)
            file=(C.c_ubyte*13).from_buffer_copy(b'STARSREC.INI\0')
            self.lib.stars_read_ui_config(C.byref(h.runtime),file)
            # Set the original BP-relative section/file buffers after prologue,
            # then leave through a synthetic RETF with the original stack intact.
            def scope(cpu,address,size,user):
                if address==0x71255:
                    bp=cpu.reg_read(reg.UC_X86_REG_BP);ss=cpu.reg_read(reg.UC_X86_REG_SS)*16
                    cpu.mem_write(ss+bp-0x46,section+b'\0')
                    cpu.mem_write(ss+bp-0x32,b'STARSREC.INI\0')
                    cpu.reg_write(reg.UC_X86_REG_IP,0x142d)
                elif address==0x7176a:
                    cpu.reg_write(reg.UC_X86_REG_IP,0x2b83)
            hook=m.cpu.hook_add(UC_HOOK_CODE,scope)
            m.call(0x124c,b'',segment=1,shared_stack=True)
            m.cpu.hook_del(hook)
            self.assertEqual(h.errors,[])
            for off in [0x22de,0x23d4,0x23d6,0x23d8,0x23da,0x23dc,0x23de,0x7cc]:
                self.assertEqual(bytes(h.data[off:off+2]),m.get(off,2),(value,hex(off)))

    def test_saved_frame_and_layout_survive_a_fresh_runtime(self):
        m=ConfigMachine();h=ConfigHost(self.lib,m)
        filename=(C.c_ubyte*13).from_buffer_copy(b'STARSREC.INI\0')
        self.lib.stars_read_ui_config(C.byref(h.runtime),filename)
        self.lib.stars_write_ui_config(C.byref(h.runtime),filename,100)
        expected={
            (b'Windows',b'Main'):b'R0030004006000400',
            (b'Windows',b'Layout'):b'1',
            (b'Windows',b'Style1Width'):b'396',
            (b'Windows',b'Style1Height'):b'110',
            (b'Windows',b'Style1Height2'):b'192',
            (b'Windows',b'Style2Width'):b'396',
            (b'Windows',b'Style2Height'):b'110',
            (b'Windows',b'Style2Height2'):b'192',
            (b'Windows',b'Toolbar'):b'1'}
        self.assertEqual(h.errors,[]);self.assertEqual(h.world.writes,expected)
        fresh=ConfigMachine();fresh.world.profile=expected
        fresh.world.numbers={k:int(v) for k,v in expected.items() if k[1]!=b'Main'}
        h2=ConfigHost(self.lib,fresh)
        self.lib.stars_read_ui_config(C.byref(h2.runtime),filename)
        self.assertEqual(h2.errors,[])
        self.assertEqual(bytes(h2.data[0x2290:0x2298]),struct.pack('<4H',30,40,600,400))
        for offset in [0x22de,0x23d4,0x23d6,0x23d8,0x23da,0x23dc,0x23de,0x7cc]:
            self.assertEqual(bytes(h.data[offset:offset+2]),bytes(h2.data[offset:offset+2]))
