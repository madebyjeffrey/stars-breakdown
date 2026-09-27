#include "core.h"

/* NE 11:7CDA-9165. Hit-test the ship order pane, dispatch transfer/order
 * commands, and turn the fuel/order gauges into bounded drag operations. The
 * original routine stores the same decisions in global fleet/order records;
 * this adapter exposes the state transitions and pane side effects by name. */

#define STARS_CLICK_CURSOR_ARROW 0U
#define STARS_CLICK_CURSOR_HAND 1U
#define STARS_CLICK_CURSOR_HELP 2U

static StarsI16 click_ship_point_in(const StarsRect *rect, StarsPoint point)
{
    return point.x >= rect->left && point.x < rect->right &&
        point.y >= rect->top && point.y < rect->bottom;
}

static void click_ship_action(const StarsClickShipOrdersInput *input,
    StarsI16 action, StarsI32 value)
{
    if (input->ops && input->ops->action)
        input->ops->action(input->context, action, value);
}

static StarsI16 click_ship_region(const StarsClickShipOrdersState *state,
    StarsPoint point)
{
    StarsI16 region;
    for (region = 0; region < STARS_CLICK_SHIP_REGIONS; ++region)
        if (click_ship_point_in(&state->reference[region], point))
            return region;
    return -1;
}

static StarsI32 click_ship_drag(const StarsClickShipOrdersInput *input,
    StarsI16 region, StarsI32 current)
{
    StarsI32 value;
    if (!input->ops || !input->ops->drag) return current;
    value = input->ops->drag(input->context, region, current);
    if (region == STARS_CLICK_REGION_FUEL_TRANSFER && value < 0L)
        value = 0L;
    if (region == STARS_CLICK_REGION_MASS_WARP) {
        if (value < 0L) value = 0L;
        if (value > 15L) value = 15L;
    }
    if (region == STARS_CLICK_REGION_ORDER_KIND) {
        if (value < 0L) value = 0L;
        if (value > 15L) value = 15L;
    }
    return value;
}

StarsU16 stars_click_ship_orders(StarsPoint point, StarsI16 sks,
    StarsI16 f_cursor, StarsI16 f_right_button,
    StarsClickShipOrdersInput *input)
{
    StarsI16 region;
    StarsI16 popup_result;
    StarsI32 value;

    (void)sks;
    if (!input || !input->state) return STARS_CLICK_CURSOR_ARROW;
    if (input->state->selected_object == 0)
        return STARS_CLICK_CURSOR_ARROW;
    region = click_ship_region(input->state, point);
    if (region < 0) {
        if (!f_cursor && !f_right_button)
            click_ship_action(input, STARS_CLICK_ACTION_SHOW_ZIP_POPUP, 0L);
        return STARS_CLICK_CURSOR_ARROW;
    }
    if (region == STARS_CLICK_REGION_POPUP) {
        if (f_cursor) return STARS_CLICK_CURSOR_HELP;
        if (!f_right_button)
            click_ship_action(input, STARS_CLICK_ACTION_SHOW_ORDER_POPUP,
                input->state->order_kind);
        else if (input->ops && input->ops->popup) {
            popup_result = input->ops->popup(input->context, region,
                point.x, point.y);
            if (popup_result >= 0) {
                input->state->order_kind = popup_result;
                click_ship_action(input, STARS_CLICK_ACTION_SET_ORDER_KIND,
                    popup_result);
            }
        }
        return STARS_CLICK_CURSOR_HELP;
    }
    if (region == STARS_CLICK_REGION_PLANET_TRANSFER) {
        if (!f_cursor)
            click_ship_action(input, STARS_CLICK_ACTION_TRANSFER_PLANET,
                input->state->selected_planet);
        return STARS_CLICK_CURSOR_HAND;
    }
    if (region == STARS_CLICK_REGION_ORBIT_TRANSFER) {
        if (!f_cursor)
            click_ship_action(input, STARS_CLICK_ACTION_TRANSFER_ORBIT,
                input->state->selected_fleet);
        return STARS_CLICK_CURSOR_HAND;
    }
    if (region == STARS_CLICK_REGION_ORDER_KIND) {
        if (input->state->order_kind != 7 && f_cursor)
            return STARS_CLICK_CURSOR_ARROW;
    }
    if (region == STARS_CLICK_REGION_ORDER_TARGET &&
        input->state->order_kind != 7)
        return STARS_CLICK_CURSOR_ARROW;
    if (f_cursor) return STARS_CLICK_CURSOR_HAND;
    if (region == STARS_CLICK_REGION_FUEL_TRANSFER) {
        value = click_ship_drag(input, region, input->state->fuel);
        input->state->fuel = value;
        click_ship_action(input, STARS_CLICK_ACTION_DRAG_FUEL, value);
    } else if (region == STARS_CLICK_REGION_MASS_WARP) {
        value = click_ship_drag(input, region, input->state->mass_warp);
        input->state->mass_warp = (StarsI16)value;
        click_ship_action(input, STARS_CLICK_ACTION_DRAG_MASS_WARP, value);
    } else if (region == STARS_CLICK_REGION_ORDER_KIND) {
        value = click_ship_drag(input, region,
            (StarsI32)input->state->order_values[0]);
        input->state->order_values[0] = (StarsI16)value;
        click_ship_action(input, STARS_CLICK_ACTION_DRAG_ORDER, value);
    } else if (region == STARS_CLICK_REGION_ORDER_TARGET) {
        value = click_ship_drag(input, region,
            (StarsI32)input->state->order_values[1]);
        input->state->order_values[1] = (StarsI16)value;
        click_ship_action(input, STARS_CLICK_ACTION_SET_ORDER_TARGET, value);
    }
    click_ship_action(input, STARS_CLICK_ACTION_REFRESH, region);
    return STARS_CLICK_CURSOR_ARROW;
}
