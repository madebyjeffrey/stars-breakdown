#include "core.h"
#include <string.h>

static StarsI16 change_scan_same(const StarsScanSelection *a,
    const StarsWayPointScan *b)
{
    return a->x == b->point.x && a->y == b->point.y &&
        a->grobj == (StarsI16)b->grobj &&
        a->grobj_full == (StarsI16)b->grobj_full &&
        a->planet == b->idpl && a->fleet == b->ifl &&
        a->waypoint == b->iwp && a->thing == b->ith;
}

static StarsI16 change_scan_special(const StarsChangeScanSelInput *input,
    const StarsScanSelection *selection, StarsPoint *point)
{
    if (selection->grobj != 8 || !input->get_thing)
        return 0;
    return input->get_thing(input->context, selection->thing, point);
}

static void change_scan_special_rect(const StarsChangeScanSelInput *input,
    const StarsPoint *origin, StarsRect *rect)
{
    StarsPoint first;
    StarsPoint second;

    first.x = (StarsI16)(origin->x - (StarsI16)input->special_radius);
    first.y = (StarsI16)(origin->y - (StarsI16)input->special_radius);
    second.x = (StarsI16)(origin->x + (StarsI16)input->special_radius);
    second.y = (StarsI16)(origin->y + (StarsI16)input->special_radius);
    if (input->logical_to_scan) {
        input->logical_to_scan(input->context, &first);
        input->logical_to_scan(input->context, &second);
    }
    rect->left = (StarsU16)(first.x - 1);
    rect->top = (StarsU16)(first.y - 1);
    rect->right = (StarsU16)(second.x + 1);
    rect->bottom = (StarsU16)(second.y + 1);
}

/* NE 0C:8CC4-909B.  Change the scanner selection only when its complete
 * eight-word value changes, preserving redraw and order-window side effects. */
void stars_change_scan_selection(StarsChangeScanSelInput *input,
    StarsWayPointScan *scan, StarsI16 valid_scan)
{
    StarsScanSelection previous;
    StarsPoint old_thing;
    StarsPoint new_thing;
    StarsRect rect;
    StarsU16 dc;
    StarsI16 active_changed;
    StarsI16 old_special;
    StarsI16 new_special;

    if (!input || !input->selection || !scan)
        return;
    if (!valid_scan && input->find_nearest) {
        input->find_nearest(input->context, scan->point,
            scan->grobj, scan);
    }
    if (change_scan_same(input->selection, scan))
        return;
    previous = *input->selection;
    active_changed = (StarsI16)(scan->iwp != -1 &&
        scan->iwp != input->active_waypoint);
    old_special = change_scan_special(input, &previous, &old_thing);
    if (input->redraw_selection)
        input->redraw_selection(input->context, 0, -1);
    input->selection->x = scan->point.x;
    input->selection->y = scan->point.y;
    input->selection->grobj = (StarsI16)scan->grobj;
    input->selection->grobj_full = (StarsI16)scan->grobj_full;
    input->selection->planet = scan->idpl;
    input->selection->fleet = scan->ifl;
    input->selection->waypoint = scan->iwp;
    input->selection->thing = scan->ith;
    if ((input->selection->grobj_full & 1) != 0 && valid_scan != 2)
        input->selection->grobj = 1;
    if (active_changed) {
        input->active_waypoint = scan->iwp;
        if (input->fill_orders)
            input->fill_orders(input->context);
        if (input->set_order_row)
            input->set_order_row(input->context, scan->iwp);
        if (input->update_orders)
            input->update_orders(input->context, 0);
        if (input->draw_ship)
            input->draw_ship(input->context, 0, 0x122U);
    }
    if (input->redraw_selection)
        input->redraw_selection(input->context, 0, 1);
    if (active_changed && input->ensure_point)
        input->ensure_point(input->context, scan->point, 1);
    if (input->draw_scroll_bar)
        input->draw_scroll_bar(input->context, 0);
    if (input->set_title)
        input->set_title(input->context, input->mine_window);
    if (old_special) {
        change_scan_special_rect(input, &old_thing, &rect);
        dc = input->get_dc ? input->get_dc(input->context,
            input->scanner_window) : 0;
        if (input->draw_scanner)
            input->draw_scanner(input->context, dc, &rect);
        if (input->release_dc)
            input->release_dc(input->context, input->scanner_window, dc);
    }
    new_special = change_scan_special(input, input->selection, &new_thing);
    if (new_special) {
        change_scan_special_rect(input, &new_thing, &rect);
        dc = input->get_dc ? input->get_dc(input->context,
            input->scanner_window) : 0;
        if (input->draw_scanner)
            input->draw_scanner(input->context, dc, &rect);
        if (input->release_dc)
            input->release_dc(input->context, input->scanner_window, dc);
    }
    if (input->selection->planet != -1 && input->draw_ship)
        input->draw_ship(input->context, 0, 0x4002U);
    if ((input->tutorial_flags & 0x0800U) != 0U && input->player == 0U &&
            input->advance_tutor)
        input->advance_tutor(input->context);
}
