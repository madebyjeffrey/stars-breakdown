#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct CleanReplay {
    StarsSatisfyFleet fleets[6];
    StarsSatisfyOrder orders[6];
    StarsSatisfyOrdersOps ops;
    StarsI16 passes[4];
    StarsI16 pass_count;
    StarsI16 done_count;
    StarsI16 message_count;
} CleanReplay;

static void satisfy(void *context, StarsI16 pass)
{
    CleanReplay *replay;
    StarsSatisfyOrdersInput input;
    replay = (CleanReplay *)context;
    if (replay->pass_count < 4)
        replay->passes[replay->pass_count] = pass;
    ++replay->pass_count;
    memset(&input, 0, sizeof input);
    input.fleets = replay->fleets;
    input.fleet_count = 6;
    input.pass = pass;
    input.ops = &replay->ops;
    stars_satisfy_orders(&input);
}

static void order_done(void *context, StarsSatisfyFleet *fleet,
    StarsSatisfyOrder *order, StarsI16 index)
{
    CleanReplay *replay;
    (void)fleet;
    (void)order;
    (void)index;
    replay = (CleanReplay *)context;
    ++replay->done_count;
}

static void send_message(void *context, StarsI16 recipient, StarsI16 code,
    StarsI16 fleet, StarsI16 target, StarsI32 amount)
{
    CleanReplay *replay;
    (void)recipient;
    (void)code;
    (void)fleet;
    (void)target;
    (void)amount;
    replay = (CleanReplay *)context;
    ++replay->message_count;
}

int main(int argc, char **argv)
{
    CleanReplay replay;
    StarsOrdersPlanet planet;
    StarsDoOrdersInput input;
    StarsI32 fuel[6] = {50, 300, 200, 450, 530, 210};
    StarsI16 index;
    StarsI16 task5;

    task5 = argc == 2 && strcmp(argv[1], "--task5") == 0;

    memset(&replay, 0, sizeof replay);
    memset(&planet, 0, sizeof planet);
    memset(&input, 0, sizeof input);
    for (index = 0; index < 6; ++index) {
        replay.fleets[index].id = index;
        replay.fleets[index].owner = 0;
        replay.fleets[index].planet_id = 10;
        replay.fleets[index].flags = 0x0907U;
        replay.fleets[index].cargo[STARS_SATISFY_RESOURCE_FUEL] = fuel[index];
        replay.fleets[index].orders = &replay.orders[index];
        replay.fleets[index].order_count = 1;
        /* Original compact order word 0x1100: valid, planet, task zero. */
        replay.orders[index].kind = 0;
        replay.orders[index].target_kind = STARS_SATISFY_TARGET_PLANET;
        replay.orders[index].target_id = 10;
        replay.orders[index].active = 1;
    }
    if (task5)
        replay.orders[0].kind = STARS_SATISFY_ORDER_SCRAP;
    replay.ops.context = &replay;
    replay.ops.order_done = order_done;
    replay.ops.send_message = send_message;
    planet.id = 10;
    planet.player = 0;
    planet.flags = 0x2b87U;
    input.planets = &planet;
    input.planet_count = 1;
    input.satisfy_orders = satisfy;
    input.context = &replay;
    stars_do_orders(0, &input);
    stars_do_orders(1, &input);
    printf("PASSES=%d,%d,%d,%d DONE=%d MESSAGES=%d\n",
        replay.passes[0], replay.passes[1], replay.passes[2],
        replay.passes[3], replay.done_count, replay.message_count);
    for (index = 0; index < 6; ++index)
        printf("FLEET=%d OWNER=%d PLANET=%d FUEL=%ld ORDER=%d ACTIVE=%d\n",
            replay.fleets[index].id, replay.fleets[index].owner,
            replay.fleets[index].planet_id,
            (long)replay.fleets[index].cargo[STARS_SATISFY_RESOURCE_FUEL],
            replay.orders[index].kind, replay.orders[index].active);
    return replay.pass_count == 4 && replay.done_count == 0 &&
        replay.message_count == 0 ? 0 : 1;
}
