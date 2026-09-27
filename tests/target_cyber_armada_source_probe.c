#include "core.h"
#include <stdio.h>

typedef struct Trace {
    StarsI16 moves;
    StarsU16 last_kind;
    StarsU16 last_target;
    StarsI16 clears;
} Trace;

static StarsI16 lookup_planet(void *context, StarsU16 id,
    StarsTargetCyberPlanet *planet)
{
    (void)context;
    planet->id = id;
    planet->owner = id == 4 ? 1 : -1;
    planet->status_flags = 0;
    planet->turn_seen = 20;
    planet->point.x = 10;
    planet->point.y = 10;
    return 1;
}

static StarsI16 find_destination(void *context,
    const StarsTargetCyberPlanet *source, StarsI16 kind,
    StarsTargetCyberPlanet *destination)
{
    (void)context;
    (void)source;
    if (kind != 0)
        return 0;
    destination->id = 4;
    destination->point.x = 40;
    destination->point.y = 40;
    return 1;
}

static void move_fleet(void *context, StarsTargetCyberFleet *fleet,
    StarsU16 kind, StarsU16 target, StarsPoint point)
{
    Trace *trace = (Trace *)context;
    (void)fleet;
    (void)point;
    ++trace->moves;
    trace->last_kind = kind;
    trace->last_target = target;
}

static void clear_task(void *context, StarsTargetCyberFleet *fleet,
    StarsI16 reason)
{
    Trace *trace = (Trace *)context;
    (void)fleet;
    (void)reason;
    ++trace->clears;
}

static StarsI16 random_value(void *context, StarsI16 limit)
{
    (void)context;
    (void)limit;
    return 0;
}

int main(void)
{
    StarsTargetCyberFleet fleet;
    StarsTargetCyberArmadaOps ops;
    StarsI16 potency[4] = { 1, 1, 1, 1 };
    Trace trace = { 0, 0, 0, 0 };
    int failures = 0;

    fleet.id = 7;
    fleet.planet_id = 2;
    fleet.cord = 1;
    fleet.point.x = 0;
    fleet.point.y = 0;
    fleet.order_kind = 0;
    fleet.order_target = 0;
    fleet.order_point.x = 0;
    fleet.order_point.y = 0;
    fleet.ship_counts[6] = 100;
    fleet.ship_counts[7] = 0;
    fleet.ship_counts[8] = 0;
    fleet.ship_counts[9] = 1;
    fleet.ship_counts[13] = 0;
    ops.random_value = random_value;
    ops.lookup_planet = lookup_planet;
    ops.find_destination = find_destination;
    ops.find_enemy_fleet = 0;
    ops.move_fleet = move_fleet;
    ops.clear_task = clear_task;
    ops.select_object = 0;
    ops.move_nearest = 0;
    ops.mark_planet = 0;
    ops.context = &trace;
    stars_target_cyber_armada(&fleet, 1, 20, 2, potency, &ops);
    if (trace.moves != 1 || trace.last_kind != 1 || trace.last_target != 4)
        ++failures;

    fleet.planet_id = 4;
    fleet.ship_counts[6] = 0;
    stars_target_cyber_armada(&fleet, 1, 20, 2, potency, &ops);
    if (trace.clears != 0)
        ++failures;
    printf("TARGET_CYBER_ARMADA_CHECKS=2 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
