"""Compare every original CRT code span with the locally installed VC library.

Literal bytes and each linker field are treated separately. Private data bases
inferred from linked references are labeled; that is relocation consistency,
not an independent recovery of the original linker command or every data byte.
"""
import argparse
import hashlib
import json
from pathlib import Path
from tools.omf import library
from tools.ne_layout import NE

ROOT = Path(__file__).resolve().parents[1]


def sha(data):
    return hashlib.sha256(data).hexdigest()


def canonical(name):
    return name.lower().removeprefix('.\\')


def review(directory):
    libpath, imppath = directory / 'MLIBCEW.LIB', directory / 'LIBW.LIB'
    blob, imports_blob = libpath.read_bytes(), imppath.read_bytes()
    members, import_members = library(blob), library(imports_blob)
    original = (ROOT / 'target/stars.exe').read_bytes()
    expected = json.loads((ROOT / 'analysis/summary.json').read_text())['sha256']
    if sha(original) != expected:
        raise ValueError('Original executable identity changed')
    ne = NE(ROOT / 'target/stars.exe')
    modules = json.loads((ROOT / 'analysis/modules.json').read_text())
    maps = json.loads((ROOT / 'analysis/segment_map.json').read_text())
    symbols = json.loads((ROOT / 'analysis/debug/symbols.json').read_text())
    publics, absolutes = {}, {}
    for s in symbols:
        if not s['kind'].startswith('S_PUB'):
            continue
        if s.get('ne_segment'):
            publics[s['name']] = (s['ne_segment'], s['ne_offset'])
        elif s.get('address_kind') == 'absolute':
            absolutes[s['name']] = s['offset']
    imports = {name: target for m in import_members for name, target in m.imports.items()}
    relocations = {(r['segment'], site): r for r in ne.relocations for site in r['sites']}
    selected, rows = [], []
    for number, module in modules.items():
        if module['library'] != 1:
            continue
        row = dict(module=int(number), name=module['name'], code_bytes=sum(s['size'] for s in module['spans']))
        rows.append(row)
        candidates = [m for m in members if canonical(m.name) == canonical(module['name']) and not m.unsupported]
        if not row['code_bytes']:
            row.update(status='no_code_span', matching_member_offsets=[m.offset for m in candidates])
            continue
        if len(module['spans']) != 1:
            raise ValueError('Audit needs an explicit multi-span mapping')
        span = module['spans'][0]
        mapping = maps[span['segment'] - 1]
        ns, start = mapping['frame'], mapping['offset'] + span['offset']
        code = bytes(ne.raw[ns - 1][start:start + span['size']])
        matches = []
        for m in candidates:
            for si, segment in enumerate(m.segments[1:], 1):
                if segment.class_name != 'CODE' or segment.size != len(code):
                    continue
                if not all(segment.initialized):
                    continue
                mask = {i for f in segment.fixups for i in range(f['offset'], f['offset'] + f['width'])}
                if all(a == b or i in mask for i, (a, b) in enumerate(zip(code, segment.data))):
                    public_matches = sum(p['name'] in publics and publics[p['name']] == (ns, start + p['offset'])
                                         for p in m.publics if p['segment'] == si)
                    if any(p['name'] in publics and publics[p['name']] != (ns, start + p['offset'])
                           for p in m.publics if p['segment'] == si):
                        continue
                    matches.append((m, si, segment, mask, public_matches))
        if matches:
            strongest = max(match[4] for match in matches)
            matches = [match for match in matches if match[4] == strongest]
        if len(matches) != 1:
            row.update(status='unresolved_code_match', candidate_count=len(matches))
            continue
        m, si, segment, mask, public_matches = matches[0]
        anchors = {si: (ns, start)}
        origins = {si: 'original_CodeView_module_span'}
        for p in m.publics:
            if p['name'] in publics and p['segment']:
                frame, off = publics[p['name']]
                anchor = (frame, off - p['offset'])
                if p['segment'] in anchors and anchors[p['segment']] != anchor:
                    raise ValueError(('Conflicting public anchors', m.name, p['name']))
                anchors[p['segment']] = anchor
                origins[p['segment']] = 'original_public_symbol'
        row.update(ne_segment=ns, ne_offset=start, member_offset=m.offset,
                   member_sha256=sha(blob[m.offset:m.end]), code_sha256=sha(code),
                   literal_bytes=len(code) - len(mask), fixup_bytes=len(mask),
                   public_identity_anchors=public_matches, code_overwrites=segment.overwrites)
        selected.append((row, m, si, segment, anchors, origins))

    # CodeView spans independently anchor even CRT exports omitted from the
    # linked public table (e.g. local conversion helpers in cvt.c).
    for row, m, si, segment, anchors, origins in selected:
        for p in m.publics:
            if p['segment'] not in anchors:
                continue
            ns, base = anchors[p['segment']]
            value = ns, base + p['offset']
            if p['name'] in publics and publics[p['name']] != value:
                raise ValueError(('Conflicting CRT export', p['name']))
            publics[p['name']] = value
    aliases = {a: t for m in members for a, t in m.aliases.items()}

    for row, m, si, segment, anchors, origins in selected:
        ns, start = row['ne_segment'], row['ne_offset']
        code = bytes(ne.raw[ns - 1][start:start + segment.size])
        # Infer private segment placement from ordinary linked near references.
        # Repeated references must agree; initialized data is checked below.
        for f in segment.fixups:
            kind, index = f['target']
            if kind != 0 or index in anchors or f['location'] != 1 or f['relative']:
                continue
            target = m.segments[index]
            if target.class_name == 'CODE':
                continue
            off = f['offset']
            base = (int.from_bytes(code[off:off + 2], 'little') - f['displacement'] -
                    int.from_bytes(segment.data[off:off + 2], 'little')) & 65535
            anchors[index] = (37, base)
            origins[index] = 'inferred_from_linked_private_data_reference'

        checks = []
        for f in segment.fixups:
            off, width = f['offset'], f['width']
            site = start + off
            check = dict(offset=off, location=f['location'], relative=f['relative'], target_method=f['target'][0])
            checks.append(check)
            kind, index = f['target']
            address = absolute = imported = name = None
            if kind == 2:
                name = m.externals[index]
                check['symbol'] = name
                seen = set()
                while name not in publics and name not in absolutes and name not in imports:
                    if name in seen:
                        break
                    seen.add(name)
                    replacement = aliases.get(name, m.weak.get(name))
                    if replacement is None:
                        break
                    name = replacement
                if seen and name != check['symbol']:
                    check['resolved_symbol'] = name
                address = publics.get(name)
                absolute = absolutes.get(name)
                imported = imports.get(name)
            elif kind == 0 and index in anchors:
                address = anchors[index]
                check['object_segment'] = index
                check['anchor_method'] = origins[index]
            if imported:
                check['import'] = imported
                relocation = relocations.get((ns, site))
                expected_kind = {1: 5, 3: 3}.get(f['location'])
                zero_addend = not f['displacement'] and not any(segment.data[off:off + width])
                check['status'] = ('verified_import' if relocation and relocation.get('import_key') == imported
                                   and relocation['address_kind'] == expected_kind and zero_addend
                                   and not f['relative'] else 'unresolved_import')
                continue
            if name and name.startswith(('FI', 'FJ')) and name.endswith('RQQ'):
                # Microsoft LINK recognizes these absolute floating fixups and
                # emits NE OS fixups while retaining the hardware instructions.
                second = name.startswith('FJ')
                relocation = relocations.get((ns, site - int(second)))
                expected_kind = {'FIDRQQ':5, 'FIWRQQ':6, 'FIERQQ':4, 'FICRQQ':3,
                                 'FJCRQQ':3, 'FISRQQ':2, 'FJSRQQ':2}.get(name)
                same = code[off:off + width] == segment.data[off:off + width]
                check['status'] = ('verified_fpu_literal_and_os_fixup' if same and relocation and
                    relocation['target_kind'] == 'os_fpu_fixup' and relocation['target1'] == expected_kind
                    else 'unresolved_fpu_fixup')
                continue
            if address is None and absolute is None:
                check['status'] = 'unresolved_target'
                continue
            value = (address[1] if address else absolute) + f['displacement']
            addend = int.from_bytes(segment.data[off:off + (2 if f['location'] == 3 else width)], 'little')
            value += addend
            relocation = relocations.get((ns, site))
            if f['location'] == 3 and relocation is None:
                relocation = relocations.get((ns, site + 2))
            if relocation and relocation['target_kind'] == 'internal':
                actual = (int.from_bytes(code[off:off + 2], 'little') if relocation['address_kind'] == 2
                          else relocation['target_offset'])
                if relocation['additive'] and relocation['address_kind'] != 2:
                    actual += int.from_bytes(code[off:off + 2], 'little')
                good = address is not None and relocation['target_segment'] == address[0] and actual & 65535 == value & 65535
                check['status'] = 'verified_internal_relocation' if good else 'unresolved_internal_relocation'
            elif f['location'] == 1:
                if f['relative']:
                    value -= site + width
                good = int.from_bytes(code[off:off + width], 'little') == value & 65535
                check['status'] = 'verified_linked_word' if good else 'unresolved_linked_word'
                if not good:
                    check.update(expected=value & 65535, actual=int.from_bytes(code[off:off + width], 'little'))
            else:
                check['status'] = 'unresolved_encoding'

        data_checks = []
        for index, (frame, base) in sorted(anchors.items()):
            s = m.segments[index]
            if s.class_name == 'CODE':
                continue
            mask = {i for f in s.fixups for i in range(f['offset'], f['offset'] + f['width'])}
            literal = [i for i in range(s.size) if s.initialized[i] and i not in mask]
            raw = ne.raw[frame - 1]
            differences = [i for i in literal if base + i >= len(raw) or raw[base + i] != s.data[i]]
            data_checks.append(dict(object_segment=index, name=s.name, ne_segment=frame, base=base,
                anchor_method=origins[index], size=s.size, literal_bytes=len(literal),
                data_fixup_bytes=len(mask), mismatches=differences))
        row['fixups'] = checks
        row['data_anchors'] = data_checks
        row['status'] = ('relocation_consistent_code_match' if all(c['status'].startswith('verified_') for c in checks)
                         else 'literal_match_pending_fixups')
        if any(d['mismatches'] for d in data_checks):
            row['status'] = 'code_match_with_data_difference'
    return dict(schema=1, original_sha256=expected,
        library=dict(path=str(libpath.relative_to(ROOT)), sha256=sha(blob), members=len(members)),
        import_library=dict(path=str(imppath.relative_to(ROOT)), sha256=sha(imports_blob)),
        scope='Original CodeView CRT code spans. Data anchors/literals are supporting evidence; data fixups, zero-size records and whole-program link layout remain separately scoped.',
        summary={status: sum(r['status'] == status for r in rows) for status in sorted({r['status'] for r in rows})},
        modules=rows)


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--library-directory', type=Path, default=ROOT / 'runs/step4/guest/MSVC/LIB')
    ap.add_argument('--output', type=Path, default=ROOT / 'analysis/reconstruction/runtime-library.json')
    ap.add_argument('--check', action='store_true')
    args = ap.parse_args()
    report = review(args.library_directory)
    content = json.dumps(report, indent=2) + '\n'
    if args.check:
        if args.output.read_text() != content:
            raise SystemExit('Runtime audit differs')
    else:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(content)
    print(json.dumps(report['summary'], sort_keys=True))


if __name__ == '__main__':
    main()
