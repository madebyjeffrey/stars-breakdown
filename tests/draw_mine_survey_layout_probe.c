#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsDrawMineSurveyPattern patterns[20];
    int count;
} Trace;

static StarsU16 create_dc(void *v, StarsU16 hdc)
{
    (void)v;
    (void)hdc;
    return 19;
}

static void delete_dc(void *v, StarsU16 hdc)
{
    (void)v;
    (void)hdc;
}

static void color(void *v, StarsU16 hdc, StarsU32 value)
{
    (void)v;
    (void)hdc;
    (void)value;
}

static void mode(void *v, StarsU16 hdc, StarsU16 value)
{
    (void)v;
    (void)hdc;
    (void)value;
}

static StarsU16 get_mode(void *v, StarsU16 hdc)
{
    (void)v;
    (void)hdc;
    return 1;
}

static StarsU32 get_color(void *v, StarsU16 hdc)
{
    (void)v;
    (void)hdc;
    return 0;
}

static StarsI16 fleet(void *v, StarsU16 dc,
    StarsDrawMineSurveyRect *r, StarsI16 id)
{
    (void)v;
    (void)dc;
    (void)r;
    (void)id;
    return 1;
}

static StarsI16 thing(void *v, StarsU16 dc,
    StarsDrawMineSurveyRect *r, StarsI16 id)
{
    (void)v;
    (void)dc;
    (void)r;
    (void)id;
    return 1;
}

static void pattern(void *v, StarsU16 dc,
    const StarsDrawMineSurveyPattern *value)
{
    Trace *trace = (Trace *)v;
    (void)dc;
    if (trace->count < (int)(sizeof trace->patterns / sizeof trace->patterns[0]))
        trace->patterns[trace->count] = *value;
    ++trace->count;
}

static int same_pattern(const StarsDrawMineSurveyPattern *actual,
    StarsI16 x, StarsI16 y, StarsI16 width, StarsI16 height,
    StarsU32 rop, StarsI16 brush)
{
    return actual->x == x && actual->y == y && actual->width == width &&
        actual->height == height && actual->rop == rop &&
        actual->brush_role == brush;
}

static void initialize_ops(StarsDrawMineSurveyOps *ops)
{
    (void)memset(ops, 0, sizeof *ops);
    ops->create_compatible_dc = create_dc;
    ops->delete_dc = delete_dc;
    ops->set_background = color;
    ops->set_text_color = color;
    ops->get_background_mode = get_mode;
    ops->get_background_color = get_color;
    ops->get_text_color = get_color;
    ops->set_background_mode = mode;
    ops->draw_fleet_panel = fleet;
    ops->draw_thing_panel = thing;
    ops->draw_panel_pattern = pattern;
}

int main(void)
{
    StarsDrawMineSurveyOps ops;
    StarsDrawMineSurveyInput input;
    StarsDrawMineSurveyThingInfo thing_info;
    Trace trace;
    static const StarsU16 fleet_values[16] = { 0 };
    int failures = 0;

    initialize_ops(&ops);
    (void)memset(&input, 0, sizeof input);
    (void)memset(&thing_info, 0, sizeof thing_info);
    (void)memset(&trace, 0, sizeof trace);
    ops.context = &trace;
    input.hdc = 3;
    input.rect.left = 10;
    input.rect.top = 20;
    input.rect.right = 110;
    input.rect.bottom = 140;
    input.selected_kind = 4;
    input.selected_full_kind = 2;
    input.selected_fleet = 5;
    input.fleet_ship_counts = fleet_values;
    input.ops = &ops;
    stars_draw_mine_survey(&input);
    if (trace.count != 13)
        ++failures;
    if (!same_pattern(&trace.patterns[0], 16, 26, 70, 2,
            0xf00021UL, 0) ||
            !same_pattern(&trace.patterns[2], 32, 94, 2, 38,
                0xf00021UL, 0) ||
            !same_pattern(&trace.patterns[10], 68, 96, 2, 34,
                0xf00021UL, 1) ||
            !same_pattern(&trace.patterns[11], 18, 28, 66, 66,
                0x42UL, 1) ||
            !same_pattern(&trace.patterns[12], 34, 94, 34, 36,
                0x42UL, 1))
        ++failures;

    (void)memset(&trace, 0, sizeof trace);
    thing_info.id_full = (StarsU16)((1U << 13) | 17U);
    input.selected_kind = 8;
    input.selected_thing = 2;
    input.thing_info = &thing_info;
    stars_draw_mine_survey(&input);
    if (trace.count != 13 ||
            !same_pattern(&trace.patterns[2], 24, 94, 2, 38,
                0xf00021UL, 0) ||
            !same_pattern(&trace.patterns[10], 42, 96, 2, 34,
                0xf00021UL, 1) ||
            !same_pattern(&trace.patterns[12], 25, 94, 34, 36,
                0x42UL, 1))
        ++failures;

    (void)memset(&trace, 0, sizeof trace);
    thing_info.id_full = (StarsU16)((2U << 13) | 17U);
    stars_draw_mine_survey(&input);
    if (trace.count != 7 ||
            !same_pattern(&trace.patterns[2], 17, 94, 68, 1,
                0xf00021UL, 1) ||
            !same_pattern(&trace.patterns[6], 17, 28, 66, 66,
                0x42UL, 1))
        ++failures;

#ifndef TARGET_NATIVE
    printf("DRAW_MINE_SURVEY_LAYOUT_CHECKS=13 FAILURES=%d RESULT=%s\n",
        failures, failures == 0 ? "PASS" : "FAIL");
#endif
#ifdef TARGET_NATIVE
    {
        FILE *report = fopen("DMSLAY.TXT", "w");
        if (!report)
            return 7;
        fprintf(report,
            "DRAW_MINE_SURVEY_LAYOUT_CHECKS=13 FAILURES=%d RESULT=%s\n",
            failures, failures == 0 ? "PASS" : "FAIL");
        fclose(report);
    }
#endif
    return failures != 0;
}
