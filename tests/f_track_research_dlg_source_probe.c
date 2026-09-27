#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 steps;
    StarsI16 step_calls;
    StarsI16 future;
    StarsI16 note;
    StarsI16 redraws;
} Trace;

static StarsI16 track_step(void *context, StarsI16 direction,
    StarsI16 step)
{
    Trace *trace = (Trace *)context;
    (void)direction; (void)step;
    ++trace->step_calls;
    return trace->step_calls <= trace->steps;
}

static void show_future(void *context, StarsI16 index)
{
    Trace *trace = (Trace *)context;
    trace->future = index;
}

static void show_note(void *context, StarsI16 string_id)
{
    Trace *trace = (Trace *)context;
    trace->note = string_id;
}

static StarsI16 race_flag(void *context, StarsI16 flag)
{
    (void)context;
    return flag == 4 ? 1 : 0;
}

static StarsI32 project(void *context, StarsI16 percent)
{
    (void)context;
    return (StarsI32)percent * 10L;
}

static void redraw(void *context)
{
    Trace *trace = (Trace *)context;
    ++trace->redraws;
}

static void initialize(StarsFTrackResearchDlgInput *input, Trace *trace)
{
    memset(input, 0, sizeof *input);
    memset(trace, 0, sizeof *trace);
    input->spin_top_left = 0;
    input->spin_top_top = 0;
    input->spin_top_right = 10;
    input->spin_top_bottom = 10;
    input->spin_bottom_left = 0;
    input->spin_bottom_top = 20;
    input->spin_bottom_right = 10;
    input->spin_bottom_bottom = 30;
    input->future_top = 40;
    input->future_count = 3;
    input->note_top = 80;
    input->resource_left = 0;
    input->resource_right = 100;
    input->font_height = 10;
    input->research_percent = (StarsI16 *)0;
    input->track_step = track_step;
    input->show_future_tech = show_future;
    input->show_tech_note = show_note;
    input->race_flag = race_flag;
    input->project_spending = project;
    input->redraw = redraw;
    input->context = trace;
}

int main(void)
{
    StarsFTrackResearchDlgInput input;
    Trace trace;
    StarsI16 percent;
    StarsI32 budget;
    int failures = 0;

    initialize(&input, &trace);
    percent = 50;
    budget = 0L;
    trace.steps = 2;
    input.research_percent = &percent;
    input.research_budget = &budget;
    input.x = 5;
    input.y = 5;
    if (!stars_f_track_research_dlg(&input) || percent != 52 ||
            budget != 520L || trace.redraws != 2)
        ++failures;

    initialize(&input, &trace);
    input.x = 5;
    input.y = 65;
    if (!stars_f_track_research_dlg(&input) || trace.future != 2)
        ++failures;

    initialize(&input, &trace);
    input.x = 50;
    input.y = 85;
    if (!stars_f_track_research_dlg(&input) || trace.note != 0x14c)
        ++failures;

#ifndef TARGET_NATIVE
    printf("F_TRACK_RESEARCH_CHECKS=3 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
#endif
    return failures != 0;
}
