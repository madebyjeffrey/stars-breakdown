/* Reviewed against NE 9 instruction ranges; see analysis/reconstruction/core.md.
 * Unsigned operations implement the original 32-bit wrap explicitly. */
#include "core.h"
static const StarsI16 seed_table[128] = {
#include "seed_table.inc"
};

static StarsU32 sign_extend_word(StarsU16 value)
{
    return value & 0x8000U ? (StarsU32)value | 0xffff0000UL : (StarsU32)value;
}

/* Quotient of a signed 32-bit bit pattern by a positive divisor, truncated
 * toward zero. Avoid C89's implementation-defined negative division. */
static StarsU32 signed_quotient(StarsU32 value, StarsU32 divisor)
{
    if (value & 0x80000000UL)
        return (StarsU32)0 - (((StarsU32)0 - value) / divisor);
    return value / divisor;
}

static StarsU32 advance_seed(StarsU32 seed, StarsU32 quotient_divisor,
    StarsU32 multiplier, StarsU32 correction, StarsU32 modulus)
{
    StarsU32 quotient, next;
    quotient = signed_quotient(seed, quotient_divisor);
    next = (seed - quotient * quotient_divisor) * multiplier - quotient * correction;
    if (next & 0x80000000UL)
        next += modulus;
    return next;
}

/* NE 9:15EA-165A. Equal table indices are separated before loading. */
void stars_randomize(StarsSeeds *state, StarsU32 seed)
{
    StarsU16 first, second;
    first = (StarsU16)(seed & 63U);
    second = (StarsU16)((seed >> 6) & 63U);
    if (first == second)
        second = (StarsU16)((second + 1U) & 63U);
    state->first = sign_extend_word((StarsU16)seed_table[first]);
    state->second = sign_extend_word((StarsU16)seed_table[second]);
}

/* NE 9:165A-16D2. Both indices use seven bits and distinct XOR masks. */
void stars_randomize2(StarsSeeds *state, StarsU32 seed)
{
    StarsU16 first, second;
    first = (StarsU16)((seed & 127U) ^ 0x35U);
    second = (StarsU16)(((seed >> 7) & 127U) ^ 0x5cU);
    if (first == second)
        second = (StarsU16)((second + 1U) & 127U);
    state->first = sign_extend_word((StarsU16)seed_table[first]);
    state->second = sign_extend_word((StarsU16)seed_table[second]);
}

/* NE 9:16D2-1889. Even a nonpositive count advances BOTH seeds. */
StarsI16 stars_random(StarsSeeds *state, StarsI16 count)
{
    StarsU32 difference;
    state->first = advance_seed(state->first, 53668UL, 40014UL, 12211UL, 2147483563UL);
    state->second = advance_seed(state->second, 52774UL, 40692UL, 3791UL, 2147483399UL);
    difference = state->first - state->second;
    if (!difference || (difference & 0x80000000UL))
        difference += 2147483562UL;
    if (count <= 0)
        return 0;
    return (StarsI16)(difference % (StarsU32)count);
}

/* NE 9:1A4E-1A7B and 1A7C-1AA5. Preserve output assignment order. */
void stars_get_file_seeds(const StarsSeeds *state, StarsU32 *first, StarsU32 *second)
{
    *first = state->first;
    *second = state->second;
}
void stars_set_file_seeds(StarsSeeds *state, StarsU32 first, StarsU32 second)
{
    state->first = first;
    state->second = second;
}

/* NE 9:1B54-1CC3. Unlike Random, the returned difference is not corrected. */
StarsU32 stars_next_file_xor(StarsSeeds *state)
{
    state->first = advance_seed(state->first, 53668UL, 40014UL, 12211UL, 2147483563UL);
    state->second = advance_seed(state->second, 52774UL, 40692UL, 3791UL, 2147483399UL);
    return state->first - state->second;
}

/* NE 9:1AA6-1B53. The warmup count is a signed-tested, wrapping 16-bit word. */
void stars_set_file_xor_stream(StarsSeeds *state, StarsU32 game_id,
    StarsI16 salt, StarsI16 turn, StarsI16 player, StarsI16 crippled)
{
    StarsU16 first, second, count;
    first = (StarsU16)((StarsU16)salt & 31U);
    second = (StarsU16)(((StarsU16)salt >> 5) & 31U);
    if ((StarsU16)salt & 0x400U)
        first += 32U;
    else
        second += 32U;
    state->first = sign_extend_word((StarsU16)seed_table[first]);
    state->second = sign_extend_word((StarsU16)seed_table[second]);
    count = (StarsU16)(((game_id & 3U) + 1U) * (((StarsU16)turn & 3U) + 1U)
        * (((StarsU16)player & 3U) + 1U) + (StarsU16)crippled);
    while (count && !(count & 0x8000U)) {
        stars_next_file_xor(state);
        --count;
    }
}

/* NE 9:685C. SUB is 16-bit BEFORE CWD/sign extension and long multiply. */
StarsU32 stars_distance_squared(StarsI16 x1, StarsI16 y1, StarsI16 x2, StarsI16 y2)
{
    StarsU32 dx, dy;
    dx = sign_extend_word((StarsU16)((StarsU16)x1 - (StarsU16)x2));
    dy = sign_extend_word((StarsU16)((StarsU16)y1 - (StarsU16)y2));
    return dx * dx + dy * dy;
}

/* NE 9:1D74-1D96. Despite its name, this returns the LOW signed word of
 * a subtraction, not a conventional -1/0/+1 comparison of long values. */
StarsI16 stars_compare_long(StarsU32 first, StarsU32 second)
{
    StarsU16 difference = (StarsU16)(first - second);
    if (difference & 0x8000U)
        return (StarsI16)(-1 - (StarsI16)(0xffffU - difference));
    return (StarsI16)difference;
}

/* NE 9:2F0C-2F3E. Assignment order matches the original. */
void stars_expand_rect(StarsRect *rect, StarsI16 dx, StarsI16 dy)
{
    rect->left = (StarsU16)(rect->left - (StarsU16)dx);
    rect->right = (StarsU16)(rect->right + (StarsU16)dx);
    rect->top = (StarsU16)(rect->top - (StarsU16)dy);
    rect->bottom = (StarsU16)(rect->bottom + (StarsU16)dy);
}

/* NE 9:2F3E-2F70. */
void stars_offset_rect(StarsRect *rect, StarsI16 dx, StarsI16 dy)
{
    rect->left = (StarsU16)(rect->left + (StarsU16)dx);
    rect->right = (StarsU16)(rect->right + (StarsU16)dx);
    rect->top = (StarsU16)(rect->top + (StarsU16)dy);
    rect->bottom = (StarsU16)(rect->bottom + (StarsU16)dy);
}
