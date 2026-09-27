#include "core.h"

/* NE 26:3AB2-42E6. Draw the selected ship component's description, mineral
 * costs, and mass line through named component/UI services. */

static void build_comp_finish(StarsBuildSelCompInput *input, StarsU16 dc,
    StarsI16 acquired)
{
    if (input->ops && input->ops->restore)
        input->ops->restore(input->context, input->window, dc);
    if (acquired && input->ops && input->ops->release_dc)
        input->ops->release_dc(input->context, input->window, dc);
}

void stars_draw_build_sel_comp(StarsBuildSelCompInput *input, StarsI16 draw_mode)
{
    StarsU16 dc;
    StarsI16 acquired;
    StarsRect client;
    StarsRect fill;
    StarsBuildSelCompRecord component;
    StarsBuildSelCompRecord hull;
    StarsI16 y;
    StarsI16 index;
    StarsI32 value;

    (void)draw_mode;
    if (!input || !input->ops) return;
    acquired = input->hdc == 0;
    dc = input->hdc;
    if (acquired) {
        if (!input->ops->get_dc) return;
        dc = input->ops->get_dc(input->context, input->window);
    }
    if (input->ops->get_client_rect)
        input->ops->get_client_rect(input->context, input->slot_dialog, &client);
    else {
        client.left = 0; client.top = 0; client.right = 0x108; client.bottom = 0x100;
    }
    fill.left = 8; fill.top = (StarsU16)(input->build_info_y - 8);
    fill.right = 0x108; fill.bottom = (StarsU16)((StarsI16)client.bottom - 8);
    if (input->ops->select_font)
        input->ops->select_font(input->context, dc, 1);
    if (input->ops->set_text_color)
        input->ops->set_text_color(input->context, dc, 0);
    if (input->ops->set_background_color)
        input->ops->set_background_color(input->context, dc,
            input->button_face_color);
    if (input->ops->fill_background)
        input->ops->fill_background(input->context, dc, &fill);
    y = input->build_info_y;
    if (input->starbase_mode) y = (StarsI16)(y + input->arial8_height);
    if (input->selected_slot == -2) {
        build_comp_finish(input, dc, acquired); return;
    }
    component.group = 0; component.flags = 0; component.level = 0; component.mass = 0;
    hull = component;
    if (input->selected_slot == -1) {
        if (!input->ops->resolve_dialog_selection ||
            !input->ops->resolve_dialog_selection(input->context,
                input->slot_dialog, &component)) {
            build_comp_finish(input, dc, acquired); return;
        }
    } else if (!input->ops->resolve_slot ||
        !input->ops->resolve_slot(input->context, input->selected_slot,
            &component, &hull)) {
        build_comp_finish(input, dc, acquired); return;
    }
    if ((component.flags >> 8) == 0) {
        if (input->ops->draw_description)
            input->ops->draw_description(input->context, dc, 8, y, &component);
        build_comp_finish(input, dc, acquired); return;
    }
    if (input->ops->draw_header)
        input->ops->draw_header(input->context, dc, y, &component);
    y = (StarsI16)(y + input->arial8_height);
    for (index = 0; index < 3; ++index) {
        y = (StarsI16)(y + input->arial8_height);
        value = (StarsI32)input->technology_level *
            (StarsI32)input->mineral_cost[index];
        if (input->ops->draw_mineral_cost)
            input->ops->draw_mineral_cost(input->context, dc, index, y, value);
    }
    y = (StarsI16)(y + input->arial8_height);
    if (!input->starbase_mode && input->ops->draw_mass_cost) {
        value = (StarsI32)input->technology_level * (StarsI32)input->mass_cost;
        input->ops->draw_mass_cost(input->context, dc, y, value);
    }
    build_comp_finish(input, dc, acquired);
}
