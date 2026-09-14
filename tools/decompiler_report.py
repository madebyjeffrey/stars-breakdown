#!/usr/bin/env python3
"""Validate the Ghidra export against independent NE/debug recovery and index it."""
import bisect
from collections import Counter, defaultdict
import hashlib
import json
import shutil
import struct
from pathlib import Path
from ne_layout import NE, patch

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / 'analysis/decompiler'
EXPORT = OUT / 'export'

def read(path):
    return json.loads((ROOT / path).read_text())

def save(name, value):
    (OUT / name).write_text(json.dumps(value, indent=2) + '\n')

def address(seg, off):
    return f'{0x1000 + (seg - 1) * 8:04x}:{off:04x}'

def split(address_string):
    sel, off = (int(x, 16) for x in address_string.split(':'))
    return (sel - 0x1000) // 8 + 1, off

def main():
    layout = read('analysis/ne/layout.json')
    procedures = read('analysis/debug/procedures.json')
    functions = read('analysis/decompiler/export/functions.json')
    units = read('analysis/decompiler/export/units.json')
    symbols = read('analysis/decompiler/export/symbols.json')
    references = read('analysis/decompiler/export/references.json')
    abi = read('references/win16_abi.json')
    assert read('analysis/decompiler/import-issues.json') == []
    assert read('analysis/decompiler/export/invalid-flows.json') == []
    assert all(f['decompiled'] for f in functions)
    imported = {f['address']: f for f in functions}
    for p in procedures:
        f = imported[address(p['ne_segment'], p['ne_offset'])]
        assert f['name'] == p['name'], (f['name'], p['name'])
        assert [a['name'] for a in f['parameters']] == p['effective_parameter_names']
    # Independent parser, raw bytes and relocation chains; translate address conventions only.
    assert hashlib.sha256((ROOT/'target/stars.exe').read_bytes()).hexdigest() == layout['input_sha256']
    ne = NE(ROOT / 'target/stars.exe')
    expected = [bytearray(x) for x in ne.raw]
    imports = {r['module'] + '!#' + str(r['ordinal']): tuple(int(v,16) for v in r['address'].split(':')) for r in abi}
    equates = {'KERNEL!#113': 3, 'KERNEL!#114': 8}
    sites = equate_sites = 0
    for r in layout['relocations']:
        if r['target_kind'] == 'os_fpu_fixup':
            continue
        if r['target_kind'] == 'internal':
            selector, offset = 0x1000 + 8 * (r['target_segment'] - 1), r['target_offset']
        else:
            selector, offset = imports[r['import_key']]
            if r['import_key'] in equates:
                offset = equates[r['import_key']]
                equate_sites += len(r['sites'])
        for site in r['sites']:
            patch(expected[r['segment']-1], site, r['address_kind'], selector, offset, r['additive'])
            sites += 1
    for n, segment in enumerate(expected, 1):
        actual = (EXPORT / 'memory' / (('Code' if n < 37 else 'Data') + str(n) + '.bin')).read_bytes()
        assert actual == segment, f'Ghidra memory mismatch in NE segment {n}'
    loader_check = dict(segments=37, bytes=sum(map(len,expected)), relocation_sites=sites,
                        absolute_export_sites=equate_sites, unmatched_bytes=0,
                        hardware_x87_fixups=len([r for r in layout['relocations'] if r['target_kind']=='os_fpu_fixup']),
                        exported_prologue='Ghidra preserves original prologue; step-1 optional runtime prologue rewrite is not applied.')
    dispatch=read('analysis/decompiler/runtime-dispatch.json')
    for row in dispatch['rows']:
        p=row['data_offset'];n=4**expected[36][p+13]
        assert row['targets']==list(struct.unpack_from('<'+'H'*n,expected[36],p+16))
        assert row['error_selector_bytes']==list(expected[36][p+6:p+13])
        assert set(row['error_selector_bytes']) <= {0,2,4,6}
    assert dispatch['error_dispatch']['targets']==[address(36,t) for t in struct.unpack_from('<4H',expected[36],0x1a8c)]
    save('loader-check.json', loader_check)
    save('functions.json', functions)
    settings=EXPORT/'analysis-settings.json'
    if settings.exists():save('analysis-settings.json',json.loads(settings.read_text()))
    # Merge adjacent classifications without hiding procedure boundaries or shared code.
    proc_by_segment = defaultdict(list)
    for p in procedures:
        proc_by_segment[p['ne_segment']].append(p)
    for ps in proc_by_segment.values():
        ps.sort(key=lambda p:p['ne_offset'])
    starts = {s:[p['ne_offset'] for p in ps] for s,ps in proc_by_segment.items()}
    events = defaultdict(lambda:defaultdict(list))
    for f in functions:
        for r in f['ranges']:
            seg,start = split(r['start']); endseg,end = split(r['end']); assert seg == endseg
            events[seg][start].append((f['address'],1)); events[seg][end+1].append((f['address'],-1))
    event_positions = {s:sorted(es) for s,es in events.items()}
    cursors=Counter(); active=defaultdict(set); coverage=[]; byte_counts=Counter()
    end_offsets=Counter()
    for u in units:
        seg,start,size = u['segment'],u['start'],u['size']
        assert start == end_offsets[seg], (seg,start,end_offsets[seg])
        end_offsets[seg] += size
        positions=event_positions.get(seg,[])
        while cursors[seg] < len(positions) and positions[cursors[seg]]<=start:
            for key, delta in events[seg][positions[cursors[seg]]]:
                if delta>0:active[seg].add(key)
                else:active[seg].discard(key)
            cursors[seg]+=1
        idx=bisect.bisect_right(starts.get(seg,[]),start)-1
        p=proc_by_segment[seg][idx] if idx>=0 else None
        if p and not p['ne_offset']<=start<p['ne_offset']+p['size']:p=None
        key=(seg,u['kind'],p['id'] if p else None,tuple(sorted(active[seg])))
        row=dict(ne_segment=seg,start=start,end_exclusive=start+size,classification=u['kind'],debug_procedure=key[2],functions=list(key[3]))
        if coverage and coverage[-1]['key']==key and coverage[-1]['row']['end_exclusive']==start:
            coverage[-1]['row']['end_exclusive']=start+size
        else:coverage.append(dict(key=key,row=row))
        byte_counts[('code_' if seg<37 else 'data_')+u['kind']]+=size
    for s in layout['segments']:assert end_offsets[s['segment']]==s['static_memory_bytes']
    save('coverage.json', dict(scope='All static NE segment bytes. Automatic classification, not reviewed reconstruction.',
         summary=dict(byte_counts),ranges=[x['row'] for x in coverage]))
    names=defaultdict(list)
    for s in symbols:names[s['address']].append(s['name'])
    graph=defaultdict(list); data=defaultdict(list)
    for r in references:
        if 'CALL' in r['type']:
            graph[r['function'],r['to']].append(r['from'])
        elif ':' in r['to'] and any(k in r['type'] for k in ('READ','WRITE','DATA','PARAM')):
            data[r['to']].append(dict(function=r['function'],site=r['from'],kind=r['type']))
    save('callgraph.json', [dict(caller=caller,caller_name=imported[caller]['name'],target=target,
         target_names=names[target],sites=sorted(set(sites))) for (caller,target),sites in sorted(graph.items())])
    save('data-references.json', [dict(address=a,names=names[a],references=rs) for a,rs in sorted(data.items())])
    indirect={}
    for r in read('analysis/decompiler/export/indirect.json'):
        q=indirect.setdefault(r['address'],dict(address=r['address'],instruction=r['instruction'],kind=r['kind'],owners=[],targets=set()))
        q['owners'].append(r['function']);q['targets'].update(r['resolved_targets'])
    for r in indirect.values():
        r['targets']=sorted(r['targets']);r['owners']=sorted(set(r['owners']))
        r['status']='static target set recovered' if r['targets'] else 'unresolved'
        if r['instruction'].startswith(('INT ', 'INTO')):
            r['status']='OS interrupt; service behavior requires runtime review'
        r['resolution_task']='Review table/index bounds or trace the pointer producer and validate in step 4/5.'
    save('indirect-targets.json',list(indirect.values()))
    save('switches.json',read('analysis/decompiler/export/switches.json'))
    # Address-taken relocation targets are candidates, not assertions of a particular callback API.
    by_seg=defaultdict(list)
    for u in units:by_seg[u['segment']].append(u)
    unit_starts={s:[u['start'] for u in us] for s,us in by_seg.items()}
    callbacks=[]
    for r in layout['relocations']:
        if r['target_kind']!='internal':continue
        target=address(r['target_segment'],r['target_offset'])
        if target not in imported:continue
        for site in r['sites']:
            u=by_seg[r['segment']][bisect.bisect_right(unit_starts[r['segment']],site)-1]
            if u['kind']=='instruction' and u.get('text','').startswith(('CALL','JMP')):continue
            callbacks.append(dict(site=address(r['segment'],site),target=target,name=imported[target]['name'],
                                  relocation_record_file_offset=r['record_file_offset'],source_classification=u['kind'],
                                  evidence=u.get('text'),status='Address-taken candidate; consumer requires review'))
    save('code-pointers.json',callbacks)
    public_records=[s for s in read('analysis/debug/symbols.json') if s['kind']=='S_PUB16' and s.get('ne_segment',37)<=36]
    public_addresses={address(s['ne_segment'],s['ne_offset']) for s in public_records}
    named_data={address(u['segment'],u['start']) for u in units if u['kind']=='data'}
    unresolved_publics=sorted(public_addresses - set(imported) - named_data)
    save('public-candidates.json',[dict(address=a,names=names[a],classification='function' if a in imported else 'defined data' if a in named_data else 'label within code; separate entry not yet established') for a in sorted(public_addresses)])
    summary=dict(input_sha256=hashlib.sha256((ROOT/'target/stars.exe').read_bytes()).hexdigest(),
        ghidra='12.1.3 (2026-08-17)',language='x86:LE:16:Protected Mode',functions=len(functions),
        debug_procedures=len(procedures),additional_functions=len(functions)-len(procedures),
        decompiled=sum(f['decompiled'] for f in functions),invalid_direct_flows=0,loader=loader_check,
        classified_bytes=dict(byte_counts),public_entry_addresses=len(public_addresses),public_labels_requiring_entry_review=unresolved_publics,
        calls=len(graph),data_reference_addresses=len(data),address_taken_candidates=len(callbacks),
        indirect_instructions=len(indirect),unresolved_indirect_instructions=sum(r['status']=='unresolved' for r in indirect.values()),
        os_interrupt_instructions=sum(r['instruction'].startswith(('INT ','INTO')) for r in indirect.values()),
        switch_tables=len(read('analysis/decompiler/export/switches.json')),
        reconstructed_source_status='Unchanged: these are raw tool outputs, not reviewed/compiled reconstruction.')
    dos=read('analysis/decompiler/export/dos/functions.json')
    assert len(dos)==2 and all(f['decompiled'] for f in dos)
    assert sum(f['body_bytes'] for f in dos)==15
    summary['dos_stub_functions']=2
    summary['dos_stub_instruction_bytes']=15
    save('dos-functions.json',dos)
    dos_acceptance=OUT/'acceptance/dos';dos_acceptance.mkdir(parents=True,exist_ok=True)
    for path in (EXPORT/'dos/raw').iterdir():shutil.copy2(path,dos_acceptance/path.name)
    save('summary.json',summary)
    # Every recorded function must have both products, including thunks in real code segments.
    for f in functions:
        key=f['address'].replace(':','_')
        assert (EXPORT/'raw'/f'{key}.asm').exists()
        assert list((EXPORT/'raw').glob(key+'_*.c'))
    cases={'GetRaceStat','GetRaceGrbit','GetFuelFree','ICompLong','AskSaveDialog','FrameWndProc','LpengineFromId','SetFileSeeds','ReadIniSettings','FLookupPart','__aFlmul','__aFldiv','__aFlshl','__ctrandisp1','__trandisp2'}
    acceptance=OUT/'acceptance';acceptance.mkdir(exist_ok=True)
    for f in functions:
        if f['name'] in cases:
            key=f['address'].replace(':','_')
            for path in (EXPORT/'raw').glob(key+'*'):
                shutil.copy2(path,acceptance/path.name)
    manifest=[]
    for path in sorted(EXPORT.rglob('*')):
        if path.is_file() and path.suffix in ('.c','.asm','.bin'):
            b=path.read_bytes();manifest.append(dict(path=str(path.relative_to(EXPORT)),size=len(b),sha256=hashlib.sha256(b).hexdigest()))
    save('export-manifest.json',manifest)
    print(json.dumps(summary,indent=2))

if __name__=='__main__':main()
