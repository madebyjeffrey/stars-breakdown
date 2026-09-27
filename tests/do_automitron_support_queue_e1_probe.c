#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 potent_calls;
    StarsI16 queue_calls;
    StarsI16 hull_calls;
    StarsI16 seen[3];
    StarsI16 queue_ok;
    StarsI16 fail_queue;
    StarsI16 suppress_design;
} Trace;

static void check(StarsI16 yes, StarsI16 *checks, StarsI16 *failures)
{
    ++*checks;
    if (!yes)
        ++*failures;
}

static StarsI16 design(void *opaque, StarsI16 first, StarsI16 last,
    StarsI16 budget, StarsI16 *selected)
{
    Trace *trace;
    trace = (Trace *)opaque;
    (void)last;
    (void)budget;
    *selected = first == 2 && !trace->suppress_design ? 7 : -1;
    return 1;
}

static StarsI16 potent(void *opaque, const StarsAutomitronTurnFleet *fleet,
    StarsI16 kind)
{
    Trace *trace;
    trace = (Trace *)opaque;
    if (trace->potent_calls < 3)
        trace->seen[trace->potent_calls] = fleet->id;
    ++trace->potent_calls;
    return kind == 2 && fleet->id != 3;
}

static StarsI16 queue(void *opaque, const StarsAutomitronTurnPlanet *planet,
    StarsI16 design_id, StarsI16 count, StarsI16 queue_kind, StarsI16 flag)
{
    Trace *trace;
    trace = (Trace *)opaque;
    ++trace->queue_calls;
    trace->queue_ok = planet->id == 4 && design_id == 7 && count == 4 &&
        queue_kind == 2 && flag == 1;
    return !trace->fail_queue;
}

static StarsI16 hull(void *opaque, const StarsAutomitronTurnPlanet *planet,
    StarsI16 design_id, StarsI16 count)
{
    Trace *trace;
    trace = (Trace *)opaque;
    (void)planet;
    (void)design_id;
    (void)count;
    ++trace->hull_calls;
    return 1;
}

int main(void)
{
    StarsAutomitronAiTurnInput input;
    StarsAutomitronAiTurnOps ops;
    StarsAutomitronTurnPlanet planet;
    StarsAutomitronTurnFleet fleets[5];
    Trace trace;
    StarsI16 checks;
    StarsI16 failures;

    checks = 0;
    failures = 0;
    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    memset(&planet, 0, sizeof planet);
    memset(fleets, 0, sizeof fleets);
    memset(&trace, 0, sizeof trace);
    planet.id = 4;
    planet.owner = 2;
    planet.visibility_flags = 0x0200U;
    planet.mineral_rate = 1;
    planet.support_queue_eligible = 1;
    planet.production_slot_open = 1;
    fleets[0].id = 1;
    fleets[0].owner = 3;
    fleets[0].planet_id = 4;
    fleets[1].id = 2;
    fleets[1].owner = 2;
    fleets[1].planet_id = 5;
    fleets[2].id = 3;
    fleets[2].owner = 2;
    fleets[2].planet_id = 4;
    fleets[3].id = 4;
    fleets[3].owner = 2;
    fleets[3].planet_id = 4;
    fleets[3].ship_counts[2] = 7;
    fleets[4].id = 5;
    fleets[4].owner = 2;
    fleets[4].planet_id = 4;
    fleets[4].ship_counts[2] = 10;
    ops.check_design_status = design;
    ops.is_potent_war_fleet = potent;
    ops.add_raw_queue_item = queue;
    ops.queue_production = hull;
    ops.context = &trace;
    input.turn = 150;
    input.player_id = 2;
    input.planet_count = 1;
    input.planets = &planet;
    input.fleet_count = 5;
    input.fleets = fleets;
    input.ops = &ops;

    check(stars_do_automitron_ai_turn(&input), &checks, &failures);
    check(input.armada_potency[2] == 8, &checks, &failures);
    check(trace.potent_calls == 2 && trace.seen[0] == 3 &&
        trace.seen[1] == 4, &checks, &failures);
    check(trace.queue_calls == 0 && input.support_design_queued == 0,
        &checks, &failures);
    check(trace.hull_calls == 0, &checks, &failures);

    memset(&trace, 0, sizeof trace);
    fleets[3].ship_counts[2] = 8;
    check(stars_do_automitron_ai_turn(&input), &checks, &failures);
    check(trace.potent_calls == 2 && trace.seen[1] == 4,
        &checks, &failures);
    check(trace.queue_calls == 1 && trace.queue_ok &&
        trace.hull_calls == 0, &checks, &failures);
    check(input.support_design_queued == 1 &&
        input.production_hulls_queued == 4 &&
        input.production_queued == 1, &checks, &failures);

    memset(&trace, 0, sizeof trace);
    trace.fail_queue = 1;
    check(!stars_do_automitron_ai_turn(&input), &checks, &failures);
    check(trace.queue_calls == 1 && trace.hull_calls == 0,
        &checks, &failures);

    memset(&trace, 0, sizeof trace);
    planet.support_queue_eligible = 0;
    check(stars_do_automitron_ai_turn(&input), &checks, &failures);
    check(trace.potent_calls == 0 && trace.queue_calls == 0,
        &checks, &failures);

    memset(&trace, 0, sizeof trace);
    planet.support_queue_eligible = 1;
    trace.suppress_design = 1;
    check(stars_do_automitron_ai_turn(&input), &checks, &failures);
    check(trace.potent_calls == 0 && trace.queue_calls == 0,
        &checks, &failures);

    printf("AUTOMITRON_SUPPORT_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures ? "FAIL" : "PASS");
    return failures ? 1 : 0;
}
