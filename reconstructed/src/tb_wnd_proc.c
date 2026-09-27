#include "core.h"

enum {
    TB_WM_MOUSEMOVE = 0x0200,
    TB_WM_LBUTTONDOWN = 0x0201,
    TB_WM_LBUTTONDOUBLECLICK = 0x0203,
    TB_WM_TOOLBAR_COMMIT = 0x05f4,
    TB_RADAR_COMMAND_NOTIFY = 8,
    TB_RADAR_PERCENT_COUNT = 10,
    TB_RADAR_FIRST_PERCENT = 100,
    TB_RADAR_PERCENT_STEP = 10,
    TB_BORDER_HEIGHT = 1,
    TB_BUTTON_HEIGHT = 0x1c,
    TB_BUTTON_START_X = 4,
    TB_BUTTON_START_Y = 4,
    TB_RADAR_ID = -3,
    TB_RADAR_Y_EXTRA = 4,
    TB_RADAR_HEIGHT_ROWS = 11,
    TB_RADAR_HEIGHT_EXTRA = 0x1c,
    TB_SPECIAL_BUTTON_RADAR = 0x0d,
    TB_SPECIAL_BUTTON_ZOOM_IN = 0x0f,
    TB_SPECIAL_BUTTON_ZOOM_OUT = 0x10,
    TB_SPECIAL_BUTTON_HELP = 8
};

static StarsI16 tb_low_word(StarsU32 value)
{ return (StarsI16)(StarsU16)value; }

static StarsI16 tb_high_word(StarsU32 value)
{ return (StarsI16)(StarsU16)(value >> 16); }

static StarsI16 tb_button_is_direct(StarsI16 button)
{
    return (StarsI16)(button == TB_SPECIAL_BUTTON_RADAR ||
        button == TB_SPECIAL_BUTTON_ZOOM_IN ||
        button == TB_SPECIAL_BUTTON_ZOOM_OUT ||
        button == TB_SPECIAL_BUTTON_HELP);
}

static StarsI16 tb_button_down(const StarsTbWndState *state,
    StarsI16 button)
{
    if (state->ops->button_down)
        return state->ops->button_down(state->context, button);
    return 0;
}

static void tb_draw_button(const StarsTbWndState *state, StarsU16 dc,
    StarsPoint point, StarsI16 button, StarsI16 down)
{
    if (state->ops->draw_button)
        state->ops->draw_button(state->context, dc, point, button, down);
}

static StarsI32 tb_default(const StarsTbWndState *state, StarsU16 hwnd,
    StarsU16 message, StarsU16 wparam, StarsU32 lparam)
{
    if (state->ops->default_proc)
        return state->ops->default_proc(state->context, hwnd, message,
            wparam, lparam);
    return 0;
}

/* Genuine semantic reconstruction of TbWndProc, NE 14:001E-06F0.
 * The toolbar is a small command surface with one special radar control.
 * Creation inserts that control and installs the proxy procedures; paint
 * draws the two-dimensional border and toolbar table; mouse movement keeps
 * tooltip coordinates synchronized; button clicks use capture so the visual
 * pressed state follows the pointer; and 05F4 commits the radar edit. */
StarsI32 stars_tb_wnd_proc(StarsTbWndState *state, StarsU16 hwnd,
    StarsU16 message, StarsU16 wparam, StarsU32 lparam)
{
    StarsI16 i;
    StarsI16 button;
    StarsI16 down;
    StarsI16 inside;
    StarsI16 old_inside;
    StarsI16 tooltip_id;
    StarsI16 width;
    StarsI16 x;
    StarsPoint point;
    StarsPoint original;
    StarsPoint mapped_rect[2];
    StarsRect client;
    StarsRect button_rect;
    StarsU16 dc;

    if (!state || !state->ops || !state->buttons) return 0;
    client.left = client.top = client.right = client.bottom = 0;

    if (message == STARS_MESSAGE_WM_CREATE) {
        x = TB_BUTTON_START_X;
        for (i = 0; i < (StarsI16)state->button_count; ++i) {
            button = state->buttons[i];
            width = stars_dx_of_btn(button, state->arial8_height);
            if (button == TB_RADAR_ID) {
                if (state->ops->create_radar)
                    state->ops->create_radar(state->context,
                        state->toolbar_window, x,
                        (StarsI16)((20 - state->arial8_height) / 2 +
                        TB_RADAR_Y_EXTRA), width,
                        (StarsI16)(state->arial8_height *
                        TB_RADAR_HEIGHT_ROWS + TB_RADAR_HEIGHT_EXTRA));
                if (state->ops->initialize_radar)
                    state->ops->initialize_radar(state->context,
                        state->radar_window, state->radar_view);
                if (state->ops->install_combo_proxy)
                    state->ops->install_combo_proxy(state->context,
                        state->radar_window);
                if (state->ops->install_edit_proxy)
                    state->ops->install_edit_proxy(state->context,
                        state->radar_window);
            }
            x = (StarsI16)(x + width);
        }
        return 0;
    }

    if (message == STARS_MESSAGE_WM_PAINT) {
        dc = state->ops->begin_paint ? state->ops->begin_paint(
            state->context, hwnd) : 0;
        if (state->ops->get_client_rect)
            state->ops->get_client_rect(state->context, hwnd, &client);
        if (state->ops->pat_blt) {
            state->ops->pat_blt(state->context, dc, 0, 0, client.right,
                TB_BORDER_HEIGHT, 0x42UL);
            state->ops->pat_blt(state->context, dc, 0,
                (StarsI16)(client.bottom - 1), client.right,
                TB_BORDER_HEIGHT, 0x42UL);
            if (state->window_layout == 0)
                state->ops->pat_blt(state->context, dc, 0, 0,
                    TB_BORDER_HEIGHT, client.bottom, 0x42UL);
        }
        if (state->ops->draw_toolbar)
            state->ops->draw_toolbar(state->context, dc, &client);
        if (state->ops->end_paint)
            state->ops->end_paint(state->context, hwnd, dc);
        return 0;
    }

    if (message == STARS_MESSAGE_WM_ERASE_BACKGROUND) {
        if (state->ops->get_client_rect)
            state->ops->get_client_rect(state->context, hwnd, &client);
        if (state->ops->fill_rect)
            state->ops->fill_rect(state->context, wparam, &client,
                state->button_face_brush);
        return 1;
    }
    if (message == STARS_MESSAGE_WM_CONTROL_COLOR) {
        if (state->ops->set_background_color)
            state->ops->set_background_color(state->context, wparam,
                state->button_face_color);
        return state->button_face_brush;
    }
    if (message == STARS_MESSAGE_WM_SETCURSOR) {
        if (state->ops->set_cursor)
            state->ops->set_cursor(state->context, state->hand_cursor);
        return 1;
    }
    if (message == STARS_MESSAGE_WM_COMMAND) {
        if (tb_low_word(lparam) == (StarsI16)state->radar_window &&
                tb_high_word(lparam) == TB_RADAR_COMMAND_NOTIFY &&
                state->ops->post_message)
            state->ops->post_message(state->context, hwnd,
                TB_WM_TOOLBAR_COMMIT, 0, 0UL);
        return 0;
    }
    if (message == TB_WM_TOOLBAR_COMMIT) {
        if (state->ops->terminate_focus)
            state->ops->terminate_focus(state->context, 0);
        return 0;
    }

    if (message == TB_WM_MOUSEMOVE) {
        point.x = tb_low_word(lparam);
        point.y = tb_high_word(lparam);
        if (hwnd != state->toolbar_window && state->ops->map_points)
            state->ops->map_points(state->context, hwnd,
                state->toolbar_window, &point, 1);
        if (point.x == state->last_x && point.y == state->last_y)
            return 0;
        state->last_x = point.x;
        state->last_y = point.y;
        original = point;
        button = stars_itb_from_ppt(&point, state->buttons,
            state->button_count, state->arial8_height);
        if (button < 0 && button != TB_RADAR_ID) {
            if (state->ops->show_tooltip)
                state->ops->show_tooltip(state->context, -1, 0);
            return 0;
        }
        tooltip_id = button == TB_RADAR_ID ? 0x017c :
            (StarsI16)(button + 0x016a);
        button_rect.left = original.x;
        button_rect.top = original.y;
        button_rect.right = (StarsU16)(original.x +
            stars_dx_of_btn(button, state->arial8_height));
        button_rect.bottom = (StarsU16)(original.y + TB_BUTTON_HEIGHT);
        mapped_rect[0].x = (StarsI16)button_rect.left;
        mapped_rect[0].y = (StarsI16)button_rect.top;
        mapped_rect[1].x = (StarsI16)button_rect.right;
        mapped_rect[1].y = (StarsI16)button_rect.bottom;
        if (state->ops->map_points)
            state->ops->map_points(state->context,
                state->toolbar_window, 0, mapped_rect, 2);
        button_rect.left = (StarsU16)mapped_rect[0].x;
        button_rect.top = (StarsU16)mapped_rect[0].y;
        button_rect.right = (StarsU16)mapped_rect[1].x;
        button_rect.bottom = (StarsU16)mapped_rect[1].y;
        if (state->ops->show_tooltip)
            state->ops->show_tooltip(state->context, tooltip_id,
                &button_rect);
        return 0;
    }

    if (message == TB_WM_LBUTTONDOWN ||
            message == TB_WM_LBUTTONDOUBLECLICK) {
        if (state->ops->show_tooltip)
            state->ops->show_tooltip(state->context, -1, 0);
        point.x = tb_low_word(lparam);
        point.y = tb_high_word(lparam);
        original = point;
        button = stars_itb_from_ppt(&point, state->buttons,
            state->button_count, state->arial8_height);
        if (button < 0) return 0;
        down = tb_button_down(state, button);
        if (down != 0 && button <= 5) {
            if (state->ops->message_beep)
                state->ops->message_beep(state->context, 0);
            return 0;
        }
        if (tb_button_is_direct(button)) {
            if (state->ops->execute_button)
                state->ops->execute_button(state->context, button,
                    (StarsI16)(down == 0));
            return 0;
        }
        width = stars_dx_of_btn(button, state->arial8_height);
        button_rect.left = (StarsU16)original.x;
        button_rect.top = (StarsU16)original.y;
        button_rect.right = (StarsU16)(original.x + width);
        button_rect.bottom = (StarsU16)(original.y + TB_BUTTON_HEIGHT);
        dc = state->ops->get_dc ? state->ops->get_dc(state->context, hwnd) : 0;
        if (state->ops->select_palette)
            state->ops->select_palette(state->context, dc, state->palette);
        if (state->ops->realize_palette)
            state->ops->realize_palette(state->context, dc);
        if (state->ops->set_capture)
            state->ops->set_capture(state->context, hwnd);
        old_inside = -1;
        inside = 0;
        while (state->ops->get_mouse_move && state->ops->get_mouse_move(
                state->context, &point)) {
            inside = state->ops->point_in_rect ?
                state->ops->point_in_rect(state->context, &button_rect,
                point) : 0;
            if (inside != old_inside) {
                tb_draw_button(state, dc, original, button,
                    (StarsI16)(down + inside));
                old_inside = inside;
            }
        }
        if (state->ops->release_capture)
            state->ops->release_capture(state->context);
        if (inside != 0 && state->ops->execute_button)
            state->ops->execute_button(state->context, button,
                (StarsI16)(down == 0));
        if (button < 6 && state->ops->draw_toolbar)
            state->ops->draw_toolbar(state->context, dc, &client);
        else tb_draw_button(state, dc, original, button,
            tb_button_down(state, button));
        if (state->ops->release_dc)
            state->ops->release_dc(state->context, hwnd, dc);
        return 0;
    }
    return tb_default(state, hwnd, message, wparam, lparam);
}
