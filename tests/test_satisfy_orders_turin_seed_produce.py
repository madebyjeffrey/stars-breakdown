import hashlib
import ctypes
import json
import struct
import subprocess
import tempfile
import unittest
from ctypes import byref, c_int16
from pathlib import Path

from tools.race_file_compare import Inspector, Seeds
from tools.stars_game_stream import decode_stream
from tools.satisfy_orders_planet10_oracle import decode_planet_record


ROOT = Path(__file__).resolve().parents[1]


def xy_point_count(path, inspector):
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
        byref(seeds), game_id, c_int16(salt), c_int16(turn),
        c_int16(player), c_int16((flags >> 12) & 1))
    header, = struct.unpack_from("<H", raw, 18)
    if header >> 10 != 7 or (header & 0x3ff) != 64:
        raise ValueError("unexpected XY game record")
    game = inspector.crypt(raw[20:84], seeds)
    count, = struct.unpack_from("<h", game, 10)
    if count < 1 or 84 + count * 4 + 4 != len(raw):
        raise ValueError("unexpected XY point-array bounds")
    return count


class ReadPlanetInput(ctypes.Structure):
    _fields_ = [
        ("id_player_word", ctypes.c_uint16),
        ("flags_word", ctypes.c_uint16),
        ("header_flags", ctypes.c_uint16),
        ("game_turn", ctypes.c_uint16),
        ("previous_flags", ctypes.c_uint16),
        ("previous_turn", ctypes.c_uint16),
        ("history", ctypes.c_uint16),
        ("preinitialized", ctypes.c_uint16),
        ("payload", ctypes.POINTER(ctypes.c_uint8)),
        ("payload_size", ctypes.c_uint16),
    ]


class ReadPlanetState(ctypes.Structure):
    _fields_ = [
        ("id", ctypes.c_int16),
        ("player", ctypes.c_int16),
        ("flags", ctypes.c_uint16),
        ("turn", ctypes.c_uint16),
        ("mineral_levels", ctypes.c_uint8 * 3),
        ("mineral_concentrations", ctypes.c_uint8 * 3),
        ("environment_original", ctypes.c_uint8 * 3),
        ("environment", ctypes.c_uint8 * 3),
        ("routing_word", ctypes.c_uint16),
        ("minerals", ctypes.c_uint32 * 4),
        ("opaque_20", ctypes.c_uint32 * 2),
        ("opaque_44", ctypes.c_uint32),
        ("opaque_48", ctypes.c_uint16),
    ]


class SatisfyOrdersTurinSeedProduceTests(unittest.TestCase):
    def test_turn_zero_seed_queue_flows_through_produce(self):
        oracle = json.loads((ROOT / "analysis/compat/"
                             "satisfy-orders-full-turn-e3-original-oracle.json"
                             ).read_text(encoding="utf-8"))
        planet = oracle["current_source_control_action_pair"][
            "owner1_planet24"]["input"]
        production_input = oracle["turin_turn_zero_seed_queue_replay"][
            "current_source_probe"]["input"]
        mining_audit = oracle["current_source_control_action_pair"][
            "owner1_planet24"]["production_mining_stage"]
        mining_inputs = mining_audit["retained_owner1_mining_inputs"]
        post_task5_stock = mining_audit["owner1_exact_mineral_stock_witness"][
            "known_task5_scrap_stock_before_mining"]
        production_costs = mining_audit["owner1_exact_mineral_stock_witness"][
            "runtime_production_cost_order"]
        fresh_pair = json.loads((ROOT / "runs/vmctl-sye3k-pair-r1/PAIR.JSON")
                                .read_text(encoding="utf-8"))
        draw_bounds = fresh_pair["mining_random100_threshold_observations"][
            "outputs"]
        expected_map_points = oracle["xy_coordinate_crosscheck"]["point_count"]
        input_hashes = oracle["current_source_control_action_pair"][
            "owner1_planet24"]["production_mining_stage"]["input_hashes"]
        m2_path = ROOT / "runs/vmctl-sye3k0/BACKUP/CLEAN.M2"
        self.assertEqual(hashlib.sha256(m2_path.read_bytes()).hexdigest(),
                         input_hashes["CLEAN.M2"])
        subprocess.run(["python3", str(ROOT / "tools/build_core.py")],
                       check=True)
        library = ROOT / "build/libstars_core.dylib"
        core = ctypes.CDLL(str(library))
        core.stars_read_planet.argtypes = [
            ctypes.POINTER(ReadPlanetInput), ctypes.POINTER(ReadPlanetState),
            ctypes.c_int16, ctypes.c_void_p, ctypes.c_void_p]
        core.stars_read_planet.restype = ctypes.c_int16
        inspector = Inspector(core)
        records = decode_stream(m2_path, inspector)
        player_records = [record["body"] for record in records
                          if record["kind"] == 6 and record["body"] and
                          record["body"][0] == 1]
        self.assertEqual(len(player_records), 1)
        player_environment = list(player_records[0][16:25])
        planet_records = [record for record in records
                          if record["kind"] == 13 and len(record["body"]) >= 4
                          and (int.from_bytes(record["body"][:2], "little")
                               & 0x7ff) == 24]
        self.assertEqual(len(planet_records), 1)
        planet_record = planet_records[0]
        planet_body = planet_record["body"]
        payload = (ctypes.c_uint8 * (len(planet_body) - 4)).from_buffer_copy(
            planet_body[4:])
        planet_input = ReadPlanetInput()
        planet_input.id_player_word = int.from_bytes(planet_body[:2], "little")
        planet_input.flags_word = int.from_bytes(planet_body[2:4], "little")
        planet_input.header_flags = (planet_record["kind"] << 10) | len(planet_body)
        planet_input.game_turn = planet_record["turn"]
        planet_input.payload = ctypes.cast(
            payload, ctypes.POINTER(ctypes.c_uint8))
        planet_input.payload_size = len(planet_body) - 4
        planet_state = ReadPlanetState()
        self.assertEqual(core.stars_read_planet(
            ctypes.byref(planet_input), ctypes.byref(planet_state),
            1, None, None), 1)
        serialized_flags = planet_input.flags_word
        planet_flags = planet_state.flags
        population = planet_state.minerals[3]
        planet_environment = list(planet_state.environment)
        map_points = xy_point_count(
            ROOT / "runs/vmctl-sye3k0/CLEAN.XY", inspector)
        self.assertEqual(map_points, expected_map_points)
        self.assertEqual((map_points, planet_state.id, planet_state.player,
                          serialized_flags, planet_flags, population),
                         (32, 24, 1, 0x2b87, 0x0f07, 250))
        self.assertEqual(planet_flags,
                         int(production_input["planet_runtime_flags_word"], 16))
        self.assertEqual(population,
                         planet["minerals_and_colonists"][3])
        self.assertEqual(planet_environment, planet["environment"])
        self.assertEqual(len(player_environment), 9)
        self.assertEqual(len(planet_environment), 3)
        input_hst_records = decode_stream(
            ROOT / "runs/vmctl-sye3k0/BACKUP/CLEAN.HST", inspector)
        output_hst_records = decode_stream(
            ROOT / "runs/vmctl-sye3k0/CLEAN.HST", inspector)
        self.assertFalse(any(record["kind"] == 28
                             for record in input_hst_records))
        original_queues = [record["body"].hex()
                           for record in output_hst_records
                           if record["kind"] == 28]
        self.assertEqual(original_queues, [
            oracle["turin_turn_zero_seed_queue_replay"][
                "retained_original_turn1_planet24_queue_payload_hex"]])

        with tempfile.TemporaryDirectory() as temp:
            temp_path = Path(temp)
            fixture = (
                "#define E3_MAP_POINT_COUNT %dU\n"
                "#define E3_PLANET_ID 24\n"
                "#define E3_PLANET_OWNER %d\n"
                "#define E3_PLANET_FLAGS 0x%04xU\n"
                "#define E3_PLANET_POPULATION %dUL\n"
            ) % (map_points, planet["owner"], planet_flags, population)
            fixture += (
                "static const StarsU8 E3_PLANET_ENVIRONMENT[3] = {%s};\n"
                "static const StarsU8 E3_PLAYER_ENVIRONMENT[9] = {%s};\n"
            ) % (", ".join(map(str, planet_environment)),
                 ", ".join(map(str, player_environment)))
            fixture += (
                "#define E3_MINE_EFFICIENCY_TENTHS %d\n"
                "#define E3_MINE_OPERATING_MINES %d\n"
                "#define E3_MINE_SPECIAL_RACE %d\n"
                "static const StarsU8 E3_MINE_CONCENTRATION[3] = {%s};\n"
                "static const StarsU8 E3_MINE_PERCENT_LEVEL[3] = {%s};\n"
                "static const StarsI32 E3_POST_TASK5_MINERALS[3] = {%s};\n"
                "static const StarsI32 E3_PRODUCTION_COSTS[4] = {%s};\n"
            ) % (mining_inputs["player_race_stats"]["mine_output_tenths"],
                 mining_inputs["operating_mines"],
                 int(mining_inputs["player_race_stats"][
                     "special_race_stat_14"] == 8),
                 ", ".join(map(str,
                    mining_inputs["planet24_concentrations"])),
                 ", ".join(map(str,
                    mining_inputs["planet24_percent_levels"])),
                 ", ".join(map(str, post_task5_stock)),
                 ", ".join(map(str, production_costs)))
            (temp_path / "satisfy_orders_turin_seed_produce_fixture.h").write_text(
                fixture, encoding="ascii")
            exe = Path(temp) / "turin_seed_produce"
            subprocess.run([
                "cc", "-std=c89", "-pedantic", "-Wall", "-Wextra",
                "-Werror", "-I", str(ROOT / "reconstructed/include"),
                "-I", str(temp_path),
                str(ROOT / "tests/satisfy_orders_turin_seed_produce_probe.c"),
                str(ROOT / "reconstructed/src/do_turin_drone_ai_turn.c"),
                str(ROOT / "reconstructed/src/ai_plord.c"),
                str(ROOT / "reconstructed/src/est_minerals_mined.c"),
                str(ROOT / "reconstructed/src/produce.c"),
                str(ROOT / "reconstructed/src/c_build_prod_item.c"),
                str(library),
                "-o", str(exe),
            ], check=True)
            for label, run_name, draws in (
                    ("control", "vmctl-sye3k0", (0, 99, 0)),
                    ("action", "vmctl-sye3k1", (0, 0, 0))):
                planet24_bounds = draw_bounds[label][3:6]
                expected_yield = [entry["yield"]
                                  for entry in planet24_bounds]
                for draw, entry in zip(draws, planet24_bounds):
                    lower, upper = entry["random100_range_inclusive"]
                    self.assertGreaterEqual(draw, lower)
                    self.assertLessEqual(draw, upper)
                result = subprocess.run([str(exe), *map(str, draws)],
                    check=False, capture_output=True, text=True)
                self.assertEqual(result.returncode, 0,
                                 result.stdout + result.stderr)
                lines = result.stdout.strip().splitlines()
                self.assertEqual(lines[0],
                    "SOE3_TURIN_SEED_PRODUCE_CHECKS=12 FAILURES=0 "
                    "RESULT=PASS")
                self.assertEqual(lines[1], "TURIN_PLANET24_DESIRABILITY=100")

                output_records = decode_stream(
                    ROOT / "runs" / run_name / "CLEAN.HST", inspector)
                output_planets = [record for record in output_records
                    if record["turn"] == 1 and record["kind"] == 13 and
                    len(record["body"]) >= 2 and
                    (int.from_bytes(record["body"][:2], "little") & 0x7ff)
                    == 24]
                self.assertEqual(len(output_planets), 1)
                output_planet = decode_planet_record(
                    output_planets[0]["body"], expected_id=24)
                output_queues = [record["body"].hex()
                    for record in output_records if record["turn"] == 1 and
                    record["kind"] == 28]
                expected_queue = oracle["turin_turn_zero_seed_queue_replay"][
                    "retained_original_turn1_planet24_queue_payload_hex"]
                self.assertEqual(output_queues, [expected_queue])

                start_stock = post_task5_stock
                prebuild_stock = [start + amount for start, amount in
                                  zip(start_stock, expected_yield)]
                expected_stock = output_planet["minerals"][:3]
                expected_mine_line = (
                    "MINE=%s STOCK=%s LEVELS=%s CONC=%s "
                    "DRAWS=3 FAILURES=0" % (
                        ",".join(map(str, expected_yield)),
                        ",".join(map(str, prebuild_stock)),
                        ",".join(map(str, output_planet["levels"])),
                        ",".join(map(str,
                                   output_planet["concentrations"]))))
                self.assertEqual(lines[2], expected_mine_line)
                self.assertEqual(lines[3],
                    "PRODUCE_MINERALS=%s ITEMS=2 "
                    "ITEM0=0,2,1,81 ITEM1=0,2,1,0" %
                    ",".join(map(str, expected_stock)))
                self.assertEqual(expected_stock,
                    [592, 963, 520] if label == "control"
                    else [592, 964, 520])
                self.assertEqual(output_planet["levels"], [247, 241, 247])
                self.assertEqual(output_planet["concentrations"], [68, 87, 68])


if __name__ == "__main__":
    unittest.main()
