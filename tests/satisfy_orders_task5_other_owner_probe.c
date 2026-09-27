#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct OtherOwnerReplay {
    StarsU8 fleets[4][124];
    StarsU8 orders[4][18];
    StarsSatisfyPackedInput packed[4];
    StarsSatisfyOrdersOps ops;
    StarsOrdersPlanet planets[2];
    StarsDoOrdersInput turn;
} OtherOwnerReplay;

static void put_word(StarsU8 *bytes, StarsU16 at, StarsU16 value)
{
    bytes[at] = (StarsU8)value;
    bytes[at + 1U] = (StarsU8)(value >> 8);
}

static StarsU16 word(const StarsU8 *bytes, StarsU16 at)
{
    return (StarsU16)(bytes[at] | ((StarsU16)bytes[at + 1U] << 8));
}

static void satisfy_owner_fleets(void *context, StarsI16 pass)
{
    OtherOwnerReplay *replay;
    StarsI16 index;

    replay = (OtherOwnerReplay *)context;
    for (index = 0; index < 4; ++index) {
        replay->packed[index].pass = pass;
        stars_satisfy_orders_packed(&replay->packed[index]);
    }
}

int main(void)
{
    static const StarsI32 fuel[4] = {300, 200, 210, 210};
    static const StarsU16 design[4] = {0, 1, 2, 2};
    OtherOwnerReplay replay;
    StarsI16 index;

    memset(&replay, 0, sizeof replay);
    for (index = 0; index < 4; ++index) {
        StarsU8 *fleet;
        StarsU8 *order;
        StarsI16 resource;

        fleet = replay.fleets[index];
        order = replay.orders[index];
        put_word(fleet, 0U, (StarsU16)(0x0200U | (StarsU16)index));
        put_word(fleet, 2U, 1U);
        put_word(fleet, 4U, 0x0907U);
        put_word(fleet, 6U, 24U);
        put_word(fleet, 8U, 1290U);
        put_word(fleet, 10U, 1258U);
        put_word(fleet, 12U, (StarsU16)(1U << design[index]));
        put_word(fleet, 14U, 1U);
        put_word(order, 0U, 1290U);
        put_word(order, 2U, 1258U);
        put_word(order, 4U, 24U);
        put_word(order, 6U, 0x1100U);
        replay.packed[index].fleet_record = fleet;
        replay.packed[index].fleet_size = sizeof replay.fleets[index];
        replay.packed[index].order_record = order;
        replay.packed[index].order_size = sizeof replay.orders[index];
        replay.packed[index].planet_id = 24;
        replay.packed[index].ops = &replay.ops;
        replay.packed[index].cargo[4] = fuel[index];
        for (resource = 0; resource < STARS_SATISFY_RESOURCE_COUNT;
                ++resource)
            replay.packed[index].capacity[resource] = 0x7fffffffL;
    }

    replay.planets[0].id = 10;
    replay.planets[0].player = 0;
    replay.planets[0].flags = 0x2b87U;
    replay.planets[1].id = 24;
    replay.planets[1].player = 1;
    replay.planets[1].flags = 0x2b87U;
    replay.turn.planets = replay.planets;
    replay.turn.planet_count = 2;
    replay.turn.satisfy_orders = satisfy_owner_fleets;
    replay.turn.context = &replay;
    stars_do_orders(0, &replay.turn);
    stars_do_orders(1, &replay.turn);

    for (index = 0; index < 4; ++index) {
        StarsI16 design_index;
        StarsU16 bitmap;

        bitmap = word(replay.fleets[index], 12U);
        printf("P1_SOURCE id=%u owner=%u planet=%u point=%u,%u flags=%04x ",
            word(replay.fleets[index], 0U) & 0x01ffU,
            word(replay.fleets[index], 2U),
            word(replay.fleets[index], 6U),
            word(replay.fleets[index], 8U),
            word(replay.fleets[index], 10U),
            word(replay.fleets[index], 4U));
        printf("SHIPS=");
        for (design_index = 0; design_index < 16; ++design_index) {
            StarsU16 count;
            count = (bitmap & (1U << design_index)) != 0U ?
                word(replay.fleets[index], 14U) : 0U;
            printf("%s%u", design_index ? "," : "", count);
        }
        printf(" CARGO=%ld,%ld,%ld,%ld,%ld ORDER=%u,%u,%u,%u\n",
            (long)replay.packed[index].cargo[0],
            (long)replay.packed[index].cargo[1],
            (long)replay.packed[index].cargo[2],
            (long)replay.packed[index].cargo[3],
            (long)replay.packed[index].cargo[4],
            word(replay.orders[index], 0U),
            word(replay.orders[index], 2U),
            word(replay.orders[index], 4U),
            word(replay.orders[index], 6U));
    }
    return 0;
}
