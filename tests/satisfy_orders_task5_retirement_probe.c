#include "core.h"
#include <stdio.h>
#include <string.h>

static int checks;
static int failures;
static int calls;

#define CHECK(expression) do { ++checks; if (!(expression)) ++failures; } while (0)

static StarsU16 word(const StarsU8 *data, StarsU16 offset)
{
    return (StarsU16)(data[offset] | ((StarsU16)data[offset + 1U] << 8));
}

static StarsI16 scrap(void *context, StarsSatisfyFleet *fleet,
    const StarsSatisfyOrder *order)
{
    (void)context;
    ++calls;
    CHECK(fleet->id == 0 && fleet->owner == 0 && fleet->planet_id == 10);
    CHECK(order->kind == 5 && order->target_id == 10 && order->active == 1);
    return 1;
}

int main(void)
{
    static const StarsU8 original_order[18] = {
        0x68, 0x04, 0x14, 0x05, 0x0a, 0x00, 0x05, 0x11,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    StarsU8 fleet[124];
    StarsU8 order[18];
    StarsSatisfyPackedInput input;
    StarsSatisfyOrdersOps ops;

    memset(fleet, 0, sizeof fleet);
    memcpy(order, original_order, sizeof order);
    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    fleet[4] = 0x07;
    fleet[5] = 0x09;
    fleet[118] = 0x60;
    input.fleet_record = fleet;
    input.fleet_size = sizeof fleet;
    input.order_record = order;
    input.order_size = sizeof order;
    input.planet_id = 10;
    input.pass = 2;
    input.ops = &ops;
    ops.scrap = scrap;

    CHECK(stars_satisfy_orders_packed(&input) == 1);
    CHECK(calls == 0 && (word(fleet, 4) & 0x0400U) == 0U &&
        (word(order, 6) & 0x1000U) != 0U);
    input.pass = 1;
    CHECK(stars_satisfy_orders_packed(&input) == 1);
    CHECK(calls == 1 && (word(fleet, 4) & 0x0400U) != 0U &&
        (word(order, 6) & 0x1000U) == 0U);
    input.pass = 3;
    CHECK(stars_satisfy_orders_packed(&input) == 1 && calls == 1);

    printf("TASK5_RETIREMENT_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures ? "FAIL" : "PASS");
    return failures ? 1 : 0;
}
