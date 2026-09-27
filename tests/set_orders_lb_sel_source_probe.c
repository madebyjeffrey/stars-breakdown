#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct OrderState {
    StarsU16 window;
    StarsU16 messages[4];
    StarsU16 values[4];
    int message_count;
    int update_count;
    int dropdown_count;
    StarsI16 dropdown_level;
} OrderState;

static void send_message(void *context, StarsU16 window, StarsU16 message,
    StarsU16 value, StarsU32 unused)
{
    OrderState *state = (OrderState *)context;
    (void)unused;
    state->window = window;
    state->messages[state->message_count] = message;
    state->values[state->message_count] = value;
    ++state->message_count;
}

static void update_window(void *context, StarsU16 window)
{
    OrderState *state = (OrderState *)context;
    state->window = window;
    ++state->update_count;
}

static void update_dropdowns(void *context, StarsI16 level)
{
    OrderState *state = (OrderState *)context;
    state->dropdown_level = level;
    ++state->dropdown_count;
}

int main(void)
{
    static const StarsSetOrdersLbSelOps ops = {
        send_message, update_window, update_dropdowns
    };
    OrderState state;
    int failures = 0;

    memset(&state, 0, sizeof state);
    stars_set_orders_lb_sel(0x1234U, 4, 0U, &ops, &state);
    if (state.message_count != 2 || state.messages[0] != 0x0407U ||
        state.values[0] != 4U || state.messages[1] != 0x0418U ||
        state.values[1] != 2U || state.update_count != 1 ||
        state.dropdown_count != 1 || state.dropdown_level != 0 ||
        state.window != 0x1234U)
        ++failures;

    memset(&state, 0, sizeof state);
    stars_set_orders_lb_sel(0x1234U, 1, 8U, &ops, &state);
    if (state.message_count != 1 || state.messages[0] != 0x0407U ||
        state.values[0] != 1U || state.update_count != 1 ||
        state.dropdown_count != 1)
        ++failures;
    printf("SET_ORDERS_SEL_CHECKS=2 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
