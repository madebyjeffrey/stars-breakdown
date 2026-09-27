"""Pin the original deep-space 0x5B frame against a paired control."""

import json
from pathlib import Path
import unittest

from tools.satisfy_orders_smoke_human_message_oracle import recover


ROOT = Path(__file__).resolve().parents[1]


class SatisfyOrdersSmokeHumanMessageOracleTests(unittest.TestCase):
    def test_original_message_and_salvage(self):
        current = recover()
        pinned = json.loads((ROOT / "analysis/compat/"
                             "satisfy-orders-smoke-human-message-original-oracle.json")
                            .read_text())
        self.assertEqual(current, pinned)
        self.assertEqual(current["action_message"]["message_id"], 0x5b)
        self.assertEqual(current["action_message"]["goto"], -6)
        self.assertEqual((current["action_message"]["first"],
                          current["action_message"]["second"]),
                         (0x2200, 0))
        self.assertFalse(current["action_fleet_present"])
        self.assertEqual(current["action_salvage"]["minerals"], [5, 0, 2])


if __name__ == "__main__":
    unittest.main()
