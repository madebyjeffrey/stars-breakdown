/* Semantic reconstruction of HostTimerProc. */
#include <stdio.h>
#include <string.h>
#include "core.h"

static const StarsU8 *timer_text(const StarsHostTimerOps *ops, void *context,
    StarsU16 id)
{ return ops->text ? ops->text(context, id) : (const StarsU8 *)""; }

/* NE 5:7716. The timer state and Windows services are explicit so the
 * reentrant guard, timer transitions, and generation loop remain visible C. */
void stars_host_timer_proc(StarsHostTimerState *state, StarsU16 hwnd,
    const StarsHostTimerOps *ops, void *context)
{
    StarsU16 saved_silent;
    StarsI16 available;
    StarsI16 outstanding;
    StarsU16 parent;
    StarsU16 child;
    StarsU8 work[128];
    StarsU8 player_file[32];

    if (!state || !ops) return;
    saved_silent = state->file_error_silent;
    if (state->processing_timer == 0U) {
        state->processing_timer = 1U;
        if (state->timer_type == 0x0eU) {
            available = ops->new_turn_available ?
                ops->new_turn_available(context, state->player) : 0;
            if (available != 0) {
                if (ops->kill_timer)
                    ops->kill_timer(context, hwnd, state->timer_id);
                sprintf((char *)player_file,
                    (const char *)(state->player_format ?
                        state->player_format : (const StarsU8 *)"%d"),
                    (int)state->player + 1);
                if (ops->destroy_game) ops->destroy_game(context);
                available = ops->load_game ?
                    ops->load_game(context, player_file) : 0;
                if (available != 0) {
                    if (ops->create_children) ops->create_children(context);
                    state->timer_id = ops->set_timer ?
                        ops->set_timer(context, 0U, 0x0fU, 1000UL, 0UL) : 0U;
                    state->timer_type = 0x0fU;
                    if (ops->flash_window)
                        ops->flash_window(context, state->frame, 1U);
                    if (ops->set_window_text)
                        ops->set_window_text(context, state->frame,
                            state->loaded_title ? state->loaded_title :
                            timer_text(ops, context, 0x31aU));
                    if (ops->message_beep) ops->message_beep(context, 0x30U);
                    outstanding = 1;
                    goto update_frame;
                }
                if (ops->alert) ops->alert(context, 0x1bU, 0x10);
            }
        } else if (state->timer_type == 0x0fU) {
            if (ops->flash_window)
                ops->flash_window(context, state->frame, 1U);
        } else {
            for (;;) {
                outstanding = ops->turns_outstanding ?
                    ops->turns_outstanding(context) : 0;
                if (state->global_flags & 0x10U) break;
                sprintf((char *)work,
                    (const char *)(state->waiting_format ?
                        state->waiting_format : (const StarsU8 *)"%d"),
                    (int)outstanding);
                if (outstanding != 1 && state->waiting_suffix)
                    strcat((char *)work, (const char *)state->waiting_suffix);
                if (ops->set_window_text)
                    ops->set_window_text(context, state->frame, work);
update_frame:
                parent = ops->get_window ?
                    ops->get_window(context, state->frame, 3U) : 0U;
                child = ops->get_window ? ops->get_window(context, parent, 4U) :
                    0U;
                if (child == state->frame && ops->invalidate)
                    ops->invalidate(context, parent);
                if (outstanding != 0) break;
                if ((state->global_flags & 0x0400U) && ops->show_progress)
                    ops->show_progress(context);
                if (ops->ensure_ais) ops->ensure_ais(context);
                if (ops->generate_turn) ops->generate_turn(context);
                if (ops->hide_progress) ops->hide_progress(context);
                if (state->ini_flags & 8U) {
                    if (ops->post_quit)
                        ops->post_quit(context, state->exit_value);
                    break;
                }
                if (ops->ensure_ais) ops->ensure_ais(context);
            }
            if (outstanding == 0 && ops->alert)
                ops->alert(context, 0x2c8U, 0x10);
            if (outstanding == 0 && ops->get_dialog_item && ops->enable_window)
                ops->enable_window(context,
                    ops->get_dialog_item(context, hwnd, state->dialog_id), 0U);
        }
        state->processing_timer = 0U;
    }
    state->file_error_silent = saved_silent;
}
