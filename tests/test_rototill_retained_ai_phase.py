"""Run one Rototill AI phase with the full retained owner-1 input view."""

import json
from pathlib import Path
import subprocess
import tempfile
import unittest

from tools.package_rototill_scout_route_e2_native import fixture_header
from tools.rototill_turn_oracle import ORACLE
from tools.satisfy_orders_input_corpus import load_pair

ROOT = Path(__file__).resolve().parents[1]


def retained_header():
    oracle = json.loads(ORACLE.read_text(encoding="utf-8"))
    fleets = oracle["before"]["fleets"]
    planet = oracle["before"]["planet_24"]
    loaded = load_pair()["control"]["M2"].planets[24]
    if len(fleets) != 4 or [row["fleet"]["id"] for row in fleets] != [0, 1, 2, 3]:
        raise ValueError("retained fleet census changed")
    if (loaded.id != 24 or loaded.player != 1 or
            list(loaded.minerals) != planet["minerals"] or
            list(loaded.mineral_concentrations) != planet["concentrations"]):
        raise ValueError("loaded planet differs from Rototill compact input")
    if loaded.flags & 0x0200 == 0 or loaded.minerals[3] != 250:
        raise ValueError("retained production gate input changed")

    def vector(name, values):
        return "static const StarsI16 %s[4] = {%s};\n" % (
            name, ", ".join(str(value) for value in values))

    lines = [fixture_header(),
             "#define RETAINED_PLANET_FLAGS %d\n" % loaded.flags,
             "static const StarsU8 RETAINED_CONCENTRATIONS[3] = {%s};\n" %
             ", ".join(str(value) for value in planet["concentrations"]),
             "static const StarsU32 RETAINED_PLANET_MINERALS[4] = {%s};\n" %
             ", ".join(str(value) + "UL" for value in loaded.minerals)]
    for name, source in (("RETAINED_FLEET_FLAGS", lambda row: row["fleet"]["saved_flags"]),
                         ("RETAINED_FLEET_PLANETS", lambda row: row["fleet"]["planet"]),
                         ("RETAINED_FLEET_X", lambda row: row["fleet"]["point"][0]),
                         ("RETAINED_FLEET_Y", lambda row: row["fleet"]["point"][1]),
                         ("RETAINED_ORDER_X", lambda row: row["order"]["point"][0]),
                         ("RETAINED_ORDER_Y", lambda row: row["order"]["point"][1]),
                         ("RETAINED_ORDER_TARGET", lambda row: row["order"]["target_id"]),
                         ("RETAINED_ORDER_FLAGS", lambda row: row["order"]["flags"])):
        lines.append(vector(name, [source(row) for row in fleets]))
    lines.append("static const StarsI16 RETAINED_SHIPS[4][16] = {\n")
    for row in fleets:
        lines.append("    {%s},\n" % ", ".join(
            str(value) for value in row["fleet"]["ship_counts"]))
    lines.append("};\n")
    lines.append("static const StarsI32 RETAINED_CARGO[4][5] = {\n")
    for row in fleets:
        lines.append("    {%s},\n" % ", ".join(
            str(value) + "L" for value in row["fleet"]["cargo"]))
    lines.append("};\n")
    return "".join(lines)


class RototillRetainedAiPhaseTests(unittest.TestCase):
    def test_four_fleets_and_production_in_one_personality_call(self):
        with tempfile.TemporaryDirectory() as temp:
            temp_path = Path(temp)
            (temp_path / "RETFLX.H").write_text(retained_header(),
                                                encoding="ascii")
            exe = temp_path / "retained_rototill_ai_phase"
            subprocess.run([
                "cc", "-std=c89", "-pedantic", "-Wall", "-Wextra",
                "-Werror", "-I", str(ROOT / "reconstructed/include"),
                "-I", str(temp_path),
                str(ROOT / "tests/do_rototill_retained_ai_phase_probe.c"),
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
        self.assertIn("FAILURES=0 RESULT=PASS", result.stdout)


if __name__ == "__main__":
    unittest.main()
