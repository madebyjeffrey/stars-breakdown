#include <stdio.h>
#include "core.h"

static StarsI32 power(void *context, StarsU16 kind, StarsU16 subtype)
{
    (void)context;
    if (kind != 0x100U)
        return 0;
    return subtype == 2U ? 30L : 50L;
}

static StarsI32 large_power(void *context, StarsU16 kind, StarsU16 subtype)
{
    (void)context;
    (void)kind;
    (void)subtype;
    return 50000L;
}

int main(void)
{
    StarsLayMinesComponent a[2];
    StarsLayMinesComponent b[1];
    StarsLayMinesDesign designs[2];
    StarsCLayMinesInput input;
    input.component_counts[0] = 2U;
    input.component_counts[1] = 1U;
    input.designs = designs;
    input.design_count = 2U;
    input.part_power = power;
    input.context = 0;
    designs[0].hull_kind = 0x1bU;
    designs[0].components = a;
    designs[0].component_count = 2U;
    a[0].kind = 0x100U;
    a[0].packed_count_type = 0x0202U;
    a[1].kind = 0x10U;
    a[1].packed_count_type = 0x0312U;
    designs[1].hull_kind = 0;
    designs[1].components = b;
    designs[1].component_count = 1U;
    b[0].kind = 0x100U;
    b[0].packed_count_type = 0x0105U;
    if (stars_c_lay_mines_from_lpfl(&input, -1, -1) != 3380L)
        return 1;
    if (stars_c_lay_mines_from_lpfl(&input, 0, 0) != 2880L)
        return 1;
    if (stars_c_lay_mines_from_lpfl(&input, 1, -1) != 500L)
        return 1;
    input.part_power = large_power;
    input.component_counts[0] = 255U;
    input.component_counts[1] = 255U;
    if (stars_c_lay_mines_from_lpfl(&input, -1, -1) != 100000000L)
        return 1;
    puts("CLAY_MINES_SOURCE=PASS");
    return 0;
}
