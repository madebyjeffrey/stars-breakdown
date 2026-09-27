#!/usr/bin/env python3
"""Compare all 150 previously unmatched common-service procedures byte-for-byte."""
import argparse
import hashlib
import json
from pathlib import Path
import sys

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT))
from tools.compare_byte_match import object_module, digest, ORIGINAL_SHA
from tools.compare_tutorial_match import original_image

def targets():
    coverage = json.loads((ROOT / 'analysis/coverage.json').read_text())
    modules = {'memory.obj', 'utilgen.obj', 'util.obj', 'strings.obj'}
    rows = [
        row for row in coverage['ranges']
        if row.get('classification') == 'debug_procedure_range'
        and set(row.get('modules', [])) & modules
        and row['names'][0] != 'Random'
    ]
    assert len(rows) == 150
    return rows

def compare(directory):
    rows = []
    objects = {}
    sources = []
    image = original_image()
    for index, target in enumerate(targets()):
        name = target['names'][0]
        source = ROOT / 'compat/common-byte-match' / f'F{index:03d}.C'
        object_path = directory / f'F{index:03d}.OBJ'
        blob = object_path.read_bytes()
        module, backpatches = object_module(blob)
        wanted = name.upper()
        public = next(
            p for p in module.publics
            if p['name'].upper().lstrip('_') == wanted.lstrip('_')
        )
        code = module.segments[public['segment']]
        generated = bytes(code.data[public['offset']:public['offset'] + target['bytes']])
        expected = bytes(image[target['ne_segment'] - 1][target['start']:target['start'] + target['bytes']])
        differences = [
            {'offset': offset, 'original': original, 'generated': actual}
            for offset, (original, actual) in enumerate(zip(expected, generated))
            if original != actual
        ]
        rows.append({
            'name': name,
            'ne_segment': target['ne_segment'],
            'original_offset': target['start'],
            'original_bytes': len(expected),
            'generated_bytes': len(generated),
            'length_difference': len(generated) - len(expected),
            'exact_after_address_resolution': generated == expected,
            'differing_bytes': len(differences),
            'differences': differences,
            'fixups': [],
            'backpatch_records': len(backpatches),
            'original_relocated_sha256': digest(expected),
            'generated_resolved_sha256': digest(generated),
            'source': f'compat/common-byte-match/F{index:03d}.C',
            'source_sha256': digest(source.read_bytes()),
        })
        objects[name] = blob.hex()
        sources.append(source.read_bytes())
    return {
        'original_sha256': ORIGINAL_SHA,
        'compiler': 'Microsoft C/C++ Compiler Version 8.00c (VC++ 1.52c)',
        'flags': '/c /AM /GA /GEs /Zp1 /Od /Gs /W3 /Fc',
        'comparison': 'No masked bytes; compiler-emitted bytes were compared against the original function extents.',
        'functions': rows,
        'all_functions_match': all(row['exact_after_address_resolution'] for row in rows),
        'source_sha256': digest(b''.join(sources)),
        'object_sha256': {name: digest(bytes.fromhex(blob)) for name, blob in objects.items()},
        'objects': objects,
    }

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('directory', type=Path)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    report = compare(args.directory)
    report['build_log'] = (args.directory / 'MDI.LOG').read_text()
    report['build_status'] = (args.directory / 'DONE.TXT').read_text()
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    for row in report['functions']:
        status = 'MATCH' if row['exact_after_address_resolution'] else f"{row['differing_bytes']} differences"
        print(row['name'], row['generated_bytes'], 'bytes:', status)
    if not report['all_functions_match']:
        raise SystemExit(1)

if __name__ == '__main__':
    main()
