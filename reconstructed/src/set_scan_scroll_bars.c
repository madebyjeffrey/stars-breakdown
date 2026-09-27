#include "core.h"

static StarsI16 scan_round_up_4(StarsI32 value)
{
    return (StarsI16)((value + 3L) & 0xfffcL);
}

/* NE 12:6C14-6D2F. Recompute the two scanner scrollbar ranges and the page
 * increment from the client rectangle and galaxy invariant. */
void stars_set_scan_scroll_bars(StarsU16 hwnd,
    StarsSetScanScrollBarsInput *input)
{
    StarsI16 scan_right;
    StarsI16 scan_bottom;
    StarsI16 horizontal;
    StarsI16 vertical;
    if (!input || !input->scan_to_point || !input->set_scroll_range)
        return;
    input->in_scroll_set = 1;
    scan_right = input->scan_to_point(input->context,
        input->client_right);
    scan_bottom = input->scan_to_point(input->context,
        input->client_bottom - input->dy_sbar);
    horizontal = scan_round_up_4((StarsI32)input->d_gal_inv - 1000L -
        scan_right);
    if (horizontal < 1000) horizontal = 1000;
    vertical = scan_round_up_4((StarsI32)input->d_gal_inv - 1000L -
        scan_bottom);
    if (vertical < 1000) vertical = 1000;
    input->set_scroll_range(input->context, hwnd, 0U,
        (StarsU16)horizontal, 1000U, 1U);
    if (input->in_scroll_set) {
        input->set_scroll_range(input->context, hwnd, 1U,
            (StarsU16)vertical, 1000U, 1U);
        if (input->in_scroll_set) {
            if (scan_bottom <= scan_right) scan_right = scan_bottom;
            input->d_scan_page = (StarsI16)((scan_right / 3) & 0xfffc);
            input->d_scan_inc = (StarsI16)((input->d_scan_page / 8 + 2) &
                0xfffc);
            input->in_scroll_set = 0;
        }
    }
}
