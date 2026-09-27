"""Regression for FCreateAiStarbase's embedded name and bitmap picker."""

from pathlib import Path
import hashlib
import json
import subprocess
import tempfile
import unittest

from tools.ne_layout import NE
from tools.race_file_compare import Inspector
from tools.stars_game_stream import decode_stream


ROOT = Path(__file__).resolve().parents[1]
STARS_EXE = ROOT / "target/stars.exe"
STARS_EXE_SHA256 = "ccab6682b55dae14dbf104e491cfbd3b32a6280022020032ae0615500949ba59"
PAIR = ROOT / "runs/vmctl-sye3k-pair-r1/PAIR.JSON"
INPUT_HST = ROOT / "runs/vmctl-sye3k0/BACKUP/CLEAN.HST"
OUTPUT_HST = ROOT / "runs/vmctl-sye3k0/CLEAN.HST"


def _retained_player_record(path, turn, inspector):
    matches = [record["body"] for record in decode_stream(path, inspector)
               if record["kind"] == 6 and record["turn"] == turn and
               record["body"] and record["body"][0] == 1]
    if len(matches) != 1 or len(matches[0]) < 58:
        raise ValueError("expected one complete retained owner-1 player record")
    return matches[0]


def _write_turin_starbase_fixture(path):
    if hashlib.sha256(STARS_EXE.read_bytes()).hexdigest() != STARS_EXE_SHA256:
        raise ValueError("original executable differs from the reviewed baseline")
    pair = json.loads(PAIR.read_text(encoding="utf-8"))
    seed = pair["current_source_binding_status"][
        "retained_turin_starbase_rng_source_binding"]
    inspector = Inspector()
    input_player = _retained_player_record(INPUT_HST, 0, inspector)
    output_player = _retained_player_record(OUTPUT_HST, 1, inspector)
    symbols = json.loads((ROOT / "analysis/debug/symbols.json").read_text())
    symbol = next(row for row in symbols
                  if row.get("name") == "vrgAiTurinDroneResOrder")
    segment = symbol["ne_segment"]
    offset = symbol["ne_offset"]
    requirements = bytes(NE(STARS_EXE).raw[segment - 1][offset:offset + 31])
    if len(requirements) != 31:
        raise ValueError("retained Turin research requirements are truncated")

    def c_array(values):
        return ", ".join(str(value) for value in values)

    rows = [
        "#define RETAINED_STARTUP_TICK_CONTROL %dUL" %
        seed["control"]["effective_tick_low14"],
        "#define RETAINED_STARTUP_TICK_ACTION %dUL" %
        seed["action"]["effective_tick_low14"],
        "#define RETAINED_LATEST_STARBASE_DESIGN 0",
        "#define RETAINED_TURIN_RESEARCH_ORDER 0",
        "#define RETAINED_PLAYER_CURRENT_TECH %dU" % input_player[57],
        "#define RETAINED_PLAYER_PERCENT_RESEARCH %d" % input_player[56],
        "#define RETAINED_OUTPUT_CURRENT_TECH %dU" % output_player[57],
        "#define RETAINED_OUTPUT_PERCENT_RESEARCH %d" % output_player[56],
        "static const StarsU8 retained_player_technology[6] = {%s};" %
        c_array(input_player[26:32]),
        "static const StarsU8 retained_starbase_capabilities[3] = {1, 1, 1};",
        "static const StarsU8 retained_turin_research_requirements[31] = {%s};" %
        c_array(requirements),
    ]
    path.write_text("\n".join(rows) + "\n", encoding="ascii")


class AiStarbaseNameTests(unittest.TestCase):
    def test_create_ai_starbase_picks_name_before_publish(self):
        with tempfile.TemporaryDirectory() as temporary:
            executable = Path(temporary) / "create_ai_starbase_source_probe"
            subprocess.run([
                "cc", "-std=c89", "-pedantic", "-Wall", "-Wextra",
                "-Werror", "-ffunction-sections", "-fdata-sections",
                "-Wl,-dead_strip", "-I",
                str(ROOT / "reconstructed/include"),
                str(ROOT / "tests/create_ai_starbase_source_probe.c"),
                str(ROOT / "reconstructed/src/ai.c"),
                "-o", str(executable),
            ], check=True)
            result = subprocess.run([str(executable)], check=False,
                                    capture_output=True, text=True)
        self.assertEqual(result.returncode, 0,
                         result.stdout + result.stderr)
        self.assertEqual(
            result.stdout.strip(),
            "CREATE_AI_STARBASE_CHECKS=3 FAILURES=0 RESULT=PASS",
        )

    def test_turin_startup_names_use_recovered_seed_and_source_order(self):
        with tempfile.TemporaryDirectory() as temporary:
            executable = Path(temporary) / "turin_starbase_rng_probe"
            _write_turin_starbase_fixture(Path(temporary) /
                                          "TURINSTARBASEFX.H")
            subprocess.run([
                "cc", "-std=c89", "-pedantic", "-Wall", "-Wextra",
                "-Werror", "-ffunction-sections", "-fdata-sections",
                "-Wl,-dead_strip", "-I", temporary, "-I",
                str(ROOT / "reconstructed/include"),
                str(ROOT / "tests/satisfy_orders_turin_starbase_rng_probe.c"),
                str(ROOT / "reconstructed/src/ai.c"),
                str(ROOT / "reconstructed/src/utilgen.c"),
                str(ROOT / "reconstructed/src/turin_scout_adapter.c"),
                str(ROOT / "reconstructed/src/do_turin_drone_ai_turn.c"),
                str(ROOT / "reconstructed/src/ai_plord.c"),
                "-o", str(executable),
            ], check=True)
            results = [subprocess.run([str(executable), label], check=False,
                                      capture_output=True, text=True)
                       for label in ("control", "action")]
        for result in results:
            self.assertEqual(result.returncode, 0,
                             result.stdout + result.stderr)
            self.assertEqual(
                result.stdout.strip(),
                "TURIN_STARBASE_STARTUP=PASS NAMES=4 DRAWS=5 SCOUT=PASS",
            )


if __name__ == "__main__":
    unittest.main()
