#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct MoveProbe {
    int selections;
    int refreshes;
    StarsI16 selected_kind;
    StarsI16 selected_id;
} MoveProbe;

static void select_fleet(void *context, StarsI16 kind, StarsI16 id)
{
    MoveProbe *probe = (MoveProbe *)context;
    ++probe->selections;
    probe->selected_kind = kind;
    probe->selected_id = id;
}

static StarsI16 refresh_fleet(void *context)
{
    MoveProbe *probe = (MoveProbe *)context;
    ++probe->refreshes;
    return 7;
}

static StarsMoveOrder make_order(StarsI16 x, StarsI16 y, StarsI16 id)
{
    StarsMoveOrder order;
    memset(&order, 0, sizeof(order));
    order.point.x = x; order.point.y = y; order.id = id;
    return order;
}

int main(void)
{
    StarsU8 fleet[124];
    StarsMoveOrder orders[4];
    StarsMoveAiFleetState state;
    StarsMoveOrder order;
    MoveProbe probe;
    StarsI16 result;
    int failures = 0;

    memset(fleet, 0, sizeof(fleet));
    fleet[0] = 0x34; fleet[1] = 0x12;
    memset(&probe, 0, sizeof(probe));
    state.orders = orders; state.count = 1; state.capacity = 4;
    state.select_fleet = select_fleet; state.refresh_fleet = refresh_fleet;
    orders[0] = make_order(1, 2, 3);
    order = make_order(1, 2, 9);
    result = stars_f_move_ai_fleet(fleet, &order, 0, &state, &probe);
    if (result != 7 || state.count != 1 || orders[0].id != 9 ||
        probe.selections != 1 || probe.refreshes != 1 ||
        probe.selected_kind != 2 || probe.selected_id != 0x1234)
        ++failures;
    order = make_order(8, 9, 10);
    result = stars_f_move_ai_fleet(fleet, &order, 1, &state, &probe);
    if (result != 7 || state.count != 2 || orders[1].id != 10)
        ++failures;
    printf("F_MOVE_AI_FLEET_CHECKS=2 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
