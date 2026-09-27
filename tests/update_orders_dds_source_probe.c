#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI32 order_mode;
    StarsI32 second_selection;
    StarsI32 selection_result;
    StarsI32 fleet_stat;
    StarsU16 resets[3];
    StarsU16 adds[3];
    StarsU16 selects[3];
    StarsU16 format_calls;
    StarsU16 last_format;
    StarsI16 last_arguments;
    StarsU16 player_names;
    StarsU16 window_texts;
} Trace;

static StarsI32 send_message(void *context, StarsU16 window, StarsU16 message,
    StarsU16 selection, const StarsU8 *text)
{
    Trace *t = (Trace *)context;
    StarsI16 column = (StarsI16)(window - 10U);
    (void)selection;
    (void)text;
    if (column >= 0 && column < 3) {
        if (message == 0x0407)
            ++t->resets[column];
        else if (message == 0x0403)
            ++t->adds[column];
        else if (message == 0x040e)
            ++t->selects[column];
    }
    if (window == 10U && message == 0x040e)
        return t->order_mode;
    if (window == 11U && message == 0x0407)
        return t->second_selection;
    if (message == 0x040e)
        return t->selection_result;
    return 0;
}

static StarsI32 fleet_stat(void *context, const void *fleet, StarsI16 stat)
{
    Trace *t = (Trace *)context;
    (void)fleet;
    (void)stat;
    return t->fleet_stat;
}

static StarsI16 get_string(void *context, StarsU16 id, StarsU8 *out,
    StarsU16 capacity)
{
    (void)context;
    (void)id;
    (void)capacity;
    strcpy((char *)out, "S");
    return 1;
}

static StarsI16 get_player_name(void *context, StarsI16 player, StarsU8 *out,
    StarsU16 capacity)
{
    Trace *t = (Trace *)context;
    (void)player;
    (void)capacity;
    ++t->player_names;
    strcpy((char *)out, "P");
    return 1;
}

static StarsI16 format_text(void *context, StarsU8 *out, StarsU16 capacity,
    StarsU16 id, StarsI32 value, StarsI32 second, StarsI16 arguments)
{
    Trace *t = (Trace *)context;
    (void)capacity;
    (void)value;
    (void)second;
    strcpy((char *)out, "F");
    ++t->format_calls;
    t->last_format = id;
    t->last_arguments = arguments;
    return 1;
}

static void set_window_text(void *context, StarsU16 window,
    const StarsU8 *text)
{
    Trace *t = (Trace *)context;
    (void)window;
    (void)text;
    ++t->window_texts;
}

int main(void)
{
    StarsUpdateOrdersInput input;
    Trace trace;
    StarsI32 values[5] = { 0x1123L, 0L, 0x2000L, 0L, 0L };

    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    input.order_dropdowns[0] = 10U;
    input.order_dropdowns[1] = 11U;
    input.order_dropdowns[2] = 12U;
    input.order_edit = 13U;
    input.order_values[0] = values[0];
    input.order_values[1] = values[1];
    input.order_values[2] = values[2];
    input.order_values[3] = values[3];
    input.order_values[4] = values[4];
    input.mineral_names[0] = (const StarsU8 *)"A";
    input.mineral_names[1] = (const StarsU8 *)"B";
    input.mineral_names[2] = (const StarsU8 *)"C";
    input.mineral_names[3] = (const StarsU8 *)"D";
    input.mineral_names[4] = (const StarsU8 *)"E";
    input.player_count = 4;
    input.player_id = 1;
    input.ops.send_message = send_message;
    input.ops.get_fleet_stat = fleet_stat;
    input.ops.get_string = get_string;
    input.ops.get_player_name = get_player_name;
    input.ops.format_text = format_text;
    input.ops.set_window_text = set_window_text;
    input.context = &trace;

    trace.order_mode = 1;
    trace.fleet_stat = 100L;
    trace.selection_result = 1;
    stars_update_orders_dds(0, &input);
    if (trace.resets[1] != 1U || trace.adds[1] != 5U ||
            trace.selects[1] != 1U || trace.adds[2] != 10U ||
            trace.selects[2] != 1U || trace.format_calls != 1U ||
            trace.last_format != 0x0994U || trace.window_texts != 1U)
        return 1;

    memset(&trace, 0, sizeof trace);
    trace.order_mode = 7;
    trace.selection_result = 0;
    stars_update_orders_dds(0, &input);
    if (trace.resets[1] != 1U || trace.adds[1] != 12U ||
            trace.selects[1] != 1U || trace.adds[2] != 0U ||
            trace.format_calls != 11U)
        return 2;

    memset(&trace, 0, sizeof trace);
    trace.order_mode = 9;
    stars_update_orders_dds(0, &input);
    if (trace.player_names != 3U || trace.adds[1] != 3U ||
            trace.selects[1] != 1U)
        return 3;
    puts("UPDATE_ORDERS_DDS_SOURCE=PASS");
    return 0;
}
