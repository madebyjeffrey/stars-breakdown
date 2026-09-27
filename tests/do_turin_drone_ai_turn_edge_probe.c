#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsU8 potency[4];
    StarsI16 fleet_visits;
    StarsI16 merge_calls;
    StarsI16 merge_needed_calls;
    StarsI16 status_calls;
    StarsI16 queued;
    StarsI16 queued_design;
    StarsI16 queued_quantity;
} Trace;

static StarsI16 visit_fleet(void *context, const StarsTurinTurnFleet *fleet,
    const StarsTurinFleetPolicy *policy, const StarsU8 potency[4])
{
    Trace *trace = (Trace *)context;
    (void)fleet;
    (void)policy;
    memcpy(trace->potency, potency, sizeof trace->potency);
    ++trace->fleet_visits;
    return 1;
}

static StarsI16 merge_needed(void *context, StarsI16 mask)
{
    Trace *trace = (Trace *)context;
    ++trace->merge_needed_calls;
    return mask != (StarsI16)-0x1f10;
}

static StarsI16 merge(void *context, StarsI16 mask)
{
    Trace *trace = (Trace *)context;
    (void)mask;
    ++trace->merge_calls;
    return 1;
}

static StarsI16 status(void *context, StarsI16 first, StarsI16 last,
    StarsI16 budget, StarsI16 *selected, StarsU8 mask[16])
{
    Trace *trace = (Trace *)context;
    (void)budget;
    (void)mask;
    *selected = first == 6 && last == 7 ? 6 : 9;
    ++trace->status_calls;
    return 1;
}

static StarsI16 queue_production(void *context,
    const StarsTurinTurnPlanet *planet, StarsI16 design, StarsI16 quantity)
{
    Trace *trace = (Trace *)context;
    (void)planet;
    trace->queued_design = design;
    trace->queued_quantity = quantity;
    ++trace->queued;
    return 1;
}

static void setup(StarsTurinDroneAiTurnInput *input,
    StarsTurinDroneAiTurnOps *ops, StarsTurinTurnPlanet *planet,
    StarsTurinTurnFleet *fleet, Trace *trace, StarsI16 turn)
{
    memset(input, 0, sizeof *input);
    memset(ops, 0, sizeof *ops);
    memset(planet, 0, sizeof *planet);
    memset(fleet, 0, sizeof *fleet);
    memset(trace, 0, sizeof *trace);
    planet->id = 1;
    planet->owner = 4;
    planet->production_slot_open = 1;
    fleet->id = 7;
    fleet->owner = 4;
    fleet->cord = 0;
    fleet->ship_counts[2] = 1;
    ops->visit_fleet_typed = visit_fleet;
    ops->context = trace;
    input->turn = turn;
    input->player_id = 4;
    input->planet_count = 1;
    input->planets = planet;
    input->fleet_count = 1;
    input->fleets = fleet;
    input->ops = ops;
}

static int run_potency_case(StarsI16 turn, StarsU8 a0, StarsU8 a1,
    StarsU8 a2, StarsU8 a3)
{
    StarsTurinDroneAiTurnInput input;
    StarsTurinDroneAiTurnOps ops;
    StarsTurinTurnPlanet planet;
    StarsTurinTurnFleet fleet;
    Trace trace;

    setup(&input, &ops, &planet, &fleet, &trace, turn);
    if (!stars_do_turin_drone_ai_turn(&input))
        return 0;
    return trace.fleet_visits == 1 && trace.potency[0] == a0 &&
        trace.potency[1] == a1 && trace.potency[2] == a2 &&
        trace.potency[3] == a3;
}

static int run_policy_cases(void)
{
    StarsTurinDroneAiTurnInput input;
    StarsTurinDroneAiTurnOps ops;
    StarsTurinTurnPlanet planet;
    StarsTurinTurnFleet fleet;
    Trace trace;
    StarsI16 i;

    setup(&input, &ops, &planet, &fleet, &trace, 150);
    ops.merge_needed = merge_needed;
    ops.merge_shdefs = merge;
    ops.check_design_status = status;
    ops.queue_production = queue_production;
    for (i = 0; i < 4; ++i) {
        planet.resources_available[i] = 100;
        planet.production_queue_cost[i] = 10;
        planet.hull_cost[i] = 10;
    }
    if (!stars_do_turin_drone_ai_turn(&input))
        return 0;
    if (trace.merge_needed_calls != 4 || trace.merge_calls != 3 ||
            trace.status_calls != 8 || trace.queued != 5 ||
            trace.queued_design != 9 || trace.queued_quantity != 1)
        return 0;
    setup(&input, &ops, &planet, &fleet, &trace, 150);
    ops.check_design_status = status;
    ops.queue_production = queue_production;
    for (i = 0; i < 4; ++i) {
        planet.resources_available[i] = 50;
        planet.production_queue_cost[i] = 10;
        planet.hull_cost[i] = 10;
    }
    if (!stars_do_turin_drone_ai_turn(&input) || trace.queued != 4)
        return 0;
    return stars_do_turin_drone_ai_turn((StarsTurinDroneAiTurnInput *)0) == 0;
}

int main(void)
{
    int checks = 0;
    int failures = 0;

#define CHECK(condition) do { ++checks; if (!(condition)) ++failures; } while (0)
    CHECK(run_potency_case(0, 3, 1, 6, 2));
    CHECK(run_potency_case(130, 3, 1, 7, 2));
    CHECK(run_potency_case(131, 3, 1, 7, 2));
    CHECK(run_potency_case(200, 7, 3, 10, 3));
    CHECK(run_potency_case(300, 12, 6, 12, 3));
    CHECK(run_policy_cases());
    CHECK(checks == 7);
    if (failures)
        puts("TURIN_DRONE_AI_TURN_EDGE_CHECKS=7 FAILURES=1 RESULT=FAIL");
    else
        puts("TURIN_DRONE_AI_TURN_EDGE_CHECKS=7 FAILURES=0 RESULT=PASS");
    return failures ? 1 : 0;
#undef CHECK
}
