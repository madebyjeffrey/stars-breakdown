#include <string.h>
#include "core.h"

enum {
    SHIP_CARGO_TILE_RESET_BIT = 0x0800,
    SHIP_CARGO_TILE_HIDE_BIT = 0x1000,
    SHIP_CARGO_MINERAL_COUNT = 3,
    SHIP_CARGO_GAUGE_FUEL = 4,
    SHIP_CARGO_GAUGE_MINERALS = 5,
    SHIP_CARGO_LABEL_TITLE = 0x02e9,
    SHIP_CARGO_LABEL_GAUGE = 0x02ea,
    SHIP_CARGO_LABEL_FUEL = 0x01b1,
    SHIP_CARGO_FORMAT_VALUE = 0x037c,
    SHIP_CARGO_LEFT_INSET = 4,
    SHIP_CARGO_TOP_INSET = 1,
    SHIP_CARGO_ROW_GAP = 4
};

#define SHIP_CARGO_WHITE_TEXT 0x00ffffffUL

/* NE 0B:1A54-1E72. Draw the cargo/fuel gauges and, in the compact layout,
 * the three mineral rows plus the fleet fuel total. */
void stars_draw_ship_cargo(StarsDrawShipCargoInput *input)
{
    StarsU8 text[128];
    StarsRect rect;
    StarsI16 left;
    StarsI16 right;
    StarsI16 top;
    StarsI16 label_width;
    StarsI16 index;
    StarsI16 length;
    StarsI32 extent_a;
    StarsI32 extent_b;

    if (!input || !input->ops.draw_tile)
        return;
    if (input->tile_flags & SHIP_CARGO_TILE_RESET_BIT) {
        if (input->ops.set_reference) {
            input->ops.set_reference(input->context, 2U, 0);
            input->ops.set_reference(input->context, 3U, 0);
        }
        input->tile_flags = (StarsU16)(input->tile_flags &
            (StarsU16)~SHIP_CARGO_TILE_RESET_BIT);
    }
    if (!input->ops.get_text)
        return;
    input->ops.get_text(input->context, SHIP_CARGO_LABEL_TITLE, text,
        (StarsU16)sizeof text);
    rect = input->tile_rect;
    if (!input->ops.draw_tile(input->context, input->hdc, input->tile_flags,
            text, &rect))
        return;
    left = (StarsI16)rect.left + SHIP_CARGO_LEFT_INSET;
    right = (StarsI16)rect.right - SHIP_CARGO_LEFT_INSET;
    top = (StarsI16)rect.top + SHIP_CARGO_TOP_INSET;
    label_width = input->max_mineral_quantity_width;
    if (input->ops.select_font)
        input->ops.select_font(input->context, input->hdc, input->bold_font);
    length = input->ops.get_text(input->context, SHIP_CARGO_LABEL_TITLE,
        text, (StarsU16)sizeof text);
    extent_a = input->ops.text_extent ?
        input->ops.text_extent(input->context, input->hdc, text, length) : 0;
    length = input->ops.get_text(input->context, SHIP_CARGO_LABEL_GAUGE,
        text, (StarsU16)sizeof text);
    extent_b = input->ops.text_extent ?
        input->ops.text_extent(input->context, input->hdc, text, length) : 0;
    if (extent_a < extent_b)
        label_width = (StarsI16)extent_b;
    if ((input->tile_flags & SHIP_CARGO_TILE_HIDE_BIT) == 0U &&
            input->ops.text_out)
        input->ops.text_out(input->context, input->hdc, left, top, text,
            length);
    rect.left = (StarsU16)(left + label_width);
    rect.top = (StarsU16)top;
    rect.right = (StarsU16)right;
    rect.bottom = (StarsU16)(top + input->arial8_height);
    if (input->ops.set_reference)
        input->ops.set_reference(input->context, 2U, &rect);
    if (input->ops.draw_gauge)
        input->ops.draw_gauge(input->context, input->hdc, &rect,
            input->planet, SHIP_CARGO_GAUGE_FUEL);
    top = (StarsI16)(top + (input->grid_bit == 0 ? 4 : 2) +
        input->arial8_height);
    if ((input->tile_flags & SHIP_CARGO_TILE_HIDE_BIT) == 0U &&
            input->ops.select_font && input->ops.text_out) {
        input->ops.select_font(input->context, input->hdc, input->bold_font);
        length = input->ops.get_text(input->context, SHIP_CARGO_LABEL_TITLE,
            text, (StarsU16)sizeof text);
        input->ops.text_out(input->context, input->hdc, left, top, text,
            length);
    }
    rect.left = (StarsU16)(left + label_width);
    rect.top = (StarsU16)top;
    rect.right = (StarsU16)right;
    rect.bottom = (StarsU16)(top + input->arial8_height);
    if (input->ops.set_reference)
        input->ops.set_reference(input->context, 3U, &rect);
    if (input->ops.draw_gauge)
        input->ops.draw_gauge(input->context, input->hdc, &rect,
            input->planet, SHIP_CARGO_GAUGE_MINERALS);
    top = (StarsI16)(top + input->arial8_height + SHIP_CARGO_ROW_GAP);
    if (input->grid_bit != 0)
        return;
    for (index = 0; index < SHIP_CARGO_MINERAL_COUNT; ++index) {
        if ((input->tile_flags & SHIP_CARGO_TILE_HIDE_BIT) == 0U) {
            if (input->ops.select_font)
                input->ops.select_font(input->context, input->hdc,
                    input->bold_font);
            if (input->ops.set_text_color)
                input->ops.set_text_color(input->context, input->hdc,
                    input->mineral_colors[index]);
            if (input->ops.text_out && input->mineral_names[index]) {
                length = (StarsI16)strlen((const char *)
                    input->mineral_names[index]);
                input->ops.text_out(input->context, input->hdc, left, top,
                    input->mineral_names[index], length);
            }
        }
        if (input->ops.select_font)
            input->ops.select_font(input->context, input->hdc,
                input->normal_font);
        if (input->ops.set_text_color)
            input->ops.set_text_color(input->context, input->hdc,
                input->button_text_color);
        length = input->ops.format_value ?
            input->ops.format_value(input->context, text,
                (StarsU16)sizeof text, input->resource_values ?
                input->resource_values[index] : 0) : 0;
        if (input->ops.right_text)
            input->ops.right_text(input->context, input->hdc, right, top,
                text, length, input->max_mineral_quantity_width);
        top = (StarsI16)(top + input->arial8_height);
    }
    if ((input->tile_flags & SHIP_CARGO_TILE_HIDE_BIT) == 0U) {
        if (input->ops.select_font)
            input->ops.select_font(input->context, input->hdc, input->bold_font);
        if (input->ops.set_text_color)
            input->ops.set_text_color(input->context, input->hdc,
                SHIP_CARGO_WHITE_TEXT);
        length = input->ops.get_text(input->context, SHIP_CARGO_LABEL_FUEL,
            text, (StarsU16)sizeof text);
        if (input->ops.text_out)
            input->ops.text_out(input->context, input->hdc, left, top, text,
                length);
        if (input->ops.select_font)
            input->ops.select_font(input->context, input->hdc,
                input->normal_font);
        if (input->ops.set_text_color)
            input->ops.set_text_color(input->context, input->hdc,
                input->button_text_color);
    }
    length = input->ops.format_value ?
        input->ops.format_value(input->context, text, (StarsU16)sizeof text,
            input->resource_values ? input->resource_values[4] : 0) : 0;
    if (input->ops.right_text)
        input->ops.right_text(input->context, input->hdc, right, top, text,
            length, input->max_mineral_quantity_width);
}
