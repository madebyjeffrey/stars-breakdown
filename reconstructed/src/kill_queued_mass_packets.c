#include "core.h"

static StarsU16 mass_packet_type(StarsU32 record)
{
    return (StarsU16)((record >> 10) & 0x007fU);
}

static StarsU16 mass_packet_group(StarsU32 record)
{
    return (StarsU16)((record >> 17) & 7U);
}

/* NE 1A:6A52-6C29. Remove queued production records for mass packets,
 * compact the four-byte queue in place, release an empty queue, and refresh
 * the selected planet's production list when it is the affected planet. */
void stars_kill_queued_mass_packets(StarsKillQueuedMassPacketsInput *input)
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
        if (mass_packet_group(record) == 1U &&
                mass_packet_type(record) >= 14U &&
                mass_packet_type(record) <= 17U)
            continue;
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
