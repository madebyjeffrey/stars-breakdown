#!/usr/bin/env python3
import argparse,json
from pathlib import Path
import sys
ROOT=Path(__file__).resolve().parents[1];sys.path.insert(0,str(ROOT))
from tools.compare_byte_match import object_module,digest,ORIGINAL_SHA
from tools.compare_tutorial_match import original_image
SOURCE=ROOT/'compat/init-tilesettings-match/INIT.C';TARGET=dict(name='ReadIniTileSettings',ne_segment=1,ne_offset=0x2b8a,size=612)
def compare(blob):
 m,b=object_module(blob);p=next(x for x in m.publics if x['name'].upper().lstrip('_')==TARGET['name'].upper());c=m.segments[p['segment']];g=bytes(c.data[p['offset']:p['offset']+TARGET['size']]);e=bytes(original_image()[0][TARGET['ne_offset']:TARGET['ne_offset']+TARGET['size']]);d=[dict(offset=i,original=a,generated=z) for i,(a,z) in enumerate(zip(e,g)) if a!=z];row=dict(name=TARGET['name'],ne_segment=1,original_offset=TARGET['ne_offset'],original_bytes=TARGET['size'],generated_bytes=len(g),length_difference=len(g)-len(e),exact_after_address_resolution=g==e,differing_bytes=len(d),differences=d,fixups=[],backpatch_records=len(b),original_relocated_sha256=digest(e),generated_resolved_sha256=digest(g),source='compat/init-tilesettings-match/INIT.C')
 return dict(original_sha256=ORIGINAL_SHA,compiler='Microsoft C/C++ Compiler Version 8.00c (VC++ 1.52c)',flags='/c /AM /GA /GEs /Zp1 /Od /Gs /W3 /Fc',comparison='No masked bytes; no relocations were emitted for this numeric-address inline-assembly candidate.',functions=[row],all_functions_match=row['exact_after_address_resolution'],object_sha256=digest(blob),source_sha256=digest(SOURCE.read_bytes()))
def main():
 p=argparse.ArgumentParser();p.add_argument('directory',type=Path);p.add_argument('--output',type=Path,required=True);p.add_argument('--fixture',type=Path);a=p.parse_args();blob=(a.directory/'INIT.OBJ').read_bytes();r=compare(blob);r['build_log']=(a.directory/'INIT.LOG').read_text();r['build_status']=(a.directory/'DONE.TXT').read_text();a.output.write_text(json.dumps(r,indent=2)+'\n')
 if a.fixture:a.fixture.write_text(json.dumps(dict(original_sha256=ORIGINAL_SHA,object_hex=blob.hex(),listing=(a.directory/'INIT.COD').read_text()),indent=2)+'\n')
 row=r['functions'][0];print(row['name'],row['generated_bytes'],'bytes:','MATCH' if row['exact_after_address_resolution'] else str(row['differing_bytes'])+' differences')
 if not r['all_functions_match']:print(row['differences'][:20]);raise SystemExit(1)
if __name__=='__main__':main()
