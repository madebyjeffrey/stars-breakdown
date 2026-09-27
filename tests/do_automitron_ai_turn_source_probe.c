#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 merges;
    StarsI16 masks[3];
    StarsI16 statuses;
    StarsI16 budget;
    StarsI16 splits;
    StarsI16 ensured;
    StarsI16 planets;
    StarsI16 fleets;
    StarsI16 typed_fleets;
    StarsU8 split_mask[16];
    StarsAutomitronFleetPolicy policy;
} Trace;
static StarsI16 merge(void *c, StarsI16 mask)
{
    Trace *t = (Trace *)c;
    t->masks[t->merges++] = mask;
    return 1;
}
static StarsI16 status(void *c, StarsI16 first, StarsI16 last,
    StarsI16 budget, StarsI16 *selected)
{
    Trace *t = (Trace *)c;
    (void)first;
    (void)last;
    t->budget = budget;
    *selected = 6;
    ++t->statuses;
    return 1;
}
static StarsI16 split(void *c, const StarsU8 mask[16])
{
    Trace *t = (Trace *)c;
    memcpy(t->split_mask, mask, 16);
    if (mask[11] != 12U || mask[4] != 5U || mask[2] != 3U ||
            mask[9] != 10U)
        return 0;
    ++t->splits;
    return 1;
}
static StarsI16 status_ex(void *c, StarsI16 first, StarsI16 last,
    StarsI16 budget, StarsI16 *selected, StarsU8 mask[16])
{
    if (!status(c, first, last, budget, selected))
        return 0;
    mask[first] = (StarsU8)(first + 1);
    return 1;
}
static StarsI16 ensure(void *c, StarsI16 selected)
{
    Trace *t = (Trace *)c;
    if (selected != 6)
        return 0;
    ++t->ensured;
    return 1;
}
static StarsI16 planet(void *c, const StarsAutomitronTurnPlanet *item,
    StarsI16 budget)
{
    Trace *t = (Trace *)c;
    (void)item;
    if (budget != 70)
        return 0;
    ++t->planets;
    return 1;
}
static StarsI16 fleet(void *c, const StarsAutomitronTurnFleet *item,
    StarsI16 potency)
{
    Trace *t = (Trace *)c;
    (void)item;
    if (potency != 4)
        return 0;
    ++t->fleets;
    return 1;
}
static StarsI16 typed_fleet(void *c,
    const StarsAutomitronTurnFleet *item,
    const StarsAutomitronFleetPolicy *policy, StarsI16 potency)
{
    Trace *t = (Trace *)c;
    (void)item;
    if (potency != 4 || policy->total_ships != 20 ||
            policy->slot_1_ships != 2 || policy->slot_2_3_ships != 7 ||
            policy->slot_6_ships != 4 || policy->slot_9_10_ships != 7)
        return 0;
    t->policy = *policy;
    ++t->typed_fleets;
    return 1;
}
int main(void)
{
    StarsAutomitronAiTurnOps ops;
    StarsAutomitronAiTurnInput input;
    StarsAutomitronTurnPlanet planets[2];
    StarsAutomitronTurnFleet fleet_item;
    Trace trace;
    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(&planets, 0, sizeof planets);
    memset(&fleet_item, 0, sizeof fleet_item);
    memset(&trace, 0, sizeof trace);
    ops.merge_shdefs = merge;
    ops.check_design_status_ex = status_ex;
    ops.split_shdefs = split;
    ops.ensure_shdefs = ensure;
    ops.visit_planet = planet;
    ops.visit_fleet = fleet;
    ops.context = &trace;
    planets[0].owner = 4;
    planets[0].desirability = -1;
    planets[1].owner = -1;
    planets[1].planet_class = 3;
    planets[1].optimal_value = 1;
    fleet_item.owner = 4;
    fleet_item.ship_counts[1] = 2;
    fleet_item.ship_counts[2] = 3;
    fleet_item.ship_counts[3] = 4;
    fleet_item.ship_counts[6] = 4;
    fleet_item.ship_counts[9] = 5;
    fleet_item.ship_counts[10] = 2;
    input.turn = 150;
    input.player_id = 4;
    input.planet_count = 2;
    input.planets = planets;
    input.fleet_count = 1;
    input.fleets = &fleet_item;
    input.ops = &ops;
    if (!stars_do_automitron_ai_turn(&input) || trace.merges != 3 ||
            trace.masks[0] != 0x1e0c || trace.masks[1] != 0x40 ||
            trace.masks[2] != 0x4000 || trace.statuses != 4 ||
            trace.budget != 70 || trace.splits != 1 || trace.ensured != 1 ||
            trace.planets != 2 || trace.fleets != 1 ||
            input.unowned_viable_count != 1 || input.owned_bad_count != 1 ||
            input.armada_potency[0] != 4 || input.armada_potency[1] != 2 ||
            input.armada_potency[2] != 8 || input.armada_potency[3] != 3)
        return 1;
    ops.visit_fleet_typed = typed_fleet;
    if (!stars_do_automitron_ai_turn(&input) || trace.typed_fleets != 1 ||
            trace.policy.total_ships != 20)
        return 5;
    fleet_item.owner = 7;
    memset(&trace, 0, sizeof trace);
    if (!stars_do_automitron_ai_turn(&input) || trace.fleets != 0)
        return 2;
    planets[1].planet_class = 2;
    if (!stars_do_automitron_ai_turn(&input) ||
            input.unowned_viable_count != 0)
        return 3;
    planets[1].planet_class = 3;
    planets[1].optimal_value = 0;
    if (!stars_do_automitron_ai_turn(&input) ||
            input.unowned_viable_count != 0)
        return 4;
    puts("AUTOMITRON_AI_TURN_CHECKS=31 FAILURES=0 RESULT=PASS");
    return 0;
}
