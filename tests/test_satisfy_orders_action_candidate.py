"""Keep experimental original-turn candidate encoding bounded and reversible."""

import hashlib
from pathlib import Path
import tempfile
import unittest

from tools.race_file_compare import Inspector
from tools.satisfy_orders_action_candidate import (SOURCE_SHA256,
                                                   make_candidate)
from tools.stars_game_stream import decode_stream, encode_stream


ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / "runs/vmctl-syorig1/BACKUP/CLEAN.M1"
TWO_TURN_OUTPUT = ROOT / "runs/vmctl-syorig1/CLEAN.M1"


class SatisfyOrdersActionCandidateTests(unittest.TestCase):
    def test_original_roundtrip_and_one_bounded_order_mutation(self):
        inspector = Inspector()
        original_bytes = SOURCE.read_bytes()
        self.assertEqual(hashlib.sha256(original_bytes).hexdigest(),
                         SOURCE_SHA256)
        original = decode_stream(SOURCE, inspector)
        self.assertEqual(encode_stream(original, inspector), original_bytes)
        candidate_bytes = make_candidate(SOURCE, inspector)
        with tempfile.TemporaryDirectory() as temp:
            path = Path(temp) / "CLEAN.M1"
            path.write_bytes(candidate_bytes)
            candidate = decode_stream(path, inspector)
        self.assertEqual(len(candidate_bytes), len(original_bytes) + 10)
        self.assertEqual(len(candidate), len(original))
        changes = [(before, after) for before, after in zip(original, candidate)
                   if (before["kind"], before["body"]) !=
                   (after["kind"], after["body"])]
        self.assertEqual(len(changes), 1)
        before, after = changes[0]
        self.assertEqual((before["kind"], before["body"].hex()),
                         (20, "680414050a000011"))
        self.assertEqual((after["kind"], after["body"].hex()),
                         (19, "680414050a000511" + "00" * 10))
        self.assertEqual(SOURCE.read_bytes(), original_bytes)

    def test_two_bof_original_output_roundtrips_byte_exactly(self):
        inspector = Inspector()
        records = decode_stream(TWO_TURN_OUTPUT, inspector)
        self.assertEqual([record["turn"] for record in records
                          if record["kind"] == 8], [0, 1])
        self.assertEqual(encode_stream(records, inspector),
                         TWO_TURN_OUTPUT.read_bytes())


if __name__ == "__main__":
    unittest.main()
