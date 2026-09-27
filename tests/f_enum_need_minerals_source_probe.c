#include <stdio.h>
#include "core.h"

typedef struct Trace {
    StarsI16 gate[2];
} Trace;

static StarsI16 warp(void *context, const StarsNeedMineralsPlanet *planet,
    StarsI16 *gate)
{
    Trace *trace = (Trace *)context;
    *gate = trace->gate[planet->id];
    return planet->id == 0U ? 10 : 8;
}

static StarsI32 distance_squared(void *context, StarsPoint first,
    StarsPoint second)
{
    StarsI32 dx = (StarsI32)first.x - (StarsI32)second.x;
    StarsI32 dy = (StarsI32)first.y - (StarsI32)second.y;
    (void)context;
    return dx * dx + dy * dy;
}

int main(void)
{
    StarsNeedMineralsPlanet source;
    StarsNeedMineralsPlanet test;
    StarsNeedMineralsInput input;
    StarsU16 source_ai[2] = { 0U, 0U };
    StarsU16 test_ai[2] = { 0U, 0x0100U };
    StarsPoint points[2] = { { 0, 0 }, { 50, 0 } };
    Trace trace = { { 0, 0 } };

    source.id = 0U; source.owner = 2; source.order_flags = 0U;
    source.status_flags = 0U;
    source.minerals[0] = 701L; source.minerals[1] = 0L;
    source.minerals[2] = 0L;
    test.id = 1U; test.owner = 3; test.order_flags = 1U;
    test.status_flags = 0x0200U;
    test.minerals[0] = 0L; test.minerals[1] = 0L; test.minerals[2] = 0L;
    input.source = &source; input.test = &test; input.player = 3;
    input.source_ai_flags = source_ai; input.test_ai_flags = test_ai;
    input.planet_points = points; input.planet_count = 2U;
    input.warp = warp; input.distance_squared = distance_squared;
    input.warp_scale = 1.0; input.context = &trace;
    if (stars_f_enum_need_minerals(&input) != 1) return 1;
    test.status_flags = 0U;
    if (stars_f_enum_need_minerals(&input) != 0) return 2;
    test.status_flags = 0x0200U; test_ai[1] = 0U;
    if (stars_f_enum_need_minerals(&input) != 0) return 3;
    test_ai[1] = 0x0100U; source_ai[1] = 0x0020U;
    if (stars_f_enum_need_minerals(&input) != 0) return 4;
    source_ai[1] = 0U; test.order_flags = 1U; points[1].x = 500;
    if (stars_f_enum_need_minerals(&input) != 0) return 5;
    puts("F_ENUM_NEED_MINERALS_SOURCE=PASS");
    return 0;
}
