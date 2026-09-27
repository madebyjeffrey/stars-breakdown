/* Semantic reconstruction of the MDI child layout routine. */
#include "core.h"

static StarsI16 muldiv(StarsI16 value, StarsI16 numerator,
    StarsI16 denominator)
{
    return (StarsI16)(((StarsI32)value * numerator) / denominator);
}

static void move_window(const StarsRefitOps *ops, void *context, StarsU16 hwnd,
    StarsI16 x, StarsI16 y, StarsI16 width, StarsI16 height)
{
    if (ops->move_window && hwnd)
        ops->move_window(context, hwnd, x, y, width, height, 1);
}

static void check_layout_menu(const StarsRefitState *state,
    const StarsRefitOps *ops, void *context)
{
    StarsU16 menu;
    StarsU16 id;

    if (!ops->get_sub_menu || !ops->check_menu_item) return;
    menu = ops->get_sub_menu(context, state->frame, 1U);
    menu = ops->get_sub_menu(context, menu, 4U);
    for (id = 0x82U; id < 0x85U; ++id)
        ops->check_menu_item(context, menu, id,
            (StarsI16)(id - 0x82U) == state->layout ? 8U : 0U);
}

/* NE 5:8C88-9135. Calculate the two-pane geometry, move all existing child
 * windows, and update the three frame-layout menu checks. */
void stars_refit_frame_children(StarsRefitState *state,
    const StarsRefitOps *ops, void *context)
{
    StarsI16 min_height;
    StarsI16 message_height;
    StarsI16 available;
    StarsI16 combined;
    StarsI16 minimum_min;
    StarsU16 toolbar_height;

    if (!state || !ops || !state->frame || state->iconic) return;
    if (state->layout == 0 ||
        (state->layout != 1 && state->layout != 2)) {
        if (state->dx - state->dx_plan_want < 100)
            state->x_top = state->dx - 100;
        else state->x_top = state->dx_plan_want;
        if (state->x_top < 199) state->x_top = 198;
        toolbar_height = (StarsU16)((state->dy_arial8 * 13) / 2 + 10);
        min_height = (StarsI16)(state->dy_arial8 * 13 - 36);
        if (state->dy_msg_want <= (StarsI16)toolbar_height)
            state->dy_msg_want = (StarsI16)toolbar_height;
        minimum_min = state->dy_min_want;
        if (minimum_min < min_height) minimum_min = min_height;
        state->dy_min_want = minimum_min;
        message_height = state->dy_msg_want;
        if (state->dy - (state->dy_msg_want + minimum_min + 16) < 50) {
            available = (StarsI16)(state->dy - 66);
            combined = (StarsI16)(state->dy_msg_want + minimum_min);
            message_height = muldiv(available, state->dy_msg_want,
                combined);
            minimum_min = muldiv(available, minimum_min, combined);
            if (message_height < (StarsI16)toolbar_height) {
                minimum_min = (StarsI16)(minimum_min -
                    ((StarsI16)toolbar_height - message_height));
                message_height = (StarsI16)toolbar_height;
            } else if (minimum_min < min_height) {
                available = (StarsI16)(min_height - minimum_min);
                minimum_min = min_height;
                message_height = (StarsI16)(message_height - available);
            }
        }
        state->y1 = (StarsI16)(state->dy - minimum_min - message_height - 16);
        state->y2 = (StarsI16)(state->y1 + message_height + 8);
        if (state->scanner) {
            if (state->color_flags & 0x8000U) {
                move_window(ops, context, state->toolbar,
                    (StarsI16)(state->x_top + 8), 0,
                    (StarsI16)(state->dx - state->x_top - 8), 36);
                toolbar_height = 36;
            } else {
                move_window(ops, context, state->toolbar, 0, -100,
                    50, 50);
                toolbar_height = 0;
            }
            move_window(ops, context, state->scanner,
                (StarsI16)(state->x_top + 8), (StarsI16)toolbar_height,
                (StarsI16)(state->dx - state->x_top - 8),
                (StarsI16)(state->dy - toolbar_height));
            move_window(ops, context, state->planet, 0, 0,
                state->x_top, state->y1);
            move_window(ops, context, state->message, 0,
                (StarsI16)(state->y1 + 8), state->x_top, message_height);
            move_window(ops, context, state->mine, 0,
                (StarsI16)(state->y2 + 8), state->x_top, minimum_min);
        }
    } else {
        if (state->dx - state->dx2_plan_want < 200)
            state->x_top = state->dx - 200;
        else state->x_top = state->dx2_plan_want;
        if (state->x_top < 199) state->x_top = 198;
        toolbar_height = (StarsU16)((state->dy_arial8 * 13) / 2 + 10);
        min_height = (StarsI16)(state->dy_arial8 * 13 - 36);
        if (state->dy2_msg_want < (StarsI16)toolbar_height)
            state->dy2_msg_want = (StarsI16)toolbar_height;
        if (state->dy2_min_want < min_height)
            state->dy2_min_want = min_height;
        message_height = state->dy2_msg_want;
        if (state->dy - (state->dy2_msg_want + 8) < 100)
            message_height = (StarsI16)(state->dy - 108);
        minimum_min = state->dy2_min_want;
        if (state->dy - (state->dy2_min_want + 8) < 100)
            minimum_min = (StarsI16)(state->dy - 108);
        state->y1 = (StarsI16)(state->dy - message_height - 8);
        state->y2 = (StarsI16)(state->dy - minimum_min - 8);
        if (state->scanner) {
            if (state->color_flags & 0x8000U) {
                move_window(ops, context, state->toolbar, 0, 0,
                    state->dx, 36);
                toolbar_height = 36;
            } else {
                move_window(ops, context, state->toolbar, 0, -100,
                    50, 50);
                toolbar_height = 0;
            }
            move_window(ops, context, state->scanner,
                (StarsI16)(state->x_top + 8), (StarsI16)toolbar_height,
                (StarsI16)(state->dx - state->x_top - 8),
                (StarsI16)(state->y2 - toolbar_height));
            move_window(ops, context, state->planet, 0,
                (StarsI16)toolbar_height, state->x_top,
                (StarsI16)(state->y1 - toolbar_height));
            move_window(ops, context, state->message, 0,
                (StarsI16)(state->y1 + 8), state->x_top, message_height);
            move_window(ops, context, state->mine,
                (StarsI16)(state->x_top + 8), (StarsI16)(state->y2 + 8),
                (StarsI16)(state->dx - state->x_top - 8), minimum_min);
        }
    }
    check_layout_menu(state, ops, context);
}
