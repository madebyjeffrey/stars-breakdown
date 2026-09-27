#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct SpeedProbe {
    StarsI16 current;
    StarsI16 best_warp;
    StarsI16 race_stat;
    StarsI16 random_value;
    StarsI16 set_value;
    int sets;
} SpeedProbe;

static StarsI16 in_field(void *context, const StarsU8 *fleet,
    const StarsU8 *field)
{
    (void)context; (void)fleet;
    return field[0] == 7U;
}

static StarsI16 random_value(void *context, StarsI16 count)
{
    SpeedProbe *probe = (SpeedProbe *)context;
    (void)count;
    return probe->random_value;
}

static StarsI16 race_stat(void *context, StarsI16 player, StarsI16 stat)
{
    SpeedProbe *probe = (SpeedProbe *)context;
    (void)player; (void)stat;
    return probe->race_stat;
}

static StarsU16 best_warp(void *context, const StarsU8 *fleet,
    const StarsMoveOrder *order)
{
    SpeedProbe *probe = (SpeedProbe *)context;
    (void)fleet; (void)order;
    return (StarsU16)probe->best_warp;
}

static StarsI16 current_speed(void *context, const StarsU8 *fleet)
{
    SpeedProbe *probe = (SpeedProbe *)context;
    (void)fleet;
    return probe->current;
}

static void set_speed(void *context, const StarsU8 *fleet, StarsU16 speed)
{
    SpeedProbe *probe = (SpeedProbe *)context;
    (void)fleet;
    probe->set_value = (StarsI16)speed;
    ++probe->sets;
}

int main(void)
{
    StarsU8 fleet[8];
    StarsU8 field[18];
    const StarsU8 *fields[2];
    StarsSetAiFleetIdealSpeedOps ops;
    SpeedProbe probe;
    int failures = 0;

    memset(fleet, 0, sizeof(fleet));
    memset(field, 0, sizeof(field));
    field[0] = 7U; field[12] = 1U;
    fields[0] = field; fields[1] = 0;
    probe.current = 0; probe.best_warp = 3; probe.race_stat = 1;
    probe.random_value = 0; probe.set_value = 0; probe.sets = 0;
    ops.fleet_in_field = in_field; ops.random_value = random_value;
    ops.race_stat = race_stat; ops.best_warp = best_warp;
    ops.current_speed = current_speed; ops.set_speed = set_speed;
    stars_set_ai_fleet_ideal_speed(fleet, 0, -1, fields, 2, 4, &ops, &probe);
    if (probe.sets != 1 || probe.set_value != 7)
        ++failures;
    probe.current = 3; probe.sets = 0;
    stars_set_ai_fleet_ideal_speed(fleet, 0, 0, fields, 2, 4, &ops, &probe);
    if (probe.sets != 0)
        ++failures;
    probe.current = 0; probe.best_warp = 3; probe.sets = 0;
    stars_set_ai_fleet_ideal_speed(fleet, 0, 0, fields, 2, 4, &ops, &probe);
    if (probe.sets != 1 || probe.set_value != 3)
        ++failures;
    printf("SET_AI_SPEED_CHECKS=3 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
