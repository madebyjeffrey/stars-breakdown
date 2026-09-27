#!/usr/bin/env python3
"""Package the read-only Win16 original GlobalPD capture helper."""

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
    args = parser.parse_args()
    if not args.guest_dir.startswith("C:\\"):
        raise ValueError("--guest-dir must be a C:\\ path")
    output = args.output.resolve()
    output.mkdir(parents=True, exist_ok=False)
    write_text(output / "CAPGPD.C", (ROOT / "tests/globalpd_original_capture_win16.c")
               .read_text(encoding="ascii"))
    (output / "WINSTUB.EXE").write_bytes(WINSTUB.read_bytes())
    write_text(output / "CAPGPD.DEF", """\
NAME CAPGPD WINDOWAPI
DESCRIPTION 'Read-only Stars GlobalPD capture'
EXETYPE WINDOWS
STUB 'WINSTUB.EXE'
CODE PRELOAD MOVEABLE DISCARDABLE
DATA PRELOAD MOVEABLE MULTIPLE
HEAPSIZE 1024
STACKSIZE 8192
""")
    write_text(output / "CAPGPD.LNK", """\
CAPGPD.OBJ
CAPGPD.EXE
CAPGPD.MAP
C:\\MSVC\\LIB\\MLIBCEW.LIB C:\\MSVC\\LIB\\LIBW.LIB C:\\MSVC\\LIB\\TOOLHELP.LIB /NOD /ALIGN:16 /CO /MAP
CAPGPD.DEF
""")
    write_text(output / "BUILD.BAT", """\
@echo off
set INCLUDE=C:\\MSVC\\INCLUDE
set LIB=C:\\MSVC\\LIB
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 CAPGPD.C > CAPCL.LOG
if errorlevel 1 goto fail
C:\\MSVC\\BIN\\LINK.EXE @CAPGPD.LNK > CAPLINK.LOG
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
               "\nCAPGPD.EXE\n")
    write_text(output / "RUNSTAR.BAT", "@echo off\ncd " +
               args.guest_dir + "\nCAPGPD.EXE 190 26\n")
    write_text(output / "RUNPLAN.BAT", "@echo off\ncd " +
               args.guest_dir + "\nCAPGPD.EXE 265 26\n")
    write_text(output / "RUNRES.BAT", "@echo off\ncd " +
               args.guest_dir + "\nCAPGPD.EXE 406 26\n")
    write_text(output / "RUNDEF.BAT", "@echo off\ncd " +
               args.guest_dir + "\nCAPGPD.EXE 171 26\n")
    write_text(output / "RUNFLEET.BAT", "@echo off\ncd " +
               args.guest_dir + "\nCAPGPD.EXE 196 26\n")
    write_text(output / "RUNMIN.BAT", "@echo off\ncd " +
               args.guest_dir + "\nCAPGPD.EXE 214 26\n")
    write_text(output / "RUNINST.BAT", "@echo off\ncd " +
               args.guest_dir + "\nCAPGPD.EXE 410 26\n")
    write_text(output / "RUNMINE.BAT", "@echo off\ncd " +
               args.guest_dir + "\nCAPGPD.EXE 380 26\n")
    write_text(output / "RUNMN2.BAT", "@echo off\ncd " +
               args.guest_dir + "\nCAPGPD.EXE 160 26\n")
    write_text(output / "RUNFC2.BAT", "@echo off\ncd " +
               args.guest_dir + "\nCAPGPD.EXE 205 26\n")


if __name__ == "__main__":
    main()
