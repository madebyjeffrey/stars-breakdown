#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct State {
    StarsU8 *plord;
    StarsI16 moves;
    StarsU16 id;
    StarsU16 flags;
    StarsPoint point;
    StarsI16 append;
} State;

static StarsI32 score(void *opaque, const StarsAiTargetPlanet *planet,
    StarsI16 percent_full)
{
    (void)opaque;
    return (StarsI32)planet->id * 10L - percent_full;
}

static StarsI16 move_target(void *opaque, StarsU16 id, StarsPoint point,
    StarsU16 flags)
{
    State *state;
    StarsMoveOrder order;

    state = (State *)opaque;
    ++state->moves;
    state->id = id;
    state->flags = flags;
    state->point = point;
    memset(&order, 0, sizeof(order));
    order.id = (StarsI16)id;
    order.point = point;
    order.flags = flags;
    state->append = 0;
    return stars_ai_move_order_to_plord(state->plord, 2, &order,
        state->append);
}

static void seed_first_order(StarsU8 *plord, StarsPoint point)
{
    StarsMoveOrder order;

    memset(&order, 0, sizeof(order));
    order.point = point;
    memcpy(plord + 4, &order, sizeof(order));
    plord[3] = 1U;
}

int main(void)
{
    StarsAiTargetFleet freighter;
    StarsAiTargetPlanet planets[3];
    StarsFreighterTargetOps ops;
    StarsU8 plord[STARS_AI_PLORD_HEADER_SIZE +
        2U * STARS_AI_PLORD_ORDER_SIZE];
    State state;
    StarsU16 target;
    StarsMoveOrder expected;
    StarsI16 failures;

    memset(&freighter, 0, sizeof(freighter));
    memset(planets, 0, sizeof(planets));
    memset(plord, 0, sizeof(plord));
    memset(&state, 0, sizeof(state));
    freighter.id = 1;
    freighter.planet_id = 3;
    planets[0].id = 4;
    planets[0].owner = 2;
    planets[0].point.x = 2;
    planets[1] = planets[0];
    planets[1].id = 5;
    planets[1].point.x = 9;
    ops.score_target = score;
    ops.move_target = move_target;
    state.plord = plord;
    seed_first_order(plord, planets[1].point);
    failures = stars_id_target_freighter(&freighter, planets, 2, 3, 10, 20,
        &ops, &state, &target) != 5;
    failures += target != 5 || state.moves != 1 || state.id != 5 ||
        state.flags != 0x0100U || state.point.x != 9 || state.append != 0;
    memset(&expected, 0, sizeof(expected));
    expected.id = 5;
    expected.point = planets[1].point;
    expected.flags = 0x0100U;
    failures += plord[3] != 1U || memcmp(plord + 4, &expected,
        sizeof(expected)) != 0;

    state.moves = 0;
    failures += stars_id_target_freighter(&freighter, planets, 2, 3, 0, 20,
        &ops, &state, &target) != (StarsI16)-1;
    failures += state.moves != 0 || target != 0xffffU;

    printf("AI_FREIGHTER_ACTION_PLORD_INTEGRATION_CHECKS=8 FAILURES=%d RESULT=%s\n",
        (int)failures, failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
}
