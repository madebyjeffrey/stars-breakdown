"""Stream control-flow boundaries and unchanged original save/load callers."""
import ctypes as C
import itertools
from pathlib import Path
import random
import struct
import subprocess
import unittest
from tests import test_core_machine as core_tests
from tools.stream_machine import StreamMachine, StreamAbort

ROOT=Path(__file__).resolve().parents[1]
PTR=C.POINTER(C.c_ubyte)
OPEN=C.CFUNCTYPE(C.c_int16,C.c_void_p,PTR,C.c_uint16,C.POINTER(C.c_uint16))
CLOSE=C.CFUNCTYPE(C.c_int16,C.c_void_p,C.c_int16)
READ=C.CFUNCTYPE(C.c_uint16,C.c_void_p,C.c_int16,PTR,C.c_uint16)
WRITE=C.CFUNCTYPE(C.c_uint16,C.c_void_p,C.c_int16,PTR,C.c_uint16)
TICK=C.CFUNCTYPE(C.c_uint32,C.c_void_p)
MEMORY=C.CFUNCTYPE(C.c_void_p,C.c_void_p,C.c_uint16,C.c_uint16)
ERROR=C.CFUNCTYPE(None,C.c_void_p,C.c_int16)
ALERT=C.CFUNCTYPE(None,C.c_void_p)
ABORT=C.CFUNCTYPE(None,C.c_void_p,C.c_uint16,C.c_int16)
TRANSFER=C.CFUNCTYPE(None,C.c_void_p,PTR,C.c_uint16)

class Api(C.Structure):
    _fields_=[('open',OPEN),('close',CLOSE),('read',READ),('write',WRITE),('ticks',TICK),
              ('memory',MEMORY),('file_error',ERROR),('write_error',ALERT),('abort',ABORT)]


class StreamMachineTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        core_tests.CoreMachineTests.setUpClass.__func__(cls)
        bridge=Path(cls.temp.name)/'stream_bridge.so'
        subprocess.run(['cc','-std=c89','-pedantic','-Wall','-Wextra','-Werror','-O2',
                        '-shared','-fPIC','-I',str(ROOT/'reconstructed/include'),
                        str(ROOT/'tests/stream_abort_bridge.c'),str(Path(cls.temp.name)/'core.so'),
                        '-o',str(bridge)],check=True)
        cls.bridge=C.CDLL(str(bridge))
        cls.bridge.stream_test_run.argtypes=[PTR,C.c_int,C.c_uint16,C.c_uint16,C.POINTER(Api),C.c_void_p]
        cls.lib.stars_write_bof.argtypes=[PTR,C.c_int16,C.c_int16,C.c_int16,TICK,TRANSFER,C.c_void_p]
        cls.lib.stars_write_bof.restype=None

    @classmethod
    def tearDownClass(cls):cls.temp.cleanup()

    def setUp(self):
        self.machine=StreamMachine(helper_boundaries=True)
        self.initial=bytearray(self.machine.get(0,65536))
        self.initial[0x99c:0x9a0]=bytes(4)
        struct.pack_into('<H',self.initial,0x6e,0x2800)
        self.initial[0x3000:0x300b]=b'C:\\TEST.R1\0'

    def compare(self,operation,offset=0x3000,count=0,initial=None,opens=None,ticks=None,
                result=None,close_result=0):
        memory=bytearray(self.initial if initial is None else initial)
        host=(C.c_ubyte*65536).from_buffer(memory)
        extra=(C.c_ubyte*65536)(*(i&255 for i in range(65536)))
        machine=self.machine
        machine.put(0,bytes(memory));machine.cpu.mem_write(0x50000,bytes(extra))
        machine.events.clear();machine.outgoing.clear();machine.position=0
        machine.opens=list(opens if opens is not None else [(7,0)])
        machine.ticks=list(ticks if ticks is not None else [])
        machine.close_result=close_result
        machine.read_result=result;machine.write_result=result
        incoming=bytes(i&255 for i in range(count))
        machine.incoming=incoming
        host_opens=list(machine.opens);host_ticks=list(machine.ticks)
        events=[];outgoing=bytearray();position=[0]
        def open_file(_,name,mode,error):
            handle,err=host_opens.pop(0);error[0]=err
            events.append(('open',C.string_at(name),mode,handle,err));return handle
        def close_file(_,handle):
            events.append(('close',handle&65535,close_result));return close_result
        def read(_,handle,p,n):
            ret=n if result is None else result
            actual=min(ret,n) if ret!=65535 else 0
            if actual:C.memmove(p,incoming[position[0]:position[0]+actual],actual)
            position[0]+=actual;events.append(('read',handle&65535,n,ret));return ret
        def write(_,handle,p,n):
            ret=n if result is None else result
            actual=min(ret,n) if ret!=65535 else 0
            outgoing.extend(C.string_at(p,actual));events.append(('write',handle&65535,n,ret));return ret
        def tick(_):
            value=host_ticks.pop(0);events.append(('tick',value));return value
        def resolve(_,selector,address):
            self.assertIn(selector,[0x3000,0x5000])
            return C.addressof(host if selector==0x3000 else extra)+address
        api=Api(OPEN(open_file),CLOSE(close_file),READ(read),WRITE(write),TICK(tick),MEMORY(resolve),
                ERROR(lambda _,id:events.append(('file_error',id))),
                ALERT(lambda _:events.append(('write_error',))),ABORT())
        aborted=self.bridge.stream_test_run(host,operation,offset,count,C.byref(api),None)
        if aborted:events.append(('abort',C.c_uint16.in_dll(self.bridge,'stream_test_abort_environment').value,
                                  C.c_int16.in_dll(self.bridge,'stream_test_abort_value').value))
        addresses=[0x52ae,0x53cc,0x53f4,0x9554]
        args=struct.pack('<HH',offset,count) if operation==0 else b'' if operation==1 else struct.pack('<3H',offset,0x3000,count)
        original_aborted=False
        try:machine.call(addresses[operation],args,segment=15,shared_stack=True)
        except StreamAbort:original_aborted=True
        self.assertEqual(bool(aborted),original_aborted)
        self.assertEqual(events,machine.events)
        self.assertEqual(bytes(outgoing),bytes(machine.outgoing))
        self.assertEqual(position[0],machine.position)
        self.assertEqual(bytes(memory[:0xe000]),machine.get(0,0xe000))
        return memory,events

    def test_stream_open_modes_handles_and_suppressed_error(self):
        for mode,handle in itertools.product([0,0x20,0x1012,0x4000,0x5022,0x8000,0xffff],[0,7,32767,-32768,-2]):
            self.compare(0,count=mode,opens=[(handle,0)])
        for mode in [0,0x20,0x1012,0x4000,0xffff]:
            for error in [2,5,32,65535]:self.compare(0,count=mode,opens=[(-1,error)])

    def test_stream_open_retry_clock_and_wrap(self):
        memory=bytearray(self.initial);memory[0x7cb]|=2
        cases=[([(-1,2)],[]),
               ([(-1,32),(7,0)],[100,200,599,600]),
               ([(-1,32),(7,0)],[65500,65535,65999,66000]),
               ([(-1,32),(-1,5),(7,0)],[100,600,1000,1499,1500]),
               ([(-1,32),(-1,32)],[100,600,4100]),
               ([(-1,32)],[0xffffffff]),
               ([(-1,32)],[0xfffff060]),
               ([(-1,32),(7,0)],[0,500])]
        for opens,ticks in cases:
            for mode in [0x20,0x4020]:self.compare(0,count=mode,initial=memory,opens=opens,ticks=ticks)

    def test_stream_close_idempotence_and_ignored_errors(self):
        for handle,result in itertools.product([0,7,0x7fff,0x8000,0xfffe,0xffff],[0,-1,1]):
            memory=bytearray(self.initial);struct.pack_into('<H',memory,0xa22,handle)
            after,_=self.compare(1,initial=memory,close_result=result)
            self.assertEqual(after[0xa22:0xa24],b'\xff\xff')
            self.compare(1,initial=after,close_result=result)

    def test_stream_io_exact_short_error_and_zero(self):
        for operation,count in itertools.product([2,3],[0,1,2,3,16,1023,4095]):
            for result in {count,max(0,count-1),65535}:
                self.compare(operation,count=count,result=result)

    def test_stream_memory_forward_words_overlap_and_pointer_alias(self):
        for source,destination,count in [(0x3400,0x3000,17),(0x3000,0x3000,16),
                                          (0x3000,0x3001,17),(0x3000,0x3002,17),
                                          (0x3002,0x3000,17),(0x3400,0x99b,7),
                                          (0x3400,0x99c,4),(0,0x3000,15)]:
            memory=bytearray(self.initial)
            memory[source:source+count]=bytes(range(count))
            struct.pack_into('<HH',memory,0x99c,source,0x3000)
            self.compare(2,offset=destination,count=count,initial=memory)
        memory=bytearray(self.initial);struct.pack_into('<HH',memory,0x99c,0xfff0,0x5000)
        after,_=self.compare(2,count=16,initial=memory)
        self.assertEqual(after[0x99c:0x9a0],struct.pack('<HH',0,0x5000))

    def test_bof_clock_rng_flags_and_unused_multi(self):
        machine=self.machine
        rng=random.Random(270022)
        cases=list(itertools.product([-32768,-1,0,15,16,31,32,32767],
                                      [-32768,-1,0,2,5,258,32767],[0,0xffff,0xffffffff]))
        for i,(player,kind,tick) in enumerate(cases):
            memory=bytearray(self.initial)
            struct.pack_into('<I',memory,0x70,rng.getrandbits(32))
            struct.pack_into('<HH',memory,0x80,rng.getrandbits(16),rng.getrandbits(16))
            struct.pack_into('<I',memory,0x7ca,rng.getrandbits(32))
            struct.pack_into('<II',memory,0x1370,rng.getrandbits(32),rng.getrandbits(32))
            if i%4==0:memory[0xa1c:0xa20]=b'X\0YZ'
            machine.put(0,bytes(memory));machine.outgoing.clear();machine.events.clear();machine.ticks=[tick]
            multi=[-32768,0,1,32767][i%4]
            output=bytearray();observed=[]
            def get_tick(_):
                observed.append(bytes(memory[0x1370:0x1378]));return tick
            callback=TICK(get_tick);writer=TRANSFER(lambda _,p,n:output.extend(C.string_at(p,n)))
            self.lib.stars_write_bof((C.c_ubyte*65536).from_buffer(memory),player,kind,multi,callback,writer,None)
            machine.call(0x8ea4,struct.pack('<hhh',player,kind,multi),segment=15,shared_stack=True)
            self.assertEqual(bytes(output),bytes(machine.outgoing),(player,kind,tick))
            self.assertEqual(bytes(memory[:0xe000]),machine.get(0,0xe000))
            self.assertEqual(observed,[bytes(memory[0x1370:0x1378])])
            self.assertEqual(machine.events[0],('tick',tick))

    def test_original_complete_save_load_and_noncanonical_checksum(self):
        # All game/CRT callees execute unchanged in this integration observation.
        # OS choices are synthetic; password UI is not invoked.
        for dirty in [None,134,84]:
            machine=StreamMachine()
            memory=bytearray(machine.get(0,65536))
            memory[0x99c:0x9a0]=bytes(4)
            struct.pack_into('<HH',memory,0x74c,1,0)
            memory[0x3000:0x300b]=b'C:\\TEST.R1\0'
            memory[0x3200:0x32c0]=bytes(192)
            memory[0x3206]=7
            memory[0x3280:0x3286]=b'Golem\0'
            memory[0x32a0:0x32a7]=b'Golems\0'
            if dirty is not None:memory[0x3200+dirty]=0x55 if dirty==134 else 8
            machine.put(0,bytes(memory));machine.ticks=[0x12345678]
            result=machine.call(0x58d4,struct.pack('<HH',0x3000,0x3200),segment=29,shared_stack=True)&65535
            self.assertEqual(result,1)
            self.assertEqual(len(machine.dialogs),1)
            dialog=machine.dialogs[0]['structure']
            self.assertEqual(struct.unpack_from('<I',dialog,0)[0],72)
            self.assertEqual(struct.unpack_from('<I',dialog,48)[0],0x8806)
            wire=bytes(machine.outgoing)
            self.assertEqual(machine.get(0x1318,9),b'SYNTH.R1\0')
            machine.incoming=wire;machine.position=0;machine.opens=[(7,0)]
            result=machine.call(0x5da8,struct.pack('<HH',0x3000,0),segment=5,shared_stack=True)&65535
            self.assertEqual(result,1 if dirty is None else 0)
            self.assertEqual(machine.get(0xa22,2),b'\xff\xff')
            self.assertEqual(machine.get(0x74c,2),b'\x01\0')
            self.assertEqual(machine.get(0x6e,2),memory[0x6e:0x70])
            if dirty is None:self.assertEqual(machine.get(0x4f9a,192),machine.get(0x3200,192))

    def test_original_save_dialog_cancel_preserves_player_and_stream(self):
        machine=StreamMachine();machine.dialog_accept=False
        before=machine.get(0,0xe000)
        result=machine.call(0x58d4,struct.pack('<HH',0,0x3200),segment=29,shared_stack=True)&65535
        self.assertEqual(result,0)
        self.assertEqual(machine.events,[])
        self.assertEqual(machine.get(0x3200,192),before[0x3200:0x32c0])
        self.assertEqual(machine.get(0xa22,2),before[0xa22:0xa24])

    def test_original_loader_version_type_magic_and_failure_cleanup(self):
        creator=StreamMachine()
        player=bytearray(192);player[6]=7
        player[128:134]=b'Golem\0';player[160:167]=b'Golems\0'
        creator.put(0x3200,bytes(player));creator.ticks=[123]
        creator.call(0x58d4,struct.pack('<HH',0,0x3200),segment=29,shared_stack=True)
        wire=bytes(creator.outgoing)
        cases=[]
        for major,minor in [(1,83),(2,48),(2,49),(2,54),(2,55),(2,83),(2,84),(3,0)]:
            stream=bytearray(wire);struct.pack_into('<H',stream,10,(major<<12)|(minor<<5))
            cases.append((stream,1 if major==2 and 49<=minor<84 else 65535))
        stream=bytearray(wire);stream[2:6]=b'XXXX';cases.append((stream,1))
        stream=bytearray(wire);stream[16]=4;cases.append((stream,0))
        stream=bytearray(wire);stream[-1]^=1;cases.append((stream,0))
        stream=bytearray(wire);struct.pack_into('<H',stream,0,16);cases.append((stream,65535))
        # A first record of the right version/type is still insufficient if the
        # second type is not player or the final type is not EOF.
        stream=bytearray(wire);header=struct.unpack_from('<H',stream,18)[0]
        struct.pack_into('<H',stream,18,(header&1023)|(7<<10));cases.append((stream,0))
        stream=bytearray(wire);struct.pack_into('<H',stream,len(stream)-4,(1<<10)|2);cases.append((stream,0))
        for stream,expected in cases:
            machine=StreamMachine();machine.incoming=bytes(stream)
            machine.put(0x99c,bytes(4));machine.put(0x74c,b'\x01\0')
            machine.put(0x6e,struct.pack('<H',0x2800));machine.put(0x3000,b'SYNTH.R1\0')
            before=machine.get(0x4f9a,192)
            result=machine.call(0x5da8,struct.pack('<HH',0x3000,1),segment=5,shared_stack=True)&65535
            self.assertEqual(result,expected)
            self.assertEqual(machine.get(0x6e,2),struct.pack('<H',0x2800))
            self.assertEqual(machine.get(0x74c,2),b'\x01\0')
            self.assertEqual(machine.get(0xa22,2),b'\xff\xff')

            self.assertEqual(machine.get(0x4f9a,192),bytes(player) if expected==1 else before)
        for fail_open in [False,True]:
            machine=StreamMachine();machine.put(0x99c,bytes(4));machine.put(0x74c,b'\x01\0')
            machine.put(0x6e,struct.pack('<H',0x2800));machine.put(0x3000,b'SYNTH.R1\0')
            if fail_open:machine.opens=[(-1,2)]
            else:machine.read_result=0
            result=machine.call(0x5da8,struct.pack('<HH',0x3000,0),segment=5,shared_stack=True)&65535
            self.assertEqual(result,0)
            self.assertEqual(machine.get(0x74e,2),struct.pack('<H',4 if fail_open else 3))
            self.assertEqual(machine.get(0x6e,2),struct.pack('<H',0x2800))
            self.assertEqual(machine.get(0xa22,2),b'\xff\xff')

    def test_demo_clock_changes_encoding_without_changing_player(self):
        from tools.race_demo import generate
        first=generate(0,True,0);second=generate(0,True,1)
        for output in [first,second]:
            record=output['record_roundtrip']
            self.assertTrue(record['player_bytes_equal'])
            self.assertTrue(record['checksum_matches'])
            self.assertEqual(record['changed_player_offsets'],[])
        self.assertNotEqual(first['record_roundtrip']['stream_sha256'],second['record_roundtrip']['stream_sha256'])
        self.assertEqual(first['record_roundtrip']['rng_after_bof'],second['record_roundtrip']['rng_after_bof'])
        self.assertEqual(first['name'],second['name'])
