#!/usr/bin/env python3
"""Stage source providers for the Rototill full-turn VC1.52c link."""

import argparse
import hashlib
import json
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
AUDIT = ROOT / "analysis/compat/do-rototill-native-link-audit-r1.json"
HEADER_SOURCES = {
    "CORE.H": ROOT / "reconstructed/include/core.h",
    "COMMON.H": ROOT / "reconstructed/include/common.h",
    "RUNTIME.H": ROOT / "reconstructed/include/runtime.h",
    "RTINT.H": ROOT / "reconstructed/src/runtime_internal.h",
    "INNATEFP.H": ROOT / "reconstructed/src/innate_fp.h",
    "BPLANS.INC": ROOT / "reconstructed/src/battle_plan_templates.inc",
    "SEEDTAB.INC": ROOT / "reconstructed/src/seed_table.inc",
}
INCLUDE_RENAMES = {
    '"runtime_internal.h"': '"RTINT.H"',
    '"innate_fp.h"': '"INNATEFP.H"',
    '"battle_plan_templates.inc"': '"BPLANS.INC"',
    '"seed_table.inc"': '"SEEDTAB.INC"',
}


def dos_text(path, body):
    path.write_bytes(body.replace("\r\n", "\n").replace("\r", "\n")
                     .replace("\n", "\r\n").encode("ascii"))


def ascii_source(path):
    body = path.read_text(encoding="utf-8")
    for old, new in INCLUDE_RENAMES.items():
        body = body.replace(old, new)
    # Non-ASCII in the reconstructed modules is in explanatory comments.
    return body.encode("ascii", "replace").decode("ascii")


def package(output, guest_dir):
    if output.exists():
        raise ValueError("package directory exists")
    if not guest_dir.startswith("C:\\") or len(guest_dir) > 16:
        raise ValueError("guest directory must be a short C:\\ path")
    audit = json.loads(AUDIT.read_text())
    candidates = audit["candidate_sources"]
    if len(candidates) != 86:
        raise ValueError("link candidate list changed")
    output.mkdir(parents=True)
    for name, source in HEADER_SOURCES.items():
        dos_text(output / name, ascii_source(source))
    source_map = {}
    commands = ["@echo off", "if exist BLDSTAT.TXT goto stale",
                "set INCLUDE=C:\\MSVC\\INCLUDE"]
    for index, relative in enumerate(candidates, 1):
        source = ROOT / relative
        name = "M%04d.C" % index
        log = "C%04d.LOG" % index
        dos_text(output / name, ascii_source(source))
        source_map[name] = {
            "source": relative,
            "source_sha256": hashlib.sha256(source.read_bytes()).hexdigest(),
            "compile_log": log,
        }
        commands.append("C:\\MSVC\\BIN\\CL.EXE /c /AL /GA /GEs /Zp1 /Od /W3 " +
                        name + " > " + log)
        commands.append("if errorlevel 1 echo " + name + " >> ERRORS.TXT")
    commands += ["if exist ERRORS.TXT goto failed",
                 "echo COMPILE=PASS > BLDSTAT.TXT", "goto done",
                 ":failed", "echo COMPILE=FAIL > BLDSTAT.TXT", "goto done",
                 ":stale", "echo STALE=REFUSED > STALE.TXT", ":done"]
    dos_text(output / "BUILD.BAT", "\n".join(commands) + "\n")
    dos_text(output / "RUNBLD.BAT", "@echo off\ncd " + guest_dir +
             "\ncall BUILD.BAT\n")
    manifest = {
        "schema": "rototill-native-provider-compile-r1",
        "scope": "Compile 86 candidate real source modules; link and run pending",
        "guest_dir": guest_dir,
        "link_audit_sha256": hashlib.sha256(AUDIT.read_bytes()).hexdigest(),
        "source_map": source_map,
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
