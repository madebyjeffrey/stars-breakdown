"""Prepare a race-bit-C task-0/task-5 original-executable pair.

Inputs are the retained high-offer trader-technology pair. Only the player-1
race bit that enables task-5 SHDEF repricing is added to both, preserving their
shared H2 and the single task-flag difference between control and action.
"""

import hashlib
import json
from pathlib import Path
import struct
import tempfile

from tools.race_file_compare import Inspector
from tools.stars_game_stream import decode_stream, encode_stream


ROOT = Path(__file__).resolve().parents[1]
SOURCES = {
    "control": ROOT / "runs/syorig-tech-trader-v3-seed1-control",
    "action": ROOT / "runs/syorig-tech-trader-v3-seed1-action",
}
OUTPUTS = {
    "control": ROOT / "runs/syorig-tech-trader-v4-bitc-seed1-control",
    "action": ROOT / "runs/syorig-tech-trader-v4-bitc-seed1-action",
}
FILES = ("CLEAN.H2", "CLEAN.HST", "CLEAN.M1", "CLEAN.M2", "CLEAN.XY",
         "STARS.EXE")
RACE_BITS_OFFSET = 78
RACE_REPRICE_BIT = 1 << 12


def sha256(data):
    return hashlib.sha256(data).hexdigest()


def read_verified_source(directory):
    manifest = json.loads((directory / "MANIFEST.JSON").read_text())
    files = {}
    for name in FILES:
        data = (directory / name).read_bytes()
        expected = (manifest["shared_h2_seed"]["sha256"] if name ==
                    "CLEAN.H2" else manifest["files"][name]["sha256"])
        if sha256(data) != expected:
            raise ValueError("source hash mismatch: %s/%s" % (directory, name))
        files[name] = data
    return files


def set_reprice_bit(hst_bytes):
    inspector = Inspector()
    with tempfile.TemporaryDirectory() as temp:
        source = Path(temp) / "CLEAN.HST"
        source.write_bytes(hst_bytes)
        records = decode_stream(source, inspector)
    players = [(index, record) for index, record in enumerate(records)
               if record["kind"] == 6 and record["turn"] == 0]
    target = [(index, record) for index, record in players
              if record["body"][0] == 1]
    if len(players) != 2 or len(target) != 1:
        raise ValueError("expected exactly the two verified player records")
    index, player = target[0]
    body = bytearray(player["body"])
    old_bits = struct.unpack_from("<I", body, RACE_BITS_OFFSET)[0]
    if old_bits & RACE_REPRICE_BIT:
        raise ValueError("source already enables race-bit-C repricing")
    struct.pack_into("<I", body, RACE_BITS_OFFSET,
                     old_bits | RACE_REPRICE_BIT)
    records[index] = {**player, "body": bytes(body)}
    encoded = encode_stream(records, inspector)
    with tempfile.TemporaryDirectory() as temp:
        path = Path(temp) / "CLEAN.HST"
        path.write_bytes(encoded)
        decoded = decode_stream(path, inspector)
    if len(decoded) != len(records) or any(
            (left["kind"], left["turn"], left["body"]) !=
            (right["kind"], right["turn"], right["body"])
            for left, right in zip(records, decoded)):
        raise ValueError("race-bit-C HST did not round-trip")
    return encoded, old_bits, old_bits | RACE_REPRICE_BIT, index, records


def prepare_pair():
    if any(path.exists() for path in OUTPUTS.values()):
        raise FileExistsError("choose new output directories; existing data is preserved")
    source_files = {side: read_verified_source(path)
                    for side, path in SOURCES.items()}
    encoded = {}
    decoded = {}
    details = {}
    for side in ("control", "action"):
        hst, old_bits, new_bits, player_index, records = set_reprice_bit(
            source_files[side]["CLEAN.HST"])
        source_files[side]["CLEAN.HST"] = hst
        encoded[side] = hst
        decoded[side] = records
        details[side] = dict(player_record=player_index,
                             old_race_bits="0x%08X" % old_bits,
                             new_race_bits="0x%08X" % new_bits)
    if source_files["control"]["CLEAN.H2"] != source_files["action"]["CLEAN.H2"]:
        raise ValueError("control/action do not share the H2 seed")
    differences = [index for index, (left, right) in enumerate(zip(
        decoded["control"], decoded["action"]))
        if (left["kind"], left["turn"], left["body"]) !=
           (right["kind"], right["turn"], right["body"])]
    if differences != [57]:
        raise ValueError("candidate pair differs outside task-flag record 57")
    manifests = []
    for side in ("control", "action"):
        destination = OUTPUTS[side]
        destination.mkdir(parents=True)
        for name, data in source_files[side].items():
            (destination / name).write_bytes(data)
        manifest = {
            "status": "candidate_input_only_not_original_behavior_evidence",
            "pair": "reprice-v4-bitc-seed1",
            "side": side,
            "mutation": "set player-1 race bit C (bit 12) in HST; retain v3 hull-valid Battleship with 11 trader components; task 0 control or task 5 action",
            "player_race_bits": details[side],
            "paired_difference_record_indices": differences,
            "shared_h2_seed": {
                "bytes": len(source_files[side]["CLEAN.H2"]),
                "sha256": sha256(source_files[side]["CLEAN.H2"]),
                "source": str(SOURCES["action"].relative_to(ROOT) / "CLEAN.H2"),
            },
            "files": {name: {"bytes": len(data), "sha256": sha256(data)}
                      for name, data in sorted(source_files[side].items())},
        }
        (destination / "MANIFEST.JSON").write_text(
            json.dumps(manifest, indent=2, sort_keys=True) + "\n",
            encoding="ascii")
        manifests.append(manifest)
    return manifests


if __name__ == "__main__":
    for item in prepare_pair():
        print(json.dumps({key: item[key] for key in
            ("side", "player_race_bits", "paired_difference_record_indices",
             "shared_h2_seed")}, sort_keys=True))
