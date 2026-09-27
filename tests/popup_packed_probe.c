#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 measure_calls;
    StarsI16 slot_updates;
    StarsI16 measure_value1;
    StarsI16 created;
    StarsI16 width;
    StarsI16 height;
    StarsI16 screen_x;
    StarsI16 screen_y;
    StarsU16 message_window;
    StarsU16 capture_window;
} Trace;

static void put16(unsigned char *raw, int offset, unsigned short value)
{
    raw[offset] = (unsigned char)(value & 0xff);
    raw[offset + 1] = (unsigned char)(value >> 8);
}

static unsigned short get16(const unsigned char *raw, int offset)
{
    return (unsigned short)(raw[offset] |
        ((unsigned short)raw[offset + 1] << 8));
}

static StarsI16 client_to_screen(void *context, StarsU16 hwnd,
    StarsPoint *point)
{
    (void)context;
    (void)hwnd;
    point->x = (StarsI16)(point->x + 5);
    point->y = (StarsI16)(point->y + 7);
    return 1;
}

static StarsU16 get_dc(void *context, StarsU16 hwnd)
{
    (void)context;
    (void)hwnd;
    return 7;
}

static StarsU16 select_font(void *context, StarsU16 hdc, StarsU16 font)
{
    (void)context;
    (void)hdc;
    return (StarsU16)(font + 1);
}

static StarsPoint measure(void *context, const StarsPopupMeasureCase *popup)
{
    Trace *trace = (Trace *)context;
    StarsPoint size;
    ++trace->measure_calls;
    trace->measure_value1 = popup->value1;
    size.x = (StarsI16)(30 + popup->popup_kind);
    size.y = (StarsI16)(40 + popup->value1);
    return size;
}

static void update_slots(void *context)
{
    ++((Trace *)context)->slot_updates;
}

static StarsI16 metric(void *context, StarsI16 index)
{
    (void)context;
    (void)index;
    return 100;
}

static StarsU16 create_window(void *context, StarsU16 parent, StarsU32 style,
    StarsI16 x, StarsI16 y, StarsI16 width, StarsI16 height)
{
    Trace *trace = (Trace *)context;
    (void)parent;
    (void)style;
    trace->created = 1;
    trace->screen_x = x;
    trace->screen_y = y;
    trace->width = width;
    trace->height = height;
    return 99;
}

static void release_dc(void *context, StarsU16 hwnd, StarsU16 hdc)
{
    (void)context;
    (void)hwnd;
    (void)hdc;
}

static void send_message(void *context, StarsU16 hwnd, StarsU16 message,
    StarsU16 font)
{
    Trace *trace = (Trace *)context;
    (void)message;
    (void)font;
    trace->message_window = hwnd;
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

static void setup(StarsPopupInput *input, StarsPopupOps *ops, Trace *trace)
{
    memset(input, 0, sizeof *input);
    memset(trace, 0, sizeof *trace);
    install(ops, trace);
    input->hwnd = 1;
    input->x = 90;
    input->y = 90;
    input->font_normal = 4;
    input->line_height = 8;
    input->line_height_large = 10;
    input->ops = ops;
}

int main(void)
{
    unsigned char raw[22];
    StarsPopupInput input;
    StarsPopupOps ops;
    Trace trace;
    int checks;
    int failures;
    int first_failure;

    memset(raw, 0, sizeof raw);
    put16(raw, 0, 10);
    put16(raw, 2, 37);
    put16(raw, 4, 12);
    put16(raw, 6, 3);
    put16(raw, 8, 21);
    put16(raw, 10, 44);
    put16(raw, 12, 7);
    put16(raw, 14, 2);
    put16(raw, 16, 9);
    put16(raw, 18, 6);
    put16(raw, 20, 5);
    checks = 0;
    failures = 0;
    first_failure = 0;
#define CHECK(condition) do { ++checks; if (!(condition)) { ++failures; if (first_failure == 0) first_failure = checks; } } while (0)
    CHECK(sizeof raw == 22);
    CHECK(get16(raw, 0) == 10);
    CHECK(get16(raw, 2) == 37);
    CHECK(get16(raw, 4) == 12);
    CHECK(get16(raw, 6) == 3);
    CHECK(get16(raw, 8) == 21);
    CHECK(get16(raw, 10) == 44);
    CHECK(get16(raw, 12) == 7);
    CHECK(get16(raw, 14) == 2);
    CHECK(get16(raw, 16) == 9);
    CHECK(get16(raw, 18) == 6);
    CHECK(get16(raw, 20) == 5);

    setup(&input, &ops, &trace);
    input.popup_kind = (StarsI16)get16(raw, 0);
    input.planet_id = (StarsI16)get16(raw, 2);
    stars_popup(&input);
    CHECK(trace.measure_calls == 1);
    CHECK(trace.measure_value1 == 37);
    CHECK(trace.created && trace.width == 42 && trace.height == 79);
    CHECK(trace.message_window == 99 && trace.capture_window == 99);

    setup(&input, &ops, &trace);
    input.popup_kind = 11;
    input.slot_extra_rows = (StarsI16)get16(raw, 6);
    stars_popup(&input);
    CHECK(trace.slot_updates == 1);
    CHECK(trace.width == 0x154);
    CHECK(trace.height == 8 * 7 + 0x13a + 3 * 8);

    setup(&input, &ops, &trace);
    input.popup_kind = 9;
    stars_popup(&input);
    CHECK(trace.width == 0x158 && trace.height == 10 + 8 * 12 + 0x4e);
    CHECK(trace.created && trace.screen_x == 0 && trace.screen_y == 0);
    CHECK(checks == 22);
    puts("POPUP_PACKED_CHECKS=22 FAILURES=0 RESULT=PASS");
    return failures ? first_failure : 0;
#undef CHECK
}
