#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 paint;
    StarsI16 erase;
    StarsI16 title;
    StarsI16 cursor;
    StarsI16 popup;
    StarsI16 create;
    StarsI16 open;
    StarsI16 edit;
    StarsI16 help;
    StarsI16 alert;
    StarsI16 sticky;
    StarsI16 end_dialog;
    StarsI16 invalidate;
    StarsI16 focus;
    StarsI16 selection;
    StarsI16 action_result;
    StarsI16 result_kind;
    StarsI16 dialog_item_calls;
    StarsI16 background_calls;
    StarsU32 last_background;
} Trace;
static void paint(void *c, StarsU16 hwnd, StarsU16 dc)
{
    (void)hwnd; (void)dc; ++((Trace *)c)->paint;
}
static void erase(void *c, StarsU16 hwnd, StarsU16 dc)
{
    (void)hwnd; (void)dc; ++((Trace *)c)->erase;
}
static void title(void *c, StarsU16 hwnd)
{
    (void)hwnd; ++((Trace *)c)->title;
}
static void cursor(void *c, StarsU16 hwnd)
{
    (void)hwnd; ++((Trace *)c)->cursor;
}
static StarsI16 popup(void *c, StarsU16 hwnd, StarsU8 type,
    StarsI16 message, StarsI16 x, StarsI16 y)
{
    Trace *t = (Trace *)c;
    (void)hwnd; (void)type; (void)message; (void)x; (void)y;
    ++t->popup;
    return t->selection;
}
static StarsI16 create_race(void *c, StarsU16 hwnd, StarsI16 row,
    StarsU8 type)
{
    Trace *t = (Trace *)c;
    (void)hwnd; (void)row; (void)type; ++t->create;
    return t->action_result;
}
static StarsI16 open_game(void *c, StarsU16 hwnd, StarsI16 row,
    StarsU8 type)
{
    Trace *t = (Trace *)c;
    (void)hwnd; (void)row; (void)type; ++t->open;
    return t->action_result;
}
static StarsI16 edit_race(void *c, StarsU16 hwnd, StarsI16 row,
    StarsU8 type)
{
    Trace *t = (Trace *)c;
    (void)hwnd; (void)row; (void)type; ++t->edit;
    return t->result_kind;
}
static void help(void *c, StarsU16 hwnd)
{
    (void)hwnd; ++((Trace *)c)->help;
}
static void alert(void *c, StarsI16 string_id, StarsI16 level)
{
    Trace *t = (Trace *)c;
    (void)string_id; (void)level; ++t->alert;
}
static void sticky(void *c, StarsU16 hwnd, StarsI16 save)
{
    (void)hwnd; (void)save; ++((Trace *)c)->sticky;
}
static void end_dialog(void *c, StarsU16 hwnd, StarsI16 result)
{
    Trace *t = (Trace *)c;
    (void)hwnd; (void)result; ++t->end_dialog;
}
static void invalidate(void *c, StarsU16 hwnd)
{
    (void)hwnd; ++((Trace *)c)->invalidate;
}
static void focus(void *c, StarsU16 hwnd)
{
    (void)hwnd; ++((Trace *)c)->focus;
}
static StarsU16 dialog_item(void *c, StarsU16 hwnd, StarsU16 id)
{
    Trace *t = (Trace *)c;
    (void)hwnd;
    ++t->dialog_item_calls;
    return id == 0x193 ? 0x55 : 0;
}
static void background(void *c, StarsU16 dc, StarsU32 color)
{
    Trace *t = (Trace *)c;
    (void)dc;
    ++t->background_calls;
    t->last_background = color;
}
static void install(StarsNewGameDlg2Ops *ops, Trace *trace)
{
    memset(ops, 0, sizeof *ops);
    ops->paint = paint;
    ops->erase_background = erase;
    ops->set_title_and_layout = title;
    ops->set_cursor = cursor;
    ops->track_popup = popup;
    ops->create_race = create_race;
    ops->open_game = open_game;
    ops->edit_race = edit_race;
    ops->help = help;
    ops->alert = alert;
    ops->sticky_position = sticky;
    ops->end_dialog = end_dialog;
    ops->invalidate = invalidate;
    ops->set_focus = focus;
    ops->get_dialog_item = dialog_item;
    ops->set_background_color = background;
    ops->context = trace;
}
static StarsI32 point(StarsI16 x, StarsI16 y)
{
    return (StarsI32)(((StarsU32)(StarsU16)y << 16) |
        (StarsU32)(StarsU16)x);
}
static void setup(StarsNewGameDlg2Input *input, StarsNewGameDlg2Ops *ops,
    StarsU8 *types, Trace *trace)
{
    static const StarsU16 buttons[4] = { 100, 101, 102, 103 };
    memset(input, 0, sizeof *input);
    install(ops, trace);
    input->x_diamond = 10;
    input->diamond_width = 5;
    input->row_top = 6;
    input->row_height = 10;
    input->row_height_active = 9;
    input->player_types = types;
    input->player_type_count = 4;
    input->race_button_ids = buttons;
    input->race_button_count = 4;
    input->button_face_brush = 0x77;
    input->button_face_color = 0x12345678UL;
    input->ops = ops;
}
int main(void)
{
    StarsNewGameDlg2Input input;
    StarsNewGameDlg2Ops ops;
    StarsU8 types[4];
    Trace trace;

    memset(types, 0, sizeof types);
    memset(&trace, 0, sizeof trace);
    types[0] = 1; types[1] = 1; types[2] = 1;
    setup(&input, &ops, types, &trace);
    trace.selection = 0x12;
    if (stars_new_game_dlg2(7, 0x0f, 33, 0, &input) != 1 ||
            stars_new_game_dlg2(7, 0x14, 33, 0, &input) != 1 ||
            stars_new_game_dlg2(7, 0x110, 0, 0, &input) != 1 ||
            stars_new_game_dlg2(7, 0x20, 0, point(12, 7), &input) != 1 ||
            trace.paint != 1 || trace.erase != 1 || trace.title != 1 ||
            trace.cursor != 1 ||
            stars_new_game_dlg2(7, 0x201, 0, point(12, 7), &input) != 0 ||
            types[0] != 9 || trace.popup != 1 || trace.invalidate != 1 ||
            trace.focus != 1)
        return 1;

    trace.selection = 0;
    trace.action_result = 3;
    if (stars_new_game_dlg2(7, 0x204, 0, point(12, 17), &input) != 0 ||
            types[1] != 14 || trace.create != 1)
        return 1;
    trace.selection = 1;
    trace.action_result = 2;
    if (stars_new_game_dlg2(7, 0x201, 0, point(12, 27), &input) != 0 ||
            types[2] != 10 || trace.open != 1)
        return 1;
    trace.selection = 3;
    if (stars_new_game_dlg2(7, 0x201, 0, point(12, 17), &input) != 0 ||
            types[1] != 0 || types[2] != 0 || types[3] != 0)
        return 1;
    trace.selection = 0x25;
    if (stars_new_game_dlg2(7, 0x201, 0, point(12, 27), &input) != 0 ||
            types[2] != 22 || types[1] != 22)
        return 1;
    trace.selection = 2;
    trace.result_kind = 7;
    if (stars_new_game_dlg2(7, 0x201, 0, point(12, 27), &input) != 0 ||
            types[2] != 7 || trace.edit != 1)
        return 1;

    if (stars_new_game_dlg2(7, 0x111, 0x76, 0, &input) != 1 ||
            trace.help != 1)
        return 1;
    if (stars_new_game_dlg2(7, 0x19, 0x22, 0x55, &input) != 0x77 ||
            trace.background_calls != 1 ||
            trace.last_background != 0x12345678UL)
        return 1;
    if (stars_new_game_dlg2(7, 0x19, 0x22, 0x66, &input) != 0)
        return 1;
    if (stars_new_game_dlg2(7, 0x111, 102, 0, &input) != 1 ||
            trace.end_dialog != 1 || trace.sticky < 1)
        return 1;
    memset(types, 0, sizeof types);
    trace.selection = 0x10;
    if (stars_new_game_dlg2(7, 0x111, 101, 0, &input) != 0 ||
            trace.alert != 1)
        return 1;
    input.read_only = 1;
    if (stars_new_game_dlg2(7, 0x201, 0, point(12, 7), &input) != 0 ||
            trace.popup != 6)
        return 1;
#ifdef TARGET_NATIVE
    {
        FILE *report;
        report = fopen("C:\\ND2\\NWD2R.TXT", "w");
        if (report) {
            fprintf(report,
                "NEW_GAME_DLG2_CHECKS=29 FAILURES=0 RESULT=PASS\n");
            fclose(report);
        }
    }
#endif
    puts("NEW_GAME_DLG2_CHECKS=29 FAILURES=0 RESULT=PASS");
    return 0;
}
