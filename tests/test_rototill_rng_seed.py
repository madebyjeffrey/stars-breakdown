"""Recover Rototill's effective main RNG seed from original AI names."""

from ctypes import POINTER, byref, c_int16, c_uint16, c_uint32, c_ubyte
from pathlib import Path
import re
import unittest

from tests.test_satisfy_orders_fresh_ai_pair import _decompress_user_name
from tools.race_file_compare import Inspector, Seeds
from tools.stars_game_stream import decode_stream


ROOT = Path(__file__).resolve().parents[1]
OUTPUT = ROOT / "runs/rototill-original-turn-r1/output/CLEAN.M2"


class RototillRngSeedTests(unittest.TestCase):
    def test_original_names_recover_unique_effective_seed(self):
        inspector = Inspector()
        rows = decode_stream(OUTPUT, inspector)
        names = {}
        for row in rows:
            if row["turn"] != 1 or row["kind"] != 26:
                continue
            body = row["body"]
            slot = (int.from_bytes(body[:2], "little") >> 10) & 31
            if slot not in range(17, 21):
                continue
            tail = body[17 + body[6] * 4:]
            self.assertEqual(len(tail), tail[0] + 1)
            names[slot] = _decompress_user_name(tail[1:])
        self.assertEqual(names, {
            17: "Evil Spawn", 18: "Evil Spawn",
            19: "Silver Tower", 20: "Potato",
        })
        raw = ROOT / "analysis/decompiler/export/raw"
        rototill = (raw / "1098_1e22_DoRototillAiTurn.c").read_text()
        iro = (raw / "1090_425a_IroEnsureAi.c").read_text()
        create = (raw / "1090_7222_EnsureAiStarbaseDesigns.c").read_text()
        self.assertLess(rototill.index("IroEnsureAi("),
                        rototill.index("IdTargetScout("))
        self.assertIn("EnsureAiStarbaseDesigns();", iro)
        self.assertEqual(re.findall(r"FCreateAiStarbase\((\d+),",
                                    create.split("if (0x31 < game.turn)", 1)[0]),
                         ["2", "4", "1", "3"])
        get_string = inspector.lib.stars_get_compressed_string
        get_string.argtypes = [POINTER(c_ubyte), POINTER(c_ubyte), c_int16]
        get_string.restype = c_uint16
        observed_pool = {}
        for string_id in range(0x432, 0x432 + 13):
            memory = (c_ubyte * 65536)()
            memory[0x1484] = 0xff
            address = get_string(memory, inspector.strings, string_id)
            observed_pool[string_id] = bytes(memory)[address:].split(
                b"\0", 1)[0].decode("cp1252")
        self.assertEqual([observed_pool[0x432 + value]
                          for value in (12, 6, 12, 2)],
                         [names[slot] for slot in (18, 20, 17, 19)])
        # EnsureAiStarbaseDesigns creates slots 18, 20, 17, 19. The game
        # string pool begins at 0x432; these names map to 12, 6, 12, 2.
        accepted = [12, 6, 12, 2]
        randomize = inspector.lib.stars_randomize2
        randomize.argtypes = [POINTER(Seeds), c_uint32]
        randomize.restype = None
        draw = inspector.lib.stars_random
        draw.argtypes = [POINTER(Seeds), c_int16]
        draw.restype = c_int16
        matches = []
        for tick in range(1 << 14):
            state = Seeds()
            randomize(byref(state), tick)
            initial = (state.first, state.second)
            values = [draw(byref(state), 13) for _ in range(4)]
            if values == accepted:
                matches.append((tick, initial, draw(byref(state), 100)))
        self.assertEqual(matches, [(6844, (31, 587), 31)])

        # FGenerateTurn resets the main RNG to this fixed seed before
        # FLoadGame only when the tutorial bit is set. The original four
        # design names exclude that branch for this retained turn.
        tutorial_reset = inspector.lib.stars_randomize
        tutorial_reset.argtypes = [POINTER(Seeds), c_uint32]
        tutorial_reset.restype = None
        state = Seeds()
        tutorial_reset(byref(state), 0x499602d2)
        self.assertEqual((state.first, state.second), (71, 41))
        self.assertEqual([draw(byref(state), 13) for _ in range(4)],
                         [9, 11, 12, 1])
        self.assertNotEqual([9, 11, 12, 1], accepted)


if __name__ == "__main__":
    unittest.main()
