"""Keep the original binary indirect RNG callsite audit reproducible."""

import unittest

from tools.rototill_rng_indirect_resolution import resolve


class RototillRngIndirectResolutionTests(unittest.TestCase):
    def test_original_indirect_sites(self):
        result = resolve()
        self.assertEqual(len(result["indirect_sites"]), 8)
        self.assertEqual(len(result["draw_plan_ship_table_callbacks"]), 13)
        self.assertTrue(all(row["target"] != "1040:16d2" for row in
                            result["draw_plan_ship_table_callbacks"]))
        self.assertFalse(result["start_tutor_directly_reachable"])
        self.assertEqual(result["bsearch_comparator"],
                         "1038:1fa2 ICompFleetPoint2")
        self.assertEqual(result["qsort_comparator"],
                         "1108:5bb8 ICompReport")


if __name__ == "__main__":
    unittest.main()
