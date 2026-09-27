"""Stage a single-race-bit task-5 resource-path original-turn candidate."""

import hashlib
import json
from pathlib import Path
import struct
import tempfile

from tools.race_file_compare import Inspector
from tools.stars_game_stream import decode_stream, encode_stream


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
        raise ValueError("source HST does not round-trip")
    player = records[1]
    if (player["turn"], player["kind"], len(player["body"]),
            player["body"][0], player["body"][76],
            struct.unpack_from("<I", player["body"], 78)[0]) != (
            0, 6, 130, 0, 9, 0):
        raise ValueError("owner-0 race record changed")
    order = records[45]
    if (order["kind"], order["body"].hex()) != (
            19, "680414050a00051100000000000000000000"):
        raise ValueError("known successful task-5 order changed")
    body = bytearray(player["body"])
    struct.pack_into("<I", body, 78, 1 << 5)
    records[1] = {**player, "body": bytes(body)}
    encoded = encode_stream(records, inspector)
    with tempfile.TemporaryDirectory() as temporary:
        candidate = Path(temporary) / "CLEAN.HST"
        candidate.write_bytes(encoded)
        decoded = decode_stream(candidate, inspector)
    if len(decoded) != len(records) or any(
            (left["kind"], left["body"]) != (right["kind"], right["body"])
            for left, right in zip(records, decoded)):
        raise ValueError("candidate does not round-trip")
    contents["CLEAN.HST"] = encoded
    destination.mkdir(parents=True)
    for name, data in contents.items():
        (destination / name).write_bytes(data)
    manifest = {
        "status": "candidate_input_only_not_original_behavior_evidence",
        "mutation": "HST turn-0 player-0 race bits at body offset 78: 0 to 0x20; all other records and files unchanged",
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
