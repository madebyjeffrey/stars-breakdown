"""Stage the verified v4 high-offer task-5 probe for VC1.52c."""

import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
PRIOR = ROOT / "compat/vc152/satisfy-orders-shdef-reprice-r2"
OUTPUT = ROOT / "compat/vc152/satisfy-orders-reprice-v4-e2-native-r1"


def dos_text(data):
    return data.replace(b"\r\n", b"\n").replace(b"\n", b"\r\n")


def package():
    if OUTPUT.exists():
        raise FileExistsError(OUTPUT)
    probe = (ROOT / "tests/satisfy_orders_reprice_v4_live_probe.c").read_bytes()
    include = b'#include "fixtures/satisfy_orders_reprice_v4_bitc_parts.h"'
    if probe.count(include) != 1:
        raise ValueError("v4 fixture include changed")
    staged = {
        "COMMON.H": (ROOT / "reconstructed/include/common.h").read_bytes().replace(
            b'"core.h"', b'"CORE.H"'),
        "CORE.H": (ROOT / "reconstructed/include/core.h").read_bytes(),
        "PARTS.H": (ROOT / "tests/fixtures/satisfy_orders_reprice_v4_bitc_parts.h").read_bytes(),
        "PROBE.C": probe.replace(include, b'#include "PARTS.H"').replace(
            b'"core.h"', b'"CORE.H"').replace(
            b'"common.h"', b'"COMMON.H"'),
        "REPRICE.C": (ROOT / "reconstructed/src/reprice_shdef.c").read_bytes().replace(
            b'"common.h"', b'"COMMON.H"'),
        "LOOKUP.C": (ROOT / "reconstructed/src/lookup_part.c").read_bytes().replace(
            b'"common.h"', b'"COMMON.H"'),
        "TRUECOST.C": (ROOT / "reconstructed/src/true_part_cost.c").read_bytes().replace(
            b'"core.h"', b'"CORE.H"'),
        "MARKTECH.C": (ROOT / "reconstructed/src/mark_techs.c").read_bytes().replace(
            b'"core.h"', b'"CORE.H"'),
        "ITECH.C": (ROOT / "reconstructed/src/itech_learn_a_tech.c").read_bytes().replace(
            b'"core.h"', b'"CORE.H"'),
        "SYO.C": (ROOT / "reconstructed/src/satisfy_orders.c").read_bytes().replace(
            b'"core.h"', b'"CORE.H"'),
    }
    for name in ("BUILD.BAT", "SYO.LNK", "SYO.DEF", "WINSTUB.EXE"):
        staged[name] = (PRIOR / name).read_bytes()
    staged["SYNAT.C"] = (
        b'#include <windows.h>\r\n'
        b'#define main task5_probe\r\n'
        b'#include "PROBE.C"\r\n'
        b'#undef main\r\n'
        b'#include "REPRICE.C"\r\n'
        b'#include "LOOKUP.C"\r\n'
        b'#include "TRUECOST.C"\r\n'
        b'#include "MARKTECH.C"\r\n'
        b'#include "ITECH.C"\r\n'
        b'#include "SYO.C"\r\n'
        b'int PASCAL WinMain(HINSTANCE a,HINSTANCE b,LPSTR c,int d)\r\n'
        b'{\r\n (void)a; (void)b; (void)c; (void)d;\r\n'
        b' return task5_probe();\r\n}\r\n'
    )
    staged["RUNVC.BAT"] = b"@echo off\r\ncd C:\\SOE2S\r\ncall BUILD.BAT\r\n"
    staged["RUNPROBE.BAT"] = b"@echo off\r\ncd C:\\SOE2S\r\nSYO.EXE\r\n"
    for name in list(staged):
        if name != "WINSTUB.EXE":
            staged[name] = dos_text(staged[name])
        if name.upper() != name or len(name.split(".")[0]) > 8:
            raise ValueError("non-DOS filename: " + name)
    OUTPUT.mkdir(parents=True)
    for name, data in staged.items():
        (OUTPUT / name).write_bytes(data)
    manifest = {
        "status": "input_only_native_execution_pending",
        "guest_directory": "C:\\SOE2S",
        "report": "SOE2SRES.TXT",
        "source_report": "tests/satisfy_orders_reprice_v4_live_probe.c",
        "fixture_manifest": "tests/fixtures/satisfy_orders_reprice_v4_bitc_parts.json",
        "files": {name: {"bytes": len(data),
                         "sha256": hashlib.sha256(data).hexdigest()}
                  for name, data in sorted(staged.items())},
    }
    (OUTPUT / "MANIFST.JSN").write_bytes(dos_text(
        (json.dumps(manifest, indent=2, sort_keys=True) + "\n").encode("ascii")))
    return OUTPUT


if __name__ == "__main__":
    print(package())
