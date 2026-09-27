#include "core.h"
#include <stdio.h>

typedef struct ShdefProbe {
    int hull_lookups;
    int part_lookups;
    int names;
    int publishes;
    StarsU16 hull_mode;
    StarsU16 hull_id;
    StarsI16 published_index;
    StarsU16 published_parts;
    StarsU16 published_group;
    StarsI16 name_base;
} ShdefProbe;

static StarsI16 lookup_hull(void *context, StarsU16 mode, StarsU16 hull_id,
    StarsAiShdefWork *work)
{
    ShdefProbe *probe = (ShdefProbe *)context;
    ++probe->hull_lookups;
    probe->hull_mode = mode;
    probe->hull_id = hull_id;
    work->part_count = 2;
    work->bitmap_id = 7;
    return 1;
}

static StarsI16 lookup_part(void *context, StarsU16 part_number,
    StarsAiPart *part)
{
    ShdefProbe *probe = (ShdefProbe *)context;
    static const StarsU8 component[2] = { 0x34, 0x12 };
    ++probe->part_lookups;
    part->tech_group = part_number;
    part->attributes = 3;
    part->component = component;
    return 1;
}

static void name_shdef(void *context, StarsAiShdefWork *work,
    StarsI16 name_base, StarsI16 name_length, StarsI16 bitmap_id)
{
    ShdefProbe *probe = (ShdefProbe *)context;
    ++probe->names;
    probe->name_base = name_base;
    work->bitmap_id = bitmap_id;
    (void)name_length;
}

static StarsI16 publish_shdef(void *context, StarsI16 index,
    const StarsAiShdefWork *work)
{
    ShdefProbe *probe = (ShdefProbe *)context;
    ++probe->publishes;
    probe->published_index = index;
    probe->published_parts = work->part_count;
    probe->published_group = work->part_groups[0];
    return 1;
}

int main(void)
{
    const StarsU8 parts[2] = { 0, 9 };
    ShdefProbe probe;
    int failures = 0;

    probe.hull_lookups = 0;
    probe.part_lookups = 0;
    probe.names = 0;
    probe.publishes = 0;
    if (stars_f_create_ai_shdef(-1, 0, parts, lookup_hull, lookup_part,
            name_shdef, publish_shdef, &probe) != 1 || probe.hull_mode != 0x400 ||
            probe.hull_id != 0 || probe.part_lookups != 2 ||
            probe.published_index != -1 || probe.published_parts != 2)
        ++failures;
    if (stars_f_create_ai_shdef(3, 0, parts, lookup_hull, lookup_part,
            name_shdef, publish_shdef, &probe) != 1 || probe.hull_mode != 0x4000 ||
            probe.published_index != 3 || probe.published_group != 0 ||
            probe.names != 1 || probe.name_base != 0x40a)
        ++failures;
    printf("CREATE_AI_SHDEF_CHECKS=2 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
