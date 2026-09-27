#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsPlanetPopInfo info;
    StarsI16 streams;
    StarsI16 wraps;
    StarsI16 fonts;
    StarsI16 ids[32];
    StarsI16 kinds[32];
    StarsI16 wrap_ids[8];
    StarsI16 last_player;
    StarsI16 prepared;
    StarsI16 saw_negative_desirability;
} Trace;
static StarsI16 lookup(void *c, StarsI16 id, StarsPlanetPopInfo *info)
{
    Trace *t = (Trace *)c;
    (void)id; *info = t->info; return 1;
}
static void select_font(void *c, StarsU16 hdc, StarsI16 font)
{
    (void)hdc; (void)font; ++((Trace *)c)->fonts;
}
static void stream(void *c, StarsU16 hdc, StarsI16 *x, StarsI16 y,
    const StarsPlanetPopSegment *segment, StarsI16 print_mode)
{
    Trace *t = (Trace *)c;
    (void)hdc; (void)y; (void)print_mode;
    if (t->streams < 32) {
        t->ids[t->streams] = segment->string_id;
        t->kinds[t->streams] = segment->kind;
    }
    ++t->streams;
    *x = (StarsI16)(*x + 2);
}
static void wrap(void *c, StarsU16 hdc, StarsI16 *x, StarsI16 *y,
    const StarsPlanetPopSegment *segment, StarsI16 width, StarsI16 print_mode)
{
    Trace *t = (Trace *)c;
    (void)hdc; (void)y; (void)width; (void)print_mode;
    if (t->wraps < 8)
        t->wrap_ids[t->wraps] = segment->string_id;
    ++t->wraps;
    *x = (StarsI16)(*x + 2);
}
static void prepare_segment(void *c, StarsPlanetPopSegment *segment)
{
    Trace *t = (Trace *)c;
    ++t->prepared;
    if (segment->kind == 5 && segment->value1 == 2 &&
            segment->value2 == 5)
        t->saw_negative_desirability = 1;
}
static StarsI32 max_population(void *c, StarsI16 id, StarsI16 player)
{
    Trace *t = (Trace *)c;
    (void)id; t->last_player = player; return t->info.max_population;
}
static StarsI16 desirability(void *c, StarsI16 id, StarsI16 player)
{
    Trace *t = (Trace *)c;
    (void)id; t->last_player = player; return t->info.desirability_tenths;
}
static StarsI32 change_population(void *c, StarsI16 id)
{
    (void)id; return ((Trace *)c)->info.population_change;
}
static void install(StarsPlanetPopInfoOps *ops, Trace *trace)
{
    memset(ops, 0, sizeof *ops);
    ops->lookup_planet = lookup;
    ops->select_font = select_font;
    ops->stream_text = stream;
    ops->wrap_text = wrap;
    ops->max_population = max_population;
    ops->desirability = desirability;
    ops->change_population = change_population;
    ops->prepare_segment = prepare_segment;
    ops->context = trace;
}
static void setup(StarsPlanetPopInfoInput *input,
    StarsPlanetPopInfoOps *ops, Trace *trace)
{
    memset(input, 0, sizeof *input);
    install(ops, trace);
    input->planet_id = 7;
    input->line_height = 8;
    input->font_bold = 1;
    input->viewer_player = 0;
    input->ops = ops;
}
int main(void)
{
    StarsPlanetPopInfoInput input;
    StarsPlanetPopInfoOps ops;
    Trace trace;
    StarsPoint point;

    memset(&trace, 0, sizeof trace);
    trace.info.owner = 0;
    trace.info.selected_player = 0;
    trace.info.mineral_rate = 10;
    trace.info.habitability_class = 4;
    trace.info.max_population = 100;
    trace.info.desirability_tenths = 10;
    trace.info.population_change = 5;
    setup(&input, &ops, &trace);
    point = stars_display_planet_pop_info(&input);
    if (trace.streams != 8 || trace.wraps != 3 || trace.ids[0] != 0x244 ||
            trace.ids[2] != 0x24f || trace.ids[5] != 0x24c ||
            trace.wrap_ids[2] != 0x213 || trace.last_player != 0 ||
            trace.prepared != 11 ||
            point.x <= 4 || point.y <= 4)
        return 1;
    memset(&trace, 0, sizeof trace);
    trace.info.owner = -1;
    trace.info.selected_player = 0;
    trace.info.habitability_class = 2;
    setup(&input, &ops, &trace);
    point = stars_display_planet_pop_info(&input);
    if (trace.streams != 2 || trace.wraps != 0 || trace.ids[1] != 0x248 ||
            point.y != 16)
        return 1;
    memset(&trace, 0, sizeof trace);
    trace.info.owner = 1;
    trace.info.selected_player = 0;
    trace.info.habitability_class = 4;
    trace.info.foreign_environment_class = 0;
    trace.info.max_population = 20;
    trace.info.desirability_tenths = 5;
    setup(&input, &ops, &trace);
    stars_display_planet_pop_info(&input);
    if (trace.ids[0] != 0x245 || trace.ids[2] != 0x246 ||
            trace.ids[4] != 0xc2b || trace.wraps != 2)
        return 1;
    memset(&trace, 0, sizeof trace);
    trace.info.owner = 0;
    trace.info.selected_player = 0;
    trace.info.habitability_class = 4;
    trace.info.desirability_tenths = -25;
    setup(&input, &ops, &trace);
    stars_display_planet_pop_info(&input);
    if (trace.ids[6] != 0x249 || trace.ids[9] != 0x24a || trace.wraps != 0)
        return 1;
    if (trace.prepared != 10 || !trace.saw_negative_desirability)
        return 1;
    puts("PT_DISPLAY_PLANET_POP_INFO_FORMAT_CHECKS=27 FAILURES=0 RESULT=PASS");
    return 0;
}
