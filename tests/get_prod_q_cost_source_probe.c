#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct ProdProbe {
    int calls;
    StarsI16 player;
    StarsI16 only_one;
    const StarsU8 *first_item;
} ProdProbe;

static const StarsU8 *get_queue(void *context, const StarsU8 *planet,
    StarsU16 *count)
{
    (void)context;
    (void)planet;
    *count = 2;
    return (const StarsU8 *)0x1000;
}

static void get_costs(void *context, const StarsU8 *planet,
    const StarsU8 *item, StarsI32 *costs, StarsI16 player, StarsI16 only_one)
{
    ProdProbe *probe = (ProdProbe *)context;
    (void)planet;
    if (probe->calls == 0) probe->first_item = item;
    probe->player = player;
    probe->only_one = only_one;
    ++probe->calls;
    costs[0] = item == (const StarsU8 *)0x1004 ? 10L : 20L;
    costs[1] = -2L;
    costs[2] = 30L;
    costs[3] = 40L;
}

int main(void)
{
    StarsU8 planet[56];
    StarsI32 costs[4];
    StarsGetProdQCostOps ops;
    ProdProbe probe;
    int failures = 0;

    memset(planet, 0, sizeof(planet));
    memset(&probe, 0, sizeof(probe));
    ops.get_queue = get_queue;
    ops.get_costs = get_costs;
    stars_get_prod_q_cost(planet, costs, 5, &ops, &probe);
    if (costs[0] != 30L || costs[1] != -4L || costs[2] != 60L ||
        costs[3] != 80L || probe.calls != 2 || probe.first_item !=
        (const StarsU8 *)0x1004 || probe.player != 5 || probe.only_one != 0)
        ++failures;
    printf("GET_PROD_Q_COST_CHECKS=1 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
