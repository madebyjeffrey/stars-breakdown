#include "core.h"

#define MINERAL_SCAN_FLEET_ENTRY ((StarsU16)0x8000U)

enum {
    MINERAL_SCAN_PLANET = 1,
    MINERAL_SCAN_FLEET = 2,
    MINERAL_SCAN_THING = 8,
    MINERAL_SCAN_THING_ENTRY = 0x2000,
    MINERAL_SCAN_SELECTION_CHANGE = 2,
    MINERAL_SCAN_REDRAW_BEFORE = 0,
    MINERAL_SCAN_REDRAW_AFTER = 1,
    MINERAL_SCAN_MAX_ENTRIES = 99
};

typedef struct MineralScanMenuEntry {
    StarsU32 identity;
    StarsU16 marker;
    StarsI16 thing_index;
} MineralScanMenuEntry;

static StarsI16 same_point(StarsPoint first, StarsPoint second)
{
    return (StarsI16)(first.x == second.x && first.y == second.y);
}

/* NE 06:4ECC-5362.  The menu contains the selected planet, every fleet at
 * the same scanner coordinate, and every thing at that coordinate.  The
 * original stores a marker beside each menu identity; the adapter keeps those
 * as typed entries so a numeric identity cannot blur the object categories. */
void stars_popup_mineral_scan_choices(
    StarsPopupMineralScanChoicesInput *input)
{
    MineralScanMenuEntry entries[MINERAL_SCAN_MAX_ENTRIES];
    StarsU32 identities[MINERAL_SCAN_MAX_ENTRIES];
    StarsU16 markers[MINERAL_SCAN_MAX_ENTRIES];
    StarsPoint selected_point;
    StarsU16 count = 0U;
    StarsI16 selected = -1;
    StarsI16 selected_owner = 0;
    StarsI16 fleet_index;
    StarsI16 thing_index;
    StarsI16 menu_index;
    StarsU32 selected_identity;
    StarsScanSelection next;

    if (!input || !input->popup_menu || !input->change_selection)
        return;
    selected_point.x = input->selection.x;
    selected_point.y = input->selection.y;

    if (input->selection.planet != -1) {
        entries[count].identity = (StarsU32)(StarsU16)input->selection.planet;
        entries[count].marker = 0U;
        entries[count].thing_index = -1;
        if (input->selection.grobj == MINERAL_SCAN_PLANET)
            selected = (StarsI16)count;
        ++count;
    }

    /* A fleet marker is the high bit of the original menu tag.  The semantic
     * view uses a stable identity instead of exposing a far pointer. */
    for (fleet_index = 0; fleet_index < (StarsI16)input->fleet_count;
            ++fleet_index) {
        const StarsMineralScanFleet *fleet;
        if (!input->fleets) break;
        fleet = &input->fleets[fleet_index];
        if (!same_point(fleet->point, selected_point)) continue;
        if (count >= MINERAL_SCAN_MAX_ENTRIES) break;
        entries[count].identity = fleet->identity;
        entries[count].marker = MINERAL_SCAN_FLEET_ENTRY;
        entries[count].thing_index = -1;
        if (input->selection.grobj == MINERAL_SCAN_FLEET &&
                fleet->identity == (StarsU32)(StarsU16)input->selection.fleet)
            selected = (StarsI16)count;
        ++count;
    }

    for (thing_index = 0; thing_index < (StarsI16)input->thing_count;
            ++thing_index) {
        const StarsMineralScanThing *thing;
        if (!input->things) break;
        thing = &input->things[thing_index];
        if (!same_point(thing->point, selected_point)) continue;
        if (count >= MINERAL_SCAN_MAX_ENTRIES) break;
        entries[count].identity = thing->identity;
        entries[count].marker = MINERAL_SCAN_THING_ENTRY;
        entries[count].thing_index = thing->index;
        if (input->selection.grobj == MINERAL_SCAN_THING &&
                thing->index == input->selection.thing)
            selected = (StarsI16)count;
        ++count;
    }

    for (fleet_index = 0; fleet_index < (StarsI16)count; ++fleet_index) {
        identities[fleet_index] = entries[fleet_index].identity;
        markers[fleet_index] = entries[fleet_index].marker;
    }
    menu_index = input->popup_menu(input->context, input->window, input->x,
        input->y, count, identities, markers, selected);
    if (menu_index < 0 || menu_index >= (StarsI16)count)
        return;

    selected_identity = entries[menu_index].identity;
    next = input->selection;
    if ((entries[menu_index].marker & MINERAL_SCAN_FLEET_ENTRY) != 0U) {
        next.grobj = MINERAL_SCAN_FLEET;
        next.fleet = (StarsI16)selected_identity;
        for (fleet_index = 0;
                fleet_index < (StarsI16)input->fleet_count; ++fleet_index)
            if (input->fleets[fleet_index].identity == selected_identity)
                selected_owner = (StarsI16)(input->fleets[fleet_index].owner ==
                    input->player);
    } else if ((entries[menu_index].marker & MINERAL_SCAN_THING_ENTRY) != 0U) {
        next.grobj = MINERAL_SCAN_THING;
        next.thing = entries[menu_index].thing_index;
    } else {
        next.grobj = MINERAL_SCAN_PLANET;
        selected_owner = (StarsI16)(input->planet_owner == input->player);
    }
    input->change_selection(input->context, &next,
        MINERAL_SCAN_SELECTION_CHANGE);
    if (selected_owner) {
        if (input->redraw_selection)
            input->redraw_selection(input->context,
                MINERAL_SCAN_REDRAW_BEFORE, MINERAL_SCAN_REDRAW_AFTER);
        if (input->change_main_selection)
            input->change_main_selection(input->context, next.grobj,
                (StarsI16)selected_identity);
        if (input->redraw_selection)
            input->redraw_selection(input->context,
                MINERAL_SCAN_REDRAW_BEFORE, MINERAL_SCAN_REDRAW_AFTER);
    }
}
