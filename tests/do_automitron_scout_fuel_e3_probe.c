#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 refreshes;
    StarsI16 plans;
    StarsI16 visits;
    StarsI16 exact_refresh;
} Trace;

static StarsI16 refresh(void *context,
    const StarsAutomitronTurnFleet *fleet)
{
    Trace *trace;
    trace = (Trace *)context;
    ++trace->refreshes;
    if (fleet->raw_plord[10] == 0xb5U &&
            fleet->raw_plord[11] == 0xa5U)
        ++trace->exact_refresh;
    return 1;
}

static StarsI16 plan(void *context, const StarsAutomitronTurnFleet *fleet,
    const StarsAutomitronFleetPolicy *policy, StarsI16 turn,
    StarsAutomitronFleetPlan *out)
{
    Trace *trace;
    (void)fleet; (void)policy; (void)turn; (void)out;
    trace = (Trace *)context;
    ++trace->plans;
    return 1;
}

static StarsI16 visit(void *context, const StarsAutomitronTurnFleet *fleet,
    const StarsAutomitronFleetPolicy *policy, StarsI16 potency)
{
    Trace *trace;
    (void)fleet; (void)policy; (void)potency;
    trace = (Trace *)context;
    ++trace->visits;
    return 1;
}

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
    StarsAutomitronTurnFleet fleet;
    StarsU8 raw[STARS_AI_PLORD_HEADER_SIZE + STARS_AI_PLORD_ORDER_SIZE];
    Trace trace;
    StarsI16 checks;
    StarsI16 failures;

    checks = 0;
    failures = 0;
    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    memset(&fleet, 0, sizeof fleet);
    memset(raw, 0x5a, sizeof raw);
    memset(&trace, 0, sizeof trace);
    raw[3] = 1;
    raw[10] = 0xb0U;
    raw[11] = 0xa5U;
    fleet.owner = 4;
    fleet.cord = 1;
    fleet.ship_counts[0] = 1;
    fleet.fuel = 1;
    fleet.raw_plord = raw;
    fleet.raw_plord_bytes = (StarsU16)sizeof raw;
    ops.refresh_packed_order = refresh;
    ops.plan_fleet_action = plan;
    ops.visit_fleet_typed = visit;
    ops.context = &trace;
    input.player_id = 4;
    input.fleet_count = 1;
    input.fleets = &fleet;
    input.ops = &ops;
    input.scout_fuel_gate_bound = 1;
    input.hull0_first_slot_byte = 9;

    check(stars_do_automitron_ai_turn(&input), &checks, &failures);
    check(raw[10] == 0xb5U && raw[11] == 0xa5U,
        &checks, &failures);
    check(trace.refreshes == 1 && trace.exact_refresh == 1 &&
        trace.plans == 0 && trace.visits == 0,
        &checks, &failures);

    raw[10] = 0xb0U;
    memset(&trace, 0, sizeof trace);
    fleet.fuel = 2;
    check(stars_do_automitron_ai_turn(&input), &checks, &failures);
    check(raw[10] == 0xb0U && trace.refreshes == 0 &&
        trace.plans == 1 && trace.visits == 1,
        &checks, &failures);

    memset(&trace, 0, sizeof trace);
    fleet.fuel = 1;
    input.hull0_first_slot_byte = 10;
    check(stars_do_automitron_ai_turn(&input), &checks, &failures);
    check(raw[10] == 0xb0U && trace.refreshes == 0 &&
        trace.plans == 1, &checks, &failures);

    memset(&trace, 0, sizeof trace);
    input.hull0_first_slot_byte = 9;
    fleet.is_ai_transport = 1;
    check(stars_do_automitron_ai_turn(&input), &checks, &failures);
    check(raw[10] == 0xb0U && trace.refreshes == 0 &&
        trace.plans == 1, &checks, &failures);

    memset(&trace, 0, sizeof trace);
    fleet.is_ai_transport = 0;
    fleet.ship_counts[2] = 1;
    check(stars_do_automitron_ai_turn(&input), &checks, &failures);
    check(raw[10] == 0xb0U && trace.refreshes == 0 &&
        trace.plans == 1, &checks, &failures);

    memset(&trace, 0, sizeof trace);
    fleet.ship_counts[2] = 0;
    fleet.raw_plord_bytes = 21;
    check(!stars_do_automitron_ai_turn(&input), &checks, &failures);
    check(raw[10] == 0xb0U && trace.refreshes == 0 &&
        trace.plans == 0, &checks, &failures);

    memset(&trace, 0, sizeof trace);
    fleet.raw_plord_bytes = (StarsU16)sizeof raw;
    fleet.owner = 5;
    check(stars_do_automitron_ai_turn(&input), &checks, &failures);
    check(raw[10] == 0xb0U && trace.refreshes == 0 &&
        trace.plans == 0 && trace.visits == 0,
        &checks, &failures);

    printf("AUTOMITRON_SCOUT_FUEL_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures ? "FAIL" : "PASS");
    return failures ? 1 : 0;
}
