"""Bind task-5 planet output differences to the same original HST input."""

import unittest

from tools.satisfy_orders_planet10_oracle import decode_planet10, recover


class SatisfyOrdersPlanet10OracleTests(unittest.TestCase):
    def test_same_input_planet_and_full_turn_mineral_delta(self):
        result = recover()
        self.assertEqual(result["original_input"]["minerals"],
                         [469, 893, 376, 250])
        self.assertEqual(result["clean_output"]["minerals"],
                         [476, 902, 383, 287])
        self.assertEqual(result["action_output"]["minerals"],
                         [485, 907, 388, 287])
        self.assertEqual(result["m1_only_output"]["minerals"],
                         [475, 902, 383, 287])
        self.assertEqual(result["m1_only_minus_clean"], [-1, 0, 0, 0])
        self.assertEqual(result["sources"]["m1_only_hst"]["sha256"],
                         result["sources"]["clean_hst"]["sha256"])
        self.assertEqual(result["clean_minus_input"], [7, 9, 7, 37])
        self.assertEqual(result["action_minus_input"], [16, 14, 12, 37])
        self.assertEqual(result["action_minus_clean"], [9, 5, 5, 0])
        self.assertEqual(result["remaining_full_turn_delta"],
                         [0, -1, 0, 0])
        self.assertEqual(result["clean_output"]["levels"],
                         result["action_output"]["levels"])
        self.assertEqual(result["clean_output"]["auxiliary_word"] -
                         result["action_output"]["auxiliary_word"], 3)
        for label in ("original_input", "clean_output", "action_output",
                      "m1_only_output"):
            self.assertEqual(result[label]["improvements"]["mines"], 10)
            self.assertEqual(result[label]["improvements"]["factories"], 10)
            self.assertEqual(result[label]["improvements"]["defenses"], 10)
        mining = result["mining_rounding_candidate"]
        self.assertEqual(mining["owner_mine_efficiency"], 10)
        self.assertEqual(mining["owner_mine_operating_limit"], 10)
        self.assertEqual(mining["operating_mines"], 10)
        self.assertEqual(mining["floor"], [6, 8, 6])
        self.assertEqual(mining["remainder"], [80, 70, 80])
        self.assertEqual(mining["clean_turn_gain"], [7, 9, 7])
        self.assertEqual(mining["action_turn_gain_after_scrap"], [7, 8, 7])

    def test_rejects_truncated_or_unexpected_planet(self):
        body = bytes.fromhex(recover()["original_input"]["body_hex"])
        with self.assertRaises(ValueError):
            decode_planet10(body[:-1])
        with self.assertRaises(ValueError):
            decode_planet10(b"\x0b" + body[1:])


if __name__ == "__main__":
    unittest.main()
