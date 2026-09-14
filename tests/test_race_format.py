"""All message escapes, including their transitive object-name lookups."""
import ctypes as C
import struct
import unittest
from tests import test_race_runtime as runtime_tests
from tests.test_race_runtime import Host
from tools.race_runtime_machine import RaceRuntimeMachine
from tools.race_ui_machine import words_bytes

class FormatMachine(RaceRuntimeMachine):
    SEGMENTS=dict(RaceRuntimeMachine.SEGMENTS,**{})
    SEGMENTS[2]=0x5000
    del SEGMENTS[30]
    SEGMENTS[28]=0x7000

class FormatHost(Host):
    def __init__(self,lib,m):
        super().__init__(lib,m)
        self.parts=(C.c_ubyte*65536).from_buffer_copy(bytes(m.cpu.mem_read(0x50000,65536)))
        self.runtime.segments[2]=self.parts
    def do_memory(self,seg,off):
        if seg==0x5000:return C.addressof(self.parts)+off
        return super().do_memory(seg,off)

class RaceFormatTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):runtime_tests.RaceRuntimeTests.setUpClass.__func__(cls)
    @classmethod
    def tearDownClass(cls):cls.temp.cleanup()
    def setUp(self):
        self.m=FormatMachine();m=self.m
        m.put(0x18c,words_bytes([0]));m.put(0x78,words_bytes([4]));m.put(0x56a2,b'SYNTH\0')
        m.put(0x57a4,b'working text\0');m.put(0x137a,bytes([255]))
        for i in range(16):
            m.put(0x59a2+i*192,m.get(0xda2,192))
            m.put(0x5a22+i*192,('Race%d\0'%i).encode());m.put(0x5a42+i*192,('Races%d\0'%i).encode())
            m.put(0xfe+i*4,struct.pack('<I',0x3000a000));m.put(0x14c+i*4,struct.pack('<I',0x3000b000))
            m.put(0xa000+i*147,bytes(147));m.put(0xa008+i*147,('Design%d\0'%i).encode())
            m.put(0xb000+i*147,bytes(147));m.put(0xb008+i*147,('Base%d\0'%i).encode())
        # rglpfl is sorted by the unflagged fleet id; each player owns one fleet.
        m.put(0xfa,struct.pack('<I',0x3000d000));m.put(0x5356,words_bytes([4]))
        for i in range(4):
            m.put(0x59a6+i*192,words_bytes([1]));m.put(0xd000+i*4,struct.pack('<I',0x3000d100+i*128))
            m.put(0xd100+i*128,bytes(128));m.put(0xd100+i*128,words_bytes([i*512,i]));m.put(0xd10c+i*128,words_bytes([1,2]))
        m.put(0x1698,struct.pack('<I',0x3000d900));m.put(0x169c,words_bytes([10]))
        self.ids=[]
        for i in range(10):
            id=(i//2<<13)|(i%2<<9)|i;self.ids.append(id)
            m.put(0xd900+i*18,bytes(18));m.put(0xd900+i*18,words_bytes([id]));m.put(0xd906+i*18,words_bytes([1024 if i==3 else 0]))
        m.put(0x276e,words_bytes([0,64,998,999,1999]))
    def case(self,format,params=()):
        self.m.put(0x3000,format+b'\0');self.m.put(0x3200,words_bytes(params)+bytes(32));h=FormatHost(self.lib,self.m)
        actual=self.lib.stars_format_string(C.byref(h.runtime),0x3000,C.byref(h.data,0x3200))
        expected=self.m.call(0x85cc,words_bytes([0x3000,0x3200,0x3000]),segment=7,shared_stack=True)&65535
        self.assertEqual(h.errors,[]);self.assertEqual(actual,expected)
        a=bytes(h.data[:0xe000]);b=self.m.get(0,0xe000)
        self.assertEqual([(hex(i),x,y) for i,(x,y) in enumerate(zip(a,b)) if x!=y and not 0x1706<=i<0x170e][:30],[],(format,params))
        return C.string_at(C.byref(h.data,actual))
    def test_numeric_units_environment_and_parameter_consumption(self):
        for esc,values in [('i',[0,1,32767,32768,65535]),('u',[0,65535]),('P',[0,99,100,999,1000,32767,32768,65535]),
                           ('E',[e*256+v for e in range(3) for v in [0,1,25,49,50,51,75,99,100]]),
                           ('G',[0,1]),('I',[0,1]),('j',range(6)),('M',range(3)),('m',range(4)),('e',range(3)),('X',[123])]:
            for value in values:
                with self.subTest(escape=esc,value=value):self.case(('a\\'+esc+'b\\i').encode(),[value,77])
        for n in [0,1,0x7fffffff,0x80000000,0xffffffff]:
            self.case(b'\\v / \\i',[n&65535,n>>16,77])
            for unit in range(4):
                self.case(b'\\m: \\U / \\i',[unit,n&65535,n>>16,77])
                self.case(b'\\V / \\i',[n&65535,n>>16,unit,77])
        for owner in [0,15,65535]:
            self.m.put(0x18c,words_bytes([owner]))
            for esc in b'fhryw t'.replace(b' ',b''):self.case(b'\\'+bytes([esc])+b':\\i',[77,88])
        self.case(b'\\q|\\\\|literal',[0])
    def test_player_and_planet_names(self):
        for esc in ['L','l']:
            for flags in range(0,256,16):self.case(('\\'+esc).encode(),[flags|1])
        for esc in ['O','S']:
            for value in [0,1,2]:self.case(('\\'+esc).encode(),[value*512 if esc=='O' else value])
        for mask in [0,1,2,3,5,7,15]:self.case(b'\\Z | \\i',[mask,77])
        for i in range(5):
            self.case(b'\\p',[i]);self.case(b'\\p',[i|0x8000]);self.case(b'\\o',[i]);self.case(b'\\n',[65535,i])
        for x,y in [(0,0),(1000,2000),(32767,32768)]:self.case(b'\\n',[x,y])
    def test_fleets_designs_and_things(self):
        for id in self.ids+[99]:self.case(b'\\g',[id]);self.case(b'\\n',[65534,id])
        for id in [0,1,512,513,1024,1536]:self.case(b'\\s',[id]);self.case(b'\\o',[id|0x8000])
        self.m.put(0xd178,struct.pack('<I',0x3000da00));self.m.put(0xda00,b'Named Fleet\0');self.case(b'\\s',[0])
        for word in [0,1,511,512,0x2000,0x2200]:self.case(b'\\F',[word])
        self.m.put(0xa07b,words_bytes([512]));self.case(b'\\F',[0])
        for word in [0,1,15,16,31,32,33,47,48,63]:self.case(b'\\z',[word])
    def test_fleet_lookup_signed_comparison_boundary(self):
        # The original search uses JL/JG on the stored word. Keep all pointers
        # valid while exercising the signed boundary that RETURN3 warned about.
        self.m.put(0x5356, words_bytes([2]))
        self.m.put(0xd180, words_bytes([0]))
        self.m.put(0xd1f8, struct.pack('<I', 0x3000da00))
        self.m.put(0xda00, b'Signed search success\0')
        for key in [0x8000, 0xffff]:
            self.m.put(0xd100, words_bytes([key]))
            self.assertEqual(self.case(b'\\s', [0]), b'Signed search success')
    def test_every_valid_component_name(self):
        limits=[16,16,10,12,24,12,15,8,10,16,5,17,11,20,32,15]
        for slot,limit in enumerate(limits):
            for index in range(limit):
                with self.subTest(type=1<<slot,index=index):self.case(b'\\k',[1<<slot,index])
    def test_all_compressed_planet_names_and_signed_byte_cache(self):
        for id in range(999):
            self.m.put(0x276e,words_bytes([id]));self.case(b'\\p',[0])
        for cached in [-128,-1,0,1,127]:
            self.m.put(0x137a,bytes([cached&255]));self.m.put(0x22c8,b'Cached Name\0')
            self.m.put(0x276e,words_bytes([cached]));self.assertEqual(self.case(b'\\p',[0]),b'Cached Name')

if __name__=='__main__':unittest.main()
