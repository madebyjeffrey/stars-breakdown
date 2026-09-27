#include <stdio.h>
#include "core.h"

typedef struct Trace { StarsU16 id; } Trace;

static StarsI16 lookup(void *context, StarsU16 id,
    StarsUpdateSlotGlobalsHull *hull)
{
    Trace *trace = (Trace *)context;
    static const StarsU8 positions[2] = { 0x21, 0x43 };
    trace->id = id;
    hull->component_count = 2;
    hull->component_rects = positions;
    hull->cargo_weight_max = 1;
    hull->cargo_rect = 0x31a2;
    return 1;
}

int main(void)
{
    StarsUpdateSlotGlobalsInput input;
    StarsRect slots[2];
    StarsRect cargo;
    StarsPoint plaque;
    StarsPoint origin;
    Trace trace;
    origin.x = 500;
    origin.y = 0;
    trace.id = 0;
    input.has_build_design = 1;
    input.build_hull_id = 9;
    input.slot_dialog = 0;
    input.arial8_height = 8;
    input.slot_origin = origin;
    input.slot_rects = slots;
    input.cargo_rect = &cargo;
    input.plaque = &plaque;
    input.lookup_hull = lookup;
    input.context = &trace;
    stars_update_slot_globals(&input);
    if (trace.id != 9U || slots[0].left != 44 || slots[0].top != 84 ||
            slots[0].right != 108 || slots[0].bottom != 148 ||
            slots[1].left != 108 || slots[1].top != 148 ||
            cargo.left != 44 || cargo.top != 116 || cargo.right != 76 ||
            cargo.bottom != 340 || plaque.x != 270 || plaque.y != 293)
        return 1;
    input.slot_dialog = 1;
    stars_update_slot_globals(&input);
    if (slots[0].left != 202 || slots[0].top != 96 || plaque.x != 428 ||
            plaque.y != 305)
        return 2;
    puts("UPDATE_SLOT_GLOBALS_SOURCE=PASS");
    return 0;
}
