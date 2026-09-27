#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct ProductionTrace {
    StarsTechLearnPlayer tech_player;
    StarsU16 goto_id;
    StarsI16 tech_result;
    StarsI16 tech_events;
    StarsSalvageThing things[4];
    StarsSalvageThing *salvage;
    StarsI16 thing_count;
    StarsI16 done_count;
} ProductionTrace;

static StarsI32 transfer(void *context, const StarsSatisfyFleet *fleet,
    const StarsSatisfyOrder *order, StarsI16 resource, StarsI32 amount)
{
    (void)context; (void)fleet; (void)order; (void)resource;
    return amount;
}

static StarsI16 tech_random(void *context, StarsI16 limit)
{
    (void)context;
    return limit == 100 ? 60 : 0;
}

static StarsI32 tech_cost(void *context, StarsI16 technology,
    StarsI16 level, StarsI16 player)
{
    (void)context; (void)technology; (void)level; (void)player;
    return 100L;
}

static void tech_mark_event(void *context, StarsI16 player)
{
    ProductionTrace *trace = (ProductionTrace *)context;
    (void)player;
    ++trace->tech_events;
}

static StarsSalvageThing *new_salvage(void *context, StarsI16 player,
    StarsI16 kind)
{
    ProductionTrace *trace = (ProductionTrace *)context;
    StarsSalvageThing *thing;
    (void)player; (void)kind;
    if (trace->thing_count >= 4)
        return 0;
    thing = &trace->things[trace->thing_count++];
    memset(thing, 0, sizeof *thing);
    return thing;
}

static void technology(void *context, StarsSatisfyFleet *fleet,
    const StarsSatisfyOrder *order, StarsI32 amount)
{
    ProductionTrace *trace = (ProductionTrace *)context;
    StarsTechLearnInput input;
    (void)order; (void)amount;
    memset(&input, 0, sizeof input);
    input.player = &trace->tech_player;
    input.player_id = fleet->owner;
    input.message_id = -1;
    input.goto_id = &trace->goto_id;
    input.random = tech_random;
    input.tech_level_cost = tech_cost;
    input.battle_threshold[0] = 1;
    input.mark_event = tech_mark_event;
    input.context = trace;
    trace->tech_result = stars_itech_learn_a_tech(&input);
}

static void salvage(void *context, StarsSatisfyFleet *fleet,
    const StarsSatisfyOrder *order,
    const StarsI32 moved[STARS_SATISFY_RESOURCE_COUNT])
{
    ProductionTrace *trace = (ProductionTrace *)context;
    StarsDropSalvageInput input;
    StarsI16 index;
    (void)order;
    memset(&input, 0, sizeof input);
    input.thing = &trace->salvage;
    input.player = fleet->owner;
    input.point.x = 4;
    input.point.y = 5;
    input.new_thing = new_salvage;
    for (index = 0; index < 3; ++index)
        input.minerals[index] = moved[index] > 0 ?
            (StarsU32)moved[index] : 0UL;
    input.context = trace;
    stars_drop_salvage(&input);
}

static void done(void *context, StarsSatisfyFleet *fleet,
    StarsSatisfyOrder *order, StarsI16 index)
{
    ProductionTrace *trace = (ProductionTrace *)context;
    (void)fleet; (void)order; (void)index;
    ++trace->done_count;
}

static void put_word(StarsU8 *record, StarsI16 offset, StarsU16 value)
{
    record[offset] = (StarsU8)(value & 0xffU);
    record[offset + 1] = (StarsU8)(value >> 8);
}

static StarsI16 run_typed(ProductionTrace *trace)
{
    StarsSatisfyFleet fleets[2];
    StarsSatisfyOrder orders[2];
    StarsSatisfyOrdersOps ops;
    StarsSatisfyOrdersInput input;

    memset(fleets, 0, sizeof fleets);
    memset(orders, 0, sizeof orders);
    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    fleets[0].owner = 2;
    fleets[0].cargo[0] = 7;
    fleets[0].orders = &orders[0];
    fleets[0].order_count = 1;
    orders[0].kind = STARS_SATISFY_ORDER_TRANSFER;
    orders[0].target_kind = STARS_SATISFY_TARGET_PLANET;
    orders[0].target_id = 9;
    orders[0].requested[0] = 7;
    orders[0].technology_flag = 1;
    orders[0].phase_mask = 1 << 2;
    orders[0].active = 1;
    fleets[1].owner = 3;
    fleets[1].cargo[0] = 30000L;
    fleets[1].orders = &orders[1];
    fleets[1].order_count = 1;
    orders[1].kind = STARS_SATISFY_ORDER_TRANSFER;
    orders[1].target_kind = STARS_SATISFY_TARGET_POINT;
    orders[1].target_id = -1;
    orders[1].requested[0] = 30000L;
    orders[1].phase_mask = 1 << 2;
    orders[1].active = 1;
    ops.transfer = transfer;
    ops.technology_discovery = technology;
    ops.create_salvage = salvage;
    ops.order_done = done;
    ops.context = trace;
    input.fleets = fleets;
    input.fleet_count = 2;
    input.pass = 2;
    input.ops = &ops;
    stars_satisfy_orders(&input);
    if (trace->tech_result != 1 || trace->tech_player.research[0] != 100UL ||
            trace->goto_id != 0xfffeU || trace->tech_events != 1 ||
            trace->thing_count != 2 || trace->salvage != &trace->things[1] ||
            trace->things[0].value != 3000U ||
            trace->things[0].minerals[0] != 19770UL ||
            trace->things[1].minerals[0] != 10230UL ||
            trace->things[1].x != 4 || trace->things[1].y != 5 ||
            trace->done_count != 2 || orders[0].active || orders[1].active)
        return 0;
    return 1;
}

static StarsI16 run_packed(ProductionTrace *trace)
{
    StarsSatisfyPackedInput input;
    StarsSatisfyOrdersOps ops;
    StarsU8 fleet_record[124];
    StarsU8 order_record[18];

    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    memset(fleet_record, 0, sizeof fleet_record);
    memset(order_record, 0, sizeof order_record);
    put_word(fleet_record, 2, 5);
    put_word(order_record, 4, 11);
    put_word(order_record, 6, (StarsU16)(STARS_SATISFY_ORDER_TRANSFER |
        (STARS_SATISFY_TARGET_PLANET << 8) | (1U << 12)));
    input.fleet_record = fleet_record;
    input.fleet_size = 124;
    input.order_record = order_record;
    input.order_size = 18;
    input.cargo[0] = 7;
    input.requested[0] = 7;
    input.phase_mask = 1 << 2;
    input.pass = 2;
    input.technology_flag = 1;
    ops.transfer = transfer;
    ops.technology_discovery = technology;
    ops.context = trace;
    input.ops = &ops;
    if (!stars_satisfy_orders_packed(&input) || trace->tech_result != 1 ||
            trace->tech_player.research[0] != 100UL ||
            trace->tech_events != 1 || input.cargo[0] != 0 ||
            input.requested[0] != 0)
        return 0;
    return 1;
}

int satisfy_orders_production_e2_probe(void)
{
    ProductionTrace trace;
    StarsI16 typed_ok;
    StarsI16 packed_ok;
    memset(&trace, 0, sizeof trace);
    typed_ok = run_typed(&trace);
    memset(&trace, 0, sizeof trace);
    packed_ok = run_packed(&trace);
#ifdef TARGET_NATIVE
    {
        FILE *report = fopen("SYOE8.TXT", "w");
        if (report) {
            fprintf(report,
                "SATISFY_ORDERS_PRODUCTION_E2_CHECKS=18 TYPED=%d PACKED=%d FAILURES=%d RESULT=%s\n",
                typed_ok ? 12 : 0, packed_ok ? 6 : 0,
                (typed_ok && packed_ok) ? 0 : 1,
                (typed_ok && packed_ok) ? "PASS" : "FAIL");
            fclose(report);
        }
    }
#else
    printf("SATISFY_ORDERS_PRODUCTION_E2_CHECKS=18 TYPED=%d PACKED=%d FAILURES=%d RESULT=%s\n",
        typed_ok ? 12 : 0, packed_ok ? 6 : 0,
        (typed_ok && packed_ok) ? 0 : 1,
        (typed_ok && packed_ok) ? "PASS" : "FAIL");
#endif
    return (typed_ok && packed_ok) ? 0 : 1;
}

#ifndef TARGET_NATIVE
int main(void)
{
    return satisfy_orders_production_e2_probe();
}
#endif
