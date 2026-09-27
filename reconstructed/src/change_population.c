#include "common.h"

/* NE 8:7082-75E1, ChgPopFromPlanet. Fraction is hundredths of one
 * colonist unit and is carried into the integer population update. */
StarsI32 stars_change_population(StarsPopulationState *state,
    StarsI32 maximum, StarsI16 true_growth, StarsI16 dead_half,
    StarsI16 update)
{
    StarsI32 change;
    StarsI32 hundredths;
    StarsI32 remainder;
    StarsI32 fraction;
    StarsI32 factor;
    StarsI32 fullness;
    StarsI32 retard;
    if (!state || state->owner < 0 || state->population <= 0 || maximum <= 0)
        return 0;
    fraction = state->fraction;
    if (state->desirability < 0) {
        hundredths = (StarsI32)(((double)state->population *
            (double)(-state->desirability)) / 10.0);
        if (hundredths < 1) hundredths = 1;
        change = hundredths / 100L;
        remainder = hundredths % 100L;
        fraction -= remainder;
        if (fraction < 0) {
            ++change;
            fraction += 100L;
        }
        change = -change;
        if (update) {
            state->fraction = (StarsU8)fraction;
            state->population += change;
        }
        return change;
    }
    factor = (StarsI32)true_growth * state->desirability;
    if (dead_half) factor >>= 1;
    if (maximum / 4L < state->population) {
        if (state->population < maximum) {
            fullness = (StarsI32)(((double)state->population * 1000.0) /
                (double)maximum);
            retard = 1000L - fullness;
            if (factor < 1000L)
                factor = (StarsI32)(((double)factor * retard * retard) /
                    562500.0);
            else
                factor = (StarsI32)(((double)(factor / 10L) * retard *
                    retard) / 562500.0) * 10L;
        } else if (state->population < maximum + 10L) {
            return 0;
        } else {
            fullness = (StarsI32)(((double)state->population * 1000.0) /
                (double)maximum);
            retard = 99L - fullness / 10L;
            if (retard < -300L) retard = -300L;
            factor = retard * 4L;
        }
    }
    hundredths = (StarsI32)(((double)state->population * factor) / 100.0);
    change = hundredths / 100L;
    remainder = hundredths % 100L;
    if (change == 0 && remainder == 0) remainder = 1;
    fraction += remainder;
    if (fraction >= 100L) {
        ++change;
        fraction -= 100L;
    } else if (fraction < 0) {
        --change;
        fraction += 100L;
    }
    if (update) {
        state->fraction = (StarsU8)fraction;
        state->population += change;
    }
    return change;
}

