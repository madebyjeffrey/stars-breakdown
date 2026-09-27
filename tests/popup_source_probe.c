#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsPoint screen;
    StarsI16 measure_calls;
    StarsI16 slot_updates;
    StarsI16 releases;
    StarsI16 created;
    StarsI16 width;
    StarsI16 height;
    StarsI16 metric_calls;
    StarsI16 measure_value1;
    StarsU16 message_window;
    StarsU16 capture_window;
} Trace;
static StarsI16 client_to_screen(void *context, StarsU16 hwnd,
    StarsPoint *point)
{
    (void)hwnd;
    point->x = (StarsI16)(point->x + 10);
    point->y = (StarsI16)(point->y + 20);
    ((Trace *)context)->screen = *point;
    return 1;
}
static StarsU16 get_dc(void *context, StarsU16 hwnd)
{
    (void)context; (void)hwnd; return 7;
}
static StarsU16 select_font(void *context, StarsU16 hdc, StarsU16 font)
{
    (void)context; (void)hdc; return (StarsU16)(font + 1);
}
static StarsPoint measure(void *context, const StarsPopupMeasureCase *popup)
{
    StarsPoint point;
    Trace *trace = (Trace *)context;
    ++trace->measure_calls;
    trace->measure_value1 = popup->value1;
    point.x = (StarsI16)(20 + popup->popup_kind);
    point.y = 30;
    return point;
}
static void update_slots(void *context)
{
    ++((Trace *)context)->slot_updates;
}
static StarsI16 metric(void *context, StarsI16 index)
{
    Trace *trace = (Trace *)context;
    ++trace->metric_calls;
    return index == 0 ? 500 : 500;
}
static StarsU16 create_window(void *context, StarsU16 parent, StarsU32 style,
    StarsI16 x, StarsI16 y, StarsI16 width, StarsI16 height)
{
    Trace *trace = (Trace *)context;
    (void)parent; (void)style;
    trace->created = 1;
    trace->screen.x = x;
    trace->screen.y = y;
    trace->width = width;
    trace->height = height;
    return 99;
}
static void release_dc(void *context, StarsU16 hwnd, StarsU16 hdc)
{
    (void)hwnd; (void)hdc; ++((Trace *)context)->releases;
}
static void send_message(void *context, StarsU16 hwnd, StarsU16 message,
    StarsU16 font)
{
    (void)message; (void)font; ((Trace *)context)->message_window = hwnd;
}
static void set_capture(void *context, StarsU16 hwnd)
{
    ((Trace *)context)->capture_window = hwnd;
}
static void install(StarsPopupOps *ops, Trace *trace)
{
    memset(ops, 0, sizeof *ops);
    ops->client_to_screen = client_to_screen;
    ops->get_dc = get_dc;
    ops->select_font = select_font;
    ops->measure = measure;
    ops->update_slots = update_slots;
    ops->get_system_metric = metric;
    ops->create_window = create_window;
    ops->release_dc = release_dc;
    ops->send_message = send_message;
    ops->set_capture = set_capture;
    ops->context = trace;
}
static void setup(StarsPopupInput *input, StarsPopupOps *ops, Trace *trace,
    StarsI16 kind)
{
    memset(input, 0, sizeof *input);
    install(ops, trace);
    input->hwnd = 1;
    input->x = 90;
    input->y = 90;
    input->popup_kind = kind;
    input->planet_id = 17;
    input->font_normal = 4;
    input->line_height = 8;
    input->line_height_large = 10;
    input->slot_extra_rows = 3;
    input->ops = ops;
}
int main(void)
{
    StarsPopupInput input;
    StarsPopupOps ops;
    Trace trace;

    memset(&trace, 0, sizeof trace);
    setup(&input, &ops, &trace, 1);
    stars_popup(&input);
    if (!trace.created || trace.measure_calls != 1 || trace.releases != 1 ||
            trace.width != 21 || trace.height != 30 || trace.screen.x != 79 ||
            trace.screen.y != 80 || trace.message_window != 99 ||
            trace.capture_window != 99)
        return 1;
    memset(&trace, 0, sizeof trace);
    setup(&input, &ops, &trace, 9);
    stars_popup(&input);
    if (trace.measure_calls != 0 || trace.width != 0x158 ||
            trace.height != 10 + 8 * 12 + 0x4e)
        return 1;
    memset(&trace, 0, sizeof trace);
    setup(&input, &ops, &trace, 8);
    stars_popup(&input);
    if (trace.measure_value1 != 200)
        return 1;
    memset(&trace, 0, sizeof trace);
    setup(&input, &ops, &trace, 10);
    stars_popup(&input);
    if (trace.measure_value1 != 17)
        return 1;
    memset(&trace, 0, sizeof trace);
    setup(&input, &ops, &trace, 12);
    stars_popup(&input);
    if (trace.measure_value1 != 200)
        return 1;
    memset(&trace, 0, sizeof trace);
    setup(&input, &ops, &trace, 11);
    stars_popup(&input);
    if (trace.slot_updates != 1 || trace.width != 0x154 ||
            trace.height != 8 * 10 + 0x13a)
        return 1;
    memset(&trace, 0, sizeof trace);
    setup(&input, &ops, &trace, 13);
    stars_popup(&input);
    if (trace.width != 0x78 || trace.height != 0x50)
        return 1;
    puts("POPUP_CHECKS=31 FAILURES=0 RESULT=PASS");
    return 0;
}
