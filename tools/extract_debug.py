#!/usr/bin/env python3
"""Read-only NE / CodeView NB09 inventory. Python standard library only.
Format references: Microsoft microsoft-pdb/include/cvinfo.h and Wine
include/wine/mscvpdb.h. Unknown records are retained as hex, never guessed.
"""
import argparse
import collections
import hashlib
import json
from pathlib import Path
import struct


def unpack(fmt, b, p=0):
    return struct.unpack_from('<' + fmt, b, p)


def pascal(b, p):
    n = b[p]
    if p + 1 + n > len(b):
        raise ValueError('Truncated length-prefixed string')
    return b[p + 1:p + 1 + n].decode('latin1')


def numeric(b, p):
    v, = unpack('H', b, p)
    if v < 0x8000:
        return v, p + 2
    fmt = {0x8000: 'b', 0x8001: 'h', 0x8002: 'H', 0x8003: 'i', 0x8004: 'I'}.get(v)
    if fmt is None:
        raise ValueError('Unsupported numeric leaf: %04x' % v)
    return unpack(fmt, b, p + 2)[0], p + 2 + struct.calcsize('<' + fmt)


def extract(path, out):
    b = path.read_bytes()
    ne, = unpack('I', b, 0x3c)
    if b[:2] != b'MZ' or b[ne:ne+2] != b'NE' or b[-8:-4] != b'NB09':
        raise ValueError('Expected MZ/NE with trailing NB09 directory pointer')
    base = len(b) - unpack('I', b, len(b)-4)[0]
    if b[base:base+4] != b'NB09':
        raise ValueError('CodeView trailer does not match header')
    directory = base + unpack('I', b, base+4)[0]
    hs, es, count, nxt, flags = unpack('HHIII', b, directory)
    if hs < 16 or es < 12 or nxt:
        raise ValueError('Unsupported CodeView directory layout')
    entries = []
    for i in range(count):
        kind, mod, off, size = unpack('HHII', b, directory+hs+i*es)
        if base+off+size > len(b)-8:
            raise ValueError('Subsection exceeds debug block')
        entries.append(dict(kind=kind, module=mod, offset=off, size=size))
    def data(e):
        return b[base+e['offset']:base+e['offset']+e['size']]
    modules = {}
    for e in entries:
        if e['kind'] == 0x120:
            s = data(e)
            ovl, lib, ns = unpack('HHH', s)
            spans = [dict(zip(('segment','padding','offset','size'),unpack('HHII',s,8+j*12))) for j in range(ns)]
            modules[e['module']] = dict(name=pascal(s,8+ns*12), spans=spans, library=lib)
    segments = []
    ns, = unpack('H',b,ne+0x1c)
    st, = unpack('H',b,ne+0x22)
    shift, = unpack('H',b,ne+0x32)
    for i in range(ns):
        sector, size, fl, alloc = unpack('HHHH',b,ne+st+8*i)
        segments.append(dict(segment=i+1,file_offset=sector<<shift if sector else None,
                             size=size or 65536, flags=fl,allocation=alloc or 65536))
    segmap = []
    for e in entries:
        if e['kind'] == 0x12d:
            s=data(e)
            for j in range(unpack('H',s)[0]):
                segmap.append(dict(zip(('flags','overlay','group','frame','name_index','class_index','offset','size'),unpack('6H2I',s,4+j*20))))
    functions, symbols, compilers = [], [], []
    record_counts = collections.Counter()
    for e in entries:
        if e['kind'] not in (0x125,0x129,0x12a,0x134):
            continue
        s=data(e)
        if e['kind']==0x125:
            if unpack('I',s)[0]!=1:
                raise ValueError('Unsupported symbol signature')
            p,end=4,len(s)
        else:
            p,end=16,16+unpack('I',s,4)[0]
        if end>len(s):
            raise ValueError('Invalid symbol table size')
        current=None
        while p<end:
            length,kind=unpack('HH',s,p)
            if length<2 or p+length+2>end:
                raise ValueError('Invalid symbol record length')
            r=s[p:p+length+2]
            record_counts[f'{kind:04x}']+=1
            item=dict(module=e['module'],record_offset=p,kind=f'{kind:04x}',raw=r.hex())
            if current is not None and p>=current['end_record']:
                current=None
            if kind in (0x104,0x105):
                parent,endrec,nxtrec,size,ds,de,off,seg,typ=unpack('III6H',r,4)
                item.update(name=pascal(r,29),segment=seg,offset=off,size=size,type_index=typ,
                            end_record=endrec,flags=r[28],locals=[])
                current=item
                functions.append(item)
            elif kind in (0x101,0x102,0x103):
                off,seg,typ=unpack('HHH',r,4)
                item.update(name=pascal(r,10),offset=off,segment=seg,type_index=typ)
            elif kind==0x100:
                off,typ=unpack('hH',r,4)
                item.update(name=pascal(r,8),bp_offset=off,type_index=typ)
                if current is not None:
                    current['locals'].append({k:v for k,v in item.items() if k!='raw'})
            elif kind==0x4:
                item.update(type_index=unpack('H',r,4)[0],name=pascal(r,6))
            elif kind==0x1:
                item.update(compiler=pascal(r,8),machine=r[4],compiler_flags=r[5:8].hex())
                compilers.append(item)
            symbols.append(item)
            p+=length+2
    source_lines=[]
    for e in entries:
        if e['kind']!=0x127:
            continue
        s=data(e)
        for j in range(unpack('H',s)[0]):
            fp=unpack('I',s,4+4*j)[0]
            n=unpack('H',s,fp)[0]
            filename=pascal(s,fp+4+12*n)
            for k in range(n):
                lp=unpack('I',s,fp+4+4*k)[0]
                seg,nl=unpack('HH',s,lp)
                for l in range(nl):
                    source_lines.append(dict(module=e['module'],file=filename,segment=seg,
                        offset=unpack('I',s,lp+4+4*l)[0],line=unpack('H',s,lp+4+4*nl+2*l)[0]))
    types=[]
    for e in entries:
        if e['kind']!=0x12b:
            continue
        s=data(e);nt=unpack('I',s,4)[0];start=8+4*nt
        for j in range(nt):
            p=start+unpack('I',s,8+4*j)[0]
            length,leaf=unpack('HH',s,p)
            r=s[p+2:p+2+length]
            if len(r)!=length:
                raise ValueError('Truncated type record')
            t=dict(index=0x1000+j,leaf=f'{leaf:04x}',raw=r.hex())
            if leaf in (4,5,6):
                t.update(member_count=unpack('H',r,2)[0],field_list=unpack('H',r,4)[0],properties=unpack('H',r,6)[0])
                size,np=numeric(r,8 if leaf==6 else 12)
                t.update(size=size,name=pascal(r,np))
            elif leaf==8:
                rt,cc,attr,narg,al=unpack('HBBHH',r,2)
                t.update(return_type=rt,calling_convention=cc,parameter_count=narg,argument_list=al)
            elif leaf in (1,2):
                t.update(attributes=unpack('H',r,2)[0],underlying_type=unpack('H',r,4)[0])
            elif leaf==3:
                size,np=numeric(r,6)
                t.update(element_type=unpack('H',r,2)[0],size=size)
            elif leaf==0x206:
                t.update(bit_length=r[2],bit_position=r[3],underlying_type=unpack('H',r,4)[0])
            elif leaf==0x201:
                na=unpack('H',r,2)[0]
                t['arguments']=list(unpack('H'*na,r,4))
            types.append(t)
    by_type={t['index']:t for t in types}
    for t in types:
        if 'field_list' not in t or not t['field_list']:
            continue
        r=bytes.fromhex(by_type[t['field_list']]['raw']);p=2;members=[]
        while p<len(r):
            if r[p]>=0xf0:
                skip=r[p]&15
                if not skip: break
                p+=skip;continue
            leaf=unpack('H',r,p)[0]
            if leaf!=0x406:
                t['undecoded_field_tail']=r[p:].hex();break
            typ,attr=unpack('HH',r,p+2)
            off,np=numeric(r,p+6);name=pascal(r,np)
            members.append(dict(name=name,type_index=typ,offset=off,attributes=attr))
            p=np+1+r[np]
        t['members']=members
        if 'undecoded_field_tail' not in t and len(members)!=t['member_count']:
            raise ValueError('Member count mismatch: '+t['name'])
    # Logical CodeView segment numbers differ from NE segment ordinals.
    for f in functions:
        sm=segmap[f['segment']-1];seg=segments[sm['frame']-1];off=sm['offset']+f['offset']
        if seg['file_offset'] is None or off+f['size']>max(seg['size'],seg['allocation']) or seg['flags']&1:
            raise ValueError('Function does not fit: '+repr((f,sm,seg)))
        f.update(ne_segment=sm['frame'],ne_offset=off,file_offset=seg['file_offset']+off)
        f['file_backed_bytes']=max(0,min(f['size'],seg['size']-off))
        lines=[x for x in source_lines if x['module']==f['module'] and x['segment']==f['segment'] and f['offset']<=x['offset']<f['offset']+f['size']]
        if lines:
            f.update(source_file=lines[0]['file'],source_line=min(x['line'] for x in lines))
    summary=dict(input=str(path),bytes=len(b),sha256=hashlib.sha256(b).hexdigest(),
                 debug_offset=base,debug_bytes=len(b)-base,ne_segments=len(segments),
                 modules=len(modules),functions=len(functions),symbol_records=len(symbols),
                 types=len(types),source_files=sorted(set(x['file'] for x in source_lines)),
                 source_line_records=len(source_lines),symbol_record_kinds=dict(record_counts),
                 compilers=sorted(set(x['compiler'] for x in compilers)))
    out.mkdir(parents=True,exist_ok=True)
    for name,obj in dict(summary=summary,modules=modules,segments=segments,segment_map=segmap,
                         functions=functions,symbols=symbols,types=types,source_lines=source_lines,
                         debug_directory=entries).items():
        (out/(name+'.json')).write_text(json.dumps(obj,indent=2)+'\n')
    rows=['# Recovered function index','', 'Generated from CodeView records; names and source locations are original debug metadata.',
          'Addresses below are NE segment:offset, not runtime Windows selectors.','',
          '| Module | Function | Address | Bytes | Original source |','|---|---|---|---:|---|']
    for f in sorted(functions,key=lambda x:(modules[x['module']]['name'],x['offset'])):
        source=f.get('source_file','').replace('\\','/')
        rows.append(f"| {modules[f['module']]['name']} | `{f['name']}` | `{f['ne_segment']:04x}:{f['ne_offset']:04x}` | {f['size']} | {source}:{f.get('source_line','')} |")
    (out/'FUNCTIONS.md').write_text('\n'.join(rows)+'\n')
    print(json.dumps(summary,indent=2))


if __name__=='__main__':
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('executable',nargs='?',type=Path,default=Path('target/stars.exe'))
    ap.add_argument('--out',type=Path,default=Path('analysis'))
    args=ap.parse_args()
    extract(args.executable,args.out)
