#include "core.h"

/* NE 34:50B4-51A8. Report whether the fleet's first waypoint/order is
 * effectively waypoint zero.  The order words are the nine-word order
 * prefix copied by the original routine; this adapter needs the flag word
 * and the first five order words used by its dispatch checks. */
StarsI16 stars_fdest_is_wp0(const StarsFleetWp0Input *input)
{
    StarsU16 i;
    StarsU16 kind;
    if (!input) return 0;
    if (input->cord < 2U) return 1;
    if ((input->order_flags & 0x1000U) == 0U) return 0;
    kind = (StarsU16)(input->order_flags & 0x000fU);
    if (kind == 1U) {
        for (i = 0; i < 5U; ++i)
            if ((input->order_words[i] >> 12) == 6U) return 1;
    } else if (kind == 2U) {
        if (((input->order_flags >> 8) & 0x000fU) == 1U) return 1;
    } else if (kind == 4U || kind == 5U || kind == 6U) {
        return 1;
    }
    return 0;
}
