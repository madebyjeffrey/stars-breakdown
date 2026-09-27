#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 tile;
    StarsI16 measure;
    StarsI16 fonts;
    StarsI16 labels;
    StarsI16 values;
    StarsI16 refs;
    StarsI16 separators;
    StarsI16 last_field;
    StarsI16 last_y;
    StarsI16 first_offset;
    StarsI16 last_offset;
    StarsI16 ref_ids[4];
} Trace;
static StarsI16 draw_tile(void *c, StarsU16 hdc, const void *tile,
    StarsRect *rect)
{
    Trace *t = (Trace *)c;
    (void)hdc; (void)tile; ++t->tile;
    rect->left = 10; rect->top = 10; rect->right = 210; rect->bottom = 100;
    return 1;
}
static void select_font(void *c, StarsU16 hdc, StarsI16 font)
{
    (void)hdc; (void)font; ++((Trace *)c)->fonts;
}
static StarsI16 measure(void *c, StarsU16 hdc, StarsI16 label)
{
    (void)hdc; (void)label; ++((Trace *)c)->measure; return label & 7;
}
static void draw_label(void *c, StarsU16 hdc, StarsI16 label,
    StarsI16 x, StarsI16 y)
{
    Trace *t = (Trace *)c;
    (void)hdc; (void)label; (void)x; (void)y; ++t->labels;
}
static void draw_value(void *c, StarsU16 hdc, StarsI16 field,
    StarsI16 right, StarsI16 value_offset, StarsI16 y,
    const StarsPlanetStatsData *data)
{
    Trace *t = (Trace *)c;
    (void)hdc; (void)right; (void)data; ++t->values;
    if (t->values == 1)
        t->first_offset = value_offset;
    t->last_offset = value_offset;
    t->last_field = field; t->last_y = y;
}
static void reference(void *c, StarsI16 id, const StarsRect *rect)
{
    Trace *t = (Trace *)c;
    (void)rect;
    if (t->refs < 4)
        t->ref_ids[t->refs] = id;
    ++t->refs;
}
static void separator(void *c, StarsU16 hdc, StarsI16 y,
    StarsI16 left, StarsI16 right)
{
    (void)hdc; (void)y; (void)left; (void)right;
    ++((Trace *)c)->separators;
}
static void install(StarsPlanetStatsOps *ops, Trace *trace)
{
    memset(ops, 0, sizeof *ops);
    ops->draw_tile = draw_tile;
    ops->select_font = select_font;
    ops->measure_label = measure;
    ops->draw_label = draw_label;
    ops->draw_value = draw_value;
    ops->set_reference = reference;
    ops->draw_separator = separator;
    ops->context = trace;
}
int main(void)
{
    StarsPlanetStatsInput input;
    StarsPlanetStatsOps ops;
    Trace trace;

    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    install(&ops, &trace);
    input.hdc = 5;
    input.line_height = 8;
    input.font_bold = 1;
    input.font_normal = 0;
    input.ops = &ops;
    stars_draw_planet_stats(&input);
    if (trace.tile != 1 || trace.measure != 7 || trace.fonts != 2 ||
            trace.labels != 7 || trace.values != 7 || trace.refs != 4 ||
            trace.separators != 2 || trace.ref_ids[0] != 9 ||
            trace.ref_ids[1] != 8 || trace.ref_ids[2] != 11 ||
            trace.ref_ids[3] != 10 || trace.last_field != 6 ||
            trace.last_y != 60 || trace.first_offset != 185 ||
            trace.last_offset != 185)
        return 1;
    trace.labels = 0;
    trace.values = 0;
    input.hidden = 1;
    stars_draw_planet_stats(&input);
    if (trace.labels != 0 || trace.values != 7 || trace.measure != 14)
        return 1;
    puts("DRAW_PLANET_STATS_CHECKS=22 FAILURES=0 RESULT=PASS");
#ifdef TARGET_NATIVE
    {
        FILE *report = fopen("C:\\DPS\\DPSR.TXT", "w");
        if (!report)
            return 2;
        fputs("DRAW_PLANET_STATS_CHECKS=22 FAILURES=0 RESULT=PASS\n", report);
        fclose(report);
    }
#endif
    return 0;
}
