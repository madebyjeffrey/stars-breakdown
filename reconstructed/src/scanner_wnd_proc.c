#include "core.h"

/* NE 12:0032-0EFB. Route scanner-window messages through typed services.
 * Win16 handles, object tables, and GDI state live behind callbacks; the
 * message ordering, zoom/scroll arithmetic, hit-test boundaries, and object
 * selection policy remain here as ordinary C. */

static StarsI16 scanner_point_in(const StarsScannerWndState *state,
    const StarsRect *rect, StarsPoint point)
{
    if (state->ops && state->ops->point_in_rect)
        return state->ops->point_in_rect(state->context, rect, point);
    return point.x >= (StarsI16)rect->left && point.x < (StarsI16)rect->right &&
        point.y >= (StarsI16)rect->top && point.y < (StarsI16)rect->bottom;
}

static void scanner_post_zoom(StarsScannerWndState *state, StarsI16 zoom)
{
    if (state->ops && state->ops->post_command)
        state->ops->post_command(state->context, state->frame_window,
            (StarsI16)(zoom + 0x0f41));
}

static StarsPoint scanner_lparam_point(StarsU32 lparam)
{
    StarsPoint point;
    point.x = (StarsI16)(lparam & 0xffffU);
    point.y = (StarsI16)((lparam >> 16) & 0xffffU);
    return point;
}

static StarsI16 scanner_key_down(const StarsScannerWndState *state,
    StarsU16 key)
{
    if (!state->ops || !state->ops->get_async_key_state) return 0;
    return (state->ops->get_async_key_state(state->context, key) & 0xfffeU) != 0;
}

static void scanner_draw_crosshair(StarsScannerWndState *state)
{
    StarsPoint point;
    if (!state->ops || !state->ops->draw_crosshair) return;
    point = state->selected_point;
    if (state->ops->logical_to_scan)
        state->ops->logical_to_scan(state->context, &point);
    state->ops->draw_crosshair(state->context, state->scanner_window, point);
}

static void scanner_scroll(StarsScannerWndState *state, StarsU16 vertical,
    StarsI16 command, StarsU32 lparam)
{
    StarsI16 old_pos;
    StarsI16 new_pos;
    StarsI16 delta;
    StarsPoint difference;

    if (command == 0) delta = (StarsI16)-state->scan_increment;
    else if (command == 1) delta = state->scan_increment;
    else if (command == 2) delta = (StarsI16)-state->scan_page;
    else if (command == 3) delta = state->scan_page;
    else if (command == 4 || command == 5) {
        old_pos = vertical ? state->y_scan_top : state->x_scan_top;
        delta = (StarsI16)((((StarsU16)(lparam & 0xffffU) -
            (StarsU16)old_pos) & 0xfffcU));
    } else delta = 0;
    if (delta == 0) return;

    old_pos = vertical ? state->y_scan_top : state->x_scan_top;
    new_pos = (StarsI16)(old_pos + delta);
    if (state->ops && state->ops->set_scroll_pos)
        state->ops->set_scroll_pos(state->context, state->scanner_window,
            vertical ? 1 : 0, new_pos);
    if (state->ops && state->ops->get_scroll_pos)
        new_pos = state->ops->get_scroll_pos(state->context,
            state->scanner_window, vertical ? 1 : 0);
    if (vertical) state->y_scan_top = new_pos;
    else state->x_scan_top = new_pos;

    difference.x = 0; difference.y = 0;
    if (vertical) difference.y = (StarsI16)(old_pos - new_pos);
    else difference.x = (StarsI16)(old_pos - new_pos);
    if (state->ops && state->ops->logical_to_scan)
        state->ops->logical_to_scan(state->context, &difference);
    if (state->ops && state->ops->scroll_scanner)
        state->ops->scroll_scanner(state->context, difference.x, difference.y);
}

static StarsI16 scanner_kind_id(const StarsScanSelection *selection)
{
    if (selection->grobj == 1) return selection->planet;
    if (selection->grobj == 2) return selection->fleet;
    if (selection->grobj == 8) return selection->thing;
    return -1;
}

static StarsI16 scanner_accept_object(const StarsScannerWndState *state,
    const StarsScanSelection *selection, StarsI16 owner)
{
    StarsI16 id;
    id = scanner_kind_id(selection);
    if (id < 0) return 0;
    if (!state->ops || !state->ops->can_select) return selection->grobj == 1 ||
        selection->grobj == 2;
    return state->ops->can_select(state->context, selection->grobj, id,
        owner);
}

static StarsI32 scanner_mouse(StarsScannerWndState *state, StarsU16 msg,
    StarsU16 wparam, StarsU32 lparam)
{
    StarsRect rect;
    StarsPoint point;
    StarsScanSelection nearest;
    StarsScannerPopupChoice choice;
    StarsI16 radius;
    StarsI16 owner;
    StarsI16 same_scan;
    StarsI16 same_object;
    StarsI16 kind;
    StarsI16 id;

    if (state->ops && state->ops->set_focus)
        state->ops->set_focus(state->context, state->frame_window);
    point = scanner_lparam_point(lparam);
    if (state->ops && state->ops->get_client_rect)
        state->ops->get_client_rect(state->context, state->scanner_window, &rect);
    else {
        rect.left = 0; rect.top = 0; rect.right = 0xffffU; rect.bottom = 0xffffU;
    }
    if (point.y >= (StarsI16)rect.bottom - state->scrollbar_height) {
        if (msg == STARS_SCANNER_WM_LBUTTONDOWN && state->selection.grobj_full & 3U &&
            state->ops && state->ops->popup_below)
            state->ops->popup_below(state->context, state->scanner_window,
                point, &state->selection);
        return 0;
    }

    if (state->ops && state->ops->scan_to_logical)
        state->ops->scan_to_logical(state->context, &point);
    nearest = state->selection;
    radius = ((state->behavior_flags & (STARS_SCANNER_BEHAVIOR_WARP |
        STARS_SCANNER_BEHAVIOR_ROUTE)) == 0) ? 0x0f : 1;
    if (state->ops && state->ops->find_nearest)
        state->ops->find_nearest(state->context, point, radius, &nearest);

    if ((msg == STARS_SCANNER_WM_MBUTTONDOWN) ||
        (msg == STARS_SCANNER_WM_RBUTTONDOWN &&
         (wparam & STARS_SCANNER_MOUSE_CONTROL) != 0)) {
        if (state->ops && state->ops->measuring_tape)
            state->ops->measuring_tape(state->context, &nearest, point);
        return 0;
    }

    if (msg == STARS_SCANNER_WM_RBUTTONDOWN) {
        choice.kind = -1; choice.id = -1; choice.owner = -1;
        if (!state->ops || !state->ops->popup_menu ||
            state->ops->popup_menu(state->context, state->scanner_window, point,
                &nearest, &choice) < 0) return 0;
        if (state->ops->change_scan_selection)
            state->ops->change_scan_selection(state->context, &nearest, 2);
        state->selection = nearest;
        if (choice.kind == 1 && choice.owner >= 0 &&
            choice.owner != state->current_player) return 0;
        return 0;
    }

    if (state->selected_kind == 2 &&
        ((wparam & STARS_SCANNER_MOUSE_CONTROL) != 0 ||
         (state->scan_flags & STARS_SCANNER_SCAN_ADD) != 0)) {
        if (state->ops && state->ops->add_waypoint)
            state->ops->add_waypoint(state->context, point, &nearest);
        return 0;
    }

    same_scan = nearest.x == state->selected_point.x &&
        nearest.y == state->selected_point.y;
    if (state->ops && state->ops->change_scan_selection)
        state->ops->change_scan_selection(state->context, &nearest, 1);
    state->selection = nearest;
    if (state->ops && state->ops->near_waypoint &&
        state->ops->near_waypoint(state->context, point, 1) != 0 &&
        state->ops->drag_waypoint &&
        state->ops->drag_waypoint(state->context, point) != 0) return 0;
    if (!same_scan) return 0;

    kind = nearest.grobj;
    id = scanner_kind_id(&nearest);
    owner = -1;
    if (state->ops && state->ops->can_select)
        owner = state->ops->can_select(state->context, kind, id, -2);
    same_object = state->current_object_point.x == nearest.x &&
        state->current_object_point.y == nearest.y;
    if (same_object && state->ops && state->ops->next_object_here &&
        state->ops->next_object_here(state->context, &nearest, 1) == 0) return 0;
    if (kind == 1 && (wparam & STARS_SCANNER_MOUSE_CONTROL) != 0 &&
        state->ops && state->ops->toggle_planet_route) {
        state->ops->toggle_planet_route(state->context, id, 0);
        return 0;
    }
    if (!scanner_accept_object(state, &nearest, owner)) return 0;
    if (state->ops && state->ops->redraw_scan_selection)
        state->ops->redraw_scan_selection(state->context, 0, 0);
    if (state->ops && state->ops->change_main_object)
        state->ops->change_main_object(state->context, kind, id);
    if (state->ops && state->ops->redraw_scan_selection)
        state->ops->redraw_scan_selection(state->context, 0, 1);
    return 0;
}

static StarsI32 scanner_cursor(StarsScannerWndState *state)
{
    StarsRect rect;
    StarsRect scan_rect;
    StarsPoint point;
    StarsU16 cursor;
    StarsI16 near_waypoint;
    StarsI16 add_cursor;

    if (!state->ops || !state->ops->get_cursor_pos) return 0;
    state->ops->get_cursor_pos(state->context, &point);
    if (state->ops->screen_to_client)
        state->ops->screen_to_client(state->context, state->scanner_window, &point);
    if (state->ops->get_client_rect)
        state->ops->get_client_rect(state->context, state->scanner_window, &rect);
    else return 0;
    if (!scanner_point_in(state, &rect, point)) return 0;
    scan_rect = rect;
    scan_rect.bottom = (StarsU16)((StarsI16)scan_rect.bottom -
        state->scrollbar_height);
    if (!scanner_point_in(state, &scan_rect, point)) {
        cursor = state->ops->load_cursor ? state->ops->load_cursor(state->context,
            STARS_SCANNER_CURSOR_SCROLLBAR) : STARS_SCANNER_CURSOR_SCROLLBAR;
        if (state->ops->set_cursor) state->ops->set_cursor(state->context, cursor);
        return 1;
    }
    add_cursor = state->selected_kind == 2 &&
        (scanner_key_down(state, 0x10) ||
         (state->scan_flags & STARS_SCANNER_SCAN_ADD) != 0);
    if (add_cursor) cursor = STARS_SCANNER_CURSOR_ADD;
    else {
        near_waypoint = state->ops->near_waypoint ?
            state->ops->near_waypoint(state->context, point, 0) : 0;
        if (near_waypoint) cursor = STARS_SCANNER_CURSOR_OPEN_GRAB;
        else if ((state->behavior_flags & (STARS_SCANNER_BEHAVIOR_WARP |
            STARS_SCANNER_BEHAVIOR_ROUTE)) == 0 &&
            (state->selected_kind != 1 ||
             ((!scanner_key_down(state, 0x10) ||
               !state->ops->warp_from_planet ||
               state->ops->warp_from_planet(state->context) == 0) &&
              !scanner_key_down(state, 0x11))))
            cursor = STARS_SCANNER_CURSOR_SCANNER;
        else cursor = STARS_SCANNER_CURSOR_ADD;
    }
    if (state->ops->set_cursor) state->ops->set_cursor(state->context, cursor);
    return 1;
}

StarsI32 stars_scanner_wnd_proc(StarsScannerWndState *state, StarsU16 msg,
    StarsU16 wparam, StarsU16 unused, StarsU32 lparam)
{
    StarsRect paint_rect;
    StarsI16 desired_zoom;

    (void)unused;
    if (!state) return 0;
    if (msg == STARS_SCANNER_WM_CREATE) {
        state->x_scan_top = 1000; state->y_scan_top = 1000;
        return 0;
    }
    if (msg == STARS_SCANNER_WM_SIZE) {
        if (state->ops && state->ops->post_command)
            state->ops->post_command(state->context, state->frame_window,
                (StarsI16)(state->zoom + 0x0f41));
        return 0;
    }
    if (msg == STARS_SCANNER_WM_PAINT) {
        StarsU16 dc = 0;
        if (state->ops && state->ops->begin_paint)
            dc = state->ops->begin_paint(state->context, state->scanner_window,
                &paint_rect);
        if (state->fleets_present && !state->drawing_busy && state->ops &&
            state->ops->get_client_rect) {
            state->ops->get_client_rect(state->context, state->scanner_window,
                &paint_rect);
            if (state->ops->draw_scrollbar)
                state->ops->draw_scrollbar(state->context, dc, &paint_rect);
            paint_rect.bottom = (StarsU16)((StarsI16)paint_rect.bottom -
                state->scrollbar_height);
            if (state->ops->draw_scanner)
                state->ops->draw_scanner(state->context, dc, &paint_rect);
        }
        if (state->ops && state->ops->end_paint)
            state->ops->end_paint(state->context, state->scanner_window, dc,
                &paint_rect);
        return 0;
    }
    if (msg == STARS_SCANNER_WM_SETCURSOR) return scanner_cursor(state);
    if (msg == STARS_SCANNER_WM_CHAR) {
        if (wparam == STARS_SCANNER_KEY_V_LOWER ||
            wparam == STARS_SCANNER_KEY_V_UPPER) scanner_draw_crosshair(state);
        else {
            desired_zoom = wparam == STARS_SCANNER_KEY_MINUS ?
                (StarsI16)(state->zoom - 1) : (StarsI16)(state->zoom + 1);
            if (desired_zoom < -4) desired_zoom = -4;
            if (desired_zoom > 4) desired_zoom = 4;
            if (desired_zoom != state->zoom) scanner_post_zoom(state, desired_zoom);
        }
        return 0;
    }
    if (msg == STARS_SCANNER_WM_HSCROLL || msg == STARS_SCANNER_WM_VSCROLL) {
        scanner_scroll(state, msg == STARS_SCANNER_WM_VSCROLL, wparam, lparam);
        return 0;
    }
    if (msg == STARS_SCANNER_WM_LBUTTONDOWN ||
        msg == STARS_SCANNER_WM_LBUTTONDOUBLECLICK ||
        msg == STARS_SCANNER_WM_RBUTTONDOWN ||
        msg == STARS_SCANNER_WM_MBUTTONDOWN)
        return scanner_mouse(state, msg, wparam, lparam);
    if (msg == STARS_SCANNER_WM_ACTIVATEAPP) {
        state->active_window = wparam ? state->scanner_window : 0;
        return 0;
    }
    if (state->ops && state->ops->default_proc)
        return state->ops->default_proc(state->context, state->scanner_window,
            msg, wparam, lparam);
    return 0;
}
