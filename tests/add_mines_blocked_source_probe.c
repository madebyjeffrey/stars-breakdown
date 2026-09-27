#include "core.h"
#include <stdio.h>

typedef struct MineProbe {
    int queue_calls;
    int mark_calls;
    StarsI16 last_type;
    StarsI16 last_amount;
} MineProbe;

static StarsI16 resources_at_planet(void *context, StarsU16 planet)
{
    (void)context;
    return planet == 1 ? 10 : 100;
}

static void add_queue_item(void *context, StarsU16 planet, StarsI16 type,
    StarsI16 amount)
{
    MineProbe *probe = (MineProbe *)context;
    (void)planet;
    ++probe->queue_calls;
    probe->last_type = type;
    probe->last_amount = amount;
}

static void mark_queue(void *context, StarsU16 planet, StarsI16 eta)
{
    MineProbe *probe = (MineProbe *)context;
    (void)planet;
    (void)eta;
    ++probe->mark_calls;
}

int main(void)
{
    StarsAiBlockedMinePlanet first;
    StarsAiBlockedMinePlanet second;
    StarsAiBlockedMinePlanet *planets[2];
    StarsAddBlockedMinesOps ops;
    MineProbe probe;
    int failures = 0;

    first.id = 1; first.production_flags = 0; first.current_eta = 600;
    first.queued_eta = -1; first.existing_mines = 4; first.max_mines = 5;
    first.mining_race_stat = 5; first.resource_units = 0; first.mine_cost = 20;
    second.id = 2; second.production_flags = 0; second.current_eta = 600;
    second.queued_eta = -1; second.existing_mines = 1; second.max_mines = 5;
    second.mining_race_stat = 5; second.resource_units = 0; second.mine_cost = 20;
    planets[0] = &first; planets[1] = &second;
    probe.queue_calls = 0; probe.mark_calls = 0; probe.last_type = 0;
    probe.last_amount = 0;
    ops.resources_at_planet = resources_at_planet;
    ops.add_queue_item = add_queue_item;
    ops.mark_queue = mark_queue;
    stars_add_mines_to_blocked_queues(planets, 2, &ops, &probe);
    if (probe.queue_calls != 2 || probe.mark_calls != 2 ||
            probe.last_type != 8 || probe.last_amount != 4)
        ++failures;
    if (first.current_eta == 1)
        ++failures;
    printf("ADD_MINES_BLOCKED_CHECKS=2 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
