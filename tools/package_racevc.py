#!/usr/bin/env python3
"""Export a VC152 Win16 compiler/output test; deterministic UI, real file I/O."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import struct
import sys
import zipfile

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT))
from tools.build_core import SOURCES
from tools.package_vc152 import dos_text, sha
from tools.race_scenarios import PAGES, catalog, select

SOURCE = ROOT / 'compat/racevc'
RENAMES = {
    'near_state.c': 'NEARSTAT.C', 'racewizard.c': 'RACEWIZ.C',
    'runtime_internal.h': 'RTINT.H', 'ui_internal.h': 'UIINT.H',
    'seed_table.inc': 'SEEDTAB.INC', 'battle_plan_templates.inc': 'BPLANS.INC',
    'innate_fp.h': 'INNATE.H',
}
FIXTURES = ('TEST.H', 'RUNNER.C', 'WORLD.C', 'FILES.C', 'BUILD.BAT', 'RACEVC.DEF', 'README.TXT')
EXTRAS = ('runtime_internal.h', 'ui_internal.h', 'innate_fp.h',
          'seed_table.inc', 'battle_plan_templates.inc')


def name_for(name):
    return RENAMES.get(name, name.upper())


def fixed(text, count):
    data = text.encode('ascii')
    if len(data) >= count:
        raise ValueError(f'Text exceeds {count-1} bytes: {text!r}')
    return data.ljust(count, b'\0')


def words(*values):
    return struct.pack('<'+'H'*len(values), *(v & 65535 for v in values))


def message(msg, wp=0, lp=0):
    return words(0, msg, wp, lp & 65535, lp >> 16, 0)+bytes(32)


def action(op, page):
    fields = [0]*6
    value = ''
    kind = op['op']
    if kind == 'text':
        fields[:2] = [1, op['control']]; value = op['value']
    elif kind == 'select':
        fields[:4] = [2, op['control'], op['first'], op['last']]
    elif kind in ['check', 'combo']:
        fields[:3] = [3 if kind == 'check' else 4, op['control'], op['value']]
    elif kind == 'mouse':
        fields[:5] = [5, page, op['rect'], int(op['shift']),
                      {None: 0, 'left': 1, 'right': 2}[op['drag']]]
    else:
        raise ValueError(kind)
    return (words(*fields)+fixed(value, 32))*op.get('count', 1)


def scripted_dialogs(case, shorten):
    """Same input-only sequence as package_race_acceptance.scripts.

    The complete generated runner is checked against the frozen original results
    to catch divergence; neither this exporter nor its adapter computes a race.
    """
    result = []

    def visit(page, ops, command):
        data = message(0x110)+message(0xf)
        data += b''.join(action(op, page) for op in ops)
        data += message(0xf)+message(0x111, command)
        result.append((PAGES[page-1], data))

    def text(control, value):
        return dict(op='text', control=control, value=value)

    first = [select(0x116 if shorten else 0x10f, 0x10f, 0x116)]
    if not shorten and case['template']:
        first.append(select(0x10f+case['template'], 0x10f, 0x116))
    singular, plural = case['singular'], case['plural']
    if case['flow'] == 'name_tail':
        singular, plural = ('X', 'Xs') if shorten else ('LongRaceName', 'LongRaceNames')
    first += [text(0x10c, singular), text(0x81b, plural), text(0x10d, case['password'])]
    for page in range(1, case['finish_page']+1):
        if page == 2 and case['flow'] == 'back':
            visit(2, [], 0x42e); visit(1, [], 0x42f)
        command = 0x42f if page < case['finish_page'] else 2 if case['flow'] == 'cancel' else 0x430
        visit(page, (first if page == 1 else [])+case['changes'].get(page, []), command)
    if case['password']:
        data = message(0x110)+action(text(0x10c, 'wrong'), 0)+message(0x111, 1)
        data += action(text(0x10c, case['password']), 0)+message(0x111, 1)
        result.append((0x8c, data))
    visit(case['finish_page'], [], 2)
    assert len(result) <= 16 and sum(len(data)//44 for _, data in result) <= 256
    return words(len(result))+b''.join(words(res, len(data)//44)+data for res, data in result)


def scenario_data(document):
    definitions = {case['id']: case for case in catalog()}
    output = bytearray(b'RACEVC1\0'+words(sum(len(row['sessions']) for row in document['cases'])))
    files = {}
    for row in document['cases']:
        case = definitions.pop(row['id'])
        for index, session in enumerate(row['sessions']):
            output.extend(fixed(case['id'], 5)+fixed(session['filename'], 13))
            output.extend(words(not index, session['result'], session['advantage_points'],
                                -1 if session['load_result'] is None else session['load_result'],
                                len(session['alerts'])))
            player = bytes.fromhex(session['player'])
            assert len(player) == 192
            output.extend(player)
            output.extend(scripted_dialogs(case, bool(index)))
            if session['wire'] is not None:
                files['EXPECT/'+session['filename']] = bytes.fromhex(session['wire'])
    assert not definitions
    return bytes(output), files


def static_images():
    # This constructs the already validated oracle's initial address space;
    # it does not execute game instructions or inspect any live installation.
    from tests.test_race_ui import RaceUiTests
    from tools.package_race_acceptance import initial
    from tools.ne_layout import NE
    t = RaceUiTests(); initial(t)
    result = {'INDATA.BIN': t.m.get(0, 65536),
              'STRINGS.BIN': bytes(t.m.cpu.mem_read(0x90000, 65536)),
              'PLANETS.BIN': bytes(t.m.cpu.mem_read(0x10000, 65536))}
    layout = NE(ROOT / 'target/stars.exe').report()
    resources = []
    for res in layout['resources']:
        if res['type'] != 5 or res['id'] not in PAGES+[0x8c]:
            continue
        dialog = res['decoded']
        controls = bytearray()
        for c in dialog['controls']:
            x, y, w, h = c['rect']
            controls.extend(words(c['id'], x*2, y*2, (x+w)*2, (y+h)*2)+fixed(c['text'], 64))
        resources.append(words(res['id'], dialog['rect'][2]*2, dialog['rect'][3]*2,
                               len(dialog['controls']))+controls)
    assert len(resources) == 7
    result['RSDATA.DAT'] = words(7)+b''.join(resources)
    return result


def payload():
    vector_bytes = (SOURCE / 'vectors.json').read_bytes()
    document = json.loads(vector_bytes)
    assert sha((ROOT / 'target/stars.exe').read_bytes()) == document['original_sha256']
    files = {}; inputs = {}
    sources = {name: SOURCE / name for name in FIXTURES}
    sources.update({name_for(n): ROOT / 'reconstructed/src' / n for n in SOURCES+list(EXTRAS)})
    sources.update({name_for(n): ROOT / 'reconstructed/include' / n for n in ('core.h', 'runtime.h', 'raceui.h')})
    for name, source in sources.items():
        original = source.read_bytes()
        code = re.sub(r'(#include\s+")([^"]+)(")',
                      lambda m: m[1]+name_for(m[2])+m[3], original.decode('ascii'))
        files[name] = dos_text(code)
        inputs[str(source.relative_to(ROOT))] = sha(original)
    files.update(static_images())
    files['CASES.DAT'], expectations = scenario_data(document)
    files.update(expectations)
    files['ACTUAL/README.TXT'] = dos_text('The test program writes newly generated race files here.\n')
    c_sources = ['RUNNER.C', 'WORLD.C', 'FILES.C']+[name_for(n) for n in SOURCES]
    files['RACEVC.RSP'] = dos_text('/c /AL /GA /GEs /Zp1 /Od /Zi /W3\n'+'\n'.join(c_sources)+'\n')
    # Five LINK fields; continuations keep each physical line short.
    files['RACEVC.LNK'] = dos_text('+\n'.join(Path(n).stem+'.OBJ' for n in c_sources)+
        '\nRACEVC.EXE\nRACEVC.MAP\nLLIBCEW.LIB LIBW.LIB /NOD /ALIGN:16 /CO /MAP\nRACEVC.DEF\n')
    manifest = dict(schema=1, package_revision=2,
                    kind='VC152 compiler/output acceptance; UI services modeled',
                    original_sha256=document['original_sha256'], vectors_sha256=sha(vector_bytes),
                    cases=88, sessions=89, expected_files=len(expectations), native_status='pending user build/run',
                    source_hashes=inputs, files={n: sha(b) for n, b in sorted(files.items())})
    files['MANIFEST.JSN'] = dos_text(json.dumps(manifest, indent=2)+'\n')
    for name in files:
        if not all(re.fullmatch(r'[A-Z0-9_]{1,8}(\.[A-Z0-9_]{1,3})?', part) for part in name.split('/')):
            raise ValueError(f'Non-8.3 filename: {name}')
    return files


def export(output):
    archive = output.with_suffix('.ZIP')
    if output.exists() or archive.exists():
        raise FileExistsError(f'Preserve prior returns; use a fresh output directory: {output}')
    files = payload()
    output.mkdir(parents=True)
    for name, data in files.items():
        path = output / name; path.parent.mkdir(parents=True, exist_ok=True); path.write_bytes(data)
    with zipfile.ZipFile(archive, 'x', compression=zipfile.ZIP_DEFLATED) as z:
        for name, data in sorted(files.items()):
            info = zipfile.ZipInfo(name, date_time=(2026, 9, 14, 0, 0, 0))
            info.compress_type = zipfile.ZIP_DEFLATED; info.create_system = 0; info.external_attr = 0x20
            z.writestr(info, data)
    return archive


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path, default=ROOT / 'ftp-share/RACEVC2')
    args = parser.parse_args()
    print(export(args.output.resolve()))


if __name__ == '__main__':
    main()
