#!/usr/bin/env python3
import argparse,json
from pathlib import Path
import sys
ROOT=Path(__file__).resolve().parents[1];sys.path.insert(0,str(ROOT))
from tools.compare_byte_match import object_module,digest,ORIGINAL_SHA
from tools.compare_startup_match import symbol_values
from tools.compare_tutorial_match import original_image
SOURCE=ROOT/'compat/mdi-racefile-match/MDI.C'; TARGET=dict(name='FWasRaceFile',ne_segment=5,ne_offset=0x5da8,size=596)
def compare(blob):
 m,b=object_module(blob);p=next(x for x in m.publics if x['name'].upper().lstrip('_')==TARGET['name'].upper());c=m.segments[p['segment']];g=bytearray(c.data[p['offset']:p['offset']+TARGET['size']]);e=bytes(original_image()[4][TARGET['ne_offset']:TARGET['ne_offset']+TARGET['size']]); sy={}
 for n,vs in symbol_values().items():
  for v in vs: sy.setdefault(n.upper(),set()).add(v);sy.setdefault(n.lstrip('_').upper(),set()).add(v);sy.setdefault('_'+n.lstrip('_').upper(),set()).add(v)
 fx=[]
 for f in c.fixups:
  if not p['offset']<=f['offset']<p['offset']+TARGET['size']:continue
  if f['relative'] or f['frame']!=(5,None):raise ValueError(str(f))
  s=m.externals[f['target'][1]];ts=sy.get(s.upper(),set()) or sy.get('_'+s.lstrip('_').upper(),set())
  if len(ts)!=1:raise ValueError('unknown '+s)
  v=next(iter(ts))+f['displacement'];
  if f['location'] in (1,5):v&=0xffff
  elif f['location']==2:v=(v>>16)&0xffff
  elif f['location']!=3:raise ValueError(str(f))
  q=f['offset']-p['offset'];old=int.from_bytes(g[q:q+f['width']],'little');v=(v+old)&((1<<(8*f['width']))-1);g[q:q+f['width']]=v.to_bytes(f['width'],'little');fx.append(dict(offset=q,width=f['width'],target=s,resolved_value=hex(v)))
 d=[dict(offset=i,original=a,generated=z) for i,(a,z) in enumerate(zip(e,g)) if a!=z];row=dict(name=TARGET['name'],ne_segment=5,original_offset=TARGET['ne_offset'],original_bytes=TARGET['size'],generated_bytes=len(g),length_difference=len(g)-len(e),exact_after_address_resolution=bytes(g)==e,differing_bytes=len(d),differences=d,fixups=fx,backpatch_records=len(b),original_relocated_sha256=digest(e),generated_resolved_sha256=digest(bytes(g)),source='compat/mdi-racefile-match/MDI.C')
 return dict(original_sha256=ORIGINAL_SHA,compiler='Microsoft C/C++ Compiler Version 8.00c (VC++ 1.52c)',flags='/c /AM /GA /GEs /Zp1 /Od /Gs /W3 /Fc',comparison='No masked bytes; named OMF relocations resolved into the original address layout.',functions=[row],all_functions_match=row['exact_after_address_resolution'],object_sha256=digest(blob),source_sha256=digest(SOURCE.read_bytes()))
def main():
 p=argparse.ArgumentParser();p.add_argument('directory',type=Path);p.add_argument('--output',type=Path,required=True);p.add_argument('--fixture',type=Path);a=p.parse_args();blob=(a.directory/'MDI.OBJ').read_bytes();r=compare(blob);r['build_log']=(a.directory/'MDI.LOG').read_text();r['build_status']=(a.directory/'DONE.TXT').read_text();a.output.write_text(json.dumps(r,indent=2)+'\n');
 if a.fixture:a.fixture.write_text(json.dumps(dict(original_sha256=ORIGINAL_SHA,object_hex=blob.hex(),listing=(a.directory/'MDI.COD').read_text()),indent=2)+'\n')
 row=r['functions'][0];print(row['name'],row['generated_bytes'],'bytes:','MATCH' if row['exact_after_address_resolution'] else str(row['differing_bytes'])+' differences');
 if not r['all_functions_match']:print(row['differences'][:20]);raise SystemExit(1)
if __name__=='__main__':main()
