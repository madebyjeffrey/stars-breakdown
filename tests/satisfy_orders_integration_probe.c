#include <stdio.h>
#include <string.h>
#include "core.h"

/* Recovered packed boundaries: _fleet is 124 bytes and _order is 18 bytes.
 * Resource vectors are turn-engine state derived from the same fleet record;
 * the adapter keeps that state typed while round-tripping the identity,
 * flags, order target, object-kind nibble, and validity bit. */
typedef struct RawSatisfy {
    unsigned char fleet[124];
    unsigned char order[18];
    StarsI16 transfers;
    StarsI32 last_amount;
    StarsI16 done;
} RawSatisfy;

static StarsU16 get_word(const unsigned char *p, StarsI16 offset)
{
    return (StarsU16)((StarsU16)p[offset] |
        ((StarsU16)p[offset + 1] << 8));
}

static void put_word(unsigned char *p, StarsI16 offset, StarsU16 value)
{
    p[offset] = (unsigned char)(value & 0xffU);
    p[offset + 1] = (unsigned char)(value >> 8);
}

static StarsI32 transfer(void *context, const StarsSatisfyFleet *fleet,
    const StarsSatisfyOrder *order, StarsI16 resource, StarsI32 amount)
{
    RawSatisfy *raw = (RawSatisfy *)context;
    (void)fleet;
    (void)order;
    (void)resource;
    ++raw->transfers;
    raw->last_amount = amount;
    return amount;
}

static void order_done(void *context, StarsSatisfyFleet *fleet,
    StarsSatisfyOrder *order, StarsI16 index)
{
    RawSatisfy *raw = (RawSatisfy *)context;
    (void)fleet;
    (void)order;
    (void)index;
    ++raw->done;
}

static StarsI16 decode_and_run(RawSatisfy *raw, StarsI16 pass,
    StarsI16 active, StarsI16 phase_mask)
{
    StarsSatisfyPackedInput input;
    StarsSatisfyOrdersOps ops;
    StarsU16 packed;

    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    (void)active;
    input.fleet_record = raw->fleet;
    input.fleet_size = sizeof raw->fleet;
    input.order_record = raw->order;
    input.order_size = sizeof raw->order;
    input.cargo[STARS_SATISFY_RESOURCE_MINERAL_0] = 7L;
    input.capacity[STARS_SATISFY_RESOURCE_MINERAL_0] = 10L;
    input.requested[STARS_SATISFY_RESOURCE_MINERAL_0] = 5L;
    input.phase_mask = phase_mask;
    input.pass = pass;
    input.target_owner = 1;
    input.target_present = 1;
    ops.transfer = transfer;
    ops.order_done = order_done;
    ops.context = raw;
    input.ops = &ops;
    packed = get_word(raw->order, 6);
    if (active)
        packed = (StarsU16)(packed | (1U << 12));
    else
        packed = (StarsU16)(packed & (StarsU16)~(StarsU16)(1U << 12));
    put_word(raw->order, 6, packed);
    if (!stars_satisfy_orders_packed(&input))
        return 0;
    packed = get_word(raw->order, 6);
    return (StarsI16)(input.cargo[0] == 2L &&
        input.requested[0] == 0L && (packed & (1U << 12)) == 0U &&
        get_word(raw->order, 4) == 42U);
}

int main(void)
{
    RawSatisfy raw;
    StarsU16 packed;
    int failures;

    memset(&raw, 0, sizeof raw);
    put_word(raw.fleet, 0, 7U);
    put_word(raw.fleet, 2, 0U);
    put_word(raw.fleet, 4, 0U);
    put_word(raw.fleet, 118, 0x0060U);
    put_word(raw.order, 0, 3U);
    put_word(raw.order, 2, 4U);
    put_word(raw.order, 4, 42U);
    packed = (StarsU16)(STARS_SATISFY_ORDER_TRANSFER |
        (STARS_SATISFY_TARGET_FLEET << 8) | (1U << 12));
    put_word(raw.order, 6, packed);
    failures = !decode_and_run(&raw, 2, 1, (StarsI16)(1 << 2));
    if (raw.transfers != 1 || raw.last_amount != 5L || raw.done != 1 ||
            (get_word(raw.order, 6) & (1U << 12)) != 0U)
        failures = 1;

    put_word(raw.fleet, 118, 0x0060U);
    decode_and_run(&raw, 1, 0, 0);
    if (get_word(raw.fleet, 118) != 0U)
        failures = 1;

    put_word(raw.fleet, 4, 0x0400U);
    put_word(raw.order, 6, (StarsU16)(STARS_SATISFY_ORDER_TRANSFER |
        (STARS_SATISFY_TARGET_FLEET << 8) | (1U << 12)));
    decode_and_run(&raw, 2, 1, (StarsI16)(1 << 2));
    if (raw.transfers != 1 ||
            (get_word(raw.order, 6) & (1U << 12)) == 0U)
        failures = 1;

#ifndef INTEGRATION_NATIVE
    printf("SATISFY_ORDERS_INTEGRATION_CHECKS=3 FAILURES=%d RESULT=%s\n",
        failures, failures ? "FAIL" : "PASS");
#endif
    return failures;
}
