#include <stdio.h>
#include <string.h>
#include "core.h"

static StarsI16 lookup(void *context, StarsU16 group, StarsU16 subtype,
        StarsU8 tech[6])
{
    StarsU16 i;
    (void)context;
    for (i = 0; i < 6U; ++i)
        tech[i] = (StarsU8)((group == 0x4000U ? 1U : 3U) + i);
    if (group == 0x800U && subtype == 4U)
        tech[0] = 20U;
    return 1;
}

int main(void)
{
    StarsMarkTechPart parts[4];
    StarsMarkTechHull hull;
    StarsMarkTechsInput input;
    StarsMarkTechsInput raw;
    StarsU8 design[147];
    StarsU16 i;
    memset(&input, 0, sizeof(input));
    input.tech_battle[0] = 10U;
    input.tech_trader[1] = 20U;
    input.hull = &hull;
    input.lookup = lookup;
    hull.hull_id = 7U;
    hull.parts = parts;
    hull.part_count = 4U;
    parts[0].group = 0x800U;
    parts[0].packed_count_type = 0x0104U;
    parts[1].group = 0x1000U;
    parts[1].packed_count_type = 0x0109U;
    parts[2].group = 4U;
    parts[2].packed_count_type = 0x0006U;
    parts[3].group = 0x20U;
    parts[3].packed_count_type = 0x0107U;
    stars_mark_techs_seen(&input);
    if (input.tech_battle[0] != 20U || input.tech_battle[5] != 8U)
        return 1;
    if (input.tech_trader[1] != 24U || input.tech_trader[11] != 9U ||
            input.tech_trader[6] != 7U || input.tech_trader[2] != 0U)
        return 1;
    memset(&raw, 0, sizeof raw);
    memset(design, 0, sizeof design);
    raw.tech_battle[0] = 10U;
    raw.tech_trader[1] = 20U;
    raw.lookup = lookup;
    design[0] = 7U;
    design[0x7aU] = 4U;
    for (i = 0; i < 4U; ++i) {
        StarsU16 offset = (StarsU16)(0x3aU + 4U * i);
        design[offset] = (StarsU8)parts[i].group;
        design[offset + 1U] = (StarsU8)(parts[i].group >> 8);
        design[offset + 2U] = (StarsU8)parts[i].packed_count_type;
        design[offset + 3U] = (StarsU8)(parts[i].packed_count_type >> 8);
    }
    if (!stars_mark_techs_seen_shdef(design, sizeof design, &raw) ||
            memcmp(raw.tech_battle, input.tech_battle,
                sizeof raw.tech_battle) != 0 ||
            memcmp(raw.tech_trader, input.tech_trader,
                sizeof raw.tech_trader) != 0)
        return 1;
    if (stars_mark_techs_seen_shdef(design, 146U, &raw))
        return 1;
    design[0x7aU] = 17U;
    if (stars_mark_techs_seen_shdef(design, sizeof design, &raw))
        return 1;
    input.tech_trader[11] = 24U;
    stars_mark_techs_seen(&input);
    if (input.tech_trader[11] != 25U)
        return 1;
    puts("MARK_TECHS_SOURCE=PASS");
    return 0;
}
