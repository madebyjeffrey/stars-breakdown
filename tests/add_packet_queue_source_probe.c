#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace { StarsI16 item; StarsI16 calls; } Trace;
static StarsI16 add_item(void *context, StarsI16 item, StarsI16 count,
    StarsI16 a, StarsI16 b)
{
    Trace *t = (Trace *)context;
    (void)count; (void)a; (void)b; t->item = item; ++t->calls; return 1;
}
int main(void)
{
    StarsAddPacketQueueInput input;
    Trace trace;
    memset(&input, 0, sizeof input); memset(&trace, 0, sizeof trace);
    input.available[0] = 500UL; input.cost[0] = 100UL;
    input.available[1] = 800UL; input.cost[1] = 200UL;
    input.available[2] = 700UL; input.cost[2] = 100UL;
    input.add_item = add_item; input.context = &trace;
    if (!stars_add_packet_to_queue(&input) || trace.item != 15 || trace.calls != 1)
        return 1;
    input.available[1] = 250UL; input.cost[1] = 200UL;
    input.available[2] = 150UL; input.cost[2] = 100UL;
    input.available[0] = 150UL; input.cost[0] = 100UL;
    if (stars_add_packet_to_queue(&input) || trace.calls != 1) return 2;
    puts("ADD_PACKET_QUEUE_SOURCE=PASS");
    return 0;
}
