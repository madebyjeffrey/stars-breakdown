"""Stage paired SMOKE turns with the deep-space fleet owner set to human."""

import hashlib
import json
from pathlib import Path
import struct
import tempfile

from tools.prepare_satisfy_orders_smoke_deepspace import (
    EXE, EXE_HASH, EXPECTED, SOURCE)
from tools.race_file_compare import Inspector
from tools.stars_game_stream import decode_stream, encode_stream


def _modify(path, player_index, order_index, task5, inspector):
    records = decode_stream(path, inspector)
    if encode_stream(records, inspector) != path.read_bytes():
        raise ValueError("source stream does not round-trip: " + str(path))
    player = records[player_index]
    body = bytearray(player["body"])
    if player["kind"] != 6 or body[0] != 1 or struct.unpack_from("<H", body, 6)[0] != 0x273f:
        raise ValueError("owner-1 AI flag moved")
    struct.pack_into("<H", body, 6, 0x253f)
    records[player_index] = {**player, "body": bytes(body)}
    if task5:
        order = records[order_index]
        body = bytearray(order["body"])
        if order["kind"] != 20 or body.hex() != "fc04640500000014":
            raise ValueError("natural deep-space order moved")
        struct.pack_into("<H", body, 6, 0x1405)
        records[order_index] = {**order, "body": bytes(body)}
    encoded = encode_stream(records, inspector)
    with tempfile.TemporaryDirectory() as directory:
        candidate = Path(directory) / path.name
        candidate.write_bytes(encoded)
        decoded = decode_stream(candidate, inspector)
    if len(decoded) != len(records) or any(
            (left["kind"], left["body"]) != (right["kind"], right["body"])
            for left, right in zip(records, decoded)):
        raise ValueError("candidate stream does not round-trip")
    return encoded


def prepare(destination, task5):
    destination = Path(destination)
    if destination.exists():
        raise FileExistsError(destination)
    contents = {}
    for name, expected in EXPECTED.items():
        data = (SOURCE / name).read_bytes()
        if hashlib.sha256(data).hexdigest() != expected:
            raise ValueError("verified source changed: " + name)
        contents[name] = data
    exe = EXE.read_bytes()
    if hashlib.sha256(exe).hexdigest() != EXE_HASH:
        raise ValueError("original executable changed")
    contents["STARS.EXE"] = exe
    inspector = Inspector()
    contents["SMOKE.HST"] = _modify(SOURCE / "SMOKE.HST", 2, 159,
                                     task5, inspector)
    contents["SMOKE.M2"] = _modify(SOURCE / "SMOKE.M2", 1, 8,
                                    task5, inspector)
    destination.mkdir(parents=True)
    for name, data in contents.items():
        (destination / name).write_bytes(data)
    manifest = {
        "status": "candidate_input_only_not_original_behavior_evidence",
        "mutation": ("HST and M2 owner-1 det word at body offset 6: 0x273f -> 0x253f (clear fAi bit 9); " +
                     ("HST and M2 fleet-0 first order 0x1400 -> 0x1405" if task5
                      else "fleet-0 first order unchanged at 0x1400")),
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
    parser.add_argument("--task5", action="store_true")
    args = parser.parse_args()
    print(prepare(args.destination, args.task5)["mutation"])
