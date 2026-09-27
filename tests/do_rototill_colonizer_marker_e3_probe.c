#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsU8 *ai;
    StarsI16 target;
    StarsI16 bad;
    char calls[16];
    StarsI16 count;
} Trace;

static void record(Trace *trace, char value)
{
    if (trace->count >= 15)
        trace->bad = 1;
    else {
        trace->calls[trace->count++] = value;
        trace->calls[trace->count] = '\0';
    }
}

static StarsI16 select_fleet(void *context,
    const StarsRototillTurnFleet *fleet, StarsI16 player)
{
    Trace *trace = (Trace *)context;
    record(trace, 'S');
    if (player != 4 || fleet->id != 3)
        trace->bad = 1;
    return 1;
}

static StarsI16 nearest(void *context,
    const StarsRototillTurnFleet *fleet, StarsI16 player)
{
    Trace *trace = (Trace *)context;
    record(trace, 'N');
    if (player != 4 || fleet->id != 3 || strcmp(trace->calls, "SN") != 0)
        trace->bad = 1;
    return trace->target;
}

static StarsI16 transfer(void *context,
    const StarsRototillTurnFleet *fleet, StarsI16 player)
{
    Trace *trace = (Trace *)context;
    record(trace, 'X');
    if (player != 4 || fleet->id != 3 || strcmp(trace->calls, "SNSX") != 0)
        trace->bad = 1;
    return 1;
}

static StarsI16 refresh(void *context,
    const StarsRototillTurnFleet *fleet, StarsI16 player)
{
    Trace *trace = (Trace *)context;
    record(trace, 'R');
    if (player != 4 || fleet->id != 3 || strcmp(trace->calls, "SNSXR") != 0)
        trace->bad = 1;
    return 1;
}

static void colonize(void *context,
    const StarsRototillTurnFleet *fleet, StarsI16 target)
{
    Trace *trace = (Trace *)context;
    record(trace, 'C');
    if (fleet->id != 3 || target != 7 ||
            strcmp(trace->calls, "SNSXRC") != 0 ||
            trace->ai[7U * 16U + 15U] != 0xa1U)
        trace->bad = 1;
}

static void check(StarsI16 yes, StarsI16 *checks, StarsI16 *failures)
{
    ++*checks;
    if (!yes) {
#ifndef TARGET_NATIVE
        fprintf(stderr, "failed check %d\n", *checks);
#endif
        ++*failures;
    }
}

int main(void)
{
    StarsRototillAiTurnInput input;
    StarsRototillAiTurnOps ops;
    StarsRototillTurnFleet fleet;
    StarsRototillTurnPlanet planet;
    StarsPoint points[12];
    StarsU8 raw_fleet[124];
    StarsU8 raw_planet[56];
    StarsU8 plord[58];
    StarsU8 saved_plord[58];
    StarsU8 ai[12U * 16U];
    StarsU8 saved_ai[12U * 16U];
    Trace trace;
    StarsI16 checks = 0;
    StarsI16 failures = 0;

    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    memset(&fleet, 0, sizeof fleet);
    memset(&planet, 0, sizeof planet);
    memset(&trace, 0, sizeof trace);
    memset(points, 0, sizeof points);
    memset(raw_fleet, 0, sizeof raw_fleet);
    memset(raw_planet, 0, sizeof raw_planet);
    memset(plord, 0x5a, sizeof plord);
    memset(ai, 0x71, sizeof ai);
    raw_fleet[0] = 3U;
    raw_fleet[2] = 4U;
    raw_fleet[6] = 10U;
    raw_fleet[14] = 1U;
    raw_fleet[98] = 1U;
    raw_planet[0] = 10U;
    raw_planet[2] = 4U;
    plord[3] = 1U;
    ai[7U * 16U + 15U] = 0xa1U;
    memcpy(saved_ai, ai, sizeof ai);
    memcpy(saved_plord, plord, sizeof plord);
    fleet.id = 3;
    fleet.raw_fleet_record = raw_fleet;
    fleet.raw_fleet_record_size = sizeof raw_fleet;
    fleet.raw_plord = plord;
    fleet.raw_plord_bytes = sizeof plord;
    planet.id = 10;
    planet.raw_planet_record = raw_planet;
    planet.raw_planet_record_size = sizeof raw_planet;
    ops.context = &trace;
    ops.select_fleet = select_fleet;
    ops.nearest_colonizable = nearest;
    ops.transfer_supply = transfer;
    ops.refresh_fleet = refresh;
    ops.colonize_fleet = colonize;
    input.turn = 20;
    input.player_id = 4;
    input.planet_count = 1;
    input.planets = &planet;
    input.fleet_count = 1;
    input.fleets = &fleet;
    input.ops = &ops;
    input.raw_ai_planet_table = ai;
    input.raw_ai_planet_record_count = 12U;
    input.planet_points = points;
    input.planet_point_count = 12U;
    input.fleet_colonizer_supply_bound = 1;
    input.fleet_colonizer_marker_bound = 1;
    trace.ai = ai;
    trace.target = 7;

    check(stars_do_rototill_ai_turn(&input) != 0, &checks, &failures);
    check(strcmp(trace.calls, "SNSXRC") == 0 && !trace.bad,
        &checks, &failures);
    check(ai[7U * 16U + 15U] == 4U, &checks, &failures);
    saved_ai[7U * 16U + 15U] = 4U;
    saved_ai[10U * 16U + 9U] = 1U;
    check(memcmp(ai, saved_ai, sizeof ai) == 0,
        &checks, &failures);
    check(memcmp(plord, saved_plord, sizeof plord) == 0,
        &checks, &failures);

    memset(&trace, 0, sizeof trace);
    trace.ai = ai;
    trace.target = -1;
    ai[7U * 16U + 15U] = 0xa1U;
    check(stars_do_rototill_ai_turn(&input) != 0 &&
        strcmp(trace.calls, "SNSXR") == 0 &&
        ai[7U * 16U + 15U] == 0xa1U && !trace.bad,
        &checks, &failures);

    memset(&trace, 0, sizeof trace);
    trace.ai = ai;
    trace.target = 12;
    check(stars_do_rototill_ai_turn(&input) == 0 &&
        strcmp(trace.calls, "SN") == 0 &&
        ai[7U * 16U + 15U] == 0xa1U && !trace.bad,
        &checks, &failures);

    ops.colonize_fleet = 0;
    memset(&trace, 0, sizeof trace);
    trace.ai = ai;
    trace.target = 7;
    check(stars_do_rototill_ai_turn(&input) == 0 &&
        trace.count == 0 && ai[7U * 16U + 15U] == 0xa1U,
        &checks, &failures);
    ops.colonize_fleet = colonize;

    raw_fleet[2] = 5U;
    memset(&trace, 0, sizeof trace);
    trace.ai = ai;
    trace.target = 7;
    check(stars_do_rototill_ai_turn(&input) != 0 &&
        trace.count == 0 && ai[7U * 16U + 15U] == 0xa1U,
        &checks, &failures);
    raw_fleet[2] = 4U;

#ifndef TARGET_NATIVE
    printf("ROTOTILL_COLONIZER_MARKER_E3_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures ? "FAIL" : "PASS");
#endif
    return failures ? 1 : 0;
}
