#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct ScanProbe {
    int find_calls;
    int change_calls;
    StarsI16 x;
    StarsI16 y;
    StarsI16 kind;
    StarsI16 waypoint;
    StarsI16 valid;
} ScanProbe;

static void find_nearest(void *context, StarsI16 x, StarsI16 y, StarsI16 kind,
    StarsScanSelection *selection)
{
    ScanProbe *probe = (ScanProbe *)context;
    ++probe->find_calls;
    probe->x = x;
    probe->y = y;
    probe->kind = kind;
    selection->grobj = kind;
}

static void change_selection(void *context, const StarsScanSelection *selection,
    StarsI16 valid)
{
    ScanProbe *probe = (ScanProbe *)context;
    ++probe->change_calls;
    probe->waypoint = selection->waypoint;
    probe->valid = valid;
}

static void put_word(StarsU8 *bytes, StarsU16 offset, StarsI16 value)
{
    bytes[offset] = (StarsU8)value;
    bytes[offset + 1U] = (StarsU8)((StarsU16)value >> 8);
}

int main(void)
{
    StarsU8 orders[36];
    StarsScanWaypointOps ops;
    ScanProbe probe;
    int failures = 0;
    memset(orders, 0, sizeof orders);
    put_word(orders, 0x12U + 4U, -12);
    put_word(orders, 0x12U + 6U, 27);
    ops.find_nearest = find_nearest;
    ops.change_selection = change_selection;
    memset(&probe, 0, sizeof probe);
    if (stars_set_scan_waypoint(1, 0, orders, 2, &ops, &probe) != 1 ||
        probe.find_calls != 1 || probe.change_calls != 1 || probe.x != -12 ||
        probe.y != 27 || probe.kind != 4 || probe.waypoint != 1 ||
        probe.valid != 1)
        ++failures;
    if (stars_set_scan_waypoint(0, 0, orders, 2, &ops, &probe) != 0 ||
        probe.find_calls != 1 || probe.change_calls != 1)
        ++failures;
    printf("SCAN_WAYPOINT_CHECKS=2 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
