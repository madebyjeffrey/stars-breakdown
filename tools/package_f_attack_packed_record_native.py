#!/usr/bin/env python3
"""Export the packed-record FAttack probe for VC++ 1.52c/Win98."""

from __future__ import annotations

import argparse
import shutil
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT))
from tools.ne_layout import NE


REFERENCE = ROOT / "compat/vc152/f-attack-complex-native-e39"


def write_crlf(path: Path, text: str) -> None:
    path.write_bytes(text.replace("\r\n", "\n").replace("\n", "\r\n").encode("ascii"))


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--guest-dir", required=True)
    args = parser.parse_args()
    output = args.output.resolve()
    output.mkdir(parents=True, exist_ok=False)

    sources = {
        "CORE.H": ROOT / "reconstructed/include/core.h",
        "COMMON.H": ROOT / "reconstructed/include/common.h",
        "FA.C": ROOT / "reconstructed/src/f_attack.c",
        "DAMAGE.C": ROOT / "reconstructed/src/f_damage_tok.c",
        "LOOKUP.C": ROOT / "reconstructed/src/lookup_part.c",
        "FAPART.C": ROOT / "tests/f_attack_packed_part_probe.c",
        "FTORDSP.C": ROOT / "tests/f_attack_raw_torpedo_dispatch_probe.c",
        "RAW.C": ROOT / "tests/f_attack_raw_damage_probe.c",
        "TORP.C": REFERENCE / "TORP.C",
        "BATTLE.C": REFERENCE / "BATTLE.C",
        "WINSTUB.EXE": REFERENCE / "WINSTUB.EXE",
        "FAPNAT.DEF": REFERENCE / "FAPNAT.DEF",
        "FAPNAT.LNK": REFERENCE / "FAPNAT.LNK",
    }
    for name, source in sources.items():
        shutil.copyfile(source, output / name)

    segment = bytes(NE(ROOT / "target/stars.exe").raw[1])
    (output / "SEG2.BIN").write_bytes(segment)
    write_crlf(output / "LPSHSTUB.C", """\
#include "core.h"
const StarsU8 *stars_lpshdef_from_tok_globals(const StarsToken *token,
    const StarsU8 *dgroup, StarsResolveFarHull resolve, void *context)
{
    (void)token;
    (void)dgroup;
    (void)resolve;
    (void)context;
    return (const StarsU8 *)0;
}
""")

    write_crlf(output / "FAPNAT.C", """\
#include <windows.h>
#include <stdio.h>
#define FACTOR_NATIVE
#include "FAPART.C"
#include "LPSHSTUB.C"
#include "FA.C"
#include "LOOKUP.C"
#include "TORP.C"
#include "BATTLE.C"

int PASCAL WinMain(HINSTANCE a, HINSTANCE b, LPSTR c, int d)
{
    FILE *f;
    int r;
    (void)a; (void)b; (void)c; (void)d;
    r = f_attack_packed_part_probe("SEG2.BIN");
    f = fopen("FAPARTN.TXT", "w");
    if (f) {
        fprintf(f,
            "F_ATTACK_PACKED_PART_CHECKS=24 FAILURES=%d RESULT=%s\\n",
            r, r ? "FAIL" : "PASS");
        fclose(f);
    }
    return r;
}
""")
    write_crlf(output / "BUILD.BAT", """\
@echo off
set INCLUDE=C:\\MSVC\\INCLUDE
set LIB=C:\\MSVC\\LIB
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 FAPNAT.C > FAPCL.LOG
if errorlevel 1 goto fail
C:\\MSVC\\BIN\\LINK.EXE @FAPNAT.LNK > FAPLINK.LOG
if errorlevel 1 goto fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 FATORP.C > TCL.LOG
if errorlevel 1 goto fail
C:\\MSVC\\BIN\\LINK.EXE @FATORP.LNK > TALINK.LOG
if errorlevel 1 goto fail
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 RAWDAM.C > RAWCL.LOG
if errorlevel 1 goto fail
C:\\MSVC\\BIN\\LINK.EXE @RAWDAM.LNK > RAWLINK.LOG
if errorlevel 1 goto fail
echo COMPILE=PASS > BLDSTAT.TXT
echo LINK=PASS >> BLDSTAT.TXT
goto done
:fail
echo COMPILE=FAIL > BLDSTAT.TXT
echo LINK=FAIL >> BLDSTAT.TXT
:done
""")
    write_crlf(output / "FATORP.C", """\
#include <windows.h>
#include <stdio.h>
#define TORPEDO_TARGET_NATIVE
#define RAW_TORPEDO_DISPATCH_NATIVE
#include "FTORDSP.C"
#include "LPSHSTUB.C"
#include "FA.C"
#include "LOOKUP.C"
#include "TORP.C"
#include "BATTLE.C"

int PASCAL WinMain(HINSTANCE a, HINSTANCE b, LPSTR c, int d)
{
    FILE *f;
    int r;
    (void)a; (void)b; (void)c; (void)d;
    r = f_attack_raw_torpedo_dispatch_probe("SEG2.BIN");
    f = fopen("TORDSP.TXT", "w");
    if (f) {
        fprintf(f,
            "F_ATTACK_RAW_TORPEDO_DISPATCH_CHECKS=17 FAILURES=%d RESULT=%s\\n",
            r ? 1 : 0, r ? "FAIL" : "PASS");
        fclose(f);
    }
    return r != 0;
}
""")
    write_crlf(output / "FATORP.DEF", """\
NAME FATORP WINDOWAPI
DESCRIPTION 'Stars raw torpedo record dispatch probe'
EXETYPE WINDOWS
STUB 'WINSTUB.EXE'
CODE PRELOAD MOVEABLE DISCARDABLE
DATA PRELOAD MOVEABLE MULTIPLE
HEAPSIZE 1024
STACKSIZE 8192
""")
    write_crlf(output / "FATORP.LNK", """\
FATORP.OBJ
FATORP.EXE
FATORP.MAP
C:\\MSVC\\LIB\\MLIBCEW.LIB C:\\MSVC\\LIB\\LIBW.LIB /NOD /ALIGN:16 /CO /MAP
FATORP.DEF
""")
    write_crlf(output / "RAWDAM.C", """\
#include <windows.h>
#include <stdio.h>
#define RAW_NATIVE
#define main f_attack_raw_damage_probe
#include "RAW.C"
#undef main
#include "LPSHSTUB.C"
#include "FA.C"
#include "DAMAGE.C"
#include "LOOKUP.C"
#include "TORP.C"
#include "BATTLE.C"

int PASCAL WinMain(HINSTANCE a, HINSTANCE b, LPSTR c, int d)
{
    FILE *f;
    int r;
    (void)a; (void)b; (void)c; (void)d;
    r = f_attack_raw_damage_probe();
    f = fopen("RAWDAM.TXT", "w");
    if (f) {
        fprintf(f,
            "F_ATTACK_RAW_DAMAGE_CHECKS=15 FAILURES=%d RESULT=%s\\n",
            r ? 1 : 0, r ? "FAIL" : "PASS");
        fclose(f);
    }
    return r != 0;
}
""")
    write_crlf(output / "RAWDAM.DEF", """\
NAME RAWDAM WINDOWAPI
DESCRIPTION 'Stars raw damage residual probe'
EXETYPE WINDOWS
STUB 'WINSTUB.EXE'
CODE PRELOAD MOVEABLE DISCARDABLE
DATA PRELOAD MOVEABLE MULTIPLE
HEAPSIZE 1024
STACKSIZE 8192
""")
    write_crlf(output / "RAWDAM.LNK", """\
RAWDAM.OBJ
RAWDAM.EXE
RAWDAM.MAP
C:\\MSVC\\LIB\\MLIBCEW.LIB C:\\MSVC\\LIB\\LIBW.LIB /NOD /ALIGN:16 /CO /MAP
RAWDAM.DEF
""")
    write_crlf(output / "RUNFAP.BAT", """\
@echo off
cd __GUEST_DIR__
FAPNAT.EXE
""".replace("__GUEST_DIR__", args.guest_dir))
    write_crlf(output / "RUNTORP.BAT", """\
@echo off
cd __GUEST_DIR__
FATORP.EXE
""".replace("__GUEST_DIR__", args.guest_dir))
    write_crlf(output / "RUNRAW.BAT", """\
@echo off
cd __GUEST_DIR__
RAWDAM.EXE
""".replace("__GUEST_DIR__", args.guest_dir))
    write_crlf(output / "RUNVC.BAT", """\
@echo off
cd __GUEST_DIR__
call BUILD.BAT C:\\MSVC
""".replace("__GUEST_DIR__", args.guest_dir))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
