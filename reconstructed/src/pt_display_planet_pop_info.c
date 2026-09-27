#include "core.h"

#define STARS_PLANET_POP_INFO_PLANET_SIZE 56

static StarsU16 planet_pop_packed_get16(const StarsU8 *record,
    StarsU16 offset)
{
    return (StarsU16)(record[offset] |
        ((StarsU16)record[offset + 1] << 8));
}

static StarsU32 planet_pop_packed_get32(const StarsU8 *record,
    StarsU16 offset)
{
    return (StarsU32)planet_pop_packed_get16(record, offset) |
        ((StarsU32)planet_pop_packed_get16(record,
            (StarsU16)(offset + 2)) << 16);
}

/* Typed semantic reconstruction of PtDisplayPlanetPopInfo, NE 25:228e-2d65.
 * Localization and text layout remain callback-owned; the owned/unowned/
 * foreign branches and the population/desirability layout decisions remain
 * explicit here. */

static void pop_stream(StarsPlanetPopInfoInput *input, StarsI16 *x,
    StarsI16 y, StarsI16 *maximum, StarsI16 kind, StarsI16 string_id,
    StarsI32 value1, StarsI32 value2, StarsI16 bold)
{
    StarsPlanetPopSegment segment;

    segment.kind = kind;
    segment.string_id = string_id;
    segment.value1 = value1;
    segment.value2 = value2;
    segment.bold = bold;
    if (input->ops->prepare_segment)
        input->ops->prepare_segment(input->ops->context, &segment);
    if (input->ops->select_font)
        input->ops->select_font(input->ops->context, input->hdc, bold ?
            input->font_bold : input->font_normal);
    if (input->ops->stream_text)
        input->ops->stream_text(input->ops->context, input->hdc, x, y,
            &segment, input->print_mode);
    if (*x > *maximum)
        *maximum = *x;
}

static void pop_wrap(StarsPlanetPopInfoInput *input, StarsI16 *x,
    StarsI16 *y, StarsI16 maximum, StarsI16 kind, StarsI16 string_id,
    StarsI32 value1, StarsI32 value2, StarsI16 bold)
{
    StarsPlanetPopSegment segment;

    segment.kind = kind;
    segment.string_id = string_id;
    segment.value1 = value1;
    segment.value2 = value2;
    segment.bold = bold;
    if (input->ops->prepare_segment)
        input->ops->prepare_segment(input->ops->context, &segment);
    if (input->ops->select_font)
        input->ops->select_font(input->ops->context, input->hdc, bold ?
            input->font_bold : input->font_normal);
    if (input->ops->wrap_text)
        input->ops->wrap_text(input->ops->context, input->hdc, x, y,
            &segment, maximum, input->print_mode);
    if (*x > maximum)
        *x = maximum;
}

StarsPoint stars_display_planet_pop_info(StarsPlanetPopInfoInput *input)
{
    StarsPoint result;
    StarsPlanetPopInfo info;
    StarsI16 x;
    StarsI16 y;
    StarsI16 maximum;
    StarsI16 line;
    StarsI32 max_population;
    StarsI16 desirability;
    StarsI32 change_population;

    result.x = 0;
    result.y = 0;
    if (!input || !input->ops)
        return result;
    if (input->packed_planet_record && input->packed_planet_size >=
            STARS_PLANET_POP_INFO_PLANET_SIZE) {
        input->planet_id = (StarsI16)planet_pop_packed_get16(
            input->packed_planet_record, 0);
        info.owner = (StarsI16)planet_pop_packed_get16(
            input->packed_planet_record, 2);
        info.selected_player = input->packed_selected_player;
        info.mineral_rate = (StarsI32)planet_pop_packed_get32(
            input->packed_planet_record, 28);
        info.habitability_class = input->packed_habitability_class;
        info.max_population = input->packed_max_population;
        info.desirability_tenths = input->packed_desirability_tenths;
        info.population_change = (StarsI32)(signed char)(
            planet_pop_packed_get32(input->packed_planet_record, 20) &
            0xffUL);
        info.foreign_environment_class =
            input->packed_foreign_environment_class;
    } else {
        if (!input->ops->lookup_planet || !input->ops->lookup_planet(
                input->ops->context, input->planet_id, &info))
            return result;
    }
    line = input->line_height > 0 ? input->line_height : 1;
    x = 4;
    y = 4;
    maximum = 4;
    if (info.owner == input->viewer_player) {
        pop_stream(input, &x, y, &maximum, 0, 0x244, 0, 0, 0);
        pop_stream(input, &x, y, &maximum, 1, input->planet_id, 0, 0, 1);
        pop_stream(input, &x, y, &maximum, 0, 0x24f, 0, 0, 0);
        pop_stream(input, &x, y, &maximum, 2, 0, info.mineral_rate, 0, 1);
        if (info.mineral_rate == 0)
            pop_stream(input, &x, y, &maximum, 3, 0xc24, 0, 0, 0);
    } else if (info.owner == -1) {
        pop_stream(input, &x, y, &maximum, 1, input->planet_id, 0, 0, 1);
        pop_stream(input, &x, y, &maximum, 0, 0x248, 0, 0, 0);
    } else {
        pop_stream(input, &x, y, &maximum, 0, 0x245, 0, 0, 0);
        pop_stream(input, &x, y, &maximum, 1, input->planet_id, 0, 0, 1);
        if (info.habitability_class < 3) {
            pop_stream(input, &x, y, &maximum, 0, 0x247, 0, 0, 0);
        } else {
            pop_stream(input, &x, y, &maximum, 0, 0x246, 0, 0, 0);
            pop_stream(input, &x, y, &maximum, 4, 0,
                (StarsI32)info.foreign_environment_class << 2, 0, 1);
        }
        pop_stream(input, &x, y, &maximum, 0, 0xc2b, 0, 0, 0);
    }
    if (x > maximum)
        maximum = x;
    x = 4;
    y = (StarsI16)(y + line);
    if (info.habitability_class > 2) {
        max_population = info.max_population;
        if (input->ops->max_population)
            max_population = input->ops->max_population(input->ops->context,
                input->planet_id, input->viewer_player);
        desirability = info.desirability_tenths;
        if (input->ops->desirability)
            desirability = input->ops->desirability(input->ops->context,
                input->planet_id, input->viewer_player);
        if (desirability < 0) {
            pop_stream(input, &x, y, &maximum, 1, input->planet_id, 0, 0, 1);
            pop_stream(input, &x, y, &maximum, 0, 0x249, 0, 0, 0);
            pop_stream(input, &x, y, &maximum, 5, 0, -desirability / 10,
                -desirability % 10, 1);
            pop_stream(input, &x, y, &maximum, 0, 0x24e, 0, 0, 0);
            pop_stream(input, &x, y, &maximum, 0,
                info.owner == info.selected_player ? 0x24a : 0x24b,
                0, 0, 0);
        } else if (info.owner == input->viewer_player && max_population > 0) {
            pop_stream(input, &x, y, &maximum, 1, input->planet_id, 0, 0, 1);
            pop_stream(input, &x, y, &maximum, 0, 0x24c, 0, 0, 0);
            y = (StarsI16)(y + line);
            pop_stream(input, &x, y, &maximum, 6, 0, max_population, 0, 1);
            pop_stream(input, &x, y, &maximum, 0, 0x240, 0, 0, 0);
        } else if (max_population < 1) {
            y = (StarsI16)(y - line);
        } else {
            pop_stream(input, &x, y, &maximum, 0, 0x243, 0, 0, 0);
            pop_stream(input, &x, y, &maximum, 1, input->planet_id, 0, 0, 1);
            pop_stream(input, &x, y, &maximum, 0, 0x242, 0, 0, 0);
            y = (StarsI16)(y + line);
            pop_stream(input, &x, y, &maximum, 6, 0, max_population, 0, 1);
            pop_stream(input, &x, y, &maximum, 0, 0x240, 0, 0, 0);
        }
        if (x > maximum)
            maximum = x;
        x = 4;
        y = (StarsI16)(y + line);
        if (info.owner == input->viewer_player && desirability >= 0 &&
                info.mineral_rate < max_population) {
            pop_wrap(input, &x, &y, maximum, 0, 0x212, 0, 0, 0);
            pop_wrap(input, &x, &y, maximum, 1, input->planet_id, 0, 0, 1);
            change_population = info.population_change;
            if (input->ops->change_population)
                change_population = input->ops->change_population(
                    input->ops->context, input->planet_id);
            if (desirability == 0 || change_population < 1) {
                pop_wrap(input, &x, &y, maximum, 0, 0x214, 0, 0, 0);
            } else {
                pop_wrap(input, &x, &y, maximum, 7, 0x213,
                    change_population,
                    change_population + info.mineral_rate, 0);
            }
            y = (StarsI16)(y + line);
        } else if (info.owner != input->viewer_player && info.owner != -1) {
            pop_wrap(input, &x, &y, maximum, 1, input->planet_id, 0, 0, 1);
            if (info.foreign_environment_class == 0)
                pop_wrap(input, &x, &y, maximum, 0, 0x1e3, 0, 0, 0);
            else
                pop_wrap(input, &x, &y, maximum, 8, 0x1e4,
                    info.foreign_environment_class * 6 + 3, 0, 0);
            y = (StarsI16)(y + line);
        }
    }
    result.x = (StarsI16)(maximum + 4);
    result.y = (StarsI16)(y + 4);
    return result;
}
