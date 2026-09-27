#include "core.h"

/* Genuine C reconstruction of CtrPointScan, NE 0C:7278-744D. */
void stars_ctr_point_scan(StarsCtrPointScanInput *input)
{
    StarsRect client;
    StarsI16 scan_width;
    StarsI16 scan_height;
    StarsI16 x;
    StarsI16 y;
    StarsI16 old_x;
    StarsI16 old_y;
    StarsI16 dx;
    StarsI16 dy;
    StarsI16 limit;

    if (!input || input->scanner_window == 0U)
        return;
    client.left = client.top = client.right = client.bottom = 0;
    if (input->get_client_rect)
        input->get_client_rect(input->context, input->scanner_window, &client);
    if (input->scan_to_point) {
        scan_width = input->scan_to_point(input->context, (StarsI16)client.right);
        scan_height = input->scan_to_point(input->context,
            (StarsI16)client.bottom - input->dy_sbar);
    } else {
        scan_width = (StarsI16)client.right;
        scan_height = (StarsI16)client.bottom - input->dy_sbar;
    }
    x = (StarsI16)(input->point.x - scan_width / 2);
    if (x <= 1000) x = 1000;
    limit = (StarsI16)(input->gal_size - 1000 - scan_width);
    if (limit < x) x = limit;
    y = (StarsI16)(scan_height / 2 + input->point.y);
    limit = (StarsI16)(input->gal_size - 1000);
    if (y >= limit) y = limit;
    if (y <= (StarsI16)(scan_height + 1000))
        y = (StarsI16)(scan_height + 1000);
    y = (StarsI16)(input->gal_size - y);
    if (x <= 1000) x = 1000;
    if (y <= 1000) y = 1000;
    old_x = input->x_scan_top;
    old_y = input->y_scan_top;
    x = (StarsI16)((x + 2) & (StarsI16)~3);
    y = (StarsI16)((y + 2) & (StarsI16)~3);
    if (old_x == x && old_y == y)
        return;
    input->x_scan_top = x;
    if (input->set_scroll_pos)
        input->set_scroll_pos(input->context, input->scanner_window, 0, x, 1);
    input->y_scan_top = y;
    if (input->set_scroll_pos)
        input->set_scroll_pos(input->context, input->scanner_window, 1, y, 1);
    if (input->scroll == 0) {
        if (input->invalidate)
            input->invalidate(input->context, input->scanner_window, 0);
    } else {
        dx = input->point_to_scan ? input->point_to_scan(input->context,
            (StarsI16)(old_x - x)) : (StarsI16)(old_x - x);
        dy = input->point_to_scan ? input->point_to_scan(input->context,
            (StarsI16)(old_y - y)) : (StarsI16)(old_y - y);
        if (input->scroll_scanner)
            input->scroll_scanner(input->context, dx, dy);
    }
}
