"""Original race integration oracle. Game helpers execute original x86.

Windows APIs and host-dependent CRT filesystem/date services use explicit,
recorded synthetic responses. No user files, credentials or GUI are accessed.
"""
import re
import struct
from tools.file_machine import FileMachine
from tools.core_machine import reg
from unicorn import UC_HOOK_CODE


class RaceRuntimeMachine(FileMachine):
    SEGMENTS = dict(FileMachine.SEGMENTS)
    del SEGMENTS[16]
    SEGMENTS.update({30:0x7000,4:0xf000})
    WINDOWS = {
        'KERNEL!#51':('MakeProcInstance',6), 'KERNEL!#52':('FreeProcInstance',4),
        'USER!#87':('DialogBox',12), 'TOOLHELP!#80':('TimerCount',4),
        'USER!#101':('SendDlgItemMessage',12), 'GDI!#1':('SetBkColor',6),
        'USER!#22':('SetFocus',2), 'USER!#33':('GetClientRect',6),
        'USER!#37':('SetWindowText',6), 'USER!#171':('WinHelp',12),
        'USER!#81':('FillRect',8), 'USER!#88':('EndDialog',4),
        'USER!#91':('GetDlgItem',4), 'USER!#93':('GetDlgItemText',10),
    }

    def __init__(self):
        self.window_slots={0xe1000+i*16:entry for i,entry in enumerate(self.WINDOWS.values())}
        imports={key:(0xe000,0x1000+i*16) for i,key in enumerate(self.WINDOWS)}
        super().__init__(format_boundary=False,imports=imports,output_boundary=False)
        self.access_results=[]; self.seek_result=0; self.date=b'09/14/26';self.time=b'12:34:56'
        self.timer_values=[]; self.dialog_result=0; self.control_text={};self.window_events=[]
        self.read_results=None
        for address,(_,cleanup) in self.window_slots.items():
            self.cpu.mem_write(address,b'\xca'+struct.pack('<H',cleanup))
            self.cpu.hook_add(UC_HOOK_CODE,self._window_api,begin=address,end=address)
        for address in [0x20a16,0x2027e,0x2050a,0x2056e]:
            self.cpu.hook_add(UC_HOOK_CODE,self._host_crt,begin=address,end=address)

    def _api(self,cpu,address,size,unused):
        if address==0xe0100 and self.read_results is not None:
            assert self.read_results,'read script exhausted'
            self.read_result=self.read_results.pop(0)
        if address!=0xe0300:return super()._api(cpu,address,size,unused)
        stack=self._stack(); dst,ds,fmt,fs=struct.unpack('<4H',bytes(cpu.mem_read(stack+4,8)))
        format=self._string(fs*16+fmt);at=stack+12;out=bytearray();last=0
        for m in re.finditer(rb'%([-+ #0]*)([0-9]*)(l?)([sdiucxX%])',format):
            out.extend(format[last:m.start()]);flags,width,long,kind=m.groups();last=m.end()
            if kind==b'%':out.extend(b'%');continue
            if kind==b's':
                off,seg=struct.unpack('<HH',bytes(cpu.mem_read(at,4)));at+=4
                value=self._string(seg*16+off)
            elif kind==b'c':
                value=bytes([struct.unpack('<H',bytes(cpu.mem_read(at,2)))[0]&255]);at+=2
            else:
                n=4 if long else 2
                value=int.from_bytes(bytes(cpu.mem_read(at,n)),'little',signed=kind in [b'd',b'i']);at+=n
                spec=b'%'+flags+width+kind
                value=spec%value
            out.extend(value)
        out.extend(format[last:])
        assert b'%' not in format[last:] or b'%%' in format[last:],('unparsed format',format)
        cpu.mem_write(ds*16+dst,bytes(out)+b'\0');cpu.reg_write(reg.UC_X86_REG_AX,len(out)&65535)

    def _host_crt(self,cpu,address,size,unused):
        stack=self._stack()
        if address==0x20a16:
            name,mode=struct.unpack('<Hh',bytes(cpu.mem_read(stack+4,4)))
            result=self.access_results.pop(0)
            self.events.append(('access',self._string(0x30000+name),mode,result))
            cpu.reg_write(reg.UC_X86_REG_AX,result&65535)
        elif address==0x2027e:
            handle,offset,origin=struct.unpack('<hIh',bytes(cpu.mem_read(stack+4,8)))
            self.events.append(('seek',handle,offset,origin,self.seek_result))
            cpu.reg_write(reg.UC_X86_REG_AX,self.seek_result&65535)
            cpu.reg_write(reg.UC_X86_REG_DX,self.seek_result>>16)
        else:
            out=struct.unpack('<H',bytes(cpu.mem_read(stack+4,2)))[0]
            label,value=('date',self.date) if address==0x2050a else ('time',self.time)
            self.events.append((label,value));self.put(out,value+b'\0');cpu.reg_write(reg.UC_X86_REG_AX,out)
        self._return_far()

    def _window_api(self,cpu,address,size,unused):
        name,_=self.window_slots[address]; stack=self._stack()
        def words(n):return struct.unpack('<'+'H'*n,bytes(cpu.mem_read(stack+4,n*2)))
        result=0
        if name=='MakeProcInstance':
            instance,off,seg=words(3);result=off|(seg<<16)
        elif name=='FreeProcInstance':pass
        elif name=='DialogBox':
            off,seg,owner,resource,rs,instance=words(6)
            self.window_events.append((name,resource,owner));result=self.dialog_result&65535
        elif name=='TimerCount':
            off,seg=words(2);assert bytes(cpu.mem_read(seg*16+off,4))==struct.pack('<I',12)
            value=self.timer_values.pop(0);self.events.append(('timer',value))
            cpu.mem_write(seg*16+off+4,struct.pack('<II',value,0));result=1
        elif name=='GetDlgItem':
            control,hwnd=words(2);result=control;self.window_events.append((name,hwnd,control))
        elif name=='SendDlgItemMessage':
            lo,hi,wp,msg,control,hwnd=words(6)
            self.window_events.append((name,hwnd,control,msg,wp,lo|(hi<<16)))
        elif name=='GetDlgItemText':
            count,off,seg,control,hwnd=words(5);text=self.control_text.get(control,b'')[:count-1]
            cpu.mem_write(seg*16+off,text+b'\0');result=len(text)
            self.window_events.append((name,hwnd,control,count,text))
        elif name=='SetWindowText':
            off,seg,hwnd=words(3);self.window_events.append((name,hwnd,self._string(seg*16+off)));result=1
        elif name=='GetClientRect':
            off,seg,hwnd=words(3);cpu.mem_write(seg*16+off,struct.pack('<4h',0,0,400,300))
            self.window_events.append((name,hwnd));result=1
        elif name=='FillRect':
            brush,off,seg,dc=words(4);rect=struct.unpack('<4h',bytes(cpu.mem_read(seg*16+off,8)))
            self.window_events.append((name,dc,brush,rect));result=1
        elif name=='SetBkColor':
            lo,hi,dc=words(3);self.window_events.append((name,dc,lo|(hi<<16)))
        elif name=='SetFocus':
            hwnd,=words(1);self.window_events.append((name,hwnd))
        elif name=='EndDialog':
            result,hwnd=words(2);self.window_events.append((name,hwnd,result));result=1
        elif name=='WinHelp':
            lo,hi,command,off,seg,hwnd=words(6)
            self.window_events.append((name,hwnd,command,lo|(hi<<16),self._string(seg*16+off)));result=1
        else:raise AssertionError(name)
        cpu.reg_write(reg.UC_X86_REG_AX,result&65535);cpu.reg_write(reg.UC_X86_REG_DX,result>>16)

    def dialog_call(self,offset,hwnd,message,wp=0,lp=0,segment=9):
        # A tiny cdecl wrapper calls the original Pascal callback, whose RETF
        # removes ten argument bytes. OriginalCore's register checks still apply.
        selector=self.SEGMENTS[segment]
        # Fixed code, separate argument data: avoids stale JIT translations
        # when successive synthetic Windows messages have different arguments.
        code=b''.join(b'\x2e\xff\x36'+struct.pack('<H',0xf100+i*2) for i in range(5))
        code+=b'\x2e\xff\x1e\x0a\xf1\xcb'
        self.cpu.mem_write(0xef100,struct.pack('<7H',hwnd,message,wp,lp>>16,lp&65535,offset,selector))
        self.cpu.mem_write(0xef000,code)
        self.SEGMENTS=dict(self.SEGMENTS,**{})
        self.SEGMENTS[0]=0xe000
        try:return self.call(0xf000,segment=0,shared_stack=True)&65535
        finally:del self.SEGMENTS[0]
