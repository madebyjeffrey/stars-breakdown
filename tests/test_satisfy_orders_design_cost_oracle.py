"""Pin the verified original task-5 design input and its source cost rule."""

import json
from pathlib import Path
import unittest

from tools.race_file_compare import Inspector
from tools.satisfy_orders_design_cost_oracle import recover
from tools.stars_game_stream import decode_stream


ROOT = Path(__file__).resolve().parents[1]


class SatisfyOrdersDesignCostOracleTests(unittest.TestCase):
    def test_original_hst_design_and_executable_tables_recover(self):
        current = recover()
        pinned = json.loads((ROOT / "analysis/compat/"
                             "satisfy-orders-design0-input-oracle.json")
                            .read_text())
        self.assertEqual(current, pinned)
        self.assertEqual(current["owner_design_cost"], [24, 12, 8, 7])
        self.assertEqual(current["ordinary_starbase_mineral_return"],
                         [9, 6, 5])
        self.assertEqual([(part["group"], part["item"], part["quantity"])
                          for part in current["components"]],
                         [(1, 3, 1), (2, 1, 1), (0x10, 1, 1)])

    def test_original_message_and_planet_delta_are_distinct(self):
        inspector = Inspector()
        clean = decode_stream(ROOT / "runs/vmctl-syorig1/CLEAN.M1",
                              inspector)
        action = decode_stream(ROOT / "runs/vmctl-syact3/CLEAN.M1",
                               inspector)

        def planet_minerals(records):
            matches = [record["body"] for record in records
                       if record["kind"] == 13 and record["turn"] == 1
                       and record["body"][:2] == b"\x0a\x00"]
            self.assertEqual(len(matches), 1)
            return [int.from_bytes(matches[0][i:i + 2], "little")
                    for i in (17, 19, 21)]

        before, after = planet_minerals(clean), planet_minerals(action)
        self.assertEqual(before, [476, 902, 383])
        self.assertEqual(after, [485, 907, 388])
        self.assertEqual([a - b for a, b in zip(after, before)], [9, 5, 5])
        messages = [record["body"] for record in action
                    if record["kind"] == 12 and record["turn"] == 1]
        self.assertEqual(len(messages), 1)
        self.assertEqual(messages[0][:8].hex(), "5a000a000014000a")
        self.assertEqual(messages[0][5], 20)
        self.assertEqual(sum(recover()["ordinary_starbase_mineral_return"]),
                         20)


if __name__ == "__main__":
    unittest.main()
