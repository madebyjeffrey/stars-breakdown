#include "core.h"
#include <string.h>

#define NEW_GAME3_ROWS 9
#define NEW_GAME3_FIRST_CONTROL 0x123
#define NEW_GAME3_FIRST_LABEL 0x3aa
#define NEW_GAME3_READ_ONLY_STYLE 4U

static StarsI16 draw_new_game3_abs(StarsI16 value)
{
    return value < 0 ? (StarsI16)-value : value;
}

static StarsI16 draw_new_game3_format_value(StarsI16 value,
    StarsU8 *text, StarsU16 capacity)
{
    StarsU8 reversed[16];
    StarsI16 number;
    StarsI16 length;
    StarsI16 negative;

    if (capacity == 0U)
        return 0;
    number = value;
    negative = number < 0;
    if (negative)
        number = (StarsI16)-number;
    length = 0;
    do {
        reversed[length++] = (StarsU8)('0' + number % 10);
        number = (StarsI16)(number / 10);
    } while (number != 0 && length < (StarsI16)sizeof reversed);
    if (negative && length < (StarsI16)sizeof reversed)
        reversed[length++] = (StarsU8)'-';
    if (length >= (StarsI16)capacity)
        length = (StarsI16)capacity - 1;
    for (number = 0; number < length; ++number)
        text[number] = reversed[length - number - 1];
    text[length] = 0;
    return length;
}

static signed char draw_new_game3_column_width(
    const StarsDrawNewGame3Input *input, StarsI16 row, StarsI16 column)
{
    StarsU16 index;

    index = (StarsU16)(row * 2 + column);
    if (input->packed_column_widths &&
            index < input->packed_column_width_count)
        return input->packed_column_widths[index];
    return input->column_widths[row][column];
}

/* Genuine source reconstruction of DrawNewGame3, NE 10:9D5E-A20F.  The
 * original's compact signed column table is supplied as named widths; the
 * renderer owns label/value ordering, reference rectangles and read-only
 * button styling. */
void stars_draw_new_game3(StarsDrawNewGame3Input *input, StarsI16 draw_mode)
{
    StarsU16 dc;
    StarsRect control;
    StarsRect button;
    StarsU8 text[64];
    StarsI16 row;
    StarsI16 column;
    StarsI16 label_id;
    StarsI16 length;
    StarsI16 value;
    StarsI16 extent;
    StarsI16 x;
    StarsI16 y;
    StarsI16 button_count;
    StarsI16 value_index;
    StarsI16 button_height;
    StarsU16 button_style;
    StarsI16 width;
    StarsI16 center;
    signed char column_width;
    StarsU8 suffix[16];
    StarsI16 suffix_length;
    StarsRect client;
    StarsU16 old_background_mode;
    StarsU32 old_background_color;

    if (!input || !input->ops)
        return;
    dc = input->hdc;
    if (dc == 0 && input->ops->get_dc)
        dc = input->ops->get_dc(input->ops->context, input->hwnd);
    client.left = client.top = client.right = client.bottom = 0;
    if (input->ops->get_client_rect)
        input->ops->get_client_rect(input->ops->context, input->hwnd,
            &client);
    old_background_mode = 0;
    old_background_color = 0;
    if (input->ops->set_bk_mode)
        old_background_mode = input->ops->set_bk_mode(
            input->ops->context, dc, 2U);
    if (input->ops->set_bk_color)
        old_background_color = input->ops->set_bk_color(
            input->ops->context, dc, input->button_face_color);
    if (input->ops->select_font)
        input->ops->select_font(input->ops->context, dc, input->bold_font);
    length = input->ops->get_string ? input->ops->get_string(
        input->ops->context, 0x0af2, text, (StarsU16)sizeof text) : 0;
    extent = input->ops->text_extent ? input->ops->text_extent(
        input->ops->context, dc, text, length) : 0;
    button_count = 0;
    value_index = 0;
    button_style = input->read_only ? NEW_GAME3_READ_ONLY_STYLE : 0U;
    button_height = (StarsI16)(input->line_height / 2 + 3);
    label_id = NEW_GAME3_FIRST_LABEL;
    control.left = control.top = control.right = control.bottom = 0;
    for (row = 0; row < NEW_GAME3_ROWS; ++row) {
        if (row < 7) {
            control.left = control.top = control.right = control.bottom = 0;
            if (input->ops->get_window_rect)
                input->ops->get_window_rect(input->ops->context,
                    (StarsU16)(NEW_GAME3_FIRST_CONTROL + row), &control);
            if (input->ops->map_window_rect)
                input->ops->map_window_rect(input->ops->context, input->hwnd,
                    &control);
            center = (StarsI16)((control.left + control.right) / 2);
            x = (StarsI16)(center - (input->line_height >> 1));
            y = (StarsI16)(control.top + 2);
        } else {
            x = control.left;
            y = (StarsI16)(control.bottom + 6 +
                (row == 8 ? input->line_height * 3 / 2 : 0));
        }
        length = input->ops->get_string ? input->ops->get_string(
            input->ops->context, (StarsU16)label_id++, text,
            (StarsU16)sizeof text) : 0;
        if (draw_mode == -1 && input->ops->text_out)
            input->ops->text_out(input->ops->context, dc, x, y, text,
                length);
        x = (StarsI16)(x + (input->ops->text_extent ?
            input->ops->text_extent(input->ops->context, dc, text, length) : 0));
        for (column = 0; column < 2; ++column) {
            column_width = draw_new_game3_column_width(input, row, column);
            width = (StarsI16)(draw_new_game3_abs(
                column_width) * extent);
            if (width == 0) {
                ++label_id;
                break;
            }
            value = input->ops->get_value ? input->ops->get_value(
                input->ops->context, value_index++, 0) : 0;
            length = input->ops->format_value ? input->ops->format_value(
                input->ops->context, text, (StarsU16)sizeof text, value) :
                draw_new_game3_format_value(value, text,
                    (StarsU16)sizeof text);
            if (column_width < 0 &&
                    length < (StarsI16)sizeof text - 1) {
                suffix_length = input->ops->get_string ?
                    input->ops->get_string(input->ops->context, 0x0af4U,
                        suffix, (StarsU16)sizeof suffix) : 0;
                if (suffix_length <= 0) {
                    suffix[0] = '%';
                    suffix[1] = 0;
                    suffix_length = 1;
                }
                if (suffix_length > (StarsI16)sizeof text - length - 1)
                    suffix_length = (StarsI16)sizeof text - length - 1;
                memcpy(text + length, suffix, (size_t)suffix_length);
                length = (StarsI16)(length + suffix_length);
                text[length] = 0;
            }
            x = (StarsI16)(x + width);
            if (column_width < 0 &&
                    input->ops->text_extent)
                x = (StarsI16)(x + input->ops->text_extent(
                    input->ops->context, dc, text, length) * 3 / 2);
            if ((draw_mode == -1 || draw_mode == value_index - 1 || row == 8) &&
                    input->ops->right_text)
                input->ops->right_text(input->ops->context, dc, x, y, text,
                    length, width);
            if (input->reference_rects) {
                button.left = (StarsU16)(x + 4);
                button.top = (StarsU16)(y - 3);
                button.right = (StarsU16)(button.left + 15);
                button.bottom = (StarsU16)(button.top + button_height);
                input->reference_rects[button_count] = button;
                input->reference_rects[button_count + 1] = button;
                input->reference_rects[button_count + 1].top =
                    (StarsU16)(button.top + button_height - 1);
                input->reference_rects[button_count + 1].bottom =
                    (StarsU16)(button.bottom + button_height - 1);
                if (draw_mode == -1 && input->ops->draw_button) {
                    input->ops->draw_button(input->ops->context, dc, &button,
                        (StarsU16)(button_style | 0xa0U));
                    input->ops->draw_button(input->ops->context, dc,
                        &input->reference_rects[button_count + 1],
                        (StarsU16)(button_style | 0xa1U));
                }
            }
            button_count = (StarsI16)(button_count + 2);
            length = input->ops->get_string ? input->ops->get_string(
                input->ops->context, (StarsU16)label_id++, text,
                (StarsU16)sizeof text) : 0;
            if (draw_mode == -1 && input->ops->text_out)
                input->ops->text_out(input->ops->context, dc,
                    (StarsI16)(button.right + 4), y, text, length);
            x = (StarsI16)(button.right + 4 +
                (input->ops->text_extent ? input->ops->text_extent(
                    input->ops->context, dc, text, length) : 0));
        }
    }
    if (input->reference_count)
        *input->reference_count = button_count;
    if (input->ops->set_bk_color)
        input->ops->set_bk_color(input->ops->context, dc,
            old_background_color);
    if (input->ops->set_bk_mode)
        input->ops->set_bk_mode(input->ops->context, dc,
            old_background_mode);
    if (input->hdc == 0 && input->ops->release_dc)
        input->ops->release_dc(input->ops->context, input->hwnd, dc);
}
