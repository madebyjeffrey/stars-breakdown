"""Prepare a high-offer, hull-valid original task-5 technology pair."""

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
TARGET_RECORD = 41
TARGET_OWNER = 1
TARGET_SLOT = 0
FLEET_RECORD = 56
ORDER_RECORD = 57

# group, item, count, trader tech. Slot masks and capacities come from the
# original Battleship HUL (hull 9), parts.c:1327-1357.
COMPONENTS = (
    (0x0001, 8, 4, 9),     # Enigma Pulsar engine
    (0x1000, 4, 1, 0),     # Multi-Cargo Pod
    (0x0004, 6, 8, 2),     # Langston Shell
    (0x0010, 18, 6, 7),    # Multi-Contained Munition
    (0x0020, 7, 6, 6),     # Anti-Matter Torpedo
    (0x0020, 7, 2, 6),     # Anti-Matter Torpedo
    (0x0010, 18, 2, 7),    # Multi-Contained Munition
    (0x0010, 18, 4, 7),    # Multi-Contained Munition
    (0x0008, 9, 6, 3),     # Mega Poly Shell
    (0x0800, 4, 3, 1),     # Multi-Function Pod
    (0x0800, 4, 3, 1),     # Multi-Function Pod
)


def _owner_by_shdef_record(records):
    players = [r for r in records if r["kind"] == 6]
    if [(r["body"][0], r["body"][1]) for r in players] != [(0, 6), (1, 3)]:
        raise ValueError("verified HST player ship-definition counts changed")
    definitions = [
        i for i, r in enumerate(records)
        if r["kind"] == 26 and
        ((struct.unpack_from("<H", r["body"])[0] >> 10) & 0x1F) < 16
    ]
    if len(definitions) != 9:
        raise ValueError("verified normal ship-definition count changed")
    owners = {}
    cursor = 0
    for player in players:
        for index in definitions[cursor:cursor + player["body"][1]]:
            owners[index] = player["body"][0]
        cursor += player["body"][1]
    if cursor != len(definitions):
        raise ValueError("ship definitions do not match saved player counts")
    return owners


def _high_offer_design(records):
    owner_map = _owner_by_shdef_record(records)
    record = records[TARGET_RECORD]
    body = record["body"]
    flags = struct.unpack_from("<H", body)[0]
    slot = (flags >> 10) & 0x1F
    if (record["kind"], owner_map.get(TARGET_RECORD), slot, body[2], body[6], len(body)) != (
            26, TARGET_OWNER, TARGET_SLOT, 4, 3, 46):
        raise ValueError("verified target fleet design-0 Scout record changed")
    old_components = []
    for component in range(body[6]):
        group, packed = struct.unpack_from("<HH", body, 17 + component * 4)
        old_components.append((group, packed & 0xFF, packed >> 8))
    if old_components != [(0x0001, 1, 1), (0x0002, 4, 1), (0x1000, 5, 1)]:
        raise ValueError("verified player-1 design-0 components changed")

    # Battleship hull 9: bitmap 36, dp 2000, and eleven original component
    # slots. Keep the serialized flags, ship counts, and user-name tail.
    fixed = body[:2] + struct.pack("<BBHB", 9, 36, 2000, len(COMPONENTS)) + body[7:17]
    slots = b"".join(struct.pack("<HH", group, (count << 8) | item)
                     for group, item, count, _ in COMPONENTS)
    updated_body = fixed + slots + body[17 + body[6] * 4:]
    records[TARGET_RECORD] = {**record, "body": updated_body}

    fleet = decode_owner_fleet(records[FLEET_RECORD]["body"])
    if (records[FLEET_RECORD]["kind"], fleet["owner"], fleet["id"],
            fleet["ship_bitmap"], fleet["ship_counts"][0]) != (16, 1, 0, 1, 1):
        raise ValueError("verified target fleet no longer selects design slot 0")
    return sum(count for _, _, count, _ in COMPONENTS)


def _candidate(task5):
    files = _source_files()
    source_path = ROOT / INPUTS["CLEAN.HST"][0]
    encoded, fleet_index, order_index = _candidate_hst(
        source_path, files["CLEAN.HST"], task5)
    if (fleet_index, order_index) != (FLEET_RECORD, ORDER_RECORD):
        raise ValueError("verified target fleet/order record indices changed")
    inspector = Inspector()
    with tempfile.TemporaryDirectory() as directory:
        path = Path(directory) / "CLEAN.HST"
        path.write_bytes(encoded)
        records = decode_stream(path, inspector)
    offer_count = _high_offer_design(records)
    encoded = encode_stream(records, inspector)
    with tempfile.TemporaryDirectory() as directory:
        path = Path(directory) / "CLEAN.HST"
        path.write_bytes(encoded)
        decoded = decode_stream(path, inspector)
    if len(decoded) != len(records) or any(
            (left["kind"], left["body"]) != (right["kind"], right["body"])
            for left, right in zip(records, decoded)):
        raise ValueError("high-offer candidate HST did not round-trip")
    _owner_by_shdef_record(decoded)
    return files, encoded, order_index, offer_count, decoded


def prepare_pair(control_directory, action_directory):
    control_directory, action_directory = map(Path,
                                               (control_directory, action_directory))
    if control_directory.exists() or action_directory.exists():
        raise FileExistsError("choose new candidate output directories")
    control = _candidate(False)
    action = _candidate(True)
    if control[:1] != action[:1] or control[2:4] != action[2:4]:
        raise ValueError("paired candidates do not share verified source and design")
    differences = [i for i, (left, right) in enumerate(zip(control[4], action[4]))
                   if (left["kind"], left["body"]) != (right["kind"], right["body"])]
    if differences != [control[2]]:
        raise ValueError("paired inputs differ outside the task flag")

    result = []
    for destination, hst, task5 in (
            (control_directory, control[1], False),
            (action_directory, action[1], True)):
        destination.mkdir(parents=True)
        candidate_files = dict(control[0])
        candidate_files["CLEAN.HST"] = hst
        for name, data in candidate_files.items():
            (destination / name).write_bytes(data)
        manifest = {
            "status": "candidate_input_only_not_original_behavior_evidence",
            "mutation": (
                "move player-1 fleet 0 to owner-0 planet 10; change its "
                "slot-0 ship definition from Scout to a hull-valid Battleship "
                "with rare trader parts; " + ("use task 5" if task5 else "use task 0")
            ),
            "target": {"fleet_record": FLEET_RECORD, "fleet_id": 0,
                       "owner": TARGET_OWNER, "ship_design_record": TARGET_RECORD,
                       "ship_design_slot": TARGET_SLOT, "hull_id": 9,
                       "hull_name": "Battleship", "component_count": 11},
            "trader_components": [
                {"slot": index, "group": group, "item": item,
                 "count": count, "trader_tech": tech}
                for index, (group, item, count, tech) in enumerate(COMPONENTS)
            ],
            "trader_offer_count_before_cap": control[3],
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
