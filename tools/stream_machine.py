"""Stream oracle with explicit OS models and optional game-error boundaries.

The five subjects execute original instructions. Unit tests may replace
FileError, PszFormatIds and AlertSz at their call entries, and capture longjmp
at its entry. With boundaries disabled, all game/CRT callees execute unchanged.
Only synthetic file names, bytes, clock values and dialog choices are supplied.
"""
import struct
from tools.record_machine import RecordMachine, reg
from unicorn import UC_HOOK_CODE


class StreamAbort(Exception):
    pass


class StreamMachine(RecordMachine):
    SEGMENTS = dict(RecordMachine.SEGMENTS)
    SEGMENTS.update({5: 0xc000, 7: 0xd000})

    def __init__(self, helper_boundaries=False):
        super().__init__({'KERNEL!#74': (0xe000,0x400), 'KERNEL!#81': (0xe000,0x500),
                          'USER!#13': (0xe000,0x600), 'COMMDLG!#2': (0xe000,0x700)})
        self.helper_boundaries = helper_boundaries
        self.events = []
        self.opens = [(7,0)]
        self.ticks = [0]
        self.close_result = 0
        self.read_result = None
        self.write_result = None
        self.dialog_accept = True
        self.dialog_path = b'C:\\SYNTH.R1'
        self.dialogs = []
        for offset,cleanup in [(0x400,10),(0x500,2),(0x600,0),(0x700,4)]:
            self.cpu.mem_write(0xe0000+offset,b'\xca'+struct.pack('<H',cleanup) if cleanup else b'\xcb')
        self.cpu.hook_add(UC_HOOK_CODE,self._extra_api,begin=0xe0400,end=0xe0700)
        for address in [0xa4a10,0xd924c,0x12160,0x209ed]:
            self.cpu.hook_add(UC_HOOK_CODE,self._helper,begin=address,end=address)

    def _stack(self):
        return self.cpu.reg_read(reg.UC_X86_REG_SS)*16+self.cpu.reg_read(reg.UC_X86_REG_SP)

    def _string(self,address):
        output=bytearray()
        for i in range(256):
            byte=bytes(self.cpu.mem_read(address+i,1))
            if byte==b'\0':return bytes(output)
            output.extend(byte)
        raise AssertionError('Synthetic string is not terminated')

    def _return_far(self):
        ip,cs=struct.unpack('<HH',bytes(self.cpu.mem_read(self._stack(),4)))
        self.cpu.reg_write(reg.UC_X86_REG_SP,self.cpu.reg_read(reg.UC_X86_REG_SP)+4)
        self.cpu.reg_write(reg.UC_X86_REG_CS,cs)
        self.cpu.reg_write(reg.UC_X86_REG_IP,ip)

    def _helper(self,cpu,address,size,unused):
        if not self.helper_boundaries:return
        stack=self._stack()
        if address==0x209ed:
            environment,value=struct.unpack('<Hh',bytes(cpu.mem_read(stack+4,4)))
            self.events.append(('abort',environment,value))
            raise StreamAbort()
        if address==0xa4a10:
            id=struct.unpack('<h',bytes(cpu.mem_read(stack+4,2)))[0]
            self.events.append(('file_error',id))
        elif address==0xd924c:
            args=struct.unpack('<3H',bytes(cpu.mem_read(stack+4,6)))
            assert args==(22,0,0),args
            cpu.reg_write(reg.UC_X86_REG_AX,0x2f00)
        elif address==0x12160:
            args=struct.unpack('<2H',bytes(cpu.mem_read(stack+4,4)))
            assert args==(0x2f00,16),args
            self.events.append(('write_error',))
            cpu.reg_write(reg.UC_X86_REG_AX,1)
        self._return_far()

    def _api(self,cpu,address,size,unused):
        if address in [0xe0100,0xe0200]:
            count,offset,selector,handle=struct.unpack('<4H',bytes(cpu.mem_read(self._stack()+4,8)))
            result=self.read_result if address==0xe0100 else self.write_result
            if result is None:result=count
            assert 0<=result<=65535
            actual=min(result,count) if result!=65535 else 0
            if address==0xe0100:
                payload=self.incoming[self.position:self.position+actual]
                assert len(payload)==actual
                if payload:cpu.mem_write(selector*16+offset,payload)
                self.position+=actual
                self.events.append(('read',handle,count,result))
            else:
                payload=bytes(cpu.mem_read(selector*16+offset,actual)) if actual else b''
                self.outgoing.extend(payload)
                self.events.append(('write',handle,count,result))
            cpu.reg_write(reg.UC_X86_REG_AX,result)
        else:super()._api(cpu,address,size,unused)

    def _extra_api(self,cpu,address,size,unused):
        stack=self._stack()
        if address==0xe0400:
            mode,of,ofs,name,ns=struct.unpack('<5H',bytes(cpu.mem_read(stack+4,10)))
            assert self.opens,'OpenFile script exhausted'
            handle,error=self.opens.pop(0)
            cpu.mem_write(ofs*16+of+2,struct.pack('<H',error))
            self.events.append(('open',self._string(ns*16+name),mode,handle,error))
            cpu.reg_write(reg.UC_X86_REG_AX,handle&65535)
        elif address==0xe0500:
            handle=struct.unpack('<H',bytes(cpu.mem_read(stack+4,2)))[0]
            self.events.append(('close',handle,self.close_result))
            cpu.reg_write(reg.UC_X86_REG_AX,self.close_result&65535)
        elif address==0xe0600:
            assert self.ticks,'GetTickCount script exhausted'
            tick=self.ticks.pop(0)
            self.events.append(('tick',tick))
            cpu.reg_write(reg.UC_X86_REG_AX,tick&65535)
            cpu.reg_write(reg.UC_X86_REG_DX,tick>>16)
        elif address==0xe0700:
            of,ofs=struct.unpack('<HH',bytes(cpu.mem_read(stack+4,4)))
            base=ofs*16+of
            record=bytes(cpu.mem_read(base,72))
            dst,ds=struct.unpack_from('<HH',record,24)
            filt,fs=struct.unpack_from('<HH',record,8)
            self.dialogs.append({'structure':record,'suggestion':self._string(ds*16+dst),
                                 'filter':bytes(cpu.mem_read(fs*16+filt,64))})
            if self.dialog_accept:
                cpu.mem_write(ds*16+dst,self.dialog_path+b'\0')
                cpu.mem_write(base+52,struct.pack('<H',self.dialog_path.rfind(b'\\')+1))
            cpu.reg_write(reg.UC_X86_REG_AX,int(self.dialog_accept))
        else:raise AssertionError(hex(address))
