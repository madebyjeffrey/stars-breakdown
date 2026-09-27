"""Read retained FAT metadata and compare source-derived disk identity with pinned logs."""
import hashlib,json,struct,sys,ctypes as C
from pathlib import Path
ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT))
from tools.race_file_compare import Inspector
from tools.stars_game_stream import decode_stream
ins=Inspector();hdrs=[next(r['body'] for r in decode_stream(ROOT/'runs'/n/'BACKUP/CLEAN.X2',ins) if r['kind']==9) for n in ('vmctl-sye3k0','vmctl-sye3k1')]
class Disk(C.Structure):
 _fields_=[('present',C.c_int16),('date',C.c_uint16),('time',C.c_uint16),('label',C.c_char_p),('nibble',C.c_ubyte)]
fn=ins.lib.stars_get_disk_serial_number;fn.argtypes=[C.POINTER(Disk),C.POINTER(C.c_ubyte)];fn.restype=C.c_uint16
results=[]
for name in ('runs/step4/win98.img','runs/step4/win95.img','runs/step4/transfer.img'):
 with (ROOT/name).open('rb') as f:
  mbr=f.read(512);start=struct.unpack_from('<I',mbr,454)[0]*512
  f.seek(start);boot=f.read(512);bps=struct.unpack_from('<H',boot,11)[0];spc=boot[13];reserved=struct.unpack_from('<H',boot,14)[0];fats=boot[16];roots=struct.unpack_from('<H',boot,17)[0]
  total=struct.unpack_from('<H',boot,19)[0] or struct.unpack_from('<I',boot,32)[0];fatsectors=struct.unpack_from('<H',boot,22)[0] or struct.unpack_from('<I',boot,36)[0]
  rootsectors=(roots*32+bps-1)//bps;data_sector=reserved+fats*fatsectors+rootsectors;clusters=(total-data_sector)//spc
  evidence=hashlib.sha256(mbr+boot);entries=[]
  if roots:
   f.seek(start+(reserved+fats*fatsectors)*bps);chunks=[f.read(rootsectors*bps)]
  else:
   cluster=struct.unpack_from('<I',boot,44)[0];chunks=[];seen=set()
   while cluster<0xffffff8:
    assert cluster>=2 and cluster not in seen and len(seen)<100;seen.add(cluster)
    f.seek(start+(data_sector+(cluster-2)*spc)*bps);chunks.append(f.read(spc*bps))
    f.seek(start+reserved*bps+cluster*4);cluster=struct.unpack('<I',f.read(4))[0]&0xfffffff
  for chunk in chunks:
   evidence.update(chunk)
   for i in range(0,len(chunk),32):
    entry=chunk[i:i+32]
    if entry[0]==0:break
    if entry[0]!=0xe5 and entry[11]&8 and entry[11]!=15:entries.append(entry)
  assert len(entries)<=1
  entry=entries[0] if entries else None
  label=entry[:11].rstrip(b' ') if entry else None
  date,time=(struct.unpack_from('<H',entry,24)[0],struct.unpack_from('<H',entry,22)[0]) if entry else (0,0)
  nibble=min(15,(((clusters*bps)//512)*spc)//195312)
  drives=(Disk*2)(Disk(bool(entry),date,time,label,nibble),Disk(0,0,0,None,0));out=(C.c_ubyte*11)();assert fn(drives,out)==11
  results.append({'path':name,'image_bytes':(ROOT/name).stat().st_size,'identity_sectors_sha256':evidence.hexdigest(),'volume_label_entry_present':bool(entry),'dos_date':date,'dos_time':time,'total_clusters':clusters,'bytes_per_sector':bps,'sectors_per_cluster':spc,'capacity_nibble':nibble,'c_identity_matches_control_action':[bytes(out)[:6]==h[6:12] for h in hdrs],'c_capacity_matches_control_action':[(out[10]>>4)==(h[16]>>4) for h in hdrs],'full_environment_matches_if_d_unavailable':[bytes(out)==h[6:] for h in hdrs]})
report={'scope':'Read-only FAT identity extraction. Images are candidates; D-drive availability and pinned-run attachment are not established.', 'candidates':results}
(ROOT/'analysis/compat/satisfy-orders-e3-disk-candidate-audit.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2))
