#!/usr/bin/env python3
"""Measure host coordinator layout to plan a segmented VC1.52c harness.

This is an architecture diagnostic, not a Win16 ABI size measurement.
"""

import argparse
import hashlib
import json
import re
import subprocess
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
COORD = ROOT / "tests/satisfy_orders_rototill_coordinator_e4_probe.c"
TURIN = ROOT / "tests/satisfy_orders_turin_binding_e3_probe.c"
DEFAULT_FIXTURE = ROOT / "compat/vc152/rototill-full-turn-r1/TURINFX.H"
FIELD = re.compile(r"^\s*(\d+) \| {3}(?! )(?:struct )?[^\n]*?\s([A-Za-z_]\w*)$")
SIZE = re.compile(r"\[sizeof=(\d+), align=(\d+)\]")


def record_layout(text, name):
    marker = "struct " + name + "\n"
    pos = text.find(marker)
    if pos < 0:
        raise ValueError("record layout missing: " + name)
    end = text.find("[sizeof=", pos)
    if end < 0:
        raise ValueError("record size missing: " + name)
    lines = text[pos:end].splitlines()[1:]
    fields = []
    for line in lines:
        match = FIELD.match(line)
        if match:
            fields.append({"name": match.group(2), "offset": int(match.group(1))})
    size_match = SIZE.search(text[end:])
    if not size_match or not fields:
        raise ValueError("incomplete record layout: " + name)
    size = int(size_match.group(1))
    for index, field in enumerate(fields):
        field["span_to_next"] = (fields[index + 1]["offset"]
                                 if index + 1 < len(fields) else size) - field["offset"]
    return {"size": size, "align": int(size_match.group(2)), "fields": fields}


def segments(layout, limit):
    result = []
    current = None
    for field in layout["fields"]:
        if current is None or current["host_span"] + field["span_to_next"] > limit:
            if current is not None:
                result.append(current)
            current = {"first": field["name"], "last": field["name"],
                       "host_span": 0, "field_count": 0}
        current["last"] = field["name"]
        current["host_span"] += field["span_to_next"]
        current["field_count"] += 1
    if current is not None:
        result.append(current)
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--fixture", type=Path, default=DEFAULT_FIXTURE)
    parser.add_argument("--limit", type=int, default=48000)
    parser.add_argument("--output", type=Path)
    args = parser.parse_args()
    if args.limit <= 0:
        parser.error("--limit must be positive")
    command = ["clang", "-std=c89", "-fsyntax-only", "-I",
               str(ROOT / "reconstructed/include"), "-I", str(args.fixture.parent),
               "-Xclang", "-fdump-record-layouts", str(COORD)]
    run = subprocess.run(command, capture_output=True, text=True)
    if run.returncode:
        raise SystemExit(run.stderr or run.stdout)
    layout = run.stdout + run.stderr
    trace = record_layout(layout, "TurinTrace")
    input_record = record_layout(layout, "Soe3CompleteInput")
    fixture_text = args.fixture.read_text(encoding="ascii")
    declaration = re.search(r"static const Soe3CompleteInput soe3_inputs\[(\d+)\]",
                            fixture_text)
    if not declaration:
        raise ValueError("fixture input declaration missing")
    fixture_count = int(declaration.group(1))
    result = {
        "schema": "rototill-native-layout-audit-v1",
        "scope": "host ABI sizing only; VC1.52c sizes require guest validation",
        "source_sha256": {path.name: hashlib.sha256(path.read_bytes()).hexdigest()
                          for path in (COORD, TURIN, args.fixture)},
        "trace_host_bytes": trace["size"],
        "complete_input_host_bytes": input_record["size"],
        "fixture_input_count": fixture_count,
        "fixture_input_host_bytes": fixture_count * input_record["size"],
        "partition_limit_host_bytes": args.limit,
        "suggested_contiguous_partitions": segments(trace, args.limit),
        "largest_fields": sorted(trace["fields"],
                                 key=lambda field: field["span_to_next"],
                                 reverse=True)[:12],
    }
    output = json.dumps(result, indent=2, sort_keys=True) + "\n"
    if args.output:
        args.output.write_text(output, encoding="utf-8")
    else:
        print(output, end="")


if __name__ == "__main__":
    main()
