#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 streams;
    StarsI16 wraps;
    StarsI16 ids[32];
    StarsI16 kinds[32];
    StarsI16 values[32];
    StarsI16 wrap_ids[8];
    StarsI16 environment_calls;
    StarsI16 desirability_calls;
    StarsI16 prepared;
    StarsI16 saw_environment_value;
} Trace;
static StarsI16 lookup(void *context, StarsPlanetStateInfoData *data)
{
    (void)context; (void)data; return 1;
}
static void select_font(void *context, StarsU16 hdc, StarsI16 font)
{
    (void)context; (void)hdc; (void)font;
}
static void stream(void *context, StarsU16 hdc, StarsI16 *x, StarsI16 y,
    const StarsPlanetStateInfoSegment *segment, StarsI16 print_mode)
{
    Trace *trace = (Trace *)context;
    (void)hdc; (void)y; (void)print_mode;
    if (trace->streams < 32) {
        trace->ids[trace->streams] = segment->string_id;
        trace->kinds[trace->streams] = segment->kind;
        trace->values[trace->streams] = segment->value;
    }
    ++trace->streams;
    *x = (StarsI16)(*x + 100);
}
static void wrap(void *context, StarsU16 hdc, StarsI16 *x, StarsI16 *y,
    const StarsPlanetStateInfoSegment *segment, StarsI16 width,
    StarsI16 print_mode)
{
    Trace *trace = (Trace *)context;
    (void)hdc; (void)y; (void)width; (void)print_mode;
    if (trace->wraps < 8)
        trace->wrap_ids[trace->wraps] = segment->string_id;
    ++trace->wraps;
    *x = (StarsI16)(*x + 2);
}
static void prepare_segment(void *context,
    StarsPlanetStateInfoSegment *segment)
{
    Trace *trace = (Trace *)context;
    ++trace->prepared;
    if (segment->kind == 2 && segment->value == 5)
        trace->saw_environment_value = 1;
}
static StarsI16 environment(void *context, StarsI16 variable, StarsI16 value,
    StarsI16 planet_id, StarsI16 environment_index)
{
    (void)variable; (void)planet_id; (void)environment_index;
    ++((Trace *)context)->environment_calls;
    return (StarsI16)(value + 1);
}
static StarsI16 desirability(void *context, StarsI16 planet_id,
    StarsI16 environment_index, StarsI16 current_value, StarsI16 candidate)
{
    (void)planet_id; (void)environment_index; (void)current_value;
    ++((Trace *)context)->desirability_calls;
    return (StarsI16)(candidate > 10 ? 20 : 5);
}
static void install(StarsPlanetStateInfoOps *ops, Trace *trace)
{
    memset(ops, 0, sizeof *ops);
    ops->lookup = lookup;
    ops->select_font = select_font;
    ops->stream_text = stream;
    ops->wrap_text = wrap;
    ops->environment = environment;
    ops->desirability = desirability;
    ops->prepare_segment = prepare_segment;
    ops->context = trace;
}
static void setup(StarsPlanetStateInfoInput *input,
    StarsPlanetStateInfoOps *ops, Trace *trace)
{
    memset(input, 0, sizeof *input);
    install(ops, trace);
    input->hdc = 1;
    input->print_mode = 1;
    input->line_height = 8;
    input->font_normal = 2;
    input->font_bold = 3;
    input->data.planet_id = 4;
    input->data.planet_var = 2;
    input->data.planet_value = 4;
    input->data.player_min = 5;
    input->data.player_max = 15;
    input->data.player_value = 7;
    input->data.planet_min = 3;
    input->data.planet_max = 12;
    input->data.player_value_secondary = 1;
    input->data.planet_max_secondary = 2;
    input->data.planet_min_secondary = 1;
    input->data.player_value_min_secondary = 2;
    input->data.difference_center = 7;
    input->data.planet_min_offset = 1;
    input->data.planet_max_offset = 1;
    input->data.environment_index = 0;
    input->ops = ops;
}
int main(void)
{
    StarsPlanetStateInfoInput input;
    StarsPlanetStateInfoOps ops;
    Trace trace;
    StarsPoint point;

    memset(&trace, 0, sizeof trace);
    setup(&input, &ops, &trace);
    point = stars_display_planet_state_info(&input);
    if (trace.streams != 21 || trace.wraps != 6 || point.x <= 100 ||
            trace.ids[1] != 0x253 || trace.ids[4] != 599 ||
            trace.ids[11] != 0x25a || trace.wrap_ids[0] != 0x166 ||
            trace.wrap_ids[5] != 0x168 || trace.prepared != 27 ||
            !trace.saw_environment_value || point.x <= 4 || point.y <= 4)
        return 1;
    memset(&trace, 0, sizeof trace);
    setup(&input, &ops, &trace);
    input.data.planet_value = -1;
    input.data.player_min = -1;
    input.data.planet_min = -1;
    point = stars_display_planet_state_info(&input);
    if (trace.streams != 8 || trace.wraps != 0 || trace.ids[2] != 0x254 ||
            trace.ids[4] != 0x255 || point.y != 32)
        return 1;
    memset(&trace, 0, sizeof trace);
    setup(&input, &ops, &trace);
    input.data.planet_value = 4;
    input.data.player_value = 7;
    input.data.planet_min = 1;
    input.data.player_min = 5;
    input.data.planet_max = 12;
    input.data.player_value_secondary = 0;
    input.data.planet_max_secondary = 1;
    stars_display_planet_state_info(&input);
    if (trace.wraps != 6 || trace.wrap_ids[0] != 0x166 ||
            trace.desirability_calls != 2)
        return 1;
    puts("PT_DISPLAY_PLANET_STATE_INFO_FORMAT_CHECKS=35 FAILURES=0 RESULT=PASS");
    return 0;
}
