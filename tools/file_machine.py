"""Filename, creation and alert oracle with explicit dependency boundaries.

FileError, AlertSz, stream code, filename code and CRT recovery run unchanged.
PszFormatIds and OutputSz can be replaced at their entries; Windows formatting
is restricted to verified static formats. No real filesystem or UI is used.
"""
import struct
from tools.stream_machine import StreamMachine, reg
from unicorn import UC_HOOK_CODE


class FileMachine(StreamMachine):
    def __init__(self, format_boundary=True):
        super().__init__(imports={'USER!#23':(0xe000,0x800),'USER!#1':(0xe000,0x900)})
        self.format_boundary=format_boundary
        self.focus=0x1234
        self.message_result=1
        self.environments=[]
        self.write_results=None
        self.cpu.mem_write(0xe0800,b'\xcb')
        self.cpu.mem_write(0xe0900,b'\xca\x0c\x00')
        self.cpu.hook_add(UC_HOOK_CODE,self._alert_api,begin=0xe0800,end=0xe0900)
        self.cpu.hook_add(UC_HOOK_CODE,self._output,begin=0xb7fe6,end=0xb7fe6)

    def _string(self,address):
        result=bytearray()
        for i in range(1024):
            byte=bytes(self.cpu.mem_read(address+i,1))
            if byte==b'\0':return bytes(result)
            result.extend(byte)
        raise AssertionError('Bounded synthetic string is not terminated')

    def _helper(self,cpu,address,size,unused):
        # In particular, the original FileError, AlertSz and longjmp execute.
        if address==0xd924c and self.format_boundary:
            id,off,seg=struct.unpack('<hHH',bytes(cpu.mem_read(self._stack()+4,6)))
            assert (off,seg)==(0,0)
            self.events.append(('format',id))
            cpu.reg_write(reg.UC_X86_REG_AX,0x2f00)
            self._return_far()

    def _output(self,cpu,address,size,unused):
        channel,off,seg=struct.unpack('<hHH',bytes(cpu.mem_read(self._stack()+4,6)))
        self.events.append(('output',channel,self._string(seg*16+off)))
        self._return_far()

    def _alert_api(self,cpu,address,size,unused):
        if address==0xe0800:
            self.events.append(('focus',self.focus))
            cpu.reg_write(reg.UC_X86_REG_AX,self.focus)
        elif address==0xe0900:
            kind,caption,cs,text,ts,owner=struct.unpack('<6H',bytes(cpu.mem_read(self._stack()+4,12)))
            self.events.append(('message',owner,self._string(ts*16+text),self._string(cs*16+caption),kind))
            cpu.reg_write(reg.UC_X86_REG_AX,self.message_result&65535)
        else:raise AssertionError(hex(address))

    def _api(self,cpu,address,size,unused):
        if address==0xe0300:
            stack=self._stack()
            dst,ds,fmt,fs=struct.unpack('<4H',bytes(cpu.mem_read(stack+4,8)))
            format=self._string(fs*16+fmt)
            if format in [b'%s.',b'Error: %s']:
                src,ss=struct.unpack('<HH',bytes(cpu.mem_read(stack+12,4)))
                payload=format%self._string(ss*16+src)
            elif format==b'%c%d':
                char,number=struct.unpack('<Hh',bytes(cpu.mem_read(stack+12,4)))
                payload=bytes([char&255])+str(number).encode('ascii')
            else:return super()._api(cpu,address,size,unused)
            cpu.mem_write(ds*16+dst,payload+b'\0')
            cpu.reg_write(reg.UC_X86_REG_AX,len(payload)&65535)
        else:
            if address in [0xe0100,0xe0200]:
                self.environments.append(struct.unpack('<H',self.get(0x6e,2))[0])
            if address==0xe0200 and self.write_results is not None:
                assert self.write_results,'Write result script exhausted'
                self.write_result=self.write_results.pop(0)
            super()._api(cpu,address,size,unused)

    def _extra_api(self,cpu,address,size,unused):
        if address in [0xe0400,0xe0600]:
            self.environments.append(struct.unpack('<H',self.get(0x6e,2))[0])
        super()._extra_api(cpu,address,size,unused)
