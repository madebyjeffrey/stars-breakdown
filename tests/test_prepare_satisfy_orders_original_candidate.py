"""Verify the action-bearing original-turn package stays isolated and pinned."""

from pathlib import Path
import tempfile
import unittest

from tools.prepare_satisfy_orders_original_candidate import prepare
from tools.race_file_compare import Inspector
from tools.stars_game_stream import decode_stream


class PrepareSatisfyOrdersOriginalCandidateTests(unittest.TestCase):
    def test_bundle_has_only_pinned_inputs_and_one_mutated_order(self):
        with tempfile.TemporaryDirectory() as temp:
            target = Path(temp) / "guest_input"
            manifest = prepare(target)
            self.assertEqual(set(manifest["files"]),
                             {"CLEAN.XY", "CLEAN.HST", "CLEAN.M1",
                              "CLEAN.M2", "STARS.EXE"})
            self.assertEqual(manifest["status"],
                             "candidate_input_only_not_original_behavior_evidence")
            records = decode_stream(target / "CLEAN.M1", Inspector())
            action = [(r["kind"], r["body"].hex()) for r in records
                      if r["turn"] == 0 and r["kind"] == 19]
            self.assertEqual(action,
                             [(19, "680414050a000511" + "00" * 10)])
            with self.assertRaisesRegex(ValueError, "destination exists"):
                prepare(target)

    def test_host_and_m1_candidate_each_change_one_matching_order(self):
        with tempfile.TemporaryDirectory() as temp:
            target = Path(temp) / "guest_host_input"
            manifest = prepare(target, host_order=True)
            self.assertIn("HST and M1", manifest["order_change"])
            for name in ("CLEAN.HST", "CLEAN.M1"):
                records = decode_stream(target / name, Inspector())
                action = [(r["kind"], r["body"].hex()) for r in records
                          if r["turn"] == 0 and r["kind"] == 19]
                self.assertEqual(action,
                                 [(19, "680414050a000511" + "00" * 10)])

    def test_host_only_candidate_keeps_verified_m1(self):
        with tempfile.TemporaryDirectory() as temp:
            target = Path(temp) / "guest_host_only"
            manifest = prepare(target, host_order=True, m1_order=False)
            self.assertIn("HST only", manifest["order_change"])
            self.assertEqual((target / "CLEAN.M1").read_bytes(),
                             (Path(__file__).resolve().parents[1] /
                              "runs/vmctl-syorig1/BACKUP/CLEAN.M1").read_bytes())
            records = decode_stream(target / "CLEAN.HST", Inspector())
            self.assertEqual([(r["kind"], r["body"].hex()) for r in records
                              if r["turn"] == 0 and r["kind"] == 19],
                             [(19, "680414050a000511" + "00" * 10)])


if __name__ == "__main__":
    unittest.main()
