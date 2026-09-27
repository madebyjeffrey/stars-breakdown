"""Stage the verified original-part SHDEF repricing probe for VC1.52c."""

import hashlib
import json
from pathlib import Path

from tools.package_satisfy_orders_task5_verified_resource import dos_text
from tools.satisfy_orders_part_fixture import render


ROOT = Path(__file__).resolve().parents[1]
PRIOR = ROOT / "compat/vc152/satisfy-orders-task5-verified-resource-r1"
OUTPUT = ROOT / "compat/vc152/satisfy-orders-shdef-reprice-r2"


def package():
    if OUTPUT.exists():
        raise FileExistsError(OUTPUT)
    probe = (ROOT / "tests/satisfy_orders_shdef_reprice_probe.c").read_bytes()
    needle = b'#include "fixtures/satisfy_orders_design0_parts.h"'
    if probe.count(needle) != 1:
        raise ValueError("probe fixture include changed")
    staged = {
        "COMMON.H": (ROOT / "reconstructed/include/common.h").read_bytes().replace(
            b'"core.h"', b'"CORE.H"'),
        "CORE.H": (ROOT / "reconstructed/include/core.h").read_bytes(),
        "PARTS.H": render().encode("ascii"),
        "PROBE.C": probe.replace(needle, b'#include "PARTS.H"').replace(
            b'"core.h"', b'"CORE.H"').replace(
            b'C:\\\\SOE2M\\\\SOE2MRES.TXT',
            b'C:\\\\SOE2N\\\\SOE2NRES.TXT'),
        "REPRICE.C": (ROOT / "reconstructed/src/reprice_shdef.c").read_bytes().replace(
            b'"common.h"', b'"COMMON.H"'),
        "LOOKUP.C": (ROOT / "reconstructed/src/lookup_part.c").read_bytes().replace(
            b'"common.h"', b'"COMMON.H"'),
        "TRUECOST.C": (ROOT / "reconstructed/src/true_part_cost.c").read_bytes().replace(
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
        b'#include "SYO.C"\r\n'
        b'int PASCAL WinMain(HINSTANCE a,HINSTANCE b,LPSTR c,int d)\r\n'
        b'{\r\n (void)a; (void)b; (void)c; (void)d;\r\n'
        b' return task5_probe();\r\n}\r\n'
    )
    staged["RUNVC.BAT"] = b"@echo off\r\ncd C:\\SOE2N\r\ncall BUILD.BAT\r\n"
    staged["RUNPROBE.BAT"] = b"@echo off\r\ncd C:\\SOE2N\r\nSYO.EXE\r\n"
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
        "guest_directory": "C:\\SOE2N",
        "report": "SOE2NRES.TXT",
        "files": {name: {"bytes": len(data),
                         "sha256": hashlib.sha256(data).hexdigest()}
                  for name, data in sorted(staged.items())},
    }
    (OUTPUT / "MANIFST.JSN").write_bytes(dos_text(
        (json.dumps(manifest, indent=2, sort_keys=True) + "\n").encode("ascii")))
    return OUTPUT


if __name__ == "__main__":
    print(package())
