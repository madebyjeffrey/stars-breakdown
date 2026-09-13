#!/usr/bin/env python3
"""NE static loader and complete file inventory for the supplied Stars! binary.
Synthetic selectors represent addresses only: imports are not executable stubs.
OS floating-point fixups retain hardware-x87 instructions, as Wine's NE loader does.
"""
import argparse
import collections
import hashlib
import json
from pathlib import Path
import struct
try:
    from tools.ne_resources import decode
    from tools.ne_report import render
except ModuleNotFoundError:
    from ne_resources import decode
    from ne_report import render

ROOT=Path(__file__).resolve().parents[1]

def u(fmt,b,p=0):return struct.unpack_from('<'+fmt,b,p)
def sha(b):return hashlib.sha256(b).hexdigest()
def pascal(b,p):
    n=b[p]
    if p+1+n>len(b):raise ValueError('Truncated string')
    return b[p+1:p+1+n].decode('cp1252',errors='replace')

def chain_sites(raw,start,additive,width):
    sites=[];p=start
    while True:
        if p in sites:raise ValueError('Cyclic relocation chain')
        if p<0 or p+max(width,2)>len(raw):raise ValueError('Relocation out of bounds')
        sites.append(p)
        if additive:break
        p=u('H',raw,p)[0]
        if p==0xffff:break
    return sites

def patch(mem,p,kind,selector,offset,additive):
    if kind==2:
        if additive and u('H',mem,p)[0]:raise ValueError('Nonzero additive selector')
        struct.pack_into('<H',mem,p,selector)
    elif kind==3:
        add=u('H',mem,p)[0] if additive else 0
        struct.pack_into('<HH',mem,p,(offset+add)&0xffff,selector)
    elif kind==5:
        add=u('H',mem,p)[0] if additive else 0
        struct.pack_into('<H',mem,p,(offset+add)&0xffff)
    elif kind==0:
        mem[p]=(offset+(mem[p] if additive else 0))&255
    else:raise ValueError('Unsupported address kind')

class NE:
    def __init__(self,path):
        self.b=path.read_bytes();b=self.b;self.n=u('I',b,60)[0];n=self.n
        assert b[:2]==b'MZ' and b[n:n+2]==b'NE'
        self.regions=[];self.segments=[];self.raw=[];self.memory=[]
        keys={4:'entry_offset',6:'entry_bytes',12:'flags',14:'autodata',16:'heap_bytes',18:'stack_bytes',
              20:'ip',22:'cs',24:'sp',26:'ss',28:'segment_count',30:'module_count',32:'nonresident_bytes',
              34:'segment_table',36:'resource_table',38:'resident_table',40:'module_table',42:'import_table',
              48:'movable_entries',50:'alignment_shift',62:'expected_windows'}
        self.header={v:u('H',b,n+k)[0] for k,v in keys.items()}
        h=self.header;h.update(ne_file_offset=n,linker_version=f'{b[n+2]}.{b[n+3]}',nonresident_offset=u('I',b,n+44)[0],target_os=b[n+54],other_flags=b[n+55])
        assert h['target_os']==2 and not h['flags']&0x8800, 'Unsupported self-loading/library module'
        self.region(0,64,'DOS header with NE pointer');self.region(64,u('H',b,8)[0]*16,'DOS header reserved area')
        ds=u('H',b,8)[0]*16;self.region(ds,n,'DOS stub program and padding')
        self.region(n,n+64,'NE header')
        for i in range(h['segment_count']):
            p=n+h['segment_table']+i*8;sector,size,flags,alloc=u('4H',b,p)
            assert not flags&(8|0x2000),'Iterated/32-bit segment unsupported'
            size=size or 65536;alloc=alloc or 65536;off=sector<<h['alignment_shift'] if sector else None
            data=b[off:off+size] if off is not None else b''
            assert len(data)==(size if off is not None else 0)
            mem=data+bytes(max(size,alloc)-len(data))
            s=dict(segment=i+1,file_offset=off,file_bytes=len(data),allocation=alloc,static_memory_bytes=len(mem),flags=flags,
                   kind='data' if flags&1 else 'code',moveable=bool(flags&16),shareable=bool(flags&32),preload=bool(flags&64),
                   discardable=bool(flags&0x1000),has_relocations=bool(flags&256),synthetic_selector=0x1000+(i+1)*8)
            self.segments.append(s);self.raw.append(mem);self.memory.append(bytearray(mem))
            if data:self.region(off,off+size,f'segment {i+1} initialized bytes')
        self.region(n+h['segment_table'],n+h['segment_table']+8*h['segment_count'],'NE segment table')
        self.entries=[];p=n+h['entry_offset'];ordinal=1
        while True:
            count=b[p];p+=1
            if not count:break
            seg=b[p];p+=1
            for i in range(count):
                e=dict(ordinal=ordinal);ordinal+=1
                if seg==0:e['kind']='unused'
                elif seg==255:
                    flags,int3f,ns,off=u('BHBH',b,p);p+=6;assert int3f==0x3fcd
                    e.update(kind='movable',flags=flags,segment=ns,offset=off)
                else:
                    flags,off=u('BH',b,p);p+=3;e.update(kind='constant' if seg==254 else 'fixed',flags=flags,segment=seg,offset=off)
                if e['kind'] in ('fixed','movable'):assert e['offset']<len(self.raw[e['segment']-1])
                self.entries.append(e)
        end=n+h['entry_offset']+h['entry_bytes'];assert p<=end and not any(b[p:end])
        self.region(n+h['entry_offset'],end,'NE entry table')
        def names(p,end=None):
            start=p;rows=[]
            while b[p]:
                name=pascal(b,p);p+=1+b[p];ord=u('H',b,p)[0];p+=2;rows.append(dict(name=name,ordinal=ord))
            p+=1
            if end:assert p<=end and not any(b[p:end]);p=end
            return rows,start,p
        self.resident,a,z=names(n+h['resident_table']);self.region(a,z,'NE resident names')
        self.nonresident,a,z=names(h['nonresident_offset'],h['nonresident_offset']+h['nonresident_bytes']);self.region(a,z,'NE nonresident names')
        self.modules=[]
        for i in range(h['module_count']):
            off=u('H',b,n+h['module_table']+i*2)[0]
            self.modules.append(pascal(b,n+h['import_table']+off))
        self.region(n+h['module_table'],n+h['module_table']+2*h['module_count'],'NE module references')
        self.region(n+h['import_table'],n+h['entry_offset'],'NE imported names')
        self.exports=[]
        for name in self.resident+self.nonresident:
            if name['ordinal']:
                self.exports.append(dict(**name,entry=self.entries[name['ordinal']-1]))
        self.relocations=[];self.imports={};self.osfixups=collections.Counter()
        refs=json.loads((ROOT/'references/win16_exports.json').read_text())
        for s,raw,mem in zip(self.segments,self.raw,self.memory):
            if not s['has_relocations']:continue
            p=s['file_offset']+s['file_bytes'];count=u('H',b,p)[0]
            self.region(p,p+2+count*8,f"segment {s['segment']} relocation table")
            for i in range(count):
                rp=p+2+i*8;kind,flags,start,t1,t2=u('BBHHH',b,rp)
                assert flags&~7==0 and kind in (0,2,3,5)
                typ=flags&3;additive=bool(flags&4)
                r=dict(segment=s['segment'],record_file_offset=rp,address_kind=kind,flags=flags,source_offset=start,target1=t1,target2=t2,additive=additive)
                if typ==3:
                    assert flags==7 and kind==5 and t2==0 and t1 in (1,2,3,4,5,6)
                    assert start+2<=len(raw)
                    pattern=raw[start:start+2]
                    expected={2:b'\x9b\x36',3:b'\x9b\x2e',4:b'\x9b\x26',6:b'\x90\x9b'}
                    if t1 in expected:assert pattern==expected[t1], 'Unexpected FPU fixup instruction'
                    elif t1==5:assert pattern[0]==0x9b and 0xd8<=pattern[1]<=0xdf
                    else:raise ValueError('FPU fixup type 1 not present in this baseline; requires validation')
                    r.update(target_kind='os_fpu_fixup',sites=[start],policy='retain hardware-x87 bytes',
                        instruction_pattern={2:'FWAIT + SS override',3:'FWAIT + CS override',4:'FWAIT + ES override',5:'FWAIT + x87 opcode',6:'NOP + FWAIT'}[t1],
                        original_bytes=raw[start:start+3].hex())
                    self.osfixups[t1]+=1;self.relocations.append(r);continue
                if typ==0:
                    if t1&255==255:
                        e=self.entries[t2-1];ns,offset=e['segment'],e['offset']
                    else:ns,offset=t1,t2
                    assert 1<=ns<=len(self.segments) and offset<len(self.raw[ns-1])
                    selector=self.segments[ns-1]['synthetic_selector']
                    r.update(target_kind='internal',target_segment=ns,target_offset=offset)
                else:
                    module=self.modules[t1-1]
                    name=pascal(b,n+h['import_table']+t2) if typ==2 else refs[module]['ordinals'].get(str(t2))
                    key=f'{module}!'+(name if typ==2 else f'#{t2}')
                    selector=0x8000+t1*8;offset=t2 if typ==1 else (0x8000+t2)&0xffff
                    self.imports[key]=dict(module=module,ordinal=t2 if typ==1 else None,name=name,
                        name_source='binary' if typ==2 else refs[module]['source'],synthetic_selector=selector,synthetic_offset=offset)
                    r.update(target_kind='import',import_key=key)
                width={0:1,2:2,3:4,5:2}[kind]
                sites=chain_sites(raw,start,additive,width)
                for site in sites:patch(mem,site,kind,selector,offset,additive)
                r.update(sites=sites,synthetic_target=dict(selector=selector,offset=offset))
                self.relocations.append(r)
        # Windows also patches exported prologues; retain this separately for transparency.
        self.prologue_patches=[]
        for e in self.entries:
            if e['kind'] not in ('fixed','movable'):continue
            mem=self.memory[e['segment']-1];p=e['offset'];old=bytes(mem[p:p+3])
            if old in (b'\x1e\x58\x90',b'\x8c\xd8\x90'):
                new=b'\x8c\xd8\x90'
                if e['flags']&2:new=b'\xb8'+struct.pack('<H',self.segments[h['autodata']-1]['synthetic_selector'])
                elif h['flags']&2 and e['flags']&1:new=b'\x90\x90\x90'
                mem[p:p+3]=new
                self.prologue_patches.append(dict(ordinal=e['ordinal'],segment=e['segment'],offset=p,before=old.hex(),after=new.hex()))
        self.resources=[];rt=n+h['resource_table'];shift=u('H',b,rt)[0];p=rt+2
        def rid(v):return v&0x7fff if v&0x8000 else pascal(b,rt+v)
        while True:
            typ=u('H',b,p)[0]
            if not typ:p+=2;break
            typ,count,res=u('HHI',b,p);assert res==0;p+=8
            for i in range(count):
                off,length,flags,id,handle,usage=u('6H',b,p);p+=12;off<<=shift;length<<=shift
                assert off+length<=len(b)
                data=b[off:off+length];typeid=rid(typ);name=rid(id)
                r=dict(index=len(self.resources),type=typeid,id=name,file_offset=off,bytes=length,flags=flags,sha256=sha(data),decoded=decode(typeid,data))
                self.resources.append(r);self.region(off,off+length,f'resource {typeid}/{name}')
        self.resource_shift=shift
        self.region(rt,n+h['resident_table'],'NE resource table and name pool')
        debug_start=len(b)-u('I',b,len(b)-4)[0]
        assert b[debug_start:debug_start+4]==b'NB09'
        self.region(debug_start,len(b),'CodeView NB09 debug block and trailer')
        self.complete_regions()
        for reg in ('cs','ss'):assert 1<=h[reg]<=len(self.segments)
        assert h['ip']<len(self.raw[h['cs']-1])
        self.initial_state=dict(initial_dgroup_allocation_before_allocator_rounding=h['heap_bytes']+h['stack_bytes']+self.segments[h['autodata']-1]['allocation'],cs_ne=h['cs'],ip=h['ip'],ss_ne=h['ss'],sp_header=h['sp'],ds_ne=h['autodata'],
            heap_reserve=h['heap_bytes'],stack_reserve=h['stack_bytes'],
            note='Header SP=0 requests loader placement; static segment images exclude dynamic heap/stack expansion. Synthetic selectors are not Windows runtime values.')

    def region(self,a,b,label):
        assert 0<=a<=b<=len(self.b),(a,b,label)
        if a<b:self.regions.append(dict(start=a,end_exclusive=b,bytes=b-a,classification=label))

    def complete_regions(self):
        rows=sorted(self.regions,key=lambda r:r['start']);out=[];p=0
        for r in rows:
            assert r['start']>=p,('overlapping file regions',r,p)
            if r['start']>p:
                data=self.b[p:r['start']]
                out.append(dict(start=p,end_exclusive=r['start'],bytes=len(data),classification='zero alignment padding' if not any(data) else 'unclassified nonzero gap',sha256=sha(data)))
            out.append(r);p=r['end_exclusive']
        assert p==len(self.b)
        self.regions=out
        if any(r['classification']=='unclassified nonzero gap' for r in out):
            raise ValueError('Unclassified file content: investigate before accepting layout')

    def ne_to_file(self,segment,offset):
        s=self.segments[segment-1]
        if not 0<=offset<s['static_memory_bytes']:raise ValueError('Outside static memory')
        return s['file_offset']+offset if offset<s['file_bytes'] else None

    def cv_to_ne(self,logical_segment,offset):
        maps=json.loads((ROOT/'analysis/segment_map.json').read_text())
        if not 1<=logical_segment<=len(maps):raise ValueError('Invalid CodeView segment')
        m=maps[logical_segment-1]
        mapped=m['offset']+offset
        self.ne_to_file(m['frame'],mapped)  # Validate extent, including zero-fill.
        return m['frame'],mapped

    def selector_to_ne(self,selector,offset):
        for s in self.segments:
            if s['synthetic_selector']==selector:
                self.ne_to_file(s['segment'],offset)
                return s['segment'],offset
        raise ValueError('Not an internal synthetic selector')

    def file_to_ne(self,offset):
        for s in self.segments:
            if s['file_offset'] is not None and s['file_offset']<=offset<s['file_offset']+s['file_bytes']:
                return s['segment'],offset-s['file_offset']
        return None

    def report(self):
        return dict(input_sha256=sha(self.b),header=self.header,initial_state=self.initial_state,
            segments=self.segments,entries=self.entries,exports=self.exports,resident_names=self.resident,nonresident_names=self.nonresident,
            imports=list(self.imports.values()),resources=self.resources,relocations=self.relocations,
            prologue_patches=self.prologue_patches,file_regions=self.regions,
            analysis_model='Static memory with synthetic internal selectors 0x1000+8*NE_ordinal and import selectors 0x8000+8*module_index; imported ordinal used as synthetic offset. Not a runnable image.',
            summary=dict(resources=len(self.resources),resource_types=dict(collections.Counter(str(r['type']) for r in self.resources)),
                entry_records=len(self.entries),exports=len(self.exports),unique_imports=len(self.imports),
                relocation_records=len(self.relocations),address_fixup_sites=sum(len(r['sites']) for r in self.relocations if r['target_kind']!='os_fpu_fixup'),
                os_fpu_fixups=dict(self.osfixups),unclassified_file_bytes=sum(r['bytes'] for r in self.regions if r['classification']=='unclassified nonzero gap')))

    def write(self,out):
        out.mkdir(parents=True,exist_ok=True)
        for s,raw,mem in zip(self.segments,self.raw,self.memory):
            p=out/'memory';p.mkdir(exist_ok=True)
            (p/f"segment_{s['segment']:02d}.bin").write_bytes(mem)
            s['image_sha256']=sha(mem)
        for r in self.resources:
            p=out/'resources';p.mkdir(exist_ok=True)
            name=f"resource_{r['index']:03d}_type_{r['type']}.bin";r['extracted_path']='resources/'+name
            (p/name).write_bytes(self.b[r['file_offset']:r['file_offset']+r['bytes']])
        (out/'layout.json').write_text(json.dumps(self.report(),indent=2)+'\n')
        (out/'README.md').write_text(render(self.report()))
        print(json.dumps(self.report()['summary'],indent=2))

if __name__=='__main__':
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--out',type=Path,default=ROOT/'analysis/ne')
    args=ap.parse_args();NE(ROOT/'target/stars.exe').write(args.out)
