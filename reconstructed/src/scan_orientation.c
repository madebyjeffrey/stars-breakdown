/* Genuine C reconstruction of GetScanFleetOrientation (NE 12:978c-987b). */
#include "core.h"

void stars_get_scan_fleet_orientation(const StarsScanFleetOrientationInput *input,
    StarsPoint *point, StarsPoint *point_delta)
{
    StarsI16 dx;
    StarsI16 dy;

    if (!input || !point || !point_delta) return;
    dx = 0;
    dy = 0;
    if (input->fleet_player == input->local_player) {
        if (input->fleet_cord > 1 &&
            (input->own_order_flags & 0x00f0U) != 0U) {
            dx = input->own_order_x;
            dy = input->own_order_y;
        }
    } else if ((input->enemy_flags & 0x0010U) != 0U &&
               (input->enemy_flags & 0x000fU) != 0U) {
        dx = (StarsI16)((input->enemy_position & 0x00ffU) - 0x007fU);
        dy = (StarsI16)((input->enemy_position >> 8) - 0x007fU);
    }
    if (input->orientation)
        input->orientation(input->context, dx, dy, point, point_delta);
}
