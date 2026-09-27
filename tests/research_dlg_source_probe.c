#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 paints;
    StarsI16 ends;
    StarsI16 fills;
    StarsI16 colors;
    StarsI16 cursors;
    StarsI16 radios;
    StarsI16 positions;
    StarsI16 strings;
    StarsI16 adds;
    StarsI16 sets;
    StarsI16 stickies;
    StarsI16 dialogs;
    StarsI16 helps;
    StarsI16 tracks;
    StarsI16 commits;
    StarsI16 writes;
    StarsI16 tutors;
    StarsU16 selection;
    StarsU16 written[2];
    StarsI16 total;
    StarsI16 budget;
    StarsI16 last_current;
    StarsI16 last_future;
    StarsI16 last_percent;
    StarsI16 last_x;
    StarsI16 last_y;
    StarsU16 write_address;
    StarsU16 write_length;
    StarsU32 technology_string_mask;
    StarsU32 future_string_mask;
    StarsI16 title_seen;
    StarsI16 title_length;
    StarsI16 technology_lengths[6];
    StarsI16 radio_items;
    StarsI16 combo_items;
    StarsI16 ok_items;
    StarsI16 help_items;
} Trace;

static StarsU16 begin_paint(void *context, StarsU16 hwnd, StarsRect *rect)
{
    (void)hwnd; (void)rect; ++((Trace *)context)->paints; return 9;
}
static void end_paint(void *context, StarsU16 hwnd, StarsRect *rect)
{
    (void)hwnd; (void)rect; ++((Trace *)context)->ends;
}
static void get_client(void *context, StarsU16 hwnd, StarsRect *rect)
{
    (void)context; (void)hwnd; rect->left = 0; rect->top = 0;
    rect->right = 360; rect->bottom = 260;
}
static void get_window(void *context, StarsU16 hwnd, StarsRect *rect)
{
    (void)context; (void)hwnd; rect->left = 0; rect->top = 0;
    rect->right = 400; rect->bottom = 300;
}
static void fill(void *context, StarsU16 dc, const StarsRect *rect,
    StarsU16 brush)
{
    (void)dc; (void)rect; (void)brush; ++((Trace *)context)->fills;
}
static void cursor(void *context, StarsU16 cursor_id)
{
    (void)cursor_id; ++((Trace *)context)->cursors;
}
static void get_cursor(void *context, StarsPoint *point)
{
    (void)context; point->x = 20; point->y = 55;
}
static void screen_to_client(void *context, StarsU16 hwnd, StarsPoint *point)
{
    (void)context; (void)hwnd; (void)point;
}
static void color(void *context, StarsU16 dc, StarsU32 value)
{
    (void)dc; (void)value; ++((Trace *)context)->colors;
}
static void check_radio(void *context, StarsU16 hwnd, StarsU16 first,
    StarsU16 last, StarsU16 checked)
{
    (void)hwnd; (void)first; (void)last; (void)checked;
    ++((Trace *)context)->radios;
}
static StarsU16 get_dc(void *context, StarsU16 hwnd)
{
    (void)context; (void)hwnd; return 3;
}
static StarsU16 select_object(void *context, StarsU16 dc, StarsU16 object)
{
    (void)context; (void)dc; (void)object; return 8;
}
static void release_dc(void *context, StarsU16 hwnd, StarsU16 dc)
{
    (void)context; (void)hwnd; (void)dc;
}
static StarsI16 get_string(void *context, StarsU16 id, StarsU8 *buffer,
    StarsU16 capacity)
{
    Trace *trace = (Trace *)context;
    const char *value;
    StarsI16 length;
    StarsI16 i;

    value = "Research";
    if (id >= 0x0054U && id <= 0x0059U) {
        static const char *const labels[6] = {
            "Energy", "Weapons", "Propulsion", "Construction",
            "Electronics", "Biotechnology"
        };
        value = labels[id - 0x0054U];
        trace->technology_string_mask |=
            (StarsU32)1UL << (id - 0x0054U);
        trace->technology_lengths[id - 0x0054U] =
            (StarsI16)strlen(value);
    } else if (id == 0x01bcU) {
        trace->title_seen = 1;
        trace->title_length = (StarsI16)strlen(value);
    }
    length = (StarsI16)strlen(value);
    if (capacity == 0) return length;
    if ((StarsU16)length >= capacity) length = (StarsI16)(capacity - 1);
    for (i = 0; i < length; ++i) buffer[i] = (StarsU8)value[i];
    buffer[length] = 0;
    return length;
}
static const StarsU8 *compressed(void *context, StarsU16 id)
{
    Trace *trace = (Trace *)context;
    static const char *const values[8] = {
        "<Same field>", "Energy", "Weapons", "Propulsion",
        "Construction", "Electronics", "Biotechnology", "No research"
    };
    if (id >= 0x0053U && id <= 0x005aU)
        trace->future_string_mask |= (StarsU32)1UL << (id - 0x0053U);
    if (id >= 0x0053U && id <= 0x005aU)
        return (const StarsU8 *)values[id - 0x0053U];
    return (const StarsU8 *)"Research";
}
static StarsI32 extent(void *context, StarsU16 dc, const StarsU8 *text,
    StarsI16 length)
{
    (void)context; (void)dc; (void)text; return length * 2;
}
static StarsU16 item(void *context, StarsU16 hwnd, StarsU16 id)
{
    Trace *trace = (Trace *)context;
    (void)hwnd;
    if (id >= 0x431U && id <= 0x436U) ++trace->radio_items;
    if (id == 0x43bU) ++trace->combo_items;
    if (id == 2U) ++trace->ok_items;
    if (id == 0x76U) ++trace->help_items;
    return (StarsU16)(id + 100);
}
static void set_text(void *context, StarsU16 hwnd, const StarsU8 *text)
{
    (void)hwnd; (void)text; ++((Trace *)context)->sets;
}
static void set_pos(void *context, StarsU16 hwnd, StarsU16 insert_after,
    StarsI16 x, StarsI16 y, StarsI16 width, StarsI16 height, StarsU16 flags)
{
    (void)hwnd; (void)insert_after; (void)x; (void)y; (void)width;
    (void)height; (void)flags; ++((Trace *)context)->positions;
}
static StarsI32 send(void *context, StarsU16 hwnd, StarsU16 message,
    StarsU16 wparam, StarsU32 lparam)
{
    (void)hwnd; (void)wparam; (void)lparam;
    if (message == 0x407) return ((Trace *)context)->selection;
    return 0;
}
static void add_string(void *context, StarsU16 combo, const StarsU8 *text)
{
    (void)combo; (void)text; ++((Trace *)context)->adds;
}
static StarsI16 checked(void *context, StarsU16 hwnd, StarsU16 id)
{
    (void)context; (void)hwnd; (void)id; return 1;
}
static void sticky(void *context, StarsU16 hwnd, StarsPoint *point,
    StarsI16 save)
{
    (void)hwnd; (void)point; (void)save; ++((Trace *)context)->stickies;
}
static void end_dialog(void *context, StarsU16 hwnd, StarsI16 result)
{
    (void)hwnd; (void)result; ++((Trace *)context)->dialogs;
}
static void help(void *context, StarsU16 hwnd, const StarsU8 *file,
    StarsU16 id)
{
    (void)hwnd; (void)file; (void)id; ++((Trace *)context)->helps;
}
static void draw(void *context, StarsU16 hwnd, StarsU16 dc,
    const StarsRect *rect, StarsI16 flags)
{
    (void)context; (void)hwnd; (void)dc; (void)rect; (void)flags;
}
static StarsI16 track(void *context, StarsU16 hwnd, StarsI16 x, StarsI16 y,
    StarsU16 flags)
{
    Trace *trace = (Trace *)context;
    (void)hwnd; (void)flags; trace->last_x = x; trace->last_y = y;
    ++trace->tracks; return 1;
}
static StarsI16 owner(void *context, StarsI16 planet)
{
    (void)context; return (StarsI16)(planet == 1 ? 2 : 1);
}
static StarsU32 resources(void *context, StarsI16 planet, StarsI16 player)
{
    (void)context; (void)player; return (StarsU32)(planet + 10);
}
static StarsI32 budget(void *context, StarsI16 percent)
{
    (void)context; return percent * 2;
}
static void commit(void *context, StarsI16 percent, StarsI16 current,
    StarsI16 future)
{
    Trace *trace = (Trace *)context;
    ++trace->commits; trace->last_percent = percent;
    trace->last_current = current; trace->last_future = future;
}
static void write_mem(void *context, StarsU16 address, StarsU16 length,
    const StarsU8 *bytes, StarsU16 byte_count)
{
    Trace *trace = (Trace *)context;
    trace->write_address = address; trace->write_length = length;
    ++trace->writes;
    if (byte_count >= 2) { trace->written[0] = bytes[0]; trace->written[1] = bytes[1]; }
}
static void tutor_mark(void *context, StarsU16 bits)
{
    (void)bits; ++((Trace *)context)->tutors;
}
static void advance(void *context)
{
    ++((Trace *)context)->tutors;
}

int main(void)
{
    StarsResearchDlgOps ops;
    StarsResearchDlgState state;
    StarsResearchDlgInput input;
    Trace trace;

    memset(&ops, 0, sizeof ops);
    memset(&state, 0, sizeof state);
    memset(&trace, 0, sizeof trace);
    ops.begin_paint = begin_paint; ops.end_paint = end_paint;
    ops.get_client_rect = get_client; ops.get_window_rect = get_window;
    ops.fill_rect = fill; ops.get_cursor_pos = get_cursor;
    ops.screen_to_client = screen_to_client; ops.set_cursor = cursor;
    ops.set_background_color = color; ops.check_radio_button = check_radio;
    ops.get_dc = get_dc; ops.select_object = select_object;
    ops.release_dc = release_dc; ops.get_string = get_string;
    ops.get_compressed_string = compressed; ops.get_text_extent = extent;
    ops.get_dialog_item = item; ops.set_window_text = set_text;
    ops.set_window_pos = set_pos; ops.send_message = send;
    ops.combo_add_string = add_string; ops.button_checked = checked;
    ops.sticky_position = sticky; ops.end_dialog = end_dialog;
    ops.win_help = help; ops.draw_dialog = draw; ops.track_research = track;
    ops.planet_owner = owner; ops.resources_at_planet = resources;
    ops.projected_budget = budget; ops.commit = commit;
    ops.write_mem = write_mem; ops.tutor_mark = tutor_mark;
    ops.advance_tutor = advance;
    state.pct_research = 20; state.current_technology = 2;
    state.future_technology = 6; state.arial8_height = 10;
    state.player_id = 1; state.planet_count = 3; state.tutor_active = 1;
    state.future_technology_count = 2;
    state.font_arial8 = 4; state.button_face_brush = 5;
    state.button_face_color = 0x11223344UL; state.help_cursor = 6;
    state.y_future_technology = 50; state.y_technology_note = -1;
    input.state = &state; input.ops = &ops; input.context = &trace;

    if (stars_research_dlg(&input, 10, 0x110, 0, 0) != 1 ||
            trace.radios != 1 || trace.adds != 8 || trace.positions != 9 ||
            state.resource_total != 22 || state.research_budget != 40)
        return 1;
    if (stars_research_dlg(&input, 10, 15, 0, 0) != 1 ||
            trace.paints != 1 || trace.ends != 1)
        return 2;
    if (stars_research_dlg(&input, 10, 20, 4, 0) != 1 || trace.fills != 1)
        return 3;
    if (stars_research_dlg(&input, 10, 25, 4, 0x00060000UL) != 5 ||
            trace.colors != 1)
        return 4;
    if (stars_research_dlg(&input, 10, 32, 0, 0) != 1 || trace.cursors != 1)
        return 5;
    if (stars_research_dlg(&input, 10, 273, 0x433, 0) != 0 ||
            state.current_technology != 2)
        return 6;
    if (stars_research_dlg(&input, 10, 513, 0, 0x00070008UL) != 1 ||
            trace.tracks != 1 || trace.last_x != 8 || trace.last_y != 7)
        return 7;
    trace.selection = 2;
    if (stars_research_dlg(&input, 10, 273, 2, 0) != 1 ||
            trace.commits != 1 || trace.writes != 1 || trace.written[0] != 20 ||
            trace.written[1] != 0x12 || trace.last_future != 1 ||
            trace.write_address != 0x22 || trace.write_length != 2 ||
            trace.dialogs != 1 || state.pct_research != -1)
        return 8;
    state.pct_research = 0;
    state.current_technology = 5;
    trace.selection = 0;
    if (stars_research_dlg(&input, 10, 273, 2, 0) != 1 ||
            trace.commits != 2 || trace.writes != 2 ||
            trace.written[0] != 0 || trace.written[1] != 0x65 ||
            trace.last_current != 5 || trace.last_future != 6 ||
            trace.write_address != 0x22 || trace.write_length != 2 ||
            trace.dialogs != 2 || state.pct_research != -1)
        return 9;
    if (stars_research_dlg(&input, 10, 273, 0x76, 0) != 1 || trace.helps != 1)
        return 10;
    if (trace.technology_string_mask != 0x3fUL ||
            trace.future_string_mask != 0xffUL || !trace.title_seen ||
            trace.title_length != 8 ||
            trace.technology_lengths[0] != 6 ||
            trace.technology_lengths[1] != 7 ||
            trace.technology_lengths[2] != 10 ||
            trace.technology_lengths[3] != 12 ||
            trace.technology_lengths[4] != 11 ||
            trace.technology_lengths[5] != 13 ||
            trace.radio_items < 12 || trace.combo_items < 1 ||
            trace.ok_items < 1 || trace.help_items < 1)
        return 11;
#ifdef TARGET_NATIVE
    {
        FILE *report = fopen("RSDR.TXT", "w");
        if (!report)
            return 11;
        fprintf(report,
            "RESEARCH_DLG_CHECKS=11 FAILURES=0 RESULT=PASS\n");
        fclose(report);
    }
#else
    puts("RESEARCH_DLG_SOURCE=PASS");
#endif
    return 0;
}
