#!/usr/bin/env python3
"""Package a compile-first VC1.52c check of the complete Rototill coordinator."""

import argparse
import hashlib
import json
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]


def write_dos(path, content):
    path.write_bytes(content.replace("\r\n", "\n").replace("\r", "\n")
                     .replace("\n", "\r\n").encode("ascii"))


def package(output, guest_dir, fixture):
    output = Path(output)
    fixture = Path(fixture)
    if output.exists():
        raise ValueError("package directory exists; use a fresh directory")
    if not guest_dir.startswith("C:\\") or len(guest_dir) > 16:
        raise ValueError("guest directory must be a short C:\\ path")
    if fixture.name.upper() != "TURINFX.H" or not fixture.is_file():
        raise ValueError("fixture must be a generated TURINFX.H")
    sources = {
        "CORE.H": ROOT / "reconstructed/include/core.h",
        "COMMON.H": ROOT / "reconstructed/include/common.h",
        "TURINFX.H": fixture,
        "TURINB.C": ROOT / "tests/satisfy_orders_turin_binding_e3_probe.c",
        "COORD.C": ROOT / "tests/satisfy_orders_rototill_coordinator_e4_probe.c",
    }
    output.mkdir(parents=True)
    for name, source in sources.items():
        body = source.read_text(encoding="ascii")
        if name == "COORD.C":
            original = '#include "satisfy_orders_turin_binding_e3_probe.c"'
            if body.count(original) != 1:
                raise ValueError("coordinator include changed")
            body = body.replace(original, '#include "TURINB.C"')
        write_dos(output / name, body)
    write_dos(output / "BUILD.BAT", """\
@echo off
if exist BLDSTAT.TXT goto stale
set INCLUDE=C:\\MSVC\\INCLUDE
C:\\MSVC\\BIN\\CL.EXE /c /AM /GA /GEs /Zp1 /Od /W3 COORD.C > COCL.LOG
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
    write_dos(output / "RUNBLD.BAT", "@echo off\ncd " + guest_dir +
              "\ncall BUILD.BAT\n")
    manifest = {
        "schema": "rototill-full-turn-native-compile-r1",
        "guest_dir": guest_dir,
        "scope": "Compile complete existing coordinator; no link or runtime claim",
        "source_sha256": {name: hashlib.sha256(source.read_bytes()).hexdigest()
                          for name, source in sorted(sources.items())},
        "package_sha256": {path.name: hashlib.sha256(path.read_bytes()).hexdigest()
                           for path in sorted(output.iterdir()) if path.is_file()},
    }
    (output / "MANIFEST.JSON").write_text(
        json.dumps(manifest, indent=2, sort_keys=True) + "\n")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--guest-dir", required=True)
    parser.add_argument("--fixture", type=Path, required=True)
    args = parser.parse_args()
    package(args.output, args.guest_dir, args.fixture)


if __name__ == "__main__":
    main()
