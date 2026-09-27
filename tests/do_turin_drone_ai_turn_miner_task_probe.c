#include "core.h"
#include <string.h>

static StarsI16 check_count;
static StarsI16 failure_count;

#define CHECK(condition) do { ++check_count; if (!(condition)) \
    ++failure_count; } while (0)

StarsI16 turin_miner_task_probe(void)
{
    StarsTurinDroneAiTurnInput input;
    StarsTurinDroneAiTurnOps ops;
    StarsTurinTurnFleet fleet;
    StarsU8 plord[STARS_AI_PLORD_HEADER_SIZE + STARS_AI_PLORD_ORDER_SIZE];
    StarsU8 before[STARS_AI_PLORD_HEADER_SIZE + STARS_AI_PLORD_ORDER_SIZE];
    StarsU16 index;

    check_count = 0;
    failure_count = 0;
    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    memset(&fleet, 0, sizeof fleet);
    for (index = 0; index < sizeof plord; ++index)
        plord[index] = (StarsU8)(index * 7U + 3U);
    plord[3] = 1U;
    plord[10] = 0x00U;
    plord[11] = 0x11U;
    memcpy(before, plord, sizeof plord);

    fleet.id = 2;
    fleet.owner = 1;
    fleet.order_kind = 0;
    fleet.planet_id = 24;
    fleet.cord = 1;
    fleet.ship_counts[2] = 2;
    fleet.raw_plord = plord;
    input.turn = 0;
    input.player_id = 1;
    input.fleet_count = 1;
    input.fleets = &fleet;
    input.ops = &ops;

    CHECK(stars_do_turin_drone_ai_turn(&input) == 1);
    CHECK((StarsU16)(plord[10] | ((StarsU16)plord[11] << 8)) == 0x1105U);
    for (index = 0; index < sizeof plord; ++index) {
        if (index == 10U)
            CHECK(plord[index] == 0x05U);
        else
            CHECK(plord[index] == before[index]);
    }
    return failure_count;
}

StarsI16 turin_miner_task_checks(void)
{
    return check_count;
}

StarsI16 turin_miner_task_failures(void)
{
    return failure_count;
}

#ifndef TURIN_MINER_TASK_NATIVE
int main(void)
{
    StarsI16 failures;

    failures = turin_miner_task_probe();
    if (failures)
        return 1;
    return 0;
}
#endif
