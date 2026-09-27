#include "core.h"
#include <stdio.h>

typedef struct MaidProbe {
    int ensure_calls;
    int task_calls;
    int fill_calls;
    StarsI16 ensure_level;
    StarsI16 task_level;
    StarsI16 task_player;
    void *task_production;
} MaidProbe;

static StarsI16 ensure_ai(void *context, StarsI16 level)
{
    MaidProbe *probe = (MaidProbe *)context;
    ++probe->ensure_calls;
    probe->ensure_level = level;
    return (StarsI16)(level + 2);
}

static void handle_tasks(void *context, StarsI16 level, void *production,
    StarsI16 player, StarsI32 *first, StarsI32 *second)
{
    MaidProbe *probe = (MaidProbe *)context;
    ++probe->task_calls;
    probe->task_level = level;
    probe->task_player = player;
    probe->task_production = production;
    first[0] = 1;
    second[0] = 2;
}

static void fill_queue(void *context)
{
    ++((MaidProbe *)context)->fill_calls;
}

int main(void)
{
    MaidProbe probe;
    StarsMaidAiOps ops;
    StarsI16 marked = 9;
    int production;
    int failures = 0;
    probe.ensure_calls = probe.task_calls = probe.fill_calls = 0;
    probe.ensure_level = probe.task_level = probe.task_player = 0;
    probe.task_production = 0;
    ops.ensure_ai = ensure_ai;
    ops.handle_tasks = handle_tasks;
    ops.fill_queue = fill_queue;
    stars_do_maid_ai_turn(19, &production, &marked, &ops, &probe);
    if (marked != 0 || probe.ensure_calls != 1 || probe.ensure_level != 0 ||
        probe.task_calls != 1 || probe.task_level != 2 ||
        probe.task_player != -1 || probe.task_production != &production ||
        probe.fill_calls != 1)
        ++failures;
    probe.ensure_calls = probe.task_calls = probe.fill_calls = 0;
    marked = 7;
    stars_do_maid_ai_turn(20, &production, &marked, &ops, &probe);
    if (marked != 0 || probe.ensure_level != 15 || probe.task_level != 17 ||
        probe.task_player != -1 || probe.fill_calls != 1)
        ++failures;
    printf("AI_MAID_CHECKS=2 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
