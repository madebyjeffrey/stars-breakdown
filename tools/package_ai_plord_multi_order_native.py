#!/usr/bin/env python3
"""Package the multi-order PLORD probe for VC++ 1.52c / Win98."""

import argparse
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
REFERENCE = ROOT / "compat/vc152/do-cyber-ai-turn-order-write-e9/WINSTUB.EXE"


def write_text(path, text):
    path.write_bytes(text.replace("\r\n", "\n").replace("\r", "\n")
                     .replace("\n", "\r\n").encode("ascii"))


def copy_text(source, target):
    write_text(target, source.read_text(encoding="ascii"))


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--guest-dir", required=True)
    args = parser.parse_args()
    output = args.output.resolve()
    output.mkdir(parents=True, exist_ok=False)

    for source, name in (
            (ROOT / "reconstructed/include/core.h", "CORE.H"),
            (ROOT / "reconstructed/src/ai_plord.c", "PLORD.C"),
            (ROOT / "tests/ai_plord_multi_order_probe.c", "PROBE.C")):
        copy_text(source, output / name)
    (output / "WINSTUB.EXE").write_bytes(REFERENCE.read_bytes())

    guest_dir = args.guest_dir
    if not guest_dir.startswith("C:\\"):
        raise ValueError("--guest-dir must be a C:\\ path")
    write_text(output / "PLORDNAT.C", """\
#include <windows.h>
#include <stdio.h>
#define PLORD_MULTI_NATIVE
#include "PROBE.C"
#include "PLORD.C"

int PASCAL WinMain(HINSTANCE a, HINSTANCE b, LPSTR c, int d)
{
    FILE *f;
    int r;
    (void)a; (void)b; (void)c; (void)d;
    r = ai_plord_multi_order_probe();
    f = fopen("PLORD.TXT", "w");
    if (f) {
        fprintf(f,
            "AI_PLORD_MULTI_ORDER_CHECKS=15 FAILURES=%d RESULT=%s\\n",
            r ? 1 : 0, r ? "FAIL" : "PASS");
        fclose(f);
    }
    return r;
}
""")
    write_text(output / "PLORDNAT.DEF", """\
NAME PLORDNAT WINDOWAPI
DESCRIPTION 'Stars multi-order PLORD probe'
EXETYPE WINDOWS
STUB 'WINSTUB.EXE'
CODE PRELOAD MOVEABLE DISCARDABLE
DATA PRELOAD MOVEABLE MULTIPLE
HEAPSIZE 1024
STACKSIZE 8192
""")
    write_text(output / "PLORDNAT.LNK", """\
PLORDNAT.OBJ
PLORDNAT.EXE
PLORDNAT.MAP
C:\\MSVC\\LIB\\MLIBCEW.LIB C:\\MSVC\\LIB\\LIBW.LIB /NOD /ALIGN:16 /CO /MAP
PLORDNAT.DEF
""")
    write_text(output / "BUILD.BAT", """\
@echo off
set INCLUDE=C:\\MSVC\\INCLUDE
set LIB=C:\\MSVC\\LIB
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 PLORDNAT.C > PLCL.LOG
if errorlevel 1 goto fail
C:\\MSVC\\BIN\\LINK.EXE @PLORDNAT.LNK > PLLINK.LOG
if errorlevel 1 goto fail
echo COMPILE=PASS > BLDSTAT.TXT
echo LINK=PASS >> BLDSTAT.TXT
goto done
:fail
echo COMPILE=FAIL > BLDSTAT.TXT
echo LINK=FAIL >> BLDSTAT.TXT
:done
""")
    write_text(output / "RUN.BAT", f"""\
@echo off
cd {guest_dir}
PLORDNAT.EXE
""")
    write_text(output / "RUNBLD.BAT", f"""\
@echo off
cd {guest_dir}
call BUILD.BAT
""")


if __name__ == "__main__":
    main()
