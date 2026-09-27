#include "core.h"
#include "RETFLX.H"
#include <stdio.h>
#include <string.h>

/* One personality call over the four retained owner-1 fleets. The compact
 * input is materialized into semantic 124-byte fleet and 18-byte PLORD
 * views; these are not claimed to be captured original heap bytes. */
typedef struct Trace {
    StarsRototillScoutAdapter scout;
    StarsU8 ai[32U * 16U];
    StarsU8 status[32];
    StarsU8 queue[12];
    StarsI16 starts, binds, adds, finishes;
    StarsI16 selects, refreshes, transports;
    StarsI16 basic_tasks, fill_queue, bad;
} Trace;

static void word(StarsU8 *raw, StarsU16 offset, StarsU16 value)
{
    raw[offset] = (StarsU8)value;
    raw[offset + 1U] = (StarsU8)(value >> 8);
}

static void dword(StarsU8 *raw, StarsU16 offset, StarsU32 value)
{
    word(raw, offset, (StarsU16)value);
    word(raw, (StarsU16)(offset + 2U), (StarsU16)(value >> 16));
}

static StarsU16 read_word(const StarsU8 *raw, StarsU16 offset)
{
    return (StarsU16)(raw[offset] | ((StarsU16)raw[offset + 1U] << 8));
}

static Trace *turn_trace(void *opaque)
{
    StarsRototillScoutAdapter *scout;
    scout = (StarsRototillScoutAdapter *)opaque;
    return (Trace *)scout->context;
}

static StarsI16 ensure_ai(void *opaque, StarsI16 percent,
    StarsI16 *latest)
{
    Trace *trace;
    trace = turn_trace(opaque);
    if (percent != 0)
        trace->bad = 1;
    *latest = -1;
    return 0;
}

static StarsI16 ensure_shdefs(void *opaque, StarsI16 order)
{
    Trace *trace;
    trace = turn_trace(opaque);
    if (order != 0)
        trace->bad = 1;
    return 1;
}

static StarsI16 init_production(void *opaque,
    const StarsRototillTurnPlanet *planet)
{
    Trace *trace;
    trace = turn_trace(opaque);
    ++trace->starts;
    if (planet->id != 24 || trace->starts != 1)
        trace->bad = 1;
    return 1;
}

static StarsI16 current_queue(void *opaque,
    const StarsRototillTurnPlanet *planet, const StarsU8 **queue,
    StarsU16 *bytes)
{
    Trace *trace;
    trace = turn_trace(opaque);
    ++trace->binds;
    if (planet->id != 24 || trace->binds > 3)
        trace->bad = 1;
    *queue = trace->queue;
    *bytes = sizeof trace->queue;
    return 1;
}

static StarsI16 add_queue(void *opaque,
    const StarsRototillTurnPlanet *planet, StarsI16 design,
    StarsI16 count, StarsI16 kind, StarsI16 flag)
{
    Trace *trace;
    trace = turn_trace(opaque);
    ++trace->adds;
    if (planet->id != 24 || planet->raw_prod_queue != trace->queue ||
            design != 0 || count != 1 || kind != 2 || flag != 1 ||
            trace->adds > 2)
        trace->bad = 1;
    trace->queue[3] = (StarsU8)trace->adds;
    return 1;
}

static StarsI16 finish_production(void *opaque,
    const StarsRototillTurnPlanet *planet, StarsI16 changed)
{
    Trace *trace;
    trace = turn_trace(opaque);
    ++trace->finishes;
    if (planet->id != 24 || changed != 0 ||
            planet->raw_prod_queue != trace->queue ||
            trace->queue[3] != 2U)
        trace->bad = 1;
    return 1;
}

static StarsI16 transport(void *opaque,
    const StarsRototillTurnFleet *fleet)
{
    Trace *trace;
    trace = turn_trace(opaque);
    ++trace->transports;
    if (fleet->id < 0 || fleet->id >= 4)
        trace->bad = 1;
    return fleet->id == 1;
}

static StarsI16 race_stat(void *opaque, StarsI16 player, StarsI16 stat)
{
    Trace *trace;
    trace = turn_trace(opaque);
    if (player != 1 || stat != 14)
        trace->bad = 1;
    return 0;
}

static StarsI16 select_fleet(void *opaque,
    const StarsRototillTurnFleet *fleet, StarsI16 player)
{
    Trace *trace;
    trace = turn_trace(opaque);
    ++trace->selects;
    if (player != 1 || fleet->id != 0)
        trace->bad = 1;
    return 1;
}

static StarsI16 refresh_fleet(void *opaque,
    const StarsRototillTurnFleet *fleet, StarsI16 player)
{
    Trace *trace;
    trace = turn_trace(opaque);
    ++trace->refreshes;
    if (player != 1 || fleet->id != 0 ||
            fleet->raw_fleet_record[98] != 2U ||
            fleet->raw_plord[3] != 2U)
        trace->bad = 1;
    return 1;
}

static StarsI16 random_value(void *opaque, StarsI16 maximum)
{
    Trace *trace;
    trace = (Trace *)opaque;
    if (maximum != 100)
        trace->bad = 1;
    return 99;
}

static const StarsHullComponent *lookup_hull(void *opaque,
    StarsU16 owner, StarsU16 design, StarsU16 *count)
{
    Trace *trace;
    trace = (Trace *)opaque;
    if (owner != 1U || design != 0U)
        trace->bad = 1;
    *count = sizeof retained_hull_0 / sizeof retained_hull_0[0];
    return retained_hull_0;
}

static StarsI16 has_teeth(void *opaque,
    const StarsRototillTurnFleet *fleet)
{
    Trace *trace;
    trace = (Trace *)opaque;
    if (fleet->id != 0)
        trace->bad = 1;
    return stars_fleet_might_have_teeth(fleet->ship_counts,
        (StarsU16)fleet->owner << 9, lookup_hull, opaque);
}

static const StarsU8 *find_wormhole(void *opaque,
    StarsPoint point, StarsU32 distance)
{
    Trace *trace;
    trace = (Trace *)opaque;
    if (point.x != SOURCE_X || point.y != SOURCE_Y ||
            distance != 545UL)
        trace->bad = 1;
    return stars_lpth_worm_find(point, distance, 1,
        retained_wormholes, 4U, 0, random_value, opaque);
}

static const StarsIdealWarpEngine *lookup_engine(void *opaque,
    StarsU16 engine_id)
{
    static StarsIdealWarpEngine engine;
    StarsU16 index;
    Trace *trace;
    trace = (Trace *)opaque;
    if (engine_id != RETAINED_ENGINE_ID)
        trace->bad = 1;
    memset(&engine, 0, sizeof engine);
    for (index = 0U; index < 12U; ++index)
        engine.fuel_used[index] = retained_engine_fuel[index];
    return &engine;
}

static StarsI16 ideal_warp(void *opaque,
    const StarsRototillTurnFleet *fleet, StarsI16 target,
    StarsPoint point)
{
    StarsFindIdealWarpInput input;
    Trace *trace;
    trace = (Trace *)opaque;
    if (fleet->id != 0 || target != EXPECTED_TARGET ||
            point.x != EXPECTED_X || point.y != EXPECTED_Y)
        trace->bad = 1;
    memset(&input, 0, sizeof input);
    memcpy(input.ship_counts, fleet->ship_counts, sizeof input.ship_counts);
    input.designs[0].engine_count = 1U;
    input.designs[0].engine_flags[0] = 1U;
    input.designs[0].engine_ids[0] = RETAINED_ENGINE_ID;
    input.lookup_engine = lookup_engine;
    input.context = opaque;
    return stars_find_ideal_warp(&input, 1);
}

static StarsI16 run_basic(void *opaque, StarsI16 order,
    StarsI16 latest, StarsI16 player)
{
    Trace *trace;
    trace = turn_trace(opaque);
    ++trace->basic_tasks;
    if (order != 0 || latest != -1 || player != 1)
        trace->bad = 1;
    return 1;
}

static StarsI16 fill_queue(void *opaque)
{
    Trace *trace;
    trace = turn_trace(opaque);
    ++trace->fill_queue;
    return 1;
}

int main(void)
{
    StarsRototillAiTurnInput input;
    StarsRototillAiTurnOps ops;
    StarsRototillTurnPlanet planet;
    StarsRototillTurnFleet fleets[4];
    StarsU8 raw_fleets[4][124];
    StarsU8 orders[4][58];
    StarsU8 raw_planet[56];
    Trace trace;
    StarsI16 checks, failures, index, ship;

    checks = 0;
    failures = 0;
    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    memset(&planet, 0, sizeof planet);
    memset(fleets, 0, sizeof fleets);
    memset(raw_fleets, 0, sizeof raw_fleets);
    memset(orders, 0, sizeof orders);
    memset(raw_planet, 0, sizeof raw_planet);
    memset(&trace, 0, sizeof trace);
    word(raw_planet, 0U, 24U);
    word(raw_planet, 2U, 1U);
    word(raw_planet, 4U, RETAINED_PLANET_FLAGS);
    raw_planet[9] = RETAINED_CONCENTRATIONS[0];
    raw_planet[10] = RETAINED_CONCENTRATIONS[1];
    raw_planet[11] = RETAINED_CONCENTRATIONS[2];
    for (index = 0; index < 4; ++index)
        dword(raw_planet, (StarsU16)(28 + index * 4),
            (StarsU32)RETAINED_PLANET_MINERALS[index]);
    planet.id = 24;
    planet.owner = 1;
    planet.raw_planet_record = raw_planet;
    planet.raw_planet_record_size = sizeof raw_planet;
    for (index = 0; index < 4; ++index) {
        StarsU8 *raw;
        StarsU8 *plord;
        raw = raw_fleets[index];
        plord = orders[index];
        word(raw, 0U, (StarsU16)index);
        word(raw, 2U, 1U);
        word(raw, 4U, (StarsU16)RETAINED_FLEET_FLAGS[index]);
        word(raw, 6U, (StarsU16)RETAINED_FLEET_PLANETS[index]);
        word(raw, 8U, (StarsU16)RETAINED_FLEET_X[index]);
        word(raw, 10U, (StarsU16)RETAINED_FLEET_Y[index]);
        for (ship = 0; ship < 16; ++ship)
            word(raw, (StarsU16)(12 + ship * 2),
                (StarsU16)RETAINED_SHIPS[index][ship]);
        word(raw, 88U, (StarsU16)RETAINED_CARGO[index][3]);
        word(raw, 92U, (StarsU16)RETAINED_CARGO[index][4]);
        raw[98] = 1U;
        plord[3] = 1U;
        word(plord, 4U, (StarsU16)RETAINED_ORDER_X[index]);
        word(plord, 6U, (StarsU16)RETAINED_ORDER_Y[index]);
        word(plord, 8U, (StarsU16)RETAINED_ORDER_TARGET[index]);
        word(plord, 10U, (StarsU16)RETAINED_ORDER_FLAGS[index]);
        fleets[index].id = index;
        fleets[index].raw_fleet_record = raw;
        fleets[index].raw_fleet_record_size = sizeof raw_fleets[0];
        fleets[index].raw_plord = plord;
        fleets[index].raw_plord_bytes = sizeof orders[0];
    }
    trace.scout.raw_ai_planet_table = trace.ai;
    trace.scout.ai_planet_record_count = 32U;
    trace.scout.planet_points = retained_points;
    trace.scout.planet_count = 32U;
    trace.scout.planet_owners = retained_owners;
    trace.scout.status_scratch = trace.status;
    trace.scout.status_scratch_count = 32U;
    trace.scout.race_mode = 3;
    trace.scout.turn = 0;
    trace.scout.random_value = random_value;
    trace.scout.find_wormhole = find_wormhole;
    trace.scout.has_teeth = has_teeth;
    trace.scout.ideal_warp = ideal_warp;
    trace.scout.select_fleet = select_fleet;
    trace.scout.refresh_fleet = refresh_fleet;
    trace.scout.context = &trace;
    ops.ensure_ai = ensure_ai;
    ops.ensure_shdefs = ensure_shdefs;
    ops.init_production = init_production;
    ops.current_prod_queue = current_queue;
    ops.add_raw_queue_item = add_queue;
    ops.finish_production = finish_production;
    ops.is_ai_transport = transport;
    ops.race_stat = race_stat;
    ops.select_fleet = select_fleet;
    ops.refresh_fleet = refresh_fleet;
    ops.target_scout = stars_rototill_target_scout;
    ops.run_basic_tasks = run_basic;
    ops.fill_production_queue = fill_queue;
    ops.context = &trace.scout;
    input.turn = 0;
    input.player_id = 1;
    input.planet_count = 1;
    input.planets = &planet;
    input.fleet_count = 4;
    input.fleets = fleets;
    input.ops = &ops;
    input.production_inputs_bound = 1;
    input.raw_ai_planet_table = trace.ai;
    input.raw_ai_planet_record_count = 32U;
    input.fleet_unowned_marker_bound = 1;
    input.fleet_occupied_repair_bound = 1;
    input.fleet_transport_reroute_bound = 1;
    input.fleet_first_kind_selected_planet_bound = 1;
    input.fleet_scout_bound = 1;
    input.planet_points = retained_points;
    input.planet_point_count = 32U;
    input.ship0_first_hull_slot_byte = 1U;

#define CHECK(value) do { ++checks; if (!(value)) { ++failures; \
    printf("FAILED_CHECK_LINE=%d\n", __LINE__); } } while (0)
    CHECK(stars_do_rototill_ai_turn(&input) == 1);
    CHECK(!trace.bad && input.production_items_added == 0 &&
          trace.starts == 0 && trace.binds == 0 &&
          trace.adds == 0 && trace.finishes == 0);
    CHECK(trace.queue[3] == 0U && input.queue_bypass_count == 0);
    CHECK(trace.selects == 2 && trace.refreshes == 1 &&
          trace.transports == 7 && trace.basic_tasks == 1 &&
          trace.fill_queue == 1);
    CHECK(input.unowned_viable_count == 0 &&
          input.owned_bad_count == 0 &&
          trace.ai[24U * 16U + 9U] == 1U);
    CHECK(raw_fleets[0][98] == 2U && orders[0][3] == 2U &&
          read_word(orders[0], 22U + 4U) == EXPECTED_TARGET &&
          read_word(orders[0], 22U + 6U) == 0x1150U);
    CHECK(read_word(orders[0], 22U) == EXPECTED_X &&
          read_word(orders[0], 24U) == EXPECTED_Y);
    for (index = 1; index < 4; ++index) {
        CHECK(raw_fleets[index][98] == 1U && orders[index][3] == 1U &&
              read_word(orders[index], 8U) == 24U);
    }
    CHECK(trace.ai[24U * 16U + 15U] == 1U &&
          trace.ai[EXPECTED_TARGET * 16U + 15U] == 4U);
    printf("ROTOTILL_RETAINED_AI_PHASE_CHECKS=%d FAILURES=%d RESULT=%s\n",
           checks, failures, failures ? "FAIL" : "PASS");
    return failures ? 1 : 0;
}
