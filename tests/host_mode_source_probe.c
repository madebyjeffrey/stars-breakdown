#include <stdio.h>
#include <string.h>
#include "core.h"

static unsigned paints;
static unsigned ends;
static unsigned fills;
static unsigned draws;
static unsigned stickies;
static unsigned texts;
static unsigned enables;
static unsigned timers;
static unsigned kills;
static unsigned cursors;
static unsigned menus;
static unsigned appends;
static unsigned tracks;
static unsigned destroys;
static unsigned peeks;
static unsigned marks;
static unsigned ensures;
static unsigned quits;
static unsigned ends_dialog;
static unsigned alerts;
static unsigned sets_focus;
static unsigned options;
static StarsU16 pending_command;
static StarsI16 outstanding_value;
static StarsU16 last_end;
static StarsI16 last_end_value;
static StarsU16 last_timer;
static StarsI16 last_mark_player;
static StarsU32 last_mark_value;
static char last_text[128];

static StarsU16 begin_paint(void *context, StarsU16 hwnd, StarsRect *rect)
{ (void)context; (void)hwnd; rect->left = 0; rect->top = 0;
    rect->right = 640; rect->bottom = 480; ++paints; return 0x55U; }
static void end_paint(void *context, StarsU16 hwnd, const StarsRect *rect)
{ (void)context; (void)hwnd; (void)rect; ++ends; }
static void get_client_rect(void *context, StarsU16 hwnd, StarsRect *rect)
{ (void)context; (void)hwnd; rect->left = 1; rect->top = 2;
    rect->right = 100; rect->bottom = 200; }
static void fill_rect(void *context, StarsU16 dc, const StarsRect *rect,
    StarsU16 brush)
{ (void)context; (void)dc; (void)rect; (void)brush; ++fills; }
static StarsU32 set_bk_color(void *context, StarsU16 dc, StarsU32 color)
{ (void)context; (void)dc; (void)color; return 0xabcUL; }
static void draw_dialog(void *context, StarsU16 hwnd, StarsU16 dc)
{ (void)context; (void)hwnd; (void)dc; ++draws; }
static void sticky(void *context, StarsU16 hwnd, StarsU16 save)
{ (void)context; (void)hwnd; (void)save; ++stickies; }
static void set_window_text(void *context, StarsU16 hwnd, const StarsU8 *text)
{ (void)context; (void)hwnd; ++texts; if (text) strcpy(last_text, (const char *)text); }
static void enable_window(void *context, StarsU16 hwnd, StarsU16 enabled)
{ (void)context; (void)hwnd; (void)enabled; ++enables; }
static StarsU16 get_item(void *context, StarsU16 hwnd, StarsU16 id)
{ (void)context; (void)hwnd; return (StarsU16)(id + 0x100U); }
static StarsU16 set_timer(void *context, StarsU16 hwnd, StarsU16 id,
    StarsU32 interval)
{ (void)context; (void)hwnd; (void)interval; ++timers; last_timer = id; return 0x77U; }
static void kill_timer(void *context, StarsU16 hwnd, StarsU16 id)
{ (void)context; (void)hwnd; (void)id; ++kills; }
static void get_cursor(void *context, StarsI16 *x, StarsI16 *y)
{ (void)context; *x = 6; *y = 49; }
static void map_point(void *context, StarsU16 hwnd, StarsI16 *x, StarsI16 *y)
{ (void)context; (void)hwnd; (void)x; (void)y; }
static void set_cursor(void *context, StarsU16 cursor)
{ (void)context; (void)cursor; ++cursors; }
static StarsU16 create_menu(void *context)
{ (void)context; ++menus; return 0x88U; }
static void append_menu(void *context, StarsU16 menu, StarsU16 flags,
    const StarsU8 *label)
{ (void)context; (void)menu; (void)flags; (void)label; ++appends; }
static void track_menu(void *context, StarsU16 menu, StarsU16 flags,
    StarsI16 x, StarsI16 y, StarsU16 owner)
{ (void)context; (void)menu; (void)flags; (void)x; (void)y; (void)owner; ++tracks; }
static void destroy_menu(void *context, StarsU16 menu)
{ (void)context; (void)menu; ++destroys; }
static StarsU16 peek_message(void *context, StarsU16 hwnd, StarsU16 min,
    StarsU16 max, StarsU16 remove, StarsU16 *command)
{ (void)context; (void)hwnd; (void)min; (void)max; (void)remove; ++peeks;
    *command = pending_command; return 1U; }
static StarsI16 async_key(void *context, StarsU16 key)
{ (void)context; (void)key; return 0; }
static StarsI16 outstanding(void *context)
{ (void)context; return outstanding_value; }
static void mark_file(void *context, StarsI16 type, StarsI16 player,
    StarsI16 mask, StarsI16 value)
{ (void)context; (void)type; (void)mask; ++marks; last_mark_player = player;
    last_mark_value = (StarsU32)value; }
static void ensure(void *context)
{ (void)context; ++ensures; }
static void post_quit(void *context, StarsI16 value)
{ (void)context; (void)value; ++quits; }
static void end_dialog(void *context, StarsU16 hwnd, StarsI16 value)
{ (void)context; (void)hwnd; ++ends_dialog; last_end = hwnd; last_end_value = value; }
static StarsI16 alert(void *context, StarsU16 id, StarsI16 value)
{ (void)context; (void)id; (void)value; ++alerts; return 6; }
static StarsU16 get_focus(void *context)
{ (void)context; return 0x66U; }
static StarsI16 message_box(void *context, StarsU16 owner, const StarsU8 *text,
    const StarsU8 *caption, StarsU16 style)
{ (void)context; (void)owner; (void)text; (void)caption; (void)style; return 6; }
static StarsI16 check_password(void *context)
{ (void)context; return 1; }
static StarsU32 make_proc(void *context, StarsU16 instance)
{ (void)context; (void)instance; return 0x1234UL; }
static StarsI16 dialog_box(void *context, StarsU16 resource, StarsU16 owner,
    StarsU32 proc)
{ (void)context; (void)resource; (void)owner; (void)proc; return 1; }
static void free_proc(void *context, StarsU32 proc)
{ (void)context; (void)proc; }
static void set_focus(void *context, StarsU16 hwnd)
{ (void)context; (void)hwnd; ++sets_focus; }
static void win_help(void *context, StarsU16 hwnd, const StarsU8 *file,
    StarsU16 command, StarsU16 data)
{ (void)context; (void)hwnd; (void)file; (void)command; (void)data; }
static StarsI16 options_dialog(void *context, StarsU16 hwnd)
{ (void)context; (void)hwnd; ++options; return 1; }
static const StarsU8 *string(void *context, StarsU16 id)
{ (void)context; (void)id; return (const StarsU8 *)"mode"; }
static const StarsU8 *compressed(void *context, StarsU16 id)
{ (void)context; (void)id; return (const StarsU8 *)"pass %d"; }
static StarsU16 get_window(void *context, StarsU16 hwnd, StarsU16 relation)
{ (void)context; (void)hwnd; (void)relation; return 0; }

static StarsHostModeOps ops = { begin_paint, end_paint, fill_rect,
    get_client_rect, set_bk_color, draw_dialog, sticky, set_window_text,
    enable_window, get_item, get_window, set_timer, kill_timer, get_cursor,
    map_point, map_point, set_cursor, create_menu, append_menu, track_menu,
    destroy_menu, peek_message, async_key, outstanding, mark_file, ensure,
    post_quit, end_dialog, alert, get_focus, message_box, check_password,
    make_proc, dialog_box, free_proc, set_focus, win_help, options_dialog,
    string, compressed };

static void reset(void)
{
    paints = ends = fills = draws = stickies = texts = enables = timers = 0U;
    kills = cursors = menus = appends = tracks = destroys = peeks = marks = 0U;
    ensures = quits = ends_dialog = alerts = sets_focus = options = 0U;
    pending_command = 0U; last_end = last_end_value = 0; last_timer = 0U;
    last_mark_player = 0; last_mark_value = 0UL; last_text[0] = 0;
}
static int init_paint_case(void)
{
    StarsHostModeState state;
    StarsI16 result;
    memset(&state, 0, sizeof(state)); state.frame = 0x20U; state.player_count = 1U;
    state.dy_arial8 = 8; state.button_brush = 4U; state.auto_generate = 1U;
    state.game_name = (const StarsU8 *)"GAME"; state.base_name =
        (const StarsU8 *)"BASE"; reset();
    result = stars_host_mode_dialog(&state, 0x20U, 0x110U, 0U, 0UL, &ops, 0);
    result = (StarsI16)(result + stars_host_mode_dialog(&state, 0x20U,
        0x0fU, 0U, 0UL, &ops, 0));
    return result == 2 && paints == 1U && ends == 1U && draws == 2U &&
        timers == 1U && last_timer == 0x0dU && texts == 2U ? 0 : 1;
}
static int popup_case(void)
{
    StarsHostModeState state;
    memset(&state, 0, sizeof(state)); state.frame = 0x20U;
    state.player_count = 1U; state.dy_arial8 = 8; state.hand_cursor = 9U;
    state.players[0].salt = 0x12345678UL; reset(); pending_command = 15001U;
    stars_host_mode_dialog(&state, 0x20U, 0x201U, 0U, 0UL, &ops, 0);
    return menus == 1U && appends == 3U && tracks == 1U && destroys == 1U &&
        peeks == 1U && marks == 2U && state.players[0].detail == 0x200U &&
        state.players[0].salt == (StarsU32)~(StarsU32)0x12345678UL &&
        draws == 1U ? 0 : 1;
}
int main(void)
{
    int failures = init_paint_case() + popup_case();
    StarsHostModeState state;
    memset(&state, 0, sizeof(state)); state.frame = 0x20U;
    state.global_low_flags = 4U; reset();
    stars_host_mode_dialog(&state, 0x20U, 0x111U, 2U, 0UL, &ops, 0);
    outstanding_value = 1;
    stars_host_mode_dialog(&state, 0x20U, 0x113U, 0U, 0UL, &ops, 0);
    failures += !(ends_dialog == 1U && last_end_value == 0 && quits == 1U && draws == 1U);
    printf("HOST_MODE_CHECKS=3 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures;
}
