#include <stdio.h>
#include <string.h>
#include "core.h"

int main(void)
{
    StarsColonistDropRecord records[2];
    StarsQueueColonistDropInput input;
    memset(records, 0, sizeof records); memset(&input, 0, sizeof input);
    input.fleet = 4; input.player = 2; input.planet = 9; input.colonists = 0;
    input.records = records; input.record_capacity = 2;
    if (stars_queue_colonist_drop(&input) != 1 || input.record_count != 1 ||
        records[0].colonists != 1 || records[0].active != 1U) return 1;
    input.colonists = 23;
    if (stars_queue_colonist_drop(&input) != 23 || records[0].colonists != 24)
        return 2;
    input.fleet = 5; input.planet = 10; input.colonists = 4;
    if (stars_queue_colonist_drop(&input) != 4 || input.record_count != 2)
        return 3;
    input.fleet = 6; input.planet = 11;
    if (stars_queue_colonist_drop(&input) != 0 || input.record_count != 2)
        return 4;
    puts("QUEUE_COLONIST_DROP_SOURCE=PASS");
    return 0;
}
