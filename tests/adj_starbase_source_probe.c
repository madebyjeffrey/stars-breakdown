#include <stdio.h>
#include "core.h"

static StarsU16 cargo(void *context, StarsU16 hull_id)
{
    (void)context;
    return hull_id == 2U ? 0U : 1U;
}

int main(void)
{
    static const StarsAdjacentStarbasePlanet planets[] = {
        { 2, 3, 0x0200U, 1U },
        { 5, 3, 0x0200U, 2U },
        { 7, 3, 0x0200U, 3U },
        { 9, 4, 0x0200U, 1U }
    };
    if (stars_id_find_adj_starbase(planets, 4U, 3, 5, 0, cargo, 0) != 2 ||
            stars_id_find_adj_starbase(planets, 4U, 3, 5, 1, cargo, 0) != 7 ||
            stars_id_find_adj_starbase(planets, 4U, 3, 1, 0, cargo, 0) != 7 ||
            stars_id_find_adj_starbase(planets, 4U, 3, 7, 1, cargo, 0) != -1)
        return 1;
    puts("ADJ_STARBASE_SOURCE=PASS");
    return 0;
}
