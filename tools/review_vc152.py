#!/usr/bin/env python3
"""Record allowlisted VC152 return evidence and compare emitted CodeView/CRT bytes."""
import argparse
import contextlib
import hashlib
import io
import json
from pathlib import Path
import struct

from tools.extract_debug import extract

ROOT = Path(__file__).resolve().parents[1]


def read_json(path):
    return json.loads(path.read_text())


def sha(data):
    return hashlib.sha256(data).hexdigest()


def module_bytes(directory, blob, name):
    modules = read_json(directory / 'modules.json')
    maps = read_json(directory / 'segment_map.json')
    segments = read_json(directory / 'segments.json')
    module = next(m for m in modules.values() if m['name'].lower() == name)
    chunks = []
    for span in module['spans']:
        mapping = maps[span['segment'] - 1]
        start = (segments[mapping['frame'] - 1]['file_offset'] +
                 mapping['offset'] + span['offset'])
        chunk = blob[start:start + span['size']]
        assert len(chunk) == span['size']
        chunks.append(chunk)
    return b''.join(chunks)


def review(returned, package, scratch):
    evidence = dict(schema=1, returned=str(returned.relative_to(ROOT)), artifacts={}, logs={})
    # Do not ingest the FTP client's log or unrelated user/server configuration.
    log_names = ('output.txt', 'BLDSTAT.TXT', 'BUILD.LOG', 'LINK.LOG', 'LAYOUT.LOG',
                 'LAYLINK.LOG', 'RESULTS.TXT', 'LAYRES.TXT', 'BANNERS.TXT')
    generated = ('PROBE.EXE', 'PROBE.MAP', 'PROBE.OBJ', 'ABI.OBJ', 'UTILGEN.OBJ',
                 'VECTEST.OBJ', 'LAYMAIN.OBJ', 'LAYOUT.OBJ', 'LAYOUT.EXE',
                 'LAYOUT.MAP', 'msvc.pdb')
    for name in log_names + generated:
        path = returned / name
        if path.exists():
            data = path.read_bytes()
            evidence['artifacts'][name] = dict(bytes=len(data), sha256=sha(data))
            if name in log_names:
                evidence['logs'][name] = data.decode('cp437').replace('\r\n', '\n')
    evidence['package_inputs'] = []
    for source in sorted(package.iterdir()):
        if not source.is_file():
            continue
        destination = returned / source.name
        evidence['package_inputs'].append(dict(name=source.name, sha256=sha(source.read_bytes()),
            unchanged=destination.exists() and source.read_bytes() == destination.read_bytes()))
    executable = returned / 'PROBE.EXE'
    if not executable.exists():
        return evidence
    debug = scratch / 'debug'
    with contextlib.redirect_stdout(io.StringIO()):
        extract(executable, debug)
    blob = executable.read_bytes()
    original = (ROOT / 'target/stars.exe').read_bytes()
    if sha(original) != read_json(ROOT / 'analysis/summary.json')['sha256']:
        raise ValueError('Original executable no longer matches the retained debug inventory')
    evidence['original_sha256'] = sha(original)
    evidence['probe_debug_summary'] = read_json(debug / 'summary.json')
    evidence['probe_debug_summary']['input'] = str(executable.relative_to(ROOT))
    symbols = read_json(debug / 'symbols.json')
    modules = read_json(debug / 'modules.json')
    compilers = [r for r in symbols if r['kind'] == '0001']
    original_compilers = [r for r in read_json(ROOT / 'analysis/debug/symbols.json')
                          if r['kind'] == 'S_COMPILE']
    evidence['compiler_records'] = compilers
    evidence['compiler_record_bytes_match_all_original_game_objects'] = (
        bool(compilers) and {r['raw'] for r in compilers} == {r['raw'] for r in original_compilers})
    evidence['original_game_compiler_records'] = len(original_compilers)
    evidence['direct_object_modules'] = {i: m for i, m in modules.items() if m['library'] == 256}
    evidence['library_module_counts'] = {
        str(index): sum(m['library'] == index for m in modules.values())
        for index in sorted({m['library'] for m in modules.values()})}
    base = len(blob) - struct.unpack_from('<I', blob, len(blob) - 4)[0]
    for entry in read_json(debug / 'debug_directory.json'):
        if entry['kind'] == 0x128:
            raw = blob[base + entry['offset']:base + entry['offset'] + entry['size']]
            libraries, cursor = [], 0
            while cursor < len(raw):
                size = raw[cursor]
                libraries.append(raw[cursor + 1:cursor + 1 + size].decode('ascii'))
                cursor += 1 + size
            evidence['libraries'] = libraries
    ne = struct.unpack_from('<I', blob, 60)[0]
    evidence['ne_linker_version'] = f'{blob[ne + 2]}.{blob[ne + 3]}'
    evidence['abi_procedures'] = []
    types = {t['index']: t for t in read_json(debug / 'types.json')}
    for function in read_json(debug / 'functions.json'):
        if function['name'] in ('probe_cdecl', 'probe_pascal', 'probe_near', 'probe_near_wrapper'):
            raw = blob[function['file_offset']:function['file_offset'] + function['size']]
            evidence['abi_procedures'].append(dict(name=function['name'],
                ne_segment=function['ne_segment'], ne_offset=function['ne_offset'],
                file_offset=function['file_offset'], size=function['size'], raw=raw.hex(),
                type_record=types[function['type_index']]))
    evidence['arithmetic_runtime_fingerprints'] = []
    for name in ('lmul.asm', 'lshl.asm', 'uldiv.asm', 'ulrem.asm', 'ulshr.asm'):
        first = module_bytes(ROOT / 'analysis', original, name)
        second = module_bytes(debug, blob, name)
        evidence['arithmetic_runtime_fingerprints'].append(dict(module=name,
            original_bytes=len(first), probe_bytes=len(second), original_sha256=sha(first),
            probe_sha256=sha(second), exact_unmodified_byte_match=first == second))
    return evidence


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('returned', type=Path)
    parser.add_argument('--package', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    evidence = review(args.returned.resolve(), args.package.resolve(),
                      ROOT / 'build' / args.out.stem)
    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_text(json.dumps(evidence, indent=2) + '\n')
    print(f'Reviewed evidence: {args.out}')


if __name__ == '__main__':
    main()
