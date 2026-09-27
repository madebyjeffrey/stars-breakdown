#include "core.h"
#include <stdio.h>
#include <string.h>

static StarsI16 opt_value(void *context, StarsI16 planet)
{
    (void)context;
    return planet == 1 ? -2 : 0;
}

static const StarsU8 *find_wormhole(void *context, StarsPoint source,
    StarsU32 distance)
{
    (void)context; (void)source; (void)distance;
    return (const StarsU8 *)"WORM";
}

int main(void)
{
    StarsU8 status[3];
    StarsI16 owners[3] = { -1, -1, 4 };
    StarsPoint points[3];
    StarsI16 blocked[1] = { 0 };
    StarsNearestColonizableOps ops;
    const StarsU8 *wormhole;
    StarsPoint origin;
    int failures = 0;

    origin.x = 0; origin.y = 0;
    points[0].x = 10; points[0].y = 0;
    points[1].x = 2; points[1].y = 0;
    points[2].x = 1; points[2].y = 1;
    ops.planet_opt_value = opt_value; ops.find_wormhole = find_wormhole;
    if (stars_id_nearest_colonizable_planet(origin, -1, 1, 10,
            0, 4, status, owners, points, 3, blocked, 1, &ops, 0,
            &wormhole) != -1 || status[1] != 8 || wormhole != 0)
        ++failures;
    if (stars_id_nearest_colonizable_planet(points[2], 2, 0, 10, 0, 4,
            status, owners, points, 3, 0, 0, &ops, 0, &wormhole) != -1 ||
            wormhole == 0)
        ++failures;
    owners[0] = -2;
    (void)stars_id_nearest_colonizable_planet(origin, -1, 1, 10,
        0, 4, status, owners, points, 3, 0, 0, &ops, 0, 0);
    if (status[0] != 16 || status[1] != 8 || status[2] != 1) ++failures;
    status[0] = 4;
    (void)stars_id_nearest_colonizable_planet(origin, -1, 1, 10,
        1, 4, status, owners, points, 3, 0, 0, &ops, 0, 0);
    if (status[0] != 4) ++failures;
    (void)stars_id_nearest_colonizable_planet(origin, -1, 0, 10,
        1, 4, status, owners, points, 3, 0, 0, &ops, 0, 0);
    if (status[0] != 0 || status[1] != 0 || status[2] != 1) ++failures;
    printf("ID_NEAREST_COLONIZABLE_CHECKS=5 FAILURES=%d RESULT=%s\n",
        failures, failures ? "FAIL" : "PASS");
    return failures != 0;
}
