#include "core.h"
#include <string.h>

#define PLANET_MIN_SUM_TITLE 0x0321U
#define PLANET_MIN_SUM_LABEL 0x0322U
#define PLANET_MIN_SUM_MINE_FORMAT 0x02eeU
#define PLANET_MIN_SUM_SPECIAL_MINE_FORMAT 0x02f0U
#define PLANET_MIN_SUM_FACTORY_LABEL 0x0323U
#define PLANET_MIN_SUM_SPECIAL_FACTORY_LABEL 0x04eaU
#define PLANET_MIN_SUM_HIDE_TEXT 0x1000U
#define PLANET_MIN_SUM_INVALID_TOP ((StarsU16)-5)
#define PLANET_MIN_SUM_INVALID_BOTTOM ((StarsU16)-6)
#define PLANET_MIN_SUM_PATCOPY 0xf00021UL

static void invalidate_lower_references(StarsPlanetMinSumInput *input)
{
    StarsRect invalid;
    StarsI16 i;

    if (!input->reference_rects)
        return;
    invalid.left = 0U;
    invalid.top = PLANET_MIN_SUM_INVALID_TOP;
    invalid.right = 0U;
    invalid.bottom = PLANET_MIN_SUM_INVALID_BOTTOM;
    /* The original invalidates the six lower report slots as a group. */
    for (i = 0; i < 4; ++i)
        input->ops.set_reference(input->context, (StarsU16)(6 + i), &invalid);
    input->ops.set_reference(input->context, 0x0bU, &invalid);
    input->ops.set_reference(input->context, 0x0aU, &invalid);
}

static StarsI16 get_label(StarsPlanetMinSumInput *input, StarsU16 id,
    const StarsU8 *fallback, StarsU8 *buffer, StarsU16 size)
{
    if (input->ops.get_text)
        return input->ops.get_text(input->context, id, buffer, size);
    if (fallback) {
        size_t n = strlen((const char *)fallback);
        if (n >= size) n = size - 1U;
        memcpy(buffer, fallback, n);
        buffer[n] = 0;
        return (StarsI16)n;
    }
    buffer[0] = 0;
    return 0;
}

static void draw_text(StarsPlanetMinSumInput *input, StarsI16 x, StarsI16 y,
    const StarsU8 *text, StarsI16 length)
{
    if (input->ops.text_out && length > 0)
        input->ops.text_out(input->context, input->hdc, x, y, text,
            (StarsU16)length);
}

static StarsI16 format_row(StarsPlanetMinSumInput *input, StarsU8 *buffer,
    StarsU16 size, StarsI32 quantity, StarsI16 capacity)
{
    if (input->ops.format)
        return input->ops.format(input->context, buffer, size,
            input->quantity_format, quantity, capacity);
    return 0;
}

static StarsI16 format_capacity(StarsPlanetMinSumInput *input, StarsU8 *buffer,
    StarsU16 size, StarsI16 capacity, StarsI32 density)
{
    if (input->ops.format)
        return input->ops.format(input->context, buffer, size,
            input->capacity_format, density, capacity);
    return 0;
}

/* NE 0A:12B2-1715. Draw the mineral-summary tile using named UI callbacks.
 * The low eight and twelve-bit extracts below are the original packed planet
 * capacity fields; keeping them here documents the field meaning without
 * relying on selector arithmetic or emitted byte arrays. */
void stars_draw_planet_min_sum(StarsPlanetMinSumInput *input)
{
    StarsRect tile_rect;
    StarsRect row_rect;
    StarsU8 text[128];
    StarsU8 label[64];
    StarsI16 left;
    StarsI16 right;
    StarsI16 y;
    StarsI16 row;
    StarsI16 length;
    StarsI16 capacity;
    StarsI16 race_stat;
    StarsI32 density;
    StarsI32 shifted_density;
    const StarsU8 *title;
    StarsU16 old_font;

    if (!input || !input->planet || !input->ops.draw_tile)
        return;
    if ((input->tile.flags & (1U << 11)) != 0U)
        invalidate_lower_references(input);
    title = input->ops.compressed_string ?
        input->ops.compressed_string(input->context, PLANET_MIN_SUM_TITLE) :
        (const StarsU8 *)"";
    if (!input->ops.draw_tile(input->context, input->hdc, &input->tile,
            &tile_rect, title))
        return;

    left = (StarsI16)(tile_rect.left + 4);
    right = (StarsI16)(tile_rect.right - 4);
    y = (StarsI16)tile_rect.top;
    if (input->reference_rects) {
        row_rect.left = (StarsU16)left;
        row_rect.top = tile_rect.top;
        row_rect.right = (StarsU16)right;
        row_rect.bottom = (StarsU16)(tile_rect.top + input->arial8_height * 3);
        if (input->ops.set_reference)
            input->ops.set_reference(input->context, 6U, &row_rect);
    }

    for (row = 0; row < 3; ++row) {
        if ((input->tile.flags & PLANET_MIN_SUM_HIDE_TEXT) == 0U) {
            if (input->ops.select_object)
                input->ops.select_object(input->context, input->hdc,
                    input->bold_font);
            if (input->ops.set_text_color)
                input->ops.set_text_color(input->context, input->hdc,
                    input->mineral_colors[row]);
            length = get_label(input, (StarsU16)(0x0448U + row),
                input->mineral_labels[row], label, (StarsU16)sizeof label);
            draw_text(input, left, y, label, length);
        }
        if (input->ops.select_object)
            input->ops.select_object(input->context, input->hdc,
                input->normal_font);
        if (input->ops.set_text_color)
            input->ops.set_text_color(input->context, input->hdc,
                input->text_color);
        length = format_row(input, text, (StarsU16)sizeof text,
            input->planet->minerals[row], input->max_mineral_width);
        if (input->ops.right_text)
            input->ops.right_text(input->context, input->hdc, right, y, text,
                length, input->max_mineral_width);
        y = (StarsI16)(y + input->arial8_height);
    }

    old_font = input->ops.select_object ? input->ops.select_object(
        input->context, input->hdc, input->highlight_brush) : 0U;
    if (input->ops.pat_blt)
        input->ops.pat_blt(input->context, input->hdc, tile_rect.left, y,
            (StarsI16)(tile_rect.right - tile_rect.left), 1,
            PLANET_MIN_SUM_PATCOPY);
    if (input->ops.select_object)
        input->ops.select_object(input->context, input->hdc, old_font);

    y = (StarsI16)(y + 1);
    if (input->reference_rects) {
        row_rect.left = (StarsU16)left;
        row_rect.top = (StarsU16)y;
        row_rect.right = (StarsU16)right;
        row_rect.bottom = (StarsU16)(y + input->arial8_height * 2);
        if (input->ops.set_reference)
            input->ops.set_reference(input->context, 7U, &row_rect);
    }
    if ((input->tile.flags & PLANET_MIN_SUM_HIDE_TEXT) == 0U) {
        if (input->ops.select_object)
            input->ops.select_object(input->context, input->hdc,
                input->bold_font);
        length = get_label(input, PLANET_MIN_SUM_LABEL, input->factory_label,
            label, (StarsU16)sizeof label);
        draw_text(input, left, y, label, length);
        if (input->ops.select_object)
            input->ops.select_object(input->context, input->hdc,
                input->normal_font);
    }

    race_stat = input->ops.race_stat ? input->ops.race_stat(input->context,
        input->player, 0x0e) : 0;
    if (race_stat == 8) {
        capacity = input->ops.operating_mines ? input->ops.operating_mines(
            input->context, input->planet) : 0;
        length = input->ops.format ? input->ops.format(input->context, text,
            (StarsU16)sizeof text, input->special_mines_format, capacity, 0) : 0;
    } else {
        capacity = input->ops.max_mines ? input->ops.max_mines(input->context,
            input->planet, input->player, 0) : 0;
        density = (StarsI32)((input->planet->mineral_capacity_bits >> 8) &
            0x0fffU);
        length = format_capacity(input, text, (StarsU16)sizeof text, capacity,
            density);
    }
    if (input->ops.right_text)
        input->ops.right_text(input->context, input->hdc, right, y, text,
            length, (StarsI16)(input->max_mineral_width << 1));
    y = (StarsI16)(y + input->arial8_height);

    if ((input->tile.flags & PLANET_MIN_SUM_HIDE_TEXT) == 0U) {
        if (input->ops.select_object)
            input->ops.select_object(input->context, input->hdc,
                input->bold_font);
        length = get_label(input, PLANET_MIN_SUM_FACTORY_LABEL,
            input->factory_label, label, (StarsU16)sizeof label);
        draw_text(input, left, y, label, length);
        if (input->ops.select_object)
            input->ops.select_object(input->context, input->hdc,
                input->normal_font);
    }
    race_stat = input->ops.race_stat ? input->ops.race_stat(input->context,
        input->player, 0x0e) : 0;
    if (race_stat == 8) {
        length = get_label(input, PLANET_MIN_SUM_SPECIAL_FACTORY_LABEL,
            input->special_factory_label, text, (StarsU16)sizeof text);
    } else {
        capacity = input->ops.max_factories ? input->ops.max_factories(
            input->context, input->planet, input->player, 0) : 0;
        shifted_density = (StarsI32)((input->planet->mineral_capacity_bits >>
            20) & 0x0fffU);
        length = format_capacity(input, text, (StarsU16)sizeof text, capacity,
            shifted_density);
    }
    if (input->ops.right_text)
        input->ops.right_text(input->context, input->hdc, right, y, text,
            length, (StarsI16)(input->max_mineral_width << 1));
}
