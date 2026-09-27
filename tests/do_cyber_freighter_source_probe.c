#include "core.h"
#include <stdio.h>

typedef struct Trace {
    StarsI16 transfers;
    StarsI16 moves;
    StarsI16 last_amount;
    StarsI16 target_calls;
} Trace;

static StarsI16 lookup_planet(void *context, StarsI16 id,
    StarsCyberFreighterPlanet *planet)
{
    (void)context;
    if (id == -1)
        return 0;
    planet->id = (StarsU16)id;
    planet->owner = 1;
    planet->status_flags = 0;
    planet->point.x = 10;
    planet->point.y = 10;
    planet->mineral_total = 5;
    planet->mining_rate = 1;
    planet->mineral_stock = 5000;
    return 1;
}

static StarsI16 find_target(void *context,
    const StarsCyberFreighterPlanet *source, StarsI16 kind,
    StarsCyberFreighterPlanet *target)
{
    Trace *trace = (Trace *)context;
    (void)source;
    ++trace->target_calls;
    if (kind != 1)
        return 0;
    target->id = 8;
    target->owner = 1;
    target->point.x = 20;
    target->point.y = 20;
    return 1;
}

static void transfer_supply(void *context, StarsI16 player,
    StarsI16 planet, StarsI16 cargo_type, StarsI16 resource_type,
    StarsI32 amount)
{
    Trace *trace = (Trace *)context;
    (void)player;
    (void)planet;
    (void)cargo_type;
    (void)resource_type;
    trace->last_amount = (StarsI16)amount;
    ++trace->transfers;
}

static StarsI16 move_fleet(void *context, StarsCyberFreighterFleet *fleet,
    StarsU16 kind, StarsU16 target, StarsPoint point, StarsU16 flags)
{
    Trace *trace = (Trace *)context;
    (void)fleet;
    (void)kind;
    (void)target;
    (void)point;
    (void)flags;
    ++trace->moves;
    return 1;
}

int main(void)
{
    StarsCyberFreighterFleet fleet = { 3, 2, { 0, 0 }, 2, { 0, 0, 0 }, 0 };
    StarsCyberFreighterPlanState plan = { 0 };
    StarsDoCyberFreighterOps ops;
    Trace trace = { 0, 0, 0, 0 };
    int failures = 0;

    ops.lookup_planet = lookup_planet;
    ops.find_nearest = 0;
    ops.find_target = find_target;
    ops.race_stat = 0;
    ops.transfer_supply = transfer_supply;
    ops.refresh_fleet = 0;
    ops.move_to_starbase = 0;
    ops.ideal_warp = 0;
    ops.move_fleet = move_fleet;
    ops.context = &trace;
    stars_do_cyber_freighter(&fleet, &plan, 1, &ops);
    if (trace.transfers != 1 || trace.last_amount != 1000 ||
            trace.moves != 1 || trace.target_calls != 1)
        ++failures;

    fleet.planet_id = -1;
    ops.find_nearest = 0;
    stars_do_cyber_freighter(&fleet, &plan, 1, &ops);
    if (trace.moves != 1)
        ++failures;
    printf("DO_CYBER_FREIGHTER_CHECKS=2 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
