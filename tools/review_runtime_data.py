"""Audit original CRT data, including the seven CodeView records without code.

Use independent original public symbols and the prior verified code mappings.
Private CDATA placement is constrained by literals, import/internal pointers,
and an explicitly unresolved Windows stub slot. Common message-padding markers
remain ambiguous. This does not infer a complete linker ordering.
"""
import argparse
import json
from pathlib import Path
from tools.omf import library
from tools.ne_layout import NE
from tools.review_runtime_library import ROOT,sha


def review(directory):
    prior=json.loads((ROOT/'analysis/reconstruction/runtime-library.json').read_text())
    blob=(directory/'MLIBCEW.LIB').read_bytes();assert sha(blob)==prior['library']['sha256']
    ne=NE(ROOT/'target/stars.exe');assert sha(ne.b)==prior['original_sha256']
    members=library(blob);by={m.offset:m for m in members}
    publics={};absolute={}
    for p in json.loads((ROOT/'analysis/debug/symbols.json').read_text()):
        if not p['kind'].startswith('S_PUB'):continue
        if p.get('ne_segment'):publics[p['name']]=(p['ne_segment'],p['ne_offset'])
        elif p.get('address_kind')=='absolute':absolute[p['name']]=p['offset']&65535
    selected=[]
    for r in prior['modules']:
        if 'member_offset' in r:
            m=by[r['member_offset']]
            anchors={d['object_segment']:(d['ne_segment'],d['base']) for d in r['data_anchors']}
            for i,s in enumerate(m.segments):
                if s and s.class_name=='CODE' and s.size==r['code_bytes']:
                    anchors[i]=(r['ne_segment'],r['ne_offset'])
            selected.append((r,m,anchors))
            for p in m.publics:
                if p['segment'] in anchors:
                    ns,base=anchors[p['segment']];value=(ns,base+p['offset'])
                    assert p['name'] not in publics or publics[p['name']]==value
                    publics[p['name']]=value
        else:
            candidates=[by[o] for o in r['matching_member_offsets']]
            scores=[sum(p['name'] in publics or p['name'] in absolute for p in m.publics) for m in candidates]
            best=max(scores)
            choices=[m for m,s in zip(candidates,scores) if s==best]
            if len(choices)!=1:raise ValueError(('Ambiguous data member',r['name']))
            m=choices[0];anchors={}
            for p in m.publics:
                if p['segment'] and p['name'] in publics:
                    ns,off=publics[p['name']];base=off-p['offset']
                    assert p['segment'] not in anchors or anchors[p['segment']]==(ns,base)
                    anchors[p['segment']]=(ns,base)
            selected.append((r,m,anchors))
    imports={n:t for m in members for n,t in m.imports.items()}
    aliases={a:t for m in members for a,t in m.aliases.items()}
    reloc={(r['segment'],site):r for r in ne.relocations for site in r['sites']}
    def check_fixup(m,s,f,ns,base,anchors):
        kind,index=f['target'];address=None;absval=None;name=None
        if kind==0:address=anchors.get(index)
        elif kind==2:
            name=m.externals[index];seen=set()
            while name not in publics and name not in absolute and name not in seen:
                seen.add(name);name=aliases.get(name,m.weak.get(name,name))
            address=publics.get(name);absval=absolute.get(name)
        check=dict(offset=f['offset'],width=f['width'],location=f['location'],target=name or f['target'])
        if address is None and absval is None and name not in imports:return dict(check,status='unresolved_target')
        site=base+f['offset'];raw=ne.raw[ns-1];width=f['width'];loc=f['location']
        value=(address[1] if address else (absval or 0))+f['displacement']+int.from_bytes(s.data[f['offset']:f['offset']+(2 if loc==3 else width)],'little')
        rel=reloc.get((ns,site))
        if loc==3 and rel is None:rel=reloc.get((ns,site+2))
        if rel and rel['target_kind']=='import':
            return dict(check,status='verified_import' if imports.get(name)==rel['import_key'] else 'import_mismatch')
        if name=='__fptaskdata' and absval==0x9876 and loc==3 and raw[site:site+4]==bytes(4):
            return dict(check,status='observed_windows_stub_slot',note='WIN/CRT0.ASM declares this absolute symbol solely to exclude the non-Windows emulator. This linked slot is zero; the historical linker transformation is not yet independently reproduced.')
        if rel and rel['target_kind']=='internal':
            actual=int.from_bytes(raw[site:site+2],'little') if rel['address_kind']==2 else rel['target_offset']
            if rel['additive'] and rel['address_kind']!=2:actual+=int.from_bytes(raw[site:site+2],'little')
            good=address is not None and rel['target_segment']==address[0] and actual&65535==value&65535
            return dict(check,status='verified_pointer_relocation' if good else 'pointer_mismatch')
        if loc==1:
            if f['relative']:value-=site+width
            actual=int.from_bytes(raw[site:site+2],'little')
            return dict(check,status='verified_linked_word' if actual==value&65535 else 'word_mismatch',expected=value&65535,actual=actual)
        return dict(check,status='unresolved_encoding')
    def literals(s,ns,base):
        raw=ne.raw[ns-1];mask={i for f in s.fixups for i in range(f['offset'],f['offset']+f['width'])}
        indexes=[i for i in range(s.size) if s.initialized[i] and i not in mask]
        bad=[i for i in indexes if base+i>=len(raw) or raw[base+i]!=s.data[i]]
        return indexes,bad
    rows=[]
    for r,m,anchors in selected:
        data=[]
        for i,s in enumerate(m.segments):
            if not s or s.class_name=='CODE' or not s.size:continue
            method='public_symbol_or_verified_code_reference'
            if i not in anchors:
                candidates=[]
                mask={j for f in s.fixups for j in range(f['offset'],f['offset']+f['width'])}
                indexes=[j for j in range(s.size) if s.initialized[j] and j not in mask]
                # Pointer-bearing records need only candidate NE relocation sites,
                # not a byte-by-byte scan through every executable segment.
                if s.fixups:
                    first=s.fixups[0]
                    locations={(ns,site-first['offset']) for ns,site in reloc}
                    if first['location']==3:locations|={(ns,site-first['offset']-2) for ns,site in reloc}
                elif indexes:
                    runs=[];start=last=indexes[0]
                    for j in indexes[1:]:
                        if j!=last+1:runs.append((start,last+1));start=j
                        last=j
                    runs.append((start,last+1));start,end=max(runs,key=lambda x:x[1]-x[0])
                    needle=bytes(s.data[start:end]);locations=set()
                    for ns,raw in enumerate(ne.raw,1):
                        at=raw.find(needle)
                        while at!=-1:
                            locations.add((ns,at-start));at=raw.find(needle,at+1)
                else:locations=set()
                for ns,base in sorted(locations):
                    raw=ne.raw[ns-1]
                    if ne.segments[ns-1]['kind']!='data' or base<0 or base+s.size>len(raw):continue
                    if any(raw[base+j]!=s.data[j] for j in indexes):continue
                    checks=[check_fixup(m,s,f,ns,base,anchors) for f in s.fixups]
                    if all(x['status'].startswith('verified_') or x['status']=='observed_windows_stub_slot' for x in checks):candidates.append((ns,base))
                if len(candidates)!=1:
                    data.append(dict(object_segment=i,name=s.name,size=s.size,status='unresolved_placement',candidate_count=len(candidates),candidate_examples=candidates[:8]));continue
                anchors[i]=candidates[0];method='unique_initialized_bytes_and_resolved_pointers'
            ns,base=anchors[i];indexes,bad=literals(s,ns,base)
            checks=[check_fixup(m,s,f,ns,base,anchors) for f in s.fixups]
            data.append(dict(object_segment=i,name=s.name,size=s.size,ne_segment=ns,base=base,
                placement_evidence=method,initialized_literals=len(indexes),zero_fill_bytes=s.size-sum(s.initialized),
                literal_mismatches=bad,fixups=checks,status='verified_data' if not bad and all(x['status'].startswith('verified_') for x in checks) else 'unresolved_data'))
        absolutes=[dict(name=p['name'],value=p['offset']&65535,verified=absolute.get(p['name'])==p['offset']&65535)
                   for p in m.publics if not p['segment'] and p['name'] in absolute]
        empty=not any(s and s.size for s in m.segments)
        row=dict(module=r['module'],name=r['name'],member_offset=m.offset,member_sha256=sha(blob[m.offset:m.end]),
            original_code_bytes=r['code_bytes'],data=data,absolute_symbols=absolutes,
            status='verified_data_mapping' if all(x['status']=='verified_data' for x in data) and all(x['verified'] for x in absolutes) else 'unresolved')
        if empty:row['note']='This member contributes no loadable segment bytes; its record/absolute-symbol role is mapped.'
        rows.append(row)
    return dict(schema=1,original_sha256=prior['original_sha256'],library_sha256=sha(blob),
        scope='CRT data member identity, initialized literals, pointer/near fixups and zero-fill extents. No original linker ordering or byte-identical whole build is claimed.',
        summary=dict(modules=len(rows),records_without_code=sum(r['original_code_bytes']==0 for r in rows),
          data_segments=sum(len(r['data']) for r in rows),initialized_literal_bytes=sum(d.get('initialized_literals',0) for r in rows for d in r['data']),
          data_fixups=sum(len(d.get('fixups',[])) for r in rows for d in r['data']),unresolved=sum(r['status']=='unresolved' for r in rows)),modules=rows)

def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--check',action='store_true');a=p.parse_args()
    result=review(ROOT/'runs/step4/guest/MSVC/LIB');out=ROOT/'analysis/reconstruction/runtime-data.json';text=json.dumps(result,indent=2)+'\n'
    if a.check:
        if out.read_text()!=text:raise SystemExit('Runtime data audit changed')
    else:out.write_text(text)
    print(json.dumps(result['summary']))
if __name__=='__main__':main()
