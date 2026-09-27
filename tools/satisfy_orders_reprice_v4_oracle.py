"""Validate and index the retained original race-bit-C task-5 pair."""

import hashlib
import json
from pathlib import Path
import struct

from tools.race_file_compare import Inspector
from tools.satisfy_orders_original_oracle import decode_owner_fleet
from tools.satisfy_orders_planet10_oracle import decode_planet10
from tools.stars_game_stream import decode_stream


ROOT = Path(__file__).resolve().parents[1]
PAIR = "reprice-v4-bitc-seed1"
INPUTS = {
    "control": ROOT / "runs/syorig-tech-trader-v4-bitc-seed1-control",
    "action": ROOT / "runs/syorig-tech-trader-v4-bitc-seed1-action",
}
RETURNS = {
    "control": ROOT / "runs/vmctl-sytech-reprice-v4-bitc-control",
    "action": ROOT / "runs/vmctl-sytech-reprice-v4-bitc-action",
}
ORACLE = ROOT / "analysis/compat/satisfy-orders-reprice-v4-bitc-original-oracle.json"
TARGET_COMPONENTS = [
    {"group": 1, "item": 8, "count": 4},
    {"group": 4096, "item": 4, "count": 1},
    {"group": 4, "item": 6, "count": 8},
    {"group": 16, "item": 18, "count": 6},
    {"group": 32, "item": 7, "count": 6},
    {"group": 32, "item": 7, "count": 2},
    {"group": 16, "item": 18, "count": 2},
    {"group": 16, "item": 18, "count": 4},
    {"group": 8, "item": 9, "count": 6},
    {"group": 2048, "item": 4, "count": 3},
    {"group": 2048, "item": 4, "count": 3},
]


def sha256(data):
    return hashlib.sha256(data).hexdigest()


def file_info(path):
    data = path.read_bytes()
    return {"bytes": len(data), "sha256": sha256(data)}


def records(path):
    return decode_stream(path, Inspector())


def find_one(rows, predicate, label):
    found = [row for row in rows if predicate(row)]
    if len(found) != 1:
        raise ValueError("expected one " + label + ", got " + str(len(found)))
    return found[0]


def candidate_observation(side):
    directory = INPUTS[side]
    manifest = json.loads((directory / "MANIFEST.JSON").read_text())
    rows = records(directory / "CLEAN.HST")
    player = find_one(rows, lambda r: r["kind"] == 6 and
                      r["turn"] == 0 and r["body"][0] == 1,
                      side + " player-1 input")
    player0 = find_one(rows, lambda r: r["kind"] == 6 and
                       r["turn"] == 0 and r["body"][0] == 0,
                       side + " player-0 input")
    target = rows[41]
    fleet = rows[56]
    order = rows[57]
    if (target["kind"], target["turn"], target["body"][2],
            target["body"][6]) != (26, 0, 9, 11):
        raise ValueError("verified high-offer design record 41 changed")
    components = []
    for index in range(target["body"][6]):
        group, packed = struct.unpack_from("<HH", target["body"],
                                           17 + index * 4)
        components.append({"group": group, "item": packed & 0xff,
                           "count": packed >> 8})
    if components != TARGET_COMPONENTS:
        raise ValueError("high-offer design parts changed")
    decoded_fleet = decode_owner_fleet(fleet["body"])
    race_bits = struct.unpack_from("<I", player["body"], 78)[0]
    technology = list(player["body"][26:32])
    player0_technology = list(player0["body"][26:32])
    if (fleet["kind"], fleet["turn"], decoded_fleet["id"],
            decoded_fleet["owner"], decoded_fleet["ship_bitmap"],
            decoded_fleet["ship_counts"][0]) != (16, 0, 0, 1, 1, 1):
        raise ValueError("target fleet 0 no longer selects SHDEF slot 0")
    if race_bits != 0x80003045 or not (race_bits & (1 << 12)):
        raise ValueError("player-1 race bit C is not set")
    if technology != [0, 0, 1, 0, 5, 0] or player0_technology != [3] * 6:
        raise ValueError("candidate player technology inputs changed")
    if order["kind"] != 19 or order["turn"] != 0 or len(order["body"]) != 18:
        raise ValueError("task-flag order record 57 changed kind")
    task = struct.unpack_from("<H", order["body"], 6)[0] & 0x0f
    if rows.index(player) != 2:
        raise ValueError("player-1 input record moved")
    return {
        "input_hashes": {name: file_info(directory / name)
                         for name in ("CLEAN.H2", "CLEAN.HST", "CLEAN.M1",
                                      "CLEAN.M2", "CLEAN.XY", "STARS.EXE")},
        "input_directory": str(directory.relative_to(ROOT)),
        "player1": {"record_index": 2, "technology": technology,
                    "race_stat": player["body"][76],
                    "race_bits": "0x%08X" % race_bits,
                    "race_bit_c": True},
        "player0_technology": player0_technology,
        "target": {"design_record": 41, "fleet_record": 56,
                   "order_record": 57, "owner": 1, "fleet_id": 0,
                   "ship_design_slot": 0, "hull_id": 9,
                   "component_count": len(components),
                   "components": components,
                   "task_flag": task},
        "pair_manifest": manifest,
    }


def returned_observation(side):
    directory = RETURNS[side]
    candidate = INPUTS[side]
    for name in ("CLEAN.HST", "CLEAN.M1", "CLEAN.M2"):
        if ((directory / "BACKUP" / name).read_bytes() !=
                (candidate / name).read_bytes()):
            raise ValueError(side + " backup mismatch for " + name)
    hst = records(directory / "CLEAN.HST")
    m1 = records(directory / "CLEAN.M1")
    player0 = find_one(hst, lambda r: r["kind"] == 6 and r["turn"] == 1
                       and r["body"][0] == 0, side + " returned player 0")
    fleets = [decode_owner_fleet(r["body"]) for r in hst
              if r["kind"] == 16 and r["turn"] == 1]
    target_present = any(f["owner"] == 1 and f["id"] == 0
                         for f in fleets)
    message = find_one(m1, lambda r: r["kind"] == 12 and r["turn"] == 1,
                       side + " turn-1 message block")
    raw_messages = message["body"]
    point_tech = b"\x3d\x07" in raw_messages
    planet_rows = [r["body"] for r in hst if r["kind"] == 13 and
                   r["turn"] == 1 and r["body"][:2] == b"\x0a\x00"]
    planet = decode_planet10(find_one(
        [{"body": b} for b in planet_rows], lambda r: True,
        side + " returned planet 10")["body"])
    tech = list(player0["body"][26:32])
    returned = {name: file_info(directory / name) for name in
                ("CLEAN.HST", "CLEAN.M1", "CLEAN.M2", "CLEAN.H2",
                 "CLEAN.XY")}
    if (directory / "guest-screen.bmp").exists():
        returned["guest-screen.bmp"] = file_info(directory / "guest-screen.bmp")
    return {
        "return_directory": str(directory.relative_to(ROOT)),
        "guest_directory": "C:\\SVC92C" if side == "control" else "C:\\SVC92A",
        "guest_command": ("command.com /c C:\\SVC92C\\RUN.BAT" if
                          side == "control" else
                          "command.com /c C:\\SVC92A\\RUN.BAT"),
        "process_exit": 0,
        "error_dialog": False,
        "clean_x2_output_present": False,
        "returned_files": returned,
        "player0_tech": tech,
        "target_fleet_owner1_id0_present": target_present,
        "planet10": {"minerals": planet["minerals"],
                      "levels": planet["levels"],
                      "improvements": planet["improvements"]},
        "turn1_message_block_hex": raw_messages.hex(),
        "ordinary_signed_technology_message_0x13d": point_tech,
    }


def build():
    control_input = candidate_observation("control")
    action_input = candidate_observation("action")
    if control_input["input_hashes"]["CLEAN.H2"] != action_input[
            "input_hashes"]["CLEAN.H2"]:
        raise ValueError("control/action H2 inputs are not identical")
    if control_input["input_hashes"]["CLEAN.M2"] != action_input[
            "input_hashes"]["CLEAN.M2"]:
        raise ValueError("control/action M2 inputs are not identical")
    control_hst = records(INPUTS["control"] / "CLEAN.HST")
    action_hst = records(INPUTS["action"] / "CLEAN.HST")
    differences = [index for index, (left, right) in enumerate(
        zip(control_hst, action_hst)) if
        (left["kind"], left["turn"], left["body"]) !=
        (right["kind"], right["turn"], right["body"])]
    if differences != [57]:
        raise ValueError("paired inputs differ outside order record 57")
    if (control_input["target"] | {"task_flag": None}) != \
            (action_input["target"] | {"task_flag": None}):
        raise ValueError("control/action high-offer inputs differ")
    for name in ("CLEAN.M1", "CLEAN.M2", "CLEAN.XY", "STARS.EXE"):
        if ((INPUTS["control"] / name).read_bytes() !=
                (INPUTS["action"] / name).read_bytes()):
            raise ValueError("paired source inputs differ in " + name)
    if (control_input["target"]["task_flag"],
            action_input["target"]["task_flag"]) != (0, 5):
        raise ValueError("candidate order task flags are not task-0/task-5")
    observations = {side: returned_observation(side)
                    for side in ("control", "action")}
    if (observations["control"]["target_fleet_owner1_id0_present"] is not True or
            observations["action"]["target_fleet_owner1_id0_present"] is not False):
        raise ValueError("original task-0/task-5 fleet outcome changed")
    if observations["control"]["ordinary_signed_technology_message_0x13d"]:
        raise ValueError("control unexpectedly has a 0x13D frame")
    if not observations["action"]["ordinary_signed_technology_message_0x13d"]:
        raise ValueError("action is missing the original 0x13D frame")
    if observations["control"]["player0_tech"] != [3] * 6 or \
            observations["action"]["player0_tech"] != [4, 3, 3, 3, 3, 3]:
        raise ValueError("original signed technology outcome changed")
    for side in ("control", "action"):
        run = {"pair": PAIR, "side": side,
               "status": "preserved_original_executable_run",
               **observations[side]}
        path = RETURNS[side] / "RUN.JSON"
        path.write_text(json.dumps(run, indent=2, sort_keys=True) + "\n",
                         encoding="ascii")
    oracle = {
        "status": "matched_original_behavior_evidence",
        "pair": PAIR,
        "input_pair": {"control": control_input, "action": action_input,
                       "paired_difference_record_indices": differences,
                       "shared_h2_sha256": control_input["input_hashes"][
                           "CLEAN.H2"]["sha256"]},
        "returns": observations,
        "interpretation": [
            "Both fresh inputs contain the same 11-component, hull-9 SHDEF, player-1 technology, planet/message inputs, and H2 seed. Their only HST difference is order record 57's task flag.",
            "The target fleet owner is 1 and its race-bit word is 0x80003045, with bit C set. The original task-0 control retains owner-1 fleet 0 and has no 0x13D frame; task 5 retires fleet 0, writes a 0x13D point-technology frame, and raises player-0 technology 0 from 3 to 4.",
            "The returned H2 files differ. They are output state after a turn with different random-call paths and are not evidence that the shared input H2 is the original random seed.",
            "This pair is original-executable evidence for the live race-bit-C condition and high-offer design. It does not replace the still-open current-source native field differential."
        ]
    }
    ORACLE.parent.mkdir(parents=True, exist_ok=True)
    ORACLE.write_text(json.dumps(oracle, indent=2, sort_keys=True) + "\n",
                      encoding="ascii")
    return oracle


if __name__ == "__main__":
    result = build()
    print(json.dumps({"oracle": str(ORACLE.relative_to(ROOT)),
                      "result": result["status"],
                      "shared_h2_sha256": result["input_pair"][
                          "shared_h2_sha256"],
                      "technology_message": result["returns"]["action"][
                          "ordinary_signed_technology_message_0x13d"]},
                     sort_keys=True))
