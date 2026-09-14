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

/* Original CS:30F4-3114. Signed-byte limits for the sixteen race statistics. */
static const StarsI16 stat_min[16] = {7,5,5,5,5,2,5,0,0,0,0,0,0,0,0,0};
static const StarsI16 stat_max[16] = {25,15,25,25,25,15,25,6,2,2,2,2,2,2,9,0};

/* NE 29:3114-3175. This setter does not set the bounds-correction flag. */
StarsI16 stars_set_race_stat(StarsU8 *data, StarsU16 player, StarsI16 index, StarsI16 value)
{
    if (value < stat_min[index]) value = stat_min[index];
    if (value > stat_max[index]) value = stat_max[index];
    data[(StarsU16)(player + 62U + (StarsU16)index)] = (StarsU8)value;
    return value;
}

static StarsU32 race_flags(const StarsU8 *data, StarsU16 player)
{
    StarsU32 value = 0;
    StarsU16 i;
    for (i = 0; i < 4U; ++i)
        value |= (StarsU32)data[(StarsU16)(player + 78U + i)] << (i * 8U);
    return value;
}

static StarsU32 trait_mask(StarsI16 index)
{
    StarsU16 shift = (StarsU16)((StarsU16)index & 255U);
    /* Original CRT clears CH, then shifts DX:AX one bit CL times. */
    return shift < 32U ? (StarsU32)1 << shift : (StarsU32)0;
}

/* NE 29:3176-31B8. */
StarsI16 stars_get_race_trait(const StarsU8 *data, StarsU16 player, StarsI16 index)
{
    return (race_flags(data, player) & trait_mask(index)) ? 1 : 0;
}

/* NE 29:31B8-3209. */
void stars_set_race_trait(StarsU8 *data, StarsU16 player, StarsI16 index, StarsI16 enabled)
{
    StarsU32 value = race_flags(data, player), mask = trait_mask(index);
    StarsU16 i;
    value = enabled ? value | mask : value & ~mask;
    for (i = 0; i < 4U; ++i)
        data[(StarsU16)(player + 78U + i)] = (StarsU8)(value >> (i * 8U));
}

/* NE 29:5888-58D3. XOR of all 96 little-endian words in the player record. */
StarsU16 stars_race_checksum(const StarsU8 *data, StarsU16 player)
{
    StarsU16 result = 0, i, address;
    for (i = 0; i < 96U; ++i) {
        address = (StarsU16)(player + i * 2U);
        result ^= (StarsU16)(data[address] | ((StarsU16)data[(StarsU16)(address + 1U)] << 8));
    }
    return result;
}

static StarsI16 signed_byte(StarsU8 value)
{
    return value < 128U ? (StarsI16)value : (StarsI16)((int)value - 256);
}

static void corrected(StarsU8 *data, StarsU16 player)
{
    data[(StarsU16)(player + 84U)] |= 16U;
}

/* NE 29:40AE-440F. Correct environmental endpoints/centers and race stats.
 * This is mutating validation, not just a predicate. */
void stars_bounds_check_player(StarsU8 *data, StarsU16 player)
{
    StarsU16 i, low, high, center, address;
    StarsI16 value;
    for (i = 0; i < 3U; ++i) {
        center = (StarsU16)(player + 16U + i);
        low = (StarsU16)(player + 19U + i);
        high = (StarsU16)(player + 22U + i);
        if (data[low] == 255U) {
            if (data[high] != 255U || data[center] != 255U) {
                data[center] = 255U;
                data[high] = 255U;
                corrected(data, player);
            }
        } else {
            if (signed_byte(data[low]) < 0) { data[low] = 0; corrected(data, player); }
            if (signed_byte(data[low]) > 100) { data[low] = 100; corrected(data, player); }
            if (signed_byte(data[high]) > 100) { data[high] = 100; corrected(data, player); }
            if (signed_byte(data[high]) < signed_byte(data[low])) {
                data[high] = data[low]; corrected(data, player);
            }
            value = (StarsI16)(data[low] + (data[high] - data[low]) / 2);
            if (signed_byte(data[center]) != value) {
                data[center] = (StarsU8)value; corrected(data, player);
            }
        }
    }
    address = (StarsU16)(player + 25U);
    if (signed_byte(data[address]) > 20) { data[address] = 20; corrected(data, player); }
    for (i = 0; i < 16U; ++i) {
        address = (StarsU16)(player + 62U + i);
        if (signed_byte(data[address]) < stat_min[i]) {
            data[address] = (StarsU8)stat_min[i]; corrected(data, player);
        }
        if (signed_byte(data[address]) > stat_max[i]) {
            data[address] = (StarsU8)stat_max[i]; corrected(data, player);
        }
    }
}
