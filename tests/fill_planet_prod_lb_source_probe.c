#include <stdio.h>
#include <string.h>
#include "core.h"

struct ProbeState {
    StarsU16 clears;
    StarsU16 adds;
    StarsU8 last[128];
    StarsU16 string_id;
};

static void get_string(void *context, StarsU16 id, StarsU8 *output,
    StarsU16 capacity)
{
    struct ProbeState *state = (struct ProbeState *)context;
    const char *text = id == 0x33cU ? "No production" : "Production";
    state->string_id = id;
    strncpy((char *)output, text, capacity - 1U);
    output[capacity - 1U] = 0;
}

static void name_item(void *context, StarsU32 packed, StarsU8 *output,
    StarsU16 capacity)
{
    (void)context;
    (void)packed;
    strncpy((char *)output, "Scout", capacity - 1U);
    output[capacity - 1U] = 0;
}

static void estimate(void *context, const void *planet, const StarsU32 *items,
    StarsU16 index, StarsI16 *first, StarsI16 *last)
{
    (void)planet;
    (void)items;
    (void)index;
    if (((struct ProbeState *)context)->string_id == 0U) {
        *first = 0;
        *last = 1;
    } else {
        *first = 1;
        *last = 1;
    }
}

static void format_line(void *context, StarsU8 marker, StarsU16 count,
    const StarsU8 *name, StarsU8 *output, StarsU16 capacity)
{
    (void)context;
    (void)count;
    strncpy((char *)output, "x", capacity - 1U);
    output[capacity - 1U] = 0;
    output[0] = marker;
    output[1] = ' ';
    output[5] = 0;
    strncpy((char *)output + 2, (const char *)name, capacity - 3U);
}

static void send_message(void *context, StarsU16 hwnd, StarsU16 message,
    const StarsU8 *value)
{
    struct ProbeState *state = (struct ProbeState *)context;
    (void)hwnd;
    if (message == 0x405U)
        ++state->clears;
    else if (message == 0x401U) {
        ++state->adds;
        strncpy((char *)state->last, (const char *)value,
            sizeof state->last - 1U);
        state->last[sizeof state->last - 1U] = 0;
    }
}

static StarsU32 packed(StarsU16 kind, StarsU16 item, StarsU16 count)
{
    return ((StarsU32)kind << 17) | ((StarsU32)item << 10) | count;
}

int main(void)
{
    struct ProbeState state;
    StarsU32 items[2];
    StarsFillPlanetProdQueue selected;
    StarsFillPlanetProdQueue direct;
    StarsFillPlanetProdInput input;
    StarsU8 work[128];

    memset(&state, 0, sizeof state);
    memset(&input, 0, sizeof input);
    input.default_hwnd = 7U;
    input.selected_queue = &selected;
    input.work = work;
    input.work_capacity = sizeof work;
    input.get_string = get_string;
    input.name_item = name_item;
    input.estimate_schedule = estimate;
    input.format_line = format_line;
    input.send_message = send_message;
    input.context = &state;
    selected.items = 0;
    selected.item_count = 0U;
    stars_fill_planet_production_list(&input);
    if (state.clears != 1U || state.adds != 1U || state.string_id != 0x33cU)
        return 1;

    state.clears = 0U;
    state.adds = 0U;
    state.string_id = 1U;
    items[0] = packed(1U, 0U, 12U);
    items[1] = packed(1U, 4U, 8U);
    selected.items = items;
    selected.item_count = 2U;
    stars_fill_planet_production_list(&input);
    if (state.clears != 1U || state.adds != 2U || state.last[0] != '*')
        return 2;

    state.string_id = 0U;
    direct.items = items;
    direct.item_count = 1U;
    input.queue = &direct;
    stars_fill_planet_production_list(&input);
    if (work[0] != '#' || work[2] != 'S')
        return 3;
    puts("FILL_PLANET_PROD_LB_CHECKS=3 FAILURES=0 RESULT=PASS");
    return 0;
}
