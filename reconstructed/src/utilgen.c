/* Reviewed against NE 9 instruction ranges; see analysis/reconstruction/core.md.
 * Unsigned operations implement the original 32-bit wrap explicitly. */
#include "core.h"
#include <math.h>
#include <string.h>
static const StarsI16 seed_table[128] = {
#include "seed_table.inc"
};

static StarsU32 sign_extend_word(StarsU16 value)
{
    return value & 0x8000U ? (StarsU32)value | 0xffff0000UL : (StarsU32)value;
}

static StarsI16 signed_word_value(StarsU16 value)
{
    return value < 32768U ? (StarsI16)value : (StarsI16)((StarsI32)value - 65536L);
}

static StarsU32 serial_code(const StarsU8 *serial, StarsU16 index)
{
    StarsI16 ch = (StarsI16)(signed char)serial[index];
    StarsU16 value;
    if (ch < 'A' || ch > 'Z')
        value = (StarsU16)((StarsU16)ch - 0x16U);
    else
        value = (StarsU16)((StarsU16)ch - (StarsU16)'A');
    if (value & 0x8000U || (value < 1U && value < 0x20U))
        value = (StarsU16)(value ^ 0x15U);
    return sign_extend_word(value);
}

/* NE 9:1FFA-215F, CopyFile. The original uses an 0x800-byte stack buffer
 * and always completes the source stream before restoring the error scope. */
StarsI16 stars_copy_file(const StarsU8 *source, const StarsU8 *destination,
    const StarsStreamApi *api, void *context, StarsFileLength file_length)
{
    StarsU8 buffer[0x800];
    StarsU16 source_handle, destination_handle, count, written;
    StarsU32 remaining;
    StarsI16 error = 0;
    if (!api || !api->open || !api->close || !api->read || !api->write ||
            !file_length) return 0;
    source_handle = (StarsU16)api->open(context, source, 0x20U, 0);
    if (source_handle == 0xffffU) return 0;
    destination_handle = (StarsU16)api->open(context, destination, 0x1012U, 0);
    if (destination_handle == 0xffffU) {
        api->close(context, (StarsI16)source_handle);
        return 0;
    }
    remaining = file_length(context, (StarsI16)source_handle);
    while (remaining >= 0x800UL) {
        count = api->read(context, (StarsI16)source_handle, buffer, 0x800U);
        if (count != 0x800U) { error = 1; break; }
        written = api->write(context, (StarsI16)destination_handle, buffer,
            0x800U);
        if (written != 0x800U) { error = 1; break; }
        remaining -= 0x800UL;
    }
    if (!error && remaining != 0U) {
        count = api->read(context, (StarsI16)source_handle, buffer,
            (StarsU16)remaining);
        if (count != (StarsU16)remaining) error = 1;
        else {
            written = api->write(context, (StarsI16)destination_handle,
                buffer, (StarsU16)remaining);
            if (written != (StarsU16)remaining) error = 1;
        }
    }
    api->close(context, (StarsI16)source_handle);
    api->close(context, (StarsI16)destination_handle);
    return (StarsI16)!error;
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

/* NE 9:188A-1A4D, RandomSeedDlg.  Dialog controls are represented by the
 * text/randomize/end callbacks; the Win16 message results and decimal parser
 * retain the original behavior. */
StarsI16 stars_random_seed_dialog(StarsU16 hwnd, StarsU16 message,
    StarsU16 wparam, StarsU32 lparam, const StarsRandomSeedDialogOps *ops,
    void *context)
{
    const StarsU8 *text;
    StarsU32 seed;
    StarsU16 high;
    if (message == 0x0014U) return 1;
    if (message == 0x0019U) {
        high = (StarsU16)(lparam >> 16);
        return high == 6U ? 1 : 0;
    }
    if (message == 0x0110U) return 1;
    if (message != 0x0111U || (wparam != 1U && wparam != 2U)) return 0;
    if (wparam == 1U) {
        text = ops && ops->text ? ops->text(context, hwnd, 0x10cU) : 0;
        seed = 0;
        if (!text) return 1;
        while (*text >= '0' && *text <= '9') {
            seed = seed * 10U + (StarsU32)(*text - '0');
            ++text;
        }
        if (seed == 0U) return 1;
        if (ops && ops->randomize) ops->randomize(context, seed);
    }
    if (ops && ops->end) ops->end(context, hwnd, (StarsI16)(wparam == 1U));
    return 1;
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

/* NE 9:62F8-6612, FValidSerialNo. The caller supplies the saved global RNG
 * state; all random draws are restored before the checksum result returns. */
StarsI16 stars_validate_serial_number(const StarsU8 *serial, StarsSeeds *state,
    StarsU32 *serial_value_out)
{
    StarsSeeds saved = *state;
    StarsU32 serial_value, random_value = 0, shifted;
    StarsU16 i, skip;
    serial_value = serial_code(serial, 0);
    if (serial_value < 0x20U) serial_value ^= 0x15U;
    serial_value = serial_value * 36U + serial_code(serial, 1);
    serial_value = serial_value * 36U + serial_code(serial, 4);
    serial_value = serial_value * 36U + serial_code(serial, 7);
    serial_value = serial_value * 36U + serial_code(serial, 3);
    if (serial_value_out) *serial_value_out = serial_value;
    stars_randomize2(state, serial_value);
    shifted = serial_value >> 14;
    for (i = 0; i < 3U; ++i) {
        skip = (StarsU16)(shifted & 15U);
        while (skip--) (void)stars_random(state, 256);
        random_value = (random_value << 8) + (StarsU16)stars_random(state, 256);
        shifted >>= 4;
    }
    *state = saved;
    if (random_value % 36U != serial_code(serial, 2)) return 0;
    random_value /= 36U;
    if (random_value % 36U != serial_code(serial, 5)) return 0;
    random_value /= 36U;
    return (StarsI16)(random_value % 36U == serial_code(serial, 6));
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

/* NE 8:3FE4-4070, DGetDistance. Coordinate subtraction is a 16-bit
 * operation before the x87 square-root calculation. */
double stars_get_distance(StarsI16 x1, StarsI16 y1, StarsI16 x2, StarsI16 y2)
{
    StarsI16 dx = signed_word_value((StarsU16)((StarsU16)x2 - (StarsU16)x1));
    StarsI16 dy = signed_word_value((StarsU16)((StarsU16)y2 - (StarsU16)y1));
    return sqrt((double)dx * (double)dx + (double)dy * (double)dy);
}

/* NE 9:53B4-5830, FIntersectCircleLine. The line coordinates are words;
 * the returned distances are signed words measured from line endpoint 1. */
StarsI16 stars_intersect_circle_line(StarsI16 l1x, StarsI16 l1y,
    StarsI16 l2x, StarsI16 l2y, StarsI16 cx, StarsI16 cy,
    StarsU32 radius_squared, StarsI16 maximum, StarsI16 *start,
    StarsI16 *end)
{
    StarsI16 dx = signed_word_value((StarsU16)((StarsU16)l2x - (StarsU16)l1x));
    StarsI16 dy = signed_word_value((StarsU16)((StarsU16)l2y - (StarsU16)l1y));
    double denominator = (double)dx * dx + (double)dy * dy;
    double foot_x, foot_y, projection, distance_squared, delta;
    StarsI16 center_distance, half_width, result_start, result_end;
    if (!denominator) {
        foot_x = l1x; foot_y = l1y;
    } else {
        foot_x = ((double)dy * dy * l1x + (double)dx * dx * cx +
            (double)dx * dy * (cy - l1y)) / denominator;
        foot_y = dx ? (foot_x - l1x) * dy / dx + l1y : l1y;
    }
    distance_squared = (foot_x - cx) * (foot_x - cx) +
        (foot_y - cy) * (foot_y - cy);
    if (!(distance_squared < (double)radius_squared)) return 0;
    projection = sqrt((foot_x - l1x) * (foot_x - l1x) +
        (foot_y - l1y) * (foot_y - l1y));
    center_distance = (StarsI16)projection;
    delta = sqrt((double)radius_squared - distance_squared);
    half_width = (StarsI16)delta;
    if ((dx > 0 && foot_x < l1x) || (dx < 0 && l1x < foot_x) ||
        (!dx && ((dy > 0 && foot_y < l1y) || (dy <= 0 && l1y < foot_y))))
        center_distance = (StarsI16)(-center_distance);
    result_start = (StarsI16)(center_distance - half_width);
    if (result_start < 0) result_start = 0;
    result_end = (StarsI16)(center_distance + half_width);
    if (result_end > maximum) result_end = maximum;
    *start = result_start;
    *end = result_end;
    return (StarsI16)(result_end >= 1 && maximum > result_start);
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

/* NE 9:5FC0-636C, GetDiskSerialNumber.  DOS volume and free-space queries
 * are represented by explicit host records; the original packed serial and
 * label encoding is retained byte-for-byte in the returned 11-byte record. */
StarsU16 stars_get_disk_serial_number(const StarsDiskIdentity *drives,
    StarsU8 *output)
{
    StarsU32 packed;
    StarsU16 serial;
    StarsU16 i;
    StarsU16 cursor = 0;
    StarsU8 nibble;
    if (!drives || !output) return 0;
    output[10] = 0;
    for (i = 0; i < 2; ++i) {
        if (drives[i].present) {
            packed = 0;
            if (drives[i].label) {
                StarsU16 limit = i == 0 ? 8U : 8U;
                StarsU16 j;
                for (j = 0; j < limit && drives[i].label[j]; ++j)
                    packed = (packed << (i == 0 ? 4 : 3)) |
                        (StarsU32)(drives[i].label[j] &
                            (i == 0 ? 0x0fU : 0x07U));
            }
            output[cursor++] = (StarsU8)packed;
            output[cursor++] = (StarsU8)(packed >> 8);
            output[cursor++] = (StarsU8)(packed >> 16);
            if (i == 0) output[cursor++] = (StarsU8)(packed >> 24);
            if (i == 0) {
                serial = (StarsU16)((drives[i].serial_word_a << 12) |
                    ((drives[i].serial_word_a >> 5 & 7U) << 9) |
                    ((drives[i].serial_word_b & 0x0fU) << 5) |
                    ((drives[i].serial_word_b >> 5 & 0x0fU) << 1) |
                    (drives[i].serial_word_b >> 11 & 1U));
                output[cursor++] = (StarsU8)serial;
                output[cursor++] = (StarsU8)(serial >> 8);
            } else {
                serial = (StarsU16)(((drives[i].serial_word_a & 3U) << 6) |
                    ((drives[i].serial_word_b & 7U) << 3) |
                    (drives[i].serial_word_b >> 5 & 7U));
                output[cursor++] = (StarsU8)serial;
            }
        } else {
            output[cursor++] = 0xa5U;
            output[cursor++] = 0xdcu;
            output[cursor++] = 0xa6U;
            if (i == 0) output[cursor++] = 0x59U;
            output[cursor++] = 0x7aU;
            if (i == 0) output[cursor++] = 0xc5U;
        }
        nibble = (StarsU8)(drives[i].free_nibble & 0x0fU);
        output[10] = (StarsU8)((i == 0 ? 0U : output[10] * 16U) + nibble);
    }
    return 11;
}

static void put_dib_word(StarsU8 *output, StarsU16 offset, StarsU16 value)
{
    output[offset] = (StarsU8)value;
    output[offset + 1U] = (StarsU8)(value >> 8);
}

static void put_dib_long(StarsU8 *output, StarsU16 offset, StarsU32 value)
{
    output[offset] = (StarsU8)value;
    output[offset + 1U] = (StarsU8)(value >> 8);
    output[offset + 2U] = (StarsU8)(value >> 16);
    output[offset + 3U] = (StarsU8)(value >> 24);
}

/* NE 9:4E90-5220, DibFromBitmap.  GDI handles/GetDIBits are replaced by an
 * explicit source bitmap and destination span; the generated DIB header,
 * palette placement, style, bit-depth default, and pixel copy are retained. */
StarsU32 stars_dib_from_bitmap(const StarsBitmapSource *bitmap,
    StarsU32 style, StarsU16 bits, const StarsU8 *palette,
    StarsU16 palette_bytes, StarsU8 *output, StarsU32 capacity)
{
    StarsU16 actual_bits;
    StarsU32 row_bytes, image_bytes, total;
    if (!bitmap || !output) return 0;
    if (bitmap->width <= 0 || bitmap->height <= 0) return 0;
    actual_bits = bits ? bits : (StarsU16)(bitmap->planes * bitmap->bits);
    row_bytes = (StarsU32)((((StarsU32)bitmap->width * actual_bits) + 31U) / 32U) * 4U;
    image_bytes = bitmap->pixel_bytes ? bitmap->pixel_bytes :
        row_bytes * (StarsU32)bitmap->height;
    total = 40U + palette_bytes + image_bytes;
    if (capacity < total) return 0;
    put_dib_long(output, 0, 40U);
    put_dib_long(output, 4, (StarsU32)bitmap->width);
    put_dib_long(output, 8, (StarsU32)bitmap->height);
    put_dib_word(output, 12, bitmap->planes ? bitmap->planes : 1U);
    put_dib_word(output, 14, actual_bits);
    put_dib_long(output, 16, style);
    put_dib_long(output, 20, image_bytes);
    put_dib_long(output, 24, 0U);
    put_dib_long(output, 28, 0U);
    put_dib_long(output, 32, palette_bytes / 4U);
    put_dib_long(output, 36, 0U);
    if (palette_bytes && palette) memcpy(output + 40, palette, palette_bytes);
    if (image_bytes && bitmap->pixels)
        memcpy(output + 40U + palette_bytes, bitmap->pixels, image_bytes);
    return total;
}
