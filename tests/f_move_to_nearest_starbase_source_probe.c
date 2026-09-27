#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct MoveState {
    StarsPoint point;
    StarsI16 found;
    StarsI16 moved;
    StarsI16 append;
    StarsU16 flags;
    StarsI16 id;
    int find_calls;
    int point_calls;
    int move_calls;
} MoveState;

static StarsI16 find_closest(void *context, const StarsU8 *fleet,
    StarsI16 big_starbase)
{
    MoveState *state = (MoveState *)context;
    (void)fleet;
    (void)big_starbase;
    ++state->find_calls;
    return state->found;
}

static void get_point(void *context, StarsI16 id, StarsPoint *point)
{
    MoveState *state = (MoveState *)context;
    ++state->point_calls;
    point->x = state->point.x;
    point->y = state->point.y;
    state->id = id;
}

static StarsI16 move_fleet(void *context, const StarsU8 *fleet,
    const StarsMoveOrder *order, StarsI16 append)
{
    MoveState *state = (MoveState *)context;
    (void)fleet;
    ++state->move_calls;
    state->moved = 7;
    state->append = append;
    state->flags = order->flags;
    if (order->id != state->found || order->point.x != state->point.x ||
        order->point.y != state->point.y)
        state->moved = 0;
    return state->moved;
}

int main(void)
{
    static const StarsMoveToStarbaseOps ops = {
        find_closest, get_point, move_fleet
    };
    StarsU8 fleet[4];
    MoveState state;
    StarsI16 result;
    int failures = 0;

    memset(fleet, 0, sizeof fleet);
    memset(&state, 0, sizeof state);
    state.found = 4;
    state.point.x = -12;
    state.point.y = 23;
    result = stars_f_move_to_nearest_starbase(fleet, 1, &ops, &state);
    if (result != 7 || state.find_calls != 1 || state.point_calls != 1 ||
        state.move_calls != 1 || state.id != 4 || state.append != 0 ||
        state.flags != 0x1040U)
        ++failures;

    state.found = -1;
    result = stars_f_move_to_nearest_starbase(fleet, 0, &ops, &state);
    if (result != 0 || state.find_calls != 2 || state.point_calls != 1 ||
        state.move_calls != 1)
        ++failures;
    printf("F_MOVE_STARBASE_CHECKS=2 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
