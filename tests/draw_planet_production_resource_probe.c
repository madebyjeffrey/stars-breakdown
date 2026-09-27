#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 calls;
    StarsI16 failures;
    StarsU16 ids[3];
} Trace;

static const StarsU8 *compressed(void *context, StarsU16 id)
{
    Trace *trace = (Trace *)context;
    const char *text;
    if (trace->calls < 3)
        trace->ids[trace->calls] = id;
    ++trace->calls;
    if (id == 0x32e)
        text = "Production";
    else if (id == 0x218)
        text = "Completion: ";
    else if (id == 0x4e9)
        text = "Route to";
    else {
        ++trace->failures;
        text = "";
    }
    if ((id == 0x32e && strcmp(text, "Production") != 0) ||
        (id == 0x218 && strcmp(text, "Completion: ") != 0) ||
        (id == 0x4e9 && strcmp(text, "Route to") != 0))
        ++trace->failures;
    return (const StarsU8 *)text;
}

static StarsI16 draw_tile(void *context, StarsU16 hdc,
    StarsPlanetProductionTile *tile, const StarsU8 *title, StarsRect *rect)
{
    (void)context;
    (void)hdc;
    (void)tile;
    (void)title;
    rect->left = 4;
    rect->top = 4;
    rect->right = 180;
    rect->bottom = 140;
    return 1;
}

int main(void)
{
    StarsPlanetProductionTile tile;
    StarsPlanetProductionData data;
    StarsPlanetProductionOps ops;
    StarsPlanetProductionInput input;
    Trace trace;

    memset(&tile, 0, sizeof tile);
    memset(&data, 0, sizeof data);
    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    strcpy((char *)data.eta, "ETA");
    strcpy((char *)data.planet_name, "New Terra");
    data.queue_count = 1;
    ops.compressed_string = compressed;
    ops.draw_tile = draw_tile;
    ops.context = &trace;
    input.hdc = 3;
    input.line_height = 10;
    input.tile = &tile;
    input.data = &data;
    input.ops = &ops;
    stars_draw_planet_production(&input);
    if (trace.calls != 3 || trace.failures != 0 ||
        trace.ids[0] != 0x32e || trace.ids[1] != 0x218 ||
        trace.ids[2] != 0x4e9)
        return 1;
    puts("DRAW_PLANET_PRODUCTION_RESOURCE_CHECKS=3 FAILURES=0 RESULT=PASS");
    return 0;
}
