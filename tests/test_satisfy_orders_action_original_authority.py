"""Pin the isolated original game's host-versus-player order experiment."""

import hashlib
from pathlib import Path
import unittest

from tools.race_file_compare import Inspector
from tools.satisfy_orders_original_oracle import decode_owner_fleet
from tools.stars_game_stream import decode_stream


ROOT = Path(__file__).resolve().parents[1]
RUNS = {
    "control": ("vmctl-syorig1",
                "74a27a31221f67328a98200c5dfde54f4c955a6498cc37ac08624b64a33997f6",
                "95d2d7fd27a39ea1668685beeb4785d876d9ed3cfb657f58173ea47e04e8c0a7",
                "96e388d36aa02b6e6640b97e3f0e18e81ffb8b052a244807723c1b516e48bb80"),
    "m1_only": ("vmctl-syact1",
                "74a27a31221f67328a98200c5dfde54f4c955a6498cc37ac08624b64a33997f6",
                "f40e854a83eeb12fd34c17cd3f8b7467f6b7ebfde28d3afa03ef4aed7a7ebfdc",
                "f85ef1df5b69998995a7ca7d405559b6ec69fbed54f3d18d36838bebf7f9dbf2"),
    "both": ("vmctl-syact2",
             "aa6161bb6c95c8276b7a5d12e5a12d9999e3dea0fa242a3be89a94a05c45df3c",
             "f40e854a83eeb12fd34c17cd3f8b7467f6b7ebfde28d3afa03ef4aed7a7ebfdc",
             "fc839e7e262387f08e8cbd5ab2c93c83e110ed52d87a1d4abd961ff75f02e6d1"),
    "host_only": ("vmctl-syact3",
                  "aa6161bb6c95c8276b7a5d12e5a12d9999e3dea0fa242a3be89a94a05c45df3c",
                  "95d2d7fd27a39ea1668685beeb4785d876d9ed3cfb657f58173ea47e04e8c0a7",
                  "c1ff47096624165fd3a94c1b39c617f832a05a92ee60d9b5bff262529d321663"),
}


def sha256(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


class SatisfyOrdersActionOriginalAuthorityTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.inspector = Inspector()

    def test_pinned_original_inputs_and_turn_one_outcomes(self):
        observed = {}
        for variant, (directory, hst_sha, m1_sha, output_sha) in RUNS.items():
            root = ROOT / "runs" / directory
            self.assertEqual(sha256(root / "BACKUP/CLEAN.HST"), hst_sha)
            self.assertEqual(sha256(root / "BACKUP/CLEAN.M1"), m1_sha)
            self.assertEqual(sha256(root / "CLEAN.M1"), output_sha)
            self.assertEqual((root / "CLEAN.XY").read_bytes(),
                             (ROOT / "runs/vmctl-syorig1/CLEAN.XY").read_bytes())
            records = decode_stream(root / "CLEAN.M1", self.inspector)
            self.assertEqual([r["turn"] for r in records if r["kind"] == 8],
                             [0, 1])
            fleets = [decode_owner_fleet(r["body"]) for r in records
                      if r["turn"] == 1 and r["kind"] == 16]
            messages = [r["body"] for r in records
                        if r["turn"] == 1 and r["kind"] == 12]
            self.assertEqual(len(messages), 1)
            observed[variant] = (
                [fleet["id"] for fleet in fleets],
                [fleet["cargo"][4] for fleet in fleets],
                int.from_bytes(messages[0][:2], "little") & 0x1ff,
                messages[0].hex(),
            )
        clean = ([0, 1, 2, 3, 4, 5], [50, 300, 200, 450, 530, 210],
                 63, "3f000a000a")
        host_changed = ([1, 2, 3, 4, 5], [300, 200, 450, 530, 210],
                        90, "5a000a000014000a41010a000014000a3f000a000a")
        self.assertEqual(observed["control"], clean)
        self.assertEqual(observed["m1_only"], clean)
        self.assertEqual(observed["both"], host_changed)
        self.assertEqual(observed["host_only"], host_changed)


if __name__ == "__main__":
    unittest.main()
