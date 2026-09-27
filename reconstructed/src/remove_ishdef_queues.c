#include "core.h"

/* NE 0B:C5E6-C82C. Remove production records whose packed kind is a ship
 * design of the requested id, compacting each surviving queue in place. */
void stars_remove_ishdef_from_all_queues(StarsRemoveIshdefQueuesInput *input,
    StarsI16 ishdef)
{
    StarsU16 planet_index;

    if (!input || !input->planets) return;
    for (planet_index = 0; planet_index < input->planet_count;
            ++planet_index) {
        StarsIshdefProductionPlanet *planet =
            &input->planets[planet_index];
        StarsU16 read_index;
        StarsU16 write_index;
        StarsU16 count;

        if (!planet->production_records || !planet->production_count ||
                *planet->production_count == 0U ||
                planet->player != input->player ||
                (planet->flags & 0x0200U) == 0U)
            continue;
        if (input->space_docks_only &&
                (!input->is_space_dock ||
                 input->is_space_dock(input->context, input->player,
                     (StarsU16)(planet->starbase_design & 0x000fU)) !=
                 0x21))
            continue;
        count = *planet->production_count;
        write_index = 0;
        for (read_index = 0; read_index < count; ++read_index) {
            StarsU32 record = planet->production_records[read_index];
            if (((record >> 17) & 7UL) == 2UL &&
                    ((record >> 10) & 0x7fUL) == (StarsU32)ishdef)
                continue;
            planet->production_records[write_index] = record;
            ++write_index;
        }
        if (write_index == 0U) {
            if (input->free_production)
                input->free_production(input->context, planet);
            planet->production_records = 0;
            *planet->production_count = 0U;
        } else if (write_index != count) {
            *planet->production_count = write_index;
        }
    }
    if (input->selected_is_planet && input->selected_planet &&
            input->selected_planet->production_records &&
            input->lookup_selected && input->fill_selected) {
        input->lookup_selected(input->context, input->selected_id,
            input->selected_planet);
        input->fill_selected(input->context, input->selected_planet);
    }
}
