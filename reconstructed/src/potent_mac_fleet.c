#include "core.h"

/* NE 21:42EC-441A. Compute the Macinti war-fleet equivalence and apply the
 * threshold rule that conditionally counts eligible heavy designs twice. */
StarsI16 stars_potent_mac_war_fleet(const StarsPotentMacFleetInput *input,
    StarsI16 *equivalence)
{
    StarsI16 value = 0;
    StarsU16 i;
    if (!input) return 0;
    for (i = 2U; i <= 4U; ++i) value += input->ship_counts[i];
    for (i = 5U; i <= 7U; ++i) value += input->ship_counts[i] * 2;
    if (value < input->potency_threshold) {
        for (i = 8U; i < 10U; ++i)
            if (input->ship_counts[i] && !input->designs[i].ai_controlled &&
                input->designs[i].hull_type == 9U)
                value += input->ship_counts[i] * 2;
        if (value <= input->potency_threshold) return 0;
    }
    if (equivalence) *equivalence = value;
    return 1;
}
