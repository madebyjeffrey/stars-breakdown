#!/usr/bin/env python3
import argparse
from pathlib import Path
import shutil

ROOT = Path(__file__).resolve().parents[1]

def prepare(work, compiler_drive):
    work = work.resolve()
    compiler_drive = compiler_drive.resolve()
    if not (compiler_drive / 'MSVC/BIN/CL.EXE').is_file():
        raise ValueError('Compiler drive must contain MSVC/BIN/CL.EXE')
    work.mkdir(parents=True, exist_ok=True)
    source = ROOT / 'compat/mdi-format-match'
    for name in ['MDI.C', 'BUILD.BAT']:
        shutil.copyfile(source / name, work / name)
    config = work.parent / 'compile.conf'
    config.write_text(
        '[sdl]\noutput=surface\n[dosbox]\nmemsize=64\n[cpu]\ncore=normal\ncycles=max\n'
        '[autoexec]\nmount c "' + str(compiler_drive) + '"\nmount d "' + str(work) + '"\n'
        'd:\ncall build.bat\nexit\n')
    return config

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--work', type=Path, required=True)
    parser.add_argument('--compiler-drive', type=Path, default=ROOT / 'runs/step4/guest')
    args = parser.parse_args()
    print(prepare(args.work, args.compiler_drive))
