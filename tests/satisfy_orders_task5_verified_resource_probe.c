#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct VerifiedResource {
    StarsU8 fleet[124];
    StarsU8 order[18];
    StarsU8 planet[60];
    StarsU8 design[147];
    StarsU8 player_stats[15];
    StarsU16 extra;
    StarsI16 resource_calls;
    StarsI16 base_resources;
    StarsI16 messages;
    StarsI16 codes[2];
    StarsI16 payloads[2][8];
    StarsI16 done;
} VerifiedResource;

static int checks;
static int failures;
#define CHECK(test) do { ++checks; if (!(test)) ++failures; } while (0)

static void put_word(StarsU8 *record, StarsU16 offset, StarsU16 value)
{
    record[offset] = (StarsU8)value;
    record[offset + 1U] = (StarsU8)(value >> 8);
}

static StarsU16 word(const StarsU8 *record, StarsU16 offset)
{
    return (StarsU16)(record[offset] |
        ((StarsU16)record[offset + 1U] << 8));
}

static void put_long(StarsU8 *record, StarsU16 offset, StarsU32 value)
{
    put_word(record, offset, (StarsU16)value);
    put_word(record, (StarsU16)(offset + 2U), (StarsU16)(value >> 16));
}

/* This symbol is used by unrelated planet.c paths, which this focused
 * CResourcesAtPlanet probe never calls. */
StarsI16 stars_get_race_trait(const StarsU8 *data, StarsU16 player,
    StarsI16 index)
{
    (void)data;
    (void)player;
    (void)index;
    return 0;
}

static StarsI32 max_population(void *context, StarsU16 planet,
    StarsI16 player)
{
    (void)context;
    CHECK(planet == 10U && player == 0);
    return 12000L;
}

static StarsI16 desirability(void *context, const StarsU8 *planet,
    StarsI16 player)
{
    (void)context;
    (void)planet;
    CHECK(player == 0);
    return 100;
}

static StarsI16 player_stat(void *context, StarsI16 player,
    StarsI16 index)
{
    VerifiedResource *state;
    state = (VerifiedResource *)context;
    CHECK(player == 0 && index >= 0 && index < 15);
    if (index < 0 || index >= 15)
        return 0;
    return state->player_stats[index];
}

static StarsI16 player_byte(void *context, StarsI16 player,
    StarsU16 index)
{
    (void)context;
    (void)player;
    (void)index;
    return 0;
}

static StarsU16 planet_resources(void *context, StarsI16 player,
    StarsU16 planet_id)
{
    VerifiedResource *state;
    StarsCResourcesAtPlanetOps ops;
    state = (VerifiedResource *)context;
    memset(&ops, 0, sizeof ops);
    ops.calc_max_population = max_population;
    ops.planet_desirability = desirability;
    ops.get_player_stat = player_stat;
    ops.get_player_byte = player_byte;
    ops.factory_calc_max_population = max_population;
    ops.factory_get_player_stat = player_stat;
    CHECK(player == 0 && planet_id == 10U);
    state->base_resources = stars_cresources_at_planet(state->planet,
        player, &ops, state);
    ++state->resource_calls;
    return (StarsU16)state->base_resources;
}

static StarsI16 learn(void *context, StarsI16 player, StarsU16 *object)
{
    (void)context;
    CHECK(player == 0 && *object == 10U);
    return 0;
}

static void message(void *context, StarsI16 recipient, StarsI16 code,
    const StarsI16 payload[8])
{
    VerifiedResource *state;
    StarsI16 index;
    state = (VerifiedResource *)context;
    index = state->messages++;
    CHECK(recipient == 0 && index < 2);
    if (index < 2) {
        state->codes[index] = code;
        memcpy(state->payloads[index], payload,
            sizeof state->payloads[index]);
    }
}

static void done(void *context, StarsSatisfyFleet *fleet,
    StarsSatisfyOrder *order, StarsI16 index)
{
    VerifiedResource *state;
    state = (VerifiedResource *)context;
    CHECK(fleet->id == 0 && order->active == 0 && index == 0);
    ++state->done;
}

int main(void)
{
    static const StarsU8 verified_stats[15] = {
        10, 10, 10, 10, 10, 5, 10, 0, 1, 1, 1, 1, 1, 1, 9
    };
    VerifiedResource state;
    StarsSatisfyPackedInput input;
    StarsSatisfyScrapPackedState scrap;
    StarsSatisfyOrdersOps ops;
    FILE *report;

    memset(&state, 0, sizeof state);
    memset(&input, 0, sizeof input);
    memset(&scrap, 0, sizeof scrap);
    memset(&ops, 0, sizeof ops);
    memcpy(state.player_stats, verified_stats, sizeof verified_stats);
    put_word(state.fleet, 0U, 0U);
    put_word(state.fleet, 2U, 0U);
    put_word(state.fleet, 4U, 0x0907U);
    put_word(state.fleet, 6U, 10U);
    put_word(state.fleet, 8U, 1128U);
    put_word(state.fleet, 10U, 1300U);
    put_word(state.fleet, 12U, 1U);
    put_long(state.fleet, 92U, 50U);
    put_word(state.fleet, 118U, 0x0060U);
    put_word(state.order, 0U, 1128U);
    put_word(state.order, 2U, 1300U);
    put_word(state.order, 4U, 10U);
    put_word(state.order, 6U, 0x1105U);
    put_word(state.planet, 0U, 10U);
    put_word(state.planet, 2U, 0U);
    put_word(state.planet, 4U, 0x2b87U);
    state.planet[9U] = 68U;
    state.planet[10U] = 87U;
    state.planet[11U] = 68U;
    state.planet[0x15U] = 10U;
    state.planet[0x16U] = 160U;
    state.planet[0x18U] = 10U;
    put_long(state.planet, 0x1cU, 469U);
    put_long(state.planet, 0x20U, 893U);
    put_long(state.planet, 0x24U, 376U);
    put_long(state.planet, 0x28U, 250U);
    put_word(state.design, 0x2aU, 24U);
    put_word(state.design, 0x2cU, 12U);
    put_word(state.design, 0x2eU, 8U);
    put_word(state.design, 0x30U, 7U);
    scrap.planet_record = state.planet;
    scrap.planet_size = sizeof state.planet;
    scrap.design_records[0] = state.design;
    scrap.design_size = sizeof state.design;
    scrap.planet_race_scrap_bonus = 1;
    scrap.planet_resource_extra = &state.extra;
    ops.scrap_planet_resources = planet_resources;
    ops.scrap_learn = learn;
    ops.raw_message = message;
    ops.order_done = done;
    ops.context = &state;
    input.fleet_record = state.fleet;
    input.fleet_size = sizeof state.fleet;
    input.order_record = state.order;
    input.order_size = sizeof state.order;
    input.scrap_state = &scrap;
    input.pass = 1;
    input.ops = &ops;
    CHECK(stars_satisfy_orders_packed(&input) == 1);
    CHECK(state.base_resources == 35 && state.resource_calls == 1);
    CHECK(word(state.planet, 0x1cU) == 479U &&
        word(state.planet, 0x20U) == 900U &&
        word(state.planet, 0x24U) == 382U);
    CHECK(state.extra == 24U);
    CHECK(state.messages == 2 && state.codes[0] == 0x5d &&
        state.codes[1] == 0x143);
    CHECK(state.payloads[0][0] == 10 && state.payloads[0][2] == 23 &&
        state.payloads[0][4] == 10 && state.payloads[0][5] == 14);
    CHECK(state.payloads[1][0] == 10 && state.payloads[1][2] == 23 &&
        state.payloads[1][4] == 10 && state.payloads[1][5] == 14);
    CHECK(state.done == 1 && (word(state.fleet, 4U) & 0x0400U) != 0U &&
        (word(state.order, 6U) & 0x1000U) == 0U);
#ifndef TARGET_NATIVE
    report = stdout;
#else
    report = fopen("C:\\SOE2L\\SOE2LRES.TXT", "w");
    if (!report)
        return 4;
#endif
    fprintf(report, "RESOURCE_BOUND_CHECKS=%d FAILURES=%d RESULT=%s "
        "BASE=%d ORE=%u,%u,%u EXTRA=%u MSG=%d,%d\n",
        checks, failures, failures ? "FAIL" : "PASS",
        state.base_resources,
        word(state.planet, 0x1cU), word(state.planet, 0x20U),
        word(state.planet, 0x24U), state.extra,
        state.codes[0], state.codes[1]);
    if (report != stdout)
        fclose(report);
    return failures ? 1 : 0;
}
