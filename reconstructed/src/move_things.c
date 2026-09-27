#include "core.h"

#define MOVE_THING_PACKET 1
#define MOVE_THING_WORMHOLE 2
#define MOVE_THING_MINEFIELD 3
#define MOVE_WORMHOLE_ATTEMPTS 100
#define MOVE_PACKET_RACE_STAT 0x0e

static StarsI16 move_distance(const StarsMoveThingsOps *ops,
    const StarsMoveThing *thing, const StarsMovePlanet *planet)
{
    if (ops->distance)
        return ops->distance(ops->context, thing->x, thing->y,
            planet->x, planet->y);
    return (StarsI16)((thing->x - planet->x) * (thing->x - planet->x) +
        (thing->y - planet->y) * (thing->y - planet->y));
}

static void move_remove(const StarsMoveThingsOps *ops, StarsMoveThing *thing)
{
    thing->active = 0;
    if (ops->remove_thing) ops->remove_thing(ops->context, thing);
}

static void move_wormhole(StarsMoveThingsInput *input,
    StarsMoveThing *thing, StarsI16 *changed)
{
    const StarsMoveThingsOps *ops;
    StarsI16 move_chance;
    StarsI16 move;
    StarsI16 best;
    StarsI16 best_x;
    StarsI16 best_y;
    StarsI16 i;
    StarsI16 limit;
    StarsI16 random_value;
    StarsI16 original_x;
    StarsI16 original_y;
    StarsI16 score;

    ops = input->ops;
    if (!ops->random_value) return;
    original_x = thing->x;
    original_y = thing->y;
    random_value = ops->random_value(ops->context, 100);
    move_chance = ops->wormhole_chance ?
        ops->wormhole_chance(ops->context, thing) : 0;
    move = (StarsI16)(random_value < move_chance);
    if (move) {
        thing->packed_destination = (StarsU16)(
            thing->packed_destination & 0xf003U);
        limit = (StarsI16)(input->map_size * 400 + 400);
    } else {
        thing->packed_destination = (StarsU16)(
            (thing->packed_destination & 0xf003U) |
            ((thing->packed_destination + 4U) & 0x0ffcU));
        limit = 25;
    }
    best = 16;
    best_x = thing->x;
    best_y = thing->y;
    for (i = 0; i < MOVE_WORMHOLE_ATTEMPTS; ++i) {
        StarsI16 x;
        StarsI16 y;
        x = ops->random_value(ops->context, limit);
        y = ops->random_value(ops->context, limit);
        if (move) {
            x = (StarsI16)(x + 1000);
            y = (StarsI16)(y + 1000);
        } else {
            x = (StarsI16)(x + original_x - 12);
            y = (StarsI16)(y + original_y - 12);
        }
        thing->x = x;
        thing->y = y;
        if (x == original_x && y == original_y)
            continue;
        score = ops->validate_wormhole ?
            ops->validate_wormhole(ops->context, thing) : 0;
        if (score == 0) break;
        if (score < best) {
            best = score;
            best_x = x;
            best_y = y;
        }
    }
    if (best != 16) {
        thing->x = best_x;
        thing->y = best_y;
        *changed = 1;
    }
    *changed = 1;
}

static void move_minefield(StarsMoveThingsInput *input,
    StarsMoveThing *thing, StarsI16 *changed)
{
    const StarsMoveThingsOps *ops;
    StarsI16 direction;
    StarsI16 limit;

    ops = input->ops;
    if (input->post_production || !ops->random_value ||
            ops->random_value(ops->context, 25) != 0) return;
    limit = (StarsI16)(input->map_size * 400 + 361);
    direction = ops->random_value(ops->context, 2);
    if (direction == 0) {
        thing->x = ops->random_value(ops->context, limit) + 1020;
        thing->y = ops->random_value(ops->context, 2) ? 1020 : limit;
    } else {
        thing->x = ops->random_value(ops->context, 2) ? 1020 : limit;
        thing->y = ops->random_value(ops->context, limit) + 1020;
    }
    thing->range = (StarsI16)(thing->range + 1);
    if (ops->send_message)
        ops->send_message(ops->context, -1, 0xc0, thing, thing->range, 0);
    *changed = 1;
}

static void move_packet(StarsMoveThingsInput *input,
    StarsMoveThing *thing, StarsI16 *changed)
{
    const StarsMoveThingsOps *ops;
    StarsMovePlanet *planet;
    StarsI16 distance;
    StarsI16 radius;
    StarsI16 step;
    StarsI16 survival;
    StarsI16 warp;
    StarsI16 i;
    StarsI16 amount[3];

    ops = input->ops;
    if (!thing->active || thing->target_planet < 0 || !ops->find_planet)
        return;
    planet = ops->find_planet(ops->context, thing->target_planet);
    if (!planet) return;
    radius = (StarsI16)((thing->warp_level + 4) *
        (thing->warp_level + 4));
    if (input->post_production) radius = (StarsI16)(radius / 2);
    distance = move_distance(ops, thing, planet);
    if (distance > radius) {
        step = radius > 0 ? radius : 1;
        if (thing->x < planet->x) thing->x = (StarsI16)(thing->x + step);
        else if (thing->x > planet->x) thing->x = (StarsI16)(thing->x - step);
        if (thing->y < planet->y) thing->y = (StarsI16)(thing->y + step);
        else if (thing->y > planet->y) thing->y = (StarsI16)(thing->y - step);
        if (input->post_production && ops->packet_decay &&
                ops->packet_decay(ops->context, thing, 50))
            move_remove(ops, thing);
        *changed = 1;
        return;
    }
    warp = 0;
    if (ops->warp_machine && !ops->warp_machine(ops->context, planet, &warp))
        warp = 0;
    survival = ops->planet_survival ?
        ops->planet_survival(ops->context, planet) : 100;
    if (survival < 0) survival = 0;
    if (survival > 100) survival = 100;
    for (i = 0; i < 3; ++i)
        amount[i] = (StarsI16)((thing->strength[i] * survival) / 100);
    if (ops->apply_packet)
        ops->apply_packet(ops->context, planet, amount[0], amount[1],
            amount[2], warp);
    if (ops->packet_decay)
        ops->packet_decay(ops->context, thing, 100);
    move_remove(ops, thing);
    *changed = 1;
}

void stars_move_things(StarsMoveThingsInput *input)
{
    StarsU16 i;
    StarsI16 changed;

    if (!input || !input->things || !input->ops) return;
    changed = 0;
    for (i = 0; i < input->thing_count; ++i) {
        StarsMoveThing *thing;
        thing = &input->things[i];
        if (!thing->active) continue;
        if (thing->kind == MOVE_THING_WORMHOLE && input->post_production)
            move_wormhole(input, thing, &changed);
        else if (thing->kind == MOVE_THING_MINEFIELD &&
                !input->post_production)
            move_minefield(input, thing, &changed);
        else if (thing->kind == MOVE_THING_PACKET)
            move_packet(input, thing, &changed);
    }
    if (changed && input->ops->validate_waypoints)
        input->ops->validate_waypoints(input->ops->context);
}
