#include "core.h"
#include <stdio.h>
#include <string.h>

/* Bind only fields recovered from the packed Win16 records.  Habitat and
 * mineral/service values, production costs, and detailed order decisions
 * remain typed service results; this adapter does not invent offsets for
 * them. */
#define RAW_PLANET_BYTES 56
#define RAW_FLEET_BYTES 124

static StarsU16 raw_u16(const StarsU8 *raw, StarsI16 offset)
{
    return (StarsU16)(raw[offset] | ((StarsU16)raw[offset + 1] << 8));
}

static StarsI16 raw_i16(const StarsU8 *raw, StarsI16 offset)
{
    return (StarsI16)raw_u16(raw, offset);
}

static void put_i16(StarsU8 *raw, StarsI16 offset, StarsI16 value)
{
    StarsU16 word;

    word = (StarsU16)value;
    raw[offset] = (StarsU8)(word & 0xffU);
    raw[offset + 1] = (StarsU8)(word >> 8);
}

static void put_planet(StarsU8 *raw, StarsI16 id, StarsI16 owner,
    StarsU16 flags, StarsU16 packet_strength)
{
    memset(raw, 0, RAW_PLANET_BYTES);
    put_i16(raw, 0, id);
    put_i16(raw, 2, owner);
    put_i16(raw, 4, (StarsI16)flags);
    put_i16(raw, 18, (StarsI16)(packet_strength & 0x0fffU));
}

static StarsI16 raw_planet_view(const StarsU8 *raw, StarsU16 habitat_kind,
    const StarsU32 minerals[3], const StarsI32 mineral_rates[3],
    const StarsI32 resources[4], const StarsI32 queue_cost[4],
    StarsCyberTurnPlanet *out)
{
    StarsI16 i;

    memset(out, 0, sizeof *out);
    out->id = raw_i16(raw, 0);
    out->owner = raw_i16(raw, 2);
    out->habitat_kind = habitat_kind;
    out->starbase_flags = raw_u16(raw, 4);
    out->packet_strength = (StarsU16)(raw_u16(raw, 18) & 0x0fffU);
    out->visibility_flags = raw_u16(raw, 4);
    for (i = 0; i < 3; ++i) {
        out->minerals[i] = minerals[i];
        out->mineral_rates[i] = mineral_rates[i];
    }
    for (i = 0; i < 4; ++i) {
        out->resources_available[i] = resources[i];
        out->production_queue_cost[i] = queue_cost[i];
    }
    return out->id >= 0;
}

static void put_fleet(StarsU8 *raw, StarsI16 id, StarsI16 owner,
    StarsI16 planet_id, StarsI16 cord, const StarsI16 counts[16])
{
    StarsI16 i;

    memset(raw, 0, RAW_FLEET_BYTES);
    put_i16(raw, 0, id);
    put_i16(raw, 2, owner);
    put_i16(raw, 6, planet_id);
    put_i16(raw, 98, cord);
    for (i = 0; i < 16; ++i)
        put_i16(raw, (StarsI16)(12 + i * 2), counts[i]);
}

static StarsI16 raw_fleet_view(const StarsU8 *raw, StarsI16 order_kind,
    StarsCyberTurnFleet *out)
{
    StarsI16 i;
    StarsI16 count;

    memset(out, 0, sizeof *out);
    out->id = raw_i16(raw, 0);
    out->owner = raw_i16(raw, 2);
    out->planet_id = raw_i16(raw, 6);
    out->cord = raw_i16(raw, 98);
    out->order_kind = order_kind;
    for (i = 0; i < 16; ++i) {
        count = raw_i16(raw, (StarsI16)(12 + i * 2));
        if (count < 0 || count > 255)
            return 0;
        out->ship_counts[i] = (StarsU8)count;
    }
    return out->id >= 0;
}

typedef struct Trace {
    StarsI16 merge_count;
    StarsI16 merge_masks[5];
    StarsI16 status_count;
    StarsI16 status_budget;
    StarsI16 split_count;
    StarsI16 ensure_ai_count;
    StarsI16 ensure_shdefs_count;
    StarsI16 marks;
    StarsU16 first_low_mask;
    StarsI16 hostile_count;
    StarsI16 production_count;
    StarsI16 fleet_count;
    StarsI16 basic_count;
    StarsI16 packet_count;
    StarsI16 packet_planet_count;
    StarsI16 packet_pressure;
    StarsU16 packet_second_strength;
    StarsI16 fill_count;
    StarsI16 valid_policy;
    StarsI16 freighter_count;
} Trace;

static StarsI16 merge(void *context, StarsI16 mask)
{
    Trace *trace;

    trace = (Trace *)context;
    if (trace->merge_count >= 5)
        return 0;
    trace->merge_masks[trace->merge_count] = mask;
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

static StarsI16 status_ex(void *context, StarsI16 first, StarsI16 last,
    StarsI16 budget, StarsI16 *selected, StarsU8 mask[16])
{
    Trace *trace;

    trace = (Trace *)context;
    if (last != first + 1 || budget != 70)
        return 0;
    if (first != 4 && first != 14 && first != 2)
        return 0;
    *selected = 12;
    mask[first] = (StarsU8)(first + 1);
    ++trace->status_count;
    trace->status_budget = budget;
    return 1;
}

static StarsI16 split(void *context, const StarsU8 mask[16])
{
    Trace *trace;

    trace = (Trace *)context;
    if (trace->split_count == 0) {
        if (mask[4] != 5U || mask[14] != 15U || mask[2] != 3U)
            return 0;
    } else if (trace->split_count == 1) {
        if (mask[0] != 2U)
            return 0;
    } else if (trace->split_count == 2) {
        if (mask[1] != 2U)
            return 0;
    } else if (trace->split_count == 3) {
        if (mask[2] != 2U || mask[3] != 2U)
            return 0;
    } else
        return 0;
    ++trace->split_count;
    return 1;
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

static StarsI16 mark_planet(void *context, const StarsCyberTurnPlanet *planet,
    StarsU16 low_mask, StarsI16 player_id)
{
    Trace *trace;

    trace = (Trace *)context;
    if (player_id != 4 || planet->id < 0)
        return 0;
    if (trace->marks == 0)
        trace->first_low_mask = low_mask;
    ++trace->marks;
    return 1;
}

static StarsI16 mark_hostile(void *context,
    const StarsCyberTurnPlanet *planet, StarsI16 player_id,
    StarsI16 threat_level)
{
    Trace *trace;

    trace = (Trace *)context;
    if (planet->owner != 7 || player_id != 4 || threat_level != 4)
        return 0;
    ++trace->hostile_count;
    return 1;
}

static StarsI16 production(void *context, const StarsCyberTurnPlanet *planet,
    StarsI16 budget)
{
    Trace *trace;

    trace = (Trace *)context;
    if (planet->id != 4 || budget != 70)
        return 0;
    ++trace->production_count;
    return 1;
}

static StarsI16 fleet_typed(void *context, const StarsCyberTurnFleet *fleet,
    const StarsCyberFleetPolicy *policy, StarsU8 potency)
{
    Trace *trace;

    trace = (Trace *)context;
    if (fleet->id != 8 || fleet->owner != 4 || fleet->planet_id != 6 ||
            potency != 4 || policy->total_ships != 15 ||
            policy->colonizer_ships != 1 || policy->transport_ships != 2 ||
            policy->armed_ships != 3 ||
            policy->auxiliary_design_ships != 3 ||
            policy->support_design_ships != 3 ||
            policy->attack_design_ships != 3 ||
            !policy->needs_order_repair)
        return 0;
    ++trace->fleet_count;
    ++trace->valid_policy;
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

static StarsI16 packets_ex(void *context, StarsI16 player_id,
    StarsCyberTurnPlanet *planets, StarsI16 planet_count,
    StarsI16 packet_pressure)
{
    Trace *trace;

    trace = (Trace *)context;
    if (player_id != 4 || !planets || planet_count != 2 ||
            packet_pressure != 16)
        return 0;
    ++trace->packet_count;
    trace->packet_planet_count = planet_count;
    trace->packet_pressure = packet_pressure;
    trace->packet_second_strength = planets[1].packet_strength;
    planets[1].packet_strength = (StarsU16)(planets[1].packet_strength -
        100U);
    return 1;
}

static StarsI16 freighter_ex(void *context,
    const StarsCyberTurnFleet *fleet, const StarsCyberFleetPolicy *policy,
    StarsU8 potency)
{
    Trace *trace;

    trace = (Trace *)context;
    if (fleet->id != 8 || fleet->ship_counts[1] != 2 ||
            policy->transport_ships != 2 || potency != 4)
        return 0;
    ++trace->freighter_count;
    return 1;
}

static StarsI16 fill(void *context)
{
    Trace *trace;

    trace = (Trace *)context;
    ++trace->fill_count;
    return 1;
}

int main(void)
{
    StarsU8 planet_raw[2][RAW_PLANET_BYTES];
    StarsU8 fleet_raw[2][RAW_FLEET_BYTES];
    StarsU32 minerals[3];
    StarsI32 mineral_rates[3];
    StarsI32 resources[4];
    StarsI32 queue_cost[4];
    StarsI16 counts[16];
    StarsCyberTurnPlanet planets[2];
    StarsCyberTurnFleet fleets[2];
    StarsCyberAiTurnOps ops;
    StarsCyberAiTurnInput input;
    Trace trace;
    StarsI16 i;
    StarsI16 failures;

    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    for (i = 0; i < 3; ++i) {
        minerals[i] = i == 0 ? 0 : 20;
        mineral_rates[i] = i == 0 ? 0 : 1;
    }
    for (i = 0; i < 4; ++i) {
        resources[i] = 100;
        queue_cost[i] = 10;
    }
    for (i = 0; i < 16; ++i)
        counts[i] = 0;
    counts[0] = 1;
    counts[1] = 2;
    counts[2] = 3;
    counts[10] = 2;
    counts[11] = 1;
    counts[12] = 2;
    counts[13] = 1;
    counts[14] = 2;
    counts[15] = 1;
    put_planet(planet_raw[0], 4, 4, 0x0200U, 0);
    put_planet(planet_raw[1], 9, 7, 0x0200U, 501);
    put_fleet(fleet_raw[0], 8, 4, 6, 0, counts);
    put_fleet(fleet_raw[1], 10, 7, 6, 0, counts);
    if (!raw_planet_view(planet_raw[0], 1, minerals, mineral_rates,
            resources, queue_cost, &planets[0]) ||
            !raw_planet_view(planet_raw[1], 2, minerals, mineral_rates,
            resources, queue_cost, &planets[1]) ||
            !raw_fleet_view(fleet_raw[0], 0, &fleets[0]) ||
            !raw_fleet_view(fleet_raw[1], 0, &fleets[1]))
        return 1;
    failures = 0;
    failures += planets[0].id != 4 || planets[0].owner != 4;
    failures += planets[1].starbase_flags != 0x0200U ||
        planets[1].packet_strength != 501;
    failures += fleets[0].id != 8 || fleets[0].owner != 4 ||
        fleets[0].planet_id != 6 || fleets[0].cord != 0;
    failures += fleets[0].ship_counts[0] != 1 ||
        fleets[0].ship_counts[3] != 0 || fleets[0].ship_counts[12] != 2 ||
        fleets[0].ship_counts[15] != 1;
    ops.merge_shdefs = merge;
    ops.ensure_ai = ensure_ai;
    ops.check_design_status_ex = status_ex;
    ops.split_shdefs = split;
    ops.ensure_shdefs = ensure_shdefs;
    ops.mark_planet = mark_planet;
    ops.mark_hostile_planet = mark_hostile;
    ops.visit_production = production;
    ops.visit_fleet_typed = fleet_typed;
    ops.run_freighter_ex = freighter_ex;
    ops.run_basic_tasks_with_research = basic;
    ops.run_packets_ex = packets_ex;
    ops.fill_production_queue = fill;
    ops.context = &trace;
    input.turn = 150;
    input.player_id = 4;
    input.planet_count = 2;
    input.planets = planets;
    input.fleet_count = 2;
    input.fleets = fleets;
    input.ops = &ops;
    failures += !stars_do_cyber_ai_turn(&input);
    failures += trace.merge_count != 5 || trace.merge_masks[0] != 1 ||
        trace.merge_masks[1] != 0x30 || trace.merge_masks[2] != -0x4000 ||
        trace.merge_masks[3] != 0x03c0 || trace.merge_masks[4] != 0x3c00;
    failures += trace.status_count != 3 || trace.status_budget != 70 ||
        trace.split_count != 4;
    failures += trace.ensure_ai_count != 1 ||
        trace.ensure_shdefs_count != 1 || input.research_order != 23 ||
        input.latest_starbase_design != 4;
    failures += trace.marks != 2 || trace.first_low_mask != 0x0100U ||
        trace.hostile_count != 1;
    failures += trace.production_count != 1 ||
        input.production_affordable_count != 1 ||
        input.production_blocked_count != 0;
    failures += trace.fleet_count != 1 || trace.valid_policy != 1 ||
        trace.basic_count != 1 || trace.packet_count != 1 ||
        trace.packet_planet_count != 2 || trace.packet_pressure != 16 ||
        trace.packet_second_strength != 501U ||
        trace.fill_count != 1 || trace.freighter_count != 1;
    failures += planets[1].packet_strength != 401U;
    failures += input.packet_pressure != 16 || input.low_mineral_planets != 1 ||
        input.armada_potency[0] != 4 || input.armada_potency[1] != 2 ||
        input.armada_potency[2] != 8 || input.armada_potency[3] != 3;
    if (failures == 0)
        puts("CYBER_AI_TURN_INTEGRATION_CHECKS=31 FAILURES=0 RESULT=PASS");
    else
        puts("CYBER_AI_TURN_INTEGRATION_CHECKS=31 FAILURES=1 RESULT=FAIL");
    return failures == 0 ? 0 : 1;
}
