"""Prepare a slot-verified original task-5 trader-technology pair."""

import hashlib
import json
from pathlib import Path
import struct
import tempfile

from tools.race_file_compare import Inspector
from tools.stars_game_stream import decode_stream, encode_stream
from tools.satisfy_orders_original_oracle import decode_owner_fleet
from tools.prepare_satisfy_orders_technology_original_candidate import (
    INPUTS, _candidate_hst, _source_files,
)


ROOT = Path(__file__).resolve().parents[1]
FLEET_RECORD = 56
FLEET_ID = 0
FLEET_OWNER = 1
FLEET_DESIGN_BITMAP = 0x0007
SHIP_DESIGN_RECORDS = {0: 41, 1: 42, 2: 43}

# slot, component index, group, old item, new item, count, evidence label
TRADER_PARTS = (
    (0, 0, 0x0001, 1, 8, 1, "Enigma Pulsar", 9),
    (0, 2, 0x1000, 5, 4, 1, "Multi-Cargo Pod", 0),
    (1, 0, 0x0001, 1, 8, 1, "Enigma Pulsar", 9),
    (1, 1, 0x1000, 0, 4, 1, "Multi-Cargo Pod", 0),
    (2, 0, 0x0001, 1, 8, 1, "Enigma Pulsar", 9),
    (2, 1, 0x0080, 0, 6, 2, "Alien Miner", 4),
)


def _verify_owner_slots(records):
    players = [r for r in records if r["kind"] == 6]
    if [(r["body"][0], r["body"][1]) for r in players] != [(0, 6), (1, 3)]:
        raise ValueError("verified HST player ship-definition counts changed")
    definitions = [
        i for i, r in enumerate(records)
        if r["kind"] == 26 and
        ((struct.unpack_from("<H", r["body"])[0] >> 10) & 0x1F) < 16
    ]
    if len(definitions) != 9:
        raise ValueError("verified HST ship-definition count changed")
    owner_by_record = {}
    cursor = 0
    for player in players:
        for index in definitions[cursor:cursor + player["body"][1]]:
            owner_by_record[index] = player["body"][0]
        cursor += player["body"][1]
    if cursor != len(definitions):
        raise ValueError("ship definitions do not match saved player counts")

    for slot, record_index in SHIP_DESIGN_RECORDS.items():
        record = records[record_index]
        flags = struct.unpack_from("<H", record["body"])[0]
        actual_slot = (flags >> 10) & 0x1F
        if (owner_by_record.get(record_index), actual_slot, record["kind"]) != (
                FLEET_OWNER, slot, 26):
            raise ValueError("target fleet design owner/slot mapping changed")


def _add_designs_to_target_fleet(records):
    record = records[FLEET_RECORD]
    fleet = decode_owner_fleet(record["body"])
    if (record["kind"], fleet["id"], fleet["owner"], fleet["ship_bitmap"],
            fleet["ship_counts"][0]) != (16, FLEET_ID, FLEET_OWNER, 1, 1):
        raise ValueError("verified target fleet-0 source changed")

    body = bytearray(record["body"])
    struct.pack_into("<H", body, 12, FLEET_DESIGN_BITMAP)
    # The saved fleet flags select byte or word ship counts. This input uses
    # byte counts, so replace its one count byte with the three present slots.
    width = 1 if fleet["saved_flags"] & 0x0800 else 2
    counts = b"".join(value.to_bytes(width, "little") for value in (1, 1, 1))
    body = body[:14] + counts + body[14 + width:]
    updated = decode_owner_fleet(body)
    if (updated["ship_bitmap"], updated["ship_counts"][:3]) != (7, [1, 1, 1]):
        raise ValueError("target fleet design bitmap/counts failed round-trip")
    records[FLEET_RECORD] = {**record, "body": bytes(body)}


def _add_trader_parts(records):
    for slot, component, expected_group, old_item, item, count, *_ in TRADER_PARTS:
        record_index = SHIP_DESIGN_RECORDS[slot]
        record = records[record_index]
        body = bytearray(record["body"])
        ncomponents = body[6]
        if component >= ncomponents:
            raise ValueError("trader component is outside its saved hull slots")
        offset = 17 + component * 4
        group, packed = struct.unpack_from("<HH", body, offset)
        if (group, packed & 0xFF) != (expected_group, old_item):
            raise ValueError("target design component source changed")
        struct.pack_into("<HH", body, offset, group, (count << 8) | item)
        records[record_index] = {**record, "body": bytes(body)}


def _candidate(task5):
    files = _source_files()
    source_path = ROOT / INPUTS["CLEAN.HST"][0]
    encoded, fleet_index, order_index = _candidate_hst(
        source_path, files["CLEAN.HST"], task5)
    if (fleet_index, order_index) != (FLEET_RECORD, 57):
        raise ValueError("verified target fleet/order record indices changed")
    inspector = Inspector()
    with tempfile.TemporaryDirectory() as directory:
        path = Path(directory) / "CLEAN.HST"
        path.write_bytes(encoded)
        records = decode_stream(path, inspector)
    _verify_owner_slots(records)
    _add_designs_to_target_fleet(records)
    _add_trader_parts(records)
    encoded = encode_stream(records, inspector)
    with tempfile.TemporaryDirectory() as directory:
        path = Path(directory) / "CLEAN.HST"
        path.write_bytes(encoded)
        decoded = decode_stream(path, inspector)
    if len(decoded) != len(records) or any(
            (left["kind"], left["body"]) != (right["kind"], right["body"])
            for left, right in zip(records, decoded)):
        raise ValueError("candidate HST did not round-trip")
    _verify_owner_slots(decoded)
    fleet = decode_owner_fleet(decoded[FLEET_RECORD]["body"])
    if (fleet["ship_bitmap"], fleet["ship_counts"][:3]) != (7, [1, 1, 1]):
        raise ValueError("target fleet changed after stream round-trip")
    return files, encoded, order_index, decoded


def prepare_pair(control_directory, action_directory):
    control_directory, action_directory = map(Path,
                                               (control_directory, action_directory))
    if control_directory.exists() or action_directory.exists():
        raise FileExistsError("choose new candidate output directories")
    control_files, control_hst, control_order, control_records = _candidate(False)
    action_files, action_hst, action_order, action_records = _candidate(True)
    if control_files != action_files or control_order != action_order:
        raise ValueError("paired candidates do not share the verified inputs")
    differences = [i for i, (left, right) in enumerate(
        zip(control_records, action_records))
        if (left["kind"], left["body"]) != (right["kind"], right["body"])]
    if differences != [control_order]:
        raise ValueError("paired inputs differ outside the task flag")

    result = []
    for destination, hst, task5 in (
            (control_directory, control_hst, False),
            (action_directory, action_hst, True)):
        destination.mkdir(parents=True)
        candidate_files = dict(control_files)
        candidate_files["CLEAN.HST"] = hst
        for name, data in candidate_files.items():
            (destination / name).write_bytes(data)
        manifest = {
            "status": "candidate_input_only_not_original_behavior_evidence",
            "mutation": (
                "move player-1 fleet 0 to owner-0 planet 10, add one ship each "
                "from verified player-1 design slots 0-2, and replace only "
                "hull-valid parts with trader-science parts; "
                + ("use task 5" if task5 else "use task 0")
            ),
            "target": {"fleet_record": FLEET_RECORD, "fleet_id": FLEET_ID,
                       "owner": FLEET_OWNER, "ship_design_bitmap": 7,
                       "design_slots": [0, 1, 2]},
            "designs": [
                {"slot": slot, "record_index": SHIP_DESIGN_RECORDS[slot],
                 "changes": [
                     {"component": component, "group": group,
                      "source_item": old, "item": item, "count": count,
                      "name": name, "trader_tech": tech}
                     for s, component, group, old, item, count, name, tech
                     in TRADER_PARTS if s == slot]}
                for slot in SHIP_DESIGN_RECORDS
            ],
            "paired_difference_record_indices": differences,
            "files": {name: {"bytes": len(data),
                             "sha256": hashlib.sha256(data).hexdigest()}
                      for name, data in sorted(candidate_files.items())},
        }
        (destination / "MANIFEST.JSON").write_text(
            json.dumps(manifest, indent=2, sort_keys=True) + "\n")
        result.append(manifest)
    return result


if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("control", type=Path)
    parser.add_argument("action", type=Path)
    args = parser.parse_args()
    for manifest in prepare_pair(args.control, args.action):
        print(manifest["mutation"])
