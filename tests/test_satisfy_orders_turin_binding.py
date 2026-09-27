"""Bind the retained selector-1 Turin route and movement to original files."""

import ctypes
import hashlib
import json
import math
import struct
import subprocess
import tempfile
import unittest
from pathlib import Path

from tools.race_file_compare import Inspector, Seeds
from tools.ne_layout import NE
from tools.satisfy_orders_planet10_oracle import decode_planet_record
from tools.stars_game_stream import decode_stream, encode_stream
from tools.satisfy_orders_input_corpus import load_pair
from tools.satisfy_orders_input_bridge import complete_input_header
from tools.satisfy_orders_original_oracle import (
    decode_compact_order, decode_owner_fleet,
)
from tools.satisfy_orders_design_cost_oracle import recover as recover_design


ROOT = Path(__file__).resolve().parents[1]
PAIR = ROOT / "runs/vmctl-sye3k-pair-r1/PAIR.JSON"
ORACLE = ROOT / "analysis/compat/satisfy-orders-full-turn-e3-original-oracle.json"
INPUT_M2 = ROOT / "runs/vmctl-sye3k0/BACKUP/CLEAN.M2"
INPUT_HST = ROOT / "runs/vmctl-sye3k0/BACKUP/CLEAN.HST"
INPUT_HST_SHA256 = "74a27a31221f67328a98200c5dfde54f4c955a6498cc37ac08624b64a33997f6"
OUTPUT_HST = ROOT / "runs/vmctl-sye3k0/CLEAN.HST"
ACTION_OUTPUT_HST = ROOT / "runs/vmctl-sye3k1/CLEAN.HST"
ACTION_INPUT_HST = ROOT / "runs/vmctl-sye3k1/BACKUP/CLEAN.HST"
OUTPUT_M2 = ROOT / "runs/vmctl-sye3k0/CLEAN.M2"
ACTION_OUTPUT_M2 = ROOT / "runs/vmctl-sye3k1/CLEAN.M2"
XY_FILE = ROOT / "runs/vmctl-sye3k0/CLEAN.XY"
STARS_EXE = ROOT / "target/stars.exe"
STARS_EXE_SHA256 = "ccab6682b55dae14dbf104e491cfbd3b32a6280022020032ae0615500949ba59"


def owner1_fleet_order_pairs(records, turn):
    pairs = []
    for index, record in enumerate(records[:-1]):
        if record["turn"] != turn or record["kind"] != 16:
            continue
        fleet = decode_owner_fleet(record["body"])
        if fleet["owner"] != 1:
            continue
        following = records[index + 1]
        if following["turn"] != turn or following["kind"] != 20:
            raise AssertionError("owner-1 fleet lacks its compact order")
        order = decode_compact_order(following["body"])
        fields = ("id", "owner", "planet", "point", "ship_counts",
                  "cargo", "saved_flags", "order_count")
        pairs.append({"fleet": {field: fleet[field] for field in fields},
                      "order": order})
    return pairs


def xy_points(path, inspector):
    raw = path.read_bytes()
    header, = struct.unpack_from("<H", raw, 0)
    if header >> 10 != 8 or (header & 0x3ff) != 16:
        raise ValueError("unexpected XY BOF")
    marker, game_id, version, turn, salt_player, flags = struct.unpack(
        "<4sI4H", raw[2:18])
    if marker != b"J3J3" or version != 0x2a60:
        raise ValueError("unexpected XY identity")
    salt = salt_player >> 5
    if salt >= 1024:
        salt -= 2048
    player = salt_player & 31
    if player >= 16:
        player -= 32
    seeds = Seeds()
    inspector.lib.stars_set_file_xor_stream(
        ctypes.byref(seeds), game_id, ctypes.c_int16(salt),
        ctypes.c_int16(turn), ctypes.c_int16(player),
        (flags >> 12) & 1)
    game_header, = struct.unpack_from("<H", raw, 18)
    if game_header >> 10 != 7 or (game_header & 0x3ff) != 64:
        raise ValueError("unexpected XY game record")
    game = inspector.crypt(raw[20:84], seeds)
    count, = struct.unpack_from("<h", game, 10)
    if count < 1 or 84 + count * 4 + 4 != len(raw):
        raise ValueError("unexpected XY point-array bounds")
    points = []
    previous_x = 1000
    for index in range(count):
        packed, = struct.unpack_from("<I", raw, 84 + index * 4)
        previous_x += packed & 0x3ff
        points.append((previous_x, (packed >> 10) & 0xfff))
    return points


def hst_wormholes(path, inspector, turn=0):
    records = [record["body"] for record in decode_stream(path, inspector)
               if record["kind"] == 43 and record["turn"] == turn]
    if not records or len(records[0]) != 2:
        raise ValueError("unexpected HST thing-count record")
    count, = struct.unpack("<H", records[0])
    things = records[1:]
    if count != len(things) or any(len(thing) != 18 for thing in things):
        raise ValueError("unexpected HST thing records")
    if any((struct.unpack_from("<H", thing)[0] >> 13) != 2
           for thing in things):
        raise ValueError("fixture contains a non-wormhole thing")
    return things


def hst_planet_records(path, inspector, turn):
    records = {}
    for record in decode_stream(path, inspector):
        if record["kind"] != 13 or record["turn"] != turn:
            continue
        body = record["body"]
        planet_id = int.from_bytes(body[:2], "little") & 0x7ff
        if planet_id in records:
            raise ValueError("duplicate turn-%d planet %d" % (turn, planet_id))
        if len(body) > 37:
            raise ValueError("planet %d record exceeds current fixture width" %
                             planet_id)
        records[planet_id] = {
            "body": body,
            "header_flags": (record["kind"] << 10) | record["length"],
            "length": len(body),
        }
    if sorted(records) != list(range(32)):
        raise ValueError("turn-%d HST does not contain all 32 planets" % turn)
    return records


def player_planet_records(path, inspector, turn):
    records = {}
    for record in decode_stream(path, inspector):
        if record["turn"] != turn or record["kind"] not in (13, 14):
            continue
        body = record["body"]
        planet_id = int.from_bytes(body[:2], "little") & 0x7ff
        if planet_id in records:
            raise ValueError("duplicate turn-%d planet %d in %s" %
                             (turn, planet_id, path))
        records[planet_id] = {
            "body": body,
            "type": record["kind"],
            "length": len(body),
            "header_flags": (record["kind"] << 10) | record["length"],
        }
    return records


def planet_production_queue_record(records, turn, planet_id):
    matches = []
    for index, record in enumerate(records[:-1]):
        if (record["kind"] != 13 or record["turn"] != turn or
                len(record["body"]) < 2 or
                (int.from_bytes(record["body"][:2], "little") & 0x7ff)
                != planet_id):
            continue
        following = records[index + 1]
        if following["kind"] == 28 and following["turn"] == turn:
            matches.append(following)
    if len(matches) != 1:
        raise ValueError("expected one type-28 queue after planet %d" % planet_id)
    return matches[0]


def player_log_fleet_records(records, turn, owner=1):
    """Retain one owner's type-16 fleets with following order records."""
    result = []
    for index, record in enumerate(records[:-1]):
        if record["kind"] != 16 or record["turn"] != turn:
            continue
        fleet = decode_owner_fleet(record["body"])
        if fleet["owner"] != owner:
            continue
        following = records[index + 1]
        if following["turn"] != turn or following["kind"] not in (19, 20):
            raise ValueError("owner-1 fleet lacks its order record")
        result.append({
            "fleet": fleet,
            "body": record["body"],
            "order_type": following["kind"],
            "order_body": following["body"],
        })
    return result


def hst_fleet_points(path, inspector, turn):
    points = []
    for record in decode_stream(path, inspector):
        if record["kind"] != 16 or record["turn"] != turn:
            continue
        fleet = decode_owner_fleet(record["body"])
        points.append(tuple(fleet["point"]))
    if not points:
        raise ValueError("retained HST has no turn-%d fleets" % turn)
    return points


def retained_hull_components(records):
    """Decode M2 design IDs to the original executable's base hull slots."""
    if hashlib.sha256(STARS_EXE.read_bytes()).hexdigest() != STARS_EXE_SHA256:
        raise ValueError("original executable hash changed")
    segment = bytes(NE(STARS_EXE).raw[1])
    designs = {}
    for record in records:
        if record["kind"] != 26 or record["turn"] != 0:
            continue
        body = record["body"]
        if len(body) < 17:
            raise ValueError("truncated SHDEF record")
        design_id = (int.from_bytes(body[:2], "little") >> 10) & 31
        if design_id >= 16:
            continue
        hull_id = body[2]
        hull_offset = 0x29f0 + hull_id * 0x8f
        hull = segment[hull_offset:hull_offset + 0x8f]
        if len(hull) != 0x8f:
            raise ValueError("hull record outside original executable")
        component_count = hull[122]
        if component_count > 16:
            raise ValueError("hull component count exceeds runtime capacity")
        components = [struct.unpack_from("<HH", hull, 58 + index * 4)
                      for index in range(component_count)]
        designs[design_id] = (hull_id, components)
    return designs


def retained_ideal_warp_data(records):
    """Read retained design engines and their original EXE fuel curves."""
    if hashlib.sha256(STARS_EXE.read_bytes()).hexdigest() != STARS_EXE_SHA256:
        raise ValueError("original executable hash changed")
    segment = bytes(NE(STARS_EXE).raw[1])
    designs = {}
    for record in records:
        if record["kind"] != 26 or record["turn"] != 0:
            continue
        body = record["body"]
        if len(body) < 17:
            raise ValueError("truncated SHDEF record")
        design_id = (int.from_bytes(body[:2], "little") >> 10) & 31
        if design_id >= 16:
            continue
        engine_ids = []
        for index in range(body[6]):
            group, packed = struct.unpack_from("<HH", body, 17 + index * 4)
            item = packed & 0xff
            quantity = packed >> 8
            if group == 1 and quantity:
                engine_ids.append(item)
        if len(engine_ids) > 10:
            raise ValueError("design has more engines than the runtime record")
        designs[design_id] = engine_ids
    engines = []
    for engine_id in range(16):
        offset = engine_id * 0x4e
        row = segment[offset:offset + 0x4e]
        if len(row) != 0x4e:
            raise ValueError("engine record outside original executable")
        engines.append(list(struct.unpack_from("<12h", row, 54)))
    return designs, engines


def retained_cargo_capacity_data(records):
    """Resolve retained SHDEF components and EXE hull cargo maxima."""
    if hashlib.sha256(STARS_EXE.read_bytes()).hexdigest() != STARS_EXE_SHA256:
        raise ValueError("original executable hash changed")
    segment = bytes(NE(STARS_EXE).raw[1])
    designs = {}
    for record in records:
        if record["kind"] != 26 or record["turn"] != 0:
            continue
        body = record["body"]
        if len(body) < 17:
            raise ValueError("truncated SHDEF record")
        design_id = (int.from_bytes(body[:2], "little") >> 10) & 31
        if design_id >= 16:
            continue
        hull_id = body[2]
        component_count = body[6]
        if component_count > 16 or len(body) < 17 + component_count * 4:
            raise ValueError("SHDEF component list exceeds the record")
        hull_offset = 0x29f0 + hull_id * 0x8f
        hull = segment[hull_offset:hull_offset + 0x8f]
        if len(hull) != 0x8f:
            raise ValueError("hull record outside original executable")
        components = [struct.unpack_from("<HH", body, 17 + index * 4)
                      for index in range(component_count)]
        cargo_max, = struct.unpack_from("<H", hull, 52)
        designs[design_id] = (hull_id, cargo_max, components)
    return designs


def retained_ai_planet_status(mode, planet_count, route_state):
    # IdNearestUnknownPlanet calls IdNearestColonizablePlanet while Turin's
    # marker table is clear. It seeds absent planets from mode, classifies
    # retained M2 planets, then marks targets of multi-order fleets.
    unknown_status = 0x10 if ((mode >> 13) & 7) not in (0, 5) else 0
    status = [unknown_status for _ in range(planet_count)]
    player_id = route_state["player_id"]
    for planet_id, owner in route_state["known_planets"]:
        if not 0 <= planet_id < planet_count or owner == 31:
            raise ValueError("fixture planet cannot be classified from M2")
        status[planet_id] = 1 if owner == player_id else 2
    for pair in route_state["fleet_orders"]:
        fleet = pair["fleet"]
        order = pair["order"]
        if (fleet["owner"] == player_id and
                fleet["id"] != route_state["active_fleet_id"] and
                fleet["order_count"] > 1):
            target = order["target_id"]
            if not 0 <= target < planet_count:
                raise ValueError("fleet order target is outside the map")
            # Mode 1 makes the original fleet-order marker condition true.
            status[target] = 4
    return status


def retained_player_record(path, turn, inspector):
    matches = [record["body"] for record in decode_stream(path, inspector)
               if record["kind"] == 6 and record["turn"] == turn and
               record["body"] and record["body"][0] == 1]
    if len(matches) != 1 or len(matches[0]) < 58:
        raise ValueError("expected one complete owner-1 player record")
    return matches[0]


def retained_type45_score_record(records, turn):
    matches = [record["body"] for record in records
               if record["kind"] == 45 and record["turn"] == turn]
    if len(matches) != 1 or len(matches[0]) != 24:
        raise ValueError("expected one complete type-45 score record")
    return matches[0]


def write_fixture_header(path, points, input_pairs, output_pairs, output_pair,
                         mode, route_state, wormholes, hull_designs,
                         engine_designs, engines, cargo_designs, input_planet,
                         output_planet, player_record, shdef_record,
                         planet_record, part_segment, wormhole_outputs,
                         wormhole_fleets, full_planet_records,
                         player_log_planets, player_log_fleets,
                         player_log_owner0_fleets, production_queue_body,
                         player_log_shdefs, score_history_records):
    fleet_rows = [pair["fleet"] for pair in input_pairs]
    order_rows = [pair["order"] for pair in input_pairs]
    expected_fleet = output_pair["fleet"]
    expected_order = output_pair["order"]
    input_supply_fleet = next(row for row in fleet_rows if row["id"] == 1)
    output_supply_fleet = next(pair["fleet"] for pair in output_pairs
                               if pair["fleet"]["id"] == 1)
    supply_gain = (output_supply_fleet["cargo"][3] -
                   input_supply_fleet["cargo"][3])
    if input_planet["minerals"][3] != 250 or supply_gain != 25:
        raise AssertionError("retained Turin supply state changed")
    source = fleet_rows[0]["point"]
    destination = expected_fleet["point"]
    distance = int(math.sqrt((destination[0] - source[0]) ** 2 +
                             (destination[1] - source[1]) ** 2) + 0.9999)
    fuel_spent = fleet_rows[0]["cargo"][4] - expected_fleet["cargo"][4]
    ai_planet_status = retained_ai_planet_status(
        mode, len(points), route_state)
    input_planets = full_planet_records["input"]
    if sorted(input_planets) != list(range(32)) or len(points) != 32:
        raise ValueError("retained sweep stage requires all 32 planets")
    retained_planet_owners = []
    for planet_id in range(32):
        packed_id = int.from_bytes(input_planets[planet_id]["body"][:2],
                                   "little")
        owner = (packed_id >> 11) & 31
        retained_planet_owners.append(-1 if owner == 31 else owner)
    if sum(owner >= 0 for owner in retained_planet_owners) != 2:
        raise ValueError("retained sweep planet ownership changed")
    for label in ("control", "action"):
        if sorted(full_planet_records[label]) != list(range(32)):
            raise ValueError("retained %s output lacks the full planet table" %
                             label)
        for planet_id in range(32):
            output_packed_id = int.from_bytes(
                full_planet_records[label][planet_id]["body"][:2], "little")
            output_owner = (output_packed_id >> 11) & 31
            output_owner = -1 if output_owner == 31 else output_owner
            if output_owner != retained_planet_owners[planet_id]:
                raise ValueError("retained %s planet ownership changed" % label)
    supply_fleet = next(row for row in fleet_rows if row["id"] == 1)
    if hashlib.sha256(STARS_EXE.read_bytes()).hexdigest() != STARS_EXE_SHA256:
        raise ValueError("original executable differs from the reviewed baseline")
    pair = json.loads(PAIR.read_text(encoding="utf-8"))
    seed_binding = pair["current_source_binding_status"][
        "retained_turin_starbase_rng_source_binding"]
    oracle_data = json.loads(ORACLE.read_text(encoding="utf-8"))
    rng_oracle = oracle_data["rng_initialization_trace"][
        "recovered_turn_rng_trace"]
    tick_mix_oracle = oracle_data["rng_initialization_trace"][
        "serialized_random2000_tick_mix_observation"]
    writer_oracle = oracle_data["rng_initialization_trace"][
        "retained_output_writer_draws"]
    rng_phases = rng_oracle["phase_draws_by_run"]

    def phase_for(run, fragment):
        return next(phase for phase in rng_phases[run]
                    if fragment in phase["phase"])

    control_upgrade = phase_for("control", "Turin scout and starbase-upgrade")
    action_upgrade = phase_for("action", "Turin scout and starbase-upgrade")
    control_writers = phase_for("control", "Turin X2/H2 BOF writers")
    action_writers = phase_for("action", "Turin X2/H2 BOF writers")
    control_shuffle = phase_for("control", "FGenerateTurn player shuffle")
    action_shuffle = phase_for("action", "FGenerateTurn player shuffle")
    control_tech = phase_for("control", "AI task-5 technology learning")
    action_tech = phase_for("action", "task-5 technology-learning gates")
    control_mining = phase_for("control", "MineMinerals rounding")
    action_mining = phase_for("action", "MineMinerals rounding")
    control_events = phase_for("control", "RandomEvents entry gates")
    action_events = phase_for("action", "RandomEvents entry gates")
    control_wormhole_moves = phase_for("control", "post-production wormhole movement")
    action_wormhole_moves = phase_for("action", "post-production wormhole movement")
    control_guesses = phase_for("control", "UpdateGuesses")
    action_guesses = phase_for("action", "UpdateGuesses")
    control_saved_flag = phase_for("control", "FGenerateTurn saved flag")
    action_saved_flag = phase_for("action", "FGenerateTurn saved flag")
    control_output_bofs = phase_for("control", "FGenerateTurn M1/M2/HST BOF writers")
    action_output_bofs = phase_for("action", "FGenerateTurn M1/M2/HST BOF writers")
    inspector = Inspector()
    expected_history = []
    expected_design_logs = []
    expected_research_log = []
    expected_production_log = []
    expected_balance_log = []
    expected_order_logs = []
    expected_cargo_log = []
    for run_name in ("vmctl-sye3k0", "vmctl-sye3k1"):
        history = [r for r in decode_stream(ROOT / "runs" / run_name / "CLEAN.H2", inspector)
                   if r["kind"] != 8]
        logs = [r for r in decode_stream(ROOT / "runs" / run_name / "BACKUP/CLEAN.X2", inspector)
                if r["kind"] == 27]
        if len(history) != 4 or len(logs) != 4:
            raise ValueError("unexpected original history/design-log census")
        expected_history.append(history)
        expected_design_logs.append(logs)
        research = [r["body"] for r in decode_stream(ROOT / "runs" / run_name / "BACKUP/CLEAN.X2", inspector)
                    if r["kind"] == 34]
        if len(research) != 1 or len(research[0]) != 2:
            raise ValueError("unexpected original research-log record")
        expected_research_log.append(research[0])
        production = [r["body"] for r in decode_stream(ROOT / "runs" / run_name / "BACKUP/CLEAN.X2", inspector)
                      if r["kind"] == 29]
        if len(production) != 1 or len(production[0]) != 10:
            raise ValueError("unexpected original production-log record")
        expected_production_log.append(production[0])
        balance = [r["body"] for r in decode_stream(ROOT / "runs" / run_name / "BACKUP/CLEAN.X2", inspector)
                   if r["kind"] == 23]
        if len(balance) != 1 or len(balance[0]) != 9:
            raise ValueError("unexpected original fleet-balance log")
        expected_balance_log.append(balance[0])
        orders = [r["body"] for r in decode_stream(ROOT / "runs" / run_name / "BACKUP/CLEAN.X2", inspector)
                  if r["kind"] in (4, 5)]
        if len(orders) != 2 or any(len(r) != 12 for r in orders):
            raise ValueError("unexpected original order logs")
        expected_order_logs.append(orders)
        cargo = [r["body"] for r in decode_stream(ROOT / "runs" / run_name / "BACKUP/CLEAN.X2", inspector)
                 if r["kind"] == 1]
        if len(cargo) != 1 or len(cargo[0]) != 7:
            raise ValueError("unexpected original cargo log")
        expected_cargo_log.append(cargo[0])
    expected_startup_components = []
    expected_startup_names = []
    expected_startup_damage = []
    byte_pointer = ctypes.POINTER(ctypes.c_ubyte)
    inspector.lib.stars_decompress_user_string.argtypes = [
        byte_pointer, byte_pointer, ctypes.c_int16, byte_pointer,
        ctypes.POINTER(ctypes.c_int16)]
    inspector.lib.stars_decompress_user_string.restype = ctypes.c_int
    name_data = (ctypes.c_ubyte * 65536).from_buffer_copy(inspector.initial)
    for output_path in (OUTPUT_M2, ACTION_OUTPUT_M2):
        designs = {((int.from_bytes(r["body"][:2], "little") >> 10) & 31):
                   r["body"] for r in decode_stream(output_path, inspector)
                   if r["kind"] == 26}
        rows = []
        names = []
        damage = []
        for design_id in (18, 20, 17, 19):
            body = designs[design_id]
            damage.append(int.from_bytes(body[4:6], "little"))
            components = body[17:17 + body[6] * 4]
            rows.append(list(components) + [0] * (64 - len(components)))
            payload = body[17 + body[6] * 4:]
            output_name = (ctypes.c_ubyte * 32)()
            if payload[0]:
                encoded = (ctypes.c_ubyte * (len(payload) - 1)).from_buffer_copy(payload[1:])
                capacity = ctypes.c_int16(31)
                if not inspector.lib.stars_decompress_user_string(
                        name_data, encoded, payload[0], output_name, ctypes.byref(capacity)):
                    raise ValueError("original starbase name did not decode")
                name = bytes(output_name).split(b"\0", 1)[0]
            else:
                name = payload[1:].split(b"\0", 1)[0]
            names.append(list(name) + [0] * (32 - len(name)))
        expected_startup_components.append(rows)
        expected_startup_names.append(names)
        expected_startup_damage.append(damage)
    if sum(inspector.initial[0x1450:0x1450 + 39]) != 125:
        raise ValueError("unexpected original AI choice table size")
    retained_output_bofs = {"control": [], "action": []}
    output_bof_ticks = {"control": [], "action": []}
    input_bofs = [record for record in decode_stream(INPUT_HST, inspector)
                  if record["kind"] == 8 and record["turn"] == 0]
    if len(input_bofs) != 1 or len(input_bofs[0]["body"]) != 16:
        raise ValueError("expected one turn-0 input HST BOF")
    input_bof_body = input_bofs[0]["body"]
    action_input_records = decode_stream(ACTION_INPUT_HST, inspector)
    owner0_task5 = None
    for record_index, record in enumerate(action_input_records[:-1]):
        if record["turn"] != 0 or record["kind"] != 16:
            continue
        fleet = decode_owner_fleet(record["body"])
        if fleet["owner"] == 0 and fleet["id"] == 0:
            following = action_input_records[record_index + 1]
            if following["turn"] != 0 or following["kind"] != 19:
                raise ValueError("owner-0 action fleet lacks task-5 order")
            if fleet["planet"] != 10 or fleet["ship_counts"][0] != 1:
                raise ValueError("retained owner-0 task-5 fleet changed")
            owner0_task5 = (fleet, following["body"])
            break
    if owner0_task5 is None or len(owner0_task5[1]) != 18:
        raise ValueError("retained owner-0 task-5 input is missing")
    owner0_fleet, owner0_order = owner0_task5
    owner0_fleet_raw = bytearray(124)
    struct.pack_into("<HHHhhh", owner0_fleet_raw, 0,
                     owner0_fleet["id"], owner0_fleet["owner"],
                     owner0_fleet["saved_flags"], owner0_fleet["planet"],
                     owner0_fleet["point"][0], owner0_fleet["point"][1])
    for ship_index, count in enumerate(owner0_fleet["ship_counts"]):
        struct.pack_into("<H", owner0_fleet_raw, 12 + ship_index * 2, count)
    for cargo_index, amount in enumerate(owner0_fleet["cargo"]):
        struct.pack_into("<I", owner0_fleet_raw, 76 + cargo_index * 4,
                         amount)
    owner0_fleet_raw[98] = owner0_fleet["order_count"]
    owner0_design_oracle = recover_design()
    owner0_design_raw = bytearray(147)
    struct.pack_into("<H", owner0_design_raw, 0,
                     owner0_design_oracle["hull"]["id"])
    for cost_index, amount in enumerate(
            owner0_design_oracle["owner_design_cost"]):
        struct.pack_into("<H", owner0_design_raw, 0x2a + cost_index * 2,
                         amount)
    production_witness = oracle_data[
        "current_source_control_action_pair"]["owner1_planet24"][
            "production_mining_stage"]["owner1_exact_mineral_stock_witness"]
    production_replay = production_witness[
        "current_source_produce_queue_progress_replay"]
    if production_replay["starting_resources"] != 28:
        raise ValueError("retained queue resource outcome changed")
    for label, output_dir, phase in (
            ("control", OUTPUT_HST.parent, control_output_bofs),
            ("action", ACTION_OUTPUT_HST.parent, action_output_bofs)):
        for filename, expected_player, expected_type, random_value in (
                ("CLEAN.M1", 0, 3, phase["values"][0]),
                ("CLEAN.M2", 1, 3, phase["values"][1]),
                ("CLEAN.HST", 31, 2, phase["values"][2])):
            records = [record for record in decode_stream(
                output_dir / filename, inspector)
                       if record["kind"] == 8 and record["turn"] == 1]
            if len(records) != 1 or len(records[0]["body"]) != 16:
                raise ValueError("expected one turn-1 %s BOF" % filename)
            body = records[0]["body"]
            marker, game_id, version, turn = struct.unpack_from(
                "<4sIHH", body)
            salt_player, flags = struct.unpack_from("<HH", body, 12)
            if ((salt_player & 31) != expected_player or
                    (flags & 0x00ff) != expected_type or
                    marker != input_bof_body[:4] or
                    game_id.to_bytes(4, "little") != input_bof_body[4:8] or
                    version != 0x2a60 or turn != 1 or
                    (flags & 0x0b00) != 0):
                raise ValueError("retained %s BOF identity changed" % filename)
            file_bytes = (output_dir / filename).read_bytes()
            record_start = records[0]["offset"]
            record_bytes = file_bytes[record_start:
                                      record_start + 2 + records[0]["length"]]
            if len(record_bytes) != 18:
                raise ValueError("retained %s BOF record is truncated" % filename)
            retained_output_bofs[label].append(list(record_bytes))
            output_bof_ticks[label].append(
                (((salt_player >> 5) - random_value) & 0x07ff))
    for label in ("control", "action"):
        expected_ticks = writer_oracle[
            "derived_gettickcount_low11_by_write"][label][2:5]
        expected_salts = [
            write["tick_plus_random2000_mod_2048"]
            for write in tick_mix_oracle["runs"][label]["writes"][2:5]]
        actual_salts = [
            struct.unpack_from("<H", bytes(record), 14)[0] >> 5
            for record in retained_output_bofs[label]]
        if output_bof_ticks[label] != expected_ticks or \
                actual_salts != expected_salts:
            raise ValueError("retained %s output BOF tick/salt binding changed" %
                             label)
    for label, phase, positions in (
            ("control", control_wormhole_moves, [40, 51]),
            ("action", action_wormhole_moves, [22, 33])):
        if (phase["positions"] != positions or len(phase["values"]) != 12 or
                phase["limits"] != [100, 25, 25] * 4 or
                len(wormhole_outputs[label]) != len(wormholes)):
            raise ValueError("retained %s wormhole movement phase changed" % label)
        if len(wormhole_fleets[label]) not in (7, 8):
            raise ValueError("retained %s fleet obstacle count changed" % label)
    input_hst_player = retained_player_record(INPUT_HST, 0, inspector)
    output_hst_player = retained_player_record(OUTPUT_HST, 1, inspector)
    input_hst_records = decode_stream(INPUT_HST, inspector)
    mining_players = {record["body"][0]: record["body"]
                      for record in input_hst_records
                      if record["kind"] == 6 and record["turn"] == 0}
    mining_planets = []
    for record in input_hst_records:
        if record["kind"] != 13 or record["turn"] != 0:
            continue
        packed_id, = struct.unpack_from("<H", record["body"], 0)
        planet_id = packed_id & 0x7ff
        owner = (packed_id >> 11) & 31
        if owner == 31:
            continue
        planet = decode_planet_record(record["body"],
                                      expected_id=planet_id)
        player = mining_players[owner]
        mine_percent = player[0x3e + 6]
        operating_mines = min(planet["improvements"]["mines"],
                              planet["minerals"][3] * mine_percent // 100)
        mining_planets.append({
            "id": planet_id,
            "owner": owner,
            "special_race": int(player[0x3e + 14] == 8),
            "efficiency": player[0x3e + 4],
            "operating_mines": operating_mines,
            "concentrations": planet["concentrations"],
            "percent_levels": planet["levels"],
            "stocks": planet["minerals"][:3],
        })
    if [planet["id"] for planet in mining_planets] != [10, 24]:
        raise ValueError("retained global HST mining planets changed")
    if any(planet["operating_mines"] != 10 for planet in mining_planets):
        raise ValueError("retained operating-mine counts changed")

    def mining_yields(phase):
        values = phase["values"]
        if phase["positions"] not in ([31, 36], [13, 18]) or len(values) != 6:
            raise ValueError("retained full-map mining draw phase changed")
        outputs = []
        cursor = 0
        for planet in mining_planets:
            planet_output = []
            for concentration in planet["concentrations"]:
                product = (planet["operating_mines"] * concentration *
                           planet["efficiency"] // 10)
                amount, remainder = divmod(product, 100)
                if remainder and values[cursor] < remainder:
                    amount += 1
                planet_output.append(amount)
                cursor += 1
            outputs.append(planet_output)
        return outputs

    control_mining_yields = mining_yields(control_mining)
    action_mining_yields = mining_yields(action_mining)
    if (control_events["limits"] != [20, 20, 15] or
            action_events["limits"] != [20, 20, 15] or
            control_events["positions"] != [37, 39] or
            action_events["positions"] != [19, 21]):
        raise ValueError("retained turn-zero random-event gates changed")
    symbols = json.loads((ROOT / "analysis/debug/symbols.json").read_text())
    research_symbol = next(row for row in symbols
                           if row.get("name") ==
                           "vrgAiTurinDroneResOrder")
    research_requirements = bytes(NE(STARS_EXE).raw[
        research_symbol["ne_segment"] - 1][research_symbol["ne_offset"]:
        research_symbol["ne_offset"] + 31])
    if len(research_requirements) != 31:
        raise ValueError("retained Turin research requirement table is truncated")
    if (input_hst_player[26:32] != bytes((0, 0, 1, 0, 5, 0)) or
            input_hst_player[56:58] != bytes((15, 0x60)) or
            output_hst_player[56:58] != bytes((15, 0x32))):
        raise ValueError("retained Turin research-state differential changed")

    guess_planet_rows = {"control": [], "action": []}
    guess_input_planets = {}
    guess_output_planet_bodies = {"control": {}, "action": {}}
    for record in input_hst_records:
        if record["kind"] != 13 or record["turn"] != 0:
            continue
        planet_id = int.from_bytes(record["body"][:2], "little") & 0x7ff
        if planet_id in (10, 24):
            guess_input_planets[planet_id] = record["body"]
    if set(guess_input_planets) != {10, 24}:
        raise ValueError("retained UpdateGuesses input planet records changed")
    planet10_input = decode_planet_record(guess_input_planets[10], 10)
    planet10_player = mining_players[0]
    planet10_output_states = {}
    guess_runtime_planets = {}
    for label, output_path in (("control", OUTPUT_HST),
                               ("action", ACTION_OUTPUT_HST)):
        output_planets = {}
        for record in decode_stream(output_path, inspector):
            if record["kind"] != 13 or record["turn"] != 1:
                continue
            planet_id = int.from_bytes(record["body"][:2], "little") & 0x7ff
            if planet_id in (10, 24):
                output_planets[planet_id] = record["body"]
        if set(output_planets) != {10, 24}:
            raise ValueError("retained HST output lacks guessed planets")
        guess_output_planet_bodies[label] = output_planets
        planet10_output_states[label] = decode_planet_record(
            output_planets[10], 10)
        for planet_id in (10, 24):
            before_body = guess_input_planets[planet_id]
            after_body = output_planets[planet_id]
            before = decode_planet_record(before_body, planet_id)
            after = decode_planet_record(after_body, planet_id)
            initial_guess, = struct.unpack_from("<H", before_body, 11)
            output_guess, = struct.unpack_from("<H", after_body, 14)
            if initial_guess != 0x003e or after["improvements"] is None:
                raise ValueError("retained UpdateGuesses input fields changed")
            guess_planet_rows[label].append({
                "owner": after["owner"],
                "guess_word": initial_guess,
                "mineral_value": after["minerals"][3],
                "survival_flags": after["improvements"]["defenses"],
                "output_guess": output_guess,
            })
            if label == "control":
                runtime_planet = bytearray(56)
                struct.pack_into("<Hh", runtime_planet, 0, planet_id,
                                 before["owner"])
                runtime_planet[12:15] = bytes(before["environment"])
                struct.pack_into("<I", runtime_planet, 40,
                                 after["minerals"][3])
                guess_runtime_planets[planet_id] = bytes(runtime_planet)
            else:
                control_body = next(
                    record["body"] for record in decode_stream(
                        OUTPUT_HST, inspector)
                    if record["kind"] == 13 and record["turn"] == 1 and
                    (int.from_bytes(record["body"][:2], "little") & 0x7ff)
                    == planet_id)
                control_state = decode_planet_record(control_body, planet_id)
                if (control_state["owner"] != after["owner"] or
                        control_state["environment"] != before["environment"] or
                        control_state["minerals"][3] != after["minerals"][3]):
                    raise ValueError(
                        "control/action survival inputs for UpdateGuesses differ")
    guess_input_planet_records = [guess_runtime_planets[planet_id]
                                  for planet_id in (10, 24)]
    turn_input_planet_bodies = [guess_input_planets[planet_id]
                                for planet_id in (10, 24)]
    turn_output_planet_states = {
        label: [decode_planet_record(
            guess_output_planet_bodies[label][planet_id], planet_id)
                for planet_id in (10, 24)]
        for label in ("control", "action")
    }
    for bodies in (turn_input_planet_bodies,
                   guess_output_planet_bodies["control"].values(),
                   guess_output_planet_bodies["action"].values()):
        if any(len(body) not in (33, 37) for body in bodies):
            raise ValueError("retained output planet record width changed")
    if (guess_planet_rows["control"][0]["mineral_value"] != 287 or
            guess_planet_rows["control"][1]["mineral_value"] != 256 or
            guess_planet_rows["action"][0]["mineral_value"] != 287 or
            guess_planet_rows["action"][1]["mineral_value"] != 256 or
            any(row["survival_flags"] != 10
                for rows in guess_planet_rows.values() for row in rows)):
        raise ValueError("retained UpdateGuesses planet values changed")
    if (planet10_input["owner"] != 0 or
            planet10_input["minerals"][3] != 250 or
            planet10_input["improvements"]["population_delta"] != 0 or
            planet10_output_states["control"]["minerals"][3] != 287 or
            planet10_output_states["control"]["improvements"][
                "population_delta"] != 50 or
            planet10_output_states["action"]["minerals"][3] != 287 or
            planet10_output_states["action"]["improvements"][
                "population_delta"] != 50):
        raise ValueError("retained planet-10 growth inputs/outputs changed")
    guess_race_stats = [mining_players[owner][0x3e + 14]
                        for owner in range(2)]
    if guess_race_stats != [9, 1] or 3 in guess_race_stats or \
            4 in guess_race_stats:
        raise ValueError("retained terraforming/transit-breeding traits changed")
    for label, phase, positions, limits in (
            ("control", control_guesses, [52, 53], [71, 64]),
            ("action", action_guesses, [34, 35], [71, 64])):
        if phase["positions"] != positions or phase["limits"] != limits:
            raise ValueError("retained UpdateGuesses phase changed")
    if (control_saved_flag["positions"] != [54, 54] or
            control_saved_flag["limits"] != [8] or
            action_saved_flag["positions"] != [36, 36] or
            action_saved_flag["limits"] != [8] or
            control_output_bofs["positions"] != [55, 57] or
            action_output_bofs["positions"] != [37, 39] or
            control_output_bofs["limits"] != [2000, 2000, 2000] or
            action_output_bofs["limits"] != [2000, 2000, 2000]):
        raise ValueError("retained saved-flag or output-BOF phase changed")

    def c_array(values):
        return ", ".join(str(value) for value in values)

    resource_planet24 = bytearray(56)
    resource_factories = input_planet["improvements"]["factories"]
    if (input_planet["owner"] != 1 or
            input_planet["minerals"][3] != 250 or resource_factories != 10):
        raise ValueError("retained CResourcesAtPlanet inputs changed")
    struct.pack_into("<Hh", resource_planet24, 0, 24, 1)
    resource_planet24[12:15] = bytes(input_planet["environment"])
    packed_improvements = (
        (resource_factories << 20) |
        (input_planet["improvements"]["mines"] << 8) |
        input_planet["improvements"]["population_delta"])
    struct.pack_into("<I", resource_planet24, 20, packed_improvements)
    resource_planet24[24] = resource_factories
    struct.pack_into("<I", resource_planet24, 40,
                     input_planet["minerals"][3])

    input_corpus = load_pair(inspector)

    cost_symbol = next(row for row in symbols if row.get("name") == "rglTechCost")
    cost_segment = NE(STARS_EXE).raw[cost_symbol["ne_segment"] - 1]
    research_costs = struct.unpack_from("<27I", cost_segment, cost_symbol["ne_offset"])
    research_expected = []
    for output_path in (OUTPUT_HST, ACTION_OUTPUT_HST):
        rows = decode_stream(output_path, inspector)
        players = {row["body"][0]: row["body"] for row in rows if row["kind"] == 6}
        research_expected.append([players[player][26:62] for player in range(2)])
    lines = [
        "#define TURIN_RETAINED_FLEET_COUNT %d" % len(fleet_rows),
        "#define TURIN_RETAINED_POINT_COUNT %d" % len(points),
        "#define TURIN_RETAINED_PLAYER_MODE 0x%04xU" % mode,
        "#define TURIN_RETAINED_EXPECTED_TARGET %d" % expected_order["target_id"],
        "#define TURIN_RETAINED_EXPECTED_WARP %d" % expected_order["warp"],
        "#define TURIN_RETAINED_EXPECTED_X %d" % destination[0],
        "#define TURIN_RETAINED_EXPECTED_Y %d" % destination[1],
        "#define TURIN_RETAINED_EXPECTED_FLAGS 0x%04xU" % expected_order["flags"],
        "#define TURIN_RETAINED_EXPECTED_FUEL %d" % expected_fleet["cargo"][4],
        "static const StarsU8 retained_planet24_production_queue[8] = {%s};" %
        c_array(production_queue_body),
        "static const StarsU8 retained_owner0_task5_fleet[124] = {%s};" %
        c_array(owner0_fleet_raw),
        "static const StarsU8 retained_owner0_task5_order[18] = {%s};" %
        c_array(owner0_order),
        "static const StarsU8 retained_owner0_task5_design[147] = {%s};" %
        c_array(owner0_design_raw),
        "#define TURIN_RETAINED_MOVE_DISTANCE %d" % distance,
        "#define TURIN_RETAINED_THING_COUNT %d" % len(wormholes),
        "#define TURIN_RETAINED_TICK_CONTROL %dUL" %
        seed_binding["control"]["effective_tick_low14"],
        "#define TURIN_RETAINED_TICK_ACTION %dUL" %
        seed_binding["action"]["effective_tick_low14"],
        "#define TURIN_RETAINED_SCOUT_DRAW_CONTROL %d" %
        seed_binding["control"]["scout_draw5_random100"],
        "#define TURIN_RETAINED_SCOUT_DRAW_ACTION %d" %
        seed_binding["action"]["scout_draw5_random100"],
        "#define TURIN_RETAINED_RESEARCH_REQUIREMENTS_COUNT 31",
        "#define TURIN_RETAINED_INPUT_CURRENT_TECH %dU" % input_hst_player[57],
        "#define TURIN_RETAINED_INPUT_PERCENT_RESEARCH %d" %
        input_hst_player[56],
        "#define TURIN_RETAINED_OUTPUT_CURRENT_TECH %dU" % output_hst_player[57],
        "#define TURIN_RETAINED_OUTPUT_PERCENT_RESEARCH %d" %
        output_hst_player[56],
        "#define TURIN_RETAINED_RESEARCH_ORDER 0",
        "#define TURIN_RETAINED_LATEST_STARBASE_DESIGN 0",
        "#define TURIN_RETAINED_UPGRADE_DRAW_CONTROL %d" %
        control_upgrade["values"][1],
        "#define TURIN_RETAINED_UPGRADE_DRAW_ACTION %d" %
        action_upgrade["values"][1],
        "#define TURIN_RETAINED_BOF_DRAW_CONTROL_0 %d" %
        control_writers["values"][0],
        "#define TURIN_RETAINED_BOF_DRAW_CONTROL_1 %d" %
        control_writers["values"][1],
        "#define TURIN_RETAINED_BOF_DRAW_ACTION_0 %d" %
        action_writers["values"][0],
        "#define TURIN_RETAINED_BOF_DRAW_ACTION_1 %d" %
        action_writers["values"][1],
        "#define TURIN_RETAINED_SHUFFLE_DRAW_CONTROL_0 %d" %
        control_shuffle["values"][0],
        "#define TURIN_RETAINED_SHUFFLE_DRAW_CONTROL_1 %d" %
        control_shuffle["values"][1],
        "#define TURIN_RETAINED_SHUFFLE_DRAW_ACTION_0 %d" %
        action_shuffle["values"][0],
        "#define TURIN_RETAINED_SHUFFLE_DRAW_ACTION_1 %d" %
        action_shuffle["values"][1],
        "#define TURIN_RETAINED_TECH_DRAWS_CONTROL %d" %
        len(control_tech["values"]),
        "#define TURIN_RETAINED_TECH_DRAWS_ACTION %d" %
        len(action_tech["values"]),
        "#define TURIN_RETAINED_MINE_DRAW_COUNT 6",
        "static const StarsI16 retained_guess_limits_control[2] = {%s};" %
        c_array(control_guesses["limits"]),
        "static const StarsI16 retained_guess_limits_action[2] = {%s};" %
        c_array(action_guesses["limits"]),
        "static const StarsI16 retained_guess_values_control[2] = {%s};" %
        c_array(control_guesses["values"]),
        "static const StarsI16 retained_guess_values_action[2] = {%s};" %
        c_array(action_guesses["values"]),
        "static const StarsI16 retained_guess_race_stats[2] = {%s};" %
        c_array(guess_race_stats),
        "static const StarsI16 retained_guess_player_current_tech[2] = {%s};" %
        c_array([mining_players[player][57] for player in range(2)]),
        "static const signed char retained_guess_player_technology[2][6] = {",
        "    {%s}," % c_array(list(mining_players[0][26:32])),
        "    {%s}," % c_array(list(mining_players[1][26:32])),
        "};",
        "static const StarsI16 retained_output_bof_values_control[3] = {%s};" %
        c_array(control_output_bofs["values"]),
        "static const StarsI16 retained_output_bof_values_action[3] = {%s};" %
        c_array(action_output_bofs["values"]),
        "static const StarsU32 retained_output_bof_ticks_control[3] = {%s};" %
        c_array(output_bof_ticks["control"]),
        "static const StarsU32 retained_output_bof_ticks_action[3] = {%s};" %
        c_array(output_bof_ticks["action"]),
        "static const StarsU8 retained_bof_marker[4] = {%s};" %
        c_array(input_bof_body[:4]),
        "static const StarsU8 retained_game_id[4] = {%s};" %
        c_array(input_bof_body[4:8]),
        "static const StarsU8 retained_output_bof_records_control[3][18] = {",
    ]
    lines.extend("    {%s}," % c_array(record)
                 for record in retained_output_bofs["control"])
    lines.extend([
        "};",
        "static const StarsU8 retained_output_bof_records_action[3][18] = {",
    ])
    lines.extend("    {%s}," % c_array(record)
                 for record in retained_output_bofs["action"])
    lines.extend([
        "};",
        "#define TURIN_RETAINED_ALL_PLANET_COUNT 32U",
        "static const StarsU16 retained_all_planet_ids[32] = {%s};" %
        c_array(list(range(32))),
        "static const StarsU16 retained_all_planet_input_header_flags[32] = {%s};" %
        c_array([full_planet_records["input"][planet_id]["header_flags"]
                 for planet_id in range(32)]),
        "static const StarsU16 retained_all_planet_input_lengths[32] = {%s};" %
        c_array([full_planet_records["input"][planet_id]["length"]
                 for planet_id in range(32)]),
        "static const StarsU8 retained_all_planet_input_bodies[32][37] = {",
    ])
    lines.extend("    {%s}," % c_array(
        list(full_planet_records["input"][planet_id]["body"]) +
        [0] * (37 - full_planet_records["input"][planet_id]["length"]))
                 for planet_id in range(32))
    lines.extend([
        "};",
        "static const StarsU16 retained_all_planet_output_lengths_control[32] = {%s};" %
        c_array([full_planet_records["control"][planet_id]["length"]
                 for planet_id in range(32)]),
        "static const StarsU8 retained_all_planet_output_bodies_control[32][37] = {",
    ])
    lines.extend("    {%s}," % c_array(
        list(full_planet_records["control"][planet_id]["body"]) +
        [0] * (37 - full_planet_records["control"][planet_id]["length"]))
                 for planet_id in range(32))
    lines.extend([
        "};",
        "static const StarsU16 retained_all_planet_output_lengths_action[32] = {%s};" %
        c_array([full_planet_records["action"][planet_id]["length"]
                 for planet_id in range(32)]),
        "static const StarsU8 retained_all_planet_output_bodies_action[32][37] = {",
    ])
    lines.extend("    {%s}," % c_array(
        list(full_planet_records["action"][planet_id]["body"]) +
        [0] * (37 - full_planet_records["action"][planet_id]["length"]))
                 for planet_id in range(32))
    m1_input_ids = sorted(player_log_planets["input_m1"])
    m1_output_ids = sorted(player_log_planets["control_m1"])
    m2_output_ids = sorted(player_log_planets["control_m2"])
    lines.extend([
        "};",
        "#define TURIN_M1_PLANET_RECORD_COUNT %dU" % len(m1_input_ids),
        "static const StarsU16 retained_m1_planet_input_ids[%d] = {%s};" %
        (len(m1_input_ids), c_array(m1_input_ids)),
        "static const StarsU16 retained_m1_planet_input_types[%d] = {%s};" %
        (len(m1_input_ids), c_array([player_log_planets["input_m1"][planet_id]["type"]
                                     for planet_id in m1_input_ids])),
        "static const StarsU16 retained_m1_planet_input_lengths[%d] = {%s};" %
        (len(m1_input_ids), c_array([player_log_planets["input_m1"][planet_id]["length"]
                                     for planet_id in m1_input_ids])),
        "static const StarsU8 retained_m1_planet_input_bodies[%d][37] = {" %
        len(m1_input_ids),
    ])
    lines.extend("    {%s}," % c_array(
        list(player_log_planets["input_m1"][planet_id]["body"]) +
        [0] * (37 - player_log_planets["input_m1"][planet_id]["length"]))
                 for planet_id in m1_input_ids)
    for run in ("control", "action"):
        records = player_log_planets[run + "_m1"]
        ids = sorted(records)
        lines.extend([
            "};",
            "static const StarsU16 retained_m1_planet_output_types_%s[%d] = {%s};" %
            (run, len(ids), c_array([records[planet_id]["type"] for planet_id in ids])),
            "static const StarsU16 retained_m1_planet_output_lengths_%s[%d] = {%s};" %
            (run, len(ids), c_array([records[planet_id]["length"] for planet_id in ids])),
            "static const StarsU8 retained_m1_planet_output_bodies_%s[%d][37] = {" %
            (run, len(ids)),
        ])
        lines.extend("    {%s}," % c_array(
            list(records[planet_id]["body"]) +
            [0] * (37 - records[planet_id]["length"])) for planet_id in ids)
    lines.extend(["};", "#define TURIN_M1_OUTPUT_PLANET_COUNT %dU" %
                  len(m1_output_ids)])
    for run in ("control", "action"):
        records = player_log_planets[run + "_m2"]
        ids = sorted(records)
        if run == "action":
            lines.append("};")
        lines.extend([
            "static const StarsU16 retained_m2_planet_output_ids_%s[%d] = {%s};" %
            (run, len(ids), c_array(ids)),
            "static const StarsU16 retained_m2_planet_output_types_%s[%d] = {%s};" %
            (run, len(ids), c_array([records[planet_id]["type"] for planet_id in ids])),
            "static const StarsU16 retained_m2_planet_output_lengths_%s[%d] = {%s};" %
            (run, len(ids), c_array([records[planet_id]["length"] for planet_id in ids])),
            "static const StarsU8 retained_m2_planet_output_bodies_%s[%d][37] = {" %
            (run, len(ids)),
        ])
        lines.extend("    {%s}," % c_array(
            list(records[planet_id]["body"]) +
            [0] * (37 - records[planet_id]["length"])) for planet_id in ids)
    lines.extend([
        "};",
        "#define TURIN_M2_OUTPUT_PLANET_COUNT %dU" % len(m2_output_ids),
        "#define TURIN_M2_PLANET_INPUT_COUNT %dU" %
        len(player_log_planets["input_m2"]),
        "static const StarsU16 retained_m2_planet_input_ids[%d] = {%s};" %
        (len(player_log_planets["input_m2"]),
         c_array(sorted(player_log_planets["input_m2"]))),
        "static const StarsU16 retained_m2_planet_input_types[%d] = {%s};" %
        (len(player_log_planets["input_m2"]),
         c_array([player_log_planets["input_m2"][planet_id]["type"]
                  for planet_id in sorted(player_log_planets["input_m2"]) ])),
        "static const StarsU16 retained_m2_planet_input_lengths[%d] = {%s};" %
        (len(player_log_planets["input_m2"]),
         c_array([player_log_planets["input_m2"][planet_id]["length"]
                  for planet_id in sorted(player_log_planets["input_m2"]) ])),
        "static const StarsU8 retained_m2_planet_input_bodies[%d][37] = {" %
        len(player_log_planets["input_m2"]),
    ])
    lines.extend("    {%s}," % c_array(
        list(player_log_planets["input_m2"][planet_id]["body"]) +
        [0] * (37 - player_log_planets["input_m2"][planet_id]["length"]))
                 for planet_id in sorted(player_log_planets["input_m2"]))
    lines.extend([
        "};",
        "#define TURIN_M2_OUTPUT_FLEET_COUNT %dU" %
        len(player_log_fleets["control"]),
        "#define TURIN_M2_INPUT_FLEET_COUNT %dU" %
        len(player_log_fleets["input"]),
        "static const StarsU16 retained_m2_fleet_input_lengths[%d] = {%s};" %
        (len(player_log_fleets["input"]), c_array([
            len(row["body"]) for row in player_log_fleets["input"]])),
        "static const StarsU8 retained_m2_fleet_input_bodies[%d][64] = {" %
        len(player_log_fleets["input"]),
    ])
    lines.extend("    {%s}," % c_array(
        list(row["body"]) + [0] * (64 - len(row["body"])))
                 for row in player_log_fleets["input"])
    lines.extend([
        "};",
        "static const StarsU16 retained_m2_order_input_types[%d] = {%s};" %
        (len(player_log_fleets["input"]), c_array([
            row["order_type"] for row in player_log_fleets["input"]])),
        "static const StarsU16 retained_m2_order_input_lengths[%d] = {%s};" %
        (len(player_log_fleets["input"]), c_array([
            len(row["order_body"]) for row in player_log_fleets["input"]])),
        "static const StarsU8 retained_m2_order_input_bodies[%d][18] = {" %
        len(player_log_fleets["input"]),
    ])
    lines.extend("    {%s}," % c_array(
        list(row["order_body"]) + [0] * (18 - len(row["order_body"])))
                 for row in player_log_fleets["input"])
    lines.extend([
        "};",
        "static const StarsU16 retained_m2_fleet_input_id_player_bits[%d] = {%s};" %
        (len(player_log_fleets["input"]), c_array([
            row["fleet"]["id_player_bits"]
            for row in player_log_fleets["input"]])),
        "static const StarsU8 retained_m2_fleet_input_design_plan[%d] = {%s};" %
        (len(player_log_fleets["input"]), c_array([
            row["fleet"]["design_plan"]
            for row in player_log_fleets["input"]])),
        "static const StarsU16 retained_m2_fleet_output_ids[%d] = {%s};" %
        (len(player_log_fleets["control"]), c_array([
            row["fleet"]["id"] for row in player_log_fleets["control"]])),
        "static const StarsU16 retained_m2_fleet_output_lengths[%d] = {%s};" %
        (len(player_log_fleets["control"]), c_array([
            len(row["body"]) for row in player_log_fleets["control"]])),
        "static const StarsU8 retained_m2_fleet_output_bodies[%d][64] = {" %
        len(player_log_fleets["control"]),
    ])
    lines.extend("    {%s}," % c_array(
        list(row["body"]) + [0] * (64 - len(row["body"])))
                 for row in player_log_fleets["control"])
    lines.extend([
        "};",
        "static const StarsU16 retained_m2_order_output_lengths[%d] = {%s};" %
        (len(player_log_fleets["control"]), c_array([
            len(row["order_body"]) for row in player_log_fleets["control"]])),
        "static const StarsU8 retained_m2_order_output_bodies[%d][18] = {" %
        len(player_log_fleets["control"]),
    ])
    lines.extend("    {%s}," % c_array(
        list(row["order_body"]) + [0] * (18 - len(row["order_body"])))
                 for row in player_log_fleets["control"])
    lines.extend([
        "};",
        "#define TURIN_HST_OWNER0_INPUT_FLEET_COUNT %dU" %
        len(player_log_owner0_fleets["input"]),
        "#define TURIN_HST_OWNER0_CONTROL_FLEET_COUNT %dU" %
        len(player_log_owner0_fleets["control"]),
        "#define TURIN_HST_OWNER0_ACTION_FLEET_COUNT %dU" %
        len(player_log_owner0_fleets["action"]),
        "static const StarsU16 retained_hst_owner0_input_lengths[%d] = {%s};" %
        (len(player_log_owner0_fleets["input"]), c_array([
            len(row["body"]) for row in player_log_owner0_fleets["input"]])),
        "static const StarsU8 retained_hst_owner0_input_bodies[%d][64] = {" %
        len(player_log_owner0_fleets["input"]),
    ])
    lines.extend("    {%s}," % c_array(
        list(row["body"]) + [0] * (64 - len(row["body"])))
                 for row in player_log_owner0_fleets["input"])
    lines.extend([
        "};",
        "static const StarsU16 retained_hst_owner0_order_input_types[%d] = {%s};" %
        (len(player_log_owner0_fleets["input"]), c_array([
            row["order_type"] for row in player_log_owner0_fleets["input"]])),
        "static const StarsU16 retained_hst_owner0_order_input_lengths[%d] = {%s};" %
        (len(player_log_owner0_fleets["input"]), c_array([
            len(row["order_body"]) for row in player_log_owner0_fleets["input"]])),
        "static const StarsU8 retained_hst_owner0_order_input_bodies[%d][18] = {" %
        len(player_log_owner0_fleets["input"]),
    ])
    lines.extend("    {%s}," % c_array(
        list(row["order_body"]) + [0] * (18 - len(row["order_body"])))
                 for row in player_log_owner0_fleets["input"])
    lines.extend([
        "};",
        "static const StarsU16 retained_hst_owner0_input_ids[%d] = {%s};" %
        (len(player_log_owner0_fleets["input"]), c_array([
            row["fleet"]["id"] for row in player_log_owner0_fleets["input"]])),
        "static const StarsU16 retained_hst_owner0_input_id_player_bits[%d] = {%s};" %
        (len(player_log_owner0_fleets["input"]), c_array([
            row["fleet"]["id_player_bits"]
            for row in player_log_owner0_fleets["input"]])),
        "static const StarsU16 retained_hst_owner0_input_planets[%d] = {%s};" %
        (len(player_log_owner0_fleets["input"]), c_array([
            row["fleet"]["planet"] for row in player_log_owner0_fleets["input"]])),
        "static const StarsI16 retained_hst_owner0_input_points[%d][2] = {" %
        len(player_log_owner0_fleets["input"]),
    ])
    lines.extend("    {%d, %d}," % tuple(row["fleet"]["point"])
                 for row in player_log_owner0_fleets["input"])
    lines.extend([
        "};",
        "static const StarsU16 retained_hst_owner0_input_flags[%d] = {%s};" %
        (len(player_log_owner0_fleets["input"]), c_array([
            row["fleet"]["saved_flags"]
            for row in player_log_owner0_fleets["input"]])),
        "static const StarsI16 retained_hst_owner0_input_ship_counts[%d][16] = {" %
        len(player_log_owner0_fleets["input"]),
    ])
    lines.extend("    {%s}," % c_array(row["fleet"]["ship_counts"])
                 for row in player_log_owner0_fleets["input"])
    lines.extend([
        "};",
        "static const StarsU32 retained_hst_owner0_input_cargo[%d][5] = {" %
        len(player_log_owner0_fleets["input"]),
    ])
    lines.extend("    {%s}," % c_array(row["fleet"]["cargo"])
                 for row in player_log_owner0_fleets["input"])
    lines.extend([
        "};",
        "static const StarsU8 retained_hst_owner0_input_design_plan[%d] = {%s};" %
        (len(player_log_owner0_fleets["input"]), c_array([
            row["fleet"]["design_plan"]
            for row in player_log_owner0_fleets["input"]])),
        "static const StarsU8 retained_hst_owner0_input_orders[%d][18] = {" %
        len(player_log_owner0_fleets["input"]),
    ])
    lines.extend("    {%s}," % c_array(
        list(row["order_body"]) + [0] * (18 - len(row["order_body"])))
                 for row in player_log_owner0_fleets["input"])
    for run in ("control", "action"):
        rows = player_log_owner0_fleets[run]
        lines.extend([
            "};",
            "static const StarsU16 retained_hst_owner0_output_ids_%s[%d] = {%s};" %
            (run, len(rows), c_array([row["fleet"]["id"] for row in rows])),
            "static const StarsU16 retained_hst_owner0_output_lengths_%s[%d] = {%s};" %
            (run, len(rows), c_array([len(row["body"]) for row in rows])),
            "static const StarsU8 retained_hst_owner0_output_bodies_%s[%d][64] = {" %
            (run, len(rows)),
        ])
        lines.extend("    {%s}," % c_array(
            list(row["body"]) + [0] * (64 - len(row["body"])))
                     for row in rows)
        lines.extend([
            "};",
            "static const StarsU16 retained_hst_owner0_order_lengths_%s[%d] = {%s};" %
            (run, len(rows), c_array([len(row["order_body"]) for row in rows])),
            "static const StarsU8 retained_hst_owner0_order_bodies_%s[%d][18] = {" %
            (run, len(rows)),
        ])
        lines.extend("    {%s}," % c_array(
            list(row["order_body"]) + [0] * (18 - len(row["order_body"])))
                     for row in rows)
    lines.extend([
        "};",
        "static const StarsU8 retained_turn_input_planet_bodies[2][33] = {",
    ])
    lines.extend("    {%s}," % c_array(body)
                 for body in turn_input_planet_bodies)
    lines.extend([
        "};",
        "static const StarsU8 retained_turn_output_planet_bodies_control[2][37] = {",
    ])
    lines.extend("    {%s}," % c_array(
        guess_output_planet_bodies["control"][planet_id])
                 for planet_id in (10, 24))
    lines.extend([
        "};",
        "static const StarsU8 retained_turn_output_planet_bodies_action[2][37] = {",
    ])
    lines.extend("    {%s}," % c_array(
        guess_output_planet_bodies["action"][planet_id])
                 for planet_id in (10, 24))
    lines.extend([
        "};",
        "static const StarsU32 retained_turn_output_planet_minerals_control[2][4] = {",
    ])
    lines.extend("    {%s}," % c_array(state["minerals"])
                 for state in turn_output_planet_states["control"])
    lines.extend([
        "};",
        "static const StarsU32 retained_turn_output_planet_minerals_action[2][4] = {",
    ])
    lines.extend("    {%s}," % c_array(state["minerals"])
                 for state in turn_output_planet_states["action"])
    lines.extend([
        "};",
        "static const StarsU8 retained_turn_output_planet_levels[2][3] = {",
    ])
    lines.extend("    {%s}," % c_array(state["levels"])
                 for state in turn_output_planet_states["control"])
    lines.extend([
        "};",
        "static const StarsU16 retained_guess_words_control[2] = {%s};" %
        c_array([row["output_guess"] for row in guess_planet_rows["control"]]),
        "static const StarsU16 retained_guess_words_action[2] = {%s};" %
        c_array([row["output_guess"] for row in guess_planet_rows["action"]]),
        "static const StarsI16 retained_mining_values_control[6] = {%s};" %
        c_array(control_mining["values"]),
        "static const StarsI16 retained_mining_values_action[6] = {%s};" %
        c_array(action_mining["values"]),
        "static const StarsI32 retained_mining_outputs_control[2][3] = {%s};" %
        ",".join("{%s}" % c_array(row) for row in control_mining_yields),
        "static const StarsI32 retained_mining_outputs_action[2][3] = {%s};" %
        ",".join("{%s}" % c_array(row) for row in action_mining_yields),
        "static const StarsI16 retained_random_event_limits[3] = {%s};" %
        c_array(control_events["limits"]),
        "static const StarsI16 retained_random_event_values_control[3] = {%s};" %
        c_array(control_events["values"]),
        "static const StarsI16 retained_random_event_values_action[3] = {%s};" %
        c_array(action_events["values"]),
        "static const StarsI16 retained_tech_limits_control[%d] = {%s};" %
        (len(control_tech["limits"]), c_array(control_tech["limits"])),
        "static const StarsI16 retained_tech_values_control[%d] = {%s};" %
        (len(control_tech["values"]), c_array(control_tech["values"])),
        "static const StarsI16 retained_tech_limits_action[%d] = {%s};" %
        (len(action_tech["limits"]), c_array(action_tech["limits"])),
        "static const StarsI16 retained_tech_values_action[%d] = {%s};" %
        (len(action_tech["values"]), c_array(action_tech["values"])),
        "#define TURIN_SUPPLY_PLANET_START %d" %
        input_planet["minerals"][3],
        "#define TURIN_LOCAL_TRANSPORT_PLANET_POPULATION %d" %
        input_planet["minerals"][3],
        "#define TURIN_SUPPLY_FLEET_START %d" %
        input_supply_fleet["cargo"][3],
        "#define TURIN_PLAYER_IDEAL_GROWTH %d" % player_record[25],
        "#define TURIN_PLAYER_GROWTH_STAT %d" % player_record[76],
        "#define TURIN_PLAYER_RACE_GRBIT_9 %d" %
        ((int.from_bytes(player_record[78:82], "little") >> 9) & 1),
        "#define TURIN_PLANET10_PLAYER_IDEAL_GROWTH %d" %
        planet10_player[25],
        "#define TURIN_PLANET10_PLAYER_GROWTH_STAT %d" %
        planet10_player[76],
        "#define TURIN_PLANET10_PLAYER_RACE_GRBIT_9 %d" %
        ((int.from_bytes(planet10_player[78:82], "little") >> 9) & 1),
        "#define TURIN_PLANET10_POPULATION_START %d" %
        planet10_input["minerals"][3],
        "#define TURIN_PLANET10_FRACTION_START %d" %
        planet10_input["improvements"]["population_delta"],
        "#define TURIN_PLANET10_POPULATION_EXPECTED %d" %
        planet10_output_states["control"]["minerals"][3],
        "#define TURIN_PLANET10_FRACTION_EXPECTED %d" %
        planet10_output_states["control"]["improvements"][
            "population_delta"],
        "#define TURIN_POPULATION_FRACTION_START %d" %
        input_planet["improvements"]["population_delta"],
        "#define TURIN_ORIGINAL_PLANET_COLONISTS %d" %
        output_planet["minerals"][3],
        "#define TURIN_ORIGINAL_POPULATION_FRACTION %d" %
        output_planet["improvements"]["population_delta"],
        "#define TURIN_RETAINED_PLANET_HEADER_FLAGS 0x%04xU" %
        ((planet_record["kind"] << 10) | planet_record["length"]),
        "#define TURIN_RETAINED_PLANET_TURN %d" % planet_record["turn"],
        "static const StarsU8 retained_player_environment[9] = {%s};" %
        c_array(list(player_record[16:25])),
        "static const StarsU8 retained_guess_player_environments[2][9] = {",
        "    {%s}," % c_array(list(mining_players[0][16:25])),
        "    {%s}," % c_array(list(mining_players[1][16:25])),
        "};",
        "static const signed char retained_player_technology[6] = {%s};" %
        c_array(list(player_record[26:32])),
        "static const StarsU8 retained_startup_player_technology[6] = {%s};" %
        c_array(list(input_hst_player[26:32])),
        "static const StarsU8 retained_turin_research_requirements[31] = {%s};" %
        c_array(list(research_requirements)),
        "static const StarsU16 retained_startup_name_ids_control[4] = {%s};" %
        c_array(seed_binding["control"]["name_string_ids"]),
        "static const StarsU16 retained_startup_name_ids_action[4] = {%s};" %
        c_array(seed_binding["action"]["name_string_ids"]),
        "static const StarsU32 retained_player_race_bits = 0x%08xUL;" %
        struct.unpack_from("<I", player_record, 78)[0],
        "static const StarsI16 retained_player_race_stat = %d;" %
        player_record[76],
        "static const StarsU8 retained_shdef_record[%d] = {%s};" %
        (len(shdef_record), c_array(shdef_record)),
        "#define TURIN_M2_SHDEF_INPUT_COUNT %dU" % len(player_log_shdefs),
        "#define TURIN_M2_SHDEF_DECODABLE_COUNT %dU" %
        sum(record["body"][2] <= 36 for record in player_log_shdefs),
        "static const StarsU16 retained_m2_shdef_input_ids[%d] = {%s};" %
        (len(player_log_shdefs), c_array([
            (int.from_bytes(record["body"][:2], "little") >> 10) & 31
            for record in player_log_shdefs])),
        "static const StarsU16 retained_m2_shdef_input_lengths[%d] = {%s};" %
        (len(player_log_shdefs), c_array([
            len(record["body"]) for record in player_log_shdefs])),
        "static const StarsU8 retained_m2_shdef_input_bodies[%d][80] = {" %
        len(player_log_shdefs),
    ])
    lines.extend("    {%s}," % c_array(
        list(record["body"]) + [0] * (80 - len(record["body"])))
                 for record in player_log_shdefs)
    lines.extend([
        "};",
        "static const StarsU8 retained_score_history_control[2][24] = {",
        "    {%s}," % c_array(score_history_records["control"][0]),
        "    {%s}," % c_array(score_history_records["control"][1]),
        "};",
        "static const StarsU8 retained_score_history_action[2][24] = {",
        "    {%s}," % c_array(score_history_records["action"][0]),
        "    {%s}," % c_array(score_history_records["action"][1]),
        "};",
        "static const StarsU8 retained_planet_body[%d] = {%s};" %
        (len(planet_record["body"]), c_array(planet_record["body"])),
        "static const StarsU8 retained_resource_planet24[56] = {%s};" %
        c_array(resource_planet24),
        "static const StarsU32 original_research_costs[27] = {%s};" % c_array(research_costs),
        "static const StarsU8 expected_research_players[2][2][36] = {%s};" %
        ",".join("{%s}" % ",".join("{%s}" % c_array(row) for row in pair)
                 for pair in research_expected),
        "static const StarsU16 expected_startup_damage[2][4] = {%s};" %
        ",".join("{%s}" % c_array(row) for row in expected_startup_damage),
        "static const StarsU8 expected_startup_names[2][4][32] = {%s};" %
        ",".join("{%s}" % ",".join("{%s}" % c_array(row) for row in pair)
                 for pair in expected_startup_names),
        "static const StarsU8 expected_startup_components[2][4][64] = {%s};" %
        ",".join("{%s}" % ",".join("{%s}" % c_array(row) for row in pair)
                 for pair in expected_startup_components),
        "static const StarsU16 expected_history_types[2][4] = {%s};" %
        ",".join("{%s}" % c_array([r["kind"] for r in pair]) for pair in expected_history),
        "static const StarsU16 expected_history_sizes[2][4] = {%s};" %
        ",".join("{%s}" % c_array([len(r["body"]) for r in pair]) for pair in expected_history),
        "static const StarsU8 expected_history_bodies[2][4][128] = {%s};" %
        ",".join("{%s}" % ",".join("{%s}" % c_array(list(r["body"]) + [0] * (128-len(r["body"]))) for r in pair)
                 for pair in expected_history),
        "static const StarsU16 expected_design_logs_types[2][4] = {%s};" %
        ",".join("{%s}" % c_array([r["kind"] for r in pair]) for pair in expected_design_logs),
        "static const StarsU16 expected_design_logs_sizes[2][4] = {%s};" %
        ",".join("{%s}" % c_array([len(r["body"]) for r in pair]) for pair in expected_design_logs),
        "static const StarsU8 expected_design_logs_bodies[2][4][128] = {%s};" %
        ",".join("{%s}" % ",".join("{%s}" % c_array(list(r["body"]) + [0] * (128-len(r["body"]))) for r in pair)
                 for pair in expected_design_logs),
        "static const StarsU8 expected_research_log[2][2] = {%s};" %
        ",".join("{%s}" % c_array(row) for row in expected_research_log),
        "static const StarsU8 expected_production_log[2][10] = {%s};" %
        ",".join("{%s}" % c_array(row) for row in expected_production_log),
        "static const StarsU8 expected_balance_log[2][9] = {%s};" %
        ",".join("{%s}" % c_array(row) for row in expected_balance_log),
        "static const StarsU8 expected_cargo_log[2][7] = {%s};" %
        ",".join("{%s}" % c_array(row) for row in expected_cargo_log),
        "static const StarsU8 expected_order_logs[2][2][12] = {%s};" %
        ",".join("{%s}" % ",".join("{%s}" % c_array(row) for row in pair) for pair in expected_order_logs),
        "static const StarsU8 original_dgroup[65536] = {%s};" % c_array(inspector.initial),
        "static const StarsU8 original_string_segment[%d] = {%s};" %
        (len(NE(STARS_EXE).raw[2]), c_array(NE(STARS_EXE).raw[2])),
        "static const StarsU8 original_ai_part_counts[39] = {%s};" %
        c_array(inspector.initial[0x1450:0x1450 + 39]),
        "static const StarsU16 original_ai_part_choices[125] = {%s};" %
        c_array(struct.unpack("<125H", bytes(NE(STARS_EXE).raw[18])[:250])),
        "static const StarsU8 original_sb_templates[85] = {%s};" %
        c_array(bytes(NE(STARS_EXE).raw[18])[0x7688:0x7688 + 85]),
        "static const StarsU8 retained_part_segment[%d] = {%s};" %
        (len(part_segment), c_array(part_segment)),
        "static const StarsU8 retained_planet_environment[3] = {%s};" %
        c_array(input_planet["environment"]),
        "static const StarsI16 retained_cresource_player_stats[15] = {%s};" %
        c_array([int(value) for value in
                 mining_players[1][0x3e:0x3e + 15]]),
        "#define TURIN_RETAINED_INPUT_CHEATER %d" %
        ((input_hst_player[84] >> 2) & 1),
        "static const StarsU8 retained_planet10_environment[3] = {%s};" %
        c_array(planet10_input["environment"]),
        "static const StarsU8 retained_ai_planet_status[TURIN_RETAINED_POINT_COUNT] = {",
        "    %s" % c_array(ai_planet_status),
        "};",
        "static const StarsU8 retained_wormholes[TURIN_RETAINED_THING_COUNT][18] = {",
    ])
    lines.extend("    {%s}," % c_array(thing) for thing in wormholes)
    lines.extend([
        "};",
        "#define TURIN_RETAINED_WORMHOLE_DRAW_COUNT 12",
        "static const StarsI16 retained_wormhole_draw_limits_control[12] = {%s};" %
        c_array(control_wormhole_moves["limits"]),
        "static const StarsI16 retained_wormhole_draw_values_control[12] = {%s};" %
        c_array(control_wormhole_moves["values"]),
        "static const StarsI16 retained_wormhole_draw_limits_action[12] = {%s};" %
        c_array(action_wormhole_moves["limits"]),
        "static const StarsI16 retained_wormhole_draw_values_action[12] = {%s};" %
        c_array(action_wormhole_moves["values"]),
        "static const StarsU8 retained_wormhole_output_control[4][18] = {",
    ])
    lines.extend("    {%s}," % c_array(record)
                 for record in wormhole_outputs["control"])
    lines.extend([
        "};",
        "static const StarsU8 retained_wormhole_output_action[4][18] = {",
    ])
    lines.extend("    {%s}," % c_array(record)
                 for record in wormhole_outputs["action"])
    lines.append("};")
    lines.extend([
        "static const StarsPoint retained_points[TURIN_RETAINED_POINT_COUNT] = {",
    ])
    lines.extend("    {%d, %d}," % point for point in points)
    lines.extend([
        "};",
        "#define TURIN_RETAINED_OWNED_PLANET_COUNT %dU" %
        sum(owner >= 0 for owner in retained_planet_owners),
        "static const StarsI16 retained_planet_owners[32] = {%s};" %
        c_array(retained_planet_owners),
        "static const StarsI32 retained_fleet_cargo[TURIN_RETAINED_FLEET_COUNT][5] = {",
    ])
    for fleet in fleet_rows:
        cargo = list(fleet["cargo"][:5])
        cargo.extend([0] * (5 - len(cargo)))
        lines.append("    {%s}," % c_array(cargo))
    lines.extend([
        "};",
        "static const StarsU8 retained_raw_fleets[%d][124] = {" %
        len(fleet_rows),
    ])
    for fleet in fleet_rows:
        raw = bytearray(124)
        struct.pack_into("<HHHhhh", raw, 0, fleet["id"], fleet["owner"],
                         fleet["saved_flags"], fleet["planet"],
                         fleet["point"][0], fleet["point"][1])
        for index, count in enumerate(fleet["ship_counts"]):
            struct.pack_into("<H", raw, 12 + index * 2, count)
        for index, amount in enumerate(fleet["cargo"]):
            struct.pack_into("<I", raw, 76 + index * 4, amount)
        raw[98] = fleet["order_count"]
        lines.append("    {%s}," % c_array(raw))
    lines.extend([
        "};",
        "static const StarsI16 retained_fleet_ids[TURIN_RETAINED_FLEET_COUNT] = {",
        "    %s" % c_array([fleet["id"] for fleet in fleet_rows]),
        "};",
        "static const StarsI16 retained_fleet_owners[TURIN_RETAINED_FLEET_COUNT] = {",
        "    %s" % c_array([fleet["owner"] for fleet in fleet_rows]),
        "};",
        "static const StarsI16 retained_fleet_planets[TURIN_RETAINED_FLEET_COUNT] = {",
        "    %s" % c_array([fleet["planet"] for fleet in fleet_rows]),
        "};",
        "static const StarsI16 retained_ship_counts[TURIN_RETAINED_FLEET_COUNT][16] = {",
    ])
    lines.extend("    {%s}," % c_array(fleet["ship_counts"])
                 for fleet in fleet_rows)
    lines.extend([
        "};",
        "static const StarsI16 retained_order_points[TURIN_RETAINED_FLEET_COUNT][2] = {",
    ])
    lines.extend("    {%d, %d}," % tuple(order["point"])
                 for order in order_rows)
    lines.extend([
        "};",
        "static const StarsU16 retained_order_targets[TURIN_RETAINED_FLEET_COUNT] = {",
        "    %s" % c_array([order["target_id"] for order in order_rows]),
        "};",
        "static const StarsU16 retained_order_flags[TURIN_RETAINED_FLEET_COUNT] = {",
        "    %s" % c_array([order["flags"] for order in order_rows]),
        "};",
        "static const StarsI16 retained_source_fuel = %d;" %
        fleet_rows[0]["cargo"][4],
        "static const StarsI16 retained_expected_target = %d;" %
        expected_order["target_id"],
    ])
    lines.append("static const StarsU8 retained_input_decode_prefix[8192] = {%s};" %
                 c_array(inspector.initial[:8192]))
    message_counts = bytes(NE(STARS_EXE).raw[6])[0x5b0e:0x5b0e + 387]
    assert len(message_counts) == 387 and max(message_counts) <= 7
    lines.append("static const StarsU8 original_message_counts[387] = {%s};" %
                 c_array(message_counts))
    inherited_message_bodies = [b"".join(r.body for r in input_corpus[label]["M1"].records
                                        if r.kind == 12) for label in ("control", "action")]
    lines.append("static const StarsU16 input_m1_message_sizes[2] = {%s};" %
                 c_array([len(b) for b in inherited_message_bodies]))
    lines.append("static const StarsU8 input_m1_messages[2][1024] = {" +
                 ",".join("{%s}" % c_array(b or b"\0") for b in inherited_message_bodies) + "};")
    expected_message_bodies = []
    for run_name in ("vmctl-sye3k0", "vmctl-sye3k1"):
        rows = []
        for player in (1, 2):
            rows.append(b"".join(r["body"] for r in decode_stream(
                ROOT / "runs" / run_name / ("CLEAN.M%d" % player), inspector)
                if r["kind"] == 12))
        expected_message_bodies.append(rows)
    lines.append("static const StarsU16 expected_turn_message_sizes[2][2] = {" +
                 ",".join("{%s}" % c_array([len(b) for b in rows]) for rows in expected_message_bodies) + "};")
    lines.append("static const StarsU8 expected_turn_messages[2][2][1024] = {" +
                 ",".join("{" + ",".join("{%s}" % c_array(b or b"\0") for b in rows) + "}" for rows in expected_message_bodies) + "};")
    expected_designs = []
    for run_name in ("vmctl-sye3k0", "vmctl-sye3k1"):
        files = []
        for suffix in ("M1", "M2", "HST"):
            rows = [r["body"] for r in decode_stream(ROOT / "runs" / run_name / ("CLEAN." + suffix), inspector)
                    if r["kind"] == 26 and r["turn"] == 1]
            assert len(rows) <= 52 and all(len(row) <= 160 for row in rows)
            files.append(rows)
        expected_designs.append(files)
    lines.append("static const StarsU16 expected_design_counts[2][3] = {" +
                 ",".join("{%s}" % c_array([len(rows) for rows in files]) for files in expected_designs) + "};")
    lines.append("static const StarsU16 expected_design_sizes[2][3][52] = {" +
                 ",".join("{" + ",".join("{%s}" % c_array([len(row) for row in rows]) for rows in files) + "}" for files in expected_designs) + "};")
    lines.append("static const StarsU8 expected_design_bodies[2][3][52][160] = {" +
                 ",".join("{" + ",".join("{" + ",".join("{%s}" % c_array(row) for row in rows) + "}" for rows in files) + "}" for files in expected_designs) + "};")
    input_m1_headers = []
    for run_name in ("vmctl-sye3k0", "vmctl-sye3k1"):
        run = json.loads((ROOT / "runs" / run_name / "RUN.JSON").read_text())
        assert "CLEAN.X1" not in run["input_files"] and "CLEAN.X2" not in run["input_files"]
        input_m1_path = ROOT / "runs" / run_name / "BACKUP/CLEAN.M1"
        raw = input_m1_path.read_bytes()
        assert hashlib.sha256(raw).hexdigest() == run["input_files"]["CLEAN.M1"]["sha256"]
        input_m1_headers.append(raw[:18])
    lines.append("static const StarsU8 input_m1_headers[2][18] = {" +
                 ",".join("{%s}" % c_array(row) for row in input_m1_headers) + "};")
    expected_plans = []
    for run_name in ("vmctl-sye3k0", "vmctl-sye3k1"):
        files = []
        for suffix in ("M1", "M2", "HST"):
            rows = [r["body"] for r in decode_stream(ROOT / "runs" / run_name / ("CLEAN." + suffix), inspector)
                    if r["kind"] == 30 and r["turn"] == 1]
            assert len(rows) <= 32 and all(len(row) <= 38 for row in rows)
            files.append(rows)
        expected_plans.append(files)
    lines.append("static const StarsU16 expected_plan_counts[2][3] = {" +
                 ",".join("{%s}" % c_array([len(rows) for rows in files]) for files in expected_plans) + "};")
    lines.append("static const StarsU16 expected_plan_sizes[2][3][32] = {" +
                 ",".join("{" + ",".join("{%s}" % c_array([len(row) for row in rows]) for rows in files) + "}" for files in expected_plans) + "};")
    lines.append("static const StarsU8 expected_plan_bodies[2][3][32][38] = {" +
                 ",".join("{" + ",".join("{" + ",".join("{%s}" % c_array(row) for row in rows) + "}" for rows in files) + "}" for files in expected_plans) + "};")
    ini_path = ROOT / "runs/step4/guest98/WINDOWS/STARS.INI"
    ini_raw = ini_path.read_bytes()
    assert hashlib.sha256(ini_raw).hexdigest() == "1d5890a30f0bdb846f70862aeab88786e9945e9c54c5a97da9b26dc150edc2cc"
    settings = next(line.split(b"=", 1)[1].strip() for line in ini_raw.splitlines() if line.startswith(b"GlobalSettings="))
    assert len(settings) == 28
    shuffle = bytes(NE(ROOT / "target/stars.exe").raw[4][0x2870:0x2885])
    assert sorted(shuffle) == list(range(21))
    lines.append("static const StarsU8 replay_environment_ini[29] = {%s};" % c_array(settings + b"\0"))
    lines.append("static const StarsU8 replay_environment_shuffle[21] = {%s};" % c_array(shuffle))
    disk_audit = json.loads((ROOT / "analysis/compat/satisfy-orders-e3-disk-candidate-audit.json").read_text())
    disk = next(row for row in disk_audit["candidates"] if row["path"] == "runs/step4/win98.img")
    assert not disk["volume_label_entry_present"]
    lines.append("#define REPLAY_DISK_CAPACITY_NIBBLE %dU" % disk["capacity_nibble"])
    log_headers = [next(r["body"] for r in decode_stream(ROOT / "runs" / name / "BACKUP/CLEAN.X2", inspector) if r["kind"] == 9)
                   for name in ("vmctl-sye3k0", "vmctl-sye3k1")]
    lines.append("static const StarsU8 expected_log_headers[2][17] = {" + ",".join("{%s}" % c_array(row) for row in log_headers) + "};")
    expected_host_players = []
    for run_name in ("vmctl-sye3k0", "vmctl-sye3k1"):
        rows = [r["body"] for r in decode_stream(ROOT / "runs" / run_name / "CLEAN.HST", inspector)
                if r["kind"] == 6]
        assert len(rows) == 2 and [r[0] for r in rows] == [0, 1]
        expected_host_players.append(rows)
    lines.append("static const StarsU16 expected_host_player_sizes[2][2] = {" +
                 ",".join("{%s}" % c_array([len(b) for b in rows]) for rows in expected_host_players) + "};")
    lines.append("static const StarsU8 expected_host_players[2][2][266] = {" +
                 ",".join("{" + ",".join("{%s}" % c_array(b) for b in rows) + "}" for rows in expected_host_players) + "};")
    expected_m2_players = [retained_player_record(path, 1, inspector)
                           for path in (OUTPUT_M2, ACTION_OUTPUT_M2)]
    lines.append("static const StarsU16 expected_m2_player_sizes[2] = {%s};" %
                 c_array([len(b) for b in expected_m2_players]))
    lines.append("static const StarsU8 expected_m2_players[2][266] = {" +
                 ",".join("{%s}" % c_array(b) for b in expected_m2_players) + "};")
    expected_m1_players = []
    for run_name in ("vmctl-sye3k0", "vmctl-sye3k1"):
        rows = [r["body"] for r in decode_stream(ROOT / "runs" / run_name / "CLEAN.M1", inspector)
                if r["kind"] == 6 and r["turn"] == 1 and r["body"][0] == 0]
        assert len(rows) == 1
        expected_m1_players.append(rows[0])
    lines.append("static const StarsU16 expected_m1_player_sizes[2] = {%s};" %
                 c_array([len(b) for b in expected_m1_players]))
    lines.append("static const StarsU8 expected_m1_players[2][266] = {" +
                 ",".join("{%s}" % c_array(b) for b in expected_m1_players) + "};")
    input_player_rows = []
    for label in ("control", "action"):
        files = input_corpus[label]
        rows = [(files["HST"], 0), (files["HST"], 1),
                (files["M1"], 0), (files["M2"], 1)]
        input_player_rows.append([
            (next(r.body for r in f.records if r.kind == 6 and r.body[0] == player),
             f.players[player]) for f, player in rows])
    for name, width, part in (("bodies", 160, 0), ("expected", 192, 1)):
        lines.append("static const StarsU8 retained_input_player_%s[2][4][%d] = {" %
                     (name, width))
        for rows in input_player_rows:
            if any(len(row[part]) > width for row in rows):
                raise ValueError("input player exceeds storage capacity")
            lines.append("{" + ",".join("{" + c_array(row[part]) + "}"
                                         for row in rows) + "},")
        lines.append("};")
    lines.append("static const StarsU16 retained_input_player_sizes[2][4] = {" +
                 ",".join("{" + c_array([len(row[0]) for row in rows]) + "}"
                          for rows in input_player_rows) + "};")
    for name, suffix, kind, width in (("game", "XY", 7, 64),
                                      ("bof", "HST", 8, 16)):
        bodies = [next(r.body for r in input_corpus[label][suffix].records
                       if r.kind == kind) for label in ("control", "action")]
        lines.append("static const StarsU8 retained_input_%s[2][%d] = {" % (name, width) +
                     ",".join("{" + c_array(body) + "}" for body in bodies) + "};")
    lines.append(complete_input_header(input_corpus))
    history_files = [(directory / "CLEAN.H2").read_bytes()
                     for directory in (OUTPUT_HST.parent, ACTION_OUTPUT_HST.parent)]
    if any(len(raw) > 4096 for raw in history_files):
        raise ValueError("history output exceeds bounded capacity")
    lines.append("static const StarsU8 expected_history_files[2][4096] = {" +
                 ",".join("{" + c_array(raw) + "}" for raw in history_files) + "};")
    lines.append("static const StarsU16 expected_history_file_sizes[2] = {" +
                 c_array([len(raw) for raw in history_files]) + "};")
    full_rng = [[[limit, value] for phase in rng_phases[label]
                 for limit, value in zip(phase["limits"], phase["values"])]
                for label in ("control", "action")]
    if [len(row) for row in full_rng] != [57, 39]:
        raise ValueError("incomplete expected RNG transcript")
    lines.append("static const StarsU16 expected_main_rng_count[2] = {57,39};")
    lines.append("static const StarsI16 expected_main_rng[2][57][2] = {" +
                 ",".join("{" + ",".join("{" + c_array(pair) + "}" for pair in row) + "}"
                          for row in full_rng) + "};")

    startup_bofs = []
    startup_ticks = []
    for label, output_dir in (("control", OUTPUT_HST.parent), ("action", ACTION_OUTPUT_HST.parent)):
        startup_bofs.append([(output_dir / filename).read_bytes()[:18]
                             for filename in ("BACKUP/CLEAN.X2", "CLEAN.H2")])
        startup_ticks.append(writer_oracle["derived_gettickcount_low11_by_write"][label][:2])
        if any(len(raw) != 18 or raw[:2] != b'\x10\x20' for raw in startup_bofs[-1]):
            raise ValueError("invalid startup BOF reference")
    lines.append("static const StarsU32 replay_startup_ticks[2][2] = {" +
                 ",".join("{" + c_array(row) + "}" for row in startup_ticks) + "};")
    lines.append("static const StarsU8 expected_startup_bofs[2][2][18] = {" +
                 ",".join("{" + ",".join("{" + c_array(raw) + "}" for raw in row) + "}"
                          for row in startup_bofs) + "};")
    starting_files = [[input_corpus[label][suffix].path.read_bytes()
                       for suffix in ("HST", "M1", "M2")]
                      for label in ("control", "action")]
    if any(len(raw) > 4096 for row in starting_files for raw in row):
        raise ValueError("starting file exceeds memory file capacity")
    lines.append("static const StarsU8 input_file_bytes[2][3][4096] = {" +
                 ",".join("{" + ",".join("{" + c_array(raw) + "}" for raw in row) + "}"
                          for row in starting_files) + "};")
    lines.append("static const StarsU16 input_file_sizes[2][3] = {" +
                 ",".join("{" + c_array([len(raw) for raw in row]) + "}"
                          for row in starting_files) + "};")
    rototill_input_files = [
        (ROOT / "runs/rototill-original-turn-r1/input" /
         f"CLEAN.{suffix}").read_bytes()
        for suffix in ("HST", "M1", "M2")
    ]
    rototill_backup_files = [
        (ROOT / "runs/rototill-original-turn-r1/backup" /
         f"CLEAN.{suffix}").read_bytes()
        for suffix in ("HST", "M1", "M2")
    ]
    if any(len(raw) > 4096 for raw in rototill_input_files + rototill_backup_files):
        raise ValueError("Rototill file exceeds memory file capacity")
    lines.append("static const StarsU8 rototill_input_file_bytes[3][4096] = {" +
                 ",".join("{" + c_array(raw) + "}" for raw in rototill_input_files) + "};")
    lines.append("static const StarsU16 rototill_input_file_sizes[3] = {" +
                 c_array([len(raw) for raw in rototill_input_files]) + "};")
    lines.append("static const StarsU8 rototill_backup_file_bytes[3][4096] = {" +
                 ",".join("{" + c_array(raw) + "}" for raw in rototill_backup_files) + "};")
    lines.append("static const StarsU16 rototill_backup_file_sizes[3] = {" +
                 c_array([len(raw) for raw in rototill_backup_files]) + "};")
    input_salts = []
    for label in ("control", "action"):
        for suffix in ("HST", "M1", "M2"):
            if any(r.kind in (31, 39) for r in input_corpus[label][suffix].records):
                raise ValueError("battle input requires a runtime battle-log loader")
        salt_records = [r.body for r in input_corpus[label]["HST"].records if r.kind == 36]
        if len(salt_records) > 1 or any(len(body) != 4 for body in salt_records):
            raise ValueError("invalid starting host salt section")
        input_salts.append(int.from_bytes(salt_records[0], "little") if salt_records else 0)
    lines.append("static const StarsU32 input_host_salts[2] = {" +
                 ",".join(str(value) + "UL" for value in input_salts) + "};")
    path.write_text("\n".join(lines) + "\n", encoding="ascii")


def owner_planet_record(records, turn, planet_id):
    matches = [decode_planet_record(record["body"], planet_id)
               for record in records
               if record["kind"] == 13 and record["turn"] == turn and
               (int.from_bytes(record["body"][:2], "little") & 0x7ff) ==
               planet_id]
    if len(matches) != 1:
        raise AssertionError("expected exactly one retained planet record")
    return matches[0]


def write_supply_fixture_header(path, input_pairs, control_pairs,
                                action_pairs, input_planet,
                                control_planet, action_planet):
    input_fleet = next(pair["fleet"] for pair in input_pairs
                       if pair["fleet"]["id"] == 1)
    control_fleet = next(pair["fleet"] for pair in control_pairs
                         if pair["fleet"]["id"] == 1)
    action_fleet = next(pair["fleet"] for pair in action_pairs
                        if pair["fleet"]["id"] == 1)
    input_colonists = input_planet["minerals"][3]
    observed_transfer = control_fleet["cargo"][3] - input_fleet["cargo"][3]
    if (action_fleet["cargo"][3] - input_fleet["cargo"][3] !=
            observed_transfer):
        raise AssertionError("control/action fleet supply deltas differ")
    if control_planet["minerals"][3] != action_planet["minerals"][3]:
        raise AssertionError("control/action planet colonists differ")
    if observed_transfer <= 0 or input_colonists < observed_transfer:
        raise AssertionError("retained supply transfer is not source-backed")
    lines = [
        "#define TURIN_PLANET_ID 24",
        "#define TURIN_FLEET_ID 1",
        "#define TURIN_INPUT_PLANET_COLONISTS %d" % input_colonists,
        "#define TURIN_INPUT_FLEET_COLONISTS %d" % input_fleet["cargo"][3],
        "#define TURIN_ORIGINAL_FLEET_COLONIST_GAIN %d" % observed_transfer,
        "#define TURIN_ORIGINAL_PLANET_COLONIST_NET %d" %
        (control_planet["minerals"][3] - input_colonists),
    ]
    path.write_text("\n".join(lines) + "\n", encoding="ascii")


class SatisfyOrdersTurinBindingTests(unittest.TestCase):
    def test_retained_selector_one_scout_order_and_movement(self):
        pair = json.loads(PAIR.read_text(encoding="utf-8"))
        inspector = Inspector()
        self.assertEqual(hashlib.sha256(INPUT_HST.read_bytes()).hexdigest(),
                         INPUT_HST_SHA256)
        input_records = decode_stream(INPUT_M2, inspector)
        output_records = decode_stream(OUTPUT_M2, inspector)
        action_records = decode_stream(ACTION_OUTPUT_M2, inspector)
        output_hst_records = decode_stream(OUTPUT_HST, inspector)
        action_hst_records = decode_stream(ACTION_OUTPUT_HST, inspector)
        m1_output_records = decode_stream(
            ROOT / "runs/vmctl-sye3k0/CLEAN.M1", inspector)
        action_m1_output_records = decode_stream(
            ROOT / "runs/vmctl-sye3k1/CLEAN.M1", inspector)
        production_queue_body = planet_production_queue_record(
            output_records, 1, 24)["body"]
        retained_queue_bodies = [
            production_queue_body,
            planet_production_queue_record(action_records, 1, 24)["body"],
            planet_production_queue_record(output_hst_records, 1, 24)["body"],
            planet_production_queue_record(action_hst_records, 1, 24)["body"],
        ]
        self.assertEqual(len(production_queue_body), 8)
        self.assertEqual(retained_queue_bodies,
                         [production_queue_body] * 4)
        output_files = (
            ROOT / "runs/vmctl-sye3k0/CLEAN.M1", OUTPUT_M2, OUTPUT_HST,
            ROOT / "runs/vmctl-sye3k1/CLEAN.M1", ACTION_OUTPUT_M2,
            ACTION_OUTPUT_HST,
        )
        for output_file in output_files:
            with self.subTest(output_turn_marker=str(output_file)):
                markers = [record for record in decode_stream(output_file,
                                                             inspector)
                           if record["turn"] == 1 and record["kind"] == 0]
                self.assertEqual(len(markers), 1)
                self.assertEqual(markers[0]["body"], b"\x01\x00")
        input_pairs = owner1_fleet_order_pairs(input_records, 0)
        output_pairs = owner1_fleet_order_pairs(output_records, 1)
        action_pairs = owner1_fleet_order_pairs(action_records, 1)
        input_planet = owner_planet_record(input_records, 0, 24)
        output_planet = owner_planet_record(output_records, 1, 24)
        action_planet = owner_planet_record(action_records, 1, 24)
        points = xy_points(XY_FILE, inspector)
        wormholes = hst_wormholes(INPUT_HST, inspector)
        wormhole_outputs = {
            "control": hst_wormholes(OUTPUT_HST, inspector, turn=1),
            "action": hst_wormholes(ACTION_OUTPUT_HST, inspector, turn=1),
        }
        wormhole_fleets = {
            "control": hst_fleet_points(OUTPUT_HST, inspector, turn=1),
            "action": hst_fleet_points(ACTION_OUTPUT_HST, inspector, turn=1),
        }
        full_planet_records = {
            "input": hst_planet_records(INPUT_HST, inspector, turn=0),
            "control": hst_planet_records(OUTPUT_HST, inspector, turn=1),
            "action": hst_planet_records(ACTION_OUTPUT_HST, inspector, turn=1),
        }
        player_log_planets = {
            "input_m1": player_planet_records(
                ROOT / "runs/vmctl-sye3k0/BACKUP/CLEAN.M1", inspector, 0),
            "input_m2": player_planet_records(INPUT_M2, inspector, 0),
            "control_m1": player_planet_records(
                ROOT / "runs/vmctl-sye3k0/CLEAN.M1", inspector, 1),
            "action_m1": player_planet_records(
                ROOT / "runs/vmctl-sye3k1/CLEAN.M1", inspector, 1),
            "control_m2": player_planet_records(OUTPUT_M2, inspector, 1),
            "action_m2": player_planet_records(ACTION_OUTPUT_M2, inspector, 1),
        }
        score_history_records = {
            "control": [
                retained_type45_score_record(m1_output_records, 1),
                retained_type45_score_record(output_records, 1)],
            "action": [
                retained_type45_score_record(action_m1_output_records, 1),
                retained_type45_score_record(action_records, 1)],
        }
        player_log_fleets = {
            "input": player_log_fleet_records(input_records, 0),
            "control": player_log_fleet_records(output_records, 1),
            "action": player_log_fleet_records(action_records, 1),
            "control_hst": player_log_fleet_records(
                decode_stream(OUTPUT_HST, inspector), 1),
            "action_hst": player_log_fleet_records(
                decode_stream(ACTION_OUTPUT_HST, inspector), 1),
        }
        player_log_owner0_fleets = {
            "input": player_log_fleet_records(
                decode_stream(INPUT_HST, inspector), 0, owner=0),
            "control": player_log_fleet_records(
                decode_stream(OUTPUT_HST, inspector), 1, owner=0),
            "action": player_log_fleet_records(
                decode_stream(ACTION_OUTPUT_HST, inspector), 1, owner=0),
            "control_m1": player_log_fleet_records(
                decode_stream(ROOT / "runs/vmctl-sye3k0/CLEAN.M1", inspector),
                1, owner=0),
            "action_m1": player_log_fleet_records(
                decode_stream(ROOT / "runs/vmctl-sye3k1/CLEAN.M1", inspector),
                1, owner=0),
        }
        self.assertEqual(sorted(player_log_planets["input_m1"]), [7, 10, 12])
        self.assertEqual(sorted(player_log_planets["input_m2"]), [24])
        self.assertEqual([row["fleet"]["id"] for row in
                          player_log_fleets["input"]], [0, 1, 2, 3])
        self.assertEqual([row["order_type"] for row in
                          player_log_fleets["input"]], [20, 20, 20, 20])
        self.assertTrue(all(len(row["body"]) <= 64 and
                            len(row["order_body"]) == 8
                            for row in player_log_fleets["input"]))
        self.assertEqual([row["fleet"]["id"] for row in
                          player_log_fleets["control"]], [0, 1])
        self.assertEqual(player_log_fleets["control"],
                         player_log_fleets["action"])
        self.assertEqual(player_log_fleets["control_hst"],
                         player_log_fleets["control"])
        self.assertEqual(player_log_fleets["action_hst"],
                         player_log_fleets["action"])
        self.assertEqual([row["fleet"]["id"] for row in
                          player_log_owner0_fleets["input"]],
                         [0, 1, 2, 3, 4, 5])
        self.assertEqual([row["order_type"] for row in
                          player_log_owner0_fleets["input"]],
                         [20, 20, 20, 20, 20, 20])
        self.assertTrue(all(len(row["body"]) <= 64 and
                            len(row["order_body"]) == 8
                            for row in player_log_owner0_fleets["input"]))
        self.assertEqual([row["fleet"]["id"] for row in
                          player_log_owner0_fleets["control"]],
                         [0, 1, 2, 3, 4, 5])
        self.assertEqual([row["fleet"]["id"] for row in
                          player_log_owner0_fleets["action"]],
                         [1, 2, 3, 4, 5])
        self.assertEqual(player_log_owner0_fleets["control"][1:],
                         player_log_owner0_fleets["action"])
        self.assertEqual(player_log_owner0_fleets["control_m1"],
                         player_log_owner0_fleets["control"])
        self.assertEqual(player_log_owner0_fleets["action_m1"],
                         player_log_owner0_fleets["action"])
        self.assertEqual([row["order_type"] for row in
                          player_log_fleets["control"]], [20, 20])
        for input_fleet, output_fleet in zip(
                player_log_fleets["input"], player_log_fleets["control"]):
            current = output_fleet["fleet"]
            original = input_fleet["fleet"]
            self.assertEqual(current["id_player_bits"],
                             original["id_player_bits"])
            self.assertEqual(current["design_plan"], original["design_plan"])
        for run in ("control", "action"):
            self.assertEqual(sorted(player_log_planets[run + "_m1"]),
                             [7, 10, 12])
            self.assertEqual(sorted(player_log_planets[run + "_m2"]),
                             [24, 26])
            self.assertEqual([player_log_planets[run + "_m1"][planet_id]["type"]
                              for planet_id in (7, 10, 12)], [14, 13, 14])
            self.assertEqual([player_log_planets[run + "_m2"][planet_id]["type"]
                              for planet_id in (24, 26)], [13, 14])
        self.assertEqual(
            [planet_id for planet_id in full_planet_records["input"]
             if full_planet_records["input"][planet_id]["body"] !=
             full_planet_records["control"][planet_id]["body"]], [10, 24])
        self.assertEqual(
            [planet_id for planet_id in full_planet_records["input"]
             if full_planet_records["input"][planet_id]["body"] !=
             full_planet_records["action"][planet_id]["body"]], [10, 24])
        hull_designs = retained_hull_components(input_records)
        engine_designs, engine_rows = retained_ideal_warp_data(input_records)
        cargo_designs = retained_cargo_capacity_data(input_records)

        self.assertEqual(input_pairs, pair["input_comparison"]
                         ["owner1_input_fleet_order_pairs"])
        self.assertEqual(output_pairs, pair["original_outputs"]
                         ["owner1_turn1_fleet_order_pairs"])
        self.assertEqual(action_pairs, output_pairs)
        source_binding = pair["current_source_binding_status"]
        retained_binding = source_binding["retained_selector1_route"]
        self.assertFalse(source_binding["production_turin_all_actions_bound"])
        self.assertIn("76/76 checks pass", retained_binding["result"])
        self.assertEqual(retained_binding["fleet0_output"]["target_planet"],
                         26)
        self.assertIn("stars_merge_two_fleets",
                      retained_binding["fleet2_3_merge"])
        self.assertIn("task-5 scrap slice",
                      retained_binding["remaining"])
        self.assertIn("stars_satisfy_orders_packed",
                      retained_binding["task5_scrap"])
        player_records = [record["body"] for record in input_records
                          if record["kind"] == 6 and record["body"]
                          and record["body"][0] == 1]
        self.assertEqual(len(player_records), 1)
        input_shdefs = [record for record in input_records
                        if record["kind"] == 26 and record["turn"] == 0]
        input_shdef_ids = [
            (int.from_bytes(record["body"][:2], "little") >> 10) & 31
            for record in input_shdefs]
        self.assertEqual(input_shdef_ids, [0, 1, 2, 16])
        self.assertEqual([(input_shdef_ids[index], record["body"][2])
                          for index, record in enumerate(input_shdefs)
                          if record["body"][2] > 31], [(16, 34)])
        self.assertTrue(all(len(record["body"]) <= 80
                            for record in input_shdefs))
        input_shdef = next(record["body"] for record in input_shdefs
                           if ((int.from_bytes(record["body"][:2], "little")
                                >> 10) & 31) == 2)
        input_planet_record = next(record for record in input_records
                                   if record["kind"] == 13 and
                                   record["turn"] == 0 and
                                   (int.from_bytes(record["body"][:2],
                                                   "little") & 0x7ff) == 24)
        self.assertEqual(hashlib.sha256(STARS_EXE.read_bytes()).hexdigest(),
                         STARS_EXE_SHA256)
        original_segment = bytes(NE(STARS_EXE).raw[1])
        part_segment = original_segment[:0x4872 + 5 * 0x8f]
        mode = struct.unpack_from("<H", player_records[0], 6)[0]
        self.assertEqual(mode, 0x2797)
        planets = []
        for record in input_records:
            if record["kind"] == 13 and record["turn"] == 0:
                id_word, = struct.unpack_from("<H", record["body"], 0)
                planets.append((id_word & 0x7ff, (id_word >> 11) & 31))
        self.assertEqual(planets, [(24, 1)])
        self.assertEqual(len(input_pairs), 4)
        self.assertEqual([pair["fleet"]["order_count"]
                          for pair in input_pairs], [1, 1, 1, 1])
        self.assertEqual([pair["order"]["target_id"]
                          for pair in input_pairs], [24, 24, 24, 24])
        self.assertEqual(len(points), 32)
        self.assertEqual(points[24], (1290, 1258))
        self.assertEqual(points[26], (1313, 1262))
        wormhole_distances = [
            (point[0] - points[24][0]) ** 2 +
            (point[1] - points[24][1]) ** 2
            for point in (struct.unpack_from("<hh", thing, 2)
                          for thing in wormholes)
        ]
        nearest_candidate_distance = (
            (points[26][0] - points[24][0]) ** 2 +
            (points[26][1] - points[24][1]) ** 2)
        self.assertEqual(nearest_candidate_distance, 545)
        self.assertEqual(min(wormhole_distances), 16101)
        self.assertTrue(all(distance > 4 * nearest_candidate_distance
                            for distance in wormhole_distances))
        route_state = dict(player_id=1, known_planets=planets,
                           fleet_orders=input_pairs, active_fleet_id=0)
        ai_planet_status = retained_ai_planet_status(
            mode, len(points), route_state)
        candidate_ids = [planet_id for planet_id, status in
                         enumerate(ai_planet_status) if status == 0x10]
        self.assertNotIn(24, candidate_ids)
        self.assertIn(26, candidate_ids)
        self.assertEqual(len(candidate_ids), 31)
        self.assertEqual(ai_planet_status[24], 1)
        self.assertEqual({status for planet_id, status in
                          enumerate(ai_planet_status) if planet_id != 24},
                         {0x10})

        # The scout's weapon check comes from its retained design's original
        # hull slots. Other present designs have no weapon slot in this input.
        self.assertEqual(hull_designs[0][0], 4)
        self.assertEqual(len(hull_designs[0][1]), 3)
        self.assertTrue(any((group & 0x30) != 0 and (packed >> 8) != 0
                            for group, packed in hull_designs[0][1]))
        self.assertEqual(hull_designs[1][0], 15)
        self.assertFalse(any((group & 0x30) != 0 and (packed >> 8) != 0
                             for group, packed in hull_designs[1][1]))
        self.assertEqual(hull_designs[2][0], 20)
        self.assertFalse(any((group & 0x30) != 0 and (packed >> 8) != 0
                             for group, packed in hull_designs[2][1]))
        self.assertEqual(engine_designs[0], [1])
        self.assertEqual(engine_rows[1],
                         [0, 0, 25, 100, 100, 100, 180, 500, 800, 900,
                          1080, 0])
        self.assertEqual(cargo_designs[1][0], 15)
        self.assertEqual(cargo_designs[1][1], 25)
        self.assertEqual(cargo_designs[1][2], [(1, 0x0101), (0x1000, 0x0100)])

        expected = next(pair for pair in output_pairs
                        if pair["fleet"]["id"] == 0)
        self.assertEqual(expected["fleet"]["point"], list(points[26]))
        self.assertEqual(expected["order"]["target_id"], 26)
        self.assertEqual(expected["order"]["warp"], 5)
        self.assertEqual(expected["fleet"]["cargo"][4], 298)

        input_fleet1 = next(pair["fleet"] for pair in input_pairs
                            if pair["fleet"]["id"] == 1)
        output_fleet1 = next(pair["fleet"] for pair in output_pairs
                             if pair["fleet"]["id"] == 1)
        self.assertEqual(input_planet["minerals"][3], 250)
        self.assertEqual(input_fleet1["cargo"][3], 0)
        self.assertEqual(output_fleet1["cargo"][3], 25)
        self.assertEqual(output_planet["minerals"][3], 256)
        self.assertEqual(action_planet["minerals"][3], 256)

        with tempfile.TemporaryDirectory() as temp:
            temp_path = Path(temp)
            fixture_inputs = route_state
            write_fixture_header(temp_path / "TURINFX.H", points,
                                 input_pairs, output_pairs, expected, mode,
                                 fixture_inputs, wormholes, hull_designs,
                                 engine_designs, engine_rows, cargo_designs,
                                 input_planet, output_planet,
                                 player_records[0], input_shdef,
                                 input_planet_record, part_segment,
                                 wormhole_outputs, wormhole_fleets,
                                 full_planet_records, player_log_planets,
                                 player_log_fleets,
                                 player_log_owner0_fleets,
                                 production_queue_body, input_shdefs,
                                 score_history_records)
            write_supply_fixture_header(
                temp_path / "TURINSUP.H", input_pairs, output_pairs,
                action_pairs, input_planet, output_planet, action_planet)
            supply_exe = temp_path / "satisfy_orders_turin_supply"
            subprocess.run([
                "cc", "-std=c89", "-pedantic", "-Wall", "-Wextra",
                "-Werror", "-ffunction-sections", "-fdata-sections",
                "-Wl,-dead_strip", "-I", str(temp_path), "-I",
                str(ROOT / "reconstructed/include"),
                str(ROOT / "tests/satisfy_orders_turin_supply_e3_probe.c"),
                str(ROOT / "reconstructed/src/common.c"),
                str(ROOT / "reconstructed/src/ai.c"),
                "-o", str(supply_exe),
            ], check=True)
            supply_result = subprocess.run([str(supply_exe)], check=False,
                                           capture_output=True, text=True)
            self.assertEqual(supply_result.returncode, 0,
                             supply_result.stdout + supply_result.stderr)
            self.assertEqual(
                supply_result.stdout.strip(),
                "SATISFY_ORDERS_TURIN_SUPPLY_CHECKS=10 FAILURES=0 RESULT=PASS",
            )
            exe = temp_path / "satisfy_orders_turin_binding"
            compile_args = [
                "cc", "-std=c89", "-pedantic", "-Wall", "-Wextra",
                "-Werror", "-ffunction-sections", "-fdata-sections",
                "-Wl,-dead_strip", "-I", str(temp_path), "-I",
                str(ROOT / "reconstructed/include"),
                str(ROOT / "tests/satisfy_orders_turin_binding_e3_probe.c"),
                str(ROOT / "reconstructed/src/strings.c"),
                str(ROOT / "reconstructed/src/log_change_fleet.c"),
                str(ROOT / "reconstructed/src/log_change_planet.c"),
                str(ROOT / "reconstructed/src/run_log_record.c"),
                str(ROOT / "reconstructed/src/write_hist_file.c"),
                str(ROOT / "reconstructed/src/write_log_file.c"),
                str(ROOT / "reconstructed/src/load_log_file.c"),
                str(ROOT / "reconstructed/src/do_ai_turn.c"),
                str(ROOT / "reconstructed/src/do_turin_drone_ai_turn.c"),
                str(ROOT / "reconstructed/src/ensure_turin_drone_shdefs.c"),
                str(ROOT / "reconstructed/src/get_production_costs.c"),
                str(ROOT / "reconstructed/src/fill_prod_mines_and_factories.c"),
                str(ROOT / "reconstructed/src/init_production.c"),
                str(ROOT / "reconstructed/src/change_main_obj_sel.c"),
                str(ROOT / "reconstructed/src/can_terraform.c"),
                str(ROOT / "reconstructed/src/generate_turn.c"),
                str(ROOT / "reconstructed/src/spd_of_ship.c"),
                str(ROOT / "reconstructed/src/update_player_scores.c"),
                str(ROOT / "reconstructed/src/heal_ships.c"),
                str(ROOT / "reconstructed/src/fuel_fleets.c"),
                str(ROOT / "reconstructed/src/merge_fleets.c"),
                str(ROOT / "reconstructed/src/fleet_transfer_cargo_balance.c"),
                str(ROOT / "reconstructed/src/invalidate_report.c"),
                str(ROOT / "reconstructed/src/turin_scout_adapter.c"),
                str(ROOT / "reconstructed/src/ai.c"),
                str(ROOT / "reconstructed/src/battle.c"),
                str(ROOT / "reconstructed/src/ai_plord.c"),
                str(ROOT / "reconstructed/src/do_orders.c"),
                str(ROOT / "reconstructed/src/find_ideal_warp.c"),
                str(ROOT / "reconstructed/src/iwarp_best_for_waypoint.c"),
                str(ROOT / "reconstructed/src/fuel_use_to_waypoint.c"),
                str(ROOT / "reconstructed/src/common.c"),
                str(ROOT / "reconstructed/src/turn2.c"),
                str(ROOT / "reconstructed/src/est_minerals_mined.c"),
                str(ROOT / "reconstructed/src/produce.c"),
                str(ROOT / "reconstructed/src/update_research_status.c"),
                str(ROOT / "reconstructed/src/meteor_strike.c"),
                str(ROOT / "reconstructed/src/planetary_climate_change.c"),
                str(ROOT / "reconstructed/src/discover_minerals.c"),
                str(ROOT / "reconstructed/src/mystery_trader.c"),
                str(ROOT / "reconstructed/src/breed_colonists_in_transit.c"),
                str(ROOT / "reconstructed/src/auto_terraform.c"),
                str(ROOT / "reconstructed/src/remote_terraforming.c"),
                str(ROOT / "reconstructed/src/terra_fleet.c"),
                str(ROOT / "reconstructed/src/spank_the_cheaters.c"),
                str(ROOT / "reconstructed/src/c_build_prod_item.c"),
                str(ROOT / "reconstructed/src/estimate_item_prod_sched.c"),
                str(ROOT / "reconstructed/src/finish_production.c"),
                str(ROOT / "reconstructed/src/records.c"),
                str(ROOT / "reconstructed/src/update_guesses.c"),
                str(ROOT / "reconstructed/src/planet_max_pop.c"),
                str(ROOT / "reconstructed/src/change_population.c"),
                str(ROOT / "reconstructed/src/planet.c"),
                str(ROOT / "reconstructed/src/move_fleets.c"),
                str(ROOT / "reconstructed/src/kill_used_waypoints.c"),
                str(ROOT / "reconstructed/src/delete_wp_far.c"),
                str(ROOT / "reconstructed/src/est_fuel_use.c"),
                str(ROOT / "reconstructed/src/move_things.c"),
                str(ROOT / "reconstructed/src/thing_decay.c"),
                str(ROOT / "reconstructed/src/thing.c"),
                str(ROOT / "reconstructed/src/link_fleets.c"),
                str(ROOT / "reconstructed/src/do_battles.c"),
                str(ROOT / "reconstructed/src/cplr_battle.c"),
                str(ROOT / "reconstructed/src/do_bombing.c"),
                str(ROOT / "reconstructed/src/drop_colonists.c"),
                str(ROOT / "reconstructed/src/transfer_to_others.c"),
                str(ROOT / "reconstructed/src/messages.c"),
                str(ROOT / "reconstructed/src/sweep_for_mines.c"),
                str(ROOT / "reconstructed/src/mine_sweep.c"),
                str(ROOT / "reconstructed/src/mine_sweep_hull.c"),
                str(ROOT / "reconstructed/src/satisfy_orders.c"),
                str(ROOT / "reconstructed/src/lookup_part.c"),
                str(ROOT / "reconstructed/src/reprice_shdef.c"),
                str(ROOT / "reconstructed/src/true_part_cost.c"),
                str(ROOT / "reconstructed/src/mark_techs.c"),
                str(ROOT / "reconstructed/src/itech_learn_a_tech.c"),
                str(ROOT / "reconstructed/src/file.c"),
                str(ROOT / "reconstructed/src/set_vis_pf_init.c"),
                str(ROOT / "reconstructed/src/set_vis_pf_fleets.c"),
                str(ROOT / "reconstructed/src/set_vis_pf_planets.c"),
                str(ROOT / "reconstructed/src/set_vis_pf_things.c"),
                str(ROOT / "reconstructed/src/write_data_file.c"),
                str(ROOT / "reconstructed/src/write_battles.c"),
                str(ROOT / "build/libstars_core.dylib"),
                "-Wl,-rpath," + str(ROOT / "build"),
                "-o", str(exe),
            ]
            subprocess.run(compile_args, check=True)
            rototill_exe = temp_path / "satisfy_orders_rototill_coordinator"
            rototill_args = [
                str(ROOT / "tests/satisfy_orders_rototill_coordinator_e4_probe.c")
                if argument == str(ROOT / "tests/satisfy_orders_turin_binding_e3_probe.c")
                else str(rototill_exe) if argument == str(exe) else argument
                for argument in compile_args
            ]
            rototill_args.insert(-2,
                str(ROOT / "reconstructed/src/do_rototill_ai_turn.c"))
            rototill_args.insert(-2,
                str(ROOT / "reconstructed/src/rototill_scout_adapter.c"))
            rototill_args.insert(-2,
                str(ROOT / "reconstructed/src/f_build_object.c"))
            subprocess.run(rototill_args, check=True)
            rototill_result = subprocess.run([str(rototill_exe), "--dump-designs"],
                check=False, capture_output=True, text=True)
            rng_preview = subprocess.run([str(rototill_exe), "--dump-rng"],
                check=False, capture_output=True, text=True)
            self.assertEqual(rng_preview.returncode, 0,
                             rng_preview.stdout + rng_preview.stderr)
            rng_lines = [line for line in rng_preview.stdout.splitlines()
                         if line.startswith("ROT_RNG ")]
            expected_rng = [line for line in
                            (ROOT / "analysis/compat/do-rototill-main-rng-r1.txt")
                            .read_text().splitlines()
                            if line.startswith("ROT_RNG ")]
            self.assertEqual(rng_lines, expected_rng)
            self.assertEqual(rototill_result.returncode, 0,
                             rototill_result.stdout + rototill_result.stderr)
            rototill_lines = rototill_result.stdout.strip().splitlines()
            self.assertEqual(rototill_lines[-1],
                             "ROTOTILL_COORDINATOR_FAILURES=0 RESULT=PASS")
            self.assertEqual(len(rototill_lines), 139)
            rototill_designs = {}
            for line in rototill_lines[:4]:
                marker, slot_text, body_hex = line.split()
                self.assertEqual(marker, "ROT_DESIGN")
                rototill_designs[int(slot_text)] = bytes.fromhex(body_hex)
            self.assertEqual(sorted(rototill_designs), [17, 18, 19, 20])
            rototill_original = decode_stream(
                ROOT / "runs/rototill-original-turn-r1/output/CLEAN.M2",
                Inspector())
            original_designs = {}
            for row in rototill_original:
                if row["turn"] != 1 or row["kind"] != 26:
                    continue
                body = row["body"]
                slot = (int.from_bytes(body[:2], "little") >> 10) & 31
                if slot in range(17, 21):
                    original_designs[slot] = body
            self.assertEqual(sorted(original_designs), [17, 18, 19, 20])
            for slot in original_designs:
                actual = rototill_designs[slot]
                expected = original_designs[slot]
                self.assertEqual(actual[2:4], expected[2:4])
                self.assertEqual(actual[6:], expected[6:])
            original_log = decode_stream(
                ROOT / "runs/rototill-original-turn-r1/backup/CLEAN.X2",
                Inspector())
            logged_designs = {}
            for row in original_log:
                if row["turn"] != 0 or row["kind"] != 27:
                    continue
                body = row["body"][2:]
                slot = (int.from_bytes(body[:2], "little") >> 10) & 31
                if slot in range(17, 21):
                    logged_designs[slot] = body
            self.assertEqual(rototill_designs, logged_designs)
            generated_mutations = []
            for line in rototill_lines[4:11]:
                marker, kind_text, body_hex = line.split()
                self.assertEqual(marker, "ROT_MUT")
                generated_mutations.append((int(kind_text),
                                            bytes.fromhex(body_hex)))
            original_mutations = [(row["kind"], row["body"])
                                  for row in original_log
                                  if row["turn"] == 0 and
                                  row["kind"] not in (8, 9, 0)]
            self.assertEqual([kind for kind, _ in original_mutations],
                             [27, 27, 27, 27, 4, 1, 29])
            self.assertEqual(generated_mutations, original_mutations)
            queue_mutations = [row["body"] for row in original_log
                               if row["turn"] == 0 and row["kind"] == 29]
            self.assertEqual(queue_mutations,
                             [bytes.fromhex("1800031c0200")])
            produced = rototill_lines[11].split()
            self.assertEqual(produced[0], "ROT_PRODUCE")
            original_queues = [row["body"] for row in rototill_original
                               if row["turn"] == 1 and row["kind"] == 28]
            self.assertEqual(original_queues, [bytes.fromhex(produced[1])])
            generated_log = rototill_lines[12].split()
            self.assertEqual(generated_log[0], "ROT_LOG")
            self.assertEqual(bytes.fromhex(generated_log[1]),
                (ROOT / "runs/rototill-original-turn-r1/backup/CLEAN.X2").read_bytes())
            generated_history = rototill_lines[13].split()
            self.assertEqual(generated_history[0], "ROT_HIST")
            self.assertEqual(bytes.fromhex(generated_history[1]),
                (ROOT / "runs/rototill-original-turn-r1/output/CLEAN.H2").read_bytes())
            generated_research = rototill_lines[14].split()
            self.assertEqual(generated_research[0], "ROT_RESEARCH")
            output_players = [row["body"] for row in rototill_original
                              if row["turn"] == 1 and row["kind"] == 6 and
                              row["body"][0] == 1]
            self.assertEqual(len(output_players), 1)
            self.assertEqual(bytes.fromhex(generated_research[1]),
                             output_players[0][26:62])
            generated_planet = rototill_lines[15].split()
            self.assertEqual(generated_planet[0], "ROT_PLANET")
            expected_planets = [row["body"] for row in rototill_original
                                if row["turn"] == 1 and row["kind"] == 13 and
                                (int.from_bytes(row["body"][:2], "little") & 0x7ff) == 24]
            self.assertEqual(len(expected_planets), 1)
            produced_planet = bytes.fromhex(generated_planet[1])
            self.assertEqual(len(produced_planet), 37)
            self.assertEqual(produced_planet, expected_planets[0])
            generated_planet10 = rototill_lines[16].split()
            self.assertEqual(generated_planet10[0], "ROT_PLANET10")
            original_m1 = decode_stream(
                ROOT / "runs/rototill-original-turn-r1/output/CLEAN.M1",
                Inspector())
            expected_planet10 = [row["body"] for row in original_m1
                                 if row["turn"] == 1 and row["kind"] == 13 and
                                 (int.from_bytes(row["body"][:2], "little") & 0x7ff) == 10]
            self.assertEqual(len(expected_planet10), 1)
            self.assertEqual(bytes.fromhex(generated_planet10[1]),
                             expected_planet10[0])
            generated_things = rototill_lines[17].split()
            self.assertEqual(generated_things[0], "ROT_THINGS")
            original_hst = decode_stream(
                ROOT / "runs/rototill-original-turn-r1/output/CLEAN.HST",
                Inspector())
            original_hst_queues = [row["body"] for row in original_hst
                                   if row["turn"] == 1 and row["kind"] == 28]
            self.assertEqual(original_hst_queues,
                             [bytes.fromhex(produced[1])])
            for original_records in (original_m1, rototill_original,
                                     original_hst):
                self.assertEqual([row["body"] for row in original_records
                                  if row["turn"] == 1 and row["kind"] == 0],
                                 [struct.pack("<H", 1)])
            original_things = [row["body"] for row in original_hst
                               if row["turn"] == 1 and row["kind"] == 43 and
                               len(row["body"]) == 18]
            self.assertEqual(len(original_things), 4)
            self.assertEqual(bytes.fromhex(generated_things[1]),
                             b"".join(original_things))
            original_fleets = []
            for record_index, row in enumerate(rototill_original[:-1]):
                if (row["turn"] == 1 and row["kind"] == 16 and
                        int.from_bytes(row["body"][2:4], "little") == 1):
                    order = rototill_original[record_index + 1]
                    self.assertEqual(order["kind"], 20)
                    original_fleets.append((row["body"], order["body"]))
            self.assertEqual(len(original_fleets), 4)
            for fleet_id, expected_pair in enumerate(original_fleets):
                generated = rototill_lines[18 + fleet_id].split()
                self.assertEqual(generated[:2], ["ROT_FLEET", str(fleet_id)])
                self.assertEqual((bytes.fromhex(generated[2]),
                                  bytes.fromhex(generated[3])), expected_pair)
            original_host_fleets = []
            for record_index, row in enumerate(original_hst[:-1]):
                if (row["turn"] == 1 and row["kind"] == 16 and
                        int.from_bytes(row["body"][2:4], "little") == 0):
                    order = original_hst[record_index + 1]
                    self.assertEqual(order["kind"], 20)
                    original_host_fleets.append((row["body"], order["body"]))
            self.assertEqual(len(original_host_fleets), 6)
            for fleet_id, expected_pair in enumerate(original_host_fleets):
                generated = rototill_lines[22 + fleet_id].split()
                self.assertEqual(generated[:2], ["ROT_HFLEET", str(fleet_id)])
                self.assertEqual((bytes.fromhex(generated[2]),
                                  bytes.fromhex(generated[3])), expected_pair)
            original_host_planets = [row["body"] for row in original_hst
                                     if row["turn"] == 1 and row["kind"] == 13]
            self.assertEqual(len(original_host_planets), 32)
            for planet_id, expected_body in enumerate(original_host_planets):
                generated = rototill_lines[28 + planet_id].split()
                self.assertEqual(generated[:2], ["ROT_HPLANET", str(planet_id)])
                self.assertEqual(bytes.fromhex(generated[2]), expected_body)
            generated_tail = rototill_lines[60].split()
            self.assertEqual(generated_tail[0], "ROT_RNGTAIL")
            tail_draws = [int(value) for value in generated_tail[1:]]
            self.assertEqual(tail_draws[0], 6)
            original_bofs = []
            for records in (original_m1, rototill_original, original_hst):
                matches = [row["body"] for row in records
                           if row["turn"] == 1 and row["kind"] == 8]
                self.assertEqual(len(matches), 1)
                original_bofs.append(matches[0])
            self.assertEqual([(int.from_bytes(bof[14:16], "little") >> 13)
                              & 7 for bof in original_bofs], [6, 6, 6])
            tick_residues = [((int.from_bytes(bof[12:14], "little") >> 5)
                              - draw) & 2047
                             for bof, draw in zip(original_bofs,
                                                  tail_draws[1:])]
            self.assertEqual(tick_residues, [917, 920, 921])
            for file_index, original_bof in enumerate(original_bofs):
                generated = rototill_lines[61 + file_index].split()
                self.assertEqual(generated[:2], ["ROT_BOF", str(file_index)])
                self.assertEqual(bytes.fromhex(generated[2]),
                                 struct.pack("<H", 8 << 10 | 16) + original_bof)
            for index, slot in enumerate((18, 20, 17, 19)):
                generated = rototill_lines[64 + index].split()
                self.assertEqual(generated[:2], ["ROT_ODSGN", str(slot)])
                self.assertEqual(bytes.fromhex(generated[2]),
                                 original_designs[slot])
            generated_player = rototill_lines[68].split()
            self.assertEqual(generated_player[0], "ROT_M2PLAYER")
            generated_player_body = bytes.fromhex(generated_player[1])
            self.assertEqual(len(generated_player_body), len(output_players[0]))
            self.assertEqual([(offset, actual, expected)
                              for offset, (actual, expected) in enumerate(
                                  zip(generated_player_body, output_players[0]))
                              if actual != expected], [])
            original_scores = [row["body"] for row in rototill_original
                               if row["turn"] == 1 and row["kind"] == 45]
            self.assertEqual(len(original_scores), 1)
            generated_score = rototill_lines[69].split()
            self.assertEqual(generated_score[0], "ROT_M2SCORE")
            generated_score_body = bytes.fromhex(generated_score[1])
            self.assertEqual([(offset, actual, expected)
                              for offset, (actual, expected) in enumerate(
                                  zip(generated_score_body, original_scores[0]))
                              if actual != expected], [])
            original_m1_scores = [row["body"] for row in original_m1
                                  if row["turn"] == 1 and row["kind"] == 45]
            self.assertEqual(len(original_m1_scores), 1)
            generated_m1_score = rototill_lines[70].split()
            self.assertEqual(generated_m1_score[0], "ROT_M1SCORE")
            self.assertEqual(bytes.fromhex(generated_m1_score[1]),
                             original_m1_scores[0])
            original_m1_players = [row["body"] for row in original_m1
                                   if row["turn"] == 1 and row["kind"] == 6]
            self.assertEqual(len(original_m1_players), 1)
            generated_m1_player = rototill_lines[71].split()
            self.assertEqual(generated_m1_player[0], "ROT_M1PLAYER")
            generated_m1_player_body = bytes.fromhex(generated_m1_player[1])
            self.assertEqual(len(generated_m1_player_body),
                             len(original_m1_players[0]))
            self.assertEqual([(offset, actual, expected)
                              for offset, (actual, expected) in enumerate(
                                  zip(generated_m1_player_body,
                                      original_m1_players[0]))
                              if actual != expected], [])
            original_hst_players = [row["body"] for row in original_hst
                                    if row["turn"] == 1 and row["kind"] == 6]
            self.assertEqual(len(original_hst_players), 2)
            for player in range(2):
                generated_hst_player = rototill_lines[72 + player].split()
                self.assertEqual(generated_hst_player[:2],
                                 ["ROT_HPLAYER", str(player)])
                generated_hst_body = bytes.fromhex(generated_hst_player[2])
                self.assertEqual(len(generated_hst_body),
                                 len(original_hst_players[player]))
                self.assertEqual([(offset, actual, expected)
                                  for offset, (actual, expected) in enumerate(
                                      zip(generated_hst_body,
                                          original_hst_players[player]))
                                  if actual != expected], [])
            generated_designs = rototill_lines[74:104]
            self.assertEqual(len(generated_designs), 30)
            for file_index, original_records in enumerate(
                    (original_m1, rototill_original, original_hst)):
                expected_designs = [row["body"] for row in original_records
                                    if row["turn"] == 1 and row["kind"] == 26]
                generated_file_designs = [bytes.fromhex(parts[2])
                    for parts in (line.split() for line in generated_designs)
                    if parts[:2] == ["ROT_DSGN", str(file_index)]]
                self.assertEqual(len(generated_file_designs),
                                 len(expected_designs))
                for design_index, (actual, expected) in enumerate(
                        zip(generated_file_designs, expected_designs)):
                    self.assertEqual([(offset, a, e)
                                      for offset, (a, e) in enumerate(
                                          zip(actual, expected)) if a != e],
                                     [], (file_index, design_index,
                                          len(actual), len(expected)))
            generated_plans = rototill_lines[104:124]
            self.assertEqual(len(generated_plans), 20)
            for file_index, original_records in enumerate(
                    (original_m1, rototill_original, original_hst)):
                expected_plans = [row["body"] for row in original_records
                                  if row["turn"] == 1 and row["kind"] == 30]
                generated_file_plans = [bytes.fromhex(parts[2])
                    for parts in (line.split() for line in generated_plans)
                    if parts[:2] == ["ROT_PLAN", str(file_index)]]
                self.assertEqual(generated_file_plans, expected_plans)
            generated_client_planets = rototill_lines[124:129]
            self.assertEqual(len(generated_client_planets), 5)
            for file_index, original_records in enumerate(
                    (original_m1, rototill_original)):
                expected_client_planets = [
                    (row["kind"], row["body"]) for row in original_records
                    if row["turn"] == 1 and row["kind"] in (13, 14)]
                generated_file_planets = [
                    (int(parts[2]), bytes.fromhex(parts[3]))
                    for parts in (line.split() for line in generated_client_planets)
                    if parts[:2] == ["ROT_CPLANET", str(file_index)]]
                self.assertEqual(generated_file_planets,
                                 expected_client_planets)
            original_m1_messages = [row["body"] for row in original_m1
                                    if row["turn"] == 1 and row["kind"] == 12]
            self.assertEqual(len(original_m1_messages), 1)
            generated_m1_message = rototill_lines[129].split()
            self.assertEqual(generated_m1_message[0], "ROT_M1MESSAGE")
            self.assertEqual(bytes.fromhex(generated_m1_message[1]),
                             original_m1_messages[0])
            original_hst_things = [row["body"] for row in original_hst
                                   if row["turn"] == 1 and row["kind"] == 43]
            generated_hst_things = [bytes.fromhex(parts[1])
                for parts in (line.split() for line in rototill_lines[130:135])
                if parts[0] == "ROT_THING"]
            self.assertEqual(generated_hst_things, original_hst_things)
            generated_bofs = [bytes.fromhex(rototill_lines[61 + file_index]
                              .split()[2])[2:] for file_index in range(3)]
            generated_design_groups = {}
            generated_plan_groups = {}
            generated_client_groups = {}
            for file_index in range(3):
                generated_design_groups[file_index] = [
                    bytes.fromhex(parts[2])
                    for parts in (line.split() for line in
                                  rototill_lines[74:104])
                    if parts[:2] == ["ROT_DSGN", str(file_index)]]
                generated_plan_groups[file_index] = [
                    bytes.fromhex(parts[2])
                    for parts in (line.split() for line in
                                  rototill_lines[104:124])
                    if parts[:2] == ["ROT_PLAN", str(file_index)]]
                generated_client_groups[file_index] = [
                    (int(parts[2]), bytes.fromhex(parts[3]))
                    for parts in (line.split() for line in
                                  rototill_lines[124:129])
                    if parts[:2] == ["ROT_CPLANET", str(file_index)]]
            generated_m1_fleets = []
            generated_m2_fleets = []
            for fleet_id in range(6):
                parts = rototill_lines[22 + fleet_id].split()
                generated_m1_fleets += [(16, bytes.fromhex(parts[2])),
                                        (20, bytes.fromhex(parts[3]))]
            for fleet_id in range(4):
                parts = rototill_lines[18 + fleet_id].split()
                generated_m2_fleets += [(16, bytes.fromhex(parts[2])),
                                        (20, bytes.fromhex(parts[3]))]
            generated_hst_planets = [
                bytes.fromhex(rototill_lines[28 + planet_id].split()[2])
                for planet_id in range(32)]
            queue_body = bytes.fromhex(rototill_lines[11].split()[1])
            turn_marker = struct.pack("<H", 1)
            generated_m1_records = (
                [(8, generated_bofs[0]),
                 (6, generated_m1_player_body),
                 (12, bytes.fromhex(generated_m1_message[1]))] +
                generated_client_groups[0] +
                [(26, body) for body in generated_design_groups[0][:6]] +
                generated_m1_fleets +
                [(26, body) for body in generated_design_groups[0][6:]] +
                [(45, bytes.fromhex(generated_m1_score[1]))] +
                [(30, body) for body in generated_plan_groups[0]] +
                [(0, turn_marker)])
            generated_m2_records = (
                [(8, generated_bofs[1]),
                 (6, generated_player_body)] +
                generated_client_groups[1][:1] +
                [(28, queue_body)] +
                generated_client_groups[1][1:] +
                [(26, body) for body in generated_design_groups[1][:3]] +
                generated_m2_fleets +
                [(26, body) for body in generated_design_groups[1][3:]] +
                [(45, generated_score_body)] +
                [(30, body) for body in generated_plan_groups[1]] +
                [(0, turn_marker)])
            generated_hst_records = (
                [(8, generated_bofs[2])] +
                [(6, bytes.fromhex(rototill_lines[72 + player]
                                    .split()[2])) for player in range(2)] +
                [(13, body) for body in generated_hst_planets[:25]] +
                [(28, queue_body)] +
                [(13, body) for body in generated_hst_planets[25:]] +
                [(26, body) for body in generated_design_groups[2][:9]] +
                generated_m1_fleets + generated_m2_fleets +
                [(26, body) for body in generated_design_groups[2][9:]] +
                [(43, body) for body in generated_hst_things] +
                [(30, body) for body in generated_plan_groups[2]] +
                [(0, turn_marker)])
            for generated_records, original_records, expected_count in (
                    (generated_m1_records, original_m1, 32),
                    (generated_m2_records, rototill_original, 28),
                    (generated_hst_records, original_hst, 87)):
                original_turn_records = [(row["kind"], row["body"])
                                         for row in original_records
                                         if row["turn"] == 1]
                self.assertEqual(len(generated_records), expected_count)
                self.assertEqual(generated_records, original_turn_records)
            for generated_records, original_records, filename in (
                    (generated_m1_records, original_m1, "CLEAN.M1"),
                    (generated_m2_records, rototill_original, "CLEAN.M2"),
                    (generated_hst_records, original_hst, "CLEAN.HST")):
                # The M1 turn-0 append prefix is retained from the original;
                # every turn-1 body comes from the source-backed probe.
                prefix = ([(row["kind"], row["body"])
                           for row in original_records if row["turn"] == 0]
                          if filename == "CLEAN.M1" else [])
                encoded = encode_stream(
                    [{"kind": kind, "body": body}
                     for kind, body in prefix + generated_records],
                    Inspector())
                self.assertEqual(encoded,
                    (ROOT / "runs/rototill-original-turn-r1/output" /
                     filename).read_bytes())
            for file_index, filename in enumerate(
                    ("CLEAN.M1", "CLEAN.M2", "CLEAN.HST")):
                generated_file = rototill_lines[135 + file_index].split()
                self.assertEqual(generated_file[:2],
                                 ["ROT_FILE", str(file_index)])
                self.assertEqual(bytes.fromhex(generated_file[2]),
                    (ROOT / "runs/rototill-original-turn-r1/output" /
                     filename).read_bytes())
            self.assertEqual(
                (ROOT / "runs/rototill-original-turn-r1/input/CLEAN.M1")
                .read_bytes(),
                (ROOT / "runs/vmctl-sye3k0/BACKUP/CLEAN.M1").read_bytes())
            results = [subprocess.run([str(exe), str(draw)], check=False,
                                      capture_output=True, text=True)
                       for draw in range(100)]
            seeded_results = []
            for label in ("control", "action"):
                stream_dir = temp_path / (label + "-streams")
                stream_dir.mkdir()
                result = subprocess.run([str(exe), label, str(stream_dir)],
                                        check=False, capture_output=True,
                                        text=True)
                seeded_results.append(result)
                if result.returncode == 0:
                    original_dir = ROOT / "runs" / (
                        "vmctl-sye3k1" if label == "action" else "vmctl-sye3k0")
                    stream_inspector = Inspector()
                    for suffix in ("M1", "M2", "HST"):
                        generated_path = stream_dir / ("CLEAN." + suffix)
                        original_path = original_dir / ("CLEAN." + suffix)
                        generated = decode_stream(generated_path,
                                                  stream_inspector)
                        original = decode_stream(original_path,
                                                 stream_inspector)
                        generated_turn = [row for row in generated
                                          if row["turn"] == 1]
                        original_turn = [row for row in original
                                         if row["turn"] == 1]
                        expected_count = {"control": {"M1": 32, "M2": 24,
                                                       "HST": 83},
                                          "action": {"M1": 30, "M2": 24,
                                                     "HST": 81}}[label][suffix]
                        self.assertEqual(len(generated_turn),
                                         expected_count,
                                         (label, suffix,
                                          [(row["kind"], row["length"])
                                           for row in generated_turn]))
                        self.assertEqual(
                            [(row["kind"], row["body"]) for row in generated_turn],
                            [(row["kind"], row["body"]) for row in
                             original_turn], (label, suffix))
                        self.assertEqual(generated_path.read_bytes(),
                                         original_path.read_bytes(),
                                         (label, suffix, "raw stream"))
                        if suffix == "M1":
                            self.assertEqual(len(generated) - expected_count, 31)
                            prefix_size = original_turn[0]["offset"]
                            self.assertEqual(generated_path.read_bytes()[:prefix_size],
                                             original_path.read_bytes()[:prefix_size])
        for result in results:
            self.assertEqual(result.returncode, 0,
                             result.stdout + result.stderr)
            self.assertEqual(
                result.stdout.strip(),
                "SATISFY_ORDERS_TURIN_BINDING_CHECKS=81 FAILURES=0 RESULT=PASS",
            )
        for label, result in zip(("control", "action"), seeded_results):
            self.assertEqual(result.returncode, 0,
                             "%s: %s%s" %
                             (label, result.stdout, result.stderr))
            self.assertEqual(
                result.stdout.strip(),
                "SATISFY_ORDERS_TURIN_BINDING_SEEDED=PASS STREAM_DRAWS=%d AI_NAMES=4 BOFS=5 SHUFFLE=PASS TECH=PASS MINING=PASS EVENTS=PASS MINE_SWEEP=PASS WORMHOLES=PASS FUELING=PASS HEALING=PASS TERRAFORM_GATES=PASS SCORES=PASS GUESSES=PASS GUESS_INPUTS=32/32 SAVED_FLAG=PASS OUTPUT_BOFS=3/3 TURN_MARKERS=3/3 PLAYER_PLANETS=5/5 TYPE28_QUEUE=2/2 M1_INPUT_PLANETS=3/3 M2_INPUT_PLANETS=1/1 M2_SHDEF_READS=4/4 LOOKUP_FAILURES=0 INPUT_PLAYER_READS=5/5 M2_INPUT_FLEETS=4/4 M1_INPUT_FLEETS=6/6 HST_INPUT_PLANETS=32/32 M1_FLEETS=%d/%d M1_ORDERS=%d/%d M2_FLEETS=2/2 M2_ORDERS=2/2 HST_FLEETS=%d/%d HST_ORDERS=%d/%d PLANET_RECORDS=32/32 SCOUT=PASS" %
                (57 if label == "control" else 39,
                 6 if label == "control" else 5,
                 6 if label == "control" else 5,
                 6 if label == "control" else 5,
                 6 if label == "control" else 5,
                 8 if label == "control" else 7,
                 8 if label == "control" else 7,
                 8 if label == "control" else 7,
                 8 if label == "control" else 7),
            )


if __name__ == "__main__":
    unittest.main()
