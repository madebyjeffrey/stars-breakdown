#include "core.h"

/* NE 12:715C-7277. Keep a scan point inside the visible rectangle, or center
 * the scanner on the nearest adjusted point when scrolling is requested. */
StarsI16 stars_ensure_point_on_screen(StarsPoint point, StarsI16 scroll,
    const StarsEnsurePointOnScreenInput *input)
{
    StarsI16 scan_right;
    StarsI16 scan_bottom;
    StarsRect visible;
    StarsPoint adjusted;
    if (!input || !input->scan_to_point || !input->point_in_rect ||
        !input->center_point) return 0;
    scan_right = input->scan_to_point(input->context, input->client_right);
    scan_bottom = input->scan_to_point(input->context,
        input->client_bottom - input->dy_sbar);
    visible.left = (StarsI16)(input->x_scan_top + 10);
    visible.right = (StarsI16)(input->x_scan_top + scan_right - 20);
    visible.bottom = (StarsI16)(input->d_gal_inv - input->y_scan_top - 10);
    visible.top = (StarsI16)(visible.bottom - scan_bottom + 20);
    if (input->point_in_rect(input->context, visible, point)) return 1;
    adjusted.x = (StarsI16)(input->x_scan_top + scan_right / 2);
    adjusted.y = (StarsI16)(input->d_gal_inv - input->y_scan_top -
        scan_bottom / 2);
    if (point.x < visible.left)
        adjusted.x = (StarsI16)(adjusted.x - (visible.left - point.x));
    else if (visible.right < point.x)
        adjusted.x = (StarsI16)(adjusted.x + (point.x - visible.right));
    if (point.y < visible.top)
        adjusted.y = (StarsI16)(adjusted.y - (visible.top - point.y));
    else if (visible.bottom < point.y)
        adjusted.y = (StarsI16)(adjusted.y + (point.y - visible.bottom));
    input->center_point(input->context, adjusted, scroll);
    return 0;
}
