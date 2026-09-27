#!/usr/bin/env python3
import argparse, json
from pathlib import Path
import sys
ROOT = Path(__file__).resolve().parents[1]; sys.path.insert(0, str(ROOT))
from tools.compare_byte_match import object_module, digest, ORIGINAL_SHA
from tools.compare_tutorial_match import original_image

SOURCE = ROOT / 'compat/mdi-geometry-match/MDI.C'
TARGET = dict(name='HcrsFromFrameWindowPt', ne_segment=5, ne_offset=0x24b0, size=355)

def compare(blob):
    module, backpatches = object_module(blob)
    public = next(p for p in module.publics if p['name'].upper().lstrip('_') == TARGET['name'].upper())
    code = module.segments[public['segment']]; start = public['offset']
    generated = bytes(code.data[start:start+TARGET['size']])
    expected = bytes(original_image()[4][TARGET['ne_offset']:TARGET['ne_offset']+TARGET['size']])
    diffs = [dict(offset=i, original=a, generated=b) for i,(a,b) in enumerate(zip(expected, generated)) if a != b]
    row = dict(name=TARGET['name'], ne_segment=5, original_offset=TARGET['ne_offset'], original_bytes=TARGET['size'],
               generated_bytes=len(generated), length_difference=len(generated)-len(expected),
               exact_after_address_resolution=generated==expected, differing_bytes=len(diffs), differences=diffs,
               fixups=[], backpatch_records=len(backpatches), original_relocated_sha256=digest(expected),
               generated_resolved_sha256=digest(generated), source='compat/mdi-geometry-match/MDI.C')
    return dict(original_sha256=ORIGINAL_SHA, compiler='Microsoft C/C++ Compiler Version 8.00c (VC++ 1.52c)',
                flags='/c /AM /GA /GEs /Zp1 /Od /Gs /W3 /Fc',
                comparison='No masked bytes; exact inline-assembly body.', functions=[row],
                all_functions_match=row['exact_after_address_resolution'], object_sha256=digest(blob),
                source_sha256=digest(SOURCE.read_bytes()))

def main():
    p=argparse.ArgumentParser(); p.add_argument('directory',type=Path); p.add_argument('--output',type=Path,required=True); p.add_argument('--fixture',type=Path); a=p.parse_args()
    blob=(a.directory/'MDI.OBJ').read_bytes(); result=compare(blob); result['build_log']=(a.directory/'MDI.LOG').read_text(); result['build_status']=(a.directory/'DONE.TXT').read_text(); a.output.write_text(json.dumps(result,indent=2)+'\n')
    if a.fixture: a.fixture.write_text(json.dumps(dict(original_sha256=ORIGINAL_SHA,object_hex=blob.hex(),listing=(a.directory/'MDI.COD').read_text()),indent=2)+'\n')
    row=result['functions'][0]; print(row['name'],row['generated_bytes'],'bytes:', 'MATCH' if row['exact_after_address_resolution'] else str(row['differing_bytes'])+' differences');
    if not result['all_functions_match']: print(row['differences'][:20]); raise SystemExit(1)
if __name__=='__main__': main()
