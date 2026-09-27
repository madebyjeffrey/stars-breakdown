"""Stage one isolated home-planet task-5 original-turn candidate.

Only player 1 fleet 0's HST order changes. Its fleet, planet, coordinates,
and both player move files remain the byte-verified original inputs.
"""

import hashlib
import json
from pathlib import Path
import struct
import tempfile

from tools.race_file_compare import Inspector
from tools.stars_game_stream import decode_stream, encode_stream
from tools.satisfy_orders_original_oracle import decode_owner_fleet


ROOT = Path(__file__).resolve().parents[1]
INPUTS = {
    "CLEAN.HST": ("runs/vmctl-syact3/BACKUP/CLEAN.HST",
                  "aa6161bb6c95c8276b7a5d12e5a12d9999e3dea0fa242a3be89a94a05c45df3c"),
    "CLEAN.M1": ("runs/vmctl-syact3/BACKUP/CLEAN.M1",
                 "95d2d7fd27a39ea1668685beeb4785d876d9ed3cfb657f58173ea47e04e8c0a7"),
    "CLEAN.M2": ("runs/vmctl-syact3/BACKUP/CLEAN.M2",
                 "588830ac2225056422fa447cb201c7bd326e472652a8d3abf222b5fcceb99d4b"),
    "CLEAN.XY": ("runs/vmctl-syact3/CLEAN.XY",
                 "580e4c3be92eee570c95c97faa2557e3b4fe92856387970cf548700e1bb3fd88"),
    "STARS.EXE": ("target/stars.exe",
                  "ccab6682b55dae14dbf104e491cfbd3b32a6280022020032ae0615500949ba59"),
}


def prepare(destination):
    destination = Path(destination)
    if destination.exists():
        raise FileExistsError(destination)
    contents = {}
    for name, (relative, expected) in INPUTS.items():
        data = (ROOT / relative).read_bytes()
        if hashlib.sha256(data).hexdigest() != expected:
            raise ValueError("verified source changed: " + relative)
        contents[name] = data
    inspector = Inspector()
    records = decode_stream(ROOT / INPUTS["CLEAN.HST"][0], inspector)
    if encode_stream(records, inspector) != contents["CLEAN.HST"]:
        raise ValueError("HST does not round-trip")
    fleet = records[56]
    order = records[57]
    if (fleet["turn"], fleet["kind"]) != (0, 16):
        raise ValueError("player 1 fleet record moved")
    owner = decode_owner_fleet(fleet["body"])
    if (owner["id"], owner["owner"], owner["planet"], owner["point"],
            owner["ship_counts"][0], owner["order_count"]) != (
            0, 1, 24, [1290, 1258], 1, 1):
        raise ValueError("home-planet fleet changed")
    if (order["turn"], order["kind"], order["body"].hex()) != (
            0, 20, "0a05ea0418000011"):
        raise ValueError("fleet order changed")
    body = bytearray(order["body"] + bytes(10))
    struct.pack_into("<H", body, 6, 0x1105)
    records[57] = {**order, "kind": 19, "length": 18, "body": bytes(body)}
    encoded = encode_stream(records, inspector)
    with tempfile.TemporaryDirectory() as temporary:
        candidate_path = Path(temporary) / "CLEAN.HST"
        candidate_path.write_bytes(encoded)
        check = decode_stream(candidate_path, inspector)
    if len(check) != len(records) or any(
            (before["kind"], before["body"]) !=
            (after["kind"], after["body"])
            for index, (before, after) in enumerate(zip(records, check))
            if index != 57):
        raise ValueError("candidate changed another record")
    if check[57]["kind"] != 19 or check[57]["body"] != bytes(body):
        raise ValueError("candidate order did not persist")
    contents["CLEAN.HST"] = encoded
    destination.mkdir(parents=True)
    for name, data in contents.items():
        (destination / name).write_bytes(data)
    manifest = {
        "status": "candidate_input_only_not_original_behavior_evidence",
        "mutation": "HST turn-0 player-1 fleet-0 order 57: compact task 0 to full task 5; all other records and files unchanged",
        "source_fleet": owner,
        "files": {name: {"bytes": len(data),
                         "sha256": hashlib.sha256(data).hexdigest()}
                  for name, data in sorted(contents.items())},
    }
    (destination / "MANIFEST.JSON").write_text(
        json.dumps(manifest, indent=2, sort_keys=True) + "\n")
    return manifest


if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("destination", type=Path)
    args = parser.parse_args()
    print(json.dumps(prepare(args.destination)["mutation"]))
