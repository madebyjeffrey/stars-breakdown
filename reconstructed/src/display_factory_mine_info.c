#include "core.h"
#include <stdio.h>
#include <string.h>

static StarsU16 factory_text_length(const StarsU8 *text)
{
    return text ? (StarsU16)strlen((const char *)text) : 0U;
}

/* NE 19:3110-3378.  Render the factory/mine popup with the original
 * title, suffix, race-stat, and nine-row dispatch. */
StarsPoint stars_display_factory_mine_info(
    const StarsDisplayFactoryMineInfoInput *input)
{
    StarsPoint result;
    StarsI16 row;
    StarsI16 next_string;
    StarsI16 x;
    StarsI16 y;
    StarsI16 height;
    StarsU16 suffix_one;
    StarsU16 suffix_many;
    StarsU8 formatted[48];
    const StarsU8 *text;

    result.x = 0;
    result.y = 0;
    if (!input)
        return result;
    next_string = 0xcd;
    x = 4;
    y = 2;
    height = 4;
    suffix_one = input->planet_max == 0 ? 0xc3fU : 0xc2dU;
    suffix_many = input->planet_max == 0 ? 0xc44U : 0xc35U;
    if (input->select_font)
        input->select_font(input->context, input->hdc, input->font_large);
    if (input->print && input->format_title && input->center_text) {
        input->format_title(input->context, suffix_one, suffix_many,
            formatted, (StarsU16)sizeof(formatted));
        input->center_text(input->context, input->hdc,
            (StarsI16)((input->width - 8) >> 1), y, formatted,
            factory_text_length(formatted));
    }
    y = (StarsI16)(y + input->line_height + 4);
    if (input->race_stat == 8) {
        if (input->select_font)
            input->select_font(input->context, input->hdc,
                input->font_small);
        text = input->get_string ? input->get_string(input->context,
            input->planet_max == 0 ? 0x4ecU : 0x4ebU) : 0;
        if (input->wrap_text && text)
            input->wrap_text(input->context, input->hdc, &x, &y, text,
                0, 4, (StarsI16)(input->width - 8), &height, input->print);
    } else {
        for (row = 0; row < 9; ++row) {
            if (input->select_font)
                input->select_font(input->context, input->hdc,
                    input->font_large);
            text = 0;
            switch (row) {
            case 0:
            case 2:
            case 4:
            case 6:
            case 8:
                if (input->select_font)
                    input->select_font(input->context, input->hdc,
                        input->font_small);
                if (input->get_string)
                    text = input->get_string(input->context,
                        (StarsU16)next_string++);
                break;
            case 1:
                if (input->format_value) {
                    input->format_value(input->context,
                        input->planet_value, input->planet_value == 1 ?
                        suffix_one : suffix_many, formatted,
                        (StarsU16)sizeof(formatted));
                    text = formatted;
                }
                break;
            case 3:
                if (input->get_planet_name)
                    text = input->get_planet_name(input->context,
                        input->planet_id);
                break;
            case 5:
                if (input->format_value) {
                    input->format_value(input->context,
                        input->planet_variable, input->planet_variable == 1 ?
                        suffix_one : suffix_many, formatted,
                        (StarsU16)sizeof(formatted));
                    text = formatted;
                }
                break;
            case 7:
                if (input->format_percent) {
                    input->format_percent(input->context,
                        input->planet_max, formatted,
                        (StarsU16)sizeof(formatted));
                    text = formatted;
                }
                break;
            default:
                break;
            }
            if (input->wrap_text && text)
                input->wrap_text(input->context, input->hdc, &x, &y, text,
                    0, 4, (StarsI16)(input->width - 8), &height,
                    input->print);
        }
    }
    result.x = (StarsI16)(height + 4);
    result.y = (StarsI16)(y + input->line_height + 2);
    return result;
}
