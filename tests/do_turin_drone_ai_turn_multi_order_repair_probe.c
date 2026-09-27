#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct RepairTrace {
    StarsI16 repairs;
    StarsI16 visits;
    StarsI16 events[3];
    StarsI16 event_count;
    StarsI16 reject;
} RepairTrace;

static int checks;
static int failures;

static void check_at(StarsI16 condition, int line)
{
    ++checks;
    if (!condition) {
        ++failures;
#ifndef TURIN_MULTI_NATIVE
        fprintf(stderr, "Turin multi-order check failed at line %d\n", line);
#else
        (void)line;
#endif
    }
}

#define check(condition) check_at((condition), __LINE__)

static void event(RepairTrace *trace, StarsI16 code)
{
    if (trace->event_count < 3)
        trace->events[trace->event_count] = code;
    ++trace->event_count;
}

static StarsI16 repair(void *opaque, const StarsTurinTurnFleet *fleet,
    const StarsTurinFleetPolicy *policy, const StarsU8 potency[4])
{
    RepairTrace *trace;
    trace = (RepairTrace *)opaque;
    ++trace->repairs;
    event(trace, 1);
    if (fleet->id != 2 || !policy->needs_order_repair ||
            policy->support_design_ships != 1 ||
            potency[0] != 4U || potency[2] != 8U)
        return 0;
    return (StarsI16)(!trace->reject);
}

static StarsI16 visit(void *opaque, const StarsTurinTurnFleet *fleet,
    const StarsTurinFleetPolicy *policy, const StarsU8 potency[4])
{
    RepairTrace *trace;
    (void)fleet;
    (void)policy;
    (void)potency;
    trace = (RepairTrace *)opaque;
    ++trace->visits;
    event(trace, 2);
    return 1;
}

static void seed_plord(StarsU8 *plord, StarsU16 length)
{
    StarsU16 i;
    for (i = 0; i < length; ++i)
        plord[i] = (StarsU8)(i * 7U + 1U);
    plord[3] = 3U;
    plord[10] = 0xb0U;
    plord[11] = 0xa5U;
}

int turin_multi_order_repair_probe(void)
{
    StarsU8 plord[STARS_AI_PLORD_HEADER_SIZE +
        STARS_AI_PLORD_ORDER_SIZE * 3U];
    StarsU8 expected[sizeof plord];
    StarsTurinTurnFleet fleet;
    StarsTurinDroneAiTurnOps ops;
    StarsTurinDroneAiTurnInput input;
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
    fleet.order_kind = 5; /* Raw PLORD is authoritative when present. */
    fleet.raw_plord = plord;
    fleet.ship_counts[12] = 1U;
    ops.repair_order = repair;
    ops.visit_fleet_typed = visit;
    ops.context = &trace;
    input.turn = 150;
    input.player_id = 4;
    input.fleet_count = 1;
    input.fleets = &fleet;
    input.ops = &ops;

    check(stars_do_turin_drone_ai_turn(&input));
    check(memcmp(plord, expected, sizeof plord) == 0);
    check(plord[3] == 3U);
    check(trace.repairs == 1 && trace.visits == 1 &&
        trace.event_count == 2 && trace.events[0] == 1 &&
        trace.events[1] == 2);

    seed_plord(plord, (StarsU16)sizeof plord);
    memset(&trace, 0, sizeof trace);
    plord[10] = 0xb5U;
    fleet.order_kind = 0; /* Stale typed value must not trigger repair. */
    check(stars_do_turin_drone_ai_turn(&input));
    check(plord[10] == 0xb5U && plord[3] == 3U && trace.repairs == 0);

    seed_plord(plord, (StarsU16)sizeof plord);
    memset(&trace, 0, sizeof trace);
    fleet.ship_counts[13] = 1U;
    check(stars_do_turin_drone_ai_turn(&input));
    check(plord[10] == 0xb0U && trace.repairs == 0);

    seed_plord(plord, (StarsU16)sizeof plord);
    memset(&trace, 0, sizeof trace);
    fleet.ship_counts[13] = 0U;
    fleet.cord = 2;
    check(stars_do_turin_drone_ai_turn(&input));
    check(plord[10] == 0xb0U && trace.repairs == 0);

    seed_plord(plord, (StarsU16)sizeof plord);
    memset(&trace, 0, sizeof trace);
    fleet.cord = 1;
    fleet.owner = 7;
    check(stars_do_turin_drone_ai_turn(&input));
    check(plord[10] == 0xb0U && trace.event_count == 0);

    seed_plord(plord, (StarsU16)sizeof plord);
    memset(&trace, 0, sizeof trace);
    fleet.owner = 4;
    ops.visit_fleet_typed = 0;
    check(stars_do_turin_drone_ai_turn(&input));
    check(memcmp(plord, expected, sizeof plord) == 0 &&
        trace.repairs == 1 && trace.visits == 0);

    seed_plord(plord, (StarsU16)sizeof plord);
    memset(&trace, 0, sizeof trace);
    ops.repair_order = 0;
    check(stars_do_turin_drone_ai_turn(&input));
    check(memcmp(plord, expected, sizeof plord) == 0 &&
        trace.event_count == 0);

    seed_plord(plord, (StarsU16)sizeof plord);
    memset(&trace, 0, sizeof trace);
    trace.reject = 1;
    ops.repair_order = repair;
    check(!stars_do_turin_drone_ai_turn(&input));
    check(memcmp(plord, expected, sizeof plord) == 0 &&
        trace.repairs == 1 && trace.visits == 0);

#ifndef TURIN_MULTI_NATIVE
    printf("TURIN_MULTI_ORDER_REPAIR_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures == 0 ? "PASS" : "FAIL");
#endif
    return failures == 0 ? 0 : 1;
}

#ifndef TURIN_MULTI_NATIVE
int main(void)
{
    return turin_multi_order_repair_probe();
}
#endif
