#include "core.h"

static StarsU16 queued_ship_type(StarsU32 record)
{
    return (StarsU16)((record >> 10) & 0x007fU);
}

static StarsU16 queued_ship_group(StarsU32 record)
{
    return (StarsU16)((record >> 17) & 7U);
}

/* NE 1A:6C2A-6E1D. Remove queued ship records, normalize retained ship
 * records by the original high-word mask, compact or release the queue, and
 * refresh the selected planet's production list when necessary. */
void stars_kill_queued_ships(StarsKillQueuedMassPacketsInput *input)
{
    StarsU16 read_index;
    StarsU16 write_index = 0;
    StarsU16 count;
    StarsU32 record;

    if (!input || !input->production_records || !input->production_count)
        return;
    count = *input->production_count;
    for (read_index = 0; read_index < count; ++read_index) {
        record = input->production_records[read_index];
        if (queued_ship_group(record) == 2U &&
                queued_ship_type(record) <= 15U)
            continue;
        if (queued_ship_group(record) == 2U) {
            record = (record & 0x0000ffffUL) |
                (((record >> 16) & 0x0f80fU) << 16);
        }
        input->production_records[write_index++] = record;
    }
    if (write_index == 0U) {
        if (input->free_production)
            input->free_production(input->context,
                input->production_records);
        *input->production_records = 0U;
        *input->production_count = 0U;
    } else {
        *input->production_count = write_index;
    }
    if (input->selected_group == 1U &&
            input->selected_planet_id == input->planet_id &&
            input->refresh_production)
        input->refresh_production(input->context, input->planet_id,
            input->production_records, *input->production_count);
}
