#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct StartTutorTrace {
    StarsStartTutorState *state;
    StarsI16 metric;
    StarsU32 make_result;
    StarsI16 access_result;
    StarsI16 alert_result;
    StarsI16 open_result;
    StarsI16 task_values[4];
    StarsI16 task_count;
    StarsI16 make_calls;
    StarsI16 metric_calls;
    StarsI16 get_string_calls;
    StarsI16 append_calls;
    StarsU16 append_ids[4];
    StarsI16 access_calls;
    StarsI16 alert_calls;
    StarsU16 alert_id;
    StarsI16 alert_type;
    StarsI16 open_calls;
    StarsU16 open_hwnd;
    StarsI16 open_startup;
    StarsI16 startup_flag_seen;
    StarsI16 child_calls;
    StarsI16 world_calls;
    StarsI16 dialog_calls;
    StarsU16 dialog_instance;
    StarsU16 dialog_resource;
    StarsU16 dialog_owner;
    StarsU32 dialog_proc;
    StarsI16 menu_calls;
    StarsI16 menu_id;
    StarsI16 post_calls;
    StarsU16 post_hwnd;
    StarsU16 post_message;
    StarsU16 post_wparam;
    StarsU32 post_lparam;
    StarsI16 destroy_calls;
    StarsU16 destroyed;
    StarsI16 show_window_calls;
    StarsU16 shown_window;
    StarsI16 shown_window_value;
    StarsI16 show_tutor_calls;
    StarsI16 show_tutor_value;
    StarsI16 invalidate_calls;
    StarsU16 invalidated_window;
    StarsI16 advance_calls;
    StarsI16 save_calls;
    StarsI16 end_calls;
    StarsI16 end_value;
} StartTutorTrace;

static StarsU32 make_proc(void *opaque, StarsU16 instance)
{
    StartTutorTrace *trace = (StartTutorTrace *)opaque;
    ++trace->make_calls;
    (void)instance;
    return trace->make_result;
}

static StarsI16 metric(void *opaque, StarsU16 index)
{
    StartTutorTrace *trace = (StartTutorTrace *)opaque;
    ++trace->metric_calls;
    (void)index;
    return trace->metric;
}

static StarsI16 get_string(void *opaque, StarsU16 id, StarsU8 *text,
    StarsU16 capacity)
{
    StartTutorTrace *trace = (StartTutorTrace *)opaque;
    ++trace->get_string_calls;
    (void)id;
    if (capacity >= 5U) {
        text[0] = 'B'; text[1] = 'A'; text[2] = 'S'; text[3] = 'E';
        text[4] = 0;
    }
    return 4;
}

static void append_string(void *opaque, StarsU8 *text, StarsU16 id)
{
    StartTutorTrace *trace = (StartTutorTrace *)opaque;
    StarsI16 slot = trace->append_calls;
    if (slot < 4)
        trace->append_ids[slot] = id;
    ++trace->append_calls;
    (void)text;
}

static StarsI16 access_file(void *opaque, const StarsU8 *text,
    StarsI16 mode)
{
    StartTutorTrace *trace = (StartTutorTrace *)opaque;
    ++trace->access_calls;
    (void)text; (void)mode;
    return trace->access_result;
}

static StarsI16 alert_ids(void *opaque, StarsU16 id, StarsI16 type)
{
    StartTutorTrace *trace = (StartTutorTrace *)opaque;
    ++trace->alert_calls;
    trace->alert_id = id;
    trace->alert_type = type;
    return trace->alert_result;
}

static StarsI16 open_game(void *opaque, StarsU16 hwnd, StarsI16 startup)
{
    StartTutorTrace *trace = (StartTutorTrace *)opaque;
    ++trace->open_calls;
    trace->open_hwnd = hwnd;
    trace->open_startup = startup;
    if (trace->state)
        trace->startup_flag_seen = (StarsI16)(trace->state->ini_flags & 1U);
    if (trace->state && trace->open_result > 0)
        trace->state->game_lid = 55UL;
    return trace->open_result;
}

static void create_children(void *opaque)
{
    StartTutorTrace *trace = (StartTutorTrace *)opaque;
    ++trace->child_calls;
}

static void create_world(void *opaque)
{
    StartTutorTrace *trace = (StartTutorTrace *)opaque;
    ++trace->world_calls;
}

static void create_dialog(void *opaque, StarsU16 instance, StarsU16 resource,
    StarsU16 owner, StarsU32 proc)
{
    StartTutorTrace *trace = (StartTutorTrace *)opaque;
    ++trace->dialog_calls;
    trace->dialog_instance = instance;
    trace->dialog_resource = resource;
    trace->dialog_owner = owner;
    trace->dialog_proc = proc;
    if (trace->state)
        trace->state->tutor_window = 88U;
}

static void initialize_menu(void *opaque, StarsI16 menu)
{
    StartTutorTrace *trace = (StartTutorTrace *)opaque;
    ++trace->menu_calls;
    trace->menu_id = menu;
}

static void post_message(void *opaque, StarsU16 hwnd, StarsU16 message,
    StarsU16 wparam, StarsU32 lparam)
{
    StartTutorTrace *trace = (StartTutorTrace *)opaque;
    ++trace->post_calls;
    trace->post_hwnd = hwnd; trace->post_message = message;
    trace->post_wparam = wparam; trace->post_lparam = lparam;
}

static void destroy_window(void *opaque, StarsU16 hwnd)
{
    StartTutorTrace *trace = (StartTutorTrace *)opaque;
    ++trace->destroy_calls;
    trace->destroyed = hwnd;
}

static void show_window(void *opaque, StarsU16 hwnd, StarsI16 show)
{
    StartTutorTrace *trace = (StartTutorTrace *)opaque;
    ++trace->show_window_calls;
    trace->shown_window = hwnd;
    trace->shown_window_value = show;
}

static void show_tutor(void *opaque, StarsI16 show)
{
    StartTutorTrace *trace = (StartTutorTrace *)opaque;
    ++trace->show_tutor_calls;
    trace->show_tutor_value = show;
}

static void invalidate(void *opaque, StarsU16 hwnd, StarsU16 erase)
{
    StartTutorTrace *trace = (StartTutorTrace *)opaque;
    ++trace->invalidate_calls;
    trace->invalidated_window = hwnd;
    (void)erase;
}

static void advance_tutor(void *opaque)
{
    StartTutorTrace *trace = (StartTutorTrace *)opaque;
    ++trace->advance_calls;
}

static StarsI16 task_done(void *opaque)
{
    StartTutorTrace *trace = (StartTutorTrace *)opaque;
    StarsI16 index = trace->task_count;
    ++trace->task_count;
    if (index < 4)
        return trace->task_values[index];
    return 1;
}

static void end_tutor(void *opaque, StarsI16 value)
{
    StartTutorTrace *trace = (StartTutorTrace *)opaque;
    ++trace->end_calls;
    trace->end_value = value;
}

static void save_game_state(void *opaque)
{
    StartTutorTrace *trace = (StartTutorTrace *)opaque;
    ++trace->save_calls;
}

static void setup(StarsStartTutorState *state, StarsStartTutorOps *ops,
    StartTutorTrace *trace, StarsU8 *base)
{
    (void)memset(state, 0, sizeof(*state));
    (void)memset(ops, 0, sizeof(*ops));
    (void)memset(trace, 0, sizeof(*trace));
    (void)memset(base, 0, 32);
    state->instance = 9U; state->title_window = 42U;
    state->frame_window = 50U; state->base_name = base;
    state->base_capacity = 32U; state->tutor_proc = 123UL;
    trace->state = state; trace->metric = 640; trace->make_result = 123UL;
    trace->access_result = 0; trace->alert_result = 7; trace->open_result = 1;
    ops->make_proc = make_proc; ops->get_system_metric = metric;
    ops->get_string = get_string; ops->append_string = append_string;
    ops->access_file = access_file; ops->open_game = open_game;
    ops->create_children = create_children; ops->create_world = create_world;
    ops->create_dialog = create_dialog;
    ops->initialize_menu = initialize_menu; ops->post_message = post_message;
    ops->destroy_window = destroy_window; ops->show_window = show_window;
    ops->show_tutor = show_tutor; ops->invalidate_rect = invalidate;
    ops->advance_tutor = advance_tutor; ops->task_done = task_done;
    ops->end_tutor = end_tutor; ops->alert_ids = alert_ids;
    ops->save_game_state = save_game_state;
}

static int check_fresh_restart(void)
{
    StarsStartTutorState state;
    StarsStartTutorOps ops;
    StartTutorTrace trace;
    StarsU8 base[32];

    setup(&state, &ops, &trace, base);
    trace.task_values[0] = 1; trace.task_values[1] = 0;
    stars_start_tutor(&state, 1, &ops, &trace);
    if (trace.save_calls != 1 || trace.make_calls != 0 ||
            trace.get_string_calls != 1 || trace.append_calls != 0 ||
            trace.access_calls != 0 || trace.world_calls != 1 ||
            trace.child_calls != 0 || trace.menu_id != 0 ||
            trace.post_hwnd != 50U || trace.post_message != 0x0111U ||
            trace.post_wparam != 0x0fa1U || trace.post_lparam != 0UL ||
            trace.destroyed != 42U || trace.shown_window != 50U ||
            trace.shown_window_value != 5 || trace.show_tutor_value != 1 ||
            trace.invalidated_window != 88U || trace.dialog_resource != 0x09c6U ||
            trace.dialog_owner != 50U || trace.dialog_proc != 123UL ||
            trace.metric_calls != 1 || state.scan_zoom != 0 ||
            state.grbit_scan != 0x04e0U || state.zip_valid != 1U ||
            state.gd_bits2 != 0x20U || state.tutor_ids_error != -1 ||
            state.tutor_idt != 0 || state.tutor_idt_bold != 0 ||
            state.tutor_flags != 0x4000U || trace.task_count != 2 ||
            trace.advance_calls != 1 || trace.show_tutor_calls != 1 ||
            trace.invalidate_calls != 1 || base[0] != 'B')
        return 1;
    return 0;
}

static int check_existing_game(void)
{
    StarsStartTutorState state;
    StarsStartTutorOps ops;
    StartTutorTrace trace;
    StarsU8 base[32];

    setup(&state, &ops, &trace, base);
    trace.metric = 1024; trace.task_values[0] = 0;
    stars_start_tutor(&state, 0, &ops, &trace);
    if (trace.metric_calls != 1 || state.scan_zoom != 2 ||
            trace.append_calls != 2 || trace.append_ids[0] != 0x148aU ||
            trace.append_ids[1] != 0x148eU || trace.access_calls != 1 ||
            trace.alert_id != 0x01e8U || trace.alert_result != 7 ||
            trace.open_calls != 1 || trace.open_hwnd != 50U ||
            trace.open_startup != 0 || trace.startup_flag_seen != 1 ||
            trace.child_calls != 1 || trace.world_calls != 0 ||
            state.game_lid != 55UL || (state.ini_flags & 1U) != 0U ||
            base[0] != 'B' || trace.show_tutor_calls != 1)
        return 1;
    return 0;
}

static int check_no_proc(void)
{
    StarsStartTutorState state;
    StarsStartTutorOps ops;
    StartTutorTrace trace;
    StarsU8 base[32];

    setup(&state, &ops, &trace, base);
    state.tutor_proc = 0UL; trace.make_result = 0UL;
    stars_start_tutor(&state, 1, &ops, &trace);
    if (trace.make_calls != 1 || trace.alert_calls != 1 ||
            trace.alert_id != 0x001aU || trace.alert_type != 0x10 ||
            trace.save_calls != 0 || trace.show_tutor_calls != 0 ||
            (state.gd_flags & 0x0800U) != 0U)
        return 1;
    return 0;
}

int main(void)
{
    int failures = 0;
    if (check_no_proc()) ++failures;
    if (check_fresh_restart()) ++failures;
    if (check_existing_game()) ++failures;
    if (failures == 0)
        puts("START_TUTOR_SOURCE=PASS");
    else
        puts("START_TUTOR_SOURCE=FAIL");
    return failures;
}
