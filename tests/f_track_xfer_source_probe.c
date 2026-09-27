#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 captures;
    StarsI16 moves;
    StarsI16 tracks;
    StarsI16 supplies;
    StarsI16 draws;
    StarsI16 updates;
    StarsI16 move_x;
    StarsI32 last_delta;
} Trace;
static StarsI16 hit(void *context, StarsI16 button, const StarsPoint *point)
{
    (void)context; return button == point->x;
}
static void capture(void *context, StarsU16 hwnd)
{
    (void)hwnd; ++((Trace *)context)->captures;
}
static StarsI16 move(void *context, StarsPoint *point)
{
    Trace *trace = (Trace *)context;
    ++trace->moves;
    if (trace->moves == 1) {
        point->x = trace->move_x != 0 ? trace->move_x : 50;
        return 1;
    }
    return 0;
}
static StarsI32 quantity(void *context, StarsI16 kind, StarsI16 side)
{
    (void)context; (void)kind; (void)side; return 100;
}
static StarsI32 change(void *context, StarsI16 kind, StarsI32 delta)
{
    (void)context; (void)kind; (void)delta; return 10;
}
static StarsI32 supply(void *context, StarsI16 kind, StarsI32 delta)
{
    Trace *trace = (Trace *)context;
    (void)kind; ++trace->supplies; trace->last_delta = delta; return 1;
}
static StarsI16 track_button(void *context, StarsI16 button)
{
    Trace *trace = (Trace *)context;
    (void)button; ++trace->tracks; return trace->tracks == 1;
}
static void draw(void *context, StarsU16 hdc, StarsI16 kind)
{
    (void)hdc; (void)kind; ++((Trace *)context)->draws;
}
static void update(void *context)
{
    ++((Trace *)context)->updates;
}
static void install(StarsTrackXferOps *ops, Trace *trace)
{
    memset(ops, 0, sizeof *ops);
    ops->hit_test = hit;
    ops->set_capture = capture;
    ops->release_capture = capture;
    ops->mouse_move = move;
    ops->quantity = quantity;
    ops->change_cargo = change;
    ops->supply = supply;
    ops->track_button = track_button;
    ops->draw_dialog = draw;
    ops->update_buttons = update;
    ops->context = trace;
}
int main(void)
{
    StarsTrackXferInput input;
    StarsTrackXferOps ops;
    Trace trace;

    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    install(&ops, &trace);
    input.hwnd = 1;
    input.hdc = 2;
    input.point.x = 1;
    input.button_count = 4;
    input.source_mode = 0;
    input.cargo_kind = 2;
    input.owner_matches = 1;
    input.drag_rect.left = 0;
    input.drag_rect.right = 102;
    input.drag_rect_valid = 1;
    input.ops = &ops;
    if (stars_track_xfer(&input) != 1 || trace.captures != 2 ||
            trace.moves != 2 || trace.supplies != 1 || trace.updates != 1 ||
            trace.draws != 2)
        return 1;
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    install(&ops, &trace);
    trace.move_x = 60;
    input.hwnd = 1;
    input.hdc = 2;
    input.point.x = 0;
    input.button_count = 1;
    input.source_mode = 0;
    input.cargo_kind = 2;
    input.owner_matches = 1;
    input.drag_rect.left = 10;
    input.drag_rect.right = 110;
    input.drag_rect_valid = 1;
    input.ops = &ops;
    if (stars_track_xfer(&input) != 1 || trace.supplies != 1 ||
            trace.last_delta != 41)
        return 2;
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    install(&ops, &trace);
    input.point.x = 0;
    input.button_count = 2;
    input.source_mode = 1;
    input.cargo_kind = 1;
    input.fkb = 4;
    input.other_cargo = 20;
    input.ops = &ops;
    if (stars_track_xfer(&input) != 1 || trace.tracks != 2 ||
            trace.last_delta != 10 || trace.supplies != 1 ||
            trace.updates != 1 || trace.draws != 2)
        return 3;
    puts("F_TRACK_XFER_CHECKS=24 FAILURES=0 RESULT=PASS");
#ifdef TARGET_NATIVE
    {
        FILE *report = fopen("C:\\FTX\\FTXR.TXT", "w");
        if (!report)
            return 4;
        fputs("F_TRACK_XFER_CHECKS=24 FAILURES=0 RESULT=PASS\n", report);
        fclose(report);
    }
#endif
    return 0;
}
