#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 stream_count;
    StarsI16 wrap_count;
    StarsI16 environment_count;
    StarsI16 desirability_count;
    StarsI16 select_count;
    StarsI16 lookup_count;
    StarsI16 max_population_count;
    StarsI16 change_population_count;
    StarsI16 last_font;
    StarsI16 ids[64];
    StarsI16 kinds[64];
    StarsI32 values1[64];
    StarsI32 values2[64];
    StarsPlanetPopInfo pop_info;
    StarsI16 pop_lookup_ok;
    StarsI16 better_value;
    StarsI32 max_population_value;
    StarsI16 desirability_value;
    StarsI32 change_population_value;
} Trace;

static void reset_trace(Trace *trace)
{
    memset(trace, 0, sizeof *trace);
    trace->pop_lookup_ok = 1;
}

static StarsI16 contains_id(const Trace *trace, StarsI16 id)
{
    StarsI16 index;

    for (index = 0; index < trace->stream_count; ++index)
        if (trace->ids[index] == id)
            return 1;
    return 0;
}

static StarsI16 contains_kind_value(const Trace *trace, StarsI16 kind,
    StarsI32 value1)
{
    StarsI16 index;

    for (index = 0; index < trace->stream_count; ++index)
        if (trace->kinds[index] == kind && trace->values1[index] == value1)
            return 1;
    return 0;
}

static StarsI16 pop_lookup(void *context, StarsI16 planet_id,
    StarsPlanetPopInfo *output)
{
    Trace *trace;
    (void)planet_id;
    trace = (Trace *)context;
    ++trace->lookup_count;
    *output = trace->pop_info;
    return trace->pop_lookup_ok;
}

static void pop_prepare(void *context, StarsPlanetPopSegment *segment)
{
    (void)context;
    (void)segment;
}

static void pop_select(void *context, StarsU16 hdc, StarsI16 font)
{
    Trace *trace;
    (void)hdc;
    trace = (Trace *)context;
    ++trace->select_count;
    trace->last_font = font;
}

static void pop_record(Trace *trace, const StarsPlanetPopSegment *segment)
{
    if (trace->stream_count < 64) {
        trace->ids[trace->stream_count] = segment->string_id;
        trace->kinds[trace->stream_count] = segment->kind;
        trace->values1[trace->stream_count] = segment->value1;
        trace->values2[trace->stream_count] = segment->value2;
    }
    ++trace->stream_count;
}

static void pop_stream(void *context, StarsU16 hdc, StarsI16 *x,
    StarsI16 y, const StarsPlanetPopSegment *segment, StarsI16 print_mode)
{
    Trace *trace;
    (void)hdc;
    (void)y;
    (void)print_mode;
    trace = (Trace *)context;
    pop_record(trace, segment);
    *x = (StarsI16)(*x + 6);
}

static void pop_wrap(void *context, StarsU16 hdc, StarsI16 *x,
    StarsI16 *y, const StarsPlanetPopSegment *segment, StarsI16 maximum,
    StarsI16 print_mode)
{
    Trace *trace;
    (void)hdc;
    (void)y;
    (void)maximum;
    (void)print_mode;
    trace = (Trace *)context;
    ++trace->wrap_count;
    pop_record(trace, segment);
    *x = (StarsI16)(*x + 7);
}

static StarsI32 pop_max_population(void *context, StarsI16 planet_id,
    StarsI16 viewer)
{
    Trace *trace;
    (void)planet_id;
    (void)viewer;
    trace = (Trace *)context;
    ++trace->max_population_count;
    return trace->max_population_value;
}

static StarsI16 pop_desirability(void *context, StarsI16 planet_id,
    StarsI16 viewer)
{
    Trace *trace;
    (void)planet_id;
    (void)viewer;
    trace = (Trace *)context;
    ++trace->desirability_count;
    return trace->desirability_value;
}

static StarsI32 pop_change_population(void *context, StarsI16 planet_id)
{
    Trace *trace;
    (void)planet_id;
    trace = (Trace *)context;
    ++trace->change_population_count;
    return trace->change_population_value;
}

static void setup_pop(StarsPlanetPopInfoInput *input,
    StarsPlanetPopInfoOps *ops, Trace *trace)
{
    memset(input, 0, sizeof *input);
    memset(ops, 0, sizeof *ops);
    input->hdc = 7;
    input->planet_id = 12;
    input->viewer_player = 2;
    input->line_height = 10;
    input->font_normal = 3;
    input->font_bold = 4;
    ops->lookup_planet = pop_lookup;
    ops->select_font = pop_select;
    ops->stream_text = pop_stream;
    ops->wrap_text = pop_wrap;
    ops->max_population = pop_max_population;
    ops->desirability = pop_desirability;
    ops->change_population = pop_change_population;
    ops->prepare_segment = pop_prepare;
    ops->context = trace;
    input->ops = ops;
}

static void state_prepare(void *context, StarsPlanetStateInfoSegment *segment)
{
    (void)context;
    (void)segment;
}

static void state_select(void *context, StarsU16 hdc, StarsI16 font)
{
    Trace *trace;
    (void)hdc;
    trace = (Trace *)context;
    ++trace->select_count;
    trace->last_font = font;
}

static void state_record(Trace *trace,
    const StarsPlanetStateInfoSegment *segment)
{
    if (trace->stream_count < 64) {
        trace->ids[trace->stream_count] = segment->string_id;
        trace->kinds[trace->stream_count] = segment->kind;
        trace->values1[trace->stream_count] = segment->value;
        trace->values2[trace->stream_count] = segment->bold;
    }
    ++trace->stream_count;
}

static void state_stream(void *context, StarsU16 hdc, StarsI16 *x,
    StarsI16 y, const StarsPlanetStateInfoSegment *segment,
    StarsI16 print_mode)
{
    Trace *trace;
    (void)hdc;
    (void)y;
    (void)print_mode;
    trace = (Trace *)context;
    state_record(trace, segment);
    *x = (StarsI16)(*x + 5);
}

static void state_wrap(void *context, StarsU16 hdc, StarsI16 *x,
    StarsI16 *y, const StarsPlanetStateInfoSegment *segment,
    StarsI16 maximum, StarsI16 print_mode)
{
    Trace *trace;
    (void)hdc;
    (void)y;
    (void)maximum;
    (void)print_mode;
    trace = (Trace *)context;
    ++trace->wrap_count;
    state_record(trace, segment);
    *x = (StarsI16)(*x + 8);
}

static StarsI16 state_environment(void *context, StarsI16 planet_var,
    StarsI16 value, StarsI16 planet_id, StarsI16 environment_index)
{
    Trace *trace;
    (void)planet_var;
    (void)planet_id;
    (void)environment_index;
    trace = (Trace *)context;
    ++trace->environment_count;
    return (StarsI16)(value + 10);
}

static StarsI16 state_desirability(void *context, StarsI16 planet_id,
    StarsI16 environment_index, StarsI16 planet_value,
    StarsI16 environment_value)
{
    Trace *trace;
    (void)planet_id;
    (void)environment_index;
    (void)planet_value;
    trace = (Trace *)context;
    ++trace->desirability_count;
    return environment_value > trace->better_value ? 90 : 20;
}

static void setup_state(StarsPlanetStateInfoInput *input,
    StarsPlanetStateInfoOps *ops, Trace *trace)
{
    memset(input, 0, sizeof *input);
    memset(ops, 0, sizeof *ops);
    input->hdc = 8;
    input->line_height = 10;
    input->font_normal = 5;
    input->font_bold = 6;
    ops->select_font = state_select;
    ops->stream_text = state_stream;
    ops->wrap_text = state_wrap;
    ops->environment = state_environment;
    ops->desirability = state_desirability;
    ops->prepare_segment = state_prepare;
    ops->context = trace;
    input->ops = ops;
}

int main(void)
{
    StarsPlanetPopInfoInput pop_input;
    StarsPlanetPopInfoOps pop_ops;
    StarsPlanetStateInfoInput state_input;
    StarsPlanetStateInfoOps state_ops;
    Trace trace;
    StarsPoint point;
    StarsI16 checks;
    StarsI16 failures;

    checks = 0;
    failures = 0;
    point = stars_display_planet_pop_info((StarsPlanetPopInfoInput *)0);
    ++checks;
    if (point.x != 0 || point.y != 0)
        ++failures;

    reset_trace(&trace);
    setup_pop(&pop_input, &pop_ops, &trace);
    trace.pop_info.owner = 2;
    trace.pop_info.mineral_rate = 0;
    trace.pop_info.habitability_class = 1;
    point = stars_display_planet_pop_info(&pop_input);
    ++checks;
    if (trace.lookup_count != 1 || !contains_id(&trace, 0x244) ||
            !contains_id(&trace, 0xc24) || point.x <= 4 || point.y <= 4)
        ++failures;

    reset_trace(&trace);
    setup_pop(&pop_input, &pop_ops, &trace);
    trace.pop_info.owner = -1;
    point = stars_display_planet_pop_info(&pop_input);
    ++checks;
    if (!contains_id(&trace, 0x248) || contains_id(&trace, 0x244))
        ++failures;

    reset_trace(&trace);
    setup_pop(&pop_input, &pop_ops, &trace);
    trace.pop_info.owner = 4;
    trace.pop_info.habitability_class = 2;
    point = stars_display_planet_pop_info(&pop_input);
    ++checks;
    if (!contains_id(&trace, 0x245) || !contains_id(&trace, 0x247) ||
            contains_id(&trace, 0x246))
        ++failures;

    reset_trace(&trace);
    setup_pop(&pop_input, &pop_ops, &trace);
    trace.pop_info.owner = 4;
    trace.pop_info.selected_player = 4;
    trace.pop_info.habitability_class = 3;
    trace.pop_info.foreign_environment_class = 4;
    trace.pop_info.max_population = 20;
    trace.pop_info.desirability_tenths = -15;
    trace.desirability_value = -15;
    point = stars_display_planet_pop_info(&pop_input);
    ++checks;
    if (!contains_id(&trace, 0x246) ||
            !contains_kind_value(&trace, 4, 16) ||
            !contains_kind_value(&trace, 5, 1) ||
            !contains_id(&trace, 0x24a))
        ++failures;

    reset_trace(&trace);
    setup_pop(&pop_input, &pop_ops, &trace);
    trace.pop_info.owner = 2;
    trace.pop_info.habitability_class = 3;
    trace.pop_info.mineral_rate = 10;
    trace.pop_info.max_population = 20;
    trace.pop_info.desirability_tenths = 60;
    trace.pop_info.population_change = 5;
    trace.max_population_value = 30;
    trace.desirability_value = 60;
    trace.change_population_value = 5;
    point = stars_display_planet_pop_info(&pop_input);
    ++checks;
    if (trace.max_population_count != 1 ||
            trace.change_population_count != 1 ||
            !contains_id(&trace, 0x24c) ||
            !contains_kind_value(&trace, 6, 30) ||
            !contains_id(&trace, 0x213) ||
            !contains_kind_value(&trace, 7, 5) ||
            !contains_id(&trace, 0x240) || point.y <= 4)
        ++failures;

    reset_trace(&trace);
    setup_state(&state_input, &state_ops, &trace);
    state_input.data.planet_id = 9;
    state_input.data.planet_var = 2;
    state_input.data.planet_value = 3;
    state_input.data.player_min = -1;
    state_input.data.planet_min = -1;
    point = stars_display_planet_state_info(&state_input);
    ++checks;
    if (!contains_id(&trace, 0x253) || !contains_id(&trace, 0xc14) ||
            !contains_id(&trace, 0x255) || !contains_id(&trace, 0xc16) ||
            trace.environment_count != 1 || point.x <= 4)
        ++failures;

    reset_trace(&trace);
    setup_state(&state_input, &state_ops, &trace);
    state_input.data.planet_id = 9;
    state_input.data.planet_var = 2;
    state_input.data.planet_value = 10;
    state_input.data.player_min = 5;
    state_input.data.player_max = 20;
    state_input.data.player_value = 15;
    state_input.data.planet_min = 8;
    state_input.data.planet_max = 18;
    state_input.data.player_value_secondary = 1;
    state_input.data.planet_max_secondary = 2;
    state_input.data.planet_min_secondary = 1;
    state_input.data.player_value_min_secondary = 2;
    state_input.data.difference_center = 12;
    state_input.data.planet_min_offset = 6;
    state_input.data.planet_max_offset = 19;
    trace.better_value = 17;
    point = stars_display_planet_state_info(&state_input);
    ++checks;
    if (!contains_id(&trace, 599) || !contains_id(&trace, 0xc18) ||
            !contains_id(&trace, 0x25a) || !contains_id(&trace, 0xc1a) ||
            trace.environment_count < 4 || point.y <= 4)
        ++failures;

    reset_trace(&trace);
    setup_state(&state_input, &state_ops, &trace);
    state_input.data.planet_id = 9;
    state_input.data.planet_var = 2;
    state_input.data.planet_value = 3;
    state_input.data.player_min = 5;
    state_input.data.player_max = 20;
    state_input.data.player_value = 15;
    state_input.data.planet_min = 8;
    state_input.data.planet_max = 18;
    state_input.data.player_value_secondary = 1;
    state_input.data.planet_max_secondary = 2;
    state_input.data.planet_min_secondary = 1;
    state_input.data.player_value_min_secondary = 2;
    state_input.data.difference_center = 12;
    state_input.data.planet_min_offset = 6;
    state_input.data.planet_max_offset = 19;
    trace.better_value = 17;
    point = stars_display_planet_state_info(&state_input);
    ++checks;
    if (trace.wrap_count < 5 || !contains_id(&trace, 0x166) ||
            !contains_id(&trace, 0x167) || trace.desirability_count != 2)
        ++failures;

    (void)checks;
    if (failures == 0)
        puts("PLANET_INFO_BRANCH_CHECKS=9 FAILURES=0 RESULT=PASS");
    else
        puts("PLANET_INFO_BRANCH_CHECKS=9 FAILURES=1 RESULT=FAIL");
    return failures != 0;
}
