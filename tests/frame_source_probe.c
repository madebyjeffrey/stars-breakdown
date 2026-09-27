#include <stdio.h>
#include <string.h>
#include "core.h"

static StarsFrameOps ops;
static unsigned get_dcs;
static unsigned releases;
static unsigned fonts;
static unsigned metrics;
static unsigned tiles;
static unsigned tile_sizes;
static unsigned refits;
static unsigned begins;
static unsigned ends;
static unsigned selects;
static unsigned blits;
static unsigned fills;
static unsigned sends;
static unsigned destroys;
static unsigned kills;
static unsigned writes;
static unsigned marks;
static unsigned posts;
static unsigned children;
static unsigned commands;
static unsigned inits;
static unsigned colors;
static unsigned icons;
static StarsU16 last_send_message;
static StarsU16 last_send_wparam;
static StarsU16 last_icon;
static StarsI16 last_dx;
static StarsI16 last_dy;

static StarsU16 get_dc(void *context, StarsU16 hwnd)
{ (void)context; (void)hwnd; ++get_dcs; return 9U; }
static void release_dc(void *context, StarsU16 hwnd, StarsU16 dc)
{ (void)context; (void)hwnd; (void)dc; ++releases; }
static StarsI16 create_fonts(void *context, StarsU16 dc)
{ (void)context; (void)dc; ++fonts; return 0; }
static StarsI16 get_metrics(void *context, StarsU16 dc)
{ (void)context; (void)dc; ++metrics; return 14; }
static void init_tiles(void *context)
{ (void)context; ++tiles; }
static void ensure_tiles(void *context)
{ (void)context; ++tile_sizes; }
static void write_ini(void *context)
{ (void)context; ++writes; }
static void destroy_window(void *context, StarsU16 hwnd)
{ (void)context; (void)hwnd; ++destroys; }
static StarsI16 mark_file(void *context, StarsI16 type, StarsI16 player,
    StarsI16 mask, StarsI16 value)
{ (void)context; (void)type; (void)player; (void)mask; (void)value; ++marks; return 1; }
static void destroy_game(void *context)
{ (void)context; ++destroys; }
static void post_quit(void *context, StarsI16 value)
{ (void)context; (void)value; ++posts; }
static void refit(void *context)
{ (void)context; ++refits; }
static StarsU16 begin_paint(void *context, StarsU16 hwnd, StarsRect *rect)
{ (void)context; (void)hwnd; rect->left = 0; rect->top = 0; rect->right = 800;
    rect->bottom = 600; ++begins; return 10U; }
static void end_paint(void *context, StarsU16 hwnd, const StarsRect *rect)
{ (void)context; (void)hwnd; (void)rect; ++ends; }
static StarsI16 iconic(void *context, StarsU16 hwnd)
{ (void)context; (void)hwnd; return 0; }
static StarsU16 select_object(void *context, StarsU16 dc, StarsU16 object)
{ (void)context; (void)dc; (void)object; ++selects; return 77U; }
static void pat_blt(void *context, StarsU16 dc, StarsI16 x, StarsI16 y,
    StarsI16 width, StarsI16 height, StarsU32 rop)
{ (void)context; (void)dc; (void)width; (void)height; (void)rop; ++blits;
    last_dx = x; last_dy = y; }
static void draw_icon(void *context, StarsU16 dc, StarsI16 x, StarsI16 y,
    StarsU16 icon)
{ (void)context; (void)dc; (void)x; (void)y; ++icons; last_icon = icon; }
static void get_rect(void *context, StarsU16 hwnd, StarsRect *rect)
{ (void)context; (void)hwnd; rect->left = 0; rect->top = 0; rect->right = 800;
    rect->bottom = 600; }
static void fill_rect(void *context, StarsU16 dc, const StarsRect *rect,
    StarsU16 brush)
{ (void)context; (void)dc; (void)rect; (void)brush; ++fills; }
static void send_message(void *context, StarsU16 hwnd, StarsU16 message,
    StarsU16 wparam, StarsU32 lparam)
{ (void)context; (void)hwnd; (void)lparam; ++sends; last_send_message = message;
    last_send_wparam = wparam; }
static void command_handler(void *context, StarsU16 hwnd, StarsU16 command)
{ (void)context; (void)hwnd; (void)command; ++commands; }
static void initialize_menu(void *context, StarsU16 menu)
{ (void)context; (void)menu; ++inits; }
static void get_system_colors(void *context)
{ (void)context; ++colors; }
static StarsU32 default_proc(void *context, StarsU16 hwnd, StarsU16 message,
    StarsU16 wparam, StarsU32 lparam)
{ (void)context; (void)hwnd; (void)message; (void)wparam; (void)lparam; return 0x1234UL; }
static void kill_timer(void *context, StarsU16 hwnd, StarsU16 timer)
{ (void)context; (void)hwnd; (void)timer; ++kills; }
static void create_children(void *context)
{ (void)context; ++children; }

static void reset(void)
{
    memset(&ops, 0, sizeof(ops));
    get_dcs = releases = fonts = metrics = tiles = tile_sizes = refits = 0U;
    begins = ends = selects = blits = fills = sends = destroys = kills = 0U;
    writes = marks = posts = children = commands = inits = colors = icons = 0U;
    last_send_message = last_send_wparam = last_icon = 0U;
    last_dx = last_dy = 0;
    ops.get_dc = get_dc; ops.release_dc = release_dc;
    ops.create_fonts = create_fonts; ops.get_text_metrics = get_metrics;
    ops.init_tiles = init_tiles; ops.ensure_tile_size = ensure_tiles;
    ops.write_ini = write_ini; ops.destroy_window = destroy_window;
    ops.mark_file = mark_file; ops.destroy_game = destroy_game;
    ops.post_quit = post_quit; ops.refit = refit;
    ops.begin_paint = begin_paint; ops.end_paint = end_paint;
    ops.is_iconic = iconic; ops.select_object = select_object;
    ops.pat_blt = pat_blt; ops.draw_icon = draw_icon;
    ops.get_client_rect = get_rect; ops.fill_rect = fill_rect;
    ops.default_proc = default_proc; ops.send_message = send_message;
    ops.command_handler = command_handler; ops.initialize_menu = initialize_menu;
    ops.get_system_colors = get_system_colors; ops.kill_timer = kill_timer;
    ops.create_children = create_children;
}
static void base_state(StarsFrameState *state)
{
    memset(state, 0, sizeof(*state)); state->frame = 0x20U;
    state->dy_arial8 = 8; state->dx = 800; state->dy = 600;
    state->x_top = 200; state->y1 = 100; state->y2 = 400;
    state->hbr_shadow = 1U; state->hbr_hilite = 2U; state->hbr_face = 3U;
    state->hbr_desktop = 4U; state->hicon_host = 5U; state->hicon_stars = 6U;
    state->hicon_wait = 7U;
}
static int setup_case(void)
{
    StarsFrameState state;
    StarsU32 packed;
    base_state(&state); reset();
    if (stars_frame_wnd_proc(&state, 0x20U, 1U, 0U, 0UL, &ops, 0) != 0UL)
        return 1;
    packed = (StarsU32)800U | ((StarsU32)600U << 16);
    stars_frame_wnd_proc(&state, 0x20U, 5U, 0U, packed, &ops, 0);
    return get_dcs == 1U && releases == 1U && fonts == 1U && metrics == 1U &&
        tiles == 1U && tile_sizes == 1U && refits == 1U && state.dx == 800 &&
        state.dy == 600 ? 0 : 1;
}
static int paint_case(void)
{
    StarsFrameState state;
    StarsU32 query_icon;
    base_state(&state); reset();
    stars_frame_wnd_proc(&state, 0x20U, 0x0fU, 0U, 0UL, &ops, 0);
    state.player = 1; state.game_lid = 1UL; state.timer_id = 1U;
    query_icon = stars_frame_wnd_proc(&state, 0x20U, 0x37U, 0U, 0UL, &ops, 0);
    return begins == 1U && ends == 1U && blits == 8U && icons == 0U &&
        query_icon == 7U ? 0 : 1;
}
static int routing_case(void)
{
    StarsFrameState state;
    base_state(&state); state.scanner = 9U; reset();
    stars_frame_wnd_proc(&state, 0x20U, 0x102U, 0x2bU, 0UL, &ops, 0);
    stars_frame_wnd_proc(&state, 0x20U, 0x111U, 0x44U, 0UL, &ops, 0);
    stars_frame_wnd_proc(&state, 0x20U, 0x116U, 0U, 0UL, &ops, 0);
    return sends == 1U && last_send_message == 0x102U && commands == 1U &&
        inits == 1U ? 0 : 1;
}
static int shutdown_case(void)
{
    StarsFrameState state;
    base_state(&state); state.timer_id = 3U; state.global_low_flags = 8U;
    reset(); stars_frame_wnd_proc(&state, 0x20U, 2U, 0U, 0UL, &ops, 0);
    return kills == 1U && writes == 1U && marks == 1U && destroys == 1U &&
        posts == 1U && state.timer_id == 0U ? 0 : 1;
}
int main(void)
{
    int failures = setup_case() + paint_case() + routing_case() + shutdown_case();
    printf("FRAME_CHECKS=4 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures;
}
