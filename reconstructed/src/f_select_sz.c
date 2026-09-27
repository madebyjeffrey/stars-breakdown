#include <ctype.h>
#include <string.h>
#include "core.h"

static StarsI16 fselect_equal(const StarsU8 *a, const StarsU8 *b)
{
    while (*a && *b) {
        if (tolower(*a) != tolower(*b)) return 0;
        ++a; ++b;
    }
    return (StarsI16)(*a == *b);
}
static StarsI16 fselect_prefix(const StarsU8 *a, const StarsU8 *b,
    StarsI16 count)
{
    StarsI16 i;
    for (i = 0; i < count; ++i) {
        if (tolower(a[i]) != tolower(b[i]) || b[i] == 0) return 0;
    }
    return 1;
}
StarsI16 stars_f_select_sz(const StarsFSelectSzInput *input)
{
    StarsI16 i, prefix_match, number;
    const StarsU8 *p;
    StarsFindNameFleet fleet;
    StarsScanSelection selection;
    StarsPoint point;
    if (!input || !input->name) return 0;
    prefix_match = -1;
    for (i = 0; i < input->planet_count; ++i) {
        if (fselect_equal(input->planets[i].name, input->name)) {
            prefix_match = i; break;
        }
        if (prefix_match < 0 && fselect_prefix(input->planets[i].name,
                input->name, (StarsI16)strlen((const char *)input->name)))
            prefix_match = i;
    }
    for (;;) {
        if (i < input->planet_count) {
            point = input->planets[i].point;
            memset(&selection, 0, sizeof selection);
            if (input->find_nearest)
                input->find_nearest(input->context, point, 1, &selection);
            if (input->change_selection)
                input->change_selection(input->context, &selection, 1);
            if (input->ensure_point)
                input->ensure_point(input->context, point, 1);
            if (input->update_window)
                input->update_window(input->context, input->scanner_window);
            if (input->send_message)
                input->send_message(input->context, input->scanner_window,
                    0x102U, 0x76U, 0);
            return 1;
        }
        p = input->name;
        if (input->prefix && input->prefix_length > 0 &&
                fselect_prefix(input->prefix, input->name,
                    input->prefix_length)) p = input->name + 6;
        while (*p == ' ') ++p;
        if (*p == '#') ++p;
        while (*p == ' ') ++p;
        number = 0;
        if (*p >= '1' && *p <= '9') {
            do { number = (StarsI16)(number * 10 + *p - '0'); ++p; }
            while (*p >= '0' && *p <= '9' && number < 0x201);
            if (number < 0x201 && input->lookup_fleet &&
                    input->lookup_fleet(input->context,
                        (StarsU16)((number - 1) |
                            (input->current_player << 9)), &fleet)) {
                point = fleet.point; memset(&selection, 0, sizeof selection);
                if (input->find_nearest)
                    input->find_nearest(input->context, point, 2, &selection);
                selection.fleet = fleet.index;
                if (input->change_selection)
                    input->change_selection(input->context, &selection, 2);
                if (input->ensure_point)
                    input->ensure_point(input->context,
                        (point.x = selection.x, point.y = selection.y, point), 1);
                if (input->update_window)
                    input->update_window(input->context, input->scanner_window);
                if (input->send_message)
                    input->send_message(input->context, input->scanner_window,
                        0x102U, 0x76U, 0);
                return 1;
            }
        }
        for (i = 0; i < input->fleet_count; ++i) {
            if (!input->fleets[i].valid) break;
            if (fselect_equal(input->fleets[i].name, input->name)) {
                fleet = input->fleets[i]; point = fleet.point;
                memset(&selection, 0, sizeof selection);
                if (input->find_nearest)
                    input->find_nearest(input->context, point, 2, &selection);
                selection.fleet = fleet.index;
                if (input->change_selection)
                    input->change_selection(input->context, &selection, 2);
                if (input->ensure_point)
                    input->ensure_point(input->context,
                        (point.x = selection.x, point.y = selection.y, point), 1);
                if (input->update_window)
                    input->update_window(input->context, input->scanner_window);
                if (input->send_message)
                    input->send_message(input->context, input->scanner_window,
                        0x102U, 0x76U, 0);
                return 1;
            }
        }
        if (prefix_match < 0) return 0;
        i = prefix_match;
        prefix_match = -1;
    }
}
