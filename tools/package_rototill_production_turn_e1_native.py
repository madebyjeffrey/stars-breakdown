#!/usr/bin/env python3
"""Package the retained Rototill queue/Produce boundary for VC++ 1.52c."""

import argparse
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
WINSTUB = ROOT / "compat/vc152/do-robotoid-ai-turn-integration/WINSTUB.EXE"


def write_dos(path, content):
    path.write_bytes(content.replace("\r\n", "\n").replace("\r", "\n")
                     .replace("\n", "\r\n").encode("ascii"))


def package(output, guest_dir):
    if output.exists():
        raise ValueError("package directory exists; use a fresh directory")
    if not guest_dir.startswith("C:\\") or len(guest_dir) > 32:
        raise ValueError("guest directory must be a short C:\\ path")
    files = (
        ("reconstructed/include/core.h", "CORE.H"),
        ("tests/do_rototill_integrated_production_native_probe.c", "PROBE.C"),
        ("reconstructed/src/ai.c", "AI.C"),
        ("reconstructed/src/battle.c", "BATTLE.C"),
        ("reconstructed/src/utilgen.c", "UTILGEN.C"),
        ("reconstructed/src/seed_table.inc", "SEEDTAB.INC"),
        ("reconstructed/src/turn2.c", "TURN2.C"),
        ("reconstructed/src/fill_prod_mines_and_factories.c", "FILL.C"),
        ("reconstructed/src/produce.c", "PRODUCE.C"),
        ("reconstructed/src/c_build_prod_item.c", "BUILD.C"),
        ("reconstructed/src/planet.c", "PLANET.C"),
        ("reconstructed/src/innate_fp.h", "INNATE.H"),
        ("reconstructed/src/planet_max_pop.c", "MAXPOP.C"),
        ("reconstructed/src/get_production_costs.c", "COSTS.C"),
    )
    output.mkdir(parents=True)
    for source, target in files:
        content = (ROOT / source).read_text(encoding="ascii")
        if target == "UTILGEN.C":
            content = content.replace('"seed_table.inc"', '"SEEDTAB.INC"')
        if target == "PLANET.C":
            content = content.replace('"innate_fp.h"', '"INNATE.H"')
        write_dos(output / target, content)
    (output / "WINSTUB.EXE").write_bytes(WINSTUB.read_bytes())
    write_dos(output / "DRIVER.C", """\
#include <windows.h>
#include <stdio.h>

int rototill_native_production_probe(void);

int PASCAL WinMain(HINSTANCE a, HINSTANCE b, LPSTR c, int d)
{
    FILE *report;
    int failures;
    (void)a; (void)b; (void)c; (void)d;
    failures = rototill_native_production_probe();
    report = fopen("RTE1RES.TXT", "w");
    if (report) {
        fprintf(report, "PRODUCTION_PATH=%s FAILURES=%d\\n",
            failures ? "FAIL" : "PASS", failures);
        fclose(report);
    }
    return failures || !report;
}
""")
    write_dos(output / "RTE1T.DEF", """\
NAME RTE1T WINDOWAPI
DESCRIPTION 'Stars Rototill native queue and Produce boundary'
EXETYPE WINDOWS
STUB 'WINSTUB.EXE'
CODE PRELOAD MOVEABLE DISCARDABLE
DATA PRELOAD MOVEABLE MULTIPLE
HEAPSIZE 2048
STACKSIZE 8192
""")
    write_dos(output / "RTE1T.LNK", """\
DRIVER.OBJ PROBE.OBJ AI.OBJ BATTLE.OBJ UTILGEN.OBJ TURN2.OBJ FILL.OBJ PRODUCE.OBJ BUILD.OBJ PLANET.OBJ MAXPOP.OBJ COSTS.OBJ
RTE1T.EXE
RTE1T.MAP
C:\\MSVC\\LIB\\MLIBCEW.LIB C:\\MSVC\\LIB\\LIBW.LIB /NOD /ALIGN:16 /CO /MAP
RTE1T.DEF
""")
    write_dos(output / "BUILD.BAT", """\
@echo off
if exist BLDSTAT.TXT goto stale
set INCLUDE=C:\\MSVC\\INCLUDE
set LIB=C:\\MSVC\\LIB
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 /DTARGET_NATIVE PROBE.C > PROBECL.LOG
if errorlevel 1 goto compile_fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 AI.C > AICL.LOG
if errorlevel 1 goto compile_fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 BATTLE.C > BATTLECL.LOG
if errorlevel 1 goto compile_fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 UTILGEN.C > UTILCL.LOG
if errorlevel 1 goto compile_fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 TURN2.C > TURN2CL.LOG
if errorlevel 1 goto compile_fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 FILL.C > FILLCL.LOG
if errorlevel 1 goto compile_fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 PRODUCE.C > PRODCL.LOG
if errorlevel 1 goto compile_fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 BUILD.C > BUILDCL.LOG
if errorlevel 1 goto compile_fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 PLANET.C > PLANETCL.LOG
if errorlevel 1 goto compile_fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 MAXPOP.C > MAXPOPCL.LOG
if errorlevel 1 goto compile_fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 COSTS.C > COSTSCL.LOG
if errorlevel 1 goto compile_fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 DRIVER.C > DRIVERCL.LOG
if errorlevel 1 goto compile_fail
echo COMPILE=PASS > BLDSTAT.TXT
C:\\MSVC\\BIN\\LINK.EXE @RTE1T.LNK > LINK.LOG
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
    write_dos(output / "RUNBLD.BAT", "@echo off\ncd " + guest_dir +
              "\ncall BUILD.BAT\n")
    write_dos(output / "RUN.BAT", "@echo off\ncd " + guest_dir +
              "\nRTE1T.EXE\n")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--guest-dir", required=True)
    args = parser.parse_args()
    package(args.output, args.guest_dir)


if __name__ == "__main__":
    main()
