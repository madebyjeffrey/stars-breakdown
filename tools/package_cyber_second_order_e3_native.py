#!/usr/bin/env python3
"""Package the Cyber second-order caller probe for VC++ 1.52c."""

import argparse
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
WINSTUB = ROOT / "compat/vc152/do-robotoid-ai-turn-integration/WINSTUB.EXE"


def dos(path, content):
    path.write_bytes(content.replace("\r\n", "\n").replace("\r", "\n")
                     .replace("\n", "\r\n").encode("ascii"))


def package(output, guest_dir):
    output = Path(output)
    if not guest_dir.startswith("C:\\") or len(guest_dir) > 32:
        raise ValueError("guest directory must be a short C:\\ path")
    if output.exists():
        raise ValueError("package directory exists; use a fresh directory")
    output.mkdir(parents=True)
    for source, name in (
        ("reconstructed/include/core.h", "CORE.H"),
        ("reconstructed/src/do_cyber_ai_turn.c", "CYB.C"),
        ("reconstructed/src/ai_plord.c", "PLORD.C"),
        ("tests/do_cyber_colonizer_second_order_e3_probe.c", "PROBE.C"),
    ):
        dos(output / name, (ROOT / source).read_text(encoding="ascii"))
    (output / "WINSTUB.EXE").write_bytes(WINSTUB.read_bytes())
    dos(output / "DRV.C", """\
#include <windows.h>
#include <stdio.h>
int cyber_second_order_probe(void);
int PASCAL WinMain(HINSTANCE a, HINSTANCE b, LPSTR c, int d)
{
    FILE *report;
    int result;
    (void)a; (void)b; (void)c; (void)d;
    result = cyber_second_order_probe();
    report = fopen("CYBE3RES.TXT", "w");
    if (report) {
        fprintf(report, "CYBER_SECOND_ORDER_E3_CHECKS=200 RESULT=%s\\n",
            result ? "FAIL" : "PASS");
        fclose(report);
    }
    return result || !report;
}
""")
    dos(output / "CYBE3.DEF", """\
NAME CYBE3 WINDOWAPI
DESCRIPTION 'Stars Cyber second order probe'
EXETYPE WINDOWS
STUB 'WINSTUB.EXE'
CODE PRELOAD MOVEABLE DISCARDABLE
DATA PRELOAD MOVEABLE MULTIPLE
HEAPSIZE 1024
STACKSIZE 8192
""")
    dos(output / "CYBE3.LNK", """\
DRV.OBJ PROBE.OBJ CYB.OBJ PLORD.OBJ
CYBE3.EXE
CYBE3.MAP
C:\\MSVC\\LIB\\MLIBCEW.LIB C:\\MSVC\\LIB\\LIBW.LIB /NOD /ALIGN:16 /CO /MAP
CYBE3.DEF
""")
    dos(output / "BUILD.BAT", """\
@echo off
if exist BLDSTAT.TXT goto stale
set INCLUDE=C:\\MSVC\\INCLUDE
set LIB=C:\\MSVC\\LIB
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 /DTARGET_NATIVE /Dmain=cyber_second_order_probe PROBE.C > PROBECL.LOG
if errorlevel 1 goto compile_fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 CYB.C > CYBCL.LOG
if errorlevel 1 goto compile_fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 PLORD.C > PLORDCL.LOG
if errorlevel 1 goto compile_fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 DRV.C > DRVCL.LOG
if errorlevel 1 goto compile_fail
echo COMPILE=PASS > BLDSTAT.TXT
C:\\MSVC\\BIN\\LINK.EXE @CYBE3.LNK > LINK.LOG
if errorlevel 1 goto link_fail
echo LINK=PASS >> BLDSTAT.TXT
goto done
:compile_fail
echo COMPILE=FAIL > BLDSTAT.TXT
echo LINK=SKIP >> BLDSTAT.TXT
goto done
:link_fail
echo LINK=FAIL >> BLDSTAT.TXT
goto done
:stale
echo STALE=REFUSED > STALE.TXT
:done
""")
    dos(output / "RUNBLD.BAT", "@echo off\ncd " + guest_dir +
        "\ncall BUILD.BAT\n")
    dos(output / "RUN.BAT", "@echo off\ncd " + guest_dir +
        "\nCYBE3.EXE\n")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--guest-dir", required=True)
    args = parser.parse_args()
    package(args.output, args.guest_dir)
