#!/usr/bin/env python3
"""Stage the large common.c provider in six VC1.52c code segments."""

import argparse
import hashlib
import json
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
BASE = ROOT / "compat/vc152/rototill-link-modules-r1"
BOUNDARIES = (
    "void stars_draw_planet_xfer_side(",
    "StarsI16 stars_check_xfer_wp(",
    "static void roman_small(",
    "StarsI16 stars_find_nearest_object(",
    "StarsI16 stars_pct_wormhole_moves(",
)


def dos_text(path, body):
    path.write_bytes(body.replace("\r\n", "\n").replace("\r", "\n")
                     .replace("\n", "\r\n").encode("ascii"))


def package(output, guest_dir):
    if output.exists():
        raise ValueError("package directory exists")
    if not guest_dir.startswith("C:\\") or len(guest_dir) > 16:
        raise ValueError("guest directory must be a short C:\\ path")
    output.mkdir(parents=True)
    body = (BASE / "M0010.C").read_text(encoding="ascii")
    for index, marker in enumerate(BOUNDARIES, 1):
        if body.count("\n" + marker) != 1:
            raise ValueError("common.c boundary changed: " + marker)
        body = body.replace("\n" + marker,
            "\n#if defined(_MSC_VER)\n"
            '#pragma code_seg("RCM%d")\n' % index +
            "#endif\n" + marker)
    dos_text(output / "M0010.C", body)
    for name in ("CORE.H", "COMMON.H", "RUNTIME.H", "RTINT.H"):
        (output / name).write_bytes((BASE / name).read_bytes())
    dos_text(output / "BUILD.BAT", """\
@echo off
if exist BLDSTAT.TXT goto stale
set INCLUDE=C:\\MSVC\\INCLUDE
C:\\MSVC\\BIN\\CL.EXE /c /AL /GA /GEs /Zp1 /Od /W3 M0010.C > C0010.LOG
if errorlevel 1 goto failed
echo COMPILE=PASS > BLDSTAT.TXT
goto done
:failed
echo COMPILE=FAIL > BLDSTAT.TXT
goto done
:stale
echo STALE=REFUSED > STALE.TXT
:done
""")
    dos_text(output / "RUNBLD.BAT", "@echo off\ncd " + guest_dir +
             "\ncall BUILD.BAT\n")
    manifest = {
        "schema": "rototill-common-segment-r1",
        "guest_dir": guest_dir,
        "source": "reconstructed/src/common.c",
        "source_sha256": hashlib.sha256(
            (ROOT / "reconstructed/src/common.c").read_bytes()).hexdigest(),
        "code_boundaries": BOUNDARIES,
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
