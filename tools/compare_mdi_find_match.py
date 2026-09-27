#!/usr/bin/env python3
import argparse, json
from pathlib import Path
import sys
ROOT=Path(__file__).resolve().parents[1]; sys.path.insert(0,str(ROOT))
from tools.compare_byte_match import object_module,digest,ORIGINAL_SHA
from tools.compare_startup_match import symbol_values
from tools.compare_tutorial_match import original_image
SOURCE=ROOT/'compat/mdi-find-match/MDI.C'; TARGET=dict(name='FFindSomethingAndSelectIt',ne_segment=5,ne_offset=0x2e1c,size=349)
def compare(blob):
 m,b=object_module(blob); p=next(p for p in m.publics if p['name'].upper().lstrip('_')==TARGET['name'].upper()); c=m.segments[p['segment']]; g=bytearray(c.data[p['offset']:p['offset']+TARGET['size']]); e=bytes(original_image()[4][TARGET['ne_offset']:TARGET['ne_offset']+TARGET['size']]); vals={}
 for n,cs in symbol_values().items():
  for v in cs: vals.setdefault(n.upper(),set()).add(v); vals.setdefault(n.lstrip('_').upper(),set()).add(v); vals.setdefault('_'+n.lstrip('_').upper(),set()).add(v)
 fixes=[]
 for f in c.fixups:
  if not p['offset']<=f['offset']<p['offset']+TARGET['size']: continue
  s=m.externals[f['target'][1]]; ts=vals.get(s.upper(),set()) or vals.get('_'+s.lstrip('_').upper(),set())
  if len(ts)!=1: raise ValueError('unknown '+s)
  v=next(iter(ts))+f['displacement']; v=(v&0xffff) if f['location'] in (1,5) else ((v>>16)&0xffff if f['location']==2 else v); q=f['offset']-p['offset']; old=int.from_bytes(g[q:q+f['width']],'little'); v=(v+old)&((1<<(8*f['width']))-1); g[q:q+f['width']]=v.to_bytes(f['width'],'little'); fixes.append(dict(offset=q,width=f['width'],target=s,resolved_value=hex(v)))
 d=[dict(offset=i,original=a,generated=z) for i,(a,z) in enumerate(zip(e,g)) if a!=z]; row=dict(name=TARGET['name'],ne_segment=5,original_offset=TARGET['ne_offset'],original_bytes=TARGET['size'],generated_bytes=len(g),length_difference=len(g)-len(e),exact_after_address_resolution=bytes(g)==e,differing_bytes=len(d),differences=d,fixups=fixes,backpatch_records=len(b),original_relocated_sha256=digest(e),generated_resolved_sha256=digest(bytes(g)),source='compat/mdi-find-match/MDI.C')
 return dict(original_sha256=ORIGINAL_SHA,compiler='Microsoft C/C++ Compiler Version 8.00c (VC++ 1.52c)',flags='/c /AM /GA /GEs /Zp1 /Od /Gs /W3 /Fc',comparison='No masked bytes; named OMF relocations resolved into the original address layout.',functions=[row],all_functions_match=row['exact_after_address_resolution'],object_sha256=digest(blob),source_sha256=digest(SOURCE.read_bytes()))
def main():
 p=argparse.ArgumentParser();p.add_argument('directory',type=Path);p.add_argument('--output',type=Path,required=True);p.add_argument('--fixture',type=Path);a=p.parse_args(); blob=(a.directory/'MDI.OBJ').read_bytes();r=compare(blob);r['build_log']=(a.directory/'MDI.LOG').read_text();r['build_status']=(a.directory/'DONE.TXT').read_text();a.output.write_text(json.dumps(r,indent=2)+'\n');
 if a.fixture:a.fixture.write_text(json.dumps(dict(original_sha256=ORIGINAL_SHA,object_hex=blob.hex(),listing=(a.directory/'MDI.COD').read_text()),indent=2)+'\n')
 row=r['functions'][0];print(row['name'],row['generated_bytes'],'bytes:','MATCH' if row['exact_after_address_resolution'] else str(row['differing_bytes'])+' differences');
 if not r['all_functions_match']:print(row['differences'][:20]);raise SystemExit(1)
if __name__=='__main__':main()
