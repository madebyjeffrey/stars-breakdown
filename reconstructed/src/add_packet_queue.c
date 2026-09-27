#include "core.h"

/* NE 22:2BC0-2CE2. Select the resource with the greatest post-production
 * remainder (the original tie rules retain the earlier resource on ties), and queue
 * the corresponding packet when at least 0xaa units remain. */
StarsI16 stars_add_packet_to_queue(StarsAddPacketQueueInput *input)
{
    StarsU16 selected = 0U;
    StarsU16 i;
    StarsU32 remainder[3];
    if (!input || !input->add_item) return 0;
    for (i = 0; i < 3U; ++i) remainder[i] = input->available[i] - input->cost[i];
    for (i = 1U; i < 3U; ++i)
        if (remainder[selected] < remainder[i]) selected = i;
    if ((StarsI32)remainder[selected] < 0 || remainder[selected] < 0xaaUL)
        return 0;
    input->add_item(input->context, (StarsI16)(selected + 14U), 1, 1, 0);
    return 1;
}
