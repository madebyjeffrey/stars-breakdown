/* Original text utilities expressed over a complete 64 KiB memory image.
 * The pointer slot and output can alias text; loads/stores retain their order.
 * Far source pointers are adapted to this one supplied data image. */
#include "core.h"

static StarsU16 word_at(const StarsU8 *data, StarsU16 offset)
{
    return (StarsU16)(data[offset] | ((StarsU16)data[(StarsU16)(offset + 1U)] << 8));
}

static void put_word(StarsU8 *data, StarsU16 offset, StarsU16 value)
{
    data[offset] = (StarsU8)value;
    data[(StarsU16)(offset + 1U)] = (StarsU8)(value >> 8);
}

/* NE 9:280C-2841. Advance only over literal spaces before unsigned end. */
void stars_add_back_trailing_spaces(StarsU8 *data, StarsU16 cursor_slot, StarsU16 end)
{
    StarsU16 cursor;
    while ((cursor = word_at(data, cursor_slot)) < end && data[cursor] == ' ')
        put_word(data, cursor_slot, (StarsU16)(cursor + 1U));
}

/* NE 9:28C6-28FC. The string itself is not NUL-terminated or modified. */
void stars_chop_trailing_spaces(StarsU8 *data, StarsU16 start, StarsU16 end_slot)
{
    StarsU16 end;
    while ((end = word_at(data, end_slot)) > start && data[(StarsU16)(end - 1U)] == ' ')
        put_word(data, end_slot, (StarsU16)(end - 1U));
}

/* NE 9:2842-28C6. Strip spaces, one non-space run, then spaces again. */
void stars_chop_last_word(StarsU8 *data, StarsU16 start, StarsU16 end_slot)
{
    StarsU16 end;
    stars_chop_trailing_spaces(data, start, end_slot);
    while ((end = word_at(data, end_slot)) > start && data[(StarsU16)(end - 1U)] != ' ')
        put_word(data, end_slot, (StarsU16)(end - 1U));
    stars_chop_trailing_spaces(data, start, end_slot);
}

/* NE 9:5830-58D7. Only I, V and X are used, even for values >= 40. */
void stars_int_to_roman(StarsU8 *data, StarsI16 value, StarsU16 output)
{
    if (value > 0) {
        while (value >= 10) {
            data[output] = 'X'; output = (StarsU16)(output + 1U);
            value -= 10;
        }
        if (value == 9) {
            data[output] = 'I'; output = (StarsU16)(output + 1U);
            data[output] = 'X'; output = (StarsU16)(output + 1U);
        } else {
            if (value >= 4) {
                if (value == 4) {
                    data[output] = 'I'; output = (StarsU16)(output + 1U);
                }
                data[output] = 'V'; output = (StarsU16)(output + 1U);
                value -= 5;
            }
            while (value > 0) {
                data[output] = 'I'; output = (StarsU16)(output + 1U);
                --value;
            }
        }
    }
    data[output] = 0;
}

static void store_number(StarsU8 *data, StarsU16 output, StarsI16 index, StarsU32 number)
{
    StarsU16 destination = (StarsU16)(output + (StarsU16)((StarsU16)index * 4U));
    put_word(data, destination, (StarsU16)number);
    put_word(data, (StarsU16)(destination + 2U), (StarsU16)(number >> 16));
}

/* NE 9:6986-6AA5. Only ASCII digits and spaces; accumulation wraps at 32 bits.
 * An error returns -1 without undoing previously completed output numbers. */
StarsI16 stars_parse_numbers(StarsU8 *data, StarsU16 input, StarsU16 output, StarsI16 maximum)
{
    StarsI16 count = 0;
    StarsU32 number = 0;
    StarsU8 ch;
    int in_number = 0;
    while (count < maximum && (ch = data[input]) != 0) {
        if (ch != ' ' && (ch < '0' || ch > '9')) return -1;
        if (ch != ' ') {
            in_number = 1;
            number = (StarsU32)(number * 10U + (ch - '0'));
        } else if (in_number) {
            store_number(data, output, count++, number);
            number = 0;
            in_number = 0;
        }
        input = (StarsU16)(input + 1U);
    }
    if (in_number) store_number(data, output, count++, number);
    return count;
}

/* NE 9:68BA-6985. Leading spaces are skipped. End marker becomes NUL.
 * CRLF advances two bytes; every other end marker (including NUL) advances one. */
StarsU32 stars_get_line(StarsU8 *data, StarsU16 pointer_slot)
{
    StarsU16 cursor = word_at(data, pointer_slot), start, next;
    StarsU16 selector = word_at(data, (StarsU16)(pointer_slot + 2U));
    while (data[cursor] == ' ') cursor = (StarsU16)(cursor + 1U);
    start = cursor;
    while (data[cursor] && data[cursor] != '\r' && data[cursor] != '\n')
        cursor = (StarsU16)(cursor + 1U);
    next = (StarsU16)(cursor + ((data[cursor] == '\r' &&
            data[(StarsU16)(cursor + 1U)] == '\n') ? 2U : 1U));
    put_word(data, pointer_slot, next);
    put_word(data, (StarsU16)(pointer_slot + 2U), selector);
    data[cursor] = 0;
    return ((StarsU32)selector << 16) | start;
}
