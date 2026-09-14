#!/usr/bin/env python3
"""Launch the isolated study guest with the recorded DOSBox-X configuration."""
import os
from pathlib import Path
import hashlib

ROOT = Path(__file__).resolve().parents[2]
EXPECTED = 'ccab6682b55dae14dbf104e491cfbd3b32a6280022020032ae0615500949ba59'

def main():
    guest = ROOT / 'runs/step4/guest'
    original = ROOT / 'target/stars.exe'
    copied = guest / 'STARS/STARS.EXE'
    for path in (original, copied):
        if hashlib.sha256(path.read_bytes()).hexdigest() != EXPECTED:
            raise SystemExit('Executable identity mismatch')
    if not (guest / 'WINDOWS/WIN.COM').is_file():
        raise SystemExit('Install Windows in the study guest first')
    binary = ROOT / '.tools/StarsRuntime.app/Contents/MacOS/dosbox-x'
    if not binary.is_file():
        raise SystemExit('Prepare the local DOSBox-X app wrapper first')
    os.chdir(ROOT / 'runs/step4')
    os.environ.setdefault('TERM', 'xterm-256color')
    os.execv(str(binary), [str(binary), '-conf', str(ROOT / 'runtime/dosbox-x.conf'),
                         '-nopromptfolder', '-fastlaunch', '-c', 'cd WINDOWS', '-c', 'WIN'])

if __name__ == '__main__':
    main()
