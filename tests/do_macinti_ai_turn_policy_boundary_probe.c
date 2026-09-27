#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct PolicyTrace {
    StarsI16 planets;
    StarsI16 fleets;
    StarsI16 planet_ids[4];
    StarsI16 owned[4];
    StarsI16 unowned[4];
    StarsI16 hostile[4];
    StarsI16 viable[4];
    StarsI16 queue_available[4];
    StarsI16 affordable[4];
    StarsI16 batch_affordable[4];
    StarsI16 threat[4];
    StarsU16 minerals[4];
    StarsI16 fleet_ids[4];
    StarsI16 tasks[4];
    StarsI16 armed[4];
    StarsI16 transport[4];
    StarsI16 colonizer[4];
    StarsI16 repair[4];
} PolicyTrace;

static StarsI16 visit_planet(void *opaque,
    const StarsMacintiTurnPlanet *planet,
    const StarsMacintiPlanetPolicy *policy, StarsI16 turn)
{
    PolicyTrace *trace;
    StarsI16 index;

    (void)turn;
    trace = (PolicyTrace *)opaque;
    index = trace->planets;
    if (index < 4) {
        trace->planet_ids[index] = planet->id;
        trace->owned[index] = policy->owned;
        trace->unowned[index] = policy->unowned;
        trace->hostile[index] = policy->hostile;
        trace->viable[index] = policy->viable_minerals;
        trace->queue_available[index] = policy->production_queue_available;
        trace->affordable[index] = policy->production_affordable;
        trace->batch_affordable[index] = policy->production_batch_affordable;
        trace->threat[index] = policy->threat_level;
        trace->minerals[index] = policy->mineral_score;
    }
    ++trace->planets;
    return 1;
}

static StarsI16 visit_fleet(void *opaque,
    const StarsMacintiTurnFleet *fleet,
    const StarsMacintiFleetPolicy *policy, const StarsU8 potency[4])
{
    PolicyTrace *trace;
    StarsI16 index;

    (void)potency;
    trace = (PolicyTrace *)opaque;
    index = trace->fleets;
    if (index < 4) {
        trace->fleet_ids[index] = fleet->id;
        trace->tasks[index] = policy->recommended_task;
        trace->armed[index] = policy->armed_ships;
        trace->transport[index] = policy->transport_ships;
        trace->colonizer[index] = policy->colonizer_ships;
        trace->repair[index] = policy->needs_order_repair;
    }
    ++trace->fleets;
    return 1;
}

int main(void)
{
    StarsMacintiAiTurnOps ops;
    StarsMacintiAiTurnInput input;
    StarsMacintiTurnPlanet planets[4];
    StarsMacintiTurnFleet fleets[4];
    PolicyTrace trace;
    StarsI16 failures;

    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(planets, 0, sizeof planets);
    memset(fleets, 0, sizeof fleets);
    memset(&trace, 0, sizeof trace);

    planets[0].id = 4;
    planets[0].owner = 3;
    planets[0].starbase_flags = STARS_MACINTI_STARBASE_FLAG;
    planets[0].packet_strength = 500;
    planets[0].mineral_concentration[0] = 0x42;
    planets[0].mineral_concentration[1] = 0x43;
    planets[0].mineral_concentration[2] = 0x44;
    planets[0].resources_available[0] = 20;
    planets[0].resources_available[1] = 30;
    planets[0].resources_available[2] = 40;
    planets[0].resources_available[3] = 50;
    planets[0].production_queue_cost[0] = 20;
    planets[0].production_queue_cost[1] = 30;
    planets[0].production_queue_cost[2] = 40;
    planets[0].production_queue_cost[3] = 50;
    planets[0].hull_cost[0] = 0;
    planets[0].hull_cost[1] = 0;
    planets[0].hull_cost[2] = 0;
    planets[0].hull_cost[3] = 0;
    planets[0].production_batch = 1;
    planets[0].production_queue_count = 5;
    planets[0].production_queue_limit = 6;

    planets[1].id = 5;
    planets[1].owner = -1;
    planets[1].mineral_concentration[0] = 10;
    planets[1].mineral_concentration[1] = 10;
    planets[1].mineral_concentration[2] = 10;
    planets[1].production_queue_count = 6;
    planets[1].production_queue_limit = 6;

    planets[2].id = 6;
    planets[2].owner = 7;
    planets[2].starbase_flags = STARS_MACINTI_STARBASE_FLAG;
    planets[2].packet_strength = 1500;
    planets[2].mineral_concentration[0] = 30;
    planets[2].mineral_concentration[1] = 30;
    planets[2].mineral_concentration[2] = 30;

    planets[3].id = 7;
    planets[3].owner = 8;
    planets[3].packet_strength = 1;
    planets[3].mineral_concentration[0] = 0x42;
    planets[3].mineral_concentration[1] = 0x42;
    planets[3].mineral_concentration[2] = 0x42;

    fleets[0].id = 10;
    fleets[0].owner = 3;
    fleets[0].cord = 1;
    fleets[0].order_kind = 0;
    fleets[0].ship_counts[2] = 2;
    fleets[1].id = 11;
    fleets[1].owner = 3;
    fleets[1].cord = 2;
    fleets[1].ship_counts[1] = 1;
    fleets[2].id = 12;
    fleets[2].owner = 3;
    fleets[2].cord = 2;
    fleets[2].ship_counts[0] = 1;
    fleets[3].id = 13;
    fleets[3].owner = 9;
    fleets[3].cord = 2;
    fleets[3].ship_counts[2] = 9;

    ops.visit_planet_typed = visit_planet;
    ops.visit_fleet_typed = visit_fleet;
    ops.context = &trace;
    input.turn = 150;
    input.player_id = 3;
    input.planet_count = 4;
    input.planets = planets;
    input.fleet_count = 4;
    input.fleets = fleets;
    input.ops = &ops;

    failures = !stars_do_macinti_ai_turn(&input);
    failures += trace.planets != 4 || trace.fleets != 3;
    failures += trace.planet_ids[0] != 4 || trace.owned[0] != 1 ||
        trace.viable[0] != 1 || trace.queue_available[0] != 1 ||
        trace.affordable[0] != 1 || trace.batch_affordable[0] != 1 ||
        trace.minerals[0] != 127;
    failures += trace.unowned[1] != 1 || trace.queue_available[1] != 0 ||
        trace.viable[1] != 0 || trace.affordable[1] != 1;
    failures += trace.hostile[2] != 1 || trace.threat[2] != 7 ||
        trace.viable[2] != 1;
    failures += trace.hostile[3] != 1 || trace.threat[3] != 1;
    failures += trace.fleet_ids[0] != 10 || trace.armed[0] != 2 ||
        trace.repair[0] != 1 ||
        trace.tasks[0] != STARS_MACINTI_TASK_REPAIR_ORDER;
    failures += trace.fleet_ids[1] != 11 || trace.transport[1] != 1 ||
        trace.tasks[1] != STARS_MACINTI_TASK_TRANSPORT;
    failures += trace.fleet_ids[2] != 12 || trace.colonizer[2] != 1 ||
        trace.tasks[2] != STARS_MACINTI_TASK_COLONIZE;
    failures += input.typed_production_candidates != 1 ||
        input.typed_armed_fleets != 1 || input.typed_colonizer_fleets != 1;

    printf("MACINTI_AI_POLICY_BOUNDARY_CHECKS=11 FAILURES=%d RESULT=%s\n",
        (int)failures, failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
}
