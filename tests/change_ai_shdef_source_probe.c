#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct ChangeProbe {
    StarsU16 old_flags;
    StarsU16 set_flags;
    StarsI16 count_delta;
    int sets;
    int costs;
    int logs;
    StarsI16 logged_slot;
} ChangeProbe;

static StarsU16 old_flags(void *context, StarsI16 slot)
{
    ChangeProbe *probe = (ChangeProbe *)context;
    (void)slot;
    return probe->old_flags;
}

static void set_flags(void *context, StarsI16 slot, StarsU16 flags)
{
    ChangeProbe *probe = (ChangeProbe *)context;
    (void)slot;
    ++probe->sets;
    probe->set_flags = flags;
}

static void change_count(void *context, StarsI16 player, StarsI16 delta)
{
    ChangeProbe *probe = (ChangeProbe *)context;
    (void)player;
    probe->count_delta = (StarsI16)(probe->count_delta + delta);
}

static void update_cost(void *context, StarsAiShdefWork *work)
{
    ChangeProbe *probe = (ChangeProbe *)context;
    ++probe->costs;
    work->existing = 12UL;
}

static void log_change(void *context, StarsI16 slot,
    const StarsAiShdefWork *work)
{
    ChangeProbe *probe = (ChangeProbe *)context;
    ++probe->logs;
    probe->logged_slot = slot;
    (void)work;
}

int main(void)
{
    StarsAiShdefWork work;
    StarsChangeAiShdefOps ops;
    ChangeProbe probe;
    int failures = 0;

    memset(&work, 0, sizeof(work));
    memset(&probe, 0, sizeof(probe));
    ops.old_flags = old_flags;
    ops.set_flags = set_flags;
    ops.change_count = change_count;
    ops.update_cost = update_cost;
    ops.log_change = log_change;
    ops.player_design_count = 7;
    work.flags = 0x0200U;
    if (stars_f_change_ai_shdef(&work, 18, 33, &ops, &probe) != 1 ||
            probe.count_delta != -1 || probe.logs != 1 ||
            probe.logged_slot != 2 || probe.costs != 1 || work.turn != 33)
        ++failures;
    probe.old_flags = 0;
    probe.sets = 0;
    probe.logs = 0;
    work.flags = 0;
    if (stars_f_change_ai_shdef(&work, 2, 34, &ops, &probe) != 1 ||
            probe.sets != 1 || probe.logs != 2 ||
            (probe.set_flags & 0x0200U) == 0U)
        ++failures;
    printf("CHANGE_AI_SHDEF_CHECKS=2 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
