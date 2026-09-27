"""Keep the complete serialized Rototill replay target tied to r1 files."""

import json
import unittest

from tools.rototill_turn_oracle import ORACLE, materialize


class RototillTurnOracleTests(unittest.TestCase):
    def test_retained_oracle_is_reproducible(self):
        saved = json.loads(ORACLE.read_text(encoding="utf-8"))
        self.assertEqual(saved, materialize())
        self.assertEqual(saved["schema"],
                         "rototill-turn-r1-serialized-oracle-v1")
        self.assertEqual(len(saved["planet_points"]), 32)
        self.assertEqual([pair["fleet"]["id"] for pair in
                          saved["before"]["fleets"]], [0, 1, 2, 3])
        self.assertEqual(saved["before"]["production_type_28"], [])
        self.assertEqual(saved["after"]["production_type_28"],
                         ["011c9205"])
        self.assertEqual(saved["output_production_items"], [{
            "quantity": 1, "type": 7, "object_kind": 1,
            "progress": 89, "reserved_high": 0,
        }])

    def test_serialized_delta_is_explicit(self):
        saved = json.loads(ORACLE.read_text(encoding="utf-8"))
        changed = {entry["field"]: (entry["before"], entry["after"])
                   for entry in saved["changes"]}
        self.assertEqual(len(changed), len(saved["changes"]))
        self.assertEqual(len(changed), 26)
        self.assertEqual(changed["fleets[0].fleet.planet"], (24, 26))
        self.assertEqual(changed["fleets[0].fleet.cargo[4]"], (300, 298))
        self.assertEqual(changed["fleets[0].order.warp"], (0, 5))
        self.assertEqual(changed["fleets[1].fleet.cargo[3]"], (0, 25))
        self.assertEqual(changed["planet_24.minerals[3]"], (250, 256))
        self.assertEqual(changed["planet_24.improvements.factories"],
                         (10, 12))
        self.assertEqual(changed["production_type_28"],
                         ([], ["011c9205"]))


if __name__ == "__main__":
    unittest.main()
