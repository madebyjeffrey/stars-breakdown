"""Stage fresh DOS-safe task-5 probes from the current reconstructed source.

The output is input-only. It does not contact or launch the Win98 guest.
Each destination must be new so a native result can never be overwritten.
"""

import hashlib
import json
import argparse
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
PACKAGES = (
    ("services", "SOE2H", "satisfy-orders-task5-e2-r2",
     "satisfy-orders-task5-e2-r3", "satisfy_orders_task5_services_probe.c",
     "SOE2RES.TXT"),
    ("resources", "SOE2G", "satisfy-orders-task5-e2-resource-r5",
     "satisfy-orders-task5-e2-resource-r6",
     "satisfy_orders_task5_resource_probe.c", "SOE2GRES.TXT"),
    ("four_pass", "SOE3G", "satisfy-orders-task5-e3-r6",
     "satisfy-orders-task5-e3-r7",
     "satisfy_orders_task5_four_pass_probe.c", "SOE3RES.TXT"),
)


def dos_text(data):
    return data.replace(b"\r\n", b"\n").replace(b"\n", b"\r\n")


def make_package(spec):
    label, guest_dir, prior_name, output_name, probe_name, report_name = spec
    prior = ROOT / "compat/vc152" / prior_name
    output = ROOT / "compat/vc152" / output_name
    if output.exists():
        raise FileExistsError(output)
    probe = (ROOT / "tests" / probe_name).read_bytes()
    expected = b'C:\\\\' + (b"SOE2A" if label == "services" else
                           b"SOE2B" if label == "resources" else b"SOE3A")
    actual = b'C:\\\\' + guest_dir.encode("ascii")
    if probe.count(expected) != 1:
        raise ValueError("unexpected native report path in " + probe_name)
    probe = probe.replace(expected, actual)
    if label == "resources":
        old_report = b"SOE2BRES.TXT"
        if probe.count(old_report) != 1:
            raise ValueError("unexpected resource report name")
        probe = probe.replace(old_report, report_name.encode("ascii"))
    staged = {
        "CORE.H": (ROOT / "reconstructed/include/core.h").read_bytes(),
        "SYO.C": (ROOT / "reconstructed/src/satisfy_orders.c").read_bytes(),
        "PROBE.C": probe,
    }
    for name in ("BUILD.BAT", "SYO.LNK", "SYO.DEF", "SYNAT.C",
                 "WINSTUB.EXE"):
        staged[name] = (prior / name).read_bytes()
    if label == "four_pass":
        staged["DOORD.C"] = (ROOT / "reconstructed/src/do_orders.c").read_bytes()
        staged["PRODUC.C"] = (ROOT / "reconstructed/src/produce.c").read_bytes()
        staged["ESTMIN.C"] = (ROOT / "reconstructed/src/est_minerals_mined.c").read_bytes()
        staged["CHGPOP.C"] = (ROOT / "reconstructed/src/change_population.c").read_bytes()
        staged["COMMON.H"] = (ROOT / "reconstructed/include/common.h").read_bytes()
        wrapper = dos_text(staged["SYNAT.C"])
        needle = b'#include "ESTMIN.C"\r\n'
        if b'#include "CHGPOP.C"\r\n' not in wrapper:
            if wrapper.count(needle) != 1:
                raise ValueError("four-pass wrapper has unexpected includes")
            staged["SYNAT.C"] = wrapper.replace(needle,
                needle + b'#include "CHGPOP.C"\r\n')
    if label == "resources":
        staged["MARKTEC.C"] = (ROOT / "reconstructed/src/mark_techs.c").read_bytes()
        wrapper = dos_text(staged["SYNAT.C"])
        needle = b'#include "SYO.C"\r\n'
        if b'#include "MARKTEC.C"\r\n' not in wrapper:
            if wrapper.count(needle) != 1:
                raise ValueError("resource wrapper has unexpected includes")
            staged["SYNAT.C"] = wrapper.replace(needle,
                needle + b'#include "MARKTEC.C"\r\n')
    staged["RUNVC.BAT"] = ("@echo off\r\ncd C:\\" + guest_dir +
                            "\r\ncall BUILD.BAT\r\n").encode("ascii")
    staged["RUNPROBE.BAT"] = ("@echo off\r\ncd C:\\" + guest_dir +
                               "\r\nSYO.EXE\r\n").encode("ascii")
    for name in list(staged):
        if name != "WINSTUB.EXE":
            staged[name] = dos_text(staged[name])
        if len(name) > 12 or name.upper() != name:
            raise ValueError("non-DOS filename: " + name)
    if actual not in staged["PROBE.C"] or report_name.encode() not in staged["PROBE.C"]:
        raise ValueError("native report path does not match package")
    output.mkdir(parents=True)
    for name, contents in staged.items():
        (output / name).write_bytes(contents)
    manifest = {
        "status": "input_only_native_execution_pending",
        "profile": label,
        "guest_directory": "C:\\" + guest_dir,
        "report": report_name,
        "files": {name: {"bytes": len(contents),
                         "sha256": hashlib.sha256(contents).hexdigest()}
                  for name, contents in sorted(staged.items())},
    }
    (output / "MANIFST.JSN").write_bytes(dos_text(
        (json.dumps(manifest, indent=2, sort_keys=True) + "\n").encode("ascii")))
    return output


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--profile", choices=[row[0] for row in PACKAGES])
    args = parser.parse_args()
    for package in PACKAGES:
        if not args.profile or package[0] == args.profile:
            print(make_package(package))
