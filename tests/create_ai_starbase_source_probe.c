#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct StarbaseProbe {
    int creates;
    int changes;
    StarsI16 create_ishdef;
    StarsI16 create_isb;
    StarsI16 create_family;
    StarsI16 changed_index;
    StarsU16 first_tech;
    StarsU16 changed_flags;
    StarsI16 changed_bitmap;
    char changed_name[32];
    StarsI16 random_calls;
    StarsI16 random_limits[2];
    StarsI16 random_values[2];
} StarbaseProbe;

static StarsI16 create_shdef(void *context, StarsI16 ishdef,
    StarsI16 isb, StarsI16 family, StarsAiStarbaseWork *work)
{
    StarbaseProbe *probe = (StarbaseProbe *)context;
    ++probe->creates;
    probe->create_ishdef = ishdef;
    probe->create_isb = isb;
    probe->create_family = family;
    work->hull_id = 7U;
    work->flags = 0x0200U;
    work->bitmap_id = 40;
    work->name[0] = '\0';
    work->component_count = 2;
    work->groups[0] = 0x0200U;
    work->groups[1] = 1;
    work->tech_levels[0] = 2;
    work->tech_levels[1] = 5;
    return 1;
}

static void change_shdef(void *context, StarsI16 index,
    const StarsAiStarbaseWork *work)
{
    StarbaseProbe *probe = (StarbaseProbe *)context;
    ++probe->changes;
    probe->changed_index = index;
    probe->first_tech = work->tech_levels[0];
    probe->changed_flags = work->flags;
    probe->changed_bitmap = work->bitmap_id;
    memcpy(probe->changed_name, work->name, sizeof(probe->changed_name));
}

static StarsI16 random_name(void *context, StarsI16 limit)
{
    StarbaseProbe *probe = (StarbaseProbe *)context;
    StarsI16 index = probe->random_calls;
    if (index >= 2)
        return 0;
    probe->random_limits[index] = limit;
    ++probe->random_calls;
    return probe->random_values[index];
}

static StarsI16 get_name(void *context, StarsI16 string_id, char *out,
    StarsU16 capacity)
{
    (void)context;
    if (capacity < 6U)
        return 0;
    sprintf(out, "N%04X", (unsigned)string_id);
    return 1;
}

static StarsI16 no_bitmap_collision(void *context, StarsU16 bitmap,
    StarsU16 hull)
{
    (void)context;
    (void)bitmap;
    (void)hull;
    return 0;
}

static StarsI16 no_name_collision(void *context, StarsU16 design_class,
    const char *name)
{
    (void)context;
    (void)design_class;
    (void)name;
    return 0;
}

int main(void)
{
    StarsAiStarbaseWork work;
    StarsPickAiNameOps name_ops;
    StarbaseProbe probe;
    int failures = 0;

    memset(&probe, 0, sizeof(probe));
    probe.random_values[0] = 4;
    probe.random_values[1] = 9;
    name_ops.random_value = random_name;
    name_ops.get_name = get_name;
    name_ops.bitmap_in_use = no_bitmap_collision;
    name_ops.name_in_use = no_name_collision;
    name_ops.format_name = 0;
    if (stars_f_create_ai_starbase(1, 1, -1, -1, 0, &work, create_shdef,
            change_shdef, &name_ops, &probe) != 1 ||
            probe.create_ishdef != -1 || probe.create_isb != 0 ||
            probe.create_family != 12 || probe.changed_index != 17 ||
            probe.first_tech != 1 || probe.changed_flags != 0x4600U ||
            probe.changed_bitmap != 40 ||
            strcmp(probe.changed_name, "N0436") != 0)
        ++failures;
    if (stars_f_create_ai_starbase(2, 1, -1, -1, 0, &work, create_shdef,
            change_shdef, &name_ops, &probe) != 1 ||
            probe.create_isb != 2 || probe.create_family != 0 ||
            probe.changed_index != 18 || probe.changed_flags != 0x4a00U ||
            probe.changed_bitmap != 40 ||
            strcmp(probe.changed_name, "N043B") != 0)
        ++failures;
    if (probe.random_calls != 2 || probe.random_limits[0] != 13 ||
            probe.random_limits[1] != 13)
        ++failures;
    printf("CREATE_AI_STARBASE_CHECKS=3 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
