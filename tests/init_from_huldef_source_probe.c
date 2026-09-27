#include <stdio.h>
#include "core.h"

typedef struct Trace {
    StarsI16 calls;
    StarsI16 kinds[4];
} Trace;

static StarsI16 part_damage(void *value, const StarsHullComponent *component)
{
    Trace *trace = (Trace *)value;
    StarsI16 kind = (StarsI16)(component->packed_damage & 0xffU);
    trace->kinds[trace->calls++] = kind;
    return (kind == 5) ? 20 : 30;
}

int main(void)
{
    StarsHullComponent components[3];
    StarsHullDefinitionInput input;
    Trace trace;
    StarsI16 coverage;
    StarsI16 result;

    components[0].attributes = 0x0010U;
    components[0].packed_damage = (StarsU16)((2U << 8) | 0x12U);
    components[1].attributes = 0x0800U;
    components[1].packed_damage = (StarsU16)((2U << 8) | 5U);
    components[2].attributes = 0;
    components[2].packed_damage = (StarsU16)((1U << 8) | 4U);
    input.base_value = 10U;
    input.components = components;
    input.component_count = 3U;
    trace.calls = 0;
    result = stars_init_from_huldef(&input, &coverage, part_damage, &trace);
    if (result != 12 || coverage != 48 || trace.calls != 1 ||
        trace.kinds[0] != 5)
        return 1;

    input.base_value = 62U;
    input.components = &components[1];
    input.component_count = 1U;
    result = stars_init_from_huldef(&input, (StarsI16 *)0, part_damage,
        &trace);
    if (result != 63)
        return 2;
    puts("INIT_FROM_HULDEF_SOURCE=PASS");
    return 0;
}
