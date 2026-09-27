#!/usr/bin/env python3
"""Package real Win16 USER/GDI ExecuteReportClick lifecycle harness."""

import argparse
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
STUB = (ROOT / "compat/vc152/execute-report-click-live-ui-e2-r2"
        / "WINSTUB.EXE")


def write_ascii_crlf(path, content):
    path.write_bytes(content.replace("\r\n", "\n").replace("\r", "\n")
                     .replace("\n", "\r\n").encode("ascii"))


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--guest-dir", required=True)
    args = parser.parse_args()
    if not args.guest_dir.startswith("C:\\"):
        raise ValueError("--guest-dir must be an absolute C: guest directory")
    output = args.output.resolve()
    output.mkdir(parents=True, exist_ok=False)
    for source, name in (
        ("reconstructed/include/core.h", "CORE.H"),
        ("reconstructed/src/execute_report_click.c", "ERCLICK.C"),
        ("reconstructed/src/popup.c", "POPUP.C"),
        ("reconstructed/src/draw_popup.c", "DRAWPOP.C"),
        ("reconstructed/src/popup_wnd_proc.c", "POPWND.C"),
        ("reconstructed/src/report_dlg.c", "REPORT.C"),
        ("tests/execute_report_click_live_user_win16.c", "HARNESS.C"),
    ):
        write_ascii_crlf(output / name, (ROOT / source).read_text(
            encoding="ascii"))
    (output / "WINSTUB.EXE").write_bytes(STUB.read_bytes())
    write_ascii_crlf(output / "LIVEUI.C", """\
#include "HARNESS.C"
#include "ERCLICK.C"
#include "POPUP.C"
#include "DRAWPOP.C"
#include "POPWND.C"
#include "REPORT.C"
""")
    write_ascii_crlf(output / "LIVEUI.DEF", """\
NAME LIVEUI WINDOWAPI
DESCRIPTION 'Stars ExecuteReportClick live Win16 USER/GDI probe'
EXETYPE WINDOWS
STUB 'WINSTUB.EXE'
CODE PRELOAD MOVEABLE DISCARDABLE
DATA PRELOAD MOVEABLE MULTIPLE
HEAPSIZE 1024
STACKSIZE 16384
""")
    write_ascii_crlf(output / "LIVEUI.LNK", """\
LIVEUI.OBJ
LIVEUI.EXE
LIVEUI.MAP
C:\\MSVC\\LIB\\MLIBCEW.LIB C:\\MSVC\\LIB\\LIBW.LIB /NOD /ALIGN:16 /CO /MAP
LIVEUI.DEF
""")
    write_ascii_crlf(output / "BUILD.BAT", """\
@echo off
if exist BLDSTAT.TXT goto stale
set INCLUDE=C:\\MSVC\\INCLUDE
set LIB=C:\\MSVC\\LIB
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 LIVEUI.C > LIVECL.LOG
if errorlevel 1 goto compilefail
echo COMPILE=PASS > BLDSTAT.TXT
C:\\MSVC\\BIN\\LINK.EXE @LIVEUI.LNK > LIVELINK.LOG
if errorlevel 1 goto linkfail
echo LINK=PASS >> BLDSTAT.TXT
goto done
:compilefail
echo COMPILE=FAIL > BLDSTAT.TXT
echo LINK=SKIP >> BLDSTAT.TXT
goto done
:linkfail
echo LINK=FAIL >> BLDSTAT.TXT
goto done
:stale
echo Existing BLDSTAT.TXT; use a fresh guest directory.
:done
""")
    write_ascii_crlf(output / "RUNBLD.BAT", "@echo off\ncd " +
                     args.guest_dir + "\ncall BUILD.BAT\n")
    write_ascii_crlf(output / "RUN.BAT", "@echo off\ncd " +
                     args.guest_dir + "\nLIVEUI.EXE\n")


if __name__ == "__main__":
    main()
