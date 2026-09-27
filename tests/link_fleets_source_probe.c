#include "core.h"
#include <string.h>
#include <stdio.h>

int main(void)
{
    StarsLinkFleet fleets[145];
    StarsLinkFleet *items[145];
    StarsU16 flags = 0x0120U;
    StarsU16 i;
    memset(fleets, 0, sizeof fleets);
    for (i = 0U; i < 145U; ++i) {
        items[i] = &fleets[i];
        fleets[i].x = (StarsI16)i;
        fleets[i].y = -10;
        fleets[i].flags = 0x1807U;
        fleets[i].next = &fleets[144];
    }
    /* Cross the original 63-root cache boundary twice. */
    for (i = 70U; i < 140U; ++i) fleets[i].x = (StarsI16)(i - 70U);
    fleets[140].x = 0;
    fleets[141].x = 0; fleets[141].flags |= 0x0400U;
    fleets[142].x = 0; fleets[142].y = -9;
    stars_link_fleet_records(items, 143U, &flags);
    if (flags != 0x0520U || fleets[0].next != &fleets[140] ||
            fleets[140].next != &fleets[70] || fleets[70].next != &fleets[0]) return 1;
    for (i = 1U; i < 70U; ++i)
        if (fleets[i].next != &fleets[i + 70U] ||
                fleets[i + 70U].next != &fleets[i]) return 2;
    if (fleets[141].next || fleets[141].flags != 0x1407U ||
            fleets[142].next != &fleets[142]) return 3;
    for (i = 0U; i < 141U; ++i)
        if (fleets[i].flags != 0x1007U) return 4;
    /* Rebuild after a fleet moves; no old links may survive. */
    fleets[70].x = 300;
    stars_link_fleet_records(items, 143U, &flags);
    if (fleets[70].next != &fleets[70] || fleets[0].next != &fleets[140] ||
            fleets[140].next != &fleets[0]) return 5;
    puts("LINK_FLEETS_SOURCE=PASS");
    return 0;
}
