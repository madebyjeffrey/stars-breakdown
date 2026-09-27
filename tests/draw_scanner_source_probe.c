#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct ScanProbe {
    int acquired;
    int released;
    int clipped;
    int filled;
    int radars;
    int planets;
    int fleets;
    int things;
    int paths;
    int counts;
} ScanProbe;

static StarsU16 get_dc(void *opaque, StarsU16 hwnd)
{
    ScanProbe *probe = (ScanProbe *)opaque;
    (void)hwnd;
    ++probe->acquired;
    return 10;
}

static void release_dc(void *opaque, StarsU16 hwnd, StarsU16 hdc)
{
    ScanProbe *probe = (ScanProbe *)opaque;
    (void)hwnd; (void)hdc;
    ++probe->released;
}

static void get_rect(void *opaque, StarsU16 hwnd, StarsRect *rect)
{
    (void)opaque; (void)hwnd;
    rect->left = 0; rect->top = 0; rect->right = 320; rect->bottom = 240;
}

static void clip(void *opaque, StarsU16 hdc, const StarsRect *rect)
{
    ScanProbe *probe = (ScanProbe *)opaque;
    (void)hdc; (void)rect;
    ++probe->clipped;
}

static StarsI16 transform(void *opaque, StarsI16 value)
{
    (void)opaque;
    return value;
}

static void link_fleets(void *opaque)
{
    (void)opaque;
}

static void fill(void *opaque, StarsU16 hdc, const StarsRect *rect)
{
    ScanProbe *probe = (ScanProbe *)opaque;
    (void)hdc; (void)rect;
    ++probe->filled;
}

static void radar(void *opaque, StarsU16 hdc, const StarsRect *rect,
    StarsI16 near_circle)
{
    ScanProbe *probe = (ScanProbe *)opaque;
    (void)hdc; (void)rect; (void)near_circle;
    ++probe->radars;
}

static void planet(void *opaque, StarsU16 hdc,
    const StarsDrawScannerPlanet *value, StarsPoint point, StarsI16 selected)
{
    ScanProbe *probe = (ScanProbe *)opaque;
    (void)hdc; (void)value; (void)point; (void)selected;
    ++probe->planets;
}

static void fleet(void *opaque, StarsU16 hdc,
    const StarsDrawScannerFleet *value, StarsPoint point, StarsI16 selected)
{
    ScanProbe *probe = (ScanProbe *)opaque;
    (void)hdc; (void)value; (void)point; (void)selected;
    ++probe->fleets;
}

static void thing(void *opaque, StarsU16 hdc,
    const StarsDrawScannerThing *value, StarsPoint point, StarsI16 selected)
{
    ScanProbe *probe = (ScanProbe *)opaque;
    (void)hdc; (void)value; (void)point; (void)selected;
    ++probe->things;
}

static void path(void *opaque, StarsU16 hdc, const StarsPoint *points,
    StarsU16 count, StarsI16 selected)
{
    ScanProbe *probe = (ScanProbe *)opaque;
    (void)hdc; (void)points; (void)count; (void)selected;
    ++probe->paths;
}

static void count(void *opaque, StarsU16 hdc,
    const StarsDrawScannerFleet *value, StarsPoint point)
{
    ScanProbe *probe = (ScanProbe *)opaque;
    (void)hdc; (void)value; (void)point;
    ++probe->counts;
}

int main(void)
{
    StarsDrawScannerOps ops;
    StarsDrawScannerInput input;
    StarsDrawScannerPlanet planets[1];
    StarsDrawScannerFleet fleets[1];
    StarsDrawScannerThing things[1];
    StarsPoint fleet_path[2];
    StarsRect rect;
    ScanProbe probe;

    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(&probe, 0, sizeof probe);
    memset(planets, 0, sizeof planets);
    memset(fleets, 0, sizeof fleets);
    memset(things, 0, sizeof things);
    ops.get_dc = get_dc;
    ops.release_dc = release_dc;
    ops.get_client_rect = get_rect;
    ops.set_clip = clip;
    ops.reset_clip = clip;
    ops.point_to_scan = transform;
    ops.scan_to_point = transform;
    ops.link_fleets = link_fleets;
    ops.fill_background = fill;
    ops.draw_radar_circle = radar;
    ops.draw_planet = planet;
    ops.draw_fleet = fleet;
    ops.draw_thing = thing;
    ops.draw_path = path;
    ops.draw_fleet_count = count;
    planets[0].id = 2; planets[0].owner = 1;
    planets[0].logical.x = 40; planets[0].logical.y = 50;
    planets[0].scanner_range = 20; planets[0].near_range = 10;
    planets[0].known = 1;
    fleet_path[0].x = 20; fleet_path[0].y = 20;
    fleet_path[1].x = 30; fleet_path[1].y = 30;
    fleets[0].id = 3; fleets[0].owner = 1;
    fleets[0].logical.x = 60; fleets[0].logical.y = 70;
    fleets[0].scanner_range = 30; fleets[0].near_range = 15;
    fleets[0].visible_ships = 4;
    fleets[0].path = fleet_path; fleets[0].path_count = 2;
    things[0].id = 4; things[0].owner = 1; things[0].kind = 1;
    things[0].logical.x = 80; things[0].logical.y = 90;
    things[0].radius = 8; things[0].flags = 3;
    input.scanner_window = 5;
    input.scrollbar_height = 24;
    input.galaxy_inverse = 200;
    input.player = 1;
    input.radar_percent = 100;
    input.view_flags = 0x10a0;
    input.selection.kind = 2; input.selection.id = 3;
    input.planets = planets; input.planet_count = 1;
    input.fleets = fleets; input.fleet_count = 1;
    input.things = things; input.thing_count = 1;
    input.ops = &ops; input.context = &probe;
    rect.left = 0; rect.top = 0; rect.right = 200; rect.bottom = 180;
    if (stars_draw_scanner(&rect, &input) != 0) return 1;
    if (probe.acquired != 1 || probe.released != 1 || probe.filled != 1 ||
            probe.clipped < 2 || probe.radars < 3 || probe.planets != 1 ||
            probe.fleets != 1 || probe.things != 1 || probe.paths != 1 ||
            probe.counts != 1) return 2;
    printf("DRAW_SCANNER_SOURCE=PASS\n");
    return 0;
}
