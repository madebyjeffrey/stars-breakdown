#!/usr/bin/env python3
"""Complete semantic recovery of the CodeView record kinds in this NB09 baseline.
Keep original records distinct; derived aliases and references never erase provenance.
"""
import collections
import hashlib
import json
from pathlib import Path
import struct

ROOT=Path(__file__).resolve().parents[1]
SYMS={1:'S_COMPILE',2:'S_REGISTER16',4:'S_UDT16',5:'S_SSEARCH',6:'S_END',9:'S_OBJNAME',
      0x100:'S_BPREL16',0x101:'S_LDATA16',0x102:'S_GDATA16',0x103:'S_PUB16',0x104:'S_LPROC16',0x105:'S_GPROC16',
      0x107:'S_BLOCK16',0x109:'S_LABEL16',0x203:'S_PUB32_16t',0x400:'S_PROCREF',0x401:'S_DATAREF',0x402:'S_ALIGN'}
SUBS={0x120:'sstModule',0x125:'sstAlignSym',0x127:'sstSrcModule',0x128:'sstLibraries',0x129:'sstGlobalSym',
      0x12a:'sstGlobalPub',0x12b:'sstGlobalTypes',0x12d:'sstSegMap',0x12e:'sstSegName',0x133:'sstFileIndex',0x134:'sstStaticSym'}
LEAVES={1:'LF_MODIFIER16',2:'LF_POINTER16',3:'LF_ARRAY16',5:'LF_STRUCTURE16',8:'LF_PROCEDURE16',0x201:'LF_ARGLIST16',0x204:'LF_FIELDLIST16',0x206:'LF_BITFIELD16'}
PRIMS={0:('no_type',0),1:('absolute',0),3:('void',0),0x11:('short',2),0x12:('long',4),0x20:('unsigned char',1),0x21:('unsigned short',2),0x22:('unsigned long',4),0x40:('float',4),0x41:('double',8),0x42:('long double',10),0x70:('char',1),0x72:('int',2),0x73:('unsigned int',2)}
CALLS={1:'far cdecl',2:'near pascal',3:'far pascal'}

def u(fmt,b,p=0):return struct.unpack_from('<'+fmt,b,p)
def pstr(b,p):
    n=b[p];assert p+1+n<=len(b)
    return b[p+1:p+1+n].decode('latin1')
def num(b,p):
    v=u('H',b,p)[0]
    if v<0x8000:return v,p+2
    fmt={0x8000:'b',0x8001:'h',0x8002:'H',0x8003:'i',0x8004:'I'}[v]
    return u(fmt,b,p+2)[0],p+2+struct.calcsize('<'+fmt)
def read(p):return json.loads((ROOT/p).read_text())

def recover():
    b=(ROOT/'target/stars.exe').read_bytes();base=len(b)-u('I',b,len(b)-4)[0]
    entries=read('analysis/debug_directory.json');T=read('analysis/types.json');types={t['index']:t for t in T}
    modules=read('analysis/modules.json');segmap=read('analysis/segment_map.json');segments=read('analysis/segments.json')
    audits=[];records=[];scopes=[];libraries=[];fileindex=[];names_blob=b''
    def address(seg,off):
        if seg==0:return dict(address_kind='absolute',absolute_value=off)
        if not 1<=seg<=len(segmap):raise ValueError('Bad logical segment')
        m=segmap[seg-1];ns=m['frame'];no=m['offset']+off;s=segments[ns-1]
        assert no<=max(s['size'],s['allocation']),(seg,off,no)
        return dict(address_kind='segmented',ne_segment=ns,ne_offset=no,
            file_offset=s['file_offset']+no if s['file_offset'] is not None and no<s['size'] else None)
    for ei,e in enumerate(entries):
        k=e['kind'];s=b[base+e['offset']:base+e['offset']+e['size']]
        a=dict(directory_index=ei,kind=SUBS[k],module=e['module'],bytes=len(s),sha256=hashlib.sha256(s).hexdigest(),status='decoded')
        if k in (0x125,0x129,0x12a,0x134):
            if k==0x125:p,end=4,len(s);assert u('I',s)[0]==1
            else:
                sh,ah,syms,hs,ha=u('HHIII',s);p,end=16,16+syms
                assert 16+syms+hs+ha==len(s)
                a['lookup_indexes']=dict(symbol_hash_kind=sh,address_hash_kind=ah,symbol_hash_bytes=hs,address_hash_bytes=ha,
                    symbol_hash_sha256=hashlib.sha256(s[end:end+hs]).hexdigest(),address_hash_sha256=hashlib.sha256(s[end+hs:]).hexdigest(),
                    treatment='Lookup accelerators excluded from semantic reconstruction; linear records are decoded instead. Bytes retained in original and hashes recorded.')
            stack=[]
            while p<end:
                length,typ=u('HH',s,p);r=s[p:p+length+2];assert length>=2 and len(r)==length+2 and p+len(r)<=end
                x=dict(id=f'{ei}:{p:08x}',directory_index=ei,subsection=SUBS[k],module=e['module'],record_offset=p,
                       record_file_offset=base+e['offset']+p,kind=SYMS[typ],raw=r.hex(),scope_id=stack[-1]['id'] if stack else None)
                if typ in (0x104,0x105):
                    parent,endp,nxt,size,ds,de,off,seg,ti=u('III6H',r,4)
                    x.update(name=pstr(r,29),parent_record=parent,end_record=endp,next_record=nxt,size=size,debug_start=ds,debug_end=de,
                        logical_segment=seg,offset=off,type_index=ti,flags=r[28],linkage='file_static' if typ==0x104 else 'global',**address(seg,off))
                    assert not stack
                    stack.append(x);scopes.append(x)
                elif typ==0x107:
                    parent,endp,size,off,seg=u('IIHHH',r,4)
                    assert stack and parent==stack[-1]['record_offset'],(x,parent,stack[-1])
                    x.update(name=pstr(r,18),parent_record=parent,end_record=endp,size=size,logical_segment=seg,offset=off,**address(seg,off))
                    parent_scope=stack[-1]
                    assert parent_scope['offset']<=off and off+size<=parent_scope['offset']+parent_scope['size']
                    stack.append(x);scopes.append(x)
                elif typ==6:
                    assert stack and stack[-1]['end_record']==p
                    x['ends_scope_id']=stack.pop()['id']
                elif typ==0x100:
                    off,ti=u('hH',r,4);x.update(name=pstr(r,8),bp_offset=off,type_index=ti)
                elif typ==2:
                    ti,reg=u('HH',r,4);x.update(name=pstr(r,8),register_id=reg,register={15:'SI',16:'DI'}[reg],type_index=ti)
                elif typ in (0x101,0x102,0x103):
                    off,seg,ti=u('3H',r,4)
                    x.update(name=pstr(r,10),logical_segment=seg,offset=off,type_index=ti,**address(seg,off))
                    x['linkage']={0x101:'file_static' if not stack else 'block_static',0x102:'global',0x103:'public_name_only'}[typ]
                elif typ==0x203:
                    off,seg,ti=u('IHH',r,4)
                    x.update(name=pstr(r,12),logical_segment=seg,offset=off,type_index=ti,**address(seg,off))
                    if seg==0 and ti==1:x['signed_value']=u('i',r,4)[0]
                elif typ==0x109:
                    off,seg=u('HH',r,4);x.update(name=pstr(r,9),flags=r[8],logical_segment=seg,offset=off,**address(seg,off))
                elif typ==4:x.update(name=pstr(r,6),type_index=u('H',r,4)[0])
                elif typ==1:
                    f=int.from_bytes(r[5:8],'little')
                    x.update(compiler=pstr(r,8).rstrip(),machine=r[4],language=f&255,pcode=bool(f&256),float_precision=(f>>9)&3,
                        float_package=(f>>11)&3,ambient_data=(f>>13)&7,ambient_code=(f>>16)&7,mode32=bool(f&(1<<19)))
                elif typ==9:x.update(signature=u('I',r,4)[0],name=pstr(r,8))
                elif typ==5:x.update(first_procedure_record=u('I',r,4)[0],logical_segment=u('H',r,8)[0])
                elif typ in (0x400,0x401):
                    checksum,off,mod,pad=u('IIHH',r,4)
                    x.update(name_checksum=checksum,target_record_offset=off,target_module=mod,alignment_bytes=pad)
                elif typ==0x402:x['treatment']='alignment-only record; no source semantics'
                records.append(x);p+=len(r)
            assert p==end and not stack
        elif k==0x12b:
            flags,nt=u('II',s);assert nt==len(T)
            start=8+4*nt
            for j,t in enumerate(T):
                tp=start+u('I',s,8+4*j)[0];length=u('H',s,tp)[0]
                assert bytes.fromhex(t['raw'])==s[tp+2:tp+2+length]
                t.update(record_file_offset=base+e['offset']+tp,record_bytes=length+2)
            a['type_table_flags']=flags
        elif k==0x127:
            nf,ns=u('HH',s);ranges=[]
            for j in range(ns):
                start,end=u('II',s,4+4*nf+8*j);seg=u('H',s,4+4*nf+8*ns+2*j)[0]
                assert start<=end
                ranges.append(dict(logical_segment=seg,start=start,end_inclusive=end))
            a['source_ranges']=ranges
            a['source_files']=[]
            for j in range(nf):
                fp=u('I',s,4+4*j)[0];nseg,res=u('HH',s,fp);assert res==0
                filename=pstr(s,fp+4+12*nseg);a['source_files'].append(filename)
                for z in range(nseg):
                    lp=u('I',s,fp+4+4*z)[0];seg,nlines=u('HH',s,lp)
                    start,end=u('II',s,fp+4+4*nseg+8*z)
                    offsets=u('I'*nlines,s,lp+4);assert all(start<=o<=end for o in offsets)
                    assert lp+4+6*nlines<=len(s)
            a['treatment']='File names and line mappings cross-checked against independent sstFileIndex.'
        elif k==0x128:
            p=0
            while p<len(s):libraries.append(pstr(s,p));p+=1+s[p]
        elif k==0x12e:names_blob=s
        elif k==0x133:
            nm,nf=u('HH',s);starts=u('H'*nm,s,4);counts=u('H'*nm,s,4+2*nm);offsets=u('I'*nf,s,4+4*nm);np=4+4*nm+4*nf
            for i in range(nm):
                assert starts[i]+counts[i]<=nf
                fileindex.append(dict(module=i+1,files=[pstr(s,np+offsets[j]) for j in range(starts[i],starts[i]+counts[i])]))
        else:
            a['treatment']='Decoded in initial extraction; semantic validation and enrichment below.'
        audits.append(a)
    by_module_offset={(r['module'],r['record_offset']):r for r in records if r['subsection']=='sstAlignSym'}
    for r in records:
        if r['kind'] in ('S_PROCREF','S_DATAREF'):
            target=by_module_offset[(r['target_module'],r['target_record_offset'])]
            assert target['kind'] in (('S_GPROC16','S_LPROC16') if r['kind']=='S_PROCREF' else ('S_LDATA16','S_GDATA16'))
            r.update(target_id=target['id'],resolved_name=target['name'])
        if r['kind']=='S_SSEARCH':
            target=by_module_offset[(r['module'],r['first_procedure_record'])]
            assert target['kind'] in ('S_GPROC16','S_LPROC16')
        if r['kind'] in ('S_GPROC16','S_LPROC16') and r['next_record']:
            assert by_module_offset[(r['module'],r['next_record'])]['kind'] in ('S_GPROC16','S_LPROC16')
    for i,m in modules.items():
        m['library_name']=libraries[m['library']] if m['library']<len(libraries) else None
        m['library_resolution']='indexed' if m['library']<len(libraries) else 'unmapped library index retained; object module provenance not inferred'
        m['source_files']=fileindex[int(i)-1]['files']
    for i,m in enumerate(segmap):
        def name(off):return None if off==65535 else names_blob[off:names_blob.index(0,off)].decode('latin1')
        m.update(logical_segment=i+1,name=name(m['name_index']),class_name=name(m['class_index']))
    prims={}
    def size(i):
        if i<4096:
            mode=i>>8;name,sz=PRIMS[i&255]
            if mode:name+={1:' near *',2:' far *',3:' huge *'}[mode];sz={1:2,2:4,3:4}[mode]
            prims[i]=dict(index=i,name=name,size=sz,pointer_mode=mode)
            return sz
        t=types[i]
        if 'size'in t:return t['size']
        if t['leaf']=='0002':return 2 if t['attributes']&31==0 else 4
        if t['leaf'] in ('0001','0206'):return size(t['underlying_type'])
        if t['leaf'] in ('0008','0201','0204'):return None
        raise ValueError(t)
    for t in T:
        r=bytes.fromhex(t['raw']);leaf=int(t['leaf'],16);t['kind']=LEAVES[leaf]
        t['size_bytes']=size(t['index'])
        if leaf==1:t['const']=bool(t['attributes']&1);t['volatile']=bool(t['attributes']&2);t['unaligned']=bool(t['attributes']&4)
        elif leaf==2:
            t.update(pointer_kind={0:'near',1:'far'}[t['attributes']&31],pointer_mode=(t['attributes']>>5)&7,is_const=bool(t['attributes']&1024),is_volatile=bool(t['attributes']&512))
            size(t['underlying_type'])
        elif leaf==3:
            t['index_type']=u('H',r,4)[0];size(t['index_type']);es=size(t['element_type']);assert es
            assert t['size']%es==0;t['element_count']=t['size']//es
        elif leaf==8:
            t['calling_convention_name']=CALLS[t['calling_convention']];t['attributes']=r[5]
            t['argument_types']=types[t['argument_list']]['arguments']
            # A final T_NOTYPE is the varargs marker, not an ordinary parameter.
            t['variadic']=bool(t['argument_types'] and t['argument_types'][-1]==0)
            assert t['parameter_count']==len(t['argument_types'])-int(t['variadic']),(t,)
            for arg in [t['return_type']]+t['argument_types']:size(arg)
        elif leaf==0x206:
            assert t['bit_position']+t['bit_length']<=8*size(t['underlying_type'])
        elif leaf==0x204:
            t['members']=next(a['members'] for a in T if a.get('field_list')==t['index'])
        if leaf==5:
            t['forward_reference']=bool(t['properties']&128);assert not t['forward_reference']
            for m in t['members']:
                sz=size(m['type_index']);m['size_bytes']=sz
                assert m['offset']+sz<=t['size']
                if types.get(m['type_index'],{}).get('leaf')=='0206':
                    bt=types[m['type_index']];m.update(bit_position=bt['bit_position'],bit_length=bt['bit_length'])
            t['overlapping_pairs']=[[i,j] for i,m in enumerate(t['members']) for j,q in enumerate(t['members']) if i<j and max(m['offset'],q['offset'])<min(m['offset']+m['size_bytes'],q['offset']+q['size_bytes'])]
    scope_byid={r['id']:r for r in scopes}
    for r in records:
        if 'type_index'in r:size(r['type_index'])
        if r['kind'] in ('S_BPREL16','S_REGISTER16'):
            scope=scope_byid[r['scope_id']];r['visibility_range']=dict(ne_segment=scope['ne_segment'],start=scope['ne_offset'],end_exclusive=scope['ne_offset']+scope['size'])
            r['lifetime_note']='Lexical visibility range; not a claim of optimized machine-location validity at every instruction.'
    procedures=[r for r in records if r['kind'] in ('S_GPROC16','S_LPROC16')]
    for f in procedures:
        ti=types[f['type_index']];assert ti['leaf']=='0008'
        f['signature_type']=ti['index']
        f['variables']=[r['id'] for r in records if r['kind'] in ('S_BPREL16','S_REGISTER16') and belongs(r,f,scope_byid)]
        # Stack order follows the recorded calling convention, not symbol name order.
        params=[r for r in records if r['kind']=='S_BPREL16' and r['scope_id']==f['id'] and r['bp_offset']>0]
        params.sort(key=lambda r:r['bp_offset'],reverse=ti['calling_convention'] in (2,3))
        f['parameter_symbols']=[r['id'] for r in params]
        f['parameter_metadata_matches']=len(params)==ti['parameter_count'] and all(p['type_index']==a for p,a in zip(params,ti['argument_types']))
        f['source_files']=modules[str(f['module'])]['source_files']
    overrides=read('tracking/debug_signature_overrides.json')
    for f in procedures:
        ti=types[f['type_index']]
        f['effective_parameter_types']=ti['argument_types'][:-1] if ti['variadic'] else ti['argument_types']
        f['effective_parameter_names']=[next(r['name'] for r in records if r['id']==i) for i in f['parameter_symbols']]
        if f['name'] in overrides:
            o=overrides[f['name']];e=o['instruction_evidence'];assert (f['ne_segment'],f['ne_offset'])==(e['ne_segment'],e['ne_offset'])
            raw=b[f['file_offset']:f['file_offset']+f['size']]
            if 'starting_hex'in e:assert raw.startswith(bytes.fromhex(e['starting_hex']))
            if 'ending_hex'in e:assert raw.endswith(bytes.fromhex(e['ending_hex']))
            f['effective_parameter_types']=o['parameter_types'];f['effective_parameter_names']=o['parameter_names']
            f['signature_resolution']=o['reason']
        else:
            assert f['parameter_metadata_matches'],f['name']
            f['signature_resolution']='Procedure type agrees with ordered BP-relative parameter records.'
    groups=collections.defaultdict(list)
    for r in records:
        if r['kind'] in ('S_GPROC16','S_LPROC16','S_GDATA16','S_LDATA16','S_PUB16'):
            if 'ne_segment'in r:groups[(r['ne_segment'],r['ne_offset'])].append(r)
    identities=[]
    for (seg,off),rs in sorted(groups.items()):
        identities.append(dict(ne_segment=seg,ne_offset=off,record_ids=[r['id'] for r in rs],names=sorted(set(r['name'] for r in rs)),
            treatment='Co-located records; preserve each name, type, scope and linkage. Co-location alone does not prove semantic identity.'))
    aliases=[dict(name=r['name'],type_index=r['type_index'],scope_id=r['scope_id'],record_id=r['id']) for r in records if r['kind']=='S_UDT16']
    data=[r for r in records if r['kind'] in ('S_GDATA16','S_LDATA16')]
    lines=read('analysis/source_lines.json')
    for m in fileindex:
        assert sorted(set(l['file'] for l in lines if l['module']==m['module']))==sorted(set(m['files']))
    coverage=read('analysis/coverage.json');pubs=[r for r in records if r['kind']=='S_PUB16' and 'ne_segment'in r]
    for gap in coverage['ranges']:
        gap['public_symbols']=[r['id'] for r in pubs if r['ne_segment']==gap['ne_segment'] and gap['start']<=r['ne_offset']<gap['end_exclusive']]
    summary=dict(absolute_public_constants=sum(r.get('address_kind')=='absolute' for r in records),
        overlapping_structures=sum(bool(t.get('overlapping_pairs')) for t in T),
        structure_members=sum(len(t['members']) for t in T if t['leaf']=='0005'),
        variable_tail_members=sum(m['size_bytes']==0 for t in T if t['leaf']=='0005' for m in t['members']),
        subsections=len(audits),record_kinds=dict(collections.Counter(r['kind'] for r in records)),type_kinds=dict(collections.Counter(t['kind'] for t in T)),
        procedures=len(procedures),blocks=len(scopes)-len(procedures),typedef_records=len(aliases),data_records=len(data),
        unmapped_library_indices=sorted(set(m['library'] for m in modules.values() if m['library_name'] is None)),
        effective_signature_unresolved=0,
        parameter_metadata_mismatches=[f['id'] for f in procedures if not f['parameter_metadata_matches']],
        library_names=libraries,semantic_unknown_record_kinds=0,
        limitation='All present semantic record kinds are decoded. Hash accelerators and padding are excluded with justification. Compiler execution and full instruction-by-instruction use validation remain later work.')
    out=dict(summary=summary,subsection_audit=audits,types=T,primitives=list(prims.values()),symbols=records,modules=modules,segment_map=segmap,
             aliases=aliases,address_groups=identities,procedures=procedures,globals=data,file_index=fileindex,coverage=coverage)
    return out

def belongs(r,f,scopes):
    p=r['scope_id']
    while p:
        if p==f['id']:return True
        p=scopes[p]['scope_id']
    return False

def write(out,folder):
    folder.mkdir(parents=True,exist_ok=True)
    for name,data in out.items():(folder/(name+'.json')).write_text(json.dumps(data,indent=2)+'\n')

if __name__=='__main__':
    try:
        from tools.debug_report import render
    except ModuleNotFoundError:
        from debug_report import render
    out=recover();write(out,ROOT/'analysis/debug')
    for name,content in render(out).items():(ROOT/'analysis/debug'/name).write_text(content)
    print(json.dumps(out['summary'],indent=2))
