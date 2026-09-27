#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Position {
    StarsU16 window;
    StarsI16 x;
    StarsI16 y;
    StarsI16 width;
    StarsI16 height;
    StarsU16 flags;
} Position;

typedef struct Trace {
    StarsProductionDlgState *state;
    Position positions[24];
    StarsU16 disabled[4];
    StarsI16 position_count;
    StarsI16 disabled_count;
    StarsI16 initialized;
    StarsI16 sticky;
} Trace;

static int layout_checks;

static void fail_check(int condition, int *failures)
{
    ++layout_checks;
    if (!condition)
        ++*failures;
}

static void client_rect(void *context, StarsU16 window, StarsRect *rect)
{
    Trace *trace = (Trace *)context;
    (void)window;
    rect->left = 0;
    rect->top = 0;
    rect->right = trace->state->compact_mode ? 0x262 : 0x2f8;
    rect->bottom = trace->state->compact_mode ?
        (StarsI16)(trace->state->arial8_height * 24 + 24) : 0x244;
}

static StarsU16 dialog_item(void *context, StarsU16 window, StarsU16 id)
{
    (void)context;
    (void)window;
    return (StarsU16)(id + 100U);
}

static void enable_window(void *context, StarsU16 window, StarsU16 enabled)
{
    Trace *trace = (Trace *)context;
    if (!enabled && trace->disabled_count < 4)
        trace->disabled[trace->disabled_count++] = window;
}

static void set_window_pos(void *context, StarsU16 window,
    StarsU16 insert_after, StarsI16 x, StarsI16 y, StarsI16 width,
    StarsI16 height, StarsU16 flags)
{
    Trace *trace = (Trace *)context;
    Position *position;
    (void)insert_after;
    if (trace->position_count >= 24)
        return;
    position = &trace->positions[trace->position_count++];
    position->window = window;
    position->x = x;
    position->y = y;
    position->width = width;
    position->height = height;
    position->flags = flags;
}

static void initialize_dialog(void *context, StarsU16 window,
    StarsU16 message, StarsU32 parameter)
{
    Trace *trace = (Trace *)context;
    (void)window;
    (void)message;
    (void)parameter;
    ++trace->initialized;
}

static void sticky_position(void *context, StarsU16 window, StarsI16 save)
{
    Trace *trace = (Trace *)context;
    (void)window;
    (void)save;
    ++trace->sticky;
}

static void reset_trace(Trace *trace)
{
    memset(trace->positions, 0, sizeof trace->positions);
    memset(trace->disabled, 0, sizeof trace->disabled);
    trace->position_count = 0;
    trace->disabled_count = 0;
    trace->initialized = 0;
    trace->sticky = 0;
}

static void install_ops(StarsProductionDlgOps *ops, Trace *trace)
{
    memset(ops, 0, sizeof *ops);
    ops->get_client_rect = client_rect;
    ops->get_dialog_item = dialog_item;
    ops->enable_window = enable_window;
    ops->set_window_pos = set_window_pos;
    ops->initialize_dialog = initialize_dialog;
    ops->sticky_position = sticky_position;
    ops->context = trace;
}

static int run_full_case(void)
{
    StarsProductionDlgState state;
    StarsProductionDlgOps ops;
    StarsProductionDlgInput input;
    Trace trace;
    int failures;
    int index;

    memset(&state, 0, sizeof state);
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    state.arial8_height = 8;
    state.selected_player_planets = 1;
    state.compact_mode = 0;
    trace.state = &state;
    install_ops(&ops, &trace);
    input.state = &state;
    input.ops = &ops;
    reset_trace(&trace);
    stars_production_dlg(&input, 10, 272, 0, 0);

    failures = 0;
    fail_check(trace.position_count == 11, &failures);
    fail_check(trace.disabled_count == 2, &failures);
    fail_check(trace.disabled[0] == (StarsU16)(0x42e + 100), &failures);
    fail_check(trace.disabled[1] == (StarsU16)(0x42f + 100), &failures);
    fail_check(trace.initialized == 1 && trace.sticky == 1, &failures);
    fail_check(trace.positions[0].window == 10 &&
        trace.positions[0].width == 0x2f8 &&
        trace.positions[0].height == 0x244 &&
        trace.positions[0].flags == 6, &failures);
    fail_check(trace.positions[1].window == (StarsU16)(0x08b + 100) &&
        trace.positions[1].x == 6 && trace.positions[1].y == 562 &&
        trace.positions[1].width == 422 && trace.positions[1].height == 12 &&
        trace.positions[1].flags == 4, &failures);
    for (index = 0; index < 4; ++index) {
        Position *position = &trace.positions[index + 2];
        fail_check(position->window ==
            (StarsU16)(((index == 0 ? 0x42e : index == 1 ? 0x42f :
            index == 2 ? 1 : 2) + 100)), &failures);
        fail_check(position->x == (StarsI16)(434 + index * 81) &&
            position->y == 562 && position->width == 75 &&
            position->height == 12 && position->flags == 4, &failures);
    }
    for (index = 0; index < 5; ++index) {
        Position *position = &trace.positions[index + 6];
        StarsU16 ids[5];
        ids[0] = 0x418; ids[1] = 0x419; ids[2] = 0x439;
        ids[3] = 0x43a; ids[4] = 0x42d;
        fail_check(position->window == (StarsU16)(ids[index] + 100) &&
            position->x == 331 && position->y == (StarsI16)(8 + index * 20) &&
            position->width == 93 && position->height == 12 &&
            position->flags == 4, &failures);
    }
    return failures;
}

static int run_compact_case(void)
{
    StarsProductionDlgState state;
    StarsProductionDlgOps ops;
    StarsProductionDlgInput input;
    Trace trace;
    int failures;

    memset(&state, 0, sizeof state);
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    state.arial8_height = 8;
    state.selected_player_planets = 3;
    state.compact_mode = 1;
    trace.state = &state;
    install_ops(&ops, &trace);
    input.state = &state;
    input.ops = &ops;
    reset_trace(&trace);
    stars_production_dlg(&input, 10, 272, 0, 0);

    failures = 0;
    fail_check(trace.position_count == 11 && trace.disabled_count == 0,
        &failures);
    fail_check(trace.positions[0].width == 0x262 &&
        trace.positions[0].height == 216 && trace.positions[0].flags == 6,
        &failures);
    fail_check(trace.positions[1].x == 6 && trace.positions[1].y == 198 &&
        trace.positions[1].width == 339 && trace.positions[1].height == 12,
        &failures);
    return failures;
}

int main(void)
{
    int checks;
    int failures;

    layout_checks = 0;
    failures = run_full_case() + run_compact_case();
    checks = layout_checks;
    (void)checks;
    if (failures)
        puts("PRODUCTION_DLG_LAYOUT_CHECKS=23 FAILURES=1 RESULT=FAIL");
    else
        puts("PRODUCTION_DLG_LAYOUT_CHECKS=23 FAILURES=0 RESULT=PASS");
#ifdef TARGET_NATIVE
    {
        FILE *report = fopen("PDGER.TXT", "w");
        if (!report)
            return 2;
        fprintf(report, "PRODUCTION_DLG_LAYOUT_CHECKS=23 FAILURES=%d RESULT=%s\n",
            failures, failures ? "FAIL" : "PASS");
        fclose(report);
    }
#endif
    return failures ? 1 : 0;
}
