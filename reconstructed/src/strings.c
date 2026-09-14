#include "core.h"

/* NE 3:743A-7599. The separate image is the original strings code segment,
 * including its length, group-offset and character tables. Valid ids 0..1413,
 * or a cache hit; 256 bytes at DGROUP:547A must remain writable. */
StarsU16 stars_get_compressed_string(StarsU8 *data, const StarsU8 *strings, StarsI16 id)
{
    StarsI16 cached = data[0x1484] < 128U ? (StarsI16)data[0x1484] : (StarsI16)((int)data[0x1484] - 256);
    StarsU16 group, lengths, table, cursor, sum = 0, count, i, nibble, character = 0;
    StarsU16 output = 0x547aU;
    int high;
    if (id == cached) return output;
    group = (StarsU16)((StarsU16)id / 64U);
    lengths = (StarsU16)(0x6e32U + group * 64U);
    for (i = 0; i < ((StarsU16)id & 63U); ++i) sum = (StarsU16)(sum + strings[lengths++]);
    table = (StarsU16)(0x73b8U + group * 2U);
    cursor = (StarsU16)(strings[table] | ((StarsU16)strings[table + 1U] << 8));
    cursor = (StarsU16)(cursor + sum / 2U);
    high = !(sum & 1U);
    count = strings[lengths];
    for (i = 0; i < count; ++i) {
        if (high) nibble = (StarsU16)(strings[cursor] >> 4);
        else nibble = (StarsU16)(strings[cursor++] & 15U);
        high = !high;
        character = (StarsU16)(character + nibble);
        if (nibble != 15U) {
            data[output++] = strings[0x73e6U + character];
            character = 0;
        }
    }
    data[output] = 0;
    /* The original tests the cache marker but does not update it here. */
    return 0x547aU;
}

/* NE 9:221E-2273. Forward byte copy, retaining aliasing and near wrap.
 * Destination must permit the copy to reach the source terminator. */
StarsI16 stars_copy_game_string(StarsU8 *data, const StarsU8 *strings, StarsI16 id, StarsU16 output)
{
    StarsU16 source = stars_get_compressed_string(data, strings, id);
    StarsU16 start = output, length;
    while (data[source]) data[output++] = data[source++];
    data[output] = 0;
    length = (StarsU16)(output - start);
    return length < 32768U ? (StarsI16)length : (StarsI16)((StarsI32)length - 65536L);
}
