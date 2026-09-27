#include "core.h"

/* Typed semantic reconstruction of MineClick, NE 06:3b4e-47db. The popup
 * payload adapter decodes the recovered raw planet/fleet/thing records before
 * handing equivalent GlobalPD-facing values to the popup service. */

static StarsU16 mine_click_read_u16(const StarsU8 *record, StarsU16 offset)
{
    return (StarsU16)(record[offset] |
        ((StarsU16)record[(StarsU16)(offset + 1U)] << 8));
}

static StarsI16 mine_click_read_i16(const StarsU8 *record, StarsU16 offset)
{
    return (StarsI16)mine_click_read_u16(record, offset);
}

static void mine_click_decode_popup_data(StarsI16 hit,
    const StarsMineClickInput *input, StarsMineClickPopupData *data)
{
    StarsU16 word;
    StarsI16 variable;

    data->hit = hit;
    data->popup_kind = 0;
    if (hit >= 1 && hit <= 3)
        data->popup_kind = 1;
    else if (hit >= 6 && hit <= 8)
        data->popup_kind = 5;
    else if (hit == 10)
        data->popup_kind = 2;
    else if (hit == 11)
        data->popup_kind = 3;
    else if (hit == 12)
        data->popup_kind = 7;
    else if (hit == 13)
        data->popup_kind = 11;
    else if (hit == 14)
        data->popup_kind = 9;
    data->popup_string_id = -1;
    data->planet_id = -1;
    data->planet_owner = -1;
    data->planet_detail = 0;
    data->planet_var = -1;
    data->planet_value = -1;
    data->planet_min = -1;
    data->planet_max = -1;
    data->player_value = -1;
    data->player_min = -1;
    data->player_max = -1;
    data->derived_minerals = 0;
    data->derived_terraform = 0;
    for (variable = 0; variable < 3; ++variable) {
        data->terraform_min[variable] = -1;
        data->terraform_max[variable] = -1;
        data->mineral_output[variable] = 0;
    }
    data->fleet_id = -1;
    data->fleet_owner = -1;
    data->fleet_planet = -1;
    data->fleet_ship_count = 0;
    data->object_id = -1;
    data->object_owner = -1;
    data->object_kind = 0;
    data->object_x = 0;
    data->object_y = 0;
    if (input->packed_planet_record && input->packed_planet_size >= 56U) {
        data->planet_id = mine_click_read_i16(input->packed_planet_record, 0);
        data->planet_owner = mine_click_read_i16(
            input->packed_planet_record, 2);
        data->planet_detail = input->packed_planet_record[4];
        variable = hit >= 6 && hit <= 8 ? (StarsI16)(hit - 6) :
            (hit >= 1 && hit <= 3 ? (StarsI16)(hit - 1) : -1);
        data->planet_var = variable;
        if (variable >= 0 && variable < 3) {
            data->planet_value = input->packed_planet_record[
                (StarsU16)(12 + variable)];
            data->planet_min = input->packed_planet_record[
                (StarsU16)(9 + variable)];
            data->planet_max = input->packed_planet_record[
                (StarsU16)(15 + variable)];
        }
    }
    if (input->packed_fleet_record && input->packed_fleet_size >= 124U) {
        word = mine_click_read_u16(input->packed_fleet_record, 0);
        data->fleet_id = (StarsI16)(word & 0x01ffU);
        data->fleet_owner = mine_click_read_i16(
            input->packed_fleet_record, 2);
        data->fleet_planet = mine_click_read_i16(
            input->packed_fleet_record, 6);
        variable = hit >= 1 && hit <= 3 ? (StarsI16)(hit - 1) : 0;
        data->fleet_ship_count = mine_click_read_i16(
            input->packed_fleet_record,
            (StarsU16)(12 + variable * 2));
    }
    if (input->packed_thing_record && input->packed_thing_size >= 18U) {
        word = mine_click_read_u16(input->packed_thing_record, 0);
        data->object_id = (StarsI16)(word & 0x01ffU);
        data->object_owner = (StarsI16)((word >> 9) & 0x000fU);
        data->object_kind = (StarsI16)((word >> 13) & 0x0007U);
        data->object_x = mine_click_read_i16(input->packed_thing_record, 2);
        data->object_y = mine_click_read_i16(input->packed_thing_record, 4);
    }
}

static void mine_click_resolve_popup(StarsI16 hit,
    StarsMineClickInput *input, StarsMineClickPopupData *data)
{
    if (input->ops->popup_string)
        data->popup_string_id = input->ops->popup_string(input->ops->context,
            data->popup_kind, data->planet_var);
    if (hit >= 1 && hit <= 3 && input->ops->resolve_minerals)
        data->derived_minerals = input->ops->resolve_minerals(
            input->ops->context, data);
    if (hit >= 6 && hit <= 8 && input->ops->resolve_terraform)
        data->derived_terraform = input->ops->resolve_terraform(
            input->ops->context, data);
}

static void mine_show_prepared(StarsI16 hit, StarsI16 x, StarsI16 y,
    StarsMineClickInput *input)
{
    StarsI16 popup_kind;
    StarsMineClickPopupData popup_data;

    if ((!input->ops->prepare_popup && !input->ops->prepare_popup_data) ||
            !input->ops->show_popup)
        return;
    if (input->ops->prepare_popup_data &&
            ((input->packed_planet_record &&
                input->packed_planet_size >= 56U) ||
             (input->packed_fleet_record &&
                input->packed_fleet_size >= 124U) ||
             (input->packed_thing_record &&
                input->packed_thing_size >= 18U))) {
        mine_click_decode_popup_data(hit, input, &popup_data);
        mine_click_resolve_popup(hit, input, &popup_data);
        popup_kind = input->ops->prepare_popup_data(input->ops->context, hit,
            &popup_data);
    } else {
        popup_kind = input->ops->prepare_popup(input->ops->context, hit);
    }
    if (popup_kind >= 0)
        input->ops->show_popup(input->ops->context, popup_kind, x, y);
}

void stars_mine_click(StarsI16 x, StarsI16 y, StarsI16 message,
    StarsI16 sks, StarsMineClickInput *input)
{
    StarsI16 hit;
    StarsI16 choice;
    StarsI16 selected;
    StarsI16 index;
    StarsI16 values[9];

    (void)sks;
    if (!input || !input->ops || !input->ops->hit_test)
        return;
    hit = input->ops->hit_test(input->ops->context, x, y);
    if (message == 0x204 && hit != 9 && hit != 11)
        return;
    switch (hit) {
    case 1:
    case 2:
    case 3:
        mine_show_prepared(hit, x, y, input);
        break;
    case 5:
        values[0] = 100;
        values[1] = 500;
        values[2] = 1000;
        values[3] = 2500;
        values[4] = 5000;
        values[5] = 7500;
        values[6] = 10000;
        values[7] = 15000;
        values[8] = 30000;
        if (input->ops->threshold_menu) {
            selected = -1;
            for (index = 0; index < 9; ++index)
                if (values[index] == input->current_graph_max)
                    selected = index;
            choice = input->ops->threshold_menu(input->ops->context, values,
                9, selected);
            if (choice >= 0 && choice < 9 && values[choice] !=
                    input->current_graph_max) {
                input->current_graph_max = values[choice];
                if (input->ops->set_threshold)
                    input->ops->set_threshold(input->ops->context,
                        values[choice]);
                if (input->ops->invalidate_mine)
                    input->ops->invalidate_mine(input->ops->context);
                if (input->ops->scanner_visible &&
                        input->ops->scanner_visible(input->ops->context) &&
                        input->ops->set_default_popup)
                    input->ops->set_default_popup(input->ops->context, 1);
            }
        }
        break;
    case 6:
    case 7:
    case 8:
        mine_show_prepared(hit, x, y, input);
        break;
    case 9:
        if (message == 0x204) {
            if (input->ops->popup_mineral_choices)
                input->ops->popup_mineral_choices(input->ops->context, hit);
        } else if (input->ops->advance_scan_selection) {
            input->ops->advance_scan_selection(input->ops->context, hit);
        }
        break;
    case 10:
        mine_show_prepared(hit, x, y, input);
        break;
    case 11:
        if (message == 0x204) {
            if (input->ops->popup_fleet_cargo)
                input->ops->popup_fleet_cargo(input->ops->context, hit);
        } else {
            mine_show_prepared(hit, x, y, input);
        }
        break;
    case 12:
    case 13:
    case 14:
        mine_show_prepared(hit, x, y, input);
        break;
    default:
        break;
    }
}
