/* Semantic reconstruction of BringUpHostDlg. */
#include "core.h"

/* NE 5:5FFC-6240. Dialog and generation services remain explicit callbacks;
 * the original flag and retry-loop behavior is retained here. */
void stars_bring_up_host_dialog(StarsBringUpHostState *state,
    const StarsBringUpHostOps *ops, void *context)
{
    StarsU32 proc_instance;
    StarsI16 result;
    StarsI16 width;
    StarsI16 height;
    StarsI16 key;

    if (!state || !ops) return;
    if (!(state->gd_low_flags & 8U)) {
        if (!(state->gd_high_flags & 0x20U) && ops->mark_file)
            ops->mark_file(context, 2, -1, 1, 1);
        state->gd_low_flags = (StarsU16)(state->gd_low_flags | 8U);
    }
    if (ops->show_window) ops->show_window(context, state->frame, 0U);
    if (!(state->ini_flags & 4U)) {
        for (;;) {
            proc_instance = ops->make_proc_instance ?
                ops->make_proc_instance(context, state->instance) : 0UL;
            result = ops->dialog_box ? ops->dialog_box(context, 0x73U,
                state->frame, proc_instance) : (StarsI16)-1;
            if (ops->free_proc_instance)
                ops->free_proc_instance(context, proc_instance);
            if (result == (StarsI16)-1) break;
            if (result == 0) {
                if (!(state->gd_high_flags & 0x20U) && ops->mark_file)
                    ops->mark_file(context, 2, -1, 1, 0);
                state->gd_low_flags = (StarsU16)(state->gd_low_flags &
                    0xfff7U);
                if (ops->destroy_game) ops->destroy_game(context);
                if (state->ini_flags & 8U) return;
                width = ops->get_system_metric ?
                    ops->get_system_metric(context, 0U) : 0;
                height = ops->get_system_metric ?
                    ops->get_system_metric(context, 1U) : 0;
                if (ops->create_title)
                    state->title = ops->create_title(context,
                        state->title_class, state->title_text,
                        0x90000000UL, 0, 0, width, height, state->frame);
                state->freeing_title = 0U;
                return;
            }
            for (;;) {
                if ((state->gd_high_flags & 0x0400U) &&
                    ops->show_progress) ops->show_progress(context);
                if (ops->ensure_ais) ops->ensure_ais(context);
                if (ops->generate_turn) ops->generate_turn(context);
                if (!state->pass_count) break;
                --state->pass_count;
                key = ops->get_async_key_state ?
                    ops->get_async_key_state(context, 0x10U) : 0;
                if (key < 0) break;
                key = ops->get_async_key_state ?
                    ops->get_async_key_state(context, 0x11U) : 0;
                if (key < 0) break;
            }
            state->pass_count = 0U;
            if (ops->hide_progress) ops->hide_progress(context);
        }
    } else state->ini_flags = (StarsU16)(state->ini_flags & 0xfffbU);
    if (ops->show_window) ops->show_window(context, state->frame, 2U);
    if (ops->set_timer)
        state->timer_id = ops->set_timer(context, 0U, 0x0dU, 10000UL,
            proc_instance);
    state->timer_type = 0x0dU;
    if (ops->host_timer_proc)
        ops->host_timer_proc(context, 0U, 0U, state->timer_id, 0UL);
}
