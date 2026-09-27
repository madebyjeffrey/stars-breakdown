#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct PackedDrawContext {
    unsigned char popup[22];
    unsigned char fleet[124];
    unsigned char planet[56];
    StarsI16 calls;
    StarsI16 state_calls;
    StarsI16 fleet_seen;
    StarsI16 planet_seen;
    StarsI16 slot_seen;
    StarsI16 default_seen;
    StarsI16 payload_seen;
    StarsI16 last_kind;
    StarsI16 generic_seen[15];
    StarsRect last_slot;
} PackedDrawContext;

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

static StarsI16 get_rect(void *context, StarsU16 hwnd, StarsRect *rect)
{
    (void)context;
    (void)hwnd;
    rect->left = 2;
    rect->top = 3;
    rect->right = 160;
    rect->bottom = 90;
    return 1;
}

static StarsU32 set_color(void *context, StarsU16 hdc, StarsU32 color)
{
    (void)hdc;
    ++((PackedDrawContext *)context)->state_calls;
    return color + 1;
}

static StarsU16 set_mode(void *context, StarsU16 hdc, StarsU16 mode)
{
    (void)hdc;
    ++((PackedDrawContext *)context)->state_calls;
    return (StarsU16)(mode + 1);
}

static StarsU16 select_font(void *context, StarsU16 hdc, StarsU16 font)
{
    (void)hdc;
    ++((PackedDrawContext *)context)->state_calls;
    return (StarsU16)(font + 1);
}

static void record(PackedDrawContext *trace,
    const StarsDrawPopupCase *popup)
{
    ++trace->calls;
    trace->last_kind = popup->popup_kind;
    trace->payload_seen = popup->payload.gr_popup == 3 &&
        popup->payload.raw[21] == 0x13 &&
        popup->payload.rgi[0] == (StarsI32)0x000c0006UL &&
        popup->payload.rgi[4] == (StarsI32)0x13141112UL &&
        popup->payload.id_planet == (StarsI16)6 &&
        popup->payload.planet_var == (StarsI16)12 &&
        popup->payload.planet_value == (StarsI16)3 &&
        popup->payload.planet_minimum == (StarsI16)21 &&
        popup->payload.planet_maximum == (StarsI16)44 &&
        popup->payload.player_value == (StarsI16)0x0b0c &&
        popup->payload.player_minimum == (StarsI16)0x0d0e &&
        popup->payload.player_maximum == (StarsI16)0x0f10 &&
        popup->payload.id_plan == (StarsI16)6 &&
        popup->payload.c_max == (StarsI16)12 &&
        popup->payload.c_current == (StarsI16)3 &&
        popup->payload.c_operate == (StarsI16)21 &&
        popup->payload.factory == (StarsI16)44 &&
        popup->payload.dx_out == (StarsI16)6 &&
        popup->payload.psz_near == (StarsU32)0x0003000cUL &&
        popup->payload.player == (StarsI16)6 &&
        popup->payload.fleet_far == (StarsU32)0x000c0006UL &&
        popup->payload.red_damage == (StarsI16)3 &&
        popup->payload.dx_damage == (StarsI16)21 &&
        popup->payload.grbit == (StarsU16)44 &&
        popup->payload.part[0] == 0x06 &&
        popup->payload.part[7] == 0x00 &&
        popup->payload.shdef_far == (StarsU32)0x000c0006UL &&
        popup->payload.show_damage == (StarsI16)3 &&
        popup->payload.hide_counts == (StarsI16)21 &&
        popup->payload.token == (StarsI16)44 &&
        popup->payload.summary == (StarsI16)0x0b0c &&
        popup->payload.token_index == (StarsI16)0x0d0e;
}

static void draw_fleet(void *context, const StarsDrawPopupCase *popup)
{
    PackedDrawContext *trace = (PackedDrawContext *)context;
    record(trace, popup);
    trace->fleet_seen = get16(trace->popup, 0) == 3 &&
        get16(trace->fleet, 12) == 4 && get16(trace->fleet, 14) == 2 &&
        get16(trace->fleet, 8) == 101 && get16(trace->fleet, 10) == 202;
}

static void draw_planet_state(void *context,
    const StarsDrawPopupCase *popup)
{
    PackedDrawContext *trace = (PackedDrawContext *)context;
    record(trace, popup);
    trace->planet_seen = get16(trace->popup, 0) == 5 &&
        get16(trace->planet, 0) == 0x1234 && popup->value1 == 1;
}

static void draw_slot_hull(void *context, const StarsDrawPopupCase *popup)
{
    PackedDrawContext *trace = (PackedDrawContext *)context;
    record(trace, popup);
    trace->last_slot = popup->slot_rect;
    trace->slot_seen = (get16(trace->popup, 0) == 11 ||
        get16(trace->popup, 0) == 14) &&
        get16(trace->popup, 6) == 3 && popup->value1 == -1 &&
        popup->starbase_mode == 1;
}

static void draw_default(void *context, const StarsDrawPopupCase *popup)
{
    PackedDrawContext *trace = (PackedDrawContext *)context;
    record(trace, popup);
    trace->default_seen = 1;
}

static void draw_generic(void *context, const StarsDrawPopupCase *popup)
{
    PackedDrawContext *trace = (PackedDrawContext *)context;

    record(trace, popup);
    if (popup->popup_kind >= 0 && popup->popup_kind < 15)
        ++trace->generic_seen[popup->popup_kind];
}

static void install(StarsDrawPopupOps *ops, PackedDrawContext *trace)
{
    memset(ops, 0, sizeof *ops);
    ops->get_client_rect = get_rect;
    ops->set_background_color = set_color;
    ops->set_text_color = set_color;
    ops->set_background_mode = set_mode;
    ops->select_font = select_font;
    ops->draw_fleet = draw_fleet;
    ops->draw_planet_state = draw_planet_state;
    ops->draw_slot_hull = draw_slot_hull;
    ops->draw_default = draw_default;
    ops->draw_mineral = draw_generic;
    ops->draw_fleet_name = draw_generic;
    ops->draw_coordinates = draw_generic;
    ops->draw_zip_order = draw_generic;
    ops->draw_planet_population = draw_generic;
    ops->draw_factory_mine = draw_generic;
    ops->draw_component = draw_generic;
    ops->draw_string = draw_generic;
    ops->draw_resource = draw_generic;
    ops->context = trace;
}

static void setup(StarsDrawPopupInput *input, StarsDrawPopupOps *ops,
    PackedDrawContext *trace)
{
    memset(input, 0, sizeof *input);
    trace->calls = 0;
    trace->state_calls = 0;
    trace->fleet_seen = 0;
    trace->planet_seen = 0;
    trace->slot_seen = 0;
    trace->default_seen = 0;
    trace->payload_seen = 0;
    trace->last_kind = 0;
    {
        StarsI16 i;
        for (i = 0; i < 15; ++i)
            trace->generic_seen[i] = 0;
    }
    install(ops, trace);
    input->hwnd = 10;
    input->hdc = 20;
    input->popup_kind = 99;
    input->packed_popupdata = trace->popup;
    input->packed_popupdata_size = (StarsU16)sizeof trace->popup;
    input->font_normal = 3;
    input->font_bold = 4;
    input->arial8_height = 8;
    input->slot_starbase_mode = 1;
    input->ops = ops;
}

int main(void)
{
    PackedDrawContext trace;
    StarsDrawPopupInput input;
    StarsDrawPopupOps ops;
    int checks;
    int failures;
    int first_failure;

    memset(&trace, 0, sizeof trace);
    put16(trace.popup, 0, 3);
    put16(trace.popup, 2, 6);
    put16(trace.popup, 4, 12);
    put16(trace.popup, 6, 3);
    put16(trace.popup, 8, 21);
    put16(trace.popup, 10, 44);
    put16(trace.popup, 12, 0x0b0c);
    put16(trace.popup, 14, 0x0d0e);
    put16(trace.popup, 16, 0x0f10);
    put16(trace.popup, 18, 0x1112);
    put16(trace.popup, 20, 0x1314);
    put16(trace.fleet, 8, 101);
    put16(trace.fleet, 10, 202);
    put16(trace.fleet, 12, 4);
    put16(trace.fleet, 14, 2);
    put16(trace.planet, 0, 0x1234);
    checks = 0;
    failures = 0;
    first_failure = 0;
#define CHECK(condition) do { ++checks; if (!(condition)) { ++failures; if (first_failure == 0) first_failure = checks; } } while (0)
    CHECK(sizeof trace.popup == 22);
    CHECK(sizeof trace.fleet == 124);
    CHECK(sizeof trace.planet == 56);
    CHECK(get16(trace.popup, 0) == 3);
    CHECK(get16(trace.popup, 2) == 6);
    CHECK(get16(trace.popup, 4) == 12);
    CHECK(get16(trace.popup, 6) == 3);
    CHECK(get16(trace.popup, 8) == 21);
    CHECK(get16(trace.popup, 10) == 44);
    CHECK(get16(trace.fleet, 12) == 4);
    CHECK(get16(trace.fleet, 14) == 2);
    CHECK(get16(trace.fleet, 8) == 101);
    CHECK(get16(trace.fleet, 10) == 202);
    CHECK(get16(trace.planet, 0) == 0x1234);

    setup(&input, &ops, &trace);
    stars_draw_popup(&input);
    CHECK(trace.calls == 1 && trace.last_kind == 3);
    CHECK(trace.fleet_seen == 1);
    CHECK(trace.payload_seen == 1);
    CHECK(trace.state_calls == 8);

    put16(trace.popup, 0, 5);
    setup(&input, &ops, &trace);
    stars_draw_popup(&input);
    CHECK(trace.calls == 1 && trace.last_kind == 5);
    CHECK(trace.planet_seen == 1);

    put16(trace.popup, 0, 11);
    setup(&input, &ops, &trace);
    stars_draw_popup(&input);
    CHECK(trace.calls == 1 && trace.slot_seen == 1);
    CHECK(trace.last_slot.left == 8 && trace.last_slot.top == 0x13a);
    CHECK(trace.last_slot.right == 160 && trace.last_slot.bottom == 90);
    CHECK(trace.state_calls == 10);

    put16(trace.popup, 0, 1);
    setup(&input, &ops, &trace);
    stars_draw_popup(&input);
    CHECK(trace.calls == 1 && trace.generic_seen[1] == 1 &&
        trace.last_kind == 1);
    put16(trace.popup, 0, 2);
    setup(&input, &ops, &trace);
    stars_draw_popup(&input);
    CHECK(trace.calls == 1 && trace.generic_seen[2] == 1 &&
        trace.last_kind == 2);
    put16(trace.popup, 0, 4);
    setup(&input, &ops, &trace);
    stars_draw_popup(&input);
    CHECK(trace.calls == 1 && trace.generic_seen[4] == 1 &&
        trace.last_kind == 4);
    put16(trace.popup, 0, 6);
    setup(&input, &ops, &trace);
    stars_draw_popup(&input);
    CHECK(trace.calls == 1 && trace.generic_seen[6] == 1 &&
        trace.last_kind == 6);
    put16(trace.popup, 0, 7);
    setup(&input, &ops, &trace);
    stars_draw_popup(&input);
    CHECK(trace.calls == 1 && trace.generic_seen[7] == 1 &&
        trace.last_kind == 7);
    put16(trace.popup, 0, 8);
    setup(&input, &ops, &trace);
    stars_draw_popup(&input);
    CHECK(trace.calls == 1 && trace.generic_seen[8] == 1 &&
        trace.last_kind == 8);
    put16(trace.popup, 0, 9);
    setup(&input, &ops, &trace);
    stars_draw_popup(&input);
    CHECK(trace.calls == 1 && trace.generic_seen[9] == 1 &&
        trace.last_kind == 9);
    put16(trace.popup, 0, 10);
    setup(&input, &ops, &trace);
    stars_draw_popup(&input);
    CHECK(trace.calls == 1 && trace.generic_seen[10] == 1 &&
        trace.last_kind == 10);
    put16(trace.popup, 0, 12);
    setup(&input, &ops, &trace);
    stars_draw_popup(&input);
    CHECK(trace.calls == 1 && trace.generic_seen[12] == 1 &&
        trace.last_kind == 12);
    put16(trace.popup, 0, 14);
    setup(&input, &ops, &trace);
    stars_draw_popup(&input);
    CHECK(trace.calls == 1 && trace.slot_seen == 1 &&
        trace.last_kind == 14);

    put16(trace.popup, 0, 99);
    setup(&input, &ops, &trace);
    stars_draw_popup(&input);
    CHECK(trace.calls == 1 && trace.default_seen == 1);
    CHECK(input.popup_kind == 99);
    put16(trace.popup, 0, 3);
    setup(&input, &ops, &trace);
    input.packed_popupdata_size = 21;
    input.popup_kind = 2;
    stars_draw_popup(&input);
    CHECK(trace.calls == 1 && trace.last_kind == 2);
    CHECK(checks == 38);
    puts("DRAW_POPUP_PACKED_CHECKS=38 FAILURES=0 RESULT=PASS");
    return failures ? first_failure : 0;
#undef CHECK
}
