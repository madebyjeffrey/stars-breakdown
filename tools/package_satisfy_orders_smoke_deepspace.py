"""Stage the natural deep-space salvage differential probe for VC1.52c."""

import hashlib
import json
from pathlib import Path

from tools.package_satisfy_orders_task5_verified_resource import dos_text


ROOT = Path(__file__).resolve().parents[1]
PRIOR = ROOT / "compat/vc152/satisfy-orders-task5-verified-resource-r1"
OUTPUT = ROOT / "compat/vc152/satisfy-orders-smoke-deepspace-r2"


def package():
    if OUTPUT.exists():
        raise FileExistsError(OUTPUT)
    staged = {
        "CORE.H": (ROOT / "reconstructed/include/core.h").read_bytes(),
        "PROBE.C": (ROOT / "tests/satisfy_orders_smoke_deepspace_probe.c").read_bytes().replace(
            b'"core.h"', b'"CORE.H"'),
        "SYO.C": (ROOT / "reconstructed/src/satisfy_orders.c").read_bytes().replace(
            b'"core.h"', b'"CORE.H"'),
        "SALVAGE.C": (ROOT / "reconstructed/src/drop_salvage.c").read_bytes().replace(
            b'"core.h"', b'"CORE.H"'),
        "DECAY.C": (ROOT / "reconstructed/src/thing_decay.c").read_bytes().replace(
            b'"core.h"', b'"CORE.H"'),
    }
    for name in ("BUILD.BAT", "SYO.LNK", "SYO.DEF", "WINSTUB.EXE"):
        staged[name] = (PRIOR / name).read_bytes()
    staged["SYNAT.C"] = (
        b'#include <windows.h>\r\n'
        b'#define main task5_probe\r\n'
        b'#include "PROBE.C"\r\n'
        b'#undef main\r\n'
        b'#include "SYO.C"\r\n'
        b'#include "SALVAGE.C"\r\n'
        b'#include "DECAY.C"\r\n'
        b'int PASCAL WinMain(HINSTANCE a,HINSTANCE b,LPSTR c,int d)\r\n'
        b'{\r\n (void)a; (void)b; (void)c; (void)d;\r\n'
        b' return task5_probe();\r\n}\r\n'
    )
    staged["RUNVC.BAT"] = b"@echo off\r\ncd C:\\SOE2Q\r\ncall BUILD.BAT\r\n"
    staged["RUNPROBE.BAT"] = b"@echo off\r\ncd C:\\SOE2Q\r\nSYO.EXE\r\n"
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
        "guest_directory": "C:\\SOE2Q",
        "report": "SOE2QRES.TXT",
        "files": {name: {"bytes": len(data),
                         "sha256": hashlib.sha256(data).hexdigest()}
                  for name, data in sorted(staged.items())},
    }
    (OUTPUT / "MANIFST.JSN").write_bytes(dos_text(
        (json.dumps(manifest, indent=2, sort_keys=True) + "\n").encode("ascii")))
    return OUTPUT


if __name__ == "__main__":
    print(package())
