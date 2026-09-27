#include "core.h"

/* Typed semantic reconstruction of DrawPlanetStats, NE 0a:1716-21f5.
 * Localization, text measurement, tile drawing, and the packed planet/race
 * calculations are callback-owned; this routine preserves row order, hidden
 * tile behavior, right-edge alignment, reference rectangles, and separators. */

static StarsU16 planet_stats_packed_get16(const StarsU8 *record,
    StarsU16 offset)
{
    return (StarsU16)(record[offset] |
        ((StarsU16)record[offset + 1] << 8));
}

static StarsU32 planet_stats_packed_get32(const StarsU8 *record,
    StarsU16 offset)
{
    return (StarsU32)planet_stats_packed_get16(record, offset) |
        ((StarsU32)planet_stats_packed_get16(record,
            (StarsU16)(offset + 2)) << 16);
}

static StarsI16 planet_stats_max_label(const StarsPlanetStatsInput *input,
    StarsI16 hdc)
{
    static const StarsI16 labels[7] = {
        0x325, 0x326, 0x327, 0x32b, 0x32c, 0x32d, 0x324
    };
    StarsI16 i;
    StarsI16 width;
    StarsI16 maximum;

    maximum = 0;
    if (!input->ops->measure_label)
        return maximum;
    for (i = 0; i < 7; ++i) {
        width = input->ops->measure_label(input->ops->context, input->hdc,
            labels[i]);
        if (width > maximum)
            maximum = width;
    }
    (void)hdc;
    return maximum;
}

static void planet_stats_label(const StarsPlanetStatsInput *input,
    StarsI16 label, StarsI16 x, StarsI16 y)
{
    if (!input->hidden && input->ops->draw_label)
        input->ops->draw_label(input->ops->context, input->hdc, label, x, y);
}

static void planet_stats_value(const StarsPlanetStatsInput *input,
    StarsI16 field, StarsI16 right, StarsI16 value_offset, StarsI16 y)
{
    StarsU8 formatted[64];
    StarsI16 length;

    if (input->ops->format_value && input->ops->draw_formatted_value) {
        length = input->ops->format_value(input->ops->context, field,
            &input->data, formatted, (StarsU16)sizeof formatted);
        if (length < 0)
            length = 0;
        input->ops->draw_formatted_value(input->ops->context, input->hdc,
            field, right, value_offset, y, formatted, length);
    } else if (input->ops->draw_value)
        input->ops->draw_value(input->ops->context, input->hdc, field, right,
            value_offset, y, &input->data);
}

void stars_draw_planet_stats(StarsPlanetStatsInput *input)
{
    StarsRect rect;
    StarsRect reference;
    StarsI16 left;
    StarsI16 right;
    StarsI16 y;
    StarsI16 line;
    StarsI16 max_label;
    StarsI16 value_offset;

    if (!input || !input->ops || !input->ops->draw_tile)
        return;
    if (input->ops->load_data && !input->ops->load_data(
            input->ops->context, input->tile, &input->data))
        return;
    if (!input->ops->draw_tile(input->ops->context, input->hdc, input->tile,
            &rect))
        return;
    if (input->ops->resolve_data)
        input->ops->resolve_data(input->ops->context, input->tile,
            input->player, &input->data);
    left = (StarsI16)(rect.left + 4);
    right = (StarsI16)(rect.right - 4);
    line = input->line_height > 0 ? input->line_height : 1;
    if (input->ops->select_font)
        input->ops->select_font(input->ops->context, input->hdc,
            input->font_bold);
    max_label = planet_stats_max_label(input, input->hdc);
    value_offset = (StarsI16)((right - left) - max_label);
    y = rect.top;
    planet_stats_label(input, 0x325, left, y);
    if (input->ops->select_font)
        input->ops->select_font(input->ops->context, input->hdc,
            input->font_normal);
    reference.left = left;
    reference.top = y;
    reference.right = right;
    reference.bottom = (StarsU16)(y + line);
    if (input->ops->set_reference)
        input->ops->set_reference(input->ops->context, 9, &reference);
    planet_stats_value(input, 0, right, value_offset, y);
    y = (StarsI16)(y + line);
    planet_stats_label(input, 0x324, left, y);
    reference.top = y;
    reference.bottom = (StarsU16)(y + line);
    if (input->ops->set_reference)
        input->ops->set_reference(input->ops->context, 8, &reference);
    planet_stats_value(input, 1, right, value_offset, y);
    y = (StarsI16)(y + line);
    if (input->ops->draw_separator)
        input->ops->draw_separator(input->ops->context, input->hdc, y,
            rect.left, rect.right);
    ++y;
    planet_stats_label(input, 0x326, left, y);
    reference.top = y;
    reference.bottom = (StarsU16)(y + line * 2);
    if (input->ops->set_reference)
        input->ops->set_reference(input->ops->context, 11, &reference);
    planet_stats_value(input, 2, right, value_offset, y);
    y = (StarsI16)(y + line);
    planet_stats_label(input, 0x327, left, y);
    planet_stats_value(input, 3, right, value_offset, y);
    y = (StarsI16)(y + line);
    if (input->ops->draw_separator)
        input->ops->draw_separator(input->ops->context, input->hdc, y,
            rect.left, rect.right);
    ++y;
    planet_stats_label(input, 0x32b, left, y);
    reference.top = y;
    reference.bottom = (StarsU16)(y + line * 3);
    if (input->ops->set_reference)
        input->ops->set_reference(input->ops->context, 10, &reference);
    planet_stats_value(input, 4, right, value_offset, y);
    y = (StarsI16)(y + line);
    planet_stats_label(input, 0x32c, left, y);
    planet_stats_value(input, 5, right, value_offset, y);
    y = (StarsI16)(y + line);
    planet_stats_label(input, 0x32d, left, y);
    planet_stats_value(input, 6, right, value_offset, y);
}

void stars_draw_planet_stats_packed(StarsPlanetStatsPackedInput *input)
{
    StarsPlanetStatsInput stats;
    StarsPlanetStatsOps ops;
    StarsPlanetStatsData data;
    StarsU32 services;

    if (!input || !input->planet_record || input->planet_size <
            STARS_PLANET_STATS_PLANET_SIZE || !input->stats.ops)
        return;
    services = planet_stats_packed_get32(input->planet_record, 24);
    data.mineral_rate_x100 = (StarsI32)(planet_stats_packed_get32(
        input->planet_record, 40) * 100UL);
    data.resources_available = input->resources_available;
    data.resources_total = input->resources_total;
    data.scanner_range = (StarsI16)((services >> 12) & 0x1fUL);
    data.scanner_width = input->scanner_width;
    data.defense_capacity = (StarsI16)(services & 0x0fffUL);
    data.best_defense_available = input->best_defense_available;
    data.survival_percent = input->survival_percent;
    data.scanner_available = input->scanner_available;
    data.special_scanner = input->special_scanner;
    stats = input->stats;
    stats.tile = input->planet_record;
    stats.data = data;
    ops = *input->stats.ops;
    ops.load_data = 0;
    stats.ops = &ops;
    stars_draw_planet_stats(&stats);
}
