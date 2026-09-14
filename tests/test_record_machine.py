"""Record and player transformations versus original code at explicit OS boundaries."""
import ctypes as C
import itertools
from pathlib import Path
import random
import struct
import unittest
from tests import test_core_machine as core_tests
from tools.record_machine import RecordMachine
from tools.ne_layout import NE

ROOT=Path(__file__).resolve().parents[1]
READ=C.CFUNCTYPE(None,C.c_void_p,C.POINTER(C.c_ubyte),C.c_uint16)
WRITE=C.CFUNCTYPE(None,C.c_void_p,C.POINTER(C.c_ubyte),C.c_uint16)

class RecordMachineTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        core_tests.CoreMachineTests.setUpClass.__func__(cls)
        ptr=C.POINTER(C.c_ubyte)
        cls.strings=(C.c_ubyte*65536).from_buffer_copy(bytes(NE(ROOT/'target/stars.exe').raw[2]).ljust(65536,b'\0'))
        cls.lib.stars_read_record.argtypes=[ptr,READ,C.c_void_p]
        cls.lib.stars_write_record.argtypes=[ptr,C.c_int16,C.c_uint16,ptr,WRITE,C.c_void_p]
        cls.lib.stars_write_player_record.argtypes=[ptr,C.c_uint16,C.c_uint16,WRITE,C.c_void_p]
        cls.lib.stars_read_player_record.argtypes=[ptr,ptr,C.c_uint16,C.c_uint16]
        cls.lib.stars_player_name.argtypes=[ptr,ptr]+[C.c_int16]*5+[C.c_uint16]
        cls.lib.stars_player_name.restype=C.c_uint16
        cls.lib.stars_compress_user_string.argtypes=[ptr,ptr,ptr,C.POINTER(C.c_int16)]
        for name in ['stars_read_record','stars_write_record','stars_write_player_record','stars_read_player_record']:
            getattr(cls.lib,name).restype=None

    @classmethod
    def tearDownClass(cls):cls.temp.cleanup()

    def setUp(self):
        self.machine=RecordMachine()
        self.initial=bytearray(self.machine.get(0,65536))
        self.initial[0x99c:0x9a0]=bytes(4)
        self.initial[0x1484]=255

    def equal_data(self,memory):
        # Original private stack is above E000; all application data below it
        # is compared, including unchanged bytes and temporary global buffers.
        self.assertEqual(bytes(memory[:0xe000]),self.machine.get(0,0xe000))

    def write_record(self,typ,payload,source=0x3000):
        memory=bytearray(self.initial)
        memory[source:source+len(payload)]=payload
        self.machine.put(0,bytes(memory));self.machine.outgoing.clear();self.machine.transfers.clear()
        host=(C.c_ubyte*65536).from_buffer(memory)
        output=bytearray()
        callback=WRITE(lambda _,p,n:output.extend(C.string_at(p,n)))
        self.lib.stars_write_record(host,typ,len(payload),C.cast(C.byref(host,source),C.POINTER(C.c_ubyte)),callback,None)
        self.machine.call(0x947c,struct.pack('<hHHH',typ,len(payload),source,0x3000),segment=15,shared_stack=True)
        self.assertEqual(bytes(output),bytes(self.machine.outgoing),(typ,len(payload)))
        self.equal_data(memory)
        return bytes(output),memory

    def read_record(self,payload,initial=None):
        memory=bytearray(self.initial if initial is None else initial)
        self.machine.put(0,bytes(memory));self.machine.incoming=payload;self.machine.position=0
        position=[0]
        def read(_,p,n):
            block=payload[position[0]:position[0]+n]
            self.assertEqual(len(block),n)
            C.memmove(p,block,n);position[0]+=n
        callback=READ(read)
        self.lib.stars_read_record((C.c_ubyte*65536).from_buffer(memory),callback,None)
        self.machine.call(0x5168,segment=15,shared_stack=True)
        self.assertEqual(position[0],self.machine.position)
        self.equal_data(memory)
        return memory

    def test_all_record_types_lengths_and_source_overlap(self):
        rng=random.Random(270021)
        for typ in list(range(64))+[-32768,-1,64,72,32767]:
            for length in [0,1,2,3,15,16,17,1023]:
                payload=rng.randbytes(length)
                encoded,_=self.write_record(typ,payload)
                self.read_record(encoded)
        for source in [0x4b97,0x4b98,0x4b99]:self.write_record(6,rng.randbytes(100),source)

    def test_bof_signed_salt_player_and_stream_sequence(self):
        for salt,player,turn in itertools.product([0,1023,1024,2047],[0,15,16,31],[0,1,65535]):
            bof=struct.pack('<4sIHHHH',b'J3J3',0x12345678,0x2a60,turn,(salt<<5)|player,0x1005)
            wire,after=self.write_record(8,bof)
            read_after=self.read_record(wire)
            self.assertEqual(after[0x2272:0x227a],read_after[0x2272:0x227a])
        memory=bytearray(self.initial)
        header_only=struct.pack('<H',8<<10)
        self.read_record(header_only,memory) # stale scratch is deliberately used

    def player(self,extended=True,relations=0,first=b'Golem',plural=b'Golems'):
        record=bytearray((i*7+3)&255 for i in range(192))
        record[6]=(record[6]&248)|(7 if extended else 2)
        record[84]=0x88
        record[112:128]=bytes([2]*relations+[0]*(16-relations))
        record[128:160]=first.ljust(32,b'\0')
        record[160:192]=plural.ljust(32,b'\0')
        return record

    def write_player(self,record,storage):
        memory=bytearray(self.initial);memory[0x3000:0x30c0]=record
        self.machine.put(0,bytes(memory));self.machine.outgoing.clear()
        output=bytearray();callback=WRITE(lambda _,p,n:output.extend(C.string_at(p,n)))
        self.lib.stars_write_player_record((C.c_ubyte*65536).from_buffer(memory),0x3000,storage,callback,None)
        self.machine.call(0x551c,struct.pack('<HH',0x3000,storage),segment=15,shared_stack=True)
        self.assertEqual(bytes(output),bytes(self.machine.outgoing))
        self.equal_data(memory)
        return bytes(output),memory

    def read_player(self,payload,minor=83):
        memory=bytearray(self.initial);memory[0x3500:0x3500+len(payload)]=payload
        struct.pack_into('<H',memory,0x750,0x2000|(minor<<5))
        self.machine.put(0,bytes(memory))
        self.lib.stars_read_player_record((C.c_ubyte*65536).from_buffer(memory),self.strings,0x3000,0x3500)
        self.machine.call(0x5e2,struct.pack('<HH',0x3000,0x3500),segment=15,shared_stack=True)
        self.equal_data(memory)
        return memory[0x3000:0x30c0]

    def test_player_relations_names_storage_and_roundtrip(self):
        names=[(b'',b''),(b'Golem',b'Golems'),(b'A'*31,b'z'*31),(bytes([0x80])*31,bytes([0xfe])*31),
               (b' X  ',b'Explicit plural'),(b'Klingons',b'')]
        for extended,relations,names_pair,storage in itertools.product([False,True],[0,1,15,16],names,[0,0x3800]):
            record=self.player(extended,relations,*names_pair)
            wire,written=self.write_player(record,storage)
            body=self.read_record(wire)[0x4b98:0x4b98+(struct.unpack_from('<H',wire)[0]&1023)]
            decoded=self.read_player(body)
            self.assertEqual(decoded[128:192],record[128:192])
            self.assertEqual(decoded[84]&8,0)
        record=self.player(False);record[84]|=1
        _,memory=self.write_player(record,0x3800)
        self.assertEqual(memory[0x3006]&7,7)

    def test_legacy_plural_and_ignored_decode_failure(self):
        for name in [b'',b'Golem',b'Klingons',b'Case',b'  ',b'He  ']:
            raw=bytes(8)+b'\0'+name+b'\0'
            self.read_player(raw,54)
        # 34 cheap one-nybble letters exceed the reader's decoded capacity 32.
        # Failure is ignored: output field remains zero, next field is read.
        raw=bytes(8)+bytes([17])+bytes([0x11])*17+b'\0Plural\0'
        decoded=self.read_player(raw)
        self.assertEqual(decoded[128],0)
        self.assertEqual(decoded[160:167],b'Plural\0')

    def test_name_grammar_articles_and_placeholders(self):
        for first,plural_name in [(b'',b''),(b'Golem',b''),(b'Case',b''),(b'  ',b''),(b'A',b'Many'),(b'Klingons ',b'')]:
            for capital,plural,article in itertools.product([0,1],repeat=3):
                for grammar in [0,1,2,3]:
                    memory=bytearray(self.initial);memory[0x3000:0x30c0]=self.player(first=first,plural=plural_name)
                    self.machine.put(0,bytes(memory))
                    actual=self.lib.stars_player_name((C.c_ubyte*65536).from_buffer(memory),self.strings,0,capital,plural,article,grammar,0x3000)
                    original=self.machine.call(0x11f2,struct.pack('<5hH',0,capital,plural,article,grammar,0x3000),segment=8,shared_stack=True)&65535
                    self.assertEqual(actual,original);self.equal_data(memory)

    def test_name_global_player_and_signed_placeholder_index(self):
        for index in [0,15,-1,32767,-32768]:
            memory=bytearray(self.initial)
            player=(0x59a2+index*192)&65535
            memory[player:player+192]=self.player(first=b'',plural=b'')
            self.machine.put(0,bytes(memory))
            actual=self.lib.stars_player_name((C.c_ubyte*65536).from_buffer(memory),self.strings,index,1,0,1,2,0)
            original=self.machine.call(0x11f2,struct.pack('<5hH',index,1,0,1,2,0),segment=8,shared_stack=True)&65535
            self.assertEqual(actual,original);self.equal_data(memory)

    def test_unserialized_name_tail_changes_checksum(self):
        record=self.player();record[84]&=247;record[134]=0x55
        wire,_=self.write_player(record,0)
        plain=self.read_record(wire)
        size=struct.unpack_from('<H',wire)[0]&1023
        decoded=self.read_player(plain[0x4b98:0x4b98+size])
        self.assertEqual(decoded[134],0)
        checks=[]
        for item in [record,decoded]:
            checksum=0
            for word in struct.unpack('<96H',bytes(item)):checksum ^= word
            checks.append(checksum)
        self.assertEqual(checks[0]^checks[1],0x55)

    def test_decoded_capacity_32_writes_terminator_after_player(self):
        # Two names of exactly 32 decoded characters are accepted. The plural
        # terminator lands at player+192, beyond its nominal 32-byte field.
        payload=bytes(8)+bytes([16])+bytes([0x11])*16+bytes([16])+bytes([0x22])*16
        memory=bytearray(self.initial);memory[0x3500:0x3500+len(payload)]=payload
        memory[0x30c0]=0xa5;struct.pack_into('<H',memory,0x750,0x2a60)
        self.machine.put(0,bytes(memory))
        self.lib.stars_read_player_record((C.c_ubyte*65536).from_buffer(memory),self.strings,0x3000,0x3500)
        self.machine.call(0x5e2,struct.pack('<HH',0x3000,0x3500),segment=15,shared_stack=True)
        self.equal_data(memory);self.assertEqual(memory[0x30c0],0)
        self.assertNotIn(0,memory[0x3080:0x30c0])

    def test_original_single_e_plural_depends_on_saved_si(self):
        # Original looks one byte before its temporary, which is the high
        # byte of saved SI. Do not call the host outside its documented domain.
        outputs=[]
        for si in [0x1234,0x7300]:
            memory=bytearray(self.initial);memory[0x3000:0x30c0]=self.player(first=b'e',plural=b'')
            self.machine.put(0,bytes(memory))
            self.machine.call(0x11f2,struct.pack('<5hH',0,0,1,0,0,0x3000),segment=8,shared_stack=True,entry_si=si)
            outputs.append(self.machine.get(0x57a4,4).split(bytes([0]))[0])
        self.assertEqual(outputs,[b'es',b'e'])

    def test_encrypted_player_file_sequence_and_checksum(self):
        # A controlled BOF replaces only WriteBOF's nondeterministic inputs;
        # every subsequent record transformation executes in original code.
        memory=bytearray(self.initial);memory[0x3000:0x30c0]=self.player(True,16)
        memory[0x3054]&=247
        bof=struct.pack('<4sIHHHH',b'J3J3',0x12345678,0x2a60,2400,(73<<5)|31,5)
        memory[0x3400:0x3410]=bof
        host=(C.c_ubyte*65536).from_buffer(memory)
        emitted=bytearray();callback=WRITE(lambda _,p,n:emitted.extend(C.string_at(p,n)))
        self.machine.put(0,bytes(memory));self.machine.outgoing.clear()
        self.lib.stars_write_record(host,8,16,C.cast(C.byref(host,0x3400),C.POINTER(C.c_ubyte)),callback,None)
        self.machine.call(0x947c,struct.pack('<hHHH',8,16,0x3400,0x3000),segment=15,shared_stack=True)
        self.lib.stars_write_player_record(host,0x3000,0,callback,None)
        self.machine.call(0x551c,struct.pack('<HH',0x3000,0),segment=15,shared_stack=True)
        checksum=0
        for word in struct.unpack('<96H',bytes(memory[0x3000:0x30c0])):checksum ^= word
        struct.pack_into('<H',memory,0x3400,checksum);self.machine.put(0x3400,struct.pack('<H',checksum))
        self.lib.stars_write_record(host,0,2,C.cast(C.byref(host,0x3400),C.POINTER(C.c_ubyte)),callback,None)
        self.machine.call(0x947c,struct.pack('<hHHH',0,2,0x3400,0x3000),segment=15,shared_stack=True)
        self.assertEqual(bytes(emitted),bytes(self.machine.outgoing));self.equal_data(memory)
        # Read all three records with seeds carried forward across calls.
        read_memory=bytearray(self.initial)
        for expected in [8,6,0]:
            at=getattr(self,'wire_cursor',0)
            header=struct.unpack_from('<H',emitted,at)[0];size=2+(header&1023)
            read_memory=self.read_record(bytes(emitted[at:at+size]),read_memory)
            self.wire_cursor=at+size
            self.assertEqual(header>>10,expected)
            if expected==6:
                decoded=self.read_player(read_memory[0x4b98:0x4b98+(header&1023)])
                got=0
                for word in struct.unpack('<96H',bytes(decoded)):got ^= word
                self.assertEqual(got,checksum)
        self.assertEqual(struct.unpack_from('<H',read_memory,0x4b98)[0],checksum)
        self.assertEqual(self.wire_cursor,len(emitted))

if __name__=='__main__':unittest.main()
