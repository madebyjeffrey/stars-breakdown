#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 merges;
    StarsI16 masks[5];
    StarsI16 status;
    StarsI16 budget;
    StarsI16 splits;
    StarsU8 split_masks[4][16];
    StarsI16 ensured;
    StarsI16 marks;
    StarsI16 productions;
    StarsI16 fleets;
    StarsI16 hostile_threat;
    StarsU16 first_low_mask;
} Trace;

static StarsI16 merge(void *c, StarsI16 mask)
{
    Trace *t = (Trace *)c;
    t->masks[t->merges++] = mask;
    return 1;
}
static StarsI16 status_ex(void *c, StarsI16 first, StarsI16 last,
    StarsI16 budget, StarsI16 *selected, StarsU8 mask[16])
{
    Trace *t = (Trace *)c;
    (void)last;
    t->budget = budget;
    *selected = 12;
    mask[first] = (StarsU8)(first + 1);
    ++t->status;
    return 1;
}
static StarsI16 split(void *c, const StarsU8 mask[16])
{
    Trace *t = (Trace *)c;
    if (t->splits < 4)
        memcpy(t->split_masks[t->splits], mask, 16);
    ++t->splits;
    return 1;
}
static StarsI16 ensure(void *c, StarsI16 selected)
{
    Trace *t = (Trace *)c;
    if (selected != 12)
        return 0;
    ++t->ensured;
    return 1;
}
static StarsI16 mark(void *c, const StarsCyberTurnPlanet *planet,
    StarsU16 low_mask, StarsI16 player_id)
{
    Trace *t = (Trace *)c;
    (void)planet;
    (void)player_id;
    if (t->marks == 0)
        t->first_low_mask = low_mask;
    ++t->marks;
    return 1;
}
static StarsI16 mark_hostile(void *c, const StarsCyberTurnPlanet *planet,
    StarsI16 player_id, StarsI16 threat_level)
{
    Trace *t = (Trace *)c;
    (void)planet;
    if (player_id != 4 || threat_level != 4)
        return 0;
    ++t->hostile_threat;
    return 1;
}
static StarsI16 production(void *c, const StarsCyberTurnPlanet *planet,
    StarsI16 budget)
{
    Trace *t = (Trace *)c;
    (void)planet;
    if (budget != 70)
        return 0;
    ++t->productions;
    return 1;
}
static StarsI16 fleet(void *c, const StarsCyberTurnFleet *item,
    StarsI16 potency)
{
    Trace *t = (Trace *)c;
    (void)item;
    if (potency != 4)
        return 0;
    ++t->fleets;
    return 1;
}

int main(void)
{
    StarsCyberAiTurnOps ops;
    StarsCyberAiTurnInput input;
    StarsCyberTurnPlanet planets[2];
    StarsCyberTurnFleet fleet_item;
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
    ops.mark_planet = mark;
    ops.mark_hostile_planet = mark_hostile;
    ops.visit_production = production;
    ops.visit_fleet = fleet;
    ops.context = &trace;
    planets[0].owner = 4;
    planets[0].habitat_kind = 1;
    planets[0].visibility_flags = 0x0200U;
    planets[0].minerals[0] = 0;
    planets[0].minerals[1] = 10;
    planets[0].minerals[2] = 20;
    planets[0].resources_available[0] = 100;
    planets[0].resources_available[1] = 100;
    planets[0].resources_available[2] = 100;
    planets[0].resources_available[3] = 100;
    planets[0].production_queue_cost[0] = 10;
    planets[0].production_queue_cost[1] = 10;
    planets[0].production_queue_cost[2] = 10;
    planets[0].production_queue_cost[3] = 10;
    planets[1].owner = 7;
    planets[1].packet_strength = 501;
    planets[1].starbase_flags = 0x0200U;
    planets[1].habitat_kind = 2;
    planets[1].minerals[0] = 0;
    fleet_item.owner = 4;
    input.turn = 150;
    input.player_id = 4;
    input.planet_count = 2;
    input.planets = planets;
    input.fleet_count = 1;
    input.fleets = &fleet_item;
    input.ops = &ops;
    if (!stars_do_cyber_ai_turn(&input) || trace.merges != 5 ||
            trace.masks[0] != 1 || trace.masks[1] != 0x30 ||
            trace.masks[2] != -0x4000 || trace.masks[3] != 0x03c0 ||
            trace.masks[4] != 0x3c00 || trace.status != 3 ||
            trace.budget != 70 || trace.splits != 4 || trace.ensured != 1 ||
            trace.marks != 2 || trace.hostile_threat != 1 ||
            trace.productions != 1 || trace.fleets != 1 ||
            trace.first_low_mask != 0x0100 || input.packet_pressure != 16 ||
            input.low_mineral_planets != 1 ||
            input.production_affordable_count != 1 ||
            input.production_blocked_count != 0 ||
            input.armada_potency[0] != 4 ||
            input.armada_potency[1] != 2 || input.armada_potency[2] != 8 ||
            input.armada_potency[3] != 3 ||
            trace.split_masks[0][4] != 5 ||
            trace.split_masks[0][14] != 15 ||
            trace.split_masks[0][2] != 3 ||
            trace.split_masks[0][12] != 0 ||
            trace.split_masks[1][0] != 2 || trace.split_masks[1][1] != 0 ||
            trace.split_masks[2][0] != 0 || trace.split_masks[2][1] != 2 ||
            trace.split_masks[3][2] != 2 || trace.split_masks[3][3] != 2)
        return 1;
    fleet_item.owner = 7;
    memset(&trace, 0, sizeof trace);
    if (!stars_do_cyber_ai_turn(&input) || trace.fleets != 0)
        return 2;
    planets[0].visibility_flags = 0;
    planets[0].mineral_rates[0] = 0;
    memset(&trace, 0, sizeof trace);
    if (!stars_do_cyber_ai_turn(&input) || trace.first_low_mask != 0 ||
            input.low_mineral_planets != 0 || trace.marks != 2)
        return 3;
    planets[0].visibility_flags = 0x0200U;
    planets[0].mineral_rates[0] = 1;
    memset(&trace, 0, sizeof trace);
    if (!stars_do_cyber_ai_turn(&input) || trace.first_low_mask != 0 ||
            input.low_mineral_planets != 0 || trace.marks != 2)
        return 4;
    planets[0].mineral_rates[0] = 0;
    planets[0].minerals[0] = 40000U;
    memset(&trace, 0, sizeof trace);
    if (!stars_do_cyber_ai_turn(&input) || trace.first_low_mask != 0 ||
            input.low_mineral_planets != 0 || trace.marks != 2)
        return 5;
    puts("CYBER_AI_TURN_CHECKS=32 FAILURES=0 RESULT=PASS");
    return 0;
}
