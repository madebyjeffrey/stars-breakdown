#!/usr/bin/env python3
"""Package Automitron E2 multi-order repair for VC++ 1.52c / Win98."""

import argparse
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
WINSTUB = ROOT / "compat/vc152/do-robotoid-ai-turn-integration/WINSTUB.EXE"


def write_text(path, content):
    path.write_bytes(content.replace("\r\n", "\n").replace("\r", "\n")
                     .replace("\n", "\r\n").encode("ascii"))


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--guest-dir", required=True)
    args = parser.parse_args()
    if not args.guest_dir.startswith("C:\\"):
        raise ValueError("--guest-dir must be a C:\\ path")
    output = args.output.resolve()
    output.mkdir(parents=True, exist_ok=False)
    for source, name in (
            (ROOT / "reconstructed/include/core.h", "CORE.H"),
            (ROOT / "reconstructed/src/ai_plord.c", "PLORD.C"),
            (ROOT / "reconstructed/src/do_automitron_ai_turn.c", "AUTO.C"),
            (ROOT / "tests/do_automitron_ai_turn_multi_order_repair_probe.c", "PROBE.C")):
        write_text(output / name, source.read_text(encoding="ascii"))
    (output / "WINSTUB.EXE").write_bytes(WINSTUB.read_bytes())
    write_text(output / "AUTREP.C", """\
#include <windows.h>
#include <stdio.h>
#define AUTOMITRON_MULTI_NATIVE
#include "PROBE.C"
#include "PLORD.C"
#include "AUTO.C"

int PASCAL WinMain(HINSTANCE a, HINSTANCE b, LPSTR c, int d)
{
    FILE *f;
    int result;
    (void)a; (void)b; (void)c; (void)d;
    result = automitron_multi_order_repair_probe();
    f = fopen("AUTREP.TXT", "w");
    if (f) {
        fprintf(f, "AUTOMITRON_MULTI_ORDER_REPAIR_CHECKS=%d FAILURES=%d RESULT=%s\\n",
            checks, failures, result ? "FAIL" : "PASS");
        fclose(f);
    }
    return result;
}
""")
    write_text(output / "AUTREP.DEF", """\
NAME AUTREP WINDOWAPI
DESCRIPTION 'Stars Automitron multi-order repair probe'
EXETYPE WINDOWS
STUB 'WINSTUB.EXE'
CODE PRELOAD MOVEABLE DISCARDABLE
DATA PRELOAD MOVEABLE MULTIPLE
HEAPSIZE 1024
STACKSIZE 8192
""")
    write_text(output / "AUTREP.LNK", """\
AUTREP.OBJ
AUTREP.EXE
AUTREP.MAP
C:\\MSVC\\LIB\\MLIBCEW.LIB C:\\MSVC\\LIB\\LIBW.LIB /NOD /ALIGN:16 /CO /MAP
AUTREP.DEF
""")
    write_text(output / "BUILD.BAT", """\
@echo off
set INCLUDE=C:\\MSVC\\INCLUDE
set LIB=C:\\MSVC\\LIB
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 AUTREP.C > AUTCL.LOG
if errorlevel 1 goto fail
C:\\MSVC\\BIN\\LINK.EXE @AUTREP.LNK > AUTLINK.LOG
if errorlevel 1 goto fail
echo COMPILE=PASS > BLDSTAT.TXT
echo LINK=PASS >> BLDSTAT.TXT
goto done
:fail
echo COMPILE=FAIL > BLDSTAT.TXT
echo LINK=FAIL >> BLDSTAT.TXT
:done
""")
    write_text(output / "RUNBLD.BAT", "@echo off\ncd " + args.guest_dir +
               "\ncall BUILD.BAT\n")
    write_text(output / "RUN.BAT", "@echo off\ncd " + args.guest_dir +
               "\nAUTREP.EXE\n")


if __name__ == "__main__":
    main()
