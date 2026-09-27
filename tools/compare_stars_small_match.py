#!/usr/bin/env python3
import argparse
import json
from pathlib import Path
import sys

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT))
from tools.compare_byte_match import object_module, digest, ORIGINAL_SHA
from tools.compare_startup_match import symbol_values
from tools.compare_tutorial_match import original_image

SOURCE = ROOT / 'compat/stars-small-match/STARS.C'
TARGET = dict(name='FHandleChar', ne_segment=4, ne_offset=0x15de, size=124)

def compare(blob):
    module, backpatches = object_module(blob)
    public = next((p for p in module.publics
                   if p['name'].upper().lstrip('_') == TARGET['name'].upper()), None)
    if public is None:
        raise ValueError('Missing FHandleChar public')
    code = module.segments[public['segment']]
    start = public['offset']
    generated = bytearray(code.data[start:start + TARGET['size']])
    expected = bytes(original_image()[TARGET['ne_segment'] - 1]
                     [TARGET['ne_offset']:TARGET['ne_offset'] + TARGET['size']])
    symbols = {}
    for name, candidates in symbol_values().items():
        for value in candidates:
            symbols.setdefault(name.upper(), set()).add(value)
            symbols.setdefault(name.lstrip('_').upper(), set()).add(value)
    resolutions = []
    for fixup in code.fixups:
        if not start <= fixup['offset'] < start + TARGET['size']:
            continue
        if fixup['relative'] or fixup['frame'] != (5, None):
            raise ValueError('Unsupported fixup: ' + str(fixup))
        symbol = module.externals[fixup['target'][1]]
        targets = symbols.get(symbol.upper(), set())
        if not targets:
            targets = symbols.get('_' + symbol.lstrip('_').upper(), set())
        if len(targets) != 1:
            raise ValueError('Unknown or ambiguous external ' + symbol)
        target = next(iter(targets)) + fixup['displacement']
        if fixup['location'] in (1, 5): value = target & 0xffff
        elif fixup['location'] == 2: value = (target >> 16) & 0xffff
        elif fixup['location'] == 3: value = target
        else: raise ValueError('Unsupported fixup location ' + str(fixup))
        local = fixup['offset'] - start
        old = int.from_bytes(generated[local:local + fixup['width']], 'little')
        value = (value + old) & ((1 << (8 * fixup['width'])) - 1)
        generated[local:local + fixup['width']] = value.to_bytes(fixup['width'], 'little')
        resolutions.append(dict(offset=local, width=fixup['width'], target=symbol,
                                resolved_value=hex(value)))
    differences = [dict(offset=i, original=a, generated=b)
                   for i, (a, b) in enumerate(zip(expected, generated)) if a != b]
    row = dict(name=TARGET['name'], ne_segment=TARGET['ne_segment'],
               original_offset=TARGET['ne_offset'], original_bytes=TARGET['size'],
               generated_bytes=len(generated),
               length_difference=len(generated) - len(expected),
               exact_after_address_resolution=bytes(generated) == expected,
               differing_bytes=len(differences), differences=differences,
               fixups=resolutions, backpatch_records=len(backpatches),
               original_relocated_sha256=digest(expected),
               generated_resolved_sha256=digest(bytes(generated)),
               source='compat/stars-small-match/STARS.C')
    return dict(original_sha256=ORIGINAL_SHA,
                compiler='Microsoft C/C++ Compiler Version 8.00c (VC++ 1.52c)',
                flags='/c /AM /GA /GEs /Zp1 /Od /Gs /W3 /Fc',
                comparison='No masked bytes; named OMF relocations resolved into the original address layout.',
                functions=[row], all_functions_match=row['exact_after_address_resolution'],
                object_sha256=digest(blob), source_sha256=digest(SOURCE.read_bytes()))

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('directory', type=Path)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--fixture', type=Path)
    args = parser.parse_args()
    blob = (args.directory / 'STARS.OBJ').read_bytes()
    result = compare(blob)
    result['build_log'] = (args.directory / 'STARS.LOG').read_text()
    result['build_status'] = (args.directory / 'DONE.TXT').read_text()
    args.output.write_text(json.dumps(result, indent=2) + '\n')
    if args.fixture:
        args.fixture.write_text(json.dumps(dict(original_sha256=ORIGINAL_SHA,
            object_hex=blob.hex(), listing=(args.directory / 'STARS.COD').read_text()),
            indent=2) + '\n')
    row = result['functions'][0]
    print(row['name'], row['generated_bytes'], 'bytes:',
          'MATCH' if row['exact_after_address_resolution'] else str(row['differing_bytes']) + ' differences')
    if not result['all_functions_match']:
        raise SystemExit(1)

if __name__ == '__main__':
    main()
