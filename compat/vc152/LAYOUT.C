/* Compile only: generated declarations must not become unresolved link inputs. */
#include "stypes.h"
#include "sfuncs.h"
#include "sglobals.h"
#include "sconst.h"

/* Additional named checkpoints for three frequently used recovered records. */
STARS_CHECK(probe_planet_size, sizeof(STARS_CV_1002) == 56);
STARS_CHECK(probe_planet_owner, offsetof(STARS_CV_1002, iPlayer.value) == 2);
STARS_CHECK(probe_fleet_size, sizeof(STARS_CV_1003) == 124);
STARS_CHECK(probe_fleet_cargo, offsetof(STARS_CV_1003, rgwtMin.value) == 76);
STARS_CHECK(probe_player_size, sizeof(STARS_CV_1014) == 192);
STARS_CHECK(probe_player_attr, offsetof(STARS_CV_1014, rgAttr.value) == 62);

int recovered_layouts_compile(void)
{
    return sizeof(STARS_CV_1002) + sizeof(STARS_CV_1003) + sizeof(STARS_CV_1014);
}
