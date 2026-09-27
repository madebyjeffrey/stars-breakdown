"""Compare four-pass task-5, mining and growth from the original HST input.

The mining random draws are an explicit output witness, not recovered seeds.
Other surrounding turn services remain outside this replay.
"""

import hashlib
import json
from pathlib import Path
import struct
import subprocess
import tempfile
import unittest

from tools.race_file_compare import Inspector
from tools.ne_layout import NE
from tools.satisfy_orders_original_oracle import (
    decode_compact_order, decode_owner_fleet, decode_planet_prefix,
)
from tools.stars_game_stream import decode_stream
from tools.satisfy_orders_design_cost_oracle import recover as recover_design
from tools.satisfy_orders_planet10_oracle import (
    decode_planet_record, recover as recover_planet,
)


ROOT = Path(__file__).resolve().parents[1]
RUN = ROOT / "runs/vmctl-syact3"


def c_byte_row(values):
    return "{%s}" % ",".join(str(value) for value in values)


def runtime_fleet_record(fleet):
    raw = bytearray(124)
    struct.pack_into("<HHHhhh", raw, 0, fleet["id"], fleet["owner"],
                     fleet["saved_flags"], fleet["planet"],
                     fleet["point"][0], fleet["point"][1])
    for index, count in enumerate(fleet["ship_counts"]):
        struct.pack_into("<H", raw, 12 + index * 2, count)
    for index, amount in enumerate(fleet["cargo"]):
        struct.pack_into("<I", raw, 76 + index * 4, amount)
    raw[98] = fleet["order_count"]
    # This live-only field is consumed by the current packed turn adapter.
    struct.pack_into("<H", raw, 118, 0x0060)
    return raw


def retained_action_fleet_order_rows(records):
    rows = []
    for index, record in enumerate(records[:-1]):
        if record["turn"] != 0 or record["kind"] != 16:
            continue
        fleet = decode_owner_fleet(record["body"])
        if fleet["owner"] != 0:
            continue
        following = records[index + 1]
        if following["turn"] != 0 or following["kind"] not in (19, 20):
            raise AssertionError("owner-0 input fleet lacks its packed order")
        order = bytearray(18)
        order_body = following["body"]
        order[:min(len(order_body), len(order))] = order_body[:len(order)]
        rows.append((fleet, runtime_fleet_record(fleet), order))
    if [fleet["id"] for fleet, _, _ in rows] != list(range(6)):
        raise AssertionError("retained owner-0 fleet set changed")
    return rows


def retained_planet_runtime_record(planet):
    raw = bytearray(60)
    struct.pack_into("<HHH", raw, 0, 10, planet["owner"], planet["flags"])
    raw[6:9] = bytes(planet["levels"])
    raw[9:12] = bytes(planet["concentrations"])
    raw[0x14] = planet["improvements"]["population_delta"]
    raw[0x15] = planet["improvements"]["mines"]
    raw[0x16] = 160
    raw[0x18] = planet["improvements"]["factories"]
    for index, amount in enumerate(planet["minerals"]):
        struct.pack_into("<I", raw, 0x1c + index * 4, amount)
    return raw


def write_retained_probe_header(temp_path, action_run=RUN,
                               control_run=ROOT / "runs/vmctl-syorig1"):
    inspector = Inspector()
    action_source = decode_stream(action_run / "BACKUP/CLEAN.HST", inspector)
    control_source = decode_stream(control_run / "BACKUP/CLEAN.HST", inspector)
    action_rows = retained_action_fleet_order_rows(action_source)
    control_rows = retained_action_fleet_order_rows(control_source)
    if [row[0] for row in action_rows] != [row[0] for row in control_rows]:
        raise AssertionError("action/control fleet state differs")
    input_planets = [record["body"] for record in control_source
                     if record["turn"] == 0 and record["kind"] == 13 and
                     len(record["body"]) >= 2 and
                     (int.from_bytes(record["body"][:2], "little") & 0x7ff)
                     == 10]
    if len(input_planets) != 1:
        raise AssertionError("retained input planet 10 is not unique")
    input_planet = decode_planet_record(input_planets[0], expected_id=10)
    design_oracle = recover_design()
    input_rows = action_rows
    control_orders = [row[2] for row in control_rows]
    retained_fleets = [row[1] for row in input_rows]
    retained_orders = [row[2] for row in input_rows]
    retained_planet = retained_planet_runtime_record(input_planet)
    retained_design = bytearray(147)
    struct.pack_into("<H", retained_design, 0, design_oracle["hull"]["id"])
    for index, amount in enumerate(design_oracle["owner_design_cost"]):
        struct.pack_into("<H", retained_design, 0x2a + index * 2, amount)
    fixture_lines = [
        "#define RETAINED_PLANET_ID 10",
        "#define RETAINED_PLANET_OWNER %d" % input_planet["owner"],
        "#define RETAINED_PLANET_FLAGS 0x%04xU" % input_planet["flags"],
        "#define RETAINED_CONTROL_ORDERS 1",
        "static const StarsU8 retained_input_fleets[6][124] = {%s};" %
        ",".join(c_byte_row(row) for row in retained_fleets),
        "static const StarsU8 retained_input_orders[6][18] = {%s};" %
        ",".join(c_byte_row(row) for row in retained_orders),
        "static const StarsU8 retained_control_orders[6][18] = {%s};" %
        ",".join(c_byte_row(row) for row in control_orders),
        "static const StarsU8 retained_input_planet[60] = {%s};" %
        ",".join(str(value) for value in retained_planet),
        "static const StarsU8 retained_input_design[147] = {%s};" %
        ",".join(str(value) for value in retained_design),
    ]
    (temp_path / "SATISFYFX.H").write_text(
        "#include \"core.h\"\n" + "\n".join(fixture_lines) + "\n",
        encoding="ascii")


class SatisfyOrdersTask5FourPassDeltaTests(unittest.TestCase):
    @staticmethod
    def _owner_fleets_with_orders(records, owner, turn=1):
        pairs = []
        for index, record in enumerate(records[:-1]):
            if record["turn"] != turn or record["kind"] != 16:
                continue
            fleet = decode_owner_fleet(record["body"])
            if fleet["owner"] != owner:
                continue
            following = records[index + 1]
            if following["turn"] != turn or following["kind"] != 20:
                raise AssertionError("fleet output is not followed by its order")
            fields = ("id", "owner", "planet", "point", "ship_counts",
                      "cargo", "saved_flags", "order_count")
            pairs.append(({field: fleet[field] for field in fields},
                          decode_compact_order(following["body"])))
        return pairs

    def test_owner1_ai_log_provenance_is_explicitly_unresolved(self):
        bundle = json.loads((ROOT / "runs/syorig-action-bundle-1/MANIFEST.JSON")
                            .read_text(encoding="ascii"))
        self.assertNotIn("CLEAN.X2", bundle["files"])
        self.assertNotIn("CLEAN.H2", bundle["files"])

        oracle = json.loads((ROOT /
            "analysis/compat/satisfy-orders-full-turn-e3-original-oracle.json")
            .read_text(encoding="utf-8"))
        audit = oracle["original_batch_replay"]["owner1_ai_log_provenance"]
        self.assertEqual(audit["status"],
            "unresolved; existing original AI output is supporting evidence only")
        self.assertFalse(audit["guest_directory_listing"][
            "listing_contents_retained"])

        inspector = Inspector()
        expected = {
            "control": ("vmctl-syorig1", 356,
                "fb56eb05a6d4a44b0c812242cbebc6b8a8826c1cdc07129dd1bdfa438664fd31"),
            "action": ("vmctl-syact3", 361,
                "fc751635acf7fe9659e460bf7b5789c547335fb12d04a6fecfd8773915e43526"),
        }
        for label, (run, size, digest) in expected.items():
            path = ROOT / "runs" / run / "BACKUP/CLEAN.X2"
            data = path.read_bytes()
            self.assertEqual(len(data), size)
            self.assertEqual(hashlib.sha256(data).hexdigest(), digest)
            records = decode_stream(path, inspector)
            self.assertEqual(len(records), 13)
            self.assertEqual({record["turn"] for record in records}, {0})
            self.assertEqual(audit["retained_x2_backup_artifacts"][label][
                "sha256"], digest)

        expected_h2 = {
            "control": ("vmctl-syorig1", "fdab9ff37560d5de45b310d33a20ca8bce18854210a00839a5a8fb92cf92afc8"),
            "action": ("vmctl-syact3", "bef799175a50f49fadcff1a352bfa9e6644cf2ffbe27862b47cb254cee3f7891"),
        }
        for label, (run, digest) in expected_h2.items():
            path = ROOT / "runs" / run / "CLEAN.H2"
            self.assertEqual(path.stat().st_size, 95)
            self.assertEqual(hashlib.sha256(path.read_bytes()).hexdigest(),
                digest)
            returned = audit["retained_h2_return_artifacts"][label]
            self.assertEqual(returned["sha256"], digest)
            self.assertEqual(returned["role"],
                "returned root file; initial H2 state is unresolved")

    def test_four_pass_action_delta_against_original(self):
        self.assertEqual(hashlib.sha256(
            (RUN / "BACKUP/CLEAN.HST").read_bytes()).hexdigest(),
            "aa6161bb6c95c8276b7a5d12e5a12d9999e3dea0fa242a3be89a94a05c45df3c")
        inspector = Inspector()
        source = decode_stream(RUN / "BACKUP/CLEAN.HST", inspector)
        turn = decode_stream(RUN / "CLEAN.M1", inspector)
        self.assertEqual((source[45]["kind"], source[45]["body"].hex()),
                         (19, "680414050a00051100000000000000000000"))
        before = [decode_owner_fleet(record["body"]) for record in source
                  if record["turn"] == 0 and record["kind"] == 16
                  and int.from_bytes(record["body"][:2], "little") < 6]
        after = [decode_owner_fleet(record["body"]) for record in turn
                 if record["turn"] == 1 and record["kind"] == 16
                 and int.from_bytes(record["body"][:2], "little") < 6]
        self.assertEqual([row["id"] for row in before], list(range(6)))
        self.assertEqual([row["id"] for row in after], list(range(1, 6)))
        self.assertEqual([row["cargo"][4] for row in before],
                         [50, 300, 200, 450, 530, 210])
        for row in after:
            self.assertEqual((row["owner"], row["planet"], row["point"]),
                             (0, 10, [1128, 1300]))
        orders = [decode_compact_order(record["body"]) for record in turn
                  if record["turn"] == 1 and record["kind"] == 20
                  and record["body"][:6] == bytes.fromhex("680414050a00")]
        self.assertEqual(len(orders), 5)
        self.assertTrue(all(order["task"] == 0 and order["valid_task"] == 1
                            for order in orders))
        planet = [record["body"] for record in turn
                  if record["turn"] == 1 and record["kind"] == 13
                  and record["body"][:2] == b"\x0a\x00"]
        self.assertEqual(len(planet), 1)
        original_minerals = [int.from_bytes(planet[0][i:i + 2], "little")
                             for i in (17, 19, 21)]
        input_planet = recover_planet()
        self.assertEqual(input_planet["original_input"]["minerals"],
                         [469, 893, 376, 250])
        design_oracle = recover_design()
        self.assertEqual(design_oracle["owner_design_cost"], [24, 12, 8, 7])
        messages = [record["body"] for record in turn
                    if record["turn"] == 1 and record["kind"] == 12]
        self.assertEqual(len(messages), 1)

        with tempfile.TemporaryDirectory() as temp:
            temp_path = Path(temp)
            write_retained_probe_header(temp_path)
            exe = temp_path / "task5_four_pass"
            subprocess.run([
                "cc", "-std=c89", "-pedantic", "-Wall", "-Wextra", "-Werror",
                "-I", str(temp_path),
                "-I", str(ROOT / "reconstructed/include"),
                str(ROOT / "tests/satisfy_orders_task5_four_pass_probe.c"),
                str(ROOT / "reconstructed/src/do_orders.c"),
                str(ROOT / "reconstructed/src/satisfy_orders.c"),
                str(ROOT / "reconstructed/src/produce.c"),
                str(ROOT / "reconstructed/src/est_minerals_mined.c"),
                str(ROOT / "reconstructed/src/change_population.c"),
                "-o", str(exe),
            ], check=True)
            output = subprocess.check_output([str(exe)], text=True)
        lines = output.splitlines()
        self.assertEqual(lines[:2], ["PASSES=1,2,3,4",
                                     "DONE=1 ORDER0=0 DEAD0=1"])
        self.assertEqual(original_minerals, [485, 907, 388])
        self.assertEqual(lines[2],
                         "PLANET=10 OWNER=0 MINERALS=485,907,388 COLONISTS=287")
        scoped_planet = [int(value) for value in
                         lines[2].split("MINERALS=")[1]
                         .split(" COLONISTS=")[0].split(",")]
        scoped_planet.append(int(lines[2].split("COLONISTS=")[1]))
        self.assertEqual([original - scoped for original, scoped in zip(
            input_planet["action_output"]["minerals"], scoped_planet)],
            [0, 0, 0, 0])
        self.assertEqual(lines[3],
                         "MESSAGES=3 IDS=90,321,63 RECIPIENTS=0,0,0")
        self.assertEqual(lines[4],
                         "ARGS=10,0,20,0,10;10,0,20,0,10")
        self.assertEqual(messages[0][:16].hex(),
                         "5a000a000014000a41010a000014000a")
        self.assertEqual(messages[0][16:].hex(), "3f000a000a")
        self.assertEqual(lines[5], "PRODUCTION_ARGS=10,10,0")
        self.assertEqual(lines[6],
                         "MINING=7,8,7 DRAWS=3 LEVELS=247,241,247 CONC=68,87,68")
        self.assertEqual(input_planet["action_output"]["levels"],
                         [247, 241, 247])
        self.assertEqual(input_planet["action_output"]["concentrations"],
                         [68, 87, 68])
        self.assertEqual(lines[7], "POPULATION_GROWTH=37 FRACTION=50")
        self.assertEqual(input_planet["action_output"]["improvements"][
            "population_delta"], 50)
        expected_fleets = [
            f"FLEET={row['id']} OWNER={row['owner']} PLANET={row['planet']} "
            f"POINT={row['point'][0]},{row['point'][1]} "
            f"SHIPS={','.join(map(str, row['ship_counts']))} "
            f"CARGO={','.join(map(str, row['cargo']))} "
            f"ORDER={order['point'][0]},{order['point'][1]},"
            f"{order['target_id']},{order['flags']}"
            for row, order in zip(after, orders)
        ]
        self.assertEqual(lines[8:13], expected_fleets)
        self.assertEqual(
            lines[13],
            "FAILURE_MESSAGE=0121 RECIPIENT=2 ARGS="
            "800b,000b,0003,001b,0000,000e,0123,0000")

    def test_fresh_pair_owner0_action_runs_all_four_orders_passes(self):
        pair = json.loads((ROOT / "runs/vmctl-sye3k-pair-r1/PAIR.JSON")
                          .read_text(encoding="utf-8"))
        self.assertTrue(pair["input_comparison"][
            "owner1_hst_and_m2_inputs_match"])
        change = pair["input_comparison"]["task5_order_change"]
        self.assertEqual(change["record_index"], 45)
        self.assertEqual(change["control"]["body_hex"],
                         "680414050a000011")
        self.assertEqual(change["action"]["body_hex"],
                         "680414050a00051100000000000000000000")

        inspector = Inspector()
        control_input = decode_stream(
            ROOT / "runs/vmctl-sye3k0/BACKUP/CLEAN.HST", inspector)
        action_input = decode_stream(
            ROOT / "runs/vmctl-sye3k1/BACKUP/CLEAN.HST", inspector)
        control_output = decode_stream(
            ROOT / "runs/vmctl-sye3k0/CLEAN.HST", inspector)
        action_output = decode_stream(
            ROOT / "runs/vmctl-sye3k1/CLEAN.HST", inspector)
        control_m1 = decode_stream(
            ROOT / "runs/vmctl-sye3k0/CLEAN.M1", inspector)
        action_m1 = decode_stream(
            ROOT / "runs/vmctl-sye3k1/CLEAN.M1", inspector)
        self.assertEqual(control_input[45]["body"],
                         bytes.fromhex("680414050a000011"))
        self.assertEqual(action_input[45]["body"],
                         bytes.fromhex("680414050a00051100000000000000000000"))

        def planet10(records, turn):
            matches = [record["body"] for record in records
                if record["turn"] == turn and record["kind"] == 13 and
                len(record["body"]) >= 2 and
                (int.from_bytes(record["body"][:2], "little") & 0x7ff) == 10]
            self.assertEqual(len(matches), 1)
            return decode_planet_record(matches[0], expected_id=10)

        input_planet = planet10(control_input, 0)
        self.assertEqual(input_planet["minerals"], [469, 893, 376, 250])
        self.assertEqual(input_planet, planet10(action_input, 0))
        expected_output = {
            "control": ([476, 902, 383, 287], [0, 0, 0],
                        ["3f000a000a"]),
            "action": ([484, 908, 388, 287], [99, 0, 0],
                       ["5a000a000014000a41010a000014000a3f000a000a"]),
        }
        pair_draws = json.loads((ROOT / "runs/vmctl-sye3k-pair-r1/PAIR.JSON")
                                .read_text(encoding="utf-8"))[
            "mining_random100_threshold_observations"]["outputs"]
        for label, run_name, draws in (
                ("control", "vmctl-sye3k0", (0, 0, 0)),
                ("action", "vmctl-sye3k1", (99, 0, 0))):
            target_minerals, target_draws, target_messages = \
                expected_output[label]
            self.assertEqual(tuple(draws), tuple(target_draws))
            for draw, bound in zip(draws, pair_draws[label][:3]):
                lower, upper = bound["random100_range_inclusive"]
                self.assertGreaterEqual(draw, lower)
                self.assertLessEqual(draw, upper)
            output_records = control_output if label == "control" else action_output
            self.assertEqual(planet10(output_records, 1)["minerals"],
                             target_minerals)
            original_messages = [record["body"].hex() for record in
                (control_m1 if label == "control" else action_m1)
                if record["turn"] == 1 and record["kind"] == 12]
            self.assertEqual(original_messages, target_messages)

        with tempfile.TemporaryDirectory() as temp:
            temp_path = Path(temp)
            write_retained_probe_header(
                temp_path, action_run=ROOT / "runs/vmctl-sye3k1",
                control_run=ROOT / "runs/vmctl-sye3k0")
            exe = temp_path / "fresh_pair_four_pass"
            subprocess.run([
                "cc", "-std=c89", "-pedantic", "-Wall", "-Wextra", "-Werror",
                "-I", str(temp_path),
                "-I", str(ROOT / "reconstructed/include"),
                str(ROOT / "tests/satisfy_orders_task5_four_pass_probe.c"),
                str(ROOT / "reconstructed/src/do_orders.c"),
                str(ROOT / "reconstructed/src/satisfy_orders.c"),
                str(ROOT / "reconstructed/src/produce.c"),
                str(ROOT / "reconstructed/src/est_minerals_mined.c"),
                str(ROOT / "reconstructed/src/change_population.c"),
                "-o", str(exe),
            ], check=True)
            for label, draws in (("control", (0, 0, 0)),
                                 ("action", (99, 0, 0))):
                output = subprocess.check_output(
                    [str(exe), label, *map(str, draws)], text=True)
                lines = output.splitlines()
                target_minerals, _target_draws, target_messages = \
                    expected_output[label]
                self.assertEqual(lines[:3], [
                    "PASSES=1,2,3,4",
                    "DONE=%d ORDER0=%d DEAD0=%d" %
                        (0 if label == "control" else 1,
                         1 if label == "control" else 0,
                         0 if label == "control" else 1),
                    "PLANET=10 OWNER=0 MINERALS=%s COLONISTS=287" %
                        ",".join(map(str, target_minerals[:3])),
                ])
                if label == "control":
                    self.assertEqual(lines[3],
                        "MESSAGES=1 IDS=63,0,0 RECIPIENTS=0,0,0")
                    self.assertEqual(lines[5], "PRODUCTION_ARGS=10,10,0")
                else:
                    self.assertEqual(lines[3],
                        "MESSAGES=3 IDS=90,321,63 RECIPIENTS=0,0,0")
                    self.assertEqual(lines[4],
                        "ARGS=10,0,20,0,10;10,0,20,0,10")
                    self.assertEqual(lines[5], "PRODUCTION_ARGS=10,10,0")
                self.assertEqual(lines[6],
                    "MINING=%s DRAWS=3 LEVELS=247,241,247 CONC=68,87,68" %
                    ("7,9,7" if label == "control" else "6,9,7"))
                output_records = (control_output if label == "control"
                                  else action_output)
                fleet_pairs = self._owner_fleets_with_orders(
                    output_records, 0, turn=1)
                expected_ids = (list(range(6)) if label == "control"
                                else list(range(1, 6)))
                self.assertEqual([fleet["id"] for fleet, _ in fleet_pairs],
                                 expected_ids)
                expected_fleets = [
                    "FLEET=%d OWNER=%d PLANET=%d POINT=%d,%d SHIPS=%s "
                    "CARGO=%s ORDER=%d,%d,%d,%d" % (
                        fleet["id"], fleet["owner"], fleet["planet"],
                        fleet["point"][0], fleet["point"][1],
                        ",".join(map(str, fleet["ship_counts"])),
                        ",".join(map(str, fleet["cargo"])),
                        order["point"][0], order["point"][1],
                        order["target_id"], order["flags"])
                    for fleet, order in fleet_pairs if fleet["id"] != 0
                ]
                self.assertEqual(lines[8:13], expected_fleets)
                self.assertEqual([record for record in lines
                                  if record.startswith("FAILURE_MESSAGE=")],
                    ["FAILURE_MESSAGE=0121 RECIPIENT=2 ARGS="
                     "800b,000b,0003,001b,0000,000e,0123,0000"])
                self.assertTrue(target_messages)

    def test_other_owner_state_is_pinned_across_action_and_control(self):
        inspector = Inspector()
        action = ROOT / "runs/vmctl-syact3"
        control = ROOT / "runs/vmctl-syorig1"
        expected = [
            ({"id": 0, "owner": 1, "planet": 26, "point": [1313, 1262],
              "ship_counts": [1] + [0] * 15,
              "cargo": [0, 0, 0, 0, 298], "saved_flags": 0x0907,
              "order_count": 1},
             {"point": [1313, 1262], "target_id": 26, "flags": 0x1150,
              "task": 0, "warp": 5, "object_kind": 1, "valid_task": 1}),
            ({"id": 1, "owner": 1, "planet": 24, "point": [1290, 1258],
              "ship_counts": [0, 1] + [0] * 14,
              "cargo": [0, 0, 0, 25, 200], "saved_flags": 0x2907,
              "order_count": 1},
             {"point": [1290, 1258], "target_id": 24, "flags": 0x1100,
              "task": 0, "warp": 0, "object_kind": 1, "valid_task": 1}),
        ]

        def read_records(directory, name):
            return decode_stream(directory / name, inspector)

        action_hst = read_records(action, "CLEAN.HST")
        control_hst = read_records(control, "CLEAN.HST")
        action_m2 = read_records(action, "CLEAN.M2")
        control_m2 = read_records(control, "CLEAN.M2")
        action_m1 = read_records(action, "CLEAN.M1")
        control_m1 = read_records(control, "CLEAN.M1")

        # The original owner-1 turn input is the same in both control/action
        # runs. M2's four fleet/order pairs also match the HST snapshot, so
        # there is no hidden owner-1 order-log difference behind the output.
        action_input_hst = read_records(action / "BACKUP", "CLEAN.HST")
        control_input_hst = read_records(control / "BACKUP", "CLEAN.HST")
        action_input_m2 = read_records(action / "BACKUP", "CLEAN.M2")
        control_input_m2 = read_records(control / "BACKUP", "CLEAN.M2")
        owner1_input = self._owner_fleets_with_orders(
            action_input_hst, 1, turn=0)
        self.assertEqual([fleet["id"] for fleet, _ in owner1_input],
                         [0, 1, 2, 3])
        self.assertEqual(self._owner_fleets_with_orders(
            control_input_hst, 1, turn=0), owner1_input)
        self.assertEqual(self._owner_fleets_with_orders(
            action_input_m2, 1, turn=0), owner1_input)
        self.assertEqual(self._owner_fleets_with_orders(
            control_input_m2, 1, turn=0), owner1_input)

        def read_owner1_player(records, turn):
            matches = [record["body"] for record in records
                       if record["turn"] == turn and record["kind"] == 6
                       and record["body"] and record["body"][0] == 1]
            self.assertEqual(len(matches), 1)
            return matches[0]

        owner1_player_input = read_owner1_player(action_input_hst, 0)
        self.assertEqual(read_owner1_player(control_input_hst, 0),
                         owner1_player_input)
        player_mode = int.from_bytes(owner1_player_input[6:8], "little")
        self.assertEqual(player_mode, 0x2797)
        # M2's player record has a different planet count from HST; compare
        # only the AI mode word shared by the two input formats.
        self.assertEqual(int.from_bytes(
            read_owner1_player(action_input_m2, 0)[6:8], "little"), player_mode)
        self.assertEqual(int.from_bytes(
            read_owner1_player(control_input_m2, 0)[6:8], "little"), player_mode)
        self.assertEqual((player_mode >> 9) & 1, 1)  # fAi
        self.assertEqual((player_mode >> 10) & 7, 1)  # lvlAi
        self.assertEqual((player_mode >> 13) & 7, 1)  # idAi

        self.assertEqual(hashlib.sha256(
            (control / "BACKUP/CLEAN.M2").read_bytes()).hexdigest(),
            "588830ac2225056422fa447cb201c7bd326e472652a8d3abf222b5fcceb99d4b")
        self.assertEqual(hashlib.sha256(
            (action / "BACKUP/CLEAN.M2").read_bytes()).hexdigest(),
            "588830ac2225056422fa447cb201c7bd326e472652a8d3abf222b5fcceb99d4b")

        def read_planet24(records, turn):
            matches = [record["body"] for record in records
                       if record["turn"] == turn and record["kind"] == 13
                       and len(record["body"]) > 20
                       and decode_planet_prefix(record["body"])["id"] == 24]
            self.assertEqual(len(matches), 1)
            return decode_planet_record(matches[0], expected_id=24)

        input_planet24 = read_planet24(action_input_hst, 0)
        self.assertEqual(read_planet24(control_input_hst, 0), input_planet24)
        self.assertEqual(read_planet24(action_input_m2, 0), input_planet24)
        self.assertEqual(read_planet24(control_input_m2, 0), input_planet24)
        self.assertEqual(
            (input_planet24["owner"], input_planet24["flags"],
             input_planet24["levels"], input_planet24["concentrations"],
             input_planet24["environment"], input_planet24["auxiliary_word"],
             input_planet24["minerals"], input_planet24["improvements"]),
            (1, 0x2b87, [0, 0, 0], [68, 87, 68], [62, 33, 61], 62,
             [532, 956, 500, 250],
             {"population_delta": 0, "mines": 10, "factories": 10,
              "defenses": 10}))
        action_planet24 = read_planet24(action_hst, 1)
        control_planet24 = read_planet24(control_hst, 1)
        self.assertEqual(read_planet24(action_m2, 1), action_planet24)
        self.assertEqual(read_planet24(control_m2, 1), control_planet24)
        self.assertEqual(
            (control_planet24["owner"], control_planet24["flags"],
             control_planet24["levels"], control_planet24["concentrations"],
             control_planet24["environment"], control_planet24["auxiliary_word"],
             control_planet24["minerals"], control_planet24["improvements"]),
            (1, 0x2b87, [247, 241, 247], [68, 87, 68], [62, 33, 61], 8257,
             [592, 963, 520, 256],
             {"population_delta": 50, "mines": 10, "factories": 10,
              "defenses": 10}))
        self.assertEqual(
            (action_planet24["owner"], action_planet24["flags"],
             action_planet24["levels"], action_planet24["concentrations"],
             action_planet24["environment"], action_planet24["auxiliary_word"],
             action_planet24["minerals"], action_planet24["improvements"]),
            (1, 0x2b87, [247, 241, 247], [68, 87, 68], [62, 33, 61], 8255,
             [591, 964, 520, 256],
             {"population_delta": 50, "mines": 10, "factories": 10,
              "defenses": 10}))
        self.assertEqual([action - control for action, control in zip(
            action_planet24["minerals"], control_planet24["minerals"])],
            [-1, 1, 0, 0])
        self.assertEqual(action_planet24["auxiliary_word"] -
                         control_planet24["auxiliary_word"], -2)

        # The task-5 action changes player 0's first fleet. These two player-1
        # fleet/order records are unchanged across the original control/action
        # runs and are duplicated consistently in HST and M2.
        for records in (action_hst, control_hst, action_m2, control_m2):
            self.assertEqual(self._owner_fleets_with_orders(records, 1),
                             expected)
        self.assertEqual(self._owner_fleets_with_orders(action_hst, 1),
                         self._owner_fleets_with_orders(action_m2, 1))
        self.assertEqual(self._owner_fleets_with_orders(control_hst, 1),
                         self._owner_fleets_with_orders(control_m2, 1))

        # Pin that the original matched HST input contained four owner-1 fleets;
        # the full turn resolves that group down to the two records above.
        input_fleets = [fleet for fleet, _ in owner1_input]
        self.assertEqual([fleet["id"] for fleet in input_fleets
                          if fleet["owner"] == 1], [0, 1, 2, 3])

        for records in (action_hst, control_hst, action_m2, control_m2):
            owner_planets = [decode_planet_prefix(record["body"])
                             for record in records
                             if record["turn"] == 1 and record["kind"] == 13
                             and len(record["body"]) > 20
                             and decode_planet_prefix(record["body"])["id"] == 24]
            self.assertEqual(len(owner_planets), 1)
            planet = owner_planets[0]
            self.assertEqual((planet["id"], planet["owner"], planet["flags"],
                              planet["mineral_levels"],
                              planet["mineral_concentrations"],
                              planet["environment"]),
                             (24, 1, 0x2b87, [247, 241, 247],
                              [68, 87, 68], [62, 33, 61]))

        action_messages = [record["body"].hex() for record in action_m1
                           if record["turn"] == 1 and record["kind"] == 12]
        control_messages = [record["body"].hex() for record in control_m1
                            if record["turn"] == 1 and record["kind"] == 12]
        self.assertEqual(action_messages,
                         ["5a000a000014000a41010a000014000a3f000a000a"])
        self.assertEqual(control_messages, ["3f000a000a"])

        # This pins more of the original output oracle; it does not claim that
        # the current-source probe simulates player-1 movement/merging.

    def test_owner1_mining_stage_is_replayed_from_retained_input(self):
        input_hst = ROOT / "runs/vmctl-sye3k0/BACKUP/CLEAN.HST"
        input_m2 = ROOT / "runs/vmctl-sye3k0/BACKUP/CLEAN.M2"
        self.assertEqual(hashlib.sha256(input_hst.read_bytes()).hexdigest(),
            "74a27a31221f67328a98200c5dfde54f4c955a6498cc37ac08624b64a33997f6")
        self.assertEqual(hashlib.sha256(input_m2.read_bytes()).hexdigest(),
            "588830ac2225056422fa447cb201c7bd326e472652a8d3abf222b5fcceb99d4b")

        inspector = Inspector()
        hst_records = decode_stream(input_hst, inspector)
        m2_records = decode_stream(input_m2, inspector)
        hst_planets = [decode_planet_record(record["body"], expected_id=24)
                       for record in hst_records if record["kind"] == 13 and
                       record["turn"] == 0 and
                       decode_planet_prefix(record["body"])["id"] == 24]
        m2_planets = [decode_planet_record(record["body"], expected_id=24)
                      for record in m2_records if record["kind"] == 13 and
                      record["turn"] == 0 and
                      decode_planet_prefix(record["body"])["id"] == 24]
        players = [record["body"] for record in m2_records
                   if record["kind"] == 6 and record["body"] and
                   record["body"][0] == 1]
        self.assertEqual(len(hst_planets), 1)
        self.assertEqual(len(m2_planets), 1)
        self.assertEqual(hst_planets, m2_planets)
        self.assertEqual(len(players), 1)
        planet = m2_planets[0]
        player = players[0]
        self.assertEqual((planet["owner"], planet["concentrations"],
                          planet["levels"], planet["minerals"],
                          planet["improvements"]["mines"]),
                         (1, [68, 87, 68], [0, 0, 0],
                          [532, 956, 500, 250], 10))
        mine_efficiency = player[0x3E + 4]
        mine_operating_limit = player[0x3E + 6]
        special_race = int(player[0x3E + 14] == 8)
        max_planet_population = 10000
        max_mines = max(10, max_planet_population * mine_operating_limit // 100)
        mines_by_population = max(1, min(max_mines,
            planet["minerals"][3] * mine_operating_limit // 100))
        operating_mines = min(planet["improvements"]["mines"],
                              mines_by_population)
        self.assertEqual((mine_efficiency, mine_operating_limit,
                          special_race, mines_by_population,
                          operating_mines), (10, 9, 0, 22, 10))

        queue_payloads = []
        for run_name in ("vmctl-sye3k0", "vmctl-sye3k1"):
            records = decode_stream(ROOT / "runs" / run_name / "CLEAN.HST",
                                    inspector)
            planet_indices = [index for index, record in enumerate(records)
                              if record["turn"] == 1 and record["kind"] == 13
                              and decode_planet_prefix(record["body"])["id"]
                              == 24]
            self.assertEqual(len(planet_indices), 1)
            queue_index = planet_indices[0] + 1
            self.assertEqual((records[queue_index]["turn"],
                              records[queue_index]["kind"]), (1, 28))
            queue_payloads.append(records[queue_index]["body"])
        self.assertEqual(queue_payloads,
                         [bytes.fromhex("0100140501000400")] * 2)
        queue_items = []
        for offset in range(0, len(queue_payloads[0]), 4):
            item = int.from_bytes(queue_payloads[0][offset:offset + 2],
                                  "little")
            item |= int.from_bytes(queue_payloads[0][offset + 2:offset + 4],
                                   "little") << 16
            queue_items.append(((item >> 17) & 7, (item >> 10) & 0x7f,
                               item & 0x3ff, (item >> 20) & 0x7f))
        self.assertEqual(queue_items, [(2, 0, 1, 81), (2, 0, 1, 0)])

        exe_path = ROOT / "target/stars.exe"
        self.assertEqual(hashlib.sha256(exe_path.read_bytes()).hexdigest(),
            "ccab6682b55dae14dbf104e491cfbd3b32a6280022020032ae0615500949ba59")
        segment = bytes(NE(exe_path).raw[1])
        design_records = [record["body"] for record in m2_records
                          if record["turn"] == 0 and record["kind"] == 26
                          and ((int.from_bytes(record["body"][:2], "little")
                                >> 10) & 31) == 0]
        self.assertEqual(len(design_records), 1)
        design = design_records[0]
        self.assertEqual((int.from_bytes(design[:2], "little"), design[2],
                          design[6]), (0x0107, 4, 3))
        parts = [struct.unpack_from("<HH", design, 17 + index * 4)
                 for index in range(design[6])]
        components = [(group, packed & 0xff, packed >> 8)
                       for group, packed in parts]
        self.assertEqual(components, [(1, 1, 1), (2, 4, 1),
                                      (0x1000, 5, 1)])
        player_tech = list(player[26:32])
        race_bits = int.from_bytes(player[78:82], "little")
        self.assertEqual(player_tech, [0, 0, 1, 0, 5, 0])
        self.assertEqual((race_bits >> 8) & 1, 0)
        self.assertEqual((race_bits >> 12) & 1, 0)
        self.assertEqual(player[76], 1)
        part_tables = {1: (0x0000, 0x4e), 2: (0x0768, 0x38),
                       0x1000: (0x109a, 0x36)}
        hull_offset = 0x29f0 + design[2] * 0x8f
        base_cost = list(struct.unpack_from("<4H", segment,
                                             hull_offset + 0x2a))
        for group, item, count in components:
            table, stride = part_tables[group]
            offset = table + item * stride
            required_tech = list(segment[offset + 2:offset + 8])
            if any(required_tech):
                progress = min(player_tech[index] - required_tech[index]
                               for index in range(6)
                               if required_tech[index] > 0)
            else:
                progress = min(player_tech)
            self.assertLessEqual(progress, 0)
            cost = struct.unpack_from("<4H", segment, offset + 0x2a)
            for resource in range(4):
                base_cost[resource] += count * cost[resource]
        self.assertEqual(base_cost, [35, 16, 2, 8])
        production_cost = base_cost[1:] + base_cost[:1]
        self.assertEqual(production_cost, [16, 2, 8, 35])
        partial_cost_at_81_percent = [cost * 81 // 100
                                      for cost in production_cost]
        self.assertEqual(partial_cost_at_81_percent, [12, 1, 6, 28])
        ore_spent_at_81_percent = partial_cost_at_81_percent[:3]
        self.assertEqual(ore_spent_at_81_percent, [12, 1, 6])

        output_stocks = {}
        for label, run_name in (("control", "vmctl-sye3k0"),
                                ("action", "vmctl-sye3k1")):
            records = decode_stream(ROOT / "runs" / run_name / "CLEAN.HST",
                                    inspector)
            matches = [decode_planet_record(record["body"], expected_id=24)
                       for record in records if record["turn"] == 1 and
                       record["kind"] == 13 and len(record["body"]) > 20 and
                       decode_planet_prefix(record["body"])["id"] == 24]
            self.assertEqual(len(matches), 1)
            output_stocks[label] = matches[0]["minerals"][:3]
            self.assertEqual(matches[0]["levels"], [247, 241, 247])
        self.assertEqual(output_stocks,
                         {"control": [592, 963, 520],
                          "action": [592, 964, 520]})
        scrap_stock = [597, 956, 519]
        pre_production_stocks = {
            label: [output + spent for output, spent in zip(stocks,
                ore_spent_at_81_percent)]
            for label, stocks in output_stocks.items()}
        self.assertEqual(pre_production_stocks,
                         {"control": [604, 964, 526],
                          "action": [604, 965, 526]})
        inferred_mining = {
            label: [stock - before for stock, before in zip(stocks,
                scrap_stock)]
            for label, stocks in pre_production_stocks.items()}
        self.assertEqual(inferred_mining,
                         {"control": [7, 8, 7], "action": [7, 9, 7]})
        self.assertTrue(all(6 <= value <= 7 for value in
                            (inferred_mining["control"][0],
                             inferred_mining["action"][0])))
        self.assertTrue(all(8 <= value <= 9 for value in
                            (inferred_mining["control"][1],
                             inferred_mining["action"][1])))
        self.assertTrue(all(6 <= value <= 7 for value in
                            (inferred_mining["control"][2],
                             inferred_mining["action"][2])))
        oracle = json.loads((ROOT /
            "analysis/compat/satisfy-orders-full-turn-e3-original-oracle.json")
            .read_text(encoding="utf-8"))
        mining_audit = oracle["current_source_control_action_pair"][
            "owner1_planet24"]["production_mining_stage"]
        self.assertEqual(mining_audit["fresh_isolated_pair_turn1_stocks"], {
            "control_run": "runs/vmctl-sye3k0/CLEAN.HST",
            "action_run": "runs/vmctl-sye3k1/CLEAN.HST",
            "control": output_stocks["control"],
            "action": output_stocks["action"],
            "difference_action_minus_control": [0, 1, 0],
        })
        self.assertEqual(mining_audit["owner1_exact_mineral_stock_witness"]
            ["inferred_mining_witnesses"], inferred_mining)
        exact_stock = mining_audit["owner1_exact_mineral_stock_witness"]
        self.assertEqual(exact_stock[
            "original_hull_and_parts_costs_table_order"], base_cost)
        self.assertEqual(exact_stock["runtime_production_cost_order"],
                         production_cost)
        self.assertEqual(exact_stock[
            "partial_build_runtime_cost_floor"], partial_cost_at_81_percent)

        def byte_array(name, values):
            return ("static const StarsU8 %s[%d] = {%s};\n" %
                    (name, len(values), ",".join(str(value)
                                                 for value in values)))

        with tempfile.TemporaryDirectory() as temp:
            temp_path = Path(temp)
            header = [
                "#define retained_efficiency_tenths %d" % mine_efficiency,
                "#define retained_operating_mines %d" % operating_mines,
                "#define retained_special_race %d" % special_race,
                "static const StarsI32 retained_stock[3] = {%s};" %
                ",".join(str(value) for value in planet["minerals"][:3]),
                byte_array("retained_concentration",
                           planet["concentrations"]),
                byte_array("retained_percent_level", planet["levels"]),
            ]
            (temp_path / "TURINMINE.H").write_text(
                "\n".join(header) + "\n", encoding="ascii")
            exe = temp_path / "turin_mining"
            subprocess.run([
                "cc", "-std=c89", "-pedantic", "-Wall", "-Wextra",
                "-Werror", "-I", str(ROOT / "reconstructed/include"),
                "-I", str(temp_path),
                str(ROOT / "tests/satisfy_orders_turin_mining_e3_probe.c"),
                str(ROOT / "reconstructed/src/est_minerals_mined.c"),
                "-o", str(exe),
            ], check=True)
            low = subprocess.check_output(
                [str(exe), "0", "99", "0"], text=True).strip()
            high = subprocess.check_output(
                [str(exe), "99", "0", "99"], text=True).strip()
            action_witness = subprocess.check_output(
                [str(exe), "99", "0", "0"], text=True).strip()

        self.assertEqual(low,
            "MINING=7,8,7 STOCK=539,964,507 LEVELS=247,241,247 "
            "CONC=68,87,68 DRAWS=3 FAILURES=0")
        self.assertEqual(high,
            "MINING=6,9,6 STOCK=538,965,506 LEVELS=247,241,247 "
            "CONC=68,87,68 DRAWS=3 FAILURES=0")
        self.assertEqual(action_witness,
            "MINING=6,9,7 STOCK=538,965,507 LEVELS=247,241,247 "
            "CONC=68,87,68 DRAWS=3 FAILURES=0")

    def test_current_orders_only_replay_exposes_owner1_turn_gap(self):
        action = RUN
        original = decode_stream(action / "CLEAN.HST", Inspector())
        expected = self._owner_fleets_with_orders(original, 1)
        self.assertEqual([fleet["id"] for fleet, _ in expected], [0, 1])

        with tempfile.TemporaryDirectory() as temp:
            exe = Path(temp) / "owner1_turn_gap"
            subprocess.run([
                "cc", "-std=c89", "-pedantic", "-Wall", "-Wextra", "-Werror",
                "-I", str(ROOT / "reconstructed/include"),
                str(ROOT / "tests/satisfy_orders_task5_other_owner_probe.c"),
                str(ROOT / "reconstructed/src/do_orders.c"),
                str(ROOT / "reconstructed/src/satisfy_orders.c"),
                str(ROOT / "reconstructed/src/est_minerals_mined.c"),
                str(ROOT / "reconstructed/src/change_population.c"),
                "-o", str(exe),
            ], check=True)
            output = subprocess.check_output([str(exe)], text=True)

        source_ids = [int(line.split("id=")[1].split()[0])
                      for line in output.splitlines()]
        self.assertEqual(source_ids, [0, 1, 2, 3])
        source_fleet0, source_fleet1 = output.splitlines()[:2]
        self.assertIn("point=1290,1258", source_fleet0)
        self.assertIn("CARGO=0,0,0,0,300", source_fleet0)
        self.assertIn("CARGO=0,0,0,0,200", source_fleet1)
        self.assertIn("ORDER=1290,1258,24,4352", source_fleet0)

        original_fleet0 = expected[0][0]
        original_fleet1 = expected[1][0]
        self.assertEqual(original_fleet0["point"], [1313, 1262])
        self.assertEqual(original_fleet0["cargo"], [0, 0, 0, 0, 298])
        self.assertEqual(original_fleet1["cargo"], [0, 0, 0, 25, 200])

        # This is a retained mismatch report, not a passing full-turn claim:
        # the probe runs task-order processing only and does not model the
        # other original order and turn services that produce this output.

    def test_source_control_action_pair_matches_original_owner0_delta(self):
        with tempfile.TemporaryDirectory() as temp:
            temp_path = Path(temp)
            write_retained_probe_header(temp_path)
            exe = temp_path / "task5_control_action"
            subprocess.run([
                "cc", "-std=c89", "-pedantic", "-Wall", "-Wextra", "-Werror",
                "-I", str(temp_path),
                "-I", str(ROOT / "reconstructed/include"),
                str(ROOT / "tests/satisfy_orders_task5_four_pass_probe.c"),
                str(ROOT / "reconstructed/src/do_orders.c"),
                str(ROOT / "reconstructed/src/satisfy_orders.c"),
                str(ROOT / "reconstructed/src/produce.c"),
                str(ROOT / "reconstructed/src/est_minerals_mined.c"),
                str(ROOT / "reconstructed/src/change_population.c"),
                "-o", str(exe),
            ], check=True)
            control_lines = subprocess.check_output(
                [str(exe), "control"], text=True).splitlines()
            action_lines = subprocess.check_output([str(exe)],
                                                   text=True).splitlines()

        self.assertEqual(control_lines[0], "PASSES=1,2,3,4")
        self.assertEqual(control_lines[1], "DONE=0 ORDER0=1 DEAD0=0")
        self.assertEqual(action_lines[1], "DONE=1 ORDER0=0 DEAD0=1")
        self.assertEqual(control_lines[2],
                         "PLANET=10 OWNER=0 MINERALS=476,902,383 COLONISTS=287")
        self.assertEqual(action_lines[2],
                         "PLANET=10 OWNER=0 MINERALS=485,907,388 COLONISTS=287")

        original_planet = recover_planet()
        self.assertEqual(original_planet["clean_output"]["minerals"],
                         [476, 902, 383, 287])
        self.assertEqual(original_planet["action_output"]["minerals"],
                         [485, 907, 388, 287])
        self.assertEqual([int(value) for value in
                          action_lines[2].split("MINERALS=")[1]
                          .split(" COLONISTS=")[0].split(",")] +
                         [int(action_lines[2].split("COLONISTS=")[1])],
                         original_planet["action_output"]["minerals"])

        self.assertTrue(control_lines[3].startswith(
            "MESSAGES=1 IDS=63,0,0 RECIPIENTS=0,0,0"))
        self.assertTrue(action_lines[3].startswith(
            "MESSAGES=3 IDS=90,321,63 RECIPIENTS=0,0,0"))
        self.assertEqual(control_lines[-1],
                         "FAILURE_MESSAGE=0121 RECIPIENT=2 ARGS="
                         "800b,000b,0003,001b,0000,000e,0123,0000")
        self.assertEqual(action_lines[-1], control_lines[-1])
        self.assertEqual([record["body"].hex() for record in decode_stream(
            RUN / "CLEAN.M1", Inspector()) if record["turn"] == 1 and
            record["kind"] == 12], ["5a000a000014000a41010a000014000a3f000a000a"])
        self.assertEqual([record["body"].hex() for record in decode_stream(
            ROOT / "runs/vmctl-syorig1/CLEAN.M1", Inspector())
                         if record["turn"] == 1 and record["kind"] == 12],
                         ["3f000a000a"])

        # The two separate mining witnesses reproduce the observed original
        # outputs; neither witness is claimed as the original RNG seed.


if __name__ == "__main__":
    unittest.main()
