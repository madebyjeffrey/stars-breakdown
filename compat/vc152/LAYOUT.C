/* VC8 rejected offsetof in file-scope array bounds in RETURN1. Evaluate every
 * recovered offset at runtime; retain every sizeof/primitive assertion. */
#define STARS_RUNTIME_OFFSETS
#include "PROBE.H"
#include "stypes.h"
#include "sfuncs.h"
#include "sglobals.h"
#include "sconst.h"

/* Additional named checkpoints for three frequently used recovered records. */
STARS_CHECK(probe_planet_size, sizeof(STARS_CV_1002) == 56);
STARS_CHECK(probe_fleet_size, sizeof(STARS_CV_1003) == 124);
STARS_CHECK(probe_player_size, sizeof(STARS_CV_1014) == 192);

void run_layouts(void)
{
    unsigned index;
    index = 0;
    /* Keep one label in DGROUP; the index maps to the ordered checklist. */
#define STARS_OFFSET(name,type,member,expected) \
    check_u32("layout.offset", index++, offsetof(type,member), expected);
#include "soffsets.inc"
#undef STARS_OFFSET
    check_u32("layout.planet.size", 0, sizeof(STARS_CV_1002), 56);
    check_u32("layout.fleet.size", 0, sizeof(STARS_CV_1003), 124);
    check_u32("layout.player.size", 0, sizeof(STARS_CV_1014), 192);
}
