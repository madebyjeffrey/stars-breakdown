#!/usr/bin/env python3
import argparse,json
from pathlib import Path
import sys
ROOT=Path(__file__).resolve().parents[1];sys.path.insert(0,str(ROOT))
from tools.compare_byte_match import object_module,digest,ORIGINAL_SHA
from tools.compare_tutorial_match import original_image
TARGETS=[('FCreateStuff',1,0x14,2718,'A.OBJ'),('ReadIniSettings',1,0x124c,6461,'B.OBJ'),('FrameWndProc',5,0x6d6,5990,'C.OBJ'),('InvertPaneBorder',5,0x1e3c,1651,'D.OBJ'),('CommandHandler',5,0x2f7a,9211,'E.OBJ'),('FOpenGame',5,0x58f4,1203,'F.OBJ'),('HostModeDialog',5,0x6c16,2488,'G.OBJ'),('WriteIniSettings',5,0x7a76,4625,'H.OBJ'),('TitleWndProc',5,0x9126,1818,'I.OBJ'),('WinMain',4,0,1699,'J.OBJ'),('FreeStuff',4,0xbae,1635,'K.OBJ'),('FHandleKey',4,0x165a,1506,'L.OBJ')]
SOURCES=['FCreateStuff.C','ReadIniSettings.C','FrameWndProc.C','InvertPaneBorder.C','CommandHandler.C','FOpenGame.C','HostModeDialog.C','WriteIniSettings.C','TitleWndProc.C','WinMain.C','FreeStuff.C','FHandleKey.C']
def compare(directory):
 rows=[];objects={}
 for (name,seg,off,size,obj),src in zip(TARGETS,SOURCES):
  blob=(directory/obj).read_bytes();objects[name]=blob.hex();m,b=object_module(blob);p=next(x for x in m.publics if x['name'].upper().lstrip('_')==name.upper());c=m.segments[p['segment']];g=bytes(c.data[p['offset']:p['offset']+size]);e=bytes(original_image()[seg-1][off:off+size]);d=[dict(offset=i,original=a,generated=z) for i,(a,z) in enumerate(zip(e,g)) if a!=z];rows.append(dict(name=name,ne_segment=seg,original_offset=off,original_bytes=size,generated_bytes=len(g),length_difference=len(g)-len(e),exact_after_address_resolution=g==e,differing_bytes=len(d),differences=d,fixups=[],backpatch_records=len(b),original_relocated_sha256=digest(e),generated_resolved_sha256=digest(g),source='compat/startup-rest-byte-match/'+src,source_sha256=digest((ROOT/'compat/startup-rest-byte-match'/src).read_bytes())))
 top=digest(b''.join((ROOT/'compat/startup-rest-byte-match'/s).read_bytes() for s in SOURCES));return dict(original_sha256=ORIGINAL_SHA,compiler='Microsoft C/C++ Compiler Version 8.00c (VC++ 1.52c)',flags='/c /AM /GA /GEs /Zp1 /Od /Gs /W3 /Fc',comparison='No masked bytes; compiler-emitted bytes were compared against the original function extents.',functions=rows,all_functions_match=all(r['exact_after_address_resolution'] for r in rows),source_sha256=top,object_sha256={k:digest(bytes.fromhex(v)) for k,v in objects.items()},objects=objects)
def main():
 p=argparse.ArgumentParser();p.add_argument('directory',type=Path);p.add_argument('--output',type=Path,required=True);a=p.parse_args();r=compare(a.directory);r['build_log']=(a.directory/'MDI.LOG').read_text();r['build_status']=(a.directory/'DONE.TXT').read_text();a.output.write_text(json.dumps(r,indent=2)+'\n');
 for row in r['functions']: print(row['name'],row['generated_bytes'],'bytes:','MATCH' if row['exact_after_address_resolution'] else str(row['differing_bytes'])+' differences')
 if not r['all_functions_match']: raise SystemExit(1)
if __name__=='__main__':main()
