#include "core.h"

/* NE 24:4FAA-50A0. Find or allocate the fleet/planet colonist-drop record,
 * initialize its ownership fields, and accumulate the requested amount. */
StarsI32 stars_queue_colonist_drop(StarsQueueColonistDropInput *input)
{
    StarsI16 i;
    StarsI32 amount;
    StarsColonistDropRecord *record;
    if (!input || !input->records) return 0;
    amount = input->colonists < 1 ? 1 : input->colonists;
    for (i = 0; i < input->record_count; ++i)
        if (input->records[i].fleet == input->fleet &&
            input->records[i].planet == input->planet) break;
    if (i == input->record_count) {
        if (input->record_count > 999 || input->record_count >= input->record_capacity)
            return 0;
        record = &input->records[i];
        record->fleet = input->fleet;
        record->player = input->player;
        record->planet = input->planet;
        record->colonists = 0;
        record->active = 1U;
        ++input->record_count;
    } else {
        record = &input->records[i];
    }
    record->colonists += amount;
    return amount;
}
