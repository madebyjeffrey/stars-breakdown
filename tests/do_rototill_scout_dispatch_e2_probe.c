#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    char calls[8];
    StarsI16 count;
    StarsI16 transport;
    StarsI16 bad;
} Trace;

static void call(Trace *trace, char letter)
{
    if (trace->count >= 7)
        trace->bad = 1;
    else
        trace->calls[trace->count++] = letter;
}

static StarsI16 is_transport(void *context,
    const StarsRototillTurnFleet *fleet)
{
    Trace *trace = (Trace *)context;
    call(trace, 'I');
    if (fleet->id != 0 || fleet->owner != 1 || fleet->planet_id != 24 ||
            fleet->cord != 1 || fleet->ship_counts[0] != 1)
        trace->bad = 1;
    return trace->transport;
}

static StarsI16 select_fleet(void *context,
    const StarsRototillTurnFleet *fleet, StarsI16 player)
{
    Trace *trace = (Trace *)context;
    call(trace, 'S');
    if (player != 1 || fleet->id != 0 ||
            fleet->raw_fleet_record[5] != 0x20U)
        trace->bad = 1;
    return 1;
}

static StarsI16 refresh_fleet(void *context,
    const StarsRototillTurnFleet *fleet, StarsI16 player)
{
    Trace *trace = (Trace *)context;
    call(trace, 'R');
    if (player != 1 || fleet->raw_plord[10] != 0xa5U ||
            fleet->raw_plord[3] != 1U)
        trace->bad = 1;
    return 1;
}

static StarsI16 target_scout(void *context,
    const StarsRototillTurnFleet *fleet, StarsI16 player)
{
    Trace *trace = (Trace *)context;
    call(trace, 'T');
    if (player != 1 || fleet->id != 0 ||
            fleet->raw_fleet_record[92] != 44U ||
            fleet->raw_fleet_record[93] != 1U)
        trace->bad = 1;
    return 1;
}

static void seed(StarsU8 raw[124], StarsU8 plord[22])
{
    memset(raw, 0, 124U);
    memset(plord, 0, 22U);
    raw[2] = 1U;
    raw[5] = 0xa0U;
    raw[6] = 24U;
    raw[12] = 1U;
    raw[92] = 44U;
    raw[93] = 1U;
    raw[98] = 1U;
    plord[3] = 1U;
    plord[10] = 0xa2U;
}

static void check(StarsI16 condition, StarsI16 *checks,
    StarsI16 *failures)
{
    ++*checks;
    if (!condition)
        ++*failures;
}

int main(void)
{
    StarsRototillAiTurnInput input;
    StarsRototillAiTurnOps ops;
    StarsRototillTurnFleet fleet;
    StarsU8 raw[124];
    StarsU8 plord[22];
    Trace trace;
    StarsI16 checks = 0;
    StarsI16 failures = 0;

    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    memset(&fleet, 0, sizeof fleet);
    memset(&trace, 0, sizeof trace);
    seed(raw, plord);
    fleet.raw_fleet_record = raw;
    fleet.raw_fleet_record_size = sizeof raw;
    fleet.raw_plord = plord;
    fleet.raw_plord_bytes = sizeof plord;
    input.player_id = 1;
    input.fleet_count = 1;
    input.fleets = &fleet;
    input.fleet_scout_bound = 1;
    ops.context = &trace;
    ops.is_ai_transport = is_transport;
    ops.select_fleet = select_fleet;
    ops.refresh_fleet = refresh_fleet;
    ops.target_scout = target_scout;
    input.ops = &ops;

    check(stars_do_rototill_ai_turn(&input) == 1, &checks, &failures);
    check(strcmp(trace.calls, "IST") == 0 && !trace.bad,
        &checks, &failures);
    check(raw[5] == 0x20U && plord[10] == 0xa2U,
        &checks, &failures);

    seed(raw, plord);
    memset(&trace, 0, sizeof trace);
    trace.transport = 1;
    check(stars_do_rototill_ai_turn(&input) == 1, &checks, &failures);
    check(strcmp(trace.calls, "I") == 0 && !trace.bad,
        &checks, &failures);

    seed(raw, plord);
    memset(&trace, 0, sizeof trace);
    raw[92] = 1U;
    raw[93] = 0U;
    input.ship0_first_hull_slot_byte = 1U;
    check(stars_do_rototill_ai_turn(&input) == 1, &checks, &failures);
    check(strcmp(trace.calls, "ISSR") == 0 && !trace.bad,
        &checks, &failures);
    check(plord[10] == 0xa5U && plord[3] == 1U,
        &checks, &failures);

    seed(raw, plord);
    memset(&trace, 0, sizeof trace);
    input.ship0_first_hull_slot_byte = 0U;
    fleet.raw_plord_bytes = 4U;
    check(stars_do_rototill_ai_turn(&input) == 0, &checks, &failures);
    check(trace.count == 0 && raw[5] == 0xa0U,
        &checks, &failures);

#ifndef TARGET_NATIVE
    printf("ROTOTILL_SCOUT_E2_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures ? "FAIL" : "PASS");
#endif
    return failures != 0;
}
