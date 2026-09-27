#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct PackedStargateState {
    StarsU8 fleet[124];
    StarsU8 designs[16][147];
} PackedStargateState;

typedef struct Trace {
    PackedStargateState *state;
    StarsFleetBalanceFleet service_snapshot;
    StarsI16 damage_result;
    StarsI16 damage_percent;
    StarsI16 messages;
    StarsI16 last_message;
    StarsU16 last_fleet_reference;
    StarsI16 last_fleet_id;
    StarsI16 last_planet;
    StarsI16 last_destination;
    StarsI16 last_slot;
    StarsU32 last_amount;
    StarsI16 random_value;
    StarsI32 random_zero_calls;
    StarsI32 random_calls;
    StarsI16 damage_source;
    StarsI16 damage_destination;
    StarsI16 damage_distance;
    StarsI16 damage_mass;
    StarsI16 damage_calls;
    StarsI16 transfers;
    StarsU16 transfer_destination_planet;
    StarsU16 transfer_destination_flags;
    StarsU16 transfer_destination_cargo;
    StarsU16 transfer_source_cargo;
    StarsI32 transfer_source_fuel;
    StarsI32 transfer_destination_fuel;
    StarsI16 service_calls;
} Trace;

static int f_stargate_jump_integration_checks;

static StarsI16 damage_lookup(void *context, StarsI16 id,
    StarsStargateArmor *armor)
{
    const StarsStargateArmor *table = (const StarsStargateArmor *)context;
    *armor = table[(unsigned short)id & 7U];
    return 1;
}

static StarsI16 reference_damage(const StarsStargateArmor *source,
    const StarsStargateArmor *destination, StarsI16 distance,
    StarsI16 weight, StarsI16 *damage_value)
{
    long range;
    long scale;
    long factor;

    range = source->fuel_max;
    if (range == -1L)
        range = 8000L;
    if (range * 5L < (long)distance)
        return -1;
    if (source->defense > 0 &&
            (long)source->defense * 5L < (long)weight)
        return -2;
    if (destination->defense > 0 &&
            (long)destination->defense * 5L < (long)weight)
        return -2;
    scale = 10000L;
    if ((long)distance > range) {
        factor = (range * 5L - (long)distance) * 2500L;
        factor /= range;
        if (factor < 1L) {
            *damage_value = 100;
            return 0;
        }
        scale = scale * factor / 10000L;
    }
    if (source->defense > 0 &&
            (long)source->defense < (long)weight) {
        factor = ((long)source->defense * 5L - (long)weight) * 2500L;
        factor /= source->defense;
        if (factor < 1L) {
            *damage_value = 100;
            return 0;
        }
        scale = scale * factor / 10000L;
    }
    if (destination->defense > 0 &&
            (long)destination->defense < (long)weight) {
        factor = ((long)destination->defense * 5L - (long)weight) * 2500L;
        factor /= destination->defense;
        if (factor < 1L) {
            *damage_value = 100;
            return 0;
        }
        scale = scale * factor / 10000L;
    }
    *damage_value = (StarsI16)((10000L - scale) / 100L);
    return 1;
}

static void add_damage_value(StarsI16 *values, int *count, long value)
{
    int i;

    if (value < 0L)
        value = 0L;
    if (value > 32767L)
        value = 32767L;
    for (i = 0; i < *count; ++i)
        if (values[i] == (StarsI16)value)
            return;
    values[*count] = (StarsI16)value;
    ++*count;
}

static int run_damage_matrix(int *checks_out)
{
    static const StarsStargateArmor armor[8] = {
        { -1, 0 }, { 0, 0 }, { 1, 1 }, { 40, 20 },
        { 80, 30 }, { 100, 40 }, { 300, 100 }, { 32767, 32767 }
    };
    StarsStargateDamageInput input;
    StarsI16 distances[8];
    StarsI16 weights[8];
    StarsI16 expected_damage;
    StarsI16 actual_damage;
    StarsI16 expected_result;
    StarsI16 actual_result;
    int source_index;
    int destination_index;
    int distance_count;
    int weight_count;
    int distance_index;
    int weight_index;
    int checks;
    int failures;
    long range;
    long defense;

    input.lookup = damage_lookup;
    input.context = (void *)armor;
    checks = 0;
    failures = 0;
    for (source_index = 0; source_index < 8; ++source_index) {
        range = armor[source_index].fuel_max;
        if (range == -1L)
            range = 8000L;
        distance_count = 0;
        add_damage_value(distances, &distance_count, 0L);
        add_damage_value(distances, &distance_count, 1L);
        add_damage_value(distances, &distance_count, range - 1L);
        add_damage_value(distances, &distance_count, range);
        add_damage_value(distances, &distance_count, range + 1L);
        add_damage_value(distances, &distance_count, range * 5L - 1L);
        add_damage_value(distances, &distance_count, range * 5L);
        add_damage_value(distances, &distance_count, range * 5L + 1L);
        for (destination_index = 0; destination_index < 8;
                ++destination_index) {
            defense = armor[destination_index].defense;
            weight_count = 0;
            add_damage_value(weights, &weight_count, 0L);
            add_damage_value(weights, &weight_count, 1L);
            add_damage_value(weights, &weight_count, defense - 1L);
            add_damage_value(weights, &weight_count, defense);
            add_damage_value(weights, &weight_count, defense + 1L);
            add_damage_value(weights, &weight_count, defense * 5L - 1L);
            add_damage_value(weights, &weight_count, defense * 5L);
            add_damage_value(weights, &weight_count, defense * 5L + 1L);
            for (distance_index = 0; distance_index < distance_count;
                    ++distance_index) {
                for (weight_index = 0; weight_index < weight_count;
                        ++weight_index) {
                    expected_damage = (StarsI16)-77;
                    actual_damage = (StarsI16)-77;
                    expected_result = reference_damage(
                        &armor[source_index], &armor[destination_index],
                        distances[distance_index], weights[weight_index],
                        &expected_damage);
                    actual_result = stars_calc_stargate_damage(&input,
                        (StarsI16)source_index, (StarsI16)destination_index,
                        distances[distance_index], weights[weight_index],
                        &actual_damage);
                    ++checks;
                    if (expected_result != actual_result ||
                            expected_damage != actual_damage)
                        ++failures;
                }
            }
        }
    }
    *checks_out = checks;
    return failures;
}

#define CHECK(condition) do { \
    if (!(condition)) \
        ++failures; \
} while (0)

static StarsU16 get_word(const StarsU8 *p, StarsU16 offset)
{
    return (StarsU16)(p[offset] | ((StarsU16)p[offset + 1U] << 8));
}

static void put_word(StarsU8 *p, StarsU16 offset, StarsU16 value)
{
    p[offset] = (StarsU8)value;
    p[offset + 1U] = (StarsU8)(value >> 8);
}

static StarsU32 get_dword(const StarsU8 *p, StarsU16 offset)
{
    return (StarsU32)p[offset] |
        ((StarsU32)p[offset + 1U] << 8) |
        ((StarsU32)p[offset + 2U] << 16) |
        ((StarsU32)p[offset + 3U] << 24);
}

static void put_dword(StarsU8 *p, StarsU16 offset, StarsU32 value)
{
    p[offset] = (StarsU8)value;
    p[offset + 1U] = (StarsU8)(value >> 8);
    p[offset + 2U] = (StarsU8)(value >> 16);
    p[offset + 3U] = (StarsU8)(value >> 24);
}

static StarsI16 damage(void *context, StarsI16 source, StarsI16 destination,
    StarsI16 distance, StarsI16 mass, StarsI16 *percent)
{
    Trace *trace = (Trace *)context;
    trace->damage_source = source;
    trace->damage_destination = destination;
    trace->damage_distance = distance;
    trace->damage_mass = mass;
    ++trace->damage_calls;
    *percent = trace->damage_percent;
    return trace->damage_result;
}

static StarsI16 random_value(void *context, StarsI16 limit)
{
    Trace *trace = (Trace *)context;
    (void)limit;
    ++trace->random_calls;
    return trace->random_calls <= (trace->random_zero_calls ?
        trace->random_zero_calls : 1) ? 0 :
        trace->random_value;
}

static void send_message(void *context, const StarsFStargateMessage *message)
{
    Trace *trace = (Trace *)context;
    ++trace->messages;
    trace->last_message = message->code;
    trace->last_fleet_reference = message->fleet_reference;
    trace->last_fleet_id = message->fleet_id;
    trace->last_planet = message->planet;
    trace->last_destination = message->destination;
    trace->last_slot = message->slot;
    trace->last_amount = message->amount;
}

static StarsI16 service_lookup(void *context, StarsI16 id,
    StarsFleetBalanceFleet *out)
{
    Trace *trace = (Trace *)context;

    if (id != 0 || !out)
        return 0;
    *out = trace->service_snapshot;
    return 1;
}

static StarsI16 service_weight(void *context, StarsI16 player,
    StarsI16 design, StarsI16 stat)
{
    (void)context;
    (void)player;
    (void)design;
    return stat == 1 ? 10 : 4;
}

static void transfer(void *context, StarsFStargateFleet *source,
    StarsFStargateFleet *destination)
{
    Trace *trace = (Trace *)context;
    StarsFleetBalanceFleet balance[2];
    StarsFleetTransferCargoBalanceInput service;
    StarsI16 i;

    memset(balance, 0, sizeof balance);
    balance[0].player = source->player;
    balance[0].flags = source->flags;
    balance[1].player = destination->player;
    balance[1].flags = destination->flags;
    for (i = 0; i < STARS_FLEET_BALANCE_DESIGNS; ++i) {
        balance[0].ship_count[i] = (StarsI16)source->cargo[i];
        balance[0].ship_status[i] = (StarsU16)source->cargo_state[i];
        balance[1].ship_count[i] = (StarsI16)destination->cargo[i];
        balance[1].ship_status[i] = (StarsU16)destination->cargo_state[i];
    }
    for (i = 0; i < STARS_FLEET_BALANCE_RESOURCES; ++i) {
        balance[0].cargo[i] = source->resources[i];
        balance[1].cargo[i] = destination->resources[i];
    }
    service.new_fleets[0] = &balance[0];
    service.new_fleets[1] = &balance[1];
    service.fleet_ids[0] = 0;
    service.fleet_ids[1] = 1;
    service.player = source->player;
    service.lookup_fleet = service_lookup;
    service.max_weight = service_weight;
    service.context = trace;
    stars_fleet_transfer_cargo_balance(&service);
    source->flags = balance[0].flags;
    destination->flags = balance[1].flags;
    for (i = 0; i < STARS_FLEET_BALANCE_DESIGNS; ++i) {
        source->cargo[i] = (StarsU32)balance[0].ship_count[i];
        source->cargo_state[i] = balance[0].ship_status[i];
        destination->cargo[i] = (StarsU32)balance[1].ship_count[i];
        destination->cargo_state[i] = balance[1].ship_status[i];
    }
    for (i = 0; i < STARS_FLEET_BALANCE_RESOURCES; ++i) {
        source->resources[i] = balance[0].cargo[i];
        destination->resources[i] = balance[1].cargo[i];
    }
    ++trace->transfers;
    trace->transfer_destination_planet = (StarsU16)destination->planet;
    trace->transfer_destination_flags = destination->flags;
    trace->transfer_destination_cargo = (StarsU16)destination->cargo[0];
    trace->transfer_source_cargo = (StarsU16)source->cargo[0];
    trace->transfer_source_fuel = source->resources[4];
    trace->transfer_destination_fuel = destination->resources[4];
    ++trace->service_calls;
}

static void seed(PackedStargateState *state, StarsU16 cargo,
    StarsU16 cargo_state)
{
    memset(state, 0, sizeof *state);
    put_word(state->fleet, 0, 41);
    put_word(state->fleet, 2, 2);
    put_word(state->fleet, 6, 9);
    put_word(state->fleet, 12, cargo);
    put_word(state->fleet, 44, cargo_state);
    put_word(state->designs[0], 40, 3);
    put_word(state->designs[0], 56, 100);
}

static void load_typed(const PackedStargateState *state,
    StarsFStargateFleet *fleet, StarsFStargateJumpInput *input)
{
    StarsI16 i;
    memset(fleet, 0, sizeof *fleet);
    memset(input, 0, sizeof *input);
    fleet->id = get_word(state->fleet, 0);
    fleet->player = (StarsI16)get_word(state->fleet, 2);
    fleet->flags = get_word(state->fleet, 4);
    fleet->planet = (StarsI16)get_word(state->fleet, 6);
    for (i = 0; i < STARS_STARGATE_CARGO_SLOTS; ++i) {
        fleet->cargo[i] = get_word(state->fleet,
            (StarsU16)(12U + (StarsU16)i * 2U));
        fleet->cargo_state[i] = get_word(state->fleet,
            (StarsU16)(44U + (StarsU16)i * 2U));
        input->design_mass[i] = (StarsI16)get_word(state->designs[i], 40);
        input->ship_weight[i] = (StarsI32)get_word(state->designs[i], 56);
    }
    for (i = 0; i < STARS_FLEET_BALANCE_RESOURCES; ++i)
        fleet->resources[i] = (StarsI32)get_dword(state->fleet,
            (StarsU16)(76U + (StarsU16)i * 4U));
}

static void store_typed(PackedStargateState *state,
    const StarsFStargateFleet *fleet)
{
    StarsI16 i;
    put_word(state->fleet, 0, fleet->id);
    put_word(state->fleet, 2, (StarsU16)fleet->player);
    put_word(state->fleet, 4, fleet->flags);
    put_word(state->fleet, 6, (StarsU16)fleet->planet);
    for (i = 0; i < STARS_STARGATE_CARGO_SLOTS; ++i) {
        put_word(state->fleet, (StarsU16)(12U + (StarsU16)i * 2U),
            (StarsU16)fleet->cargo[i]);
        put_word(state->fleet, (StarsU16)(44U + (StarsU16)i * 2U),
            (StarsU16)fleet->cargo_state[i]);
    }
    for (i = 0; i < STARS_FLEET_BALANCE_RESOURCES; ++i)
        put_dword(state->fleet, (StarsU16)(76U + (StarsU16)i * 4U),
            (StarsU32)fleet->resources[i]);
}

static void install(Trace *trace, StarsFStargateJumpInput *input,
    StarsFStargateFleet *fleet)
{
    StarsI16 i;

    input->fleet = fleet;
    input->destination = 12;
    input->race_stat_stargate_loss = 0;
    input->stargate_damage = damage;
    input->random_value = random_value;
    input->send_message = send_message;
    input->transfer_cargo_balance = transfer;
    input->context = trace;
    memset(&trace->service_snapshot, 0, sizeof trace->service_snapshot);
    trace->service_snapshot.player = fleet->player;
    trace->service_snapshot.flags = fleet->flags;
    for (i = 0; i < STARS_FLEET_BALANCE_DESIGNS; ++i) {
        trace->service_snapshot.ship_count[i] = (StarsI16)fleet->cargo[i];
        trace->service_snapshot.ship_status[i] =
            (StarsU16)fleet->cargo_state[i];
    }
    for (i = 0; i < STARS_FLEET_BALANCE_RESOURCES; ++i)
        trace->service_snapshot.cargo[i] = fleet->resources[i];
}

int main(void)
{
    PackedStargateState state;
    StarsFStargateFleet fleet;
    StarsFStargateJumpInput input;
    Trace trace;
    StarsI16 result;
    int damage_checks;
    int failures = 0;

    memset(&trace, 0, sizeof trace);
    seed(&state, 10, 50);
    trace.state = &state;
    trace.damage_result = -2;
    trace.damage_percent = 0;
    load_typed(&state, &fleet, &input);
    install(&trace, &input, &fleet);
    result = stars_f_stargate_jump(&input, 1, 2, 30);
    CHECK(result == 0 && trace.last_message == STARS_STARGATE_MSG_DAMAGE_ABORT &&
        trace.last_fleet_reference == 0x8029U && trace.last_fleet_id == 41 &&
        trace.last_planet == 9 && trace.last_destination == 12 &&
        trace.last_slot == 0 && trace.last_amount == 0U);

    memset(&trace, 0, sizeof trace);
    seed(&state, 10, 50);
    trace.state = &state;
    trace.damage_result = -1;
    load_typed(&state, &fleet, &input);
    install(&trace, &input, &fleet);
    result = stars_f_stargate_jump(&input, 1, 2, 30);
    CHECK(result == 0 && trace.last_message == STARS_STARGATE_MSG_DAMAGE_FAIL &&
        trace.messages == 1 && fleet.cargo[0] == 10U &&
        trace.transfers == 0);

    memset(&trace, 0, sizeof trace);
    seed(&state, 0, 0);
    trace.state = &state;
    trace.damage_result = 0;
    load_typed(&state, &fleet, &input);
    install(&trace, &input, &fleet);
    result = stars_f_stargate_jump(&input, 1, 2, 30);
    CHECK(result == 0 && trace.last_message == STARS_STARGATE_MSG_EMPTY &&
        trace.messages == 1 && fleet.flags == 0x0400U &&
        trace.transfers == 0);

    memset(&trace, 0, sizeof trace);
    seed(&state, 4, 2660);
    trace.state = &state;
    trace.damage_result = 1;
    trace.damage_percent = 7;
    trace.random_value = 99;
    load_typed(&state, &fleet, &input);
    install(&trace, &input, &fleet);
    result = stars_f_stargate_jump(&input, 1, 2, 30);
    store_typed(&state, &fleet);
    CHECK(result == 1 && trace.transfers == 1 && fleet.cargo[0] == 3U &&
        fleet.flags == 0x4000U && get_word(state.fleet, 12) == 3U &&
        get_word(state.fleet, 44) == 7780U &&
        get_word(state.designs[0], 40) == 3U &&
        get_word(state.designs[0], 56) == 100U && fleet.planet == 9 &&
        trace.last_message == STARS_STARGATE_MSG_MEDIUM_LOSS &&
        trace.last_amount == 1U);

    memset(&trace, 0, sizeof trace);
    seed(&state, 10, 50);
    trace.state = &state;
    trace.damage_result = 1;
    trace.damage_percent = 100;
    load_typed(&state, &fleet, &input);
    install(&trace, &input, &fleet);
    result = stars_f_stargate_jump(&input, 1, 2, 30);
    store_typed(&state, &fleet);
    CHECK(result == 0 && trace.last_message == STARS_STARGATE_MSG_EMPTY &&
        fleet.cargo[0] == 0U && fleet.cargo_state[0] == 0U &&
        fleet.flags == 0x4400U && get_word(state.fleet, 12) == 0U &&
        get_word(state.fleet, 44) == 0U);

    memset(&trace, 0, sizeof trace);
    seed(&state, 4, 2660);
    trace.state = &state;
    trace.damage_result = 1;
    trace.damage_percent = 7;
    trace.random_value = 99;
    load_typed(&state, &fleet, &input);
    input.race_stat_stargate_loss = 7;
    install(&trace, &input, &fleet);
    input.race_stat_stargate_loss = 7;
    result = stars_f_stargate_jump(&input, 1, 2, 30);
    store_typed(&state, &fleet);
    CHECK(result == 1 && trace.transfers == 0 && trace.messages == 0 &&
        fleet.cargo[0] == 4U && fleet.cargo_state[0] == 7140U &&
        fleet.flags == 0x4000U && get_word(state.fleet, 12) == 4U &&
        get_word(state.fleet, 44) == 7140U);

    memset(&trace, 0, sizeof trace);
    seed(&state, 4, 2660);
    put_word(state.fleet, 14, 6);
    put_word(state.fleet, 46, 0);
    put_word(state.designs[1], 40, 3);
    put_word(state.designs[1], 56, 100);
    trace.state = &state;
    trace.damage_result = 1;
    trace.damage_percent = 7;
    trace.random_value = 99;
    load_typed(&state, &fleet, &input);
    install(&trace, &input, &fleet);
    result = stars_f_stargate_jump(&input, 1, 2, 30);
    store_typed(&state, &fleet);
    CHECK(result == 1 && trace.transfers == 1 && trace.messages == 1 &&
        trace.last_message == STARS_STARGATE_MSG_LIGHT_LOSS &&
        trace.last_amount == 1U && fleet.cargo[0] == 3U &&
        fleet.cargo[1] == 6U && fleet.cargo_state[0] == 7780U &&
        fleet.cargo_state[1] == 4580U && fleet.flags == 0x4000U &&
        fleet.planet == 9);

    memset(&trace, 0, sizeof trace);
    seed(&state, 4, 0);
    put_dword(state.fleet, 76, 20U);
    put_dword(state.fleet, 80, 15U);
    put_dword(state.fleet, 92, 30U);
    trace.state = &state;
    trace.damage_result = 1;
    trace.damage_percent = 7;
    trace.random_value = 99;
    load_typed(&state, &fleet, &input);
    install(&trace, &input, &fleet);
    result = stars_f_stargate_jump(&input, 5, 8, 77);
    store_typed(&state, &fleet);
    CHECK(result == 1 && trace.damage_calls == 1 &&
        trace.damage_source == 5 && trace.damage_destination == 8 &&
        trace.damage_distance == 77 && trace.damage_mass == 3 &&
        fleet.cargo[0] == 3U && fleet.cargo_state[0] == 4580U &&
        trace.transfer_destination_planet == 0U &&
        trace.transfer_destination_flags == 0x0407U &&
        trace.transfer_destination_cargo == 1U &&
        trace.transfer_source_cargo == 3U && trace.service_calls == 1 &&
        fleet.resources[0] == 15L && fleet.resources[1] == 12L &&
        fleet.resources[4] == 23L && trace.transfer_source_fuel == 23L &&
        trace.transfer_destination_fuel == 7L &&
        get_dword(state.fleet, 76) == 15UL &&
        get_dword(state.fleet, 80) == 12UL &&
        get_dword(state.fleet, 92) == 23UL);
    CHECK(trace.service_calls == 1 && trace.transfer_source_cargo == 3U &&
        trace.transfer_destination_cargo == 1U &&
        trace.transfer_source_fuel == 23L &&
        trace.transfer_destination_fuel == 7L);

    memset(&trace, 0, sizeof trace);
    seed(&state, 5000U, 0);
    put_word(state.fleet, 14, 5000U);
    put_word(state.fleet, 16, 5000U);
    put_word(state.fleet, 18, 5000U);
    put_word(state.fleet, 20, 5000U);
    put_word(state.fleet, 22, 5000U);
    put_word(state.fleet, 24, 5000U);
    put_word(state.fleet, 26, 5000U);
    put_word(state.fleet, 28, 5000U);
    put_word(state.fleet, 30, 5000U);
    put_word(state.fleet, 32, 5000U);
    put_word(state.fleet, 34, 5000U);
    put_word(state.fleet, 36, 5000U);
    put_word(state.fleet, 38, 5000U);
    put_word(state.fleet, 40, 5000U);
    put_word(state.fleet, 42, 5000U);
    trace.state = &state;
    trace.damage_result = 1;
    trace.damage_percent = 3;
    trace.random_zero_calls = 70000L;
    trace.random_value = 99;
    load_typed(&state, &fleet, &input);
    install(&trace, &input, &fleet);
    result = stars_f_stargate_jump(&input, 1, 2, 30);
    store_typed(&state, &fleet);
    CHECK(result == 1 && trace.damage_calls == 16 && trace.transfers == 1 &&
        trace.last_message == STARS_STARGATE_MSG_LARGE_LOSS &&
        trace.last_amount == 70000UL && fleet.cargo[0] == 0U &&
        fleet.cargo[13] == 0U && fleet.cargo[14] == 5000U &&
        fleet.cargo[15] == 5000U && fleet.cargo_state[0] == 0U &&
        fleet.cargo_state[14] != 0U);

    failures += run_damage_matrix(&damage_checks);
    f_stargate_jump_integration_checks = 11 + damage_checks;

#ifndef INTEGRATION_NATIVE
    printf("F_STARGATE_JUMP_INTEGRATION_CHECKS=%d FAILURES=%d RESULT=%s\n",
        f_stargate_jump_integration_checks, failures,
        failures == 0 ? "PASS" : "FAIL");
#endif
    return failures != 0;
}
