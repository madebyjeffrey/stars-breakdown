#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct OrdersState {
    int cleared;
    int added;
    int selected;
    int drawn;
    StarsI16 warp;
    StarsI16 id;
    StarsI16 x;
    StarsI16 y;
} OrdersState;

static void clear_list(void *context, StarsU16 listbox)
{ OrdersState *s = (OrdersState *)context; s->cleared = listbox; }

static const StarsU8 *get_name(void *context, StarsI16 warp, StarsI16 id,
    StarsI16 x, StarsI16 y)
{
    OrdersState *s = (OrdersState *)context;
    ++s->added; s->warp = warp; s->id = id; s->x = x; s->y = y;
    return (const StarsU8 *)"loc";
}

static void add_text(void *context, StarsU16 listbox, const StarsU8 *text)
{
    OrdersState *s = (OrdersState *)context;
    (void)listbox; (void)text;
    ++s->selected;
}

static void select_row(void *context, StarsI16 row)
{ OrdersState *s = (OrdersState *)context; s->selected = row + 10; }

static void draw_plan_ship(void *context, StarsU16 a, StarsU16 b)
{ OrdersState *s = (OrdersState *)context; (void)a; (void)b; ++s->drawn; }

int main(void)
{
    StarsU8 list[4 + 2 * 18];
    StarsMoveOrder order;
    StarsFillOrdersOps ops;
    OrdersState state;
    int failures = 0;

    memset(list, 0, sizeof(list));
    memset(&order, 0, sizeof(order)); order.point.x = 10; order.point.y = 20;
    order.id = 4; order.flags = 0x7300U;
    memcpy(list + 4, &order, sizeof(order));
    order.point.x = -3; order.point.y = 8; order.id = 9; order.flags = 0x0200U;
    memcpy(list + 22, &order, sizeof(order));
    ops.clear_list = clear_list; ops.get_location_name = get_name;
    ops.add_text = add_text; ops.select_row = select_row;
    ops.draw_plan_ship = draw_plan_ship;
    memset(&state, 0, sizeof(state));
    stars_fill_orders_lb(7U, list, 2U, 3, 2, &ops, &state);
    if (state.cleared != 7 || state.added != 2 || state.selected != 13 ||
        state.drawn != 1 || state.warp != 2 || state.id != 9 ||
        state.x != -3 || state.y != 8)
        ++failures;
    printf("FILL_ORDERS_LB_CHECKS=2 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
