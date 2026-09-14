/* NE 9 user-string codec. Table addresses and nibble order are original.
 * Host pointers replace far pointers; see analysis/reconstruction/codec.md. */
#include "core.h"
#include <string.h>

static StarsU16 word_at(const StarsU8 *data, StarsU16 offset)
{
    return (StarsU16)(data[offset] | ((StarsU16)data[(StarsU16)(offset + 1U)] << 8));
}

/* NE 9:4880-49E9. The original strchr also finds the terminating zero. */
StarsU16 stars_nybble_from_char(const StarsU8 *data, StarsU8 ch)
{
    StarsU16 at;
    if (ch >= 'a' && ch <= 'z')
        return word_at(data, (StarsU16)(0x13aeU + (ch - 'a') * 2U));
    if (ch == ' ') return 0;
    if (ch >= 'A' && ch <= 'P') return (StarsU16)(((ch - 'A') << 4) | 11);
    if (ch >= 'Q' && ch <= 'Z') return (StarsU16)(((ch - 'Q') << 4) | 12);
    if (ch >= '0' && ch <= '5') return (StarsU16)(((ch - '0' + 10) << 4) | 12);
    if (ch >= '6' && ch <= '9') return (StarsU16)(((ch - '6') << 4) | 13);
    at = 0x1400U;
    for (;;) {
        if (data[at] == ch) return (StarsU16)(((at - 0x1400U + 4U) << 4) | 14U);
        if (!data[at]) break;
        at = (StarsU16)(at + 1U);
    }
    return (StarsU16)(((StarsU16)ch << 4) | 15U);
}

/* NE 9:49EA-4A99. The signed initial comparison permits negative table indices. */
StarsU8 stars_char_from_nybble(const StarsU8 *data, StarsI16 code)
{
    StarsU16 index, value = (StarsU16)code;
    if (code < 11)
        return data[(StarsU16)(word_at(data, 0x13feU) + value)];
    if ((value & 15U) == 15U) return (StarsU8)(value >> 4);
    index = (StarsU16)((value >> 4) + ((value & 15U) - 11U) * 16U);
    /* index can be negative when a noncanonical code has low nibble < 11. */
    if ((index & 0x8000U) || index < 26U) return (StarsU8)(index + 'A');
    if (index < 36U) return (StarsU8)(index + 22U);
    if (index >= 52U) return data[(StarsU16)(0x1400U + index - 52U)];
    return data[(StarsU16)(word_at(data, 0x13feU) + index - 25U)];
}

/* NE 9:45A0-46E8. Output is untouched on either failure path. */
int stars_compress_user_string(const StarsU8 *data, const StarsU8 *input,
    StarsU8 *output, StarsI16 *capacity)
{
    StarsU8 temporary[1024];
    StarsU16 used = 0, code, pieces;
    int low = 0;
    while (*input) {
        code = stars_nybble_from_char(data, *input++);
        pieces = code < 11U ? 1U : ((code & 15U) == 15U ? 3U : 2U);
        while (pieces--) {
            if (!low) {
                temporary[used] = (StarsU8)((code & 15U) << 4);
                low = 1;
            } else {
                temporary[used++] |= (StarsU8)(code & 15U);
                low = 0;
                if (used >= 1024U) return 0;
            }
            code >>= 4;
        }
    }
    if (low) temporary[used++] |= 15U;
    if (*capacity < 0 || used > (StarsU16)*capacity) return 0;
    *capacity = (StarsI16)used;
    memcpy(output, temporary, used);
    return 1;
}

static StarsU16 take_nybble(const StarsU8 **input, int *low, StarsI16 *remaining)
{
    StarsU16 value;
    if (*low) {
        value = **input & 15U;
        ++*input;
        --*remaining;
    } else value = **input >> 4;
    *low = !*low;
    return value;
}

/* NE 9:46E8-487F. Requires <=1023 decoded bytes before the final NUL and
 * accessible lookahead for truncated escape sequences, as the original does.
 * Capacity counts decoded bytes, not the terminator; it is never updated. */
int stars_decompress_user_string(const StarsU8 *data, const StarsU8 *input,
    StarsI16 length, StarsU8 *output, const StarsI16 *capacity)
{
    StarsU8 temporary[1024];
    StarsU16 used = 0, code;
    int low = 0, was_low;
    while (length > 0) {
        was_low = low;
        code = take_nybble(&input, &low, &length);
        if (was_low && code == 15U && length == 0) break;
        if (code >= 11U) {
            code |= (StarsU16)(take_nybble(&input, &low, &length) << 4);
            if ((code & 15U) == 15U)
                code |= (StarsU16)(take_nybble(&input, &low, &length) << 8);
        }
        temporary[used++] = stars_char_from_nybble(data, (StarsI16)code);
        if (*capacity < 0 || used > (StarsU16)*capacity) return 0;
    }
    temporary[used] = 0;
    strcpy((char *)output, (const char *)temporary);
    return 1;
}
