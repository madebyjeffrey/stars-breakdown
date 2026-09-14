"""Integrated race dependencies compared with the unmodified instructions."""
import ctypes as C
import itertools
from pathlib import Path
import random
import struct
import subprocess
import tempfile
import unittest
from tools.build_core import ROOT, SOURCES
from tools.race_runtime_machine import RaceRuntimeMachine
from tests.test_stream_machine import Api as Streams
from tests.test_file_machine import Alerts

PTR=C.POINTER(C.c_ubyte)
class WindowCall(C.Structure):
    _fields_=[('operation',C.c_char_p),('window',C.c_uint16),('a',C.c_uint16),
              ('b',C.c_uint16),('value',C.c_uint32),('p',C.c_void_p),('q',C.c_void_p)]
class SaveChoice(C.Structure):
    _fields_=[('owner',C.c_uint16),('file_offset',C.c_uint16),('flags',C.c_uint32),
              ('filter_index',C.c_uint32),('file',PTR),('title',PTR),('directory',PTR),
              ('filter',PTR),('extension',PTR),('file_capacity',C.c_uint16),('title_capacity',C.c_uint16)]
class Runtime(C.Structure):pass
class Platform(C.Structure):
    _fields_=[(n,C.c_void_p) for n in ['open','close','read','write','ticks','memory','access',
              'seek','date','time','timer_count','window','dialog','save_choice','abort']]
Runtime._fields_=[('data',PTR),('strings',PTR),('platform',C.POINTER(Platform)),('context',C.c_void_p),
                 ('stream',Streams),('alerts',Alerts),('bottom',C.c_uint16),('top',C.c_uint16),
                 ('recovery',C.c_void_p),('segments',PTR*38)]
TYPES={
    'open':(C.c_int16,PTR,C.c_uint16,C.POINTER(C.c_uint16)),
    'close':(C.c_int16,C.c_int16),'read':(C.c_uint16,C.c_int16,PTR,C.c_uint16),
    'write':(C.c_uint16,C.c_int16,PTR,C.c_uint16),'ticks':(C.c_uint32,),
    'memory':(C.c_void_p,C.c_uint16,C.c_uint16),'access':(C.c_int16,PTR,C.c_int16),
    'seek':(C.c_uint32,C.c_int16,C.c_uint32,C.c_int16),'date':(None,PTR),'time':(None,PTR),
    'timer_count':(C.c_uint32,), 'window':(C.c_uint32,C.POINTER(WindowCall)),
    'dialog':(C.c_int16,C.POINTER(Runtime),C.c_uint16,C.c_uint16,C.c_void_p),
    'save_choice':(C.c_int16,C.POINTER(SaveChoice)), 'abort':(None,C.c_uint16,C.c_int16)}

class Host:
    def __init__(self,lib,machine,initial=None):
        self.lib=lib;self.machine=machine;self.events=[];self.window_events=[]
        self.data=(C.c_ubyte*65536).from_buffer_copy(initial or machine.get(0,65536))
        self.strings=(C.c_ubyte*65536).from_buffer_copy(bytes(machine.cpu.mem_read(0x90000,65536)))
        self.util=(C.c_ubyte*65536).from_buffer_copy(bytes(machine.cpu.mem_read(0x10000,65536)))
        self.platform=Platform();self.callbacks={};self.runtime=Runtime();self.position=0;self.outgoing=bytearray()
        self.opens=list(machine.opens);self.ticks=list(machine.ticks);self.accesses=list(machine.access_results)
        self.timers=list(machine.timer_values);self.writes=list(machine.write_results) if machine.write_results is not None else None
        self.reads=list(machine.read_results) if machine.read_results is not None else None
        self.errors=[]
        for name,types in TYPES.items():
            fn=getattr(self,'do_'+name)
            def checked(ctx,*args,_fn=fn,_name=name):
                try:return _fn(*args)
                except BaseException as e:
                    self.errors.append((_name,e))
                    return 0
            callback=C.CFUNCTYPE(types[0],C.c_void_p,*types[1:])(checked)
            self.callbacks[name]=callback;setattr(self.platform,name,C.cast(callback,C.c_void_p).value)
        lib.stars_runtime_init(C.byref(self.runtime),self.data,self.strings,C.byref(self.platform),None,0xe000,0xff00)
        self.runtime.segments[9]=self.util
    def do_open(self,name,mode,error):
        h,e=self.opens.pop(0);error[0]=e;self.events.append(('open',C.string_at(name),mode,h,e));return h
    def do_close(self,h):self.events.append(('close',h&65535,self.machine.close_result));return self.machine.close_result
    def do_read(self,h,out,n):
        result=self.reads.pop(0) if self.reads is not None else self.machine.read_result
        if result is None:result=n
        size=min(n,result) if result!=65535 else 0;payload=self.machine.incoming[self.position:self.position+size]
        assert len(payload)==size
        if payload:C.memmove(out,payload,len(payload))
        self.position+=size;self.events.append(('read',h&65535,n,result));return result
    def do_write(self,h,p,n):
        result=self.writes.pop(0) if self.writes is not None else self.machine.write_result
        if result is None:result=n
        size=min(n,result) if result!=65535 else 0;self.outgoing.extend(C.string_at(p,size))
        self.events.append(('write',h&65535,n,result));return result
    def do_ticks(self):
        value=self.ticks.pop(0);self.events.append(('tick',value));return value
    def do_memory(self,seg,off):
        assert seg==0x3000
        return C.addressof(self.data)+off
    def do_access(self,p,mode):
        value=self.accesses.pop(0);self.events.append(('access',C.string_at(p),mode,value));return value
    def do_seek(self,h,offset,origin):
        self.events.append(('seek',h,offset,origin,self.machine.seek_result));return self.machine.seek_result
    def do_date(self,p):C.memmove(p,self.machine.date+b'\0',len(self.machine.date)+1);self.events.append(('date',self.machine.date))
    def do_time(self,p):C.memmove(p,self.machine.time+b'\0',len(self.machine.time)+1);self.events.append(('time',self.machine.time))
    def do_timer_count(self):
        value=self.timers.pop(0);self.events.append(('timer',value));return value
    def do_dialog(self,r,resource,owner,proc):
        assert proc
        self.window_events.append(('DialogBox',resource,owner));return self.machine.dialog_result
    def do_save_choice(self,choice):
        c=choice.contents
        assert c.flags==0x8806 and c.filter_index==1 and c.file_capacity==256 and c.title_capacity==256
        expected=b'Stars! Race Files\0*.R*\0\0'
        assert C.string_at(c.filter,len(expected))==expected
        if self.machine.dialog_accept:
            path=self.machine.dialog_path
            C.memmove(c.file,path+b'\0',len(path)+1);c.file_offset=path.rfind(b'\\')+1
        return int(self.machine.dialog_accept)
    def do_abort(self,token,value):raise AssertionError(('unhandled recovery',token,value))
    def do_window(self,p):
        c=p.contents;n=c.operation.decode();h,a,b,v=c.window,c.a,c.b,c.value
        if n=='GetFocus':self.events.append(('focus',self.machine.focus));return self.machine.focus
        if n=='MessageBox':
            self.events.append(('message',h,C.string_at(c.p),C.string_at(c.q),a));return self.machine.message_result&65535
        if n=='GetDlgItem':self.window_events.append((n,h,a));return a
        if n=='SendDlgItemMessage':
            wp=C.cast(c.p,C.POINTER(C.c_uint16))[0];self.window_events.append((n,h,a,b,wp,v));return 0
        if n=='GetDlgItemText':
            text=self.machine.control_text.get(a,b'')[:b-1];C.memmove(c.p,text+b'\0',len(text)+1)
            self.window_events.append((n,h,a,b,text));return len(text)
        if n=='SetWindowText':self.window_events.append((n,h,C.string_at(c.p)));return 1
        if n=='GetClientRect':C.memmove(c.p,struct.pack('<4h',0,0,400,300),8);self.window_events.append((n,h));return 1
        if n=='FillRect':self.window_events.append((n,h,a,struct.unpack('<4h',C.string_at(c.p,8))));return 1
        if n=='SetBkColor':self.window_events.append((n,h,v));return 0
        if n=='SetFocus':self.window_events.append((n,h));return 0
        if n=='EndDialog':self.window_events.append((n,h,a));return 1
        if n=='WinHelp':self.window_events.append((n,h,a,v,C.string_at(c.p)));return 1
        raise AssertionError(('unmodelled window API',n))

class RaceRuntimeTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp=tempfile.TemporaryDirectory();library=Path(cls.temp.name)/'race.so'
        subprocess.run(['cc','-std=c89','-pedantic','-Wall','-Wextra','-Werror','-shared','-fPIC','-O2',
            '-I',str(ROOT/'reconstructed/include'),*[str(ROOT/'reconstructed/src'/s) for s in SOURCES],
            str(ROOT/'tests/race_runtime_abort_bridge.c'),'-o',str(library)],check=True)
        cls.lib=C.CDLL(str(library))
        signatures={
            'runtime_init':(None,C.POINTER(Runtime),PTR,PTR,C.POINTER(Platform),C.c_void_p,C.c_uint16,C.c_uint16),
            'password_salt':(C.c_uint32,C.c_void_p),'delay':(None,C.c_int16,C.c_void_p,C.c_void_p),
            'version_string':(C.c_uint16,PTR,PTR),'format_id':(C.c_uint16,C.POINTER(Runtime),C.c_int16,C.c_void_p),
            'format_string':(C.c_uint16,C.POINTER(Runtime),C.c_uint16,C.c_void_p),
            'check_password':(C.c_int16,C.POINTER(Runtime)),
            'password_dialog':(C.c_int16,C.POINTER(Runtime),C.c_uint16,C.c_uint16,C.c_uint16,C.c_uint32),
            'output_file_string':(None,C.POINTER(Runtime),C.c_void_p,C.c_void_p),
            'output_string':(None,C.POINTER(Runtime),C.c_int16,C.c_void_p),
            'save_race':(C.c_int16,C.POINTER(Runtime),C.c_uint16,C.c_uint16),
            'was_race_file':(C.c_int16,C.POINTER(Runtime),C.c_uint16,C.c_int16),
            'environment_string':(C.c_uint16,C.POINTER(Runtime),C.c_int16,C.c_int16)}
        for n,t in signatures.items():
            f=getattr(cls.lib,'stars_'+n);f.restype=t[0];f.argtypes=t[1:]
    @classmethod
    def tearDownClass(cls):cls.temp.cleanup()
    def setUp(self):
        self.m=RaceRuntimeMachine()
        self.m.put(0x229a,b'\0\0');self.m.put(0x74c,b'\0'*4);self.m.put(0x99c,b'\0'*4)
        self.m.put(0x6e,struct.pack('<H',0x2800));self.m.put(0x7ca,b'\0'*8)
        self.m.put(0x3000,b'C:\\SYNTH.R1\0');self.m.put(0x56a2,b'SYNTH\0')
        self.m.put(0x4f9a,self.m.get(0xda2,192));self.m.put(0x501a,bytes(64))
        self.m.put(0x501a,b'Synthetic\0');self.m.put(0x503a,b'Synthetics\0')
        self.m.put(0x4fa6,bytes(4));self.m.put(0x35c,bytes(8))
    def compare(self,h):
        self.assertEqual(h.errors,[])
        self.assertEqual(h.events,self.m.events)
        self.assertEqual(h.window_events,self.m.window_events)
        actual=bytes(h.data[:0xe000]);expected=self.m.get(0,0xe000)
        if actual!=expected:
            diffs=[(hex(i),a,b) for i,(a,b) in enumerate(zip(actual,expected)) if a!=b]
            self.fail(str(diffs[:30]))
    def test_password_salt_signed_bytes_and_wrap(self):
        rng=random.Random(27003)
        cases=[b'',b'A',b'AB',b'ABC',b'ABCD',bytes([128,255]),bytes([1,128,128]),b'X'*1000]
        cases += [bytes(rng.randrange(1,256) for _ in range(rng.randrange(1,128))) for _ in range(400)]
        for value in cases:
            self.m.put(0x3000,value+b'\0')
            self.assertEqual(self.lib.stars_password_salt(value),self.m.call(0x59ce,struct.pack('<H',0x3000)),value)
    def test_delay_unsigned_deadline_and_tick_wrap(self):
        for count,times in [(0,[10,10]),(10,[10,10,19,20]),(10,[0xfffffffd,0xfffffffe]),
                            (10,[0xffffffff,0]),(-1,[10,10]),(-32768,[0,100,0xffff8000])]:
            self.m.events.clear();self.m.timer_values=list(times);h=Host(self.lib,self.m)
            self.lib.stars_delay(count,h.callbacks['timer_count'],None)
            self.m.call(0x3a3e,struct.pack('<h',count),segment=30,shared_stack=True)
            self.compare(h)
    def test_version_and_all_race_error_templates(self):
        h=Host(self.lib,self.m);actual=self.lib.stars_version_string(h.data,h.strings)
        original=self.m.call(0x1212,segment=4,shared_stack=True)&65535
        self.assertEqual(actual,original);self.compare(h)
        for id in [3,4,13,22,0x10b,0xea]:
            h=Host(self.lib,self.m);actual=self.lib.stars_format_id(C.byref(h.runtime),id,None)
            original=self.m.call(0x924c,struct.pack('<hHH',id,0,0),segment=7,shared_stack=True)&65535
            self.assertEqual(actual,original);self.compare(h)
    def test_check_password_shortcuts_owner_and_raw_dialog_result(self):
        for salt,last,ai,default,flags,owner,result in [
            (0,1,0,b'',0,0,0),(1,1,0,b'',0,0,0),(2,1,1,b'',0,0,0),
            (self.lib.stars_password_salt(b'test'),0,0,b'test',0,0,0),
            (2,1,0,b'',0x4000,0,0),(2,1,0,b'x',0,0,-1),(2,1,0,b'',0,123,1)]:
            self.m.put(0x35c,struct.pack('<II',salt,last));self.m.put(0x1442,struct.pack('<H',ai))
            self.m.put(0x2548,default+b'\0');self.m.put(0x229a,struct.pack('<H',flags))
            self.m.put(0x352,struct.pack('<H',owner));self.m.dialog_result=result
            self.m.events.clear();self.m.window_events.clear();h=Host(self.lib,self.m)
            actual=self.lib.stars_check_password(C.byref(h.runtime))
            original=C.c_int16(self.m.call(0x58d8,shared_stack=True)&65535).value
            self.assertEqual(actual,original);self.compare(h)
    def test_password_dialog_events_success_failure_cache_and_cancel(self):
        for msg,wp,lp,password,failures,flags in [
            (0x110,0,0,b'',0,0),(0x14,25,0,b'',0,0),(0x19,25,6<<16,b'',0,0),
            (0x19,25,5<<16,b'',0,0),(0x111,2,0,b'',0,0),(0x111,0x76,0,b'',0,0),
            (0x111,1,0,b'test',0,0),(0x111,1,0,b'test',0,0x200),
            *[(0x111,1,0,b'wrong',f,0) for f in [0,8,9,98,99,32767,65534,65535]]]:
            self.m.put(0x35c,struct.pack('<II',self.lib.stars_password_salt(b'test'),0))
            self.m.put(0x140e,struct.pack('<H',failures));self.m.put(0x7ce,struct.pack('<H',flags))
            self.m.control_text[0x10c]=password;self.m.timer_values=[0,20000]
            self.m.events.clear();self.m.window_events.clear();h=Host(self.lib,self.m)
            actual=self.lib.stars_password_dialog(C.byref(h.runtime),100,msg,wp,lp)&65535
            original=self.m.dialog_call(0x5a72,100,msg,wp,lp)
            self.assertEqual(actual,original);self.compare(h)
    def test_log_new_existing_missing_and_short_write(self):
        for existing,opens,write_result in [(True,[(7,0)],None),(False,[(7,0),(7,0)],None),
                                          (False,[(-1,5),(-1,5)],None),(True,[(7,0)],0)]:
            self.m.access_results=[0,0] if existing else [-1,-1,0]
            self.m.opens=list(opens);self.m.write_result=write_result
            self.m.events.clear();self.m.window_events.clear();self.m.outgoing.clear()
            self.m.put(0x3100,b'Synthetic message\0');h=Host(self.lib,self.m)
            self.lib.stars_output_string(C.byref(h.runtime),7,C.byref(h.data,0x3100))
            self.m.call(0x7fe6,struct.pack('<hHH',7,0x3100,0x3000),segment=8,shared_stack=True)
            self.compare(h);self.assertEqual(h.outgoing,self.m.outgoing)
    def save_case(self,accept=True,opens=None):
        self.m.dialog_accept=accept;self.m.opens=opens or [(7,0)];self.m.ticks=[123]
        self.m.events.clear();self.m.window_events.clear();self.m.outgoing.clear();h=Host(self.lib,self.m)
        actual=self.lib.stars_save_race(C.byref(h.runtime),0x3000,0x4f9a)
        original=self.m.call(0x58d4,struct.pack('<HH',0x3000,0x4f9a),segment=29,shared_stack=True)&65535
        self.assertEqual(actual,original);self.compare(h);self.assertEqual(h.outgoing,self.m.outgoing)
        self.assertEqual(h.runtime.top,0xff00)
        return bytes(h.outgoing)
    def test_save_load_cancel_and_create_failure(self):
        self.save_case(False);self.save_case(True,[(-1,2)]);payload=self.save_case()
        self.m.incoming=payload;self.m.position=0;self.m.opens=[(7,0)]
        self.m.events.clear();self.m.window_events.clear();h=Host(self.lib,self.m)
        actual=self.lib.stars_was_race_file(C.byref(h.runtime),0x3000,1)
        original=C.c_int16(self.m.call(0x5da8,struct.pack('<Hh',0x3000,1),segment=5,shared_stack=True)&65535).value
        self.assertEqual(actual,original);self.assertEqual(actual,1);self.compare(h)
        self.assertEqual(h.runtime.top,0xff00)
    def load_case(self,payload,check=1,reads=None,opens=None,silent=0,dialog=0):
        self.setUp();self.m.incoming=payload;self.m.position=0;self.m.opens=opens or [(7,0)]
        self.m.read_results=reads;self.m.put(0x74c,struct.pack('<H',silent));self.m.dialog_result=dialog
        self.m.put(0x22e0,b'cached\0');h=Host(self.lib,self.m)
        actual=self.lib.stars_was_race_file(C.byref(h.runtime),0x3000,check)
        expected=C.c_int16(self.m.call(0x5da8,struct.pack('<Hh',0x3000,check),segment=5,shared_stack=True)&65535).value
        self.assertEqual(actual,expected);self.compare(h);self.assertEqual(h.runtime.top,0xff00)
        return actual
    def test_load_invalid_headers_records_checksums_and_truncation(self):
        payload=self.save_case();variants=[]
        for version in [0,0x1000|(60<<5),0x2000|(48<<5),0x2000|(49<<5),0x2000|(83<<5),0x2000|(84<<5)]:
            p=bytearray(payload);struct.pack_into('<H',p,10,version);variants.append(('version'+str(version),bytes(p)))
        for label,offset,mask in [('header_type',1,4),('file_type',16,1),('player_type',19,4),('eof_type',len(payload)-3,4),('checksum',len(payload)-1,1)]:
            p=bytearray(payload);p[offset]^=mask;variants.append((label,bytes(p)))
        for label,p in variants:
            for silent in [0,1]:
                with self.subTest(case=label,silent=silent):self.load_case(p,silent=silent)
        count=struct.unpack_from('<H',payload,18)[0]&1023;requests=[2,16,2,count,2,2]
        for length in [0,1,2,17,18,19,20,len(payload)-3,len(payload)-2,len(payload)-1]:
            remaining=length;reads=[]
            for n in requests:
                actual=min(remaining,n);reads.append(actual);remaining-=actual
                if actual!=n:break
            with self.subTest(truncated=length):self.load_case(payload[:length],reads=reads)
        self.assertEqual(self.load_case(payload,opens=[(-1,2)]),0)
        for index in range(len(requests)):
            self.load_case(payload,reads=requests[:index]+[65535])
    def test_password_protected_load_outcomes_and_live_reference_fixture(self):
        self.m.put(0x4fa6,struct.pack('<I',self.lib.stars_password_salt(b'test')))
        payload=self.save_case()
        self.assertEqual(self.load_case(payload,check=0),1)
        self.assertEqual(self.load_case(payload,check=1,dialog=0),-1)
        self.assertEqual(self.load_case(payload,check=1,dialog=1),1)
        fixture=ROOT/'runs/step4/fixtures/race/STUDY.R1'
        if fixture.exists():self.assertEqual(self.load_case(fixture.read_bytes(),check=0),1)
    def test_header_write_failure_recovery(self):
        for writes in [[0],[2,0],[65535],[2,65535]]:
            self.setUp();self.m.write_results=writes;self.save_case()
    def test_player_and_eof_write_failures_reach_outer_recovery(self):
        from tools.stream_machine import StreamAbort
        from unicorn import UC_HOOK_CODE
        def catch_outer(cpu,address,size,unused):
            token,value=struct.unpack('<Hh',bytes(cpu.mem_read(self.m._stack()+4,4)))
            if token==0x2800:self.m.events.append(('abort',token,value));raise StreamAbort()
        self.lib.race_test_save.argtypes=[C.POINTER(Runtime),C.c_uint16,C.c_uint16]
        payload=self.save_case();count=struct.unpack_from('<H',payload,18)[0]&1023
        requests=[2,16,2,count,2,2]
        for at in range(2,6):
            for result in [0,65535]:
                self.setUp();self.m.write_results=requests[:at]+[result];self.m.ticks=[123]
                self.m.cpu.hook_add(UC_HOOK_CODE,catch_outer,begin=0x209ed,end=0x209ed)
                h=Host(self.lib,self.m)
                self.assertEqual(self.lib.race_test_save(C.byref(h.runtime),0x3000,0x4f9a),-2)
                h.events.append(('abort',C.c_uint16.in_dll(self.lib,'race_test_abort_token').value,
                                 C.c_int16.in_dll(self.lib,'race_test_abort_value').value))
                with self.assertRaises(StreamAbort):self.m.call(0x58d4,struct.pack('<HH',0x3000,0x4f9a),segment=29,shared_stack=True)
                self.compare(h);self.assertEqual(h.outgoing,self.m.outgoing)

if __name__=='__main__':unittest.main()
