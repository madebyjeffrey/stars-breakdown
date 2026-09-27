#include <string.h>
#include "core.h"

static void ship_dropdown_prefix(StarsU8 *text, StarsU8 prefix)
{
    memmove(text + 1, text, 0x32U);
    text[0] = prefix;
}

/* Genuine C reconstruction of FillShipDD, NE 0A:42A0-44D0. */
void stars_fill_ship_dd(StarsI16 skip_id,
    const StarsFillShipDropdownInput *input)
{
    StarsPoint selected_point;
    StarsI16 index;
    StarsU8 text[52];
    const StarsShipDropdownFleet *fleet;
    const StarsShipDropdownThing *thing;

    if (!input || !input->selection || !input->send_message)
        return;
    input->send_message(input->context, input->dropdown, 0x040bU, 0,
        (const StarsU8 *)0);
    if (input->selection->selected_object_kind == 1U &&
        input->selection->planet_points) {
        selected_point = input->selection->planet_points[
            input->selection->selected_id];
    } else {
        selected_point = input->selection->selected_fleet_point;
    }
    if (input->fleets && input->fleet_name) {
        for (index = 0; index < (StarsI16)input->fleet_count; ++index) {
            fleet = &input->fleets[index];
            if ((skip_id == -1 && fleet->planet_id ==
                input->selection->selected_id) ||
                (skip_id != -1 && fleet->id != skip_id &&
                fleet->point.x == input->selection->selected_fleet_point.x &&
                fleet->point.y == input->selection->selected_fleet_point.y)) {
                input->fleet_name(input->context, fleet->id, text,
                    (StarsU16)sizeof text);
                ship_dropdown_prefix(text,
                    (StarsU8)(fleet->owner == input->current_player ?
                    ' ' : 'x'));
                input->send_message(input->context, input->dropdown, 0x0403U,
                    0, text);
            }
        }
    }
    if (input->things && input->thing_name) {
        for (index = 0; index < (StarsI16)input->thing_count; ++index) {
            thing = &input->things[index];
            if (thing->kind == 1 && thing->point.x == selected_point.x &&
                thing->point.y == selected_point.y) {
                input->thing_name(input->context, thing->id, text,
                    (StarsU16)sizeof text);
                ship_dropdown_prefix(text,
                    (StarsU8)(thing->owner == input->current_player ?
                    ' ' : 'x'));
                input->send_message(input->context, input->dropdown, 0x0403U,
                    0, text);
            }
        }
    }
    input->send_message(input->context, input->dropdown, 0x040eU, 0,
        (const StarsU8 *)0);
}
