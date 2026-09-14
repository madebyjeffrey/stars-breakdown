#!/usr/bin/env python3
"""Generate checked wizard scenarios and an ASCII/8.3 live acceptance package.

This uses the existing differential-test service adapters, not native Windows.
Run --review on returned live fixtures. Native VC++ UI packaging remains pending.
"""
import argparse
import ctypes as C
import hashlib
import json
from pathlib import Path
import struct
import sys
import zipfile

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT))
from tools.race_file_compare import Inspector, ORIGINAL_SHA256, compare, player_fields
from tools.race_scenarios import PAGES, TEMPLATES, catalog, select, action_words


def compile_action(action, page):
    def apply(world, put):
        op = action['op']
        if op == 'text':
            value = action['value'].encode('ascii')
            limit = world.limits.get(action['control'], 32767)
            if len(value) > limit:
                raise ValueError('Scenario exceeds the actual edit-control limit')
            world.text[action['control']] = value
            return None
        if op == 'select':
            for id in range(action['first'], action['last']+1):
                world.checks[id] = int(id == action['control'])
        elif op in ['check', 'combo']:
            world.checks[action['control']] = action['value']
            if op == 'combo':
                return None  # CB_GETCURSEL is read at page exit.
        elif op == 'mouse':
            base = 0x5926 if page == 1 else struct.unpack('<H', world.read_near(0x48ac, 2))[0]
            l, top, right, bottom = struct.unpack('<4H', world.read_near(base+action['rect']*8, 8))
            x, y = (l+right)//2, (top+bottom)//2
            if action['drag']:
                end = l-40 if action['drag']=='left' else right+40
                world.mouse.append((0x200, end, y))
            world.mouse.append((0x202, x, y))
            return 0x201, 4 if action['shift'] else 0, x | (y << 16)
        else:
            raise ValueError(f'Unknown scenario operation: {op}')
        return 0x111, action['control'], 0
    return apply


def visit(page, actions, command, labels):
    def observe(world, put):
        for id, value in world.text.items():
            if value:
                labels[page, id] = value.decode('cp1252')
    compiled = [(0x110, 0, 0), (0xf, 0, 0), observe]
    for action in actions:
        compiled.extend([compile_action(action, page)]*action.get('count', 1))
    compiled.extend([(0xf, 0, 0), (0x111, command, 0)])
    return PAGES[page-1], compiled


def text(control, value):
    return dict(op='text', control=control, value=value)


def scripts(case, labels, shorten=False):
    first = [select(0x10f, 0x10f, 0x116)]
    if shorten:
        first = [select(0x116, 0x10f, 0x116)]
    elif case['template']:
        first.append(select(0x10f+case['template'], 0x10f, 0x116))
    singular, plural = case['singular'], case['plural']
    if case['flow'] == 'name_tail':
        singular, plural = ('X', 'Xs') if shorten else ('LongRaceName', 'LongRaceNames')
    first += [text(0x10c, singular), text(0x81b, plural), text(0x10d, case['password'])]
    result = []
    for page in range(1, case['finish_page']+1):
        actions = (first if page == 1 else [])+case['changes'].get(page, [])
        if page == 2 and case['flow'] == 'back':
            result.extend([visit(2, [], 0x42e, labels), visit(1, [], 0x42f, labels)])
        command = 0x42f if page < case['finish_page'] else 2 if case['flow']=='cancel' else 0x430
        result.append(visit(page, actions, command, labels))
    if case['password']:
        wrong = compile_action(text(0x10c, 'wrong'), 0)
        correct = compile_action(text(0x10c, case['password']), 0)
        result.append((0x8c, [(0x110, 0, 0), wrong, (0x111, 1, 0), correct, (0x111, 1, 0)]))
    # A failed point check reopens the same page. This optional final script
    # cancels there; a successful save leaves it unused in BOTH implementations.
    result.append(visit(case['finish_page'], [], 2, labels))
    return result


def run_session(t, script, filename, new=True, tick=123):
    from tests.test_race_ui import UiHost
    t.m.events.clear(); t.m.window_events.clear(); t.m.world.events.clear()
    t.m.outgoing.clear(); t.m.dialog_scripts = script
    t.m.dialog_accept = True; t.m.dialog_path = ('C:\\'+filename).encode('ascii')
    t.m.opens = [(7, 0)]; t.m.ticks = [tick]; t.m.timer_values = [0, 2000]
    host = UiHost(t.lib, t.m)
    if new:
        actual = t.lib.stars_new_race(C.byref(host.runtime), 200)
        expected = t.original('CommandHandler', struct.pack('<HH', 200, 0x81))
        # The original menu caller has a larger frame; this pointer is expired.
        host.data[0x48ac:0x48ae] = t.m.get(0x48ac, 2)
    else:
        actual = t.lib.stars_race_wizard(C.byref(host.runtime), 200, 0, 0)
        expected = t.original('RaceCreationWizard', struct.pack('<HHH', 200, 0, 0))
    t.assertEqual(actual, expected)
    t.compare(host)
    t.assertEqual(bytes(host.outgoing), bytes(t.m.outgoing))
    t.assertEqual([r for r, _ in host.dialog_scripts], [r for r, _ in t.m.dialog_scripts])
    t.assertLessEqual(len(t.m.dialog_scripts), 1)
    points = C.c_int16(t.original('CAdvantagePoints', struct.pack('<H', 0x4f9a))).value
    t.assertEqual(t.lib.stars_race_advantage_points(host.data, 0x4f9a), points)
    t.compare(host)
    record = t.m.get(0x4f9a, 192)
    return dict(result=expected, advantage_points=points,
                player=record.hex(), fields=player_fields(record),
                event_count=len(t.m.world.events),
                alerts=[e[2].decode('cp1252') for e in t.m.events if e[0]=='message']), bytes(host.outgoing), bytes(t.m.outgoing)


def load_session(t, payload, filename):
    from tests.test_race_ui import UiHost
    t.m.incoming = payload; t.m.position = 0; t.m.opens = [(7, 0)]
    t.m.events.clear(); t.m.window_events.clear(); t.m.world.events.clear()
    t.m.put(0x3000, filename.encode('ascii')+b'\0')
    host = UiHost(t.lib, t.m)
    actual = t.lib.stars_was_race_file(C.byref(host.runtime), 0x3000, 0)
    expected = C.c_int16(t.original('FWasRaceFile', struct.pack('<HH', 0x3000, 0))).value
    t.assertEqual(actual, expected); t.compare(host)
    return expected


def initial(t):
    t.setUp()
    for at, count in [(0x99c, 4), (0x7ca, 8), (0x1318, 16), (0x1308, 16),
                      (0x35c, 8), (0x70, 4), (0x80, 4)]:
        t.m.put(at, bytes(count))
    t.m.seeds(value=(53668, 52774))


def generate(cases, tick=123):
    # Deliberately reuse the already validated adapters/assertions rather than
    # create a second, weaker mock of the complete component.
    from tests.test_race_ui import RaceUiTests
    RaceUiTests.setUpClass()
    try:
        t = RaceUiTests(); inspector = Inspector(t.lib)
        results, files, labels = [], {}, {}
        for case in cases:
            initial(t)
            phases = [False, True] if case['flow']=='name_tail' else [False]
            row = dict(case=case, sessions=[])
            for shorten in phases:
                filename = case['id']+('A' if not shorten else 'B')+'.R1' if len(phases)>1 else case['id']+'.R1'
                outcome, candidate, reference = run_session(t, scripts(case, labels, shorten), filename,
                                                           new=not shorten, tick=tick)
                session = dict(filename=filename, **outcome, file=None, load_result=None)
                if reference:
                    session['file'] = inspector.inspect(reference)
                    session['load_result'] = load_session(t, candidate, filename)
                    files['MODEL/'+filename] = reference
                    files['CANDIDAT/'+filename] = candidate
                row['sessions'].append(session)
            for page, actions in case['changes'].items():
                for action in actions:
                    if action['op']=='check' and page==3:
                        case['title'] = 'Lesser trait: '+labels[page, action['control']].replace('&', '')
            results.append(row)
            print(f"{case['id']}: result={row['sessions'][-1]['result']} points={row['sessions'][-1]['advantage_points']}", flush=True)
        return dict(schema=1, original_sha256=ORIGINAL_SHA256,
                    method='Original x86 and recovered C callbacks; modeled Windows services. Live VM results pending.',
                    control_labels=[dict(page=p, control=c, text=v) for (p, c), v in sorted(labels.items())],
                    controlled_inputs=dict(game_id=0, turn=0, game_flags=0,
                                           initial_game_rng=[53668, 52774], tick=tick),
                    cases=results), files, labels
    finally:
        RaceUiTests.tearDownClass()


def recipes(report, labels, pilot):
    lines = ['STARS RACE ACCEPTANCE - '+('PILOT' if pilot else 'ALL CASES'), '',
             'Generated predictions are checked against original instructions under modeled',
             'Windows services. These are NOT recorded DOSBox/86Box observations.', '',
             'For each case: exit and restart Stars! with no game loaded. At the title',
             'screen choose New Game (Alt+N), then Customize Race (Alt+C). Leave the',
             'New Game race choice at Humanoid. Do not create a world for these tests.',
             'Do not reuse a previous wizard session except where specified.',
             'Choose Humanoid first. The case then names the next template choice.',
             'Use a separate empty folder for live files. Save using the listed filenames.',
             'Do not overwrite MODEL or CANDIDAT. Copy live files into RETURN.',
             'Write observed scores, messages, load results and any deviations in OBSERVE.TXT.',
             'To reload from the title: Open Game (Alt+O). First navigate into the',
             'fixture directory (enter its path plus \\*.R1 and press Enter). Then',
             'enter the bare filename. Confirm the dialog shows the correct directory.',
             'Use a separate fresh session to test loading CANDIDAT files; do not resave',
             'those files before copying original live creations back for comparison.', '',
             'Mouse-dependent cases have descriptive targets, not assumed screen coordinates.',
             'Verify displayed values after clicks; timing can cause repeated steps.',
             'Only the pilot needs initial live replay. Full permutation replay is optional.', '']
    for row in report['cases']:
        case = row['case']
        if pilot and not case['pilot']:
            continue
        lines += [case['id']+' - '+case['title'], 'Choose '+TEMPLATES[case['template']]+'.']
        names = ('LongRaceName', 'LongRaceNames') if case['flow']=='name_tail' else (case['singular'], case['plural'])
        lines += [f'Page 1: singular "{names[0]}"; plural "{names[1]}".',
                  'Password: '+('leave empty.' if not case['password'] else 'use synthetic "'+case['password']+'".')]
        for page in range(1, case['finish_page']+1):
            if page == 2 and case['flow']=='back':
                lines += ['Page 2: click Back; check the names on page 1; click Next again.']
            for action in case['changes'].get(page, []):
                lines += [f'Page {page}: '+action_words(action, page, labels)]
            if page < case['finish_page']:
                lines += [f'Page {page}: click Next.']
        lines += [f'Page {case["finish_page"]}: click '+('Cancel.' if case['flow']=='cancel' else 'Finish.')]
        if case['password']:
            lines += ['At password confirmation: try "wrong", then "race-test".']
        for index, session in enumerate(row['sessions']):
            if index:
                lines += ['Without restarting: reopen the saved U020A.R1 for editing, select',
                          'Custom explicitly, then replace the names with "X" and "Xs".',
                          'Click Finish and save separately as U020B.R1.']
            lines += [f'Predicted advantage points: {session["advantage_points"]}.']
            if session['file']:
                lines += ['Save as '+session['filename']+'.',
                          'Expected reload: '+('accepted.' if session['load_result']==1 else 'rejected (checksum case; live confirmation pending).'),
                          'Final settings: '+json.dumps(session['fields'], ensure_ascii=True, sort_keys=True)]
            else:
                lines += ['Expected: '+('cancel without a file.' if case['flow']=='cancel' else 'negative-points warning; dismiss it, then Cancel. No file.')]
            lines.extend('Predicted message: '+message for message in session['alerts'])
        lines.append('')
    return '\n'.join(lines)+'\n'


def publish(output, report, files, labels):
    archive = output.with_suffix('.ZIP')
    if output.exists() or archive.exists():
        raise ValueError('Output already exists; choose a fresh directory to preserve prior returns')
    readme = (ROOT/'compat/race/README.TXT').read_text(encoding='ascii')
    files = dict(files)
    files.update({'README.TXT': readme, 'PILOT.TXT': recipes(report, labels, True),
                  'ALLCASES.TXT': recipes(report, labels, False),
                  'EXPECT.JSN': json.dumps(report, indent=2, ensure_ascii=True)+'\n',
                  'RETURN/OBSERVE.TXT': 'Case ID | observed score | create result | live reload | CANDIDAT load | notes\n'})
    for name, value in list(files.items()):
        if isinstance(value, str):
            files[name] = value.replace('\r\n', '\n').replace('\n', '\r\n').encode('ascii')
    manifest = dict(schema=1, files={name: hashlib.sha256(raw).hexdigest() for name, raw in sorted(files.items())})
    files['MANIFEST.JSN'] = (json.dumps(manifest, indent=2)+'\n').replace('\n', '\r\n').encode('ascii')
    output.mkdir(parents=True)
    for name, raw in sorted(files.items()):
        destination = output/name
        destination.parent.mkdir(parents=True, exist_ok=True)
        destination.write_bytes(raw)
    with zipfile.ZipFile(archive, 'w', zipfile.ZIP_DEFLATED) as zipout:
        for name, raw in sorted(files.items()):
            info = zipfile.ZipInfo(output.name+'/'+name, date_time=(2000, 1, 1, 0, 0, 0))
            info.compress_type = zipfile.ZIP_DEFLATED
            zipout.writestr(info, raw)
    return archive


def review(package, returned):
    # Read only filenames listed by our scenarios, never sweep the share.
    report = json.loads((package/'EXPECT.JSN').read_text(encoding='ascii'))
    manifest = json.loads((package/'MANIFEST.JSN').read_text(encoding='ascii'))
    digest = hashlib.sha256((package/'EXPECT.JSN').read_bytes()).hexdigest()
    if manifest['files']['EXPECT.JSN'] != digest:
        raise ValueError('Package expectations differ from their integrity manifest')
    inspector = Inspector()
    rows = []
    allowed = {c['id']+'.R1' for c in catalog()} | {'U020A.R1', 'U020B.R1'}
    for case in report['cases']:
        for session in case['sessions']:
            filename = session['filename']
            if filename not in allowed:
                raise ValueError('Invalid fixture filename in package')
            path = returned/filename
            if not path.exists():
                rows.append(dict(filename=filename, status='MISSING' if session['file'] else 'OBSERVATION_NEEDED'))
            elif not session['file']:
                rows.append(dict(filename=filename, status='UNEXPECTED_FILE'))
            else:
                try:
                    rows.append(dict(filename=filename, **compare(session['file'], inspector.inspect(path.read_bytes()))))
                except ValueError as error:
                    rows.append(dict(filename=filename, status='INVALID_FILE', reason=str(error)))
    return dict(scope='File comparison only; live actions/rendering/load observations require review.', files=rows)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path, default=ROOT/'ftp-share/RACEACC1')
    parser.add_argument('--review', type=Path, metavar='PACKAGE')
    parser.add_argument('--returned', type=Path)
    args = parser.parse_args()
    if args.review:
        result = review(args.review, args.returned or args.review/'RETURN')
        print(json.dumps(result, indent=2))
        return int(any(row['status']!='PASS' for row in result['files']))
    if args.output.exists() or args.output.with_suffix('.ZIP').exists():
        parser.error('Output exists; use --output with a fresh directory')
    report, files, labels = generate(catalog())
    archive = publish(args.output, report, files, labels)
    print(f"Prepared {len(report['cases'])} cases; {sum(c['case']['pilot'] for c in report['cases'])} pilot cases.\n{archive}")
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
