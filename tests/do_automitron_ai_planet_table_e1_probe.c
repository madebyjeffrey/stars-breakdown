#include "core.h"
#include <stdio.h>
#include <string.h>

static void check(StarsI16 yes, StarsI16 *checks, StarsI16 *failures)
{
    ++*checks;
    if (!yes)
        ++*failures;
}

int main(void)
{
    StarsAutomitronAiTurnInput input;
    StarsAutomitronAiTurnOps ops;
    StarsAutomitronTurnPlanet planets[3];
    StarsU8 table[4][16];
    StarsU8 override[16];
    StarsI16 checks;
    StarsI16 failures;

    checks = 0;
    failures = 0;
    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    memset(planets, 0, sizeof planets);
    memset(table, 0, sizeof table);
    memset(override, 0, sizeof override);
    planets[0].id = 3;
    planets[0].owner = 2;
    planets[0].desirability = -1;
    planets[1].id = 1;
    planets[1].owner = 4;
    planets[1].optimal_value = 1;
    planets[1].visibility_flags = 0x0200U;
    planets[2].id = 0;
    planets[2].owner = -1;
    planets[2].raw_ai_planet = override;
    planets[2].raw_ai_planet_size = 16;
    input.player_id = 2;
    input.planet_count = 3;
    input.planets = planets;
    input.ops = &ops;
    input.raw_ai_planet_table = &table[0][0];
    input.raw_ai_planet_record_count = 4;

    check(stars_do_automitron_ai_turn(&input), &checks, &failures);
    check(table[3][9] == 1 && table[3][2] == 1, &checks, &failures);
    check(table[1][9] == 1 && table[1][3] == 1 && table[1][10] == 2,
        &checks, &failures);
    check(table[0][9] == 0 && table[2][9] == 0 && override[9] == 1,
        &checks, &failures);
    memset(table, 0, sizeof table);
    memset(override, 0, sizeof override);
    planets[1].id = 4;
    check(!stars_do_automitron_ai_turn(&input), &checks, &failures);
    check(table[3][9] == 0 && override[9] == 0, &checks, &failures);
    planets[1].id = -1;
    check(!stars_do_automitron_ai_turn(&input), &checks, &failures);
    planets[1].id = 1;
    planets[2].raw_ai_planet_size = 15;
    check(!stars_do_automitron_ai_turn(&input), &checks, &failures);

    printf("AUTOMITRON_AI_TABLE_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures ? "FAIL" : "PASS");
    return failures ? 1 : 0;
}
