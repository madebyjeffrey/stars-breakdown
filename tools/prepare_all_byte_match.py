#!/usr/bin/env python3
import argparse
import shutil
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]

def prepare(work, compiler_drive):
    work = work.resolve()
    compiler_drive = compiler_drive.resolve()
    work.mkdir(parents=True, exist_ok=True)
    source = ROOT / 'compat/all-byte-match'
    for path in sorted(source.glob('F*.C')) + [source / 'BUILD.BAT']:
        shutil.copyfile(path, work / path.name)
    config = work.parent / 'compile.conf'
    config.write_text(
        '[sdl]\noutput=surface\n[dosbox]\nmemsize=64\n'
        '[cpu]\ncore=normal\ncycles=max\n[autoexec]\n'
        'mount c "' + str(compiler_drive) + '"\n'
        'mount d "' + str(work) + '"\nd:\ncall build.bat\nexit\n'
    )
    return config

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--work', type=Path, required=True)
    parser.add_argument('--compiler-drive', type=Path,
                        default=ROOT / 'runs/step4/guest')
    args = parser.parse_args()
    print(prepare(args.work, args.compiler_drive))
