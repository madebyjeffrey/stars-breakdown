#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct BuddyProbe {
    StarsU32 distances[2];
    int distance_calls;
    int clears;
    int moves;
    StarsI16 order_id;
    StarsU16 order_flags;
} BuddyProbe;

static StarsU32 distance(void *context, const StarsU8 *fleet,
    const StarsU8 *candidate)
{
    BuddyProbe *probe = (BuddyProbe *)context;
    (void)fleet; (void)candidate;
    return probe->distances[probe->distance_calls++];
}

static StarsI16 random_value(void *context, StarsI16 count)
{
    (void)context; (void)count;
    return 1;
}

static void clear_task(void *context, const StarsU8 *fleet, StarsI16 task)
{
    BuddyProbe *probe = (BuddyProbe *)context;
    (void)fleet; (void)task;
    ++probe->clears;
}

static StarsI16 move_order(void *context, const StarsU8 *fleet,
    const StarsMoveOrder *order, StarsI16 replace)
{
    BuddyProbe *probe = (BuddyProbe *)context;
    (void)fleet; (void)replace;
    ++probe->moves; probe->order_id = order->id;
    probe->order_flags = order->flags;
    return 1;
}

static void put_word(StarsU8 *bytes, StarsU16 offset, StarsI16 value)
{
    StarsU16 bits = (StarsU16)value;
    bytes[offset] = (StarsU8)bits;
    bytes[offset + 1U] = (StarsU8)(bits >> 8);
}

int main(void)
{
    StarsU8 fleet[32];
    StarsU8 first[32];
    StarsU8 second[32];
    const StarsU8 *candidates[2];
    StarsPoint points[2];
    StarsFindBuddyOps ops;
    BuddyProbe probe;
    int failures = 0;

    memset(fleet, 0, sizeof(fleet));
    memset(first, 0, sizeof(first));
    memset(second, 0, sizeof(second));
    put_word(first, 2, 4); put_word(first, 0, 11); put_word(first, 12, 1);
    put_word(second, 2, 4); put_word(second, 0, 12); put_word(second, 14, 1);
    candidates[0] = first; candidates[1] = second;
    points[0].x = 10; points[0].y = 20;
    points[1].x = 30; points[1].y = 40;
    probe.distances[0] = 4; probe.distances[1] = 9;
    probe.distance_calls = 0; probe.clears = 0; probe.moves = 0;
    probe.order_id = 0; probe.order_flags = 0;
    ops.distance = distance; ops.random_value = random_value;
    ops.clear_task = clear_task; ops.move_order = move_order;
    if (stars_f_find_buddy_and_join_up(fleet, 0, 1, 2, 3, candidates,
            points, 2, 4, &ops, &probe) != 1)
        ++failures;
    if (probe.clears != 1 || probe.moves != 1 || probe.order_id != 11 ||
            probe.order_flags != 0x1260U)
        ++failures;
    printf("FIND_BUDDY_CHECKS=2 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
