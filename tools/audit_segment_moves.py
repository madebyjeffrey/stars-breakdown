#!/usr/bin/env python3
"""Retain a byte-derived correction overlay for Ghidra segment-move text.

The pinned SLEIGH semantics assign the right direction; only display operands
are reversed for register-form opcode 8E. Raw exports remain unmodified.
"""
import hashlib
import json
from pathlib import Path
from tools.ne_layout import NE

ROOT = Path(__file__).resolve().parents[1]


def segment_move(raw):
    at, wide = 0, False
    while at < len(raw) and raw[at] in [0x26,0x2e,0x36,0x3e,0x64,0x65,0x66,0x67,0xf0,0xf2,0xf3]:
        wide |= raw[at] == 0x66
        at += 1
    if len(raw) != at + 2 or raw[at] not in (0x8c, 0x8e): return None
    modrm = raw[at + 1]
    if modrm >> 6 != 3 or ((modrm >> 3) & 7) >= 6: return None
    general = ['AX','CX','DX','BX','SP','BP','SI','DI'][modrm & 7]
    if wide: general = 'E' + general
    segment = ['ES','CS','SS','DS','FS','GS'][(modrm >> 3) & 7]
    return 'MOV ' + (segment + ',' + general if raw[at] == 0x8e else general + ',' + segment)


def audit():
    source = ROOT / 'analysis/decompiler/export/units.json'
    ne = NE(ROOT / 'target/stars.exe')
    rows, inspected = [], 0
    for unit in json.loads(source.read_text()):
        if unit['kind'] != 'instruction': continue
        raw = bytes(ne.raw[unit['segment'] - 1][unit['start']:unit['start'] + unit['size']])
        expected = segment_move(raw)
        if expected is None: continue
        inspected += 1
        if expected != unit['text']:
            rows.append(dict(segment=unit['segment'], offset=unit['start'], bytes=raw.hex(),
                             exported_text=unit['text'], corrected_text=expected))
    return dict(scope='Register-form segment MOV display correction; not a p-code patch',
                source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),
                inspected=inspected, corrections=rows)


if __name__ == '__main__':
    result = audit()
    (ROOT / 'analysis/reconstruction/segment-moves.json').write_text(json.dumps(result, indent=2) + '\n')
    print(f"Audited {result['inspected']} moves; {len(result['corrections'])} display corrections")
