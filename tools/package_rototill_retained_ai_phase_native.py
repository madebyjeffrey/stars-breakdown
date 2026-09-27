#!/usr/bin/env python3
"""Package the retained four-fleet Rototill AI-phase probe for Win16."""

import argparse
from pathlib import Path

from tests.test_rototill_retained_ai_phase import retained_header
from tools.package_rototill_scout_route_e2_native import (
    ROOT, package as route_package, write_dos,
)


def package(output, guest_dir):
    output = Path(output)
    route_package(output, guest_dir)
    write_dos(output / "SCRPROBE.C",
              (ROOT / "tests/do_rototill_retained_ai_phase_probe.c")
              .read_text(encoding="ascii"))
    write_dos(output / "ROTSCFX.H", retained_header().replace(
        '#include "RETFLX.H"', ''))
    # The shared compiler command renames this probe's main symbol to
    # rototill_route_probe. The report identifies the actual AI-phase test.
    source = (output / "SCRPROBE.C").read_bytes().replace(
        b'#include "RETFLX.H"', b'#include "ROTSCFX.H"')
    (output / "SCRPROBE.C").write_bytes(source)
    write_dos(output / "RTE2.C", """\
#include <windows.h>
#include <stdio.h>

int rototill_route_probe(void);
int printf(const char *format, ...)
{
    (void)format;
    return 0;
}

int PASCAL WinMain(HINSTANCE a, HINSTANCE b, LPSTR c, int d)
{
    FILE *report;
    int result;
    (void)a; (void)b; (void)c; (void)d;
    result = rototill_route_probe();
    report = fopen("RTAIRES.TXT", "w");
    if (report) {
        fprintf(report, "AI_PHASE_CHECKS=11 RESULT=%s\\n",
            result ? "FAIL" : "PASS");
        fclose(report);
    }
    return result || !report;
}
""")
    write_dos(output / "RTE2.DEF", """\
NAME RTE2 WINDOWAPI
DESCRIPTION 'Stars Rototill retained AI phase probe'
EXETYPE WINDOWS
STUB 'WINSTUB.EXE'
CODE PRELOAD MOVEABLE DISCARDABLE
DATA PRELOAD MOVEABLE MULTIPLE
HEAPSIZE 2048
STACKSIZE 8192
""")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--guest-dir", required=True)
    args = parser.parse_args()
    package(args.output, args.guest_dir)


if __name__ == "__main__":
    main()
