#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct TypedTrace {
    StarsI16 planets;
    StarsI16 fleets;
    StarsI16 owned;
    StarsI16 unowned;
    StarsI16 armed;
    StarsI16 mission;
    StarsI16 repair;
    StarsI16 queue_available;
    StarsI16 queue_blocked;
    StarsI16 production_candidates;
    StarsI16 hostile;
    StarsI16 threat_level;
    StarsU16 mineral_score;
} TypedTrace;

static StarsI16 visit_planet(void *opaque,
    const StarsMacintiTurnPlanet *planet,
    const StarsMacintiPlanetPolicy *policy, StarsI16 turn)
{
    TypedTrace *trace;
    (void)planet;
    (void)turn;
    trace = (TypedTrace *)opaque;
    ++trace->planets;
    trace->owned += policy->owned;
    trace->unowned += policy->unowned;
    trace->hostile += policy->hostile;
    trace->threat_level = policy->threat_level;
    trace->mineral_score = (StarsU16)(trace->mineral_score +
        policy->mineral_score);
    trace->queue_available += policy->production_queue_available;
    trace->queue_blocked += !policy->production_queue_available;
    return 1;
}

static StarsI16 visit_fleet(void *opaque,
    const StarsMacintiTurnFleet *fleet,
    const StarsMacintiFleetPolicy *policy, const StarsU8 potency[4])
{
    TypedTrace *trace;
    (void)fleet;
    (void)potency;
    trace = (TypedTrace *)opaque;
    ++trace->fleets;
    trace->armed += policy->armed_ships > 0;
    trace->mission += policy->mission_design_ships;
    trace->repair += policy->needs_order_repair;
    return 1;
}

static StarsI16 choose_production(void *opaque,
    const StarsMacintiTurnPlanet *planet,
    const StarsMacintiPlanetPolicy *policy, StarsI16 turn)
{
    TypedTrace *trace;
    (void)planet;
    (void)policy;
    (void)turn;
    trace = (TypedTrace *)opaque;
    ++trace->production_candidates;
    return 1;
}

int main(void)
{
    StarsMacintiAiTurnOps ops;
    StarsMacintiAiTurnInput input;
    StarsMacintiTurnPlanet planets[3];
    StarsMacintiTurnFleet fleets[2];
    TypedTrace trace;
    StarsI16 failures;

    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(planets, 0, sizeof planets);
    memset(fleets, 0, sizeof fleets);
    memset(&trace, 0, sizeof trace);
    planets[0].id = 4;
    planets[0].owner = 3;
    planets[0].starbase_flags = 1;
    planets[0].mineral_concentration[0] = 50;
    planets[0].mineral_concentration[1] = 70;
    planets[0].mineral_concentration[2] = 10;
    planets[0].production_queue_count = 23;
    planets[0].production_queue_limit = 24;
    planets[1].id = 5;
    planets[1].owner = -1;
    planets[1].mineral_concentration[0] = 20;
    planets[1].mineral_concentration[1] = 20;
    planets[1].mineral_concentration[2] = 20;
    planets[1].production_queue_count = 24;
    planets[1].production_queue_limit = 24;
    planets[2].id = 6;
    planets[2].owner = 4;
    planets[2].starbase_flags = STARS_MACINTI_STARBASE_FLAG;
    planets[2].packet_strength = 501;
    fleets[0].id = 8;
    fleets[0].owner = 3;
    fleets[0].cord = 1;
    fleets[0].order_kind = 0;
    fleets[0].ship_counts[0] = 1;
    fleets[0].ship_counts[1] = 2;
    fleets[0].ship_counts[2] = 3;
    fleets[0].ship_counts[0xe] = 2;
    fleets[0].ship_counts[0xf] = 1;
    fleets[1].ship_counts[10] = 2;
    fleets[1].owner = 3;
    fleets[1].cord = 2;
    fleets[1].order_kind = 1;
    ops.visit_planet_typed = visit_planet;
    ops.choose_planet_production = choose_production;
    ops.visit_fleet_typed = visit_fleet;
    ops.context = &trace;
    input.turn = 150;
    input.player_id = 3;
    input.planet_count = 3;
    input.planets = planets;
    input.fleet_count = 2;
    input.fleets = fleets;
    input.ops = &ops;
    failures = !stars_do_macinti_ai_turn(&input);
    failures += trace.planets != 3 || trace.owned != 1 || trace.unowned != 1 ||
        trace.hostile != 1 || trace.threat_level != 4;
    failures += trace.mineral_score != 135;
    failures += trace.queue_available != 2 || trace.queue_blocked != 1;
    failures += trace.production_candidates != 1 ||
        input.typed_production_candidates != 1;
    failures += trace.fleets != 2 || trace.armed != 1 || trace.repair != 1;
    failures += trace.mission != 2;
    failures += input.typed_planets_visited != 3 ||
        input.typed_fleets_visited != 2 || input.typed_armed_fleets != 1 ||
        input.typed_colonizer_fleets != 1 || input.typed_mission_fleets != 1 ||
        input.typed_support_design_fleets != 0 ||
        input.typed_attack_design_fleets != 1;
    fleets[1].owner = 2;
    memset(&trace, 0, sizeof trace);
    if (!stars_do_macinti_ai_turn(&input) || trace.fleets != 1 ||
            input.typed_fleets_visited != 1 || input.typed_armed_fleets != 1 ||
            input.typed_colonizer_fleets != 1 || input.typed_mission_fleets != 0 ||
            input.typed_attack_design_fleets != 1)
        ++failures;
    printf("MACINTI_AI_TYPED_CHECKS=9 FAILURES=%d RESULT=%s\n",
        (int)failures, failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
}
