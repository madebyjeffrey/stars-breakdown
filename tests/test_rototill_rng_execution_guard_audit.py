"""Verify the retained original RNG execution-context cut set."""

import unittest

from tools.rototill_rng_execution_guard_audit import audit


class RototillRngExecutionGuardAuditTests(unittest.TestCase):
    def test_original_selector_three_guard_cut(self):
        result = audit()
        self.assertEqual(len(result["cut_edges"]), 7)
        self.assertEqual(result["four_design_low_nibbles"], [1, 1, 1, 1])
        self.assertEqual(result["retained_fleet_order_count"], 10)
        self.assertFalse(result["draw_plan_ship_reachable_after_guards"])
        self.assertFalse(result["sort_report_cache_reachable_after_guards"])
        self.assertFalse(result["randomize2_reachable_after_guards"])


if __name__ == "__main__":
    unittest.main()
