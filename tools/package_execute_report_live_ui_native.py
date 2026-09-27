#!/usr/bin/env python3
"""Fresh VC1.52c packet for ExecuteReportClick -> Popup live-UI geometry."""

import argparse
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
TEMPLATE = ROOT / "compat/vc152/execute-report-click-popup-production-e3"


def write_ascii_crlf(path, content):
    path.write_bytes(content.replace("\r\n", "\n").replace("\r", "\n")
                     .replace("\n", "\r\n").encode("ascii"))


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--guest-dir", required=True)
    args = parser.parse_args()
    if not args.guest_dir.startswith("C:\\"):
        raise ValueError("--guest-dir must be an absolute C: guest directory")
    output = args.output.resolve()
    output.mkdir(parents=True, exist_ok=False)

    for name in ("BUILD.BAT", "POPINT.DEF", "POPINT.LNK", "WINSTUB.EXE"):
        (output / name).write_bytes((TEMPLATE / name).read_bytes())
    for source, name in (
        ("reconstructed/include/core.h", "CORE.H"),
        ("reconstructed/src/execute_report_click.c", "EXE.C"),
        ("reconstructed/src/popup.c", "POPUP.C"),
        ("reconstructed/src/draw_popup.c", "DRAWPOP.C"),
        ("tests/execute_report_click_popup_integration_probe.c", "PROBE.C"),
    ):
        write_ascii_crlf(output / name, (ROOT / source).read_text(
            encoding="ascii"))
    wrapper = (TEMPLATE / "POPINT.C").read_text(encoding="ascii")
    if "CHECKS=26" not in wrapper:
        raise ValueError("template report count changed")
    write_ascii_crlf(output / "POPINT.C", wrapper.replace(
        "CHECKS=26", "CHECKS=86"))
    write_ascii_crlf(output / "RUNVC.BAT", "@echo off\ncd " +
                     args.guest_dir + "\ncall BUILD.BAT C:\\MSVC\n")
    write_ascii_crlf(output / "RUNPROBE.BAT", "@echo off\ncd " +
                     args.guest_dir + "\nPOPINT.EXE\n")


if __name__ == "__main__":
    main()
