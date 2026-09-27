#include <stdio.h>
#include "core.h"

typedef struct Trace { StarsI16 calls; } Trace;

static StarsU16 category(void *value, StarsI16 owner, StarsU16 design)
{
    Trace *trace = (Trace *)value;
    trace->calls++;
    (void)owner;
    return (design == 2U) ? 2U : 4U;
}

int main(void)
{
    StarsBattleUnitToken tokens[5];
    StarsBattleUnitsInput input;
    Trace trace;

    tokens[0].owner = 1; tokens[0].design = 2; tokens[0].battle_units = 100UL;
    tokens[1].owner = 2; tokens[1].design = 3; tokens[1].battle_units = 200UL;
    tokens[2].owner = 1; tokens[2].design = 20; tokens[2].battle_units = 400UL;
    tokens[3].owner = 1; tokens[3].design = 4; tokens[3].battle_units = 800UL;
    tokens[4].owner = 2; tokens[4].design = 5; tokens[4].battle_units = 1600UL;
    input.local_player = 1;
    input.tokens = tokens;
    input.token_count = 5;
    trace.calls = 0;
    if (stars_c_battle_units(&input, 1U | 0x10U, category, &trace) !=
        100UL || trace.calls != 2)
        return 1;
    trace.calls = 0;
    if (stars_c_battle_units(&input, 2U | 4U | 0xf8U, category, &trace) !=
        1800UL || trace.calls != 0)
        return 2;
    puts("C_BATTLE_UNITS_SOURCE=PASS");
    return 0;
}
