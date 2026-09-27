#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct RepairTrace {
    StarsI16 events[4];
    StarsI16 event_count;
    StarsI16 repairs;
    StarsI16 plans;
    StarsI16 visits;
    StarsI16 reject_repair;
} RepairTrace;

static int checks;
static int failures;

static void check_at(StarsI16 condition, int line)
{
    ++checks;
    if (!condition) {
        ++failures;
#ifndef AUTOMITRON_MULTI_NATIVE
        fprintf(stderr, "automitron multi-order check failed at line %d\n", line);
#else
        (void)line;
#endif
    }
}

#define check(condition) check_at((condition), __LINE__)

static void event(RepairTrace *trace, StarsI16 code)
{
    if (trace->event_count < 4)
        trace->events[trace->event_count] = code;
    ++trace->event_count;
}

static StarsI16 repair(void *opaque,
    const StarsAutomitronTurnFleet *fleet,
    const StarsAutomitronFleetPolicy *policy, const StarsU8 potency[4])
{
    RepairTrace *trace;
    trace = (RepairTrace *)opaque;
    ++trace->repairs;
    event(trace, 1);
    if (fleet->id != 2 || !policy->needs_order_repair ||
            policy->slot_6_ships != 1 || potency[0] != 4U)
        return 0;
    return (StarsI16)(!trace->reject_repair);
}

static StarsI16 plan(void *opaque,
    const StarsAutomitronTurnFleet *fleet,
    const StarsAutomitronFleetPolicy *policy, StarsI16 turn,
    StarsAutomitronFleetPlan *result)
{
    RepairTrace *trace;
    (void)fleet;
    (void)policy;
    (void)turn;
    (void)result;
    trace = (RepairTrace *)opaque;
    ++trace->plans;
    event(trace, 2);
    return 1;
}

static StarsI16 visit(void *opaque,
    const StarsAutomitronTurnFleet *fleet,
    const StarsAutomitronFleetPolicy *policy, StarsI16 potency)
{
    RepairTrace *trace;
    (void)fleet;
    (void)policy;
    (void)potency;
    trace = (RepairTrace *)opaque;
    ++trace->visits;
    event(trace, 3);
    return 1;
}

static StarsI16 visit_legacy(void *opaque,
    const StarsAutomitronTurnFleet *fleet, StarsI16 potency)
{
    RepairTrace *trace;
    (void)fleet;
    (void)potency;
    trace = (RepairTrace *)opaque;
    ++trace->visits;
    event(trace, 3);
    return 1;
}

static void seed_plord(StarsU8 *plord, StarsU16 length)
{
    StarsU16 i;
    for (i = 0; i < length; ++i)
        plord[i] = (StarsU8)(i * 11U + 3U);
    plord[3] = 3U;
    plord[10] = 0xb0U;
    plord[11] = 0xa5U;
}

int automitron_multi_order_repair_probe(void)
{
    StarsU8 plord[STARS_AI_PLORD_HEADER_SIZE +
        STARS_AI_PLORD_ORDER_SIZE * 3U];
    StarsU8 expected[sizeof plord];
    StarsAutomitronTurnFleet fleet;
    StarsAutomitronAiTurnOps ops;
    StarsAutomitronAiTurnInput input;
    RepairTrace trace;

    checks = failures = 0;
    memset(&fleet, 0, sizeof fleet);
    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    seed_plord(plord, (StarsU16)sizeof plord);
    memcpy(expected, plord, sizeof expected);
    expected[10] = 0xb6U;
    expected[12] = 5U;
    expected[13] = 0U;
    expected[14] = 5U;
    expected[15] = 0U;
    fleet.id = 2;
    fleet.owner = 4;
    fleet.cord = 1;
    fleet.order_kind = 0;
    fleet.raw_plord = plord;
    fleet.ship_counts[6] = 1U;
    ops.repair_order = repair;
    ops.plan_fleet_action = plan;
    ops.visit_fleet_typed = visit;
    ops.context = &trace;
    input.turn = 150;
    input.player_id = 4;
    input.fleet_count = 1;
    input.fleets = &fleet;
    input.ops = &ops;

    check(stars_do_automitron_ai_turn(&input));
    check(plord[3] == 3U);
    check(memcmp(plord, expected, sizeof plord) == 0);
    check(trace.repairs == 1 && trace.plans == 1 && trace.visits == 1);
    check(trace.event_count == 3 && trace.events[0] == 1 &&
        trace.events[1] == 2 && trace.events[2] == 3);

    seed_plord(plord, (StarsU16)sizeof plord);
    memset(&trace, 0, sizeof trace);
    ops.visit_fleet_typed = 0;
    ops.visit_fleet = visit_legacy;
    check(stars_do_automitron_ai_turn(&input));
    check(memcmp(plord, expected, sizeof plord) == 0);
    check(trace.repairs == 1 && trace.plans == 0 && trace.visits == 1 &&
        trace.event_count == 2 && trace.events[0] == 1 &&
        trace.events[1] == 3);

    seed_plord(plord, (StarsU16)sizeof plord);
    memset(&trace, 0, sizeof trace);
    ops.visit_fleet = 0;
    ops.repair_order = 0;
    check(stars_do_automitron_ai_turn(&input));
    check(memcmp(plord, expected, sizeof plord) == 0 &&
        trace.event_count == 0);
    ops.repair_order = repair;
    ops.visit_fleet_typed = visit;

    seed_plord(plord, (StarsU16)sizeof plord);
    memset(&trace, 0, sizeof trace);
    fleet.ship_counts[6] = 0U;
    fleet.ship_counts[2] = 1U;
    check(stars_do_automitron_ai_turn(&input));
    check(plord[3] == 3U && plord[10] == 0xb0U && trace.repairs == 0);
    check(trace.plans == 1 && trace.visits == 1);

    seed_plord(plord, (StarsU16)sizeof plord);
    memset(&trace, 0, sizeof trace);
    fleet.ship_counts[6] = 1U;
    fleet.cord = 2;
    check(stars_do_automitron_ai_turn(&input));
    check(plord[3] == 3U && plord[10] == 0xb0U && trace.repairs == 0);

    seed_plord(plord, (StarsU16)sizeof plord);
    memset(&trace, 0, sizeof trace);
    fleet.cord = 1;
    fleet.order_kind = 5;
    plord[10] = 0xb5U;
    check(stars_do_automitron_ai_turn(&input));
    check(plord[3] == 3U && plord[10] == 0xb5U && trace.repairs == 0);

    seed_plord(plord, (StarsU16)sizeof plord);
    memset(&trace, 0, sizeof trace);
    fleet.order_kind = 0;
    fleet.owner = 9;
    check(stars_do_automitron_ai_turn(&input));
    check(plord[3] == 3U && plord[10] == 0xb0U &&
        trace.event_count == 0);

    seed_plord(plord, (StarsU16)sizeof plord);
    memset(&trace, 0, sizeof trace);
    fleet.owner = 4;
    trace.reject_repair = 1;
    check(!stars_do_automitron_ai_turn(&input));
    check(memcmp(plord, expected, sizeof plord) == 0);
    check(trace.repairs == 1 && trace.plans == 0 && trace.visits == 0);

#ifndef AUTOMITRON_MULTI_NATIVE
    printf("AUTOMITRON_MULTI_ORDER_REPAIR_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures == 0 ? "PASS" : "FAIL");
#endif
    return failures == 0 ? 0 : 1;
}

#ifndef AUTOMITRON_MULTI_NATIVE
int main(void)
{
    return automitron_multi_order_repair_probe();
}
#endif
