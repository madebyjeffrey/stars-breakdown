/* NE 10:6E1E-7095. PctPlanetDesirability.
 * Valid-domain integer equivalent of the original sqrt(sum/3)+0.9 rounding.
 * See analysis/reconstruction/habitability.md for limits and proof. */
#include "core.h"

static StarsI16 env_byte(const StarsU8 *data, StarsU16 address)
{
    StarsU8 byte = data[address];
    return byte < 128U ? (StarsI16)byte : (StarsI16)((int)byte - 256);
}

StarsI16 stars_planet_desirability(const StarsU8 *data, StarsU16 planet, StarsI16 player_index)
{
    StarsU16 player = (StarsU16)(0x59a2U + (StarsU16)player_index * 192U);
    StarsU16 axis, rounded = 0;
    StarsI16 environment, center, low, high, distance, width, edge, score, red = 0;
    StarsU32 squares = 0, penalty = 10000UL, boundary;
    for (axis = 0; axis < 3U; ++axis) {
        environment = env_byte(data, (StarsU16)(planet + 12U + axis));
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
