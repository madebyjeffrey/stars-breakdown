#include "core.h"

/* Standalone FAttack probes do not exercise design-to-hull lookup. The
 * battle integration probe links the production stars_lpshdef_from_tok. */
const StarsU8 *stars_lpshdef_from_tok(const StarsToken *token,
    StarsLookupTokenHull lookup, void *context)
{
    (void)token;
    (void)lookup;
    (void)context;
    return (const StarsU8 *)0;
}

const StarsU8 *stars_lpshdef_from_tok_globals(const StarsToken *token,
    const StarsU8 *dgroup, StarsResolveFarHull resolve, void *context)
{
    (void)token;
    (void)dgroup;
    (void)resolve;
    (void)context;
    return (const StarsU8 *)0;
}
