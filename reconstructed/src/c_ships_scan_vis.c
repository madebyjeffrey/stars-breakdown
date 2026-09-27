#include "core.h"

#define SHIP_SLOT_COUNT 16U
#define SCAN_VISIBILITY_GATE 0x0100U
#define SCAN_OWN_SHIP_MASK 0x0200U
#define SCAN_ENEMY_CATEGORY_MASK 0x0800U
#define SCAN_ORDER_MASK 0x000fU
#define SCAN_OVERLAP_MASK 0x000fU
#define FLEET_AT_PLANET 1U
#define ORDER_PATROL 3U
#define ORDER_REMOTE 5U
#define ORDER_STARBASE 6U
#define ORDER_WORMHOLE 7U

static StarsU32 ships_scan_sum_all(const StarsShipsScanFleet *fleet)
{
    StarsU16 index;
    StarsU32 total;

    total = 0UL;
    for (index = 0; index < SHIP_SLOT_COUNT; ++index)
        total += fleet->ship_counts[index];
    return total;
}

/* Genuine C reconstruction of CShipsScanVis, NE 0C:4BF4-4E7B. */
StarsU32 stars_c_ships_scan_vis(const StarsShipsScanFleet *fleet,
    const StarsShipsScanInput *input)
{
    StarsU16 index;
    StarsU16 category;
    StarsU16 selected;
    StarsU32 total;

    if (!fleet || !input)
        return 0UL;
    if ((input->scan_flags & SCAN_VISIBILITY_GATE) != 0U) {
        if (fleet->player == input->local_player &&
            (fleet->cord != FLEET_AT_PLANET ||
             (fleet->order_flags & SCAN_ORDER_MASK) == ORDER_STARBASE ||
             (fleet->order_flags & SCAN_ORDER_MASK) == ORDER_REMOTE ||
             (fleet->order_flags & SCAN_ORDER_MASK) == ORDER_PATROL ||
             (fleet->order_flags & SCAN_ORDER_MASK) == ORDER_WORMHOLE))
            return 0UL;
        if (fleet->player != input->local_player &&
            (fleet->overlap_flags & SCAN_OVERLAP_MASK) == 0U)
            return 0UL;
    }
    if ((input->scan_flags & SCAN_OWN_SHIP_MASK) == 0U ||
        fleet->player != input->local_player) {
        if ((input->scan_flags & SCAN_ENEMY_CATEGORY_MASK) == 0U ||
            fleet->player == input->local_player)
            return ships_scan_sum_all(fleet);
        total = 0UL;
        selected = input->enemy_category_mask;
        for (category = 0; selected != 0U; ++category,
             selected = (StarsU16)(selected >> 1)) {
            if ((selected & 1U) != 0U) {
                for (index = 0; index < SHIP_SLOT_COUNT; ++index) {
                    if (fleet->ship_counts[index] != 0U &&
                        input->hull_category != 0 &&
                        input->hull_category(input->context, fleet->player,
                            fleet->ship_designs[index]) == category)
                        total += fleet->ship_counts[index];
                }
            }
        }
        return total;
    }
    total = 0UL;
    selected = input->own_ship_mask;
    for (index = 0; selected != 0U; ++index,
         selected = (StarsU16)(selected >> 1)) {
        if ((selected & 1U) != 0U && fleet->ship_counts[index] != 0U)
            total += fleet->ship_counts[index];
    }
    return total;
}
