#!/usr/bin/env python3
"""Build the reviewed C89 semantic core as a host shared library."""
import argparse
from pathlib import Path
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[1]
SOURCES = ['utilgen.c', 'race.c', 'near_state.c', 'codec.c', 'text.c', 'create.c',
           'planet.c', 'strings.c', 'records.c', 'stream.c', 'file.c', 'alerts.c',
           'runtime.c', 'password.c', 'output.c', 'format.c', 'ui.c', 'raceui.c',
           'racedraw.c', 'racewizard.c']


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--cc', default='cc')
    args = parser.parse_args()
    output = ROOT / 'build' / ('libstars_core.dylib' if sys.platform == 'darwin' else 'libstars_core.so')
    output.parent.mkdir(exist_ok=True)
    subprocess.run([args.cc, '-std=c89', '-pedantic', '-Wall', '-Wextra', '-Werror',
                    '-O2', '-shared', '-fPIC', '-I', str(ROOT / 'reconstructed/include'),
                    *[str(ROOT / 'reconstructed/src' / name) for name in SOURCES], '-o', str(output)], check=True)
    print(output)


if __name__ == '__main__':
    main()
