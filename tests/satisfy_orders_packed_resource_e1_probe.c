#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct E1Trace {
    StarsI16 transfers;
    StarsI16 messages;
    StarsI16 completed;
    StarsI16 battle_plans;
    StarsI16 stat_calls;
    StarsI16 stat_selector;
    StarsI16 last_code;
    StarsI16 last_resource;
    StarsI16 last_target;
    StarsI32 last_amount;
    StarsI32 target_limit;
} E1Trace;

static void put_word(StarsU8 *record, StarsU16 offset, StarsU16 value)
{
    record[offset] = (StarsU8)(value & 0xffU);
    record[offset + 1U] = (StarsU8)(value >> 8);
}

static void set_order(StarsU8 *record, StarsI16 kind, StarsI16 target_kind,
    StarsI16 active)
{
    StarsU16 value;

    value = (StarsU16)kind;
    value = (StarsU16)(value | (StarsU16)(target_kind << 8));
    if (active)
        value = (StarsU16)(value | 0x1000U);
    put_word(record, 6, value);
    put_word(record, 4, 9);
}

static StarsI32 target_capacity(void *context,
    const StarsSatisfyOrder *order, StarsI16 resource)
{
    E1Trace *trace;
    (void)order;
    (void)resource;
    trace = (E1Trace *)context;
    return trace->target_limit;
}

static StarsI32 transfer(void *context, const StarsSatisfyFleet *fleet,
    const StarsSatisfyOrder *order, StarsI16 resource, StarsI32 amount)
{
    E1Trace *trace;
    (void)fleet;
    (void)order;
    trace = (E1Trace *)context;
    ++trace->transfers;
    trace->last_resource = resource;
    trace->last_amount = amount;
    return amount;
}

static StarsI32 fleet_stat(void *context, const StarsSatisfyFleet *fleet,
    StarsI16 selector)
{
    E1Trace *trace;
    (void)fleet;
    trace = (E1Trace *)context;
    ++trace->stat_calls;
    trace->stat_selector = selector;
    return 50;
}

static void message(void *context, StarsI16 owner, StarsI16 code,
    StarsI16 target, StarsI16 resource, StarsI32 amount)
{
    E1Trace *trace;
    (void)owner;
    trace = (E1Trace *)context;
    ++trace->messages;
    trace->last_code = code;
    trace->last_target = target;
    trace->last_resource = resource;
    trace->last_amount = amount;
}

static void done(void *context, StarsSatisfyFleet *fleet,
    StarsSatisfyOrder *order, StarsI16 index)
{
    (void)fleet;
    (void)order;
    (void)index;
    ++((E1Trace *)context)->completed;
}

static StarsI16 battle_plan(void *context, StarsSatisfyFleet *fleet,
    const StarsSatisfyOrder *order)
{
    (void)fleet;
    (void)order;
    ++((E1Trace *)context)->battle_plans;
    return 1;
}

static void reset(E1Trace *trace, StarsU8 *fleet_record,
    StarsU8 *order_record, StarsSatisfyPackedInput *input)
{
    memset(trace, 0, sizeof *trace);
    memset(fleet_record, 0, 124);
    memset(order_record, 0, 18);
    memset(input, 0, sizeof *input);
    put_word(fleet_record, 0, 4);
    put_word(fleet_record, 2, 3);
    put_word(fleet_record, 118, 0x0060U);
}

static StarsI16 run(StarsU8 *fleet_record, StarsU8 *order_record,
    StarsSatisfyPackedInput *input, E1Trace *trace,
    StarsSatisfyOrdersOps *ops, StarsI16 kind, StarsI16 target_kind,
    StarsI16 pass)
{
    set_order(order_record, kind, target_kind, 1);
    input->fleet_record = fleet_record;
    input->fleet_size = 124;
    input->order_record = order_record;
    input->order_size = 18;
    input->pass = pass;
    input->target_owner = 3;
    input->target_present = 1;
    input->ops = ops;
    ops->context = trace;
    return stars_satisfy_orders_packed(input);
}

int main(void)
{
    StarsU8 fleet_record[124];
    StarsU8 order_record[18];
    StarsSatisfyPackedInput input;
    StarsSatisfyOrdersOps ops;
    E1Trace trace;
    int checks;
    int failures;

    memset(&ops, 0, sizeof ops);
    ops.target_capacity = target_capacity;
    ops.transfer = transfer;
    ops.fleet_stat = fleet_stat;
    ops.send_message = message;
    ops.order_done = done;
    ops.battle_plan = battle_plan;
    checks = 0;
    failures = 0;
#define CHECK(condition) do { ++checks; if (!(condition)) ++failures; } while (0)

    reset(&trace, fleet_record, order_record, &input);
    trace.target_limit = 4;
    input.cargo[STARS_SATISFY_RESOURCE_MINERAL_0] = 10;
    input.capacity[STARS_SATISFY_RESOURCE_MINERAL_0] = 100;
    input.requested[STARS_SATISFY_RESOURCE_MINERAL_0] = 6;
    CHECK(run(fleet_record, order_record, &input, &trace, &ops,
        STARS_SATISFY_ORDER_TRANSFER, STARS_SATISFY_TARGET_PLANET, 1) == 1);
    CHECK(input.cargo[0] == 6 && input.requested[0] == 2);
    CHECK(trace.transfers == 1 && trace.last_resource == 0 &&
        trace.last_amount == 4);
    CHECK(trace.messages == 1 && trace.last_code ==
        STARS_SATISFY_MESSAGE_TRANSFER && trace.last_target == 9);
    CHECK(input.fleet_record[118] == 0 && input.fleet_record[119] == 0);
    CHECK((input.order_record[7] & 0x10U) != 0U);

    reset(&trace, fleet_record, order_record, &input);
    trace.target_limit = 3;
    input.cargo[STARS_SATISFY_RESOURCE_COLONISTS] = 8;
    input.requested[STARS_SATISFY_RESOURCE_COLONISTS] = 4;
    CHECK(run(fleet_record, order_record, &input, &trace, &ops,
        STARS_SATISFY_ORDER_TRANSFER, STARS_SATISFY_TARGET_FLEET, 2) == 1);
    CHECK(input.cargo[3] == 5 && input.requested[3] == 1);
    CHECK(trace.messages == 1 && trace.last_code ==
        STARS_SATISFY_MESSAGE_FLEET_TRANSFER_COLONISTS &&
        trace.last_amount == 3);

    reset(&trace, fleet_record, order_record, &input);
    trace.target_limit = 100;
    input.cargo[STARS_SATISFY_RESOURCE_FUEL] = 5;
    input.requested[STARS_SATISFY_RESOURCE_FUEL] = 2;
    CHECK(run(fleet_record, order_record, &input, &trace, &ops,
        STARS_SATISFY_ORDER_TRANSFER, STARS_SATISFY_TARGET_THING, 2) == 1);
    CHECK(trace.transfers == 0 && trace.messages == 0 &&
        input.cargo[4] == 5 && input.requested[4] == 2);

    reset(&trace, fleet_record, order_record, &input);
    trace.target_limit = 100;
    input.requested[STARS_SATISFY_RESOURCE_MINERAL_0] = 2;
    CHECK(run(fleet_record, order_record, &input, &trace, &ops,
        STARS_SATISFY_ORDER_TRANSFER, STARS_SATISFY_TARGET_POINT, 4) == 1);
    CHECK(trace.transfers == 0 && trace.messages == 1 &&
        trace.last_code == STARS_SATISFY_MESSAGE_POINT_SHORTAGE &&
        trace.last_resource == 0);

    reset(&trace, fleet_record, order_record, &input);
    trace.target_limit = 100;
    input.cargo[0] = 10;
    input.requested[0] = 40;
    input.resource_mode[0] = STARS_SATISFY_RESOURCE_MODE_STAT_PERCENT;
    CHECK(run(fleet_record, order_record, &input, &trace, &ops,
        STARS_SATISFY_ORDER_TRANSFER, STARS_SATISFY_TARGET_PLANET, 2) == 1);
    CHECK(trace.stat_calls == 1 && trace.stat_selector == 2);
    CHECK(input.cargo[0] == 0 && input.requested[0] == 30 &&
        trace.last_amount == 10 && trace.completed == 0);
    CHECK((input.order_record[7] & 0x10U) != 0U);

    reset(&trace, fleet_record, order_record, &input);
    CHECK(run(fleet_record, order_record, &input, &trace, &ops,
        STARS_SATISFY_ORDER_BATTLE_PLAN, STARS_SATISFY_TARGET_FLEET, 1) == 1);
    CHECK(trace.battle_plans == 1 && trace.completed == 1 &&
        (input.order_record[7] & 0x10U) == 0U);

    reset(&trace, fleet_record, order_record, &input);
    set_order(order_record, STARS_SATISFY_ORDER_TRANSFER,
        STARS_SATISFY_TARGET_PLANET, 1);
    memset(&input, 0, sizeof input);
    input.fleet_record = fleet_record;
    input.fleet_size = 123;
    input.order_record = order_record;
    input.order_size = 18;
    input.ops = &ops;
    CHECK(stars_satisfy_orders_packed(&input) == 0 &&
        trace.transfers == 0 && trace.completed == 0);

    CHECK(checks == 21);
#ifndef TARGET_NATIVE
    printf("SATISFY_ORDERS_PACKED_RESOURCE_E1_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures ? "FAIL" : "PASS");
#else
    {
        FILE *report;
        report = fopen("C:\\SYOE6\\SYOE6.TXT", "w");
        if (!report)
            return 4;
        fprintf(report,
            "SATISFY_ORDERS_PACKED_RESOURCE_E1_CHECKS=%d FAILURES=%d RESULT=%s\n",
            checks, failures, failures ? "FAIL" : "PASS");
        fclose(report);
    }
#endif
    return failures ? 1 : 0;
#undef CHECK
}
