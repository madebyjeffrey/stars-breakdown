#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct ActionState {
    StarsU8 *plord;
    StarsI16 moves;
    StarsI16 selected_kind;
    StarsI16 selected_id;
    StarsMoveOrder last_order;
    StarsI16 last_append;
} ActionState;

static void select_fleet(void *opaque, StarsI16 kind, StarsI16 id)
{
    ActionState *state;
    state = (ActionState *)opaque;
    state->selected_kind = kind;
    state->selected_id = id;
}

static StarsU16 find_warp(void *opaque, const StarsU8 *fleet, StarsI16 mode)
{
    (void)opaque;
    (void)fleet;
    (void)mode;
    return 7U;
}

static StarsI16 move_fleet(void *opaque, const StarsU8 *fleet,
    const StarsMoveOrder *order, StarsI16 append)
{
    ActionState *state;
    (void)fleet;
    state = (ActionState *)opaque;
    ++state->moves;
    state->last_order = *order;
    state->last_append = append;
    return stars_ai_move_order_to_plord(state->plord, 2, order, append);
}

static void seed_first_order(StarsU8 *plord, StarsPoint point)
{
    StarsMoveOrder order;

    memset(&order, 0, sizeof(order));
    order.point = point;
    memcpy(plord + STARS_AI_PLORD_HEADER_SIZE, &order, sizeof(order));
    plord[3] = 1U;
}

int main(void)
{
    StarsU8 fleet[124];
    StarsU8 plord[STARS_AI_PLORD_HEADER_SIZE +
        2U * STARS_AI_PLORD_ORDER_SIZE];
    StarsPoint planets[3];
    StarsPoint wormhole_point;
    StarsU8 wormhole[6];
    ActionState state;
    StarsI16 failures;

    memset(fleet, 0, sizeof(fleet));
    memset(plord, 0, sizeof(plord));
    memset(planets, 0, sizeof(planets));
    memset(wormhole, 0, sizeof(wormhole));
    memset(&state, 0, sizeof(state));
    fleet[0] = 0x34U;
    fleet[1] = 0x12U;
    planets[2].x = 300;
    planets[2].y = -40;
    wormhole_point.x = 111;
    wormhole_point.y = -22;
    state.plord = plord;
    seed_first_order(plord, planets[2]);
    failures = !stars_f_colonize_ai_fleet(fleet, planets, 2,
        select_fleet, find_warp, move_fleet, &state);
    failures += state.selected_kind != 2 || state.selected_id != 0x1234;
    failures += state.moves != 1 || state.last_append != 0;
    failures += state.last_order.id != 2 || state.last_order.point.x != 300 ||
        state.last_order.point.y != -40;
    failures += state.last_order.flags != 0x1172U;
    failures += plord[3] != 1U || memcmp(plord + 4, &state.last_order,
        sizeof(state.last_order)) != 0;
    failures += (fleet[5] & 0x80U) == 0U;

    memset(&state, 0, sizeof(state));
    state.plord = plord;
    wormhole[0] = 17U;
    wormhole[1] = 0U;
    wormhole[2] = 111U;
    wormhole[3] = 0U;
    wormhole[4] = 234U;
    wormhole[5] = 255U;
    seed_first_order(plord, wormhole_point);
    failures += !stars_f_goto_wormhole_ai_fleet(fleet, wormhole,
        select_fleet, find_warp, move_fleet, &state);
    failures += state.selected_kind != 2 || state.selected_id != 0x1234;
    failures += state.moves != 1 || state.last_order.id != 17;
    failures += state.last_order.point.x != 111 ||
        state.last_order.point.y != -22 || state.last_order.flags != 0x0870U;
    failures += plord[3] != 1U || memcmp(plord + 4, &state.last_order,
        sizeof(state.last_order)) != 0;
    failures += (fleet[5] & 0x80U) == 0U;

    printf("AI_FLEET_ACTION_PLORD_INTEGRATION_CHECKS=12 FAILURES=%d RESULT=%s\n",
        (int)failures, failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
}
