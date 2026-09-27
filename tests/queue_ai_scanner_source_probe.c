#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct ScannerProbe {
    int adds;
    StarsI16 type;
    StarsI16 quantity;
} ScannerProbe;

static void add_item(void *context, StarsI16 type, StarsI16 quantity)
{
    ScannerProbe *probe = (ScannerProbe *)context;
    ++probe->adds; probe->type = type; probe->quantity = quantity;
}

int main(void)
{
    StarsAiProductionCandidate global_items[2];
    StarsAiProductionCandidate pending[1];
    StarsI32 resources[4] = { 100, 100, 100, 100 };
    StarsI32 costs[4] = { 0, 0, 0, 0 };
    ScannerProbe probe;
    int failures = 0;

    memset(global_items, 0, sizeof(global_items));
    memset(pending, 0, sizeof(pending));
    global_items[0].category = 1; global_items[0].subtype = 18;
    global_items[0].quantity = 1; global_items[0].queue_type = 6;
    global_items[0].costs[0] = 10;
    global_items[1].category = 1; global_items[1].subtype = 20;
    global_items[1].quantity = 1; global_items[1].queue_type = 7;
    global_items[1].costs[0] = 20;
    probe.adds = 0; probe.type = 0; probe.quantity = 0;
    if (stars_f_queue_ai_scanner(pending, 0, global_items, 2, resources,
            costs, add_item, &probe) != 1 || probe.type != 7 ||
            costs[0] != 20)
        ++failures;
    pending[0].category = 1; pending[0].subtype = 19;
    probe.adds = 0;
    if (stars_f_queue_ai_scanner(pending, 1, global_items, 2, resources,
            costs, add_item, &probe) != 0 || probe.adds != 0)
        ++failures;
    printf("QUEUE_AI_SCANNER_CHECKS=2 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
