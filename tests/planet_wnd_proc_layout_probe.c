#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct CreateRecord {
    StarsU16 class_kind;
    StarsU16 title_token;
    StarsU32 style;
    StarsI16 x;
    StarsI16 y;
    StarsI16 width;
    StarsI16 height;
    StarsU16 parent;
    StarsU16 handle;
} CreateRecord;

typedef struct Trace {
    StarsI16 create_count;
    StarsI16 font_count;
    StarsI16 edit_limit_count;
    StarsI16 get_string_count;
    StarsI16 get_rect_count;
    StarsI16 get_long_count;
    StarsI16 set_long_count;
    StarsU16 font_handles[24];
    StarsU16 font_values[24];
    StarsU16 edit_handle;
    StarsU16 edit_limit;
    StarsI16 string_ids[24];
    StarsU16 get_long_handle;
    StarsU16 rect_handle;
    StarsI16 get_long_index;
    StarsU16 set_long_handle;
    StarsI16 set_long_index;
    StarsI32 set_long_value;
    CreateRecord creates[24];
} Trace;

static StarsU16 create_window(void *context, StarsU16 class_kind,
    StarsU16 title_token, const StarsU8 *title, StarsU32 style,
    StarsI16 x, StarsI16 y, StarsI16 width, StarsI16 height, StarsU16 parent)
{
    Trace *trace;
    CreateRecord *record;

    trace = (Trace *)context;
    (void)title;
    record = &trace->creates[trace->create_count];
    record->class_kind = class_kind;
    record->title_token = title_token;
    record->style = style;
    record->x = x;
    record->y = y;
    record->width = width;
    record->height = height;
    record->parent = parent;
    record->handle = (StarsU16)(500 + trace->create_count);
    ++trace->create_count;
    return record->handle;
}

static void set_font(void *context, StarsU16 hwnd, StarsU16 font)
{
    Trace *trace;

    trace = (Trace *)context;
    trace->font_handles[trace->font_count] = hwnd;
    trace->font_values[trace->font_count] = font;
    ++trace->font_count;
}

static void set_edit_limit(void *context, StarsU16 hwnd, StarsU16 limit)
{
    Trace *trace;

    trace = (Trace *)context;
    trace->edit_handle = hwnd;
    trace->edit_limit = limit;
    ++trace->edit_limit_count;
}

static const StarsU8 *get_string(void *context, StarsU16 id)
{
    Trace *trace;

    trace = (Trace *)context;
    trace->string_ids[trace->get_string_count] = (StarsI16)id;
    ++trace->get_string_count;
    return (const StarsU8 *)"label";
}

static void get_rect(void *context, StarsU16 hwnd, StarsRect *rect)
{
    Trace *trace;

    trace = (Trace *)context;
    trace->rect_handle = hwnd;
    rect->left = 0;
    rect->top = 0;
    rect->right = 200;
    rect->bottom = 18;
    ++trace->get_rect_count;
}

static StarsI32 get_long(void *context, StarsU16 hwnd, StarsI16 index)
{
    Trace *trace;

    trace = (Trace *)context;
    trace->get_long_handle = hwnd;
    trace->get_long_index = index;
    ++trace->get_long_count;
    return 0x12345678L;
}

static void set_long(void *context, StarsU16 hwnd, StarsI16 index,
    StarsI32 value)
{
    Trace *trace;

    trace = (Trace *)context;
    trace->set_long_handle = hwnd;
    trace->set_long_index = index;
    trace->set_long_value = value;
    ++trace->set_long_count;
}

static StarsI16 check(StarsI16 condition, StarsI16 *checks)
{
    ++*checks;
    return condition ? 0 : 1;
}

static StarsI16 equal_create(const Trace *trace, StarsI16 index,
    StarsU16 class_kind, StarsU16 title_token, StarsU32 style,
    StarsI16 height, StarsU16 parent)
{
    const CreateRecord *record;

    record = &trace->creates[index];
    return record->class_kind == class_kind &&
        record->title_token == title_token && record->style == style &&
        record->x == 100 && record->y == 100 && record->width == 200 &&
        record->height == height && record->parent == parent;
}

int main(void)
{
    StarsPlanetWndLayoutOps layout_ops;
    StarsPlanetWndLayoutState state;
    StarsPlanetWndProcOps proc_ops;
    StarsPlanetWndProcInput input;
    Trace trace;
    StarsI16 checks;
    StarsI16 failures;
    StarsI16 index;

    memset(&layout_ops, 0, sizeof layout_ops);
    memset(&state, 0, sizeof state);
    memset(&proc_ops, 0, sizeof proc_ops);
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    layout_ops.create_window = create_window;
    layout_ops.set_font = set_font;
    layout_ops.set_edit_limit = set_edit_limit;
    layout_ops.get_compressed_string = get_string;
    layout_ops.get_client_rect = get_rect;
    layout_ops.get_window_long = get_long;
    layout_ops.set_window_long = set_long;
    layout_ops.font_ariel8 = 0x30;
    layout_ops.font_ariel8_bold = 0x31;
    layout_ops.ariel8_height = 8;
    layout_ops.fake_edit_proc = 0x4567;
    layout_ops.context = &trace;
    proc_ops.context = &trace;
    input.ops = &proc_ops;
    input.layout_ops = &layout_ops;
    input.layout_state = &state;
    checks = 0;
    failures = 0;
    failures += check(stars_planet_wnd_proc(77, 1, 0, 0, &input) == 0,
        &checks);
    failures += check(trace.create_count == 23 && trace.font_count == 23,
        &checks);
    failures += check(equal_create(&trace, 0, 1, 0x0876, 0x40200003UL,
        0x50, 77) && equal_create(&trace, 1, 1, 0x0876, 0x40200213UL,
        0x50, 77) && equal_create(&trace, 2, 1, 0x0876, 0x40200003UL,
        0xa0, 77), &checks);
    failures += check(equal_create(&trace, 3, 2, 0, 0x40800002UL,
        0x32, 77) && equal_create(&trace, 4, 1, 0x087c, 0x40200003UL,
        0x50, 77) && equal_create(&trace, 5, 1, 0x0885, 0x40200213UL,
        0x50, 77), &checks);
    failures += check(equal_create(&trace, 6, 3, 0x088c, 0x40a01001UL,
        0x50, 77) && equal_create(&trace, 7, 3, 0x0893, 0x40a00051UL,
        0x50, 77) && equal_create(&trace, 8, 3, 0x089f, 0x40a00051UL,
        0x50, 77), &checks);
    for (index = 0; index < 13; ++index)
        failures += check(equal_create(&trace, (StarsI16)(9 + index), 4,
            (StarsU16)(0x19f + index), 0x40000000UL, 16, 77), &checks);
    failures += check(equal_create(&trace, 22, 4, 799, 0x40000003UL,
        8, 77), &checks);
    failures += check(trace.get_string_count == 24 &&
        trace.string_ids[0] == 0x63 && trace.string_ids[9] == 0x6c &&
        trace.string_ids[10] == 0x19f &&
        trace.string_ids[22] == 0x1ab && trace.string_ids[23] == 799,
        &checks);
    failures += check(trace.edit_limit_count == 1 && trace.edit_handle == 503 &&
        trace.edit_limit == 4, &checks);
    failures += check(trace.get_long_count == 1 && trace.get_long_handle == 503 &&
        trace.get_long_index == -4 && trace.set_long_count == 1 &&
        trace.set_long_handle == 503 && trace.set_long_index == -4 &&
        trace.set_long_value == 0x4567L && state.real_edit_proc == 0x12345678L,
        &checks);
    failures += check(trace.get_rect_count == 1 &&
        trace.rect_handle == 505 && state.ship_dropdown_height == 18,
        &checks);
    failures += check(state.order_dropdowns[0] == 500 &&
        state.order_dropdowns[2] == 502 && state.order_edit == 503 &&
        state.battle_dropdown == 504 && state.ship_dropdown == 505 &&
        state.ship_list == 506 && state.fleet_component_list == 507 &&
        state.planet_production_list == 508 && state.buttons[0] == 509 &&
        state.buttons[12] == 521 && state.report_combo == 522,
        &checks);
    failures += check(input.ship_dropdown == 505 &&
        input.planet_production_list == 508 && input.rep_combo == 522 &&
        input.button_handles[0] == 509 && input.button_handles[12] == 521,
        &checks);
    for (index = 0; index < 23; ++index)
        failures += check(trace.font_values[index] ==
            (index == 7 || index == 8 ? 0x30 : 0x31), &checks);
#ifndef TARGET_NATIVE
    printf("PLANET_WND_PROC_LAYOUT_E1_CHECKS=%d FAILURES=%d RESULT=%s\n",
        (int)checks, (int)failures,
        failures == 0 ? "PASS" : "FAIL");
#endif
#ifdef TARGET_NATIVE
    {
        FILE *report;

        report = fopen("C:\\PWLE\\PWLE1.TXT", "w");
        if (!report)
            return 2;
        fprintf(report,
            "PLANET_WND_PROC_LAYOUT_E1_CHECKS=%d FAILURES=%d RESULT=%s\n",
            (int)checks, (int)failures,
            failures == 0 ? "PASS" : "FAIL");
        fclose(report);
    }
#endif
    return failures == 0 ? 0 : 1;
}
