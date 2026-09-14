"""Synthetic Win16 UI services, with independent x86/host argument adapters.

The original game decides all control flow and drawing. This model only answers
OS calls, records their arguments, and supplies deterministic window messages.
It intentionally doesn't pretend to be a real Windows rendering comparison.
"""
import json
import struct
from collections import deque
from tools.core_machine import ROOT, reg
from tools.race_runtime_machine import RaceRuntimeMachine
from unicorn import UC_HOOK_CODE

def signed(word):return word-65536 if word&32768 else word
def words_bytes(values):return struct.pack('<'+'H'*len(values),*(v&65535 for v in values))

class WindowWorld:
    def __init__(self,write_near,read_near,resource=0x92):
        self.write_near=write_near;self.read_near=read_near;self.events=[]
        self.checks={};self.text={};self.limits={};self.combo={};self.rectangles={}
        self.selected={};self.bg={};self.fg={};self.mode={};self.clock=0;self.mouse=deque()
        self.cursor=(100,100);self.position=(20,30);self.screen=(1024,768)
        self.char_width=7;self.char_height=13;self.alloc=0xd000;self.font=0x900;self.result=None
        self.lock_result=0x3000c000
        self.load_resource(resource)
    def load_resource(self,resource):
        layout=json.loads((ROOT/'analysis/ne/layout.json').read_text())
        dialog=next(x['decoded'] for x in layout['resources'] if x['type']==5 and x['id']==resource)
        self.client=(0,0,dialog['rect'][2]*2,dialog['rect'][3]*2)
        self.text={};self.checks={};self.combo={};self.limits={};self.rectangles={}
        for c in dialog['controls']:
            self.text[c['id']]=c['text'].encode('latin1');x,y,w,h=c['rect']
            self.rectangles[c['id']]=(x*2,y*2,(x+w)*2,(y+h)*2)
    def call(self,name,h=0,a=0,b=0,v=0,p=None,q=None):
        event=(name,h,a,b,v,p,q)
        result=0;output=None
        if name=='GetDlgItem':result=a
        elif name in ['GetWindowRect','GetClientRect']:
            if name=='GetClientRect':output=self.client
            elif h==100:output=(self.position[0],self.position[1],self.position[0]+self.client[2],self.position[1]+self.client[3])
            else:
                rc=self.rectangles.get(h,(20,20,120,40));output=tuple(rc[i]+self.position[i%2] for i in range(4))
            result=1
        elif name=='ScreenToClient':output=(signed(p[0])-self.position[0],signed(p[1])-self.position[1]);result=1
        elif name=='GetSystemMetrics':result=self.screen[a]
        elif name=='SetWindowPos':
            if h==100:self.position=(signed(p[0]),signed(p[1]))
            elif not b&1:self.rectangles[h]=(signed(p[0]),signed(p[1]),signed(p[0])+signed(p[2]),signed(p[1])+signed(p[3]))
            result=1
        elif name in ['GetDC','BeginPaint']:result=500
        elif name in ['ReleaseDC','EndPaint']:result=1
        elif name=='SelectObject':result=self.selected.get(h,1);self.selected[h]=a
        elif name=='SetBkMode':result=self.mode.get(h,2);self.mode[h]=a
        elif name=='SetBkColor':result=self.bg.get(h,0);self.bg[h]=v
        elif name=='SetTextColor':result=self.fg.get(h,0);self.fg[h]=v
        elif name=='GetTextExtent':result=len(p)*self.char_width|(self.char_height<<16)
        elif name=='GetTextMetrics':output=words_bytes([self.char_height,10,3,0,2,7,10,400])+bytes(15);result=1
        elif name=='LocalAlloc':result=self.alloc;self.write_near(result,bytes(b))
        elif name=='LocalFree':result=0
        elif name=='CreateFontIndirect':result=self.font
        elif name in ['DeleteObject','FillRect','FrameRect','TextOut','PatBlt','DrawText','ExtTextOut','EnableWindow','RealizePalette','SelectPalette','GlobalUnlock','InvalidateRect']:
            result=1
        elif name=='GlobalLock':result=self.lock_result
        elif name=='StretchDIBits':result=signed(p[3])
        elif name in ['SetWindowText','SetDlgItemText']:
            self.text[a if name=='SetDlgItemText' else h]=p;result=1
        elif name=='GetDlgItemText':output=self.text.get(a,b'')[:max(b-1,0)]+b'\0';result=len(output)-1
        elif name=='CheckRadioButton':
            for i in range(a,b+1):self.checks[i]=int(i==v)
            result=1
        elif name=='CheckDlgButton':self.checks[a]=b;result=1
        elif name=='IsDlgButtonChecked':result=self.checks.get(a,0)
        elif name=='SendMessage':
            if a==0x401:self.checks[h]=b
            elif a==0x400:result=self.checks.get(h,0)
            elif a==0x403:self.combo.setdefault(h,[]).append(p);result=len(self.combo[h])-1
            elif a==0x40e:self.checks[h]=b
            elif a==0x407:result=self.checks.get(h,0)
            else:raise AssertionError((name,a))
        elif name=='SendDlgItemMessage':
            if b==0x415:self.limits[a]=p
            elif b!=0x401:raise AssertionError((name,b))
        elif name=='GetCurrentTime':result=self.clock;self.clock=(self.clock+1000)&0xffffffff
        elif name=='PeekMessage':
            if self.mouse:
                message,x,y=self.mouse.popleft();output=words_bytes([100,message,0,x,y,0,0,x,y]);result=1
            else:result=0
        elif name=='GetCursorPos':output=self.cursor;result=1
        elif name in ['SetCapture','ReleaseCapture','SetCursor','SetFocus']:result=1
        elif name=='EndDialog':self.result=signed(a);result=1
        elif name=='WinHelp':result=1
        elif name=='MulDiv':
            product=signed(a)*signed(b);denominator=signed(v&65535)
            if not denominator:result=-32768
            else:
                magnitude=(abs(product)*2+abs(denominator))//(abs(denominator)*2)
                result=-magnitude if (product<0)!=(denominator<0) else magnitude
                if not -32768<=result<=32767:result=-32768
        else:raise AssertionError(('unmodelled UI API',event))
        self.events.append(event)
        return result&0xffffffff,output

class RaceUiMachine(RaceRuntimeMachine):
    WINDOWS=dict(RaceRuntimeMachine.WINDOWS)
    WINDOWS.update({
        'USER!#39':('BeginPaint',6),'USER!#40':('EndPaint',6),
        'USER!#66':('GetDC',2),'USER!#68':('ReleaseDC',4),'USER!#32':('GetWindowRect',6),
        'USER!#29':('ScreenToClient',6),'USER!#179':('GetSystemMetrics',2),
        'USER!#232':('SetWindowPos',14),'GDI!#45':('SelectObject',4),
        'GDI!#2':('SetBkMode',4),'GDI!#9':('SetTextColor',6),'GDI!#91':('GetTextExtent',8),
        'GDI!#33':('TextOut',12),'GDI!#29':('PatBlt',14),'USER!#85':('DrawText',14),
        'USER!#83':('FrameRect',8),'KERNEL!#5':('LocalAlloc',4),'KERNEL!#7':('LocalFree',2),
        'GDI!#57':('CreateFontIndirect',4),'GDI!#93':('GetTextMetrics',6),'GDI!#69':('DeleteObject',2),
        'USER!#111':('SendMessage',10),'USER!#92':('SetDlgItemText',8),'USER!#96':('CheckRadioButton',8),
        'USER!#97':('CheckDlgButton',6),'USER!#34':('EnableWindow',4),'USER!#98':('IsDlgButtonChecked',4),
        'USER!#282':('SelectPalette',6),'USER!#283':('RealizePalette',2),'KERNEL!#18':('GlobalLock',2),
        'KERNEL!#19':('GlobalUnlock',2),'GDI!#439':('StretchDIBits',32),'USER!#18':('SetCapture',2),
        'USER!#19':('ReleaseCapture',0),'USER!#15':('GetCurrentTime',0),'USER!#109':('PeekMessage',12),
        'USER!#17':('GetCursorPos',4),'USER!#69':('SetCursor',2),'USER!#72':('SetRect',12),
        'USER!#77':('OffsetRect',8),'USER!#76':('PtInRect',8),'KERNEL!#90':('lstrlen',4),
        'GDI!#351':('ExtTextOut',22),'GDI!#128':('MulDiv',6),'USER!#125':('InvalidateRect',8),
    })
    def __init__(self,resource=0x92):
        super().__init__();self.world=WindowWorld(self.put,self.get,resource)
        self.dialog_scripts=[];self.dialog_frames=[];self.save_choices=[]
        self.cpu.mem_write(0xe1900,b'\xf4')
        self.cpu.hook_add(UC_HOOK_CODE,self._dialog_continue,begin=0xe1900,end=0xe1900)
    def _extra_api(self,cpu,address,size,unused):
        if address==0xe0700 and self.save_choices:self.dialog_accept=self.save_choices.pop(0)
        return super()._extra_api(cpu,address,size,unused)
    def _dialog_continue(self,cpu,address,size,unused):
        frame=self.dialog_frames[-1]
        if self.world.result is not None:
            sp=frame['sp'];off,seg=struct.unpack('<2H',bytes(cpu.mem_read(0x30000+sp,4)))
            cpu.reg_write(reg.UC_X86_REG_SP,sp+16)
            cpu.reg_write(reg.UC_X86_REG_CS,seg);cpu.reg_write(reg.UC_X86_REG_IP,off)
            cpu.reg_write(reg.UC_X86_REG_AX,self.world.result&65535);cpu.reg_write(reg.UC_X86_REG_DX,0)
            self.dialog_frames.pop();return
        assert frame['actions'],'dialog script ended without EndDialog'
        action=frame['actions'].pop(0)
        while callable(action):
            action(self.world,self.put)
            assert frame['actions'];action=frame['actions'].pop(0)
        msg,wp,lp=action
        sp=frame['sp']-14
        cpu.mem_write(0x30000+sp,struct.pack('<HHIHHH',0x1900,0xe000,lp,wp,msg,100))
        cpu.reg_write(reg.UC_X86_REG_SP,sp)
        cpu.reg_write(reg.UC_X86_REG_CS,frame['selector']);cpu.reg_write(reg.UC_X86_REG_IP,frame['offset'])
    def _window_api(self,cpu,address,size,unused):
        name,cleanup=self.window_slots[address]
        if name=='DialogBox' and self.dialog_scripts:
            off,seg,owner,resource,rs,instance=struct.unpack('<6H',bytes(cpu.mem_read(self._stack()+4,12)))
            expected,actions=self.dialog_scripts.pop(0);assert resource==expected,(resource,expected)
            self.window_events.append((name,resource,owner));self.world.load_resource(resource);self.world.result=None
            self.dialog_frames.append(dict(sp=cpu.reg_read(reg.UC_X86_REG_SP),offset=off,selector=seg,actions=list(actions)))
            return self._dialog_continue(cpu,address,size,unused)
        if name in ['MakeProcInstance','FreeProcInstance','DialogBox','TimerCount']:
            return super()._window_api(cpu,address,size,unused)
        w=struct.unpack('<'+'H'*(cleanup//2),bytes(cpu.mem_read(self._stack()+4,cleanup)))
        def ptr(i):return w[i]+(w[i+1]<<16)
        def memory(i,n):return bytes(cpu.mem_read(w[i+1]*16+w[i],n))
        def string(i):return self._string(w[i+1]*16+w[i])
        def rect(i):return struct.unpack('<4H',memory(i,8))
        def point(i):return struct.unpack('<2H',memory(i,4))
        output_pointer=None;h=a=b=v=0;p=q=None
        if name=='lstrlen':result=len(string(0));output=None
        elif name=='SetRect':cpu.mem_write(w[5]*16+w[4],words_bytes([w[3],w[2],w[1],w[0]]));result=1;output=None
        elif name=='OffsetRect':
            rc=rect(2);cpu.mem_write(w[3]*16+w[2],words_bytes([rc[0]+signed(w[1]),rc[1]+signed(w[0]),rc[2]+signed(w[1]),rc[3]+signed(w[0])]))
            result=1;output=None
        elif name=='PtInRect':
            rc=list(map(signed,rect(2)));x,y=map(signed,w[:2]);result=int(rc[0]<=x<rc[2] and rc[1]<=y<rc[3]);output=None
        else:
            if name in ['GetDC','LocalFree','DeleteObject','GlobalLock','GlobalUnlock','RealizePalette','SetCapture','SetCursor','SetFocus']:h=w[0]
            elif name in ['ReleaseCapture','GetCurrentTime']:pass
            elif name in ['BeginPaint','EndPaint','GetWindowRect','GetClientRect','GetTextMetrics']:
                h=w[2];output_pointer=ptr(0) if name not in ['EndPaint','BeginPaint'] else None
            elif name=='ScreenToClient':h=w[2];p=point(0);output_pointer=ptr(0)
            elif name=='GetCursorPos':output_pointer=ptr(0)
            elif name=='GetSystemMetrics':a=w[0]
            elif name=='SetWindowPos':h=w[6];a=w[5];b=w[0];p=(w[4],w[3],w[2],w[1])
            elif name in ['SelectObject','SetBkMode','EnableWindow','EndDialog']:h=w[1];a=w[0]
            elif name=='ReleaseDC':h=w[1];a=w[0]
            elif name in ['SetBkColor','SetTextColor']:h=w[2];v=ptr(0)
            elif name=='GetTextExtent':h=w[3];a=w[0];p=memory(1,a)
            elif name=='TextOut':h=w[5];a=w[4];b=w[3];v=w[0];p=memory(1,v)
            elif name=='PatBlt':h=w[6];a=w[5];b=w[4];v=ptr(0);p=(w[3],w[2])
            elif name=='DrawText':h=w[6];a=w[3];b=w[0];p=memory(4,a);q=rect(1)
            elif name in ['FrameRect','FillRect']:h=w[3];a=w[0];p=rect(1)
            elif name=='LocalAlloc':a=w[1];b=w[0]
            elif name=='CreateFontIndirect':p=memory(0,50)
            elif name=='SendMessage':h=w[4];a=w[3];b=w[2];v=ptr(0);p=string(0) if a==0x403 else None;v=0 if p is not None else v
            elif name=='SetDlgItemText':h=w[3];a=w[2];p=string(0)
            elif name=='SetWindowText':h=w[2];p=string(0)
            elif name=='GetDlgItemText':h=w[4];a=w[3];b=w[0];output_pointer=ptr(1)
            elif name in ['GetDlgItem','IsDlgButtonChecked']:h=w[1];a=w[0]
            elif name=='CheckRadioButton':h=w[3];a=w[2];b=w[1];v=w[0]
            elif name=='CheckDlgButton':h=w[2];a=w[1];b=w[0]
            elif name=='SelectPalette':h=w[2];a=w[1];b=w[0]
            elif name=='StretchDIBits':
                h=w[15];v=ptr(0);b=w[2];a=(w[5]-w[3])&65535;p=tuple(reversed(w[7:15]));q=memory(3,40)
            elif name=='PeekMessage':h=w[3];a=w[2];b=w[1];v=w[0];output_pointer=ptr(4)
            elif name=='SendDlgItemMessage':h=w[5];a=w[4];b=w[3];v=ptr(0);p=w[2]
            elif name=='WinHelp':h=w[5];a=w[2];v=ptr(0);p=string(3)
            elif name=='MulDiv':a=w[2];b=w[1];v=w[0]
            elif name=='InvalidateRect':h=w[3];a=w[0];p=rect(1) if ptr(1) else None
            elif name=='ExtTextOut':h=w[10];a=w[9];b=w[8];v=(w[7]<<16)|w[2];p=memory(3,w[2]);q=rect(5)
            else:raise AssertionError(('adapter missing',name,w))
            result,output=self.world.call(name,h,a,b,v,p,q)
        if output_pointer and output is not None:
            payload=words_bytes(output) if isinstance(output,tuple) else output
            cpu.mem_write((output_pointer>>16)*16+(output_pointer&65535),payload)
        cpu.reg_write(reg.UC_X86_REG_AX,result&65535);cpu.reg_write(reg.UC_X86_REG_DX,(result>>16)&65535)
