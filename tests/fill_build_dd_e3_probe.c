#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 resets;
    StarsI16 adds;
    StarsI16 count_messages;
    StarsI16 ends;
    StarsI16 positions;
    StarsI16 event_count;
    StarsI16 enable_count;
    StarsU16 enable_items[12];
    StarsU16 enable_values[12];
    StarsI16 queued_calls;
    StarsI16 queued_last_index;
    StarsI16 override_count;
    StarsI16 part_skip;
    StarsI16 part_calls;
    StarsI16 player_calls;
    StarsI16 last_width;
    StarsI16 last_height;
    StarsU8 events[80];
    StarsU8 added[40][80];
    const StarsU8 *player_names[4];
} Trace;

static void event(Trace *trace, StarsU8 value)
{
    if (trace->event_count < (StarsI16)(sizeof trace->events - 1)) {
        trace->events[trace->event_count++] = value;
        trace->events[trace->event_count] = 0;
    }
}

static void send_message(void *context, StarsU16 hwnd, StarsU16 message,
    StarsU16 wparam, const StarsU8 *text)
{
    Trace *trace;

    trace = (Trace *)context;
    (void)hwnd;
    (void)wparam;
    if (message == 0x40b) {
        ++trace->resets;
        event(trace, 'R');
    } else if (message == 0x403) {
        if (trace->adds < 40 && text) {
            strncpy((char *)trace->added[trace->adds], (const char *)text,
                sizeof trace->added[trace->adds] - 1);
            trace->added[trace->adds][sizeof trace->added[trace->adds] - 1] = 0;
        }
        ++trace->adds;
        event(trace, 'A');
    } else if (message == 0x406) {
        ++trace->count_messages;
        event(trace, 'C');
    } else if (message == 0x40e) {
        ++trace->ends;
        event(trace, 'E');
    }
}

static void enable_window(void *context, StarsU16 item, StarsU16 enable)
{
    Trace *trace;

    trace = (Trace *)context;
    if (trace->enable_count < (StarsI16)(sizeof trace->enable_items /
            sizeof trace->enable_items[0])) {
        trace->enable_items[trace->enable_count] = item;
        trace->enable_values[trace->enable_count] = enable;
    }
    ++trace->enable_count;
}

static StarsU16 get_dialog_item(void *context, StarsU16 hwnd, StarsU16 id)
{
    (void)context;
    (void)hwnd;
    return (StarsU16)(id + 100);
}

static StarsI16 part_name(void *context, StarsI16 starbase_mode,
    StarsI16 index, StarsU8 *buffer, StarsU16 size)
{
    Trace *trace;

    trace = (Trace *)context;
    (void)starbase_mode;
    (void)size;
    ++trace->part_calls;
    if (index == trace->part_skip)
        return 0;
    sprintf((char *)buffer, "Part%d", (int)index);
    return 1;
}

static StarsI16 player_name(void *context, StarsI16 player, StarsU8 *buffer,
    StarsU16 size)
{
    Trace *trace;
    const char *name;

    trace = (Trace *)context;
    (void)size;
    ++trace->player_calls;
    name = (const char *)trace->player_names[player];
    strcpy((char *)buffer, name);
    return (StarsI16)strlen(name);
}

static StarsI16 queued(void *context, StarsI16 index, StarsI16 *value)
{
    Trace *trace;

    trace = (Trace *)context;
    ++trace->queued_calls;
    trace->queued_last_index = index;
    *value = (index == 0) ? 1 : 0;
    return 1;
}

static StarsI16 list_count(void *context, StarsU16 hwnd)
{
    Trace *trace;

    trace = (Trace *)context;
    (void)hwnd;
    event(trace, 'L');
    return trace->override_count >= 0 ? trace->override_count : trace->adds;
}

static void get_window_rect(void *context, StarsU16 hwnd, StarsRect *rect)
{
    (void)context;
    (void)hwnd;
    rect->left = 4;
    rect->top = 0;
    rect->right = 100;
    rect->bottom = 20;
}

static void set_window_pos(void *context, StarsU16 hwnd, StarsU16 insert,
    StarsI16 x, StarsI16 y, StarsI16 width, StarsI16 height, StarsU16 flags)
{
    Trace *trace;

    trace = (Trace *)context;
    (void)hwnd;
    (void)insert;
    (void)x;
    (void)y;
    (void)flags;
    ++trace->positions;
    trace->last_width = width;
    trace->last_height = height;
    event(trace, 'P');
}

static StarsI16 check(StarsI16 condition, StarsI16 *checks)
{
    ++*checks;
    return condition ? 0 : 1;
}

static void init_input(StarsFillBuildInput *input, Trace *trace)
{
    memset(input, 0, sizeof *input);
    memset(trace, 0, sizeof *trace);
    trace->override_count = -1;
    trace->part_skip = -1;
    input->dropdown = 3;
    input->slot_dialog = 4;
    input->line_height = 15;
    input->parent_width = 200;
    input->send_message = send_message;
    input->enable_window = enable_window;
    input->get_dialog_item = get_dialog_item;
    input->part_name = part_name;
    input->player_name = player_name;
    input->queued = queued;
    input->list_count = list_count;
    input->get_window_rect = get_window_rect;
    input->set_window_pos = set_window_pos;
    input->context = trace;
}

static StarsI16 has_enable(const Trace *trace, StarsU16 item, StarsU16 value)
{
    StarsI16 index;

    for (index = 0; index < trace->enable_count && index < 12; ++index)
        if (trace->enable_items[index] == item &&
                trace->enable_values[index] == value)
            return 1;
    return 0;
}

int main(void)
{
    StarsFillBuildDesign designs[16];
    StarsFillBuildDesign empty_designs[16];
    StarsFillBuildDesign enemy_designs[6];
    StarsFillBuildInput input;
    Trace trace;
    StarsI16 checks;
    StarsI16 failures;
    StarsI16 index;

    checks = 0;
    failures = 0;
    memset(designs, 0, sizeof designs);
    memset(empty_designs, 0, sizeof empty_designs);
    designs[0].visible = 1;
    strcpy((char *)designs[0].name, "Queued");
    designs[1].visible = 1;
    designs[1].has_hull = 1;
    strcpy((char *)designs[1].name, "Built");
    designs[2].visible = 0;
    strcpy((char *)designs[2].name, "Hidden");
    init_input(&input, &trace);
    input.mode = 0;
    input.designs = designs;
    stars_fill_build_dropdown(&input);
    failures += check(strcmp((char *)trace.events, "RAALCPE") == 0, &checks);
    failures += check(trace.adds == 2 && strcmp((char *)trace.added[0],
        "Queued") == 0 && strcmp((char *)trace.added[1], "Built") == 0,
        &checks);
    failures += check(trace.queued_calls == 1 && trace.queued_last_index == 0,
        &checks);
    failures += check(has_enable(&trace, 0x87a, 1) &&
        has_enable(&trace, 0x87b, 1) && !has_enable(&trace, 0x87c, 1),
        &checks);
    failures += check(trace.last_width == 196 && trace.last_height == 54,
        &checks);

    init_input(&input, &trace);
    input.mode = 0;
    input.designs = designs;
    trace.override_count = 99;
    stars_fill_build_dropdown(&input);
    failures += check(trace.last_height == 504 && trace.last_width == 196,
        &checks);

    init_input(&input, &trace);
    input.mode = 0;
    input.designs = empty_designs;
    stars_fill_build_dropdown(&input);
    failures += check(strcmp((char *)trace.events, "RLCPE") == 0 &&
        trace.adds == 0 && has_enable(&trace, 0x87a, 0), &checks);

    init_input(&input, &trace);
    input.mode = 0;
    input.starbase_mode = 1;
    input.designs = designs;
    stars_fill_build_dropdown(&input);
    failures += check(trace.adds == 2 && trace.last_height == 54,
        &checks);

    init_input(&input, &trace);
    input.mode = 1;
    input.designs = designs;
    trace.part_skip = 3;
    stars_fill_build_dropdown(&input);
    failures += check(trace.part_calls == 32 && trace.adds == 31,
        &checks);
    failures += check(strcmp((char *)trace.added[0], "Part0") == 0 &&
        strcmp((char *)trace.added[30], "Part31") == 0 &&
        has_enable(&trace, 0x87b, 0) && has_enable(&trace, 0x87c, 0),
        &checks);

    init_input(&input, &trace);
    input.mode = 1;
    input.starbase_mode = 1;
    input.designs = designs;
    trace.part_skip = -1;
    stars_fill_build_dropdown(&input);
    failures += check(trace.part_calls == 5 && trace.adds == 5,
        &checks);

    memset(enemy_designs, 0, sizeof enemy_designs);
    for (index = 0; index < 6; ++index) {
        enemy_designs[index].visible = 1;
        sprintf((char *)enemy_designs[index].name, "D%d", (int)index);
    }
    enemy_designs[2].visible = 0;
    init_input(&input, &trace);
    trace.player_names[0] = (const StarsU8 *)"Alpha";
    trace.player_names[1] = (const StarsU8 *)"Human";
    trace.player_names[2] = (const StarsU8 *)"Gamma";
    input.mode = 2;
    input.current_player = 1;
    input.player_count = 3;
    input.designs_per_player = 2;
    input.designs = designs;
    input.player_designs = enemy_designs;
    stars_fill_build_dropdown(&input);
    failures += check(trace.adds == 4 &&
        strcmp((char *)trace.added[0], "Alpha:D0") == 0 &&
        strcmp((char *)trace.added[1], "Alpha:D1") == 0 &&
        strcmp((char *)trace.added[2], "Gamma:D4") == 0, &checks);
    failures += check(trace.player_calls == 4 && trace.queued_calls == 0 &&
        has_enable(&trace, 0x87a, 1), &checks);

    init_input(&input, &trace);
    input.mode = 0;
    input.starbase_mode = 1;
    input.designs = designs;
    designs[10].visible = 1;
    strcpy((char *)designs[10].name, "OutOfRange");
    stars_fill_build_dropdown(&input);
    failures += check(trace.adds == 2 &&
        strstr((char *)trace.added[0], "OutOfRange") == (char *)0,
        &checks);

    printf("FILL_BUILD_DD_E3_CHECKS=%d FAILURES=%d RESULT=%s\n",
        (int)checks, (int)failures,
        failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
}
