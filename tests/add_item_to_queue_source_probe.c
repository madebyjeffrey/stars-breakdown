#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct QueueProbe {
    int initialized;
    int finished;
    StarsI16 finish_value;
} QueueProbe;

static StarsU16 item_capacity(void *context, StarsU16 item,
    StarsU16 object_group)
{
    (void)context;
    return item == 9U && object_group == 3U ? 3U : 0U;
}

static void initialize(void *context, StarsU8 *first_record)
{
    QueueProbe *probe = (QueueProbe *)context;
    ++probe->initialized;
    memset(first_record, 0, 4);
}

static void finish(void *context, StarsI16 value)
{
    QueueProbe *probe = (QueueProbe *)context;
    ++probe->finished;
    probe->finish_value = value;
}

static StarsU32 record_at(const StarsU8 *record)
{
    return (StarsU32)record[0] | ((StarsU32)record[1] << 8) |
        ((StarsU32)record[2] << 16) | ((StarsU32)record[3] << 24);
}

int main(void)
{
    StarsU8 records[16];
    StarsAiProductionQueue queue;
    StarsAddItemToQueueOps ops;
    QueueProbe probe;
    StarsU32 expected;
    int failures = 0;

    memset(records, 0, sizeof(records));
    queue.records = records; queue.count = 2; queue.capacity = 4;
    records[0] = 1; records[4] = 2;
    memset(&probe, 0, sizeof(probe));
    ops.item_capacity = item_capacity; ops.initialize = initialize;
    ops.finish = finish;
    stars_add_item_to_queue(0, &queue, 9, 5, 3, 0, &ops, &probe);
    expected = 5UL | (9UL << 10) | (3UL << 17);
    if (queue.count != 3 || record_at(records) != expected ||
        record_at(records + 4) != 1UL)
        ++failures;
    stars_add_item_to_queue(0, &queue, 10, 2, 1, 1, &ops, &probe);
    expected = 2UL | (10UL << 10) | (1UL << 17);
    if (queue.count != 4 || record_at(records + 12) != expected)
        ++failures;
    probe.initialized = 0; probe.finished = 0;
    stars_add_item_to_queue(0, 0, 9, 5, 3, 2, &ops, &probe);
    if (probe.initialized != 1 || probe.finished != 1 ||
        probe.finish_value != 1)
        ++failures;
    printf("ADD_ITEM_TO_QUEUE_CHECKS=3 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
