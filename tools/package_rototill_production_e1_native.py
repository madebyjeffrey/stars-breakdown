#!/usr/bin/env python3
"""Package current Rototill E1 production source probe for VC++ 1.52c."""

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
        ("reconstructed/src/do_rototill_ai_turn.c", "ROT.C"),
        ("tests/do_rototill_production_e1_probe.c", "PROBE.C"),
        ("tests/do_rototill_packed_fleet_e2_probe.c", "FLPROBE.C"),
        ("tests/do_rototill_live_prod_queue_e1_probe.c", "LQPROBE.C"),
        ("tests/do_rototill_multi_order_e3_probe.c", "MOPROBE.C"),
        ("tests/do_rototill_transport_reroute_e3_probe.c", "RRPROBE.C"),
        ("tests/do_rototill_deep_space_first_kind_e3_probe.c", "FKPROBE.C"),
        ("tests/do_rototill_scout_dispatch_e2_probe.c", "SCPROBE.C"),
        ("tests/do_rototill_colonizer_marker_e3_probe.c", "CMPROBE.C"),
    )
    payloads = [(name, (ROOT / source).read_text(encoding="ascii"))
                for source, name in sources]
    stub = WINSTUB.read_bytes()
    output.mkdir(parents=True)
    for name, content in payloads:
        write_dos(output / name, content)
    (output / "WINSTUB.EXE").write_bytes(stub)
    write_dos(output / "RTE1.C", """\
#include <windows.h>
#include <stdio.h>

int rototill_production_probe(void);
int rototill_packed_fleet_probe(void);
int rototill_live_queue_probe(void);
int rototill_multi_order_probe(void);
int rototill_reroute_probe(void);
int rototill_first_kind_probe(void);
int rototill_scout_probe(void);
int rototill_colonizer_marker_probe(void);

/* Probe summaries use printf on the host.  This Win16 GUI driver writes its
 * own result file, so no console output is needed from the linked probes. */
int printf(const char *format, ...)
{
    (void)format;
    return 0;
}

int PASCAL WinMain(HINSTANCE a, HINSTANCE b, LPSTR c, int d)
{
    FILE *report;
    int production_result;
    int fleet_result;
    int queue_result;
    int multi_result;
    int reroute_result;
    int first_kind_result;
    int scout_result;
    int colonizer_result;
    (void)a; (void)b; (void)c; (void)d;
    production_result = rototill_production_probe();
    fleet_result = rototill_packed_fleet_probe();
    queue_result = rototill_live_queue_probe();
    multi_result = rototill_multi_order_probe();
    reroute_result = rototill_reroute_probe();
    first_kind_result = rototill_first_kind_probe();
    scout_result = rototill_scout_probe();
    colonizer_result = rototill_colonizer_marker_probe();
    report = fopen("RTE1RES.TXT", "w");
    if (report) {
        fprintf(report, "PRODUCTION_CHECKS=20 FLEET_CHECKS=17 LIVE_QUEUE_CHECKS=12 MULTI_ORDER_CHECKS=11 REROUTE_CHECKS=14 FIRST_KIND_CHECKS=19 SCOUT_CHECKS=10 COLONIZER_MARKER_CHECKS=9 RESULT=%s\\n",
            (production_result || fleet_result || queue_result ||
             multi_result || reroute_result || first_kind_result ||
             scout_result || colonizer_result) ?
                "FAIL" : "PASS");
        fclose(report);
    }
    return production_result || fleet_result || queue_result ||
        multi_result || reroute_result || first_kind_result ||
        scout_result || colonizer_result || !report;
}
""")
    write_dos(output / "RTE1.DEF", """\
NAME RTE1 WINDOWAPI
DESCRIPTION 'Stars Rototill E1 production probe'
EXETYPE WINDOWS
STUB 'WINSTUB.EXE'
CODE PRELOAD MOVEABLE DISCARDABLE
DATA PRELOAD MOVEABLE MULTIPLE
HEAPSIZE 1024
STACKSIZE 8192
""")
    write_dos(output / "RTE1.LNK", """\
RTE1.OBJ PROBE.OBJ FLPROBE.OBJ LQPROBE.OBJ MOPROBE.OBJ RRPROBE.OBJ FKPROBE.OBJ SCPROBE.OBJ CMPROBE.OBJ ROT.OBJ
RTE1.EXE
RTE1.MAP
C:\\MSVC\\LIB\\MLIBCEW.LIB C:\\MSVC\\LIB\\LIBW.LIB /NOD /ALIGN:16 /CO /MAP
RTE1.DEF
""")
    write_dos(output / "BUILD.BAT", """\
@echo off
if exist BLDSTAT.TXT goto stale
set INCLUDE=C:\\MSVC\\INCLUDE
set LIB=C:\\MSVC\\LIB
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 /Dmain=rototill_production_probe PROBE.C > PROBECL.LOG
if errorlevel 1 goto compile_fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 /Dmain=rototill_packed_fleet_probe FLPROBE.C > FLEETCL.LOG
if errorlevel 1 goto compile_fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 /Dmain=rototill_live_queue_probe LQPROBE.C > LQCL.LOG
if errorlevel 1 goto compile_fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 /DTARGET_NATIVE /Dmain=rototill_multi_order_probe MOPROBE.C > MOCL.LOG
if errorlevel 1 goto compile_fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 /DTARGET_NATIVE /Dmain=rototill_reroute_probe RRPROBE.C > RRCL.LOG
if errorlevel 1 goto compile_fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 /DTARGET_NATIVE /Dmain=rototill_first_kind_probe FKPROBE.C > FKCL.LOG
if errorlevel 1 goto compile_fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 /DTARGET_NATIVE /Dmain=rototill_scout_probe SCPROBE.C > SCCL.LOG
if errorlevel 1 goto compile_fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 /DTARGET_NATIVE /Dmain=rototill_colonizer_marker_probe CMPROBE.C > CMCL.LOG
if errorlevel 1 goto compile_fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 ROT.C > ROTCL.LOG
if errorlevel 1 goto compile_fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 RTE1.C > DRVCL.LOG
if errorlevel 1 goto compile_fail
echo COMPILE=PASS > BLDSTAT.TXT
C:\\MSVC\\BIN\\LINK.EXE @RTE1.LNK > LINK.LOG
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
              "\nRTE1.EXE\n")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--guest-dir", required=True)
    args = parser.parse_args()
    package(args.output, args.guest_dir)


if __name__ == "__main__":
    main()
