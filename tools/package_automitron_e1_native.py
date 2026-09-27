#!/usr/bin/env python3
"""Package current Automitron E1/E3 probes for VC++ 1.52c Win98."""

import argparse
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
WINSTUB = ROOT / "compat/vc152/do-robotoid-ai-turn-integration/WINSTUB.EXE"


def write_dos(path, content):
    path.write_bytes(content.replace("\r\n", "\n").replace("\r", "\n")
                     .replace("\n", "\r\n").encode("ascii"))


def package(output, guest_dir):
    output = Path(output)
    if not guest_dir.startswith("C:\\") or len(guest_dir) > 32:
        raise ValueError("guest directory must be a short C:\\ path")
    if output.exists():
        raise ValueError("package directory exists; use a fresh directory")
    sources = (
        ("reconstructed/include/core.h", "CORE.H"),
        ("reconstructed/src/ai_plord.c", "PLD.C"),
        ("reconstructed/src/do_automitron_ai_turn.c", "AUT.C"),
        ("tests/do_automitron_colonizer_seed_probe.c", "CO.C"),
        ("tests/do_automitron_ai_planet_e1_probe.c", "AI.C"),
        ("tests/do_automitron_ai_planet_table_e1_probe.c", "AT.C"),
        ("tests/do_automitron_support_queue_e1_probe.c", "SUP.C"),
        ("tests/do_automitron_planet_census_e1_probe.c", "CEN.C"),
        ("tests/do_automitron_early_queue_e1_probe.c", "ERQ.C"),
        ("tests/do_automitron_hull_batches_e1_probe.c", "HUL.C"),
        ("tests/do_automitron_prod_queue_gate_e1_probe.c", "QGT.C"),
        ("tests/do_automitron_production_lifecycle_e1_probe.c", "LFC.C"),
        ("tests/do_automitron_planet_values_e1_probe.c", "PVL.C"),
        ("tests/do_automitron_fleet_flags_e3_probe.c", "FLG.C"),
        ("tests/do_automitron_packed_route_e3_probe.c", "RTE.C"),
        ("tests/do_automitron_scout_fuel_e3_probe.c", "SCT.C"),
    )
    payloads = [(name, (ROOT / source).read_text(encoding="ascii"))
                for source, name in sources]
    stub = WINSTUB.read_bytes()
    output.mkdir(parents=True)
    for name, content in payloads:
        write_dos(output / name, content)
    (output / "WINSTUB.EXE").write_bytes(stub)
    write_dos(output / "AUE1.C", """\
#include <windows.h>
#include <stdio.h>

int colonizer_probe(void);
int ai_planet_probe(void);
int ai_planet_table_probe(void);
int support_queue_probe(void);
int planet_census_probe(void);
int early_queue_probe(void);
int hull_batches_probe(void);
int prod_queue_gate_probe(void);
int production_lifecycle_probe(void);
int planet_values_probe(void);
int fleet_flags_probe(void);
int packed_route_probe(void);
int scout_fuel_probe(void);

int PASCAL WinMain(HINSTANCE a, HINSTANCE b, LPSTR c, int d)
{
    FILE *report;
    int colonizer;
    int planet;
    int table;
    int support;
    int census;
    int early;
    int hulls;
    int gate;
    int lifecycle;
    int values;
    int fleet;
    int route;
    int scout;
    (void)a; (void)b; (void)c; (void)d;
    colonizer = colonizer_probe();
    planet = ai_planet_probe();
    table = ai_planet_table_probe();
    support = support_queue_probe();
    census = planet_census_probe();
    early = early_queue_probe();
    hulls = hull_batches_probe();
    gate = prod_queue_gate_probe();
    lifecycle = production_lifecycle_probe();
    values = planet_values_probe();
    fleet = fleet_flags_probe();
    route = packed_route_probe();
    scout = scout_fuel_probe();
    report = fopen("AUE1RES.TXT", "w");
    if (report) {
        fprintf(report, "COLONIZER_CHECKS=19 RESULT=%s\\n",
            colonizer ? "FAIL" : "PASS");
        fprintf(report, "AI_PLANET_CHECKS=14 RESULT=%s\\n",
            planet ? "FAIL" : "PASS");
        fprintf(report, "AI_TABLE_CHECKS=8 RESULT=%s\\n",
            table ? "FAIL" : "PASS");
        fprintf(report, "SUPPORT_CHECKS=15 RESULT=%s\\n",
            support ? "FAIL" : "PASS");
        fprintf(report, "CENSUS_CHECKS=5 RESULT=%s\\n",
            census ? "FAIL" : "PASS");
        fprintf(report, "EARLY_QUEUE_CHECKS=17 RESULT=%s\\n",
            early ? "FAIL" : "PASS");
        fprintf(report, "HULL_BATCH_CHECKS=18 RESULT=%s\\n",
            hulls ? "FAIL" : "PASS");
        fprintf(report, "PROD_GATE_CHECKS=13 RESULT=%s\\n",
            gate ? "FAIL" : "PASS");
        fprintf(report, "LIFECYCLE_CHECKS=16 RESULT=%s\\n",
            lifecycle ? "FAIL" : "PASS");
        fprintf(report, "PLANET_VALUES_CHECKS=8 RESULT=%s\\n",
            values ? "FAIL" : "PASS");
        fprintf(report, "FLEET_FLAGS_CHECKS=8 RESULT=%s\\n",
            fleet ? "FAIL" : "PASS");
        fprintf(report, "PACKED_ROUTE_CHECKS=10 RESULT=%s\\n",
            route ? "FAIL" : "PASS");
        fprintf(report, "SCOUT_FUEL_CHECKS=15 RESULT=%s\\n",
            scout ? "FAIL" : "PASS");
        fprintf(report, "RESULT=%s\\n",
            colonizer || planet || table || support || census || early ||
            hulls || gate || lifecycle || values ||
            fleet || route || scout ? "FAIL" : "PASS");
        fclose(report);
    }
    return colonizer || planet || table || support || census || early ||
        hulls || gate || lifecycle || values ||
        fleet || route || scout || !report;
}
""")
    write_dos(output / "AUE1.DEF", """\
NAME AUE1 WINDOWAPI
DESCRIPTION 'Stars Automitron E1 source probes'
EXETYPE WINDOWS
STUB 'WINSTUB.EXE'
CODE PRELOAD MOVEABLE DISCARDABLE
DATA PRELOAD MOVEABLE MULTIPLE
HEAPSIZE 1024
STACKSIZE 8192
""")
    write_dos(output / "AUE1.LNK", """\
AUE1.OBJ CO.OBJ AI.OBJ AT.OBJ SUP.OBJ CEN.OBJ ERQ.OBJ HUL.OBJ QGT.OBJ LFC.OBJ PVL.OBJ FLG.OBJ RTE.OBJ SCT.OBJ AUT.OBJ PLD.OBJ
AUE1.EXE
AUE1.MAP
C:\\MSVC\\LIB\\MLIBCEW.LIB C:\\MSVC\\LIB\\LIBW.LIB /NOD /ALIGN:16 /CO /MAP
AUE1.DEF
""")
    write_dos(output / "BUILD.BAT", """\
@echo off
if exist BLDSTAT.TXT goto stale
set INCLUDE=C:\\MSVC\\INCLUDE
set LIB=C:\\MSVC\\LIB
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 /Dmain=colonizer_probe CO.C > COLCL.LOG
if errorlevel 1 goto compile_fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 /Dmain=ai_planet_probe AI.C > AICL.LOG
if errorlevel 1 goto compile_fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 /Dmain=ai_planet_table_probe AT.C > AITCL.LOG
if errorlevel 1 goto compile_fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 /Dmain=support_queue_probe SUP.C > SUPCL.LOG
if errorlevel 1 goto compile_fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 /Dmain=planet_census_probe CEN.C > CENCL.LOG
if errorlevel 1 goto compile_fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 /Dmain=early_queue_probe ERQ.C > ERQCL.LOG
if errorlevel 1 goto compile_fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 /Dmain=hull_batches_probe HUL.C > HULCL.LOG
if errorlevel 1 goto compile_fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 /Dmain=prod_queue_gate_probe QGT.C > QGTCL.LOG
if errorlevel 1 goto compile_fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 /Dmain=production_lifecycle_probe LFC.C > LFCCL.LOG
if errorlevel 1 goto compile_fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 /Dmain=planet_values_probe PVL.C > PVLCL.LOG
if errorlevel 1 goto compile_fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 /Dmain=fleet_flags_probe FLG.C > FLCL.LOG
if errorlevel 1 goto compile_fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 /Dmain=packed_route_probe RTE.C > RTCL.LOG
if errorlevel 1 goto compile_fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 /Dmain=scout_fuel_probe SCT.C > SCCL.LOG
if errorlevel 1 goto compile_fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 AUT.C > AUTOCL.LOG
if errorlevel 1 goto compile_fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 PLD.C > PLCL.LOG
if errorlevel 1 goto compile_fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 AUE1.C > DRVCL.LOG
if errorlevel 1 goto compile_fail
echo COMPILE=PASS > BLDSTAT.TXT
C:\\MSVC\\BIN\\LINK.EXE @AUE1.LNK > LINK.LOG
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
              "\nAUE1.EXE\n")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--guest-dir", required=True)
    args = parser.parse_args()
    package(args.output, args.guest_dir)


if __name__ == "__main__":
    main()
