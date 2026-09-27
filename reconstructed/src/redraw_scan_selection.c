#include "core.h"

/* Genuine C reconstruction of RedrawScanSel, NE 0C:6F30-715B. */
void stars_redraw_scan_selection(StarsU16 hdc, StarsI16 visible,
    StarsRedrawScanInput *input)
{
    StarsScanSelection saved_scan;
    StarsPoint point;
    StarsRect rect;
    StarsU16 dc;
    StarsI16 acquired;
    StarsI16 was_all;
    StarsI16 expand;

    if (!input || !input->selection || input->scanner_window == 0U)
        return;
    saved_scan = *input->selection;
    if (!input->is_visible ||
            !input->is_visible(input->context, input->scanner_window)) return;
    was_all = (StarsI16)(visible == -1);
    if (was_all) visible = 0;
    acquired = 0;
    dc = hdc;
    if (dc == 0U) {
        acquired = 1;
        dc = input->get_dc ? input->get_dc(input->context,
            input->scanner_window) : 0;
    }
    if (input->draw_ship_path)
        input->draw_ship_path(input->context, dc, visible);
    if (visible == 0) {
        input->selection->waypoint = -1;
        input->selection->fleet = -1;
        input->selection->planet = -1;
        input->selected_id = -1;
        input->selected_full = 0;
        input->selected_kind = 0;
        input->selection->grobj_full = 0;
        input->selection->grobj = 0;
    }
    if (input->scan_zoom < 3 || (input->scan_flags & 0x000fU) != 4U) {
        expand = (input->scan_flags & 0x1000U) != 0U ? 7 : 0;
    } else {
        expand = 11;
    }
    if (saved_scan.grobj != 0 && !was_all) {
        point = input->selected_point;
        if (input->logical_to_scan)
            input->logical_to_scan(input->context, &point);
        rect.left = (StarsU16)(point.x - 11 - expand);
        rect.top = (StarsU16)(point.y - 11 - expand);
        rect.right = (StarsU16)(point.x + 12 + expand);
        rect.bottom = (StarsU16)(point.y + 23 + expand);
        if (input->draw_scanner)
            input->draw_scanner(input->context, dc, &rect);
    }
    if (saved_scan.grobj != 0 &&
            (saved_scan.x != input->selected_point.x ||
                saved_scan.y != input->selected_point.y)) {
        point.x = saved_scan.x;
        point.y = saved_scan.y;
        if (input->logical_to_scan)
            input->logical_to_scan(input->context, &point);
        rect.left = (StarsU16)(point.x - 6 - expand);
        rect.top = (StarsU16)(point.y - 6 - expand);
        rect.right = (StarsU16)(point.x + 7 + expand);
        rect.bottom = (StarsU16)(point.y + 15 + expand);
        if (input->draw_scanner)
            input->draw_scanner(input->context, dc, &rect);
    }
    if (visible == 0)
        *input->selection = saved_scan;
    if (acquired && input->release_dc)
        input->release_dc(input->context, input->scanner_window, dc);
}
