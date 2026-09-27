#include "core.h"
#include <string.h>

#define NEW_GAME2_MAX_PLAYERS 16
#define NEW_GAME2_TYPE_MASK 3U
#define NEW_GAME2_RACE_SHIFT 2
#define NEW_GAME2_ROW_LABEL_FORMAT 0x262U
#define NEW_GAME2_INACTIVE_PLAYER_STRING 0x20cU
#define NEW_GAME2_GENERIC_PLAYER_STRING 0x20dU
#define NEW_GAME2_SPECIAL_PLAYER_STRING 0x20bU
#define NEW_GAME2_EMPTY_PLAYER_STRING 0x30cU
#define NEW_GAME2_RACE_FORMAT 0x310U
#define NEW_GAME2_FILE_FORMAT 0x311U
#define NEW_GAME2_AI_FORMAT 0x312U
#define NEW_GAME2_DEAD_STRING 800U
#define NEW_GAME2_OPAQUE 1U
#define NEW_GAME2_PLAYER_FILE_MODE 0x0040UL

static const StarsU8 draw_new_game2_space[] = { ' ', 0 };

static StarsI16 draw_new_game2_length(const StarsU8 *text)
{
    return text ? (StarsI16)strlen((const char *)text) : 0;
}

static void draw_new_game2_copy(StarsU8 *destination, StarsU16 capacity,
    const StarsU8 *source)
{
    StarsU16 index;

    if (capacity == 0U)
        return;
    for (index = 0; index + 1U < capacity && source && source[index] != 0U;
         ++index)
        destination[index] = source[index];
    destination[index] = 0;
}

static StarsI16 draw_new_game2_decimal(StarsU8 *destination, StarsU16 capacity,
    StarsI16 value)
{
    StarsU8 reverse[8];
    StarsU16 count = 0U;
    StarsU16 index;
    StarsI16 negative = 0;

    if (capacity == 0U)
        return 0;
    if (value < 0) {
        negative = 1;
        value = (StarsI16)-value;
    }
    do {
        if (count >= (StarsU16)sizeof reverse)
            break;
        reverse[count++] = (StarsU8)('0' + value % 10);
        value = (StarsI16)(value / 10);
    } while (value != 0);
    if (negative && count < (StarsU16)sizeof reverse)
        reverse[count++] = '-';
    if (count >= capacity)
        count = (StarsU16)(capacity - 1U);
    for (index = 0U; index < count; ++index)
        destination[index] = reverse[count - index - 1U];
    destination[count] = 0;
    return (StarsI16)count;
}

static StarsI16 draw_new_game2_append(StarsU8 *destination, StarsU16 capacity,
    StarsI16 length, const StarsU8 *source)
{
    StarsU16 remaining;

    if (length < 0 || (StarsU16)length >= capacity)
        return length;
    remaining = (StarsU16)(capacity - (StarsU16)length);
    draw_new_game2_copy(destination + length, remaining, source);
    return (StarsI16)((StarsU16)length + draw_new_game2_length(
        destination + length));
}

static StarsI16 draw_new_game2_format(const StarsDrawNewGame2Input *input,
    StarsU8 *destination, StarsU16 capacity, StarsU16 format_id,
    const StarsU8 *first, const StarsU8 *second)
{
    if (input->ops->format_text)
        return input->ops->format_text(input->ops->context, destination,
            capacity, format_id, first, second);
    draw_new_game2_copy(destination, capacity, first);
    if (second && second[0] != 0U) {
        StarsI16 length = draw_new_game2_length(destination);
        length = draw_new_game2_append(destination, capacity, length,
            draw_new_game2_space);
        length = draw_new_game2_append(destination, capacity, length, second);
        return length;
    }
    return draw_new_game2_length(destination);
}

static StarsU8 draw_new_game2_packed_type(
    const StarsDrawNewGame2Input *input, StarsI16 row)
{
    if (input->packed_player_types && row >= 0 &&
            (StarsU16)row < input->packed_player_type_count)
        return input->packed_player_types[row];
    return input->players[row].type;
}

static const StarsU8 *draw_new_game2_packed_file(
    const StarsDrawNewGame2Input *input, StarsI16 row, StarsI16 race)
{
    StarsU16 stride;

    if (input->packed_race_files && race >= 0 &&
            (StarsU16)race < input->packed_race_file_count) {
        stride = input->packed_race_file_stride;
        if (stride == 0U)
            stride = 13U;
        return input->packed_race_files + (StarsU16)race * stride;
    }
    return input->players[row].file_name;
}

/* Genuine source reconstruction of DrawNewGame2, NE 10:9556-99F3.  Player
 * type decoding, editable/read-only text choices, dead-player color, diamond
 * placement and row spacing are explicit typed operations. */
void stars_draw_new_game2(StarsDrawNewGame2Input *input, StarsI16 draw_mode)
{
    StarsU16 dc;
    StarsU8 text[96];
    StarsU8 label[64];
    StarsRect diamond;
    StarsI16 row;
    StarsI16 type;
    StarsI16 race;
    StarsI16 y;
    StarsI16 width;
    StarsI16 length;
    StarsU16 old_mode = 0U;
    const StarsU8 *compressed;

    (void)draw_mode;
    if (!input || !input->ops || !input->players)
        return;
    dc = input->hdc;
    if (dc == 0 && input->ops->get_dc)
        dc = input->ops->get_dc(input->ops->context, input->hwnd);
    if (input->ops->set_bk_mode)
        old_mode = input->ops->set_bk_mode(input->ops->context, dc,
            NEW_GAME2_OPAQUE);
    if (input->ops->select_font)
        input->ops->select_font(input->ops->context, dc, input->bold_font);
    compressed = input->ops->compressed_string ?
        input->ops->compressed_string(input->ops->context, 0x319) :
        (const StarsU8 *)0;
    width = compressed && input->ops->text_extent ? input->ops->text_extent(
        input->ops->context, dc, compressed, 11) : 0;
    input->diamond_x = (StarsI16)(width + 12);
    y = 6;
    for (row = 0; row < NEW_GAME2_MAX_PLAYERS &&
            (!input->read_only || row < input->player_count); ++row) {
        type = draw_new_game2_packed_type(input, row) & NEW_GAME2_TYPE_MASK;
        race = (draw_new_game2_packed_type(input, row) >>
            NEW_GAME2_RACE_SHIFT) & 0x0f;
        (void)memset(text, 0, sizeof text);
        if (!input->read_only) {
            if (type == 0) {
                length = input->ops->get_string ? input->ops->get_string(
                    input->ops->context, NEW_GAME2_EMPTY_PLAYER_STRING, text,
                    (StarsU16)sizeof text) : 0;
            } else if (type == 1) {
                if (race < 6) {
                    compressed = input->ops->compressed_string ?
                        input->ops->compressed_string(input->ops->context,
                            (StarsU16)(0x567 + race)) : (const StarsU8 *)0;
                    if (input->ops->format_text)
                        length = draw_new_game2_format(input, text,
                            (StarsU16)sizeof text, NEW_GAME2_RACE_FORMAT,
                            compressed, (const StarsU8 *)0);
                    else
                        length = input->ops->get_string ? input->ops->get_string(
                            input->ops->context, (StarsU16)(0x567 + race), text,
                            (StarsU16)sizeof text) : 0;
                } else {
                    length = input->ops->get_string ? input->ops->get_string(
                        input->ops->context,
                        race == 6 ? NEW_GAME2_SPECIAL_PLAYER_STRING :
                        NEW_GAME2_GENERIC_PLAYER_STRING, text,
                        (StarsU16)sizeof text) : 0;
                }
            } else if (type == 2) {
                if (draw_new_game2_packed_file(input, row, race)[0] != 0U &&
                        (input->game_flags & NEW_GAME2_PLAYER_FILE_MODE) != 0U)
                    length = draw_new_game2_format(input, text,
                        (StarsU16)sizeof text, 0x0aeeU,
                        draw_new_game2_packed_file(input, row, race),
                        (const StarsU8 *)0);
                else if (draw_new_game2_packed_file(input, row, race)[0] != 0U)
                    length = draw_new_game2_format(input, text,
                        (StarsU16)sizeof text, NEW_GAME2_FILE_FORMAT,
                        input->players[row].name,
                        draw_new_game2_packed_file(input, row, race));
                else
                    length = draw_new_game2_format(input, text,
                        (StarsU16)sizeof text, NEW_GAME2_RACE_FORMAT,
                        input->players[row].name, (const StarsU8 *)0);
            } else if (type == 3) {
                const StarsU8 *group_label;
                const StarsU8 *level_label;

                group_label = input->ops->ai_group_label ?
                    input->ops->ai_group_label(input->ops->context,
                        (StarsU16)((draw_new_game2_packed_type(input, row) >>
                        NEW_GAME2_RACE_SHIFT) & 7U)) : (const StarsU8 *)0;
                level_label = input->ops->ai_level_label ?
                    input->ops->ai_level_label(input->ops->context,
                        (StarsU16)(draw_new_game2_packed_type(input, row) >>
                        5)) : (const StarsU8 *)0;
                if (input->ops->format_text && group_label && level_label)
                    length = draw_new_game2_format(input, text,
                        (StarsU16)sizeof text, NEW_GAME2_AI_FORMAT,
                        group_label, level_label);
                else
                    length = input->ops->get_string ? input->ops->get_string(
                        input->ops->context, NEW_GAME2_GENERIC_PLAYER_STRING,
                        text, (StarsU16)sizeof text) : 0;
            } else {
                length = input->ops->get_string ? input->ops->get_string(
                    input->ops->context, NEW_GAME2_GENERIC_PLAYER_STRING, text,
                    (StarsU16)sizeof text) : 0;
            }
        } else if (!input->players[row].active) {
            length = input->ops->get_string ? input->ops->get_string(
                input->ops->context, NEW_GAME2_INACTIVE_PLAYER_STRING, text,
                (StarsU16)sizeof text) : 0;
        } else {
            draw_new_game2_copy(text, (StarsU16)sizeof text,
                input->players[row].name);
            length = draw_new_game2_length(text);
            if (input->players[row].dead) {
                if (input->ops->compressed_string) {
                    compressed = input->ops->compressed_string(
                        input->ops->context, NEW_GAME2_DEAD_STRING);
                    if (compressed && length + draw_new_game2_length(compressed) +
                            2 < (StarsI16)sizeof text) {
                        text[length++] = ' ';
                        text[length++] = '(';
                        draw_new_game2_copy(text + length,
                            (StarsU16)(sizeof text - length), compressed);
                        length = draw_new_game2_length(text);
                        text[length++] = ')';
                        text[length] = 0;
                    }
                }
                if (input->ops->set_text_color)
                    input->ops->set_text_color(input->ops->context, dc,
                        input->dead_color);
            }
        }
        diamond.left = (StarsU16)input->diamond_x;
        diamond.top = (StarsU16)y;
        diamond.right = (StarsU16)(input->diamond_x + input->line_height + 1);
        diamond.bottom = (StarsU16)(y + input->line_height + 1);
        if (input->ops->right_text) {
            length = draw_new_game2_decimal(label, (StarsU16)sizeof label,
                (StarsI16)(row + 1));
            label[length++] = ':';
            label[length] = 0;
            input->ops->right_text(input->ops->context, dc,
                (StarsI16)(input->diamond_x - 6), y, label,
                length, 0);
        }
        if (input->ops->draw_diamond)
            input->ops->draw_diamond(input->ops->context, dc, &diamond);
        if (input->ops->text_out) {
            input->ops->text_out(input->ops->context, dc,
                (StarsI16)(diamond.right + 6), y, text, length);
        }
        if (input->ops->set_text_color)
            input->ops->set_text_color(input->ops->context, dc,
                input->text_color);
        y = (StarsI16)(y + input->line_height + 4);
    }
    if (input->ops->set_bk_mode)
        (void)input->ops->set_bk_mode(input->ops->context, dc, old_mode);
    if (input->hdc == 0 && input->ops->release_dc)
        input->ops->release_dc(input->ops->context, input->hwnd, dc);
}
