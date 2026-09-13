#!/usr/bin/env python3
"""Render extracted type metadata as a study reference, not compilable C."""
import json
from pathlib import Path

root=Path(__file__).resolve().parents[1]/'analysis'
types=json.loads((root/'types.json').read_text())
T={t['index']:t for t in types}
primitives={0x03:('void',0),0x10:('signed char',1),0x11:('short',2),0x12:('long',4),
            0x20:('unsigned char',1),0x21:('unsigned short',2),0x22:('unsigned long',4),
            0x40:('float',4),0x41:('double',8),0x70:('char',1),0x72:('int',2),0x73:('unsigned int',2)}
def describe(i,depth=0):
    if depth>12:return f'type_{i:04x}'
    if i<0x1000:
        base=primitives.get(i&255,(f'type_{i&255:04x}',None))[0]
        return base+{0:'',1:' near *',2:' far *',3:' huge *'}.get((i>>8)&7,' pointer')
    t=T[i];leaf=t['leaf']
    if 'name' in t:return t['name']
    if leaf=='0002':return describe(t['underlying_type'],depth+1)+' '+{0:'near *',1:'far *',2:'huge *'}.get(t['attributes']&31,'pointer')
    if leaf=='0001':return 'qualified '+describe(t['underlying_type'],depth+1)
    if leaf=='0003':return f"array of {describe(t['element_type'],depth+1)} ({t['size']} bytes total)"
    if leaf=='0206':return f"{describe(t['underlying_type'],depth+1)}; bits {t['bit_position']}..{t['bit_position']+t['bit_length']-1}"
    if leaf=='0008':return 'function returning '+describe(t['return_type'],depth+1)
    return f'type_{i:04x}'
rows=['# Recovered data layouts','','These are debug metadata, not reconstructed source declarations. Sizes are bytes in the original 16-bit build. Overlapping members and bit fields are preserved; do not paste these layouts into a modern compiler and assume the same packing.','']
for t in types:
    if 'members' not in t:continue
    rows += [f"## {t['name']} — {t['size']} bytes (type 0x{t['index']:04x})",'', '| Offset | Member | Type |','|---:|---|---|']
    for m in t['members']:
        rows.append(f"| {m['offset']} | `{m['name']}` | {describe(m['type_index'])} |")
    if 'undecoded_field_tail' in t:rows+=['','Some field records remain undecoded.']
    rows+=['']
(root/'DATA_LAYOUTS.md').write_text('\n'.join(rows).rstrip()+'\n')
print('Rendered',sum('members' in t for t in types),'data layouts')
