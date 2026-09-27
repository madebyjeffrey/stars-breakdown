#!/usr/bin/env python3
"""Extract retained selector-3 values that gate original RNG branches."""

import hashlib
import json
from collections import Counter
from pathlib import Path

from tools.race_file_compare import Inspector
from tools.satisfy_orders_original_oracle import (decode_compact_order,
                                                  decode_owner_fleet,
                                                  decode_planet_prefix)
from tools.stars_game_stream import decode_stream


ROOT = Path(__file__).resolve().parents[1]
RUN = ROOT / "runs/rototill-original-turn-r1"
OUTPUT = ROOT / "analysis/compat/do-rototill-rng-guards-r1.json"


def main():
    inspector = Inspector()
    sources = {}
    rows = {}
    for label, relative in (
            ("input_hst", "input/CLEAN.HST"),
            ("input_m1", "input/CLEAN.M1"),
            ("input_m2", "input/CLEAN.M2"),
            ("backup_x2", "backup/CLEAN.X2"),
            ("output_hst", "output/CLEAN.HST"),
            ("output_m2", "output/CLEAN.M2")):
        path = RUN / relative
        sources[relative] = hashlib.sha256(path.read_bytes()).hexdigest()
        rows[label] = decode_stream(path, inspector)
        expected_turn = 1 if label.startswith("output_") else 0
        if {row["turn"] for row in rows[label]} != {expected_turn}:
            raise ValueError("unexpected turn in " + relative)
    players = {row["body"][0]: row["body"]
               for row in rows["input_hst"] if row["kind"] == 6}
    if set(players) != {0, 1}:
        raise ValueError("expected exactly two retained players")
    race_stats = [players[player][62 + 14] for player in (0, 1)]
    if race_stats != [9, 1]:
        raise ValueError("race-stat gate changed")
    dead_flags = [int.from_bytes(players[player][84:86], "little")
                  for player in (0, 1)]
    if dead_flags != [0, 0]:
        raise ValueError("player dead flags changed")
    fleets = []
    battle_boundaries = {}
    for label in ("input_hst", "output_hst"):
        planets = {planet["id"]: planet["owner"]
                   for planet in (decode_planet_prefix(row["body"])
                                  for row in rows[label] if row["kind"] == 13)}
        if len(planets) != 32 or [planets[index] for index in (10, 24, 26)] != [
                0, 1, 31]:
            raise ValueError("battle planet ownership changed")
        groups = {}
        fleet_positions = []
        for row in rows[label]:
            if row["kind"] != 16:
                continue
            fleet = decode_owner_fleet(row["body"])
            owner = fleet["owner"]
            planet = fleet["planet"]
            point = tuple(fleet["point"])
            if planets[planet] not in (owner, 31):
                raise ValueError("fleet occupies a hostile planet")
            groups.setdefault(point, set()).add(owner)
            fleet_positions.append((owner, fleet["id"], planet, *point))
        if len(fleet_positions) != 10 or any(len(owners) != 1
                                              for owners in groups.values()):
            raise ValueError("mixed-owner battle location")
        expected_groups = ({(1128, 1300): {0}, (1290, 1258): {1}}
                           if label == "input_hst" else
                           {(1128, 1300): {0}, (1290, 1258): {1},
                            (1313, 1262): {1}})
        if groups != expected_groups:
            raise ValueError("battle boundary locations changed")
        battle_boundaries[label] = {
            "fleet_owner_id_planet_x_y": fleet_positions,
            "location_owners": {"%d,%d" % point: sorted(owners)
                                for point, owners in groups.items()},
            "planet_owners_10_24_26": [planets[index]
                                       for index in (10, 24, 26)],
        }
    hst = rows["input_hst"]
    for index, row in enumerate(hst[:-1]):
        if row["kind"] != 16:
            continue
        following = hst[index + 1]
        if following["kind"] != 20:
            raise ValueError("retained fleet lacks compact order")
        fleet = decode_owner_fleet(row["body"])
        order = decode_compact_order(following["body"])
        fleets.append((fleet["owner"], fleet["id"], fleet["order_count"],
                       order["object_kind"], order["warp"], order["task"]))
    if len(fleets) != 10 or any(count != 1 or kind != 1 or warp or task
                                 for _, _, count, kind, warp, task in fleets):
        raise ValueError("retained fleet order changed")
    changes = [row["body"] for row in rows["backup_x2"]
               if row["kind"] == 4]
    x2_kinds = Counter(row["kind"] for row in rows["backup_x2"])
    if x2_kinds != Counter({27: 4, 8: 1, 9: 1, 4: 1,
                            1: 1, 29: 1, 0: 1}):
        raise ValueError("X2 record inventory changed")
    x2_transfers = [row["body"] for row in rows["backup_x2"]
                    if row["kind"] in (1, 2, 25)]
    if len(x2_transfers) != 1 or x2_transfers[0].hex() != "01021800120819":
        raise ValueError("X2 cargo transfer changed")
    if len(changes) != 1 or changes[0].hex() != "000201002105ee041a005011":
        raise ValueError("AI scout order changed")
    scout_flags = int.from_bytes(changes[0][10:12], "little")
    if ((scout_flags >> 4) & 15) != 5 or ((scout_flags >> 8) & 15) != 1:
        raise ValueError("unexpected AI scout warp")
    output_fleets = []
    output_rows = rows["output_hst"]
    for index, row in enumerate(output_rows[:-1]):
        if row["kind"] != 16:
            continue
        following = output_rows[index + 1]
        if following["kind"] != 20:
            raise ValueError("output fleet lacks compact order")
        fleet = decode_owner_fleet(row["body"])
        order = decode_compact_order(following["body"])
        output_fleets.append((fleet["owner"], fleet["id"],
                              fleet["order_count"], order["object_kind"],
                              order["warp"], order["task"]))
    if (len(output_fleets) != 10 or
            any(count != 1 or kind != 1
                for _, _, count, kind, _, _ in output_fleets)):
        raise ValueError("output order inventory changed")
    things = {}
    for label in ("input_hst", "output_hst"):
        bodies = [row["body"] for row in rows[label] if row["kind"] == 43]
        if len(bodies) != 5 or bodies[0] != b"\x04\x00" or \
                any(len(body) != 18 or
                    (int.from_bytes(body[:2], "little") >> 13) != 2
                    for body in bodies[1:]):
            raise ValueError("retained thing inventory changed")
        things[label] = [2, 2, 2, 2]
    starting_queue_counts = {
        label: sum(row["kind"] == 28 for row in rows[label])
        for label in ("input_hst", "input_m1", "input_m2")
    }
    if any(starting_queue_counts.values()):
        raise ValueError("starting production queue changed")
    backup_queue = [row["body"] for row in rows["backup_x2"]
                    if row["kind"] == 29]
    if len(backup_queue) != 1 or backup_queue[0].hex() != "1800031c0200":
        raise ValueError("AI factory queue changed")
    production_word = int.from_bytes(backup_queue[0][2:6], "little")
    if ((production_word & 1023) != 3 or
            ((production_word >> 10) & 127) != 7 or
            ((production_word >> 17) & 7) != 1):
        raise ValueError("unexpected queued object")
    after_queue = [row["body"] for row in rows["output_m2"]
                   if row["kind"] == 28]
    if len(after_queue) != 1 or after_queue[0].hex() != "011c9205":
        raise ValueError("produced factory queue changed")
    result = {
        "schema": "rototill-rng-guards-r1",
        "source_sha256": sources,
        "turn_at_production_and_pre_output": 0,
        "player_race_stat_14": race_stats,
        "player_dead_flags_84": dead_flags,
        "input_fleet_owner_id_count_kind_warp_task": fleets,
        "ai_scout_order_change_body": changes[0].hex(),
        "backup_x2_record_kinds": dict(sorted(x2_kinds.items())),
        "backup_x2_cargo_transfer_body": x2_transfers[0].hex(),
        "ai_scout_warp": 5,
        "ai_scout_object_kind": 1,
        "output_fleet_owner_id_count_kind_warp_task": output_fleets,
        "battle_boundaries": battle_boundaries,
        "type_43_thing_kinds": things,
        "starting_type_28_count": starting_queue_counts,
        "ai_queue_planet_id": int.from_bytes(backup_queue[0][:2], "little"),
        "ai_queue_word": "0x%08x" % production_word,
        "ai_queue_kind": 7,
        "ai_queue_object_kind": 1,
        "ai_queue_quantity": 3,
        "output_type_28_body": after_queue[0].hex(),
    }
    OUTPUT.write_text(json.dumps(result, indent=2, sort_keys=True) + "\n")
    print("race-stat14=%s, starting queues=%s, AI queue=%s" %
          (race_stats, starting_queue_counts, result["ai_queue_word"]))


if __name__ == "__main__":
    main()
