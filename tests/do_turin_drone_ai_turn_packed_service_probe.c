#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct PackedTrace {
    StarsI16 queues;
    StarsI16 design;
    StarsI16 quantity;
    StarsI16 mode;
    StarsI16 flag;
    StarsI16 finishes;
    StarsI16 changed;
} PackedTrace;

static int checks;
static int failures;

static void check_at(StarsI16 condition, int line)
{
    ++checks;
    if (!condition) {
        ++failures;
#ifndef TURIN_PACKED_NATIVE
        fprintf(stderr, "Turin packed check failed at line %d\n", line);
#else
        (void)line;
#endif
    }
}

#define check(condition) check_at((condition), __LINE__)

static void put_u16(StarsU8 *raw, StarsU16 offset, StarsU16 value)
{
    raw[offset] = (StarsU8)(value & 0xffU);
    raw[offset + 1U] = (StarsU8)(value >> 8);
}

static void put_u32(StarsU8 *raw, StarsU16 offset, StarsU32 value)
{
    put_u16(raw, offset, (StarsU16)value);
    put_u16(raw, (StarsU16)(offset + 2U), (StarsU16)(value >> 16));
}

static StarsI16 status(void *opaque, StarsI16 begin, StarsI16 end,
    StarsI16 budget, StarsI16 *selected, StarsU8 mask[16])
{
    (void)opaque;
    (void)end;
    (void)budget;
    (void)mask;
    *selected = begin == 13 ? 13 : -1;
    return 1;
}

static StarsI16 queue_full(void *opaque,
    const StarsTurinTurnPlanet *planet, StarsI16 design,
    StarsI16 quantity, StarsI16 mode, StarsI16 flag)
{
    PackedTrace *trace;
    (void)planet;
    trace = (PackedTrace *)opaque;
    ++trace->queues;
    trace->design = design;
    trace->quantity = quantity;
    trace->mode = mode;
    trace->flag = flag;
    return 1;
}

static StarsI16 finish(void *opaque, const StarsTurinTurnPlanet *planet,
    StarsI16 changed)
{
    PackedTrace *trace;
    (void)planet;
    trace = (PackedTrace *)opaque;
    ++trace->finishes;
    trace->changed = changed;
    return 1;
}

int turin_packed_service_probe(void)
{
    StarsU8 planet_raw[56];
    StarsU8 fleet_raw[124];
    StarsU8 queue_raw[12];
    StarsU8 order_raw[4];
    StarsU8 ai_raw[128];
    StarsTurinTurnPlanet planet;
    StarsTurinTurnFleet fleet;
    StarsTurinDroneAiTurnInput input;
    StarsTurinDroneAiTurnOps ops;
    PackedTrace trace;
    StarsI16 has_ship;

    checks = failures = 0;
    memset(planet_raw, 0, sizeof planet_raw);
    memset(fleet_raw, 0, sizeof fleet_raw);
    memset(queue_raw, 0, sizeof queue_raw);
    memset(order_raw, 0, sizeof order_raw);
    memset(ai_raw, 0x5a, sizeof ai_raw);
    put_u16(planet_raw, 0U, 7U);
    put_u16(planet_raw, 2U, 4U);
    planet_raw[4] = 3U;
    planet_raw[9] = 80U;
    planet_raw[10] = 90U;
    planet_raw[11] = 100U;
    put_u32(planet_raw, 28U + 3U * 4U, 250U);
    check(!stars_turin_planet_from_packed(planet_raw, 55U, &planet));
    check(stars_turin_planet_from_packed(planet_raw, 56U, &planet));
    check(planet.id == 7 && planet.owner == 4 &&
        planet.visibility_flags == 3U &&
        planet.mineral_concentration[0] == 80U &&
        planet.mineral_concentration[1] == 90U &&
        planet.mineral_concentration[2] == 100U &&
        planet.current_minerals[3] == 250L);

    put_u16(fleet_raw, 0U, 1U);
    put_u16(fleet_raw, 2U, 4U);
    put_u16(fleet_raw, 6U, 7U);
    put_u16(fleet_raw, 12U + 2U * 2U, 4U);
    put_u16(fleet_raw, 12U + 12U * 2U, 256U);
    put_u16(fleet_raw, 12U + 13U * 2U, 8U);
    put_u16(fleet_raw, 98U, 0U);
    put_u32(fleet_raw, 76U + 3U * 4U, 25U);
    put_u32(fleet_raw, 76U + 4U * 4U, 300U);
    check(!stars_turin_fleet_from_packed(fleet_raw, 123U, 0,
        order_raw, &fleet));
    check(stars_turin_fleet_from_packed(fleet_raw, 124U, 0,
        order_raw, &fleet));
    check(fleet.id == 1 && fleet.owner == 4 && fleet.planet_id == 7 &&
        fleet.ship_counts[2] == 4 && fleet.ship_counts[12] == 256 &&
        fleet.ship_counts[13] == 8 && fleet.current_cargo[3] == 25L &&
        fleet.current_cargo[4] == 300L && fleet.raw_plord == order_raw);

    queue_raw[3] = 2U;
    put_u32(queue_raw, 4U, (3UL << 17) | (1UL << 10));
    put_u32(queue_raw, 8U, (2UL << 17) | (15UL << 10));
    check(!stars_turin_ship_queue_present(queue_raw, 11U, &has_ship));
    check(stars_turin_ship_queue_present(queue_raw, 12U, &has_ship));
    check(has_ship == 1);
    put_u32(queue_raw, 8U, (2UL << 17) | (16UL << 10));
    check(stars_turin_ship_queue_present(queue_raw, 12U, &has_ship));
    check(has_ship == 0);
    put_u32(queue_raw, 8U, (2UL << 17) | (15UL << 10));
    check(stars_turin_ship_queue_present(queue_raw, 12U, &has_ship));

    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    memset(&trace, 0, sizeof trace);
    input.turn = 150;
    input.player_id = 4;
    input.planet_count = 1;
    input.planets = &planet;
    input.fleet_count = 1;
    input.fleets = &fleet;
    input.ops = &ops;
    input.raw_ai_planet = ai_raw;
    input.raw_ai_planet_size = sizeof ai_raw;
    input.support_policy_valid = 1;
    input.production_designs_valid = 1;
    planet.production_slot_open = 1;
    planet.production_queue_has_ship = 0;
    ops.check_design_status = status;
    ops.queue_production_full = queue_full;
    ops.finish_production = finish;
    ops.context = &trace;
    check(stars_do_turin_drone_ai_turn(&input));
    check(input.armada_potency[0] == 4U && trace.queues == 1 &&
        trace.design == 13 && trace.quantity == 4 &&
        trace.mode == 2 && trace.flag == 1);
    check(trace.finishes == 1 && trace.changed == 1 &&
        input.production_hulls_queued == 4);
    check(ai_raw[7U * 16U + 9U] == 1U &&
        ai_raw[7U * 16U + 1U] == 0x5aU &&
        ai_raw[7U * 16U + 2U] == 0x5aU);

    memset(&trace, 0, sizeof trace);
    planet.production_queue_has_ship = has_ship;
    check(stars_do_turin_drone_ai_turn(&input));
    check(trace.queues == 0 && trace.finishes == 1 &&
        trace.changed == 0);

    planet.owner = -1;
    planet.production_slot_open = 0;
    check(stars_do_turin_drone_ai_turn(&input));
    check(ai_raw[7U * 16U + 1U] == 127U &&
        ai_raw[7U * 16U + 2U] == 0x5aU &&
        input.unowned_good_count == 1);
    planet.owner = 4;
    planet.desirability = -1;
    planet.production_slot_open = 1;
    memset(&trace, 0, sizeof trace);
    check(stars_do_turin_drone_ai_turn(&input));
    check(ai_raw[7U * 16U + 2U] == 1U && input.owned_bad_count == 1);
    check(trace.queues == 0 && trace.finishes == 0);
    ai_raw[7U * 16U + 9U] = 0x5aU;
    input.raw_ai_planet_size = 7U * 16U + 15U;
    check(!stars_do_turin_drone_ai_turn(&input));
    check(ai_raw[7U * 16U + 9U] == 0x5aU);
    input.raw_ai_planet_size = sizeof ai_raw;
    planet.id = -1;
    check(!stars_do_turin_drone_ai_turn(&input));
    check(ai_raw[7U * 16U + 9U] == 0x5aU);

#ifndef TURIN_PACKED_NATIVE
    printf("TURIN_PACKED_SERVICE_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures == 0 ? "PASS" : "FAIL");
#endif
    return failures == 0 ? 0 : 1;
}

#ifndef TURIN_PACKED_NATIVE
int main(void)
{
    return turin_packed_service_probe();
}
#endif
