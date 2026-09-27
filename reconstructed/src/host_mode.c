/* Semantic reconstruction of HostModeDialog. */
#include <stdio.h>
#include <string.h>
#include "core.h"

static StarsU16 can_generate(const StarsHostModeState *state)
{
    return (StarsU16)((state->global_high_flags & 0x10U) == 0U &&
        (state->auto_generate != 0U || state->force_mode != 0U));
}

static void update_generate_button(StarsHostModeState *state,
    const StarsHostModeOps *ops, void *context, StarsU16 hwnd)
{
    StarsU16 item;

    if ((state->global_high_flags & 0x20U) != 0U || !ops->get_dialog_item ||
        !ops->enable_window) return;
    item = ops->get_dialog_item(context, hwnd, 0x408U);
    ops->enable_window(context, item, can_generate(state));
}

/* NE 5:6C16. The callbacks retain the original USER/GDI boundaries; the
 * message dispatch, player mode encoding, salt inversion, and timer state
 * are ordinary C rather than byte-replay glue. */
StarsI16 stars_host_mode_dialog(StarsHostModeState *state, StarsU16 hwnd,
    StarsU16 message, StarsU16 wparam, StarsU32 lparam,
    const StarsHostModeOps *ops, void *context)
{
    StarsRect rect;
    StarsU16 hdc;
    StarsU16 item;
    StarsU16 menu;
    StarsU16 command;
    StarsU16 i;
    StarsU16 current;
    StarsU16 selected;
    StarsI16 x;
    StarsI16 y;
    StarsI16 index;
    StarsI16 remainder;
    StarsI16 result;
    StarsI16 key_alt;
    StarsI16 key_shift;
    StarsU8 work[128];

    if (!state || !ops) return 0;
    if (message == 2U) {
        if (ops->kill_timer) ops->kill_timer(context, hwnd, state->timer_id);
        state->timer_id = 0U;
        return 0;
    }
    if (message == 0x0fU) {
        hdc = ops->begin_paint ? ops->begin_paint(context, hwnd, &rect) : 0U;
        if (state->tick_last == 0UL && ops->turns_outstanding)
            ops->turns_outstanding(context);
        if ((state->global_high_flags & 0x20U) == 0U) {
            update_generate_button(state, ops, context, hwnd);
        }
        if (ops->draw_dialog) ops->draw_dialog(context, hwnd, hdc);
        if (ops->end_paint) ops->end_paint(context, hwnd, &rect);
        return 1;
    }
    if (message == 0x14U) {
        if (ops->get_client_rect) ops->get_client_rect(context, hwnd, &rect);
        if (ops->fill_rect) ops->fill_rect(context, wparam, &rect,
            state->button_brush);
        return 1;
    }
    if (message == 0x19U) {
        if (ops->set_bk_color)
            ops->set_bk_color(context, wparam, state->button_face_color);
        return (StarsI16)state->button_brush;
    }
    if (message == 0x20U || message == 0x201U || message == 0x204U) {
        if (!ops->get_cursor_pos || !ops->screen_to_client) return 0;
        ops->get_cursor_pos(context, &x, &y);
        ops->screen_to_client(context, hwnd, &x, &y);
        if (!(x > 5 && x < state->dy_arial8 + 7 && y > 0x2f)) return 0;
        index = (StarsI16)((y - 0x30) / (state->dy_arial8 + 4));
        remainder = (StarsI16)((y - 0x30) % (state->dy_arial8 + 4));
        if (index < 0 || index >= (StarsI16)state->player_count ||
            remainder >= state->dy_arial8 + 1) return 0;
        if (message == 0x20U) {
            if (ops->set_cursor) ops->set_cursor(context, state->hand_cursor);
            return 1;
        }
        if ((state->players[index].detail & 0x0200U) == 0U) current = 0U;
        else if ((state->players[index].detail >> 13) == 7U) current = 2U;
        else current = 1U;
        menu = ops->create_popup_menu ? ops->create_popup_menu(context) : 0U;
        for (i = 0U; i < 3U; ++i) {
            StarsU16 flags;
            const StarsU8 *label;
            label = ops->string ? ops->string(context, (StarsU16)(0x20fU + i)) :
                (const StarsU8 *)"";
            if (i == 1U)
                flags = ((state->players[index].detail & 0x0200U) == 0U ||
                    (state->players[index].detail >> 13) == 7U) ? 3U : 0U;
            else
                flags = ((state->players[index].detail & 0x0200U) == 0U ||
                    (state->players[index].detail >> 13) == 7U) ? 0U : 3U;
            if (i == current) flags = (StarsU16)(flags | 8U);
            if (ops->append_menu)
                ops->append_menu(context, menu, flags,
                    (const StarsU8 *)label);
        }
        if (ops->client_to_screen) ops->client_to_screen(context, hwnd, &x, &y);
        if (ops->track_popup_menu)
            ops->track_popup_menu(context, menu,
                (StarsU16)(4U | (message == 0x204U ? 2U : 0U)), x, y, hwnd);
        if (ops->destroy_menu) ops->destroy_menu(context, menu);
        command = 0U;
        if (ops->peek_message)
            ops->peek_message(context, hwnd, 0x111U, 0x111U, 2U, &command);
        selected = command >= 15000U && command < 0x3afcU ?
            (StarsU16)(command - 15000U) : 0xffffU;
        if (selected != 0xffffU && selected != current) {
            state->players[index].detail = (StarsU16)(
                (state->players[index].detail & 0xfdffU) |
                (selected != 0U ? 0x0200U : 0U));
            if (selected == 2U)
                state->players[index].detail = (StarsU16)(
                    (state->players[index].detail & 0x1fffU) | 0xe000U);
            state->players[index].salt = ~state->players[index].salt;
            if (ops->mark_file) {
                ops->mark_file(context, 3, index, 8, selected != 0U);
                ops->mark_file(context, 2, index, 8, selected != 0U);
            }
            state->global_low_flags = (StarsU16)(state->global_low_flags &
                0xfbffU);
            state->processing_timer = 1U;
            if (ops->turns_outstanding) ops->turns_outstanding(context);
            update_generate_button(state, ops, context, hwnd);
            if (ops->draw_dialog) ops->draw_dialog(context, hwnd, 0U);
            state->processing_timer = 0U;
        }
        return 0;
    }
    if (message == 0x110U) {
        if (ops->sticky_position) ops->sticky_position(context, hwnd, 1U);
        if (ops->get_dialog_item && ops->set_window_text) {
            item = ops->get_dialog_item(context, hwnd, 0x409U);
            ops->set_window_text(context, item, state->game_name);
            item = ops->get_dialog_item(context, hwnd, 0x40aU);
            ops->set_window_text(context, item, state->base_name);
            update_generate_button(state, ops, context, hwnd);
            item = ops->get_dialog_item(context, hwnd, 0x407U);
            ops->enable_window(context, item,
                (StarsU16)((state->global_high_flags & 0x20U) == 0U));
            item = ops->get_dialog_item(context, hwnd, 0x7dfU);
            ops->enable_window(context, item,
                (StarsU16)((state->global_high_flags & 0x20U) == 0U));
        }
        if (ops->set_timer)
            state->timer_id = ops->set_timer(context, hwnd, 0x0dU, 10000UL);
    } else if (message == 0x111U) {
        if (wparam == 0x407U || wparam == 2U || wparam == 0x408U) {
            if (wparam == 0x407U && ops->get_async_key_state) {
                key_alt = ops->get_async_key_state(context, 0x10U);
                key_shift = ops->get_async_key_state(context, 0x11U);
                state->pass_count = key_alt < 0 ?
                    (key_shift < 0 ? 999 : 9) :
                    (key_shift < 0 ? 99 : 0);
                if (state->pass_count == 0 && ops->turns_outstanding &&
                    ops->turns_outstanding(context) != 0) {
                    result = ops->alert ? ops->alert(context, 0x26aU, 0x1024) : 6;
                    if (result != 6) return 1;
                } else if (state->pass_count != 0) {
                    sprintf((char *)work, (const char *)(ops->compressed ?
                        ops->compressed(context, 0x562U) : (const StarsU8 *)"%d"),
                        state->pass_count + 1);
                    result = ops->message_box ? ops->message_box(context,
                        ops->get_focus ? ops->get_focus(context) : 0U, work,
                        (const StarsU8 *)"", 0x2034U) : 6;
                    if (result != 6) { state->pass_count = 0; return 1; }
                }
            }
            if (ops->sticky_position) ops->sticky_position(context, hwnd, 0U);
            result = wparam == 2U ? 0 : (wparam == 0x408U ? -1 : 1);
            if (ops->end_dialog) ops->end_dialog(context, hwnd, result);
            if (wparam == 0x408U) {
                if (ops->ensure_ais) ops->ensure_ais(context);
            } else if (wparam == 2U && (state->global_low_flags & 4U) != 0U) {
                if (ops->post_quit) ops->post_quit(context, state->exit_value);
            }
            return 1;
        }
        if (wparam == 0x7dfU) {
            if (ops->check_password && ops->check_password(context) == 0) return 0;
            {
                StarsU32 proc;
                proc = ops->make_proc ? ops->make_proc(context, state->instance) :
                    0UL;
                result = ops->dialog_box ? ops->dialog_box(context, 0x8dU,
                    hwnd, proc) : 0;
                if (ops->free_proc) ops->free_proc(context, proc);
            }
            if (ops->set_focus) ops->set_focus(context, hwnd);
            return result;
        }
        if (wparam == 0x76U) {
            if (ops->win_help) ops->win_help(context, hwnd, state->help_file,
                1U, 0x440U);
            return 1;
        }
        if (wparam == 0x405U) {
            result = ops->options_dialog ? ops->options_dialog(context, hwnd) : 0;
            if (result == 0) return 0;
            update_generate_button(state, ops, context, hwnd);
            return result;
        }
        return 0;
    } else if (message != 0x113U) return 0;
    if (state->processing_timer == 0U) {
        state->processing_timer = 1U;
        if (ops->turns_outstanding) ops->turns_outstanding(context);
        if (ops->draw_dialog) ops->draw_dialog(context, hwnd, 0U);
        state->processing_timer = 0U;
    }
    (void)lparam;
    return (StarsI16)(message != 0x113U);
}
