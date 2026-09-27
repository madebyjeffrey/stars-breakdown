#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    char events[8];
    StarsI16 count;
    StarsI16 bad;
    StarsI16 transport_result;
    StarsU8 *raw;
} Trace;

static void event(Trace *trace, char code)
{
    if (trace->count < 7)
        trace->events[trace->count++] = code;
}

static void check(StarsI16 yes, StarsI16 *checks, StarsI16 *failures)
{
    ++*checks;
    if (!yes) {
        ++*failures;
        fprintf(stderr, "failed check %d\n", *checks);
    }
}

static void word(StarsU8 *raw, StarsI16 offset, StarsU16 value)
{
    raw[offset] = (StarsU8)value;
    raw[offset + 1] = (StarsU8)(value >> 8);
}

static StarsI16 plan(void *opaque, const StarsMacintiTurnFleet *fleet,
    const StarsMacintiFleetPolicy *policy, StarsI16 turn,
    StarsMacintiFleetPlan *choice)
{
    Trace *trace;
    trace = (Trace *)opaque;
    event(trace, 'P');
    if (turn != 50 || fleet->id != 7 || fleet->owner != 2 ||
            fleet->planet_id != -1 || fleet->point.x != 10 ||
            fleet->point.y != 20 || fleet->ship_counts[1] != 300 ||
            fleet->ship_counts[2] != 2 ||
            policy->transport_ships != 300 || policy->armed_ships != 2 ||
            !policy->needs_order_repair ||
            (fleet->raw_plord[10] & 0x0fU) != 6U)
        trace->bad = 1;
    choice->action = STARS_MACINTI_FLEET_ACTION_CLEAR_TASK;
    return 1;
}

static StarsI16 clear(void *opaque, const StarsMacintiTurnFleet *fleet,
    const StarsMacintiFleetPlan *choice)
{
    Trace *trace;
    trace = (Trace *)opaque;
    event(trace, 'A');
    if (fleet->ship_counts[1] != 300 ||
            choice->action != STARS_MACINTI_FLEET_ACTION_CLEAR_TASK)
        trace->bad = 1;
    return 1;
}

static StarsI16 is_transport(void *opaque,
    const StarsMacintiTurnFleet *fleet, StarsI16 *answer)
{
    Trace *trace;
    trace = (Trace *)opaque;
    event(trace, 'T');
    if (fleet->owner != 2 || fleet->cord != 1)
        trace->bad = 1;
    *answer = trace->transport_result;
    return 1;
}

static StarsI16 set_plan(void *opaque,
    const StarsMacintiTurnFleet *fleet, StarsU8 plan_value)
{
    Trace *trace;
    trace = (Trace *)opaque;
    event(trace, 'L');
    if (fleet->plan != 0U || plan_value != 4U)
        trace->bad = 1;
    trace->raw[96] = plan_value;
    return 1;
}

static StarsI16 route(void *opaque, const StarsMacintiTurnFleet *fleet,
    const StarsMacintiFleetPolicy *policy, const StarsU8 potency[4])
{
    Trace *trace;
    (void)potency;
    trace = (Trace *)opaque;
    event(trace, 'R');
    if (fleet->ship_counts[1] != 300 || policy->transport_ships != 300 ||
            fleet->order_kind != 6 || fleet->plan != 4U)
        trace->bad = 1;
    return 1;
}

static StarsI16 finalize(void *opaque, const StarsMacintiTurnFleet *fleet,
    const StarsMacintiFleetPolicy *policy, const StarsU8 potency[4])
{
    Trace *trace;
    (void)potency;
    trace = (Trace *)opaque;
    event(trace, 'F');
    if (fleet->ship_counts[1] != 300 || policy->transport_ships != 300 ||
            fleet->plan != 4U)
        trace->bad = 1;
    return 1;
}

int main(void)
{
    StarsMacintiAiTurnInput input;
    StarsMacintiAiTurnOps ops;
    StarsMacintiTurnFleet fleet;
    StarsU8 raw[124];
    StarsU8 plord[40];
    StarsU8 ai[4][16];
    Trace trace;
    StarsI16 checks;
    StarsI16 failures;

    checks = 0;
    failures = 0;
    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    memset(&fleet, 0, sizeof fleet);
    memset(raw, 0, sizeof raw);
    memset(plord, 0, sizeof plord);
    memset(ai, 0, sizeof ai);
    memset(&trace, 0, sizeof trace);
    trace.transport_result = 1;
    trace.raw = raw;
    word(raw, 0, 7);
    word(raw, 2, 2);
    word(raw, 6, 0xffffU);
    word(raw, 8, 10);
    word(raw, 10, 20);
    word(raw, 14, 300);
    word(raw, 16, 2);
    word(raw, 98, 1);
    plord[3] = 1;
    word(plord, 10, 0xabc0U);
    fleet.id = 7;
    fleet.owner = 3;
    fleet.raw_record = raw;
    fleet.raw_record_size = sizeof raw;
    fleet.raw_plord = plord;
    fleet.raw_plord_size = sizeof plord;
    ops.plan_fleet_action = plan;
    ops.clear_task = clear;
    ops.is_ai_transport = is_transport;
    ops.set_transport_plan = set_plan;
    ops.route_transport_fleet = route;
    ops.finalize_fleet = finalize;
    ops.context = &trace;
    input.turn = 50;
    input.player_id = 2;
    input.fleet_count = 1;
    input.fleets = &fleet;
    input.planet_table_bound = 1;
    input.game_planet_max = 4;
    input.raw_ai_planet_table = &ai[0][0];
    input.raw_ai_planet_record_count = 4;
    input.ops = &ops;

    check(stars_do_macinti_ai_turn(&input), &checks, &failures);
    check(strcmp(trace.events, "PATLRF") == 0 && !trace.bad &&
        input.typed_fleets_visited == 1 &&
        input.typed_transport_routes == 1 &&
        input.typed_fleet_finalizations == 1 &&
        input.typed_fleet_actions == 1 &&
        plord[10] == 0xc6U && plord[11] == 0xabU,
        &checks, &failures);
    check(fleet.owner == 3 && fleet.ship_counts[1] == 0 &&
        fleet.point.x == 0, &checks, &failures);

    fleet.raw_record_size = 123;
    trace.count = 0;
    memset(trace.events, 0, sizeof trace.events);
    check(!stars_do_macinti_ai_turn(&input) && trace.count == 0,
        &checks, &failures);
    fleet.raw_record_size = 124;
    fleet.raw_plord_size = 21;
    check(!stars_do_macinti_ai_turn(&input) && trace.count == 0,
        &checks, &failures);
    fleet.raw_plord_size = 22;
    word(raw, 14, 0xffffU);
    check(!stars_do_macinti_ai_turn(&input) && trace.count == 0,
        &checks, &failures);
    word(raw, 14, 300);
    word(raw, 0, 8);
    check(!stars_do_macinti_ai_turn(&input) && trace.count == 0,
        &checks, &failures);
    word(raw, 0, 7);
    word(raw, 2, 3);
    check(stars_do_macinti_ai_turn(&input) && trace.count == 0 &&
        input.typed_fleets_visited == 0 &&
        input.typed_transport_routes == 0,
        &checks, &failures);

    word(raw, 2, 2);
    word(raw, 98, 2);
    fleet.raw_plord_size = sizeof plord;
    plord[3] = 2;
    ops.plan_fleet_action = 0;
    trace.count = 0;
    memset(trace.events, 0, sizeof trace.events);
    check(stars_do_macinti_ai_turn(&input) &&
        strcmp(trace.events, "F") == 0 &&
        input.typed_transport_routes == 0,
        &checks, &failures);
    word(raw, 98, 1);
    plord[3] = 1;
    trace.transport_result = 0;
    trace.count = 0;
    memset(trace.events, 0, sizeof trace.events);
    check(stars_do_macinti_ai_turn(&input) &&
        strcmp(trace.events, "TF") == 0 &&
        input.typed_transport_routes == 0,
        &checks, &failures);
    trace.transport_result = 1;
    trace.count = 0;
    memset(trace.events, 0, sizeof trace.events);
    check(stars_do_macinti_ai_turn(&input) &&
        strcmp(trace.events, "TRF") == 0 &&
        input.typed_transport_routes == 1,
        &checks, &failures);
    ops.is_ai_transport = 0;
    trace.count = 0;
    memset(trace.events, 0, sizeof trace.events);
    check(!stars_do_macinti_ai_turn(&input) && trace.count == 0,
        &checks, &failures);

    printf("MACINTI_PACKED_FLEET_E2_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures ? "FAIL" : "PASS");
    return failures ? 1 : 0;
}
