#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 calls;
    StarsI16 cost_calls;
    StarsI16 kind[4];
    StarsU16 quantity[4];
    StarsI16 flags[4];
} Trace;

static StarsU32 item_word(StarsU16 kind, StarsU16 quantity)
{
    return (StarsU32)(1UL << 17) | ((StarsU32)kind << 10) | quantity;
}

static void get_costs(void *context, StarsU32 item, StarsI32 costs[4])
{
    StarsU16 kind = (StarsU16)((item >> 10) & 0x7fU);
    ++((Trace *)context)->cost_calls;
    memset(costs, 0, sizeof(StarsI32) * 4U);
    if (kind == 7U) costs[0] = 10L;
    else if (kind == 8U) costs[1] = 10L;
    else if (kind == 11U) costs[2] = 10L;
}

static StarsI16 add_item(void *context, StarsI16 kind, StarsU16 quantity,
    StarsI16 build_now, StarsI16 flags)
{
    Trace *trace = (Trace *)context;
    (void)build_now;
    if (trace->calls < 4) {
        trace->kind[trace->calls] = kind;
        trace->quantity[trace->calls] = quantity;
        trace->flags[trace->calls] = flags;
    }
    ++trace->calls;
    return 1;
}

static void initialize(StarsFillProdMinesAndFactoriesInput *input,
    Trace *trace)
{
    memset(input, 0, sizeof *input);
    memset(trace, 0, sizeof *trace);
    input->available[0] = 100L;
    input->available[1] = 100L;
    input->available[2] = 100L;
    input->available[3] = 100L;
    input->max_factories = 2;
    input->max_mines = 3;
    input->get_costs = get_costs;
    input->add_item = add_item;
    input->context = trace;
}

int main(void)
{
    StarsFillProdMinesAndFactoriesInput input;
    Trace trace;
    StarsU32 planet_items[3];
    StarsU32 global_items[3];
    int failures = 0;

    initialize(&input, &trace);
    global_items[0] = item_word(7U, 1U);
    global_items[1] = item_word(8U, 1U);
    global_items[2] = item_word(11U, 1U);
    input.global_queue_items = global_items;
    input.global_queue_count = 3;
    input.turn = 50;
    if (!stars_fill_prod_mines_and_factories(&input) || trace.calls != 2 ||
            trace.kind[0] != 7 || trace.quantity[0] != 2 ||
            trace.kind[1] != 8 || trace.quantity[1] != 3)
        ++failures;

    initialize(&input, &trace);
    planet_items[0] = item_word(12U, 4U);
    input.planet_queue_items = planet_items;
    input.planet_queue_count = 1;
    input.research_type = 5;
    input.available[3] = 139L;
    if (!stars_fill_prod_mines_and_factories(&input) || trace.calls != 1 ||
            trace.kind[0] != 12 || trace.quantity[0] != 2)
        ++failures;

    initialize(&input, &trace);
    input.current_order = item_word(11U, 1U);
    input.available[0] = -1L;
    if (stars_fill_prod_mines_and_factories(&input) || trace.calls != 0)
        ++failures;

    initialize(&input, &trace);
    input.max_factories = 0;
    input.max_mines = 0;
    input.turn = 101;
    input.available[2] = 20L;
    input.technology[0] = 0x1a;
    input.technology[1] = 0x1a;
    input.technology[2] = 0x1a;
    input.technology[3] = 0x1a;
    input.technology[4] = 0x1a;
    input.technology[5] = 0x1a;
    global_items[0] = item_word(11U, 1U);
    input.global_queue_items = global_items;
    input.global_queue_count = 1;
    if (!stars_fill_prod_mines_and_factories(&input) || trace.calls != 1 ||
            trace.kind[0] != 11 || trace.quantity[0] != 3)
        ++failures;

    /* A negative resource balance stops before costs or additions, even
     * when mineral-only factory/mine costs would otherwise be affordable. */
    initialize(&input, &trace);
    global_items[0] = item_word(7U, 1U);
    global_items[1] = item_word(8U, 1U);
    input.global_queue_items = global_items;
    input.global_queue_count = 2;
    input.queued_cost[3] = input.available[3] + 1L;
    if (stars_fill_prod_mines_and_factories(&input) || trace.calls != 0 ||
            trace.cost_calls != 0)
        ++failures;

    /* Zero is not negative: retain the original boundary distinction. */
    memset(&trace, 0, sizeof trace);
    input.queued_cost[3] = input.available[3];
    if (!stars_fill_prod_mines_and_factories(&input) || trace.calls != 2 ||
            trace.cost_calls != 2)
        ++failures;

#ifndef TARGET_NATIVE
    printf("FILL_PROD_MF_CHECKS=6 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
#endif
    return failures != 0;
}
