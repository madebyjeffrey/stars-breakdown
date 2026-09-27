#include "core.h"

enum {
    CHANGE_MAIN_PLANET = 1,
    CHANGE_MAIN_FLEET = 2,
    CHANGE_MAIN_PRODUCTION_RESET = 0x0407,
    CHANGE_MAIN_ORDER_SELECT = 0x040e,
    CHANGE_MAIN_DRAW_ALL = 0x4fff,
    CHANGE_MAIN_SCAN_FLEET = 0x0010,
    CHANGE_MAIN_BUTTON_COUNT = 13,
    CHANGE_MAIN_ORDER_COUNT = 3,
    CHANGE_MAIN_REFERENCE_COUNT = 0x13
};

#define CHANGE_MAIN_HIDDEN_TOP 0xfffbU
#define CHANGE_MAIN_HIDDEN_BOTTOM 0xfffaU

static void hide_windows(const StarsChangeMainObjInput *input)
{
    StarsI16 index;

    if (!input->show_window)
        return;
    for (index = 0; index < CHANGE_MAIN_BUTTON_COUNT; ++index)
        input->show_window(input->context, input->button_windows[index], 0);
    for (index = 0; index < CHANGE_MAIN_ORDER_COUNT; ++index)
        input->show_window(input->context,
            input->order_dropdown_windows[index], 0);
    input->show_window(input->context, input->order_edit_window, 0);
    input->show_window(input->context, input->ship_dropdown_window, 0);
    input->show_window(input->context, input->battle_dropdown_window, 0);
    input->show_window(input->context, input->ship_list_window, 0);
    input->show_window(input->context, input->fleet_component_window, 0);
    input->show_window(input->context, input->planet_production_window, 0);
    input->show_window(input->context, input->report_combo_window, 0);
}

static void reset_reference_rects(const StarsChangeMainObjInput *input)
{
    StarsU16 index;
    StarsU16 count = input->reference_rect_count;

    if (count > CHANGE_MAIN_REFERENCE_COUNT)
        count = CHANGE_MAIN_REFERENCE_COUNT;
    for (index = 0; index < count; ++index) {
        input->reference_rects[index].top = CHANGE_MAIN_HIDDEN_TOP;
        input->reference_rects[index].bottom = CHANGE_MAIN_HIDDEN_BOTTOM;
    }
}

/* Genuine semantic reconstruction of ChangeMainObjSel, NE 0A:3E7A-4299.
 * Selection records, fleet/planet lookup, and USER/GDI-facing effects are
 * explicit inputs.  The coordinator retains the executable's early returns
 * and side-effect order, while the callbacks isolate Win16 global state. */
void stars_change_main_obj_sel(StarsI16 grobj_new, StarsI16 object_id,
    StarsChangeMainObjInput *input)
{
    StarsI16 same_kind;
    StarsI16 fleet_index;
    StarsI16 skip_ship_id = -1;
    StarsChangeMainPlanet planet;
    StarsChangeMainFleet fleet;

    if (!input || !input->selection)
        return;
    same_kind = (StarsI16)(grobj_new == input->selection->grobj);
    if (input->f_ai != 0 && same_kind &&
            object_id == input->selection->id)
        return;

    if (input->invalidate_report)
        input->invalidate_report(input->context,
            (StarsI16)(input->selection->grobj != CHANGE_MAIN_PLANET), 0);

    if (grobj_new == CHANGE_MAIN_PLANET) {
        if (input->invalidate_report)
            input->invalidate_report(input->context, 0, 0);
        if (!input->lookup_planet ||
                !input->lookup_planet(input->context, object_id, &planet))
            return;
        if (!input->planet_points)
            return;
        input->selection->planet_id = object_id;
        input->selection->point = input->planet_points[object_id];
        input->selection->scan_waypoint = -1;
        input->selection->active_waypoint = -1;
        fleet_index = 0;
        while (fleet_index < input->fleet_count) {
            if (!input->fleets[fleet_index].present)
                break;
            if (input->fleets[fleet_index].id_planet == object_id &&
                    input->fleets[fleet_index].player ==
                    input->current_player)
                break;
            ++fleet_index;
        }
        if (fleet_index == input->fleet_count) {
            input->selection->fleet_overlap = 0xffffU;
            input->selection->grobj_full = 1;
        } else {
            fleet = input->fleets[fleet_index];
            if (input->copy_fleet)
                input->copy_fleet(input->context, &fleet,
                    &input->selection->fleet);
            input->selection->grobj_full = 3;
        }
        if (input->f_ai == 0) {
            if (input->fill_planet_production)
                input->fill_planet_production(input->context);
            if (input->send_message)
                input->send_message(input->context,
                    input->planet_production_window,
                    CHANGE_MAIN_PRODUCTION_RESET, 0, 0L);
        }
    } else {
        if (input->invalidate_report)
            input->invalidate_report(input->context, 1, 0);
        if (!input->lookup_fleet ||
                !input->lookup_fleet(input->context, object_id, &fleet))
            return;
        input->selection->fleet = fleet;
        input->selection->point = fleet.point;
        if (fleet.id_planet == -1 ||
                (fleet.id_planet != input->selection->planet_id &&
                (!input->lookup_planet || !input->lookup_planet(input->context,
                    fleet.id_planet, &planet))))
            input->selection->planet_id = -1;
        else if (fleet.id_planet != input->selection->planet_id)
            input->selection->planet_id = planet.id;
        input->selection->grobj_full =
            (StarsI16)(input->selection->planet_id != -1) | 2;
        input->selection->active_waypoint = 0;
        if (input->f_ai == 0) {
            if (input->fill_orders) input->fill_orders(input->context);
            if (input->fill_fleet_components)
                input->fill_fleet_components(input->context);
            if (input->fill_battle)
                input->fill_battle(input->context,
                    (StarsI16)(fleet.planet_slot + 1));
            skip_ship_id = object_id;
            if (input->send_message && input->order_dropdown_windows)
                input->send_message(input->context,
                    input->order_dropdown_windows[0],
                    CHANGE_MAIN_ORDER_SELECT,
                    (StarsU16)(fleet.order_max & 0x000fU), 0L);
        }
    }

    input->selection->grobj = grobj_new;
    input->selection->id = object_id;
    input->overlap_flags &= (StarsU16)~0x0100U;
    input->overlap_flags &= (StarsU16)~0x2000U;
    if (input->f_ai != 0)
        return;
    if (!same_kind) {
        hide_windows(input);
        reset_reference_rects(input);
    }
    if (input->fill_ship)
        input->fill_ship(input->context, skip_ship_id);
    if (same_kind) {
        if (input->draw_plan_ship)
            input->draw_plan_ship(input->context, 0, CHANGE_MAIN_DRAW_ALL);
    } else {
        if (input->invalidate_rect)
            input->invalidate_rect(input->context,
                input->planet_window, 1);
        if ((input->scan_flags & CHANGE_MAIN_SCAN_FLEET) != 0U &&
                input->selection->grobj == CHANGE_MAIN_PLANET) {
            input->scan_flags &= (StarsU16)~CHANGE_MAIN_SCAN_FLEET;
            if (input->invalidate_rect)
                input->invalidate_rect(input->context,
                    input->toolbar_window, 1);
        }
    }
    if (input->set_title)
        input->set_title(input->context, input->planet_window);
    if ((input->tutorial_flags & 0x0800U) != 0U && input->advance_tutor)
        input->advance_tutor(input->context);
}
