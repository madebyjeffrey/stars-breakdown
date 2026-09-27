#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace { StarsU16 id; StarsU16 color; } Trace;
static StarsI16 compressed(void *context, StarsU16 id, StarsU8 *output,
    StarsU16 capacity)
{
    Trace *t = (Trace *)context;
    (void)capacity; t->id = id; strcpy((char *)output, "In transit"); return 1;
}
static StarsI16 location(void *context, StarsU16 warp, StarsI16 x,
    StarsI16 y, StarsI16 z, StarsU8 *output, StarsU16 capacity)
{
    Trace *t = (Trace *)context;
    (void)capacity; t->id = warp; sprintf((char *)output, "%d,%d,%d", x, y, z);
    return 1;
}
static void set_color(void *context, StarsU16 hdc, StarsU16 color)
{ Trace *t = (Trace *)context; (void)hdc; t->color = color; }

int main(void)
{
    StarsDestinationNameInput input;
    Trace trace;
    StarsU8 output[64];
    memset(&input, 0, sizeof input); memset(&trace, 0, sizeof trace);
    input.compressed = compressed; input.location = location;
    input.set_color = set_color; input.context = &trace;
    input.dash_text = (const StarsU8 *)"--"; input.cord = 1U;
    if (!stars_get_destination_name(&input, output, sizeof output) ||
        strcmp((char *)output, "--") != 0) return 1;
    input.cord = 2U; input.orders[3] = 0x1006U; input.hdc = 4U;
    input.orders[4] = 0x6000U;
    if (!stars_get_destination_name(&input, output, sizeof output) ||
        trace.id != 0x049dU || trace.color != 0x007fU) return 2;
    input.hdc = 0U; input.orders[3] = 0x1102U;
    if (!stars_get_destination_name(&input, output, sizeof output) ||
        strcmp((char *)output, "--") != 0) return 3;
    memset(input.orders, 0, sizeof input.orders);
    input.next_orders[0] = 12U; input.next_orders[1] = 34U;
    input.next_orders[2] = 56U; input.next_orders[3] = 0x0300U;
    if (!stars_get_destination_name(&input, output, sizeof output) ||
        strcmp((char *)output, "56,12,34") != 0 || trace.id != 3U) return 4;
    puts("DESTINATION_NAME_SOURCE=PASS");
    return 0;
}
