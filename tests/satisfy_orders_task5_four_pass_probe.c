#include "core.h"
#include "common.h"
#include "SATISFYFX.H"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Replay {
    StarsU8 fleet[124];
    StarsU8 order[18];
    StarsU8 planet[60];
    StarsU8 design[147];
    StarsSatisfyScrapPackedState scrap;
    StarsSatisfyPackedInput packed;
    StarsSatisfyOrdersOps ops;
    StarsU8 other_fleets[5][124];
    StarsU8 other_orders[5][18];
    StarsSatisfyPackedInput other_packed[5];
    StarsI16 passes[4];
    StarsI16 pass_count;
    StarsI16 done_count;
    StarsI16 message_count;
    StarsI16 code[3];
    StarsI16 recipient[3];
    StarsI16 payload[3][8];
    StarsI16 production_message_count;
    StarsI16 production_message_recipient;
    StarsI16 production_message_code;
    StarsI16 production_message_payload[3];
    StarsI16 mining_draw_count;
    StarsI16 mining_witness[3];
    StarsI32 mined[3];
    StarsI32 population_growth;
} Replay;

typedef struct FailureTrace {
    StarsI16 calls;
    StarsI16 recipient;
    StarsI16 code;
    StarsI16 payload[8];
} FailureTrace;

static StarsU16 word(const StarsU8 *bytes, StarsU16 at)
{
    return (StarsU16)(bytes[at] | ((StarsU16)bytes[at + 1U] << 8));
}

static StarsU32 long_at(const StarsU8 *bytes, StarsU16 at)
{
    return (StarsU32)bytes[at] | ((StarsU32)bytes[at + 1U] << 8) |
        ((StarsU32)bytes[at + 2U] << 16) |
        ((StarsU32)bytes[at + 3U] << 24);
}

static void failure_message(void *context, StarsI16 recipient,
    StarsI16 code, const StarsI16 payload[8])
{
    FailureTrace *trace;
    trace = (FailureTrace *)context;
    ++trace->calls;
    trace->recipient = recipient;
    trace->code = code;
    memcpy(trace->payload, payload, sizeof trace->payload);
}

static void put_long(StarsU8 *bytes, StarsU16 at, StarsU32 value)
{
    bytes[at] = (StarsU8)value;
    bytes[at + 1U] = (StarsU8)(value >> 8);
    bytes[at + 2U] = (StarsU8)(value >> 16);
    bytes[at + 3U] = (StarsU8)(value >> 24);
}

static void raw_message(void *context, StarsI16 recipient, StarsI16 code,
    const StarsI16 payload[8])
{
    Replay *replay;
    StarsI16 index;
    replay = (Replay *)context;
    index = replay->message_count;
    if (index < 3) {
        replay->recipient[index] = recipient;
        replay->code[index] = code;
        memcpy(replay->payload[index], payload,
            sizeof replay->payload[index]);
    }
    ++replay->message_count;
}

static void production_message(void *context, StarsI16 recipient,
    StarsI16 code, StarsI16 object, StarsI16 planet, StarsI16 value)
{
    Replay *replay;
    StarsI16 index;
    replay = (Replay *)context;
    index = replay->message_count;
    if (index < 3) {
        replay->recipient[index] = recipient;
        replay->code[index] = code;
        replay->payload[index][0] = object;
        replay->payload[index][1] = planet;
        replay->payload[index][2] = value;
    }
    ++replay->production_message_count;
    replay->production_message_recipient = recipient;
    replay->production_message_code = code;
    replay->production_message_payload[0] = object;
    replay->production_message_payload[1] = planet;
    replay->production_message_payload[2] = value;
    ++replay->message_count;
}

static StarsI16 mining_random(void *context, StarsI16 range)
{
    Replay *replay;
    StarsI16 index;
    replay = (Replay *)context;
    index = replay->mining_draw_count++;
    if (range != 100 || index >= 3)
        return 99;
    return replay->mining_witness[index];
}

static void mine_planet10(void *context)
{
    Replay *replay;
    StarsMineralMinedInput mining;
    StarsMineralMinedOps ops;
    StarsI32 weights[3];
    StarsI16 index;
    replay = (Replay *)context;
    memset(&mining, 0, sizeof mining);
    memset(&ops, 0, sizeof ops);
    for (index = 0; index < 3; ++index) {
        mining.concentration[index] = replay->planet[9U + index];
        mining.percent_level[index] = replay->planet[6U + index];
        weights[index] = (StarsI32)long_at(replay->planet,
            (StarsU16)(0x1cU + index * 4U));
    }
    mining.player = 0;
    mining.efficiency_tenths = 10;
    mining.operating_mines = 10;
    mining.c_mines = -1;
    mining.random_rounding = 1;
    mining.apply = 1;
    mining.mineral_weight = weights;
    mining.ops = &ops;
    ops.random = mining_random;
    ops.context = replay;
    stars_est_minerals_mined(&mining);
    for (index = 0; index < 3; ++index) {
        replay->mined[index] = mining.output[index];
        replay->planet[6U + index] = mining.percent_level[index];
        replay->planet[9U + index] = mining.concentration[index];
        put_long(replay->planet, (StarsU16)(0x1cU + index * 4U),
            (StarsU32)weights[index]);
    }
}

static void grow_planet10(void *context, StarsProducePlanet *planet)
{
    Replay *replay;
    StarsPopulationState state;
    replay = (Replay *)context;
    if (planet->id != 10 || planet->owner != 0)
        return;
    state.owner = 0;
    state.desirability = 100;
    state.population = (StarsI32)long_at(replay->planet, 0x28U);
    state.fraction = replay->planet[0x14U];
    /* Original player 0: perfect 50/50/50 environment, 15% ideal growth,
     * raNone capacity bonus makes this planet's maximum 12,000. */
    replay->population_growth = stars_change_population(&state,
        12000L, 15, 0, 1);
    put_long(replay->planet, 0x28U, (StarsU32)state.population);
    replay->planet[0x14U] = state.fraction;
}

static void done(void *context, StarsSatisfyFleet *fleet,
    StarsSatisfyOrder *order, StarsI16 index)
{
    Replay *replay;
    (void)fleet;
    (void)order;
    (void)index;
    replay = (Replay *)context;
    ++replay->done_count;
}

static void satisfy(void *context, StarsI16 pass)
{
    Replay *replay;
    StarsI16 index;
    replay = (Replay *)context;
    if (replay->pass_count < 4)
        replay->passes[replay->pass_count] = pass;
    ++replay->pass_count;
    replay->packed.pass = pass;
    stars_satisfy_orders_packed(&replay->packed);
    for (index = 0; index < 5; ++index) {
        replay->other_packed[index].pass = pass;
        stars_satisfy_orders_packed(&replay->other_packed[index]);
    }
}

#ifdef TARGET_NATIVE
int main(void)
#else
int main(int argc, char **argv)
#endif
{
    Replay replay;
    StarsDoOrdersInput turn;
    StarsOrdersPlanet turn_planet;
    StarsProducePlanet production_planet;
    StarsProduceInput production;
    StarsProduceOps production_ops;
    StarsSatisfyFailureFrame failure_frame;
    FailureTrace failure;
    StarsI16 index;
    StarsI16 control_mode;
    FILE *report;

    memset(&replay, 0, sizeof replay);
    memset(&turn, 0, sizeof turn);
    memset(&turn_planet, 0, sizeof turn_planet);
    memset(&production_planet, 0, sizeof production_planet);
    memset(&production, 0, sizeof production);
    memset(&production_ops, 0, sizeof production_ops);
    memset(&failure_frame, 0, sizeof failure_frame);
    memset(&failure, 0, sizeof failure);
#ifdef TARGET_NATIVE
    control_mode = 0;
    replay.mining_witness[0] = 0;
    replay.mining_witness[1] = 99;
    replay.mining_witness[2] = 0;
#else
    if (argc != 1 && argc != 2 && argc != 5)
        return 2;
    control_mode = (StarsI16)(argc > 1 && strcmp(argv[1], "control") == 0);
    replay.mining_witness[0] = 0;
    replay.mining_witness[1] = control_mode ? 0 : 99;
    replay.mining_witness[2] = 0;
    if (argc == 5) {
        replay.mining_witness[0] = (StarsI16)atoi(argv[2]);
        replay.mining_witness[1] = (StarsI16)atoi(argv[3]);
        replay.mining_witness[2] = (StarsI16)atoi(argv[4]);
    }
#endif
    memcpy(replay.fleet, retained_input_fleets[0], sizeof replay.fleet);
#ifdef RETAINED_CONTROL_ORDERS
    memcpy(replay.order, control_mode ? retained_control_orders[0] :
        retained_input_orders[0], sizeof replay.order);
#else
    memcpy(replay.order, retained_input_orders[0], sizeof replay.order);
#endif
    memcpy(replay.planet, retained_input_planet, sizeof replay.planet);
    memcpy(replay.design, retained_input_design, sizeof replay.design);
    replay.scrap.planet_record = replay.planet;
    replay.scrap.planet_size = sizeof replay.planet;
    replay.scrap.design_records[0] = replay.design;
    replay.scrap.design_size = sizeof replay.design;
    replay.packed.fleet_record = replay.fleet;
    replay.packed.fleet_size = sizeof replay.fleet;
    replay.packed.order_record = replay.order;
    replay.packed.order_size = sizeof replay.order;
    replay.packed.scrap_state = control_mode ? (StarsSatisfyScrapPackedState *)0 :
        &replay.scrap;
    replay.packed.ops = &replay.ops;
    replay.ops.context = &replay;
    replay.ops.raw_message = raw_message;
    replay.ops.order_done = done;
    for (index = 0; index < 5; ++index) {
        memcpy(replay.other_fleets[index],
            retained_input_fleets[index + 1],
            sizeof replay.other_fleets[index]);
#ifdef RETAINED_CONTROL_ORDERS
        memcpy(replay.other_orders[index], control_mode ?
            retained_control_orders[index + 1] :
            retained_input_orders[index + 1],
            sizeof replay.other_orders[index]);
#else
        memcpy(replay.other_orders[index],
            retained_input_orders[index + 1],
            sizeof replay.other_orders[index]);
#endif
        replay.other_packed[index].fleet_record = replay.other_fleets[index];
        replay.other_packed[index].fleet_size = 124U;
        replay.other_packed[index].order_record = replay.other_orders[index];
        replay.other_packed[index].order_size = 18U;
        replay.other_packed[index].planet_id = 10;
        replay.other_packed[index].ops = &replay.ops;
    }
    turn_planet.id = RETAINED_PLANET_ID;
    turn_planet.player = RETAINED_PLANET_OWNER;
    turn_planet.flags = RETAINED_PLANET_FLAGS;
    turn.planets = &turn_planet;
    turn.planet_count = 1;
    turn.satisfy_orders = satisfy;
    turn.context = &replay;
    stars_do_orders(0, &turn);
    /* Original Produce runs between DoOrders(0) and DoOrders(1). Planet 10
     * has a null production queue in the verified turn-0 input. */
    production_planet.id = RETAINED_PLANET_ID;
    production_planet.owner = RETAINED_PLANET_OWNER;
    production.planets = &production_planet;
    production.planet_count = 1U;
    production.ops = &production_ops;
    production_ops.context = &replay;
    production_ops.mine_minerals = mine_planet10;
    production_ops.send_message = production_message;
    production_ops.update_populations = grow_planet10;
    stars_produce(&production);
    stars_do_orders(1, &turn);

    /* Keep one recovered FSendPlrMsg failure frame beside the complete-turn
     * trace so the same host/native packet checks its exact raw arguments. */
    failure_frame.variant = STARS_SATISFY_FAILURE_SET_AMOUNT;
    failure_frame.recipient = 2;
    failure_frame.fleet_id = 11;
    failure_frame.resource = 3;
    failure_frame.requested = 27;
    failure_frame.target_word = 0x0123;
    failure_frame.target_x = 14;
    stars_satisfy_raw_failure_message(&failure_frame, failure_message, 0,
        &failure);

#ifndef TARGET_NATIVE
    report = stdout;
#else
    report = fopen("C:\\SOE3A\\SOE3RES.TXT", "w");
    if (!report)
        return 4;
#endif
    fprintf(report, "PASSES=%d,%d,%d,%d\n", replay.passes[0], replay.passes[1],
        replay.passes[2], replay.passes[3]);
    fprintf(report, "DONE=%d ORDER0=%d DEAD0=%d\n", replay.done_count,
        (word(replay.order, 6U) & 0x1000U) != 0U,
        (word(replay.fleet, 4U) & 0x0400U) != 0U);
    fprintf(report, "PLANET=%u OWNER=%u MINERALS=%u,%u,%u COLONISTS=%u\n",
        word(replay.planet, 0U), word(replay.planet, 2U),
        word(replay.planet, 0x1cU), word(replay.planet, 0x20U),
        word(replay.planet, 0x24U), word(replay.planet, 0x28U));
    fprintf(report, "MESSAGES=%d IDS=%d,%d,%d RECIPIENTS=%d,%d,%d\n",
        replay.message_count, replay.code[0], replay.code[1],
        replay.code[2], replay.recipient[0], replay.recipient[1],
        replay.recipient[2]);
    fprintf(report, "ARGS=%d,%d,%d,%d,%d;%d,%d,%d,%d,%d\n",
        replay.payload[0][0], replay.payload[0][1],
        replay.payload[0][2], replay.payload[0][3],
        replay.payload[0][4], replay.payload[1][0],
        replay.payload[1][1], replay.payload[1][2],
        replay.payload[1][3], replay.payload[1][4]);
    fprintf(report, "PRODUCTION_ARGS=%d,%d,%d\n",
        replay.production_message_payload[0],
        replay.production_message_payload[1],
        replay.production_message_payload[2]);
    fprintf(report, "MINING=%ld,%ld,%ld DRAWS=%d LEVELS=%u,%u,%u CONC=%u,%u,%u\n",
        (long)replay.mined[0], (long)replay.mined[1],
        (long)replay.mined[2], replay.mining_draw_count,
        replay.planet[6], replay.planet[7], replay.planet[8],
        replay.planet[9], replay.planet[10], replay.planet[11]);
    fprintf(report, "POPULATION_GROWTH=%ld FRACTION=%u\n",
        (long)replay.population_growth, replay.planet[0x14U]);
    for (index = 0; index < 5; ++index) {
        StarsI16 slot;
        fprintf(report, "FLEET=%u OWNER=%u PLANET=%u POINT=%u,%u "
            "SHIPS=",
            word(replay.other_fleets[index], 0U),
            word(replay.other_fleets[index], 2U),
            word(replay.other_fleets[index], 6U),
            word(replay.other_fleets[index], 8U),
            word(replay.other_fleets[index], 10U));
        for (slot = 0; slot < 16; ++slot)
            fprintf(report, "%s%u", slot ? "," : "",
                word(replay.other_fleets[index],
                    (StarsU16)(12U + slot * 2U)));
        fprintf(report, " CARGO=%lu,%lu,%lu,%lu,%lu ORDER=%u,%u,%u,%u\n",
            (unsigned long)long_at(replay.other_fleets[index], 76U),
            (unsigned long)long_at(replay.other_fleets[index], 80U),
            (unsigned long)long_at(replay.other_fleets[index], 84U),
            (unsigned long)long_at(replay.other_fleets[index], 88U),
            (unsigned long)long_at(replay.other_fleets[index], 92U),
            word(replay.other_orders[index], 0U),
            word(replay.other_orders[index], 2U),
            word(replay.other_orders[index], 4U),
            word(replay.other_orders[index], 6U));
    }
    fprintf(report, "FAILURE_MESSAGE=%04x RECIPIENT=%d ARGS=",
        (unsigned)(StarsU16)failure.code, failure.recipient);
    for (index = 0; index < 8; ++index)
        fprintf(report, "%s%04x", index ? "," : "",
            (unsigned)(StarsU16)failure.payload[index]);
    fprintf(report, "\n");
    if (report != stdout)
        fclose(report);
    return replay.pass_count == 4 &&
        replay.done_count == (control_mode ? 0 : 1) &&
        replay.message_count == (control_mode ? 1 : 3) &&
        failure.calls == 1 &&
        failure.code == STARS_SATISFY_MESSAGE_SET_AMOUNT ? 0 : 1;
}
