#include "core.h"

#define DRAW_PLAN_SHIP_NO_SELECTION (-1)
#define DRAW_PLAN_SHIP_FLEET_KIND 2
#define DRAW_PLAN_SHIP_PRINT 0x8000U
#define DRAW_PLAN_SHIP_SELECTION 0x4000U
#define DRAW_PLAN_SHIP_FLAG_PRINT 0x0400U
#define DRAW_PLAN_SHIP_FLAG_SELECTION 0x1000U
#define DRAW_PLAN_SHIP_DEAD_STRING 800

static void hide_windows(StarsU16 hwnd, StarsDrawPlanShipInput *input)
{
    StarsI16 i;
    const StarsDrawPlanShipOps *ops = input->ops;
    if (!ops->show_window) return;
    for (i = 0; i < (StarsI16)input->button_count; ++i)
        ops->show_window(input->context, input->button_windows[i], 0);
    for (i = 0; i < (StarsI16)input->order_dropdown_count; ++i)
        ops->show_window(input->context, input->order_dropdown_windows[i], 0);
    ops->show_window(input->context, input->order_edit_window, 0);
    ops->show_window(input->context, input->ship_dropdown_window, 0);
    ops->show_window(input->context, input->battle_dropdown_window, 0);
    ops->show_window(input->context, input->ship_list_window, 0);
    ops->show_window(input->context, input->fleet_component_list_window, 0);
    ops->show_window(input->context, input->planet_production_list_window, 0);
    ops->show_window(input->context, input->report_combo_window, 0);
    (void)hwnd;
}

/* NE 10:0D16-1086. Hide or redraw the planet ship controls while preserving
 * the original indirect per-record drawing dispatch and DC ownership. */
void stars_draw_plan_ship(StarsU16 dc, StarsU16 grbit,
    StarsDrawPlanShipInput *input)
{
    StarsRect rect;
    StarsU8 text[128];
    StarsI16 length;
    StarsI16 index;
    StarsI16 count;
    StarsU16 obtained = 0U;
    StarsU16 old_font = 0U;
    StarsU32 old_background = 0UL;
    StarsU32 old_text = 0UL;
    StarsDrawPlanShipRecord *records;

    if (!input || !input->ops)
        return;
    if (input->selected_id == DRAW_PLAN_SHIP_NO_SELECTION) {
        hide_windows(input->hwnd_planet, input);
        for (index = 0; index < (StarsI16)input->reference_count; ++index) {
            input->reference_rects[index].left = (StarsU16)-6;
            input->reference_rects[index].top = (StarsU16)-5;
        }
        if (input->player_dead && dc != 0U && input->ops->get_client_rect) {
            input->ops->get_client_rect(input->context, input->hwnd_planet,
                &rect);
            if (input->ops->set_background_color)
                (void)input->ops->set_background_color(input->context, dc,
                    input->button_face_color);
            if (input->ops->set_text_color)
                (void)input->ops->set_text_color(input->context, dc,
                    input->button_text_color);
            text[0] = 0;
            length = input->ops->get_string ? input->ops->get_string(
                input->context, DRAW_PLAN_SHIP_DEAD_STRING, text,
                (StarsU16)sizeof text) : 0;
            if (input->ops->diagonal_text)
                input->ops->diagonal_text(input->context, dc, &rect, text,
                    length);
        }
        return;
    }
    if (input->selected_object_kind == DRAW_PLAN_SHIP_FLEET_KIND) {
        records = input->fleet_records;
        count = (StarsI16)input->fleet_record_count;
    } else {
        records = input->planet_records;
        count = (StarsI16)input->planet_record_count;
    }
    if (dc == 0U && input->ops->get_dc) {
        dc = input->ops->get_dc(input->context, input->hwnd_planet);
        obtained = 1U;
    }
    if (input->ops->select_font)
        old_font = input->ops->select_font(input->context, dc,
            input->font_normal);
    if (input->ops->set_background_color)
        old_background = input->ops->set_background_color(input->context, dc,
            input->button_face_color);
    if (input->ops->set_text_color)
        old_text = input->ops->set_text_color(input->context, dc,
            input->button_text_color);
    for (index = 0; index < count; ++index) {
        if (!records || (records[index].draw_mask & (StarsU32)grbit) == 0UL)
            continue;
        records[index].flags = (StarsU16)((records[index].flags &
            (StarsU16)~(DRAW_PLAN_SHIP_FLAG_PRINT |
            DRAW_PLAN_SHIP_FLAG_SELECTION)) |
            (((grbit & DRAW_PLAN_SHIP_PRINT) != 0U) ?
            DRAW_PLAN_SHIP_FLAG_PRINT : 0U) |
            (((grbit & DRAW_PLAN_SHIP_SELECTION) != 0U) ?
            DRAW_PLAN_SHIP_FLAG_SELECTION : 0U));
        if (input->ops->draw_record)
            input->ops->draw_record(input->context, dc, index,
                input->selected_value, records[index].flags);
    }
    if (input->ops->set_text_color)
        (void)input->ops->set_text_color(input->context, dc, old_text);
    if (input->ops->set_background_color)
        (void)input->ops->set_background_color(input->context, dc,
            old_background);
    if (input->ops->select_font)
        (void)input->ops->select_font(input->context, dc, old_font);
    if (obtained && input->ops->release_dc)
        input->ops->release_dc(input->context, input->hwnd_planet, dc);
}
