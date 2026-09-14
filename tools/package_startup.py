#!/usr/bin/env python3
"""Build the isolated Win16 starting-screen/race-workshop transfer package."""
import argparse
import json
from pathlib import Path
import re
import struct
import sys
import zipfile

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT))
from tools.ne_layout import NE
from tools.package_vc152 import dos_text, sha
from tools.build_core import SOURCES, FOUNDATION_SOURCES

ORIGINAL = 'ccab6682b55dae14dbf104e491cfbd3b32a6280022020032ae0615500949ba59'
RENAMES = {'near_state.c':'NEARSTAT.C', 'racewizard.c':'RACEWIZ.C',
           'runtime_internal.h':'RTINT.H', 'ui_internal.h':'UIINT.H',
           'seed_table.inc':'SEEDTAB.INC', 'battle_plan_templates.inc':'BPLANS.INC',
           'innate_fp.h':'INNATE.H', 'resources.c':'RESOURCE.C'}


def name_for(name): return RENAMES.get(name, name.upper())


def resource_identifier(value):
    if isinstance(value, int):
        if not 0 <= value <= 65535: raise ValueError('Invalid resource ordinal')
        return b'\xff' + struct.pack('<H', value)
    data = value.encode('cp1252')
    if b'\0' in data or data[:1] == b'\xff': raise ValueError('Invalid resource name')
    return data + b'\0'


def resource_file(ne):
    """Win16 .RES records, independently confirmed by RC 3.11's RCDATA probe.

    Preserve original NE resource payloads including their existing padding.
    RC binds these records without converting bitmap or dialog formats.
    """
    return b''.join(resource_identifier(r['type']) + resource_identifier(r['id']) +
                    struct.pack('<HI', r['flags'], r['bytes']) +
                    ne.b[r['file_offset']:r['file_offset'] + r['bytes']]
                    for r in ne.resources)


def compare_resources(original, rebuilt):
    a = {(r['type'], r['id']):(r['bytes'], r['flags'], r['sha256']) for r in original.resources}
    b = {(r['type'], r['id']):(r['bytes'], r['flags'], r['sha256']) for r in rebuilt.resources}
    if a != b:
        raise ValueError('Resource mismatch: ' + repr([k for k in a.keys() | b.keys() if a.get(k) != b.get(k)]))
    return len(a)


def payload():
    ne = NE(ROOT / 'target/stars.exe')
    if sha(ne.b) != ORIGINAL: raise ValueError('Original input hash changed')
    files, sources = {}, {}
    inputs = {name_for(n):ROOT/'reconstructed/src'/n for n in SOURCES + FOUNDATION_SOURCES +
              ['runtime_internal.h','ui_internal.h','innate_fp.h','seed_table.inc','battle_plan_templates.inc']}
    inputs.update({n.upper():ROOT/'reconstructed/include'/n for n in ['core.h','runtime.h','raceui.h','memory.h','common.h','startup.h','title.h','config.h']})
    inputs.update({n:ROOT/'compat/startup'/n for n in ['APP.C','APP.H','NATIVE.C','START.DEF','BUILD.BAT','README.TXT','CHECKS.TXT']})
    for name, path in inputs.items():
        raw = path.read_bytes()
        code = re.sub(r'(#include\s+")([^"]+)(")', lambda m:m[1]+name_for(m[2])+m[3], raw.decode('ascii'))
        files[name] = dos_text(code)
        sources[str(path.relative_to(ROOT))] = sha(raw)
    # Only the static data prefix of strings.obj is needed. Remove its code.
    # All original string lookup offsets remain unchanged in a 64 KiB image.
    files['STRINGS.BIN'] = ne.raw[2][:0x743a].ljust(65536, b'\0')
    files['INDATA.BIN'] = ne.raw[36].ljust(65536, b'\0')
    files['STARS.RES'] = resource_file(ne)
    objects = [name_for(n).replace('.C','.OBJ') for n in SOURCES + FOUNDATION_SOURCES] + ['NATIVE.OBJ','APP.OBJ']
    files['START.RSP'] = dos_text('/c /AL /GA /GEs /Zp1 /Od /Zi /W3\n' +
        '\n'.join(name_for(n) for n in SOURCES + FOUNDATION_SOURCES) + '\nNATIVE.C\nAPP.C\n')
    files['START.LNK'] = dos_text('/NOLOGO /NOD /MAP /CO /ALIGN:64 ' + '+\n'.join(objects) +
                                '\nSTART.EXE\nSTART.MAP\nLLIBCEW LIBW COMMDLG TOOLHELP\nSTART.DEF;\n')
    manifest = {'revision':3, 'package':'START2A', 'original_sha256':ORIGINAL, 'sources':sources,
                'resources':[dict(type=r['type'],id=r['id'],bytes=r['bytes'],flags=r['flags'],sha256=r['sha256']) for r in ne.resources],
                'files':{n:dict(bytes=len(b),sha256=sha(b)) for n,b in files.items()}}
    files['MANIFEST.TXT'] = dos_text(json.dumps(manifest,indent=2,ensure_ascii=True)+'\n')
    for n in files:
        if not re.fullmatch(r'[A-Z0-9_]{1,8}\.[A-Z0-9_]{1,3}', n): raise ValueError(n)
    return files


def export(output, development=False, native_build=None, race_build=None):
    files = payload()
    if native_build:
        # Require every executable input to match the current generated files.
        # A README/log change alone does not require a compiler rerun.
        for name,data in files.items():
            if Path(name).suffix in ['.C','.H','.INC','.RES','.BIN','.RSP','.LNK','.DEF']:
                if (native_build/name).read_bytes()!=data:
                    raise ValueError('Native build input differs: '+name)
        status=(native_build/'BLDSTAT.TXT').read_text()
        if any(s not in status for s in ['COMPILE=PASS','LINK=PASS','RESOURCES=PASS']) or 'FAIL' in status:
            raise ValueError('Native build did not pass')
        compare_resources(NE(ROOT/'target/stars.exe'),NE(native_build/'START.EXE'))
        for name in ['START.EXE','START.MAP','BLDSTAT.TXT','BUILD.LOG','LINK.LOG','RC.LOG']:
            files[name]=(native_build/name).read_bytes()
        manifest=json.loads(files['MANIFEST.TXT'])
        manifest['native_build']={name:dict(bytes=len(files[name]),sha256=sha(files[name])) for name in
                                  ['START.EXE','START.MAP','BLDSTAT.TXT','BUILD.LOG','LINK.LOG','RC.LOG']}
        accept=ROOT/'analysis/compat/startup2a-native.json'
        if accept.exists():
            report=json.loads(accept.read_text())
            if report['executable_sha256']!=sha(files['START.EXE']):
                raise ValueError('Acceptance log belongs to a different executable')
            files['CHECK.LOG']=dos_text(report['check_log'])
            manifest['acceptance_log_sha256']=sha(files['CHECK.LOG'])
        files['MANIFEST.TXT']=dos_text(json.dumps(manifest,indent=2,ensure_ascii=True)+'\n')
    if race_build:
        from tools.package_racevc import payload as race_payload
        regression=race_payload()
        for name,data in regression.items():
            if (race_build/name).read_bytes()!=data:
                raise ValueError('Regression input differs: '+name)
        result=(race_build/'RESULTS.TXT').read_text()
        if not all(re.search(r'\b'+x+r'\b',result) for x in ['STATUS=PASS','CASES=88','SESSIONS=89','CHECKS=816','FAILURES=0']):
            raise ValueError('Native regression must complete before export')
        vectors=json.loads((ROOT/'compat/racevc/vectors.json').read_text())
        for case in vectors['cases']:
            for session in case['sessions']:
                player=race_build/'ACTUAL'/(Path(session['filename']).stem+'.STA')
                if player.read_bytes()!=bytes.fromhex(session['player']):
                    raise ValueError('Native regression state mismatch: '+player.name)
                if session['wire'] is not None:
                    if (race_build/'ACTUAL'/session['filename']).read_bytes()!=bytes.fromhex(session['wire']):
                        raise ValueError('Native regression file mismatch: '+session['filename'])
        # Carry the developer's result as BASELINE, leaving new-run files absent.
        for name in ['RACEVC.EXE','RACEVC.MAP','BLDSTAT.TXT','BUILD.LOG','LINK.LOG']:
            regression[name]=(race_build/name).read_bytes()
        regression['BASELINE.TXT']=(race_build/'RESULTS.TXT').read_bytes()
        readme=(ROOT/'compat/startup/REGRESS.TXT').read_bytes()
        regression['README.TXT']=dos_text(readme.decode('ascii'))
        child=json.loads(regression['MANIFEST.JSN'])
        child['files']['README.TXT']=sha(regression['README.TXT'])
        child['bundled_readme_override']='compat/startup/REGRESS.TXT replaces the historical compat/racevc/README.TXT text'
        child['source_hashes']['compat/startup/REGRESS.TXT']=sha(readme)
        child['native_status']='Developer VC152/WfW311 run passed 816 checks; user Win98/86Box run pending'
        regression['MANIFEST.JSN']=dos_text(json.dumps(child,indent=2)+'\n')
        manifest=json.loads(files['MANIFEST.TXT'])
        manifest['regression']={name:dict(bytes=len(data),sha256=sha(data)) for name,data in regression.items()}
        files.update({'REGRESS/'+name:data for name,data in regression.items()})
        files['MANIFEST.TXT']=dos_text(json.dumps(manifest,indent=2,ensure_ascii=True)+'\n')
    if development:
        if output.resolve() != ROOT/'runs/step4/guest/STARTDEV': raise ValueError('Development output must be STARTDEV')
    elif output.exists() or output.with_suffix('.ZIP').exists():
        raise FileExistsError('Preserve prior builds; choose a new folder')
    output.mkdir(parents=True,exist_ok=development)
    for name,data in files.items():
        path=output/name;path.parent.mkdir(parents=True,exist_ok=True);path.write_bytes(data)
    if not development:
        with zipfile.ZipFile(output.with_suffix('.ZIP'),'x',compression=zipfile.ZIP_DEFLATED) as package:
            for name,data in sorted(files.items()):
                info=zipfile.ZipInfo(name,date_time=(2026,9,14,0,0,0));info.compress_type=zipfile.ZIP_DEFLATED
                info.create_system=0;info.external_attr=0x20;package.writestr(info,data)
    return files


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--output',type=Path,default=ROOT/'ftp-share/START2A')
    p.add_argument('--development',action='store_true')
    p.add_argument('--verify-exe',type=Path)
    p.add_argument('--race-build',type=Path,help='Include a matching completed native race regression build')
    p.add_argument('--native-build',type=Path,help='Include a verified matching native executable and build logs')
    a=p.parse_args()
    if a.verify_exe:
        count=compare_resources(NE(ROOT/'target/stars.exe'),NE(a.verify_exe))
        print(f'PASS: all {count} resource identifiers, sizes, flags and SHA-256 hashes match')
    else:
        export(a.output,a.development,a.native_build,a.race_build);print(a.output)


if __name__ == '__main__': main()
