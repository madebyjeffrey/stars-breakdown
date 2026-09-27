"""Audit the single isolated Rototill selector-3 original-turn output."""

import ctypes
import hashlib
from pathlib import Path
import struct
import unittest

from tests.test_satisfy_orders_turin_binding import (
    STARS_EXE, STARS_EXE_SHA256, owner1_fleet_order_pairs,
    owner_planet_record, retained_ai_planet_status, xy_points)
from tools.ne_layout import NE
from tools.race_file_compare import Inspector
from tools.stars_game_stream import decode_stream

ROOT = Path(__file__).resolve().parents[1]
RUN = ROOT / "runs/rototill-original-turn-r1"


class Point(ctypes.Structure):
    _fields_ = [("x", ctypes.c_int16), ("y", ctypes.c_int16)]


class RototillOriginalTurnWitnessTests(unittest.TestCase):
    def test_fleet_zero_scout_dispatch_and_nearest_unknown_input(self):
        inspector = Inspector(ctypes.CDLL(str(
            (ROOT / "build/libstars_core.dylib").resolve())))
        records = decode_stream(RUN / "input/CLEAN.M2", inspector)
        player = [record["body"] for record in records
                  if record["kind"] == 6 and record["turn"] == 0 and
                  record["body"][0] == 1]
        self.assertEqual(len(player), 1)
        self.assertEqual(struct.unpack_from("<H", player[0], 6)[0], 0x6797)
        fleets = owner1_fleet_order_pairs(records, 0)
        self.assertEqual([pair["fleet"]["id"] for pair in fleets],
                         [0, 1, 2, 3])
        scout = fleets[0]["fleet"]
        self.assertEqual(scout["owner"], 1)
        self.assertEqual(scout["planet"], 24)
        self.assertEqual(scout["order_count"], 1)
        self.assertEqual(scout["ship_counts"], [1] + [0] * 15)
        self.assertEqual(scout["cargo"][4], 300)

        designs = [record["body"] for record in records
                   if record["kind"] == 26 and record["turn"] == 0 and
                   ((int.from_bytes(record["body"][:2], "little") >> 10)
                    & 31) == 0]
        self.assertEqual(len(designs), 1)
        self.assertEqual(designs[0][2], 4)
        components = [struct.unpack_from("<HH", designs[0],
                                         17 + index * 4)
                      for index in range(designs[0][6])]
        self.assertEqual(components, [(1, 257), (2, 260), (4096, 261)])
        self.assertEqual(hashlib.sha256(STARS_EXE.read_bytes()).hexdigest(),
                         STARS_EXE_SHA256)
        segment = bytes(NE(STARS_EXE).raw[1])
        hull4 = segment[0x29f0 + 4 * 0x8f:0x29f0 + 5 * 0x8f]
        hull_slots = [struct.unpack_from("<HH", hull4, 58 + 4 * index)
                      for index in range(hull4[122])]
        self.assertEqual(len(hull_slots), 3)
        self.assertTrue(any((group & 0x30) and packed >> 8
                            for group, packed in hull_slots))

        points = xy_points(RUN / "input/CLEAN.XY", inspector)
        self.assertEqual(len(points), 32)
        self.assertEqual(points[24], (1290, 1258))
        player_planets = [record["body"] for record in records
                          if record["kind"] == 13 and record["turn"] == 0]
        self.assertEqual(len(player_planets), 1)
        self.assertEqual(int.from_bytes(player_planets[0][:2], "little")
                         & 2047, 24)
        hst = decode_stream(RUN / "input/CLEAN.HST", inspector)
        planets = [record["body"] for record in hst
                   if record["kind"] == 13 and record["turn"] == 0]
        self.assertEqual(len(planets), 32)
        status = retained_ai_planet_status(0x6797, len(points), dict(
            player_id=1, known_planets=[(24, 1)], fleet_orders=fleets,
            active_fleet_id=0))
        self.assertEqual(status[24], 1)
        candidates = [planet_id for planet_id, value in enumerate(status)
                      if value == 0x10]
        self.assertEqual(len(candidates), 31)
        ranked = sorted(candidates, key=lambda planet_id: (
            sum((points[planet_id][axis] - points[24][axis]) ** 2
                for axis in (0, 1)), planet_id))
        self.assertEqual(ranked[0], 26)
        self.assertEqual(sum((points[26][axis] - points[24][axis]) ** 2
                             for axis in (0, 1)), 545)
        self.assertEqual(points[26], (1313, 1262))

    def test_current_ai_status_helpers_select_original_scout_target(self):
        lib = ctypes.CDLL(str((ROOT / "build/libstars_core.dylib").resolve()))
        inspector = Inspector(lib)
        records = decode_stream(RUN / "input/CLEAN.M2", inspector)
        pairs = owner1_fleet_order_pairs(records, 0)
        points = xy_points(RUN / "input/CLEAN.XY", inspector)
        self.assertEqual(len(points), 32)
        mode = next(int.from_bytes(record["body"][6:8], "little")
                    for record in records if record["kind"] == 6 and
                    record["body"][0] == 1)
        owners = [-2] * len(points)
        for record in records:
            if record["kind"] == 13 and record["turn"] == 0:
                word = int.from_bytes(record["body"][:2], "little")
                owners[word & 2047] = word >> 11
        self.assertEqual([(index, owner) for index, owner in
                          enumerate(owners) if owner != -2], [(24, 1)])
        c_points = (Point * len(points))(*(Point(*point) for point in points))
        c_owners = (ctypes.c_int16 * len(owners))(*owners)
        c_status = (ctypes.c_uint8 * len(points))()
        prepare = lib.stars_id_nearest_colonizable_planet
        prepare.argtypes = [Point, ctypes.c_int16, ctypes.c_int16,
                            ctypes.c_int16, ctypes.c_int16, ctypes.c_int16,
                            ctypes.POINTER(ctypes.c_uint8),
                            ctypes.POINTER(ctypes.c_int16),
                            ctypes.POINTER(Point), ctypes.c_uint16,
                            ctypes.c_void_p, ctypes.c_uint16, ctypes.c_void_p,
                            ctypes.c_void_p, ctypes.c_void_p]
        prepare.restype = ctypes.c_int16
        source = c_points[pairs[0]["fleet"]["planet"]]
        self.assertEqual(prepare(source, 24, (mode >> 13) & 7, 0, 0, 1,
                                 c_status, c_owners, c_points, len(points),
                                 None, 0, None, None, None), -1)
        self.assertEqual(list(c_status), [0x10] * 24 + [1] + [0x10] * 7)
        nearest = lib.stars_id_nearest_unknown_planet_strided
        nearest.argtypes = [Point, ctypes.POINTER(ctypes.c_uint8),
                            ctypes.c_uint16, ctypes.c_uint16,
                            ctypes.POINTER(Point), ctypes.c_uint16,
                            ctypes.c_int16, ctypes.c_int16,
                            ctypes.c_void_p, ctypes.c_void_p,
                            ctypes.c_void_p, ctypes.c_void_p, ctypes.c_void_p]
        nearest.restype = ctypes.c_int16
        target = nearest(source, c_status, 1, 0, c_points, len(points),
                         1, 1, None, None, None, None, None)
        self.assertEqual(target, 26)
        output = decode_stream(RUN / "output/CLEAN.M2", inspector)
        self.assertEqual(target,
                         owner1_fleet_order_pairs(output, 1)[0]["order"]
                         ["target_id"])

    def test_backup_and_turn_one_outputs(self):
        inspector = Inspector(ctypes.CDLL(str(
            (ROOT / "build/libstars_core.dylib").resolve())))
        for name in ("CLEAN.HST", "CLEAN.M1", "CLEAN.M2"):
            with self.subTest(backup=name):
                self.assertEqual((RUN / "backup" / name).read_bytes(),
                                 (RUN / "input" / name).read_bytes())
        self.assertEqual((RUN / "output/CLEAN.XY").read_bytes(),
                         (RUN / "input/CLEAN.XY").read_bytes())
        input_m2 = decode_stream(RUN / "input/CLEAN.M2", inspector)
        input_hst = decode_stream(RUN / "input/CLEAN.HST", inspector)
        output_m2 = decode_stream(RUN / "output/CLEAN.M2", inspector)
        output_hst = decode_stream(RUN / "output/CLEAN.HST", inspector)
        output_m1 = decode_stream(RUN / "output/CLEAN.M1", inspector)
        input_player = next(record["body"] for record in input_m2
                            if record["kind"] == 6 and record["body"][0] == 1)
        output_player = next(record["body"] for record in output_m2
                             if record["kind"] == 6 and record["body"][0] == 1)
        self.assertEqual((input_player[56], output_player[56]), (15, 15))
        self.assertEqual((input_player[57], output_player[57]), (96, 96))
        for records in (input_hst, output_hst):
            host_player = [record["body"] for record in records
                           if record["kind"] == 6 and record["body"][0] == 1]
            self.assertEqual(len(host_player), 1)
            self.assertEqual(host_player[0][56], 15)
        for records in (output_m2, output_hst):
            self.assertEqual({record["turn"] for record in records}, {1})
        self.assertEqual({record["turn"] for record in output_m1}, {0, 1})
        ai_log = decode_stream(RUN / "backup/CLEAN.X2", inspector)
        self.assertEqual({record["turn"] for record in ai_log}, {0})
        self.assertEqual([record["kind"] for record in ai_log],
                         [8, 9, 27, 27, 27, 27, 4, 1, 29, 0])
        self.assertEqual({record["turn"] for record in decode_stream(
            RUN / "output/CLEAN.H2", inspector)}, {0})
        owner1 = [record for record in output_m2
                  if record["kind"] == 6 and record["body"][0] == 1]
        self.assertEqual(len(owner1), 1)
        self.assertEqual(struct.unpack_from("<H", owner1[0]["body"], 6)[0],
                         0x6797)
        before = owner1_fleet_order_pairs(input_m2, 0)
        after = owner1_fleet_order_pairs(output_m2, 1)
        after_hst = owner1_fleet_order_pairs(output_hst, 1)
        self.assertEqual([pair["fleet"]["id"] for pair in before],
                         [0, 1, 2, 3])
        self.assertEqual([pair["fleet"]["id"] for pair in after],
                         [0, 1, 2, 3])
        self.assertEqual(after_hst, after)
        fleet0 = after[0]
        self.assertEqual(fleet0["fleet"]["point"], [1313, 1262])
        self.assertEqual(fleet0["fleet"]["cargo"][4], 298)
        self.assertEqual(fleet0["order"]["target_id"], 26)
        self.assertEqual(fleet0["order"]["warp"], 5)
        self.assertEqual(after[1]["fleet"]["cargo"][3], 25)
        self.assertEqual(owner_planet_record(output_m2, 1, 24)["minerals"],
                         [539, 965, 498, 256])
        for records in (output_m2, output_hst):
            self.assertEqual([record["body"].hex() for record in records
                              if record["kind"] == 28], ["011c9205"])

    def test_retained_hashes(self):
        expected = {
            "input/CLEAN.HST": "3a28217e52ebc9619237d9af54c40e614480d42e2d1e5b37c2f2d9e0f961e207",
            "input/CLEAN.M2": "62b53d7d47a64525256f847c51fece2aabf2c76920eb92bae87eabd79fca0856",
            "backup/CLEAN.X2": "591c69c1c395c1021a9b8f8714ef5c9076515d989e40a225f60bce7a2a95d0b7",
            "output/CLEAN.H2": "a16517ee48d78eafc141bdc2d511fa069d690901a7da4a4594d29dd00f9fc4dc",
            "output/CLEAN.HST": "18a2735c067c040788dddbd36fc11772191c1931cb9ca38245a221ea6052c76d",
            "output/CLEAN.M1": "66afc0b29b56a7c25fe6a318ff0184cee797caafa2356ecfcb23e3590e491497",
            "output/CLEAN.M2": "6df100298e84976e31c55b0c8c426e15e3640b00c9a4285f9d2e9cdc1db36d66",
        }
        for relative, digest in expected.items():
            with self.subTest(file=relative):
                self.assertEqual(hashlib.sha256((RUN / relative).read_bytes())
                                 .hexdigest(), digest)


if __name__ == "__main__":
    unittest.main()
