"""Materialize the retained selector-3 turn as a replay comparison target.

This describes serialized game state. It does not infer live PLORD, AI-table,
allocator, callback, or RNG state from the compact files.
"""

import hashlib
import json
from pathlib import Path

from tests.test_satisfy_orders_turin_binding import (
    owner1_fleet_order_pairs, owner_planet_record, xy_points,
)
from tools.race_file_compare import Inspector
from tools.stars_game_stream import decode_stream

ROOT = Path(__file__).resolve().parents[1]
RUN = ROOT / "runs/rototill-original-turn-r1"
ORACLE = ROOT / "analysis/compat/do-rototill-turn-r1-oracle.json"


def _sha256(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def _queue_bodies(records):
    return [record["body"].hex() for record in records
            if record["kind"] == 28]


def _production_items(bodies):
    items = []
    for body in bodies:
        raw = bytes.fromhex(body)
        if len(raw) % 4:
            raise ValueError("production body is not a sequence of _prod words")
        for offset in range(0, len(raw), 4):
            word = int.from_bytes(raw[offset:offset + 4], "little")
            items.append({"quantity": word & 0x3ff,
                          "type": (word >> 10) & 0x7f,
                          "object_kind": (word >> 17) & 7,
                          "progress": (word >> 20) & 0x7f,
                          "reserved_high": word >> 27})
    return items


def _changes(before, after, prefix=""):
    """Return every changed leaf, keeping list indices stable for replay."""
    if isinstance(before, dict) and isinstance(after, dict):
        if before.keys() != after.keys():
            raise ValueError("oracle fields differ")
        result = []
        for key in before:
            result.extend(_changes(before[key], after[key],
                                   prefix + "." + key if prefix else key))
        return result
    if isinstance(before, list) and isinstance(after, list):
        if len(before) != len(after):
            return [{"field": prefix, "before": before, "after": after}]
        result = []
        for index, (old, new) in enumerate(zip(before, after)):
            result.extend(_changes(old, new, "%s[%d]" % (prefix, index)))
        return result
    return ([{"field": prefix, "before": before, "after": after}]
            if before != after else [])


def materialize():
    inspector = Inspector()
    source = RUN / "input/CLEAN.M2"
    result = RUN / "output/CLEAN.M2"
    host = RUN / "output/CLEAN.HST"
    xy = RUN / "input/CLEAN.XY"
    input_records = decode_stream(source, inspector)
    output_records = decode_stream(result, inspector)
    host_records = decode_stream(host, inspector)
    before_fleets = owner1_fleet_order_pairs(input_records, 0)
    after_fleets = owner1_fleet_order_pairs(output_records, 1)
    if [pair["fleet"]["id"] for pair in before_fleets] != [0, 1, 2, 3]:
        raise ValueError("unexpected input owner-1 fleet IDs")
    if owner1_fleet_order_pairs(host_records, 1) != after_fleets:
        raise ValueError("HST/M2 owner-1 fleet mismatch")
    before_planet = owner_planet_record(input_records, 0, 24)
    after_planet = owner_planet_record(output_records, 1, 24)
    if owner_planet_record(host_records, 1, 24) != after_planet:
        raise ValueError("HST/M2 planet-24 mismatch")
    before_queue = _queue_bodies(input_records)
    after_queue = _queue_bodies(output_records)
    if _queue_bodies(host_records) != after_queue:
        raise ValueError("HST/M2 production-queue mismatch")
    points = xy_points(xy, inspector)
    before = {"fleets": before_fleets, "planet_24": before_planet,
              "production_type_28": before_queue}
    after = {"fleets": after_fleets, "planet_24": after_planet,
             "production_type_28": after_queue}
    files = ("input/CLEAN.HST", "input/CLEAN.M2", "input/CLEAN.XY",
             "output/CLEAN.HST", "output/CLEAN.M2", "output/CLEAN.XY")
    return {
        "schema": "rototill-turn-r1-serialized-oracle-v1",
        "source_sha256": {name: _sha256(RUN / name) for name in files},
        "scope": "owner-1 compact fleets/orders, planet 24, type-28 queue",
        "player_id": 1, "selector": 3, "input_turn": 0,
        "output_turn": 1, "planet_points": [list(point) for point in points],
        "output_production_items": _production_items(after_queue),
        "before": before, "after": after,
        "changes": _changes(before, after),
    }


if __name__ == "__main__":
    ORACLE.write_text(json.dumps(materialize(), indent=2) + "\n",
                      encoding="utf-8")
