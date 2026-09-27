#include "core.h"

/* LinkFleets: clear prior links/seen flags, then insert equal-position
 * fleets after the first fleet at that position. The original's 63-entry
 * sorted search cache is an optimization; scanning prior roots preserves
 * the same circular insertion order without a fixed cache limit. */
void stars_link_fleet_records(StarsLinkFleet **fleets, StarsU16 count,
    StarsU16 *game_flags2)
{
    StarsU16 i, j, used;
    StarsLinkFleet *fleet, *head;
    if (!fleets) return;
    used = 0U;
    while (used < count && fleets[used]) {
        fleets[used]->next = 0;
        fleets[used]->flags = (StarsU16)(fleets[used]->flags & 0xf7ffU);
        ++used;
    }
    for (i = 0U; i < used; ++i) {
        fleet = fleets[i];
        if (fleet->flags & 0x0400U) continue;
        head = 0;
        for (j = 0U; j < i; ++j) {
            if (!(fleets[j]->flags & 0x0400U) &&
                    fleets[j]->x == fleet->x && fleets[j]->y == fleet->y) {
                head = fleets[j];
                break;
            }
        }
        if (head) {
            fleet->next = head->next;
            head->next = fleet;
        } else fleet->next = fleet;
    }
    if (game_flags2) *game_flags2 = (StarsU16)(*game_flags2 | 0x0400U);
}
