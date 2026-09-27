#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct NameProbe {
    int random_calls;
    int names;
    int formatted;
    StarsI16 bitmap;
    StarsI16 last_name;
} NameProbe;

static StarsI16 random_value(void *context, StarsI16 bound)
{
    NameProbe *probe = (NameProbe *)context;
    ++probe->random_calls;
    if (bound == 4)
        return 2;
    return 1;
}

static StarsI16 get_name(void *context, StarsI16 number, char *name,
    StarsU16 capacity)
{
    NameProbe *probe = (NameProbe *)context;
    ++probe->names;
    probe->last_name = number;
    if (capacity < 5U)
        return 0;
    strcpy(name, "ALPHA");
    return 1;
}

static StarsI16 name_in_use(void *context, StarsU16 family,
    const char *name)
{
    (void)context;
    (void)family;
    return strcmp(name, "ALPHA") == 0 ? 0 : 1;
}

static StarsI16 bitmap_in_use(void *context, StarsU16 bitmap, StarsU16 hull)
{
    (void)context;
    (void)bitmap;
    (void)hull;
    return 0;
}

static void format_name(void *context, char *name, StarsU16 capacity,
    StarsI16 number, StarsI16 suffix)
{
    NameProbe *probe = (NameProbe *)context;
    ++probe->formatted;
    if (capacity >= 8U)
        sprintf(name, "N%d-%d", number, suffix);
}

int main(void)
{
    StarsAiShdefWork work;
    StarsPickAiNameOps ops;
    NameProbe probe;

    memset(&work, 0, sizeof(work));
    memset(&probe, 0, sizeof(probe));
    ops.random_value = random_value;
    ops.get_name = get_name;
    ops.bitmap_in_use = bitmap_in_use;
    ops.name_in_use = name_in_use;
    ops.format_name = format_name;
    work.flags = (StarsU16)(3U << 10);
    stars_pick_ai_name_and_bitmap(&work, 100, 20, 40, &ops, &probe);
    printf("PICK_AI_NAME_CHECKS=2 FAILURES=%d RESULT=%s\n",
        (work.bitmap_id != 40 || probe.names != 1 ||
         strcmp(work.name, "ALPHA") != 0) ? 1 : 0,
        (work.bitmap_id == 40 && probe.names == 1 &&
         strcmp(work.name, "ALPHA") == 0) ? "PASS" : "FAIL");
    return (work.bitmap_id == 40 && probe.names == 1 &&
        strcmp(work.name, "ALPHA") == 0) ? 0 : 1;
}
