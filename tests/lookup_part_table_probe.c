#include <stdio.h>
#include <string.h>
#include "common.h"

typedef struct Case {
    StarsU16 group;
    StarsU16 item;
    StarsU16 base;
    StarsU16 stride;
    StarsU16 segment;
    StarsU16 segment_is_current_cs;
} Case;

static StarsU32 resolve(void *context, StarsU16 group, StarsU16 item)
{
    (void)context;
    return ((StarsU32)group << 16) | item;
}

static StarsI16 tech_status(void *context, StarsU32 component)
{
    (void)context;
    (void)component;
    return 1;
}

int main(void)
{
    static const Case cases[] = {
        { 0x0001U, 3U, 0x0000U, 0x004eU, 0U, 1U },
        { 0x0002U, 4U, 0x0768U, 0x0038U, 0U, 1U },
        { 0x0004U, 5U, 0x0ae8U, 0x0036U, 0U, 1U },
        { 0x0008U, 3U, 0x04e0U, 0x0036U, 0U, 1U },
        { 0x0010U, 7U, 0x2450U, 0x003cU, 0U, 1U },
        { 0x0020U, 7U, 0x2180U, 0x003cU, 0U, 1U },
        { 0x0040U, 8U, 0x1e1aU, 0x003aU, 0U, 1U },
        { 0x0080U, 5U, 0x1508U, 0x0036U, 0U, 1U },
        { 0x0100U, 2U, 0x12ecU, 0x0036U, 0U, 1U },
        { 0x0200U, 15U, 0x4d8aU, 0x0038U, 0U, 1U },
        { 0x0400U, 4U, 0x4872U, 0x008fU, 0U, 1U },
        { 0x0800U, 9U, 0x0d04U, 0x0036U, 0U, 1U },
        { 0x1000U, 4U, 0x109aU, 0x0036U, 0U, 1U },
        { 0x2000U, 7U, 0x19e2U, 0x0036U, 0U, 1U },
        { 0x4000U, 20U, 0x29f0U, 0x008fU, 0U, 1U },
        { 0x8000U, 14U, 0x16b8U, 0x0036U, 0U, 1U }
    };
    StarsLookupPartInput input;
    StarsI16 failures;
    StarsU16 index;

    failures = 0;
    for (index = 0; index < (StarsU16)(sizeof cases / sizeof cases[0]);
            ++index) {
        memset(&input, 0, sizeof input);
        input.group = cases[index].group;
        input.item = cases[index].item;
        input.player = -1;
        input.resolve = resolve;
        input.tech_status = tech_status;
        if (stars_lookup_part(&input) != 1 ||
                input.table_segment != cases[index].segment ||
                input.table_segment_is_current_cs !=
                    cases[index].segment_is_current_cs ||
                input.table_base_offset != cases[index].base ||
                input.table_stride != cases[index].stride ||
                input.table_offset != (StarsU16)(cases[index].base +
                    cases[index].item * cases[index].stride) ||
                input.component != (((StarsU32)cases[index].group << 16) |
                    cases[index].item))
            ++failures;
    }
    memset(&input, 0, sizeof input);
    input.group = 0x1234U;
    input.item = 2U;
    input.player = -1;
    input.resolve = resolve;
    input.tech_status = tech_status;
    if (stars_lookup_part(&input) != 0 || input.table_segment != 0U ||
            input.table_base_offset != 0U ||
            input.table_offset != 0U || input.table_stride != 0U)
        ++failures;
#ifndef TARGET_NATIVE
    printf("LOOKUP_PART_TABLE_CHECKS=66 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
#endif
    return failures != 0;
}
