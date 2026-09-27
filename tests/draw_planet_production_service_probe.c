#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct ServiceTrace {
    StarsI16 client_rects;
    StarsI16 positioned;
    StarsI16 shown;
    StarsI16 texts;
    StarsI16 right_texts;
    StarsI16 enables;
    StarsI16 buttons;
    StarsI16 position_flags;
    StarsI16 position_width;
    StarsI16 position_height;
    StarsI16 list_position_flags;
    StarsI16 list_position_width;
    StarsI16 list_position_height;
    StarsI16 button_kind;
    StarsI16 button_enabled;
    StarsI16 configured;
    StarsI16 target_height;
    StarsI16 string_calls;
    StarsU16 strings[4];
} ServiceTrace;

static const StarsU8 *compressed(void *context, StarsU16 id)
{
    ServiceTrace *trace = (ServiceTrace *)context;
    const char *text;
    if (trace->string_calls < 4)
        trace->strings[trace->string_calls] = id;
    ++trace->string_calls;
    if (id == 0x32eU)
        text = "Production";
    else if (id == 0x218U)
        text = "Completion: ";
    else if (id == 0x4e9U)
        text = "Route to";
    else
        text = "";
    return (const StarsU8 *)text;
}

static StarsI16 draw_tile(void *context, StarsU16 hdc,
    StarsPlanetProductionTile *tile, const StarsU8 *title, StarsRect *rect)
{
    (void)context;
    (void)hdc;
    (void)tile;
    (void)title;
    rect->left = 4;
    rect->top = 4;
    rect->right = 180;
    rect->bottom = 140;
    return 1;
}

static void get_client_rect(void *context, StarsU16 window, StarsRect *rect)
{
    ServiceTrace *trace = (ServiceTrace *)context;
    (void)window;
    ++trace->client_rects;
    rect->left = 0;
    rect->top = 0;
    rect->right = 168;
    rect->bottom = trace->configured ? trace->target_height : 58;
}

static void show_window(void *context, StarsU16 window, StarsU16 command)
{
    ServiceTrace *trace = (ServiceTrace *)context;
    (void)window;
    (void)command;
    ++trace->shown;
}

static void enable_window(void *context, StarsU16 window, StarsU16 enabled)
{
    ServiceTrace *trace = (ServiceTrace *)context;
    (void)window;
    ++trace->enables;
    trace->button_enabled = (StarsI16)enabled;
}

static void set_window_pos(void *context, StarsU16 window,
    StarsU16 insert_after, StarsI16 x, StarsI16 y, StarsI16 width,
    StarsI16 height, StarsU16 flags)
{
    ServiceTrace *trace = (ServiceTrace *)context;
    (void)window;
    (void)insert_after;
    (void)x;
    (void)y;
    ++trace->positioned;
    trace->position_width = width;
    trace->position_height = height;
    trace->position_flags = (StarsI16)flags;
    if (window == 9U) {
        trace->list_position_flags = (StarsI16)flags;
        trace->list_position_width = width;
        trace->list_position_height = height;
    }
    trace->target_height = height;
    trace->configured = 1;
}

static StarsI16 selection(void *context, StarsU16 window)
{
    (void)context;
    (void)window;
    return 0;
}

static void select_font(void *context, StarsU16 hdc, StarsU16 font)
{
    (void)context;
    (void)hdc;
    (void)font;
}

static StarsI16 text_extent(void *context, StarsU16 hdc,
    const StarsU8 *text, StarsI16 length)
{
    (void)context;
    (void)hdc;
    (void)text;
    return length;
}

static void text_out(void *context, StarsU16 hdc, StarsI16 x, StarsI16 y,
    const StarsU8 *text, StarsI16 length)
{
    ServiceTrace *trace = (ServiceTrace *)context;
    (void)hdc;
    (void)x;
    (void)y;
    (void)text;
    (void)length;
    ++trace->texts;
}

static void right_text(void *context, StarsU16 hdc, StarsI16 x, StarsI16 y,
    const StarsU8 *text, StarsI16 length, StarsI16 offset)
{
    ServiceTrace *trace = (ServiceTrace *)context;
    (void)hdc;
    (void)x;
    (void)y;
    (void)text;
    (void)length;
    (void)offset;
    ++trace->right_texts;
}

static void draw_button(void *context, StarsU16 hdc, const StarsRect *rect,
    StarsI16 kind, StarsI16 enabled)
{
    ServiceTrace *trace = (ServiceTrace *)context;
    (void)hdc;
    (void)rect;
    ++trace->buttons;
    trace->button_kind = kind;
    trace->button_enabled = enabled;
}

static void install_ops(StarsPlanetProductionOps *ops, ServiceTrace *trace)
{
    memset(ops, 0, sizeof *ops);
    ops->compressed_string = compressed;
    ops->draw_tile = draw_tile;
    ops->get_client_rect = get_client_rect;
    ops->show_window = show_window;
    ops->enable_window = enable_window;
    ops->set_window_pos = set_window_pos;
    ops->selection = selection;
    ops->select_font = select_font;
    ops->text_extent = text_extent;
    ops->text_out = text_out;
    ops->right_text = right_text;
    ops->draw_button = draw_button;
    ops->context = trace;
}

int main(void)
{
    StarsPlanetProductionTile tile;
    StarsPlanetProductionData data;
    StarsPlanetProductionOps ops;
    StarsPlanetProductionInput input;
    StarsRect reference;
    ServiceTrace trace;
    int checks;
    int failures;

    memset(&tile, 0, sizeof tile);
    memset(&data, 0, sizeof data);
    memset(&input, 0, sizeof input);
    memset(&reference, 0, sizeof reference);
    memset(&trace, 0, sizeof trace);
    strcpy((char *)data.eta, "ETA 4");
    strcpy((char *)data.planet_name, "New Terra");
    data.queue_count = 2;
    trace.target_height = 58;
    install_ops(&ops, &trace);
    input.hdc = 3;
    input.line_height = 10;
    input.normal_font = 1;
    input.bold_font = 2;
    input.list_window = 9;
    input.buttons[0] = 10;
    input.buttons[1] = 11;
    input.reference_rect = &reference;
    input.tile = &tile;
    input.data = &data;
    input.ops = &ops;
    checks = 0;
    failures = 0;
#define CHECK(condition) do { ++checks; if (!(condition)) ++failures; } while (0)
    stars_draw_planet_production(&input);
    CHECK(trace.client_rects == 2);
    CHECK(trace.positioned == 3 && trace.list_position_width == 168);
    CHECK(trace.list_position_height == 60 && trace.list_position_flags == 0x15);
    CHECK(trace.shown == 3 && trace.texts == 2 && trace.right_texts == 2);
    CHECK(trace.strings[0] == 0x32eU && trace.strings[1] == 0x218U);
    CHECK(trace.strings[2] == 0x4e9U);
    CHECK(trace.enables == 1 && trace.button_enabled == 1);
    CHECK(trace.buttons == 1 && trace.button_kind == 8);
    CHECK(reference.left == 116 && reference.top == 92 &&
        reference.right == 166 && reference.bottom == 107);
    CHECK((tile.flags & 0x0800U) == 0U);

    memset(&trace, 0, sizeof trace);
    install_ops(&ops, &trace);
    trace.target_height = 36;
    data.eta[0] = 0;
    data.queue_count = 0;
    input.compact_rows = 1;
    input.ops = &ops;
    stars_draw_planet_production(&input);
    CHECK(trace.list_position_height == 36 && trace.list_position_flags == 0x14);
    CHECK(trace.right_texts == 1 && trace.button_enabled == 0);
    CHECK(trace.buttons == 1 && reference.bottom == 83);

    CHECK(checks == 14);
#ifndef TARGET_NATIVE
    printf("DRAW_PLANET_PRODUCTION_SERVICE_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures == 0 ? "PASS" : "FAIL");
#endif
    return failures ? 1 : 0;
#undef CHECK
}
