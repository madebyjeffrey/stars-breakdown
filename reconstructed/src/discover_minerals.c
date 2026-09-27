#include "core.h"

/* NE 24:5E0C-5EFA. With the original random gate, choose a planet and—after
 * turn nine—announce and raise one mineral concentration. */
void stars_discover_new_minerals(StarsDiscoverMineralsInput *input)
{
    StarsI16 planet;
    StarsI16 mineral;
    StarsI16 increase;
    StarsDiscoverMineralPlanet *p;
    if (!input || !input->random || !input->planets || !input->planet_count)
        return;
    if (input->random(input->context, (StarsI16)(15 - input->mineral_size)) != 0)
        return;
    planet = input->random(input->context, (StarsI16)input->planet_count);
    if (planet < 0 || planet >= (StarsI16)input->planet_count) return;
    p = &input->planets[planet];
    if (input->turn > 9) {
        mineral = input->random(input->context, 3);
        if (p->player != -1 && input->message)
            input->message(input->context, p->player, p->id, p->id, mineral);
        if (mineral >= 0 && mineral < 3 && p->concentration[mineral] < 0xb4U) {
            increase = input->random(input->context, 15);
            p->concentration[mineral] = (StarsU8)(p->concentration[mineral] +
                (StarsI16)(signed char)increase + 5);
        }
    }
}
