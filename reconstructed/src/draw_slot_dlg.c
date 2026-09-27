#include "core.h"

/* NE 26:2650-306A. Paint the ship-builder slot strip, cargo panel, and
 * optional design plaque in the same ordered branches as the Win16 routine. */

void stars_draw_slot_dlg(StarsDrawSlotInput *input)
{
    StarsU16 dc;
    StarsI16 acquired;
    StarsI16 first;
    StarsI16 last;
    StarsI16 index;
    StarsI16 origin_x;
    StarsI16 dock_style;

    if (!input || !input->ops || input->mode == 3 || input->slot_count <= 0)
        return;
    acquired = input->hdc == 0;
    dc = input->hdc;
    if (acquired) {
        if (!input->ops->get_dc) return;
        dc = input->ops->get_dc(input->context, input->window);
    }
    origin_x = input->window == 0 ? 4 : 0;
    if (input->ops->draw_fleet_bitmap)
        input->ops->draw_fleet_bitmap(input->context, dc, origin_x, 6,
            (StarsU16)input->mode);
    if (input->mode == 4) {
        if (input->ops->draw_spin_buttons) {
            input->ops->draw_spin_buttons(input->context, dc, 0);
            input->ops->draw_spin_buttons(input->context, dc, 1);
        }
    } else if (input->ops->draw_spin_buttons) {
        input->ops->draw_spin_buttons(input->context, dc, -1);
    }
    if (input->draw_index < 0) {
        first = 0; last = input->cargo_max;
    } else {
        first = input->draw_index; last = (StarsI16)(input->draw_index + 1);
    }
    if (input->cargo_max > 0 && input->ops->draw_cargo_panel) {
        dock_style = input->starbase_mode &&
            (input->hull_id == 0x21 || input->hull_id == 0x24);
        input->ops->draw_cargo_panel(input->context, dc, &input->cargo_rect,
            input->starbase_mode, dock_style);
    }
    if (first < 0) first = 0;
    if (last > input->slot_count) last = input->slot_count;
    if (last > STARS_DRAW_SLOT_MAX) last = STARS_DRAW_SLOT_MAX;
    for (index = first; index < last; ++index) {
        if (input->ops->fill_slot)
            input->ops->fill_slot(input->context, dc, &input->slot_rect[index], index);
        if ((input->slots[index].flags >> 8) == 0) {
            if (input->ops->draw_empty_slot)
                input->ops->draw_empty_slot(input->context, dc,
                    &input->slot_rect[index], &input->slots[index]);
        } else if (input->ops->draw_part_slot) {
            input->ops->draw_part_slot(input->context, dc,
                &input->slot_rect[index], &input->slots[index]);
        }
        if (input->selected_slot == index && input->ops->draw_selected_frame)
            input->ops->draw_selected_frame(input->context, dc,
                &input->slot_rect[index]);
    }
    if (input->popup_window == 0 && input->global_planet == 0 &&
        input->mode == 0 && input->ops->draw_plaque)
        input->ops->draw_plaque(input->context, dc, &input->plaque_rect,
            input->built_count, input->existing_count, input->slot_count,
            input->mode);
    if (input->ops->restore) input->ops->restore(input->context, input->window, dc);
    if (acquired && input->ops->release_dc)
        input->ops->release_dc(input->context, input->window, dc);
}
