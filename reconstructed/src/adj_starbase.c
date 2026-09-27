#include "core.h"

/* NE 10:474E-489E. Find the previous or next qualifying starbase in the
 * planet table.  The original consults the player's hull definition through
 * a far table; the cargo-capacity callback supplies that resolved field while
 * preserving the original scan order and fallback behavior. */
StarsI16 stars_id_find_adj_starbase(const StarsAdjacentStarbasePlanet *planets,
    StarsU16 count, StarsI16 player, StarsI16 planet_id, StarsI16 next,
    StarsGetHullCargoMax get_cargo, void *context)
{
    StarsI16 first = -1;
    StarsI16 last = -1;
    StarsI16 lower = -1;
    StarsI16 higher = -1;
    StarsU16 i;
    const StarsAdjacentStarbasePlanet *planet;
    if (!planets || !get_cargo) return -1;
    for (i = 0; i < count; ++i) {
        planet = &planets[i];
        if (planet->owner != player) continue;
        if ((planet->flags & 0x0200U) == 0U ||
                get_cargo(context, planet->hull_id) == 0U) continue;
        if (planet->id > planet_id && higher == -1) higher = planet->id;
        else if (planet->id < planet_id) lower = planet->id;
        if (first == -1) first = planet->id;
        last = planet->id;
    }
    if (next) return higher;
    if (lower != -1) return lower;
    return last;
}
