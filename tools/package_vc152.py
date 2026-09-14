#!/usr/bin/env python3
"""Export an allowlisted, DOS 8.3 / ASCII / CRLF VC++ 1.52c probe package."""
import argparse
from collections import defaultdict
import hashlib
import json
from pathlib import Path
import re
import zipfile

ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / 'compat/vc152'
FIXTURE_FILES = ('PROBE.H', 'PROBE.C', 'ABI.C', 'LAYOUT.C', 'PROBE.RSP',
                 'LAYOUT.RSP', 'LAYMAIN.RSP', 'LAYOUT.LNK', 'LAYOUT.DEF',
                 'PROBE.LNK', 'PROBE.DEF', 'BUILD.BAT', 'README.TXT')
RECOVERED_FILES = {
    'CORE.H': 'reconstructed/include/core.h',
    'UTILGEN.C': 'reconstructed/src/utilgen.c',
    'SEEDTAB.INC': 'reconstructed/src/seed_table.inc',
    **{name.upper(): f'reconstructed/include/{name}' for name in
       ('stypes.h', 'sfuncs.h', 'sglobals.h', 'sconst.h', 'soffsets.inc')},
}


def sha(data):
    return hashlib.sha256(data).hexdigest()


def dos_text(text):
    return text.replace('\r\n', '\n').replace('\r', '\n').replace('\n', '\r\n').encode('ascii')


def render_vectors(document):
    """Render frozen original-machine observations as ordinary C89 checks."""
    groups = defaultdict(list)
    for index, case in enumerate(document['cases']):
        groups[case['kind']].append((index, case))
    lines = ['/* Generated from vectors.json; expected values come from original x86. */',
             '#include "PROBE.H"']
    checks = 0

    def u32(value):
        return f'0x{value:08x}UL'

    def i16(value):
        return '(-32767 - 1)' if value == -32768 else str(value)

    for kind, cases in groups.items():
        lines += [f'\nstatic void test_{kind}(void)', '{']
        if kind in ('randomize', 'randomize2', 'random', 'next_file_xor',
                    'set_file_xor_stream', 'file_seeds'):
            lines.append('    StarsSeeds state;')
        if kind == 'file_seeds':
            lines.append('    StarsU32 first, second;')
        if kind in ('expand_rect', 'offset_rect'):
            lines.append('    StarsRect rect;')
        for index, case in cases:
            a = case['args']

            def check(label, expression, expected):
                nonlocal checks
                checks += 1
                lines.append(f'    check_u32("{kind}.{label}", {index}, {expression}, {u32(expected)});')

            lines.append(f"    /* case {index}: {case['original']} */")
            if kind in ('randomize', 'randomize2'):
                lines.append(f'    stars_{kind}(&state, {u32(a[0])});')
            elif kind in ('random', 'next_file_xor'):
                lines += [f'    state.first = {u32(a[0])};', f'    state.second = {u32(a[1])};']
                expr = (f'(StarsU16)stars_random(&state, {i16(a[2])})' if kind == 'random'
                        else 'stars_next_file_xor(&state)')
                check('result', expr, case['result'])
            elif kind == 'set_file_xor_stream':
                lines.append('    stars_set_file_xor_stream(&state, ' +
                             ', '.join([u32(a[0]), *map(i16, a[1:])]) + ');')
            elif kind == 'file_seeds':
                lines.append(f'    stars_set_file_seeds(&state, {u32(a[0])}, {u32(a[1])});')
                lines.append('    stars_get_file_seeds(&state, &first, &second);')
                check('get.first', 'first', case['outputs'][0])
                check('get.second', 'second', case['outputs'][1])
                lines.append('    stars_get_file_seeds(&state, &first, &first);')
                check('get.alias', 'first', case['alias'])
            elif kind == 'distance_squared':
                check('result', 'stars_distance_squared(' + ', '.join(map(i16, a)) + ')', case['result'])
            elif kind == 'compare_long':
                check('result', '(StarsU16)stars_compare_long(' + ', '.join(map(u32, a)) + ')', case['result'])
            elif kind in ('expand_rect', 'offset_rect'):
                for field, value in zip(('left', 'top', 'right', 'bottom'), a[:4]):
                    lines.append(f'    rect.{field} = {value}U;')
                lines.append(f'    stars_{kind}(&rect, {i16(a[4])}, {i16(a[5])});')
                for field, value in zip(('left', 'top', 'right', 'bottom'), case['rect']):
                    check(field, f'rect.{field}', value)
            else:
                raise ValueError(f'Unrecognized vector kind: {kind}')
            if 'state' in case:
                check('first', 'state.first', case['state'][0])
                check('second', 'state.second', case['state'][1])
        lines.append('}')
    lines += ['\nvoid run_vectors(void)', '{']
    lines += [f'    test_{kind}();' for kind in groups]
    lines += ['}', '']
    return '\n'.join(lines), checks


def payload():
    files, manifest = {}, ['STARS VC152 PACKAGE 2',
                           'ASCII; CRLF; DOS 8.3 names. Source hashes are SHA-256.',
                           'Only quoted local include filenames are renamed; C logic is unchanged.', '']
    inputs = {name: SOURCE / name for name in FIXTURE_FILES}
    inputs.update({name: ROOT / path for name, path in RECOVERED_FILES.items()})
    include_names = {Path(path).name: name for name, path in RECOVERED_FILES.items()}
    for name, path in inputs.items():
        original = path.read_bytes()
        text = original.decode('ascii')
        text = re.sub(r'(#include\s+")([^"]+)(")',
                      lambda m: m[1] + include_names.get(m[2], m[2]) + m[3], text)
        files[name] = dos_text(text)
        manifest += [f'{name} <- {path.relative_to(ROOT)}', f'  SOURCE {sha(original)}',
                     f'  PACKAGE {sha(files[name])}']
    vector_bytes = (SOURCE / 'vectors.json').read_bytes()
    document = json.loads(vector_bytes)
    source, checks = render_vectors(document)
    files['VECTEST.C'] = dos_text(source)
    offset_count = files['SOFFSETS.INC'].count(b'\r\nSTARS_OFFSET(')
    manifest += ['', f'ORIGINAL SHA256 {document["original_sha256"]}',
                 f'VECTORS SHA256 {sha(vector_bytes)}',
                 f'VECTORS {len(document["cases"])} cases; {checks} value/state assertions',
                 f'HOST_RUNTIME_CHECKS {checks + 264}',
                 f'WIN16_RUNTIME_CHECKS {checks + 275}',
                 f'LAYOUT_OFFSET_CHECKS {offset_count}',
                 f'HOST_LAYOUT_CHECKS {offset_count + 11}',
                 f'WIN16_LAYOUT_CHECKS {offset_count + 22}',
                 'VECTEST.C <- generated by tools/package_vc152.py from compat/vc152/vectors.json',
                 f'  PACKAGE {sha(files["VECTEST.C"])}',
                 'NATIVE_BUILD_STATUS package 1 utility passed; package 2 and native layouts await VM results.', '']
    files['MANIFEST.TXT'] = dos_text('\n'.join(manifest))
    for name in files:
        if not re.fullmatch(r'[A-Z0-9_]{1,8}\.[A-Z0-9_]{1,3}', name):
            raise ValueError(f'Not a DOS 8.3 filename: {name}')
    return files


def export(output):
    files = payload()
    archive = output.with_suffix('.ZIP')
    if output.exists() or archive.exists():
        raise FileExistsError(f'Use a new output directory to preserve prior builds: {output}')
    output.mkdir(parents=True)
    for name, data in files.items():
        (output / name).write_bytes(data)
    # A stable flat ZIP, without Mac metadata or host path names.
    with zipfile.ZipFile(archive, 'x', compression=zipfile.ZIP_DEFLATED) as package:
        for name, data in sorted(files.items()):
            info = zipfile.ZipInfo(name, date_time=(2026, 9, 14, 0, 0, 0))
            info.compress_type = zipfile.ZIP_DEFLATED
            info.create_system = 0
            info.external_attr = 0x20
            package.writestr(info, data)
    return archive


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path, default=ROOT / 'ftp-share/VC152B')
    args = parser.parse_args()
    output = args.output.resolve()
    archive = export(output)
    print(f'Folder: {output}\nArchive: {archive}\nNative build awaits the user VM.')


if __name__ == '__main__':
    main()
