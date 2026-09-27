#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 calls;
    StarsI16 valid;
} Trace;

static StarsI16 choose_task(void *opaque,
    const StarsMacintiTurnFleet *fleet,
    const StarsMacintiFleetPolicy *policy, StarsI16 turn)
{
    Trace *trace;
    (void)turn;
    trace = (Trace *)opaque;
    ++trace->calls;
    if (fleet->owner == 3 && policy->recommended_task ==
            STARS_MACINTI_TASK_ATTACK)
        ++trace->valid;
    return 1;
}

static StarsI16 visit_fleet(void *opaque,
    const StarsMacintiTurnFleet *fleet,
    const StarsMacintiFleetPolicy *policy, const StarsU8 potency[4])
{
    (void)opaque;
    (void)fleet;
    (void)policy;
    (void)potency;
    return 1;
}

int main(void)
{
    StarsMacintiAiTurnOps ops;
    StarsMacintiAiTurnInput input;
    StarsMacintiTurnFleet fleets[4];
    Trace trace;
    StarsI16 failures;

    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(fleets, 0, sizeof fleets);
    memset(&trace, 0, sizeof trace);
    ops.choose_fleet_task = choose_task;
    ops.visit_fleet_typed = visit_fleet;
    ops.context = &trace;
    fleets[0].owner = 3;
    fleets[0].cord = 1;
    fleets[0].order_kind = 1;
    fleets[0].ship_counts[2] = 2;
    fleets[1].owner = 3;
    fleets[1].ship_counts[1] = 2;
    fleets[2].owner = 3;
    fleets[2].ship_counts[0] = 1;
    fleets[2].cord = 1;
    fleets[2].order_kind = 0;
    fleets[3].owner = 3;
    fleets[3].cord = 1;
    input.turn = 150;
    input.player_id = 3;
    input.fleet_count = 4;
    input.fleets = fleets;
    input.ops = &ops;
    failures = !stars_do_macinti_ai_turn(&input);
    failures += trace.calls != 4 || trace.valid != 1;
    printf("MACINTI_AI_TASK_CHECKS=3 FAILURES=%d RESULT=%s\n",
        (int)failures, failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
}
