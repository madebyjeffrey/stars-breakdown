#!/usr/bin/env python3
"""Compare compiler-generated tutorial code with the original, without masking bytes."""
import argparse
import json
import re
from pathlib import Path
import sys
ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT))
from tools.compare_byte_match import object_module, digest, ORIGINAL_SHA
from tools.ne_layout import NE, patch


def original_image():
    binary = ROOT/'target/stars.exe'
    if digest(binary.read_bytes()) != ORIGINAL_SHA:
        raise ValueError('Original executable hash mismatch')
    ne = NE(binary)
    image = [bytearray(x) for x in ne.raw]
    abi = json.loads((ROOT/'references/win16_abi.json').read_text())
    imports = {r['module']+'!#'+str(r['ordinal']): tuple(int(v,16) for v in r['address'].split(':')) for r in abi}
    for r in ne.relocations:
        if r['target_kind'] == 'os_fpu_fixup':
            continue
        if r['target_kind'] == 'internal':
            sel, off = 0x1000+8*(r['target_segment']-1), r['target_offset']
        else:
            sel, off = imports[r['import_key']]
            off = {'KERNEL!#113':3, 'KERNEL!#114':8}.get(r['import_key'],off)
        for site in r['sites']:
            patch(image[r['segment']-1],site,r['address_kind'],sel,off,r['additive'])
    return image


def symbols():
    values = {}
    def add(name, value):
        values.setdefault(name,set()).add(value)
    for r in json.loads((ROOT/'analysis/decompiler/functions.json').read_text()):
        sel,off = (int(x,16) for x in r['address'].split(':'))
        add(r['name'],(sel<<16)|off)
        add('_'+r['name'],(sel<<16)|off)
    for r in json.loads((ROOT/'analysis/debug/globals.json').read_text()):
        add('_'+r['name'], ((0x1000+8*(r['ne_segment']-1))<<16)|r['ne_offset'])
    for r in json.loads((ROOT/'references/win16_abi.json').read_text()):
        sel,off = (int(x,16) for x in r['address'].split(':'))
        add(r['name'].upper(),(sel<<16)|off)
        if r['name'].lower() == '_wsprintf': add('_wsprintf',(sel<<16)|off)
    return values


def extent(listing,name):
    active=False;start=None;end=None
    for line in listing.splitlines():
        if re.fullmatch(r'\s+_'+re.escape(name)+':',line):active=True
        ins = re.search(r'\*\*\* ([0-9a-fA-F]+)\s+((?:[0-9a-fA-F]{2} )*[0-9a-fA-F]{2})\s',line)
        if active and ins:
            address=int(ins[1],16)
            if start is None:start=address
            end=address+len(bytes.fromhex(ins[2]))
        if active and '; Local Size:' in line:
            return start,end-start
    raise ValueError('Missing function extent: '+name)


def compare(blob,listing,p,image,known):
    name=p['name'];m,bak=object_module(blob)
    pub=next(x for x in m.publics if x['name']=='_'+name)
    entry,n=extent(listing,name)
    if entry!=pub['offset']:raise ValueError('Listing/public entry disagreement')
    code=m.segments[pub['segment']]
    generated=bytearray(code.data[entry:entry+n]);raw=bytes(generated)
    if not all(code.initialized[entry:entry+n]):raise ValueError('Uninitialized function bytes')
    resolutions=[]
    for f in code.fixups:
        if not entry<=f['offset']<entry+n:continue
        rel=f['offset']-entry
        if rel+f['width']>n or f['relative'] or f['location'] not in (1,3) or f['frame']!=(5,None):
            raise ValueError('Unsupported tutorial fixup semantics: '+str(f))
        method,datum=f['target']
        if method==2:
            symbol=m.externals[datum]
            targets=known.get(symbol,set())
            if len(targets)!=1:raise ValueError('Unknown/ambiguous external '+symbol+': '+str(targets))
            value=next(iter(targets))
        elif method==0 and datum==pub['segment']:
            symbol='current code segment'
            value=((0x1000+8*(p['ne_segment']-1))<<16)+p['ne_offset']-entry
        else:raise ValueError('Unmapped segment/target: '+str(f))
        old=int.from_bytes(generated[rel:rel+f['width']],'little')
        value=(value+f['displacement']+old)&((1<<(f['width']*8))-1)
        generated[rel:rel+f['width']]=value.to_bytes(f['width'],'little')
        resolutions.append(dict(offset=rel,width=f['width'],target=symbol,resolved_value=hex(value)))
    expected=bytes(image[p['ne_segment']-1][p['ne_offset']:p['ne_offset']+p['size']])
    differences=[dict(offset=i,original=a,generated=b) for i,(a,b) in enumerate(zip(expected,generated)) if a!=b]
    result=dict(name=name,ne_segment=p['ne_segment'],original_offset=p['ne_offset'],original_bytes=len(expected),generated_bytes=n,
        exact_before_address_resolution=raw==expected,exact_after_address_resolution=bytes(generated)==expected,
        length_difference=n-len(expected),differing_bytes=len(differences),differences=differences,
        fixups=resolutions,backpatch_records=len(bak),object_sha256=digest(blob),
        original_relocated_sha256=digest(expected),generated_resolved_sha256=digest(generated))
    return result,bytes(generated),expected


def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('directory',type=Path)
    ap.add_argument('--output',type=Path,required=True)
    ap.add_argument('--fixture',type=Path,help='Also retain compiler objects and listings for regression tests')
    args=ap.parse_args()
    rows=json.loads((args.directory/'candidates.json').read_text())
    procs={p['name']:p for p in json.loads((ROOT/'analysis/debug/procedures.json').read_text()) if p['ne_segment'] in (32,33)}
    image=original_image();known=symbols();results=[];objects=[]
    for row in rows:
        stem=row['stem'];p=procs[row['name']]
        try:
            result,generated,expected=compare((args.directory/(stem+'.OBJ')).read_bytes(),(args.directory/(stem+'.COD')).read_text(),p,image,known)
            (args.directory/(stem+'.generated.bin')).write_bytes(generated)
            (args.directory/(stem+'.original.bin')).write_bytes(expected)
            result['source_sha256']=digest((args.directory/(stem+'.C')).read_bytes())
            result['header_sha256']=digest((args.directory/'TYPES.H').read_bytes())
            result['stem']=stem
            source=ROOT/'compat/tutorial-match'/(stem+'.C')
            if source.read_bytes() != (args.directory/(stem+'.C')).read_bytes():
                raise ValueError('Working source differs from checked-in candidate')
            if (ROOT/'compat/tutorial-match/TYPES.H').read_bytes() != (args.directory/'TYPES.H').read_bytes():
                raise ValueError('Working header differs from checked-in header')
            result['source']=str(source.relative_to(ROOT))
            result['build_log']=(args.directory/(stem+'.LOG')).read_text()
            objects.append(dict(stem=stem,object_hex=(args.directory/(stem+'.OBJ')).read_bytes().hex(),listing=(args.directory/(stem+'.COD')).read_text()))
            print(stem,p['name'],result['generated_bytes'],'/',result['original_bytes'], 'MATCH' if result['exact_after_address_resolution'] else str(result['differing_bytes'])+' differences')
        except (ValueError,FileNotFoundError) as e:
            result=dict(name=p['name'],stem=stem,exact_after_address_resolution=False,error=str(e))
            print(stem,p['name'],str(e))
        results.append(result)
    if any('error' in r for r in results):
        raise SystemExit('Comparison failed; evidence was not written')
    args.output.write_text(json.dumps(dict(original_sha256=ORIGINAL_SHA,
        compiler='Microsoft C/C++ Compiler Version 8.00c (VC++ 1.52c)',
        flags='/c /AM /GA /GEs /Zp1 /Od /Gs /W3 /Fc',
        comparison='No masked bytes; named OMF relocations resolved into the original address layout.',
        functions=results),indent=2)+'\n')
    if args.fixture:
        args.fixture.write_text(json.dumps(dict(original_sha256=ORIGINAL_SHA,objects=objects),indent=2)+'\n')

if __name__=='__main__':main()
