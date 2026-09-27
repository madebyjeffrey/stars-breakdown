#!/usr/bin/env python3
"""Stage the first segmented selector-3 coordinator for native compilation.

The generated C retains the complete callback path. It moves the retained
input and five large buffers out of TurinTrace, and keeps only selector 3's
input initializer. It is a compile diagnostic, not an E4 pass.
"""

import argparse
import hashlib
import json
import re
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
TURIN = ROOT / "tests/satisfy_orders_turin_binding_e3_probe.c"
COORD = ROOT / "tests/satisfy_orders_rototill_coordinator_e4_probe.c"
FIXTURE = ROOT / "compat/vc152/rototill-full-turn-r1/TURINFX.H"


def dos_text(path, body):
    path.write_bytes(body.replace("\r\n", "\n").replace("\r", "\n")
                     .replace("\n", "\r\n").encode("ascii"))


def split_long_initializer_lines(body, width=512):
    result = []
    for line in body.splitlines():
        if len(line) <= width:
            result.append(line)
            continue
        pieces = line.split(",")
        current = ""
        for index, piece in enumerate(pieces):
            token = piece + ("," if index + 1 < len(pieces) else "")
            if current and len(current) + len(token) > width:
                result.append(current)
                current = ""
            current += token
        if current:
            result.append(current)
    return "\n".join(result) + "\n"


def split_code_segments(body, prefix, thresholds):
    result = []
    pending = list(thresholds)
    segment = 1
    for line_number, line in enumerate(body.splitlines(), 1):
        if pending and line_number >= pending[0] and (
                line.startswith("static ") or line.startswith("int main(")):
            result.extend(("#if defined(_MSC_VER)",
                           '#pragma code_seg("%s%d")' % (prefix, segment),
                           "#endif"))
            segment += 1
            pending.pop(0)
        result.append(line)
    if pending:
        raise ValueError("could not place all code-segment boundaries")
    return "\n".join(result) + "\n"


def split_fixture_data_segments(fixture):
    declaration = re.search(
        r"^static const StarsU8 original_dgroup\[65536\] = \{([^}]*)\};$",
        fixture, re.M)
    if not declaration:
        raise ValueError("original DGROUP declaration changed")
    values = [int(token.strip()) for token in declaration.group(1).split(",")]
    if len(values) != 65536 or any(values[9000:]):
        raise ValueError("original DGROUP has data outside retained prefix")
    fixture = (fixture[:declaration.start()] +
               "static const StarsU8 original_dgroup[9000] = {" +
               ", ".join(str(value) for value in values[:9000]) + "};" +
               fixture[declaration.end():])
    pattern = re.compile(r"^static const [^\n]*?\[[^\n]*?= \{.*?\};",
                         re.M | re.S)
    matches = list(pattern.finditer(fixture))
    if len(matches) != len(re.findall(r"^static const .*\[", fixture, re.M)):
        raise ValueError("fixture array declaration was not segmented")
    result, cursor = [], 0
    segment, used = 1, 0
    unit_size = {"StarsU8": 1, "StarsI8": 1, "signed char": 1,
                 "StarsU16": 2, "StarsI16": 2,
                 "StarsU32": 4, "StarsI32": 4,
                 "StarsPoint": 4,
                 "Soe3CompleteInput": 57312}
    for match in matches:
        header = match.group().split("=", 1)[0]
        declared = re.fullmatch(
            r"static const (Stars[UI](?:8|16|32)|signed char|"
            r"Soe3CompleteInput|StarsPoint) [A-Za-z_]\w*"
            r"((?:\[[A-Za-z_0-9]+\])+)[ \t]*", header)
        if not declared:
            raise ValueError("unsupported fixture declaration: " + header)
        dimensions = []
        for value in re.findall(r"\[([A-Za-z_0-9]+)\]", declared.group(2)):
            if value.isdigit():
                dimensions.append(int(value))
            else:
                defined = re.search(r"^#define " + re.escape(value) +
                                    r" (\d+)U?$", fixture, re.M)
                if not defined:
                    raise ValueError("unknown fixture dimension: " + value)
                dimensions.append(int(defined.group(1)))
        size = unit_size[declared.group(1)]
        for dimension in dimensions:
            size *= dimension
        if size > 60000:
            raise ValueError("fixture array still exceeds one segment")
        if used and used + size > 48000:
            segment += 1
            used = 0
        used += size
        result.append(fixture[cursor:match.start()])
        result.append("#if defined(_MSC_VER)\n"
                      '#pragma data_seg("RF%05d")\n' % segment +
                      "#endif\n")
        result.append(match.group())
        result.append("\n#if defined(_MSC_VER)\n"
                      "#pragma data_seg()\n"
                      "#endif")
        cursor = match.end()
    result.append(fixture[cursor:])
    return "".join(result)


def stage(output, guest_dir, memory_model="AM", split_fixture=False):
    if output.exists():
        raise ValueError("stage directory exists; use a fresh path")
    if not guest_dir.startswith("C:\\") or len(guest_dir) > 16:
        raise ValueError("guest directory must be a short C:\\ path")
    if memory_model not in ("AM", "AL"):
        raise ValueError("unsupported memory model")
    output.mkdir(parents=True)
    turin = TURIN.read_text(encoding="ascii")
    coord = COORD.read_text(encoding="ascii")
    fixture = FIXTURE.read_text(encoding="ascii")
    field_replacements = {
        "    Soe3CompleteInput complete_input;":
            "    Soe3CompleteInput *complete_input;",
        "    StarsU8 ai_data[0x2000];":
            "    StarsU8 *ai_data;",
        "    StarsU8 rng_file_data[0x5000];":
            "    StarsU8 *rng_file_data;",
        "    StarsU8 guess_race_data_storage[0x5b22];":
            "    StarsU8 *guess_race_data_storage;",
        "    StarsU8 active_files[3][4096], backup_files[3][4096], backup_log[4096];":
            "    StarsU8 (*active_files)[4096], (*backup_files)[4096];\n"
            "    StarsU8 backup_log[4096];",
    }
    for old, new in field_replacements.items():
        if turin.count(old) != 1:
            raise ValueError("TurinTrace field changed: " + old)
        turin = turin.replace(old, new)
    # These scratch arrays only touch offsets through 0x5b21 and 0x547a
    # respectively in the retained selector-3 path. The original host test
    # used a blanket 64 KB array for each, which Win16 cannot allocate.
    for old, new in (("static StarsU8 string_data[65536];",
                      "static StarsU8 string_data[0x5600];"),
                     ("    StarsU8 data[65536];",
                      "    StarsU8 data[0x5b22];")):
        if turin.count(old) != 1:
            raise ValueError("scratch declaration changed: " + old)
        turin = turin.replace(old, new)
    original_main = "int main(int argc, char **argv)\n"
    if turin.count(original_main) != 1:
        raise ValueError("Turin standalone main changed")
    turin = turin[:turin.index(original_main)]
    allocation = (
        "    trace.complete_input = (Soe3CompleteInput *)malloc(\n"
        "        sizeof *trace.complete_input);\n"
        "    trace.rng_file_data = (StarsU8 *)calloc(1U, 0x5000U);\n"
        "    trace.guess_race_data_storage = (StarsU8 *)calloc(1U, 0x5b22U);\n"
        "    trace.ai_data = (StarsU8 *)calloc(1U, 0x2000U);\n"
        "    trace.active_files = (StarsU8 (*)[4096])calloc(3U, 4096U);\n"
        "    trace.backup_files = (StarsU8 (*)[4096])calloc(3U, 4096U);\n"
        "    if (!trace.complete_input || !trace.rng_file_data ||\n"
        "            !trace.guess_race_data_storage || !trace.ai_data ||\n"
        "            !trace.active_files || !trace.backup_files) return 4;\n"
    )
    for name, body in (("TURINB.C", turin), ("COORD.C", coord)):
        body = body.replace("->complete_input.", "->complete_input->")
        body = body.replace(".complete_input.", ".complete_input->")
        body = re.sub(r"([A-Za-z_]\w*(?:->|\.)complete_input) = (soe3_inputs\[[^;]+);",
                      r"*(\1) = \2;", body)
        body = body.replace("sizeof trace->ai_data", "0x2000U")
        body = body.replace("sizeof(trace->ai_data)", "0x2000U")
        body = body.replace("sizeof trace->guess_race_data_storage", "0x5b22U")
        if name == "TURINB.C":
            body = split_code_segments(body, "RTB", (2000, 4000, 6000,
                                                      8000, 10000))
        else:
            needle = "    memset(&trace, 0, sizeof trace);\n"
            if body.count(needle) != 1:
                raise ValueError("Rototill main initialization changed")
            body = body.replace(needle, needle + allocation)
            include = '#include "satisfy_orders_turin_binding_e3_probe.c"'
            if body.count(include) != 1:
                raise ValueError("coordinator include changed")
            body = body.replace(include, '#include "TURINB.C"')
            switch = "#undef stars_random\n"
            if body.count(switch) != 1:
                raise ValueError("coordinator segment switch changed")
            body = body.replace(switch, switch +
                "#if defined(_MSC_VER)\n"
                '#pragma code_seg("RTC0")\n'
                "#endif\n")
            body = split_code_segments(body, "RTC", (1400, 2800))
            main_marker = "int main(int argc, char **argv)\n"
            if body.count(main_marker) != 1:
                raise ValueError("Rototill main changed")
            start = body.index(main_marker)
            prefix, main_body = body[:start], body[start:]
            if main_body.count("    TurinTrace trace;\n") != 1:
                raise ValueError("Rototill trace declaration changed")
            main_body = main_body.replace("    TurinTrace trace;\n",
                "    TurinTrace *trace;\n")
            main_body = main_body.replace(needle,
                "    trace = (TurinTrace *)malloc(sizeof *trace);\n"
                "    if (!trace) return 4;\n"
                "    memset(trace, 0, sizeof *trace);\n")
            main_body = main_body.replace("trace.", "trace->")
            main_body = re.sub(r"&trace(?!->)", "trace", main_body)
            body = prefix + main_body
        if re.search(r"(?:->|\.)complete_input\.", body):
            raise ValueError("unconverted complete-input member access")
        dos_text(output / name, body)
    lines = fixture.splitlines()
    declaration = "static const Soe3CompleteInput soe3_inputs[2] = {"
    if lines.count(declaration) != 1:
        raise ValueError("fixture declaration changed")
    index = lines.index(declaration)
    if not lines[index + 1].startswith("{{") or not lines[index + 2].startswith("{{"):
        raise ValueError("fixture initializers changed")
    lines[index] = declaration.replace("[2]", "[1]")
    del lines[index + 2]
    fixture = split_long_initializer_lines("\n".join(lines) + "\n")
    if split_fixture:
        fixture = split_fixture_data_segments(fixture)
    dos_text(output / "TURINFX.H", fixture)
    for name, path in (("CORE.H", ROOT / "reconstructed/include/core.h"),
                       ("COMMON.H", ROOT / "reconstructed/include/common.h")):
        dos_text(output / name, path.read_text(encoding="ascii"))
    dos_text(output / "BUILD.BAT", """\
@echo off
if exist BLDSTAT.TXT goto stale
set INCLUDE=C:\\MSVC\\INCLUDE
C:\\MSVC\\BIN\\CL.EXE /c /%s /GA /GEs /Zp1 /Od /W3 COORD.C > COCL.LOG
if errorlevel 1 goto failed
echo COMPILE=PASS > BLDSTAT.TXT
goto done
:failed
echo COMPILE=FAIL > BLDSTAT.TXT
goto done
:stale
echo STALE=REFUSED > STALE.TXT
:done
""" % memory_model)
    dos_text(output / "RUNBLD.BAT", "@echo off\ncd " + guest_dir +
             "\ncall BUILD.BAT\n")
    manifest = {
        "schema": "rototill-segmented-package-v1",
        "scope": "Host-verified segmentation candidate; native compile pending",
        "guest_dir": guest_dir,
        "memory_model": memory_model,
        "split_fixture_data": split_fixture,
        "source_sha256": {path.name: hashlib.sha256(path.read_bytes()).hexdigest()
                          for path in (TURIN, COORD, FIXTURE)},
        "package_sha256": {path.name: hashlib.sha256(path.read_bytes()).hexdigest()
                           for path in sorted(output.iterdir())},
    }
    (output / "MANIFEST.JSON").write_text(
        json.dumps(manifest, indent=2, sort_keys=True) + "\n")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--guest-dir", required=True)
    parser.add_argument("--memory-model", choices=("AM", "AL"), default="AM")
    parser.add_argument("--split-fixture-data", action="store_true")
    args = parser.parse_args()
    stage(args.output, args.guest_dir, args.memory_model,
          args.split_fixture_data)


if __name__ == "__main__":
    main()
