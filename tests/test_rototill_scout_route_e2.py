"""Replay retained selector-3 scout input through the current Rototill C path."""

import ctypes
from pathlib import Path
import subprocess
import tempfile
import unittest

from tests.test_satisfy_orders_turin_binding import (
    owner1_fleet_order_pairs, xy_points)
from tools.race_file_compare import Inspector
from tools.stars_game_stream import decode_stream
from tools.package_rototill_scout_route_e2_native import fixture_header

ROOT = Path(__file__).resolve().parents[1]
RUN = ROOT / "runs/rototill-original-turn-r1"


class RototillScoutRouteE2Tests(unittest.TestCase):
    def test_retained_player_view_routes_and_writes_second_order(self):
        inspector = Inspector(ctypes.CDLL(str(
            (ROOT / "build/libstars_core.dylib").resolve())))
        records = decode_stream(RUN / "input/CLEAN.M2", inspector)
        original = decode_stream(RUN / "output/CLEAN.M2", inspector)
        before = owner1_fleet_order_pairs(records, 0)[0]
        after = owner1_fleet_order_pairs(original, 1)[0]
        points = xy_points(RUN / "input/CLEAN.XY", inspector)
        owners = [-2] * len(points)
        for record in records:
            if record["kind"] == 13 and record["turn"] == 0:
                packed = int.from_bytes(record["body"][:2], "little")
                owners[packed & 2047] = packed >> 11
        self.assertEqual([index for index, owner in enumerate(owners)
                          if owner != -2], [24])
        self.assertEqual(before["fleet"]["id"], 0)
        self.assertEqual(before["fleet"]["ship_counts"],
                         [1] + [0] * 15)
        self.assertEqual(after["order"]["target_id"], 26)
        self.assertEqual(after["order"]["point"], [1313, 1262])
        self.assertEqual(after["order"]["warp"], 5)
        header = fixture_header()
        with tempfile.TemporaryDirectory() as temp:
            temp_path = Path(temp)
            (temp_path / "ROTSCFX.H").write_text(header)
            exe = temp_path / "rototill_scout_route"
            subprocess.run([
                "cc", "-std=c89", "-pedantic", "-Wall", "-Wextra",
                "-Werror", "-I", str(ROOT / "reconstructed/include"),
                "-I", str(temp_path),
                str(ROOT / "tests/do_rototill_scout_route_e2_probe.c"),
                str(ROOT / "reconstructed/src/do_rototill_ai_turn.c"),
                str(ROOT / "reconstructed/src/rototill_scout_adapter.c"),
                str(ROOT / "reconstructed/src/ai.c"),
                str(ROOT / "reconstructed/src/ai_plord.c"),
                str(ROOT / "reconstructed/src/find_ideal_warp.c"),
                str(ROOT / "reconstructed/src/battle.c"),
                str(ROOT / "reconstructed/src/utilgen.c"),
                "-o", str(exe),
            ], check=True)
            result = subprocess.run([str(exe)], capture_output=True,
                                    text=True, check=False)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertIn(
            "ROTOTILL_SCOUT_ROUTE_E2_CHECKS=180 FAILURES=0 RESULT=PASS",
            result.stdout)


if __name__ == "__main__":
    unittest.main()
