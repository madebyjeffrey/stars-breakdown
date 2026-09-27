#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsU8 *plord;
    StarsPoint fleet_point;
    StarsI16 merge_count;
    StarsI16 ensure_ai_count;
    StarsI16 ensure_shdefs_count;
    StarsI16 freighter_count;
    StarsI16 transfer_count;
    StarsI16 move_count;
    StarsI16 visitor_count;
    StarsI16 basic_count;
    StarsI16 packet_count;
    StarsI16 queue_count;
    StarsI16 select_count;
    StarsI16 fill_count;
    StarsI16 event;
    StarsI32 transfer_amount;
    StarsU16 move_flags;
    StarsI16 queue_item;
    StarsI16 queue_items;
    StarsU16 packet_target;
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
    if (id != 6)
        return 0;
    memset(planet, 0, sizeof *planet);
    planet->id = 6;
    planet->owner = 4;
    planet->point = trace->fleet_point;
    planet->mining_rate = 1;
    planet->mineral_stock = 5000;
    return 1;
}

static StarsI16 freighter_target(void *context,
    const StarsCyberFreighterPlanet *source, StarsI16 kind,
    StarsCyberFreighterPlanet *target)
{
    Trace *trace;

    trace = (Trace *)context;
    if (source->id != 6 || kind != 1)
        return 0;
    memset(target, 0, sizeof *target);
    target->id = 9;
    target->owner = 4;
    target->point.x = 20;
    target->point.y = 20;
    trace->event = 1;
    return 1;
}

static void transfer_supply(void *context, StarsI16 player,
    StarsI16 planet, StarsI16 cargo_type, StarsI16 resource_type,
    StarsI32 amount)
{
    Trace *trace;

    trace = (Trace *)context;
    if (player != 4 || planet != 6 || cargo_type != 2 ||
            resource_type != 3)
        return;
    trace->transfer_amount = amount;
    ++trace->transfer_count;
    trace->event = 2;
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

    trace = (Trace *)context;
    if (kind != 1U || target != 9U || point.x != 20 || point.y != 20 ||
            flags != 0x11b0U)
        return 0;
    memset(&order, 0, sizeof order);
    order.id = (StarsI16)target;
    order.point = point;
    order.flags = flags;
    if (!stars_ai_move_order_to_plord(trace->plord, 2, &order, 0))
        return 0;
    ++trace->move_count;
    trace->move_flags = flags;
    trace->event = 3;
    (void)fleet;
    return 1;
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
    if (fleet->id != 8 || fleet->planet_id != 6 ||
            policy->transport_ships != 2 || potency != 4)
        return 0;
    memset(&freighter, 0, sizeof freighter);
    memset(&plan, 0, sizeof plan);
    memset(&ops, 0, sizeof ops);
    freighter.id = (StarsU16)fleet->id;
    freighter.planet_id = fleet->planet_id;
    freighter.point = trace->fleet_point;
    ops.lookup_planet = lookup_planet;
    ops.find_target = freighter_target;
    ops.transfer_supply = transfer_supply;
    ops.ideal_warp = ideal_warp;
    ops.move_fleet = move_fleet;
    ops.context = trace;
    ++trace->freighter_count;
    trace->event = 0;
    stars_do_cyber_freighter(&freighter, &plan, 4, &ops);
    return 1;
}

static StarsI16 visit_fleet(void *context,
    const StarsCyberTurnFleet *fleet, const StarsCyberFleetPolicy *policy,
    StarsU8 potency)
{
    Trace *trace;

    trace = (Trace *)context;
    if (fleet->id != 8 || policy->total_ships != 2 || potency != 4 ||
            trace->event != 3)
        return 0;
    ++trace->visitor_count;
    trace->event = 4;
    return 1;
}

static StarsI16 basic(void *context, StarsI16 research_order,
    StarsI16 latest_starbase_design, StarsI16 player_id)
{
    Trace *trace;

    trace = (Trace *)context;
    if (research_order != 23 || latest_starbase_design != 4 ||
            player_id != 4 || trace->event != 4)
        return 0;
    ++trace->basic_count;
    trace->event = 5;
    return 1;
}

static StarsI16 packet_target(void *context,
    const StarsCyberPacketPlanet *source, StarsU16 *target)
{
    Trace *trace;

    trace = (Trace *)context;
    if (source->id != 6 || source->owner != 4)
        return 0;
    *target = 9U;
    trace->packet_target = *target;
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
    trace->queue_item = item;
    trace->queue_items = count;
    trace->event = 6;
    return 1;
}

static void packet_select(void *context, StarsU16 source, StarsU16 target)
{
    Trace *trace;

    trace = (Trace *)context;
    if (source == 6U && target == 9U)
        ++trace->select_count;
    trace->event = 7;
}

static StarsI16 run_packets(void *context, StarsI16 player_id,
    StarsCyberTurnPlanet *planets, StarsI16 planet_count,
    StarsI16 packet_pressure)
{
    Trace *trace;
    StarsCyberPacketPlanet packet_planets[1];
    StarsDoCyberPacketsOps packet_ops;
    StarsDoCyberPacketsInput packet_input;

    trace = (Trace *)context;
    if (player_id != 4 || planet_count != 1 || packet_pressure != 16 ||
            !planets || trace->event != 5)
        return 0;
    memset(packet_planets, 0, sizeof packet_planets);
    memset(&packet_ops, 0, sizeof packet_ops);
    packet_planets[0].id = (StarsU16)planets[0].id;
    packet_planets[0].owner = planets[0].owner;
    packet_planets[0].packet_strength = planets[0].packet_strength;
    packet_planets[0].packet_flags = 0x0080U;
    packet_planets[0].available[0] = 1000UL;
    packet_planets[0].available[1] = 800UL;
    packet_planets[0].available[2] = 600UL;
    packet_planets[0].cost[0] = 900UL;
    packet_planets[0].cost[1] = 100UL;
    packet_planets[0].cost[2] = 500UL;
    packet_ops.find_target = packet_target;
    packet_ops.queue_item = packet_queue;
    packet_ops.select_planet = packet_select;
    packet_ops.context = trace;
    packet_input.planets = packet_planets;
    packet_input.planet_count = 1;
    packet_input.player = player_id;
    packet_input.minimum_strength = (StarsU16)packet_pressure;
    packet_input.maximum_packets = 0;
    packet_input.ops = &packet_ops;
    ++trace->packet_count;
    stars_do_cyber_packets(&packet_input);
    planets[0].packet_strength = packet_planets[0].packet_strength;
    return 1;
}

static StarsI16 fill(void *context)
{
    Trace *trace;

    trace = (Trace *)context;
    if (trace->event != 7)
        return 0;
    ++trace->fill_count;
    trace->event = 8;
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

int main(void)
{
    StarsCyberTurnPlanet planet;
    StarsCyberTurnFleet fleet;
    StarsCyberAiTurnOps ops;
    StarsCyberAiTurnInput input;
    StarsU8 plord[STARS_AI_PLORD_HEADER_SIZE +
        2U * STARS_AI_PLORD_ORDER_SIZE];
    StarsMoveOrder expected;
    Trace trace;
    StarsI16 failures;

    memset(&planet, 0, sizeof planet);
    memset(&fleet, 0, sizeof fleet);
    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    memset(plord, 0, sizeof plord);
    planet.id = 6;
    planet.owner = 4;
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
    trace.plord = plord;
    trace.fleet_point.x = 10;
    trace.fleet_point.y = 10;
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
    failures += trace.merge_count != 5;
    failures += trace.ensure_ai_count != 1 || trace.ensure_shdefs_count != 1;
    failures += trace.freighter_count != 1 || trace.transfer_count != 1;
    failures += trace.transfer_amount != 1000L || trace.move_count != 1;
    failures += trace.move_flags != 0x11b0U || trace.visitor_count != 1;
    failures += trace.basic_count != 1 || trace.packet_count != 1;
    failures += trace.queue_count != 1 || trace.queue_item != 15 ||
        trace.queue_items != 7;
    failures += trace.packet_target != 9U || trace.select_count != 1;
    failures += planet.packet_strength != 389U;
    failures += trace.fill_count != 1 || trace.event != 8;
    failures += plord[3] != 1U;
    memset(&expected, 0, sizeof expected);
    expected.id = 9;
    expected.point.x = 20;
    expected.point.y = 20;
    expected.flags = 0x11b0U;
    failures += memcmp(plord + STARS_AI_PLORD_HEADER_SIZE, &expected,
        sizeof expected) != 0;
    if (failures == 0)
        puts("CYBER_AI_TURN_PACKET_FREIGHTER_COMPOSITION_CHECKS=20 FAILURES=0 RESULT=PASS");
    else
        puts("CYBER_AI_TURN_PACKET_FREIGHTER_COMPOSITION_CHECKS=20 FAILURES=1 RESULT=FAIL");
    return failures == 0 ? 0 : 1;
}
