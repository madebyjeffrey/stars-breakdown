#!/usr/bin/env python3
"""Package the Turin turn-zero miner task-5 check for VC++ 1.52c."""

import argparse
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
WINSTUB = ROOT / "compat/vc152/do-robotoid-ai-turn-integration/WINSTUB.EXE"


def dos_text(text):
    return text.replace("\r\n", "\n").replace("\r", "\n") \
        .replace("\n", "\r\n").encode("ascii")


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--guest-dir", required=True)
    args = parser.parse_args()
    if not args.guest_dir.startswith("C:\\"):
        raise ValueError("--guest-dir must be a C:\\ path")
    output = args.output.resolve()
    output.mkdir(parents=True, exist_ok=False)
    sources = (
        (ROOT / "reconstructed/include/core.h", "CORE.H"),
        (ROOT / "reconstructed/src/ai_plord.c", "PLORD.C"),
        (ROOT / "reconstructed/src/do_turin_drone_ai_turn.c", "TURIN.C"),
        (ROOT / "tests/do_turin_drone_ai_turn_miner_task_probe.c", "PROBE.C"),
    )
    hashes = {}
    for source, name in sources:
        raw = source.read_bytes()
        packaged = dos_text(raw.decode("ascii"))
        (output / name).write_bytes(packaged)
        hashes[name] = hashlib.sha256(packaged).hexdigest()
    stub = WINSTUB.read_bytes()
    (output / "WINSTUB.EXE").write_bytes(stub)
    hashes["WINSTUB.EXE"] = hashlib.sha256(stub).hexdigest()
    files = {
        "TURMINE.C": """\
#include <windows.h>
#include <stdio.h>
#define TURIN_MINER_TASK_NATIVE
#include "PROBE.C"
#include "PLORD.C"
#include "TURIN.C"

int PASCAL WinMain(HINSTANCE a, HINSTANCE b, LPSTR c, int d)
{
    FILE *f;
    int failures;
    (void)a; (void)b; (void)c; (void)d;
    failures = turin_miner_task_probe();
    f = fopen("MINER.TXT", "w");
    if (f) {
        fprintf(f, "TURIN_MINER_TASK_CHECKS=%d FAILURES=%d RESULT=%s\\n",
            turin_miner_task_checks(), turin_miner_task_failures(),
            failures ? "FAIL" : "PASS");
        fclose(f);
    }
    return failures;
}
""",
        "TURMINE.DEF": """\
NAME TURMINE WINDOWAPI
DESCRIPTION 'Stars Turin turn-zero miner task probe'
EXETYPE WINDOWS
STUB 'WINSTUB.EXE'
CODE PRELOAD MOVEABLE DISCARDABLE
DATA PRELOAD MOVEABLE MULTIPLE
HEAPSIZE 1024
STACKSIZE 8192
""",
        "TURMINE.LNK": """\
TURMINE.OBJ
TURMINE.EXE
TURMINE.MAP
C:\\MSVC\\LIB\\MLIBCEW.LIB C:\\MSVC\\LIB\\LIBW.LIB /NOD /ALIGN:16 /CO /MAP
TURMINE.DEF
""",
        "BUILD.BAT": """\
@echo off
set INCLUDE=C:\\MSVC\\INCLUDE
set LIB=C:\\MSVC\\LIB
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 TURMINE.C > BUILD.LOG
if errorlevel 1 goto fail
C:\\MSVC\\BIN\\LINK.EXE @TURMINE.LNK > LINK.LOG
if errorlevel 1 goto fail
echo COMPILE=PASS > BLDSTAT.TXT
echo LINK=PASS >> BLDSTAT.TXT
goto done
:fail
echo COMPILE=FAIL > BLDSTAT.TXT
echo LINK=FAIL >> BLDSTAT.TXT
:done
""",
        "RUNVC.BAT": "@echo off\ncd " + args.guest_dir + "\ncall BUILD.BAT\n",
        "RUNPROBE.BAT": "@echo off\ncd " + args.guest_dir + "\nTURMINE.EXE\n",
    }
    for name, contents in files.items():
        data = dos_text(contents)
        (output / name).write_bytes(data)
        hashes[name] = hashlib.sha256(data).hexdigest()
    manifest = {
        "status": "input_only_native_execution_pending",
        "scope": "turin_turn_zero_design_2_miner_writes_task_5",
        "guest_directory": args.guest_dir,
        "report": "MINER.TXT",
        "files": hashes,
    }
    (output / "MANIFEST.JSON").write_text(
        json.dumps(manifest, indent=2) + "\n", encoding="utf-8")
    print(output)


if __name__ == "__main__":
    main()
