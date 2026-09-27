"""Pin the naturally deep-space original scrap action and control turn."""

import json
from pathlib import Path
import unittest

from tools.satisfy_orders_smoke_deepspace_oracle import recover


ROOT = Path(__file__).resolve().parents[1]


class SatisfyOrdersSmokeDeepSpaceOracleTests(unittest.TestCase):
    def test_original_salvage_and_retirement(self):
        current = recover()
        pinned = json.loads((ROOT / "analysis/compat/"
                             "satisfy-orders-smoke-deepspace-original-oracle.json")
                            .read_text())
        self.assertEqual(current, pinned)
        self.assertEqual(current["design_cost"], [35, 16, 2, 8])
        self.assertEqual(current["action_salvage"]["minerals"], [5, 0, 2])
        self.assertEqual(current["action_salvage"]["point"], [1276, 1380])
        self.assertFalse(current["action_fleet_present"])
        self.assertFalse(current["action_m2_saved_message_0x5b"])


if __name__ == "__main__":
    unittest.main()
