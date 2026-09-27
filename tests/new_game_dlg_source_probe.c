#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 begins;
    StarsI16 ends;
    StarsI16 frames;
    StarsI16 texts;
    StarsI16 fills;
    StarsI16 colors;
    StarsI16 titles;
    StarsI16 radios;
    StarsI16 window_texts;
    StarsI16 sends;
    StarsI16 disables;
    StarsI16 stickies;
    StarsI16 dialogs;
    StarsI16 helps;
    StarsI16 defaults;
    StarsI16 no_checked;
    StarsI16 last_result;
} Trace;

static StarsU16 begin_paint(void *context, StarsU16 hwnd, StarsRect *rect)
{
    (void)hwnd; (void)rect; ++((Trace *)context)->begins; return 4;
}
static void end_paint(void *context, StarsU16 hwnd, StarsRect *rect)
{
    (void)hwnd; (void)rect; ++((Trace *)context)->ends;
}
static void get_rect(void *context, StarsU16 hwnd, StarsRect *rect)
{
    (void)hwnd; (void)context;
    rect->left = 10; rect->top = 10; rect->right = 100; rect->bottom = 40;
}
static void map_points(void *context, StarsU16 from, StarsU16 to,
    StarsRect *rect, StarsU16 count)
{
    (void)context; (void)from; (void)to; (void)rect; (void)count;
}
static void expand_rect(void *context, StarsRect *rect, StarsI16 x,
    StarsI16 y)
{
    (void)context; rect->left = (StarsU16)(rect->left - x);
    rect->top = (StarsU16)(rect->top - y);
    rect->right = (StarsU16)(rect->right + x);
    rect->bottom = (StarsU16)(rect->bottom + y);
}
static void draw_frame(void *context, StarsU16 dc, const StarsRect *rect,
    StarsI16 raised)
{
    (void)dc; (void)rect; (void)raised; ++((Trace *)context)->frames;
}
static void select_object(void *context, StarsU16 dc, StarsU16 object)
{
    (void)context; (void)dc; (void)object;
}
static void set_color(void *context, StarsU16 dc, StarsU32 color)
{
    (void)dc; (void)color; ++((Trace *)context)->colors;
}
static StarsI16 get_string(void *context, StarsU16 id, StarsU8 *text,
    StarsU16 capacity)
{
    const char *value = id == 0x1c5U ? "Size" :
        (id == 0x1c7U ? "Density" : "Distance");
    StarsI16 i;
    (void)context;
    for (i = 0; i + 1 < (StarsI16)capacity && value[i] != 0; ++i)
        text[i] = (StarsU8)value[i];
    if (capacity != 0U) text[i] = 0;
    return i;
}
static void text_out(void *context, StarsU16 dc, StarsI16 x, StarsI16 y,
    const StarsU8 *text, StarsI16 length)
{
    (void)dc; (void)x; (void)y; (void)text; (void)length;
    ++((Trace *)context)->texts;
}
static void set_title(void *context, StarsU16 hwnd, StarsI16 page)
{
    (void)hwnd; (void)page; ++((Trace *)context)->titles;
}
static void check_radio(void *context, StarsU16 hwnd, StarsU16 first,
    StarsU16 last, StarsU16 selected)
{
    (void)hwnd; (void)first; (void)last; (void)selected;
    ++((Trace *)context)->radios;
}
static StarsU16 get_item(void *context, StarsU16 hwnd, StarsU16 id)
{
    (void)context; (void)hwnd; return (StarsU16)(id + 100);
}
static void set_window_text(void *context, StarsU16 hwnd, const StarsU8 *text)
{
    (void)hwnd; (void)text; ++((Trace *)context)->window_texts;
}
static void send_message(void *context, StarsU16 hwnd, StarsU16 message,
    StarsU16 wparam, StarsU32 lparam)
{
    (void)hwnd; (void)message; (void)wparam; (void)lparam;
    ++((Trace *)context)->sends;
}
static void enable_window(void *context, StarsU16 hwnd, StarsU16 enable)
{
    (void)hwnd; (void)enable; ++((Trace *)context)->disables;
}
static StarsI16 checked(void *context, StarsU16 hwnd, StarsU16 id)
{
    Trace *trace = (Trace *)context;
    (void)hwnd;
    if (trace->no_checked)
        return 0;
    if (id == 1002U || id == 1006U || id == 1010U)
        return 1;
    return id == 0x3f8U || id == 0x3faU || id == 0x3fdU || id == 0x41aU;
}
static void get_text(void *context, StarsU16 hwnd, StarsU16 id,
    StarsU8 *text, StarsU16 capacity)
{
    const char *value = "Game";
    StarsI16 i;
    (void)context; (void)hwnd; (void)id;
    for (i = 0; i + 1 < (StarsI16)capacity && value[i] != 0; ++i)
        text[i] = (StarsU8)value[i];
    text[i] = 0;
}
static StarsI16 race_index(void *context, StarsU16 wparam)
{
    (void)context;
    if (wparam == 100U) return 0;
    if (wparam == 101U) return 1;
    if (wparam == 102U) return 2;
    if (wparam == 103U) return 3;
    return -1;
}
static void default_name(void *context, StarsU8 *text, StarsU16 capacity)
{
    (void)context;
    if (capacity > 1U) { text[0] = (StarsU8)'D'; text[1] = 0; }
}
static void sticky(void *context, StarsU16 hwnd, StarsPoint *point,
    StarsI16 save)
{
    (void)hwnd; (void)point; (void)save; ++((Trace *)context)->stickies;
}
static void end_dialog(void *context, StarsU16 hwnd, StarsI16 result)
{
    (void)hwnd; ++((Trace *)context)->dialogs;
    ((Trace *)context)->last_result = result;
}
static void win_help(void *context, StarsU16 hwnd, const StarsU8 *file,
    StarsU16 id)
{
    (void)hwnd; (void)file; (void)id; ++((Trace *)context)->helps;
}
static void install(StarsNewGameDlgOps *ops)
{
    memset(ops, 0, sizeof *ops);
    ops->begin_paint = begin_paint;
    ops->end_paint = end_paint;
    ops->get_control_rect = get_rect;
    ops->map_points = map_points;
    ops->expand_rect = expand_rect;
    ops->draw_frame = draw_frame;
    ops->select_object = select_object;
    ops->set_background_color = set_color;
    ops->fill_rect = (StarsNewGameDlgFillRect)0;
    ops->get_string = get_string;
    ops->text_out = text_out;
    ops->set_title = set_title;
    ops->check_radio_button = check_radio;
    ops->get_dialog_item = get_item;
    ops->set_window_text = set_window_text;
    ops->send_message = send_message;
    ops->enable_window = enable_window;
    ops->is_button_checked = checked;
    ops->get_dialog_text = get_text;
    ops->race_button_index = race_index;
    ops->default_name = default_name;
    ops->sticky_position = sticky;
    ops->end_dialog = end_dialog;
    ops->win_help = win_help;
}

static void fill(void *context, StarsU16 dc, const StarsRect *rect,
    StarsU16 brush)
{
    (void)dc; (void)rect; (void)brush; ++((Trace *)context)->fills;
}

int main(void)
{
    StarsNewGameDlgState state;
    StarsNewGameDlgOps ops;
    StarsNewGameDlgInput input;
    Trace trace;
    StarsI16 checks;
    StarsI16 failures;

    memset(&state, 0, sizeof state);
    memset(&trace, 0, sizeof trace);
    install(&ops);
    ops.fill_rect = fill;
    state.map_size = 1;
    state.density = 2;
    state.start_distance = 0;
    state.options = 0x161U;
    state.game_name[0] = (StarsU8)'X'; state.game_name[1] = 0;
    state.race_button_ids[0] = 100;
    state.race_button_ids[1] = 101;
    state.race_button_ids[2] = 102;
    state.race_button_ids[3] = 103;
    state.button_face_brush = 3;
    state.button_face_color = 0x1234UL;
    state.arial8_height = 12;
    state.help_id = 0x3f4;
    input.state = &state;
    input.ops = &ops;
    input.context = &trace;
    checks = 0;
    failures = 0;
#define CHECK(condition) do { ++checks; if (!(condition)) ++failures; } while (0)
    CHECK(stars_new_game_dlg(&input, 20, 272, 0, 0) == 1);
    CHECK(trace.titles == 1 && trace.radios == 3 && trace.sends == 8);
    CHECK(trace.stickies == 1);
    CHECK(stars_new_game_dlg(&input, 20, 15, 0, 0) == 1);
    CHECK(trace.begins == 1 && trace.ends == 1 && trace.frames == 3);
    CHECK(trace.texts == 3 && trace.colors == 3);
    CHECK(stars_new_game_dlg(&input, 20, 20, 4, 0) == 1);
    CHECK(trace.fills == 1);
    CHECK(stars_new_game_dlg(&input, 20, 25, 4, 1100) == 3);
    CHECK(trace.colors == 4);
    CHECK(stars_new_game_dlg(&input, 20, 273, 0x76, 0) == 1);
    CHECK(trace.helps == 1);
    CHECK(stars_new_game_dlg(&input, 20, 273, 102, 0) == 1);
    CHECK(state.map_size == 2 && state.density == 1 && state.start_distance == 1);
    CHECK((state.options & 0x1ffU) == 0x161U && trace.last_result == 2);
    CHECK(trace.dialogs == 1 && trace.stickies == 2);
    state.read_only = 1;
    CHECK(stars_new_game_dlg(&input, 20, 272, 0, 0) == 1);
    CHECK(trace.disables == 21);
    CHECK(stars_new_game_dlg(&input, 20, 273, 100, 0) == 1);
    CHECK(trace.last_result == 0);
    state.read_only = 0;
    trace.no_checked = 1;
    CHECK(stars_new_game_dlg(&input, 20, 273, 102, 0) == 1);
    CHECK(state.map_size == 5 && state.density == 4 &&
        state.start_distance == 4);
#undef CHECK
    if (checks != 22)
        failures = 1;
    if (failures == 0) {
        puts("NEW_GAME_DLG_CHECKS=22 FAILURES=0 RESULT=PASS");
#ifdef TARGET_NATIVE
        {
            FILE *report = fopen("C:\\NGD\\NGDR.TXT", "w");
            if (!report)
                return 2;
            fputs("NEW_GAME_DLG_CHECKS=22 FAILURES=0 RESULT=PASS\n", report);
            fclose(report);
        }
#endif
    } else
        puts("NEW_GAME_DLG_CHECKS=22 FAILURES=1 RESULT=FAIL");
    return failures == 0 ? 0 : 1;
}
