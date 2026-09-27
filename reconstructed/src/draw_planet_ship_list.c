#include "core.h"

static StarsU16 planet_ship_list_packed_get16(const StarsU8 *record,
    StarsU16 offset)
{
    return (StarsU16)(record[offset] |
        ((StarsU16)record[offset + 1] << 8));
}

/* Typed semantic reconstruction of DrawPlanetShipList, NE 10:377e-3dec.
 * Win16 controls, transfer lookup, and packed fleet/thing drawing remain
 * callback-owned; this file preserves the visibility, enablement, and layout
 * decisions around them. */

void stars_draw_planet_ship_list(StarsPlanetShipListInput *input)
{
    const StarsPlanetShipListOps *ops;
    StarsRect tile_rect;
    StarsRect detail_rect;
    StarsI16 item;
    StarsI16 unavailable;
    StarsI16 detail_width;
    StarsU16 combo_flags;
    StarsI16 packed_records;

    if (!input || !input->ops)
        return;
    ops = input->ops;
    packed_records = 0;
    if (input->packed_selection_record &&
            input->packed_selection_size >= 226U) {
        input->state.has_object = planet_ship_list_packed_get16(
            input->packed_selection_record, 152) != 0;
        packed_records = 1;
    }
    if (input->packed_transfer_record &&
            input->packed_transfer_size >= 128U) {
        input->state.transfer_index = (StarsI16)
            planet_ship_list_packed_get16(input->packed_transfer_record, 0);
        input->state.transfer_kind = (StarsI16)
            planet_ship_list_packed_get16(input->packed_transfer_record, 2);
        input->state.fleet_owner = (StarsI16)
            planet_ship_list_packed_get16(input->packed_transfer_record, 6);
        packed_records = 1;
    }
    tile_rect.left = 0;
    tile_rect.top = 0;
    tile_rect.right = 0;
    tile_rect.bottom = 0;
    if (input->state.tile_changed) {
        for (item = 0; item < 3; ++item)
            if (ops->show_control)
                ops->show_control(ops->context, item, 0);
        if (ops->show_control)
            ops->show_control(ops->context, 3, 0);
        if (ops->reset_references)
            ops->reset_references(ops->context);
        input->state.tile_changed = 0;
    }
    item = input->state.has_object ? 0x339 : 0x338;
    if (!ops->draw_tile || !ops->draw_tile(ops->context, input->hdc, item,
            &tile_rect)) {
        for (item = 0; item < 3; ++item)
            if (ops->show_control)
                ops->show_control(ops->context, item, 0);
        if (ops->show_control)
            ops->show_control(ops->context, 3, 0);
        return;
    }
    detail_rect.left = (StarsI16)(tile_rect.left + 4);
    detail_rect.right = (StarsI16)(tile_rect.right - 4);
    detail_rect.top = (StarsI16)(tile_rect.top + 2);
    detail_rect.bottom = (StarsI16)(detail_rect.top + input->line_height);
    detail_width = (StarsI16)(detail_rect.right - detail_rect.left);
    combo_flags = input->state.tile_width == detail_width ? 0x15U : 0x14U;
    input->state.tile_width = detail_width;
    if (ops->set_combo_position_ex)
        ops->set_combo_position_ex(ops->context, detail_rect.left,
            detail_rect.top, detail_width, input->combo_height, combo_flags);
    else if (ops->set_combo_position)
        ops->set_combo_position(ops->context, detail_rect.left,
            detail_rect.top, detail_width, input->combo_height);
    if (ops->show_control)
        ops->show_control(ops->context, 3, 1);
    unavailable = 0;
    if (ops->combo_selection)
        input->state.combo_selection = (StarsI16)ops->combo_selection(
            ops->context, input->hdc);
    if (input->state.combo_selection < 0)
        unavailable = 1;
    if (!unavailable && !packed_records && ops->lookup_transfer)
        if (!ops->lookup_transfer(ops->context, input->player_id,
                input->state.combo_selection, input->state.transfer_index,
                &input->state.transfer_kind))
            unavailable = 1;
    if (ops->enable_button)
        ops->enable_button(ops->context, 0, !unavailable);
    input->state.transfer_found = unavailable ? 0 : 1;
    if (!unavailable && input->state.transfer_kind != 8 &&
            input->state.fleet_owner == input->player_id) {
        if (ops->enable_button)
            ops->enable_button(ops->context, 1, 1);
    } else if (ops->enable_button) {
        ops->enable_button(ops->context, 1, 0);
    }
    if (ops->reset_references)
        ops->reset_references(ops->context);
    input->state.mode_flag = input->state.tile_ship_mode ? 1 : 0;
    if (!input->state.mode_flag && !unavailable) {
        if (ops->draw_details)
            ops->draw_details(ops->context, &detail_rect,
                input->state.transfer_kind, input->state.transfer_index);
        else if (ops->fill_details)
            ops->fill_details(ops->context, &detail_rect);
    }
    detail_rect.top = (StarsI16)(detail_rect.top + input->line_height + 3);
    detail_width = (StarsI16)((detail_rect.right - detail_rect.left - 10) / 3);
    for (item = 0; item < 3; ++item) {
        if (ops->set_button_position)
            ops->set_button_position(ops->context, (StarsI16)((item + 1) % 3),
                (StarsI16)(detail_rect.left + item * (detail_width + 6)),
                detail_rect.top, detail_width,
                (StarsI16)(input->line_height / 2 + input->line_height));
        else if (ops->set_combo_position)
            ops->set_combo_position(ops->context,
                (StarsI16)(detail_rect.left + item * (detail_width + 6)),
                detail_rect.top, detail_width,
                (StarsI16)(input->line_height / 2 + input->line_height));
        if (item != 2 || input->state.has_object)
            if (ops->show_control)
                ops->show_control(ops->context, item, 1);
    }
    if (ops->enable_button)
        ops->enable_button(ops->context, 2,
            (!input->state.transfer_found || !input->state.mode_flag) ? 1 : 0);
}
