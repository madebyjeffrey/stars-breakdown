#include "core.h"
#include "TURINSUP.H"
#include <stdio.h>
#include <string.h>

typedef struct TurinSupplyState {
    StarsCargoState planet;
    StarsCargoState fleet;
    StarsI16 calls;
    StarsI16 groups[3];
    StarsI16 ids[3];
    StarsI16 supplies[3];
    StarsI32 deltas[3];
    StarsI16 bad_lookup;
} TurinSupplyState;

static StarsU32 fleet_cargo_free(void *context,
    const StarsCargoState *fleet)
{
    TurinSupplyState *state = (TurinSupplyState *)context;
    if (fleet != &state->fleet) {
        state->bad_lookup = 1;
        return 0UL;
    }
    return TURIN_ORIGINAL_FLEET_COLONIST_GAIN;
}

static StarsI32 change_cargo(void *context, StarsI16 group, StarsI16 id,
    StarsI16 supply, StarsI32 delta)
{
    TurinSupplyState *state = (TurinSupplyState *)context;
    StarsCargoState *object;
    StarsChgCargoOps ops;
    StarsI16 slot;

    slot = state->calls;
    if (slot < 3) {
        state->groups[slot] = group;
        state->ids[slot] = id;
        state->supplies[slot] = supply;
        state->deltas[slot] = delta;
    }
    state->calls = (StarsI16)(state->calls + 1);
    if (group == 1 && id == TURIN_PLANET_ID)
        object = &state->planet;
    else if (group == 2 && id == TURIN_FLEET_ID)
        object = &state->fleet;
    else {
        state->bad_lookup = 1;
        return 0L;
    }
    ops.lookup = 0;
    ops.store = 0;
    ops.cargo_free = fleet_cargo_free;
    ops.fuel_free = 0;
    return stars_change_cargo(group, (StarsU16)id, supply, delta,
        object, &ops, state);
}

int main(void)
{
    TurinSupplyState state;
    StarsI16 result;
    StarsI16 failures;

    memset(&state, 0, sizeof state);
    state.planet.minerals[3] = TURIN_INPUT_PLANET_COLONISTS;
    state.fleet.minerals[3] = TURIN_INPUT_FLEET_COLONISTS;
    state.fleet.fleet_flags = 7U;
    failures = 0;
    if (state.planet.minerals[3] != 250UL) ++failures;
    if (state.fleet.minerals[3] != 0UL) ++failures;
    if (TURIN_ORIGINAL_FLEET_COLONIST_GAIN != 25) ++failures;
    if (TURIN_ORIGINAL_PLANET_COLONIST_NET != 6) ++failures;

    result = stars_xfer_ai_supply(1, TURIN_PLANET_ID, 2, TURIN_FLEET_ID,
        3, TURIN_ORIGINAL_FLEET_COLONIST_GAIN, change_cargo, &state);
    if (result != TURIN_ORIGINAL_FLEET_COLONIST_GAIN) ++failures;
    if (state.calls != 3) ++failures;
    if (state.bad_lookup != 0) ++failures;
    if (state.planet.minerals[3] != 225UL) ++failures;
    if (state.fleet.minerals[3] != 25UL) ++failures;
    if (state.groups[0] != 1 || state.ids[0] != TURIN_PLANET_ID ||
        state.supplies[0] != 3 || state.deltas[0] != 0L ||
        state.groups[1] != 2 || state.ids[1] != TURIN_FLEET_ID ||
        state.supplies[1] != 3 ||
        state.deltas[1] != TURIN_ORIGINAL_FLEET_COLONIST_GAIN ||
        state.groups[2] != 1 || state.ids[2] != TURIN_PLANET_ID ||
        state.supplies[2] != 3 ||
        state.deltas[2] != -TURIN_ORIGINAL_FLEET_COLONIST_GAIN)
        ++failures;

    printf("SATISFY_ORDERS_TURIN_SUPPLY_CHECKS=10 FAILURES=%d RESULT=%s\n",
        failures, failures ? "FAIL" : "PASS");
    return failures != 0;
}
