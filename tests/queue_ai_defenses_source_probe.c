#include "core.h"
#include <stdio.h>

typedef struct DefenseProbe {
    int has_item;
    int adds;
    StarsI16 type;
    StarsI16 quantity;
} DefenseProbe;

static StarsI16 has_type(void *context, StarsI16 type)
{
    DefenseProbe *probe = (DefenseProbe *)context;
    (void)type;
    return (StarsI16)probe->has_item;
}

static void add_item(void *context, StarsI16 type, StarsI16 quantity)
{
    DefenseProbe *probe = (DefenseProbe *)context;
    ++probe->adds; probe->type = type; probe->quantity = quantity;
}

int main(void)
{
    DefenseProbe probe;
    int failures = 0;

    probe.has_item = 0; probe.adds = 0; probe.type = 0; probe.quantity = 0;
    if (stars_f_queue_ai_defenses(1600, 5, 9, 9, 9, has_type, add_item,
            &probe) != 1 || probe.adds != 1 || probe.quantity != 4)
        ++failures;
    probe.has_item = 1; probe.adds = 0;
    if (stars_f_queue_ai_defenses(1600, 5, 2, 9, 9, has_type, add_item,
            &probe) != 0 || probe.adds != 0)
        ++failures;
    if (stars_f_queue_ai_defenses(0, 0, 2, 9, 9, has_type, add_item,
            &probe) != 0)
        ++failures;
    probe.has_item = 0; probe.adds = 0;
    if (stars_f_queue_ai_defenses(1599, 0, 9, 9, 9, has_type, add_item,
            &probe) != 0 || probe.adds != 0)
        ++failures;
    if (stars_f_queue_ai_defenses(1600, 20, 9, 9, 9, has_type, add_item,
            &probe) != 0 || probe.adds != 0)
        ++failures;
    printf("QUEUE_AI_DEFENSES_CHECKS=5 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
