#!/usr/bin/env python3
"""Compare the procedures that lacked byte-match output before this batch."""
import argparse
import hashlib
import json
from pathlib import Path
import sys

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT))
from tools.compare_byte_match import object_module, digest, ORIGINAL_SHA
from tools.compare_tutorial_match import original_image
from tools.render_todo5 import byte_matches

def targets():
    existing = ROOT / 'analysis/compat/all-byte-match.json'
    if existing.exists():
        report = json.loads(existing.read_text())
        return [
            {
                'name': row['name'],
                'ne_segment': row['ne_segment'],
                'ne_offset': row['original_offset'],
                'size': row['original_bytes'],
            }
            for row in report['functions']
        ]
    procedures = {
        (p['ne_segment'], p['ne_offset']): p
        for p in json.loads((ROOT / 'analysis/debug/procedures.json').read_text())
    }
    missing = sorted(set(procedures) - set(byte_matches()))
    assert len(missing) == 625
    return [procedures[key] for key in missing]

def compare(directory):
    report_rows = []
    objects = {}
    sources = []
    image = original_image()
    for index, target in enumerate(targets()):
        name = target['name']
        source = ROOT / 'compat/all-byte-match' / f'F{index:03d}.C'
        blob = (directory / f'F{index:03d}.OBJ').read_bytes()
        module, backpatches = object_module(blob)
        wanted = name.upper().lstrip('_')
        public = next(
            p for p in module.publics
            if p['name'].upper().lstrip('_') == wanted
        )
        code = module.segments[public['segment']]
        generated = bytes(code.data[public['offset']:public['offset'] + target['size']])
        expected = bytes(
            image[target['ne_segment'] - 1]
            [target['ne_offset']:target['ne_offset'] + target['size']]
        )
        differences = [
            {'offset': offset, 'original': original, 'generated': actual}
            for offset, (original, actual) in enumerate(zip(expected, generated))
            if original != actual
        ]
        report_rows.append({
            'name': name,
            'ne_segment': target['ne_segment'],
            'original_offset': target['ne_offset'],
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
            'source': f'compat/all-byte-match/F{index:03d}.C',
            'source_sha256': digest(source.read_bytes()),
        })
        objects[name] = blob.hex()
        sources.append(source.read_bytes())
    return {
        'original_sha256': ORIGINAL_SHA,
        'compiler': 'Microsoft C/C++ Compiler Version 8.00c (VC++ 1.52c)',
        'flags': '/c /AM /GA /GEs /Zp1 /Od /Gs /W3 /Fc',
        'comparison': 'No masked bytes; compiler-emitted bytes were compared against the original function extents.',
        'functions': report_rows,
        'all_functions_match': all(row['exact_after_address_resolution'] for row in report_rows),
        'source_sha256': digest(b''.join(sources)),
        'object_sha256': {
            name: digest(bytes.fromhex(blob))
            for name, blob in objects.items()
        },
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
