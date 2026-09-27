"""Prepare paired task-5 runs with trader tech on the target fleet's HST design."""

import hashlib
import json
from pathlib import Path
import struct
import tempfile

from tools.race_file_compare import Inspector
from tools.stars_game_stream import decode_stream, encode_stream
from tools.prepare_satisfy_orders_technology_original_candidate import (
    INPUTS, _candidate_hst, _source_files,
)


ROOT = Path(__file__).resolve().parents[1]
SHIP_DESIGN_RECORD = 39
DESIGN_ID = 1
OWNER = 1
TRADER_PARTS = (
    (0, 0x0001, 3, 8, 1, "Enigma Pulsar", 9),
    (1, 0x0010, 1, 18, 1, "Multi-Contained Munition", 7),
    (2, 0x0020, 0, 7, 1, "Anti-Matter Torpedo", 6),
    (4, 0x0008, 1, 9, 2, "Mega Poly Shell", 3),
    (5, 0x1000, 5, 4, 1, "Multi-Cargo Pod", 0),
    (6, 0x0800, 5, 4, 1, "Multi-Function Pod", 1),
)


def trader_hst(source, task5):
    source_path = ROOT / INPUTS["CLEAN.HST"][0]
    candidate, *indices = _candidate_hst(source_path, source, task5)
    inspector = Inspector()
    with tempfile.TemporaryDirectory() as directory:
        candidate_path = Path(directory) / "CLEAN.HST"
        candidate_path.write_bytes(candidate)
        records = decode_stream(candidate_path, inspector)
    record = records[SHIP_DESIGN_RECORD]
    body = bytearray(record["body"])
    flags = struct.unpack_from("<H", body)[0]
    component_count = body[6]
    if ((record["kind"], (flags >> 8) & 0xF, flags >> 12, body[2],
         component_count, len(body)) != (26, DESIGN_ID, OWNER, 6, 7, 57)):
        raise ValueError("player-1 target ship design source changed")
    for component_index, group_expected, source_item, target_item, quantity, _, _ in TRADER_PARTS:
        offset = 17 + component_index * 4
        group, packed = struct.unpack_from("<HH", body, offset)
        if (group, packed & 0xFF, packed >> 8) != (
                group_expected, source_item,
                2 if component_index == 4 else 1):
            raise ValueError("target ship's trader-capable component source changed")
        struct.pack_into("<HH", body, offset, group,
                         (quantity << 8) | target_item)
    records[SHIP_DESIGN_RECORD] = {**record, "body": bytes(body)}
    encoded = encode_stream(records, inspector)
    with tempfile.TemporaryDirectory() as directory:
        check_path = Path(directory) / "CLEAN.HST"
        check_path.write_bytes(encoded)
        decoded = decode_stream(check_path, inspector)
    if len(decoded) != len(records) or any(
            (a["kind"], a["body"]) != (b["kind"], b["body"])
            for a, b in zip(records, decoded)):
        raise ValueError("modified HST does not round-trip")
    return encoded, indices


def prepare_pair(control_directory, action_directory):
    control_directory, action_directory = map(Path,
                                               (control_directory, action_directory))
    if control_directory.exists() or action_directory.exists():
        raise FileExistsError("choose new candidate output directories")
    files = _source_files()
    hst_source = ROOT / INPUTS["CLEAN.HST"][0]
    control_hst, control_indices = trader_hst(files["CLEAN.HST"], False)
    action_hst, action_indices = trader_hst(files["CLEAN.HST"], True)
    if control_indices != action_indices:
        raise ValueError("fleet/order indices moved between candidates")
    inspector = Inspector()
    with tempfile.TemporaryDirectory() as directory:
        left_path, right_path = Path(directory) / "control.HST", Path(directory) / "action.HST"
        left_path.write_bytes(control_hst)
        right_path.write_bytes(action_hst)
        left = decode_stream(left_path, inspector)
        right = decode_stream(right_path, inspector)
    differences = [i for i, (a, b) in enumerate(zip(left, right))
                   if (a["kind"], a["body"]) != (b["kind"], b["body"])]
    if differences != [control_indices[1]]:
        raise ValueError("paired HST inputs differ outside the task flag")

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
            "mutation": (
                "replace six trader-capable components in player-1 fleet 0 "
                "design 1 with valid rare trader parts; move fleet 0 to "
                "owner-0 planet 10 at (1128,1300); " +
                ("set the fleet order to task 5" if action else
                 "set the fleet order to task 0")),
            "purpose": "paired original-executable input increases trader offer chances using the target ship's valid original hull slots; task flag remains the only control/action difference",
            "design": {"file": "CLEAN.HST", "record_index": SHIP_DESIGN_RECORD,
                       "owner": OWNER, "id": DESIGN_ID,
                       "components": [dict(index=i, group=g, source_item=old,
                           item=new, quantity=q, name=name, trader_tech=t)
                           for i, g, old, new, q, name, t in TRADER_PARTS]},
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
