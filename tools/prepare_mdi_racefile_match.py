#!/usr/bin/env python3
import argparse
from pathlib import Path
import shutil
ROOT=Path(__file__).resolve().parents[1]
def prepare(work, compiler_drive):
    work=work.resolve(); compiler_drive=compiler_drive.resolve()
    if not (compiler_drive/'MSVC/BIN/CL.EXE').is_file(): raise ValueError('Compiler drive must contain MSVC/BIN/CL.EXE')
    work.mkdir(parents=True,exist_ok=True); source=ROOT/'compat/mdi-racefile-match'
    for n in ['MDI.C','BUILD.BAT']: shutil.copyfile(source/n,work/n)
    config=work.parent/'compile.conf'; config.write_text('[sdl]\noutput=surface\n[dosbox]\nmemsize=64\n[cpu]\ncore=normal\ncycles=max\n[autoexec]\nmount c "'+str(compiler_drive)+'"\nmount d "'+str(work)+'"\nd:\ncall build.bat\nexit\n'); return config
if __name__=='__main__':
    p=argparse.ArgumentParser();p.add_argument('--work',type=Path,required=True);p.add_argument('--compiler-drive',type=Path,default=ROOT/'runs/step4/guest');a=p.parse_args();print(prepare(a.work,a.compiler_drive))
