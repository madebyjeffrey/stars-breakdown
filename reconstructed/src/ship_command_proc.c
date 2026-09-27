#include "core.h"

/* NE 11:2640-3D32. Route ship-pane button and control notifications. The
 * original routine reaches into the global selection, fleet/order records and
 * a large set of Windows controls; this typed adapter keeps the notification
 * decoding and order-field rules local and exposes pane mutations as named
 * actions for the application integration layer. */

static StarsU16 ship_command_low_word(StarsU32 value)
{
    return (StarsU16)value;
}

static StarsU16 ship_command_high_word(StarsU32 value)
{
    return (StarsU16)(value >> 16);
}

static StarsI16 ship_command_selection(const StarsShipCommandInput *input,
    StarsU16 control)
{
    StarsI32 value;
    if (!input->ops || !input->ops->selection) return -1;
    value = input->ops->selection(input->context, control);
    return (StarsI16)(StarsU16)value;
}

static void ship_command_action(const StarsShipCommandInput *input,
    StarsI16 action, StarsI16 value)
{
    if (input->ops && input->ops->action)
        input->ops->action(input->context, action, value);
}

static StarsI16 ship_command_button(const StarsShipCommandState *state,
    StarsU16 control)
{
    StarsI16 index;
    if (!state) return -1;
    for (index = 0; index < STARS_SHIP_COMMAND_BUTTONS; ++index)
        if (state->button[index] == control) return index;
    return -1;
}

static void ship_command_button_click(const StarsShipCommandInput *input,
    StarsI16 button)
{
    StarsI16 selection;
    if (button == 0) ship_command_action(input,
        STARS_SHIP_ACTION_SELECT_ORBITING_FLEET, 0);
    else if (button == 1) ship_command_action(input,
        STARS_SHIP_ACTION_SELECT_ORBITING_FLEET, 1);
    else if (button == 2) ship_command_action(input,
        STARS_SHIP_ACTION_TRANSFER_FLEET, 1);
    else if (button == 3) ship_command_action(input,
        STARS_SHIP_ACTION_SELECT_PLANET, input->state->planet_id);
    else if (button == 4) ship_command_action(input,
        STARS_SHIP_ACTION_SELECT_NEXT_FLEET, 1);
    else if (button == 5) ship_command_action(input,
        STARS_SHIP_ACTION_SELECT_PREVIOUS_FLEET, -1);
    else if (button == 6) ship_command_action(input,
        STARS_SHIP_ACTION_RENAME_FLEET, input->state->fleet_id);
    else if (button == 7) ship_command_action(input,
        STARS_SHIP_ACTION_TRANSFER_TO_PLANET, input->state->planet_id);
    else if (button == 8) ship_command_action(input,
        STARS_SHIP_ACTION_TRANSFER_ALL, input->state->fleet_id);
    else if (button == 9) ship_command_action(input,
        STARS_SHIP_ACTION_SPLIT_FLEET, input->state->fleet_id);
    else if (button == 10) {
        selection = ship_command_selection(input, input->state->component_list);
        ship_command_action(input, STARS_SHIP_ACTION_MERGE_FLEETS, selection);
    }
}

static void ship_command_order_kind(const StarsShipCommandInput *input)
{
    StarsI16 kind;
    StarsI16 i;
    if (!input->state) return;
    kind = ship_command_selection(input, input->state->order_dropdown[0]);
    if (kind < 0) return;
    input->state->order_kind = kind;
    for (i = 0; i < 3; ++i) input->state->order_values[i] = 0;
    ship_command_action(input, STARS_SHIP_ACTION_SET_ORDER_KIND, kind);
}

static void ship_command_order_target(const StarsShipCommandInput *input)
{
    StarsI16 target;
    StarsI16 order_kind;
    if (!input->state) return;
    target = ship_command_selection(input, input->state->order_dropdown[1]);
    if (target < 0) return;
    order_kind = input->state->order_kind;
    if (order_kind == 7) input->state->order_values[0] = target;
    else if (order_kind == 9) input->state->order_values[1] = target;
    else {
        input->state->order_values[0] = target;
        input->state->order_values[1] = target;
    }
    ship_command_action(input, STARS_SHIP_ACTION_SET_ORDER_TARGET, target);
}

static void ship_command_order_value(const StarsShipCommandInput *input)
{
    StarsI16 slot;
    StarsI16 value;
    if (!input->state) return;
    slot = ship_command_selection(input, input->state->order_dropdown[2]);
    value = ship_command_selection(input, input->state->order_dropdown[1]);
    if (slot < 0 || slot > 2) slot = 0;
    if (value < 0) value = 0;
    input->state->order_values[slot] = value;
    ship_command_action(input, STARS_SHIP_ACTION_SET_ORDER_VALUE, value);
}

static void ship_command_edit(const StarsShipCommandInput *input)
{
    StarsI32 entered;
    StarsI32 limit;
    StarsI16 slot;

    if (!input->state || !input->ops || !input->ops->edit_value) return;
    entered = input->ops->edit_value(input->context, input->state->order_edit);
    if (entered < 0L) entered = 0L;
    if (entered > 0x7fffL) entered = 0x7fffL;
    slot = ship_command_selection(input, input->state->order_dropdown[1]);
    limit = (slot == 5 || slot == 6) ? 100L : 4000L;
    if (entered > limit) entered = limit;
    input->state->order_values[0] = (StarsI16)entered;
    ship_command_action(input, STARS_SHIP_ACTION_SET_ORDER_VALUE,
        (StarsI16)entered);
}

void stars_ship_command_proc(StarsU16 hwnd, StarsU16 wparam, StarsU32 lparam,
    StarsShipCommandInput *input)
{
    StarsU16 control;
    StarsU16 notify;
    StarsI16 button;
    StarsI16 selection;

    (void)wparam;
    if (!input || !input->state) return;
    control = ship_command_low_word(lparam);
    notify = ship_command_high_word(lparam);
    if (notify == STARS_SHIP_COMMAND_NOTIFY_CLICK && control == hwnd) {
        ship_command_action(input, STARS_SHIP_ACTION_SELECT_NEXT_FLEET, 0);
        return;
    }
    button = ship_command_button(input->state, control);
    if (button >= 0 && notify == STARS_SHIP_COMMAND_NOTIFY_CLICK) {
        ship_command_button_click(input, button);
        return;
    }
    if (control == input->state->ship_dropdown &&
        notify == STARS_SHIP_COMMAND_NOTIFY_CHANGE) {
        ship_command_action(input, STARS_SHIP_ACTION_DRAW, 0);
        return;
    }
    if (control == input->state->ship_list &&
        notify == STARS_SHIP_COMMAND_NOTIFY_CHANGE) {
        selection = ship_command_selection(input, control);
        ship_command_action(input, STARS_SHIP_ACTION_SET_SCAN_WAYPOINT,
            selection);
        return;
    }
    if (control == input->state->component_list &&
        notify == STARS_SHIP_COMMAND_NOTIFY_CHANGE) {
        selection = ship_command_selection(input, control);
        ship_command_action(input, STARS_SHIP_ACTION_COMPONENT_POPUP,
            selection);
        return;
    }
    if (control == input->state->battle_dropdown &&
        notify == STARS_SHIP_COMMAND_NOTIFY_CHANGE) {
        selection = ship_command_selection(input, control);
        ship_command_action(input, STARS_SHIP_ACTION_SELECT_BATTLE_PLAN,
            selection);
        return;
    }
    if (control == input->state->report_combo &&
        notify == STARS_SHIP_COMMAND_NOTIFY_CLICK) {
        selection = ship_command_selection(input, control);
        input->state->report_flags = (StarsU16)((input->state->report_flags &
            (StarsU16)~0x0200U) | ((selection & 1) << 9));
        ship_command_action(input, STARS_SHIP_ACTION_UPDATE_REPORT,
            selection & 1);
        return;
    }
    if (control == input->state->order_dropdown[0] &&
        notify == STARS_SHIP_COMMAND_NOTIFY_CHANGE) {
        ship_command_order_kind(input);
        return;
    }
    if (control == input->state->order_dropdown[1] &&
        notify == STARS_SHIP_COMMAND_NOTIFY_CHANGE) {
        ship_command_order_target(input);
        return;
    }
    if (control == input->state->order_dropdown[2] &&
        notify == STARS_SHIP_COMMAND_NOTIFY_CHANGE) {
        ship_command_order_value(input);
        return;
    }
    if (control == input->state->order_edit &&
        notify == STARS_SHIP_COMMAND_NOTIFY_EDIT) ship_command_edit(input);
}
