#include <stdio.h>
#include <string.h>
#include "core.h"

static unsigned diamonds;
static unsigned right_texts;
static unsigned text_outs;
static unsigned colors;
static unsigned controls;
static unsigned releases;
static StarsI16 last_right_x;
static StarsI16 last_right_y;
static StarsI16 last_text_x;
static StarsI16 last_text_y;
static StarsU16 last_control;
static char last_turn[128];
static char last_time[128];
static StarsU32 fake_tick;

static const StarsU8 *compressed(void *context, StarsU16 id)
{
    (void)context;
    switch (id) {
    case 0x2f1U: return (const StarsU8 *)"%d";
    case 0x2f2U: return (const StarsU8 *)"TIME";
    case 0x2f5U: return (const StarsU8 *)"%lu sec";
    case 0x2f6U: return (const StarsU8 *)"%lu min %lu sec";
    case 0x2f7U: return (const StarsU8 *)"%lu hr %lu min %lu sec";
    case 0x2f8U: return (const StarsU8 *)"%lu d %lu hr %lu min %lu sec";
    case 0x2fdU: return (const StarsU8 *)"%s: %s";
    case 0x3ffU: return (const StarsU8 *)"%s";
    case 0x403U: return (const StarsU8 *)" [DEAD!!]";
    default: return (const StarsU8 *)"";
    }
}
static StarsU16 get_string(void *context, StarsU16 id, StarsU8 *output,
    StarsU16 capacity)
{
    const char *text = id == 0x2ccU ? "Ready" : "Busy";
    StarsU16 length = (StarsU16)strlen(text);
    (void)context;
    if (length >= capacity) length = (StarsU16)(capacity - 1U);
    memcpy(output, text, length); output[length] = 0;
    return length;
}
static const StarsU8 *player_name(void *context, StarsI16 index)
{ (void)context; return index == 0 ? (const StarsU8 *)"P1" :
    (const StarsU8 *)"P2"; }
static StarsU16 get_dc(void *context, StarsU16 hwnd)
{ (void)context; (void)hwnd; return 0x44U; }
static void release_dc(void *context, StarsU16 hwnd, StarsU16 dc)
{ (void)context; (void)hwnd; (void)dc; ++releases; }
static StarsU16 set_bk_mode(void *context, StarsU16 dc, StarsU16 mode)
{ (void)context; (void)dc; return mode == 2U ? 1U : 2U; }
static StarsU32 set_bk_color(void *context, StarsU16 dc, StarsU32 color)
{ (void)context; (void)dc; (void)color; return 0x11111111UL; }
static void select_font(void *context, StarsU16 dc, StarsU16 font)
{ (void)context; (void)dc; (void)font; }
static void set_text_color(void *context, StarsU16 dc, StarsU32 color)
{ (void)context; (void)dc; (void)color; ++colors; }
static void set_rect(void *context, StarsRect *rect, StarsI16 left,
    StarsI16 top, StarsI16 right, StarsI16 bottom)
{ (void)context; rect->left = (StarsU16)left; rect->top = (StarsU16)top;
    rect->right = (StarsU16)right; rect->bottom = (StarsU16)bottom; }
static void offset_rect(void *context, StarsRect *rect, StarsI16 dx,
    StarsI16 dy)
{ (void)context; rect->left = (StarsU16)(rect->left + dx);
    rect->right = (StarsU16)(rect->right + dx);
    rect->top = (StarsU16)(rect->top + dy);
    rect->bottom = (StarsU16)(rect->bottom + dy); }
static void draw_diamond(void *context, StarsU16 dc, const StarsRect *rect,
    StarsU16 brush)
{ (void)context; (void)dc; (void)rect; (void)brush; ++diamonds; }
static void right_text(void *context, StarsU16 dc, StarsI16 x, StarsI16 y,
    const StarsU8 *text, StarsU16 length, StarsI16 mode)
{ (void)context; (void)dc; (void)text; (void)length; (void)mode;
    ++right_texts; last_right_x = x; last_right_y = y; }
static void text_out(void *context, StarsU16 dc, StarsI16 x, StarsI16 y,
    const StarsU8 *text, StarsU16 length)
{ (void)context; (void)dc; (void)length; ++text_outs; last_text_x = x;
    last_text_y = y; if (y == 48) strcpy(last_turn, (const char *)text);
    else strcpy(last_time, (const char *)text); }
static StarsI16 text_extent(void *context, StarsU16 dc, const StarsU8 *text,
    StarsU16 length)
{ (void)context; (void)dc; (void)text; return (StarsI16)length; }
static StarsU16 get_dialog_item(void *context, StarsU16 hwnd, StarsU16 id)
{ (void)context; (void)hwnd; ++controls; return id == 0x7e0U ? 0x70U : 0x71U; }
static void set_window_text(void *context, StarsU16 hwnd, const StarsU8 *text)
{ (void)context; if (hwnd == 0x70U) strcpy(last_turn, (const char *)text);
    else { strcpy(last_time, (const char *)text); last_control = hwnd; } }
static StarsU32 get_tick(void *context)
{ (void)context; return fake_tick; }

static StarsDrawHostOps ops = { compressed, get_string, player_name, get_dc,
    release_dc, set_bk_mode, set_bk_color, select_font, set_text_color,
    set_rect, offset_rect, draw_diamond, right_text, text_out, text_extent,
    get_dialog_item, set_window_text, get_tick };

static void reset(void)
{
    diamonds = right_texts = text_outs = colors = controls = releases = 0U;
    last_right_x = last_right_y = last_text_x = last_text_y = 0;
    last_control = 0U; last_turn[0] = 0; last_time[0] = 0;
}
static int short_time_case(void)
{
    StarsDrawHostState state;
    memset(&state, 0, sizeof(state));
    state.frame = 0x33U; state.player_count = 2U;
    state.output[0] = 0; state.output[1] = 1;
    state.dead_flags[1] = 0x10U; state.font = 10U; state.blue_brush = 11U;
    state.button_face = 0x22222222UL; state.window_text = 0x33333333UL;
    state.dy_arial8 = 8; state.turn = 3; state.turn_format =
        (const StarsU8 *)"Turn %d"; state.tick_last = 1000UL;
    fake_tick = 6000UL; reset();
    stars_draw_host_dialog(&state, 0U, &ops, 0);
    return diamonds == 2U && right_texts == 2U && text_outs == 2U &&
        controls == 2U && releases == 1U && last_right_x == 22 &&
        last_right_y == 60 && last_text_x == 26 && last_text_y == 60 &&
        strcmp(last_turn, "Turn 2404") == 0 &&
        strcmp(last_time, "5 sec") == 0 && last_control == 0x71U ? 0 : 1;
}
static int long_time_case(void)
{
    StarsDrawHostState state;
    memset(&state, 0, sizeof(state));
    state.frame = 0x33U; state.turn_format = (const StarsU8 *)"Turn %d";
    state.tick_last = 1000UL; fake_tick = 90061000UL; reset();
    stars_draw_host_dialog(&state, 0x55U, &ops, 0);
    return strcmp(last_time, "1 d 1 hr 1 min 0 sec") == 0 &&
        releases == 0U ? 0 : 1;
}
int main(void)
{
    int failures = short_time_case() + long_time_case();
    printf("DRAW_HOST_CHECKS=2 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures;
}
