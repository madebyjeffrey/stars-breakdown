#include "core.h"

/* Genuine C reconstruction of FGetNextObjHere, NE 0C:909C-9285. */
StarsI16 stars_get_next_obj_here(StarsGetNextObjHereInput *input)
{
    StarsI16 i;
    StarsI16 found_next;
    const StarsNextObjectFleet *fleet;

    if (!input || !input->scan || !input->fleets)
        return 0;
    found_next = (StarsI16)(input->selected_kind != 2);
    for (i = 0; i < input->fleet_count; ++i) {
        fleet = &input->fleets[i];
        if (!fleet->valid)
            break;
        if (found_next) {
            if (fleet->point.x == input->selected_point.x &&
                    fleet->point.y == input->selected_point.y &&
                    (input->only_ours == 0 ||
                        fleet->player == input->current_player))
                break;
        } else if (fleet->id == input->selected_id) {
            found_next = 1;
        }
    }
    if (!found_next)
        return 0;
    if (i < input->fleet_count) {
        input->scan->fleet_index = i;
        input->scan->object_kind = 2;
        return 1;
    }
    if ((input->selected_full_flags & 1U) == 0U ||
            input->selected_planet_player != input->current_player) {
        for (i = 0; i < input->fleet_count; ++i) {
            fleet = &input->fleets[i];
            if (!fleet->valid ||
                    (fleet->point.x == input->scan->point.x &&
                    fleet->point.y == input->scan->point.y &&
                    (input->only_ours == 0 ||
                        fleet->player == input->current_player)))
                break;
        }
        if (i >= input->fleet_count ||
                input->fleets[i].id == input->selected_id)
            return 0;
        input->scan->fleet_index = i;
        input->scan->object_kind = 2;
    } else {
        input->scan->object_kind = 1;
        input->scan->planet_id = input->selected_planet_id;
    }
    return 1;
}
