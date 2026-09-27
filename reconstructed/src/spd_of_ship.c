#include "core.h"

#define SPEED_ENGINE_GROUP 0x0001
#define SPEED_ALTERNATE_ENGINE_GROUP 0x0080
#define SPEED_FUEL_TANK_GROUP 0x0800
#define SPEED_ENGINE_ROOM_GROUP 0x1000
#define SPEED_CARGO_STAT 2
#define SPEED_RACE_STAT 0x0e
#define SPEED_FUEL_LIMIT 120
#define SPEED_WEIGHT_UNIT 0x46
#define SPEED_MAX_WARP 8

static StarsI16 speed_is_fast_engine(StarsU16 engine_id)
{
    return engine_id == 7U || engine_id == 8U || engine_id == 9U ||
        engine_id == 0x0eU || engine_id == 0x0fU;
}

/* NE 1F:339C-388D. Calculate a ship's combat warp rating from its engine,
 * fuel-system components, hull/cargo weight, and racial speed bonus.  The
 * token's upper speed bits are refreshed with the original four-bit random
 * value when a live fleet is supplied. */
StarsI16 stars_speed_of_ship(StarsSpeedInput *input)
{
    StarsU16 i;
    StarsU16 engine_id;
    StarsU16 engine_count;
    StarsU16 maneuver_points;
    StarsU16 fuel_system_points;
    StarsU16 engine_room_points;
    StarsI16 base_speed;
    StarsI16 speed;
    StarsI16 race_stat;
    StarsU32 hull_cargo;
    StarsU32 fleet_weight;
    StarsU32 added_weight;
    StarsU32 ship_weight;
    StarsSpeedEngine engine;
    const StarsSpeedComponent *component;

    if (!input || !input->design)
        return 0;
    engine_id = 0xffffU;
    engine_count = 0;
    maneuver_points = 0;
    fuel_system_points = 0;
    engine_room_points = 0;
    for (i = 0; i < input->design->component_count; ++i) {
        component = &input->design->components[i];
        if (component->count == 0U) continue;
        if (component->group == SPEED_ENGINE_GROUP) {
            engine_id = component->type;
            engine_count = component->count;
            if (component->type == 8U)
                maneuver_points = (StarsU16)(maneuver_points + component->count);
        } else if (component->group == SPEED_ALTERNATE_ENGINE_GROUP &&
                component->type == 6U) {
            maneuver_points = (StarsU16)(maneuver_points + component->count);
        } else if (component->group == SPEED_FUEL_TANK_GROUP &&
                component->type == 4U) {
            fuel_system_points = (StarsU16)(fuel_system_points +
                component->count);
        } else if (component->group == SPEED_ENGINE_ROOM_GROUP) {
            if (component->type == 7U)
                engine_room_points = (StarsU16)(engine_room_points +
                    component->count);
            else if (component->type == 8U)
                engine_room_points = (StarsU16)(engine_room_points +
                    component->count * 2U);
        }
    }
    if (engine_id == 0xffffU || engine_count == 0U ||
            !input->engine_lookup ||
            !input->engine_lookup(input->context, engine_id, &engine))
        return 0;
    if (speed_is_fast_engine(engine_id))
        base_speed = 10;
    else {
        base_speed = 9;
        while (base_speed > 0 && engine.fuel_used[base_speed] >
                SPEED_FUEL_LIMIT)
            --base_speed;
    }
    speed = (StarsI16)(base_speed - 4 + fuel_system_points +
        (maneuver_points + 1U) / 2U + engine_room_points);
    if (input->fleet) {
        race_stat = input->race_stat ? input->race_stat(input->context,
            input->fleet->player, SPEED_RACE_STAT) : 0;
        if (race_stat == 2)
            speed = (StarsI16)(speed + 2);
        hull_cargo = input->hull_stat ? input->hull_stat(input->context,
            input->design, SPEED_CARGO_STAT) : 0UL;
        fleet_weight = input->fleet_stat ? input->fleet_stat(input->context,
            input->fleet, SPEED_CARGO_STAT) : 0UL;
        if (hull_cargo != 0UL && fleet_weight != 0UL) {
            added_weight = (hull_cargo * input->fleet->cargo_weight) /
                fleet_weight;
            ship_weight = input->design->empty_weight + added_weight;
        } else
            ship_weight = input->design->empty_weight;
        if (input->dump_cargo)
            speed = (StarsI16)(speed - 1);
        if (input->random && input->token)
            input->token->random_speed_bits = (StarsU16)(
                (input->token->random_speed_bits & 0xc3ffU) |
                ((StarsU16)input->random(input->context, 15) & 0x0fU) << 10);
    } else
        ship_weight = input->design->empty_weight;
    if (input->token)
        input->token->weight = (StarsU16)ship_weight;
    if (input->design->components[0].count != 0U)
        speed = (StarsI16)(speed -
            (StarsI16)((ship_weight / SPEED_WEIGHT_UNIT) /
                input->design->components[0].count));
    if (speed < 0)
        speed = 0;
    if (speed > SPEED_MAX_WARP)
        speed = SPEED_MAX_WARP;
    return speed;
}
