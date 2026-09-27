#include "core.h"
#include <stdio.h>
#include <string.h>

enum {
    CASE_HIGH = 0,
    CASE_LOW = 1,
    CASE_ENEMY = 2,
    CASE_SPACE = 3,
    CASE_PACKET_REJECT = 4
};

typedef struct Trace {
    StarsU8 *plord;
    StarsI16 scenario;
    StarsI16 merge_count;
    StarsI16 ensure_ai_count;
    StarsI16 ensure_shdefs_count;
    StarsI16 visitor_count;
    StarsI16 basic_count;
    StarsI16 fill_count;
    StarsI16 freighter_target_calls;
    StarsI16 packet_target_calls;
    StarsI16 moves;
    StarsI16 transfers;
    StarsI16 starbase_moves;
    StarsI16 queue_count;
    StarsI16 select_count;
    StarsI32 transfer_amount;
    StarsU16 move_flags;
    StarsU16 packet_target_id;
} Trace;

static StarsI16 merge(void *context, StarsI16 mask)
{
    Trace *trace;
    trace = (Trace *)context;
    if (mask == 0)
        return 0;
    ++trace->merge_count;
    return 1;
}

static StarsI16 ensure_ai(void *context, StarsI16 percent,
    StarsI16 *latest_starbase_design)
{
    Trace *trace;
    trace = (Trace *)context;
    if (percent != 17)
        return -1;
    ++trace->ensure_ai_count;
    *latest_starbase_design = 4;
    return 23;
}

static StarsI16 ensure_shdefs(void *context, StarsI16 research_order)
{
    Trace *trace;
    trace = (Trace *)context;
    if (research_order != 23)
        return 0;
    ++trace->ensure_shdefs_count;
    return 1;
}

static StarsI16 lookup_planet(void *context, StarsI16 id,
    StarsCyberFreighterPlanet *planet)
{
    Trace *trace;

    trace = (Trace *)context;
    if (trace->scenario == CASE_SPACE || id != 6)
        return 0;
    memset(planet, 0, sizeof *planet);
    planet->id = 6;
    planet->owner = trace->scenario == CASE_ENEMY ? 7 : 4;
    planet->point.x = 10;
    planet->point.y = 10;
    planet->mining_rate = trace->scenario == CASE_LOW ? 0 : 1;
    planet->mineral_stock = trace->scenario == CASE_LOW ? 2000L : 5000L;
    return 1;
}

static StarsI16 find_freighter_target(void *context,
    const StarsCyberFreighterPlanet *source, StarsI16 kind,
    StarsCyberFreighterPlanet *target)
{
    Trace *trace;

    trace = (Trace *)context;
    ++trace->freighter_target_calls;
    if (source->id != 6 || trace->scenario == CASE_ENEMY ||
            (kind != 0 && kind != 1))
        return 0;
    memset(target, 0, sizeof *target);
    target->id = 9;
    target->owner = 4;
    target->point.x = 20;
    target->point.y = 20;
    return 1;
}

static void transfer_supply(void *context, StarsI16 player,
    StarsI16 planet, StarsI16 cargo_type, StarsI16 resource_type,
    StarsI32 amount)
{
    Trace *trace;
    trace = (Trace *)context;
    if (player == 4 && planet == 6 && cargo_type == 2 && resource_type == 3) {
        ++trace->transfers;
        trace->transfer_amount = amount;
    }
}

static void move_to_starbase(void *context, StarsCyberFreighterFleet *fleet)
{
    Trace *trace;
    (void)fleet;
    trace = (Trace *)context;
    ++trace->starbase_moves;
}

static StarsU16 ideal_warp(void *context,
    const StarsCyberFreighterFleet *fleet, StarsI16 purpose)
{
    (void)context;
    (void)fleet;
    return purpose == 1 ? 11U : 0U;
}

static StarsI16 move_fleet(void *context, StarsCyberFreighterFleet *fleet,
    StarsU16 kind, StarsU16 target, StarsPoint point, StarsU16 flags)
{
    Trace *trace;
    StarsMoveOrder order;

    (void)fleet;
    trace = (Trace *)context;
    if (kind != 1U || target != 9U || point.x != 20 || point.y != 20)
        return 0;
    ++trace->moves;
    trace->move_flags = flags;
    memset(&order, 0, sizeof order);
    order.id = (StarsI16)target;
    order.point = point;
    order.flags = flags;
    return stars_ai_move_order_to_plord(trace->plord, 2, &order, 0);
}

static StarsI16 run_freighter(void *context,
    const StarsCyberTurnFleet *fleet, const StarsCyberFleetPolicy *policy,
    StarsU8 potency)
{
    Trace *trace;
    StarsCyberFreighterFleet freighter;
    StarsCyberFreighterPlanState plan;
    StarsDoCyberFreighterOps ops;

    trace = (Trace *)context;
    if (fleet->id != 8 || policy->transport_ships != 2 || potency != 4)
        return 0;
    memset(&freighter, 0, sizeof freighter);
    memset(&plan, 0, sizeof plan);
    memset(&ops, 0, sizeof ops);
    freighter.id = 8;
    freighter.planet_id = fleet->planet_id;
    ops.lookup_planet = lookup_planet;
    ops.find_target = find_freighter_target;
    ops.transfer_supply = transfer_supply;
    ops.move_to_starbase = move_to_starbase;
    ops.ideal_warp = ideal_warp;
    ops.move_fleet = move_fleet;
    ops.context = trace;
    stars_do_cyber_freighter(&freighter, &plan, 4, &ops);
    return 1;
}

static StarsI16 visit_fleet(void *context,
    const StarsCyberTurnFleet *fleet, const StarsCyberFleetPolicy *policy,
    StarsU8 potency)
{
    Trace *trace;
    trace = (Trace *)context;
    if (fleet->id != 8 || policy->total_ships != 2 || potency != 4)
        return 0;
    ++trace->visitor_count;
    return 1;
}

static StarsI16 basic(void *context, StarsI16 research_order,
    StarsI16 latest_starbase_design, StarsI16 player_id)
{
    Trace *trace;
    trace = (Trace *)context;
    if (research_order != 23 || latest_starbase_design != 4 ||
            player_id != 4)
        return 0;
    ++trace->basic_count;
    return 1;
}

static StarsI16 packet_target(void *context,
    const StarsCyberPacketPlanet *source, StarsU16 *target)
{
    Trace *trace;
    trace = (Trace *)context;
    ++trace->packet_target_calls;
    if (trace->scenario == CASE_PACKET_REJECT)
        return 0;
    if (source->id != 6 || source->owner != 4)
        return 0;
    *target = 9U;
    trace->packet_target_id = *target;
    return 1;
}

static StarsI16 packet_queue(void *context, StarsI16 item, StarsI16 count,
    StarsI16 object, StarsI16 mode)
{
    Trace *trace;
    trace = (Trace *)context;
    if (item != 15 || count != 7 || object != 1 || mode != 0)
        return 0;
    ++trace->queue_count;
    return 1;
}

static void packet_select(void *context, StarsU16 source, StarsU16 target)
{
    Trace *trace;
    trace = (Trace *)context;
    if (source == 6U && target == 9U)
        ++trace->select_count;
}

static StarsI16 run_packets(void *context, StarsI16 player_id,
    StarsCyberTurnPlanet *planets, StarsI16 planet_count,
    StarsI16 packet_pressure)
{
    Trace *trace;
    StarsCyberPacketPlanet packet_planet;
    StarsDoCyberPacketsOps packet_ops;
    StarsDoCyberPacketsInput packet_input;

    trace = (Trace *)context;
    if (player_id != 4 || planet_count != 1 || packet_pressure != 16)
        return 0;
    memset(&packet_planet, 0, sizeof packet_planet);
    memset(&packet_ops, 0, sizeof packet_ops);
    packet_planet.id = (StarsU16)planets[0].id;
    packet_planet.owner = planets[0].owner;
    packet_planet.packet_strength = planets[0].packet_strength;
    packet_planet.packet_flags = trace->scenario == CASE_HIGH ||
        trace->scenario == CASE_PACKET_REJECT ? 0x0080U : 0U;
    packet_planet.available[0] = 1000UL;
    packet_planet.available[1] = 800UL;
    packet_planet.available[2] = 600UL;
    packet_planet.cost[0] = 900UL;
    packet_planet.cost[1] = 100UL;
    packet_planet.cost[2] = 500UL;
    packet_ops.find_target = packet_target;
    packet_ops.queue_item = packet_queue;
    packet_ops.select_planet = packet_select;
    packet_ops.context = trace;
    packet_input.planets = &packet_planet;
    packet_input.planet_count = 1;
    packet_input.player = player_id;
    packet_input.minimum_strength = (StarsU16)packet_pressure;
    packet_input.maximum_packets = 0;
    packet_input.ops = &packet_ops;
    stars_do_cyber_packets(&packet_input);
    planets[0].packet_strength = packet_planet.packet_strength;
    return 1;
}

static StarsI16 fill(void *context)
{
    Trace *trace;
    trace = (Trace *)context;
    ++trace->fill_count;
    return 1;
}

static void seed_first_order(StarsU8 *plord)
{
    StarsMoveOrder order;
    memset(&order, 0, sizeof order);
    order.point.x = 20;
    order.point.y = 20;
    memcpy(plord + STARS_AI_PLORD_HEADER_SIZE, &order, sizeof order);
    plord[3] = 1U;
}

static StarsI16 run_case(StarsI16 scenario, StarsI16 expected_transfers,
    StarsI32 expected_amount, StarsI16 expected_moves,
    StarsU16 expected_flags, StarsI16 expected_starbase,
    StarsI16 expected_queue, StarsI16 expected_select,
    StarsU16 expected_strength)
{
    StarsCyberTurnPlanet planet;
    StarsCyberTurnFleet fleet;
    StarsCyberAiTurnOps ops;
    StarsCyberAiTurnInput input;
    StarsU8 plord[STARS_AI_PLORD_HEADER_SIZE +
        2U * STARS_AI_PLORD_ORDER_SIZE];
    Trace trace;
    StarsI16 failures;

    memset(&planet, 0, sizeof planet);
    memset(&fleet, 0, sizeof fleet);
    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    memset(plord, 0, sizeof plord);
    trace.scenario = scenario;
    trace.plord = plord;
    planet.id = 6;
    planet.owner = scenario == CASE_ENEMY ? 7 : 4;
    planet.visibility_flags = 0x0200U;
    planet.packet_strength = 501U;
    planet.minerals[0] = 20;
    planet.minerals[1] = 20;
    planet.minerals[2] = 20;
    planet.mineral_rates[0] = 1;
    planet.mineral_rates[1] = 1;
    planet.mineral_rates[2] = 1;
    planet.resources_available[0] = 100;
    planet.resources_available[1] = 100;
    planet.resources_available[2] = 100;
    planet.resources_available[3] = 100;
    planet.production_queue_cost[0] = 10;
    planet.production_queue_cost[1] = 10;
    planet.production_queue_cost[2] = 10;
    planet.production_queue_cost[3] = 10;
    fleet.id = 8;
    fleet.owner = 4;
    fleet.planet_id = 6;
    fleet.order_kind = 1;
    fleet.ship_counts[1] = 2;
    fleet.raw_plord = plord;
    seed_first_order(plord);
    ops.merge_shdefs = merge;
    ops.ensure_ai = ensure_ai;
    ops.ensure_shdefs = ensure_shdefs;
    ops.visit_fleet_typed = visit_fleet;
    ops.run_freighter_ex = run_freighter;
    ops.run_basic_tasks_with_research = basic;
    ops.run_packets_ex = run_packets;
    ops.fill_production_queue = fill;
    ops.context = &trace;
    input.turn = 150;
    input.player_id = 4;
    input.planet_count = 1;
    input.planets = &planet;
    input.fleet_count = 1;
    input.fleets = &fleet;
    input.ops = &ops;
    failures = !stars_do_cyber_ai_turn(&input);
    failures += trace.merge_count != 5 || trace.ensure_ai_count != 1 ||
        trace.ensure_shdefs_count != 1;
    failures += trace.visitor_count != 1 || trace.basic_count != 1 ||
        trace.fill_count != 1;
    failures += trace.transfers != expected_transfers ||
        trace.transfer_amount != expected_amount;
    failures += trace.moves != expected_moves ||
        trace.move_flags != expected_flags;
    failures += trace.starbase_moves != expected_starbase;
    failures += trace.queue_count != expected_queue ||
        trace.select_count != expected_select;
    failures += planet.packet_strength != expected_strength;
    if (scenario == CASE_SPACE)
        failures += trace.freighter_target_calls != 0;
    if (scenario == CASE_ENEMY)
        failures += trace.freighter_target_calls != 1;
    if (scenario == CASE_HIGH || scenario == CASE_PACKET_REJECT)
        failures += trace.packet_target_calls != 1;
    return failures;
}

int main(void)
{
    StarsI16 failures;

    failures = 0;
    failures += run_case(CASE_HIGH, 1, 1000L, 1, 0x11b0U, 0, 1, 1,
        389U);
    failures += run_case(CASE_LOW, 1, -1000L, 1, 0x11b1U, 0, 0, 0,
        501U);
    failures += run_case(CASE_ENEMY, 0, 0L, 0, 0U, 1, 0, 0, 501U);
    failures += run_case(CASE_SPACE, 0, 0L, 0, 0U, 0, 0, 0, 501U);
    failures += run_case(CASE_PACKET_REJECT, 1, 1000L, 1, 0x11b0U, 0,
        0, 0, 501U);
    if (failures == 0)
        puts("CYBER_AI_TURN_PACKET_FREIGHTER_EDGE_CHECKS=45 FAILURES=0 RESULT=PASS");
    else
        puts("CYBER_AI_TURN_PACKET_FREIGHTER_EDGE_CHECKS=45 FAILURES=1 RESULT=FAIL");
    return failures == 0 ? 0 : 1;
}
