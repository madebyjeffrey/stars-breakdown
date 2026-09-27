#include "core.h"
#include <string.h>

enum {
    MINE_WM_CREATE = 1,
    MINE_WM_PAINT = 0x000f,
    MINE_WM_ERASE_BACKGROUND = 0x0014,
    MINE_WM_CONTROL_COLOR = 0x0019,
    MINE_WM_SETCURSOR = 0x0020,
    MINE_WM_COMMAND = 0x0111,
    MINE_WM_LBUTTONDOWN = 0x0201,
    MINE_WM_LBUTTONDOUBLECLICK = 0x0203,
    MINE_WM_RBUTTONDOWN = 0x0204,
    MINE_WM_SETFONT = 0x0030,
    MINE_BUTTON_QUERY = 0x0400,
    MINE_TITLE_STRING = 0x04bd,
    MINE_TITLE_MARGIN = 4,
    MINE_TITLE_RIGHT_MARGIN = 8,
    MINE_TITLE_TEXT_MARGIN = 0x18,
    MINE_CURSOR_ARROW_HIT = 9,
    MINE_MEMORY_RECORD = 0x2b,
    MINE_MEMORY_SIZE = 4
};

#define MINE_BUTTON_STYLE 0x40000003UL

static StarsI16 mine_low_word(StarsU32 value)
{
    return (StarsI16)(StarsU16)value;
}

static StarsI16 mine_high_word(StarsU32 value)
{
    return (StarsI16)(StarsU16)(value >> 16);
}

static StarsI16 mine_text_length(const StarsU8 *text)
{
    return (StarsI16)strlen((const char *)text);
}

static StarsI16 mine_title_length(const StarsMineWndInput *input,
    StarsU16 dc, StarsI16 available)
{
    StarsI16 length;
    StarsI32 extent;

    length = mine_text_length(input->title);
    while (length > 0) {
        extent = input->ops->text_extent ? input->ops->text_extent(
            input->context, dc, input->title, length) : 0L;
        if ((StarsU16)extent <= (StarsU16)available)
            break;
        --length;
    }
    return length;
}

static StarsI32 mine_default(const StarsMineWndInput *input, StarsU16 hwnd,
    StarsU16 message, StarsU16 wparam, StarsU32 lparam)
{
    if (input->ops->default_proc)
        return input->ops->default_proc(input->context, hwnd, message,
            wparam, lparam);
    return 0L;
}

/* Genuine semantic reconstruction of MineWndProc, NE 6:0000-0409.
 * Creation adds the mineral-window checkbox and updates its title.  Painting
 * draws the framed, width-limited title, selection arrow, and survey.  The
 * remaining messages implement the Win16 background, cursor hit testing,
 * checkbox command, and popup-safe mouse dispatch. */
StarsI32 stars_mine_wnd_proc(StarsMineWndInput *input, StarsU16 hwnd,
    StarsU16 message, StarsU16 wparam, StarsU32 lparam)
{
    StarsRect client;
    StarsRect title_rect;
    StarsRect arrow_rect;
    StarsPoint point;
    StarsU16 dc;
    StarsU32 old_text_color;
    StarsU32 old_background_color;
    StarsI16 length;
    StarsI16 hit;
    StarsI16 x;
    StarsI16 y;
    StarsI32 check;
    StarsU8 class_name[64];
    StarsU8 button_title[128];

    if (!input || !input->ops || !input->title)
        return 0L;

    if (message == MINE_WM_CREATE) {
        class_name[0] = 0;
        button_title[0] = 0;
        if (input->ops->get_text) {
            input->ops->get_text(input->context, 0xffffU, class_name,
                (StarsU16)sizeof class_name);
            input->ops->get_text(input->context, MINE_TITLE_STRING,
                button_title, (StarsU16)sizeof button_title);
        }
        if (input->ops->create_window)
            input->combo_window = input->ops->create_window(input->context,
                input->button_class ? input->button_class : class_name,
                button_title, MINE_BUTTON_STYLE, 100, 100, 0x96,
                input->font_height, hwnd, 0, input->instance);
        if (input->ops->send_message)
            input->ops->send_message(input->context, input->combo_window,
                MINE_WM_SETFONT, input->font, 0UL);
        if (input->ops->set_title_bar)
            input->ops->set_title_bar(input->context, hwnd);
        return 0L;
    }

    if (message == MINE_WM_PAINT) {
        client.left = client.top = client.right = client.bottom = 0;
        title_rect.left = title_rect.top = title_rect.right =
            title_rect.bottom = 0;
        dc = input->ops->begin_paint ? input->ops->begin_paint(
            input->context, hwnd, &client) : 0;
        if (input->ops->get_client_rect)
            input->ops->get_client_rect(input->context, hwnd, &client);
        title_rect.left = MINE_TITLE_MARGIN;
        title_rect.top = MINE_TITLE_MARGIN;
        title_rect.right = (StarsU16)(client.right - MINE_TITLE_MARGIN);
        title_rect.bottom = (StarsU16)(input->font_height * 2 -
            MINE_TITLE_MARGIN);
        if (input->ops->draw_frame)
            input->ops->draw_frame(input->context, dc, &title_rect, 0);
        old_text_color = input->ops->set_text_color ?
            input->ops->set_text_color(input->context, dc,
                input->text_color) : 0UL;
        old_background_color = input->ops->set_background_color ?
            input->ops->set_background_color(input->context, dc,
                input->background_color) : 0UL;
        length = mine_title_length(input, dc, (StarsI16)(title_rect.right -
            title_rect.left - MINE_TITLE_TEXT_MARGIN));
        title_rect.right = (StarsU16)(title_rect.right -
            MINE_TITLE_RIGHT_MARGIN);
        if (input->ops->center_text)
            input->ops->center_text(input->context, dc, &title_rect,
                input->title, length);
        title_rect.right = (StarsU16)(title_rect.right +
            MINE_TITLE_RIGHT_MARGIN);
        if (input->ops->set_text_color)
            input->ops->set_text_color(input->context, dc, old_text_color);
        if (input->ops->set_background_color)
            input->ops->set_background_color(input->context, dc,
                old_background_color);
        arrow_rect.left = (StarsU16)(title_rect.right -
            (title_rect.bottom - title_rect.top) + 2);
        arrow_rect.top = (StarsU16)(title_rect.top + 3);
        arrow_rect.right = (StarsU16)(title_rect.right - 4);
        arrow_rect.bottom = (StarsU16)(title_rect.bottom - 2);
        if (input->ops->draw_arrow)
            input->ops->draw_arrow(input->context, dc, &arrow_rect,
                input->ops->other_at_selection ?
                input->ops->other_at_selection(input->context) : 0);
        client.top = (StarsU16)(client.top + input->font_height * 2 -
            MINE_TITLE_MARGIN);
        if (input->ops->draw_survey)
            input->ops->draw_survey(input->context, dc, &client);
        if (input->ops->end_paint)
            input->ops->end_paint(input->context, hwnd, &client);
        return 0L;
    }

    if (message == MINE_WM_ERASE_BACKGROUND) {
        client.left = client.top = client.right = client.bottom = 0;
        if (input->ops->get_client_rect)
            input->ops->get_client_rect(input->context, hwnd, &client);
        if (input->ops->fill_rect)
            input->ops->fill_rect(input->context, wparam, &client,
                input->button_face_brush);
        return 1L;
    }
    if (message == MINE_WM_CONTROL_COLOR) {
        if (input->ops->set_background_color)
            input->ops->set_background_color(input->context, wparam,
                input->background_color);
        return (StarsI32)input->button_face_brush;
    }
    if (message == MINE_WM_SETCURSOR) {
        point.x = point.y = 0;
        if (input->ops->get_cursor_pos)
            input->ops->get_cursor_pos(input->context, &point);
        if (input->ops->screen_to_client)
            input->ops->screen_to_client(input->context, hwnd, &point);
        client.left = client.top = client.right = client.bottom = 0;
        if (input->ops->get_client_rect)
            input->ops->get_client_rect(input->context, hwnd, &client);
        if (input->ops->point_in_rect && input->ops->point_in_rect(
                input->context, &client, point)) {
            hit = input->ops->hit_test ? input->ops->hit_test(input->context,
                hwnd, point.x, point.y) : 0;
            if (hit != 0) {
                if (input->ops->set_cursor)
                    input->ops->set_cursor(input->context,
                        hit == MINE_CURSOR_ARROW_HIT ? input->hand_cursor :
                        input->arrow_help_cursor);
                return 1L;
            }
        }
        return mine_default(input, hwnd, message, wparam, lparam);
    }
    if (message == MINE_WM_COMMAND) {
        if (mine_low_word(lparam) == (StarsI16)input->combo_window &&
                mine_high_word(lparam) == 0 && input->ops->send_message) {
            check = input->ops->send_message(input->context,
                input->combo_window, MINE_BUTTON_QUERY, 0, 0UL);
            if (input->selected_thing && input->ops->write_mem) {
                input->ops->write_mem(input->context, MINE_MEMORY_RECORD,
                    MINE_MEMORY_SIZE, input->selected_thing->overlap_0);
                input->selected_thing->cargo_value = (StarsU8)check;
            }
        }
        return 0L;
    }
    if (message == MINE_WM_LBUTTONDOWN ||
            message == MINE_WM_LBUTTONDOUBLECLICK ||
            message == MINE_WM_RBUTTONDOWN) {
        if (input->popup_window == 0 && input->ops->click) {
            x = mine_low_word(lparam);
            y = mine_high_word(lparam);
            input->ops->click(input->context, x, y, message, wparam);
        }
        return 0L;
    }
    return mine_default(input, hwnd, message, wparam, lparam);
}
