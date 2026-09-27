"""Stage a pinned, isolated Stars! action-candidate original-turn bundle.

The bundle is input only. Running the original executable and interpreting its
output remain separate validation steps. An existing destination is refused.
"""

import argparse
import hashlib
import json
from pathlib import Path

from tools.race_file_compare import Inspector
from tools.satisfy_orders_action_candidate import (SOURCE_SHA256,
                                                   make_candidate,
                                                   make_host_candidate)


ROOT = Path(__file__).resolve().parents[1]
INPUTS = {
    "CLEAN.XY": ("runs/vmctl-syorig1/CLEAN.XY",
                 "580e4c3be92eee570c95c97faa2557e3b4fe92856387970cf548700e1bb3fd88"),
    "CLEAN.HST": ("runs/vmctl-syorig1/BACKUP/CLEAN.HST",
                  "74a27a31221f67328a98200c5dfde54f4c955a6498cc37ac08624b64a33997f6"),
    "CLEAN.M2": ("runs/vmctl-syorig1/BACKUP/CLEAN.M2",
                 "588830ac2225056422fa447cb201c7bd326e472652a8d3abf222b5fcceb99d4b"),
    "STARS.EXE": ("target/stars.exe",
                  "ccab6682b55dae14dbf104e491cfbd3b32a6280022020032ae0615500949ba59"),
}


def prepare(destination, host_order=False, m1_order=True):
    destination = Path(destination)
    if destination.exists():
        raise ValueError("destination exists; use a fresh isolated directory")
    contents = {}
    for name, (relative, expected) in INPUTS.items():
        data = (ROOT / relative).read_bytes()
        if hashlib.sha256(data).hexdigest() != expected:
            raise ValueError("verified input changed: " + relative)
        contents[name] = data
    m1_source = ROOT / "runs/vmctl-syorig1/BACKUP/CLEAN.M1"
    if m1_order:
        contents["CLEAN.M1"] = make_candidate(m1_source, Inspector())
    else:
        contents["CLEAN.M1"] = m1_source.read_bytes()
        if hashlib.sha256(contents["CLEAN.M1"]).hexdigest() != SOURCE_SHA256:
            raise ValueError("verified M1 input changed")
    if host_order:
        contents["CLEAN.HST"] = make_host_candidate(
            ROOT / "runs/vmctl-syorig1/BACKUP/CLEAN.HST", Inspector())
    manifest = {
        "status": "candidate_input_only_not_original_behavior_evidence",
        "order_change": ("first turn-0 type-20 task-0 order to type-19 task-5; "
                         "extra union zeroed; " +
                         ("HST and M1" if host_order and m1_order else
                          "HST only" if host_order else "M1 only")),
        "files": {name: {"bytes": len(data),
                         "sha256": hashlib.sha256(data).hexdigest()}
                  for name, data in sorted(contents.items())},
    }
    destination.mkdir(parents=True)
    for name, data in contents.items():
        (destination / name).write_bytes(data)
    (destination / "MANIFEST.JSON").write_text(
        json.dumps(manifest, indent=2, sort_keys=True) + "\n", encoding="ascii")
    return manifest


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("destination", type=Path)
    parser.add_argument("--host-order", action="store_true")
    parser.add_argument("--host-only", action="store_true")
    args = parser.parse_args()
    manifest = prepare(args.destination,
                       host_order=args.host_order or args.host_only,
                       m1_order=not args.host_only)
    print("staged %d pinned files in %s" %
          (len(manifest["files"]), args.destination))


if __name__ == "__main__":
    main()
