#!/usr/bin/env python3
"""Prepare a new local study guest from explicitly identified original inputs."""
import argparse
import hashlib
import json
from pathlib import Path
import plistlib
import shutil
from tools.runtime.launch import EXPECTED, ROOT


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--windows-media', type=Path, required=True)
    ap.add_argument('--dosbox', type=Path, default=Path('/opt/homebrew/Cellar/dosbox-x/2026.08.31/bin/dosbox-x'))
    args = ap.parse_args()
    expected_emulator = next(r['sha256'] for r in json.loads((ROOT/'analysis/runtime/environment.json').read_text())['components'] if r['component'] == 'dosbox-x')
    if hashlib.sha256(args.dosbox.read_bytes()).hexdigest() != expected_emulator:
        raise SystemExit('DOSBox-X binary differs from recorded build')
    original = (ROOT/'target/stars.exe').read_bytes()
    if hashlib.sha256(original).hexdigest() != EXPECTED:
        raise SystemExit('Original executable identity mismatch')
    media = []
    for row in json.loads((ROOT/'analysis/runtime/install-media.json').read_text())['files']:
        name = row['name']
        if Path(name).name != name:
            raise SystemExit('Invalid media manifest path')
        data = (args.windows_media/name).read_bytes()
        if len(data) != row['size'] or hashlib.sha256(data).hexdigest() != row['sha256']:
            raise SystemExit('Windows installation media identity mismatch')
        media.append((name,data))
    guest = ROOT/'runs/step4/guest'
    if guest.exists():
        raise SystemExit('Study guest already exists; it has not been changed')
    guest.mkdir(parents=True)
    for directory in ['STARS','INSTALL','TEMP']:
        (guest/directory).mkdir()
    (ROOT/'runs/step4/captures').mkdir(exist_ok=True)
    (guest/'STARS/STARS.EXE').write_bytes(original)
    for name,data in media:
        (guest/'INSTALL'/name).write_bytes(data)
    app = ROOT/'.tools/StarsRuntime.app/Contents'
    (app/'MacOS').mkdir(parents=True,exist_ok=True)
    shutil.copy2(args.dosbox,app/'MacOS/dosbox-x')
    (app/'Info.plist').write_bytes(plistlib.dumps(dict(CFBundleIdentifier='local.stars.runtime',
        CFBundleName='Stars Runtime',CFBundleDisplayName='Stars Runtime',CFBundleExecutable='dosbox-x',
        CFBundlePackageType='APPL',CFBundleVersion='2026.08.31',NSHighResolutionCapable=True)))
    print('Prepared isolated guest; install Windows before using the launcher.')

if __name__ == '__main__':
    main()
