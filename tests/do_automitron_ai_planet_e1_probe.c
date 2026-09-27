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
    StarsAutomitronTurnPlanet planets[5];
    StarsU8 records[5][16];
    StarsI16 checks;
    StarsI16 failures;
    StarsI16 i;

    checks = 0;
    failures = 0;
    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    memset(planets, 0, sizeof planets);
    memset(records, 0, sizeof records);
    for (i = 0; i < 5; ++i) {
        planets[i].id = i;
        planets[i].raw_ai_planet = records[i];
        planets[i].raw_ai_planet_size = 16;
    }
    planets[0].owner = 2;
    planets[0].desirability = -1;
    planets[1].owner = 2;
    planets[1].desirability = 4;
    planets[2].owner = -1;
    planets[2].optimal_value = 3;
    planets[3].owner = 1;
    planets[3].optimal_value = 5;
    planets[3].visibility_flags = 0x0200U;
    planets[4].owner = 1;
    planets[4].optimal_value = 0;
    input.player_id = 2;
    input.planet_count = 5;
    input.planets = planets;
    input.ops = &ops;
    check(stars_do_automitron_ai_turn(&input), &checks, &failures);
    check(input.owned_bad_count == 1, &checks, &failures);
    for (i = 0; i < 5; ++i)
        check(records[i][9] == 1, &checks, &failures);
    check(records[0][2] == 1 && records[0][3] == 0 &&
        records[0][10] == 0, &checks, &failures);
    check(records[1][2] == 0 && records[1][3] == 0 &&
        records[1][10] == 0, &checks, &failures);
    check(records[2][2] == 0 && records[2][3] == 0 &&
        records[2][10] == 0, &checks, &failures);
    check(records[3][2] == 0 && records[3][3] == 1 &&
        records[3][10] == 2, &checks, &failures);
    check(records[4][2] == 0 && records[4][3] == 0 &&
        records[4][10] == 1, &checks, &failures);

    memset(records, 0, sizeof records);
    planets[0].raw_ai_planet_size = 15;
    check(!stars_do_automitron_ai_turn(&input), &checks, &failures);
    check(records[0][9] == 0 && records[1][9] == 0,
        &checks, &failures);

    printf("AUTOMITRON_AI_PLANET_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures ? "FAIL" : "PASS");
    return failures ? 1 : 0;
}
