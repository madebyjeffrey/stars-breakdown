"""Pin the natural deep-space task-5 original turn against its control."""

import hashlib
import json
from pathlib import Path
import struct

from tools.ne_layout import NE
from tools.race_file_compare import Inspector
from tools.satisfy_orders_original_oracle import decode_owner_fleet
from tools.stars_game_stream import decode_stream


ROOT = Path(__file__).resolve().parents[1]
INPUT = ROOT / "runs/syorig-smoke-deepspace-bundle-1"
CONTROL = ROOT / "runs/vmctl-sysmk0"
ACTION = ROOT / "runs/vmctl-sysmk1"
EXE = ROOT / "target/stars.exe"


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def _fleet(records):
    found = []
    for record in records:
        if record["kind"] != 16:
            continue
        try:
            fleet = decode_owner_fleet(record["body"])
        except ValueError:
            continue
        if fleet["owner"] == 1 and fleet["id"] == 0:
            found.append(fleet)
    return found


def _salvage(records):
    found = []
    for record in records:
        body = record["body"]
        if record["kind"] != 43 or len(body) != 18:
            continue
        id_full, x, y, packed, a, b, c, value, turn = struct.unpack(
            "<HHHHhhhHH", body)
        if ((id_full >> 13) & 7) == 1 and x == 1276 and y == 1380:
            found.append(dict(id_full=id_full, id=id_full & 0x1ff,
                              owner=(id_full >> 9) & 15,
                              point=[x, y], packed=packed,
                              minerals=[a, b, c], value=value, turn=turn,
                              raw_hex=body.hex()))
    return found


def recover():
    input_manifest = json.loads((INPUT / "MANIFEST.JSON").read_text())
    for name, details in input_manifest["files"].items():
        path = INPUT / name
        if path.stat().st_size != details["bytes"] or sha(path) != details["sha256"]:
            raise ValueError("candidate input changed: " + name)
    inspector = Inspector()
    hst = decode_stream(INPUT / "SMOKE.HST", inspector)
    m2 = decode_stream(INPUT / "SMOKE.M2", inspector)
    if (hst[159]["kind"], hst[159]["body"].hex(),
            m2[8]["kind"], m2[8]["body"].hex()) != (
            20, "fc04640500000514", 20, "fc04640500000514"):
        raise ValueError("paired deep-space task-5 orders changed")
    fleets = _fleet(hst)
    if len(fleets) != 1 or fleets[0]["planet"] != -1 or fleets[0]["point"] != [1276, 1380]:
        raise ValueError("deep-space source fleet changed")
    owner = hst[2]["body"]
    if owner[0] != 1 or list(owner[26:32]) != [0, 0, 1, 0, 5, 0] or owner[76] != 1:
        raise ValueError("fleet owner technology/race changed")
    design = hst[140]["body"]
    if (design[2], design[6]) != (4, 3):
        raise ValueError("fleet design changed")
    segment = bytes(NE(EXE).raw[1])
    parts = [(0x4000, 4, 0x29f0, 0x8f, 1)]
    layout = {1: (0, 0x4e), 2: (0x768, 0x38), 0x1000: (0x109a, 0x36)}
    for index in range(3):
        group, packed = struct.unpack_from("<HH", design, 17 + 4 * index)
        quantity, item = packed >> 8, packed & 0xff
        if group not in layout or not quantity:
            raise ValueError("unexpected design component")
        base, stride = layout[group]
        parts.append((group, item, base, stride, quantity))
    detail = []
    cost = [0, 0, 0, 0]
    for group, item, base, stride, quantity in parts:
        offset = base + item * stride
        required = list(segment[offset + 2:offset + 8])
        base_cost = list(struct.unpack_from("<4H", segment, offset + 0x2a))
        if any(required):
            progress = min(have - need for have, need in
                           zip(owner[26:32], required) if need > 0)
        else:
            progress = min(owner[26:32])
        if progress != 0:
            raise ValueError("this fixture needs separate cost-discount review")
        cost = [a + quantity * b for a, b in zip(cost, base_cost)]
        detail.append(dict(group=group, item=item, quantity=quantity,
                           offset=offset, required_tech=required,
                           cost=base_cost))
    if cost != [35, 16, 2, 8]:
        raise ValueError("original design cost changed")
    control_hst = decode_stream(CONTROL / "SMOKE.HST", inspector)
    action_hst = decode_stream(ACTION / "SMOKE.HST", inspector)
    control_m2 = decode_stream(CONTROL / "SMOKE.M2", inspector)
    action_m2 = decode_stream(ACTION / "SMOKE.M2", inspector)
    if len(_fleet(control_hst)) != 1 or _fleet(action_hst):
        raise ValueError("fleet retirement did not differ as expected")
    if len(_fleet(control_m2)) != 1 or _fleet(action_m2):
        raise ValueError("player-file fleet retirement did not differ")
    if _salvage(control_hst) or len(_salvage(action_hst)) != 1:
        raise ValueError("salvage difference changed")
    salvage = _salvage(action_hst)[0]
    if salvage["minerals"] != [cost[i] // 3 for i in (1, 2, 3)]:
        raise ValueError("salvage minerals disagree with one-third cost")
    if len(_salvage(action_m2)) != 1:
        raise ValueError("salvage not retained in player file")
    saved_messages = [r["body"] for r in action_m2 if r["kind"] == 12]
    if saved_messages:
        raise ValueError("AI player message policy changed")
    return dict(input_hst_sha256=sha(INPUT / "SMOKE.HST"),
                input_m2_sha256=sha(INPUT / "SMOKE.M2"),
                control_hst_sha256=sha(CONTROL / "SMOKE.HST"),
                action_hst_sha256=sha(ACTION / "SMOKE.HST"),
                control_m2_sha256=sha(CONTROL / "SMOKE.M2"),
                action_m2_sha256=sha(ACTION / "SMOKE.M2"),
                original_exe_sha256=sha(EXE),
                input_fleet=fleets[0],
                design_parts=detail, design_cost=cost,
                control_fleet_present=True, action_fleet_present=False,
                control_salvage_present=False, action_salvage=salvage,
                action_m2_saved_message_0x5b=False,
                message_observation="AI player M2 contains no kind-12 message; original decompiled branch calls FSendPlrMsg2 0x5B but the message is not a saved-output observation")


if __name__ == "__main__":
    print(json.dumps(recover(), indent=2))
