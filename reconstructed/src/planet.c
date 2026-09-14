/* NE 10:6E1E-7095. PctPlanetDesirability.
 * Valid-domain integer equivalent of the original sqrt(sum/3)+0.9 rounding.
 * See analysis/reconstruction/habitability.md for limits and proof. */
#include "core.h"
#include "innate_fp.h"

static StarsI16 env_byte(const StarsU8 *data, StarsU16 address)
{
    StarsU8 byte = data[address];
    return byte < 128U ? (StarsI16)byte : (StarsI16)((int)byte - 256);
}

static StarsI16 environment_desirability(const StarsU8 *data, const StarsI16 values[3], StarsI16 player_index)
{
    StarsU16 player = (StarsU16)(0x59a2U + (StarsU16)player_index * 192U);
    StarsU16 axis, rounded = 0;
    StarsI16 environment, center, low, high, distance, width, edge, score, red = 0;
    StarsU32 squares = 0, penalty = 10000UL, boundary;
    for (axis = 0; axis < 3U; ++axis) {
        environment = values[axis];
        center = env_byte(data, (StarsU16)(player + 16U + axis));
        low = env_byte(data, (StarsU16)(player + 19U + axis));
        high = env_byte(data, (StarsU16)(player + 22U + axis));
        if (high < 0) {
            squares += 10000UL;
        } else if (environment < low || environment > high) {
            distance = environment < low ? (StarsI16)(low - environment) : (StarsI16)(environment - high);
            red = (StarsI16)(red + (distance < 15 ? distance : 15));
        } else {
            distance = environment < center ? (StarsI16)(center - environment) : (StarsI16)(environment - center);
            width = environment < center ? (StarsI16)(center - low) : (StarsI16)(high - center);
            score = (StarsI16)(100 - distance * 100 / width);
            squares += (StarsU32)score * (StarsU32)score;
            edge = (StarsI16)(distance * 2 - width);
            if (edge > 0)
                penalty = penalty * (StarsU32)(width * 2 - edge) / (StarsU32)(width * 2);
        }
    }
    if (red) return (StarsI16)-red;
    /* The original biases sqrt(squares/3) upward by adding 0.9 then
     * truncating. Compare squares against each n+0.1 threshold exactly.
     * No equality is possible: 3*(10*n+1)^2 is never divisible by 100. */
    for (;;) {
        boundary = (StarsU32)(rounded * 10U + 1U);
        if (100UL * squares < 3UL * boundary * boundary) break;
        ++rounded;
    }
    return (StarsI16)((StarsU32)rounded * penalty / 10000UL);
}

StarsI16 stars_planet_desirability(const StarsU8 *data, StarsU16 planet, StarsI16 player_index)
{
    StarsI16 values[3];
    StarsU16 axis;
    for (axis = 0; axis < 3U; ++axis)
        values[axis] = env_byte(data, (StarsU16)(planet + 12U + axis));
    return environment_desirability(data, values, player_index);
}

static void copy_player_zero(StarsU8 *data, StarsU16 player)
{
    StarsU16 i;
    StarsU8 first, second;
    for (i = 0; i < 192U; i += 2U) {
        first = data[(StarsU16)(player + i)];
        second = data[(StarsU16)(player + i + 1U)];
        data[0x59a2U + i] = first;
        data[0x59a3U + i] = second;
    }
}

static StarsI16 innate_sample(StarsI16 start, StarsI16 span, StarsI16 count,
    StarsI16 index, StarsI16 center, StarsI16 phase, StarsI16 adjust, StarsI16 *delta)
{
    StarsI16 value = start;
    if (index && count >= 2) value = (StarsI16)(start + index * span / (count - 1));
    if (phase && center >= 0) {
        *delta = (StarsI16)(center - value);
        if (*delta >= -adjust && *delta <= adjust) *delta = 0;
        else if (*delta < 0) *delta = (StarsI16)(*delta + adjust);
        else *delta = (StarsI16)(*delta - adjust);
        value = (StarsI16)(center - *delta);
    }
    return value;
}

/* NE 29:4C6E-54D4. Host semantic adapter; see innate-habitability.md.
 * Explicit positive floating arithmetic preserves x87 operations and double
 * stores independently of the host floating-point precision. */
StarsI32 stars_innate_race_habitability(StarsU8 *data, StarsU16 player)
{
    StarsU8 saved[192];
    StarsI16 center[3], start[3], span[3], count[3], delta[3], values[3];
    StarsI16 phase, axis, x, y, z, low, high, end, adjust, trait, excess;
    StarsU16 i;
    StarsI32 desirability, inner;
    InnateFP total = innate_fp_integer(0), outer, row;
    static const StarsI16 weights[3] = {7, 5, 6};
    for (i = 0; i < 192U; ++i) saved[i] = data[0x59a2U + i];
    trait = stars_get_race_trait(data, player, 1);
    copy_player_zero(data, player);
    for (axis = 0; axis < 3; ++axis) delta[axis] = 0;
    for (phase = 0; phase < 3; ++phase) {
        adjust = phase == 0 ? 0 : (phase == 1 ? (trait ? 8 : 5) : (trait ? 17 : 15));
        for (axis = 0; axis < 3; ++axis) {
            center[axis] = env_byte(data, (StarsU16)(player + 16U + axis));
            low = env_byte(data, (StarsU16)(player + 19U + axis));
            high = env_byte(data, (StarsU16)(player + 22U + axis));
            if ((center[axis] < 0 || center[axis] > 100 || low < 0 || low > 100 || high < 0 || high > 100)
                && !(center[axis] == -1 && low == -1 && high == -1)) {
                data[(StarsU16)(player + 22U + axis)] = 255U;
                data[(StarsU16)(player + 19U + axis)] = 255U;
                data[(StarsU16)(player + 16U + axis)] = 255U;
                data[(StarsU16)(player + 84U)] |= 16U;
                center[axis] = low = high = -1;
                copy_player_zero(data, player);
            }
            if (center[axis] < 0) {
                start[axis] = 50; span[axis] = 11; count[axis] = 1;
            } else {
                start[axis] = (StarsI16)(low > adjust ? low - adjust : 0);
                end = (StarsI16)(high + adjust < 100 ? high + adjust : 100);
                span[axis] = (StarsI16)(end - start[axis]); count[axis] = 11;
            }
        }
        outer = innate_fp_integer(0);
        for (x = 0; x < count[0]; ++x) {
            values[0] = innate_sample(start[0], span[0], count[0], x, center[0], phase, adjust, &delta[0]);
            row = innate_fp_integer(0);
            for (y = 0; y < count[1]; ++y) {
                values[1] = innate_sample(start[1], span[1], count[1], y, center[1], phase, adjust, &delta[1]);
                inner = 0;
                for (z = 0; z < count[2]; ++z) {
                    values[2] = innate_sample(start[2], span[2], count[2], z, center[2], phase, adjust, &delta[2]);
                    desirability = environment_desirability(data, values, 0);
                    excess = (StarsI16)(delta[0] + delta[1] + delta[2] - adjust);
                    if (excess > 0) {
                        desirability -= excess;
                        if (desirability < 0) desirability = 0;
                    }
                    inner += desirability * desirability * weights[phase];
                }
                if (center[2] < 0) inner *= 11;
                else inner = inner * span[2] / 100;
                row = innate_fp_store(innate_fp_add(row, innate_fp_integer((StarsU32)inner)));
            }
            row = innate_fp_scale(row, center[1], span[1]);
            outer = innate_fp_store(innate_fp_add(outer, row));
        }
        outer = innate_fp_scale(outer, center[0], span[0]);
        total = innate_fp_store(innate_fp_add(total, outer));
    }
    if (player != 0x59a2U)
        for (i = 0; i < 192U; ++i) data[0x59a2U + i] = saved[i];
    return innate_fp_result(total);
}
