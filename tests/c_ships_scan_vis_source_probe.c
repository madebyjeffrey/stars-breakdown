#include <stdio.h>
#include "core.h"

static StarsU16 category(void *context, StarsI16 player, StarsU16 design)
{
    (void)context;
    return (StarsU16)((player + design) & 3);
}

int main(void)
{
    StarsShipsScanFleet fleet;
    StarsShipsScanInput input;
    StarsU16 index;
    StarsU32 result;

    fleet.player = 2;
    fleet.cord = 1;
    fleet.order_flags = 2;
    fleet.overlap_flags = 0;
    for (index = 0; index < 16U; ++index) {
        fleet.ship_counts[index] = (StarsU16)(index + 1U);
        fleet.ship_designs[index] = index;
    }
    input.scan_flags = 0;
    input.local_player = 1;
    input.own_ship_mask = 0;
    input.enemy_category_mask = 0;
    input.hull_category = category;
    input.context = 0;
    result = stars_c_ships_scan_vis(&fleet, &input);
    if (result != 136UL)
        return 1;

    input.scan_flags = 0x0100U;
    fleet.overlap_flags = 0;
    if (stars_c_ships_scan_vis(&fleet, &input) != 0UL)
        return 2;
    fleet.overlap_flags = 1;
    input.scan_flags = 0x0900U;
    input.enemy_category_mask = (StarsU16)((1U << 0) | (1U << 2));
    /* Categories (2 + design) & 3: category 0 and category 2 are selected. */
    if (stars_c_ships_scan_vis(&fleet, &input) != 64UL)
        return 3;

    fleet.player = 1;
    fleet.order_flags = 2;
    input.local_player = 1;
    input.scan_flags = 0x0200U;
    input.own_ship_mask = (StarsU16)((1U << 0) | (1U << 3) | (1U << 15));
    if (stars_c_ships_scan_vis(&fleet, &input) != 21UL)
        return 4;
    fleet.order_flags = 6;
    input.scan_flags = 0x0300U;
    if (stars_c_ships_scan_vis(&fleet, &input) != 0UL)
        return 5;
    fleet.order_flags = 2;
    fleet.cord = 2;
    input.scan_flags = 0x0200U;
    if (stars_c_ships_scan_vis(&fleet, &input) != 21UL)
        return 6;
    puts("C_SHIPS_SCAN_VIS_SOURCE=PASS");
    return 0;
}
