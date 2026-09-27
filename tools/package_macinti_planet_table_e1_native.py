#!/usr/bin/env python3
"""Package the bounded Macinti planet-table E1 probe for VC++ 1.52c."""

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
        ("reconstructed/src/do_macinti_ai_turn.c", "MAI.C"),
        ("reconstructed/src/ai_plord.c", "PLORD.C"),
        ("tests/do_macinti_planet_table_e1_probe.c", "PROBE.C"),
        ("tests/do_macinti_packed_fleet_e2_probe.c", "FLPROBE.C"),
        ("tests/do_macinti_first_transport_e2_probe.c", "FSPROBE.C"),
        ("tests/do_macinti_buddy_miner_e2_probe.c", "BUDPROBE.C"),
        ("tests/do_macinti_attack_pressure_queue_e1_probe.c", "PRSPROBE.C"),
        ("tests/do_macinti_far_waypoint_e3_probe.c", "WPPROBE.C"),
        ("tests/do_macinti_early_order_e3_probe.c", "EOPROBE.C"),
        ("tests/do_macinti_attack_gate_e1_probe.c", "GATPROBE.C"),
    )
    payloads = [(name, (ROOT / source).read_text(encoding="ascii"))
                for source, name in sources]
    stub = WINSTUB.read_bytes()
    output.mkdir(parents=True)
    for name, content in payloads:
        write_dos(output / name, content)
    (output / "WINSTUB.EXE").write_bytes(stub)
    write_dos(output / "MAE1.C", """\
#include <windows.h>
#include <stdio.h>

/* Win16's Windows CRT omits console printf.  Probe success is recorded in
 * MAE1RES.TXT by WinMain, so discard only their console summaries. */
int printf(const char *format, ...)
{
    (void)format;
    return 0;
}

int macinti_planet_table_probe(void);
int macinti_packed_fleet_probe(void);
int macinti_first_transport_probe(void);
int macinti_buddy_miner_probe(void);
int macinti_attack_pressure_probe(void);
int macinti_far_waypoint_probe(void);
int macinti_early_order_probe(void);
int macinti_attack_gate_probe(void);

int PASCAL WinMain(HINSTANCE a, HINSTANCE b, LPSTR c, int d)
{
    FILE *report;
    int result;
    int fleet_result;
    int first_result;
    int buddy_result;
    int pressure_result;
    int waypoint_result;
    int early_order_result;
    int gate_result;
    (void)a; (void)b; (void)c; (void)d;
    result = macinti_planet_table_probe();
    fleet_result = macinti_packed_fleet_probe();
    first_result = macinti_first_transport_probe();
    buddy_result = macinti_buddy_miner_probe();
    pressure_result = macinti_attack_pressure_probe();
    waypoint_result = macinti_far_waypoint_probe();
    early_order_result = macinti_early_order_probe();
    gate_result = macinti_attack_gate_probe();
    report = fopen("MAE1RES.TXT", "w");
    if (report) {
        fprintf(report, "PLANET_E1=37 PRESS_E1=19 GATE_E1=39 FLEET_E2=12 FIRST_E2=10 BUDDY_E2=16 WAYPOINT_E3=8 ORDER_E3=14 RESULT=%s\\n",
            (result || pressure_result || fleet_result ||
             first_result || buddy_result ||
             waypoint_result ||
             early_order_result || gate_result) ?
            "FAIL" : "PASS");
        fclose(report);
    }
    return result || pressure_result || fleet_result || first_result || buddy_result ||
        waypoint_result || early_order_result || gate_result || !report;
}
""")
    write_dos(output / "MAE1.DEF", """\
NAME MAE1 WINDOWAPI
DESCRIPTION 'Stars Macinti planet-table probe'
EXETYPE WINDOWS
STUB 'WINSTUB.EXE'
CODE PRELOAD MOVEABLE DISCARDABLE
DATA PRELOAD MOVEABLE MULTIPLE
HEAPSIZE 1024
STACKSIZE 8192
""")
    write_dos(output / "MAE1.LNK", """\
MAE1.OBJ PROBE.OBJ PRSPROBE.OBJ GATPROBE.OBJ FLPROBE.OBJ FSPROBE.OBJ BUDPROBE.OBJ WPPROBE.OBJ EOPROBE.OBJ MAI.OBJ PLORD.OBJ
MAE1.EXE
MAE1.MAP
C:\\MSVC\\LIB\\MLIBCEW.LIB C:\\MSVC\\LIB\\LIBW.LIB /NOD /ALIGN:16 /CO /MAP
MAE1.DEF
""")
    write_dos(output / "BUILD.BAT", """\
@echo off
if exist BLDSTAT.TXT goto stale
set INCLUDE=C:\\MSVC\\INCLUDE
set LIB=C:\\MSVC\\LIB
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 /Dmain=macinti_planet_table_probe PROBE.C > PROBECL.LOG
if errorlevel 1 goto compile_fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 /Dmain=macinti_attack_pressure_probe PRSPROBE.C > PRSCL.LOG
if errorlevel 1 goto compile_fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 /Dmain=macinti_attack_gate_probe GATPROBE.C > GATCL.LOG
if errorlevel 1 goto compile_fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 /Dmain=macinti_packed_fleet_probe FLPROBE.C > FLEETCL.LOG
if errorlevel 1 goto compile_fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 /Dmain=macinti_first_transport_probe FSPROBE.C > FSCL.LOG
if errorlevel 1 goto compile_fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 /Dmain=macinti_buddy_miner_probe BUDPROBE.C > BUDCL.LOG
if errorlevel 1 goto compile_fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 /Dmain=macinti_far_waypoint_probe WPPROBE.C > WPCL.LOG
if errorlevel 1 goto compile_fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 /Dmain=macinti_early_order_probe EOPROBE.C > EOCL.LOG
if errorlevel 1 goto compile_fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 MAI.C > MAICL.LOG
if errorlevel 1 goto compile_fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 PLORD.C > PLORDCL.LOG
if errorlevel 1 goto compile_fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 MAE1.C > DRVCL.LOG
if errorlevel 1 goto compile_fail
echo COMPILE=PASS > BLDSTAT.TXT
C:\\MSVC\\BIN\\LINK.EXE @MAE1.LNK > LINK.LOG
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
              "\nMAE1.EXE\n")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--guest-dir", required=True)
    args = parser.parse_args()
    package(args.output, args.guest_dir)


if __name__ == "__main__":
    main()
