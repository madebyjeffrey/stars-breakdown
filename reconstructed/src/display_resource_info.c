#include "core.h"
#include <stdio.h>
#include <string.h>

static StarsU16 resource_text_length(const StarsU8 *text)
{
    return text ? (StarsU16)strlen((const char *)text) : 0U;
}

/* NE 19:3378-355B.  Render the resource popup's title and variable rows,
 * retaining the original string-id dispatch and early equal-value exit. */
StarsPoint stars_display_resource_info(const StarsDisplayResourceInfoInput *input)
{
    StarsPoint result;
    StarsI16 row;
    StarsI16 row_count;
    StarsI16 next_string;
    StarsI16 x;
    StarsI16 y;
    StarsI16 height;
    StarsU16 length;
    StarsU8 formatted[32];
    const StarsU8 *text;

    result.x = 0;
    result.y = 0;
    if (!input)
        return result;
    row_count = (StarsI16)(input->race_stat == 8 ? 9 : 8);
    next_string = 0xed;
    x = 4;
    y = 2;
    height = 4;
    if (input->select_font)
        input->select_font(input->context, input->hdc, input->font_large);
    if (input->print && input->get_string && input->center_text) {
        text = input->get_string(input->context, 0x233U);
        length = resource_text_length(text);
        input->center_text(input->context, input->hdc,
            (StarsI16)((input->width - 8) >> 1), y, text, length);
    }
    y = (StarsI16)(y + input->line_height + 4);
    for (row = 0; row <= row_count; ++row) {
        if (input->select_font)
            input->select_font(input->context, input->hdc, input->font_large);
        text = 0;
        switch (row) {
        case 0:
            if (input->get_string)
                text = input->get_string(input->context,
                    (StarsU16)input->planet_id);
            break;
        case 1:
        case 3:
        case 5:
        case 8:
        case 9:
            if (input->select_font)
                input->select_font(input->context, input->hdc,
                    input->font_small);
            if (input->get_string)
                text = input->get_string(input->context,
                    (StarsU16)next_string++);
            break;
        case 2:
            if (input->format_percent)
                input->format_percent(input->context,
                    input->planet_variable, formatted,
                    (StarsU16)sizeof(formatted));
            else
                sprintf((char *)formatted, "%d%%",
                    (int)input->planet_variable);
            text = formatted;
            break;
        case 4:
            if (input->resource_text_value > 0) {
                if (input->format_percent)
                    input->format_percent(input->context,
                        input->resource_text_value, formatted,
                        (StarsU16)sizeof(formatted));
                else
                    sprintf((char *)formatted, "%d%%",
                        (int)input->resource_text_value);
                text = formatted;
            } else if (input->get_string) {
                text = input->get_string(input->context, 0x263U);
            }
            break;
        case 6:
            if (input->planet_value == input->planet_variable) {
                result.x = (StarsI16)(height + 4);
                result.y = (StarsI16)(y + input->line_height + 2);
                return result;
            }
            if (input->select_font)
                input->select_font(input->context, input->hdc,
                    input->font_small);
            if (input->get_string)
                text = input->get_string(input->context,
                    (StarsU16)next_string++);
            break;
        case 7:
            if (input->format_percent)
                input->format_percent(input->context, input->planet_value,
                    formatted, (StarsU16)sizeof(formatted));
            else
                sprintf((char *)formatted, "%d%%",
                    (int)input->planet_value);
            text = formatted;
            break;
        default:
            break;
        }
        if (input->wrap_text && text)
            input->wrap_text(input->context, input->hdc, &x, &y, text,
                0, 4, (StarsI16)(input->width - 8), &height, input->print);
    }
    result.x = (StarsI16)(height + 4);
    result.y = (StarsI16)(y + input->line_height + 2);
    return result;
}
