"""Verify the isolated Rototill original-turn candidate changes only selector."""

import ctypes
import hashlib
from pathlib import Path
import struct
import tempfile
import unittest

from tools.prepare_rototill_original_turn import (
    PINNED, SOURCE_PATHS, prepare)
from tools.race_file_compare import Inspector
from tools.stars_game_stream import decode_stream, encode_stream

ROOT = Path(__file__).resolve().parents[1]


class PrepareRototillOriginalTurnTests(unittest.TestCase):
    def test_pinned_streams_change_one_personality_byte(self):
        inspector = Inspector(ctypes.CDLL(str(
            (ROOT / "build/libstars_core.dylib").resolve())))
        with tempfile.TemporaryDirectory() as temporary:
            destination = Path(temporary) / "candidate"
            manifest = prepare(destination)
            self.assertEqual(manifest["pre_run_absent"],
                             ["CLEAN.X2", "CLEAN.H2"])
            self.assertEqual(sorted(path.name for path in
                                    destination.iterdir()),
                             sorted(list(PINNED) + ["MANIFEST.JSON"]))
            for name in PINNED:
                with self.subTest(name=name):
                    source = SOURCE_PATHS[name].read_bytes()
                    candidate = (destination / name).read_bytes()
                    self.assertEqual(hashlib.sha256(source).hexdigest(),
                                     PINNED[name])
                    self.assertEqual(len(candidate), len(source))
                    if name not in ("CLEAN.HST", "CLEAN.M2"):
                        self.assertEqual(candidate, source)
                        continue
                    original_records = decode_stream(SOURCE_PATHS[name],
                                                     inspector)
                    candidate_records = decode_stream(destination / name,
                                                      inspector)
                    self.assertEqual(encode_stream(candidate_records,
                                                   inspector), candidate)
                    self.assertEqual(len(candidate_records),
                                     len(original_records))
                    changes = [(old, new) for old, new in zip(
                        original_records, candidate_records)
                        if old["body"] != new["body"]]
                    self.assertEqual(len(changes), 1)
                    old, new = changes[0]
                    self.assertEqual((old["kind"], old["turn"],
                                      old["body"][0]), (6, 0, 1))
                    self.assertEqual(struct.unpack_from("<H", old["body"],
                                                        6)[0], 0x2797)
                    self.assertEqual(struct.unpack_from("<H", new["body"],
                                                        6)[0], 0x6797)
                    self.assertEqual([offset for offset, (a, b) in enumerate(
                        zip(old["body"], new["body"])) if a != b], [7])
            with self.assertRaisesRegex(ValueError, "exists"):
                prepare(destination)


if __name__ == "__main__":
    unittest.main()
