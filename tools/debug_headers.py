#!/usr/bin/env python3
"""Generate C89 storage-view headers with explicit Win16 scalar/pointer widths.
Host builds encode pointers as integers; STARS_NATIVE16 enables VC1.5 near/far types.
Overlaps are independent named views, not guesses at original anonymous unions.
"""
import json
from pathlib import Path
import re

ROOT=Path(__file__).resolve().parents[1]

def ident(s):return re.sub(r'[^A-Za-z0-9_]','_',s)

def generate(out):
    T=json.loads((ROOT/'analysis/debug/types.json').read_text());ts={t['index']:t for t in T}
    ps=json.loads((ROOT/'analysis/debug/primitives.json').read_text());S=json.loads((ROOT/'analysis/debug/symbols.json').read_text())
    funcs=json.loads((ROOT/'analysis/debug/procedures.json').read_text())
    aliases=json.loads((ROOT/'analysis/debug/aliases.json').read_text())
    mods=json.loads((ROOT/'analysis/debug/modules.json').read_text())
    def cv(i):return f'STARS_CV_{i:04x}'
    lines=['/* Generated; see analysis/debug/README.md. Storage views, not recovered original declarations. */',
           '#ifndef STARS_RECOVERED_TYPES_H','#define STARS_RECOVERED_TYPES_H','#include <stddef.h>',
           '#ifdef STARS_NATIVE16','#define STARS_NEAR __near','#define STARS_FAR __far','#define STARS_HUGE __huge',
           '#define STARS_CDECL __cdecl','#define STARS_PASCAL __pascal',
           '#else','#define STARS_NEAR','#define STARS_FAR','#define STARS_HUGE','#define STARS_CDECL','#define STARS_PASCAL','#endif',
           '#define STARS_CHECK(name,expr) typedef char stars_check_##name[(expr)?1:-1]',
           '#pragma pack(1)',
           'typedef struct { unsigned char bytes[10]; } STARS_REAL80; /* encoded x87 extended, not host arithmetic */']
    base={0:'void',1:'void',3:'void',17:'signed short',18:'STARS_LONG32',32:'unsigned char',33:'unsigned short',34:'STARS_ULONG32',64:'float',65:'double',66:'STARS_REAL80',112:'char',114:'signed short',115:'unsigned short'}
    lines+=['#ifdef STARS_NATIVE16','typedef signed long STARS_LONG32;','typedef unsigned long STARS_ULONG32;',
            '#else','typedef signed int STARS_LONG32;','typedef unsigned int STARS_ULONG32;','#endif']
    # Include pointee primitives even if they only occur through primitive pointers.
    pp={p['index'] for p in ps};pp|={i&255 for i in pp}
    for i in sorted(pp):
        if i<256:
            lines.append(f'typedef {base[i]} {cv(i)};')
            if i not in (0,1,3):
                size={17:2,18:4,32:1,33:2,34:4,64:4,65:8,66:10,112:1,114:2,115:2}[i]
                lines.append(f'STARS_CHECK(p{i:04x},sizeof({cv(i)})=={size});')
        else:
            mode=i>>8;sz=2 if mode==1 else 4;qual={1:'STARS_NEAR',2:'STARS_FAR',3:'STARS_HUGE'}[mode]
            lines+=['#ifdef STARS_NATIVE16',f'typedef {cv(i&255)} {qual} *{cv(i)};','#else',f'typedef {"unsigned short" if sz==2 else "STARS_ULONG32"} {cv(i)};','#endif',f'STARS_CHECK(p{i:04x},sizeof({cv(i)})=={sz});']
    for t in T:
        if t['leaf']=='0005':lines.append(f"typedef union {cv(t['index'])} {cv(t['index'])}; /* {t['name']} */")
    emitted=set();visiting=set();checks=[];offset_checks=[]
    def emit(i):
        if i<4096 or i in emitted:return
        t=ts[i];leaf=t['leaf'];name=cv(i)
        if leaf in ('0201','0204'):emitted.add(i);return
        if i in visiting:raise ValueError('By-value dependency cycle: '+hex(i))
        visiting.add(i)
        if leaf=='0005':
            for m in t['members']:
                if m['size_bytes']:emit(m['type_index'])
            lines.extend([f'/* {t["name"]}: {t["size"]} bytes; each view begins at its recorded offset. */',f'union {name} {{',f'    unsigned char raw[{t["size"]}];'])
            for j,m in enumerate(t['members']):
                mn=ident(m['name']);off=m['offset'];sz=m['size_bytes']
                prefix=f'unsigned char _pad[{off}]; ' if off else ''
                if sz:
                    lines.append(f'    struct {{ {prefix}{cv(m["type_index"])} value; }} {mn};')
                    offset_checks.append(f'STARS_OFFSET(t{i:04x}_m{j},{name},{mn}.value,{off})')
                else:lines.append(f'    /* {mn}: variable tail at offset {off}; no fixed storage. */')
            lines.append('};')
            for j,m in enumerate(t['members']):
                mn=ident(m['name']);lines.append(f'#define STARS_OFFSET_{i:04x}_{mn} {m["offset"]}')
                if 'bit_length'in m:
                    lines.extend([f'#define STARS_BITSHIFT_{i:04x}_{mn} {m["bit_position"]}',f'#define STARS_BITMASK_{i:04x}_{mn} 0x{((1<<m["bit_length"])-1):x}UL'])
        elif leaf=='0002':
            dep=t['underlying_type']
            if ts.get(dep,{}).get('leaf')!='0005':emit(dep)
            lines.extend(['#ifdef STARS_NATIVE16',f'typedef {cv(dep)} STARS_{t["pointer_kind"].upper()} *{name};', '#else',f'typedef {"unsigned short" if t["size_bytes"]==2 else "STARS_ULONG32"} {name};','#endif'])
        elif leaf=='0003':
            emit(t['element_type']);count=str(t['element_count']) if t['element_count'] else ''
            lines.append(f'typedef {cv(t["element_type"])} {name}[{count}];')
        elif leaf in ('0001','0206'):
            emit(t['underlying_type']);qual='volatile ' if t.get('volatile') else ''
            if t.get('const'):qual+='const '
            lines.append(f'typedef {qual}{cv(t["underlying_type"])} {name};'+(' /* bit-field backing storage; use recorded bit positions/masks */' if leaf=='0206' else ''))
        elif leaf=='0008':
            for dep in [t['return_type']]+t['argument_types']:
                if ts.get(dep,{}).get('leaf')!='0005':emit(dep)
            args=[cv(a) for a in t['argument_types'] if a]
            if t['variadic']:args.append('...')
            qual='STARS_FAR ' if t['calling_convention'] in (1,3) else 'STARS_NEAR '
            qual+='STARS_CDECL' if t['calling_convention']==1 else 'STARS_PASCAL'
            lines.append(f'typedef {cv(t["return_type"])} {qual} {name}({", ".join(args)});')
        else:raise ValueError(leaf)
        if t['size_bytes']:
            checks.append(f'STARS_CHECK(t{i:04x},sizeof({name})=={t["size_bytes"]});')
        visiting.remove(i);emitted.add(i)
    # nested function modifies the list, never rebind it
    for t in T:emit(t['index'])
    for a in aliases:
        lines.append(f'typedef {cv(a["type_index"])} STARS_ALIAS_{ident(a["name"])};')
    lines+=checks+['#pragma pack()',
        '/* VC8 rejects these offsetof expressions in file-scope array bounds.',
        ' * STARS_RUNTIME_OFFSETS requires a runner to include soffsets.inc.',
        ' * Size/primitive checks above remain compile-time assertions. */',
        '#ifndef STARS_RUNTIME_OFFSETS',
        '#define STARS_OFFSET(name,type,member,expected) STARS_CHECK(name,offsetof(type,member)==expected);',
        '#include "soffsets.inc"','#undef STARS_OFFSET','#endif','#endif','']
    out.mkdir(parents=True,exist_ok=True);(out/'stypes.h').write_text('\n'.join(lines))
    (out/'soffsets.inc').write_text('\n'.join([
        '/* Generated complete offset checklist; deliberately no include guard. */',
        '#ifndef STARS_OFFSET','#error Define STARS_OFFSET before including this checklist','#endif',
        *offset_checks,'']))
    proto=['/* Effective signatures; prefixed names avoid SDK collisions. Not implementations. */','#ifndef STARS_RECOVERED_FUNCTIONS_H','#define STARS_RECOVERED_FUNCTIONS_H','#include "stypes.h"']
    for f in funcs:
        t=ts[f['type_index']];module=ident(mods[str(f['module'])]['name'].split('.')[0]);params=[]
        for i,typ in enumerate(f['effective_parameter_types']):
            params.append(f'{cv(typ)} {ident(f["effective_parameter_names"][i])}')
        if t['variadic']:params.append('...')
        qual=('STARS_FAR ' if t['calling_convention'] in (1,3) else 'STARS_NEAR ')+('STARS_CDECL' if t['calling_convention']==1 else 'STARS_PASCAL')
        proto.append(f'/* {f["name"]} at {f["ne_segment"]:04x}:{f["ne_offset"]:04x}; {f["linkage"]} */')
        proto.append(f'{cv(t["return_type"])} {qual} stars_{module}_{ident(f["name"])}({", ".join(params) if params else "void"});')
    proto+=['#endif',''];(out/'sfuncs.h').write_text('\n'.join(proto))
    gl=['/* One declaration per original data record; aliases are reconciled in address_groups.json. */','#ifndef STARS_RECOVERED_GLOBALS_H','#define STARS_RECOVERED_GLOBALS_H','#include "stypes.h"']
    for r in S:
        if r['kind'] in ('S_LDATA16','S_GDATA16'):
            gl.append(f'/* {r["name"]}: {r["ne_segment"]:04x}:{r["ne_offset"]:04x}, {r["linkage"]} */')
            gl.append(f'extern {cv(r["type_index"])} stars_data_{r["directory_index"]}_{r["record_offset"]:x}_{ident(r["name"])};')
    gl+=['#endif',''];(out/'sglobals.h').write_text('\n'.join(gl))
    constants=['/* Absolute linker symbols; raw values, not loaded addresses or recovered source macros. */','#ifndef STARS_RECOVERED_CONSTANTS_H','#define STARS_RECOVERED_CONSTANTS_H']
    for r in S:
        if r.get('address_kind')=='absolute':
            constants.append(f'#define STARS_ABS_{ident(r["name"])} 0x{r["absolute_value"]:x}UL')
    constants+=['#endif',''];(out/'sconst.h').write_text('\n'.join(constants))
    (out/'checklay.c').write_text('#include "stypes.h"\n#include "sfuncs.h"\n#include "sglobals.h"\n#include "sconst.h"\nint main(void) { return 0; }\n')
    print('Generated headers:',len(checks)+len(offset_checks),'layout assertions,',len(funcs),'function declarations')

if __name__=='__main__':generate(ROOT/'reconstructed/include')
