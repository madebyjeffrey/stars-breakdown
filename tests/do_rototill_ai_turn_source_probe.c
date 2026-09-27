#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 ensure;
    StarsI16 order;
    StarsI16 planets;
    StarsI16 fleets;
    StarsU16 first_score;
} Trace;
static StarsI16 ensure(void *c, StarsI16 order)
{
    Trace *t = (Trace *)c;
    t->order = order;
    ++t->ensure;
    return 1;
}
static StarsI16 planet(void *c, const StarsRototillTurnPlanet *item,
    StarsU16 score, StarsI16 player_id)
{
    Trace *t = (Trace *)c;
    (void)item;
    (void)player_id;
    if (t->planets == 0)
        t->first_score = score;
    ++t->planets;
    return 1;
}
static StarsI16 fleet(void *c, const StarsRototillTurnFleet *item,
    StarsI16 player_id)
{
    Trace *t = (Trace *)c;
    (void)item;
    (void)player_id;
    ++t->fleets;
    return 1;
}
int main(void)
{
    StarsRototillAiTurnOps ops;
    StarsRototillAiTurnInput input;
    StarsRototillTurnPlanet planets[3];
    StarsRototillTurnFleet fleet_item;
    Trace trace;
    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(&planets, 0, sizeof planets);
    memset(&fleet_item, 0, sizeof fleet_item);
    memset(&trace, 0, sizeof trace);
    ops.ensure_shdefs = ensure;
    ops.visit_planet = planet;
    ops.visit_fleet = fleet;
    ops.context = &trace;
    planets[0].owner = -1;
    planets[0].visibility_flags = 3;
    planets[0].planet_class = 3;
    planets[0].optimal_value = 1;
    planets[0].mineral_concentration[0] = 80;
    planets[0].mineral_concentration[1] = 90;
    planets[0].mineral_concentration[2] = 100;
    planets[1].owner = 4;
    planets[1].desirability = -1;
    planets[1].productive = 1;
    planets[2].owner = -1;
    planets[2].visibility_flags = 1;
    planets[2].planet_class = 1;
    fleet_item.owner = 4;
    input.turn = 30;
    input.player_id = 4;
    input.planet_count = 3;
    input.planets = planets;
    input.fleet_count = 1;
    input.fleets = &fleet_item;
    input.ops = &ops;
    if (!stars_do_rototill_ai_turn(&input) || trace.ensure != 1 ||
            trace.order != 15 || trace.planets != 3 || trace.fleets != 1 ||
            trace.first_score != 127 || input.unowned_viable_count != 1 ||
            input.owned_bad_count != 1 || input.productive_count != 1 ||
            input.mineral_score != 127)
        return 1;
    fleet_item.owner = 7;
    memset(&trace, 0, sizeof trace);
    if (!stars_do_rototill_ai_turn(&input) || trace.fleets != 0)
        return 2;
    planets[0].optimal_value = 0;
    if (!stars_do_rototill_ai_turn(&input) || input.unowned_viable_count != 0)
        return 3;
    puts("ROTOTILL_AI_TURN_CHECKS=15 FAILURES=0 RESULT=PASS");
    return 0;
}
