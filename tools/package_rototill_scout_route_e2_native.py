#!/usr/bin/env python3
"""Package the retained Rototill scout route for a fresh VC++ 1.52c run."""

import argparse
import ctypes
from pathlib import Path
import sys

from tests.test_satisfy_orders_turin_binding import (
    hst_wormholes, owner1_fleet_order_pairs, retained_hull_components,
    retained_ideal_warp_data,
    xy_points)
from tools.race_file_compare import Inspector
from tools.stars_game_stream import decode_stream

ROOT = Path(__file__).resolve().parents[1]
RUN = ROOT / "runs/rototill-original-turn-r1"
WINSTUB = ROOT / "compat/vc152/do-robotoid-ai-turn-integration/WINSTUB.EXE"


def write_dos(path, content):
    path.write_bytes(content.replace("\r\n", "\n").replace("\r", "\n")
                     .replace("\n", "\r\n").encode("ascii"))


def fixture_header():
    suffix = "dylib" if sys.platform == "darwin" else "so"
    inspector = Inspector(ctypes.CDLL(str(
        (ROOT / "build" / ("libstars_core." + suffix)).resolve())))
    records = decode_stream(RUN / "input/CLEAN.M2", inspector)
    original = decode_stream(RUN / "output/CLEAN.M2", inspector)
    wormholes = hst_wormholes(RUN / "input/CLEAN.HST", inspector)
    designs, engines = retained_ideal_warp_data(records)
    hulls = retained_hull_components(records)
    before = owner1_fleet_order_pairs(records, 0)[0]
    after = owner1_fleet_order_pairs(original, 1)[0]
    points = xy_points(RUN / "input/CLEAN.XY", inspector)
    if len(points) != 32 or before["fleet"]["id"] != 0 or \
            after["order"]["target_id"] != 26:
        raise ValueError("retained Rototill route fixture changed")
    owners = [-2] * len(points)
    for record in records:
        if record["kind"] == 13 and record["turn"] == 0:
            packed = int.from_bytes(record["body"][:2], "little")
            owners[packed & 2047] = packed >> 11
    if [(index, owner) for index, owner in enumerate(owners)
            if owner != -2] != [(24, 1)]:
        raise ValueError("retained player planet view changed")
    if designs[0] != [1] or len(wormholes) != 4 or hulls[0][0] != 4:
        raise ValueError("retained engine or wormhole fixture changed")
    point_rows = ["    " + ", ".join("{%d, %d}" % point
                  for point in points[index:index + 4]) +
                  ("," if index + 4 < len(points) else "")
                  for index in range(0, len(points), 4)]
    owner_rows = ["    " + ", ".join(str(owner)
                  for owner in owners[index:index + 8]) +
                  ("," if index + 8 < len(owners) else "")
                  for index in range(0, len(owners), 8)]
    return "\n".join([
        "#define SOURCE_X %d" % before["fleet"]["point"][0],
        "#define SOURCE_Y %d" % before["fleet"]["point"][1],
        "#define SOURCE_FUEL %d" % before["fleet"]["cargo"][4],
        "#define EXPECTED_TARGET %d" % after["order"]["target_id"],
        "#define EXPECTED_X %d" % after["order"]["point"][0],
        "#define EXPECTED_Y %d" % after["order"]["point"][1],
        "#define EXPECTED_WARP %d" % after["order"]["warp"],
        "#define RETAINED_ENGINE_ID %d" % designs[0][0],
        "static const StarsHullComponent retained_hull_0[%d] = {" %
        len(hulls[0][1]),
        "    " + ", ".join("{%d, %d}" % component
                          for component in hulls[0][1]),
        "};",
        "static const StarsI16 retained_engine_fuel[12] = {",
        "    " + ", ".join(str(value) for value in engines[1][:6]) + ",",
        "    " + ", ".join(str(value) for value in engines[1][6:]),
        "};",
        "static const StarsPoint retained_points[32] = {",
        *point_rows,
        "};",
        "static const StarsI16 retained_owners[32] = {",
        *owner_rows,
        "};",
        *("static const StarsU8 retained_wormhole_%d[18] = {\n"
          "    %s,\n    %s\n};" % (index,
          ", ".join(str(value) for value in wormhole[:9]),
          ", ".join(str(value) for value in wormhole[9:]))
          for index, wormhole in enumerate(wormholes)),
        "static const StarsU8 *const retained_wormholes[4] = {",
        "    retained_wormhole_0, retained_wormhole_1,",
        "    retained_wormhole_2, retained_wormhole_3",
        "};",
        "",
    ])


def package(output, guest_dir):
    output = Path(output)
    if output.exists():
        raise ValueError("package directory exists; use a fresh directory")
    if not guest_dir.startswith("C:\\") or len(guest_dir) > 32:
        raise ValueError("guest directory must be a short C:\\ path")
    source_files = (
        ("reconstructed/include/core.h", "CORE.H"),
        ("reconstructed/src/do_rototill_ai_turn.c", "ROT.C"),
        ("reconstructed/src/rototill_scout_adapter.c", "SCADAPT.C"),
        ("reconstructed/src/ai.c", "AI.C"),
        ("reconstructed/src/ai_plord.c", "AIPLORD.C"),
        ("reconstructed/src/find_ideal_warp.c", "WARP.C"),
        ("reconstructed/src/battle.c", "BATTLE.C"),
        ("reconstructed/src/utilgen.c", "UTILGEN.C"),
        ("reconstructed/src/seed_table.inc", "SEEDTAB.INC"),
        ("tests/do_rototill_scout_route_e2_probe.c", "SCRPROBE.C"),
    )
    payloads = [(name, (ROOT / source).read_text(encoding="ascii"))
                for source, name in source_files]
    header = fixture_header()
    output.mkdir(parents=True)
    for name, content in payloads:
        if name == "UTILGEN.C":
            content = content.replace('"seed_table.inc"', '"SEEDTAB.INC"')
        write_dos(output / name, content)
    write_dos(output / "ROTSCFX.H", header)
    (output / "WINSTUB.EXE").write_bytes(WINSTUB.read_bytes())
    write_dos(output / "RTE2.C", """\
#include <windows.h>
#include <stdio.h>

int rototill_route_probe(void);
int printf(const char *format, ...)
{
    (void)format;
    return 0;
}

int PASCAL WinMain(HINSTANCE a, HINSTANCE b, LPSTR c, int d)
{
    FILE *report;
    int result;
    (void)a; (void)b; (void)c; (void)d;
    result = rototill_route_probe();
    report = fopen("RTE2RES.TXT", "w");
    if (report) {
        fprintf(report, "ROUTE_CHECKS=180 RESULT=%s\\n",
            result ? "FAIL" : "PASS");
        fclose(report);
    }
    return result || !report;
}
""")
    write_dos(output / "RTE2.DEF", """\
NAME RTE2 WINDOWAPI
DESCRIPTION 'Stars Rototill scout route probe'
EXETYPE WINDOWS
STUB 'WINSTUB.EXE'
CODE PRELOAD MOVEABLE DISCARDABLE
DATA PRELOAD MOVEABLE MULTIPLE
HEAPSIZE 2048
STACKSIZE 8192
""")
    write_dos(output / "RTE2.LNK", """\
RTE2.OBJ SCRPROBE.OBJ ROT.OBJ SCADAPT.OBJ AI.OBJ AIPLORD.OBJ WARP.OBJ BATTLE.OBJ UTILGEN.OBJ
RTE2.EXE
RTE2.MAP
C:\\MSVC\\LIB\\MLIBCEW.LIB C:\\MSVC\\LIB\\LIBW.LIB /NOD /ALIGN:16 /CO /MAP
RTE2.DEF
""")
    write_dos(output / "BUILD.BAT", """\
@echo off
if exist BLDSTAT.TXT goto stale
set INCLUDE=C:\\MSVC\\INCLUDE
set LIB=C:\\MSVC\\LIB
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 /DTARGET_NATIVE /Dmain=rototill_route_probe SCRPROBE.C > SCPCL.LOG
if errorlevel 1 goto compile_fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 ROT.C > ROTCL.LOG
if errorlevel 1 goto compile_fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 SCADAPT.C > ADCL.LOG
if errorlevel 1 goto compile_fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 AI.C > AICL.LOG
if errorlevel 1 goto compile_fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 AIPLORD.C > PLCL.LOG
if errorlevel 1 goto compile_fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 WARP.C > WARPCL.LOG
if errorlevel 1 goto compile_fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 BATTLE.C > BTCL.LOG
if errorlevel 1 goto compile_fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 UTILGEN.C > UTCL.LOG
if errorlevel 1 goto compile_fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 RTE2.C > DRVCL.LOG
if errorlevel 1 goto compile_fail
echo COMPILE=PASS > BLDSTAT.TXT
C:\\MSVC\\BIN\\LINK.EXE @RTE2.LNK > LINK.LOG
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
              "\nRTE2.EXE\n")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--guest-dir", required=True)
    args = parser.parse_args()
    package(args.output, args.guest_dir)


if __name__ == "__main__":
    main()
