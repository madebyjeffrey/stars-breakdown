/* Shared drawing and input helpers used by the race dialogs. Coordinates are
 * Win16 signed words. Valid Windows rectangles/text spans are the input domain. */
#include "ui_internal.h"
#include <math.h>
#include <string.h>

/* NE 5:6996-6A25. The callback is CFindTurnsOutstanding; the two global
 * fields are kept in the supplied DGROUP image and restored on return. */
StarsI16 stars_turns_out_safe(StarsU8 *data,
    StarsFindTurnsOutstanding find_turns, void *context)
{
    StarsU16 flags = stars_word(data, 0x7caU);
    StarsU16 saved_player = stars_word(data, 0x18cU);
    StarsU16 saved_bit_three = (StarsU16)((flags >> 3) & 1U);
    StarsU16 saved_bit_one = (StarsU16)((flags >> 1) & 1U);
    StarsI16 result;
    stars_put_word(data, 0x18cU, 0xffffU);
    stars_put_word(data, 0x7caU, (StarsU16)((flags & 0xfff5U) | 8U));
    result = find_turns ? find_turns(context) : 0;
    flags = stars_word(data, 0x7caU);
    stars_put_word(data, 0x7caU, (StarsU16)((flags & 0xfff5U) |
        (saved_bit_one << 1) | (saved_bit_three << 3)));
    stars_put_word(data, 0x18cU, saved_player);
    return result;
}

/* NE 5:7706-7715. The original drawing hook has no body. */
void stars_draw_host_options(StarsU16 hwnd, StarsU16 dc, StarsI16 draw)
{
    (void)hwnd; (void)dc; (void)draw;
}

/* NE 9:63DA-647D, UpdateProgressGauge. The callback represents the original
 * DrawProgressGauge(0,0,iNumOnly) call at the host boundary. */
StarsI16 stars_update_progress(StarsProgressState *state, StarsI16 value,
    StarsU16 flags, StarsProgressDraw draw, void *context)
{
    StarsI16 number = 0;
    if (!state->window) return 0;
    if (value == (StarsI16)-0x39e)
        value = (StarsI16)(state->percent + 4);
    else if (value == (StarsI16)-0x39f)
        value = (StarsI16)(state->percent + 1);
    else if (value < 0)
        value = 0;
    else if (value > 1000) {
        if ((flags & 0x0400U) == 0U) return 0;
        number = value;
        value = state->percent;
    }
    state->percent = value;
    if (draw) draw(context, value, number);
    return 1;
}

/* NE 9:636C-63B1, ShowProgressGauge; NE 9:63B2-63DA,
 * HideProgressGauge. Dialog creation/destruction remain explicit host
 * services, while the original state transitions are retained. */
StarsI16 stars_show_progress(StarsProgressState *state,
    StarsProgressCreate create, StarsProgressDraw draw, void *context)
{
    if (!state->window) {
        if (!create) return 0;
        state->window = create(context);
        return state->window != 0;
    }
    return stars_update_progress(state, 0, 0, draw, context);
}
void stars_hide_progress(StarsProgressState *state,
    StarsProgressDestroy destroy, void *context)
{
    if (state->window) {
        if (destroy) destroy(context, state->window);
        state->window = 0;
    }
}

/* NE 9:31A2-3369, LDrawGauge. Win16 drawing is represented by frame/fill
 * callbacks; all rectangle geometry and 32-bit scaling remain here. */
StarsI32 stars_draw_gauge(void *context, StarsU16 dc, const StarsRect *input,
    StarsU16 segment_count, const StarsI32 *sizes, const StarsU16 *brushes,
    StarsI32 total, StarsGaugeFrame frame, StarsGaugeFill fill,
    StarsU16 frame_brush, StarsU16 face_brush)
{
    StarsRect rect;
    StarsI32 cumulative = 0, width, scaled_total = total, amount;
    StarsI16 scaled = 0;
    StarsU16 i;
    if (!input) return 0;
    rect = *input;
    if (frame) frame(context, dc, &rect, frame_brush);
    stars_expand_rect(&rect, -1, -1);
    if (total > 0) {
        if (total > 9999999L) {
            scaled = 1;
            scaled_total = total / 1000L;
        }
        width = (StarsI32)(StarsI16)(rect.right - rect.left);
        for (i = 0; i < segment_count; ++i) {
            amount = sizes ? sizes[i] : 0;
            if (scaled) amount /= 1000L;
            cumulative += amount;
            if (scaled_total != 0)
                rect.right = (StarsU16)((StarsI16)input->left + 1 +
                    (StarsI16)((width * cumulative) / scaled_total));
            if ((StarsI16)rect.left < (StarsI16)rect.right && fill)
                fill(context, dc, &rect, brushes ? brushes[i] : 0);
            rect.left = rect.right;
        }
        rect.right = (StarsU16)((StarsI16)input->right - 1);
        if ((StarsI16)rect.left < (StarsI16)rect.right && fill)
            fill(context, dc, &rect, face_brush);
    }
    return scaled ? cumulative * 1000L : cumulative;
}

/* NE 9:647E-65F5, ProgressGaugeDlg. The four USER/GDI operations are
 * explicit callbacks; message dispatch and progress-state initialization are
 * unchanged from the original dialog procedure. */
StarsI16 stars_progress_dialog(StarsProgressState *state, StarsU16 hwnd,
    StarsU16 message, StarsU16 wparam, StarsI16 turn,
    const StarsProgressDialogOps *ops, void *context)
{
    if (!state) return 0;
    if (message == 0x000fU) {
        if (ops && ops->paint) ops->paint(context, hwnd, 1);
        return 1;
    }
    if (message == 0x0014U) {
        if (ops && ops->erase) ops->erase(context, wparam, 0);
        return 1;
    }
    if (message == 0x0019U)
        return ops && ops->color ? (StarsI16)ops->color(context, wparam, 0) : 0;
    if (message == 0x0110U) {
        state->window = hwnd;
        state->percent = 0;
        if (ops && ops->init) ops->init(context, hwnd, (StarsI16)(turn + 0x961));
        return 1;
    }
    return 0;
}

/* NE 9:65F6-685C, DrawProgressGauge. The host supplies the client rectangle;
 * paint callbacks receive the same border and blue/face fill rectangles that
 * the original PATBLT calls cover. Text rendering remains an outer concern. */
StarsI16 stars_draw_progress_gauge(const StarsRect *client, StarsI16 percent,
    StarsI16 full, StarsProgressGaugePaint paint, void *context)
{
    StarsRect bar, inner, fill;
    StarsI16 width, height, filled;
    if (!client) return 0;
    bar = *client;
    bar.top = (StarsU16)((StarsI16)bar.bottom / 2);
    stars_expand_rect(&bar, -8, -8);
    width = (StarsI16)(bar.right - bar.left);
    height = (StarsI16)(bar.bottom - bar.top);
    if (full && paint) {
        fill = stars_rect((StarsU16)((StarsI16)bar.left - 1),
            (StarsU16)((StarsI16)bar.top - 1),
            (StarsU16)((StarsI16)bar.right + 1), bar.top);
        paint(context, &fill, 0x21U);
        fill = stars_rect((StarsU16)((StarsI16)bar.left - 1),
            (StarsU16)((StarsI16)bar.top - 1), bar.left,
            (StarsU16)((StarsI16)bar.bottom + 1));
        paint(context, &fill, 0x21U);
    }
    stars_expand_rect(&bar, -2, -2);
    width = (StarsI16)(bar.right - bar.left);
    height = (StarsI16)(bar.bottom - bar.top);
    filled = (StarsI16)(((StarsI32)width * percent) / 1000L);
    if (filled > 0 && paint) {
        fill = bar;
        fill.right = (StarsU16)((StarsI16)fill.left + filled);
        paint(context, &fill, 0x21U);
    }
    if (bar.left < bar.right && paint) {
        inner = bar;
        inner.left = (StarsU16)((StarsI16)inner.left + filled);
        if ((StarsI16)inner.left < (StarsI16)inner.right)
            paint(context, &inner, 0x00U);
    }
    (void)height;
    return 1;
}

/* NE 9:2AFA-2F0C, DiaganolTextOut.  Font creation, text measurement, and
 * drawing are explicit host callbacks; this routine retains the original
 * shrink-to-fit loop, diagonal angle, centered placement, and eight-pixel
 * safety margins. */
StarsI16 stars_diagonal_text_out(const StarsRect *rect, const StarsU8 *text,
    StarsI16 length, StarsDiagonalMeasure measure, StarsDiagonalDraw draw,
    void *context)
{
    StarsI16 width, height, font, nominal, measured_width, measured_height;
    StarsI16 rotated_width, rotated_height, x, y, correction;
    StarsI16 limit = 0;
    double angle, cosine, sine;
    if (!rect || !text || !measure || !draw) return 0;
    if (!length) length = (StarsI16)strlen((const char *)text);
    width = (StarsI16)(rect->right - rect->left);
    height = (StarsI16)(rect->bottom - rect->top);
    if (width <= 9 || height <= 9 || length <= 0) return 0;
    font = (StarsI16)(-(width < height ? width : height));
    while (font < -4 && limit++ != 256) {
        nominal = (StarsI16)((-(StarsI32)font * 111L) / 100L);
        if (height < nominal) {
            correction = (StarsI16)((nominal - height) / 2);
            if (correction < 2) correction = 2;
            font = (StarsI16)(font + correction);
            continue;
        }
        angle = atan2((double)(height - nominal), (double)width);
        cosine = cos(angle);
        sine = sin(angle);
        measure(context, font, text, length, &measured_width,
            &measured_height);
        rotated_width = (StarsI16)(fabs((double)measured_width * cosine) +
            fabs((double)measured_height * sine) + 0.5);
        rotated_height = (StarsI16)(fabs((double)measured_width * sine) +
            fabs((double)measured_height * cosine) + 0.5);
        if (rotated_width + 8 <= width && rotated_height + 8 <= height) {
            x = (StarsI16)(rect->left + (width - rotated_width) / 2);
            y = (StarsI16)(rect->top + (height - rotated_height) / 2);
            draw(context, x, y, measured_width, measured_height, font,
                (StarsI16)(angle * 1000.0), text, length);
            return 1;
        }
        correction = 2;
        if (rotated_width + 8 > width) {
            correction = (StarsI16)(((-font) *
                (rotated_width + 8 - width)) / (rotated_width + 8));
        }
        if (rotated_height + 8 > height) {
            StarsI16 other = (StarsI16)(((-font) *
                (rotated_height + 8 - height)) / (rotated_height + 8));
            if (other > correction) correction = other;
        }
        if (correction < 2) correction = 2;
        font = (StarsI16)(font + correction);
    }
    return 0;
}

/* NE 5:24B0-2613, HcrsFromFrameWindowPt. */
StarsU16 stars_hcrs_from_frame_window_pt(StarsI16 x, StarsI16 y,
    const StarsFrameResizeInfo *vfs, StarsI16 layout, StarsU16 cursor_ns,
    StarsU16 cursor_we, StarsU16 cursor_four_way, StarsI16 *selection)
{
    StarsI16 vertical = 0, top = 0, bottom = 0;
    StarsU16 cursor = 0;
    if (!vfs) return 0;
    if (vfs->x_top <= x && x < vfs->x_top + 8) vertical = 1;
    if (x < vfs->x_top + 8 && vfs->y1 <= y && y < vfs->y1 + 8)
        top = 1;
    if (layout == 1 || layout == 2) {
        if (vfs->x_top <= x && vfs->y2 <= y && y < vfs->y2 + 8)
            bottom = 1;
    } else if (x < vfs->x_top + 8 && vfs->y2 <= y && y < vfs->y2 + 8) {
        bottom = 1;
    }
    if (!vertical) {
        if (top || bottom) cursor = cursor_ns;
    } else if (!top && !bottom) {
        cursor = cursor_we;
    } else {
        cursor = cursor_four_way;
    }
    if (selection) *selection = (StarsI16)(top * 2 + vertical + bottom * 4);
    return cursor;
}

/* NE 5:589A-58F3, GetASubMenu. */
StarsU16 stars_get_a_sub_menu(StarsU16 hwnd, StarsI16 menu_index,
    StarsU16 active_window, StarsI16 active_zoomed, StarsGetMenu get_menu,
    StarsGetSubMenu get_sub_menu, void *context)
{
    StarsU16 menu;
    StarsI16 offset = (active_window && active_zoomed) ? 1 : 0;
    if (!get_menu || !get_sub_menu) return 0;
    menu = get_menu(context, hwnd);
    return get_sub_menu(context, menu, (StarsI16)(menu_index + offset));
}

/* NE 5:2E1C-2F78. The host resolves far pointers through memory(), while
 * the original player/planet/fleet selection order and sentinel behavior are
 * retained here. */
StarsI16 stars_find_something_and_select_it(StarsU8 *data,
    const StarsFindSomethingOps *ops, void *context)
{
    StarsU16 player;
    StarsU16 home_id;
    StarsU16 count;
    StarsU16 i;
    StarsU32 token;
    StarsU32 base;
    StarsU16 base_offset;
    StarsU16 selector;
    const StarsU8 *planet;
    const StarsU8 *fleet_table;
    const StarsU8 *fleet;

    if (!data || !ops || !ops->memory) return 0;
    player = stars_word(data, 0x018cU);
    home_id = stars_word(data, (StarsU16)(0x59a2U +
        (StarsU16)(stars_signed_word(player) * 0xc0) + 8U));
    token = ops->planet_from_id ?
        ops->planet_from_id(context, stars_signed_word(home_id)) : 0;
    planet = token ? ops->memory(context, token) : 0;
    if (!planet || stars_word(planet, 2U) != player) {
        base = stars_dword(data, 0x00f6U);
        base_offset = (StarsU16)base;
        selector = (StarsU16)(base >> 16);
        count = stars_word(data, 0x4b7cU);
        planet = 0;
        for (i = 0; i < count; ++i) {
            token = (StarsU32)(StarsU16)(base_offset + i * 0x38U) |
                ((StarsU32)selector << 16);
            planet = ops->memory(context, token);
            if (planet && stars_word(planet, 2U) == player) break;
            planet = 0;
        }
    }
    if (planet) {
        if (ops->select_planet)
            ops->select_planet(context, 0, stars_word(planet, 0));
        return 1;
    }
    base = stars_dword(data, 0x00faU);
    fleet_table = base ? ops->memory(context, base) : 0;
    count = stars_word(data, 0x5356U);
    for (i = 0; fleet_table && i < count; ++i) {
        token = stars_dword(fleet_table, (StarsU16)(i * 4U));
        if (!token) break;
        fleet = ops->memory(context, token);
        if (fleet && stars_word(fleet, 2U) == player) {
            if (ops->select_fleet)
                ops->select_fleet(context, 0, stars_word(fleet, 0));
            return 1;
        }
    }
    return 0;
}

/* NE 5:2614-286E. Restoring saved state keeps the original validity gates,
 * object-mode repair, message cursor update and tutor notification. */
void stars_restore_selection(StarsU8 *data,
    const StarsRestoreSelectionOps *ops, void *context)
{
    StarsU16 player;
    StarsU16 flags;
    StarsU16 object_id;
    StarsU16 mode;
    StarsU16 home_id;
    StarsU32 token;
    const StarsU8 *object;
    StarsI16 current;
    StarsI16 limit;

    if (!data || !ops || !ops->memory) return;
    player = stars_word(data, 0x018cU);
    object_id = stars_word(data, 0x229eU);
    home_id = stars_word(data, (StarsU16)(0x59a2U +
        (StarsU16)(stars_signed_word(player) * 0xc0) + 8U));
    flags = stars_word(data, 0x229aU);
    mode = (StarsU16)((flags >> 5) & 0x000fU);
    if (stars_word(data, 0x22a0U) == player &&
        stars_dword(data, 0x22a2U) == stars_dword(data, 0x0070U)) {
        if (mode == 2U) {
            token = ops->fleet_from_id ?
                ops->fleet_from_id(context, stars_signed_word(object_id)) : 0;
            if (token && ops->select_fleet) {
                ops->select_fleet(context, 0, object_id);
                stars_put_word(data, 0x229aU,
                    (StarsU16)(stars_word(data, 0x229aU) & 0xfe1fU));
            } else {
                stars_put_word(data, 0x229aU,
                    (StarsU16)((stars_word(data, 0x229aU) & 0xfe1fU) | 0x20U));
                stars_put_word(data, 0x229eU, home_id);
            }
        } else if (mode == 1U) {
            token = ops->planet_from_id ?
                ops->planet_from_id(context, stars_signed_word(object_id)) : 0;
            object = token ? ops->memory(context, token) : 0;
            if (!object || stars_word(object, 2U) != player) {
                stars_put_word(data, 0x229eU, home_id);
            } else {
                if (ops->select_planet)
                    ops->select_planet(context, 0, object_id);
                stars_put_word(data, 0x229aU,
                    (StarsU16)(stars_word(data, 0x229aU) & 0xfe1fU));
            }
        }
        if (stars_word(data, 0x229cU) == stars_word(data, 0x0082U) &&
            stars_signed_word(stars_word(data, 0x22a8U)) > 0) {
            current = (StarsI16)(stars_word(data, 0x22a8U) - 1U);
            limit = stars_signed_word((StarsU16)(stars_word(data, 0x0afeU) +
                stars_word(data, 0x0b0eU)));
            if (current >= limit) current = -1;
            stars_put_word(data, 0x0b00U, (StarsU16)current);
            if (ops->next_message)
                stars_put_word(data, 0x0b00U,
                    (StarsU16)ops->next_message(context, 0));
            stars_put_word(data, 0x07caU,
                (StarsU16)(stars_word(data, 0x07caU) & 0xefffU));
            if (ops->set_message_title)
                ops->set_message_title(context, stars_word(data, 0x0198U));
            if (ops->invalidate)
                ops->invalidate(context, stars_word(data, 0x0198U), 1);
            if (((stars_word(data, 0x07caU) >> 11) & 1U) != 0U) {
                stars_put_word(data, 0x520cU,
                    (StarsU16)((stars_word(data, 0x520cU) & 0xfffbU) | 4U));
                if (ops->advance_tutor) ops->advance_tutor(context);
            }
        }
    } else {
        stars_put_word(data, 0x229aU,
            (StarsU16)((stars_word(data, 0x229aU) & 0xfe1fU) | 0x20U));
        stars_put_word(data, 0x229eU, home_id);
    }
    flags = stars_word(data, 0x229aU);
    if (((flags >> 5) & 0x000fU) != 0U) {
        object_id = stars_word(data, 0x229eU);
        if (object_id == 0xffffU) {
            if (ops->find_selection) ops->find_selection(context);
        } else {
            token = ops->planet_from_id ?
                ops->planet_from_id(context, stars_signed_word(object_id)) : 0;
            object = token ? ops->memory(context, token) : 0;
            if (!object || stars_word(object, 2U) != player) {
                if (ops->find_selection) ops->find_selection(context);
            } else if (ops->select_planet) {
                ops->select_planet(context, 0, object_id);
            }
        }
        stars_put_word(data, 0x229aU,
            (StarsU16)(stars_word(data, 0x229aU) & 0xfe1fU));
    }
}

/* NE 5:75CE-7700. The dialog procedure preserves the original message
 * dispatch; USER/GDI calls and the host help file are explicit callbacks. */
StarsI16 stars_host_options_dialog(StarsU8 *data, StarsU16 hwnd,
    StarsU16 message, StarsU16 wparam, StarsU32 lparam,
    const StarsHostOptionsOps *ops, void *context)
{
    StarsU8 paint_state[32];
    StarsRect rect;
    StarsU16 dc;
    (void)lparam;
    if (message == 2U) return 0;
    if (message == 0x000fU) {
        dc = (ops && ops->begin_paint) ?
            ops->begin_paint(context, hwnd, paint_state) : 0;
        if (ops && ops->draw_options)
            ops->draw_options(context, hwnd, dc, (StarsI16)-1);
        if (ops && ops->end_paint)
            ops->end_paint(context, hwnd, paint_state);
        return 1;
    }
    if (message == 0x0019U) {
        if (ops && ops->set_bk_color)
            ops->set_bk_color(context, wparam, stars_dword(data, 0x22baU));
        return stars_word(data, 0x10U);
    }
    if (message == 0x0111U) {
        if (wparam == 2U || wparam == 1U) {
            if (ops && ops->end_dialog)
                ops->end_dialog(context, hwnd, (StarsI16)(wparam == 1U));
            return 1;
        }
        if (wparam == 0x76U) {
            if (ops && ops->win_help)
                ops->win_help(context, hwnd, data + 0x14aU, 1U, 0x440U);
            return 1;
        }
        return 0;
    }
    if (message == 0x0014U || message == 0x0110U) {
        rect.left = rect.top = rect.right = rect.bottom = 0;
        if (ops && ops->get_client_rect)
            ops->get_client_rect(context, hwnd, &rect);
        if (ops && ops->fill_rect)
            ops->fill_rect(context, wparam, &rect, stars_word(data, 0x10U));
        return 1;
    }
    return 0;
}

/* NE 5:6A26-6C15. Log-file status is host supplied; player status mapping,
 * dirty/error flags, and the tick-cache policy remain in the component. */
StarsI16 stars_find_turns_outstanding(StarsU8 *data,
    const StarsFindTurnsOps *ops, void *context)
{
    StarsU16 player_count;
    StarsU16 i;
    StarsU16 previous;
    StarsU16 player_flags;
    StarsU16 global_flags;
    StarsU16 global_flags_hi;
    StarsU16 count = 0;
    StarsU16 log_error_players = 0;
    StarsU16 output;
    StarsI16 error;
    StarsI16 status;
    StarsU32 tick;
    StarsU16 player_offset;

    if (!data) return 0;
    player_count = stars_word(data, 0x78U);
    tick = stars_dword(data, 0x1d2U);
    stars_put_word(data, 0x74cU, 1U);
    global_flags = stars_word(data, 0x7caU);
    global_flags_hi = stars_word(data, 0x7ccU);
    stars_put_word(data, 0x7caU, (StarsU16)((global_flags & 0xfffdU) | 2U));
    for (i = 0; i < player_count; ++i) {
        player_offset = (StarsU16)(0x59a2U + i * 0xc0U);
        previous = stars_word(data, (StarsU16)(0x65a4U + i * 2U));
        error = 0;
        player_flags = stars_word(data, (StarsU16)(player_offset + 6U));
        if ((player_flags & 0x0200U) == 0U) {
            stars_put_dword(data, 0x1d2U, tick);
            status = ops && ops->check_log_file ?
                ops->check_log_file(context, stars_signed_word(i), &error) : 0;
            tick = stars_dword(data, 0x1d2U);
            if (status == 0) {
                if (error == 0) {
                    if ((stars_word(data, (StarsU16)(player_offset + 0x54U)) & 1U) == 0U) {
                        output = (stars_word(data, 0x7ccU) & 0x0080U) ? 2U : 1U;
                        ++count;
                    } else {
                        output = 0xffffU;
                    }
                } else {
                    output = error == 0x1c ? 4U :
                        (error == 0x1d ? 5U : 3U);
                    ++count;
                }
            } else {
                if ((player_flags & 0x0200U) != 0U) ++log_error_players;
                output = 0;
            }
        } else {
            ++log_error_players;
            output = 0;
        }
        stars_put_word(data, (StarsU16)(0x65a4U + i * 2U), output);
        if (tick == 0U || output != previous) {
            tick = ops && ops->ticks ? ops->ticks(context) : 0;
            stars_put_dword(data, 0x1d2U, tick);
        }
    }
    stars_put_word(data, 0x7caU,
        (StarsU16)(stars_word(data, 0x7caU) & 0xfffdU));
    global_flags_hi = stars_word(data, 0x7ccU);
    stars_put_word(data, 0x7ccU,
        (StarsU16)((global_flags_hi & 0xffefU) |
            ((log_error_players == player_count) ? 0x10U : 0U)));
    stars_put_word(data, 0x74cU, 0U);
    return stars_signed_word(count);
}

/* NE 26:53DA-5475. Selection redraws use the stored slot dialog and a
 * caller-owned client rectangle; drawing remains an explicit host boundary. */
void stars_set_build_selection(StarsU8 *data, StarsI16 source,
    const StarsBuildSelectionOps *ops, void *context)
{
    StarsI16 previous;
    StarsU16 hwnd;
    StarsRect rect;
    if (!data || !ops) return;
    previous = stars_signed_word(stars_word(data, 0x0c7cU));
    if (source == previous && source != -1) return;
    stars_put_word(data, 0x0c7cU, (StarsU16)source);
    hwnd = stars_word(data, 0x0c90U);
    rect.left = rect.top = rect.right = rect.bottom = 0;
    if (ops->get_client_rect) ops->get_client_rect(context, hwnd, &rect);
    if (previous >= 0 && ops->draw_slot)
        ops->draw_slot(context, hwnd, 0, &rect, previous);
    if (source >= 0 && ops->draw_slot)
        ops->draw_slot(context, hwnd, 0, &rect, source);
    if (ops->draw_component)
        ops->draw_component(context, hwnd, 0, -1);
}

/* NE 10:3DEC-3E79. Select the title source from the active object and keep
 * the original planet-title prefix concatenation at the host boundary. */
void stars_set_planet_title_bar(StarsU8 *data, StarsU16 hwnd,
    const StarsPlanetTitleOps *ops, void *context)
{
    StarsU16 selected;
    StarsI16 object_id;
    const StarsU8 *name;
    const StarsU8 *prefix;
    StarsU8 title[128];
    if (!data || !ops || !ops->set_window_text) return;
    selected = stars_word(data, 0x495aU);
    title[0] = 0;
    if (selected == 1U) {
        object_id = stars_signed_word(stars_word(data, 0x49eeU));
        prefix = ops->string ? ops->string(context, 0x33bU) : 0;
        name = ops->planet_name ? ops->planet_name(context, object_id) : 0;
        if (prefix) strcpy((char *)title, (const char *)prefix);
        if (name) strcat((char *)title, (const char *)name);
    } else if (selected == 2U) {
        object_id = stars_signed_word(stars_word(data, 0x4972U));
        name = ops->fleet_name ? ops->fleet_name(context, object_id) : 0;
        if (name) strcpy((char *)title, (const char *)name);
    } else {
        name = ops->string ? ops->string(context, 0x33aU) : 0;
        if (name) strcpy((char *)title, (const char *)name);
    }
    ops->set_window_text(context, hwnd, title);
}

/* NE 9:6AA6-6B70, HfontPrinterCreate. The LOGFONT and TEXTMETRIC buffers
 * use the Win16 field offsets; the adapter owns the returned font handle. */
StarsU16 stars_printer_font_create(StarsRuntime *r, StarsU16 dc,
    StarsI16 size, StarsI16 *font_height)
{
    StarsU8 logfont[50], metrics[32];
    StarsI16 dpi, height;
    StarsU16 font, old;
    memset(logfont, 0, sizeof(logfont));
    dpi = stars_signed_word((StarsU16)stars_window(r, "GetDeviceCaps",
        dc, 0x5aU, 0, 0, 0, 0));
    height = (StarsI16)(((StarsI32)size * dpi) / 72L);
    stars_put_word(logfont, 0, (StarsU16)(0U - (StarsU16)height));
    strcpy((char *)logfont + 18, (char *)r->data + 0x6c6U);
    font = (StarsU16)stars_window(r, "CreateFontIndirect", 0, 0, 0, 0,
        logfont, 0);
    if (font_height && font) {
        old = stars_select(r, dc, font);
        memset(metrics, 0, sizeof(metrics));
        stars_window(r, "GetTextMetrics", dc, 0, 0, 0, metrics, 0);
        *font_height = stars_signed_word((StarsU16)(stars_word(metrics, 0) +
            stars_word(metrics, 8)));
        stars_select(r, dc, old);
    }
    return font;
}

/* NE 8:7E44-7F69, DrawPlanetPrintDot. */
void stars_draw_planet_print_dot(StarsRuntime *r, StarsU16 dc, StarsI16 x,
    StarsI16 y, StarsI16 size)
{
    if (size == 0) {
        stars_pat_blt(r, dc, (StarsU16)(x - 3), (StarsU16)(y - 1), 7, 3, 0x42);
        stars_pat_blt(r, dc, (StarsU16)(x - 1), (StarsU16)(y - 3), 3, 7, 0x42);
        stars_pat_blt(r, dc, (StarsU16)(x - 2), (StarsU16)(y - 2), 5, 5, 0x42);
    } else {
        stars_pat_blt(r, dc, (StarsU16)(x - 5), (StarsU16)(y - 2), 11, 5, 0x42);
        stars_pat_blt(r, dc, (StarsU16)(x - 2), (StarsU16)(y - 5), 5, 11, 0x42);
        stars_pat_blt(r, dc, (StarsU16)(x - 4), (StarsU16)(y - 3), 9, 7, 0x42);
        stars_pat_blt(r, dc, (StarsU16)(x - 3), (StarsU16)(y - 4), 7, 9, 0x42);
    }
}

StarsI16 stars_half(StarsU16 value)
{ return stars_signed_word((StarsU16)((value >> 1) | (value & 0x8000U))); }
StarsRect stars_rect(StarsU16 l, StarsU16 t, StarsU16 rr, StarsU16 b)
{ StarsRect rect; rect.left = l; rect.top = t; rect.right = rr; rect.bottom = b; return rect; }
StarsRect stars_read_rect(const StarsU8 *d, StarsU16 off)
{ return stars_rect(stars_word(d, off), stars_word(d, (StarsU16)(off + 2U)),
    stars_word(d, (StarsU16)(off + 4U)), stars_word(d, (StarsU16)(off + 6U))); }
void stars_write_rect(StarsU8 *d, StarsU16 off, const StarsRect *p)
{
    stars_put_word(d, off, p->left); stars_put_word(d, (StarsU16)(off + 2U), p->top);
    stars_put_word(d, (StarsU16)(off + 4U), p->right); stars_put_word(d, (StarsU16)(off + 6U), p->bottom);
}
int stars_point_in_rect(const StarsRect *p, StarsPoint point)
{
    return stars_signed_word(point.x) >= stars_signed_word(p->left) &&
        stars_signed_word(point.x) < stars_signed_word(p->right) &&
        stars_signed_word(point.y) >= stars_signed_word(p->top) &&
        stars_signed_word(point.y) < stars_signed_word(p->bottom);
}
StarsU32 stars_text_extent(StarsRuntime *r, StarsU16 dc, const StarsU8 *text, StarsU16 n)
{ return stars_window(r, "GetTextExtent", dc, n, 0, 0, (void *)text, 0); }
void stars_text_out(StarsRuntime *r, StarsU16 dc, StarsU16 x, StarsU16 y, const StarsU8 *text, StarsU16 n)
{ stars_window(r, "TextOut", dc, x, y, n, (void *)text, 0); }
void stars_pat_blt(StarsRuntime *r, StarsU16 dc, StarsU16 x, StarsU16 y, StarsU16 w, StarsU16 h, StarsU32 rop)
{ StarsPoint size; size.x = w; size.y = h; stars_window(r, "PatBlt", dc, x, y, rop, &size, 0); }
void stars_window_position(StarsRuntime *r, StarsU16 hwnd, StarsU16 x, StarsU16 y,
    StarsU16 width, StarsU16 height, StarsU16 flags)
{ StarsRect box = stars_rect(x, y, width, height); stars_window(r, "SetWindowPos", hwnd, 0, flags, 0, &box, 0); }
StarsU16 stars_select(StarsRuntime *r, StarsU16 dc, StarsU16 object)
{ return (StarsU16)stars_window(r, "SelectObject", dc, object, 0, 0, 0, 0); }
void stars_fill_rect(StarsRuntime *r, StarsU16 dc, const StarsRect *rect, StarsU16 brush)
{ stars_window(r, "FillRect", dc, brush, 0, 0, (void *)rect, 0); }
void stars_screen_point(StarsRuntime *r, StarsU16 hwnd, void *point)
{ stars_window(r, "ScreenToClient", hwnd, 0, 0, 0, point, 0); }

/* NE 9:3094-31A2. Remembered positions are clamped; a newly centered dialog
 * retains the original negative position when larger than the screen. */
void stars_sticky_dialog(StarsRuntime *r, StarsU16 hwnd, StarsPoint *position, StarsI16 init)
{
    StarsRect rect;
    StarsI16 width, height;
    stars_window(r, "GetWindowRect", hwnd, 0, 0, 0, &rect, 0);
    if (!init) { position->x = rect.left; position->y = rect.top; return; }
    width = stars_signed_word((StarsU16)stars_window(r, "GetSystemMetrics", 0, 0, 0, 0, 0, 0));
    height = stars_signed_word((StarsU16)stars_window(r, "GetSystemMetrics", 0, 1, 0, 0, 0, 0));
    if ((StarsU16)position->x == 0xffffU &&
            (StarsU16)position->y == 0xffffU) {
        rect.left = (StarsU16)stars_half((StarsU16)(width - (rect.right - rect.left)));
        rect.top = (StarsU16)stars_half((StarsU16)(height - (rect.bottom - rect.top)));
    } else {
        stars_offset_rect(&rect, stars_signed_word((StarsU16)(position->x - rect.left)),
            stars_signed_word((StarsU16)(position->y - rect.top)));
        if (width < stars_signed_word(rect.right)) rect.left = (StarsU16)(rect.left + width - rect.right);
        if (height < stars_signed_word(rect.bottom)) rect.top = (StarsU16)(rect.top + height - rect.bottom);
        if (stars_signed_word(rect.left) < 0) rect.left = 0;
        if (stars_signed_word(rect.top) < 0) rect.top = 0;
    }
    stars_window_position(r, hwnd, rect.left, rect.top, 0, 0, 0x15U);
}

void stars_draw_frame(StarsRuntime *r, StarsU16 dc, const StarsRect *p, StarsI16 erase)
{
    StarsRect rect = *p;
    StarsU16 width = (StarsU16)(p->right - p->left), height = (StarsU16)(p->bottom - p->top);
    StarsU16 old = stars_select(r, dc, stars_word(r->data, 0x14U));
    if (erase == -1 || erase == -2) {
        stars_pat_blt(r, dc, rect.left, rect.bottom, (StarsU16)(width + 1U), 1, 0xf00021UL);
        stars_pat_blt(r, dc, rect.right, rect.top, 1, height, 0xf00021UL);
        stars_select(r, dc, stars_word(r->data, 0x16U));
        stars_pat_blt(r, dc, rect.left, rect.top, width, 1, 0xf00021UL);
        stars_pat_blt(r, dc, rect.left, rect.top, 1, height, 0xf00021UL);
        stars_expand_rect(&rect, -1, -1); width = (StarsU16)(width - 2U); height = (StarsU16)(height - 2U);
        stars_select(r, dc, stars_word(r->data, 0x14U));
    }
    stars_pat_blt(r, dc, rect.left, rect.top, width, 1, 0xf00021UL);
    stars_pat_blt(r, dc, rect.left, rect.top, 1, height, 0xf00021UL);
    stars_select(r, dc, stars_word(r->data, 0x16U));
    stars_pat_blt(r, dc, rect.left, rect.bottom, (StarsU16)(width + 1U), 1, 0xf00021UL);
    stars_pat_blt(r, dc, rect.right, rect.top, 1, height, 0xf00021UL);
    if (erase && erase != -2) {
        stars_select(r, dc, stars_word(r->data, 0x10U));
        stars_pat_blt(r, dc, (StarsU16)(rect.left + 1U), (StarsU16)(rect.top + 1U),
            (StarsU16)(width - 1U), (StarsU16)(height - 1U), 0xf00021UL);
    }
    stars_select(r, dc, old);
}

void stars_center_text(StarsRuntime *r, StarsU16 dc, StarsI16 x, StarsI16 y, StarsU16 text, StarsI16 n)
{
    StarsU32 extent;
    if (!n) n = (StarsI16)strlen((char *)r->data + text);
    extent = stars_text_extent(r, dc, r->data + text, (StarsU16)n);
    stars_text_out(r, dc, (StarsU16)(x - stars_half((StarsU16)extent)), (StarsU16)y, r->data + text, (StarsU16)n);
}
void stars_rect_center_text(StarsRuntime *r, StarsU16 dc, const StarsRect *rect, StarsU16 text, StarsI16 n)
{
    StarsU32 extent;
    StarsI16 dx, dy;
    if (n == -1) { stars_fill_rect(r, dc, rect, stars_word(r->data, 0x10U)); n = 0; }
    if (!n) n = (StarsI16)strlen((char *)r->data + text);
    extent = stars_text_extent(r, dc, r->data + text, (StarsU16)n);
    dx = stars_signed_word((StarsU16)(rect->right - rect->left - (StarsU16)extent));
    dy = stars_signed_word((StarsU16)(rect->bottom - rect->top - (StarsU16)(extent >> 16)));
    stars_text_out(r, dc, (StarsU16)(dx / 2 + rect->left), (StarsU16)(dy / 2 + rect->top), r->data + text, (StarsU16)n);
}
void stars_right_text(StarsRuntime *r, StarsU16 dc, StarsI16 x, StarsI16 y, StarsU16 text, StarsI16 n, StarsI16 erase)
{
    StarsI16 width;
    StarsRect rect;
    if (n == -1) n = stars_copy_game_string(r->data, r->strings, 0x263, text);
    else if (!n) n = (StarsI16)strlen((char *)r->data + text);
    width = stars_signed_word((StarsU16)stars_text_extent(r, dc, r->data + text, (StarsU16)n));
    if (width < erase) {
        rect = stars_rect((StarsU16)(x - erase), (StarsU16)y, (StarsU16)(x - width),
            (StarsU16)(y + stars_word(r->data, 0x23faU)));
        stars_fill_rect(r, dc, &rect, stars_word(r->data, 0x10U));
    } else if (erase > 0 && erase < width) {
        rect = stars_rect((StarsU16)(x - erase), (StarsU16)y, (StarsU16)x,
            (StarsU16)(y + stars_word(r->data, 0x23faU)));
        /* value packs options in the high word, length in the low word. */
        stars_window(r, "ExtTextOut", dc, (StarsU16)(x - width), (StarsU16)y,
            0x60000UL | (StarsU16)n, r->data + text, &rect);
        return;
    }
    stars_text_out(r, dc, (StarsU16)(x - width), (StarsU16)y, r->data + text, (StarsU16)n);
}

StarsU16 stars_dib_num_colors(const StarsU8 *header)
{
    StarsU16 depth;
    if (stars_dword(header, 0) == 12U) depth = stars_word(header, 10U);
    else {
        if (stars_dword(header, 32U)) return stars_word(header, 32U);
        depth = stars_word(header, 14U);
    }
    return depth == 1U ? 2U : depth == 4U ? 16U : depth == 8U ? 256U : 0;
}
StarsU16 stars_palette_size(const StarsU8 *header)
{ return (StarsU16)(stars_dib_num_colors(header) * (stars_dword(header, 0) == 12U ? 3U : 4U)); }
StarsI16 stars_dib_blt(StarsRuntime *r, StarsU16 dc, StarsI16 x, StarsI16 y, StarsI16 width,
    StarsI16 height, StarsU16 bitmap, StarsI16 sx, StarsI16 sy, StarsI16 sw, StarsI16 sh, StarsU32 rop)
{
    StarsU32 pointer;
    const StarsU8 *header;
    StarsI16 geometry[8];
    StarsU16 bits_offset;
    if (!bitmap) {
        StarsPoint size;
        size.x = (StarsU16)width; size.y = (StarsU16)height;
        return stars_signed_word((StarsU16)stars_window(r, "PatBlt", dc,
            (StarsU16)x, (StarsU16)y, rop, &size, 0));
    }
    pointer = stars_window(r, "GlobalLock", bitmap, 0, 0, 0, 0, 0);
    if (!pointer) return 0;
    header = r->platform->memory(r->context, (StarsU16)(pointer >> 16), (StarsU16)pointer);
    bits_offset = (StarsU16)(stars_word(header, 0) + stars_palette_size(header));
    geometry[0] = x; geometry[1] = y; geometry[2] = width; geometry[3] = height;
    geometry[4] = sx; geometry[5] = sy; geometry[6] = sw; geometry[7] = sh;
    /* q points to the BITMAPINFO, a gives its near offset to the bits.
     * The native adapter preserves the selector when adding that offset. */
    stars_window(r, "StretchDIBits", dc, bits_offset, 0, rop, geometry, (void *)header);
    stars_window(r, "GlobalUnlock", bitmap, 0, 0, 0, 0, 0);
    return 1;
}

StarsI16 stars_get_mouse_move(StarsRuntime *r, StarsPoint *point)
{
    StarsU8 message[18];
    StarsU16 type;
    for (;;) {
        if (!stars_window(r, "PeekMessage", 0, 0, 0, 1, message, 0)) return 1;
        type = stars_word(message, 2);
        if (type == 0x200U || type == 0x202U) {
            point->x = stars_word(message, 6); point->y = stars_word(message, 8);
            return (StarsI16)(type != 0x202U);
        }
        /* The original consumes unrelated messages without dispatching them. */
    }
}

/* NE 9:41E0-4279, FGetRMouseMove. Unlike the left-button helper, the
 * terminating button event is WM_RBUTTONUP (205h). */
StarsI16 stars_get_right_mouse_move(StarsRuntime *r, StarsPoint *point)
{
    StarsU8 message[18];
    StarsU16 type;
    for (;;) {
        if (!stars_window(r, "PeekMessage", 0, 0, 0, 1, message, 0)) return 1;
        type = stars_word(message, 2);
        if (type == 0x200U || type == 0x205U) {
            point->x = stars_word(message, 6);
            point->y = stars_word(message, 8);
            return (StarsI16)(type != 0x205U);
        }
    }
}

/* NE 9:2590-25FD, DxStreamTextOut. GetTextExtent returns packed width and
 * height; this routine uses the signed low word as the stream advance. */
StarsI16 stars_dx_stream_text_out(StarsRuntime *r, StarsU16 dc, StarsI16 *x,
    StarsI16 y, const StarsU8 *text, StarsI16 count, StarsI16 print)
{
    StarsU32 extent;
    StarsI16 width;
    if (!count) count = (StarsI16)strlen((const char *)text);
    extent = stars_text_extent(r, dc, text, (StarsU16)count);
    width = stars_signed_word((StarsU16)extent);
    if (print) stars_text_out(r, dc, (StarsU16)*x, (StarsU16)y, text, (StarsU16)count);
    *x = stars_signed_word((StarsU16)((StarsU16)*x + (StarsU16)width));
    return width;
}

/* NE 9:43AA-4448, FStringFitsScreen. The near adapter supplies the frame
 * window and Arial font handles explicitly; the original truncates in place
 * until the unsigned extent fits the unsigned 16-bit limit. */
StarsI16 stars_string_fits_screen(StarsRuntime *r, StarsU16 frame,
    StarsU16 font, StarsU8 *text, StarsI16 maximum)
{
    StarsU16 dc, old;
    StarsU16 length = (StarsU16)strlen((const char *)text);
    StarsI16 result = 1;
    dc = (StarsU16)stars_window(r, "GetDC", frame, 0, 0, 0, 0, 0);
    old = stars_select(r, dc, font);
    while (length) {
        if ((StarsU16)stars_text_extent(r, dc, text, length) <= (StarsU16)maximum)
            break;
        result = 0;
        text[--length] = 0;
    }
    stars_select(r, dc, old);
    stars_window(r, "ReleaseDC", frame, dc, 0, 0, 0, 0);
    return result;
}

static StarsU16 trim_spaces(const StarsU8 *text, StarsU16 start, StarsU16 end)
{
    while (end > start && text[(StarsU16)(end - 1U)] == ' ') --end;
    return end;
}

static StarsU16 chop_word(const StarsU8 *text, StarsU16 start, StarsU16 end)
{
    end = trim_spaces(text, start, end);
    while (end > start && text[(StarsU16)(end - 1U)] != ' ') --end;
    return trim_spaces(text, start, end);
}

/* NE 9:25FE-280C, WrapTextOut. The source adapter supplies the original
 * window and text pointers; text is read-only because the original helpers
 * adjust only their local end pointers. */
void stars_wrap_text_out(StarsRuntime *r, StarsU16 dc, StarsI16 *x,
    StarsI16 *y, const StarsU8 *text, StarsI16 count, StarsI16 left,
    StarsI16 width_limit, StarsI16 *maximum, StarsI16 new_line,
    StarsI16 print)
{
    const StarsU8 *start = text, *end, *full_end;
    StarsI16 available, extent;
    int keep;
    if (!count) count = (StarsI16)strlen((const char *)text);
    full_end = text + count;
    available = (StarsI16)(width_limit - (*x - left));
    if (new_line) {
        *y = stars_signed_word((StarsU16)((StarsU16)*y +
            stars_word(r->data, 0x23faU)));
        *x = left;
    }
    do {
        end = start + count;
        end = start + trim_spaces(start, 0, (StarsU16)(end - start));
        extent = stars_signed_word((StarsU16)stars_text_extent(r, dc, start,
            (StarsU16)(end - start)));
        keep = 1;
        while (available < extent && start < end && extent > 0) {
            keep = 0;
            end = start + chop_word(start, 0, (StarsU16)(end - start));
            extent = stars_signed_word((StarsU16)stars_text_extent(r, dc,
                start, (StarsU16)(end - start)));
        }
        if (keep) {
            while (end < full_end && *end == ' ') ++end;
            extent = stars_signed_word((StarsU16)stars_text_extent(r, dc,
                start, (StarsU16)(end - start)));
        }
        if (start == end) {
            if (*x == left) {
                end = full_end;
                extent = stars_signed_word((StarsU16)stars_text_extent(r, dc,
                    start, (StarsU16)(end - start)));
            }
        } else {
            if (print) stars_text_out(r, dc, (StarsU16)*x, (StarsU16)*y,
                start, (StarsU16)(end - start));
            *x = stars_signed_word((StarsU16)((StarsU16)*x +
                (StarsU16)extent));
            if (maximum && *maximum < *x) *maximum = *x;
            if (end == full_end) return;
        }
        while (end < full_end && *end == ' ') ++end;
        count = (StarsI16)(full_end - end);
        start = end;
        *y = stars_signed_word((StarsU16)((StarsU16)*y +
            stars_word(r->data, 0x23faU)));
        *x = left;
        available = width_limit;
    } while (1);
}

/* NE 9:427A-43AA, DrawFuzzyBorder. Restore all three selected drawing
 * attributes in the same reverse order as the original. */
void stars_draw_fuzzy_border(StarsRuntime *r, StarsU16 dc,
    const StarsRect *rect, StarsU16 brush, StarsU32 button_face,
    StarsU32 window_text)
{
    StarsU32 old_background, old_foreground;
    StarsU16 old_brush;
    StarsI16 width = stars_signed_word((StarsU16)(rect->right - rect->left));
    StarsI16 height = stars_signed_word((StarsU16)(rect->bottom - rect->top));
    old_background = stars_window(r, "SetBkColor", dc, 0, 0, button_face, 0, 0);
    old_foreground = stars_window(r, "SetTextColor", dc, 0, 0, window_text, 0, 0);
    old_brush = stars_select(r, dc, brush);
    stars_pat_blt(r, dc, rect->left, rect->top, (StarsU16)width, 2, 0x5a0049UL);
    stars_pat_blt(r, dc, rect->left, (StarsU16)(rect->bottom - 2U),
        (StarsU16)width, 2, 0x5a0049UL);
    stars_pat_blt(r, dc, rect->left, (StarsU16)(rect->top + 2U), 2,
        (StarsU16)(height - 4), 0x5a0049UL);
    stars_pat_blt(r, dc, (StarsU16)(rect->right - 2U),
        (StarsU16)(rect->top + 2U), 2, (StarsU16)(height - 4), 0x5a0049UL);
    stars_select(r, dc, old_brush);
    stars_window(r, "SetTextColor", dc, 0, 0, old_foreground, 0, 0);
    stars_window(r, "SetBkColor", dc, 0, 0, old_background, 0, 0);
}

/* NE 9:4C9C-4D60, HpalBlackReserved. LOGPALETTE is emitted explicitly so
 * the reserved flag and all 256 black RGB entries remain observable. */
StarsU16 stars_black_reserved_palette(StarsRuntime *r)
{
    StarsU8 palette[4U + 4U * 256U];
    StarsU16 i;
    palette[0] = 0; palette[1] = 3;
    palette[2] = 0; palette[3] = 1;
    for (i = 0; i < 256U; ++i) {
        palette[4U + i * 4U] = 0;
        palette[5U + i * 4U] = 0;
        palette[6U + i * 4U] = 0;
        palette[7U + i * 4U] = 1;
    }
    return (StarsU16)stars_window(r, "CreatePalette", 0, 0, 0, 0,
        palette, 0);
}
void stars_init_button_track(StarsRuntime *r, StarsButtonTrack *p, StarsU16 hwnd, StarsU16 dc,
    const StarsRect *rect, StarsU16 type, StarsU16 delay, StarsU16 down, StarsU16 no_redraw, StarsU16 text)
{
    p->window = hwnd; p->flags = (StarsU16)((p->flags & 0xfff7U) | (dc ? 0 : 8U));
    if (!dc) dc = (StarsU16)stars_window(r, "GetDC", hwnd, 0, 0, 0, 0, 0);
    p->dc = dc; p->rect = *rect; p->type = type; p->delay = delay;
    p->flags = (StarsU16)((p->flags & 0xffe0U) | (p->flags & 8U) | 3U |
        ((down & 1U) << 2) | ((no_redraw & 1U) << 4));
    p->text = text;
}
StarsI16 stars_track_button(StarsRuntime *r, StarsButtonTrack *p)
{
    StarsPoint point;
    StarsU16 inside;
    StarsU32 now, deadline;
    if (p->flags & 1U) {
        stars_window(r, "SetCapture", p->window, 0, 0, 0, 0, 0);
        stars_draw_button(r, p->dc, &p->rect, p->type,
            (StarsI16)(((p->flags >> 1) ^ (p->flags >> 2)) & 1U), p->text);
        now = stars_window(r, "GetCurrentTime", 0, 0, 0, 0, 0, 0);
        now += (StarsU32)(StarsI32)stars_signed_word((StarsU16)(p->delay * 3U));
        stars_put_dword(p->ticks, 0, now); p->flags &= 0xfffeU;
    } else {
        point.x = p->rect.left; point.y = p->rect.top;
        for (;;) {
            if (!stars_get_mouse_move(r, &point)) {
                if ((p->flags & 2U) && !(p->flags & 16U)) {
                    p->flags &= 0xfffdU;
                    stars_draw_button(r, p->dc, &p->rect, p->type, (StarsI16)((p->flags >> 2) & 1U), p->text);
                    p->flags |= 2U;
                }
                stars_window(r, "ReleaseCapture", 0, 0, 0, 0, 0, 0);
                if (p->flags & 8U) stars_window(r, "ReleaseDC", p->window, p->dc, 0, 0, 0, 0);
                return 0;
            }
            inside = (StarsU16)stars_point_in_rect(&p->rect, point);
            if (inside != ((p->flags >> 1) & 1U)) {
                p->flags = (StarsU16)((p->flags & 0xfffdU) | (inside << 1));
                stars_draw_button(r, p->dc, &p->rect, p->type,
                    (StarsI16)(((p->flags >> 1) ^ (p->flags >> 2)) & 1U), p->text);
            }
            now = stars_window(r, "GetCurrentTime", 0, 0, 0, 0, 0, 0);
            deadline = stars_dword(p->ticks, 0);
            if ((now ^ 0x80000000UL) >= (deadline ^ 0x80000000UL) && (p->flags & 2U)) break;
        }
        stars_put_dword(p->ticks, 0, now + (StarsU32)(StarsI32)stars_signed_word(p->delay));
    }
    return 1;
}

void stars_draw_button(StarsRuntime *r, StarsU16 dc, const StarsRect *p, StarsU16 type, StarsI16 down, StarsU16 text)
{
    StarsRect rect = *p;
    StarsU8 *d = r->data;
    StarsU16 old = stars_select(r, dc, stars_word(d, 0x1cU)), brush, font, mode;
    StarsU32 color;
    StarsI16 pts[12], w, h, dx, dy, x, y, step, length, t;
    StarsI32 scale;
    int i, count, disabled = (type & 4U) != 0, direction = type & 3U;
    if (!(type & 0x40U)) {
        stars_window(r, "FrameRect", dc, stars_word(d, 0x1cU), 0, 0, &rect, 0);
        stars_expand_rect(&rect, -1, -1);
    }
    w = stars_signed_word((StarsU16)(rect.right - rect.left - 1U));
    h = stars_signed_word((StarsU16)(rect.bottom - rect.top - 1U));
    scale = (StarsI32)stars_signed_word((StarsU16)(p->right - p->left)) - 5L;
    stars_select(r, dc, stars_word(d, down ? 0x16U : 0x14U));
    stars_pat_blt(r, dc, rect.left, rect.top, (StarsU16)w, 1, 0xf00021UL);
    stars_pat_blt(r, dc, rect.left, rect.top, 1, (StarsU16)h, 0xf00021UL);
    stars_select(r, dc, stars_word(d, down ? 0x10U : 0x16U));
    stars_pat_blt(r, dc, rect.left, (StarsU16)(rect.bottom - 1U), (StarsU16)(w + 1), 1, 0xf00021UL);
    stars_pat_blt(r, dc, (StarsU16)(rect.right - 1U), rect.top, 1, (StarsU16)h, 0xf00021UL);
    if (w < 14 || h < 14 || (type & 0xc0U)) stars_expand_rect(&rect, -1, -1);
    else {
        stars_pat_blt(r, dc, (StarsU16)(rect.left + 1U), (StarsU16)(rect.bottom - 2U), (StarsU16)(w - 1), 1, 0xf00021UL);
        stars_pat_blt(r, dc, (StarsU16)(rect.right - 2U), (StarsU16)(rect.top + 1U), 1, (StarsU16)(h - 2), 0xf00021UL);
        rect = stars_rect((StarsU16)(rect.left + 1U), (StarsU16)(rect.top + 1U),
            (StarsU16)(rect.right - 2U), (StarsU16)(rect.bottom - 2U));
    }
    stars_select(r, dc, stars_word(d, 0x10U)); stars_fill_rect(r, dc, &rect, stars_word(d, 0x10U));
    if (!(type & 8U)) {
        count = (type & 0x20U) ? 3 : 5; w = (type & 0x20U) ? 8 : 6; h = (type & 0x20U) ? 4 : 6;
        for (i = 0; i < count * 2; ++i) pts[i] = stars_signed_word(stars_word(d,
            (StarsU16)(((type & 0x20U) ? 0x13a0U : 0x138cU) + i * 2U)));
        if (type & 16U) {
            for (i = 0; i < count; ++i) ++pts[i * 2 + 1];
            h += 2; pts[count * 2] = 0; pts[count * 2 + 1] = 0; ++count;
        }
        for (i = 0; i < count; ++i) {
            if (pts[i * 2] < 0) pts[i * 2] = (StarsI16)(pts[0] * 2 - pts[-pts[i * 2] * 2]);
            else pts[i * 2] = (StarsI16)((StarsI32)pts[i * 2] * scale / 11L);
            pts[i * 2 + 1] = (StarsI16)((StarsI32)pts[i * 2 + 1] * scale / 11L);
        }
        w = (StarsI16)((StarsI32)w * scale / 11L); h = (StarsI16)((StarsI32)h * scale / 11L);
        dx = (StarsI16)(stars_half((StarsU16)(p->right - p->left - w)) + down);
        dy = (StarsI16)(stars_half((StarsU16)(p->bottom - p->top - h)) + down);
        if (direction >= 2) { t = dx; dx = dy; dy = t; }
        for (i = 0; i < count; ++i) {
            if (direction == 1) pts[i * 2 + 1] = (StarsI16)(h - pts[i * 2 + 1]);
            if (direction >= 2) {
                t = (StarsI16)(w - pts[i * 2]); pts[i * 2] = pts[i * 2 + 1]; pts[i * 2 + 1] = t;
                if (direction == 3) pts[i * 2] = (StarsI16)(h - pts[i * 2]);
            }
            pts[i * 2] = stars_signed_word((StarsU16)(pts[i * 2] + p->left + dx));
            pts[i * 2 + 1] = stars_signed_word((StarsU16)(pts[i * 2 + 1] + p->top + dy));
        }
        brush = stars_word(d, disabled ? 0x14U : 0x12U); stars_select(r, dc, brush);
        for (;;) {
            if (direction < 2) {
                step = direction == 0 ? -1 : 1; x = pts[2]; y = pts[3]; length = (StarsI16)(pts[4] - pts[2] + 1);
                if (type & 16U) stars_pat_blt(r, dc, (StarsU16)x, (StarsU16)pts[(count - 1) * 2 + 1], (StarsU16)length, 1, 0xf00021UL);
                for (; length > 0; length -= 2, ++x, y += step)
                    stars_pat_blt(r, dc, (StarsU16)x, (StarsU16)y, (StarsU16)length, 1, 0xf00021UL);
            } else {
                step = direction == 2 ? -1 : 1; x = pts[2]; y = pts[5]; length = (StarsI16)(pts[3] - pts[5] + 1);
                if (type & 16U) stars_pat_blt(r, dc, (StarsU16)pts[(count - 1) * 2], (StarsU16)y, 1, (StarsU16)length, 0xf00021UL);
                for (; length > 0; length -= 2, ++y, x += step)
                    stars_pat_blt(r, dc, (StarsU16)x, (StarsU16)y, 1, (StarsU16)length, 0xf00021UL);
            }
            if (!(type & 0x20U)) {
                rect = stars_rect((StarsU16)(pts[6] < pts[8] ? pts[6] : pts[8]),
                    (StarsU16)(pts[7] < pts[9] ? pts[7] : pts[9]),
                    (StarsU16)((pts[6] > pts[8] ? pts[6] : pts[8]) + 1),
                    (StarsU16)((pts[7] > pts[9] ? pts[7] : pts[9]) + 1));
                stars_fill_rect(r, dc, &rect, brush);
            }
            if (!disabled) break;
            for (i = 0; i < count * 2; ++i) --pts[i];
            disabled = 0; brush = stars_word(d, 0x16U); stars_select(r, dc, brush);
        }
    }
    stars_select(r, dc, old);
    if (text) {
        font = stars_select(r, dc, stars_word(d, 0x26b6U));
        mode = (StarsU16)stars_window(r, "SetBkMode", dc, 1, 0, 0, 0, 0);
        color = stars_window(r, "SetTextColor", dc, 0, 0, stars_dword(d, disabled ? 0x3eecU : 0x25f6U), 0, 0);
        rect = *p; if (down) stars_offset_rect(&rect, 1, 1);
        stars_rect_center_text(r, dc, &rect, text, 0);
        if (disabled) {
            stars_offset_rect(&rect, -1, -1); stars_window(r, "SetTextColor", dc, 0, 0, stars_dword(d, 0x23feU), 0, 0);
            stars_rect_center_text(r, dc, &rect, text, 0);
        }
        stars_window(r, "SetTextColor", dc, 0, 0, color, 0, 0);
        stars_window(r, "SetBkMode", dc, mode, 0, 0, 0, 0); stars_select(r, dc, font);
    }
}
