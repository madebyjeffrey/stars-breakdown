"""Stage a task-5 order on a naturally occurring deep-space SMOKE fleet."""

import hashlib
import json
from pathlib import Path
import struct
import tempfile

from tools.race_file_compare import Inspector
from tools.satisfy_orders_original_oracle import decode_owner_fleet
from tools.stars_game_stream import decode_stream, encode_stream


ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / "runs/step4/fixtures/replay-a"
EXPECTED = {
    "SMOKE.XY": "fea362759635c3cb7d1f223c2ff574efee67d36202622fced79c6629897bda10",
    "SMOKE.HST": "63b07c320932ff3919b4ee5c5d13ef6f20d3df03ce70e1f8cf5fa0cd4b33f934",
    "SMOKE.M1": "7873e1d5350fba36abbf7ec7155fbc306677ed51c195480c069a16ad237cad67",
    "SMOKE.M2": "3f56033bf15a0ca6ff86eafdb509d9853c56b2a1ee80108a2c29e0462e094dff",
    "SMOKE.M3": "5115ca39c3d9d7bfa9c2ef04de6bf20ad472b89d754dac8ee14643425cb057c7",
}
EXE = ROOT / "target/stars.exe"
EXE_HASH = "ccab6682b55dae14dbf104e491cfbd3b32a6280022020032ae0615500949ba59"


def _edit(path, fleet_index, order_index, inspector):
    records = decode_stream(path, inspector)
    if encode_stream(records, inspector) != path.read_bytes():
        raise ValueError("original stream does not round-trip: " + str(path))
    fleet = records[fleet_index]
    order = records[order_index]
    parsed = decode_owner_fleet(fleet["body"])
    if (fleet["kind"], fleet["turn"], parsed["id"], parsed["owner"],
            parsed["planet"], parsed["point"], parsed["order_count"]) != (
            16, 1, 0, 1, -1, [1276, 1380], 2):
        raise ValueError("deep-space source fleet changed")
    if (order["kind"], order["turn"], order["body"].hex()) != (
            20, 1, "fc04640500000014"):
        raise ValueError("deep-space source order changed")
    changed = bytearray(order["body"])
    struct.pack_into("<H", changed, 6, 0x1405)
    records[order_index] = {**order, "body": bytes(changed)}
    encoded = encode_stream(records, inspector)
    with tempfile.TemporaryDirectory() as directory:
        candidate = Path(directory) / path.name
        candidate.write_bytes(encoded)
        decoded = decode_stream(candidate, inspector)
    if len(decoded) != len(records) or any(
            (left["kind"], left["body"]) != (right["kind"], right["body"])
            for left, right in zip(records, decoded)):
        raise ValueError("edited stream does not round-trip")
    return encoded


def prepare(destination):
    destination = Path(destination)
    if destination.exists():
        raise FileExistsError(destination)
    contents = {}
    for name, expected in EXPECTED.items():
        data = (SOURCE / name).read_bytes()
        if hashlib.sha256(data).hexdigest() != expected:
            raise ValueError("verified SMOKE fixture changed: " + name)
        contents[name] = data
    exe = EXE.read_bytes()
    if hashlib.sha256(exe).hexdigest() != EXE_HASH:
        raise ValueError("original executable changed")
    contents["STARS.EXE"] = exe
    inspector = Inspector()
    contents["SMOKE.HST"] = _edit(SOURCE / "SMOKE.HST", 158, 159, inspector)
    contents["SMOKE.M2"] = _edit(SOURCE / "SMOKE.M2", 7, 8, inspector)
    destination.mkdir(parents=True)
    for name, data in contents.items():
        (destination / name).write_bytes(data)
    manifest = {
        "status": "candidate_input_only_not_original_behavior_evidence",
        "mutation": "HST and M2 existing turn-1 owner-1 deep-space fleet 0 first order: point target valid task 0x1400 -> task 5 0x1405; all other records/files unchanged",
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
    print(prepare(args.destination)["mutation"])
