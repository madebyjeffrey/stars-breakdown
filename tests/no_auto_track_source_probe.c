#include <stdio.h>
#include "core.h"

int main(void)
{
    StarsAutoTrackOrder orders[3];
    StarsAutoTrackFleet fleets[2];
    StarsNoAutoTrackInput input;
    orders[0].flags = 0; orders[1].flags = 0x0200U;
    orders[1].target_fleet = 7; orders[1].x = 1; orders[1].y = 2;
    orders[2].flags = 0x0300U; orders[2].target_fleet = 7;
    fleets[0].player = 3; fleets[0].present = 1U; fleets[0].cord = 2U;
    fleets[0].orders = orders; fleets[0].order_count = 3U;
    fleets[1].present = 0U;
    input.target_player = 1; input.target_fleet = 7; input.target_x = 40;
    input.target_y = 50; input.fleets = fleets; input.fleet_count = 2U;
    stars_no_auto_track_fleet(&input);
    if ((orders[1].flags & 0x2000U) == 0U || orders[1].x != 40 || orders[1].y != 50)
        return 1;
    if (orders[2].flags != 0x0300U) return 2;
    fleets[0].player = 1; orders[1].flags = 0x0200U;
    stars_no_auto_track_fleet(&input);
    if (orders[1].flags != 0x0200U) return 3;
    puts("NO_AUTO_TRACK_SOURCE=PASS");
    return 0;
}
