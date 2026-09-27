#include "core.h"
#include <stdio.h>

typedef struct ProbeContext {
    StarsFleetBalanceFleet snapshots[2];
} ProbeContext;

static StarsI16 probe_lookup(void *opaque, StarsI16 id,
    StarsFleetBalanceFleet *out)
{
    ProbeContext *context = (ProbeContext *)opaque;
    if (id < 0 || id > 1 || !out) return 0;
    *out = context->snapshots[id];
    return 1;
}

static StarsI16 probe_weight(void *opaque, StarsI16 player, StarsI16 design,
    StarsI16 stat)
{
    (void)opaque;
    (void)player;
    (void)design;
    return stat == 1 ? 10 : 4;
}

static void probe_clear(StarsFleetBalanceFleet *fleet, StarsI16 player)
{
    StarsI16 i;
    fleet->player = player;
    fleet->flags = 0U;
    for (i = 0; i < STARS_FLEET_BALANCE_DESIGNS; ++i) {
        fleet->ship_count[i] = 0;
        fleet->ship_status[i] = 0U;
    }
    for (i = 0; i < STARS_FLEET_BALANCE_RESOURCES; ++i)
        fleet->cargo[i] = 0L;
}

static StarsI16 probe_expect(StarsI16 condition)
{
    return condition ? 0 : 1;
}

int main(void)
{
    ProbeContext context;
    StarsFleetBalanceFleet new_fleets[2];
    StarsFleetTransferCargoBalanceInput input;
    StarsI32 before;
    StarsI32 after;
    StarsI16 failures;
    StarsI16 i;

    probe_clear(&context.snapshots[0], 3);
    probe_clear(&context.snapshots[1], 3);
    probe_clear(&new_fleets[0], 3);
    probe_clear(&new_fleets[1], 3);

    context.snapshots[0].ship_count[0] = 10;
    context.snapshots[0].ship_status[0] = 20U;
    context.snapshots[1].ship_count[0] = 10;
    context.snapshots[1].ship_status[0] = 50U;
    new_fleets[0].ship_count[0] = 8;
    new_fleets[1].ship_count[0] = 12;
    new_fleets[0].cargo[0] = 20L;
    new_fleets[0].cargo[1] = 15L;
    new_fleets[0].cargo[4] = 30L;
    new_fleets[1].cargo[0] = 4L;
    new_fleets[1].cargo[1] = 5L;
    new_fleets[1].cargo[4] = 7L;

    before = 0L;
    for (i = 0; i < STARS_FLEET_BALANCE_RESOURCES; ++i)
        before += new_fleets[0].cargo[i] + new_fleets[1].cargo[i];
    input.new_fleets[0] = &new_fleets[0];
    input.new_fleets[1] = &new_fleets[1];
    input.fleet_ids[0] = 0;
    input.fleet_ids[1] = 1;
    input.player = 3;
    input.lookup_fleet = probe_lookup;
    input.max_weight = probe_weight;
    input.context = &context;
    stars_fleet_transfer_cargo_balance(&input);
    after = 0L;
    for (i = 0; i < STARS_FLEET_BALANCE_RESOURCES; ++i)
        after += new_fleets[0].cargo[i] + new_fleets[1].cargo[i];

    failures = 0;
    failures += probe_expect(before == after);
    failures += probe_expect(new_fleets[0].cargo[4] < 30L);
    failures += probe_expect(new_fleets[1].cargo[4] > 7L);
    failures += probe_expect((new_fleets[0].ship_status[0] &
        STARS_FLEET_BALANCE_STATUS_MASK) != 20U);

    probe_clear(&new_fleets[0], 3);
    probe_clear(&new_fleets[1], 3);
    new_fleets[0].flags = STARS_FLEET_BALANCE_SPECIAL_FLAG;
    new_fleets[0].ship_count[0] = 3;
    new_fleets[1].ship_count[0] = 3;
    new_fleets[0].ship_status[0] = 17U;
    new_fleets[1].ship_status[0] = 29U;
    stars_fleet_transfer_cargo_balance(&input);
    failures += probe_expect(new_fleets[0].ship_status[0] == 17U);
    failures += probe_expect(new_fleets[1].ship_status[0] == 29U);

    printf("FLEET_BALANCE_CHECKS=6 FAILURES=%d RESULT=%s\n", (int)failures,
        failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
}
