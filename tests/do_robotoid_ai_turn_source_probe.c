#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 merges;
    StarsI16 masks[4];
    StarsI16 status;
    StarsI16 budget;
    StarsI16 split;
    StarsI16 ensured;
    StarsI16 war_count;
    StarsI16 marked;
    StarsI16 production;
    StarsI16 fleets;
    StarsI16 threat;
} Trace;

static StarsI16 merge(void *c, StarsI16 mask)
{
    Trace *t = (Trace *)c;
    t->masks[t->merges++] = mask;
    return 1;
}
static StarsI16 status(void *c, StarsI16 first, StarsI16 last,
    StarsI16 budget, StarsI16 *selected, StarsU8 mask[16])
{
    Trace *t = (Trace *)c;
    (void)first;
    (void)last;
    t->budget = budget;
    *selected = 9;
    if (first == 14 && last == 15) {
        mask[5] = 2;
        mask[14] = 3;
        mask[15] = 4;
    }
    ++t->status;
    return 1;
}
static StarsI16 design_info(void *c, StarsI16 index, StarsI16 *active,
    StarsI16 *hull_kind)
{
    (void)c;
    if (index == 14) {
        *active = 0;
        *hull_kind = 0x1d;
    } else if (index == 15) {
        *active = 0;
        *hull_kind = 0x1e;
    } else {
        return 0;
    }
    return 1;
}
static StarsI16 split(void *c, const StarsU8 mask[16])
{
    Trace *t = (Trace *)c;
    if ((t->split == 0 && (mask[5] != 2 || mask[14] != 0 ||
                mask[15] != 4)) ||
            (t->split == 1 && mask[0] != 2) ||
            (t->split == 2 && mask[1] != 2) ||
            (t->split == 3 && (mask[11] != 2 || mask[12] != 2 ||
                mask[13] != 2)))
        return 0;
    ++t->split;
    return 1;
}
static StarsI16 ensure(void *c)
{
    Trace *t = (Trace *)c;
    ++t->ensured;
    return 1;
}
static StarsI16 count_war(void *c, const StarsRobotoidTurnFleet *fleet,
    StarsI16 potency)
{
    Trace *t = (Trace *)c;
    (void)fleet;
    if (potency != 8)
        return 0;
    ++t->war_count;
    return 1;
}
static StarsI16 mark(void *c, const StarsRobotoidTurnPlanet *planet,
    StarsI16 player_id)
{
    Trace *t = (Trace *)c;
    (void)planet;
    (void)player_id;
    ++t->marked;
    return 1;
}
static StarsI16 mark_typed(void *c, const StarsRobotoidTurnPlanet *planet,
    StarsI16 player_id, StarsI16 threat_level)
{
    Trace *t = (Trace *)c;
    (void)planet;
    if (player_id != 4 || threat_level != 4)
        return 0;
    ++t->marked;
    ++t->threat;
    return 1;
}
static StarsI16 production(void *c, const StarsRobotoidTurnPlanet *planet,
    StarsI16 budget)
{
    Trace *t = (Trace *)c;
    (void)planet;
    if (budget != 70)
        return 0;
    ++t->production;
    return 1;
}
static StarsI16 fleets(void *c, StarsI16 player_id)
{
    Trace *t = (Trace *)c;
    (void)player_id;
    ++t->fleets;
    return 1;
}

int main(void)
{
    StarsRobotoidAiTurnOps ops;
    StarsRobotoidAiTurnInput input;
    StarsRobotoidTurnPlanet planets[3];
    StarsRobotoidTurnFleet fleet;
    Trace trace;

    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(&planets, 0, sizeof planets);
    memset(&fleet, 0, sizeof fleet);
    memset(&trace, 0, sizeof trace);
    ops.merge_shdefs = merge;
    ops.check_design_status = status;
    ops.design_status_info = design_info;
    ops.split_shdefs = split;
    ops.ensure_shdefs = ensure;
    ops.count_war_fleet = count_war;
    ops.mark_planet = mark;
    ops.mark_planet_typed = mark_typed;
    ops.run_production = production;
    ops.run_fleets = fleets;
    ops.context = &trace;
    planets[0].owner = 4;
    planets[0].visibility_flags = 0x0200U;
    planets[0].mineral_rate = 1;
    planets[0].mineral_concentration = 0;
    planets[1].owner = 9;
    planets[1].packet_strength = 501;
    planets[1].starbase_flags = 0x0200U;
    planets[2].owner = -1;
    fleet.owner = 4;
    fleet.warship_count = 3;
    fleet.ship_counts[14] = 1;
    input.turn = 150;
    input.player_id = 4;
    input.planet_count = 3;
    input.planets = planets;
    input.fleet_count = 1;
    input.fleets = &fleet;
    input.ops = &ops;
    if (!stars_do_robotoid_ai_turn(&input) || trace.merges != 3 ||
            trace.masks[0] != 0x06fc || trace.masks[1] != 1 ||
            trace.masks[2] != -0x4000 || trace.status != 5 ||
            trace.budget != 105 || trace.split != 4 || trace.ensured != 1 ||
            trace.war_count != 1 || trace.marked != 1 || trace.threat != 1 ||
            trace.production != 1 ||
            trace.fleets != 1 || input.armada_potency[0] != 5 ||
            input.armada_potency[1] != 2 || input.armada_potency[2] != 8 ||
            input.armada_potency[3] != 3 || input.hostile_planet_count != 1)
        return 1;
    fleet.ship_counts[14] = 0;
    fleet.warship_count = 3;
    memset(&trace, 0, sizeof trace);
    if (!stars_do_robotoid_ai_turn(&input) || trace.war_count != 0)
        return 2;
    puts("ROBOTOID_AI_TURN_CHECKS=23 FAILURES=0 RESULT=PASS");
    return 0;
}
