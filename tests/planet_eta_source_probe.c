#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace { StarsU16 id; } Trace;
static void get_string(void *context, StarsU16 id, StarsU8 *output,
    StarsU16 capacity)
{
    Trace *trace = (Trace *)context;
    trace->id = id;
    if (capacity > 1U) strcpy((char *)output, id == 0x333U ? "now" : "never");
}
static void format(void *context, StarsU16 id, StarsI16 first, StarsI16 last,
    StarsU8 *output, StarsU16 capacity)
{
    Trace *trace = (Trace *)context;
    (void)capacity; trace->id = id;
    if (id == 0x330U) sprintf((char *)output, "%d-%d", first, last);
    else sprintf((char *)output, "%d year", first);
}
int main(void)
{
    StarsProductionEtaInput input;
    StarsProductionEtaOps ops;
    StarsU8 output[64];
    StarsI16 first, last;
    Trace trace;
    memset(&ops, 0, sizeof ops); ops.get_string = get_string; ops.format = format;
    memset(&trace, 0, sizeof trace); memset(&input, 0, sizeof input);
    input.eta_first = 100; input.item_index = 2; input.production_count = 4;
    input.item_phase = 1; input.item_stage = 6;
    if (!stars_psz_production_eta(&input, &first, &last, output, sizeof output,
            &ops, &trace) || trace.id != 0x254U) return 1;
    input.eta_first = 3; input.eta_last = 100;
    if (!stars_psz_production_eta(&input, 0, 0, output, sizeof output,
            &ops, &trace) || trace.id != 0x32fU || strcmp((char *)output, "3 year") != 0)
        return 2;
    input.eta_first = 2; input.eta_last = 2;
    if (!stars_psz_production_eta(&input, 0, 0, output, sizeof output,
            &ops, &trace) || strcmp((char *)output, "2 years") != 0) return 3;
    input.eta_first = 0; input.eta_last = 0;
    if (!stars_psz_production_eta(&input, 0, 0, output, sizeof output,
            &ops, &trace) || trace.id != 0x333U) return 4;
    input.eta_first = 2; input.eta_last = 5;
    if (!stars_psz_production_eta(&input, 0, 0, output, sizeof output,
            &ops, &trace) || trace.id != 0x330U || strcmp((char *)output, "2-5") != 0)
        return 5;
    puts("PLANET_ETA_SOURCE=PASS");
    return 0;
}
