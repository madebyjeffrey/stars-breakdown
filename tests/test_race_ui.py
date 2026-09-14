"""Race UI behavior: original instructions and recovered C share only OS answers."""
import ctypes as C
import copy
import json
import struct
import unittest
from tests import test_race_runtime as runtime_tests
from tests.test_race_runtime import Host, Runtime, PTR
from tools.race_ui_machine import RaceUiMachine, WindowWorld, words_bytes
from tools.core_machine import ROOT

class Point(C.Structure):_fields_=[('x',C.c_uint16),('y',C.c_uint16)]
class Rect(C.Structure):_fields_=[('left',C.c_uint16),('top',C.c_uint16),('right',C.c_uint16),('bottom',C.c_uint16)]
class Track(C.Structure):
    _fields_=[('window',C.c_uint16),('dc',C.c_uint16),('rect',Rect),('delay',C.c_uint16),
              ('type',C.c_uint16),('text',C.c_uint16),('ticks',C.c_ubyte*4),('flags',C.c_uint16)]

class UiHost(Host):
    def __init__(self,lib,machine):
        super().__init__(lib,machine)
        self.dialog_scripts=copy.deepcopy(machine.dialog_scripts)
        self.save_choices=list(machine.save_choices)
        self.world=copy.copy(machine.world)
        for n,v in vars(machine.world).items():
            if n not in ['write_near','read_near']:setattr(self.world,n,copy.deepcopy(v))
        self.world.write_near=lambda off,b:C.memmove(C.byref(self.data,off),b,len(b))
        self.world.read_near=lambda off,n:bytes(self.data[off:off+n])
    def do_dialog(self,r,resource,owner,proc):
        if not self.dialog_scripts:return super().do_dialog(r,resource,owner,proc)
        expected,actions=self.dialog_scripts.pop(0);assert resource==expected,(resource,expected)
        self.window_events.append(('DialogBox',resource,owner));self.world.load_resource(resource);self.world.result=None
        callback=C.CFUNCTYPE(C.c_int16,C.POINTER(Runtime),C.c_uint16,C.c_uint16,C.c_uint16,C.c_uint32)(proc)
        for action in actions:
            if callable(action):action(self.world,self.world.write_near)
            else:callback(r,100,*action)
            if self.world.result is not None:break
        assert self.world.result is not None,'dialog script ended without EndDialog'
        return self.world.result
    def do_save_choice(self,choice):
        if not self.save_choices:return super().do_save_choice(choice)
        accept=self.save_choices.pop(0);previous=self.machine.dialog_accept;self.machine.dialog_accept=accept
        try:return super().do_save_choice(choice)
        finally:self.machine.dialog_accept=previous
    def do_window(self,cp):
        c=cp.contents;n=c.operation.decode();h,a,b,v=c.window,c.a,c.b,c.value
        if n in ['MessageBox','GetFocus']:return super().do_window(cp)
        def words(p,n):return struct.unpack('<'+'H'*n,C.string_at(p,n*2))
        p=q=None
        if n in ['TextOut']:p=C.string_at(c.p,v)
        elif n=='GetTextExtent':p=C.string_at(c.p,a)
        elif n=='DrawText':p=C.string_at(c.p,a);q=words(c.q,4)
        elif n=='ExtTextOut':p=C.string_at(c.p,v&65535);q=words(c.q,4)
        elif n in ['SetWindowText','SetDlgItemText','WinHelp']:p=C.string_at(c.p)
        elif n=='SendMessage':p=C.string_at(c.p) if c.p else None
        elif n=='SendDlgItemMessage':p=C.cast(c.p,C.POINTER(C.c_uint16))[0]
        elif n in ['FillRect','FrameRect','SetWindowPos']:p=words(c.p,4)
        elif n=='InvalidateRect':p=words(c.p,4) if c.p else None
        elif n in ['ScreenToClient','PatBlt']:p=words(c.p,2)
        elif n=='StretchDIBits':p=words(c.p,8);q=C.string_at(c.q,40)
        elif n=='CreateFontIndirect':p=C.string_at(c.p,50)
        result,output=self.world.call(n,h,a,b,v,p,q)
        if output is not None:
            payload=words_bytes(output) if isinstance(output,tuple) else output
            C.memmove(c.p,payload,len(payload))
        return result

class RaceUiTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        runtime_tests.RaceRuntimeTests.setUpClass.__func__(cls)
        rp=C.POINTER(Runtime);u=C.c_uint16;s=C.c_int16;pt=C.POINTER(Point);rc=C.POINTER(Rect);tr=C.POINTER(Track)
        sig={
            'sticky_dialog':(None,rp,u,pt,s),'draw_frame':(None,rp,u,rc,s),
            'draw_button':(None,rp,u,rc,u,s,u),'init_button_track':(None,rp,tr,u,u,rc,u,u,u,u,u),
            'track_button':(s,rp,tr),'get_mouse_move':(s,rp,pt),
            'dib_num_colors':(u,C.c_void_p),'palette_size':(u,C.c_void_p),
            'dib_blt':(s,rp,u,s,s,s,s,u,s,s,s,s,C.c_uint32),
            'center_text':(None,rp,u,s,s,u,s),'rect_center_text':(None,rp,u,rc,u,s),
            'right_text':(None,rp,u,s,s,u,s,s),'invalidate_advantage':(None,rp,u),
            'draw_advantage':(None,rp,u,rc,u),'race_title':(None,rp,u,s),
            'race_wizard':(s,rp,u,s,s),'new_race':(s,rp,u),'race_hit_test':(s,rp,Point),
            'track_race2':(s,rp,u,Point,u),'track_race3':(s,rp,u,Point,u),
            'draw_race2':(None,rp,u,u,u),'draw_race3':(None,rp,u,u,s),
            **{'race_dialog'+str(i):(s,rp,u,u,u,C.c_uint32) for i in range(1,7)}}
        for n,t in sig.items():f=getattr(cls.lib,'stars_'+n);f.restype=t[0];f.argtypes=t[1:]
        cls.procs={p['name']:(p['ne_segment'],p['ne_offset']) for p in json.loads((ROOT/'analysis/debug/procedures.json').read_text())}
    @classmethod
    def tearDownClass(cls):cls.temp.cleanup()
    def setUp(self):
        self.m=RaceUiMachine()
        self.m.put(0x229a,bytes(2));self.m.put(0x6e,words_bytes([0x2800]))
        self.m.put(0x4f9a,self.m.get(0xda2,192));self.m.put(0x501a,bytes(64));self.m.put(0x501a,b'Synthetic\0');self.m.put(0x503a,b'Synthetics\0')
        for off,val in [(0x2602,1),(0x48ac,0xb000),(0x23fa,15),(0x5320,0),(0x5476,33),(0x5314,44),(0x26b6,55)]:self.m.put(off,words_bytes([val]))
        self.m.put(0xc000,struct.pack('<IiiHHIIIIII',40,256,128,1,8,0,32768,0,0,256,256))
        for off in range(0x10,0x20,2):self.m.put(off,words_bytes([off+1000]))
        self.m.put(0x12ec,words_bytes([65535,65535]));self.m.put(0x1308,bytes(16))
    def compare(self,h):
        self.assertEqual(h.errors,[])
        self.assertEqual(h.events,self.m.events)
        self.assertEqual(h.window_events,self.m.window_events)
        for i,(a,b) in enumerate(zip(h.world.events,self.m.world.events)):
            self.assertEqual(a,b,('event',i,h.world.events[max(0,i-3):i+3],self.m.world.events[max(0,i-3):i+3]))
        self.assertEqual(len(h.world.events),len(self.m.world.events))
        for n in ['checks','text','limits','combo','rectangles','selected','bg','fg','mode','position','result']:
            self.assertEqual(getattr(h.world,n),getattr(self.m.world,n),n)
        # 1706..170d is the private CRT double-result buffer (NE36:2090),
        # also excluded in earlier floating-point comparisons.
        a=bytes(h.data[:0xe000]);b=self.m.get(0,0xe000)
        self.assertEqual([(hex(i),x,y) for i,(x,y) in enumerate(zip(a,b)) if x!=y and not 0x1706<=i<0x170e][:40],[])
    def original(self,name,args=b''):
        seg,off=self.procs[name];return self.m.call(off,args,segment=seg,shared_stack=True,instruction_budget=100000000)&65535
    def dialog(self,page,msg,wp=0,lp=0,h=None):
        h=h or UiHost(self.lib,self.m)
        actual=getattr(self.lib,'stars_race_dialog'+str(page))(C.byref(h.runtime),100,msg,wp,lp)&65535
        seg,off=self.procs['RaceWizardDlg'+str(page)]
        expected=self.m.dialog_call(off,100,msg,wp,lp,segment=seg)
        self.assertEqual(actual,expected);self.compare(h);return h
    def test_six_pages_initialization_paint_and_commands(self):
        for readonly in [0,1]:
            for page,res,panel in [(1,0x92,1),(2,0x93,4),(3,0x94,5),(4,0x95,2),(5,0x96,3),(6,0x97,6)]:
                with self.subTest(readonly=readonly,page=page):
                    self.setUp();self.m.world.load_resource(res)
                    self.m.put(0x5320,words_bytes([readonly]));self.m.put(0x2602,words_bytes([panel]))
                    h=self.dialog(page,0x110)
                    self.dialog(page,0xf,h=h)
                    self.dialog(page,0x14,500,h=h)
                    self.dialog(page,0x19,500,6<<16,h=h)
                    self.dialog(page,0x111,0x76,h=h)
                    for command in struct.unpack('<4H',self.m.get(0x12e2,8)):
                        self.dialog(page,0x111,command,h=h)
    def test_sticky_and_frames(self):
        for init in [0,1]:
            for point in [(-1,-1),(0,0),(-500,800),(1000,700)]:
                self.setUp();self.m.put(0x3100,words_bytes(point));h=UiHost(self.lib,self.m)
                self.lib.stars_sticky_dialog(C.byref(h.runtime),100,C.cast(C.byref(h.data,0x3100),C.POINTER(Point)),init)
                self.original('StickyDlgPos',words_bytes([100,0x3100,init]));self.compare(h)
        for erase in [-2,-1,0,1]:
            self.setUp();rect=Rect(20,30,130,140);self.m.put(0x3100,bytes(rect));h=UiHost(self.lib,self.m)
            self.lib.stars_draw_frame(C.byref(h.runtime),500,C.byref(rect),erase)
            self.original('_Draw3dFrame',words_bytes([500,0x3100,erase]));self.compare(h)
    def test_buttons_directions_styles_states(self):
        for kind in [0,1,2,3,4,5,6,7,8,12,0x10,0x11,0x12,0x13,0x20,0x21,0x22,0x23,0x28,0x34,0x40,0x44,0x64,0x84,0xa0,0xa1,0xa4]:
            for down in [0,1,-1]:
                with self.subTest(kind=kind,down=down):
                    self.setUp();rect=Rect(20,30,40,51);self.m.put(0x3100,bytes(rect));self.m.put(0x3200,b'Test\0');h=UiHost(self.lib,self.m)
                    self.lib.stars_draw_button(C.byref(h.runtime),500,C.byref(rect),kind,down,0x3200)
                    self.original('DrawBtn',words_bytes([500,0x3100,kind,down,0x3200]));self.compare(h)
    def test_traits_research_immunities_and_names(self):
        for page,res,panel,ids in [(1,0x92,1,range(0x10f,0x117)),(4,0x95,2,range(0x10f,0x119)),
                                   (5,0x96,3,range(0x123,0x131)),(6,0x97,6,list(range(0x10f,0x121))+[0x123]),
                                   (2,0x93,4,range(0x123,0x126)),(3,0x94,5,[0x123])]:
            self.setUp();self.m.world.load_resource(res);self.m.put(0x2602,words_bytes([panel]))
            h=self.dialog(page,0x110);self.dialog(page,0xf,h=h)
            for id in ids:
                for selected in [1,0]:
                    with self.subTest(page=page,control=id,selected=selected):
                        self.m.world.checks[id]=selected;h.world.checks[id]=selected
                        if page==1:
                            self.m.world.text[0x10c]=h.world.text[0x10c]=b'Humanoid'
                        self.dialog(page,0x111,id,h=h)
                        self.dialog(page,0xf,h=h)
            self.dialog(page,0x111,0x42f,h=h)
    def test_mouse_tracking_all_environment_economy_and_portrait_controls(self):
        for page,res,panel,count,base in [(1,0x92,1,2,0x5926),(2,0x93,4,17,0xb000),(3,0x94,5,14,0xb000)]:
            for keyboard in [0,4]:
                for i in range(count):
                    with self.subTest(page=page,keyboard=keyboard,rect=i):
                        self.setUp();self.m.world.load_resource(res);self.m.put(0x2602,words_bytes([panel]))
                        h=self.dialog(page,0x110);self.dialog(page,0xf,h=h)
                        l,t,rr,bb=struct.unpack('<4H',self.m.get(base+i*8,8));x=(l+rr)//2;y=(t+bb)//2
                        messages=[(0x200,x,y),(0x200,rr+30,y),(0x200,l,y),(0x202,x,y)]
                        self.m.world.mouse.extend(messages);h.world.mouse.extend(messages)
                        self.dialog(page,0x201,keyboard,x|(y<<16),h=h)
    def test_modal_wizard_creates_file_with_real_callbacks(self):
        def names(world,put):
            world.text[0x10c]=b'Test';world.text[0x81b]=b'Tests';world.text[0x10d]=b''
        for dont_write in [0,1]:
            self.setUp();self.m.put(0x99c,bytes(4));self.m.put(0x7ca,bytes(8));self.m.put(0x1318,bytes(16))
            self.m.dialog_scripts=[(res,[(0x110,0,0),(0xf,0,0)]+([names] if res==0x92 else [])+[(0x111,0x42f if res!=0x97 else 0x430,0)])
                for res in [0x92,0x95,0x96,0x93,0x94,0x97]]
            self.m.dialog_accept=True;self.m.opens=[(7,0)];self.m.ticks=[123]
            h=UiHost(self.lib,self.m)
            actual=self.lib.stars_race_wizard(C.byref(h.runtime),200,0,dont_write)
            expected=self.original('RaceCreationWizard',words_bytes([200,0,dont_write]))
            self.assertEqual(h.errors,[]);self.assertEqual(actual,expected);self.assertEqual(actual,1)
            self.compare(h);self.assertEqual(h.outgoing,self.m.outgoing)
            self.assertEqual(self.m.dialog_scripts,[]);self.assertEqual(h.dialog_scripts,[])
    def run_wizard(self,scripts,readonly=0,expected_result=None):
        self.m.dialog_scripts=scripts;self.m.events.clear();self.m.window_events.clear();self.m.world.events.clear()
        self.m.outgoing.clear();self.m.opens=[(7,0)];self.m.ticks=[123];h=UiHost(self.lib,self.m)
        actual=self.lib.stars_race_wizard(C.byref(h.runtime),200,readonly,0)
        expected=self.original('RaceCreationWizard',words_bytes([200,readonly,0]))
        self.assertEqual(h.errors,[]);self.assertEqual(actual,expected)
        if expected_result is not None:self.assertEqual(actual,expected_result)
        self.compare(h);self.assertEqual(h.outgoing,self.m.outgoing);self.assertEqual(h.dialog_scripts,[]);self.assertEqual(self.m.dialog_scripts,[])
        return h
    @staticmethod
    def visit(resource,command,actions=()):return (resource,[(0x110,0,0),*actions,(0x111,command,0)])
    def test_modal_back_cancel_readonly_and_failed_finish(self):
        order=[0x92,0x95,0x96,0x93,0x94,0x97]
        for i in range(6):
            self.setUp();prefix=[self.visit(r,0x42f) for r in order[:i]]
            self.run_wizard(prefix+[self.visit(order[i],2)],expected_result=0)
            self.setUp();self.run_wizard(prefix+[self.visit(order[i],0x430)],readonly=1,expected_result=0)
            if i:
                self.setUp();self.run_wizard(prefix+[self.visit(order[i],0x42e),self.visit(order[i-1],2)],expected_result=0)
        self.setUp();self.m.save_choices=[False]
        self.run_wizard([self.visit(0x92,0x430),self.visit(0x92,2)],expected_result=0)
        def costly(world,put):
            put(0x4faa,bytes([255]*9));put(0x4fb3,bytes([20]))
        self.setUp();self.run_wizard([self.visit(0x92,0x42f),self.visit(0x95,0x430,[costly]),self.visit(0x95,2)],expected_result=0)
    def test_modal_password_wrong_then_correct_and_cancel(self):
        def set_pass(world,put):world.text[0x10d]=b'test'
        def wrong(world,put):world.text[0x10c]=b'wrong'
        def correct(world,put):world.text[0x10c]=b'test'
        self.setUp();self.m.timer_values=[0,2000]
        h=self.run_wizard([self.visit(0x92,0x430,[set_pass]),(0x8c,[(0x110,0,0),wrong,(0x111,1,0),correct,(0x111,1,0)])],expected_result=1)
        self.assertEqual(bytes(h.data[0x140e:0x1410]),words_bytes([1]))
        self.setUp();self.run_wizard([self.visit(0x92,0x430,[set_pass]),self.visit(0x8c,2),self.visit(0x92,2)],expected_result=0)
    def test_original_wizard_name_tail_checksum_failure_u020(self):
        def long_name(world,put):world.text[0x10c]=b'LongRaceName';world.text[0x81b]=b'LongRaceNames'
        def select_custom(world,put):
            # The user selects Custom and then shortens both text fields. The
            # auto-radio action clears tails before editing, not on page exit.
            for id in range(0x10f,0x117):world.checks[id]=int(id==0x116)
        def short_custom(world,put):
            world.text[0x10c]=b'X';world.text[0x81b]=b'Xs'
        self.setUp();self.m.put(0x99c,bytes(4));self.m.put(0x7ca,bytes(8))
        first=self.run_wizard([self.visit(0x92,0x430,[long_name])],expected_result=1)
        payload=bytes(first.outgoing)
        # The first saved record is valid; reloading it supplies the old names.
        self.m.incoming=payload;self.m.position=0;self.m.opens=[(7,0)]
        self.m.events.clear();self.m.window_events.clear();self.m.world.events.clear();h=UiHost(self.lib,self.m)
        self.m.put(0x3000,b'SYNTH.R1\0');C.memmove(C.byref(h.data,0x3000),b'SYNTH.R1\0',9)
        actual=self.lib.stars_was_race_file(C.byref(h.runtime),0x3000,0)
        expected=self.original('FWasRaceFile',words_bytes([0x3000,0]));self.assertEqual(actual,expected);self.assertEqual(actual,1);self.compare(h)
        second=self.run_wizard([self.visit(0x92,0x430,[select_custom,(0x111,0x116,0),short_custom])],expected_result=1)
        self.assertNotEqual(bytes(second.data[0x501c:0x503a]),bytes(30))
        self.m.incoming=bytes(second.outgoing);self.m.position=0;self.m.opens=[(7,0)]
        self.m.events.clear();self.m.window_events.clear();self.m.world.events.clear();h=UiHost(self.lib,self.m)
        actual=self.lib.stars_was_race_file(C.byref(h.runtime),0x3000,0)
        expected=self.original('FWasRaceFile',words_bytes([0x3000,0]));self.assertEqual(actual,expected);self.assertEqual(actual,0);self.compare(h)
    def test_new_race_menu_initialization_and_random_template(self):
        for selection in [0x10f,0x115,0x116]:
            self.setUp();self.m.put(0x99c,bytes(4));self.m.put(0x7ca,bytes(8))
            def choose(world,put):
                for id in range(0x10f,0x117):world.checks[id]=int(id==selection)
            self.m.dialog_scripts=[self.visit(0x92,0x430,[choose,(0x111,selection,0)])]
            self.m.ticks=[123];self.m.opens=[(7,0)];h=UiHost(self.lib,self.m)
            self.assertEqual(self.lib.stars_new_race(C.byref(h.runtime),200),1)
            self.original('CommandHandler',words_bytes([200,0x81]))
            # Original outer menu handler has a larger private stack. Only
            # the expired rectangle pointer's numeric address is nonportable.
            h.data[0x48ac:0x48ae]=self.m.get(0x48ac,2)
            self.compare(h);self.assertEqual(h.outgoing,self.m.outgoing)
            if selection==0x115:
                # The wizard stores the random-race marker. Materialization
                # is deferred to GenerateWorld, whose CreateRandomRace core
                # has its own existing original-machine validation.
                self.lib.stars_get_race_trait.argtypes=[PTR,C.c_uint16,C.c_int16]
                self.assertEqual(self.lib.stars_get_race_trait(h.data,0x4f9a,30),1)
    def test_text_alignment_and_dib_helpers(self):
        for size,depth,colors in [(12,1,0),(12,4,0),(12,8,0),(12,24,0),(40,1,0),(40,4,0),(40,8,0),(40,24,0),(40,8,17)]:
            self.setUp();header=bytearray(40);struct.pack_into('<I',header,0,size)
            struct.pack_into('<H',header,10 if size==12 else 14,depth);struct.pack_into('<I',header,32,colors)
            self.m.put(0xc000,bytes(header));h=UiHost(self.lib,self.m)
            for c,original in [('dib_num_colors','DibNumColors'),('palette_size','PaletteSize')]:
                value=getattr(self.lib,'stars_'+c)(C.byref(h.data,0xc000))
                self.assertEqual(value,self.original(original,words_bytes([0xc000,0x3000])))
            self.lib.stars_dib_blt(C.byref(h.runtime),500,-2,3,32,32,33,0,1,32,32,0xcc0020)
            self.original('DibBlt',words_bytes([500,-2,3,32,32,33,0,1,32,32])+struct.pack('<I',0xcc0020));self.compare(h)
        for bitmap,lock in [(0,0x3000c000),(33,0)]:
            self.setUp();self.m.world.lock_result=lock;h=UiHost(self.lib,self.m)
            value=self.lib.stars_dib_blt(C.byref(h.runtime),500,0,0,20,20,bitmap,0,0,20,20,0xcc0020)
            self.assertEqual(value,self.original('DibBlt',words_bytes([500,0,0,20,20,bitmap,0,0,20,20])+struct.pack('<I',0xcc0020)));self.compare(h)
        for length in [-1,0,3]:
            for erase in [0,7,100]:
                self.setUp();self.m.put(0x3200,b'Synthetic\0');self.m.put(0x3100,words_bytes([20,30,31,42]));h=UiHost(self.lib,self.m)
                self.lib.stars_right_text(C.byref(h.runtime),500,100,15,0x3200,length,erase)
                self.original('RightTextOut',words_bytes([500,100,15,0x3200,length,erase]));self.compare(h)
                self.lib.stars_rect_center_text(C.byref(h.runtime),500,C.cast(C.byref(h.data,0x3100),C.POINTER(Rect)),0x3200,length)
                self.original('RcCtrTextOut',words_bytes([500,0x3100,0x3200,length]));self.compare(h)
    def test_button_tracking_flags_delays_and_mouse_message_filter(self):
        for dc in [0,500]:
            for down,no_redraw in [(0,0),(1,0),(0,1),(1,1)]:
                self.setUp();self.m.put(0x3100,words_bytes([20,30,50,60]));self.m.put(0x3200,bytes([0xaa]*24));h=UiHost(self.lib,self.m)
                track=C.cast(C.byref(h.data,0x3200),C.POINTER(Track));rect=C.cast(C.byref(h.data,0x3100),C.POINTER(Rect))
                self.lib.stars_init_button_track(C.byref(h.runtime),track,100,dc,rect,0x22,80,down,no_redraw,0)
                self.original('InitBtnTrack',words_bytes([0x3200,100,dc,0x3100,0x22,80,down,no_redraw,0]));self.compare(h)
                for messages in [[],[(0x123,1,1),(0x200,100,100),(0x200,20,30)],[(0x202,25,35)]]:
                    self.m.world.mouse.extend(messages);h.world.mouse.extend(messages)
                    value=self.lib.stars_track_button(C.byref(h.runtime),track)
                    self.assertEqual(value,self.original('FTrackBtn',words_bytes([0x3200])));self.compare(h)
        for messages in [[],[(0x123,2,3)],[(0x123,4,5),(0x200,10,20)],[(0x202,30,40)]]:
            self.setUp();self.m.put(0x3200,words_bytes([1,2]));self.m.world.mouse.extend(messages);h=UiHost(self.lib,self.m)
            value=self.lib.stars_get_mouse_move(C.byref(h.runtime),C.cast(C.byref(h.data,0x3200),C.POINTER(Point)))
            self.assertEqual(value,self.original('FGetMouseMove',words_bytes([0x3200])));self.compare(h)
    def test_alternate_reality_and_environment_limits(self):
        self.m.put(0x4fe6,bytes([8]));self.m.put(0x2602,words_bytes([5]));self.m.world.load_resource(0x94)
        h=self.dialog(3,0x110);self.dialog(3,0xf,h=h)
        self.assertEqual(self.m.get(0x4b94,2),words_bytes([2]))
        for low,high,hit in [(0,20,0),(80,100,2),(40,60,4),(0,100,3),(0,100,1)]:
            self.setUp();self.m.put(0x4faa,bytes([(low+high)//2]*3));self.m.put(0x4fad,bytes([low]*3));self.m.put(0x4fb0,bytes([high]*3))
            self.m.put(0x2602,words_bytes([4]));self.m.world.load_resource(0x93)
            h=self.dialog(2,0x110);self.dialog(2,0xf,h=h)
            l,t,rr,bb=struct.unpack('<4H',self.m.get(0xb000+hit*8,8));x=(l+rr)//2;y=(t+bb)//2
            self.m.world.mouse.extend([(0x200,x,y),(0x202,x,y)]);h.world.mouse.extend([(0x200,x,y),(0x202,x,y)])
            self.dialog(2,0x201,4,x|(y<<16),h=h)

if __name__=='__main__':unittest.main()
