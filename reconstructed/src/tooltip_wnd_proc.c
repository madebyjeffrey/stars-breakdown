#include "core.h"

enum {
    TOOLTIP_WM_CREATE = 1,
    TOOLTIP_WM_DESTROY = 2,
    TOOLTIP_WM_PAINT = 0x000f,
    TOOLTIP_WM_ERASE_BACKGROUND = 0x0014,
    TOOLTIP_WM_MOUSE_MOVE = 0x0200,
    TOOLTIP_WM_LBUTTON_DOWN = 0x0201,
    TOOLTIP_WM_RBUTTON_DOWN = 0x0203,
    TOOLTIP_WM_TIMER = 0x0113,
    TOOLTIP_WM_SHOW = 0x05f3,
    TOOLTIP_TIMER_ID = 0x039e,
    TOOLTIP_INITIAL_DELAY = 700,
    TOOLTIP_REPEAT_DELAY = 50,
    TOOLTIP_VISIBLE_LIMIT = 10000,
    TOOLTIP_BORDER = 3,
    TOOLTIP_POSITION_FLAGS = 0x0251
};

static void tooltip_kill_timer(StarsTooltipWndInput *input)
{
    if (input->timer_id != -1 && input->ops.kill_timer)
        input->ops.kill_timer(input->ops.context, input->tooltip_window,
            (StarsU16)input->timer_id, 0U);
    input->timer_id = -1;
}

static void tooltip_destroy(StarsTooltipWndInput *input)
{
    if (input->ops.destroy_window)
        input->ops.destroy_window(input->ops.context, input->tooltip_window);
}

static void tooltip_schedule(StarsTooltipWndInput *input, StarsU16 delay)
{
    StarsI16 timer;

    tooltip_kill_timer(input);
    timer = input->ops.set_timer ? input->ops.set_timer(input->ops.context,
        input->tooltip_window, TOOLTIP_TIMER_ID, delay) : 0;
    input->timer_id = timer == 0 ? -1 : TOOLTIP_TIMER_ID;
}

/* Genuine semantic reconstruction of TooltipWndProc, NE 14:19E4-1D72.
 * The custom show message starts the delayed display; the timer then keeps a
 * visible tooltip alive while the cursor remains over its source rectangle,
 * repositions it near the cursor, and removes it after the ten-second hover
 * limit or when the pointer leaves.  Paint and erase handling are kept as
 * ordinary typed drawing operations so the source explains the UI behavior
 * instead of replaying the original instruction bytes. */
StarsI32 stars_tooltip_wnd_proc(StarsTooltipWndInput *input, StarsU16 hwnd,
    StarsU16 message, StarsU16 wparam, StarsU32 lparam)
{
    StarsRect paint;
    StarsRect client;
    StarsPoint point;
    StarsU16 dc;
    StarsU16 old_font;
    StarsI16 old_mode;
    StarsI16 length;
    StarsI16 visible;
    StarsI16 inside;
    StarsU32 now;
    StarsU16 timer;
    StarsU16 delay;
    StarsI32 result;
    StarsU8 text[80];

    if (!input)
        return 0L;
    result = 0L;
    if (message == TOOLTIP_WM_CREATE) {
        input->tooltip_window = hwnd;
        return result;
    }
    if (message == TOOLTIP_WM_DESTROY) {
        input->tooltip_window = 0U;
        tooltip_kill_timer(input);
        input->tooltip_id = -1;
        return result;
    }

    if (message == TOOLTIP_WM_PAINT) {
        if (input->ops.begin_paint)
            dc = input->ops.begin_paint(input->ops.context, hwnd, &paint);
        else
            dc = 0U;
        if (input->ops.get_client_rect)
            input->ops.get_client_rect(input->ops.context, hwnd, &client);
        else {
            client.left = client.top = client.right = client.bottom = 0U;
        }
        if (input->ops.frame_rect)
            input->ops.frame_rect(input->ops.context, dc, &client,
                input->window_frame_brush);
        old_font = input->ops.select_object ?
            input->ops.select_object(input->ops.context, dc, input->font) : 0U;
        length = input->ops.get_string ? input->ops.get_string(
            input->ops.context, input->tooltip_id, text) : 0;
        old_mode = input->ops.set_background_mode ?
            input->ops.set_background_mode(input->ops.context, dc, 1) : 1;
        if (input->ops.set_text_color)
            input->ops.set_text_color(input->ops.context, dc,
                input->text_color);
        if (input->ops.text_out)
            input->ops.text_out(input->ops.context, dc, TOOLTIP_BORDER,
                TOOLTIP_BORDER, text, length);
        if (input->ops.set_background_mode)
            input->ops.set_background_mode(input->ops.context, dc, old_mode);
        if (input->ops.select_object)
            input->ops.select_object(input->ops.context, dc, old_font);
        if (input->ops.end_paint)
            input->ops.end_paint(input->ops.context, hwnd, &paint);
        return result;
    }
    if (message == TOOLTIP_WM_ERASE_BACKGROUND) {
        if (input->ops.get_client_rect)
            input->ops.get_client_rect(input->ops.context, hwnd, &client);
        if (input->ops.fill_rect)
            input->ops.fill_rect(input->ops.context, wparam, &client,
                input->tooltip_brush);
        return 1L;
    }
    if (message == TOOLTIP_WM_MOUSE_MOVE ||
            message == TOOLTIP_WM_LBUTTON_DOWN ||
            message == TOOLTIP_WM_RBUTTON_DOWN) {
        tooltip_destroy(input);
        return result;
    }

    timer = TOOLTIP_TIMER_ID;
    delay = TOOLTIP_INITIAL_DELAY;
    if (message == TOOLTIP_WM_SHOW && wparam == 0U) {
        tooltip_schedule(input, delay);
        return result;
    }
    if (message == TOOLTIP_WM_SHOW)
        wparam = timer;
    if (wparam != timer)
        goto default_processing;

    visible = message == TOOLTIP_WM_TIMER && input->ops.is_visible ?
        input->ops.is_visible(input->ops.context, hwnd) : 0;
    if (message == TOOLTIP_WM_TIMER && visible) {
        now = input->ops.get_tick ? input->ops.get_tick(input->ops.context) :
            input->last_visible_tick;
        input->last_visible_tick = now;
        point.x = point.y = 0;
        if (input->ops.get_cursor_pos)
            input->ops.get_cursor_pos(input->ops.context, &point);
        inside = input->ops.point_in_rect ? input->ops.point_in_rect(
            input->ops.context, &input->tooltip_rect, point) : 0;
        if (!inside || input->first_visible_tick + TOOLTIP_VISIBLE_LIMIT < now)
            tooltip_destroy(input);
        return result;
    }

    now = input->ops.get_tick ? input->ops.get_tick(input->ops.context) : 0UL;
    input->first_visible_tick = now;
    point.x = point.y = 0;
    if (input->ops.get_cursor_pos)
        input->ops.get_cursor_pos(input->ops.context, &point);
    inside = input->ops.point_in_rect ? input->ops.point_in_rect(
        input->ops.context, &input->tooltip_rect, point) : 0;
    if (!inside) {
        tooltip_destroy(input);
        return result;
    }
    if (input->ops.screen_to_client)
        input->ops.screen_to_client(input->ops.context, input->frame_window,
            &point);
    if (input->screen_width < point.x + input->tip_width)
        point.x = (StarsI16)(input->screen_width - input->tip_width - 5);
    if (input->ops.client_to_screen)
        input->ops.client_to_screen(input->ops.context, input->frame_window,
            &point);
    if (input->ops.set_window_pos)
        input->ops.set_window_pos(input->ops.context, hwnd, 0xffffU, point.x,
            (StarsI16)((input->font_height * 3) / 2 + point.y), 0, 0,
            TOOLTIP_POSITION_FLAGS);
    if (input->ops.update_window)
        input->ops.update_window(input->ops.context, hwnd);
    delay = TOOLTIP_REPEAT_DELAY;
    tooltip_schedule(input, delay);
    return result;

default_processing:
    if (input->ops.default_proc)
        return input->ops.default_proc(input->ops.context, hwnd, message,
            wparam, lparam);
    return result;
}
