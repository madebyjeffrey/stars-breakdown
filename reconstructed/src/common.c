/* Shared numeric/geometry helpers; widths and aliasing follow the x86 code. */
#include "common.h"
#include "runtime_internal.h"

static StarsI32 signed_long(StarsU32 n)
{ return n <= 0x7fffffffUL ? (StarsI32)n : -1L - (StarsI32)(~n); }

/* NE 9:2274-22b5, PszFromInt. */
StarsU16 stars_number_int(StarsU8 *d, StarsI16 n, StarsU16 count)
{
    StarsU16 length = (StarsU16)sprintf((char *)d + 0x590eU,
        (char *)d + stars_word(d, 0x1aaU), (int)n);
    if (count) stars_put_word(d, count, length);
    return 0x590eU;
}

/* NE 9:22b6-22fc, PszFromLong. The original tests *pcch, not pcch. Thus a
 * zero initial length remains zero, and offset zero is actually dereferenced. */
StarsU16 stars_number_long(StarsU8 *d, StarsI32 n, StarsU16 count)
{
    StarsU16 length = (StarsU16)sprintf((char *)d + 0x590eU,
        (char *)d + stars_word(d, 0x1b0U), (long)n);
    if (stars_word(d, count)) stars_put_word(d, count, length);
    return 0x590eU;
}

/* NE 9:22fe-243f, PszFromLongK. Appends over the formatter's terminator without
 * writing another NUL. Consumers use the returned count. Positive rounding
 * near INT32_MAX can wrap negative before the signed division. */
StarsU16 stars_number_scaled(StarsU8 *d, StarsI32 n, StarsU16 count)
{
    int thousands = n >= 10000L, millions = n >= 1000000L;
    StarsU16 at;
    if (millions) {
        n = signed_long((StarsU32)n + 500000UL) / 1000000L;
        if (n > 999L) n = 999;
    } else if (thousands) {
        n = signed_long((StarsU32)n + 500UL) / 1000L;
        if (n > 999L) n = 999;
    }
    stars_number_int(d, stars_signed_word((StarsU16)n), count);
    if (millions || thousands) {
        at = stars_word(d, count);
        stars_put_word(d, count, (StarsU16)(at + 1U));
        d[(StarsU16)(0x590eU + at)] = (StarsU8)(millions ? 'M' : 'k');
    }
    return 0x590eU;
}

/* NE 9:2440-2532, CommaFormatLong. The first group includes the minus sign. */
StarsU16 stars_number_commas(StarsU8 *d, StarsU16 out, StarsI32 n)
{
    char buffer[18];
    int length, group, i = 0;
    StarsU16 begin = out;
    length = sprintf(buffer, (char *)d + stars_word(d, 0x1b0U), (long)n);
    group = length % 3;
    if (!group) group = 3;
    else if (group == 1 && n < 0) group = 4;
    while (group-- > 0) d[out++] = (StarsU8)buffer[i++];
    while (buffer[i]) {
        d[out++] = ',';
        for (group = 0; group < 3; ++group) d[out++] = (StarsU8)buffer[i++];
    }
    d[out] = 0;
    return (StarsU16)(out - begin);
}

/* NE 9:2f70-3093, BoundPoints. Reads point zero even when count <= zero;
 * one-pixel expansion wraps at the signed 16-bit coordinate boundaries. */
void stars_bound_points(StarsU8 *d, StarsU16 rect, StarsU16 points, StarsI16 count)
{
    StarsI16 x, y, left, top, right, bottom, i;
    left = right = stars_signed_word(stars_word(d, points));
    top = bottom = stars_signed_word(stars_word(d, (StarsU16)(points + 2U)));
    for (i = 1; i < count; ++i) {
        x = stars_signed_word(stars_word(d, (StarsU16)(points + (StarsU16)i * 4U)));
        y = stars_signed_word(stars_word(d, (StarsU16)(points + (StarsU16)i * 4U + 2U)));
        if (x <= left) left = x;
        if (x >= right) right = x;
        if (y <= top) top = y;
        if (y >= bottom) bottom = y;
    }
    stars_put_word(d, (StarsU16)(rect + 2U), (StarsU16)((StarsU16)top - 1U));
    stars_put_word(d, (StarsU16)(rect + 6U), (StarsU16)((StarsU16)bottom + 1U));
    stars_put_word(d, rect, (StarsU16)((StarsU16)left - 1U));
    stars_put_word(d, (StarsU16)(rect + 4U), (StarsU16)((StarsU16)right + 1U));
}

/* NE 8:4ba2-4c00, WPackLong. Logical shifts, including negative input bit
 * patterns. The exponent can exceed three bits and truncates on return. */
StarsU16 stars_pack_long(StarsU32 n)
{
    StarsU16 exponent = 0;
    while (n & 0xffffe000UL) { n >>= 2; ++exponent; }
    return (StarsU16)(((StarsU32)exponent << 13) | n);
}
