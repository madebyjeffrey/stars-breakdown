#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace { StarsU16 last_id; } Trace;

static StarsI16 compressed(void *context, StarsU16 id, StarsU8 *output,
    StarsU16 capacity)
{
    Trace *trace = (Trace *)context;
    const char *text = "Task %d...";
    trace->last_id = id;
    if (id == 100U) text = "Unknown";
    if (id == 105U || id == 106U) text = id == 105U ? "Pop %d" : "Rate %d";
    if (strlen(text) + 1U > capacity) return 0;
    strcpy((char *)output, text);
    return 1;
}

static StarsI16 localized(void *context, StarsU16 id, StarsU8 *output,
    StarsU16 capacity)
{
    Trace *trace = (Trace *)context;
    (void)capacity;
    trace->last_id = id;
    strcpy((char *)output, id == 105U ? "Localized 105" : "Localized 106");
    return 1;
}

int main(void)
{
    StarsTaskNameInput input;
    StarsZipOrder zip[4];
    Trace trace;
    StarsU8 output[64];
    StarsI16 order;
    memset(&input, 0, sizeof input);
    memset(zip, 0, sizeof zip);
    memset(&trace, 0, sizeof trace);
    input.compressed = compressed;
    input.localized = localized;
    input.context = &trace;
    input.dash_text = (const StarsU8 *)"--";
    input.special_names[0] = (const StarsU8 *)"Zip";
    input.special_names[1] = (const StarsU8 *)"Zip2";
    input.special_names[2] = (const StarsU8 *)"Zip6";
    input.suffix_names[0] = (const StarsU8 *)"%s planet %d";
    input.suffix_names[1] = (const StarsU8 *)"%s task %d";
    if (!stars_get_task_name(&input, output, sizeof output, &order) ||
        strcmp((char *)output, "--") != 0 || order != -1) return 1;
    input.orders[3] = 0x1006U;
    input.orders[4] = 2U;
    if (!stars_get_task_name(&input, output, sizeof output, &order) ||
        strcmp((char *)output, "Pop 3") != 0 || trace.last_id != 105U) return 2;
    input.orders[3] = 0x1007U;
    input.orders[5] = 10U;
    if (!stars_get_task_name(&input, output, sizeof output, &order) ||
        strcmp((char *)output, "Rate 550") != 0 || trace.last_id != 106U) return 3;
    memset(input.orders, 0, sizeof input.orders);
    input.orders[3] = 0x1001U;
    input.orders[4] = 0x2001U; input.orders[5] = 0x2002U;
    input.orders[6] = 0x2003U;
    memcpy(zip[0].words, input.orders + 4U, 10U);
    zip[0].valid = 1U; zip[0].name = (const StarsU8 *)"Zip route";
    input.zip_orders = zip;
    if (!stars_get_task_name(&input, output, sizeof output, &order) ||
        strcmp((char *)output, "Zip route") != 0) return 4;
    input.zip_orders = 0;
    input.orders[5] = 0x5002U;
    if (!stars_get_task_name(&input, output, sizeof output, &order) ||
        strcmp((char *)output, "Task 2") != 0 || order != 1) return 5;
    puts("TASK_NAME_SOURCE=PASS");
    return 0;
}
