#include "core.h"
#include <math.h>

/* NE 12:540C-62D8. Draw the scanner's fleet/thing vector, arrowhead, and
 * selected-fleet order path. Coordinates in this adapter are already in scan
 * space; the original obtains them through LogicalToScan and sends the same
 * primitives to GDI. */

#define STARS_SCAN_PATH_PEN_SHIP 1
#define STARS_SCAN_PATH_PEN_ROUTE 2
#define STARS_SCAN_PATH_MODE_XOR 7
#define STARS_SCAN_PATH_ARROW_RADIUS 5
#define STARS_SCAN_PATH_ARROW_TICKS 5

static void scan_path_line(const StarsDrawShipScanPathInput *input,
    StarsU16 hdc, StarsPoint from, StarsPoint to, StarsI16 kind)
{
    if (input->ops && input->ops->line)
        input->ops->line(input->context, hdc, from, to, kind);
}

static void scan_path_vector(const StarsDrawShipScanPathInput *input,
    StarsU16 hdc, StarsPoint origin, StarsPoint destination, StarsI16 kind)
{
    double dx;
    double dy;
    double length;
    double ux;
    double uy;
    StarsPoint from;
    StarsPoint to;
    StarsI16 tick;

    dx = (double)destination.x - (double)origin.x;
    dy = (double)destination.y - (double)origin.y;
    length = sqrt(dx * dx + dy * dy);
    if (length <= 0.0) return;
    ux = -dy / length;
    uy = dx / length;
    scan_path_line(input, hdc, origin, destination, kind);
    for (tick = -STARS_SCAN_PATH_ARROW_TICKS;
         tick <= STARS_SCAN_PATH_ARROW_TICKS; ++tick) {
        if (tick == 0) continue;
        from.x = (StarsI16)(destination.x + (StarsI16)(ux * tick));
        from.y = (StarsI16)(destination.y + (StarsI16)(uy * tick));
        to.x = (StarsI16)(destination.x - (StarsI16)(ux * tick) -
            (StarsI16)(dx / length * STARS_SCAN_PATH_ARROW_RADIUS));
        to.y = (StarsI16)(destination.y - (StarsI16)(uy * tick) -
            (StarsI16)(dy / length * STARS_SCAN_PATH_ARROW_RADIUS));
        scan_path_line(input, hdc, from, to, kind + 1);
    }
}

static StarsI16 scan_path_same_point(StarsPoint left, StarsPoint right)
{
    return left.x == right.x && left.y == right.y;
}

static StarsI16 scan_path_same_segment(StarsPoint first, StarsPoint second,
    StarsPoint left, StarsPoint right)
{
    return (scan_path_same_point(first, left) &&
        scan_path_same_point(second, right)) ||
        (scan_path_same_point(first, right) &&
        scan_path_same_point(second, left));
}

static void scan_path_orders(const StarsDrawShipScanPathInput *input,
    StarsU16 hdc)
{
    StarsI16 index;
    StarsI16 prior;
    StarsI16 duplicate;
    StarsPoint from;
    StarsPoint to;

    if (!input->state || input->state->route_count < 2) return;
    if (input->state->orders_visible == 0U ||
        (input->state->scanner_flags & STARS_SCAN_PATH_FLAG_STARBASE) == 0U)
        return;
    from = input->state->route_points[0];
    for (index = 1; index < input->state->route_count &&
         index < STARS_SCAN_PATH_MAX_ORDERS; ++index) {
        to = input->state->route_points[index];
        duplicate = 0;
        for (prior = 1; prior < index; ++prior)
            if (scan_path_same_segment(input->state->route_points[prior - 1],
                input->state->route_points[prior], from, to)) duplicate = 1;
        if (!duplicate && !scan_path_same_point(from, to))
            scan_path_line(input, hdc, from, to, STARS_SCAN_PATH_PEN_ROUTE);
        from = to;
    }
}

static void scan_path_planet_routes(const StarsDrawShipScanPathInput *input,
    StarsU16 hdc)
{
    StarsI16 index;
    StarsPoint from;
    StarsPoint to;

    if (!input->state || input->state->planet_route_count < 2) return;
    from = input->state->planet_route[0];
    for (index = 1; index < input->state->planet_route_count &&
         index < STARS_SCAN_PATH_MAX_ORDERS; ++index) {
        to = input->state->planet_route[index];
        if (!scan_path_same_point(from, to))
            scan_path_line(input, hdc, from, to, STARS_SCAN_PATH_PEN_ROUTE);
        from = to;
    }
}

void stars_draw_ship_scan_path(StarsU16 hdc, StarsI16 show,
    StarsDrawShipScanPathInput *input)
{
    StarsU16 acquired_dc;
    StarsI16 acquired;
    StarsI16 eligible;

    if (!input || !input->state) return;
    eligible = input->state->selected_kind == STARS_SCAN_PATH_KIND_FLEET ||
        input->state->scan_kind == STARS_SCAN_PATH_KIND_FLEET ||
        input->state->scan_kind == STARS_SCAN_PATH_KIND_THING ||
        input->state->selected_kind == STARS_SCAN_PATH_KIND_PLANET;
    if (!eligible || input->state->orders_visible == (StarsU16)show ||
        input->state->drawing_busy) return;
    input->state->orders_visible = (StarsU16)show;
    acquired = hdc == 0U;
    acquired_dc = hdc;
    if (acquired) {
        if (!input->ops || !input->ops->acquire_dc) return;
        acquired_dc = input->ops->acquire_dc(input->context,
            input->scanner_window);
    }
    if (input->ops && input->ops->set_mode)
        input->ops->set_mode(input->context, acquired_dc,
            STARS_SCAN_PATH_MODE_XOR);
    if (input->state->transit_valid &&
        !scan_path_same_point(input->state->selected_point,
            input->state->transit_point)) {
        if (input->ops && input->ops->set_pen)
            input->ops->set_pen(input->context, acquired_dc,
                STARS_SCAN_PATH_PEN_SHIP);
        scan_path_vector(input, acquired_dc, input->state->selected_point,
            input->state->transit_point, STARS_SCAN_PATH_PEN_SHIP);
    }
    if (input->state->selected_kind == STARS_SCAN_PATH_KIND_PLANET)
        scan_path_planet_routes(input, acquired_dc);
    if (input->state->selected_kind == STARS_SCAN_PATH_KIND_FLEET)
        scan_path_orders(input, acquired_dc);
    if (input->ops && input->ops->set_mode)
        input->ops->set_mode(input->context, acquired_dc, 0);
    if (acquired && input->ops && input->ops->release_dc)
        input->ops->release_dc(input->context, input->scanner_window,
            acquired_dc);
}
