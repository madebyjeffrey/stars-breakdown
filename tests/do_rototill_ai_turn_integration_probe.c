#include "core.h"
#include <stdio.h>
#include <string.h>

/* The original records are packed Win16 objects.  This adapter binds only
 * fields recovered from the CodeView layouts; optimal value, desirability,
 * and productive status are supplied as the service results that the raw
 * procedure obtains from PctPlanetOptValue/PctPlanetDesirability and its
 * production gate. */
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
    StarsU8 visibility, StarsU8 a, StarsU8 b, StarsU8 c)
{
    memset(raw, 0, RAW_PLANET_BYTES);
    put_i16(raw, 0, id);
    put_i16(raw, 2, owner);
    raw[4] = visibility;
    raw[9] = a;
    raw[10] = b;
    raw[11] = c;
}

static StarsI16 raw_planet_view(const StarsU8 *raw, StarsI16 optimal,
    StarsI16 desirability, StarsU16 productive, StarsRototillTurnPlanet *out)
{
    StarsI16 i;
    memset(out, 0, sizeof *out);
    out->id = raw_i16(raw, 0);
    out->owner = raw_i16(raw, 2);
    out->visibility_flags = (StarsU16)raw[4];
    out->planet_class = (StarsI16)(StarsU16)raw[4];
    for (i = 0; i < 3; ++i)
        out->mineral_concentration[i] = raw[9 + i];
    out->optimal_value = optimal;
    out->desirability = desirability;
    out->productive = productive;
    return out->id >= 0;
}

static void put_fleet(StarsU8 *raw, StarsI16 id, StarsI16 owner,
    StarsI16 planet_id, StarsI16 cord, StarsI16 order_kind,
    const StarsI16 *ship_counts)
{
    StarsI16 i;
    memset(raw, 0, RAW_FLEET_BYTES);
    put_i16(raw, 0, id);
    put_i16(raw, 2, owner);
    put_i16(raw, 6, planet_id);
    put_i16(raw, 98, cord);
    for (i = 0; i < 16; ++i)
        put_i16(raw, (StarsI16)(12 + i * 2), ship_counts[i]);
    /* The typed boundary keeps the abstract order discriminator explicit;
     * the raw PLORD pointer/record is intentionally not claimed here. */
    (void)order_kind;
}

static StarsI16 raw_fleet_view(const StarsU8 *raw, StarsI16 order_kind,
    StarsRototillTurnFleet *out)
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
    StarsI16 ensure_ai_calls;
    StarsI16 ensure_shdefs_calls;
    StarsI16 planet_calls;
    StarsI16 fleet_calls;
    StarsI16 repair_calls;
    StarsI16 basic_calls;
    StarsI16 queue_calls;
    StarsI16 first_planet_id;
    StarsI16 first_fleet_id;
    StarsU16 first_score;
    StarsI16 valid_policy;
} Trace;

static StarsI16 ensure_ai(void *context, StarsI16 research_percent,
    StarsI16 *latest_starbase_design)
{
    Trace *trace;
    trace = (Trace *)context;
    ++trace->ensure_ai_calls;
    if (research_percent != 15)
        return -1;
    *latest_starbase_design = 23;
    return 41;
}

static StarsI16 ensure_shdefs(void *context, StarsI16 order)
{
    Trace *trace;
    trace = (Trace *)context;
    ++trace->ensure_shdefs_calls;
    return order == 41;
}

static StarsI16 visit_planet(void *context,
    const StarsRototillTurnPlanet *planet, StarsU16 score, StarsI16 player_id)
{
    Trace *trace;
    trace = (Trace *)context;
    if (trace->planet_calls == 0)
        trace->first_planet_id = planet->id;
    if (trace->planet_calls == 0 && score != 127)
        return 0;
    if (planet->owner == player_id && planet->desirability >= 0 &&
            !planet->productive)
        return 0;
    ++trace->planet_calls;
    if (trace->planet_calls == 1)
        trace->first_score = score;
    return 1;
}

static StarsI16 repair_order(void *context,
    const StarsRototillTurnFleet *fleet, StarsI16 player_id)
{
    Trace *trace;
    trace = (Trace *)context;
    if (fleet->owner != player_id || fleet->id != 19)
        return 0;
    ++trace->repair_calls;
    return 1;
}

static StarsI16 visit_fleet(void *context,
    const StarsRototillTurnFleet *fleet, StarsI16 player_id)
{
    Trace *trace;
    trace = (Trace *)context;
    if (fleet->owner != player_id)
        return 0;
    ++trace->fleet_calls;
    if (trace->fleet_calls == 1)
        trace->first_fleet_id = fleet->id;
    return 1;
}

static StarsI16 visit_fleet_typed(void *context,
    const StarsRototillTurnFleet *fleet,
    const StarsRototillFleetPolicy *policy, StarsI16 player_id)
{
    Trace *trace;
    trace = (Trace *)context;
    if (fleet->owner != player_id || fleet->id != 19)
        return 0;
    if (policy->colonizer_ships != 1 || policy->transport_ships != 2 ||
            policy->armed_ships != 3 || policy->total_ships != 6 ||
            !policy->needs_order_repair)
        return 0;
    ++trace->valid_policy;
    return 1;
}

static StarsI16 run_basic(void *context, StarsI16 order,
    StarsI16 latest_starbase_design, StarsI16 player_id)
{
    Trace *trace;
    trace = (Trace *)context;
    ++trace->basic_calls;
    return order == 41 && latest_starbase_design == 23 && player_id == 4;
}

static StarsI16 fill_queue(void *context)
{
    Trace *trace;
    trace = (Trace *)context;
    ++trace->queue_calls;
    return 1;
}

static int run_case(void)
{
    StarsU8 planet_raw[3][RAW_PLANET_BYTES];
    StarsU8 fleet_raw[RAW_FLEET_BYTES];
    StarsI16 ship_counts[16];
    StarsRototillTurnPlanet planets[3];
    StarsRototillTurnFleet fleet;
    StarsRototillAiTurnOps ops;
    StarsRototillAiTurnInput input;
    Trace trace;
    StarsI16 i;

    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    for (i = 0; i < 16; ++i)
        ship_counts[i] = 0;
    ship_counts[0] = 1;
    ship_counts[1] = 2;
    ship_counts[2] = 3;
    put_planet(planet_raw[0], 3, -1, 3, 80, 90, 100);
    put_planet(planet_raw[1], 4, 4, 3, 20, 20, 20);
    put_planet(planet_raw[2], 8, -1, 1, 100, 100, 100);
    if (!raw_planet_view(planet_raw[0], 1, 0, 0, &planets[0]) ||
            !raw_planet_view(planet_raw[1], 0, -1, 1, &planets[1]) ||
            !raw_planet_view(planet_raw[2], 1, 0, 0, &planets[2]))
        return 1;
    put_fleet(fleet_raw, 19, 4, 3, 0, 0, ship_counts);
    if (!raw_fleet_view(fleet_raw, 0, &fleet))
        return 2;
    if (fleet.id != 19 || fleet.owner != 4 || fleet.planet_id != 3 ||
            fleet.cord != 0 || fleet.ship_counts[0] != 1 ||
            fleet.ship_counts[1] != 2 || fleet.ship_counts[2] != 3)
        return 3;
    ops.ensure_ai = ensure_ai;
    ops.ensure_shdefs = ensure_shdefs;
    ops.visit_planet = visit_planet;
    ops.visit_fleet = visit_fleet;
    ops.visit_fleet_typed = visit_fleet_typed;
    ops.repair_order = repair_order;
    ops.run_basic_tasks = run_basic;
    ops.fill_production_queue = fill_queue;
    ops.context = &trace;
    input.turn = 30;
    input.player_id = 4;
    input.planet_count = 3;
    input.planets = planets;
    input.fleet_count = 1;
    input.fleets = &fleet;
    input.ops = &ops;
    if (!stars_do_rototill_ai_turn(&input))
        return 4;
    if (trace.ensure_ai_calls != 1 || trace.ensure_shdefs_calls != 1 ||
            trace.planet_calls != 3 || trace.first_planet_id != 3 ||
            trace.first_score != 127 || trace.fleet_calls != 1 ||
            trace.first_fleet_id != 19 || trace.valid_policy != 1 ||
            trace.repair_calls != 1 || trace.basic_calls != 1 ||
            trace.queue_calls != 1 || input.unowned_viable_count != 1 ||
            input.owned_bad_count != 1 || input.productive_count != 1 ||
            input.mineral_score != 127 || input.current_order != 41 ||
            input.latest_starbase_design != 23)
        return 5;
    return 0;
}

int main(void)
{
    int failures;
    failures = run_case();
    if (failures == 0)
        puts("ROTOTILL_AI_TURN_INTEGRATION_CHECKS=12 FAILURES=0 RESULT=PASS");
    else
        puts("ROTOTILL_AI_TURN_INTEGRATION_CHECKS=12 FAILURES=1 RESULT=FAIL");
    return failures == 0 ? 0 : 1;
}
