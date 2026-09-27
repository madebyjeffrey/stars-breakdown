#!/usr/bin/env python3
"""Package current ExecuteReportClick GlobalPD probe for VC++ 1.52c."""

import argparse
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
WINSTUB = (ROOT / "compat/vc152/execute-report-click-globalpd-e1-native-r2"
           / "WINSTUB.EXE")


def write_text(path, content):
    path.write_bytes(content.replace("\r\n", "\n").replace("\r", "\n")
                     .replace("\n", "\r\n").encode("ascii"))


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--guest-dir", required=True)
    parser.add_argument("--stem", default="GPDE3")
    args = parser.parse_args()
    if not args.guest_dir.startswith("C:\\"):
        raise ValueError("--guest-dir must be a C:\\ path")
    output = args.output.resolve()
    stem = args.stem
    if not stem.isascii() or not stem.isalnum() or len(stem) > 8:
        raise ValueError("--stem must be a DOS 8.3-compatible basename")
    output.mkdir(parents=True, exist_ok=False)
    def write_named(name, content):
        write_text(output / name.replace("GPDE3", stem),
                   content.replace("GPDE3", stem))

    for source, name in (
            (ROOT / "reconstructed/include/core.h", "CORE.H"),
            (ROOT / "reconstructed/src/execute_report_click.c", "ERCLICK.C"),
            (ROOT / "tests/execute_report_click_globalpd_probe.c", "PROBE.C")):
        write_text(output / name, source.read_text(encoding="ascii"))
    (output / "WINSTUB.EXE").write_bytes(WINSTUB.read_bytes())
    write_named("GPDE3.C", """\
#include <windows.h>
#include <stdio.h>
#define TARGET_NATIVE
#define main execute_report_click_globalpd_probe
#include "PROBE.C"
#undef main
#include "ERCLICK.C"

int PASCAL WinMain(HINSTANCE a, HINSTANCE b, LPSTR c, int d)
{
    FILE *f;
    int r;
    (void)a; (void)b; (void)c; (void)d;
    r = execute_report_click_globalpd_probe();
    f = fopen("GPDE3.TXT", "w");
    if (f) {
        fprintf(f,
            "EXECUTE_REPORT_CLICK_GLOBALPD_CHECKS=%d FAILURES=%d RESULT=%s\\n",
            checks, failures, r ? "FAIL" : "PASS");
        fclose(f);
    }
    return r;
}
""")
    write_named("GPDE3.DEF", """\
NAME GPDE3
DESCRIPTION 'Stars ExecuteReportClick GlobalPD resource union probe'
EXETYPE WINDOWS
STUB 'WINSTUB.EXE'
CODE PRELOAD MOVEABLE DISCARDABLE
DATA PRELOAD MOVEABLE MULTIPLE
HEAPSIZE 1024
STACKSIZE 8192
""")
    write_named("GPDE3.LNK", """\
GPDE3.OBJ
GPDE3.EXE
GPDE3.MAP
C:\\MSVC\\LIB\\MLIBCEW.LIB C:\\MSVC\\LIB\\LIBW.LIB /NOD /ALIGN:16 /CO /MAP
GPDE3.DEF
""")
    write_named("BUILD.BAT", """\
@echo off
set INCLUDE=C:\\MSVC\\INCLUDE
set LIB=C:\\MSVC\\LIB
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 GPDE3.C > GPDE3CL.LOG
if errorlevel 1 goto fail
C:\\MSVC\\BIN\\LINK.EXE @GPDE3.LNK > GPDE3LINK.LOG
if errorlevel 1 goto fail
echo COMPILE=PASS > GPDE3STA.TXT
echo LINK=PASS >> GPDE3STA.TXT
goto done
:fail
echo COMPILE=FAIL > GPDE3STA.TXT
echo LINK=FAIL >> GPDE3STA.TXT
:done
""")
    write_named("RUNBLD.BAT", "@echo off\ncd " + args.guest_dir +
               "\ncall BUILD.BAT\n")
    write_named("RUN.BAT", "@echo off\ncd " + args.guest_dir +
               "\nGPDE3.EXE\n")


if __name__ == "__main__":
    main()
