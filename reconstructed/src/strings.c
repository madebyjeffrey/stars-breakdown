#include "core.h"
#include <stdio.h>
#include <string.h>

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

/* NE 8:11F2-145C. Formatting uses the verified static "Player %d" string.
 * 50-byte temporary limit; see player-records.md for the one-letter-e edge. */
StarsU16 stars_player_name(StarsU8 *data, const StarsU8 *strings, StarsI16 index,
    StarsI16 capital, StarsI16 plural, StarsI16 article, StarsI16 grammar, StarsU16 player)
{
    char temporary[50];
    StarsU16 text, number_word;
    StarsI16 number;
    unsigned int length;
    if (!player) player = (StarsU16)(0x59a2U + (StarsU16)index * 192U);
    if (!data[player + 128U]) {
        number_word = (StarsU16)((StarsU16)index + 1U);
        number = number_word < 32768U ? (StarsI16)number_word : (StarsI16)((StarsI32)number_word - 65536L);
        text = stars_get_compressed_string(data, strings, 0x55e);
        sprintf(temporary, (const char *)data + text, (int)number);
        if (!plural) strcat(temporary, (const char *)data + 0x515U);
        if (grammar == 1 || grammar == 2) {
            text = stars_get_compressed_string(data, strings, (StarsI16)(0x55b + grammar));
            strcat(temporary, (const char *)data + text);
        }
    } else {
        temporary[0] = 0;
        if (article) {
            strcpy(temporary, (const char *)data + 0x50eU);
            if (capital) temporary[0] = 'T';
        }
        text = (StarsU16)(player + (plural && data[player + 160U] ? 160U : 128U));
        strcat(temporary, (const char *)data + text);
        length = (unsigned int)strlen(temporary);
        while (length && temporary[length - 1U] == ' ') temporary[--length] = 0;
        if (!length) {
            text = stars_get_compressed_string(data, strings, 0x559);
            strcpy(temporary, (const char *)data + text);
        }
        if (plural && !data[player + 160U]) {
            length = (unsigned int)strlen(temporary);
            if (temporary[length - 1U] != 's' &&
                !(length >= 2U && temporary[length - 1U] == 'e' && temporary[length - 2U] == 's'))
                strcat(temporary, (const char *)data + 0x513U);
        }
        if (grammar == 1 || grammar == 2) {
            text = stars_get_compressed_string(data, strings, (StarsI16)(0x559 + grammar));
            strcat(temporary, (const char *)data + text);
        }
    }
    strcpy((char *)data + 0x57a4U, temporary);
    return 0x57a4U;
}
