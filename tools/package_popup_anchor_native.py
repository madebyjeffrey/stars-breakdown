#!/usr/bin/env python3
"""Package the current Popup placement E2 matrix for fresh VC1.52c."""

import argparse
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
TEMPLATE = ROOT / "compat/vc152/popup-packed"


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
    for name in ("BUILDE2.BAT", "PUINTE2.C", "PUINTE2.DEF",
                 "PUINTE2.LNK", "WINSTUB.EXE"):
        (output / name).write_bytes((TEMPLATE / name).read_bytes())
    for source, name in (
        ("reconstructed/include/core.h", "CORE.H"),
        ("reconstructed/src/popup.c", "POP.C"),
        ("tests/popup_e2_probe.c", "E2PROBE.C"),
        ("tests/popup_packed_probe.c", "BASE.C"),
    ):
        write_ascii_crlf(output / name, (ROOT / source).read_text(
            encoding="ascii"))
    write_ascii_crlf(output / "RUNBLD.BAT", "@echo off\ncd " +
                     args.guest_dir + "\ncall BUILDE2.BAT\n")
    write_ascii_crlf(output / "RUN.BAT", "@echo off\ncd " +
                     args.guest_dir + "\nPUINTE2.EXE\n")


if __name__ == "__main__":
    main()
