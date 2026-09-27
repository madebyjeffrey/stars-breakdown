#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct TechnologyTrace {
    StarsTechLearnPlayer player;
    StarsSatisfyTechnologyOutcome outcome;
    StarsU16 goto_id;
    StarsI16 mode;
    StarsI16 bind_calls;
    StarsI16 events;
    StarsI16 done;
} TechnologyTrace;

static StarsI32 transfer(void *context, const StarsSatisfyFleet *fleet,
    const StarsSatisfyOrder *order, StarsI16 resource, StarsI32 amount)
{
    (void)context; (void)fleet; (void)order; (void)resource;
    return amount;
}

static StarsI16 random_value(void *context, StarsI16 limit)
{
    TechnologyTrace *trace = (TechnologyTrace *)context;
    if (limit == 100)
        return 60;
    if (limit == 13)
        return 2;
    return trace->mode == 1 ? 0 : 0;
}

static StarsI16 give_part(void *context, StarsU32 mask, StarsI16 player,
    StarsU16 *part_id)
{
    (void)context; (void)mask; (void)player;
    *part_id = 42U;
    return 5;
}

static StarsI32 level_cost(void *context, StarsI16 technology,
    StarsI16 level, StarsI16 player)
{
    (void)context; (void)technology; (void)level; (void)player;
    return 100L;
}

static void mark_event(void *context, StarsI16 player)
{
    TechnologyTrace *trace = (TechnologyTrace *)context;
    (void)player;
    ++trace->events;
}

static StarsI16 bind_technology(void *context, StarsSatisfyFleet *fleet,
    const StarsSatisfyOrder *order, StarsI32 amount, StarsI16 negative_code,
    StarsI16 positive_code, StarsSatisfyTechnologyOutcome *outcome)
{
    TechnologyTrace *trace = (TechnologyTrace *)context;
    StarsTechLearnInput input;
    (void)order;
    memset(&input, 0, sizeof input);
    ++trace->bind_calls;
    input.player = &trace->player;
    input.player_id = fleet->owner;
    input.message_id = -1;
    input.trader_chance[2] = trace->mode == 0 ? 100U : 0U;
    input.battle_threshold[0] = 1U;
    input.goto_id = &trace->goto_id;
    input.random = random_value;
    input.give_trader_part = give_part;
    input.tech_level_cost = level_cost;
    input.mark_event = mark_event;
    input.context = trace;
    outcome->amount = amount;
    trace->outcome = *outcome;
    trace->outcome.result = stars_bind_satisfy_technology(&input,
        negative_code, positive_code, &trace->outcome);
    *outcome = trace->outcome;
    return trace->outcome.result;
}

static void done(void *context, StarsSatisfyFleet *fleet,
    StarsSatisfyOrder *order, StarsI16 index)
{
    TechnologyTrace *trace = (TechnologyTrace *)context;
    (void)fleet; (void)order; (void)index;
    ++trace->done;
}

static StarsI16 run_case(TechnologyTrace *trace, StarsI16 target_kind,
    StarsI16 mode, StarsI16 expected_result, StarsI16 expected_message,
    StarsI16 expected_id)
{
    StarsSatisfyFleet fleet;
    StarsSatisfyOrder order;
    StarsSatisfyOrdersOps ops;
    StarsSatisfyOrdersInput input;

    memset(&fleet, 0, sizeof fleet);
    memset(&order, 0, sizeof order);
    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    trace->mode = mode;
    trace->goto_id = 0U;
    fleet.owner = 3;
    fleet.cargo[0] = 7L;
    fleet.orders = &order;
    fleet.order_count = 1U;
    order.kind = STARS_SATISFY_ORDER_TRANSFER;
    order.target_kind = target_kind;
    order.target_id = 9;
    order.requested[0] = 7L;
    order.technology_flag = 1;
    order.phase_mask = 1 << 2;
    order.active = 1;
    ops.transfer = transfer;
    ops.technology_binding = bind_technology;
    ops.order_done = done;
    ops.context = trace;
    input.fleets = &fleet;
    input.fleet_count = 1U;
    input.pass = 2;
    input.ops = &ops;
    stars_satisfy_orders(&input);
    return trace->outcome.result == expected_result &&
        trace->outcome.message_code == expected_message &&
        trace->outcome.result_id == expected_id &&
        trace->outcome.amount == 7L && trace->bind_calls == 1 &&
        trace->events == 1 && trace->done == 1 && !order.active;
}

int satisfy_orders_technology_binding_e2_probe(void)
{
    TechnologyTrace trace;
    StarsI16 failures;

    memset(&trace, 0, sizeof trace);
    failures = !run_case(&trace, STARS_SATISFY_TARGET_POINT, 0, -3,
        0x013c, 2);
    memset(&trace, 0, sizeof trace);
    failures = failures || !run_case(&trace, STARS_SATISFY_TARGET_PLANET, 1,
        1, 0x013f, 0) || trace.player.research[0] != 100UL ||
        trace.goto_id != 0xfffeU;

#ifdef TARGET_NATIVE
    {
        FILE *report = fopen("SYO11.TXT", "w");
        if (report) {
            fprintf(report,
                "SATISFY_ORDERS_TECHNOLOGY_BINDING_E2_CHECKS=12 FAILURES=%d RESULT=%s\n",
                failures ? 1 : 0, failures ? "FAIL" : "PASS");
            fclose(report);
        }
    }
#else
    printf("SATISFY_ORDERS_TECHNOLOGY_BINDING_E2_CHECKS=12 FAILURES=%d RESULT=%s\n",
        failures ? 1 : 0, failures ? "FAIL" : "PASS");
#endif
    return failures ? 1 : 0;
}

#ifndef TARGET_NATIVE
int main(void)
{
    return satisfy_orders_technology_binding_e2_probe();
}
#endif
