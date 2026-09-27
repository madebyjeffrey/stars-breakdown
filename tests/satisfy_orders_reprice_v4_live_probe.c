#include "core.h"
#include "common.h"
#include "fixtures/satisfy_orders_reprice_v4_bitc_parts.h"
#include <stdio.h>
#include <string.h>

typedef struct LiveV4 {
    StarsShdefRepriceInput reprice;
    StarsU16 game_flags;
    StarsU32 race_bits[2];
    signed char owner_technology[6];
    StarsU8 tech_battle[6];
    StarsU8 tech_trader[12];
    StarsTechLearnPlayer learn_player;
    StarsI16 random_calls;
    StarsI16 mark_lookups;
    StarsI16 reprice_calls;
    StarsU16 repriced_cost[4];
    StarsI16 learn_result;
    StarsI16 learn_events;
    StarsI16 message_count;
    StarsI16 message_codes[2];
    StarsI16 message_recipients[2];
    StarsI16 message_payloads[2][8];
    StarsI16 remove_calls;
    StarsI16 remove_saw_live;
    StarsI16 done_calls;
    StarsI16 done_active;
    StarsU8 *fleet;
    StarsU8 *planet;
    char events[20];
    StarsI16 event_count;
} LiveV4;

static int checks;
static int failures;
#define CHECK(test) do { ++checks; if (!(test)) ++failures; } while (0)

static void put_word(StarsU8 *bytes, StarsU16 offset, StarsU16 value)
{
    bytes[offset] = (StarsU8)value;
    bytes[offset + 1U] = (StarsU8)(value >> 8);
}

static StarsU16 word(const StarsU8 *bytes, StarsU16 offset)
{
    return (StarsU16)(bytes[offset] | ((StarsU16)bytes[offset + 1U] << 8));
}

static void put_long(StarsU8 *bytes, StarsU16 offset, StarsU32 value)
{
    put_word(bytes, offset, (StarsU16)value);
    put_word(bytes, (StarsU16)(offset + 2U), (StarsU16)(value >> 16));
}

static StarsU32 get_long(const StarsU8 *bytes, StarsU16 offset)
{
    return (StarsU32)word(bytes, offset) |
        ((StarsU32)word(bytes, (StarsU16)(offset + 2U)) << 16);
}

static void event(LiveV4 *state, char value)
{
    if (state->event_count < 19) {
        state->events[state->event_count++] = value;
        state->events[state->event_count] = '\0';
    }
}

static StarsI16 race_stat(void *context, StarsI16 player, StarsI16 stat)
{
    (void)context;
    CHECK(player == 1 && stat == 0x0e);
    return player == 1 ? 1 : 0;
}

static StarsI16 race_grbit(void *context, StarsI16 player, StarsI16 bit)
{
    LiveV4 *state;
    state = (LiveV4 *)context;
    if (player < 0 || player > 1 || bit < 0 || bit > 31)
        return 0;
    return (StarsI16)((state->race_bits[player] >> bit) & 1UL);
}

static StarsI16 part_technology(void *context, StarsU16 group,
    StarsU16 subtype, StarsU8 technology[6])
{
    LiveV4 *state;
    StarsLookupPartInput lookup;
    const StarsU8 *record;
    state = (LiveV4 *)context;
    memset(&lookup, 0, sizeof lookup);
    lookup.group = group;
    lookup.item = subtype;
    lookup.player = -1;
    (void)stars_lookup_part(&lookup);
    record = stars_lookup_part_record(&lookup, v4_part_segment,
        (StarsU16)sizeof v4_part_segment);
    if (!record)
        return 0;
    memcpy(technology, record + 2U, 6U);
    ++state->mark_lookups;
    return 1;
}

static void mark_technology(void *context, const StarsU8 *design,
    StarsI16 owner)
{
    LiveV4 *state;
    StarsMarkTechsInput input;
    state = (LiveV4 *)context;
    CHECK(owner == 1);
    memset(&input, 0, sizeof input);
    memcpy(input.tech_battle, state->tech_battle, sizeof input.tech_battle);
    memcpy(input.tech_trader, state->tech_trader, sizeof input.tech_trader);
    input.lookup = part_technology;
    input.context = state;
    CHECK(stars_mark_techs_seen_shdef(design, 147U, &input) == 1);
    memcpy(state->tech_battle, input.tech_battle, sizeof state->tech_battle);
    memcpy(state->tech_trader, input.tech_trader, sizeof state->tech_trader);
    event(state, 'K');
}

static StarsI16 reprice(void *context, const StarsU8 *source,
    StarsU8 *destination)
{
    LiveV4 *state;
    StarsI16 result;
    state = (LiveV4 *)context;
    result = stars_reprice_shdef_cost(&state->reprice, source, 147U,
        destination);
    if (result) {
        CHECK((*state->reprice.game_flags & 0x0008U) != 0U);
        ++state->reprice_calls;
        state->repriced_cost[0] = word(destination, 0x2aU);
        state->repriced_cost[1] = word(destination, 0x2cU);
        state->repriced_cost[2] = word(destination, 0x2eU);
        state->repriced_cost[3] = word(destination, 0x30U);
        event(state, 'U');
    }
    return result;
}

static StarsI16 random_value(void *context, StarsI16 limit)
{
    LiveV4 *state;
    state = (LiveV4 *)context;
    ++state->random_calls;
    if (limit == 100)
        return 60;
    return 0;
}

static StarsI32 next_tech_cost(void *context, StarsI16 technology,
    StarsI16 level, StarsI16 player)
{
    (void)context;
    CHECK(technology == 0 && level == 4 && player == 0);
    return 0L;
}

static void mark_learn_event(void *context, StarsI16 player)
{
    LiveV4 *state;
    state = (LiveV4 *)context;
    CHECK(player == 0);
    ++state->learn_events;
}

static StarsI16 learn_technology(void *context, StarsI16 owner,
    StarsU16 *message_object)
{
    LiveV4 *state;
    StarsTechLearnInput input;
    StarsU16 goto_id;
    state = (LiveV4 *)context;
    CHECK(owner == 0 && *message_object == 10U);
    memset(&input, 0, sizeof input);
    input.player = &state->learn_player;
    input.player_id = 0;
    input.message_id = -1;
    input.battle_threshold[0] = 4U;
    input.goto_id = &goto_id;
    input.random = random_value;
    input.tech_level_cost = next_tech_cost;
    input.mark_event = mark_learn_event;
    input.context = state;
    state->learn_result = stars_itech_learn_a_tech(&input);
    CHECK(state->learn_result == 1 && goto_id == 0xfffeU);
    event(state, 'T');
    return state->learn_result;
}

static void message(void *context, StarsI16 recipient, StarsI16 code,
    const StarsI16 payload[8])
{
    LiveV4 *state;
    StarsI16 index;
    state = (LiveV4 *)context;
    index = state->message_count;
    CHECK(index < 2);
    if (index < 2) {
        state->message_codes[index] = code;
        state->message_recipients[index] = recipient;
        memcpy(state->message_payloads[index], payload,
            sizeof state->message_payloads[index]);
    }
    ++state->message_count;
    event(state, 'M');
}

static void remove_message(void *context, StarsI16 owner, StarsI16 code,
    StarsU16 object)
{
    LiveV4 *state;
    state = (LiveV4 *)context;
    CHECK(owner == 1 && code == 0x4e && object == 0x8000U);
    state->remove_saw_live = (StarsI16)(
        (word(state->fleet, 4U) & 0x0400U) == 0U);
    ++state->remove_calls;
    event(state, 'R');
}

static void order_done(void *context, StarsSatisfyFleet *fleet,
    StarsSatisfyOrder *order, StarsI16 index)
{
    LiveV4 *state;
    state = (LiveV4 *)context;
    CHECK(fleet->id == 0 && fleet->owner == 1 && index == 0);
    state->done_active = order->active;
    ++state->done_calls;
    event(state, 'D');
}

int main(void)
{
    LiveV4 state;
    StarsU8 fleet[124];
    StarsU8 order[18];
    StarsU8 planet[60];
    StarsU16 planet_extra;
    StarsSatisfyScrapPackedState scrap;
    StarsSatisfyPackedInput input;
    StarsSatisfyOrdersOps ops;
    FILE *report;

    memset(&state, 0, sizeof state);
    memset(fleet, 0, sizeof fleet);
    memset(order, 0, sizeof order);
    memset(planet, 0, sizeof planet);
    memset(&scrap, 0, sizeof scrap);
    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    state.race_bits[1] = 0x80003045UL;
    state.owner_technology[0] = 0;
    state.owner_technology[1] = 0;
    state.owner_technology[2] = 1;
    state.owner_technology[3] = 0;
    state.owner_technology[4] = 5;
    state.owner_technology[5] = 0;
    state.learn_player.tech_level[0] = 3;
    state.learn_player.tech_level[1] = 3;
    state.learn_player.tech_level[2] = 3;
    state.learn_player.tech_level[3] = 3;
    state.learn_player.tech_level[4] = 3;
    state.learn_player.tech_level[5] = 3;
    state.fleet = fleet;
    state.planet = planet;

    put_word(fleet, 0U, 0U);
    put_word(fleet, 2U, 1U);
    put_word(fleet, 4U, 0x0907U);
    put_word(fleet, 6U, 10U);
    put_word(fleet, 8U, 1128U);
    put_word(fleet, 10U, 1300U);
    put_word(fleet, 12U, 1U);
    put_long(fleet, 92U, 300UL);
    put_word(fleet, 118U, 0x0060U);
    put_word(order, 0U, 1128U);
    put_word(order, 2U, 1300U);
    put_word(order, 4U, 10U);
    put_word(order, 6U, 0x1105U);
    put_word(planet, 0U, 10U);
    put_word(planet, 2U, 0U);
    put_word(planet, 4U, 0x2b87U);
    put_long(planet, 0x1cU, 469UL);
    put_long(planet, 0x20U, 893UL);
    put_long(planet, 0x24U, 376UL);
    put_long(planet, 0x28U, 250UL);
    planet_extra = 0U;

    state.reprice.part_segment = v4_part_segment;
    state.reprice.part_segment_size = (StarsU16)sizeof v4_part_segment;
    state.reprice.player_technology = state.owner_technology;
    state.reprice.game_flags = &state.game_flags;
    state.reprice.race_stat = race_stat;
    state.reprice.race_grbit = race_grbit;
    state.reprice.context = &state;
    state.reprice.player = 1;
    scrap.planet_record = planet;
    scrap.planet_size = sizeof planet;
    scrap.design_records[0] = v4_high_offer_design;
    scrap.design_size = sizeof v4_high_offer_design;
    scrap.fleet_race_reprice = 1;
    scrap.planet_resource_extra = &planet_extra;
    scrap.game_flags = &state.game_flags;
    ops.raw_message = message;
    ops.scrap_learn = learn_technology;
    ops.scrap_remove_message = remove_message;
    ops.scrap_mark_techs = mark_technology;
    ops.scrap_reprice = reprice;
    ops.order_done = order_done;
    ops.context = &state;
    input.fleet_record = fleet;
    input.fleet_size = sizeof fleet;
    input.order_record = order;
    input.order_size = sizeof order;
    input.scrap_state = &scrap;
    input.pass = 1;
    input.ops = &ops;

    CHECK(stars_satisfy_orders_packed(&input) == 1);
    CHECK(state.mark_lookups == 12 && state.reprice_calls == 4);
    CHECK((state.game_flags & 0x0008U) == 0U);
    CHECK(state.message_count == 2 && state.message_codes[0] == 0x5a &&
        state.message_codes[1] == 0x13d);
    CHECK(state.message_recipients[0] == 1 &&
        state.message_recipients[1] == 0);
    CHECK(state.message_payloads[0][0] == 10 &&
        state.message_payloads[0][1] == 0);
    CHECK(state.message_payloads[1][0] == 10 &&
        state.message_payloads[1][1] == 0 &&
        state.message_payloads[1][5] == 0);
    CHECK(state.remove_calls == 1 && state.remove_saw_live == 1);
    CHECK(state.done_calls == 1 && state.done_active == 0);
    CHECK((word(fleet, 4U) & 0x0400U) != 0U &&
        (word(order, 6U) & 0x1000U) == 0U);
    CHECK(get_long(planet, 0x1cU) > 469UL &&
        get_long(planet, 0x20U) > 893UL &&
        get_long(planet, 0x24U) > 376UL);
    CHECK(strcmp(state.events, "KUUUUMTMRD") == 0);

#ifndef TARGET_NATIVE
    report = stdout;
#else
    report = fopen("C:\\SOE2S\\SOE2SRES.TXT", "w");
    if (!report) return 4;
#endif
    fprintf(report,
        "V4_LIVE_TASK5_CHECKS=%d FAILURES=%d RESULT=%s "
        "LOOKUPS=%d REPRICE=%d COST=%u,%u,%u,%u "
        "EVENTS=%s MESSAGES=%d,%d RECIPIENTS=%d,%d "
        "MINERALS=%lu,%lu,%lu TECH=%d RESEARCH0=%lu FLEET_DEAD=%d\n",
        checks, failures, failures ? "FAIL" : "PASS", state.mark_lookups,
        state.reprice_calls,
        state.repriced_cost[0], state.repriced_cost[1],
        state.repriced_cost[2], state.repriced_cost[3], state.events,
        state.message_codes[0], state.message_codes[1],
        state.message_recipients[0], state.message_recipients[1],
        (unsigned long)get_long(planet, 0x1cU),
        (unsigned long)get_long(planet, 0x20U),
        (unsigned long)get_long(planet, 0x24U), state.learn_result,
        (unsigned long)state.learn_player.research[0],
        (word(fleet, 4U) & 0x0400U) != 0U);
#ifdef TARGET_NATIVE
    fclose(report);
#endif
    return failures ? 1 : 0;
}
