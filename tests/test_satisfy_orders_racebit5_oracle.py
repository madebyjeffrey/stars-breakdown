"""Pin the original task-5 resource branch and its single-bit input edit."""

import unittest

from tools.satisfy_orders_racebit5_oracle import recover


class SatisfyOrdersRacebit5OracleTests(unittest.TestCase):
    def test_original_resource_branch(self):
        evidence = recover()
        self.assertEqual(evidence["candidate_race_bits"], 0x20)
        self.assertEqual(evidence["resource_scrap_minus_clean_minerals"],
                         [10, 7, 6, 0])
        self.assertEqual(evidence["resource_message_ids"],
                         [0x5d, 0x143, 0x3f])
        self.assertEqual(evidence["outputs"]["racebit_m1"]["owner0_fleet_ids"],
                         [1, 2, 3, 4, 5])


if __name__ == "__main__":
    unittest.main()
