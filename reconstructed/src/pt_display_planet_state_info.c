#include "core.h"

static StarsU16 planet_state_packed_get16(const StarsU8 *record,
    StarsU16 offset)
{
    return (StarsU16)(record[offset] |
        ((StarsU16)record[offset + 1] << 8));
}

/* Typed semantic reconstruction of PtDisplayPlanetStateInfo, NE 25:1938-228e.
 * The production input owns the bounded _planet/_popupdata field decode;
 * callbacks retain localization, formatting, derived service values, and GDI
 * output while this file preserves the original row and comparison decisions. */

static void state_stream(StarsPlanetStateInfoInput *input, StarsI16 *x,
    StarsI16 y, StarsI16 *maximum, StarsI16 string_id, StarsI16 kind,
    StarsI16 value, StarsI16 bold)
{
    StarsPlanetStateInfoSegment segment;

    segment.string_id = string_id;
    segment.kind = kind;
    segment.value = value;
    segment.bold = bold;
    if (input->ops->prepare_segment)
        input->ops->prepare_segment(input->ops->context, &segment);
    if (input->ops->select_font)
        input->ops->select_font(input->ops->context, input->hdc,
            bold ? input->font_bold : input->font_normal);
    if (input->ops->stream_text)
        input->ops->stream_text(input->ops->context, input->hdc, x, y,
            &segment, input->print_mode);
    if (*x > *maximum)
        *maximum = *x;
}

static void state_wrap(StarsPlanetStateInfoInput *input, StarsI16 *x,
    StarsI16 *y, StarsI16 *maximum, StarsI16 string_id, StarsI16 kind,
    StarsI16 value, StarsI16 bold)
{
    StarsPlanetStateInfoSegment segment;

    segment.string_id = string_id;
    segment.kind = kind;
    segment.value = value;
    segment.bold = bold;
    if (input->ops->prepare_segment)
        input->ops->prepare_segment(input->ops->context, &segment);
    if (input->ops->select_font)
        input->ops->select_font(input->ops->context, input->hdc,
            bold ? input->font_bold : input->font_normal);
    if (input->ops->wrap_text)
        input->ops->wrap_text(input->ops->context, input->hdc, x, y,
            &segment, (StarsI16)(*maximum - 4), input->print_mode);
    if (*x > *maximum)
        *maximum = *x;
}

static StarsI16 state_environment(StarsPlanetStateInfoInput *input,
    StarsI16 value)
{
    if (input->ops->environment)
        return input->ops->environment(input->ops->context,
            input->data.planet_var, value, input->data.planet_id,
            input->data.environment_index);
    return value;
}

static StarsI16 state_desirability(StarsPlanetStateInfoInput *input,
    StarsI16 environment_value)
{
    if (input->ops->desirability)
        return input->ops->desirability(input->ops->context,
            input->data.planet_id, input->data.environment_index,
            input->data.planet_value, environment_value);
    return 0;
}

StarsPoint stars_display_planet_state_info(StarsPlanetStateInfoInput *input)
{
    StarsPoint result;
    StarsI16 x;
    StarsI16 y;
    StarsI16 maximum;
    StarsI16 line;
    StarsI16 threshold;
    StarsI16 current_desirability;
    StarsI16 improved_desirability;
    StarsI16 difference;
    StarsI16 difference_id;

    result.x = 0;
    result.y = 0;
    if (!input || !input->ops)
        return result;
    if (input->packed_planet_record && input->packed_planet_size >= 56U &&
            input->packed_popupdata && input->packed_popupdata_size >= 22U) {
        StarsI16 environment_index;
        StarsU16 environment_offset;

        environment_index = input->packed_environment_index;
        environment_offset = environment_index >= 0 &&
            (StarsU16)environment_index < 41U ?
            (StarsU16)(15 + environment_index) : 15U;
        input->data.planet_id = (StarsI16)planet_state_packed_get16(
            input->packed_planet_record, 0);
        input->data.planet_var = (StarsI16)planet_state_packed_get16(
            input->packed_popupdata, 4);
        input->data.planet_value = (StarsI16)planet_state_packed_get16(
            input->packed_popupdata, 6);
        input->data.player_min = (StarsI16)planet_state_packed_get16(
            input->packed_popupdata, 14);
        input->data.player_max = (StarsI16)planet_state_packed_get16(
            input->packed_popupdata, 16);
        input->data.player_value = (StarsI16)planet_state_packed_get16(
            input->packed_popupdata, 12);
        input->data.planet_min = (StarsI16)planet_state_packed_get16(
            input->packed_popupdata, 8);
        input->data.planet_max = (StarsI16)planet_state_packed_get16(
            input->packed_popupdata, 10);
        input->data.player_value_secondary = (StarsI16)
            input->packed_planet_record[environment_offset];
        input->data.planet_max_secondary = (StarsI16)
            input->packed_planet_record[(StarsU16)(environment_offset + 1U)];
        input->data.planet_min_secondary =
            input->data.player_value_secondary;
        input->data.player_value_min_secondary =
            input->data.planet_max_secondary;
        input->data.difference_center = (StarsI16)
            planet_state_packed_get16(input->packed_popupdata, 10);
        input->data.planet_min_offset = (StarsI16)
            planet_state_packed_get16(input->packed_popupdata, 4);
        input->data.planet_max_offset = (StarsI16)
            planet_state_packed_get16(input->packed_popupdata, 14);
        input->data.environment_index = environment_index;
    }
    line = input->line_height > 0 ? input->line_height : 1;
    x = 4;
    y = 4;
    maximum = 4;
    state_stream(input, &x, y, &maximum, -1, 1,
        input->data.planet_var, 1);
    state_stream(input, &x, y, &maximum, 0x253, 0, 0, 0);
    if (input->data.planet_value < 0)
        state_stream(input, &x, y, &maximum, 0x254, 0, 0, 0);
    else
        state_stream(input, &x, y, &maximum, 0, 2,
            state_environment(input, input->data.planet_value), 0);
    state_stream(input, &x, y, &maximum, 0xc14, 0, 0, 0);
    x = 4;
    y = (StarsI16)(y + line);

    if (input->data.player_min == -1) {
        state_stream(input, &x, y, &maximum, 0x255, 0, 0, 0);
        x = 4;
        y = (StarsI16)(y + line);
        state_stream(input, &x, y, &maximum, 0x256, 0, 0, 0);
        state_stream(input, &x, y, &maximum, -1, 1,
            input->data.planet_var, 1);
        state_stream(input, &x, y, &maximum, 0xc16, 0, 0, 0);
    } else {
        state_stream(input, &x, y, &maximum, 599, 0, 0, 0);
        x = 4;
        y = (StarsI16)(y + line);
        state_stream(input, &x, y, &maximum, -1, 1,
            input->data.planet_var, 0);
        state_stream(input, &x, y, &maximum, 600, 0, 0, 0);
        state_stream(input, &x, y, &maximum, 0, 2,
            state_environment(input, input->data.player_min), 1);
        state_stream(input, &x, y, &maximum, 0x259, 0, 0, 0);
        state_stream(input, &x, y, &maximum, 0, 2,
            state_environment(input, input->data.player_max), 1);
        state_stream(input, &x, y, &maximum, 0xc18, 0, 0, 0);
    }
    x = 4;
    y = (StarsI16)(y + line);

    if (input->data.planet_min >= 0) {
        state_stream(input, &x, y, &maximum, 0x25a, 0, 0, 0);
        x = 4;
        y = (StarsI16)(y + line);
        state_stream(input, &x, y, &maximum, 0x252, 0, 0, 0);
        state_stream(input, &x, y, &maximum, -1, 1,
            input->data.planet_var, 0);
        state_stream(input, &x, y, &maximum, 0x251, 0, 0, 0);
        state_stream(input, &x, y, &maximum, -1, 3,
            input->data.planet_id, 1);
        state_stream(input, &x, y, &maximum, 0x24d, 0, 0, 0);
        state_stream(input, &x, y, &maximum, 0, 2,
            state_environment(input, input->data.planet_min), 1);
        state_stream(input, &x, y, &maximum, 0x250, 0, 0, 0);
        state_stream(input, &x, y, &maximum, 0, 2,
            state_environment(input, input->data.planet_max), 1);
        state_stream(input, &x, y, &maximum, 0xc1a, 0, 0, 0);
        x = 4;
        y = (StarsI16)(y + line);
    }

    if (input->data.planet_min >= 0 && input->data.player_min != -1 &&
            input->data.planet_value != input->data.player_value) {
        threshold = -1;
        if (input->data.planet_value < input->data.player_value) {
            threshold = input->data.player_value;
            if (input->data.planet_value < input->data.planet_max &&
                    input->data.player_value_secondary <=
                    input->data.planet_max_secondary)
                threshold = input->data.planet_max;
        } else if (input->data.planet_min < input->data.planet_value &&
                input->data.planet_min_secondary <=
                input->data.player_value_min_secondary) {
            threshold = input->data.planet_min;
        }
        if (threshold != -1) {
            current_desirability = state_desirability(input,
                input->data.planet_value);
            improved_desirability = state_desirability(input, threshold);
            if (current_desirability < improved_desirability) {
                state_wrap(input, &x, &y, &maximum, 0x166, 0, 0, 0);
                state_wrap(input, &x, &y, &maximum, -1, 1,
                    input->data.planet_var, 1);
                state_wrap(input, &x, &y, &maximum, 0x250, 0, 0, 0);
                state_wrap(input, &x, &y, &maximum, 0, 2,
                    state_environment(input, threshold), 1);
                state_wrap(input, &x, &y, &maximum, 0x167, 0,
                    improved_desirability, 1);
            }
        }
    }

    if (input->data.player_min != -1 &&
            input->data.planet_value != input->data.player_value &&
            input->data.planet_value != -1) {
        if (input->data.planet_value < input->data.player_min)
            difference = (StarsI16)(input->data.player_min -
                input->data.planet_min_offset);
        else if (input->data.player_max < input->data.planet_value)
            difference = (StarsI16)(input->data.planet_value -
                input->data.planet_max_offset);
        else
            difference = (StarsI16)(input->data.planet_value -
                input->data.difference_center);
        if (difference < 0) {
            difference = (StarsI16)-difference;
            difference_id = 0x169;
        } else {
            difference_id = 0x168;
        }
        state_wrap(input, &x, &y, &maximum, difference_id, 4,
            difference, 0);
    }
    if (x > 4)
        y = (StarsI16)(y + line);
    result.x = (StarsI16)(maximum + 4);
    result.y = (StarsI16)(y + 4);
    return result;
}
