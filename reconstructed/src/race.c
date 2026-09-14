#include "core.h"
/* NE 29:30D2-30F4. Near offsets wrap at 16 bits; CBW sign-extends the byte.
 * No bounds check is present in the original. Caller validity is separate. */
StarsI16 stars_get_race_stat(const StarsU8 *dgroup, StarsU16 player, StarsI16 index)
{
    StarsU16 address;
    StarsU8 value;
    address = (StarsU16)(player + (StarsU16)index + 62U);
    value = dgroup[address];
    return value < 128U ? (StarsI16)value : (StarsI16)((int)value - 256);
}
