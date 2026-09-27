#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct ButtonProbe {
    int lookups;
    int enables;
    StarsU16 ids[2];
    StarsU16 states[2];
} ButtonProbe;

static StarsU16 get_item(void *context, StarsU16 hwnd, StarsU16 id)
{
    ButtonProbe *probe = (ButtonProbe *)context;
    (void)hwnd;
    probe->ids[probe->lookups++] = id;
    return (StarsU16)(id + 0x1000U);
}

static void enable(void *context, StarsU16 control, StarsU16 state)
{
    ButtonProbe *probe = (ButtonProbe *)context;
    (void)control;
    probe->states[probe->enables++] = state;
}

int main(void)
{
    StarsU8 records[5][0x28];
    StarsZipButtonOps ops;
    ButtonProbe probe;
    int failures = 0;
    memset(records, 0, sizeof records);
    records[1][0x0d] = 1;
    ops.get_item = get_item;
    ops.enable = enable;
    probe.lookups = probe.enables = 0;
    stars_enable_zip_prod_buttons(7, 1, &records[0][0], 5, &ops, &probe);
    if (probe.lookups != 2 || probe.enables != 2 || probe.ids[0] != 0x0817U ||
        probe.ids[1] != 0x041bU || probe.states[0] != 1 || probe.states[1] != 1)
        ++failures;
    probe.lookups = probe.enables = 0;
    stars_enable_zip_prod_buttons(7, 0, &records[0][0], 5, &ops, &probe);
    if (probe.states[0] != 0 || probe.states[1] != 0)
        ++failures;
    probe.lookups = probe.enables = 0;
    stars_enable_zip_prod_buttons(7, 5, &records[0][0], 5, &ops, &probe);
    if (probe.states[0] != 0 || probe.states[1] != 0)
        ++failures;
    printf("PRODUCE_ZIP_BUTTON_CHECKS=3 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
