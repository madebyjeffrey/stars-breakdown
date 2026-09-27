/* Genuine C reconstruction of FOtherStuffAtScanSel (NE 6:4d6e-4ecc). */
#include "core.h"
StarsI16 stars_other_stuff_at_scan_selection(const StarsMineScanSelection *s,
    const StarsMinePoint *fleets, StarsU16 fleet_count,
    const StarsMinePoint *things, StarsU16 thing_count)
{
    StarsU16 i; StarsU16 matches;
    if (s->planet!=-1 && (s->fleet!=-1 || s->thing!=-1)) return 1;
    if (s->fleet!=-1 && s->thing!=-1) return 1;
    if (s->fleet!=-1) {
        matches=0;
        for(i=0;i<fleet_count;++i) {
            if (!fleets[i].present) break;
            if (fleets[i].x==s->point.x && fleets[i].y==s->point.y) {
                if (matches!=0) return 1; ++matches;
            }
        }
    }
    if (s->thing!=-1) {
        matches=0;
        for(i=0;i<thing_count;++i) if(things[i].x==s->point.x && things[i].y==s->point.y) {
            if(matches!=0)return 1; ++matches;
        }
    }
    return 0;
}
