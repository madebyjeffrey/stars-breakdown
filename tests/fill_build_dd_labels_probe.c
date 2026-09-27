#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 resets;
    StarsI16 adds;
    StarsI16 ends;
    StarsI16 disable_calls;
    StarsI16 ids[16];
    StarsI16 id_count;
    StarsI16 list_count;
} Trace;

static void send_message(void *context, StarsU16 hwnd, StarsU16 message,
    StarsU16 wparam, const StarsU8 *text)
{
    Trace *trace;

    trace = (Trace *)context;
    (void)hwnd;
    (void)wparam;
    (void)text;
    if (message == 0x40b)
        ++trace->resets;
    else if (message == 0x403)
        ++trace->adds;
    else if (message == 0x40e)
        ++trace->ends;
}

static void enable_window(void *context, StarsU16 item, StarsU16 enable)
{
    Trace *trace;

    trace = (Trace *)context;
    (void)item;
    if (!enable)
        ++trace->disable_calls;
}

static StarsU16 get_dialog_item(void *context, StarsU16 hwnd, StarsU16 id)
{
    (void)context;
    (void)hwnd;
    return (StarsU16)(id + 10);
}

static const StarsU8 *compressed_string(void *context, StarsU16 id)
{
    Trace *trace;

    trace = (Trace *)context;
    if (trace->id_count < 16)
        trace->ids[trace->id_count++] = (StarsI16)id;
    return (const StarsU8 *)"label";
}

static StarsI16 list_count(void *context, StarsU16 hwnd)
{
    Trace *trace;

    trace = (Trace *)context;
    (void)hwnd;
    trace->list_count = trace->adds;
    return trace->list_count;
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
    (void)context;
    (void)hwnd;
    (void)insert;
    (void)x;
    (void)y;
    (void)width;
    (void)height;
    (void)flags;
}

static StarsI16 check(StarsI16 condition, StarsI16 *checks)
{
    ++*checks;
    return condition ? 0 : 1;
}

static void reset_trace(Trace *trace)
{
    memset(trace, 0, sizeof *trace);
}

static StarsI16 run_case(StarsI16 starbase_mode, StarsI16 mode,
    const StarsI16 *expected, StarsI16 expected_count, StarsI16 *checks)
{
    StarsFillBuildInput input;
    Trace trace;
    StarsI16 index;
    StarsI16 failures;

    memset(&input, 0, sizeof input);
    reset_trace(&trace);
    input.dropdown = 3;
    input.slot_dialog = 4;
    input.starbase_mode = starbase_mode;
    input.mode = mode;
    input.line_height = 15;
    input.parent_width = 200;
    input.send_message = send_message;
    input.enable_window = enable_window;
    input.get_dialog_item = get_dialog_item;
    input.compressed_string = compressed_string;
    input.list_count = list_count;
    input.get_window_rect = get_window_rect;
    input.set_window_pos = set_window_pos;
    input.context = &trace;
    stars_fill_build_dropdown(&input);
    failures = 0;
    failures += check(trace.resets == 1 && trace.ends == 1, checks);
    failures += check(trace.adds == expected_count &&
        trace.list_count == expected_count, checks);
    failures += check(trace.id_count == expected_count, checks);
    for (index = 0; index < expected_count; ++index)
        failures += check(trace.ids[index] == expected[index], checks);
    failures += check(trace.disable_calls == 3, checks);
    return failures;
}

int main(void)
{
    static const StarsI16 normal_ids[13] = {
        0x052, 0x054, 0x056, 0x058, 0x05a, 0x05c, 0x05e,
        0x060, 0x062, 0x064, 0x066, 0x068, 0x06a
    };
    static const StarsI16 starbase_ids[8] = {
        0x07c, 0x07e, 0x080, 0x082, 0x084, 0x086, 0x088, 0x08a
    };
    StarsI16 checks;
    StarsI16 failures;

    checks = 0;
    failures = 0;
    failures += run_case(0, 3, normal_ids, 13, &checks);
    failures += run_case(0, 4, normal_ids, 13, &checks);
    failures += run_case(1, 3, starbase_ids, 8, &checks);
    failures += run_case(1, 4, starbase_ids, 8, &checks);
    printf("FILL_BUILD_DD_LABELS_E2_CHECKS=%d FAILURES=%d RESULT=%s\n",
        (int)checks, (int)failures,
        failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
}
