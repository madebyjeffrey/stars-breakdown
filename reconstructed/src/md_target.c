#include "core.h"

/* NE 31:587A-598B. Match a battle token against the broad missile-defense
 * target classes, including the shared class-7 cases for target kinds 4–5. */
StarsI16 stars_is_target_of_md_target(const StarsMdTargetToken *token,
    StarsI16 target)
{
    StarsU16 kind;
    if (!token) return 0;
    kind = (StarsU16)(token->overlap_23 >> 12);
    if (target == 1) return 1;
    if (target == 2) return token->grobj == 1U;
    if (target == 3 || target == 6 || target == 7) return kind == (StarsU16)target;
    if (target == 4) return kind == 4U || kind == 7U;
    if (target == 5) return kind == 5U || kind == 6U || kind == 7U;
    return 0;
}
