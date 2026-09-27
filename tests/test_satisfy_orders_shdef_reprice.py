"""Check the original-part SHDEF cost binding and its native report."""

from pathlib import Path
import subprocess
import tempfile
import unittest

from tools.satisfy_orders_part_fixture import OUTPUT, render
from tools.ne_layout import NE


ROOT = Path(__file__).resolve().parents[1]
EXPECTED = (b"SHDEF_REPRICE_CHECKS=68 FAILURES=0 RESULT=PASS "
            b"COST=24,12,8,7 TASK5=478,899,381 MSG=90,321\n")
HISTORICAL_NATIVE = (
    b"SHDEF_REPRICE_CHECKS=67 FAILURES=0 RESULT=PASS "
    b"COST=24,12,8,7 TASK5=478,899,381 MSG=90,321\n")


class SatisfyOrdersShdefRepriceTests(unittest.TestCase):
    def test_all_original_starbase_hulls_and_invalid_next_id(self):
        source = r'''
#include "common.h"
#include <stdio.h>
#include <string.h>
static StarsI16 race(void *p, StarsI16 player, StarsI16 field)
{ (void)p; (void)player; (void)field; return 0; }
int main(int argc, char **argv) {
    static StarsU8 segment[30000];
    static const StarsU16 expected[5][4] = {
        {80,24,0,34}, {200,40,10,50}, {1200,240,160,500},
        {1200,240,160,600}, {1500,240,160,700}};
    StarsShdefRepriceInput input;
    StarsU8 design[147], result[147];
    signed char tech[6];
    StarsU16 i, j, value;
    FILE *file;
    if (argc != 2 || !(file = fopen(argv[1], "rb"))) return 1;
    memset(&input, 0, sizeof input);
    input.part_segment_size = (StarsU16)fread(segment, 1, sizeof segment, file);
    fclose(file);
    memset(tech, 0, sizeof tech);
    input.part_segment = segment; input.player_technology = tech;
    input.race_stat = race; input.race_grbit = race; input.player = -1;
    memset(design, 0, sizeof design);
    for (i = 0; i < 5; ++i) {
        design[0] = (StarsU8)(32 + i);
        if (!stars_reprice_shdef_cost(&input, design, 147, result)) return 2;
        for (j = 0; j < 4; ++j) {
            value = (StarsU16)(result[42 + j * 2] |
                ((StarsU16)result[43 + j * 2] << 8));
            if (value != expected[i][j]) return 3;
        }
    }
    design[0] = 37;
    memset(result, 0xa5, sizeof result);
    if (stars_reprice_shdef_cost(&input, design, 147, result)) return 4;
    for (i = 0; i < 147; ++i) if (result[i] != 0xa5) return 5;
    puts("STARBASE_HULL_REPRICE=5/5 INVALID_REJECTED=1");
    return 0;
}
'''
        with tempfile.TemporaryDirectory() as directory:
            directory = Path(directory)
            (directory / 'probe.c').write_text(source)
            (directory / 'parts.bin').write_bytes(bytes(NE(ROOT / 'target/stars.exe').raw[1]))
            executable = directory / 'probe'
            subprocess.run(['cc', '-std=c89', '-pedantic', '-Wall', '-Wextra',
                            '-Werror', '-Ireconstructed/include',
                            'reconstructed/src/reprice_shdef.c',
                            'reconstructed/src/lookup_part.c',
                            'reconstructed/src/true_part_cost.c',
                            str(directory / 'probe.c'), '-o', str(executable)],
                           cwd=ROOT, check=True, capture_output=True)
            result = subprocess.run([str(executable), str(directory / 'parts.bin')],
                                    check=True, capture_output=True, text=True)
        self.assertEqual(result.stdout, 'STARBASE_HULL_REPRICE=5/5 INVALID_REJECTED=1\n')

    def test_original_task5_temporary_bit_suppresses_technology_premium(self):
        with tempfile.TemporaryDirectory() as directory:
            executable = Path(directory) / "true_part_cost"
            subprocess.run([
                "cc", "-std=c89", "-pedantic", "-Wall", "-Wextra",
                "-Werror", "-Ireconstructed/include",
                "reconstructed/src/true_part_cost.c",
                "tests/true_part_cost_source_probe.c", "-o", str(executable),
            ], cwd=ROOT, check=True, capture_output=True)
            result = subprocess.run([str(executable)], cwd=ROOT, check=True,
                                    capture_output=True, text=True)
        self.assertEqual(result.stdout, "TRUE_PART_COST_SOURCE=PASS\n")

    def test_original_records_and_production_reprice(self):
        self.assertEqual(OUTPUT.read_text(), render())
        with tempfile.TemporaryDirectory() as directory:
            executable = Path(directory) / "probe"
            subprocess.run([
                "cc", "-std=c89", "-Wall", "-Wextra", "-Werror",
                "-Ireconstructed/include", "-Itests",
                "reconstructed/src/reprice_shdef.c",
                "reconstructed/src/lookup_part.c",
                "reconstructed/src/true_part_cost.c",
                "reconstructed/src/satisfy_orders.c",
                "tests/satisfy_orders_shdef_reprice_probe.c",
                "-o", str(executable),
            ], cwd=ROOT, check=True, capture_output=True)
            result = subprocess.run([str(executable)], cwd=ROOT, check=True,
                                    capture_output=True)
        self.assertEqual(result.stdout, EXPECTED)

    def test_native_report_and_build(self):
        returned = ROOT / "runs/vmctl-soe2n"
        self.assertIn(b"COMPILE=PASS", (returned / "SYOSTA.TXT").read_bytes())
        self.assertIn(b"LINK=PASS", (returned / "SYOSTA.TXT").read_bytes())
        self.assertEqual((returned / "SOE2NRES.TXT").read_bytes().replace(
            b"\r\n", b"\n"), HISTORICAL_NATIVE)


if __name__ == "__main__":
    unittest.main()
