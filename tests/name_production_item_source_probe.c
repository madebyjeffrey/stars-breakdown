#include <stdio.h>
#include <string.h>
#include "core.h"

static void get_string(void *context, StarsU16 id, StarsU8 *output)
{
    (void)context;
    if (id == 0x7eU) strcpy((char *)output, "mines");
    else if (id == 0x51bU) strcpy((char *)output, "custom");
    else strcpy((char *)output, "wrong");
}

static StarsU32 packed(StarsU16 kind, StarsU16 item)
{
    return ((StarsU32)kind << 17) | ((StarsU32)item << 10);
}

int main(void)
{
    static const StarsU8 base_name[] = "Scout";
    static const StarsU8 player_name[] = "Starbase";
    static const StarsU8 planet_name[] = "Barren";
    static const StarsU8 base_suffix[] = " base";
    static const StarsU8 ship_suffix[] = " hull";
    const StarsU8 *base_names[16] = {base_name};
    const StarsU8 *player_names[16] = {player_name};
    const StarsU8 *planet_names[9] = {planet_name};
    StarsU16 base_flags[16] = {0};
    StarsU16 player_flags[16] = {0};
    StarsI16 player_hulls[16] = {4};
    StarsProductionNameInput input;
    StarsU8 output[64];
    memset(&input, 0, sizeof(input));
    input.base_ship_names = base_names;
    input.player_ship_names = player_names;
    input.base_ship_flags = base_flags;
    input.player_ship_flags = player_flags;
    input.player_ship_hulls = player_hulls;
    input.planetary_names = planet_names;
    input.starbase_suffix = base_suffix;
    input.ship_suffix = ship_suffix;
    input.get_string = get_string;
    input.output = output;

    input.packed = packed(2U, 0U);
    if (strcmp((char *)stars_name_production_item(&input), "Scout") != 0)
        return 1;
    base_flags[0] = 0x200U;
    if (stars_name_production_item(&input)[0] != 0)
        return 1;
    base_flags[0] = 0;
    input.packed = packed(2U, 0x10U);
    input.selected_planet_flags = 0x200U;
    input.selected_starbase_hull = 6;
    if (strcmp((char *)stars_name_production_item(&input), "Starbase base") != 0)
        return 1;
    input.selected_starbase_hull = 2;
    if (strcmp((char *)stars_name_production_item(&input), "Starbase hull") != 0)
        return 1;
    input.packed = packed(0U, 0x12U);
    if (strcmp((char *)stars_name_production_item(&input), "Barren") != 0)
        return 1;
    input.packed = packed(0U, 0U);
    if (strcmp((char *)stars_name_production_item(&input), "mines") != 0)
        return 1;
    input.packed = packed(0U, 0x1bU);
    if (strcmp((char *)stars_name_production_item(&input), "custom") != 0)
        return 1;
    puts("NAME_PRODUCTION_ITEM_SOURCE=PASS");
    return 0;
}
