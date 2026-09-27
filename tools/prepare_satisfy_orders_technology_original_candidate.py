"""Prepare a paired, isolated cross-owner task-5 technology experiment."""

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


def _source_files():
    result = {}
    for name, (relative, expected) in INPUTS.items():
        data = (ROOT / relative).read_bytes()
        if hashlib.sha256(data).hexdigest() != expected:
            raise ValueError("verified source changed: " + relative)
        result[name] = data
    return result


def _candidate_hst(source_path, source, task5):
    inspector = Inspector()
    records = decode_stream(source_path, inspector)
    if encode_stream(records, inspector) != source:
        raise ValueError("input HST does not round-trip")
    fleet_index, order_index = 56, 57
    fleet = records[fleet_index]
    order = records[order_index]
    fleet_fields = decode_owner_fleet(fleet["body"])
    if (fleet["kind"], fleet["turn"], fleet_fields["id"],
            fleet_fields["owner"], fleet_fields["planet"],
            fleet_fields["point"], order["kind"], order["body"].hex()) != (
            16, 0, 0, 1, 24, [1290, 1258], 20,
            "0a05ea0418000011"):
        raise ValueError("player-1 home fleet-0 source changed")

    # Keep a coherent physical record: the fleet is now already orbiting
    # owner-0 planet 10 at its stored map point. Ownership of both players
    # and the planet table stays unchanged.
    fleet_body = bytearray(fleet["body"])
    struct.pack_into("<hhh", fleet_body, 6, 10, 1128, 1300)
    records[fleet_index] = {**fleet, "body": bytes(fleet_body)}
    order_body = bytearray(order["body"] + bytes(10))
    struct.pack_into("<hhhH", order_body, 0, 1128, 1300, 10,
                     0x1105 if task5 else 0x1100)
    records[order_index] = {**order, "kind": 19, "length": 18,
                            "body": bytes(order_body)}

    encoded = encode_stream(records, inspector)
    with tempfile.TemporaryDirectory() as directory:
        roundtrip_path = Path(directory) / "CLEAN.HST"
        roundtrip_path.write_bytes(encoded)
        decoded = decode_stream(roundtrip_path, inspector)
    if len(decoded) != len(records) or any(
            (left["kind"], left["body"]) != (right["kind"], right["body"])
            for left, right in zip(records, decoded)):
        raise ValueError("candidate HST does not round-trip")
    return encoded, fleet_index, order_index


def prepare_pair(control_directory, action_directory):
    control_directory = Path(control_directory)
    action_directory = Path(action_directory)
    if control_directory.exists() or action_directory.exists():
        raise FileExistsError("choose new candidate output directories")
    files = _source_files()
    hst_source = ROOT / INPUTS["CLEAN.HST"][0]
    control_hst, *control_indices = _candidate_hst(
        hst_source, files["CLEAN.HST"], False)
    action_hst, *action_indices = _candidate_hst(
        hst_source, files["CLEAN.HST"], True)
    if control_indices != action_indices:
        raise ValueError("fleet/order indices moved between candidates")
    manifests = []
    for destination, hst, action in (
            (control_directory, control_hst, False),
            (action_directory, action_hst, True)):
        destination.mkdir(parents=True)
        candidate_files = dict(files)
        candidate_files["CLEAN.HST"] = hst
        for name, data in candidate_files.items():
            (destination / name).write_bytes(data)
        manifest = {
            "status": "candidate_input_only_not_original_behavior_evidence",
            "mutation": "move player-1 fleet 0 from home planet 24 to owner-0 planet 10 at (1128,1300), preserving both planet and player ownership; " +
                        ("set its valid full order to task 5"
                         if action else "set a valid full task-0 order to the same planet"),
            "purpose": "paired input tests whether dismantling an opposing fleet at its original target planet yields an original signed technology result",
            "files": {name: {"bytes": len(data),
                             "sha256": hashlib.sha256(data).hexdigest()}
                      for name, data in sorted(candidate_files.items())},
        }
        (destination / "MANIFEST.JSON").write_text(
            json.dumps(manifest, indent=2, sort_keys=True) + "\n")
        manifests.append(manifest)
    return manifests


if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("control", type=Path)
    parser.add_argument("action", type=Path)
    args = parser.parse_args()
    for manifest in prepare_pair(args.control, args.action):
        print(manifest["mutation"])
