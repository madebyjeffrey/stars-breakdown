"""Decode the original 0x5B message from paired human-owner SMOKE turns."""

import hashlib
import json
from pathlib import Path
import struct

from tools.race_file_compare import Inspector
from tools.satisfy_orders_smoke_deepspace_oracle import _fleet, _salvage
from tools.stars_game_stream import decode_stream


ROOT = Path(__file__).resolve().parents[1]
CONTROL_INPUT = ROOT / "runs/syorig-smoke-human-control-bundle-1"
ACTION_INPUT = ROOT / "runs/syorig-smoke-human-action-bundle-1"
CONTROL_OUTPUT = ROOT / "runs/vmctl-syhum0"
ACTION_OUTPUT = ROOT / "runs/vmctl-syhum1"


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def first_message(records):
    chunks = [record["body"] for record in records if record["kind"] == 12]
    if len(chunks) != 1 or len(chunks[0]) < 7:
        raise ValueError("player message block changed")
    body = chunks[0]
    word, goto = struct.unpack_from("<Hh", body)
    message_id = word & 0x1ff
    word_flags = word >> 9
    if word_flags & 1:
        first = struct.unpack_from("<H", body, 4)[0]
        second = body[6]
        consumed = 7
    else:
        first = body[4]
        second = body[5]
        consumed = 6
    return dict(message_id=message_id, goto=goto,
                word_flags=word_flags, first=first, second=second,
                first_entry_hex=body[:consumed].hex(),
                block_hex=body.hex())


def recover():
    inspector = Inspector()
    inputs = {}
    for label, directory in (("control", CONTROL_INPUT),
                             ("action", ACTION_INPUT)):
        manifest = json.loads((directory / "MANIFEST.JSON").read_text())
        for name, meta in manifest["files"].items():
            path = directory / name
            if path.stat().st_size != meta["bytes"] or sha(path) != meta["sha256"]:
                raise ValueError(label + " input changed: " + name)
        inputs[label] = {name: decode_stream(directory / name, inspector)
                         for name in ("SMOKE.HST", "SMOKE.M2")}
        for name, player_index in (("SMOKE.HST", 2), ("SMOKE.M2", 1)):
            player = inputs[label][name][player_index]
            if (player["kind"] != 6 or player["body"][0] != 1 or
                    struct.unpack_from("<H", player["body"], 6)[0] != 0x253f):
                raise ValueError(label + " owner fAi flag changed")
    for name, order_index in (("SMOKE.HST", 159), ("SMOKE.M2", 8)):
        control = inputs["control"][name]
        action = inputs["action"][name]
        if len(control) != len(action):
            raise ValueError("paired input length differs")
        for index, (left, right) in enumerate(zip(control, action)):
            if index != order_index and (left["kind"], left["body"]) != (
                    right["kind"], right["body"]):
                raise ValueError("paired inputs differ outside task 5")
        if (control[order_index]["body"].hex(),
                action[order_index]["body"].hex()) != (
                "fc04640500000014", "fc04640500000514"):
            raise ValueError("paired task-5 order changed")
    output = {}
    for label, directory in (("control", CONTROL_OUTPUT),
                             ("action", ACTION_OUTPUT)):
        output[label] = {name: decode_stream(directory / name, inspector)
                         for name in ("SMOKE.HST", "SMOKE.M2")}
    control_message = first_message(output["control"]["SMOKE.M2"])
    action_message = first_message(output["action"]["SMOKE.M2"])
    if control_message["message_id"] != 0x4e:
        raise ValueError("control fleet message changed")
    if action_message != dict(message_id=0x5b, goto=-6, word_flags=1,
            first=0x2200, second=0,
            first_entry_hex="5b02faff002200",
            block_hex="5b02faff0022002f00028229205901feff0006"):
        raise ValueError("original task-5 message frame changed")
    control_fleets = _fleet(output["control"]["SMOKE.HST"])
    action_fleets = _fleet(output["action"]["SMOKE.HST"])
    if len(control_fleets) != 1 or action_fleets:
        raise ValueError("humanized control/action fleet retirement changed")
    control_salvage = _salvage(output["control"]["SMOKE.HST"])
    action_salvage = _salvage(output["action"]["SMOKE.HST"])
    if (control_salvage or len(action_salvage) != 1 or
            action_salvage[0]["raw_hex"] != "0022fc046405ff8305000000020002000000"):
        raise ValueError("humanized control/action salvage changed")
    return dict(input_hashes={label: {name: sha(directory / name)
                         for name in ("SMOKE.HST", "SMOKE.M2")}
                         for label, directory in (("control", CONTROL_INPUT),
                                                  ("action", ACTION_INPUT))},
                output_hashes={label: {name: sha(directory / name)
                         for name in ("SMOKE.HST", "SMOKE.M2")}
                         for label, directory in (("control", CONTROL_OUTPUT),
                                                  ("action", ACTION_OUTPUT))},
                owner_det_word=0x253f,
                control_message=control_message,
                action_message=action_message,
                control_fleet_present=True,
                action_fleet_present=False,
                action_salvage=action_salvage[0])


if __name__ == "__main__":
    print(json.dumps(recover(), indent=2))
