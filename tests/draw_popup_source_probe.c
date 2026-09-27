#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 calls;
    StarsI16 kinds[16];
    StarsI16 value1[16];
    StarsI16 value2[16];
    StarsRect slots[2];
    StarsI16 state_calls;
    StarsU32 colors[8];
    StarsU16 modes[4];
    StarsU16 fonts[4];
} Trace;

static StarsI16 get_rect(void *context, StarsU16 hwnd, StarsRect *rect)
{
    (void)hwnd;
    rect->left = 2;
    rect->top = 3;
    rect->right = 100;
    rect->bottom = 40;
    (void)context;
    return 1;
}
static StarsU32 set_color(void *context, StarsU16 hdc, StarsU32 color)
{
    Trace *trace = (Trace *)context;
    (void)hdc;
    if (trace->state_calls < 8)
        trace->colors[trace->state_calls] = color;
    ++trace->state_calls;
    return color + 1;
}
static StarsU16 set_mode(void *context, StarsU16 hdc, StarsU16 mode)
{
    Trace *trace = (Trace *)context;
    (void)hdc;
    if (trace->state_calls < 4)
        trace->modes[trace->state_calls] = mode;
    ++trace->state_calls;
    return (StarsU16)(mode + 1);
}
static StarsU16 select_font(void *context, StarsU16 hdc, StarsU16 font)
{
    Trace *trace = (Trace *)context;
    (void)hdc;
    if (trace->state_calls < 4)
        trace->fonts[trace->state_calls] = font;
    ++trace->state_calls;
    return (StarsU16)(font + 1);
}
static void record(void *context, const StarsDrawPopupCase *popup)
{
    Trace *trace = (Trace *)context;
    StarsI16 index = trace->calls;
    if (index < 16) {
        trace->kinds[index] = popup->popup_kind;
        trace->value1[index] = popup->value1;
        trace->value2[index] = popup->value2;
        if (popup->popup_kind == 11 || popup->popup_kind == 14)
            trace->slots[popup->popup_kind == 11 ? 0 : 1] = popup->slot_rect;
    }
    ++trace->calls;
}
static void install(StarsDrawPopupOps *ops, Trace *trace)
{
    memset(ops, 0, sizeof *ops);
    ops->get_client_rect = get_rect;
    ops->set_background_color = set_color;
    ops->set_text_color = set_color;
    ops->set_background_mode = set_mode;
    ops->select_font = select_font;
    ops->draw_mineral = record;
    ops->draw_fleet_name = record;
    ops->draw_fleet = record;
    ops->draw_coordinates = record;
    ops->draw_planet_state = record;
    ops->draw_zip_order = record;
    ops->draw_planet_population = record;
    ops->draw_factory_mine = record;
    ops->draw_component = record;
    ops->draw_string = record;
    ops->draw_resource = record;
    ops->draw_slot_hull = record;
    ops->draw_default = record;
    ops->context = trace;
}
static void setup(StarsDrawPopupInput *input, StarsDrawPopupOps *ops,
    Trace *trace, StarsI16 kind)
{
    memset(input, 0, sizeof *input);
    install(ops, trace);
    input->hwnd = 10;
    input->hdc = 20;
    input->popup_kind = kind;
    input->font_normal = 3;
    input->font_bold = 4;
    input->arial8_height = 9;
    input->slot_starbase_mode = 1;
    input->ops = ops;
}
int main(void)
{
    StarsDrawPopupInput input;
    StarsDrawPopupOps ops;
    Trace trace;
    StarsI16 kind;

    memset(&trace, 0, sizeof trace);
    setup(&input, &ops, &trace, 1);
    stars_draw_popup(&input);
    if (trace.calls != 1 || trace.kinds[0] != 1 || trace.state_calls != 8 ||
            trace.colors[0] != 0xffffffUL || trace.colors[1] != 0 ||
            trace.colors[6] != 1 || trace.colors[7] != 0x1000000UL)
        return 1;
    for (kind = 2; kind <= 12; ++kind) {
        memset(&trace, 0, sizeof trace);
        setup(&input, &ops, &trace, kind);
        stars_draw_popup(&input);
        if (trace.calls != 1 || trace.kinds[0] != kind)
            return 1;
        if (kind == 6 && (trace.value1[0] != 50 || trace.value2[0] != 1))
            return 1;
        if (kind == 9 && (trace.value1[0] != 100 || trace.value2[0] != 40))
            return 1;
    }
    memset(&trace, 0, sizeof trace);
    setup(&input, &ops, &trace, 11);
    stars_draw_popup(&input);
    if (trace.calls != 1 || trace.slots[0].top != 0x13b ||
            trace.slots[0].left != 8 || trace.value1[0] != -1 ||
            trace.state_calls != 10)
        return 1;
    memset(&trace, 0, sizeof trace);
    setup(&input, &ops, &trace, 14);
    stars_draw_popup(&input);
    if (trace.calls != 1 || trace.slots[1].top != 0x13b ||
            trace.slots[1].left != 8 || trace.state_calls != 10)
        return 1;
    memset(&trace, 0, sizeof trace);
    setup(&input, &ops, &trace, 99);
    stars_draw_popup(&input);
    if (trace.calls != 1 || trace.kinds[0] != 99)
        return 1;
    puts("DRAW_POPUP_CHECKS=51 FAILURES=0 RESULT=PASS");
    return 0;
}
