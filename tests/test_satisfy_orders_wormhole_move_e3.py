"""Replay retained turn-one wormhole changes through production MoveThings."""

import json
from pathlib import Path
import struct
import subprocess
import tempfile
import unittest

from tests.test_satisfy_orders_turin_binding import Inspector, xy_points
from tools.satisfy_orders_original_oracle import decode_owner_fleet
from tools.stars_game_stream import decode_stream


ROOT = Path(__file__).resolve().parents[1]
PAIR = ROOT / "runs/vmctl-sye3k-pair-r1/PAIR.JSON"
WORMHOLE_COUNT = 4


def thing_records(path, turn):
    records = decode_stream(path, Inspector())
    rows = [row["body"] for row in records
            if row["kind"] == 43 and row["turn"] == turn]
    if not rows or len(rows[0]) != 2:
        raise AssertionError("missing retained HST thing-count record")
    count, = struct.unpack("<H", rows[0])
    things = rows[1:]
    if count != WORMHOLE_COUNT or len(things) != count or any(
            len(body) != 18 for body in things):
        raise AssertionError("unexpected retained wormhole records")
    if any((struct.unpack_from("<H", body)[0] >> 13) != 2
           for body in things):
        raise AssertionError("retained thing list is not four wormholes")
    return things


def fleet_points(path, turn):
    records = decode_stream(path, Inspector())
    points = []
    for row in records:
        if row["kind"] == 16 and row["turn"] == turn:
            fleet = decode_owner_fleet(row["body"])
            points.append((fleet["point"][0], fleet["point"][1]))
    if not points:
        raise AssertionError("retained HST has no fleet positions")
    return points


def c_rows(rows):
    return ",\n    ".join("{" + ", ".join(
        "0x%02xU" % byte for byte in row) + "}" for row in rows)


def make_header(input_things, output_things, planets, fleets):
    if len(input_things) != WORMHOLE_COUNT or len(output_things) != WORMHOLE_COUNT:
        raise AssertionError("wormhole pair has the wrong record count")
    draws = []
    limits = []
    expected_x = []
    expected_y = []
    expected_destination = []
    for before, after in zip(input_things, output_things):
        before_x, before_y = struct.unpack_from("<hh", before, 2)
        after_x, after_y = struct.unpack_from("<hh", after, 2)
        packed_destination, = struct.unpack_from("<H", before, 6)
        after_destination, = struct.unpack_from("<H", after, 6)
        chance = ((packed_destination >> 2) & 0x03ff) // 5 - (
            2 - (packed_destination & 3))
        chance = max(0, min(6, chance))
        local_x = after_x - before_x + 12
        local_y = after_y - before_y + 12
        if not (0 <= local_x < 25 and 0 <= local_y < 25):
            raise AssertionError("output is outside local jitter bounds")
        expected = ((packed_destination & 0xf003) |
                    ((packed_destination + 4) & 0x0ffc))
        if after_destination != expected:
            raise AssertionError("retained wormhole destination word mismatch")
        reconstructed = bytearray(before)
        struct.pack_into("<hhH", reconstructed, 2,
                         after_x, after_y, after_destination)
        if reconstructed != after:
            raise AssertionError("unexpected non-movement wormhole fields")
        limits.extend((100, 25, 25))
        draws.extend((chance, local_x, local_y))
        expected_x.append(after_x)
        expected_y.append(after_y)
        expected_destination.append(after_destination)

    lines = [
        "#define WORMHOLE_COUNT %d" % WORMHOLE_COUNT,
        "#define WORMHOLE_DRAW_COUNT %d" % len(draws),
        "#define WORMHOLE_MAP_SIZE %d" % len(planets),
        "#define WORMHOLE_PLANET_COUNT %d" % len(planets),
        "#define WORMHOLE_FLEET_COUNT %d" % len(fleets),
        "static const StarsU8 retained_input_things[WORMHOLE_COUNT][18] = {",
        "    %s" % c_rows(input_things),
        "};",
        "static const StarsU8 retained_output_things[WORMHOLE_COUNT][18] = {",
        "    %s" % c_rows(output_things),
        "};",
        "static const StarsI16 retained_draw_limits[WORMHOLE_DRAW_COUNT] = {%s};" %
        ", ".join(map(str, limits)),
        "static const StarsI16 retained_draws[WORMHOLE_DRAW_COUNT] = {%s};" %
        ", ".join(map(str, draws)),
        "static const StarsI16 retained_expected_x[WORMHOLE_COUNT] = {%s};" %
        ", ".join(map(str, expected_x)),
        "static const StarsI16 retained_expected_y[WORMHOLE_COUNT] = {%s};" %
        ", ".join(map(str, expected_y)),
        "static const StarsU16 retained_expected_destination[WORMHOLE_COUNT] = {%s};" %
        ", ".join("0x%04xU" % value for value in expected_destination),
        "static const StarsNearestPoint retained_planets[WORMHOLE_PLANET_COUNT] = {",
        "    %s" % ", ".join("{%d, %d}" % point for point in planets),
        "};",
        "static const StarsNearestPoint retained_fleets[WORMHOLE_FLEET_COUNT] = {",
        "    %s" % ", ".join("{%d, %d}" % point for point in fleets),
        "};",
    ]
    return "\n".join(lines) + "\n", draws, limits


def source_brace_block(source, marker):
    start = source.index(marker)
    opening = source.index("{", start)
    depth = 0
    for offset in range(opening, len(source)):
        if source[offset] == "{":
            depth += 1
        elif source[offset] == "}":
            depth -= 1
            if depth == 0:
                return source[opening:offset + 1]
    raise AssertionError("unterminated original-source block")


class SatisfyOrdersWormholeMoveE3Tests(unittest.TestCase):
    def test_retained_thing_table_skips_minefield_random_paths(self):
        turn = (ROOT / "analysis/decompiler/export/raw/"
                "10b0_0000_FGenerateTurn.c").read_text()
        orders = (ROOT / "analysis/decompiler/export/raw/"
                  "10b0_179a_DoOrders.c").read_text()
        move_things = (ROOT / "analysis/decompiler/export/raw/"
                       "10b0_18f4_MoveThings.c").read_text()
        move_fleets = (ROOT / "analysis/decompiler/export/raw/"
                       "10b0_32ce_MoveFleets.c").read_text()
        keep_fleets = (ROOT / "analysis/decompiler/export/raw/"
                       "1090_45ca_KeepFleetsMoving.c").read_text()
        ideal_speed = (ROOT / "analysis/decompiler/export/raw/"
                       "1090_1d9e_SetAiFleetIdealSpeed.c").read_text()
        minefield_travel = (ROOT / "analysis/decompiler/export/raw/"
                            "10b0_4f60_FTravelThroughMineFields.c").read_text()
        interactions = (ROOT / "analysis/decompiler/export/raw/"
                        "1110_0b3a_DoThingInteractions.c").read_text()

        self.assertIn("MoveThings(0);", turn)
        self.assertIn("MoveThings(1);", turn)
        self.assertIn("MoveFleets();", turn)
        self.assertIn("DoThingInteractions(fPostMovement);", orders)
        self.assertIn("FTravelThroughMineFields(", move_fleets)

        for run_name in ("vmctl-sye3k0", "vmctl-sye3k1"):
            things = thing_records(
                ROOT / "runs" / run_name / "BACKUP/CLEAN.HST", 0)
            kinds = [(struct.unpack_from("<H", thing)[0] >> 13) & 7
                     for thing in things]
            self.assertEqual(kinds, [2] * WORMHOLE_COUNT)
            self.assertNotIn(0, kinds)
            self.assertNotIn(3, kinds)

        # HandleBasicAiTasks begins with KeepFleetsMoving. Its candidate list
        # excludes type-2 things, so this retained table supplies cMinefields=0
        # to SetAiFleetIdealSpeed. The helper leaves bVar2 false when the list
        # is empty; its random draws are reached only when a listed thing puts
        # the fleet in a minefield.
        self.assertIn("SetAiFleetIdealSpeed", keep_fleets)
        self.assertIn("!= '\\x02'", keep_fleets)
        self.assertIn("bVar2 = false;", ideal_speed)
        self.assertIn("bVar2 = true;", ideal_speed)
        self.assertEqual(ideal_speed.count("Random("), 2)
        random_minefield_branch = source_brace_block(ideal_speed, "if (bVar2)")
        self.assertEqual(random_minefield_branch.count("Random("), 2)

        # Pre-production MoveThings only moves type-3 things. Its type-2
        # Random(100) gate is behind fPostProd != 0 and is skipped at mode 0.
        move_minefield = source_brace_block(
            move_things,
            "if (((uint)uStack_20->overlap_0 >> 0xd == 3) && (fPostProd == 0))")
        self.assertIn("Random(0x19)", move_minefield)
        self.assertIn("Random(3)", move_minefield)
        wormhole = source_brace_block(
            move_things,
            "if (((uint)uStack_20->overlap_0 >> 0xd == 2) && (fPostProd != 0))")
        self.assertIn("Random(100)", wormhole)
        self.assertNotIn("Random(100)", move_minefield)

        # Minefield travel builds its crossings from type-0 things and returns
        # before the sole Random(1000) loop when none intersect the route.
        self.assertIn("((uint)uStack_ce->overlap_0 >> 0xd == 0)",
                      minefield_travel)
        empty_crossings = minefield_travel.index("if (iStack_dc == 0)")
        draw = minefield_travel.index("Random(1000)")
        self.assertLess(empty_crossings, draw)
        self.assertIn("return 1;", minefield_travel[empty_crossings:draw])

        # Every random call in DoThingInteractions is inside its type-3 thing
        # block; no type-3 records means this procedure contributes no draws.
        interaction_gate = (
            "if ((uint)uStack_3c->overlap_0 >> 0xd == 3)")
        interaction_block = source_brace_block(interactions, interaction_gate)
        self.assertEqual(interactions.count("Random("), 11)
        self.assertEqual(interaction_block.count("Random("), 11)

    def test_retained_control_and_action_wormholes_replay_move_things(self):
        pair = json.loads(PAIR.read_text(encoding="utf-8"))
        inspector = Inspector()
        points = xy_points(ROOT / "runs/vmctl-sye3k0/CLEAN.XY", inspector)
        self.assertEqual(len(points), 32)
        turn_source = (ROOT / "analysis/decompiler/export/raw/"
                       "10b0_0000_FGenerateTurn.c").read_text()
        move_source = (ROOT / "analysis/decompiler/export/raw/"
                       "10b0_18f4_MoveThings.c").read_text()
        self.assertIn("MoveThings(1);", turn_source)
        self.assertLess(turn_source.index("Produce();"),
                        turn_source.index("MoveThings(1);"))
        gate_at = move_source.index("sVar12 = Random(100);")
        attempts_at = move_source.index(
            "while (iVar8 = iStack_6 + 1", gate_at)
        attempts = move_source[attempts_at:move_source.index(
            "    else {\n      if (((uint)uStack_20->overlap_0 >> 0xd == 3)",
            attempts_at)]
        self.assertIn("iStack_6 < 100", attempts)
        self.assertEqual(attempts.count("Random(0x19)"), 2)
        input_by_run = {}
        for label, run_name in (("control", "vmctl-sye3k0"),
                                ("action", "vmctl-sye3k1")):
            run = ROOT / "runs" / run_name
            before = thing_records(run / "BACKUP/CLEAN.HST", 0)
            after = thing_records(run / "CLEAN.HST", 1)
            self.assertEqual(len(before), WORMHOLE_COUNT)
            self.assertEqual(len(after), WORMHOLE_COUNT)
            input_by_run[label] = before
            output_fleets = fleet_points(run / "CLEAN.HST", 1)
            self.assertIn(len(output_fleets), (7, 8))
            for item in pair["original_outputs"][
                    "owner1_turn1_fleet_order_pairs"]:
                self.assertIn(tuple(item["fleet"]["point"]), output_fleets)
            header, draws, limits = make_header(
                before, after, points, output_fleets)
            chances = [((struct.unpack_from("<H", body, 6)[0] >> 2) & 0x03ff)
                       // 5 - (2 - (struct.unpack_from("<H", body, 6)[0] & 3))
                       for body in before]
            chances = [max(0, min(6, value)) for value in chances]
            self.assertEqual(chances, [0] * WORMHOLE_COUNT)
            self.assertEqual(draws[::3], chances)
            self.assertEqual(limits, [100, 25, 25] * WORMHOLE_COUNT)
            self.assertEqual(3 * len(before), 12)
            with tempfile.TemporaryDirectory() as temp:
                temp_path = Path(temp)
                (temp_path / "satisfy_orders_wormholes_turn_e3_fixture.h").write_text(
                    header, encoding="ascii")
                exe = temp_path / "wormholes_turn_e3"
                subprocess.run([
                    "cc", "-std=c89", "-pedantic", "-Wall", "-Wextra",
                    "-Werror", "-ffunction-sections", "-fdata-sections",
                    "-Wl,-dead_strip", "-I", str(temp_path),
                    "-I", str(ROOT / "reconstructed/include"),
                    str(ROOT / "tests/satisfy_orders_wormholes_turn_e3_probe.c"),
                    str(ROOT / "reconstructed/src/move_things.c"),
                    str(ROOT / "reconstructed/src/common.c"),
                    "-o", str(exe),
                ], check=True)
                result = subprocess.run([str(exe)], check=False,
                                        capture_output=True, text=True)
            self.assertEqual(result.returncode, 0,
                             result.stdout + result.stderr)
            self.assertEqual(
                result.stdout.strip(),
                "SATISFY_ORDERS_WORMHOLE_MOVE_CHECKS=19 FAILURES=0 RESULT=PASS")
        self.assertEqual(input_by_run["control"], input_by_run["action"])


if __name__ == "__main__":
    unittest.main()
