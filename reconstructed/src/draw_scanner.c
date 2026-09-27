#include "core.h"

enum {
    SCAN_FLAG_RADAR = 0x0020,
    SCAN_FLAG_MINES = 0x0040,
    SCAN_FLAG_PATHS = 0x0080,
    SCAN_FLAG_COUNTS = 0x1000,
    SCAN_MODE_HIDDEN = 5,
    SCAN_KIND_PLANET = 1,
    SCAN_KIND_FLEET = 2,
    SCAN_KIND_THING = 8,
    SCAN_RADAR_MAX = 100,
    SCAN_VIEW_MARGIN = 9,
    SCAN_VIEW_BOTTOM_EXTRA = 14
};

static StarsI16 scan_transform(const StarsDrawScannerInput *input,
    StarsI16 value, StarsI16 inverse)
{
    StarsI16 result;

    if (inverse) {
        result = input->ops->scan_to_point ? input->ops->scan_to_point(
            input->context, value) : value;
    } else {
        result = input->ops->point_to_scan ? input->ops->point_to_scan(
            input->context, value) : value;
    }
    return result;
}

static StarsPoint scan_point(const StarsDrawScannerInput *input,
    StarsPoint logical)
{
    StarsPoint result;
    StarsI16 x;
    StarsI16 y;

    x = (StarsI16)(input->x_scan_top + logical.x);
    y = (StarsI16)(input->galaxy_inverse - input->y_scan_top - logical.y);
    result.x = scan_transform(input, x, 0);
    result.y = scan_transform(input, y, 0);
    return result;
}

static StarsI16 scan_in_view(const StarsRect *rect, StarsPoint point)
{
    return point.x >= (StarsI16)rect->left &&
        point.x < (StarsI16)rect->right &&
        point.y >= (StarsI16)rect->top &&
        point.y < (StarsI16)rect->bottom;
}

static StarsI16 scan_scaled_range(const StarsDrawScannerInput *input,
    StarsI16 range)
{
    StarsI32 scaled;

    if (range <= 0) return 0;
    scaled = (StarsI32)range * input->radar_percent;
    if (input->radar_percent <= 0) scaled = range;
    else scaled /= SCAN_RADAR_MAX;
    return (StarsI16)scan_transform(input, (StarsI16)scaled, 0);
}

static void scan_radar(const StarsDrawScannerInput *input, StarsU16 hdc,
    StarsPoint center, StarsI16 radius, StarsI16 near_circle)
{
    StarsRect rect;

    if (radius <= 0 || !input->ops->draw_radar_circle) return;
    rect.left = (StarsI16)(center.x - radius);
    rect.top = (StarsI16)(center.y - radius);
    rect.right = (StarsI16)(center.x + radius);
    rect.bottom = (StarsI16)(center.y + radius);
    input->ops->draw_radar_circle(input->context, hdc, &rect, near_circle);
}

static StarsI16 scan_selected(const StarsDrawScannerInput *input,
    StarsI16 kind, StarsI16 id)
{
    return input->selection.kind == kind && input->selection.id == id;
}

static StarsI16 scan_fleet_visible(const StarsDrawScannerInput *input,
    const StarsDrawScannerFleet *fleet)
{
    if (fleet->owner == input->player) return 1;
    if (fleet->flags & 1U) return 1;
    if (scan_selected(input, SCAN_KIND_FLEET, fleet->id)) return 1;
    return 0;
}

static void scan_draw_radars(const StarsDrawScannerInput *input, StarsU16 hdc,
    const StarsRect *view)
{
    StarsU16 index;
    StarsPoint point;
    StarsI16 range;

    if ((input->view_flags & SCAN_FLAG_RADAR) == 0) return;
    for (index = 0; index < input->planet_count; ++index) {
        if (input->planets[index].owner != input->player) continue;
        if (!input->planets[index].known) continue;
        point = scan_point(input, input->planets[index].logical);
        range = scan_scaled_range(input, input->planets[index].scanner_range);
        if (scan_in_view(view, point)) scan_radar(input, hdc, point, range, 0);
        range = scan_scaled_range(input, input->planets[index].near_range);
        if (scan_in_view(view, point)) scan_radar(input, hdc, point, range, 1);
    }
    for (index = 0; index < input->fleet_count; ++index) {
        if (!scan_fleet_visible(input, &input->fleets[index])) continue;
        point = scan_point(input, input->fleets[index].logical);
        range = scan_scaled_range(input, input->fleets[index].scanner_range);
        if (scan_in_view(view, point)) scan_radar(input, hdc, point, range, 0);
        range = scan_scaled_range(input, input->fleets[index].near_range);
        if (scan_in_view(view, point)) scan_radar(input, hdc, point, range, 1);
    }
    if ((input->view_flags & SCAN_FLAG_MINES) != 0) {
        for (index = 0; index < input->thing_count; ++index) {
            if ((input->things[index].flags & 1U) == 0) continue;
            point = scan_point(input, input->things[index].logical);
            range = scan_scaled_range(input, input->things[index].radius);
            if (scan_in_view(view, point))
                scan_radar(input, hdc, point, range, 0);
        }
    }
}

static void scan_draw_objects(const StarsDrawScannerInput *input, StarsU16 hdc,
    const StarsRect *view)
{
    StarsU16 index;
    StarsPoint point;
    StarsI16 selected;

    if ((input->view_flags & 0x000fU) == SCAN_MODE_HIDDEN) return;
    for (index = 0; index < input->planet_count; ++index) {
        point = scan_point(input, input->planets[index].logical);
        if (!scan_in_view(view, point)) continue;
        selected = scan_selected(input, SCAN_KIND_PLANET,
            input->planets[index].id);
        if (input->ops->draw_planet)
            input->ops->draw_planet(input->context, hdc,
                &input->planets[index], point, selected);
    }
    if ((input->view_flags & SCAN_FLAG_MINES) == 0) {
        for (index = 0; index < input->thing_count; ++index) {
            if ((input->things[index].flags & 2U) == 0) continue;
            point = scan_point(input, input->things[index].logical);
            if (!scan_in_view(view, point)) continue;
            selected = scan_selected(input, SCAN_KIND_THING,
                input->things[index].id);
            if (input->ops->draw_thing)
                input->ops->draw_thing(input->context, hdc,
                    &input->things[index], point, selected);
        }
    }
    for (index = 0; index < input->fleet_count; ++index) {
        if (!scan_fleet_visible(input, &input->fleets[index])) continue;
        point = scan_point(input, input->fleets[index].logical);
        if (!scan_in_view(view, point)) continue;
        selected = scan_selected(input, SCAN_KIND_FLEET,
            input->fleets[index].id);
        if (input->ops->draw_fleet)
            input->ops->draw_fleet(input->context, hdc,
                &input->fleets[index], point, selected);
        if ((input->view_flags & SCAN_FLAG_COUNTS) != 0 &&
                input->ops->draw_fleet_count)
            input->ops->draw_fleet_count(input->context, hdc,
                &input->fleets[index], point);
    }
}

static void scan_draw_paths(const StarsDrawScannerInput *input, StarsU16 hdc)
{
    StarsU16 index;

    if ((input->view_flags & SCAN_FLAG_PATHS) == 0) return;
    if (!input->ops->draw_path) return;
    for (index = 0; index < input->fleet_count; ++index) {
        if (!scan_fleet_visible(input, &input->fleets[index])) continue;
        if (!input->fleets[index].path || input->fleets[index].path_count == 0)
            continue;
        input->ops->draw_path(input->context, hdc,
            input->fleets[index].path, input->fleets[index].path_count,
            input->fleets[index].id == input->selection.id);
    }
}

/* NE 12:108A-47D1.  Render the scanner viewport through typed planet, fleet,
 * and thing records.  The original routine's off-screen bitmap, segmented
 * object tables, GDI selections, radar-circle accumulator, and bitmap sprite
 * sheets are represented by the drawing callbacks; the viewport gates,
 * inverted galaxy coordinates, zoom/radar scaling, visibility rules, marker
 * dispatch, fleet counts, paths, and DC ownership remain ordinary C. */
StarsI16 stars_draw_scanner(const StarsRect *paint_rect,
    StarsDrawScannerInput *input)
{
    StarsRect client;
    StarsRect view;
    StarsU16 hdc;
    StarsI16 acquired;

    if (!input || !input->ops || !paint_rect) return 0;
    if (input->fleet_count == 0 && input->planet_count == 0 &&
            input->thing_count == 0) return 0;
    if (input->ops->get_client_rect)
        input->ops->get_client_rect(input->context, input->scanner_window,
            &client);
    else {
        client.left = 0;
        client.top = 0;
        client.right = paint_rect->right;
        client.bottom = paint_rect->bottom;
    }
    view = *paint_rect;
    if ((StarsI16)view.bottom >
            (StarsI16)(client.bottom - input->scrollbar_height))
        view.bottom = (StarsI16)(client.bottom - input->scrollbar_height);
    view.left = view.left > SCAN_VIEW_MARGIN ?
        (StarsU16)(view.left - SCAN_VIEW_MARGIN) : 0;
    view.top = view.top > SCAN_VIEW_MARGIN ?
        (StarsU16)(view.top - SCAN_VIEW_MARGIN) : 0;
    view.right = (StarsI16)(view.right + SCAN_VIEW_MARGIN);
    view.bottom = (StarsI16)(view.bottom + SCAN_VIEW_MARGIN +
        SCAN_VIEW_BOTTOM_EXTRA);
    acquired = 0;
    hdc = input->hdc;
    if (hdc == 0 && input->ops->get_dc) {
        hdc = input->ops->get_dc(input->context, input->scanner_window);
        acquired = 1;
    }
    if (hdc == 0) return 0;
    if (input->ops->link_fleets) input->ops->link_fleets(input->context);
    if (input->ops->set_clip) input->ops->set_clip(input->context, hdc, &view);
    if (input->ops->fill_background)
        input->ops->fill_background(input->context, hdc, &view);
    scan_draw_radars(input, hdc, &view);
    scan_draw_objects(input, hdc, &view);
    scan_draw_paths(input, hdc);
    if (input->ops->reset_clip)
        input->ops->reset_clip(input->context, hdc, &client);
    if (acquired && input->ops->release_dc)
        input->ops->release_dc(input->context, input->scanner_window, hdc);
    return 0;
}
