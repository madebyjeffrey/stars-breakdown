#!/usr/bin/env python3
"""Stage a VC1.52c link of the full Rototill coordinator and 86 providers."""

import argparse
import hashlib
import json
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
COORD = ROOT / "runs/rototill-segmented-stage9-r1/COORD.OBJ"
COMMON = ROOT / "runs/rototill-common-segment-r1/M0010.OBJ"
STUB = ROOT / "compat/vc152/rototill-production-turn-e1-r4/WINSTUB.EXE"


def dos_text(path, body):
    path.write_bytes(body.replace("\r\n", "\n").replace("\r", "\n")
                     .replace("\n", "\r\n").encode("ascii"))


def package(output, guest_dir):
    if output.exists():
        raise ValueError("package directory exists")
    if not guest_dir.startswith("C:\\") or len(guest_dir) > 16:
        raise ValueError("guest directory must be a short C:\\ path")
    output.mkdir(parents=True)
    for name, path in (("COORD.OBJ", COORD), ("M0010.OBJ", COMMON),
                       ("WINSTUB.EXE", STUB)):
        (output / name).write_bytes(path.read_bytes())
    dos_text(output / "RTFULL.DEF", """\
NAME RTFULL WINDOWAPI
DESCRIPTION 'Rototill complete-turn coordinator'
EXETYPE WINDOWS
STUB 'WINSTUB.EXE'
CODE PRELOAD MOVEABLE DISCARDABLE
DATA PRELOAD MOVEABLE MULTIPLE
HEAPSIZE 4096
STACKSIZE 32768
""")
    objects = ["COORD.OBJ"] + ["M%04d.OBJ" % index for index in range(1, 87)]
    groups = [objects[index:index + 10] for index in range(0, len(objects), 10)]
    object_lines = ["+".join(group) + ("+" if index + 1 < len(groups) else "")
                    for index, group in enumerate(groups)]
    dos_text(output / "RTFULL.LNK", "\n".join(object_lines) + "\n" + """\
RTFULL.EXE
RTFULL.MAP
C:\\MSVC\\LIB\\LLIBCEW.LIB C:\\MSVC\\LIB\\LIBW.LIB /NOD /ALIGN:16 /CO /MAP
RTFULL.DEF
""")
    dos_text(output / "LINK.BAT", """\
@echo off
if exist LNKSTAT.TXT goto stale
C:\\MSVC\\BIN\\LINK.EXE @RTFULL.LNK > LINK.LOG
if errorlevel 1 goto failed
echo LINK=PASS > LNKSTAT.TXT
goto done
:failed
echo LINK=FAIL > LNKSTAT.TXT
goto done
:stale
echo STALE=REFUSED > STALE.TXT
:done
""")
    dos_text(output / "RUNLINK.BAT", "@echo off\ncd " + guest_dir +
             "\ncall LINK.BAT\n")
    manifest = {
        "schema": "rototill-complete-link-r1",
        "guest_dir": guest_dir,
        "object_count": len(objects),
        "source_object_sha256": {
            "COORD.OBJ": hashlib.sha256(COORD.read_bytes()).hexdigest(),
            "M0010.OBJ": hashlib.sha256(COMMON.read_bytes()).hexdigest(),
        },
        "package_sha256": {path.name: hashlib.sha256(path.read_bytes()).hexdigest()
                           for path in sorted(output.iterdir())},
    }
    (output / "MANIFEST.JSON").write_text(
        json.dumps(manifest, indent=2, sort_keys=True) + "\n")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--guest-dir", required=True)
    args = parser.parse_args()
    package(args.output, args.guest_dir)


if __name__ == "__main__":
    main()
