"""Pin the original-turn delta used to reuse the Turin turn harness."""

import json
import unittest

from tools.rototill_turin_variant_audit import REPORT, audit


class RototillTurinVariantAuditTests(unittest.TestCase):
    def test_input_and_turn_one_variant(self):
        report = json.loads(REPORT.read_text(encoding="utf-8"))
        self.assertEqual(report, audit())
        self.assertTrue(report["input"]["M1"]["bytes_identical"])
        self.assertTrue(report["input"]["XY"]["bytes_identical"])
        for suffix, index in (("HST", 2), ("M2", 1)):
            self.assertEqual(report["input"][suffix]
                             ["decoded_differences"], [{
                                 "record_index": index, "kind": 6,
                                 "byte_offsets": [7],
                                 "before_hex": "27", "after_hex": "67",
                             }])
        self.assertEqual({suffix: (item["identical_records_in_order"],
                                   item["turin_turn_1_records"],
                                   item["rototill_turn_1_records"])
                          for suffix, item in report["output"].items()},
                         {"HST": (69, 83, 87), "M1": (30, 32, 32),
                          "M2": (14, 24, 28)})


if __name__ == "__main__":
    unittest.main()
