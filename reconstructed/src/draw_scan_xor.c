#include "core.h"

/* NE 12:8AF6-8C59. Restrict clipping around the scanner route, draw its XOR
 * segments, and restore the selected pen, raster mode, and full clip region. */
void stars_draw_scan_xor_lines(StarsDrawScanXorLinesInput *input)
{
    StarsRect client;
    StarsI16 count;
    StarsU16 pen;
    StarsU16 old_pen;
    StarsU16 old_rop;
    StarsI16 i;

    if (!input || !input->get_client_rect || !input->exclude_clip_rect ||
        !input->select_object || !input->set_rop2 || !input->move_to ||
        !input->line_to || !input->select_clip_region)
        return;
    input->get_client_rect(input->context, input->scanner_window, &client);
    input->exclude_clip_rect(input->context, input->hdc, 0,
        (StarsI16)(client.bottom - input->scrollbar_height),
        (StarsI16)client.right, (StarsI16)client.bottom);
    count = input->point_count;
    if (count == 4 && input->points[2].x == input->points[3].x &&
        input->points[2].y == input->points[3].y) {
        count = 3;
        pen = input->get_stock_object ?
            input->get_stock_object(input->context, 6U) : 0U;
    } else pen = input->ship_pen;
    for (i = 1; i < count; ++i)
        input->exclude_clip_rect(input->context, input->hdc,
            (StarsI16)(input->points[i].x - 5),
            (StarsI16)(input->points[i].y - 5),
            (StarsI16)(input->points[i].x + 6),
            (StarsI16)(input->points[i].y + 6));
    old_pen = input->select_object(input->context, input->hdc, pen);
    old_rop = input->set_rop2(input->context, input->hdc, 7U);
    input->move_to(input->context, input->hdc, input->points[2].x,
        input->points[2].y);
    input->line_to(input->context, input->hdc, input->points[0].x,
        input->points[0].y);
    if (count > 3)
        input->line_to(input->context, input->hdc, input->points[3].x,
            input->points[3].y);
    input->set_rop2(input->context, input->hdc, old_rop);
    input->select_object(input->context, input->hdc, old_pen);
    input->select_clip_region(input->context, input->hdc, input->huge_region);
}
