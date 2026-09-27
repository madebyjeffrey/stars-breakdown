#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct TitleContext {
    StarsTitleOps ops;
    int creates;
    int disables;
    int sends;
    int stars;
    int centered;
    int icons;
    int dibs;
    int allocs;
    int frees;
    int destroys;
    int focuses;
    int shows;
    int opens;
    int menus;
    int posts;
    int writes;
    int quits;
    int exits;
    int invalidates;
    int defaults;
    int palette_realizes;
    int iconic;
    int open_result;
    StarsU16 last_message;
    StarsU16 last_command;
} TitleContext;

static StarsU16 load_resource(void *p, StarsU16 id)
{ (void)p; return id == 0x01c1U ? 77U : 0U; }
static StarsU16 palette_from_dib(void *p, StarsU16 dib)
{ (void)p; return (StarsU16)(dib + 1U); }
static void unlock_resource(void *p, StarsU16 dib)
{ (void)p; (void)dib; }
static void free_resource(void *p, StarsU16 dib)
{ (void)p; (void)dib; }
static void get_client(void *p, StarsU16 hwnd, StarsRect *rect)
{ (void)p; (void)hwnd; rect->left = 0; rect->top = 0; rect->right = 800; rect->bottom = 600; }
static const StarsU8 *get_string(void *p, StarsU16 id)
{ (void)p; return id == 0x0441U ? (const StarsU8 *)"VERSION" : (const StarsU8 *)"BUTTON"; }
static StarsU16 create_window(void *p, const StarsU8 *klass, const StarsU8 *text,
    StarsU32 style, StarsI16 x, StarsI16 y, StarsI16 width, StarsI16 height,
    StarsU16 parent, StarsU16 id, StarsU16 instance)
{ TitleContext *c = (TitleContext *)p; (void)klass; (void)text; (void)style;
    (void)x; (void)y; (void)width; (void)height; (void)parent; (void)instance;
    ++c->creates; return (StarsU16)(100U + id); }
static StarsI16 access_file(void *p, const StarsU8 *name, StarsI16 mode)
{ (void)p; (void)name; (void)mode; return -1; }
static void enable(void *p, StarsU16 hwnd, StarsI16 enabled)
{ TitleContext *c = (TitleContext *)p; (void)hwnd; if (!enabled) ++c->disables; }
static void send(void *p, StarsU16 hwnd, StarsU16 message, StarsU16 wparam, StarsU32 lparam)
{ TitleContext *c = (TitleContext *)p; (void)hwnd; (void)wparam; (void)lparam;
    ++c->sends; c->last_message = message; }
static StarsI16 is_iconic(void *p, StarsU16 hwnd)
{ TitleContext *c = (TitleContext *)p; (void)hwnd; return (StarsI16)c->iconic; }
static StarsU16 begin_paint(void *p, StarsU16 hwnd, StarsRect *rect)
{ (void)p; (void)hwnd; memset(rect, 0, sizeof(*rect)); return 5U; }
static void end_paint(void *p, StarsU16 hwnd, const StarsRect *rect)
{ (void)p; (void)hwnd; (void)rect; }
static StarsU16 select_object(void *p, StarsU16 dc, StarsU16 object)
{ (void)p; (void)dc; return object + 1U; }
static void set_text_color(void *p, StarsU16 dc, StarsU32 color)
{ (void)p; (void)dc; (void)color; }
static void draw_stars(void *p, StarsU16 dc, const StarsRect *rect)
{ TitleContext *c = (TitleContext *)p; (void)dc; (void)rect; ++c->stars; }
static StarsU16 create_font(void *p, StarsI16 height, const StarsU8 *face)
{ (void)p; (void)height; (void)face; return 44U; }
static void delete_object(void *p, StarsU16 object)
{ (void)p; (void)object; }
static void set_bk_mode(void *p, StarsU16 dc, StarsI16 mode)
{ (void)p; (void)dc; (void)mode; }
static void draw_centered(void *p, StarsU16 dc, const StarsRect *rect,
    const StarsU8 *text, StarsI16 length)
{ TitleContext *c = (TitleContext *)p; (void)dc; (void)rect; (void)text; (void)length; ++c->centered; }
static StarsU16 select_palette(void *p, StarsU16 dc, StarsU16 palette, StarsI16 background)
{ (void)p; (void)dc; (void)background; return palette + 1U; }
static StarsU16 realize_palette(void *p, StarsU16 dc)
{ TitleContext *c = (TitleContext *)p; (void)dc; ++c->palette_realizes; return 1U; }
static StarsI16 metric(void *p, StarsU16 metric_id)
{ (void)p; return metric_id == 0U ? 640 : 480; }
static void dib_blt(void *p, StarsU16 dc, StarsI16 x, StarsI16 y, StarsI16 width,
    StarsI16 height, StarsU16 dib, StarsI16 sx, StarsI16 sy, StarsI16 sw,
    StarsI16 sh, StarsU32 rop)
{ TitleContext *c = (TitleContext *)p; (void)dc; (void)x; (void)y; (void)width;
    (void)height; (void)dib; (void)sx; (void)sy; (void)sw; (void)sh; (void)rop; ++c->dibs; }
static void *local_alloc(void *p, StarsU16 size, StarsU16 flags)
{ TitleContext *c = (TitleContext *)p; (void)size; (void)flags; ++c->allocs; return c; }
static void local_free(void *p, void *memory)
{ TitleContext *c = (TitleContext *)p; (void)memory; ++c->frees; }
static void version(void *p, char *output, StarsU16 capacity)
{ (void)p; if (capacity > 0U) { strncpy(output, "1.0", capacity - 1U); output[capacity - 1U] = '\0'; } }
static void get_window(void *p, StarsU16 hwnd, StarsRect *rect)
{ (void)p; (void)hwnd; rect->top = 300; rect->bottom = 320; rect->left = 0; rect->right = 800; }
static StarsU16 get_dc(void *p, StarsU16 hwnd)
{ (void)p; (void)hwnd; return 6U; }
static void release_dc(void *p, StarsU16 hwnd, StarsU16 dc)
{ (void)p; (void)hwnd; (void)dc; }
static void draw_icon(void *p, StarsU16 dc, StarsI16 x, StarsI16 y, StarsU16 icon)
{ TitleContext *c = (TitleContext *)p; (void)dc; (void)x; (void)y; (void)icon; ++c->icons; }
static void set_focus(void *p, StarsU16 hwnd)
{ TitleContext *c = (TitleContext *)p; (void)hwnd; ++c->focuses; }
static void new_game(void *p, StarsU16 hwnd, StarsU16 value)
{ (void)p; (void)hwnd; (void)value; }
static void destroy_window(void *p, StarsU16 hwnd)
{ TitleContext *c = (TitleContext *)p; (void)hwnd; ++c->destroys; }
static void show_window(void *p, StarsU16 hwnd, StarsU16 show)
{ TitleContext *c = (TitleContext *)p; (void)hwnd; (void)show; ++c->shows; }
static StarsI16 open_game(void *p, StarsU16 hwnd, StarsI16 mode)
{ TitleContext *c = (TitleContext *)p; (void)hwnd; (void)mode; ++c->opens; return (StarsI16)c->open_result; }
static void initialize_menu(void *p, StarsU16 menu)
{ TitleContext *c = (TitleContext *)p; (void)menu; ++c->menus; }
static void post_message(void *p, StarsU16 hwnd, StarsU16 message, StarsU16 wparam, StarsU32 lparam)
{ TitleContext *c = (TitleContext *)p; (void)hwnd; (void)lparam; ++c->posts; c->last_command = wparam; c->last_message = message; }
static void start_tutor(void *p, StarsI16 value)
{ (void)p; (void)value; }
static void write_ini(void *p)
{ TitleContext *c = (TitleContext *)p; ++c->writes; }
static void post_quit(void *p, StarsI16 value)
{ TitleContext *c = (TitleContext *)p; (void)value; ++c->quits; }
static void exit_windows(void *p, StarsI16 value)
{ TitleContext *c = (TitleContext *)p; (void)value; ++c->exits; }
static StarsU32 default_proc(void *p, StarsU16 hwnd, StarsU16 message, StarsU16 wparam, StarsU32 lparam)
{ TitleContext *c = (TitleContext *)p; (void)hwnd; (void)message; (void)wparam; (void)lparam; ++c->defaults; return 0x1234UL; }
static void invalidate(void *p, StarsU16 hwnd, StarsI16 erase)
{ TitleContext *c = (TitleContext *)p; (void)hwnd; (void)erase; ++c->invalidates; }

static void setup(TitleContext *c)
{
    memset(c, 0, sizeof(*c)); c->open_result = 1;
    c->ops.load_big_resource = load_resource; c->ops.palette_from_dib = palette_from_dib;
    c->ops.unlock_resource = unlock_resource; c->ops.free_resource = free_resource;
    c->ops.get_client_rect = get_client; c->ops.get_string = get_string;
    c->ops.create_window = create_window; c->ops.access = access_file;
    c->ops.enable_window = enable; c->ops.send_message = send;
    c->ops.is_iconic = is_iconic; c->ops.begin_paint = begin_paint; c->ops.end_paint = end_paint;
    c->ops.select_object = select_object; c->ops.set_text_color = set_text_color;
    c->ops.draw_stars = draw_stars; c->ops.create_font = create_font;
    c->ops.delete_object = delete_object;
    c->ops.set_bk_mode = set_bk_mode; c->ops.draw_centered = draw_centered;
    c->ops.select_palette = select_palette; c->ops.realize_palette = realize_palette;
    c->ops.system_metric = metric; c->ops.dib_blt = dib_blt; c->ops.local_alloc = local_alloc;
    c->ops.local_free = local_free; c->ops.version = version; c->ops.get_window_rect = get_window;
    c->ops.get_dc = get_dc; c->ops.release_dc = release_dc; c->ops.draw_icon = draw_icon;
    c->ops.set_focus = set_focus; c->ops.new_game_wizard = new_game;
    c->ops.destroy_window = destroy_window; c->ops.show_window = show_window;
    c->ops.open_game = open_game; c->ops.initialize_menu = initialize_menu;
    c->ops.post_message = post_message; c->ops.start_tutor = start_tutor;
    c->ops.write_ini = write_ini; c->ops.post_quit = post_quit;
    c->ops.exit_windows = exit_windows; c->ops.default_proc = default_proc;
    c->ops.invalidate_rect = invalidate;
}

static int create_and_paint_case(void)
{
    TitleContext c; StarsTitleState state;
    setup(&c); memset(&state, 0, sizeof(state)); state.screen_colors = 4;
    state.button_face = 1; state.font_arial8 = 2; state.font_arial10 = 3;
    state.frame = 9; state.title = 10; state.instance = 11; state.dy_arial8 = 8;
    state.base_name = (const StarsU8 *)"missing";
    stars_title_wnd_proc(&state, 10U, 1U, 0U, 0UL, &c.ops, &c);
    stars_title_wnd_proc(&state, 10U, 0x000fU, 0U, 0UL, &c.ops, &c);
    return c.creates == 4 && c.disables == 1 && c.stars == 1 && c.centered == 2 &&
        c.allocs == 1 && c.frees == 1 ? 0 : 1;
}

static int high_color_case(void)
{
    TitleContext c; StarsTitleState state;
    setup(&c); memset(&state, 0, sizeof(state)); state.screen_colors = 16;
    state.dib_title = 77; state.palette_splash = 78; state.font_arial10 = 3;
    state.dy_arial8 = 8; state.buttons[0] = 100;
    stars_title_wnd_proc(&state, 10U, 0x000fU, 0U, 0UL, &c.ops, &c);
    return c.dibs == 1 && c.stars == 0 && c.centered == 1 ? 0 : 1;
}

static int commands_case(void)
{
    TitleContext c; StarsTitleState state;
    setup(&c); memset(&state, 0, sizeof(state)); state.frame = 20; state.title = 10;
    state.id_player = 0; state.game_flags = 8; state.planet_present = 1;
    stars_title_wnd_proc(&state, 10U, 0x0111U, 1U, 0UL, &c.ops, &c);
    if (c.opens != 1 || c.menus != 1 || c.posts != 1 || c.last_command != 0x0fa1U)
        return 1;
    state.global_high_flags = 0; stars_title_wnd_proc(&state, 10U, 0x0111U,
        3U, 0UL, &c.ops, &c);
    return c.writes == 1 && c.quits == 1 ? 0 : 1;
}

static int palette_and_shutdown_case(void)
{
    TitleContext c; StarsTitleState state; StarsU32 result;
    setup(&c); memset(&state, 0, sizeof(state)); state.screen_colors = 16;
    state.dib_title = 77; state.palette_splash = 78; state.global_high_flags = 0x40;
    state.exit_value = 7; result = stars_title_wnd_proc(&state, 10U, 0x030fU,
        0U, 0UL, &c.ops, &c);
    if (result != 1UL || c.invalidates != 1) return 1;
    result = stars_title_wnd_proc(&state, 10U, 2U, 0U, 0UL, &c.ops, &c);
    return result == 0x1234UL && state.dib_title == 0U && c.exits == 1 &&
        c.defaults == 1 ? 0 : 1;
}

static int iconic_case(void)
{
    TitleContext c; StarsTitleState state;
    setup(&c); c.iconic = 1; memset(&state, 0, sizeof(state)); state.icon_stars = 9;
    stars_title_wnd_proc(&state, 10U, 0x000fU, 0U, 0UL, &c.ops, &c);
    return c.icons == 1 && c.stars == 0 ? 0 : 1;
}

int main(void)
{
    int failures = create_and_paint_case() + high_color_case() + commands_case() +
        palette_and_shutdown_case() + iconic_case();
    printf("TITLE_CHECKS=5 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures;
}
