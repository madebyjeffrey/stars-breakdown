#include "core.h"

#define CREATE_SALVAGE_MINERALS 3
#define CREATE_SALVAGE_COMPONENT_DIVISOR 3L
#define CREATE_SALVAGE_PLANET_DIVISOR 10L
#define CREATE_SALVAGE_PLANET_NORMAL_RATE 5L
#define CREATE_SALVAGE_PLANET_DEPLETED_RATE 8L

/* NE 1F:7EE8-81D3. Convert the destroyed fleet's hull costs and carried
 * minerals into salvage.  A destruction over a planet returns the scaled
 * amount directly to that planet; destruction in space keeps half.  The
 * callbacks represent race-cost resolution, planet storage, and DropSalvage. */
void stars_create_salvage(StarsCreateSalvageInput *input)
{
    StarsU32 salvage[CREATE_SALVAGE_MINERALS];
    StarsI16 race_grbit;
    StarsI16 design;
    StarsI16 mineral;
    StarsI32 cost;
    StarsI32 amount;
    StarsI32 rate;
    StarsI16 has_salvage;

    if (!input || !input->fleet)
        return;
    race_grbit = input->ops.race_grbit ?
        input->ops.race_grbit(input->context, input->fleet->player, 12) : 0;
    for (mineral = 0; mineral < CREATE_SALVAGE_MINERALS; ++mineral)
        salvage[mineral] = input->fleet->cargo[mineral];

    for (mineral = 0; mineral < CREATE_SALVAGE_MINERALS; ++mineral) {
        for (design = 0; design < 16; ++design) {
            if (input->fleet->ship_counts[design] == 0U ||
                    !input->ops.ore_cost)
                continue;
            cost = input->ops.ore_cost(input->context, input->fleet->player,
                design, mineral, race_grbit != 0);
            amount = (StarsI32)input->fleet->ship_counts[design] * cost;
            salvage[mineral] = (StarsU32)(salvage[mineral] +
                amount / CREATE_SALVAGE_COMPONENT_DIVISOR);
        }
        if (input->planet) {
            rate = input->planet_depleted ?
                CREATE_SALVAGE_PLANET_DEPLETED_RATE :
                CREATE_SALVAGE_PLANET_NORMAL_RATE;
            amount = (StarsI32)(salvage[mineral] * rate /
                CREATE_SALVAGE_PLANET_DIVISOR);
            if (input->ops.add_planet_mineral)
                input->ops.add_planet_mineral(input->context, input->planet,
                    mineral, amount);
        } else {
            salvage[mineral] = (StarsU32)(salvage[mineral] -
                salvage[mineral] / 2UL);
        }
    }
    has_salvage = 0;
    for (mineral = 0; mineral < CREATE_SALVAGE_MINERALS; ++mineral)
        if (salvage[mineral] != 0UL)
            has_salvage = 1;
    if (!input->planet && has_salvage && input->ops.drop_salvage)
        input->ops.drop_salvage(input->context, salvage,
            input->fleet->player, input->fleet->x, input->fleet->y);
}
