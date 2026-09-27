/* Genuine C reconstruction of FShouldPartBeHidden (NE 28:68c4-6a55). */
#include "core.h"

StarsI16 stars_should_part_be_hidden(StarsI16 local_player, StarsU16 group,
    StarsU16 subtype, StarsU16 trader_flags)
{
    StarsU16 required;

    if (local_player == -1) return 0;
    required = 0U;
    if (group == 1U && subtype == 8U) required = 0x0200U;
    else if (group == 4U && subtype == 6U) required = 4U;
    else if (group == 8U && subtype == 9U) required = 8U;
    else if (group == 0x10U && subtype == 0x12U) required = 0x80U;
    else if (group == 0x20U && subtype == 7U) required = 0x40U;
    else if (group == 0x40U && subtype == 8U) required = 0x20U;
    else if (group == 0x80U && subtype == 6U) required = 0x10U;
    else if (group == 0x800U && subtype == 4U) required = 2U;
    else if (group == 0x1000U && subtype == 4U) required = 1U;
    else if (group == 0x1000U && subtype == 9U) required = 0x0800U;
    else if (group == 0x4000U && subtype == 0x1eU) required = 0x0100U;
    else if (group == 0x8000U && subtype == 0x0eU) required = 0x0400U;
    if (required == 0U || (trader_flags & required) != 0U) return 0;
    return 1;
}
