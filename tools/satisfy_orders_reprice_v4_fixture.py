"""Materialize the verified v4 HST SHDEF and original NE part tables."""

import hashlib
import json
from pathlib import Path
import struct

from tools.ne_layout import NE
from tools.race_file_compare import Inspector
from tools.satisfy_orders_original_oracle import decode_owner_fleet
from tools.stars_game_stream import decode_stream


ROOT = Path(__file__).resolve().parents[1]
HST = ROOT / "runs/syorig-tech-trader-v4-bitc-seed1-action/CLEAN.HST"
EXE = ROOT / "target/stars.exe"
HEADER = ROOT / "tests/fixtures/satisfy_orders_reprice_v4_bitc_parts.h"
MANIFEST = ROOT / "tests/fixtures/satisfy_orders_reprice_v4_bitc_parts.json"
EXE_SHA256 = "ccab6682b55dae14dbf104e491cfbd3b32a6280022020032ae0615500949ba59"
HST_SHA256 = "10fba172f2fed7fd676f319b48a50bb4c3ba0042cf67a7ce300f04bf8df1288d"
TABLES = {
    1: (0x0000, 0x4e), 2: (0x0768, 0x38), 4: (0x0ae8, 0x36),
    8: (0x12ec, 0x36), 16: (0x2450, 0x3c), 32: (0x2180, 0x3c),
    0x800: (0x0d04, 0x36), 0x1000: (0x109a, 0x36),
    0x4000: (0x29f0, 0x8f),
}
EXPECTED_COMPONENTS = [
    (1, 8, 4), (0x1000, 4, 1), (4, 6, 8), (16, 18, 6),
    (32, 7, 6), (32, 7, 2), (16, 18, 2), (16, 18, 4),
    (8, 9, 6), (0x800, 4, 3), (0x800, 4, 3),
]


def sha256(data):
    return hashlib.sha256(data).hexdigest()


def render_array(name, data):
    lines = ["static const unsigned char %s[%d] = {" % (name, len(data))]
    for offset in range(0, len(data), 12):
        lines.append("    " + ", ".join("0x%02x" % b
                                        for b in data[offset:offset + 12]) + ",")
    lines.append("};")
    return lines


def prepare():
    if HEADER.exists() or MANIFEST.exists():
        raise FileExistsError("fixture already exists; preserving prior output")
    exe_bytes = EXE.read_bytes()
    hst_bytes = HST.read_bytes()
    if sha256(exe_bytes) != EXE_SHA256 or sha256(hst_bytes) != HST_SHA256:
        raise ValueError("v4 original input hashes changed")
    rows = decode_stream(HST, Inspector())
    player0, player1 = rows[1], rows[2]
    fleet, order, design = rows[56], rows[57], rows[41]
    if (player0["kind"], player1["kind"], player1["body"][0]) != (6, 6, 1):
        raise ValueError("verified player records moved")
    race_bits = struct.unpack_from("<I", player1["body"], 78)[0]
    if race_bits != 0x80003045:
        raise ValueError("verified player-1 race-bit-C word changed")
    fleet_fields = decode_owner_fleet(fleet["body"])
    if (fleet["kind"], fleet_fields["id"], fleet_fields["owner"],
            fleet_fields["planet"], fleet_fields["ship_bitmap"],
            fleet_fields["ship_counts"][0]) != (16, 0, 1, 10, 1, 1):
        raise ValueError("verified target fleet record changed")
    task = struct.unpack_from("<H", order["body"], 6)[0] & 0x0f
    if (order["kind"], task, design["kind"], design["body"][2],
            design["body"][6]) != (19, 5, 26, 9, 11):
        raise ValueError("verified task or high-offer SHDEF changed")
    components = []
    for slot in range(design["body"][6]):
        group, packed = struct.unpack_from("<HH", design["body"], 17 + slot * 4)
        components.append((group, packed & 0xff, packed >> 8))
    if components != EXPECTED_COMPONENTS:
        raise ValueError("verified high-offer components changed")
    raw_design = bytearray(147)
    struct.pack_into("<H", raw_design, 0, design["body"][2])
    raw_design[0x3a:0x3a + len(components) * 4] = design["body"][17:17 +
                                                                         len(components) * 4]
    raw_design[0x7a] = len(components)
    segment = bytes(NE(EXE).raw[1])
    if len(segment) > 0xffff:
        raise ValueError("NE part segment cannot be addressed by current adapter")
    part_rows = []
    for group, item, count in [(0x4000, 9, 1), *components]:
        if group not in TABLES:
            raise ValueError("unmapped original SHDEF group")
        base, stride = TABLES[group]
        offset = base + item * stride
        raw = segment[offset:offset + stride]
        if len(raw) != stride:
            raise ValueError("part record truncated")
        part_rows.append({"group": group, "item": item, "quantity": count,
                          "segment": 2, "offset": offset, "stride": stride,
                          "technology": list(raw[2:8]),
                          "base_costs": list(struct.unpack_from("<4H", raw, 0x2a)),
                          "record_sha256": sha256(raw)})
    guard = ["/* Generated from the hash-pinned v4 HST and STARS.EXE NE segment 2. */",
             "#ifndef SATISFY_ORDERS_REPRICE_V4_BITC_PARTS_H",
             "#define SATISFY_ORDERS_REPRICE_V4_BITC_PARTS_H"]
    guard.extend(render_array("v4_part_segment", segment))
    guard.extend(render_array("v4_high_offer_design", bytes(raw_design)))
    guard.append("#endif")
    HEADER.write_text("\n".join(guard) + "\n", encoding="ascii")
    result = {
        "status": "verified_v4_high_offer_fixture",
        "source": {"hst": str(HST.relative_to(ROOT)),
                   "hst_sha256": HST_SHA256,
                   "executable": str(EXE.relative_to(ROOT)),
                   "executable_sha256": EXE_SHA256,
                   "part_segment_bytes": len(segment),
                   "part_segment_sha256": sha256(segment)},
        "input": {"target_design_record": 41, "fleet_record": 56,
                  "order_record": 57, "fleet_owner": 1, "fleet_id": 0,
                  "planet_id": 10, "ship_count": 1, "hull_id": 9,
                  "owner_technology": list(player1["body"][26:32]),
                  "owner_race_stat": player1["body"][76],
                  "owner_race_bits": "0x%08X" % race_bits,
                  "planet_owner": 0, "planet_minerals": [469, 893, 376],
                  "planet_colonists": 250,
                  "components": [{"group": g, "item": i, "count": n}
                                 for g, i, n in components]},
        "part_records": part_rows,
        "fixture": {"header": str(HEADER.relative_to(ROOT)),
                    "header_sha256": sha256(HEADER.read_bytes())},
    }
    MANIFEST.write_text(json.dumps(result, indent=2, sort_keys=True) + "\n",
                        encoding="ascii")
    return result


if __name__ == "__main__":
    print(json.dumps(prepare()["fixture"], sort_keys=True))
