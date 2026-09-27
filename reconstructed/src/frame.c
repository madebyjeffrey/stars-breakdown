/* Semantic reconstruction of FrameWndProc. */
#include <stdio.h>
#include <string.h>
#include "core.h"

#define FRAME_ROP 0x00f00021UL

static void frame_blt(const StarsFrameOps *ops, void *context, StarsU16 dc,
    StarsI16 x, StarsI16 y, StarsI16 width, StarsI16 height)
{
    if (ops->pat_blt) ops->pat_blt(context, dc, x, y, width, height,
        FRAME_ROP);
}

static StarsU16 frame_icon(const StarsFrameState *state)
{
    if (state->timer_id != 0U &&
        (state->player != -1 || state->game_lid == 0UL))
        return state->hicon_wait;
    return state->player != -1 ? state->hicon_stars : state->hicon_host;
}

static StarsU16 frame_drag_icon(const StarsFrameState *state)
{
    if (state->player != -1 && state->timer_id != 0U)
        return state->hicon_wait;
    return state->player != -1 ? state->hicon_stars : state->hicon_host;
}

static void frame_paint_border(StarsFrameState *state,
    const StarsFrameOps *ops, void *context, StarsU16 dc)
{
    StarsU16 old;
    StarsI16 top;

    old = ops->select_object ? ops->select_object(context, dc,
        state->hbr_shadow) : 0U;
    if (state->window_layout == 0U ||
        (state->window_layout != 1U && state->window_layout != 2U)) {
        frame_blt(ops, context, dc, (StarsI16)(state->x_top + 5), 0, 2,
            state->dy);
        frame_blt(ops, context, dc, 0, (StarsI16)(state->y1 + 5),
            (StarsI16)(state->x_top + 2), 2);
        frame_blt(ops, context, dc, 0, (StarsI16)(state->y2 + 5),
            (StarsI16)(state->x_top + 2), 2);
        if (ops->select_object) ops->select_object(context, dc,
            state->hbr_hilite);
        frame_blt(ops, context, dc, (StarsI16)(state->x_top + 1), 0, 1,
            (StarsI16)(state->y1 + 2));
        frame_blt(ops, context, dc, 0, (StarsI16)(state->y1 + 1),
            (StarsI16)(state->x_top + 1), 1);
        frame_blt(ops, context, dc, (StarsI16)(state->x_top + 1),
            (StarsI16)(state->y1 + 6), 1,
            (StarsI16)(state->y2 - state->y1 - 4));
        frame_blt(ops, context, dc, 0, (StarsI16)(state->y2 + 1),
            (StarsI16)(state->x_top + 1), 1);
        frame_blt(ops, context, dc, (StarsI16)(state->x_top + 1),
            (StarsI16)(state->y2 + 6), 1,
            (StarsI16)(state->dy - state->y2 - 6));
    } else {
        top = (state->global_high_flags & 0x8000U) ? 0x24 : 0;
        frame_blt(ops, context, dc, (StarsI16)(state->x_top + 5), top, 2,
            (StarsI16)(state->y2 + 2 - top));
        frame_blt(ops, context, dc, 0, (StarsI16)(state->y1 + 5),
            (StarsI16)(state->x_top + 2), 2);
        frame_blt(ops, context, dc, (StarsI16)(state->x_top + 5),
            (StarsI16)(state->y2 + 5), (StarsI16)(state->dx - state->x_top - 5), 2);
        frame_blt(ops, context, dc, (StarsI16)(state->x_top + 5),
            (StarsI16)(state->y2 + 6), 2, (StarsI16)(state->dy - state->y2 - 5));
        if (ops->select_object) ops->select_object(context, dc,
            state->hbr_hilite);
        frame_blt(ops, context, dc, (StarsI16)(state->x_top + 1), top, 1,
            (StarsI16)(state->y1 + 2 - top));
        frame_blt(ops, context, dc, 0, (StarsI16)(state->y1 + 1),
            (StarsI16)(state->x_top + 1), 1);
        frame_blt(ops, context, dc, (StarsI16)(state->x_top + 1),
            (StarsI16)(state->y1 + 6), 1,
            (StarsI16)(state->dy - state->y1 - 6));
        frame_blt(ops, context, dc, (StarsI16)(state->x_top + 6),
            (StarsI16)(state->y2 + 1), (StarsI16)(state->dx - state->x_top - 6), 1);
    }
    if (ops->select_object) ops->select_object(context, dc, old);
}

static void frame_send_player(StarsFrameState *state,
    const StarsFrameOps *ops, void *context, StarsU16 hwnd, StarsU16 timer)
{
    StarsI16 available;
    StarsI16 answer;
    StarsI16 mark_result;

    if (state->timer_id != 0U && ops->kill_timer) {
        ops->kill_timer(context, 0U, state->timer_id);
        state->timer_id = 0U;
        if (ops->create_children) ops->create_children(context);
    }
    available = state->player == -1 || !ops->new_turn_available ? 0 :
        ops->new_turn_available(context, state->player);
    if (state->player == -1 || available == 0) {
        if (timer != 0U) {
            if (state->timer_type == 0x0dU) {
                if (ops->send_message) ops->send_message(context, hwnd, 0x465U, 0U, 0UL);
            } else if (state->timer_type == 0x0eU) {
                answer = ops->alert ? ops->alert(context, 0x18eU, 1) : 2;
                if (answer == 6) {
                    mark_result = ops->mark_file ? ops->mark_file(context, 1,
                        state->player, 2, 0) : 1;
                    if (mark_result == 0) {
                        if (ops->alert) ops->alert(context, 399U, 16);
                        if (ops->destroy_game) ops->destroy_game(context);
                        if (ops->load_game && ops->load_game(context,
                            state->base_name, 0x38eU) != 0 && ops->create_children)
                            ops->create_children(context);
                    }
                } else if (answer == 2) {
                    if (ops->send_message)
                        ops->send_message(context, state->frame, 0x111U,
                            0x6aU, 0UL);
                    return;
                }
            }
        }
    } else {
        answer = ops->alert ? ops->alert(context, timer == 0U ? 0x2c5U :
            0x2c6U, timer == 0U ? 1 : 0x40) : 2;
        if (answer == 6) {
            if (ops->destroy_game) ops->destroy_game(context);
            if (ops->load_game && ops->load_game(context, state->base_name,
                0x38eU) != 0 && ops->create_children) ops->create_children(context);
        } else if (answer == 2) {
            if (ops->send_message) ops->send_message(context, state->frame,
                timer == 0U ? 0x112U : 0x111U, timer == 0U ? 0xf020U : 0x6aU, 0UL);
            return;
        }
    }
    if (ops->send_message) ops->send_message(context, state->frame, 0x111U,
        0xfa1U, 0UL);
    if (state->selected_point.x > 1000 && state->selected_point.y > 1000 &&
        ops->scan_to_point)
        ops->scan_to_point(context, &state->selected_point, 1);
}

static StarsU32 frame_startup(StarsFrameState *state, StarsU16 hwnd,
    const StarsFrameOps *ops, void *context)
{
    StarsU16 old_flags;
    StarsI16 result;
    StarsU32 proc;
    StarsU32 salt;
    StarsU8 work[256];

    state->player = -1;
    if (state->ini_flags & 2U) {
        old_flags = (StarsU16)(state->ini_flags & 0xfffdU);
        state->ini_flags = old_flags;
        if (state->ini_flags & 0x4000U) {
            state->file_error_silent = 1;
            if (ops->clear_file) ops->clear_file(context, 7);
            result = ops->load_game ? ops->load_game(context, state->base_name,
                0x38eU) : 0;
            if (result != 0) {
                if (ops->verify_turns) ops->verify_turns(context);
                if (ops->destroy_game) ops->destroy_game(context);
                if (ops->ensure_ais) ops->ensure_ais(context);
                if (ops->output) ops->output(context, 7,
                    (const StarsU8 *)"Year report");
            }
            if ((state->global_high_flags & 0x40U) == 0U) {
                if (ops->post_quit) ops->post_quit(context, state->exit_value);
            } else if (ops->exit_windows) ops->exit_windows(context, state->exit_value);
            return 0UL;
        }
        if (state->ini_flags & 0x0400U) {
            if (state->serial_number != 0UL && ops->generate_new_game)
                ops->generate_new_game(context, state->base_name);
            if (ops->post_quit) ops->post_quit(context, state->exit_value);
            return 0UL;
        }
        if (state->ini_flags & 8U) {
            while (state->turn_generation_count != 0) {
                if (ops->ensure_ais) ops->ensure_ais(context);
                if (ops->generate_turn) ops->generate_turn(context);
                if (state->batch_active && ops->next_batch_line)
                    ops->next_batch_line(context, work, (StarsU16)sizeof(work));
                --state->turn_generation_count;
                if ((state->ini_flags & 0x10U) && !state->batch_active) break;
            }
            if (state->turn_generation_count == 0 && ops->post_quit)
                ops->post_quit(context, state->exit_value);
            return 0UL;
        }
        if (ops->command_handler) ops->command_handler(context, hwnd, 0xed9U);
        if (state->ini_flags & 0x10U) {
            if (ops->post_quit) ops->post_quit(context, state->exit_value);
            return 0UL;
        }
        if (state->game_lid != 0UL) {
            if (state->player != -1 && (state->ini_flags & 0xe000U)) {
                if (state->ini_flags & 0x2000U && ops->send_message)
                    ops->send_message(context, state->frame, 0x111U, 0x55U, 0UL);
                if (state->ini_flags & 0x1000U && ops->send_message)
                    ops->send_message(context, state->frame, 0x111U, 0x54U, 0UL);
                if (state->ini_flags & 0x0800U && ops->send_message)
                    ops->send_message(context, state->frame, 0x111U, 0x53U, 0UL);
                return 0UL;
            }
            if (ops->show_window) ops->show_window(context, state->frame, 5U);
            if (ops->initialize_menu) ops->initialize_menu(context, 0U);
            if (ops->send_message) ops->send_message(context, state->frame, 0x111U,
                0xfa1U, 0UL);
            if (state->ini_flags & 4U) {
                state->ini_flags = (StarsU16)(state->ini_flags & 0xfffbU);
                if (ops->command_handler) ops->command_handler(context, hwnd, 0x6aU);
            }
        }
    }
    if (state->title == 0U && ops->create_window && ops->system_metric) {
        state->title = ops->create_window(context, 0U, 0U, 0x90000000UL, 0, 0,
            ops->system_metric(context, 0U), ops->system_metric(context, 1U),
            state->frame);
        state->freeing_title = 0U;
    }
    state->ini_flags = (StarsU16)(state->ini_flags & 0xfffeU);
    if (ops->destroy_game) ops->destroy_game(context);
    if (state->serial_number != 0UL && ops->compare_environment &&
        ops->compare_environment(context, state->machine_config,
            state->current_environment, 11U) == 0) return 0UL;
    memset(work, 0, sizeof(work));
    work[200] = state->serial_number != 0UL ? 1U : 0U;
    proc = ops->make_proc ? ops->make_proc(context, state->instance) : 0UL;
    result = ops->dialog_box ? ops->dialog_box(context, 0x56U,
        state->title ? state->title : state->frame, proc) : 0;
    if (ops->free_proc) ops->free_proc(context, proc);
    if (result == 0) {
        state->serial_number = 0UL;
        memcpy(state->machine_config, state->current_environment, 11U);
        if (ops->post_quit) ops->post_quit(context, state->exit_value);
    } else if (ops->valid_serial && ops->valid_serial(context, work, &salt) != 0) {
        state->serial_number = salt;
        memcpy(state->machine_config, state->current_environment, 11U);
    } else if (state->serial_number == 0UL) {
        memcpy(state->machine_config, state->current_environment, 11U);
        if (ops->post_quit) ops->post_quit(context, state->exit_value);
    }
    if (ops->write_ini) ops->write_ini(context);
    return 0UL;
}

StarsU32 stars_frame_wnd_proc(StarsFrameState *state, StarsU16 hwnd,
    StarsU16 message, StarsU16 wparam, StarsU32 lparam,
    const StarsFrameOps *ops, void *context)
{
    StarsU16 dc;
    StarsU16 old_object;
    StarsU16 icon;
    StarsU16 cursor;
    StarsI16 metric;
    StarsI16 x;
    StarsI16 y;
    StarsI16 grsel;
    StarsI16 result;
    StarsI16 available;
    StarsI16 move_result;
    StarsInvertPoint point;
    StarsInvertPoint delta;
    StarsInvertPoint last_delta;
    StarsRect rect;
    StarsU16 old_timer;

    if (!state || !ops) return 0UL;
    if (message == 1U) {
        dc = ops->get_dc ? ops->get_dc(context, hwnd) : 0U;
        if (ops->create_fonts) ops->create_fonts(context, dc);
        metric = ops->get_text_metrics ? ops->get_text_metrics(context, dc) : 0;
        state->dy_sys_font = metric;
        state->dy_sbar = (StarsI16)((state->dy_arial8 + 0x0c) * 2);
        if (ops->release_dc) ops->release_dc(context, hwnd, dc);
        if (ops->init_tiles) ops->init_tiles(context);
        if (ops->ensure_tile_size) ops->ensure_tile_size(context);
        return 0UL;
    }
    if (message == 2U) {
        if (state->timer_id != 0U && ops->kill_timer)
            ops->kill_timer(context, 0U, state->timer_id);
        if (ops->write_ini) ops->write_ini(context);
        state->timer_id = 0U;
        if (state->global_low_flags & 8U && ops->mark_file)
            ops->mark_file(context, 2, -1, 1, 0);
        if (ops->destroy_game) ops->destroy_game(context);
        if ((state->global_high_flags & 0x40U) == 0U) {
            if (ops->post_quit) ops->post_quit(context, state->exit_value);
        } else if (ops->exit_windows) ops->exit_windows(context, state->exit_value);
        return 0UL;
    }
    if (message == 5U) {
        if (wparam == 0U || wparam == 2U) {
            state->dx = (StarsI16)(lparam & 0xffffU);
            state->dy = (StarsI16)(lparam >> 16);
            if (ops->refit) ops->refit(context);
            return 0UL;
        }
        return ops->default_proc ? ops->default_proc(context, hwnd, message,
            wparam, lparam) : 0UL;
    }
    if (message == 6U) return 0UL;
    if (message == 0x0fU) {
        if (ops->is_iconic && ops->is_iconic(context, hwnd) == 0) {
            dc = ops->begin_paint ? ops->begin_paint(context, hwnd, &rect) : 0U;
            frame_paint_border(state, ops, context, dc);
            if (ops->end_paint) ops->end_paint(context, hwnd, &rect);
        } else {
            dc = ops->begin_paint ? ops->begin_paint(context, hwnd, &rect) : 0U;
            icon = frame_icon(state);
            if (ops->draw_icon) ops->draw_icon(context, dc, 2, 2, icon);
            if (ops->end_paint) ops->end_paint(context, hwnd, &rect);
        }
        return 0UL;
    }
    if (message == 0x10U) {
        if (ops->destroy_window) ops->destroy_window(context, hwnd);
        return 0UL;
    }
    if (message == 0x14U) {
        if (ops->get_client_rect) ops->get_client_rect(context, hwnd, &rect);
        if (ops->is_iconic && ops->is_iconic(context, hwnd) != 0) {
            if (ops->fill_rect) ops->fill_rect(context, wparam, &rect,
                state->hbr_desktop);
            return 0UL;
        }
        if (state->scanner != 0U && ops->get_client_rect &&
            ops->exclude_clip_rect) {
            StarsRect scan_rect;
            ops->get_client_rect(context, state->scanner, &scan_rect);
            if (ops->map_window_points) ops->map_window_points(context,
                state->scanner, hwnd, &scan_rect);
            ops->exclude_clip_rect(context, wparam, &scan_rect);
        }
        if (ops->fill_rect) ops->fill_rect(context, wparam, &rect, state->hbr_face);
        return 1UL;
    }
    if (message == 0x15U || message == 0x1aU) {
        if (ops->get_system_colors) ops->get_system_colors(context);
        return 0UL;
    }
    if (message == 0x20U) {
        if (ops->is_iconic && ops->is_iconic(context, hwnd) == 0 &&
            ops->get_cursor_pos && ops->screen_to_client && ops->get_client_rect &&
            ops->point_in_rect && ops->cursor_from_point) {
            ops->get_cursor_pos(context, &x, &y);
            ops->screen_to_client(context, state->frame, &x, &y);
            ops->get_client_rect(context, hwnd, &rect);
            if (ops->point_in_rect(context, &rect, x, y)) {
                point.x = x; point.y = y;
                cursor = ops->cursor_from_point(context, &point, &grsel);
                if (cursor != 0U) {
                    if (ops->set_cursor) ops->set_cursor(context, cursor);
                    return 1UL;
                }
            }
        }
        return ops->default_proc ? ops->default_proc(context, hwnd, message,
            wparam, lparam) : 0UL;
    }
    if (message == 0x24U) {
        if (ops->set_nc_size) ops->set_nc_size(context, lparam, 0x208, 0x17c);
        return 0UL;
    }
    if (message == 0x37U) return (StarsU32)frame_drag_icon(state);
    if (message == 0x102U) {
        if (state->scanner != 0U && (wparam == 0x2dU || wparam == 0x2bU)) {
            if (ops->send_message) ops->send_message(context, state->scanner,
                0x102U, wparam, lparam);
            return 0UL;
        }
        if (state->message_window != 0U && (wparam == 0x2dU ||
            wparam == 0x2bU || wparam == 0x0dU)) {
            if (ops->send_message) ops->send_message(context, state->message_window,
                0x102U, wparam, lparam);
            return 0UL;
        }
        if (state->planet != 0U && (wparam == 0x66U || wparam == 0x46U)) {
            if (ops->send_message) ops->send_message(context, state->planet,
                0x102U, wparam, lparam);
            return 0UL;
        }
        if (state->planet != 0U && state->selected_object == 1 &&
            (wparam == 0x71U || wparam == 0x51U)) {
            if (ops->change_production) ops->change_production(context, 0);
            return 0UL;
        }
        if ((state->selected_object & 3) == 0) return 0UL;
        result = 0; available = 0;
        if (wparam == 0x6eU) result = 1;
        else if (wparam == 0x4eU || wparam == 0x50U) {
            if (state->selected_object == 1 && ops->find_starbase)
                available = (StarsI16)ops->find_starbase(context,
                    state->selected_planet, (StarsU16)(wparam == 0x4eU));
            else result = wparam == 0x4eU ? 1 : -1;
        } else if (wparam == 0x70U) result = -1;
        else if ((wparam == 0x72U || wparam == 0x52U) &&
            state->selected_object == 2 && ops->ship_command)
            ops->ship_command(context, state->planet, 0U, 0U);
        if (result == 0 && available == 0) return 0UL;
        if (state->selected_object != 2) {
            if (ops->select_planet) ops->select_planet(context, result, (StarsU16)available);
        } else if (ops->select_fleet)
            ops->select_fleet(context, result, (StarsU16)available);
        return 0UL;
    }
    if (message == 0x111U) {
        if (ops->command_handler) ops->command_handler(context, hwnd, wparam);
        return 0UL;
    }
    if (message == 0x112U && ((wparam & 0xfff0U) == 0xf030U ||
        (wparam & 0xfff0U) == 0xf120U)) {
        old_timer = state->timer_id;
        frame_send_player(state, ops, context, hwnd, old_timer);
        return 0UL;
    }
    if (message == 0x116U) {
        if (ops->initialize_menu) ops->initialize_menu(context, wparam);
        return 0UL;
    }
    if (message == 0x121U) {
        if ((state->global_low_flags & 0x0800U) &&
            (state->tutorial_flags & 4U) && ops->advance_tutor)
            ops->advance_tutor(context);
        return 0UL;
    }
    if (message == 0x201U) {
        point.x = (StarsI16)(lparam & 0xffffU);
        point.y = (StarsI16)(lparam >> 16);
        if (!ops->cursor_from_point) return 0UL;
        cursor = ops->cursor_from_point(context, &point, &grsel);
        if (cursor == 0U) return 0UL;
        dc = ops->get_dc ? ops->get_dc(context, hwnd) : 0U;
        old_object = ops->select_object ? ops->select_object(context, dc,
            state->hbr_50) : 0U;
        if (ops->invert_border) {
            last_delta.x = 0; last_delta.y = 0;
            ops->invert_border(context, wparam, grsel, last_delta, 0);
        }
        if (ops->set_capture) ops->set_capture(context, hwnd);
        while (ops->mouse_move && ops->mouse_move(context, &point) != 0) {
            delta.x = (StarsI16)(point.x - state->selected_point.x);
            delta.y = (StarsI16)(point.y - state->selected_point.y);
            move_result = (StarsI16)(delta.x != last_delta.x ||
                delta.y != last_delta.y);
            if (move_result && ops->invert_border)
                last_delta = ops->invert_border(context, wparam, grsel, delta,
                    &last_delta);
            if (!move_result) break;
        }
        if (ops->invert_border) point = ops->invert_border(context, wparam,
            grsel, last_delta, 0);
        if (ops->release_capture) ops->release_capture(context);
        if (ops->select_object) ops->select_object(context, dc, old_object);
        if (ops->release_dc) ops->release_dc(context, hwnd, dc);
        if (point.x == 0 && point.y == 0) return 0UL;
        if (grsel & 1) {
            if (state->window_layout == 0) state->dx_plan_want =
                (StarsI16)(state->x_top + point.x);
            else state->dx2_plan_want = (StarsI16)(state->x_top + point.x);
        }
        if (grsel & 2) {
            if (state->window_layout == 0) state->dy_msg_want =
                (StarsI16)(state->y2 - state->y1 - 8 - point.y);
            else state->dy2_msg_want = (StarsI16)(state->dy - state->y1 - 8 - point.y);
        }
        if (grsel & 4) {
            if (state->window_layout == 0) {
                state->dy_msg_want = (StarsI16)(state->y2 - state->y1 - 8 + point.y);
                state->dy_min_want = (StarsI16)(state->dy - state->y2 - 8 - point.y);
            } else state->dy2_min_want = (StarsI16)(state->dy - state->y2 - 8 - point.y);
        }
        if (ops->refit) ops->refit(context);
        return 0UL;
    }
    if (message == 0x30fU || message == 0x311U) {
        if (message == 0x311U && wparam == hwnd) return 0UL;
        if (state->title != 0U) {
            if (ops->send_message) return 0UL +
                (ops->default_proc ? ops->default_proc(context, state->title,
                    message, wparam, lparam) : 0UL);
        } else if (ops->select_palette && ops->realize_palette &&
            ops->begin_paint && ops->end_paint) {
            dc = ops->begin_paint(context, hwnd, &rect);
            old_object = ops->select_palette(context, dc, state->palette, 0U);
            result = ops->realize_palette(context, dc);
            ops->select_palette(context, dc, old_object, 0U);
            ops->end_paint(context, hwnd, &rect);
            if (result != 0 && ops->default_proc) return 1UL;
        }
        return 0UL;
    }
    if (message == 0x465U) {
        if (ops->create_children) ops->create_children(context);
        return 1UL;
    }
    if (message == 0x466U) {
        if (ops->show_tutor) ops->show_tutor(context, 0);
        if (ops->destroy_game) ops->destroy_game(context);
        state->file_error_silent = 1;
        result = ops->load_game ? ops->load_game(context, state->base_name, 0x3c8U) : 0;
        if (result != 0) {
            state->file_error_silent = 0; state->player = 0;
            if (ops->create_children) ops->create_children(context);
            if (ops->send_message) ops->send_message(context, state->frame, 0x111U,
                0xfa1U, 0UL);
            if (ops->advance_tutor) ops->advance_tutor(context);
        } else state->file_error_silent = 0;
        return 0UL;
    }
    if (message == 0x464U) return frame_startup(state, hwnd, ops, context);
    return ops->default_proc ? ops->default_proc(context, hwnd, message,
        wparam, lparam) : 0UL;
}
