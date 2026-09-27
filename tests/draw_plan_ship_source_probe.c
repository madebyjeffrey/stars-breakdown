#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct DrawPlanShipTrace {
    StarsI16 show_calls;
    StarsU16 shown[32];
    StarsI16 rect_calls;
    StarsI16 background_calls;
    StarsI16 text_calls;
    StarsI16 font_calls;
    StarsU16 last_dc;
    StarsU32 last_color;
    StarsU16 last_font;
    StarsU16 acquired_dc;
    StarsI16 release_calls;
    StarsI16 string_calls;
    StarsI16 string_id;
    StarsI16 diagonal_calls;
    StarsU16 diagonal_dc;
    StarsI16 diagonal_length;
    StarsI16 record_calls;
    StarsI16 record_index;
    StarsU16 record_dc;
    StarsU16 record_selected;
    StarsU16 record_flags;
} DrawPlanShipTrace;

static void show_window(void *opaque, StarsU16 hwnd, StarsI16 show)
{
    DrawPlanShipTrace *trace = (DrawPlanShipTrace *)opaque;
    (void)show;
    if (trace->show_calls < 32) trace->shown[trace->show_calls] = hwnd;
    ++trace->show_calls;
}

static void get_client_rect(void *opaque, StarsU16 hwnd, StarsRect *rect)
{
    DrawPlanShipTrace *trace = (DrawPlanShipTrace *)opaque;
    ++trace->rect_calls; (void)hwnd;
    rect->left = 1; rect->top = 2; rect->right = 300; rect->bottom = 200;
}

static StarsU32 set_color(void *opaque, StarsU16 dc, StarsU32 color)
{
    DrawPlanShipTrace *trace = (DrawPlanShipTrace *)opaque;
    ++trace->background_calls; trace->last_dc = dc; trace->last_color = color;
    return 0x44UL;
}

static StarsU32 set_text_color(void *opaque, StarsU16 dc, StarsU32 color)
{
    DrawPlanShipTrace *trace = (DrawPlanShipTrace *)opaque;
    ++trace->text_calls; trace->last_dc = dc; trace->last_color = color;
    return 0x55UL;
}

static StarsU16 select_font(void *opaque, StarsU16 dc, StarsU16 font)
{
    DrawPlanShipTrace *trace = (DrawPlanShipTrace *)opaque;
    ++trace->font_calls; trace->last_dc = dc; trace->last_font = font;
    return 33U;
}

static StarsU16 get_dc(void *opaque, StarsU16 hwnd)
{
    DrawPlanShipTrace *trace = (DrawPlanShipTrace *)opaque;
    trace->acquired_dc = 9U; (void)hwnd;
    return trace->acquired_dc;
}

static void release_dc(void *opaque, StarsU16 hwnd, StarsU16 dc)
{
    DrawPlanShipTrace *trace = (DrawPlanShipTrace *)opaque;
    ++trace->release_calls; trace->last_dc = dc; (void)hwnd;
}

static StarsI16 get_string(void *opaque, StarsI16 id, StarsU8 *text,
    StarsU16 capacity)
{
    DrawPlanShipTrace *trace = (DrawPlanShipTrace *)opaque;
    ++trace->string_calls; trace->string_id = id;
    if (capacity > 3U) { text[0] = 'D'; text[1] = 'E'; text[2] = 'A';
        text[3] = 'D'; text[4] = 0; }
    return 4;
}

static void diagonal_text(void *opaque, StarsU16 dc, const StarsRect *rect,
    const StarsU8 *text, StarsI16 length)
{
    DrawPlanShipTrace *trace = (DrawPlanShipTrace *)opaque;
    ++trace->diagonal_calls; trace->diagonal_dc = dc;
    trace->diagonal_length = length;
    (void)rect; (void)text;
}

static void draw_record(void *opaque, StarsU16 dc, StarsI16 index,
    StarsU16 selected, StarsU16 flags)
{
    DrawPlanShipTrace *trace = (DrawPlanShipTrace *)opaque;
    ++trace->record_calls; trace->record_index = index;
    trace->record_dc = dc; trace->record_selected = selected;
    trace->record_flags = flags;
}

static void setup(StarsDrawPlanShipInput *input, StarsDrawPlanShipOps *ops,
    DrawPlanShipTrace *trace, StarsU16 *buttons, StarsU16 *dropdowns,
    StarsRect *rects, StarsDrawPlanShipRecord *fleet,
    StarsDrawPlanShipRecord *planet)
{
    StarsI16 i;
    (void)memset(input, 0, sizeof(*input));
    (void)memset(ops, 0, sizeof(*ops));
    (void)memset(trace, 0, sizeof(*trace));
    for (i = 0; i < 13; ++i) buttons[i] = (StarsU16)(100 + i);
    for (i = 0; i < 3; ++i) dropdowns[i] = (StarsU16)(200 + i);
    for (i = 0; i < 3; ++i) fleet[i].flags = 0U;
    fleet[0].draw_mask = 1UL; fleet[1].draw_mask = 2UL;
    fleet[2].draw_mask = 4UL;
    planet[0].flags = 0U;
    planet[0].draw_mask = 2UL;
    input->hwnd_planet = 500U; input->font_normal = 7U;
    input->button_face_color = 0x11UL; input->button_text_color = 0x22UL;
    input->button_windows = buttons; input->button_count = 13U;
    input->order_dropdown_windows = dropdowns; input->order_dropdown_count = 3U;
    input->order_edit_window = 300U; input->ship_dropdown_window = 301U;
    input->battle_dropdown_window = 302U; input->ship_list_window = 303U;
    input->fleet_component_list_window = 304U;
    input->planet_production_list_window = 305U; input->report_combo_window = 306U;
    input->reference_rects = rects; input->reference_count = 19U;
    input->fleet_records = fleet; input->fleet_record_count = 3U;
    input->planet_records = planet; input->planet_record_count = 1U;
    input->selected_value = 77U; input->ops = ops; input->context = trace;
    ops->show_window = show_window; ops->get_client_rect = get_client_rect;
    ops->set_background_color = set_color; ops->set_text_color = set_text_color;
    ops->select_font = select_font; ops->get_dc = get_dc;
    ops->release_dc = release_dc; ops->get_string = get_string;
    ops->diagonal_text = diagonal_text; ops->draw_record = draw_record;
}

int main(void)
{
    StarsDrawPlanShipInput input;
    StarsDrawPlanShipOps ops;
    DrawPlanShipTrace trace;
    StarsU16 buttons[13];
    StarsU16 dropdowns[3];
    StarsRect rects[19];
    StarsDrawPlanShipRecord fleet[3];
    StarsDrawPlanShipRecord planet[1];
    int failures = 0;

    setup(&input, &ops, &trace, buttons, dropdowns, rects, fleet, planet);
    input.selected_id = -1;
    stars_draw_plan_ship(0U, 0U, &input);
    if (trace.show_calls != 23 || trace.rect_calls != 0 ||
            rects[0].left != (StarsU16)-6 || rects[0].top != (StarsU16)-5 ||
            rects[18].left != (StarsU16)-6 || rects[18].top != (StarsU16)-5) { puts("HIDE_FAIL"); ++failures; }
    input.player_dead = 1U;
    stars_draw_plan_ship(7U, 0U, &input);
    if (trace.show_calls != 46 || trace.rect_calls != 1 ||
            trace.string_id != 800 || trace.diagonal_calls != 1 ||
            trace.diagonal_dc != 7U || trace.diagonal_length != 4) { puts("DEAD_FAIL"); ++failures; }
    input.selected_id = 12; input.selected_object_kind = 2;
    trace.record_calls = 0; trace.release_calls = 0;
    stars_draw_plan_ship(0U, 0xc001U, &input);
    if (trace.record_calls != 1 || trace.record_index != 0 ||
            trace.record_dc != 9U || trace.record_selected != 77U ||
            trace.record_flags != 0x1400U || fleet[0].flags != 0x1400U ||
            trace.release_calls != 1 || trace.last_dc != 9U ||
            trace.font_calls != 2 || trace.background_calls != 3 ||
            trace.text_calls != 3) ++failures;
    if (failures == 0)
        puts("DRAW_PLAN_SHIP_SOURCE=PASS");
    else
        puts("DRAW_PLAN_SHIP_SOURCE=FAIL");
    return failures;
}
