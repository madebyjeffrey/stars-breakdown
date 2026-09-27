#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct State {
    StarsU8 *plord;
    StarsI16 starbase;
    StarsPoint point;
    StarsI16 find_calls;
    StarsI16 point_calls;
    StarsI16 move_calls;
    StarsI16 append;
    StarsMoveOrder order;
} State;

static StarsI16 find_closest(void *opaque, const StarsU8 *fleet,
    StarsI16 big_starbase)
{
    State *state;
    (void)fleet;
    (void)big_starbase;
    state = (State *)opaque;
    ++state->find_calls;
    return state->starbase;
}

static void get_point(void *opaque, StarsI16 id, StarsPoint *point)
{
    State *state;
    (void)id;
    state = (State *)opaque;
    ++state->point_calls;
    *point = state->point;
}

static StarsI16 move_fleet(void *opaque, const StarsU8 *fleet,
    const StarsMoveOrder *order, StarsI16 append)
{
    State *state;
    (void)fleet;
    state = (State *)opaque;
    ++state->move_calls;
    state->order = *order;
    state->append = append;
    return stars_ai_move_order_to_plord(state->plord, 2, order, append);
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
    static const StarsMoveToStarbaseOps ops = {
        find_closest, get_point, move_fleet
    };
    StarsU8 fleet[124];
    StarsU8 plord[STARS_AI_PLORD_HEADER_SIZE +
        2U * STARS_AI_PLORD_ORDER_SIZE];
    State state;
    StarsI16 failures;

    memset(fleet, 0, sizeof(fleet));
    memset(plord, 0, sizeof(plord));
    memset(&state, 0, sizeof(state));
    state.plord = plord;
    state.starbase = 4;
    state.point.x = -12;
    state.point.y = 23;
    seed_first_order(plord, state.point);
    failures = !stars_f_move_to_nearest_starbase(fleet, 1, &ops, &state);
    failures += state.find_calls != 1 || state.point_calls != 1 ||
        state.move_calls != 1 || state.append != 0;
    failures += state.order.id != 4 || state.order.point.x != -12 ||
        state.order.point.y != 23 || state.order.flags != 0x1040U;
    failures += plord[3] != 1U || memcmp(plord + 4, &state.order,
        sizeof(state.order)) != 0;

    state.starbase = -1;
    failures += stars_f_move_to_nearest_starbase(fleet, 0, &ops, &state) != 0;
    failures += state.find_calls != 2 || state.point_calls != 1 ||
        state.move_calls != 1;

    printf("AI_STARBASE_ACTION_PLORD_INTEGRATION_CHECKS=8 FAILURES=%d RESULT=%s\n",
        (int)failures, failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
}
