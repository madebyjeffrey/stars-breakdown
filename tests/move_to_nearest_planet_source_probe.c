#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct MoveProbe {
    int moves;
    StarsI16 id;
    StarsU16 flags;
} MoveProbe;

static StarsI16 find_nearest(void *context, StarsPoint point, StarsI16 *id)
{
    (void)context; (void)point;
    *id = 8;
    return 1;
}

static StarsI16 move_fleet(void *context, const StarsU8 *fleet,
    const StarsMoveOrder *order, StarsI16 replace)
{
    MoveProbe *probe = (MoveProbe *)context;
    (void)fleet; (void)replace;
    ++probe->moves; probe->id = order->id; probe->flags = order->flags;
    return 1;
}

static void put_word(StarsU8 *bytes, StarsU16 offset, StarsI16 value)
{
    StarsU16 bits = (StarsU16)value;
    bytes[offset] = (StarsU8)bits;
    bytes[offset + 1U] = (StarsU8)(bits >> 8);
}

int main(void)
{
    StarsU8 fleet[8];
    StarsU8 enemy[8];
    StarsU8 own[8];
    const StarsU8 *planets[2];
    StarsPoint points[2];
    StarsPoint order_point;
    StarsMoveNearestPlanetOps ops;
    MoveProbe probe;
    int failures = 0;

    memset(fleet, 0, sizeof(fleet));
    memset(enemy, 0, sizeof(enemy));
    memset(own, 0, sizeof(own));
    put_word(enemy, 0, 7); put_word(enemy, 2, 3);
    put_word(own, 0, 6); put_word(own, 2, 4);
    planets[0] = enemy; planets[1] = own;
    points[0].x = 3; points[0].y = 4;
    points[1].x = 1; points[1].y = 1;
    order_point.x = 0; order_point.y = 0;
    probe.moves = 0; probe.id = 0; probe.flags = 0;
    ops.find_nearest = find_nearest; ops.move_fleet = move_fleet;
    stars_move_to_nearest_planet_or_enemy(fleet, 6, order_point, 10,
        planets, points, 2, 4, &ops, &probe);
    if (probe.moves != 1 || probe.id != 7 || probe.flags != 0x1140U)
        ++failures;
    probe.moves = 0;
    stars_move_to_nearest_planet_or_enemy(fleet, 6, order_point, 1,
        planets, points, 2, 4, &ops, &probe);
    if (probe.moves != 1 || probe.id != 8)
        ++failures;
    printf("MOVE_NEAREST_PLANET_CHECKS=2 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
