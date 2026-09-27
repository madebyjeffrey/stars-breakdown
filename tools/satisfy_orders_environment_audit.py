"""Compare archived INI metadata with pinned log headers without printing registration data."""
import ctypes as C, hashlib, json, sys
from pathlib import Path
ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT))
from tools.ne_layout import NE
from tools.race_file_compare import Inspector,Seeds
from tools.stars_game_stream import decode_stream
root=ROOT; ins=Inspector(); lib=ins.lib
raw=NE(root/'target/stars.exe').raw
order=bytes(raw[4][0x2870:0x2870+21])
assert sorted(order)==list(range(21)), list(order)
U8=C.c_ubyte; PTR=C.POINTER(U8)
lib.stars_file_valid_serial_long.argtypes=[PTR,C.c_uint32];lib.stars_file_valid_serial_long.restype=C.c_int16
lib.stars_serial_and_env_from_string.argtypes=[C.POINTER(Seeds),PTR,C.POINTER(C.c_uint32),PTR,PTR,C.c_void_p,C.c_void_p];lib.stars_serial_and_env_from_string.restype=C.c_int16
data=(U8*65536).from_buffer_copy(ins.initial)
validator=C.CFUNCTYPE(C.c_int16,C.c_void_p,C.c_uint32)(lambda _,serial:lib.stars_file_valid_serial_long(data,serial))
headers=[]
for name in ('vmctl-sye3k0','vmctl-sye3k1'):
 rows=decode_stream(root/'runs'/name/'BACKUP/CLEAN.X2',ins)
 hdr=next(r['body'] for r in rows if r['kind']==9);assert len(hdr)==17
 headers.append(hdr)
report={'scope':'Read-only comparison; no runtime inputs changed. INI capture provenance and current disk environment remain unproven.', 'shuffle_source':{'path':'target/stars.exe','segment':5,'offset':10352,'sha256':hashlib.sha256(order).hexdigest()},'candidates':[]}
for relative in ('runs/step4/guest/WINDOWS/STARS.INI','runs/step4/guest98/WINDOWS/STARS.INI'):
 path=root/relative; encoded=next(line.split('=',1)[1].strip() for line in path.read_text(errors='replace').splitlines() if line.startswith('GlobalSettings='))
 assert len(encoded)==28
 buf=(U8*29).from_buffer_copy(encoded.encode()+b'\0'); serial=C.c_uint32();env=(U8*11)();state=Seeds()
 ok=lib.stars_serial_and_env_from_string(C.byref(state),buf,C.byref(serial),env,(U8*21).from_buffer_copy(order),C.cast(validator,C.c_void_p),None)
 report['candidates'].append({'path':relative,'sha256':hashlib.sha256(path.read_bytes()).hexdigest(),'source_decode_valid':bool(ok),'serial_matches_control_action':[bool(ok and serial.value==int.from_bytes(h[2:6],'little')) for h in headers], 'registered_environment_matches_current_log_environment':[bool(ok and bytes(env)==h[6:]) for h in headers]})
report['original_header_environments_equal']=headers[0][6:]==headers[1][6:]
(root/'analysis/compat/satisfy-orders-e3-environment-candidate-audit.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(report,indent=2))
