"""Pin the stable original-turn M1 message/fleet/order plaintext subset."""

import hashlib
import json
from pathlib import Path
import unittest

from tools.race_file_compare import Inspector
from tools.satisfy_orders_original_oracle import (decode_compact_order,
                                                  decode_owner_fleet,
                                                  decode_planet_prefix)
from tools.stars_game_stream import decode_stream


ROOT = Path(__file__).resolve().parents[1]
ORACLE = ROOT / "analysis/compat/satisfy-orders-original-m1-stable-e3.json"


class SatisfyOrdersOriginalM1OracleTests(unittest.TestCase):
    def test_scoped_decoder_rejects_truncated_and_wrong_record_kinds(self):
        oracle = json.loads(ORACLE.read_text())
        fleet = bytes.fromhex(oracle["fleet_bodies_hex"][0])
        order = bytes.fromhex(oracle["order_body_hex"])
        with self.assertRaisesRegex(ValueError, "truncated"):
            decode_owner_fleet(fleet[:-1])
        wrong_det = bytearray(fleet)
        wrong_det[4] = 3
        with self.assertRaisesRegex(ValueError, "det=7"):
            decode_owner_fleet(wrong_det)
        with self.assertRaisesRegex(ValueError, "eight bytes"):
            decode_compact_order(order[:-1])
        wrong_task = bytearray(order)
        wrong_task[6] = 1
        with self.assertRaisesRegex(ValueError, "noncompact"):
            decode_compact_order(wrong_task)
        with self.assertRaisesRegex(ValueError, "truncated"):
            decode_planet_prefix(b"\x0a\x00\x87\x2b\x15")
        bad_planet = bytearray.fromhex(
            "0a00872b15f7f1f74457443232324320aadc0186037f011f01320aa0000a00000000000000")
        bad_planet[4] = 0x02
        with self.assertRaisesRegex(ValueError, "invalid mineral"):
            decode_planet_prefix(bad_planet)

    def test_three_original_replays_agree_on_turn_one_subset(self):
        oracle = json.loads(ORACLE.read_text())
        inspector = Inspector()
        expected = [(oracle["message"]["kind"],
                     oracle["message"]["body_hex"])]
        for fleet in oracle["fleet_bodies_hex"]:
            expected.append((oracle["fleet_kind"], fleet))
            expected.append((oracle["order_kind"],
                             oracle["order_body_hex"]))
        self.assertEqual(len(expected), 13)
        self.assertEqual(len(oracle["source_files"]), 3)
        for source in oracle["source_files"]:
            path = ROOT / source["path"]
            self.assertEqual(hashlib.sha256(path.read_bytes()).hexdigest(),
                             source["sha256"])
            records = decode_stream(path, inspector)
            actual = [(record["kind"], record["body"].hex())
                      for record in records
                      if record["turn"] == oracle["turn"] and
                      record["kind"] in (oracle["message"]["kind"],
                                         oracle["fleet_kind"],
                                         oracle["order_kind"])]
            self.assertEqual(actual, expected, str(path))
            planets = [decode_planet_prefix(record["body"])
                       for record in records if record["kind"] == 13]
            self.assertEqual(len(planets), 2)
            for planet in planets:
                self.assertEqual((planet["id"], planet["owner"],
                                  planet["flags"]), (10, 0, 0x2b87))
                self.assertEqual(planet["mineral_concentrations"],
                                 [68, 87, 68])
                self.assertEqual(planet["environment"], [50, 50, 50])
            self.assertEqual(planets[0]["mineral_levels"], [0, 0, 0])
            self.assertEqual(planets[1]["mineral_levels"], [247, 241, 247])
            fleets = [decode_owner_fleet(record["body"])
                      for record in records
                      if record["turn"] == 1 and record["kind"] == 16]
            orders = [decode_compact_order(record["body"])
                      for record in records
                      if record["turn"] == 1 and record["kind"] == 20]
            self.assertEqual([fleet["id"] for fleet in fleets], list(range(6)))
            self.assertEqual([fleet["owner"] for fleet in fleets], [0] * 6)
            self.assertEqual([fleet["saved_flags"] for fleet in fleets],
                             [0x2907] * 6)
            self.assertEqual([fleet["planet"] for fleet in fleets], [10] * 6)
            self.assertEqual([fleet["point"] for fleet in fleets],
                             [[1128, 1300]] * 6)
            self.assertEqual([fleet["ship_bitmap"] for fleet in fleets],
                             [1 << index for index in range(6)])
            self.assertEqual([fleet["ship_counts"][index]
                              for index, fleet in enumerate(fleets)], [1] * 6)
            self.assertEqual([fleet["cargo"][4] for fleet in fleets],
                             [50, 300, 200, 450, 530, 210])
            self.assertEqual([fleet["order_count"] for fleet in fleets],
                             [1] * 6)
            self.assertEqual(orders, [dict(point=[1128, 1300], target_id=10,
                                           flags=0x1100, task=0, warp=0,
                                           object_kind=1, valid_task=1)] * 6)
            prior = [decode_owner_fleet(record["body"])
                     for record in records
                     if record["turn"] == 0 and record["kind"] == 16]
            prior_orders = [decode_compact_order(record["body"])
                            for record in records
                            if record["turn"] == 0 and record["kind"] == 20]
            self.assertEqual(len(prior), 6)
            self.assertEqual(prior_orders, orders)
            for before, after in zip(prior, fleets):
                self.assertEqual(before["saved_flags"], 0x0907)
                self.assertEqual({**before, "saved_flags": 0x2907}, after)


if __name__ == "__main__":
    unittest.main()
