#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct RepairTrace {
    StarsI16 events[4];
    StarsI16 event_count;
    StarsI16 choose_route;
    StarsI16 repairs;
    StarsI16 applications;
    StarsI16 visits;
} RepairTrace;

static int checks;
static int failures;

static void check(StarsI16 condition)
{
    ++checks;
    if (!condition)
        ++failures;
}

static void event(RepairTrace *trace, StarsI16 code)
{
    if (trace->event_count < 4)
        trace->events[trace->event_count] = code;
    ++trace->event_count;
}

static StarsI16 plan(void *opaque, const StarsRobotoidTurnFleet *fleet,
    const StarsRobotoidFleetPolicy *policy, StarsI16 turn,
    StarsRobotoidFleetPlan *result)
{
    RepairTrace *trace;
    (void)fleet;
    (void)policy;
    (void)turn;
    trace = (RepairTrace *)opaque;
    event(trace, 1);
    if (trace->choose_route)
        result->action = STARS_ROBOTOID_FLEET_ACTION_MOVE;
    return 1;
}

static StarsI16 apply(void *opaque, const StarsRobotoidTurnFleet *fleet,
    const StarsRobotoidFleetPlan *plan_result)
{
    RepairTrace *trace;
    (void)fleet;
    trace = (RepairTrace *)opaque;
    ++trace->applications;
    event(trace, 4);
    return (StarsI16)(plan_result->action == STARS_ROBOTOID_FLEET_ACTION_MOVE);
}

static StarsI16 repair(void *opaque, const StarsRobotoidTurnFleet *fleet,
    const StarsRobotoidFleetPolicy *policy, const StarsU8 potency[4])
{
    RepairTrace *trace;
    (void)fleet;
    (void)policy;
    (void)potency;
    trace = (RepairTrace *)opaque;
    ++trace->repairs;
    event(trace, 2);
    return 1;
}

static StarsI16 visit(void *opaque, const StarsRobotoidTurnFleet *fleet,
    const StarsRobotoidFleetPolicy *policy, const StarsU8 potency[4])
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

static void seed_plord(StarsU8 *plord, StarsU16 length)
{
    StarsU16 i;
    for (i = 0; i < length; ++i)
        plord[i] = (StarsU8)(i * 13U + 7U);
    plord[3] = 3U;
    plord[10] = 0xf0U;
    plord[11] = 0xabU;
}

int robotoid_multi_order_repair_probe(void)
{
    StarsU8 plord[STARS_AI_PLORD_HEADER_SIZE +
        STARS_AI_PLORD_ORDER_SIZE * 3U];
    StarsU8 expected[sizeof plord];
    StarsRobotoidTurnFleet fleet;
    StarsRobotoidAiTurnOps ops;
    StarsRobotoidAiTurnInput input;
    RepairTrace trace;

    checks = failures = 0;
    memset(&fleet, 0, sizeof fleet);
    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    seed_plord(plord, (StarsU16)sizeof plord);
    memcpy(expected, plord, sizeof expected);
    expected[10] = 0xf6U;
    expected[12] = 5U;
    expected[13] = 0U;
    expected[14] = 5U;
    expected[15] = 0U;
    fleet.id = 2;
    fleet.owner = 4;
    fleet.cord = 1;
    fleet.order_kind = 0;
    fleet.raw_plord = plord;
    fleet.ship_counts[0] = 1U;
    ops.plan_fleet_action = plan;
    ops.move = apply;
    ops.repair_order = repair;
    ops.visit_fleet_typed = visit;
    ops.context = &trace;
    input.turn = 41;
    input.player_id = 4;
    input.fleet_count = 1;
    input.fleets = &fleet;
    input.ops = &ops;

    check(stars_do_robotoid_ai_turn(&input));
    check(plord[3] == 3U);
    check(memcmp(plord, expected, sizeof plord) == 0);
    check(trace.repairs == 1 && trace.applications == 0 && trace.visits == 1);
    check(trace.event_count == 3 && trace.events[0] == 1 &&
        trace.events[1] == 2 && trace.events[2] == 3);

    seed_plord(plord, (StarsU16)sizeof plord);
    memset(&trace, 0, sizeof trace);
    trace.choose_route = 1;
    check(stars_do_robotoid_ai_turn(&input));
    check(memcmp(plord, expected, sizeof plord) != 0 && plord[3] == 3U &&
        plord[10] == 0xf0U);
    check(trace.repairs == 0 && trace.applications == 1 && trace.visits == 1);
    check(trace.event_count == 3 && trace.events[0] == 1 &&
        trace.events[1] == 4 && trace.events[2] == 3);

    seed_plord(plord, (StarsU16)sizeof plord);
    memset(&trace, 0, sizeof trace);
    fleet.cord = 2;
    check(stars_do_robotoid_ai_turn(&input));
    check(plord[3] == 3U && plord[10] == 0xf0U && trace.repairs == 0);
    check(trace.event_count == 2 && trace.events[0] == 1 &&
        trace.events[1] == 3);

    seed_plord(plord, (StarsU16)sizeof plord);
    memset(&trace, 0, sizeof trace);
    fleet.cord = 1;
    fleet.owner = 9;
    check(stars_do_robotoid_ai_turn(&input));
    check(plord[3] == 3U && plord[10] == 0xf0U && trace.event_count == 0);

#ifndef ROBOTOID_MULTI_NATIVE
    printf("ROBOTOID_MULTI_ORDER_REPAIR_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures == 0 ? "PASS" : "FAIL");
#endif
    return failures == 0 ? 0 : 1;
}

#ifndef ROBOTOID_MULTI_NATIVE
int main(void)
{
    return robotoid_multi_order_repair_probe();
}
#endif
