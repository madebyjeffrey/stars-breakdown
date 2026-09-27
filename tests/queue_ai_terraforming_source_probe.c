#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct TerraformProbe {
    int adds;
    StarsI16 type;
    StarsI16 quantity;
} TerraformProbe;

static void add_item(void *context, StarsI16 type, StarsI16 quantity)
{
    TerraformProbe *probe = (TerraformProbe *)context;
    ++probe->adds; probe->type = type; probe->quantity = quantity;
}

int main(void)
{
    StarsAiProductionCandidate global_item;
    StarsI32 costs[4] = { 0, 0, 0, 0 };
    TerraformProbe probe;
    int failures = 0;

    memset(&global_item, 0, sizeof(global_item));
    global_item.category = 1; global_item.subtype = 12;
    global_item.quantity = 7; global_item.queue_type = 13;
    global_item.costs[0] = 5;
    probe.adds = 0; probe.type = 0; probe.quantity = 0;
    if (stars_f_queue_ai_terraforming(1, 1, 0, 0, 0, &global_item, 1,
            costs, add_item, &probe) != 1 || probe.type != 13 ||
            probe.quantity != 4 || global_item.quantity != 0 ||
            costs[0] != 20)
        ++failures;
    if (stars_f_queue_ai_terraforming(4, 1, 0, 0, 0, &global_item, 1,
            costs, add_item, &probe) != 0)
        ++failures;
    printf("QUEUE_AI_TERRAFORMING_CHECKS=2 FAILURES=%d RESULT=%s\n",
        failures, failures ? "FAIL" : "PASS");
    return failures != 0;
}
