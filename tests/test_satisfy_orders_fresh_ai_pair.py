"""Verify provenance and decoded state for the fresh selector-1 original pair."""

from collections import Counter
import hashlib
import json
import re
import struct
from ctypes import POINTER, byref, c_int16, c_uint16, c_uint32, c_ubyte
from pathlib import Path
import subprocess
import tempfile
import unittest

from tools.race_file_compare import Inspector, Seeds
from tools.ne_layout import NE
from tools.stars_game_stream import compare_records, decode_stream
from tools.satisfy_orders_original_oracle import (
    decode_compact_order, decode_owner_fleet,
)
from tools.satisfy_orders_planet10_oracle import decode_planet_record


ROOT = Path(__file__).resolve().parents[1]
PAIR_PATH = ROOT / "runs/vmctl-sye3k-pair-r1/PAIR.JSON"
STARS_EXE_SHA256 = "ccab6682b55dae14dbf104e491cfbd3b32a6280022020032ae0615500949ba59"


def c_byte_array(name, data):
    rows = ["static const StarsU8 %s[%d] = {" % (name, len(data))]
    for offset in range(0, len(data), 12):
        rows.append("    " + ", ".join("0x%02x" % value
                                       for value in data[offset:offset + 12]) + ",")
    rows.append("};")
    return "\n".join(rows)


# These values are the retained original STARS.EXE's rgcompstrlower table.
# This decoder supports the letters, digits, and spaces used by the retained
# AI design names; it rejects characters whose original lookup table is not
# needed for this evidence.
_USER_LOWER_CODES = (
    0x001, 0x04d, 0x05d, 0x06d, 0x002, 0x07d, 0x08d, 0x003, 0x004,
    0x09d, 0x0ad, 0x005, 0x0bd, 0x006, 0x007, 0x0cd, 0x0dd, 0x008,
    0x009, 0x00a, 0x0ed, 0x0fd, 0x00e, 0x01e, 0x02e, 0x03e,
)
_USER_COMMON_CHARS = " aehilnorstbcdfgjkmpquvwxyz"


def _decompress_user_name(data):
    """Decode the retained letter/digit/space FCompressUserString payloads."""
    nibbles = []
    for value in data:
        nibbles.extend((value >> 4, value & 0x0f))
    chars = []
    cursor = 0
    while cursor < len(nibbles):
        value = nibbles[cursor]
        cursor += 1
        if value == 0x0f and cursor == len(nibbles):
            break
        if value < 0x0b:
            chars.append(_USER_COMMON_CHARS[value])
            continue
        if value == 0x0f:
            if cursor + 2 > len(nibbles):
                raise ValueError("truncated literal in compressed user name")
            value |= nibbles[cursor] << 4
            value |= nibbles[cursor + 1] << 8
            cursor += 2
        else:
            if cursor == len(nibbles):
                raise ValueError("truncated character in compressed user name")
            value |= nibbles[cursor] << 4
            cursor += 1
        if value & 0x0f == 0x0f:
            codepoint = value >> 4
        else:
            index = ((value & 0x0f) - 0x0b) * 16 + (value >> 4)
            if index < 26:
                codepoint = ord("A") + index
            elif index < 36:
                codepoint = ord("0") + index - 26
            elif index < 52:
                codepoint = ord(_USER_COMMON_CHARS[index - 25])
            else:
                raise ValueError("unsupported character in retained user name")
        chars.append(chr(codepoint))
    return "".join(chars)


def _compress_user_name(name):
    """Encode the retained letter/digit/space subset for round-trip checks."""
    nibbles = []
    for char in name:
        if char == " ":
            value = 0
        elif "a" <= char <= "z":
            value = _USER_LOWER_CODES[ord(char) - ord("a")]
        elif "A" <= char <= "P":
            value = ((ord(char) - ord("A")) << 4) | 0x0b
        elif "Q" <= char <= "Z":
            value = ((ord(char) - ord("Q")) << 4) | 0x0c
        elif "0" <= char <= "5":
            value = ((ord(char) - 0x26) << 4) | 0x0c
        elif "6" <= char <= "9":
            value = ((ord(char) - 0x36) << 4) | 0x0d
        else:
            raise ValueError("unsupported character in retained user name")
        nibble_count = (1 if value < 0x0b else
                        3 if value & 0x0f == 0x0f else 2)
        for _ in range(nibble_count):
            nibbles.append(value & 0x0f)
            value >>= 4
    if len(nibbles) & 1:
        nibbles.append(0x0f)
    return bytes((nibbles[index] << 4) | nibbles[index + 1]
                 for index in range(0, len(nibbles), 2))


class SatisfyOrdersFreshAiPairTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.pair = json.loads(PAIR_PATH.read_text(encoding="utf-8"))
        cls.inspector = Inspector()

    @classmethod
    def _run(cls, run_name):
        return json.loads((ROOT / "runs" / run_name / "RUN.JSON")
                          .read_text(encoding="utf-8"))

    @classmethod
    def _records(cls, run_name, filename):
        return decode_stream(ROOT / "runs" / run_name / filename,
                             cls.inspector)

    @classmethod
    def _turn_random8(cls, run_name):
        records = cls._records(run_name, "CLEAN.HST")
        bofs = [record for record in records if record["kind"] == 8 and
                record["turn"] == 1]
        if len(bofs) != 1 or len(bofs[0]["body"]) != 16:
            raise AssertionError("expected one turn-1 HST BOF")
        flags, = struct.unpack_from("<H", bofs[0]["body"], 14)
        return (flags >> 13) & 7

    @classmethod
    def _bof_tick_random2000_mix(cls, run_name, filename, turn):
        records = cls._records(run_name, filename)
        bofs = [record for record in records if record["kind"] == 8 and
                record["turn"] == turn]
        if len(bofs) != 1 or len(bofs[0]["body"]) != 16:
            raise AssertionError("expected one 16-byte BOF")
        packed, = struct.unpack_from("<H", bofs[0]["body"], 12)
        return packed & 0x1f, packed >> 5

    @classmethod
    def _planet(cls, run_name, filename, turn, planet_id):
        matches = []
        for record in cls._records(run_name, filename):
            if record["kind"] != 13 or record["turn"] != turn:
                continue
            body_id = int.from_bytes(record["body"][:2], "little") & 0x7ff
            if body_id == planet_id:
                matches.append(decode_planet_record(record["body"],
                    expected_id=planet_id))
        if len(matches) != 1:
            raise AssertionError("expected one complete planet record")
        return matches[0]

    @classmethod
    def _decode_xy_game_record(cls, path):
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
        cls.inspector.lib.stars_set_file_xor_stream(
            byref(seeds), game_id, c_int16(salt), c_int16(turn),
            c_int16(player), c_int16((flags >> 12) & 1))
        header, = struct.unpack_from("<H", raw, 18)
        if header >> 10 != 7 or (header & 0x3ff) != 64:
            raise ValueError("unexpected XY game record")
        game = cls.inspector.crypt(raw[20:84], seeds)
        count, = struct.unpack_from("<h", game, 10)
        if count < 1 or 84 + count * 4 + 4 != len(raw):
            raise ValueError("unexpected XY point-array bounds")
        return raw, game, count

    @classmethod
    def _xy_points(cls, path):
        raw, _game, count = cls._decode_xy_game_record(path)
        points = []
        previous_x = 1000
        for index in range(count):
            packed, = struct.unpack_from("<I", raw, 84 + index * 4)
            previous_x += packed & 0x3ff
            points.append((previous_x, (packed >> 10) & 0xfff))
        return points

    @staticmethod
    def _owner1_fleets(records, turn):
        pairs = []
        for index, record in enumerate(records[:-1]):
            if record["turn"] != turn or record["kind"] != 16:
                continue
            fleet = decode_owner_fleet(record["body"])
            if fleet["owner"] != 1:
                continue
            following = records[index + 1]
            if following["turn"] != turn or following["kind"] != 20:
                raise AssertionError("owner-1 fleet is not followed by an order")
            order = decode_compact_order(following["body"])
            fields = ("id", "owner", "saved_flags", "planet", "point",
                      "ship_counts", "cargo", "order_count")
            pairs.append({"fleet": {key: fleet[key] for key in fields},
                          "order": order})
        return pairs

    @classmethod
    def _owner1_player(cls, records, turn):
        bodies = [record["body"] for record in records
                  if record["turn"] == turn and record["kind"] == 6
                  and record["body"] and record["body"][0] == 1]
        if len(bodies) != 1:
            raise AssertionError("expected one owner-1 player record")
        return bodies[0]

    @staticmethod
    def _planet24(records, turn):
        bodies = [record["body"] for record in records
                  if record["turn"] == turn and record["kind"] == 13
                  and len(record["body"]) >= 2
                  and int.from_bytes(record["body"][:2], "little")
                  & 0x7ff == 24]
        if len(bodies) != 1:
            raise AssertionError("expected one planet-24 record")
        return decode_planet_record(bodies[0], expected_id=24)

    def test_runs_start_without_player_logs_and_preserve_return_hashes(self):
        for label, run_name in (("control", "vmctl-sye3k0"),
                                ("action", "vmctl-sye3k1")):
            with self.subTest(run=label):
                run = self._run(run_name)
                recorded = self.pair["runs"][label]
                self.assertEqual(run["pre_upload_guest_listing"], [])
                self.assertEqual(run["pre_run_absent_files"],
                                 ["CLEAN.X2", "CLEAN.H2"])
                self.assertEqual(set(run["input_files"]), {
                    "CLEAN.HST", "CLEAN.M1", "CLEAN.M2", "CLEAN.XY",
                    "STARS.EXE"})
                self.assertEqual(recorded["input_names"],
                                 sorted(run["input_files"]))
                self.assertFalse(run["error_dialog_listed"])
                self.assertEqual(recorded["error_dialog_listed"], False)
                self.assertEqual(run["process_result"].split(";")[0],
                                 "EXIT 1")
                self.assertEqual(recorded["post_upload_listing"],
                                 run["post_upload_guest_listing"])
                self.assertEqual(
                    {entry["name"] for entry in
                     run["post_upload_guest_listing"]},
                    set(run["input_files"]))
                for relative, details in run["returned_files"].items():
                    path = ROOT / "runs" / run_name / relative
                    data = path.read_bytes()
                    self.assertEqual(len(data), details["bytes"], relative)
                    self.assertEqual(hashlib.sha256(data).hexdigest(),
                                     details["sha256"], relative)
                    self.assertEqual(recorded["returned_files"][relative],
                                     details)

    def test_only_the_task5_order_differs_and_selector1_input_is_pinned(self):
        control = self._records("vmctl-sye3k0", "BACKUP/CLEAN.HST")
        action = self._records("vmctl-sye3k1", "BACKUP/CLEAN.HST")
        comparison = compare_records(control, action)
        self.assertEqual(len(comparison["differences"]), 1)
        self.assertEqual(comparison["differences"][0]["index"], 45)
        self.assertEqual((control[45]["kind"], control[45]["body"].hex()),
                         (20, "680414050a000011"))
        self.assertEqual((action[45]["kind"], action[45]["body"].hex()),
                         (19, "680414050a00051100000000000000000000"))

        for filename in ("CLEAN.M1", "CLEAN.M2", "CLEAN.XY", "STARS.EXE"):
            self.assertEqual(
                self._run("vmctl-sye3k0")["input_files"][filename]["sha256"],
                self._run("vmctl-sye3k1")["input_files"][filename]["sha256"])

        control_m2 = self._records("vmctl-sye3k0", "BACKUP/CLEAN.M2")
        action_m2 = self._records("vmctl-sye3k1", "BACKUP/CLEAN.M2")
        input_pairs = self._owner1_fleets(control, 0)
        self.assertEqual(input_pairs, self._owner1_fleets(action, 0))
        self.assertEqual(input_pairs, self._owner1_fleets(control_m2, 0))
        self.assertEqual(input_pairs, self._owner1_fleets(action_m2, 0))
        self.assertEqual(input_pairs,
                         self.pair["input_comparison"]
                         ["owner1_input_fleet_order_pairs"])

        control_player = self._owner1_player(control, 0)
        action_player = self._owner1_player(action, 0)
        mode = int.from_bytes(control_player[6:8], "little")
        self.assertEqual(mode, 0x2797)
        self.assertEqual(int.from_bytes(action_player[6:8], "little"), mode)
        self.assertEqual(self.pair["input_comparison"]
                         ["owner1_player_mode_word"], "0x2797")
        self.assertEqual(self.pair["input_comparison"]["ai_selector"], 1)

    def test_xy_map_points_match_the_original_fleet0_movement(self):
        path = ROOT / "runs/vmctl-sye3k0/CLEAN.XY"
        self.assertEqual(hashlib.sha256(path.read_bytes()).hexdigest(),
                         "580e4c3be92eee570c95c97faa2557e3b4fe92856387970cf548700e1bb3fd88")
        points = self._xy_points(path)
        self.assertEqual(len(points), 32)
        self.assertEqual(points[24], (1290, 1258))
        self.assertEqual(points[26], (1313, 1262))
        ranked = sorted(((x - points[24][0]) ** 2 +
                         (y - points[24][1]) ** 2, index)
                        for index, (x, y) in enumerate(points)
                        if index != 24)
        self.assertEqual(ranked[:2], [(545, 26), (1250, 27)])
        input_fleet = self.pair["input_comparison"]
        output_fleet = self.pair["original_outputs"]
        self.assertEqual(input_fleet["owner1_input_fleet_order_pairs"][0]
                         ["fleet"]["point"], list(points[24]))
        self.assertEqual(output_fleet["owner1_turn1_fleet_order_pairs"][0]
                         ["fleet"]["point"], list(points[26]))

    def test_fgenerate_turn_player_shuffle_draw_count_is_pinned(self):
        game_records = []
        for run_name in ("vmctl-sye3k0", "vmctl-sye3k1"):
            path = ROOT / "runs" / run_name / "CLEAN.XY"
            raw, game, _point_count = self._decode_xy_game_record(path)
            self.assertEqual(hashlib.sha256(raw).hexdigest(),
                             "580e4c3be92eee570c95c97faa2557e3b4fe92856387970cf548700e1bb3fd88")
            game_records.append(game)
            # _game.cPlayer is the signed word at byte offset 8.
            self.assertEqual(struct.unpack_from("<h", game, 8)[0], 2)
        self.assertEqual(game_records[0], game_records[1])

        generate = (ROOT / "analysis/decompiler/export/raw/"
                    "10b0_0000_FGenerateTurn.c").read_text()
        random_source = (ROOT / "reconstructed/src/utilgen.c").read_text()
        offsets = (ROOT / "reconstructed/include/soffsets.inc").read_text()
        self.assertIn(
            "STARS_OFFSET(t1012_m3,STARS_CV_1012,cPlayer.value,8)",
            offsets)
        self.assertIn("(int)p_Stack_38 < game.cPlayer", generate)
        self.assertIn("Random(game.cPlayer - (int)p_Stack_38)", generate)
        advances_first = random_source.index("state->first = advance_seed")
        advances_second = random_source.index("state->second = advance_seed")
        count_gate = random_source.index("if (count <= 0)")
        self.assertLess(advances_first, count_gate)
        self.assertLess(advances_second, count_gate)
        player_count = struct.unpack_from("<h", game_records[0], 8)[0]
        shuffle_counts = [player_count - player
                          for player in range(player_count)]
        self.assertEqual(shuffle_counts, [2, 1])
        # Random(1) returns zero, but still advances both RNG seeds.
        self.assertIn("return (StarsI16)(difference % (StarsU32)count)",
                      random_source)

    def test_validate_waypoints_draws_are_skipped_for_retained_fleet_orders(self):
        source = (ROOT / "analysis/decompiler/export/raw/"
                  "1038_68c6_ValidateWaypoints.c").read_text()
        fleet_target = "else if ((((uint)piVar10[3] >> 8 & 0xf) == 2)"
        branch_start = source.index(fleet_target)
        loop_advance = "uStack_1c = (int *)CONCAT22(uVar11,piVar10 + 9);"
        branch_end = source.index(loop_advance, branch_start)
        fleet_target_branch = source[branch_start:branch_end]
        self.assertEqual(source.count("Random("), 4)
        self.assertEqual(fleet_target_branch.count("Random("), 4)
        self.assertNotIn("Random(", source[:branch_start])
        self.assertNotIn("Random(", source[branch_end:])

        for run_name in ("vmctl-sye3k0", "vmctl-sye3k1"):
            for filename, turn in (("BACKUP/CLEAN.HST", 0),
                                   ("CLEAN.HST", 1)):
                records = self._records(run_name, filename)
                kinds = []
                owners = set()
                for index, record in enumerate(records[:-1]):
                    if record["turn"] != turn or record["kind"] != 16:
                        continue
                    fleet = decode_owner_fleet(record["body"])
                    order = records[index + 1]
                    self.assertEqual(order["turn"], turn)
                    self.assertIn(order["kind"], (19, 20))
                    self.assertGreaterEqual(len(order["body"]), 8)
                    flags, = struct.unpack_from("<H", order["body"], 6)
                    kinds.append((flags >> 8) & 15)
                    owners.add(fleet["owner"])
                self.assertTrue(kinds, (run_name, filename, turn))
                self.assertEqual(owners, {0, 1})
                self.assertNotIn(2, kinds,
                    (run_name, filename, turn, kinds))

    def test_transit_breeding_random_gate_is_skipped_for_retained_players(self):
        source = (ROOT / "analysis/decompiler/export/raw/"
                  "10b8_7e4e_BreedColonistsInTransit.c").read_text()
        get_race_stat = (ROOT / "analysis/decompiler/export/raw/"
                         "10e0_30d2_GetRaceStat.c").read_text()
        player_layout = (ROOT / "analysis/DATA_LAYOUTS.md").read_text()
        self.assertEqual(source.count("Random(3)"), 1)
        race_test = "sVar10 = GetRaceStat((_player *)rgplr + iStack_26,0xe);"
        self.assertIn(race_test, source)
        self.assertIn("if (sVar10 == 4) {\n      iStack_6 = 0;\n    }",
                      source)
        self.assertIn("return (int)pplr->rgAttr[iStat];", get_race_stat)
        self.assertIn("| 62 | `rgAttr` | array of char (16 bytes total) |",
                      player_layout)
        self.assertEqual(62 + 0x0e, 76)
        race_loop = source.index(race_test)
        no_breeder_branch = source.index("if (iStack_6 == 0)", race_loop)
        random_call = source.index("Random(3)")
        fleet_scan = source.index("for (iStack_20 = 0;", no_breeder_branch)
        self.assertLess(race_loop, no_breeder_branch)
        self.assertLess(no_breeder_branch, fleet_scan)
        self.assertLess(fleet_scan, random_call)

        player_rows = []
        for run_name in ("vmctl-sye3k0", "vmctl-sye3k1"):
            records = self._records(run_name, "BACKUP/CLEAN.HST")
            rows = [record["body"] for record in records
                    if record["turn"] == 0 and record["kind"] == 6 and
                    len(record["body"]) > 76]
            self.assertEqual(len(rows), 2)
            self.assertEqual([row[0] for row in rows], [0, 1])
            stats = [row[76] for row in rows]
            self.assertEqual(stats, [9, 1])
            self.assertNotIn(4, stats)
            player_rows.append(rows)
        self.assertEqual(player_rows[0], player_rows[1])

    def test_auto_terraform_random_gate_is_skipped_for_retained_players(self):
        source = (ROOT / "analysis/decompiler/export/raw/"
                  "10b8_48f6_AutoTerraform.c").read_text()
        self.assertEqual([source.count("Random(%s)" % limit)
                          for limit in ("3", "10", "1000")], [1, 1, 1])
        self.assertIn("sVar3 = GetRaceStat((_player *)rgplr + iStack_30,0xe);",
                      source)
        self.assertIn("auStack_2a[iStack_30] = (uint)(sVar3 == 3);", source)
        planet_scan = source.index("if (bVar2) {")
        self.assertTrue(all(source.index("Random(%s)" % limit) > planet_scan
                            for limit in ("3", "10", "1000")))

        player_rows = []
        for run_name in ("vmctl-sye3k0", "vmctl-sye3k1"):
            records = self._records(run_name, "BACKUP/CLEAN.HST")
            rows = [record["body"] for record in records
                    if record["turn"] == 0 and record["kind"] == 6 and
                    len(record["body"]) > 76]
            self.assertEqual([row[0] for row in rows], [0, 1])
            stats = [row[76] for row in rows]
            self.assertEqual(stats, [9, 1])
            self.assertNotIn(3, stats)
            player_rows.append(rows)
        self.assertEqual(player_rows[0], player_rows[1])

    def test_update_guesses_draws_are_bounded_by_retained_planet_outputs(self):
        source = (ROOT / "analysis/decompiler/export/raw/"
                  "10b8_532c_UpdateGuesses.c").read_text()
        read_planet = (ROOT / "analysis/decompiler/export/raw/"
                       "1070_3206_FReadPlanet.c").read_text()
        load_game = (ROOT / "analysis/decompiler/export/raw/"
                     "1070_0810_FLoadGame.c").read_text()
        layout = (ROOT / "analysis/DATA_LAYOUTS.md").read_text()
        generate = (ROOT / "analysis/decompiler/export/raw/"
                    "10b0_0000_FGenerateTurn.c").read_text()
        reconstructed = (ROOT / "reconstructed/src/update_guesses.c").read_text()

        # FReadPlanet starts a newly loaded planet as zero and only populates
        # rgwtMin when the serialized full-mineral-detail flag is present.
        self.assertIn("__fmemset(lppl,0,0x38);", read_planet)
        detail_gate = read_planet.index(
            "if (((uint)rgbCur._2_2_ >> 0xd & 1) != 0) {")
        first_mineral_load = read_planet.index(
            "((_planet *)lppl)->rgwtMin + iStack_c", detail_gate)
        self.assertLess(detail_gate, first_mineral_load)
        self.assertIn("LpAlloc(iStack_e * 0x38,3)", load_game)
        self.assertIn("FReadPlanet(iStack_3c,uStack_18,0,sStack_10)",
                      load_game)
        self.assertIn("sStack_10 = 0;", load_game)
        self.assertIn("| 18 | `uGuesses` | unsigned short |", layout)
        self.assertIn("| 28 | `rgwtMin` | array of long (16 bytes total) |",
                      layout)

        self.assertEqual(source.count("Random("), 1)
        zero_guard = "if (((int)p_Stack_8->rgwtMin[3] == 0)"
        self.assertIn(zero_guard, source)
        race_gate = source.index(
            "GetRaceStat((_player *)rgplr + p_Stack_8->iPlayer,0xe)")
        no_draw = source.index("if (sVar3 == 8)", race_gate)
        random_call = source.index("Random((short)lVar8)", no_draw)
        self.assertLess(race_gate, no_draw)
        self.assertLess(no_draw, random_call)
        self.assertIn("random(context, (StarsI16)quarter)", reconstructed)
        self.assertIn("(value - eighth + (StarsI32)perturbation) >> 2",
                      reconstructed)
        update_call = generate.index("UpdateGuesses();")
        write_call = generate.index("FWriteDataFile(", update_call)
        self.assertLess(update_call, write_call)

        player_stats = []
        for run_name in ("vmctl-sye3k0", "vmctl-sye3k1"):
            input_records = self._records(run_name, "BACKUP/CLEAN.HST")
            output_records = self._records(run_name, "CLEAN.HST")
            input_planets = [record for record in input_records
                             if record["kind"] == 13 and record["turn"] == 0]
            output_planets = [record for record in output_records
                              if record["kind"] == 13 and record["turn"] == 1]
            self.assertEqual(len(input_planets), 32)
            self.assertEqual(len(output_planets), 32)

            def planet_ids(records):
                return sorted(int.from_bytes(record["body"][:2], "little") &
                              0x7ff for record in records)

            self.assertEqual(planet_ids(input_planets), list(range(32)))
            self.assertEqual(planet_ids(output_planets), list(range(32)))

            def detailed_planets(records):
                result = []
                for record in records:
                    body = record["body"]
                    flags, = struct.unpack_from("<H", body, 2)
                    if flags & 0x2000:
                        planet_id = int.from_bytes(body[:2], "little") & 0x7ff
                        owner = (int.from_bytes(body[:2], "little") >> 11) & 31
                        result.append((planet_id, owner,
                                       decode_planet_record(
                                           body, expected_id=planet_id)))
                return result

            input_detail = detailed_planets(input_planets)
            output_detail = detailed_planets(output_planets)
            self.assertEqual([(pid, owner) for pid, owner, _ in input_detail],
                             [(10, 0), (24, 1)])
            self.assertEqual([planet["minerals"][3]
                              for _, _, planet in input_detail], [250, 250])
            self.assertEqual([(pid, owner) for pid, owner, _ in output_detail],
                             [(10, 0), (24, 1)])
            self.assertEqual([planet["minerals"][3]
                              for _, _, planet in output_detail], [287, 256])

            players = [record["body"] for record in input_records
                       if record["kind"] == 6 and record["turn"] == 0]
            players.sort(key=lambda body: body[0])
            self.assertEqual([body[0] for body in players], [0, 1])
            stats = [body[76] for body in players]
            self.assertEqual(stats, [9, 1])
            self.assertNotIn(8, stats)
            player_stats.append(players)

            expected_bounds = {
                ("vmctl-sye3k0", 10): (60, 63),
                ("vmctl-sye3k0", 24): (4, 7),
                ("vmctl-sye3k1", 10): (56, 59),
                ("vmctl-sye3k1", 24): (0, 3),
            }
            for planet_id, owner, planet in output_detail:
                value = planet["minerals"][3]
                guess = planet["auxiliary_word"] & 0x0fff
                random_limit = value >> 2
                base = value - (value >> 3)
                minimum = max(0, guess * 4 - base)
                maximum = min(random_limit - 1, (guess + 1) * 4 - 1 - base)
                self.assertEqual((minimum, maximum),
                                 expected_bounds[(run_name, planet_id)])
                self.assertLessEqual(minimum, maximum)
                self.assertEqual(owner, {10: 0, 24: 1}[planet_id])
        self.assertEqual(player_stats[0], player_stats[1])

    def test_spank_cheaters_random_sites_are_skipped_before_turn_ten(self):
        source = (ROOT / "analysis/decompiler/export/raw/"
                  "10f0_192a_SpankTheCheaters.c").read_text()
        generate = (ROOT / "analysis/decompiler/export/raw/"
                    "10b0_0000_FGenerateTurn.c").read_text()
        self.assertEqual(source.count("Random("), 7)
        turn_gate = source.index(
            "if ((iStack_1a != 0) && (9 < game.turn)) {")
        self.assertNotIn("Random(", source[:turn_gate])

        spank = generate.index("SpankTheCheaters();")
        update_guesses = generate.index("UpdateGuesses();", spank)
        increment_turn = generate.index("game.turn = game.turn + 1;",
                                        update_guesses)
        self.assertLess(spank, update_guesses)
        self.assertLess(update_guesses, increment_turn)

        for run_name in ("vmctl-sye3k0", "vmctl-sye3k1"):
            records = self._records(run_name, "BACKUP/CLEAN.HST")
            turn_zero_bofs = [record for record in records
                              if record["kind"] == 8 and record["turn"] == 0]
            self.assertEqual(len(turn_zero_bofs), 1)

    def test_random_events_enter_and_take_three_first_draws_at_turn_zero(self):
        produce = (ROOT / "analysis/decompiler/export/raw/"
                   "10b8_0000_Produce.c").read_text()
        dispatcher = (ROOT / "analysis/decompiler/export/raw/"
                      "10b8_3064_RandomEvents.c").read_text()
        event_sources = {
            "MeteorStrike": ("10b8_560e_MeteorStrike.c",
                             "if (sVar5 == 0) {"),
            "PlanetaryClimateChange": ("10b8_5c54_PlanetaryClimateChange.c",
                                        "if (sVar3 == 0) {"),
            "DiscoverNewMinerals": ("10b8_5e0c_DiscoverNewMinerals.c",
                                    "if (sVar2 == 0) {"),
        }
        trader = (ROOT / "analysis/decompiler/export/raw/"
                  "10b8_5efa_MysteryTrader.c").read_text()
        climate = (ROOT / "analysis/decompiler/export/raw/"
                   "10b8_5c54_PlanetaryClimateChange.c").read_text()
        meteor = (ROOT / "analysis/decompiler/export/raw/"
                  "10b8_560e_MeteorStrike.c").read_text()
        discovery = (ROOT / "analysis/decompiler/export/raw/"
                     "10b8_5e0c_DiscoverNewMinerals.c").read_text()
        toss_items = (ROOT / "analysis/decompiler/export/raw/"
                      "10b8_5aec_TossNonAutoBuildItems.c").read_text()
        layout = (ROOT / "analysis/DATA_LAYOUTS.md").read_text()
        self.assertIn("| 16 | `fNoRandom` | unsigned short; bits 7..7 |", layout)
        self.assertIn("if (((uint)game.overlap_16 >> 7 & 1) == 0) {\n        RandomEvents();",
                      produce)
        calls = [dispatcher.index(name + "();") for name in event_sources]
        self.assertEqual(calls, sorted(calls))
        for function, (filename, event_gate) in event_sources.items():
            source = (ROOT / "analysis/decompiler/export/raw/"
                      / filename).read_text()
            first_random = source.index("Random(")
            self.assertGreater(source.count("Random("), 0)
            self.assertLess(first_random, source.index(event_gate))
            self.assertIn(function + "();", dispatcher)

        trader_gate = trader.index("if (0x27 < game.turn)")
        self.assertNotIn("Random(", trader[:trader_gate])
        self.assertGreater(trader.index("Random(", trader_gate), trader_gate)

        climate_gate = climate.index(
            "if ((((p_Var5->iPlayer == -1)")
        self.assertEqual(climate.count("Random("), 6)
        self.assertEqual(climate[climate_gate:].count("Random("), 4)
        self.assertLess(climate.index("Random(0x14)"),
                        climate.index("if (sVar3 == 0)"))
        self.assertLess(climate.index("Random(cPlanet)"), climate_gate)
        self.assertIn("0x13 < game.turn", climate)

        for source, first_roll, selection, turn_gate, effect_draw in (
                (meteor, "Random(0x14)", "Random(cPlanet)",
                 "9 < game.turn", "Random(4)"),
                (discovery, "Random(0xf - game.mdSize)", "Random(cPlanet)",
                 "9 < game.turn", "Random(3)")):
            self.assertLess(source.index(first_roll),
                            source.index("if ("))
            self.assertLess(source.index(selection),
                            source.index(turn_gate))
            self.assertGreater(source.index(effect_draw),
                               source.index(turn_gate))
        self.assertNotIn("Random(", toss_items)

        event_call_bounds = ((1, 2), (1, 6), (1, 2), (0, 0))
        self.assertEqual((sum(bounds[0] for bounds in event_call_bounds),
                          sum(bounds[1] for bounds in event_call_bounds)),
                         (3, 10))

        for run_name in ("vmctl-sye3k0", "vmctl-sye3k1"):
            _raw, game, _count = self._decode_xy_game_record(
                ROOT / "runs" / run_name / "CLEAN.XY")
            game_flags, = struct.unpack_from("<H", game, 16)
            game_turn, = struct.unpack_from("<h", game, 18)
            mineral_density, = struct.unpack_from("<h", game, 4)
            self.assertEqual(game_turn, 0)
            self.assertEqual(mineral_density, 0)
            self.assertEqual(game_flags & 0x0080, 0)
            self.assertEqual(0x0f - mineral_density, 15)
            for filename, turn in (("BACKUP/CLEAN.HST", 0),
                                   ("CLEAN.HST", 1)):
                planet_records = [record for record in
                                  self._records(run_name, filename)
                                  if record["kind"] == 13 and
                                  record["turn"] == turn]
                self.assertEqual(len(planet_records), 32)
                owner_map = {
                    int.from_bytes(record["body"][:2], "little") & 0x7ff:
                    (int.from_bytes(record["body"][:2], "little") >> 11) & 31
                    for record in planet_records
                }
                self.assertEqual({pid: owner for pid, owner in owner_map.items()
                                  if owner != 31}, {10: 0, 24: 1})

    def test_successful_output_writers_pin_five_random2000_draws(self):
        expected_bof_turns = {
            "CLEAN.HST": [1],
            "CLEAN.M1": [0, 1],
            "CLEAN.M2": [1],
            "CLEAN.H2": [0],
            "BACKUP/CLEAN.X2": [0],
        }
        for run_name in ("vmctl-sye3k0", "vmctl-sye3k1"):
            for filename, expected_turns in expected_bof_turns.items():
                records = self._records(run_name, filename)
                self.assertEqual(
                    [record["turn"] for record in records
                     if record["kind"] == 8],
                    expected_turns,
                    "%s %s" % (run_name, filename))

        do_ai = (ROOT / "analysis/decompiler/export/raw/"
                 "1088_0000_DoAiTurn.c").read_text()
        generate = (ROOT / "analysis/decompiler/export/raw/"
                    "10b0_0000_FGenerateTurn.c").read_text()
        data_writer = (ROOT / "analysis/decompiler/export/raw/"
                       "1070_5964_FWriteDataFile.c").read_text()
        create_file = (ROOT / "analysis/decompiler/export/raw/"
                       "1070_8e16_FCreateFile.c").read_text()
        append_file = (ROOT / "analysis/decompiler/export/raw/"
                       "1070_704e_FAppendFile.c").read_text()
        bof_writer = (ROOT / "analysis/decompiler/export/raw/"
                      "1070_8ea4_WriteBOF.c").read_text()
        self.assertIn("FWriteLogFile((schar *)szBase,iPlayer)", do_ai)
        self.assertIn("FWriteHistFile(iPlayer)", do_ai)
        self.assertIn("FWriteDataFile((schar *)szBase,(short)p_Stack_38,sVar14)",
                      generate)
        self.assertIn("game.cPlayer <= (int)p_Stack_38", generate)
        self.assertIn("p_Stack_38 = (_fleet *)0xffff", generate)
        self.assertIn("FCreateFile(dt,iPlayer", data_writer)
        self.assertIn("FAppendFile(iPlayer)", data_writer)
        self.assertIn("WriteBOF(iPlayer,dt,0)", create_file)
        self.assertIn("WriteBOF(iPlayer,3,1)", append_file)
        self.assertIn("Random(2000)", bof_writer)

    def test_bof_records_pin_tick_plus_random2000_low_bits(self):
        observation = self.pair[
            "write_bof_tick_plus_random2000_mod_2048"]
        for label, run in observation["runs"].items():
            run_name = run["run"]
            for bof in run["writes"]:
                with self.subTest(run=run_name, file=bof["file"],
                                  turn=bof["turn"]):
                    self.assertEqual(self._bof_tick_random2000_mix(
                        run_name, bof["file"], bof["turn"]),
                        (bof["player"],
                         bof["tick_plus_random2000_mod_2048"]))

        writer = (ROOT / "analysis/decompiler/export/raw/"
                  "1070_8ea4_WriteBOF.c").read_text()
        self.assertIn("sVar1 = Random(2000);", writer)
        self.assertIn("lVar3 = GETTICKCOUNT();", writer)
        self.assertIn(
            "uStack_8 = uStack_8 & 0x1f | ((int)lVar3 + sVar1) * 0x20;",
            writer)

    def test_original_mining_outputs_bound_six_random100_draws(self):
        source_records = self._records("vmctl-sye3k0",
                                       "BACKUP/CLEAN.HST")
        source_sha256 = hashlib.sha256((ROOT / "runs/vmctl-sye3k0/"
            "BACKUP/CLEAN.HST").read_bytes()).hexdigest()
        self.assertEqual(source_sha256,
            "74a27a31221f67328a98200c5dfde54f4c955a6498cc37ac08624b64a33997f6")
        input_planets = {
            planet_id: self._planet("vmctl-sye3k0", "BACKUP/CLEAN.HST",
                                    0, planet_id)
            for planet_id in (10, 24)
        }
        self.assertEqual([(planet_id, planet["owner"],
                           planet["minerals"][:3],
                           planet["concentrations"])
                          for planet_id, planet in input_planets.items()], [
            (10, 0, [469, 893, 376], [68, 87, 68]),
            (24, 1, [532, 956, 500], [68, 87, 68]),
        ])
        input_planet_ids = [
            int.from_bytes(record["body"][:2], "little") & 0x7ff
            for record in source_records
            if record["kind"] == 13 and record["turn"] == 0 and
            ((int.from_bytes(record["body"][:2], "little") >> 11) & 31)
            != 31
        ]
        self.assertEqual(input_planet_ids, [10, 24])

        pair_inputs = self.pair["input_comparison"]
        control_order = decode_compact_order(
            source_records[45]["body"])
        action_records = self._records("vmctl-sye3k1",
                                       "BACKUP/CLEAN.HST")
        self.assertEqual((pair_inputs["ai_selector"],
                          source_records[45]["kind"],
                          control_order["task"],
                          action_records[45]["kind"],
                          action_records[45]["body"].hex()),
                         (1, 20, 0, 19,
                          "680414050a00051100000000000000000000"))

        e3_oracle = json.loads((ROOT / "analysis/compat/"
            "satisfy-orders-full-turn-e3-original-oracle.json").read_text(
                encoding="utf-8"))
        owner1_mining = e3_oracle["current_source_control_action_pair"][
            "owner1_planet24"]["production_mining_stage"]
        owner1_scrap_stock = owner1_mining[
            "owner1_exact_mineral_stock_witness"][
                "known_task5_scrap_stock_before_mining"]
        production_spend = owner1_mining[
            "owner1_exact_mineral_stock_witness"][
                "partial_build_mineral_spend"]
        self.assertEqual(owner1_scrap_stock, [597, 956, 519])
        self.assertEqual(production_spend, [12, 1, 6])

        task5_scrap = [9, 6, 5]
        remainders = {}
        for planet_id, planet in input_planets.items():
            self.assertEqual(planet["improvements"]["mines"], 10)
            remainders[planet_id] = [
                (10 * concentration) % 100
                for concentration in planet["concentrations"]]
            self.assertEqual(remainders[planet_id], [80, 70, 80])

        observed_yields = {}
        for label, run_name in (("control", "vmctl-sye3k0"),
                                ("action", "vmctl-sye3k1")):
            final = {planet_id: self._planet(run_name, "CLEAN.HST", 1,
                                              planet_id)
                     for planet_id in (10, 24)}
            p10_scrap = [0, 0, 0] if label == "control" else task5_scrap
            p10_mined = [
                final[10]["minerals"][index] -
                input_planets[10]["minerals"][index] - p10_scrap[index]
                for index in range(3)]
            p24_mined = [
                final[24]["minerals"][index] + production_spend[index] -
                owner1_scrap_stock[index]
                for index in range(3)]
            observed_yields[label] = {10: p10_mined, 24: p24_mined}

        self.assertEqual(observed_yields, {
            "control": {10: [7, 9, 7], 24: [7, 8, 7]},
            "action": {10: [6, 9, 7], 24: [7, 9, 7]},
        })

        def random_range(planet_id, index, amount):
            floor, remainder = divmod(
                10 * input_planets[planet_id]["concentrations"][index], 100)
            if amount == floor + 1:
                return [0, remainder - 1]
            if amount == floor:
                return [remainder, 99]
            raise AssertionError("observed mining yield is outside source bound")

        resource_names = ["ironium", "boranium", "germanium"]
        intervals = {
            label: [
                {"planet": planet_id, "resource": resource_names[index],
                 "yield": amount,
                 "random100_range_inclusive": random_range(
                     planet_id, index, amount)}
                for planet_id in (10, 24)
                for index, amount in enumerate(observed_yields[label][planet_id])
            ]
            for label in ("control", "action")
        }
        expected_intervals = {
            "control": [
                [0, 79], [0, 69], [0, 79],
                [0, 79], [70, 99], [0, 79],
            ],
            "action": [
                [80, 99], [0, 69], [0, 79],
                [0, 79], [0, 69], [0, 79],
            ],
        }
        for label in ("control", "action"):
            self.assertEqual([entry["random100_range_inclusive"]
                              for entry in intervals[label]],
                             expected_intervals[label])

        miner = (ROOT / "analysis/decompiler/export/raw/"
                 "10b8_55a4_MineMinerals.c").read_text()
        estimator = (ROOT / "analysis/decompiler/export/raw/"
                     "1028_5362_EstMineralsMined.c").read_text()
        self.assertIn("for (; p_Stack_14 < p_Var2;", miner)
        self.assertIn("sVar7 = Random(100);", estimator)
        self.assertIn("for (iStack_e = 0; iStack_e < 3;", estimator)

        pair_evidence = self.pair.get(
            "mining_random100_threshold_observations")
        self.assertEqual(pair_evidence["source_input_sha256"],
                         source_sha256)
        self.assertEqual(pair_evidence["outputs"], intervals)

    def test_full_map_mining_pins_six_random100_draws(self):
        hst = self._records("vmctl-sye3k0", "BACKUP/CLEAN.HST")
        planet_bodies = [record["body"] for record in hst
                         if record["kind"] == 13 and record["turn"] == 0]
        self.assertEqual(len(planet_bodies), 32)
        owned = {}
        for body in planet_bodies:
            packed = int.from_bytes(body[:2], "little")
            planet_id = packed & 0x7ff
            owner = (packed >> 11) & 31
            if owner != 31:
                owned[planet_id] = (owner, decode_planet_record(
                    body, expected_id=planet_id))
        self.assertEqual(set(owned), {10, 24})

        players = {record["body"][0]: record["body"] for record in hst
                   if record["kind"] == 6 and record["turn"] == 0}
        self.assertEqual(set(players), {0, 1})
        draw_count = 0
        for planet_id, (owner, planet) in owned.items():
            player = players[owner]
            efficiency_tenths = player[0x3e + 4]
            mine_operating_percent = player[0x3e + 6]
            special_race = player[0x3e + 14] == 8
            self.assertEqual(efficiency_tenths, 10)
            self.assertFalse(special_race)
            self.assertEqual(planet["improvements"]["mines"], 10)
            self.assertEqual(planet["minerals"][3], 250)
            population_limited_mines = (planet["minerals"][3] *
                                        mine_operating_percent // 100)
            self.assertGreaterEqual(population_limited_mines, 10,
                                    planet_id)
            # CMaxMines has a minimum of ten for non-Macinti players, then
            # CMaxOperableMines caps that by population; CMinesOperating caps
            # again by the ten packed built mines.
            operating_mines = min(planet["improvements"]["mines"],
                                  population_limited_mines)
            self.assertEqual(operating_mines, 10, planet_id)
            products = [operating_mines * concentration *
                        efficiency_tenths // 10
                        for concentration in planet["concentrations"]]
            self.assertEqual(products, [680, 870, 680], planet_id)
            remainders = [product % 100 for product in products]
            self.assertEqual(remainders, [80, 70, 80], planet_id)
            draw_count += sum(remainder != 0 for remainder in remainders)
        self.assertEqual(draw_count, 6)

        mine_source = (ROOT / "analysis/decompiler/export/raw/"
                       "10b8_55a4_MineMinerals.c").read_text()
        estimate_source = (ROOT / "analysis/decompiler/export/raw/"
                           "1028_5362_EstMineralsMined.c").read_text()
        max_mines_source = (ROOT / "analysis/decompiler/export/raw/"
                            "1048_7248_CMaxMines.c").read_text()
        operable_source = (ROOT / "analysis/decompiler/export/raw/"
                           "1048_7304_CMaxOperableMines.c").read_text()
        operating_source = (ROOT / "analysis/decompiler/export/raw/"
                            "1048_73fc_CMinesOperating.c").read_text()
        self.assertIn("p_Stack_14 < p_Var2", mine_source)
        self.assertIn("EstMineralsMined", mine_source)
        self.assertIn("p_Var8->iPlayer == -1", estimate_source)
        self.assertIn("sVar7 = Random(100)", estimate_source)
        self.assertIn("if (lVar2 < 10)", max_mines_source)
        self.assertIn("if (sVar1 < lVar4)", operable_source)
        self.assertIn("p_Var4->iPlayer == -1", operating_source)

    def test_selector1_fleet0_inputs_reach_original_turin_scout_dispatch(self):
        records = self._records("vmctl-sye3k0", "BACKUP/CLEAN.M2")
        player = self._owner1_player(records, 0)
        fleet_records = [r for r in records if r["kind"] == 16]
        self.assertEqual(len(fleet_records), 4)
        self.assertEqual({decode_owner_fleet(r["body"])["owner"]
                          for r in fleet_records}, {1})
        self.assertEqual(int.from_bytes(player[6:8], "little"), 0x2797)
        self.assertEqual(list(player[26:32]), [0, 0, 1, 0, 5, 0])
        self.assertEqual(int.from_bytes(player[2:4], "little"), 1)
        self.assertEqual([int.from_bytes(r["body"][:2], "little") & 0x7ff
                          for r in records if r["kind"] == 13], [24])

        shdef = [r["body"] for r in records if r["kind"] == 26 and
                 ((int.from_bytes(r["body"][:2], "little") >> 10) & 31) == 0]
        self.assertEqual(len(shdef), 1)
        self.assertEqual(shdef[0][2], 4)

        exe = ROOT / "target/stars.exe"
        self.assertEqual(hashlib.sha256(exe.read_bytes()).hexdigest(),
                         "ccab6682b55dae14dbf104e491cfbd3b32a6280022020032ae0615500949ba59")
        hull_table = bytes(NE(exe).raw[1])
        hull4 = hull_table[0x29f0 + 4 * 0x8f:0x29f0 + 5 * 0x8f]
        slot_count = hull4[122]
        hull_slots = [struct.unpack_from("<HH", hull4, 58 + 4 * index)
                      for index in range(slot_count)]
        self.assertEqual(slot_count, 3)
        self.assertTrue(any((group & 0x30) != 0 and (item >> 8) != 0
                            for group, item in hull_slots))

        fleet_record = next(r["body"] for r in records if r["kind"] == 16
                            and decode_owner_fleet(r["body"])["owner"] == 1
                            and decode_owner_fleet(r["body"])["id"] == 0)
        fleet = decode_owner_fleet(fleet_record)
        self.assertEqual(int.from_bytes(fleet_record[98:100], "little",
                                        signed=True), 0)
        self.assertEqual(fleet["ship_counts"], [1] + [0] * 15)
        # In raw DoTurinDroneAiTurn lines 674-702, these input predicates
        # pass the fleet-0 scout filters; tech[3] == 0 prevents the hull-4,
        # tech>5 early exit, so execution reaches IdTargetScout.

    def test_turin_rng_guard_inputs_are_pinned(self):
        records = self._records("vmctl-sye3k0", "BACKUP/CLEAN.M2")
        player = self._owner1_player(records, 0)
        self.assertEqual(list(player[26:32]), [0, 0, 1, 0, 5, 0])

        shdef_slots = sorted(
            (int.from_bytes(record["body"][:2], "little") >> 10) & 31
            for record in records if record["kind"] == 26 and
            record["turn"] == 0)
        self.assertEqual(shdef_slots, [0, 1, 2, 16])
        fleet_records = [record["body"] for record in records
                         if record["kind"] == 16]
        fleets = [decode_owner_fleet(body) for body in fleet_records]
        self.assertEqual(len(fleets), 4)
        self.assertEqual([fleet["ship_counts"][12] for fleet in fleets],
                         [0, 0, 0, 0])

        load_source = (ROOT / "analysis/decompiler/export/raw/"
                       "1070_0810_FLoadGame.c").read_text()
        turin_source = (ROOT / "analysis/decompiler/export/raw/"
                        "1088_3670_DoTurinDroneAiTurn.c").read_text()
        self.assertIn("iStack_3e < 0x10", load_source)
        self.assertIn("& 0xfdff", load_source)
        self.assertIn("| 0x200", load_source)
        self.assertIn("rgTech + 2 + idPlayer * 0xc0", turin_source)
        self.assertIn("rgshdef[0xc].overlap_123 >> 9 & 1) == 0",
                      turin_source)

    def test_retained_turin_fleet0_scout_path_has_one_known_random_call(self):
        m2 = self._records("vmctl-sye3k0", "BACKUP/CLEAN.M2")
        fleet_records = [record for record in m2
                         if record["kind"] == 16 and record["turn"] == 0]
        fleets = [decode_owner_fleet(record["body"])
                  for record in fleet_records]
        self.assertEqual(len(fleets), 4)
        self.assertEqual({fleet["owner"] for fleet in fleets}, {1})
        self.assertEqual([(fleet["id"], fleet["planet"],
                           fleet["order_count"])
                          for fleet in fleets],
                         [(0, 24, 1), (1, 24, 1),
                          (2, 24, 1), (3, 24, 1)])

        points = self._xy_points(ROOT / "runs/vmctl-sye3k0/CLEAN.XY")
        candidates = [planet_id for planet_id in
                      (*range(24), *range(25, len(points)))]
        nearest = min(candidates, key=lambda planet_id:
                      sum((points[planet_id][axis] - points[24][axis]) ** 2
                          for axis in (0, 1)))
        self.assertEqual(points[24], (1290, 1258))
        self.assertEqual(nearest, 26)
        nearest_distance_squared = sum(
            (points[nearest][axis] - points[24][axis]) ** 2
            for axis in (0, 1))
        self.assertEqual(nearest_distance_squared, 545)

        hst = [record["body"] for record in
               self._records("vmctl-sye3k0", "BACKUP/CLEAN.HST")
               if record["kind"] == 43 and record["turn"] == 0]
        self.assertEqual(len(hst[0]), 2)
        thing_count, = struct.unpack("<H", hst[0])
        things = hst[1:]
        self.assertEqual(thing_count, 4)
        self.assertEqual(len(things), thing_count)
        self.assertTrue(all(len(thing) == 18 for thing in things))
        self.assertEqual({struct.unpack_from("<H", thing)[0] >> 13
                          for thing in things}, {2})
        wormhole_distances = []
        for thing in things:
            x, y = struct.unpack_from("<hh", thing, 2)
            wormhole_distances.append(
                (x - points[24][0]) ** 2 + (y - points[24][1]) ** 2)
        self.assertEqual(min(wormhole_distances), 16101)
        self.assertGreater(min(wormhole_distances),
                           4 * nearest_distance_squared)

        id_target = (ROOT / "analysis/decompiler/export/raw/"
                     "1090_61de_IdTargetScout.c").read_text()
        nearest_source = (ROOT / "analysis/decompiler/export/raw/"
                          "1090_15a4_IdNearestUnknownPlanet.c").read_text()
        wormhole_source = (ROOT / "analysis/decompiler/export/raw/"
                           "1090_12e6_LpthWormFind.c").read_text()
        turin_source = (ROOT / "analysis/decompiler/export/raw/"
                        "1088_3670_DoTurinDroneAiTurn.c").read_text()

        # No non-owner fleet enters the retained M2 enemy list, so the
        # Random(3) candidate tie break is skipped. The known unknown-planet
        # result avoids IdTargetScout's Random(game.cPlanMax) sentinel path.
        self.assertIn("if (p_Var15->iPlayer == idPlayer)", turin_source)
        self.assertIn("p_Stack_2a = p_Var15", turin_source)
        self.assertIn("if (uStack_2e != (_fleet *)0x0)", id_target)
        self.assertIn("sVar3 = Random(3)", id_target)
        self.assertIn("if (_Stack_24 == (_thing_at_0)0xffff)", id_target)
        self.assertIn("IdNearestUnknownPlanet(lpfl,plpthWorm)", id_target)
        self.assertIn("IdTargetScout(uStack_78", turin_source)
        self.assertIn("(_thing **)&uStack_32", turin_source)

        # Fleet 0 is on a planet, so IdNearestUnknownPlanet makes one
        # Random(100) call. All wormholes exceed LpthWormFind's 4*d2 bound;
        # it returns null before its own Random(100), and DoTurin's optional
        # wormhole Random(100) is skipped as well.
        self.assertIn("((_fleet *)lpfl)->idPlanet != -1", nearest_source)
        self.assertIn("sVar3 = Random(100)", nearest_source)
        self.assertIn("((p_Stack_16 == (_thing *)0x0) && (iStack_14 == 0))",
                      wormhole_source)
        self.assertIn("sVar4 = Random(100)", wormhole_source)
        self.assertIn("uStack_32._2_2_ != 0", turin_source)
        self.assertIn("sVar9 = Random(100)", turin_source)

    def test_missing_x2_runs_original_selector1_turin_dispatch(self):
        for run_name in ("vmctl-sye3k0", "vmctl-sye3k1"):
            run = self._run(run_name)
            self.assertIn("CLEAN.X2", run["pre_run_absent_files"])
            self.assertIn("BACKUP/CLEAN.X2", run["returned_files"])

        ensure = (ROOT / "analysis/decompiler/export/raw/1020_56bc_EnsureAis.c").read_text()
        fopen = (ROOT / "analysis/decompiler/export/raw/1070_4ac2_FOpenFile.c").read_text()
        suffix = (ROOT / "analysis/decompiler/export/raw/1070_8cfe_SetSzWorkFromDt.c").read_text()
        dispatch = (ROOT / "analysis/decompiler/export/raw/1088_0000_DoAiTurn.c").read_text()
        self.assertIn("FOpenFile(1,iStack_10,0x20)", ensure)
        self.assertIn("if (sStack_a == 0)", ensure)
        self.assertIn("DoAiTurn(iStack_10", ensure)
        self.assertIn("SetSzWorkFromDt(dt_00,iPlayer)", fopen)
        self.assertIn("uVar4 = 0x78", suffix)  # dt 1 names the X player log.
        self.assertIn("FLoadGame((schar *)szBase,asStack_8)", dispatch)
        self.assertIn("DoTurinDroneAiTurn(a_Stack_108)", dispatch)
        self.assertEqual((0x2797 >> 13) & 7, 1)

        segments = json.loads((ROOT / "analysis/segments.json")
                              .read_text(encoding="utf-8"))
        data_segment = next(s for s in segments if s["segment"] == 37)
        exe = (ROOT / "target/stars.exe").read_bytes()
        data_start = data_segment["file_offset"]
        mpctd_string = int.from_bytes(exe[data_start + 0x1c6:
                                          data_start + 0x1c8], "little")
        self.assertEqual(mpctd_string, 0x1c2)
        self.assertEqual(exe[data_start + mpctd_string:
                             data_start + mpctd_string + 4], b"m%d\0")

    def test_turin_merge_gate_and_mine_sentinel_for_fleets2_3(self):
        records = self._records("vmctl-sye3k0", "BACKUP/CLEAN.M2")
        slot2 = [r["body"] for r in records if r["kind"] == 26 and
                 r["turn"] == 0 and
                 ((int.from_bytes(r["body"][:2], "little") >> 10) & 31) == 2]
        self.assertEqual(len(slot2), 1)
        design = slot2[0]
        flags = int.from_bytes(design[:2], "little")
        self.assertEqual(flags, 0x0907)
        self.assertEqual(flags & 0x0200, 0)  # Turin's MergeAllShdefs gate is open.
        self.assertEqual((design[2], design[6]), (20, 2))

        components = []
        for index in range(design[6]):
            group, packed = struct.unpack_from("<HH", design,
                                               17 + index * 4)
            components.append((group, packed & 0xff, packed >> 8))
        self.assertEqual(components, [(1, 1, 1), (0x80, 0, 2)])

        exe = ROOT / "target/stars.exe"
        self.assertEqual(hashlib.sha256(exe.read_bytes()).hexdigest(),
                         "ccab6682b55dae14dbf104e491cfbd3b32a6280022020032ae0615500949ba59")
        segment = bytes(NE(exe).raw[1])  # Original NE segment 2.
        hull_offset = 0x29f0 + design[2] * 0x8f
        hull = segment[hull_offset:hull_offset + 0x8f]
        self.assertEqual(hull[8:40].split(b"\0", 1)[0], b"Midget Miner")
        self.assertEqual(hull[122], design[6])

        mine_part_offset = 0x1508  # FLookupPart group 0x80, item 0.
        mine_part = segment[mine_part_offset:mine_part_offset + 0x36]
        self.assertEqual(len(mine_part), 0x36)
        self.assertEqual(mine_part[8:40].split(b"\0", 1)[0],
                         b"Robo-Midget Miner")
        mine_dp, = struct.unpack_from("<H", mine_part, 52)
        self.assertEqual(mine_dp, 5)

        input_pairs = self._owner1_fleets(records, 0)
        self.assertEqual([pair["fleet"]["id"] for pair in input_pairs],
                         [0, 1, 2, 3])
        fleets = {pair["fleet"]["id"]: pair["fleet"]
                  for pair in input_pairs}
        for fleet_id in (2, 3):
            fleet = fleets[fleet_id]
            self.assertEqual(fleet["ship_counts"][2], 1)
            self.assertEqual((fleet["planet"], fleet["point"]),
                             (24, [1290, 1258]))
            self.assertEqual(2 * mine_dp, 10)  # CMineFromLpfl per ship.
            self.assertNotEqual(2 * mine_dp, 4000)

        turin = (ROOT / "analysis/decompiler/export/raw/1088_3670_DoTurinDroneAiTurn.c").read_text()
        merge = (ROOT / "analysis/decompiler/export/raw/1090_58be_MergeAllShdefs.c").read_text()
        merge_two = (ROOT / "analysis/decompiler/export/raw/1050_c9f6_Merge2Fleets.c").read_text()
        orders = (ROOT / "analysis/decompiler/export/raw/10b0_179a_DoOrders.c").read_text()
        satisfy = (ROOT / "analysis/decompiler/export/raw/10b0_6798_SatisfyOrders.c").read_text()
        self.assertIn("if (((uint)rgshdef[2].overlap_123 >> 9 & 1) == 0)",
                      turin)
        self.assertIn("MergeAllShdefs(0xc)", turin)
        self.assertIn("lVar4 != 4000", merge)
        self.assertIn("Merge2Fleets", merge)
        self.assertIn("FDeleteFleet", merge_two)
        # The role/action path requires empty design-2/3 slots. The other
        # branch routes turn-zero miners to the task-5 write at LAB_1088_5135.
        self.assertIn("if ((p_Var15->rgcsh[2] == 0) && (p_Var15->rgcsh[3] == 0))",
                      turin)
        self.assertIn("if (game.turn == 0) goto LAB_1088_5135;", turin)
        self.assertIn("& 0xfff0 | 5", turin)
        self.assertIn("sVar2 = 1;", orders)
        self.assertIn("SatisfyOrders(sVar2);", orders)  # Pre-movement pass 1.
        self.assertIn("((uStack_78 & 0xf) == 5) && (iPass == 1)", satisfy)
        self.assertIn("p_Var29->overlap_4 & 0xfbff | 0x400", satisfy)

        # The sorted file order makes fleet 2 the Merge2Fleets destination.
        # Turin assigns it task 5 after the merge; pass 1 then retires it.
        self.assertEqual([pair["fleet"]["id"] for pair in
                          self._owner1_fleets(
                              self._records("vmctl-sye3k0", "CLEAN.HST"), 1)],
                         [0, 1])

    def test_random8_result_is_observable_and_clean_runs_are_not_seed_matched(self):
        # FGenerateTurn writes its Random(8) result into game.overlap_16, and
        # WriteBOF copies those three bits into each turn-1 BOF flags word.
        # This gives a direct observation of one draw without pretending that
        # it recovers the earlier seed or every intervening call.
        oracle = json.loads((ROOT / "analysis/compat/"
                             "satisfy-orders-full-turn-e3-original-oracle.json")
                            .read_text(encoding="utf-8"))
        clean_a_command = oracle["original_batch_replay"]["run_command"]
        clean_b = self._run("vmctl-sye3k0")
        for filename in ("CLEAN.HST", "CLEAN.M1", "CLEAN.M2"):
            with self.subTest(input_file=filename):
                a = ROOT / "runs/vmctl-syorig1/BACKUP" / filename
                b = ROOT / "runs/vmctl-sye3k0/BACKUP" / filename
                self.assertEqual(hashlib.sha256(a.read_bytes()).digest(),
                                 hashlib.sha256(b.read_bytes()).digest())
        self.assertIn("STARS.EXE -g", clean_a_command)
        self.assertIn("STARS.EXE -g", clean_b["run_command"])
        self.assertEqual(self._turn_random8("vmctl-syorig1"), 0)
        self.assertEqual(self._turn_random8("vmctl-sye3k0"), 1)
        self.assertEqual(self._turn_random8("vmctl-sye3k1"), 4)

        generate = (ROOT / "analysis/decompiler/export/raw/10b0_0000_FGenerateTurn.c").read_text()
        write_bof = (ROOT / "analysis/decompiler/export/raw/1070_8ea4_WriteBOF.c").read_text()
        self.assertIn("uVar19 = Random(8);", generate)
        self.assertIn("((uint)game.overlap_16 >> 9) << 0xd", write_bof)

    def test_fresh_pair_seeded_turn_rng_trace_through_output_writers(self):
        oracle = json.loads((ROOT / "analysis/compat/"
                             "satisfy-orders-full-turn-e3-original-oracle.json")
                            .read_text(encoding="utf-8"))
        seed_words = oracle["rng_initialization_trace"][
            "startup_randomization"]["effective_seed_words_by_run"]
        mining = self.pair["mining_random100_threshold_observations"]["outputs"]
        guess_bounds = oracle["rng_initialization_trace"][
            "retained_update_guesses_draw_bounds"]["output_draw_bounds"]
        recovered_trace = oracle["rng_initialization_trace"][
            "recovered_turn_rng_trace"]
        random = self.inspector.lib.stars_random
        random.argtypes = [POINTER(Seeds), c_int16]
        random.restype = c_int16

        plans = {
            "control": {
                "run": "vmctl-sye3k0",
                "phases": [
                    (1, "startup, AI outputs and player shuffle",
                     [13] * 4 + [100, 100, 2000, 2000, 2, 1],
                     [4, 9, 7, 0, 99, 7, 1240, 1434, 0, 0]),
                    (11, "task-5 technology learning",
                     [100] + [13] * 13 + [6] * 6,
                     [74, 8, 1, 9, 2, 3, 4, 0, 8, 0, 10, 11, 3, 2, 0,
                      5, 2, 4, 4, 4]),
                    (31, "mineral rounding", [100] * 6,
                     [67, 11, 28, 32, 99, 3]),
                    (37, "turn-zero event gates", [20, 20, 15], [19, 18, 10]),
                    (40, "post-production wormhole movement",
                     [100, 25, 25] * 4,
                     [29, 8, 17, 21, 9, 0, 28, 6, 18, 77, 11, 13]),
                    (52, "planet population guesses", [71, 64], [63, 6]),
                    (54, "serialized turn flag", [8], [1]),
                    (55, "turn output BOF writers", [2000] * 3,
                     [153, 442, 1900]),
                ],
                "random8": 1,
                "total": 57,
            },
            "action": {
                "run": "vmctl-sye3k1",
                "phases": [
                    (1, "startup, AI outputs and player shuffle",
                     [13] * 4 + [100, 100, 2000, 2000, 2, 1],
                     [9, 5, 6, 10, 4, 75, 1922, 267, 1, 0]),
                    (11, "task-5 technology-learning gates", [100, 100],
                     [43, 12]),
                    (13, "mineral rounding", [100] * 6,
                     [95, 17, 59, 38, 26, 18]),
                    (19, "turn-zero event gates", [20, 20, 15], [11, 7, 2]),
                    (22, "post-production wormhole movement",
                     [100, 25, 25] * 4,
                     [62, 15, 13, 20, 5, 19, 78, 16, 23, 69, 14, 23]),
                    (34, "planet population guesses", [71, 64], [59, 1]),
                    (36, "serialized turn flag", [8], [4]),
                    (37, "turn output BOF writers", [2000] * 3,
                     [1114, 424, 993]),
                ],
                "random8": 4,
                "total": 39,
            },
        }

        tech = (ROOT / "analysis/decompiler/export/raw/"
                "10f0_9918_ITechLearnATech.c").read_text()
        self.assertIn("sVar3 = Random(100), 0x31 < sVar3", tech)
        self.assertIn("iStack_a < 0xd", tech)
        self.assertIn("sVar3 = Random(0xd);", tech)
        self.assertIn("iStack_a < 6", tech)
        self.assertIn("sVar3 = Random(6);", tech)

        for label, plan in plans.items():
            with self.subTest(run=label):
                state = Seeds(*seed_words[label])
                position = 0
                phases = {}
                actual_draws = []
                for first_position, phase, limits, expected in plan["phases"]:
                    self.assertEqual(position + 1, first_position, phase)
                    values = [random(byref(state), limit) for limit in limits]
                    self.assertEqual(values, expected, phase)
                    phases[phase] = values
                    actual_draws.extend((position + index + 1, limit, value)
                                        for index, (limit, value) in enumerate(
                                            zip(limits, values)))
                    position += len(values)

                self.assertEqual(position, plan["total"])
                documented_draws = []
                for row in recovered_trace["phase_draws_by_run"][label]:
                    first, last = row["positions"]
                    self.assertEqual(last - first + 1, len(row["limits"]))
                    self.assertEqual(len(row["limits"]), len(row["values"]))
                    documented_draws.extend(
                        (first + index, limit, value)
                        for index, (limit, value) in enumerate(
                            zip(row["limits"], row["values"])))
                self.assertEqual(actual_draws, documented_draws)
                self.assertEqual(phases["serialized turn flag"][0],
                                 self._turn_random8(plan["run"]))
                self.assertEqual(self._turn_random8(plan["run"]),
                                 plan["random8"])
                self.assertEqual(recovered_trace["call_count_by_run"][label],
                                 plan["total"])
                self.assertEqual(recovered_trace["random8_position_by_run"][label],
                                 plan["total"] - 3)

                # The recovered draws fall inside the independent stockpile
                # bounds derived from the original turn's retained outputs.
                for value, result in zip(phases["mineral rounding"],
                                         mining[label]):
                    low, high = result["random100_range_inclusive"]
                    self.assertLessEqual(low, value)
                    self.assertLessEqual(value, high)

                # All three event gates are nonzero, so their conditional
                # planet/climate draws are absent on these turn-zero inputs.
                self.assertTrue(all(phases["turn-zero event gates"]))

                # Each zero-chance wormhole uses its first local candidate.
                # The seeded draws now equal the coordinate deltas retained
                # in the original turn-1 HST records.
                before = [record["body"] for record in self._records(
                    plan["run"], "BACKUP/CLEAN.HST")
                    if record["kind"] == 43 and record["turn"] == 0][1:]
                after = [record["body"] for record in self._records(
                    plan["run"], "CLEAN.HST")
                    if record["kind"] == 43 and record["turn"] == 1][1:]
                wormhole_draws = phases["post-production wormhole movement"]
                self.assertEqual((len(before), len(after)), (4, 4))
                for index, (old, new) in enumerate(zip(before, after)):
                    gate, x_draw, y_draw = wormhole_draws[index * 3:index * 3 + 3]
                    old_x, old_y = struct.unpack_from("<hh", old, 2)
                    new_x, new_y = struct.unpack_from("<hh", new, 2)
                    self.assertGreater(gate, 0)
                    self.assertEqual((x_draw, y_draw),
                                     (new_x - old_x + 12,
                                      new_y - old_y + 12))

                for value, bound in zip(phases["planet population guesses"],
                                        guess_bounds[label]):
                    self.assertLessEqual(bound["draw_range_inclusive"][0],
                                         value)
                    self.assertLessEqual(value,
                                         bound["draw_range_inclusive"][1])

                # BOF records store tick+Random(2000) modulo 2048. Subtracting
                # the recovered writer values yields a short, increasing tick
                # sequence, independently checking both late writer values.
                writer_values = (
                    phases["startup, AI outputs and player shuffle"][6:8] +
                    phases["turn output BOF writers"])
                writer_records = self.pair[
                    "write_bof_tick_plus_random2000_mod_2048"]["runs"][label][
                        "writes"]
                tick_residues = [
                    (record["tick_plus_random2000_mod_2048"] - value) % 2048
                    for record, value in zip(writer_records, writer_values)
                ]
                self.assertEqual(tick_residues,
                                 recovered_trace["writer_tick_low11_by_run"][label])
                self.assertEqual(tick_residues,
                                 sorted(tick_residues))

    def test_randomize2_tick_seed_search_uses_only_low_fourteen_bits(self):
        randomize2 = self.inspector.lib.stars_randomize2
        randomize2.argtypes = [POINTER(Seeds), c_uint32]
        randomize2.restype = None
        for tick_low14 in range(1 << 14):
            low = Seeds()
            high = Seeds()
            randomize2(byref(low), tick_low14)
            randomize2(byref(high), tick_low14 | 0xffffc000)
            self.assertEqual((high.first, high.second),
                             (low.first, low.second), tick_low14)

        winmain = (ROOT / "analysis/decompiler/export/raw/"
                   "1018_0000_WinMain.c").read_text()
        randomize2_source = (ROOT / "analysis/decompiler/export/raw/"
                             "1040_165a_Randomize2.c").read_text()
        self.assertIn("Randomize2(dw);", winmain)
        self.assertIn("uVar1 = (uint)dw & 0x7f ^ 0x35;",
                      randomize2_source)
        self.assertIn("uStack_8 = (uint)lVar2 & 0x7f ^ 0x5c;",
                      randomize2_source)

    def test_production_merge_and_turin_task5_scrap_slice(self):
        oracle = json.loads((ROOT / "analysis/compat/"
            "satisfy-orders-full-turn-e3-original-oracle.json"
        ).read_text(encoding="utf-8"))
        planet_oracle = oracle["current_source_control_action_pair"][
            "owner1_planet24"]
        mining_inputs = planet_oracle["production_mining_stage"][
            "retained_owner1_mining_inputs"]
        production_input = oracle["turin_turn_zero_seed_queue_replay"][
            "current_source_probe"]["input"]
        production_costs = planet_oracle["production_mining_stage"][
            "owner1_exact_mineral_stock_witness"][
                "runtime_production_cost_order"]
        fresh_pair = json.loads((ROOT / "runs/vmctl-sye3k-pair-r1/PAIR.JSON")
                                .read_text(encoding="utf-8"))
        draw_bounds = fresh_pair[
            "mining_random100_threshold_observations"]["outputs"]
        recovered_rng = oracle["rng_initialization_trace"][
            "recovered_turn_rng_trace"]["phase_draws_by_run"]["action"]
        tech_gate = next(phase for phase in recovered_rng
                         if phase["phase"] ==
                         "task-5 technology-learning gates")
        self.assertEqual(tech_gate["positions"], [11, 12])
        self.assertEqual(tech_gate["limits"], [100, 100])
        self.assertEqual(tech_gate["values"][0], 43)
        tech_draw = tech_gate["values"][0]
        mining_phase = next(phase for phase in recovered_rng
                            if phase["phase"] == "MineMinerals rounding")
        self.assertEqual(mining_phase["positions"], [13, 18])
        self.assertEqual(mining_phase["limits"], [100] * 6)
        action_planet24_draws = tuple(mining_phase["values"][3:6])
        self.assertEqual(action_planet24_draws, (38, 26, 18))
        records = self._records("vmctl-sye3k0", "BACKUP/CLEAN.M2")
        pairs = self._owner1_fleets(records, 0)
        fleets = {pair["fleet"]["id"]: pair["fleet"] for pair in pairs}
        self.assertEqual(set(fleets), {0, 1, 2, 3})
        self.assertEqual(hashlib.sha256(
            (ROOT / "runs/vmctl-sye3k0/BACKUP/CLEAN.M2").read_bytes()
        ).hexdigest(), "588830ac2225056422fa447cb201c7bd326e472652a8d3abf222b5fcceb99d4b")

        design = next(record["body"] for record in records
                      if record["kind"] == 26 and
                      ((int.from_bytes(record["body"][:2], "little") >> 10)
                       & 31) == 2)
        exe_path = ROOT / "target/stars.exe"
        self.assertEqual(hashlib.sha256(exe_path.read_bytes()).hexdigest(),
                         STARS_EXE_SHA256)
        original_segment = bytes(NE(exe_path).raw[1])
        mine_record = original_segment[0x1508:0x1508 + 0x36]
        self.assertEqual(len(mine_record), 0x36)
        mine_dp, = struct.unpack_from("<H", mine_record, 0x34)
        self.assertEqual(mine_dp, 5)
        self.assertEqual(struct.unpack_from("<4H", mine_record, 0x2a),
                         (50, 14, 0, 4))
        component_count = design[6]
        components = [struct.unpack_from("<HH", design, 17 + i * 4)
                      for i in range(component_count)]
        self.assertIn((0x80, 0x0000 | (2 << 8)), components)
        self.assertEqual(mine_dp * 2, 10)

        selected = [fleets[index] for index in (2, 3)]
        for fleet in selected:
            self.assertEqual(fleet["owner"], 1)
            self.assertEqual(fleet["planet"], 24)
            self.assertEqual(fleet["point"], [1290, 1258])
            self.assertEqual(fleet["ship_counts"][2], 1)
        self.assertEqual(selected[0]["cargo"], [0, 0, 0, 0, 210])
        self.assertEqual(selected[1]["cargo"], [0, 0, 0, 0, 210])
        self.assertEqual([pair["fleet"]["id"] for pair in
                          self._owner1_fleets(
                              self._records("vmctl-sye3k0", "CLEAN.HST"), 1)],
                         [0, 1])
        player_records = [record["body"] for record in records
                          if record["kind"] == 6 and record["body"] and
                          record["body"][0] == 1]
        self.assertEqual(len(player_records), 1)
        player = player_records[0]
        planet_records = [record for record in records
                          if record["kind"] == 13 and len(record["body"]) >= 2
                          and (int.from_bytes(record["body"][:2], "little")
                               & 0x7ff) == 24]
        self.assertEqual(len(planet_records), 1)
        planet_record = planet_records[0]
        self.assertEqual(planet_record["body"].hex(),
                         "1808872b004457443e213d3e006a1402bc03f401fa000aa0000a00000000000000")
        self.assertEqual(design.hex(),
                         "0709145200000201000200000002000000010001018000000207bf7a1a70b1ded8")
        part_segment = original_segment[:0x29f0 + 21 * 0x8f]
        self.assertEqual(len(part_segment), 0x35ab)
        header = [
            "static const StarsI16 retained_ids[2] = {%s};" %
            ", ".join(str(f["id"]) for f in selected),
            "static const StarsI16 retained_players[2] = {%s};" %
            ", ".join(str(f["owner"]) for f in selected),
            "static const StarsI16 retained_planets[2] = {%s};" %
            ", ".join(str(f["planet"]) for f in selected),
            "static const StarsI16 retained_points[2][2] = {%s};" %
            ", ".join("{%d, %d}" % tuple(f["point"]) for f in selected),
            "static const StarsU16 retained_ship_counts[2][16] = {%s};" %
            ", ".join("{%s}" % ", ".join(map(str, f["ship_counts"]))
                      for f in selected),
            "static const StarsI16 retained_slot2_counts[2] = {%s};" %
            ", ".join(str(f["ship_counts"][2]) for f in selected),
            "static const StarsI16 retained_mine_value = %d;" %
            (mine_dp * 2),
            "static const StarsU32 retained_cargo[2][5] = {%s};" %
            ", ".join("{%s}" % ", ".join("%dUL" % value
                      for value in f["cargo"]) for f in selected),
            "static const signed char retained_player_technology[6] = {%s};" %
            ", ".join(str(value) for value in player[26:32]),
            "static const StarsU32 retained_player_race_bits = 0x%08xUL;" %
            struct.unpack_from("<I", player, 78)[0],
            "static const StarsI16 retained_player_race_stat = %d;" %
            player[76],
            "static const StarsU16 retained_planet_header_flags = 0x%04xU;" %
            ((planet_record["kind"] << 10) | planet_record["length"]),
            "static const StarsU16 retained_planet_turn = %d;" %
            planet_record["turn"],
            "#define retained_planet_runtime_flags 0x%04xU" %
            int(production_input["planet_runtime_flags_word"], 16),
            "#define retained_planet_population %dUL" %
            planet_oracle["input"]["minerals_and_colonists"][3],
            "#define retained_map_points %dU" %
            oracle["xy_coordinate_crosscheck"]["point_count"],
            "#define retained_mine_efficiency_tenths %d" %
            mining_inputs["player_race_stats"]["mine_output_tenths"],
            "#define retained_operating_mines %d" %
            mining_inputs["operating_mines"],
            "#define retained_special_race %d" % int(
                mining_inputs["player_race_stats"][
                    "special_race_stat_14"] == 8),
            "static const StarsU8 retained_planet_environment[3] = {%s};" %
            ", ".join(map(str, planet_oracle["input"]["environment"])),
            "static const StarsU8 retained_planet_concentrations[3] = {%s};" %
            ", ".join(map(str, mining_inputs["planet24_concentrations"])),
            "static const StarsU8 retained_planet_percent_levels[3] = {%s};" %
            ", ".join(map(str, mining_inputs["planet24_percent_levels"])),
            "static const StarsU8 retained_player_environment[9] = {%s};" %
            ", ".join(map(str, player[16:25])),
            "static const StarsI32 retained_production_costs[4] = {%s};" %
            ", ".join(map(str, production_costs)),
            "static const StarsU16 retained_player_research_percent = %dU;" % player[56],
            c_byte_array("retained_shdef_record", design),
            c_byte_array("retained_planet_body", planet_record["body"]),
            c_byte_array("retained_part_segment", part_segment),
            c_byte_array("retained_decode_prefix", self.inspector.initial[:8192]),
        ]

        inspector = self.inspector  # Also ensures the host core library exists.
        self.assertIsNotNone(inspector)
        library = ROOT / "build/libstars_core.dylib"
        self.assertTrue(library.exists())
        with tempfile.TemporaryDirectory() as temp:
            temp_path = Path(temp)
            (temp_path / "SATISFYORD.H").write_text(
                "#include \"core.h\"\n" + "\n".join(header) + "\n",
                encoding="ascii")
            exe = temp_path / "merge_scrap_probe"
            subprocess.run([
                "cc", "-std=c89", "-pedantic", "-Wall", "-Wextra",
                "-Werror", "-I", str(ROOT / "reconstructed/include"),
                "-I", str(temp_path),
                str(ROOT / "tests/satisfy_orders_fresh_ai_merge_scrap_probe.c"),
                str(ROOT / "reconstructed/src/do_orders.c"),
                str(ROOT / "reconstructed/src/merge_fleets.c"),
                str(ROOT / "reconstructed/src/fleet_transfer_cargo_balance.c"),
                str(ROOT / "reconstructed/src/invalidate_report.c"),
                str(ROOT / "reconstructed/src/do_turin_drone_ai_turn.c"),
                str(ROOT / "reconstructed/src/ai_plord.c"),
                str(ROOT / "reconstructed/src/satisfy_orders.c"),
                str(ROOT / "reconstructed/src/est_minerals_mined.c"),
                str(ROOT / "reconstructed/src/produce.c"),
                str(ROOT / "reconstructed/src/c_build_prod_item.c"),
                str(ROOT / "reconstructed/src/lookup_part.c"),
                str(ROOT / "reconstructed/src/reprice_shdef.c"),
                str(ROOT / "reconstructed/src/true_part_cost.c"),
                str(ROOT / "reconstructed/src/mark_techs.c"),
                str(ROOT / "reconstructed/src/itech_learn_a_tech.c"),
                str(library), "-Wl,-rpath," + str(library.parent),
                "-o", str(exe),
            ], check=True)
            for label, draws in (("control", (0, 99, 0)),
                                 ("action", action_planet24_draws)):
                bounds = draw_bounds[label][3:6]
                for draw, bound in zip(draws, bounds):
                    lower, upper = bound["random100_range_inclusive"]
                    self.assertGreaterEqual(draw, lower)
                    self.assertLessEqual(draw, upper)
                result = subprocess.run([str(exe), str(tech_draw),
                                         *map(str, draws)],
                    capture_output=True, text=True, check=False)
                self.assertEqual(result.returncode, 0,
                                 result.stdout + result.stderr)
                lines = result.stdout.strip().splitlines()
                self.assertRegex(lines[0],
                    r"SOE3_MERGE_SCRAP_CHECKS=[0-9]+ FAILURES=0 "
                    r"RESULT=PASS COST=123,41,0,12 "
                    r"PLANET=597,956,519,250 MSG=90,321 "
                    r"TECH_LOOKUPS=[0-9]+ TECH_DRAW=43 TECH_RANDOM=1")
                expected_yield = [entry["yield"] for entry in bounds]
                expected_stock = [start + mined for start, mined in zip(
                    (597, 956, 519), expected_yield)]
                expected_ending = [592,
                    964 if label == "action" else 963, 520]
                self.assertEqual(lines[1],
                    "PASSES=1,2,3,4 TURIN_QUEUE=2 DESIRABILITY=100 "
                    "MINE=%s STOCK=%s "
                    "PRODUCE=%s ITEMS=2 ITEM0=0,2,81 ITEM1=0,2,0" % (
                        ",".join(map(str, expected_yield)),
                        ",".join(map(str, expected_stock)),
                        ",".join(map(str, expected_ending))))

    def test_outputs_and_attached_corrupt_x2_case_are_separate(self):
        control_input_hst = self._records(
            "vmctl-sye3k0", "BACKUP/CLEAN.HST")
        control_hst = self._records("vmctl-sye3k0", "CLEAN.HST")
        action_hst = self._records("vmctl-sye3k1", "CLEAN.HST")
        control_m2 = self._records("vmctl-sye3k0", "CLEAN.M2")
        action_m2 = self._records("vmctl-sye3k1", "CLEAN.M2")
        control_fleets = self._owner1_fleets(control_hst, 1)
        action_fleets = self._owner1_fleets(action_hst, 1)
        input_fleets = self._owner1_fleets(control_input_hst, 0)
        self.assertEqual([pair["fleet"]["id"] for pair in input_fleets],
                         [0, 1, 2, 3])
        self.assertEqual([pair["fleet"]["id"] for pair in control_fleets],
                         [0, 1])
        self.assertEqual(control_fleets, action_fleets)
        self.assertEqual(control_fleets, self._owner1_fleets(control_m2, 1))
        self.assertEqual(action_fleets, self._owner1_fleets(action_m2, 1))
        self.assertEqual(control_fleets,
                         self.pair["original_outputs"]
                         ["owner1_turn1_fleet_order_pairs"])
        self.assertEqual(control_fleets[0]["fleet"]["planet"], 26)
        self.assertEqual(control_fleets[0]["order"]["warp"], 5)
        self.assertEqual(control_fleets[1]["fleet"]["cargo"][3], 25)

        control_planet = self._planet24(control_hst, 1)
        action_planet = self._planet24(action_hst, 1)
        self.assertEqual(control_planet["minerals"], [592, 963, 520, 256])
        self.assertEqual(action_planet["minerals"], [592, 964, 520, 256])
        self.assertEqual(control_planet["auxiliary_word"], 8249)
        self.assertEqual(action_planet["auxiliary_word"], 8248)
        self.assertEqual(self.pair["original_outputs"]
                         ["planet24_deltas_action_minus_control"],
                         {"minerals": [0, 1, 0, 0], "auxiliary_word": -1})

        control_h2 = self._records("vmctl-sye3k0", "CLEAN.H2")
        action_h2 = self._records("vmctl-sye3k1", "CLEAN.H2")
        self.assertEqual([(r["kind"], r["body"]) for r in control_h2[1:]],
                         [(r["kind"], r["body"]) for r in action_h2[1:]])
        self.assertTrue(self.pair["original_outputs"]
                        ["h2_post_run_comparison"]
                        ["decrypted_non_bof_records_identical"])
        self.assertIn("do not recover the absent starting H2",
                      self.pair["original_outputs"]
                      ["h2_post_run_comparison"]["interpretation"])

        # Pin all original control/action output-record differences by file,
        # turn, and kind. This inventory is not causal attribution and does
        # not claim that production reproduces every changed record.
        expected_output_census = {
            "CLEAN.HST": {
                "control_only": {"1:6": 1, "1:8": 1, "1:13": 2,
                                 "1:16": 1, "1:20": 1, "1:26": 5,
                                 "1:43": 4},
                "action_only": {"1:6": 1, "1:8": 1, "1:13": 2,
                                "1:26": 5, "1:43": 4},
            },
            "CLEAN.M1": {
                "control_only": {"1:6": 1, "1:8": 1, "1:12": 1,
                                 "1:13": 1, "1:16": 1, "1:20": 1,
                                 "1:26": 1, "1:45": 1},
                "action_only": {"1:6": 1, "1:8": 1, "1:12": 1,
                                "1:13": 1, "1:26": 1, "1:45": 1},
            },
            "CLEAN.M2": {
                "control_only": {"1:8": 1, "1:13": 1, "1:26": 4},
                "action_only": {"1:8": 1, "1:13": 1, "1:26": 4},
            },
            "CLEAN.H2": {
                "control_only": {"0:8": 1},
                "action_only": {"0:8": 1},
            },
        }
        actual_output_census = {}
        for filename in expected_output_census:
            control_records = self._records("vmctl-sye3k0", filename)
            action_records = self._records("vmctl-sye3k1", filename)
            control_bodies = Counter((record["turn"], record["kind"],
                                      record["body"])
                                     for record in control_records)
            action_bodies = Counter((record["turn"], record["kind"],
                                     record["body"])
                                    for record in action_records)

            def grouped_census(records):
                grouped = Counter()
                for (turn, kind, _body), count in records.items():
                    grouped[(turn, kind)] += count
                return {"%d:%d" % key: value
                        for key, value in sorted(grouped.items()) if value}

            actual_output_census[filename] = {
                "control_only": grouped_census(control_bodies - action_bodies),
                "action_only": grouped_census(action_bodies - control_bodies),
            }
        self.assertEqual(actual_output_census, expected_output_census)
        oracle = json.loads((ROOT / "analysis/compat/"
            "satisfy-orders-full-turn-e3-original-oracle.json"
        ).read_text(encoding="utf-8"))
        retained_census = oracle["current_source_control_action_pair"][
            "retained_original_output_record_census"]
        self.assertEqual(actual_output_census, {
            filename: {
                "control_only": retained_census["control_only"][filename],
                "action_only": retained_census["action_only"][filename],
            }
            for filename in expected_output_census
        })

        def owner0_player_body(records):
            bodies = [record["body"] for record in records
                      if record["turn"] == 1 and record["kind"] == 6 and
                      record["body"] and record["body"][0] == 0]
            self.assertEqual(len(bodies), 1)
            return bodies[0]

        owner0_player_counts = {}
        for filename in ("CLEAN.HST", "CLEAN.M1"):
            control_body = owner0_player_body(
                self._records("vmctl-sye3k0", filename))
            action_body = owner0_player_body(
                self._records("vmctl-sye3k1", filename))
            self.assertEqual(len(control_body), 130)
            self.assertEqual(len(action_body), 130)
            self.assertEqual([offset for offset, (left, right) in
                              enumerate(zip(control_body, action_body))
                              if left != right], [4])
            control_count = int.from_bytes(control_body[4:6], "little") & 0x0fff
            action_count = int.from_bytes(action_body[4:6], "little") & 0x0fff
            self.assertEqual((control_count, action_count), (6, 5))
            owner0_player_counts[filename] = {
                "control_cFleet": control_count,
                "action_cFleet": action_count,
            }
        self.assertEqual(owner0_player_counts,
                         retained_census["owner0_player_cFleet"])

        score_bodies = {}
        for label, run_name in (("control", "vmctl-sye3k0"),
                                ("action", "vmctl-sye3k1")):
            records = self._records(run_name, "CLEAN.M1")
            bodies = [record["body"] for record in records
                      if record["turn"] == 1 and record["kind"] == 45 and
                      (int.from_bytes(record["body"][:2], "little") & 0x1f)
                      == 0]
            self.assertEqual(len(bodies), 1)
            score_bodies[label] = bodies[0]
        self.assertEqual(len(score_bodies["control"]), 24)
        self.assertEqual(len(score_bodies["action"]), 24)
        self.assertEqual(score_bodies["control"][2:4], b"\x01\x00")
        self.assertEqual(score_bodies["action"][2:4], b"\x01\x00")
        score_totals = {
            label: {
                "score": struct.unpack_from("<i", body, 4)[0],
                "resources": struct.unpack_from("<i", body, 8)[0],
                "planets": struct.unpack_from("<H", body, 12)[0],
                "starbases": struct.unpack_from("<H", body, 14)[0],
                "technology_levels": struct.unpack_from("<H", body, 22)[0],
            }
            for label, body in score_bodies.items()
        }
        self.assertEqual(score_totals, {
            "control": {"score": 25, "resources": 38, "planets": 1,
                        "starbases": 1, "technology_levels": 18},
            "action": {"score": 25, "resources": 38, "planets": 1,
                       "starbases": 1, "technology_levels": 18},
        })
        self.assertEqual([offset for offset, (left, right) in
                          enumerate(zip(score_bodies["control"],
                                        score_bodies["action"]))
                          if left != right], [18])
        score_lane_counts = {
            label: list(struct.unpack_from("<3H", body, 16))
            for label, body in score_bodies.items()
        }
        self.assertEqual(score_lane_counts,
                         retained_census["owner0_score_history_rgcsh"])

        # Decode the changed type-26 records using the retained _rtshdef
        # layout: a 17-byte header, component pairs, then the name tail.
        # This separates the retired design's cExist update from AI name data.
        retained_shdef_audit = {}
        decoded_ai_design_names = {
            filename: {"control": {}, "action": {}}
            for filename in ("CLEAN.HST", "CLEAN.M2")
        }
        expected_changed_slots = {
            "CLEAN.HST": [0, 17, 18, 19, 20],
            "CLEAN.M1": [0],
            "CLEAN.M2": [17, 18, 19, 20],
        }
        for filename, expected_slots in expected_changed_slots.items():
            control_records = self._records("vmctl-sye3k0", filename)
            action_records = self._records("vmctl-sye3k1", filename)
            control_bodies = Counter((r["turn"], r["kind"], r["body"])
                                     for r in control_records)
            action_bodies = Counter((r["turn"], r["kind"], r["body"])
                                    for r in action_records)

            def changed_shdefs(delta):
                result = {}
                for (turn, kind, body), count in delta.items():
                    if turn != 1 or kind != 26:
                        continue
                    slot = (int.from_bytes(body[:2], "little") >> 10) & 31
                    self.assertNotIn(slot, result)
                    result[slot] = body
                    self.assertEqual(count, 1)
                return result

            control_changed = changed_shdefs(control_bodies - action_bodies)
            action_changed = changed_shdefs(action_bodies - control_bodies)
            self.assertEqual(sorted(control_changed), expected_slots)
            self.assertEqual(sorted(action_changed), expected_slots)
            self.assertEqual(set(control_changed), set(action_changed))
            for slot in expected_slots:
                before = control_changed[slot]
                after = action_changed[slot]
                self.assertEqual(before[6], after[6])
                component_end = 17 + before[6] * 4
                self.assertLessEqual(component_end, len(before))
                self.assertLessEqual(component_end, len(after))
                if slot == 0:
                    self.assertEqual(len(before), len(after))
                    self.assertEqual([offset for offset, (left, right) in
                                      enumerate(zip(before, after))
                                      if left != right], [13])
                    self.assertEqual(struct.unpack_from("<I", before, 13)[0], 1)
                    self.assertEqual(struct.unpack_from("<I", after, 13)[0], 0)
                else:
                    self.assertEqual(before[:17], after[:17])
                    self.assertEqual(before[17:component_end],
                                     after[17:component_end])
                    self.assertNotEqual(before[component_end:],
                                        after[component_end:])
                    for label, body in (("control", before),
                                        ("action", after)):
                        tail = body[component_end:]
                        encoded_length = tail[0]
                        encoded = tail[1:1 + encoded_length]
                        self.assertEqual(1 + encoded_length, len(tail))
                        name = _decompress_user_name(encoded)
                        self.assertEqual(_compress_user_name(name), encoded)
                        decoded_ai_design_names[filename][label][str(slot)] = name
            retained_shdef_audit[filename] = {
                "control_only_slots": sorted(control_changed),
                "action_only_slots": sorted(action_changed),
            }
        self.assertEqual(retained_shdef_audit,
                         retained_census["shdef_semantic_audit"]
                         ["changed_design_slots"])
        self.assertEqual(decoded_ai_design_names["CLEAN.HST"],
                         decoded_ai_design_names["CLEAN.M2"])
        self.assertEqual(decoded_ai_design_names["CLEAN.HST"],
                         retained_census["shdef_semantic_audit"]
                         ["ai_design_name_tails"]["decoded_names_by_run"])

        for label, run_name, expected_id in (
                ("control", "vmctl-sye3k0", 63),
                ("action", "vmctl-sye3k1", 90)):
            with self.subTest(message=label):
                records = self._records(run_name, "CLEAN.M1")
                messages = [r["body"] for r in records
                            if r["turn"] == 1 and r["kind"] == 12]
                self.assertEqual(len(messages), 1)
                self.assertEqual(int.from_bytes(messages[0][:2], "little")
                                 & 0x1ff, expected_id)

        # The screenshot identifies the older C:\SOTC candidate. Its file
        # has valid stream framing, but Stars! explicitly rejected it.
        sotc = self.pair["attached_corrupt_x2_case"]
        manifest = json.loads((ROOT / sotc["run_manifest"])
                              .read_text(encoding="utf-8"))
        self.assertEqual(manifest["observed_error"],
                         "Player log file C:\\SOTC\\CLEAN.X2 appears to be corrupt, unable to load file.")
        screenshot_info = sotc["user_supplied_error_screenshot"]
        screenshot = ROOT / screenshot_info["path"]
        self.assertTrue(screenshot.is_file())
        self.assertEqual(screenshot.stat().st_size, 7398)
        self.assertEqual(hashlib.sha256(screenshot.read_bytes()).hexdigest(),
                         "254a4c6baf7cc560dde3976da7e16d6bb26396cae43a26c0749463b1ed4db780")
        self.assertEqual(screenshot_info["message"], sotc["dialog"])
        self.assertEqual(manifest["user_supplied_error_screenshot"],
                         screenshot_info)
        diagnostic_manifest = json.loads((ROOT / "runs/diagnostic-soe3-x2-20260924/MANIFEST.JSON")
                                         .read_text(encoding="utf-8"))
        self.assertEqual(diagnostic_manifest["user_supplied_error_screenshot"],
                         screenshot_info)
        e3_oracle = json.loads((ROOT / "analysis/compat/"
                                "satisfy-orders-full-turn-e3-original-oracle.json")
                               .read_text(encoding="utf-8"))
        self.assertEqual(e3_oracle["original_batch_replay"]
                         ["attached_corrupt_x2_case"]
                         ["user_supplied_screenshot"], screenshot_info)
        candidate = ROOT / sotc["returned_x2"]["path"]
        self.assertEqual(candidate.stat().st_size, 355)
        self.assertEqual(hashlib.sha256(candidate.read_bytes()).hexdigest(),
                         "17cf6d4b2dd559360247ff66c01430c0fb2801d879a1230866cf13919a7905e0")
        candidate_records = decode_stream(candidate, self.inspector)
        self.assertEqual(len(candidate_records), 13)
        x2_comparison = sotc["paired_generated_x2_comparison"]
        self.assertEqual([record["kind"] for record in candidate_records],
                         x2_comparison["record_kinds"])
        candidate_bof = struct.unpack_from("<4sI4H", candidate.read_bytes(), 2)
        candidate_salt_player = candidate_bof[4]
        candidate_salt = candidate_salt_player >> 5
        if candidate_salt >= 1024:
            candidate_salt -= 2048
        candidate_player = candidate_salt_player & 31
        if candidate_player >= 16:
            candidate_player -= 32
        self.assertEqual({
            "marker": candidate_bof[0].decode("ascii"),
            "game_id": candidate_bof[1],
            "version": candidate_bof[2],
            "turn": candidate_bof[3],
            "player": candidate_player,
            "flags": candidate_bof[5],
        }, x2_comparison["common_bof_fields"])
        self.assertEqual(candidate_salt, x2_comparison["bof_salt"]["rejected"])
        for label in ("control", "action"):
            with self.subTest(x2_reference=label):
                reference = x2_comparison["reference_files"][label]
                reference_path = ROOT / reference["path"]
                reference_raw = reference_path.read_bytes()
                self.assertEqual(len(reference_raw), reference["bytes"])
                self.assertEqual(hashlib.sha256(reference_raw).hexdigest(),
                                 reference["sha256"])
                reference_records = decode_stream(reference_path, self.inspector)
                self.assertEqual([record["kind"] for record in reference_records],
                                 x2_comparison["record_kinds"])
                reference_bof = struct.unpack_from("<4sI4H", reference_raw, 2)
                reference_salt_player = reference_bof[4]
                reference_salt = reference_salt_player >> 5
                if reference_salt >= 1024:
                    reference_salt -= 2048
                reference_player = reference_salt_player & 31
                if reference_player >= 16:
                    reference_player -= 32
                self.assertEqual({
                    "marker": reference_bof[0].decode("ascii"),
                    "game_id": reference_bof[1],
                    "version": reference_bof[2],
                    "turn": reference_bof[3],
                    "player": reference_player,
                    "flags": reference_bof[5],
                }, x2_comparison["common_bof_fields"])
                self.assertEqual(reference_salt,
                                 x2_comparison["bof_salt"][label])
                comparison = compare_records(candidate_records,
                                             reference_records)
                recorded = x2_comparison["comparisons"][label]
                self.assertEqual(comparison["framing_kinds_equal"],
                                 recorded["same_record_kinds"])
                self.assertEqual(comparison["plaintext_equal"],
                                 recorded["plaintext_equal"])
                self.assertEqual([item["index"]
                                  for item in comparison["differences"]],
                                 recorded["differing_record_indices"])
                self.assertEqual(
                    [record["length"] for record in candidate_records
                     if record["kind"] == 27],
                    recorded["type_27_lengths"]["rejected"])
                self.assertEqual(
                    [record["length"] for record in reference_records
                     if record["kind"] == 27],
                    recorded["type_27_lengths"]["reference"])
        current_guest = ROOT / sotc["current_guest_readonly_retrieval"]["path"]
        self.assertEqual(current_guest.read_bytes(), candidate.read_bytes())
        readback = json.loads((current_guest.parent / "READBACK.JSON")
                              .read_text(encoding="utf-8"))
        self.assertEqual(readback["matches_archived_file"],
                         sotc["returned_x2"]["path"])
        self.assertFalse(readback["guest_directory_listing_retained_before_run"])
        self.assertNotIn("CLEAN.X2", sotc["original_bundle_input_files"])
        self.assertFalse(sotc["guest_pre_run_listing_retained"])
        self.assertIn("not acceptance by Stars!", sotc["interpretation"])

        live_path = ROOT / sotc["latest_live_readonly_followup"]
        live = json.loads(live_path.read_text(encoding="utf-8"))
        live_file = live_path.parent / "CLEAN.X2"
        self.assertEqual(live_file.read_bytes(), candidate.read_bytes())
        self.assertFalse(live["guest_input_manifest"]
                         ["CLEAN.X2_listed_as_input"])
        self.assertTrue(any(item["name"] == "CLEAN.X2"
                            for item in live["guest_directory_listing"]
                            ["C:\\SOTC"]))
        self.assertFalse(live["clean_x2_readback"]
                         ["pre_run_presence_resolved"])
        self.assertFalse(live["guest_window_state"]
                         ["stars_application_window_present"])
        self.assertFalse(self.pair["full_turn_production_comparison_complete"])

    def test_new_turin_starbase_names_pin_accepted_random13_values(self):
        expected_names = {
            "control": {17: "Cube", 18: "Monolith",
                        19: "Guardian Angel",
                        20: "Hip to be Square"},
            "action": {17: "Potato", 18: "Hip to be Square",
                       19: "Sphere of Doom",
                       20: "Rock of Gibraltar"},
        }
        expected_ids = {
            "Cube": 0x439, "Monolith": 0x436,
            "Guardian Angel": 0x432, "Hip to be Square": 0x43b,
            "Potato": 0x438, "Sphere of Doom": 0x43c,
            "Rock of Gibraltar": 0x437,
        }
        creation_order = [18, 20, 17, 19]

        # Resolve the embedded strings with the recovered PszGetCompressedString
        # implementation. Its machine-level comparison against the original
        # executable covers all 1,414 ids in test_game_strings_machine.py.
        get_string = self.inspector.lib.stars_get_compressed_string
        get_string.argtypes = [POINTER(c_ubyte), POINTER(c_ubyte), c_int16]
        get_string.restype = c_uint16

        def game_string(string_id):
            memory = (c_ubyte * 65536)()
            memory[0x1484] = 0xff
            address = get_string(memory, self.inspector.strings, string_id)
            self.assertEqual(address, 0x547a)
            return bytes(memory)[address:].split(b"\0", 1)[0].decode("cp1252")

        pool = {string_id: game_string(string_id)
                for string_id in range(0x432, 0x432 + 13)}
        self.assertEqual({string_id: pool[string_id]
                          for string_id in expected_ids.values()}, {
            0x432: "Guardian Angel", 0x436: "Monolith",
            0x437: "Rock of Gibraltar", 0x438: "Potato",
            0x439: "Cube", 0x43b: "Hip to be Square",
            0x43c: "Sphere of Doom",
        })

        def stored_name(body):
            name_start = 17 + body[6] * 4
            tail = body[name_start:]
            encoded_length = tail[0]
            encoded = tail[1:1 + encoded_length]
            self.assertEqual(1 + encoded_length, len(tail))
            name = _decompress_user_name(encoded)
            self.assertEqual(_compress_user_name(name), encoded)
            return name

        output_ids_by_run = {}
        for label, run_name in (("control", "vmctl-sye3k0"),
                                ("action", "vmctl-sye3k1")):
            with self.subTest(run=label):
                input_records = [record for record in self._records(
                    run_name, "BACKUP/CLEAN.M2")
                    if record["turn"] == 0 and record["kind"] == 26]
                input_by_slot = {
                    (int.from_bytes(record["body"][:2], "little") >> 10) & 31:
                    record["body"] for record in input_records
                }
                self.assertEqual(sorted(input_by_slot), [0, 1, 2, 16])
                self.assertTrue(set(range(17, 21)).isdisjoint(input_by_slot))
                self.assertEqual(stored_name(input_by_slot[16]), "Starbase")

                output_records = self._records(run_name, "CLEAN.M2")
                output_bodies = {}
                output_names = {}
                for record in output_records:
                    if record["turn"] != 1 or record["kind"] != 26:
                        continue
                    body = record["body"]
                    slot = (int.from_bytes(body[:2], "little") >> 10) & 31
                    if slot not in range(17, 21):
                        continue
                    name = stored_name(body)
                    output_bodies[slot] = body
                    output_names[slot] = name
                self.assertEqual(output_names, expected_names[label])
                output_ids_by_run[label] = {
                    slot: expected_ids[name]
                    for slot, name in output_names.items()
                }

                # The name uniqueness check in PickANameAndBmp only compares
                # designs of the same hull. Retain the hull and bitmap bytes so
                # candidate retries and the optional Random(4) branch can be
                # bounded against the actual per-player design table.
                current_player_slots = set(range(16, 26))
                self.assertEqual(sorted(current_player_slots &
                                        set(input_by_slot)), [16])
                prior_bodies = [input_by_slot[16]]
                for slot in creation_order:
                    body = output_bodies[slot]
                    matching_hull = [prior for prior in prior_bodies
                                     if prior[2] == body[2]]
                    self.assertLess(len(matching_hull), 4)
                    if slot in (18, 17):
                        self.assertFalse(any(
                            stored_name(prior) in pool.values()
                            for prior in matching_hull))
                    prior_bodies.append(body)

                # The first and third starbase creations have no same-hull
                # name candidate to collide with, so these two Random(13)
                # returns are directly exposed. The second and fourth slots
                # expose accepted values; duplicate retries remain possible.
                self.assertEqual(output_ids_by_run[label][18] - 0x432,
                                 {"control": 4, "action": 9}[label])
                self.assertEqual(output_ids_by_run[label][17] - 0x432,
                                 {"control": 7, "action": 6}[label])

        raw = ROOT / "analysis/decompiler/export/raw"
        ensure_ais = (raw / "1020_56bc_EnsureAis.c").read_text()
        do_ai_turn = (raw / "1088_0000_DoAiTurn.c").read_text()
        turin_turn = (raw / "1088_3670_DoTurinDroneAiTurn.c").read_text()
        iro_ensure_ai = (raw / "1090_425a_IroEnsureAi.c").read_text()
        ensure_starbase = (raw / "1090_7222_EnsureAiStarbaseDesigns.c").read_text()
        create_starbase = (raw / "1090_7bca_FCreateAiStarbase.c").read_text()
        pick_name = (raw / "1090_055a_PickANameAndBmp.c").read_text()
        self.assertIn("DoAiTurn(iStack_10", ensure_ais)
        self.assertIn("DoTurinDroneAiTurn", do_ai_turn)
        self.assertIn("IroEnsureAi(vrgAiTurinDroneResOrder,0x1f,&sStack_92", turin_turn)
        self.assertIn("EnsureAiStarbaseDesigns();", iro_ensure_ai)
        before_late_turn_designs = ensure_starbase.split(
            "if (0x31 < game.turn)", 1)[0]
        self.assertEqual(re.findall(
            r"FCreateAiStarbase\((\d+),", before_late_turn_designs),
            ["2", "4", "1", "3"])
        self.assertIn("ishdef + 0x10", create_starbase)
        self.assertIn("PickANameAndBmp(&_Stack_9a,0x432,0xd", create_starbase)
        self.assertIn("sVar1 = Random(cids)", pick_name)
        self.assertIn("iStack_6 = Random(4)", pick_name)

        source_ids_by_run = {
            label: [output_ids_by_run[label][slot] for slot in creation_order]
            for label in ("control", "action")
        }
        self.assertEqual(source_ids_by_run, {
            "control": [0x436, 0x43b, 0x439, 0x432],
            "action": [0x43b, 0x437, 0x438, 0x43c],
        })

        oracle = json.loads((ROOT / "analysis/compat/"
            "satisfy-orders-full-turn-e3-original-oracle.json"
        ).read_text(encoding="utf-8"))
        name_rng = oracle["current_source_control_action_pair"][
            "retained_original_output_record_census"][
                "shdef_semantic_audit"]["ai_design_name_tails"][
                    "fresh_design_rng_minimum"]
        self.assertEqual(name_rng["creation_order_slots"], creation_order)
        self.assertEqual(name_rng["accepted_values_and_local_order_observed"],
                         True)
        self.assertEqual(name_rng["main_stream_positions_recovered"], True)
        self.assertEqual(name_rng["main_stream_draw_positions_by_run"], {
            "control": [1, 2, 3, 4],
            "action": [1, 2, 3, 4],
        })
        self.assertEqual(name_rng["effective_tick_seed_residue_recovered"],
                         True)
        self.assertEqual(name_rng["duplicate_retry_counts_recovered"], True)
        self.assertEqual(name_rng["accepted_random13_values_by_creation_order"], {
            label: [string_id - 0x432 for string_id in
                    [output_ids_by_run[label][slot]
                     for slot in creation_order]]
            for label in ("control", "action")
        })
        lower_bound = oracle["rng_initialization_trace"][
            "known_rng_call_lower_bound"]
        ai_name_basis = [entry for entry in lower_bound["basis"]
                         if entry["phase"] ==
                         "Turin AI starbase design names"]
        self.assertEqual(len(ai_name_basis), 1)
        self.assertEqual(ai_name_basis[0]["calls"], 4)
        self.assertEqual(sum(entry["calls"] for entry in lower_bound["basis"]),
                         lower_bound["count"])
        self.assertEqual(lower_bound["count"], 37)

    def test_recover_startup_tick_and_first_turin_scout_draw(self):
        oracle = json.loads((ROOT / "analysis/compat/"
            "satisfy-orders-full-turn-e3-original-oracle.json"
        ).read_text(encoding="utf-8"))
        name_rng = oracle["current_source_control_action_pair"][
            "retained_original_output_record_census"][
                "shdef_semantic_audit"]["ai_design_name_tails"][
                    "fresh_design_rng_minimum"]
        accepted = name_rng["accepted_random13_values_by_creation_order"]
        self.assertEqual(accepted, {
            "control": [4, 9, 7, 0],
            "action": [9, 5, 6, 10],
        })

        lib = self.inspector.lib
        lib.stars_randomize2.argtypes = [POINTER(Seeds), c_uint32]
        lib.stars_randomize2.restype = None
        lib.stars_random.argtypes = [POINTER(Seeds), c_int16]
        lib.stars_random.restype = c_int16

        def observed_name_sequence_matches(tick, values):
            state = Seeds()
            lib.stars_randomize2(byref(state), tick)
            initial_pair = (state.first, state.second)

            def draw_name():
                return lib.stars_random(byref(state), 13)

            # Source call order is slots 18, 20, 17, 19. The second and
            # fourth picks may retry only the previous same-hull name.
            first = draw_name()
            if first != values[0]:
                return None
            second = draw_name()
            retries_20 = 0
            while second == values[0] and retries_20 < 19:
                retries_20 += 1
                second = draw_name()
            if second != values[1]:
                return None
            third = draw_name()
            if third != values[2]:
                return None
            fourth = draw_name()
            retries_19 = 0
            while fourth == values[2] and retries_19 < 19:
                retries_19 += 1
                fourth = draw_name()
            if fourth != values[3]:
                return None
            return ([retries_20, retries_19], initial_pair,
                    (state.first, state.second))

        recovered = {}
        for label, values in accepted.items():
            with self.subTest(run=label):
                candidates = {}
                for tick_low14 in range(1 << 14):
                    match = observed_name_sequence_matches(tick_low14, values)
                    if match is not None:
                        candidates[tick_low14] = match
                self.assertEqual(len(candidates), 1, candidates)
                tick_low14, (retry_counts, seed_words, post_name_words) = next(
                    iter(candidates.items()))
                self.assertEqual(retry_counts, [0, 0])
                post_names = Seeds()
                post_names.first, post_names.second = post_name_words
                scout_random100 = lib.stars_random(byref(post_names), 100)
                post_scout_random100 = lib.stars_random(byref(post_names), 100)
                ai_writer_random2000 = [
                    lib.stars_random(byref(post_names), 2000)
                    for _ in range(2)
                ]
                player_shuffle_draws = [
                    lib.stars_random(byref(post_names), 2),
                    lib.stars_random(byref(post_names), 1),
                ]
                recovered[label] = {
                    "tick_low14": tick_low14,
                    "seed_words": list(seed_words),
                    "first_post_name_random100": scout_random100,
                    "next_random100": post_scout_random100,
                    "ai_writer_random2000": ai_writer_random2000,
                    "player_shuffle_draws": player_shuffle_draws,
                }

        self.assertEqual(recovered, {
            "control": {"tick_low14": 13747, "seed_words": [19, 279],
                        "first_post_name_random100": 99,
                        "next_random100": 7,
                        "ai_writer_random2000": [1240, 1434],
                        "player_shuffle_draws": [0, 0]},
            "action": {"tick_low14": 9953, "seed_words": [443, 67],
                       "first_post_name_random100": 4,
                       "next_random100": 75,
                       "ai_writer_random2000": [1922, 267],
                       "player_shuffle_draws": [1, 0]},
        })

        # Pin the zero-prior-draw source path used by the exhaustive seed
        # search. Launch setup seeds before FCreateStuff; its callgraph has no
        # Random edge. InitInstance's only reachable Random calls are inside
        # the serial/environment parser, which saves and restores the stream.
        raw = ROOT / "analysis/decompiler/export/raw"
        winmain = (raw / "1018_0000_WinMain.c").read_text()
        create_stuff = (raw / "1000_0014_FCreateStuff.c").read_text()
        init_instance = (raw / "1000_0d70_InitInstance.c").read_text()
        serial_parse = (raw / "1020_2aec_FSerialAndEnvFromSz.c").read_text()
        draw_stars = (raw / "1038_5fe2_DrawABunchOfStars.c").read_text()
        randomize2 = (raw / "1040_165a_Randomize2.c").read_text()

        def direct_random_positions(source):
            return [match.start() for match in re.finditer(
                r"(?<![A-Za-z_])Random\(", source)]

        self.assertLess(winmain.index("Randomize2(dw)"),
                        winmain.index("FCreateStuff()"))
        self.assertNotIn("Random(", create_stuff)
        self.assertIn("ReadIniSettings();", init_instance)
        self.assertLess(serial_parse.index("PushRandom("),
                        serial_parse.index("Randomize("))
        self.assertGreater(serial_parse.index("PopRandom()"),
                           direct_random_positions(serial_parse)[-1])
        self.assertIn("(uint)dw & 0x7f", randomize2)
        self.assertIn("__aFulshr(dw,'\\a')", randomize2)
        draw_positions = direct_random_positions(draw_stars)
        self.assertTrue(draw_positions)
        self.assertLess(draw_stars.index("PushRandom("), draw_positions[0])
        self.assertGreater(draw_stars.index("PopRandom()"),
                           draw_positions[-1])

        callgraph = json.loads((ROOT / "analysis/decompiler/callgraph.json")
                               .read_text(encoding="utf-8"))
        outgoing = {}
        for edge in callgraph:
            outgoing.setdefault(edge["caller"], []).append(edge)

        def reachable_random_callers(start):
            pending = [start]
            visited = set()
            random_callers = set()
            while pending:
                node = pending.pop()
                if node in visited:
                    continue
                visited.add(node)
                for edge in outgoing.get(node, []):
                    if any(target.lower() == "random"
                           for target in edge.get("target_names", [])):
                        random_callers.add(edge["caller_name"])
                    if edge.get("target"):
                        pending.append(edge["target"])
            return random_callers

        self.assertEqual(reachable_random_callers("1000:0014"), set())
        self.assertEqual(reachable_random_callers("1000:0d70"),
                         {"FSerialAndEnvFromSz"})
        self.assertEqual(reachable_random_callers("1090:6896"), set())
        self.assertEqual(reachable_random_callers("1090:9c66"), set())
        self.assertEqual(reachable_random_callers("1090:4b9a"), set())

        ai_turn = (raw / "1088_0000_DoAiTurn.c").read_text()
        ensure_ais = (raw / "1020_56bc_EnsureAis.c").read_text()
        init_planets = (raw / "1090_a082_InitRandomPlanetList.c").read_text()
        compute_power = (raw / "1038_0b32_LComputePower.c").read_text()
        check_shdef_status = (raw / "1090_9b10_CheckAiShdefStatus.c").read_text()
        ship_speed = (raw / "10f0_339c_SpdOfShip.c").read_text()
        turin_turn = (raw / "1088_3670_DoTurinDroneAiTurn.c").read_text()
        turin_designs = (raw / "1088_58ba_EnsureTurinDroneShdefs.c").read_text()
        load_game = (raw / "1070_0810_FLoadGame.c").read_text()
        change_selection = (raw / "1048_3e7a_ChangeMainObjSel.c").read_text()
        finish_production = (raw / "10d0_1090_FinishProduction.c").read_text()
        get_resources = (raw / "1090_56d0_GetResourcesAvailable.c").read_text()
        estimate_mined = (raw / "1028_5362_EstMineralsMined.c").read_text()
        fill_production_queue = (raw / "10a8_2ce2_FillProductionQueue.c").read_text()
        write_log = (raw / "1048_cdf6_FWriteLogFile.c").read_text()
        write_history = (raw / "1048_d29e_FWriteHistFile.c").read_text()
        dirty_game_source = (raw / "1048_a228_DirtyGame.c").read_text()
        create_file = (raw / "1070_8e16_FCreateFile.c").read_text()
        write_bof = (raw / "1070_8ea4_WriteBOF.c").read_text()
        destroy_game = (raw / "1070_44b0_DestroyCurGame.c").read_text()
        handle_basic = (raw / "1090_95a4_HandleBasicAiTasks.c").read_text()
        queue_starbases = (raw / "1090_8524_QueueAiStarbases.c").read_text()
        upgrade_starbase = (raw / "1090_882a_FUpgradeAiStarbase.c").read_text()
        latest_starbase = (raw / "1090_84be_IshdefAiSBLatest.c").read_text()
        ai_fling = (raw / "1090_7dd6_FAIFling.c").read_text()
        host_timer = (raw / "1020_7716_HostTimerProc.c").read_text()
        turns_outstanding = (raw / "1020_6a26_CFindTurnsOutstanding.c").read_text()
        generate_turn = (raw / "10b0_0000_FGenerateTurn.c").read_text()
        self.assertIn("gd.overlap_0.grBits_view.grBits._0_2_ & 0xfffd;",
                      turns_outstanding)
        self.assertIn("if ((uStack_8 != 0) && (((uint)gd.overlap_0.grBits_view.grBits._0_2_ >> 1 & 1) != 0))",
                      estimate_mined)
        clear_random_yield_gate = ensure_ais.index(
            "gd.overlap_0.grBits_view.grBits._0_2_ & 0xfff5 | (uStack_6 & 1) << 3;")
        self.assertLess(ensure_ais.index("sStack_a = FOpenFile("),
                        clear_random_yield_gate)
        self.assertLess(clear_random_yield_gate, ensure_ais.index("DoAiTurn("))
        self.assertLess(host_timer.index("EnsureAis();"),
                        host_timer.index("FGenerateTurn();"))
        self.assertIn("gd.overlap_0.grBits_view.grBits._0_2_ & 0xfdfd | 0x202",
                      generate_turn)
        self.assertIn("EstMineralsMined(lppl,rgRes,-1,0);", get_resources)
        self.assertEqual(turin_turn.count("GetResourcesAvailable("), 4)
        scout_source_position = turin_turn.index("IdTargetScout(")
        self.assertTrue(all(match.start() < scout_source_position
                            for match in re.finditer("GetResourcesAvailable\\(",
                                                     turin_turn)))
        # DoAiTurn keeps fAi set through its two successful AI-output writes;
        # the only RNG-bearing descendants of those writers are WriteBOF.
        self.assertLess(ai_turn.index("fAi = 1;"), ai_turn.index("FWriteLogFile("))
        self.assertLess(ai_turn.index("FWriteLogFile("),
                        ai_turn.index("FWriteHistFile("))
        self.assertLess(ai_turn.index("FWriteHistFile("), ai_turn.rindex("fAi = 0;"))
        self.assertIn("FCreateFile(1,iPlayer", write_log)
        self.assertIn("FCreateFile(4,iPlayer", write_history)
        self.assertIn("WriteBOF(iPlayer,dt,0)", create_file)
        self.assertIn("game.fDirty = fDirty", dirty_game_source)
        for run_name in ("vmctl-sye3k0", "vmctl-sye3k1"):
            x2_records = self._records(run_name, "BACKUP/CLEAN.X2")
            h2_records = self._records(run_name, "CLEAN.H2")
            self.assertTrue(any(record["kind"] == 8 and record["turn"] == 0
                                for record in x2_records))
            self.assertTrue(any(record["kind"] == 8 and record["turn"] == 0
                                for record in h2_records))
        self.assertEqual(reachable_random_callers("10a8:2ce2"),
                         {"EstMineralsMined"})
        self.assertEqual(reachable_random_callers("1048:cdf6"), {"WriteBOF"})
        self.assertEqual(reachable_random_callers("1048:d29e"), {"WriteBOF"})
        self.assertEqual(reachable_random_callers("1070:0810"),
                         {"EstMineralsMined", "WriteBOF"})
        self.assertEqual(reachable_random_callers("1070:44b0"), {"WriteBOF"})
        # These broad static closures include conditional paths. On the
        # retained host path, mineral rounding is disabled through EnsureAis,
        # and DestroyCurGame can reach PromptSaveGame only for a dirty game;
        # the successful AI log write clears that dirty flag before cleanup.
        self.assertIn("(idPlayer != -1) && (game.fDirty != 0)", destroy_game)
        self.assertIn("DirtyGame(0);", write_log)
        self.assertIn("sVar1 = Random(2000);", write_bof)
        self.assertFalse(oracle["rng_initialization_trace"][
            "recorded_minus_g_tutorial_gate"]["value"])
        self.assertLess(generate_turn.index("Random(game.cPlayer - (int)p_Stack_38)"),
                        generate_turn.index("Random(8)"))
        for run_name in ("vmctl-sye3k0", "vmctl-sye3k1"):
            run = self._run(run_name)
            self.assertIn("STARS.EXE -g", run["run_command"])
        self.assertIn("InitRandomPlanetList();", ai_turn)
        self.assertIn("fAi = 1;", ai_turn)
        self.assertLess(ai_turn.index("ComputeShdefPowers();"),
                        ai_turn.index("MarkPlanetsUnderAttack();"))
        self.assertLess(ai_turn.index("MarkPlanetsUnderAttack();"),
                        ai_turn.index("IncreaseAIMinefieldSizes();"))
        self.assertLess(ai_turn.index("IncreaseAIMinefieldSizes();"),
                        ai_turn.index("InitRandomPlanetList();"))
        self.assertIn("iStack_10 < vclpplAi + -1", init_planets)
        self.assertIn("SpdOfShip((_fleet *)0x0", compute_power)
        speed_random = ship_speed.index("Random(0xf)")
        speed_guard = ship_speed.rindex(
            "if (lpfl != (_fleet *)0x0)", 0, speed_random)
        self.assertLess(speed_guard, speed_random)
        self.assertLess(turin_turn.index("IroEnsureAi("),
                        turin_turn.index("Random("))

        # The only random-bearing EnsureTurinDroneShdefs design creations are
        # guarded by technology/slot-presence conditions. Retained technology
        # bytes and the present slots make every such branch false, so the
        # stream proceeds from the four IroEnsureAi names to Turin's scout.
        retained_m2 = self._records("vmctl-sye3k0", "BACKUP/CLEAN.M2")
        owner1_player = self._owner1_player(retained_m2, 0)
        techs = list(owner1_player[26:32])
        self.assertEqual(techs, [0, 0, 1, 0, 5, 0])
        self.assertEqual(re.findall(r"FCreateAiShdef\((\d+),",
                                    turin_designs),
                         ["8", "9", "10", "1", "0", "2", "4"])
        self.assertIn("FCreateAiShdef(0xc,0xb", turin_designs)
        self.assertIn("FCreateAiShdef(0xd,0x12", turin_designs)
        self.assertIn("FCreateAiShdef(0xe,0x12", turin_designs)
        self.assertIn("FCreateAiShdef(0xf,0xc", turin_designs)
        self.assertFalse(techs[2] > 4 and techs[3] > 7)
        self.assertFalse(techs[2] > 6 and techs[3] > 10)
        self.assertFalse(techs[1] > 4 and techs[4] > 4 and
                         techs[3] > 3 and techs[2] > 4)
        self.assertFalse(techs[3] > 3 and techs[5] > 3)
        self.assertFalse(techs[1] > 7 and techs[4] > 6 and
                         techs[3] > 5)
        self.assertFalse(techs[1] > 10 and techs[4] > 11 and
                         techs[3] > 14 and techs[2] > 8)
        self.assertFalse(techs[1] > 4 and techs[4] > 5 and
                         techs[3] > 12 and techs[2] > 6)
        self.assertFalse(techs[2] > 4)
        self.assertIn(r"'\x04' < *(char *)((int)rgplr[0].rgTech + 2",
                      turin_turn)
        self.assertIn("rgshdef[0xc].overlap_123 >> 9 & 1) == 0",
                      turin_turn)
        self.assertIn("iStack_3e < 0x10", load_game)
        self.assertIn("& 0xfdff", load_game)
        self.assertIn("| 0x200", load_game)
        present_shdefs = {
            ((int.from_bytes(record["body"][:2], "little") >> 10) & 31):
            record["body"]
            for record in retained_m2
            if record["kind"] == 26 and record["turn"] == 0
        }
        self.assertEqual(sorted(present_shdefs), [0, 1, 2, 16])
        self.assertNotIn(12, present_shdefs)
        # FLoadGame marks every empty player SHDEF slot with bit 9 before
        # reading M2 records. The retained slot-12 record is absent, and its
        # EnsureTurin creation gate also fails on technology bytes 3 and 5.
        shdef_init = load_game[
            load_game.index("pvVar31 = LpAlloc(0x930,7);"):
            load_game.index("sVar16 = idPlayer",
                            load_game.index("pvVar31 = LpAlloc(0x930,7);"))]
        self.assertIn("iStack_3e < 0x10", shdef_init)
        self.assertIn("& 0xfdff", shdef_init)
        self.assertIn("| 0x200", shdef_init)
        self.assertFalse(techs[3] > 3 and techs[5] > 3)
        self.assertIn("rgshdef[0xc].overlap_123 >> 9 & 1) == 0",
                      turin_turn)

        # The first post-scout main-stream draw is the one-planet
        # FUpgradeAiStarbase Random(100). Slot 5 is absent and retains
        # FLoadGame's bit-9 marker, so IshdefAiSBLatest returns 0. Planet 24
        # has a starbase of type 0; the alternate gate sees present slot 2 and
        # consumes its Random(100) < 6 roll. Draw 6 is 7/75, so both retained
        # runs skip the queue change.
        self.assertLess(handle_basic.index("QueueAiStarbases("),
                        handle_basic.index("FUpgradeAiStarbase("))
        self.assertLess(turin_turn.index("HandleBasicAiTasks("),
                        turin_turn.index("FillProductionQueue("))
        self.assertLess(handle_basic.index("FUpgradeAiStarbase("),
                        handle_basic.index("FAIFling("))
        self.assertNotIn("Random(", queue_starbases)
        self.assertIn("sVar3 = 5;", latest_starbase)
        self.assertIn("sVar3 = 0;", latest_starbase)
        self.assertIn("sVar3 = Random(100);", upgrade_starbase)
        self.assertIn("sVar3 = Random(100), sVar3 < 6", upgrade_starbase)
        self.assertIn("1 < ((uint)((_player_at_6_det *)&rgplr[0].overlap_6.det_view)",
                      ai_fling)
        self.assertNotIn("Random(", (raw / "1090_90d6_FQueueAiScanner.c").read_text())
        self.assertNotIn("Random(", (raw / "1090_939a_FQueueAiDefenses.c").read_text())
        self.assertNotIn("Random(", (raw / "1090_8d28_FQueueAiTerraforming.c").read_text())
        self.assertNotIn("Random(", (raw / "1090_1792_AddMinesToBlockedQueues.c").read_text())
        self.assertNotIn("Random(", (raw / "1090_69e6_FixPlanetsUnderAttack.c").read_text())
        for run_name in ("vmctl-sye3k0", "vmctl-sye3k1"):
            records = self._records(run_name, "BACKUP/CLEAN.M2")
            planet = next(record["body"] for record in records
                          if record["kind"] == 13 and
                          (int.from_bytes(record["body"][:2], "little") & 0x7ff) == 24)
            self.assertEqual((int.from_bytes(planet[2:4], "little") >> 9) & 1, 1)
            self.assertEqual(planet[-4:], b"\0\0\0\0")
        self.assertNotIn(5, present_shdefs)
        self.assertEqual((int.from_bytes(present_shdefs[1][:2], "little") >> 9) & 1,
                         0)
        self.assertEqual((int.from_bytes(present_shdefs[2][:2], "little") >> 9) & 1,
                         0)
        starbase_type = int.from_bytes(planet[-4:], "little") & 0xf
        self.assertEqual(starbase_type, 0)
        self.assertEqual((int.from_bytes(owner1_player[6:8], "little") >> 10) & 7,
                         1)
        self.assertEqual((int.from_bytes(owner1_player[6:8], "little") >> 13) & 7,
                         1)
        self.assertIn("(iStack_8 + -1) * 2", upgrade_starbase)
        # Four lexically earlier Random sites sit in Turin's production
        # queue logic. Random(4) is gated by tech byte 2 > 4; Random(3) is
        # gated by the slot-12 marker; Random(8) and Random(iStack_9c*2+1)
        # are nested inside the Random(3) branch.
        direct_randoms = list(re.finditer(r"Random\(([^)]*)\)", turin_turn))
        self.assertEqual([match.group(1) for match in direct_randoms], [
            "4", "3", "8", "iStack_9c * 2 + 1", "100",
        ])
        scout_position = turin_turn.index("IdTargetScout(")
        self.assertEqual([match.group(1) for match in direct_randoms
                          if match.start() < scout_position], [
                              "4", "3", "8", "iStack_9c * 2 + 1",
                          ])
        self.assertFalse(techs[2] > 4)
        random3 = turin_turn.index("sVar9 = Random(3)")
        slot12_guard = turin_turn.rindex(
            "rgshdef[0xc].overlap_123 >> 9 & 1) == 0", 0, random3)
        self.assertIn("&&", turin_turn[slot12_guard:random3])
        self.assertLess(random3, turin_turn.index("sVar9 = Random(8)"))
        self.assertLess(turin_turn.index("sVar9 = Random(8)"),
                        turin_turn.index("Random(iStack_9c * 2 + 1)"))
        for slot, count in ((0, 1), (1, 1), (2, 2)):
            body = present_shdefs[slot]
            self.assertEqual((int.from_bytes(body[:2], "little") >> 9) & 1,
                             0)
            self.assertEqual(int.from_bytes(body[13:17], "little"), count)
        self.assertIn("*piLatest = -1;", check_shdef_status)
        self.assertIn(">>\n         9 & 1) == 0", check_shdef_status)
        for latest in ("uStack_a0", "uStack_16", "uStack_2e", "uStack_64"):
            self.assertIn(f"{latest} != 0xffff", turin_turn)
        self.assertEqual(turin_turn.count("GetResourcesAvailable("), 4)
        for call, start, end, output in (
                ("4,5", 4, 5, "uStack_a0"),
                ("6,7", 6, 7, "uStack_16"),
                ("8,9", 8, 9, "auStack_8a"),
                ("10,0xb", 10, 11, "uStack_2e"),
                ("0xc,0xc", 12, 12, "sStack_4c"),
                ("0xd,0xe", 13, 14, "uStack_a2"),
                ("0xf,0xf", 15, 15, "uStack_64")):
            self.assertIn(f"CheckAiShdefStatus({call},", turin_turn)
            if output not in ("auStack_8a", "sStack_4c", "uStack_a2"):
                self.assertEqual(set(range(start, end + 1)) &
                                 set(present_shdefs), set())

        # FLoadGame fills rglpfl in the order fleets are read from M2. The
        # retained record order starts with fleet 0, and both Turin fleet
        # passes enumerate that array before HandleBasicAiTasks runs.
        input_fleets = [decode_owner_fleet(record["body"])
                        for record in retained_m2 if record["kind"] == 16]
        self.assertEqual([fleet["id"] for fleet in input_fleets],
                         [0, 1, 2, 3])
        fleet0 = input_fleets[0]
        self.assertEqual(fleet0["ship_counts"][2:4], [0, 0])
        self.assertEqual(fleet0["ship_counts"][8:10], [0, 0])
        self.assertEqual(fleet0["ship_counts"][1], 0)
        self.assertIn("pp_Var22 = (_fleet **)rglpfl + iStack_1a",
                      load_game)
        self.assertIn("sVar16 = FReadFleet(uStack_22)", load_game)
        self.assertLess(turin_turn.index("IdTargetScout("),
                        turin_turn.index("HandleBasicAiTasks("))
        self.assertLess(turin_turn.index("IdTargetScout("),
                        turin_turn.index("FMoveAiFleet("))
        self.assertLess(turin_turn.index("IdTargetScout("),
                        turin_turn.rindex("Random(100)"))
        finish_production_calls = [
            match.start() for match in re.finditer(r"FinishProduction\(", turin_turn)
        ]
        scout_position = turin_turn.index("IdTargetScout(")
        self.assertEqual(len(finish_production_calls), 2)
        self.assertTrue(all(position < scout_position
                            for position in finish_production_calls))
        # FinishProduction can reach Random through FillPlanetProdLB and
        # EstMineralsMined, but that UI refresh is guarded by fAi == 0. The
        # Turin turn sets fAi to 1 before the pre-scout production calls.
        self.assertEqual(reachable_random_callers("10d0:1090"),
                         {"EstMineralsMined"})
        self.assertIn("if (fAi == 0)", finish_production)
        self.assertLess(finish_production.index("if (fAi == 0)"),
                        finish_production.index("FillPlanetProdLB("))
        self.assertLess(ai_turn.index("fAi = 1;"),
                        ai_turn.index("DoTurinDroneAiTurn("))
        self.assertIn("if (fAi == 0)", change_selection)
        self.assertLess(change_selection.index("if (fAi == 0)"),
                        change_selection.index("FillPlanetProdLB("))

        for run_name in ("vmctl-sye3k0", "vmctl-sye3k1"):
            with self.subTest(owner1_planets=run_name):
                retained_hst = self._records(run_name, "BACKUP/CLEAN.HST")
                owner1_planets = [
                    record for record in retained_hst
                    if record["kind"] == 13 and record["turn"] == 0 and
                    ((int.from_bytes(record["body"][:2], "little") >> 11)
                     & 31) == 1
                ]
                self.assertEqual(len(owner1_planets), 1)

        self.assertEqual(name_rng["effective_startup_tick_low14_by_run"], {
            label: entry["tick_low14"]
            for label, entry in recovered.items()
        })
        self.assertEqual(name_rng["effective_seed_words_by_run"], {
            label: entry["seed_words"]
            for label, entry in recovered.items()
        })
        self.assertEqual(
            name_rng["first_post_name_scout_random100_by_run"], {
                label: entry["first_post_name_random100"]
                for label, entry in recovered.items()
            })
        lower_bound = oracle["rng_initialization_trace"]["known_rng_call_lower_bound"]
        upgrade_draw = oracle["rng_initialization_trace"][
            "retained_turin_guard_bounds"]["post_scout_ai_starbase_upgrade_random"]
        self.assertEqual(upgrade_draw["position"], 6)
        self.assertEqual(upgrade_draw["draw_value_by_run"], {
            label: entry["next_random100"]
            for label, entry in recovered.items()
        })
        output_draws = oracle["rng_initialization_trace"][
            "retained_turin_guard_bounds"]["post_upgrade_ai_output_random_draws"]
        self.assertEqual(output_draws["draw_positions_by_run"], {
            "control": [7, 8], "action": [7, 8],
        })
        self.assertEqual(output_draws["draw_value_by_run"], {
            label: entry["ai_writer_random2000"]
            for label, entry in recovered.items()
        })
        writer_basis = next(item for item in lower_bound["basis"]
                            if item["phase"] == "owner-1 log and history outputs")
        self.assertEqual(writer_basis["draw_positions_by_run"], {
            "control": [7, 8], "action": [7, 8],
        })
        self.assertEqual(writer_basis["draw_value_by_run"], {
            label: entry["ai_writer_random2000"]
            for label, entry in recovered.items()
        })
        shuffle_draws = oracle["rng_initialization_trace"][
            "retained_turin_guard_bounds"]["post_ai_writer_player_shuffle"]
        self.assertEqual(shuffle_draws["draw_positions_by_run"], {
            "control": [9, 10], "action": [9, 10],
        })
        self.assertEqual(shuffle_draws["draw_value_by_run"], {
            label: entry["player_shuffle_draws"]
            for label, entry in recovered.items()
        })
        shuffle_basis = next(item for item in lower_bound["basis"]
                             if item["phase"] == "FGenerateTurn player shuffle")
        self.assertEqual(shuffle_basis["draw_positions_by_run"], {
            "control": [9, 10], "action": [9, 10],
        })
        self.assertEqual(shuffle_basis["draw_value_by_run"], {
            label: entry["player_shuffle_draws"]
            for label, entry in recovered.items()
        })
        self.assertEqual(name_rng["first_post_name_random100_position_by_run"],
                         {"control": 5, "action": 5})
        finish_audit = name_rng["pre_scout_finish_production_draw_audit"]
        self.assertEqual(finish_audit["finish_production_call_count_before_scout"], 2)
        self.assertTrue(finish_audit["ai_flag_set_before_turin_dispatch"])
        self.assertIn("fAi = 1", finish_audit["fill_planet_prod_lb_guard"])
        self.assertEqual(finish_audit["conclusion"],
                         "Neither pre-scout FinishProduction call advances the retained main RNG stream.")
        queue_audit = name_rng["pre_scout_turin_queue_random_guards"]
        self.assertEqual(queue_audit["direct_random_sites_before_scout"], [
            "Random(4)", "Random(3)", "Random(8)",
            "Random(iStack_9c * 2 + 1)",
        ])
        self.assertIn("byte 2 is 1", queue_audit["random4_guard"])
        self.assertIn("has no slot 12", queue_audit["random3_guard"])
        self.assertIn("Both are inside", queue_audit["nested_random8_and_variable_guard"])
        self.assertTrue(queue_audit["conclusion"].startswith("No direct Turin"))
        self.assertEqual(lower_bound["count"], 37)
        upgrade_basis = next(item for item in lower_bound["basis"]
                             if item["phase"] == "owner-1 Turin starbase upgrade roll")
        self.assertEqual(upgrade_basis["draw_position"], 6)
        self.assertEqual(upgrade_basis["draw_value_by_run"],
                         {"control": 7, "action": 75})
        self.assertTrue(lower_bound["startup_seed_recovered"])
        self.assertIn("full 32-bit GetTickCount value is not",
                      lower_bound["startup_seed_recovered_scope"])
        self.assertEqual(name_rng["main_stream_positions_recovered"], True)
        self.assertEqual(name_rng["main_stream_draw_positions_by_run"], {
            "control": [1, 2, 3, 4],
            "action": [1, 2, 3, 4],
        })
        self.assertEqual(
            name_rng["complete_turn_global_draw_positions_recovered"], True)
        self.assertEqual(name_rng["complete_turn_random_call_sequence_recovered"],
                         True)


if __name__ == "__main__":
    unittest.main()
