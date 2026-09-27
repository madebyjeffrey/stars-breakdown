#include "core.h"
#include <string.h>

static void mark_tech_max(StarsU8 destination[6], const StarsU8 source[6])
{
    StarsU16 i;
    for (i = 0; i < 6U; ++i)
        if (destination[i] < source[i])
            destination[i] = source[i];
}

static StarsI16 trader_index(StarsU16 group, StarsU16 subtype)
{
    if (group == 1U && subtype == 8U) return 9;
    if (group == 4U && subtype == 6U) return 2;
    if (group == 8U && subtype == 9U) return 3;
    if (group == 0x10U && subtype == 0x12U) return 7;
    if (group == 0x20U && subtype == 7U) return 6;
    if (group == 0x40U && subtype == 8U) return 5;
    if (group == 0x80U && subtype == 6U) return 4;
    if (group == 0x800U && subtype == 4U) return 1;
    if (group == 0x1000U && subtype == 4U) return 0;
    if (group == 0x1000U && subtype == 9U) return 11;
    return -1;
}

void stars_mark_techs_seen(StarsMarkTechsInput *input)
{
    StarsU8 tech[6];
    StarsU16 i;
    StarsU16 subtype;
    StarsI16 index;

    if (!input || !input->hull || !input->lookup)
        return;
    if (input->lookup(input->context, 0x4000U,
            (StarsU16)(input->hull->hull_id & 0xffU), tech) != 0)
        mark_tech_max(input->tech_battle, tech);
    for (i = 0; i < input->hull->part_count; ++i) {
        const StarsMarkTechPart *part = &input->hull->parts[i];
        if ((part->packed_count_type >> 8) == 0U)
            continue;
        subtype = (StarsU16)(part->packed_count_type & 0xffU);
        if (input->lookup(input->context, part->group, subtype, tech) != 0)
            mark_tech_max(input->tech_battle, tech);
        index = trader_index(part->group, subtype);
        if (index >= 0 && input->tech_trader[index] < 0x19U) {
            input->tech_trader[index] = (StarsU8)(input->tech_trader[index] +
                subtype);
            if (input->tech_trader[index] > 0x19U)
                input->tech_trader[index] = 0x19U;
        }
    }
}

StarsI16 stars_mark_techs_seen_shdef(const StarsU8 *design, StarsU16 bytes,
    StarsMarkTechsInput *input)
{
    StarsMarkTechPart parts[16];
    StarsMarkTechHull hull;
    StarsMarkTechsInput work;
    StarsU16 count;
    StarsU16 index;
    StarsU16 offset;

    if (!design || bytes < 147U || !input || !input->lookup)
        return 0;
    count = design[0x7aU];
    if (count > 16U)
        return 0;
    hull.hull_id = (StarsU16)(design[0] |
        ((StarsU16)design[1] << 8));
    hull.parts = parts;
    hull.part_count = count;
    for (index = 0; index < count; ++index) {
        offset = (StarsU16)(0x3aU + index * 4U);
        parts[index].group = (StarsU16)(design[offset] |
            ((StarsU16)design[offset + 1U] << 8));
        parts[index].packed_count_type = (StarsU16)(design[offset + 2U] |
            ((StarsU16)design[offset + 3U] << 8));
    }
    work = *input;
    work.hull = &hull;
    stars_mark_techs_seen(&work);
    memcpy(input->tech_battle, work.tech_battle, sizeof input->tech_battle);
    memcpy(input->tech_trader, work.tech_trader, sizeof input->tech_trader);
    return 1;
}
