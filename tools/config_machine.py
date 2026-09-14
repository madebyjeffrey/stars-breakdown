"""Original configuration instructions with Windows profile/placement services."""
import struct
from tools.title_machine import TitleMachine, TitleWorld
from tools.race_ui_machine import words_bytes, signed, reg

class ConfigWorld(TitleWorld):
    def __init__(self,write_near,read_near):
        super().__init__(write_near,read_near)
        self.normal=(30,40,630,440)
        self.zoomed=0
        self.numbers={}
        self.writes={}
    def call(self,name,h=0,a=0,b=0,v=0,p=None,q=None):
        out=None;result=1
        if name=='GetWindowPlacement':
            out=words_bytes([22,0,1,0,0,0,0,*self.normal])
        elif name=='IsZoomed': result=self.zoomed
        elif name=='GetPrivateProfileInt': result=self.numbers.get(q[:2],a)&65535
        elif name=='WritePrivateProfileString': self.writes[q[:2]]=p
        else:return super().call(name,h,a,b,v,p,q)
        self.events.append((name,h,a,b,v,p,q))
        return result,out

class ConfigMachine(TitleMachine):
    WINDOWS=dict(TitleMachine.WINDOWS)
    MORE={'USER!#370':('GetWindowPlacement',6),'USER!#272':('IsZoomed',2),
          'KERNEL!#127':('GetPrivateProfileInt',14),'USER!#420':('wsprintf',0)}
    WINDOWS.update(MORE)
    def __init__(self):
        super().__init__();self.world=ConfigWorld(self.put,self.get)
    def _window_api(self,cpu,address,size,unused):
        name,cleanup=self.window_slots[address]
        if name not in {n for n,_ in self.MORE.values()}:
            return super()._window_api(cpu,address,size,unused)
        stack=self._stack()+4
        if name=='wsprintf':
            dst,ds,fmt,fs,*values=struct.unpack('<9H',bytes(cpu.mem_read(stack,18)))
            pattern=self._string(fs*16+fmt)
            assert pattern==b'%c%04d%04d%04d%04d',pattern
            payload=(chr(values[0])+''.join('%04d'%signed(x) for x in values[1:])).encode('ascii')
            cpu.mem_write(ds*16+dst,payload+b'\0');cpu.reg_write(reg.UC_X86_REG_AX,len(payload));return
        w=struct.unpack('<'+'H'*(cleanup//2),bytes(cpu.mem_read(stack,cleanup)))
        p=q=None;h=a=0;dest=None
        if name=='GetWindowPlacement':h=w[2];dest=w[0]+w[1]*16
        elif name=='IsZoomed':h=w[0]
        elif name=='GetPrivateProfileInt':
            a=w[2];q=(self._string(w[5]+w[6]*16),self._string(w[3]+w[4]*16),None,self._string(w[0]+w[1]*16))
        result,out=self.world.call(name,h,a,0,0,p,q)
        if out is not None:cpu.mem_write(dest,out)
        cpu.reg_write(reg.UC_X86_REG_AX,result&65535);cpu.reg_write(reg.UC_X86_REG_DX,result>>16)
