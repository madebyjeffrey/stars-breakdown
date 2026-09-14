"""Original record routines with explicit simulated Windows I/O boundaries.

All loaded game and CRT callees execute unchanged. Only successful _lread,
_lwrite, and the verified Player %d wsprintf form are simulated here. This is
not Windows integration or validation of UI/error handling.
"""
import struct
from tools.core_machine import OriginalCore, reg
from unicorn import UC_HOOK_CODE


class RecordMachine(OriginalCore):
    SEGMENTS = dict(OriginalCore.SEGMENTS)
    SEGMENTS.update({15: 0xa000, 8: 0xb000})

    def __init__(self):
        super().__init__({'KERNEL!#82': (0xe000,0x100), 'KERNEL!#86': (0xe000,0x200),
                          'USER!#420': (0xe000,0x300)})
        self.cpu.mem_map(0xe0000,0x10000)
        for offset,ret in [(0x100,b'\xca\x08\x00'),(0x200,b'\xca\x08\x00'),(0x300,b'\xcb')]:
            self.cpu.mem_write(0xe0000+offset,ret)
        self.cpu.hook_add(UC_HOOK_CODE,self._api,begin=0xe0100,end=0xe0300)
        self.incoming = b''
        self.position = 0
        self.outgoing = bytearray()
        self.transfers = []
        self.put(0xa22,struct.pack('<h',7))

    def _api(self,cpu,address,size,unused):
        stack=cpu.reg_read(reg.UC_X86_REG_SS)*16+cpu.reg_read(reg.UC_X86_REG_SP)
        if address in [0xe0100,0xe0200]:
            count,offset,selector,handle=struct.unpack('<4H',bytes(cpu.mem_read(stack+4,8)))
            assert handle == 7, ('Unexpected synthetic handle',handle)
            buffer=selector*16+offset
            if address==0xe0100:
                payload=self.incoming[self.position:self.position+count]
                assert len(payload)==count,'Synthetic stream exhausted; error path is outside this API model'
                cpu.mem_write(buffer,payload);self.position+=count
                self.transfers.append(('read',count))
            else:
                payload=bytes(cpu.mem_read(buffer,count));self.outgoing.extend(payload)
                self.transfers.append(('write',count))
            cpu.reg_write(reg.UC_X86_REG_AX,count)
        elif address==0xe0300:
            dst,ds,fmt,fs,index=struct.unpack('<4Hh',bytes(cpu.mem_read(stack+4,10)))
            format_bytes=bytes(cpu.mem_read(fs*16+fmt,10))
            assert format_bytes==b'Player %d\0', ('Unexpected wsprintf format',format_bytes)
            payload=('Player %d'%index).encode('ascii')
            cpu.mem_write(ds*16+dst,payload+b'\0')
            cpu.reg_write(reg.UC_X86_REG_AX,len(payload))
        else:raise AssertionError(('Unexpected API entry',hex(address)))
