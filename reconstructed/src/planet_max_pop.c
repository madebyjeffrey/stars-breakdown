#include "core.h"

/* NE 10:7096-7247. Calculate the population ceiling from habitability,
 * race modifiers, and the starbase exception.  The original resolves the
 * planet and race records through far globals; this typed input carries those
 * resolved values while retaining the fixed-point arithmetic and modifier
 * order. */
StarsI32 stars_calc_planet_max_pop(const StarsCalcPlanetMaxPopInput *input)
{
    StarsI32 maximum;
    StarsI32 adjustment;
    if (!input) return 0;
    if (input->race_stat == 8) {
        if (input->planet_owner != input->player || !input->has_starbase)
            return 0;
        maximum = input->starbase_population;
    } else {
        if (input->desirability < 5L)
            maximum = 500L;
        else
            maximum = input->desirability * 100L;
        if (input->race_stat == 0) {
            adjustment = maximum / 2L;
            maximum -= adjustment;
        } else if (input->race_stat == 9) {
            adjustment = maximum / 5L;
            maximum += adjustment;
        }
    }
    if (input->race_grbit != 0) {
        adjustment = maximum / 10L;
        maximum += adjustment;
    }
    return maximum;
}
