#!/usr/bin/env python3
"""Export a fresh VC++ 1.52c package for original FLookupPart record bytes."""
import argparse
import hashlib
from pathlib import Path
import zipfile

try:
    from tools.ne_layout import NE
except ModuleNotFoundError:
    from ne_layout import NE


ROOT = Path(__file__).resolve().parents[1]
DEFAULT_OUTPUT = ROOT / "compat/vc152/f-lookup-part-memory-native-e50"

COMMON_H = '''#ifndef LOOKUP_PART_MEMORY_COMMON_H
#define LOOKUP_PART_MEMORY_COMMON_H
typedef signed short StarsI16;
typedef unsigned char StarsU8;
typedef unsigned short StarsU16;
typedef unsigned long StarsU32;
typedef StarsU32 (*StarsPartResolve)(void *, StarsU16, StarsU16);
typedef StarsI16 (*StarsPartRaceStat)(void *, StarsI16, StarsI16);
typedef StarsI16 (*StarsPartRaceGrbit)(void *, StarsI16, StarsI16);
typedef StarsI16 (*StarsPartHidden)(void *, StarsU16, StarsU16);
typedef StarsI16 (*StarsPartTechStatus)(void *, StarsU32);
typedef StarsI16 (*StarsPartArmorDp)(void *, StarsU32);
typedef struct StarsLookupPartInput {
    StarsU16 group;
    StarsU16 item;
    StarsI16 player;
    StarsU32 component;
    StarsU16 table_segment;
    StarsU16 table_segment_is_current_cs;
    StarsU16 table_base_offset;
    StarsU16 table_offset;
    StarsU16 table_stride;
    StarsPartResolve resolve;
    StarsPartRaceStat race_stat;
    StarsPartRaceGrbit race_grbit;
    StarsPartHidden should_hide;
    StarsPartTechStatus tech_status;
    StarsPartArmorDp armor_dp;
    void *context;
} StarsLookupPartInput;
StarsI16 stars_lookup_part(StarsLookupPartInput *);
const StarsU8 *stars_lookup_part_record(const StarsLookupPartInput *,
    const StarsU8 *, StarsU16);
#endif
'''

LOOKAT_C = '''#include <windows.h>
#include <stdio.h>
#define TARGET_NATIVE 1
#include "LOOKMEM.C"
#include "LOOKUP.C"

int PASCAL WinMain(HINSTANCE instance, HINSTANCE previous, LPSTR command,
    int show)
{
    FILE *report;
    int failures;
    (void)instance;
    (void)previous;
    (void)command;
    (void)show;
    failures = lookup_part_memory_probe("SEG2.BIN");
    report = fopen("LKMRES.TXT", "w");
    if (!report)
        return 2;
    fprintf(report,
        "LOOKUP_PART_MEMORY_CHECKS=129 FAILURES=%d RESULT=%s\\n",
        failures, failures ? "FAIL" : "PASS");
    fclose(report);
    return failures;
}
'''

BUILD_BAT = '''@echo off
set INCLUDE=C:\\MSVC\\INCLUDE
set LIB=C:\\MSVC\\LIB
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 LOOKAT.C > BUILD.LOG
if errorlevel 1 goto fail
C:\\MSVC\\BIN\\LINK.EXE @LOOKAT.LNK > LINK.LOG
if errorlevel 1 goto fail
echo COMPILE=PASS > BLDSTAT.TXT
echo LINK=PASS >> BLDSTAT.TXT
goto done
:fail
echo COMPILE=FAIL > BLDSTAT.TXT
echo LINK=FAIL >> BLDSTAT.TXT
:done
'''

RUNVC_BAT = '''@echo off
cd C:\\FAPTA50
call BUILD.BAT
'''

RUNMEM_BAT = '''@echo off
cd C:\\FAPTA50
LOOKAT.EXE
'''

LOOKAT_DEF = '''NAME LOOKAT WINDOWAPI
DESCRIPTION 'Stars FLookupPart original record-memory probe'
EXETYPE WINDOWS
STUB 'WINSTUB.EXE'
CODE PRELOAD MOVEABLE DISCARDABLE
DATA PRELOAD MOVEABLE MULTIPLE
HEAPSIZE 1024
STACKSIZE 8192
'''

LOOKAT_LNK = '''LOOKAT.OBJ
LOOKAT.EXE
LOOKAT.MAP
C:\\MSVC\\LIB\\MLIBCEW.LIB C:\\MSVC\\LIB\\LIBW.LIB /NOD /ALIGN:16 /CO /MAP
LOOKAT.DEF
'''


def dos_text(text):
    return text.replace("\r\n", "\n").replace("\r", "\n").replace(
        "\n", "\r\n").encode("ascii")


def sha(data):
    return hashlib.sha256(data).hexdigest()


def payload():
    source = (ROOT / "reconstructed/src/lookup_part.c").read_text()
    probe = (ROOT / "tests/lookup_part_memory_probe.c").read_text()
    source = source.replace('#include "common.h"', '#include "COMMON.H"')
    probe = probe.replace('#include "common.h"', '#include "COMMON.H"')
    original = ROOT / "target/stars.exe"
    ne = NE(original)
    if sha(ne.b) != "ccab6682b55dae14dbf104e491cfbd3b32a6280022020032ae0615500949ba59":
        raise ValueError("Unexpected original executable identity")
    files = {
        "COMMON.H": dos_text(COMMON_H),
        "LOOKAT.C": dos_text(LOOKAT_C),
        "LOOKUP.C": dos_text(source),
        "LOOKMEM.C": dos_text(probe),
        "BUILD.BAT": dos_text(BUILD_BAT),
        "RUNVC.BAT": dos_text(RUNVC_BAT),
        "RUNMEM.BAT": dos_text(RUNMEM_BAT),
        "LOOKAT.DEF": dos_text(LOOKAT_DEF),
        "LOOKAT.LNK": dos_text(LOOKAT_LNK),
        "SEG2.BIN": bytes(ne.raw[1]),
    }
    stub = ROOT / "compat/vc152/f-lookup-part-anchor-native-e48/WINSTUB.EXE"
    files["WINSTUB.EXE"] = stub.read_bytes()
    manifest = [
        "STARS FLOOKUPPART ORIGINAL SEGMENT MEMORY PROBE E50",
        "ASCII source uses CRLF; SEG2.BIN is original unrelocated NE segment 2.",
        f"ORIGINAL SHA256 {sha(ne.b)}",
        f"SEGMENT2 SHA256 {sha(files['SEG2.BIN'])}; BYTES {len(files['SEG2.BIN'])}",
    ]
    for name, data in sorted(files.items()):
        manifest.append(f"{name} SHA256 {sha(data)}; BYTES {len(data)}")
    files["MANIFEST.TXT"] = dos_text("\n".join(manifest) + "\n")
    return files


def export(output):
    if output.exists() or output.with_suffix(".ZIP").exists():
        raise FileExistsError(f"Use a fresh output path: {output}")
    files = payload()
    output.mkdir(parents=True)
    for name, data in files.items():
        (output / name).write_bytes(data)
    archive = output.with_suffix(".ZIP")
    with zipfile.ZipFile(archive, "x", compression=zipfile.ZIP_DEFLATED) as package:
        for name, data in sorted(files.items()):
            info = zipfile.ZipInfo(name, date_time=(2026, 9, 22, 0, 0, 0))
            info.compress_type = zipfile.ZIP_DEFLATED
            info.create_system = 0
            info.external_attr = 0x20
            package.writestr(info, data)
    return archive


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path, default=DEFAULT_OUTPUT)
    args = parser.parse_args()
    archive = export(args.output.resolve())
    print(f"Folder: {args.output.resolve()}\nArchive: {archive}")


if __name__ == "__main__":
    main()
