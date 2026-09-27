#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsU8 *first;
    StarsU8 *second;
    StarsU8 *foreign;
    StarsI16 visit_count;
    StarsI16 observed_all_cleared;
} Trace;

static StarsI16 visit(void *context, const StarsAutomitronTurnFleet *fleet,
    StarsI16 potency)
{
    Trace *trace;
    (void)fleet;
    (void)potency;
    trace = (Trace *)context;
    ++trace->visit_count;
    if ((trace->first[5] & 0x80U) == 0U &&
            (trace->second[5] & 0x80U) == 0U &&
            (trace->foreign[5] & 0x80U) != 0U)
        ++trace->observed_all_cleared;
    return 1;
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
    StarsAutomitronAiTurnInput input;
    StarsAutomitronAiTurnOps ops;
    StarsAutomitronTurnFleet fleets[3];
    StarsU8 records[3][124];
    Trace trace;
    StarsI16 checks;
    StarsI16 failures;
    StarsI16 index;

    checks = 0;
    failures = 0;
    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    memset(fleets, 0, sizeof fleets);
    memset(records, 0, sizeof records);
    memset(&trace, 0, sizeof trace);
    for (index = 0; index < 3; ++index) {
        fleets[index].owner = 4;
        fleets[index].raw_fleet_record = records[index];
        fleets[index].raw_fleet_size = 124;
        records[index][4] = 0x67U;
        records[index][5] = 0xabU;
    }
    fleets[1].owner = 5;
    trace.first = records[0];
    trace.foreign = records[1];
    trace.second = records[2];
    ops.visit_fleet = visit;
    ops.context = &trace;
    input.player_id = 4;
    input.fleet_count = 3;
    input.fleets = fleets;
    input.ops = &ops;

    check(stars_do_automitron_ai_turn(&input), &checks, &failures);
    check(records[0][4] == 0x67U && records[0][5] == 0x2bU,
        &checks, &failures);
    check(records[1][4] == 0x67U && records[1][5] == 0xabU,
        &checks, &failures);
    check(records[2][4] == 0x67U && records[2][5] == 0x2bU,
        &checks, &failures);
    check(trace.visit_count == 2 && trace.observed_all_cleared == 2,
        &checks, &failures);

    records[0][5] = 0xabU;
    records[2][5] = 0xabU;
    fleets[2].raw_fleet_size = 123;
    memset(&trace, 0, sizeof trace);
    trace.first = records[0];
    trace.foreign = records[1];
    trace.second = records[2];
    check(!stars_do_automitron_ai_turn(&input), &checks, &failures);
    check(records[0][5] == 0xabU && records[2][5] == 0xabU,
        &checks, &failures);
    check(trace.visit_count == 0, &checks, &failures);

    printf("AUTOMITRON_FLEET_FLAGS_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures ? "FAIL" : "PASS");
    return failures ? 1 : 0;
}
