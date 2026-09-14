"""Filename, alert and creation comparisons against original x86 execution."""
import ctypes as C
import itertools
import struct
import unittest
from tests import test_core_machine as core_tests
from tests import test_stream_machine as stream_tests
from tools.file_machine import FileMachine

PTR=C.POINTER(C.c_ubyte)
FOCUS=C.CFUNCTYPE(C.c_uint16,C.c_void_p)
MESSAGE=C.CFUNCTYPE(C.c_int16,C.c_void_p,C.c_uint16,PTR,PTR,C.c_int16)
OUTPUT=C.CFUNCTYPE(None,C.c_void_p,C.c_int16,PTR)
FORMAT=C.CFUNCTYPE(C.c_uint16,C.c_void_p,C.c_int16)

class Alerts(C.Structure):
    _fields_=[('focus',FOCUS),('message',MESSAGE),('output',OUTPUT),('format_id',FORMAT)]


class FileMachineTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        core_tests.CoreMachineTests.setUpClass.__func__(cls)
        cls.lib.stars_set_file_name.argtypes=[PTR,C.c_uint16,C.c_int16]
        cls.lib.stars_set_file_name.restype=None
        cls.lib.stars_create_file.argtypes=[PTR,C.c_uint16,C.c_int16,C.c_uint16,C.c_uint16,
                                           C.POINTER(stream_tests.Api),C.c_void_p]
        cls.lib.stars_create_file.restype=C.c_int16
        cls.lib.stars_alert_string.argtypes=[PTR,C.c_uint16,C.c_int16,C.POINTER(Alerts),C.c_void_p]
        cls.lib.stars_alert_string.restype=C.c_int16
        cls.lib.stars_file_error.argtypes=[PTR,C.c_int16,C.POINTER(Alerts),C.c_void_p]
        cls.lib.stars_file_error.restype=None
        cls.lib.stars_bad_file_error.argtypes=[C.c_int16]
        cls.lib.stars_bad_file_error.restype=C.c_int16

    @classmethod
    def tearDownClass(cls):cls.temp.cleanup()

    def setUp(self):
        self.machine=FileMachine()
        self.initial=bytearray(self.machine.get(0,65536))
        self.initial[0x3000:0x300a]=b'SYNTH.R1\0\0'
        self.initial[0x2f00:0x2f10]=b'Synthetic error\0'
        self.initial[0x99c:0x9a0]=bytes(4)
        struct.pack_into('<HH',self.initial,0x74c,0,0)
        struct.pack_into('<H',self.initial,0x229a,0)
        struct.pack_into('<H',self.initial,0x6e,0x2800)

    def equal_data(self,memory):self.assertEqual(bytes(memory[:0xe000]),self.machine.get(0,0xe000))

    def alert_api(self,events):
        machine=self.machine
        def focus(_):events.append(('focus',machine.focus));return machine.focus
        def message(_,owner,text,caption,kind):
            events.append(('message',owner,C.string_at(text),C.string_at(caption),kind&65535))
            return machine.message_result
        def output(_,channel,text):events.append(('output',channel,C.string_at(text)))
        def format_id(_,id):events.append(('format',id));return 0x2f00
        return Alerts(FOCUS(focus),MESSAGE(message),OUTPUT(output),FORMAT(format_id))

    def test_file_name_paths_type_defaults_and_signed_player(self):
        names=[b'',b'STARS',b'STARS.R1',b'C:\\A.B\\STARS',b'C:\\A.B\\STARS.R1',
               b'.hidden',b'.',b'foo..bar',b'foo.',b'foo/bar.ext',b'foo.ext\\',
               b'C:\\a.b\\c.d\\e',b'\xe9game.r1',b'A'*255]
        for name,kind,player in itertools.product(names,[0,1,2,3,4,5,6,255,256,65535],
                                                 [-32768,-2,-1,0,15,32767]):
            memory=bytearray(self.initial);memory[0x56a2:0x56a2+len(name)+1]=name+b'\0'
            self.machine.put(0,bytes(memory))
            self.lib.stars_set_file_name((C.c_ubyte*65536).from_buffer(memory),kind,player)
            self.machine.call(0x8cfe,struct.pack('<Hh',kind,player),segment=15,shared_stack=True)
            self.equal_data(memory)

    def test_bad_file_error_all_signed_ids(self):
        before=self.machine.get(0,0xe000)
        for value in range(65536):
            id=C.c_int16(value).value
            original=self.machine.call(0x524e,struct.pack('<h',id),segment=15)&65535
            self.assertEqual(self.lib.stars_bad_file_error(id),original,id)
        self.assertEqual(self.machine.get(0,0xe000),before)

    def test_alert_flag_branches_text_boundaries_and_return_values(self):
        for flags,message,kind,result in itertools.product(
                [0,8,0x4000,0x4008,0x8000,0x8008,0xc000,0xc008,0xffff],
                [b'',b'Synthetic error',b'100% done',b'\xe9',b'X'*250],
                [0,16,-1],[0,1,6,-1]):
            memory=bytearray(self.initial);struct.pack_into('<H',memory,0x229a,flags)
            memory[0x3000:0x3000+len(message)+1]=message+b'\0'
            self.machine.put(0,bytes(memory));self.machine.events.clear();self.machine.message_result=result
            events=[];api=self.alert_api(events)
            actual=self.lib.stars_alert_string((C.c_ubyte*65536).from_buffer(memory),0x3000,kind,C.byref(api),None)
            original=C.c_int16(self.machine.call(0x2160,struct.pack('<Hh',0x3000,kind),shared_stack=True)&65535).value
            self.assertEqual(actual,original)
            self.assertEqual(events,self.machine.events)
            self.equal_data(memory)

    def test_file_error_always_records_id_and_conditionally_presents(self):
        for silent,flags,id,log in itertools.product([0,1,0x100,65535],[0,2,0xffff],
                                                    [-32768,-1,0,3,4,22,32767],[0,0x4000,0x8008]):
            memory=bytearray(self.initial)
            struct.pack_into('<H',memory,0x74c,silent);struct.pack_into('<H',memory,0x7ca,flags)
            struct.pack_into('<H',memory,0x229a,log)
            self.machine.put(0,bytes(memory));self.machine.events.clear()
            events=[];api=self.alert_api(events)
            self.lib.stars_file_error((C.c_ubyte*65536).from_buffer(memory),id,C.byref(api),None)
            self.machine.call(0x4a10,struct.pack('<h',id),segment=15,shared_stack=True)
            self.assertEqual(events,self.machine.events)
            self.equal_data(memory)

    def create_case(self,kind=5,player=-1,forced=True,opens=None,ticks=None,writes=None,log=0,silent=0):
        machine=self.machine
        memory=bytearray(self.initial)
        base=b'C:\\SYNTHETIC.GAME\0'
        memory[0x56a2:0x56a2+len(base)]=base
        self.assertEqual(len(memory),65536)
        struct.pack_into('<H',memory,0x229a,log);struct.pack_into('<H',memory,0x74c,silent)
        memory[0x7cb]|=2
        struct.pack_into('<II',memory,0x1370,12345678,87654321)
        machine.put(0,bytes(memory));machine.events.clear();machine.environments.clear();machine.outgoing.clear()
        machine.opens=list(opens if opens is not None else [(7,0)])
        machine.ticks=list(ticks if ticks is not None else [123])
        machine.write_results=list(writes if writes is not None else [None,None])
        host_opens=list(machine.opens);host_ticks=list(machine.ticks);host_writes=list(machine.write_results)
        events=[];environments=[];output=bytearray()
        host=(C.c_ubyte*65536).from_buffer(memory)
        alerts=self.alert_api(events)
        def observe():environments.append(struct.unpack_from('<H',memory,0x6e)[0])
        def open_file(_,name,mode,error):
            observe();handle,err=host_opens.pop(0);error[0]=err
            events.append(('open',C.string_at(name),mode,handle,err));return handle
        def write(_,handle,p,count):
            observe();result=host_writes.pop(0)
            if result is None:result=count
            actual=min(count,result) if result!=65535 else 0
            output.extend(C.string_at(p,actual));events.append(('write',handle&65535,count,result));return result
        def tick(_):observe();value=host_ticks.pop(0);events.append(('tick',value));return value
        def file_error(_,id):self.lib.stars_file_error(host,id,C.byref(alerts),None)
        def write_error(_):
            text=alerts.format_id(None,22)
            self.lib.stars_alert_string(host,text,16,C.byref(alerts),None)
        api=stream_tests.Api(stream_tests.OPEN(open_file),stream_tests.CLOSE(),stream_tests.READ(),
                             stream_tests.WRITE(write),stream_tests.TICK(tick),stream_tests.MEMORY(),
                             stream_tests.ERROR(file_error),stream_tests.ALERT(write_error),stream_tests.ABORT())
        # Direct original call: BP=FEFE and its env is BP-16h. Native host
        # setjmp uses the corresponding token; private jump-buffer bytes differ.
        actual=self.lib.stars_create_file(host,kind,player,0x3000 if forced else 0,0xfee8,C.byref(api),None)
        original=machine.call(0x8e16,struct.pack('<HhH',kind,player,0x3000 if forced else 0),segment=15,shared_stack=True)&65535
        self.assertEqual(actual,original)
        self.assertEqual(events,machine.events)
        self.assertEqual(environments,machine.environments)
        self.assertEqual(bytes(output),bytes(machine.outgoing))
        self.equal_data(memory)
        self.assertEqual(memory[0x6e:0x70],struct.pack('<H',0x2800))
        return actual,memory,events

    def test_create_file_forced_generated_names_and_success_state(self):
        for kind,player,forced in itertools.product([0,1,2,3,4,5,65535],[-32768,-1,0,32767],[False,True]):
            result,memory,events=self.create_case(kind,player,forced)
            self.assertEqual(result,1)
            self.assertEqual(memory[0xa22:0xa24],b'\x07\0')
            self.assertFalse(any(event[0]=='close' for event in events))

    def test_create_file_recovers_open_and_header_transfer_errors(self):
        for log,silent in itertools.product([0,0x4000,0x8008],[0,1]):
            result,memory,_=self.create_case(opens=[(-1,2)],ticks=[],writes=[],log=log,silent=silent)
            self.assertEqual(result,0);self.assertEqual(memory[0xa22:0xa24],b'\xff\xff')
            for writes in [[0],[1],[65535],[None,0],[None,15],[None,65535]]:
                result,memory,events=self.create_case(writes=writes,log=log,silent=silent)
                self.assertEqual(result,0)
                self.assertEqual(memory[0xa22:0xa24],b'\x07\0')
                self.assertFalse(any(event[0]=='close' for event in events))

    def test_create_file_retry_then_success_or_timeout(self):
        self.assertEqual(self.create_case(opens=[(-1,32),(7,0)],ticks=[100,600,123])[0],1)
        self.assertEqual(self.create_case(opens=[(-1,32),(-1,32)],ticks=[100,600,4100],writes=[])[0],0)
