#!/usr/bin/env python3
"""Bounded VC152 object comparison; resolve named fixups, never mask differences.

OMF record semantics: https://openwatcom.org/ftp/devel/docs/omf.pdf
Only the record subset actually emitted by this no-debug probe is accepted.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import struct
import sys

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT))
from tools.omf import Cursor, library
from tools.ne_layout import NE, patch

ORIGINAL_SHA = 'ccab6682b55dae14dbf104e491cfbd3b32a6280022020032ae0615500949ba59'
TARGETS = {'GetRaceStat': (29, 0x30d2, 34), 'SetRaceStat': (29, 0x3114, 97), 'Random': (9, 0x16d2, 439)}

def digest(data):
    return hashlib.sha256(data).hexdigest()

def object_module(blob):
    """Adapt only extra source headers, empty debug SEGDEF32 and BAKPAT.

    Retain all code/data/FIXUPP records. Backpatch values are compiler output,
    applied as specified by OMF, not inferred from the comparison target.
    """
    pos = 0
    filtered = bytearray()
    seen = False
    backpatches = []
    while pos < len(blob):
        if pos + 3 > len(blob):
            raise ValueError('Truncated OMF record')
        kind, length = struct.unpack_from('<BH', blob, pos)
        raw = blob[pos:pos+3+length]
        if not length or len(raw) != 3+length:
            raise ValueError('Invalid OMF length')
        if raw[-1] and sum(raw) & 255:
            raise ValueError('OMF checksum mismatch')
        body = raw[3:-1]
        pos += 3+length
        if kind == 0x80:
            if seen:
                continue  # Additional source-file THEADR; not a new library member.
            seen = True
        if kind == 0x99:
            if len(body) != 8 or body[0] != 0x21 or int.from_bytes(body[1:5], 'little'):
                raise ValueError('Only empty debug SEGDEF32 supported')
            payload = body[:1] + b'\0\0' + body[5:]
            raw = b'\x98' + struct.pack('<H', len(payload)+1) + payload + b'\0'
        if kind == 0xb2:
            c = Cursor(body)
            seg, location = c.index(), c.byte()
            if location not in (0, 1):
                raise ValueError('Only 16-bit BAKPAT supported')
            while c.remaining():
                backpatches.append((seg, c.word(), c.word()))
        else:
            filtered.extend(raw)
    modules = library(filtered)
    if len(modules) != 1 or modules[0].unsupported:
        raise ValueError('Unsupported or multiple object modules')
    m = modules[0]
    for seg in m.segments[1:]:
        if seg.name.startswith('$$') and seg.size:
            raise ValueError('Nonempty debug segment unsupported')
    for index, off, value in backpatches:
        seg = m.segments[index]
        if off+2 > seg.size or not all(seg.initialized[off:off+2]):
            raise ValueError('Backpatch outside initialized segment')
        if any(off < f['offset']+f['width'] and f['offset'] < off+2 for f in seg.fixups):
            raise ValueError('Backpatch overlapping relocation unsupported')
        old = int.from_bytes(seg.data[off:off+2], 'little')
        seg.data[off:off+2] = ((old+value)&65535).to_bytes(2,'little')
    return m, backpatches

def function_sizes(listing):
    result = {}
    current = None
    end = 0
    for line in listing.splitlines():
        label = re.fullmatch(r'\s+_([A-Za-z0-9]+):', line)
        if label and label[1] in TARGETS:
            current = label[1]
            start = None
        ins = re.search(r'\*\*\* ([0-9a-fA-F]+)\s+((?:[0-9a-fA-F]{2} )*[0-9a-fA-F]{2})\s', line)
        if current and ins:
            address = int(ins[1],16)
            if start is None:
                start = address
            end = address+len(bytes.fromhex(ins[2]))
        if current and '; Local Size:' in line:
            result[current] = (start, end-start)
            current = None
    if set(result) != set(TARGETS):
        raise ValueError('Missing function extent in compiler listing')
    return result

def original_functions():
    binary = ROOT/'target/stars.exe'
    if digest(binary.read_bytes()) != ORIGINAL_SHA:
        raise ValueError('Original executable hash mismatch')
    ne = NE(binary)
    result = {}
    for name,(seg,off,size) in TARGETS.items():
        memory = bytearray(ne.raw[seg-1])
        for relocation in ne.relocations:
            if relocation['segment'] != seg:
                continue
            for site in relocation['sites']:
                if not off <= site < off+size:
                    continue
                if relocation['target_kind'] != 'internal':
                    raise ValueError('Unexpected original relocation')
                selector = 0x1000 + (relocation['target_segment']-1)*8
                patch(memory, site, relocation['address_kind'], selector,
                      relocation['target_offset'], relocation['additive'])
        result[name] = bytes(memory[off:off+size])
    return ne,result

def compare(blob, listing):
    m,bak = object_module(blob)
    sizes = function_sizes(listing)
    ne,original = original_functions()
    publics = {p['name']:p for p in m.publics}
    exported = json.loads((ROOT/'analysis/decompiler/functions.json').read_text())
    externals = {}
    for symbol in ['__aFldiv','__aFlmul','__aFulrem']:
        row = next(f for f in exported if f['name']==symbol)
        selector,offset = (int(x,16) for x in row['address'].split(':'))
        externals[symbol] = (selector<<16)|offset
    globals_ = json.loads((ROOT/'analysis/debug/globals.json').read_text())
    for symbol in ['lRandSeed1','lRandSeed2']:
        rows=[g for g in globals_ if g['name']==symbol and g['ne_segment']==37]
        if len(rows)!=1:raise ValueError('Ambiguous global identity')
        externals['_'+symbol] = rows[0]['ne_offset']
        # Explicit semantic alias used by the matching code-generation candidate.
        externals['_'+symbol.replace('lRandSeed','lSeed')] = rows[0]['ne_offset']
    # Place the probe's two race routines and intervening tables together.
    race = publics['_GetRaceStat']
    bases = {race['segment']: TARGETS['GetRaceStat'][1]-race['offset']}
    results=[]
    for name,(seg,off,size) in TARGETS.items():
        pub=publics['_'+name];entry,n=sizes[name]
        if pub['offset']!=entry:raise ValueError('Listing/public entry disagreement')
        code=m.segments[pub['segment']]
        generated=bytearray(code.data[entry:entry+n]);raw=bytes(generated)
        resolutions=[]
        for f in code.fixups:
            if not entry<=f['offset']<entry+n:continue
            relative=f['offset']-entry
            if f['relative'] or f['location'] not in (1,3) or f['frame']!=(5,None):
                raise ValueError('Unsupported probe fixup semantics')
            method,datum=f['target']
            if method==0:
                if datum not in bases:raise ValueError('Unknown code segment mapping')
                value=bases[datum]+f['displacement'];symbol='race code/table segment'
            elif method==2:
                symbol=m.externals[datum]
                if symbol not in externals:raise ValueError('Unmapped external '+symbol)
                value=externals[symbol]+f['displacement']
            else:raise ValueError('Unsupported target method')
            old=int.from_bytes(generated[relative:relative+f['width']],'little')
            value+=old
            generated[relative:relative+f['width']]=value.to_bytes(f['width'],'little')
            resolutions.append(dict(offset=relative,width=f['width'],target=symbol,resolved_value=hex(value)))
        expected=original[name]
        diffs=[dict(offset=i,original=a,generated=b) for i,(a,b) in enumerate(zip(expected,generated)) if a!=b]
        results.append(dict(name=name,ne_segment=seg,original_offset=off,original_bytes=len(expected),generated_bytes=n,
            exact_before_address_resolution=raw==expected,exact_after_address_resolution=generated==expected,
            nonaddress_byte_differences=diffs,length_difference=len(generated)-len(expected),
            fixups=resolutions,original_relocated_sha256=digest(expected),generated_resolved_sha256=digest(generated)))
    # Validate table contents separately, not only the operands referencing them.
    table=m.segments[race['segment']].data[race['offset']+34:race['offset']+66]
    expected_table=ne.raw[28][0x30f4:0x3114]
    return dict(original_sha256=ORIGINAL_SHA,object_sha256=digest(blob),backpatch_records=len(bak),
                comparison='No masked bytes: resolve OMF fixups by named identities into original address layout; retain all other bytes exactly.',
                functions=results,stat_table_bytes=32,stat_tables_match=table==expected_table,
                all_functions_match=all(r['exact_after_address_resolution'] for r in results))

def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('directory',type=Path)
    ap.add_argument('--output',type=Path,required=True)
    args=ap.parse_args()
    result=compare((args.directory/'PROBE.OBJ').read_bytes(),(args.directory/'PROBE.COD').read_text())
    result['source_sha256']=digest((args.directory/'PROBE.C').read_bytes())
    result['build_log']=(args.directory/'BUILD.LOG').read_text()
    result['build_status']=(args.directory/'DONE.TXT').read_text()
    args.output.write_text(json.dumps(result,indent=2)+'\n')
    for row in result['functions']:
        print(row['name'],row['generated_bytes'],'bytes:', 'MATCH' if row['exact_after_address_resolution'] else 'DIFFERENT')
    if not result['all_functions_match'] or not result['stat_tables_match']:
        raise SystemExit(1)

if __name__=='__main__':main()
