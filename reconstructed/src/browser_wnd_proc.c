#include "core.h"

enum {
    BROWSER_WM_CREATE = 1,
    BROWSER_WM_PAINT = 0x000f,
    BROWSER_WM_ERASE_BACKGROUND = 0x0014,
    BROWSER_WM_SETCURSOR = 0x0020,
    BROWSER_WM_LBUTTONDOWN = 0x0201,
    BROWSER_COMPONENT_X_MIN = 4,
    BROWSER_COMPONENT_X_MAX = 0x45,
    BROWSER_COMPONENT_Y_MIN = 5,
    BROWSER_COMPONENT_Y_MAX = 0x45,
    BROWSER_BROWSER_HULL = 0x4000,
    BROWSER_STARBASE_HULL = 0x0400,
    BROWSER_POPUP_COMPONENT = 0x000e
};

static StarsI16 browser_low_word(StarsU32 value)
{ return (StarsI16)(StarsU16)value; }

static StarsI16 browser_high_word(StarsU32 value)
{ return (StarsI16)(StarsU16)(value >> 16); }

static StarsI16 browser_component_hit(const StarsBrowserWndInput *input,
    StarsPoint point)
{
    return (input->browser_grhst == BROWSER_BROWSER_HULL ||
            input->browser_grhst == BROWSER_STARBASE_HULL) &&
        input->slot_dialog == 0U &&
        point.x > BROWSER_COMPONENT_X_MIN &&
        point.x < BROWSER_COMPONENT_X_MAX &&
        point.y >= input->arial_height + BROWSER_COMPONENT_Y_MIN &&
        point.y < input->arial_height + BROWSER_COMPONENT_Y_MAX;
}

/* NE 28:2876-2A6F. Paint the browser component, erase it with the standard
 * button face, and open the component popup when the small hit target is
 * clicked.  The original copies the selected hull into shdefBuild and masks
 * component identifiers before Popup; those two stateful operations are
 * explicit callbacks here. */
StarsI32 stars_browser_wnd_proc(StarsBrowserWndInput *input, StarsU16 hwnd,
    StarsU16 message, StarsU16 wparam, StarsU32 lparam)
{
    StarsRect rect;
    StarsPoint point;

    if (!input || !input->ops)
        return 0;
    if (message == BROWSER_WM_CREATE)
        return 0;
    if (message == BROWSER_WM_PAINT) {
        rect.left = rect.top = rect.right = rect.bottom = 0;
        point.x = point.y = 0;
        if (input->ops->begin_paint)
            wparam = input->ops->begin_paint(input->context, hwnd, &rect);
        if (input->ops->get_client_rect)
            input->ops->get_client_rect(input->context, hwnd, &rect);
        if (input->ops->draw_component)
            input->ops->draw_component(input->context, wparam, hwnd,
                rect.left, rect.top);
        if (input->ops->end_paint)
            input->ops->end_paint(input->context, hwnd, &rect);
        return 0;
    }
    if (message == BROWSER_WM_ERASE_BACKGROUND) {
        rect.left = rect.top = rect.right = rect.bottom = 0;
        if (input->ops->get_client_rect)
            input->ops->get_client_rect(input->context, hwnd, &rect);
        if (input->ops->fill_rect)
            input->ops->fill_rect(input->context, wparam, &rect,
                input->button_face_brush);
        return 1;
    }
    if (message == BROWSER_WM_SETCURSOR) {
        if (input->ops->get_cursor_pos)
            input->ops->get_cursor_pos(input->context, &point);
        if (input->ops->screen_to_client)
            input->ops->screen_to_client(input->context, hwnd, &point);
    } else if (message == BROWSER_WM_LBUTTONDOWN) {
        point.x = browser_low_word(lparam);
        point.y = browser_high_word(lparam);
    } else {
        if (input->ops->default_proc)
            return input->ops->default_proc(input->context, hwnd, message,
                wparam, lparam);
        return 0;
    }
    if (!browser_component_hit(input, point)) {
        if (input->ops->default_proc)
            return input->ops->default_proc(input->context, hwnd, message,
                wparam, lparam);
        return 0;
    }
    if (message == BROWSER_WM_LBUTTONDOWN) {
        input->popup_kind = BROWSER_POPUP_COMPONENT;
        if (input->ops->copy_hull)
            input->ops->copy_hull(input->context, input->browser_id,
                input->browser_grhst == BROWSER_STARBASE_HULL);
        if (input->ops->normalize_hull)
            input->ops->normalize_hull(input->context, input->browser_id);
        if (input->ops->popup)
            input->ops->popup(input->context, hwnd, point.x, point.y);
    } else if (input->ops->set_cursor) {
        input->ops->set_cursor(input->context, input->help_cursor);
    }
    return 1;
}
