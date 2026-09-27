#include "core.h"

enum {
    FLEET_GAUGE_MINERAL_COUNT = 4,
    FLEET_GAUGE_FUEL_MODE = 4,
    FLEET_GAUGE_MINERALS_MODE = 5,
    FLEET_GAUGE_WARP_MODE = 6,
    FLEET_GAUGE_ENGINE_MODE = 7,
    FLEET_GAUGE_SINGLE_SEGMENT = 1,
    FLEET_GAUGE_STAT_FUEL = 1,
    FLEET_GAUGE_STAT_CARGO = 2,
    FLEET_GAUGE_WARP_TOTAL = 11,
    FLEET_GAUGE_ENGINE_TOTAL = 10,
    FLEET_GAUGE_WARP_SHIFT = 4,
    FLEET_GAUGE_WARP_MASK = 0x000f,
    FLEET_GAUGE_WARP_LIMIT = 11,
    FLEET_GAUGE_WARP_FULL = 10,
    FLEET_GAUGE_ID_FORMAT_WARP_ZERO = 0x0368,
    FLEET_GAUGE_ID_FORMAT_VALUE = 0x0369,
    FLEET_GAUGE_ID_FORMAT_TOTAL = 0x036b,
    FLEET_GAUGE_ID_FORMAT_SHORT = 0x037c,
    FLEET_GAUGE_ID_FORMAT_FUEL = 0x036c,
    FLEET_GAUGE_ID_FORMAT_WARP_FULL = 0x0394,
    FLEET_GAUGE_ID_FORMAT_ENGINE_ZERO = 0x0508
};

static StarsI32 fleet_stat(const StarsDrawFleetGaugeInput *input,
    const void *fleet, StarsI16 stat)
{
    if (!input->ops.get_fleet_stat)
        return 0;
    return input->ops.get_fleet_stat(input->context, fleet, stat);
}

static StarsI16 fleet_ideal_warp(const StarsDrawFleetGaugeInput *input,
    const void *fleet)
{
    if (!input->ops.find_ideal_warp)
        return 0;
    return input->ops.find_ideal_warp(input->context, fleet,
        input->ideal_warp_ignore_scoops);
}

static StarsU16 gauge_brush_for_speed(const StarsDrawFleetGaugeInput *input,
    const void *fleet, StarsU16 speed)
{
    StarsI16 ideal_warp;

    if (speed < FLEET_GAUGE_WARP_LIMIT &&
            (speed != FLEET_GAUGE_WARP_FULL ||
             (ideal_warp = fleet_ideal_warp(input, fleet)) > 9))
        return input->mineral_brushes[4];
    return input->mineral_brushes[2];
}

/* NE 0B:44B6-490E. Draw one fleet resource gauge or the active warp/engine
 * indicator, then format and center the value returned by the gauge. */
void stars_draw_fleet_gauge(StarsDrawFleetGaugeInput *input, StarsI16 mode)
{
    const void *fleet;
    StarsI32 values[4];
    StarsU16 brushes[4];
    StarsI32 total;
    StarsI32 gauge_value;
    StarsU16 segments = FLEET_GAUGE_SINGLE_SEGMENT;
    StarsU16 old_mode;
    StarsI16 text_length = 0;
    StarsI16 extent;
    StarsU16 format_id = 0U;
    StarsI16 format_arguments = 0;
    StarsU8 text[128];
    StarsU16 mineral_index;
    StarsU16 speed;

    if (!input || !input->rect || !input->ops.draw_gauge)
        return;
    fleet = input->fleet ? input->fleet : input->selected_fleet;
    if (input->ops.select_font)
        input->ops.select_font(input->context, input->hdc,
            input->font_arial8);

    total = fleet_stat(input, fleet, FLEET_GAUGE_STAT_CARGO);
    if (mode >= 0 && mode <= FLEET_GAUGE_FUEL_MODE) {
        mineral_index = (StarsU16)mode;
        brushes[0] = input->mineral_brushes[mineral_index];
        values[0] = input->mineral_weights ?
            input->mineral_weights[mineral_index] : 0;
        if (mode == FLEET_GAUGE_FUEL_MODE)
            total = fleet_stat(input, fleet, FLEET_GAUGE_STAT_FUEL);
    } else if (mode == FLEET_GAUGE_MINERALS_MODE) {
        segments = FLEET_GAUGE_MINERAL_COUNT;
        for (mineral_index = 0; mineral_index < segments; ++mineral_index) {
            brushes[mineral_index] = input->mineral_brushes[mineral_index];
            values[mineral_index] = input->mineral_weights ?
                input->mineral_weights[mineral_index] : 0;
        }
    } else if (mode == FLEET_GAUGE_WARP_MODE) {
        total = FLEET_GAUGE_WARP_TOTAL;
        speed = (StarsU16)((input->active_warp_word >>
            FLEET_GAUGE_WARP_SHIFT) & FLEET_GAUGE_WARP_MASK);
        values[0] = (StarsI32)speed;
        brushes[0] = gauge_brush_for_speed(input, fleet, speed);
    } else if (mode == FLEET_GAUGE_ENGINE_MODE) {
        total = FLEET_GAUGE_ENGINE_TOTAL;
        values[0] = input->active_engine_value;
        speed = (StarsU16)input->active_engine_value;
        brushes[0] = gauge_brush_for_speed(input, fleet, speed);
    } else {
        return;
    }

    gauge_value = input->ops.draw_gauge(input->context, input->hdc,
        input->rect, (StarsI16)segments, values, brushes, total);
    old_mode = input->ops.set_bk_mode ?
        input->ops.set_bk_mode(input->context, input->hdc, 1U) : 0U;

    if (mode == FLEET_GAUGE_WARP_MODE) {
        if (gauge_value == 0L) {
            format_id = FLEET_GAUGE_ID_FORMAT_WARP_ZERO;
            if (input->ops.get_string)
                text_length = input->ops.get_string(input->context,
                    format_id, text);
        } else if (gauge_value < FLEET_GAUGE_WARP_LIMIT) {
            format_id = FLEET_GAUGE_ID_FORMAT_VALUE;
            format_arguments = 1;
        } else {
            format_id = FLEET_GAUGE_ID_FORMAT_WARP_FULL;
            if (input->ops.get_string)
                text_length = input->ops.get_string(input->context,
                    format_id, text);
        }
    } else if (mode == FLEET_GAUGE_ENGINE_MODE) {
        if (gauge_value == 0L) {
            format_id = FLEET_GAUGE_ID_FORMAT_ENGINE_ZERO;
            if (input->ops.get_string)
                text_length = input->ops.get_string(input->context,
                    format_id, text);
        } else {
            format_id = FLEET_GAUGE_ID_FORMAT_VALUE;
            format_arguments = 1;
        }
    } else if (segments == FLEET_GAUGE_SINGLE_SEGMENT &&
            mode != FLEET_GAUGE_FUEL_MODE) {
        format_id = FLEET_GAUGE_ID_FORMAT_SHORT;
        format_arguments = 1;
    } else if (mode == FLEET_GAUGE_FUEL_MODE) {
        format_id = FLEET_GAUGE_ID_FORMAT_FUEL;
        format_arguments = 2;
    } else {
        format_id = FLEET_GAUGE_ID_FORMAT_TOTAL;
        format_arguments = 2;
    }
    if (format_arguments && input->ops.format_text)
        text_length = input->ops.format_text(input->context, text,
            (StarsU16)sizeof text, format_id, gauge_value, total,
            format_arguments);
    if (input->ops.text_extent && input->ops.center_text) {
        extent = (StarsI16)input->ops.text_extent(input->context, input->hdc,
            text, text_length);
        if (extent < (StarsI16)(input->rect->right - input->rect->left) - 3)
            input->ops.center_text(input->context, input->hdc, input->rect,
                text, text_length);
    }
    if (input->ops.set_bk_mode)
        input->ops.set_bk_mode(input->context, input->hdc, old_mode);
}
