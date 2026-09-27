"""Recover task-5 fleet 0's base design costs from verified original inputs.

The HST stores a compact SHDEF component list. Its hull and component cost
records are in the original executable's NE segment 2. This module reads
those records directly and reports their unadjusted costs; player-specific
GetTruePartCost discounts remain a separate service input.
"""

import hashlib
import json
from pathlib import Path
import struct

from tools.ne_layout import NE
from tools.race_file_compare import Inspector
from tools.stars_game_stream import decode_stream


ROOT = Path(__file__).resolve().parents[1]
EXE = ROOT / "target/stars.exe"
HST = ROOT / "runs/vmctl-syact3/BACKUP/CLEAN.HST"
EXE_SHA256 = "ccab6682b55dae14dbf104e491cfbd3b32a6280022020032ae0615500949ba59"
HST_SHA256 = "aa6161bb6c95c8276b7a5d12e5a12d9999e3dea0fa242a3be89a94a05c45df3c"
PART_LAYOUT = {1: (0x0000, 0x4E), 2: (0x0768, 0x38), 0x10: (0x2450, 0x3C)}


def costs(segment, offset):
    if offset < 0 or offset + 0x32 > len(segment):
        raise ValueError("cost record outside original segment")
    resource, ironium, boranium, germanium = struct.unpack_from(
        "<4H", segment, offset + 0x2A)
    return [resource, ironium, boranium, germanium]


def owner_zero_cost(segment, offset, technology):
    """Apply the reviewed GetTruePartCost rule for this owner-0 fixture.

    The owner has race stat 9 and zero trait bits; none of the selected
    hull/part families has an additional race-specific cost modifier.
    """
    base = costs(segment, offset)
    required = list(segment[offset + 2:offset + 8])
    if any(required):
        progress = min(have - need for have, need in
                       zip(technology, required) if need > 0)
    else:
        progress = min(technology)
    percent = min(progress, 19) * 4 if progress > 0 else 0
    adjusted = [max(1, value - value * percent // 100) if value else 0
                for value in base]
    return required, percent, adjusted


def recover():
    if hashlib.sha256(EXE.read_bytes()).hexdigest() != EXE_SHA256:
        raise ValueError("original executable hash changed")
    if hashlib.sha256(HST.read_bytes()).hexdigest() != HST_SHA256:
        raise ValueError("original HST input hash changed")
    records = decode_stream(HST, Inspector())
    design = records[35]
    if (design["kind"], design["turn"]) != (26, 0):
        raise ValueError("design-0 record moved or changed kind")
    body = design["body"]
    flags = struct.unpack_from("<H", body)[0]
    hull_id = body[2]
    count = body[6]
    if flags != 0x0107 or hull_id != 4 or count != 3:
        raise ValueError("unexpected fleet-0 design header")
    if len(body) < 17 + count * 4:
        raise ValueError("truncated component list")
    segment = bytes(NE(EXE).raw[1])  # NE segment 2, zero-based list index 1
    hull_offset = 0x29F0 + hull_id * 143
    player = records[1]
    if player["kind"] != 6 or player["body"][0] != 0:
        raise ValueError("owner-0 player record changed")
    technology = list(player["body"][26:32])
    race_bits = struct.unpack_from("<I", player["body"], 78)[0]
    race_stat = player["body"][76]
    if race_bits != 0 or race_stat != 9:
        raise ValueError("owner-0 race cost modifiers need separate review")
    hull_cost = costs(segment, hull_offset)
    hull_required, hull_discount, hull_adjusted = owner_zero_cost(
        segment, hull_offset, technology)
    components = []
    total = hull_cost[:]
    adjusted_total = hull_adjusted[:]
    for index in range(count):
        group, packed = struct.unpack_from("<HH", body, 17 + index * 4)
        quantity, item = packed >> 8, packed & 0xFF
        if group not in PART_LAYOUT or not quantity:
            raise ValueError("unexpected original component")
        base, stride = PART_LAYOUT[group]
        offset = base + item * stride
        component_cost = costs(segment, offset)
        required, discount, adjusted = owner_zero_cost(segment, offset,
                                                         technology)
        components.append(dict(group=group, item=item, quantity=quantity,
                               segment=2, offset=offset,
                               base_cost=component_cost,
                               required_tech=required,
                               discount_percent=discount,
                               owner_cost=adjusted))
        total = [value + quantity * cost for value, cost
                 in zip(total, component_cost)]
        adjusted_total = [value + quantity * cost for value, cost
                          in zip(adjusted_total, adjusted)]
    return dict(executable_sha256=EXE_SHA256, hst_sha256=HST_SHA256,
                design_record_index=35, design_record_hex=body.hex(),
                owner_tech=technology, owner_race_bits=race_bits,
                owner_race_stat=race_stat,
                hull=dict(id=hull_id, segment=2, offset=hull_offset,
                          base_cost=hull_cost,
                          required_tech=hull_required,
                          discount_percent=hull_discount,
                          owner_cost=hull_adjusted),
                components=components, base_design_cost=total,
                owner_design_cost=adjusted_total,
                ordinary_starbase_mineral_return=[4 * value // 5
                    for value in adjusted_total[1:]])


if __name__ == "__main__":
    print(json.dumps(recover(), indent=2))
