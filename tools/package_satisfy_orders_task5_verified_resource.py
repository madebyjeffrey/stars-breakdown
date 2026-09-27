"""Stage a fresh VC1.52c packet for the verified resource service binding."""

import hashlib
import json
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
PRIOR = ROOT / "compat/vc152/satisfy-orders-task5-e2-resource-r6"
OUTPUT = ROOT / "compat/vc152/satisfy-orders-task5-verified-resource-r1"


def dos_text(data):
    return data.replace(b"\r\n", b"\n").replace(b"\n", b"\r\n")


def package():
    if OUTPUT.exists():
        raise FileExistsError(OUTPUT)
    planet = (ROOT / "reconstructed/src/planet.c").read_bytes()
    needle = b'#include "innate_fp.h"'
    if planet.count(needle) != 1:
        raise ValueError("planet include changed")
    staged = {
        "CORE.H": (ROOT / "reconstructed/include/core.h").read_bytes(),
        "INNATE.H": (ROOT / "reconstructed/src/innate_fp.h").read_bytes(),
        "PLANET.C": planet.replace(needle, b'#include "INNATE.H"'),
        "PROBE.C": (ROOT / "tests/satisfy_orders_task5_verified_resource_probe.c").read_bytes(),
        "SYO.C": (ROOT / "reconstructed/src/satisfy_orders.c").read_bytes(),
    }
    for name in ("BUILD.BAT", "SYO.LNK", "SYO.DEF", "WINSTUB.EXE"):
        staged[name] = (PRIOR / name).read_bytes()
    staged["SYNAT.C"] = (
        b'#include <windows.h>\r\n'
        b'#define main task5_probe\r\n'
        b'#include "PROBE.C"\r\n'
        b'#undef main\r\n'
        b'#include "SYO.C"\r\n'
        b'#include "PLANET.C"\r\n'
        b'int PASCAL WinMain(HINSTANCE a,HINSTANCE b,LPSTR c,int d)\r\n'
        b'{\r\n (void)a; (void)b; (void)c; (void)d;\r\n'
        b' return task5_probe();\r\n}\r\n'
    )
    staged["RUNVC.BAT"] = b"@echo off\r\ncd C:\\SOE2L\r\ncall BUILD.BAT\r\n"
    staged["RUNPROBE.BAT"] = b"@echo off\r\ncd C:\\SOE2L\r\nSYO.EXE\r\n"
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
        "guest_directory": "C:\\SOE2L",
        "report": "SOE2LRES.TXT",
        "files": {name: {"bytes": len(data),
                         "sha256": hashlib.sha256(data).hexdigest()}
                  for name, data in sorted(staged.items())},
    }
    (OUTPUT / "MANIFST.JSN").write_bytes(dos_text(
        (json.dumps(manifest, indent=2, sort_keys=True) + "\n").encode("ascii")))
    return OUTPUT


if __name__ == "__main__":
    print(package())
