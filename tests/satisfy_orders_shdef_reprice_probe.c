#include "core.h"
#include "fixtures/satisfy_orders_design0_parts.h"
#include <stdio.h>
#include <string.h>

static unsigned char segment[0x2cd0];
static int checks;
static int failures;
static int reprice_calls;
static int message_count;
static StarsI16 message_codes[2];
#define CHECK(test) do { ++checks; if (!(test)) ++failures; } while (0)

static void put_word(StarsU8 *bytes, StarsU16 at, StarsU16 value)
{
    bytes[at] = (StarsU8)value;
    bytes[at + 1U] = (StarsU8)(value >> 8);
}

static StarsU16 word(const StarsU8 *bytes, StarsU16 at)
{
    return (StarsU16)(bytes[at] | ((StarsU16)bytes[at + 1U] << 8));
}

static StarsI16 race_stat(void *context, StarsI16 player, StarsI16 stat)
{
    (void)context;
    CHECK(player == 0 && stat == 0x0e);
    return 9;
}

static StarsI16 race_grbit(void *context, StarsI16 player, StarsI16 bit)
{
    (void)context;
    CHECK(player == 0);
    (void)bit;
    return 0;
}

static StarsI16 task5_reprice(void *context, const StarsU8 *source,
    StarsU8 *destination)
{
    StarsShdefRepriceInput *input;
    input = (StarsShdefRepriceInput *)context;
    ++reprice_calls;
    return stars_reprice_shdef_cost(input, source, 147U, destination);
}

static void task5_message(void *context, StarsI16 recipient,
    StarsI16 code, const StarsI16 payload[8])
{
    (void)context;
    CHECK(recipient == 0 && message_count < 2 && payload[0] == 10);
    if (message_count < 2)
        message_codes[message_count] = code;
    ++message_count;
}

int main(void)
{
    StarsShdefRepriceInput input;
    StarsU8 source[147];
    StarsU8 result[147];
    signed char technology[6];
    StarsU16 flags;
    StarsU8 fleet[124];
    StarsU8 order[18];
    StarsU8 planet[60];
    StarsSatisfyScrapPackedState scrap;
    StarsSatisfyPackedInput task5;
    StarsSatisfyOrdersOps ops;
    FILE *report;

    memset(segment, 0, sizeof segment);
    memcpy(segment + 0x2c2cU, original_part_0, sizeof original_part_0);
    memcpy(segment + 0x00eaU, original_part_1, sizeof original_part_1);
    memcpy(segment + 0x07a0U, original_part_2, sizeof original_part_2);
    memcpy(segment + 0x248cU, original_part_3, sizeof original_part_3);
    memset(source, 0, sizeof source);
    memset(result, 0xa5, sizeof result);
    memset(technology, 3, sizeof technology);
    put_word(source, 0U, 4U);
    put_word(source, 0x2aU, 24U);
    put_word(source, 0x2cU, 12U);
    put_word(source, 0x2eU, 8U);
    put_word(source, 0x30U, 7U);
    put_word(source, 0x3aU, 1U);
    put_word(source, 0x3cU, 0x0103U);
    put_word(source, 0x3eU, 2U);
    put_word(source, 0x40U, 0x0101U);
    put_word(source, 0x42U, 0x10U);
    put_word(source, 0x44U, 0x0101U);
    source[0x7aU] = 3U;
    flags = 0;
    memset(&input, 0, sizeof input);
    input.part_segment = segment;
    input.part_segment_size = sizeof segment;
    input.player_technology = technology;
    input.game_flags = &flags;
    input.race_stat = race_stat;
    input.race_grbit = race_grbit;
    input.player = 0;
    CHECK(stars_reprice_shdef_cost(&input, source, sizeof source,
        result) == 1);
    CHECK(word(result, 0x2aU) == 24U &&
        word(result, 0x2cU) == 12U &&
        word(result, 0x2eU) == 8U &&
        word(result, 0x30U) == 7U);
    CHECK(result[0x7aU] == 3U && word(result, 0x42U) == 0x10U);
    CHECK(flags == 0U);

    memset(result, 0xa5, sizeof result);
    source[0x7aU] = 17U;
    CHECK(stars_reprice_shdef_cost(&input, source, sizeof source,
        result) == 0);
    CHECK(result[0] == 0xa5U && result[0x2aU] == 0xa5U);
    source[0x7aU] = 3U;
    put_word(source, 0x3eU, 0x200U);
    CHECK(stars_reprice_shdef_cost(&input, source, sizeof source,
        result) == 0);
    CHECK(result[0] == 0xa5U && flags == 0U);
    put_word(source, 0x3eU, 2U);

    /* Invoke that same production callback from the packed task-5 branch.
     * The retained owner-0 record has bit 12 clear, so this deliberately
     * enables the repricing branch as an isolated service-binding check. */
    input.part_segment_size = sizeof segment;
    memset(fleet, 0, sizeof fleet);
    memset(order, 0, sizeof order);
    memset(planet, 0, sizeof planet);
    memset(&scrap, 0, sizeof scrap);
    memset(&task5, 0, sizeof task5);
    memset(&ops, 0, sizeof ops);
    put_word(fleet, 0U, 0U);
    put_word(fleet, 2U, 0U);
    put_word(fleet, 4U, 0x0907U);
    put_word(fleet, 6U, 10U);
    put_word(fleet, 8U, 1128U);
    put_word(fleet, 10U, 1300U);
    put_word(fleet, 12U, 1U);
    put_word(fleet, 118U, 0x0060U);
    put_word(order, 0U, 1128U);
    put_word(order, 2U, 1300U);
    put_word(order, 4U, 10U);
    put_word(order, 6U, 0x1105U);
    put_word(planet, 0U, 10U);
    put_word(planet, 2U, 0U);
    put_word(planet, 4U, 0x2b87U);
    put_word(planet, 0x1cU, 469U);
    put_word(planet, 0x20U, 893U);
    put_word(planet, 0x24U, 376U);
    scrap.planet_record = planet;
    scrap.planet_size = sizeof planet;
    scrap.design_records[0] = source;
    scrap.design_size = sizeof source;
    scrap.fleet_race_reprice = 1;
    scrap.game_flags = &flags;
    ops.scrap_reprice = task5_reprice;
    ops.raw_message = task5_message;
    ops.context = &input;
    task5.fleet_record = fleet;
    task5.fleet_size = sizeof fleet;
    task5.order_record = order;
    task5.order_size = sizeof order;
    task5.scrap_state = &scrap;
    task5.pass = 1;
    task5.ops = &ops;
    CHECK(stars_satisfy_orders_packed(&task5) == 1);
    CHECK(reprice_calls == 4 && message_count == 2);
    CHECK(flags == 0U);
    CHECK(message_codes[0] == 0x5a && message_codes[1] == 0x141);
    CHECK(word(planet, 0x1cU) == 478U && word(planet, 0x20U) == 899U &&
        word(planet, 0x24U) == 381U);
    CHECK((word(fleet, 4U) & 0x0400U) != 0U &&
        (word(order, 6U) & 0x1000U) == 0U);
    input.part_segment_size = 0x1000U;
    CHECK(stars_reprice_shdef_cost(&input, source, sizeof source,
        result) == 0);
    CHECK(result[0] == 0xa5U && flags == 0U);

#ifndef TARGET_NATIVE
    report = stdout;
#else
    report = fopen("C:\\SOE2M\\SOE2MRES.TXT", "w");
    if (!report) return 4;
#endif
    fprintf(report, "SHDEF_REPRICE_CHECKS=%d FAILURES=%d RESULT=%s "
        "COST=24,12,8,7 TASK5=478,899,381 MSG=90,321\n", checks, failures,
        failures ? "FAIL" : "PASS");
#ifdef TARGET_NATIVE
    fclose(report);
#endif
    return failures ? 1 : 0;
}
