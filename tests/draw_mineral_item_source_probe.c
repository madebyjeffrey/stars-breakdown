#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct MineralState {
    int colors;
    StarsI16 color_index;
    StarsU32 text_color;
    int formatted;
    StarsI32 formatted_value;
    int drawn;
    StarsU16 hdc;
    StarsI16 x;
    StarsI16 y;
    StarsI16 length;
    StarsI16 erase;
    char text[16];
} MineralState;

static StarsU32 get_color(void *context, StarsI16 mineral)
{
    MineralState *state = (MineralState *)context;
    ++state->colors;
    state->color_index = mineral;
    return 0x123456UL;
}

static void set_text_color(void *context, StarsU16 hdc, StarsU32 color)
{
    MineralState *state = (MineralState *)context;
    (void)hdc;
    state->text_color = color;
}

static const StarsU8 *format_long(void *context, StarsI32 value,
    StarsI16 *length)
{
    MineralState *state = (MineralState *)context;
    ++state->formatted;
    state->formatted_value = value;
    *length = 3;
    return (const StarsU8 *)"1.2";
}

static void right_text(void *context, StarsU16 hdc, StarsI16 x, StarsI16 y,
    const StarsU8 *text, StarsI16 length, StarsI16 erase)
{
    MineralState *state = (MineralState *)context;
    ++state->drawn;
    state->hdc = hdc; state->x = x; state->y = y;
    state->length = length; state->erase = erase;
    if (text && length > 0) {
        memcpy(state->text, text, (size_t)length);
        state->text[length] = '\0';
    } else state->text[0] = '\0';
}

int main(void)
{
    static const StarsU8 dash[] = "--";
    StarsDrawMineralItemOps ops;
    MineralState state;
    int failures = 0;

    ops.get_color = get_color;
    ops.set_text_color = set_text_color;
    ops.format_long = format_long;
    ops.right_text = right_text;
    ops.double_dash = dash;
    memset(&state, 0, sizeof(state));
    stars_draw_mineral_item(7U, 11, 13, -1, -1L, &ops, &state);
    if (state.colors != 0 || state.text_color != 0UL || state.formatted != 0 ||
        state.drawn != 1 || state.hdc != 7U || state.x != 11 || state.y != 13 ||
        state.length != 2 || strcmp(state.text, "--") != 0 || state.erase != 0)
        ++failures;

    memset(&state, 0, sizeof(state));
    stars_draw_mineral_item(8U, 17, 19, 2, 12345L, &ops, &state);
    if (state.colors != 1 || state.color_index != 2 ||
        state.text_color != 0x123456UL || state.formatted != 1 ||
        state.formatted_value != 12345L || state.drawn != 1 || state.length != 3 ||
        strcmp(state.text, "1.2") != 0)
        ++failures;

    printf("DRAW_MINERAL_ITEM_CHECKS=2 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
