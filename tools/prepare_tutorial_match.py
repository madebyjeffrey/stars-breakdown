#!/usr/bin/env python3
"""Prepare an isolated DOS compiler drive for the tutorial byte comparison."""
import argparse
import json
from pathlib import Path
import shutil

ROOT = Path(__file__).resolve().parents[1]
FLAGS = '/c /AM /GA /GEs /Zp1 /Od /Gs /W3 /Fc'


def prepare(work, compiler_drive):
    work = work.resolve()
    compiler_drive = compiler_drive.resolve()
    if not (compiler_drive/'MSVC/BIN/CL.EXE').is_file():
        raise ValueError('Compiler drive must contain MSVC/BIN/CL.EXE')
    work.mkdir(parents=True,exist_ok=True)
    source = ROOT/'compat/tutorial-match'
    rows = json.loads((source/'candidates.json').read_text())
    for name in ['TYPES.H','candidates.json']+[r['stem']+'.C' for r in rows]:
        shutil.copyfile(source/name,work/name)
    lines = ['@echo off','set PATH=C:\\MSVC\\BIN','set INCLUDE=C:\\MSVC\\INCLUDE',
             'set TMP=D:\\','set TEMP=D:\\','set CL=',
             'if exist DONE.TXT del DONE.TXT']
    for row in rows:
        stem=row['stem']
        lines.extend([f'if exist {stem}.OBJ del {stem}.OBJ',f'if exist {stem}.COD del {stem}.COD',
                      f'echo Compiling {row["name"]}',f'CL {FLAGS} {stem}.C > {stem}.LOG',
                      'if errorlevel 1 goto failed'])
    lines += ['echo PASS > DONE.TXT','goto end',':failed','echo FAIL > DONE.TXT',':end']
    (work/'BUILD.BAT').write_bytes(('\r\n'.join(lines)+'\r\n').encode('ascii'))
    config = work.parent/'compile.conf'
    config.write_text(f'[sdl]\noutput=surface\n[dosbox]\nmemsize=64\n[cpu]\ncore=normal\ncycles=max\n'
                      f'[autoexec]\nmount c "{compiler_drive}"\nmount d "{work}"\nd:\ncall build.bat\nexit\n')
    return config


def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--work',type=Path,default=ROOT/'runs/tutorial-match/rebuild')
    ap.add_argument('--compiler-drive',type=Path,default=ROOT/'runs/step4/guest')
    args=ap.parse_args()
    print(prepare(args.work,args.compiler_drive))

if __name__=='__main__':main()
