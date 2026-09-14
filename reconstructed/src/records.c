/* File-record transformations; original stream I/O remains a host boundary.
 * See analysis/reconstruction/player-records.md for bounds and failure scope. */
#include "core.h"
#include <string.h>

static StarsU16 record_word(const StarsU8 *data, StarsU16 at)
{
    return (StarsU16)(data[at] | ((StarsU16)data[(StarsU16)(at + 1U)] << 8));
}

static StarsI16 record_signed(StarsU16 word)
{
    return word < 32768U ? (StarsI16)word : (StarsI16)((StarsI32)word - 65536L);
}

static void record_seed_stream(StarsU8 *data)
{
    StarsSeeds seeds;
    StarsU16 flags = record_word(data, 0x4ba4U), i;
    StarsI16 salt = record_signed((StarsU16)((flags >> 5) | ((flags & 0x8000U) ? 0xf800U : 0)));
    StarsI16 player = (StarsI16)(flags & 31U);
    StarsU32 game = record_word(data, 0x4b9cU) | ((StarsU32)record_word(data, 0x4b9eU) << 16);
    if (player >= 16) player = (StarsI16)(player - 32);
    /* SetFileXorStream initializes both seeds; no preceding seed is read. */
    stars_set_file_xor_stream(&seeds, game, salt, record_signed(record_word(data, 0x4ba2U)),
        player, (StarsI16)((record_word(data, 0x4ba6U) >> 12) & 1U));
    for (i = 0; i < 4U; ++i) {
        data[0x2272U + i] = (StarsU8)(seeds.first >> (i * 8U));
        data[0x2276U + i] = (StarsU8)(seeds.second >> (i * 8U));
    }
}

/* NE 15:5168-524D. Zero-length records retain the preceding body contents. */
void stars_read_record(StarsU8 *data, StarsReadTransfer read, void *context)
{
    StarsU16 header, count, type;
    read(context, data + 0x2696U, 2);
    header = record_word(data, 0x2696U);
    count = (StarsU16)(header & 1023U);
    if (count) read(context, data + 0x4b98U, count);
    type = (StarsU16)(record_word(data, 0x2696U) >> 10);
    if (type == 8U) record_seed_stream(data);
    else if (type) stars_xor_file_buffer(data, 0x4b98U, (StarsI16)count);
}

/* NE 15:947C-9553. Header is local; hdrCur is not changed by writing. */
void stars_write_record(StarsU8 *data, StarsI16 type, StarsU16 count, const StarsU8 *input,
    StarsWriteTransfer write, void *context)
{
    StarsU16 word;
    StarsU8 header[2];
    memmove(data + 0x4b98U, input, count);
    if (type == 8) record_seed_stream(data);
    else if (type) stars_xor_file_buffer(data, 0x4b98U, (StarsI16)count);
    word = (StarsU16)((count & 1023U) | (((StarsU16)type & 63U) << 10));
    header[0] = (StarsU8)word; header[1] = (StarsU8)(word >> 8);
    write(context, header, 2);
    write(context, data + 0x4b98U, count);
}

/* NE 15:05E2-080F. Input and output regions must be disjoint. */
void stars_read_player_record(StarsU8 *data, const StarsU8 *strings, StarsU16 player, StarsU16 input)
{
    StarsU16 cursor, length, name;
    StarsI16 capacity = 32, first, capital;
    memset(data + player, 0, 192);
    if ((record_word(data, (StarsU16)(input + 6U)) & 7U) == 7U) {
        memmove(data + player, data + input, 112);
        length = data[input + 112U];
        memmove(data + player + 112U, data + input + 113U, length);
        cursor = (StarsU16)(113U + length);
    } else {
        memmove(data + player, data + input, 8);
        cursor = 8;
    }
    length = data[input + cursor];
    if (!length) {
        strcpy((char *)data + player + 128U, (const char *)data + input + cursor + 1U);
        cursor = (StarsU16)(cursor + strlen((const char *)data + player + 128U) + 2U);
    } else {
        stars_decompress_user_string(data, data + input + cursor + 1U, (StarsI16)length,
            data + player + 128U, &capacity);
        cursor = (StarsU16)(cursor + length + 1U);
    }
    if (((record_word(data, 0x750U) >> 5) & 127U) < 55U) {
        first = data[player + 128U] < 128U ? (StarsI16)data[player + 128U] : (StarsI16)((int)data[player + 128U] - 256);
        capital = (StarsI16)(data[(StarsU16)(0x175fU + first)] & 1U);
        name = stars_player_name(data, strings, 0, capital, 1, 0, 0, player);
        strcpy((char *)data + player + 160U, (const char *)data + name);
    } else {
        length = data[input + cursor];
        if (!length) strcpy((char *)data + player + 160U, (const char *)data + input + cursor + 1U);
        else stars_decompress_user_string(data, data + input + cursor + 1U, (StarsI16)length,
            data + player + 160U, &capacity);
    }
    data[player + 84U] &= 247U;
}

/* NE 15:551C-574D. A null storage offset selects a local 266-byte buffer. */
void stars_write_player_record(StarsU8 *data, StarsU16 player, StarsU16 storage,
    StarsWriteTransfer write, void *context)
{
    StarsU8 temporary[266];
    StarsU8 *body = storage ? data + storage : temporary;
    StarsU16 cursor, name, i;
    StarsI16 relations, capacity;
    if (data[player + 84U] & 1U) data[player + 6U] |= 7U;
    memmove(body, data + player, 192);
    if ((record_word(data, (StarsU16)(player + 6U)) & 7U) == 7U) {
        relations = 15;
        while (relations >= 0 && !data[player + 112U + relations]) --relations;
        ++relations;
        body[112] = (StarsU8)relations;
        memmove(body + 113U, data + player + 112U, (StarsU16)relations);
        cursor = (StarsU16)(113 + relations);
    } else cursor = 8;
    for (i = 0; i < 2U; ++i) {
        name = (StarsU16)(player + 128U + i * 32U);
        capacity = 31;
        if (!data[name] || !stars_compress_user_string(data, data + name, body + cursor + 1U, &capacity)) {
            strcpy((char *)body + cursor + 1U, (const char *)data + name);
            body[cursor] = 0;
            cursor = (StarsU16)(cursor + strlen((const char *)data + name) + 2U);
        } else {
            body[cursor] = (StarsU8)capacity;
            cursor = (StarsU16)(cursor + capacity + 1U);
        }
    }
    stars_write_record(data, 6, cursor, body, write, context);
}

/* NE 15:8EA4-9049. multi is present in the debug signature but never read. */
void stars_write_bof(StarsU8 *data, StarsI16 player, StarsI16 type, StarsI16 multi,
    StarsTickCount ticks, StarsWriteTransfer write, void *context)
{
    StarsU8 body[16];
    StarsSeeds seeds;
    StarsU16 flags, salt, i;
    StarsI16 random;
    (void)multi;
    memset(body, 0, sizeof(body));
    strncpy((char *)body, (const char *)data + 0xa1cU, 4);
    memcpy(body + 4, data + 0x70U, 4);
    body[8] = 0x60U; body[9] = 0x2aU;
    memcpy(body + 10, data + 0x82U, 2);
    flags = (StarsU16)((record_word(data, 0x80U) & 0xe00U) << 4);
    seeds.first = record_word(data, 0x1370U) | ((StarsU32)record_word(data, 0x1372U) << 16);
    seeds.second = record_word(data, 0x1374U) | ((StarsU32)record_word(data, 0x1376U) << 16);
    random = stars_random(&seeds, 2000);
    for (i = 0; i < 4U; ++i) {
        data[0x1370U + i] = (StarsU8)(seeds.first >> (i * 8U));
        data[0x1374U + i] = (StarsU8)(seeds.second >> (i * 8U));
    }
    salt = (StarsU16)((StarsU16)ticks(context) + (StarsU16)random);
    salt = (StarsU16)(((salt & 2047U) << 5) | ((StarsU16)player & 31U));
    body[12] = (StarsU8)salt; body[13] = (StarsU8)(salt >> 8);
    flags |= (StarsU16)((StarsU16)type & 255U);
    flags |= (StarsU16)((record_word(data, 0x7caU) & 16U) << 4);
    flags |= (StarsU16)((record_word(data, 0x7caU) & 8U) << 6);
    if (type == 2 && (data[0x7ccU] & 1U)) flags |= 0x800U;
    body[14] = (StarsU8)flags; body[15] = (StarsU8)(flags >> 8);
    stars_write_record(data, 8, 16, body, write, context);
}
