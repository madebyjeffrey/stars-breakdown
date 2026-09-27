#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct SplitProbe {
    int calls;
    StarsU16 moved;
    StarsU16 moved_second;
} SplitProbe;

static void split_fleet(void *context, StarsU8 *fleet,
    const StarsU16 *selected_counts)
{
    SplitProbe *probe = (SplitProbe *)context;
    (void)fleet;
    ++probe->calls;
    probe->moved = selected_counts[0];
    probe->moved_second = selected_counts[1];
}

static void put_word(StarsU8 *bytes, StarsU16 offset, StarsI16 value)
{
    StarsU16 bits = (StarsU16)value;
    bytes[offset] = (StarsU8)bits;
    bytes[offset + 1U] = (StarsU8)(bits >> 8);
}

int main(void)
{
    StarsU8 selected[16];
    StarsU8 fleet[48];
    StarsU8 *fleets[1];
    SplitProbe probe;
    int failures = 0;

    memset(selected, 0, sizeof(selected));
    memset(fleet, 0, sizeof(fleet));
    selected[0] = 1; selected[1] = 0;
    put_word(fleet, 2, 4); put_word(fleet, 12, 3); put_word(fleet, 14, 2);
    fleets[0] = fleet;
    probe.calls = 0; probe.moved = 0; probe.moved_second = 0;
    stars_split_out_shdefs(selected, fleets, 1, 4, 0, split_fleet, &probe);
    if (probe.calls != 1 || probe.moved != 3 || probe.moved_second != 0 ||
            fleet[12] != 0 || fleet[13] != 0 || fleet[14] != 2)
        ++failures;
    selected[1] = 0; probe.calls = 0;
    stars_split_out_shdefs(selected, fleets, 1, 4, 0, split_fleet, &probe);
    if (probe.calls != 0)
        ++failures;
    printf("SPLIT_OUT_SHDEFS_CHECKS=2 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
