#include "core.h"

/* NE 1A:6528-6715. Resolve the selected hull and derive the slot, cargo,
 * and plaque rectangles used by the ship-builder dialog.  The original
 * stores the hull's packed component positions in a far table; the callback
 * exposes that table without exposing the original global address. */
void stars_update_slot_globals(StarsUpdateSlotGlobalsInput *input)
{
    StarsUpdateSlotGlobalsHull hull;
    StarsI16 x;
    StarsI16 y;
    StarsU16 i;
    StarsU16 packed;

    if (!input || !input->has_build_design || !input->lookup_hull)
        return;
    hull.component_count = 0;
    hull.component_rects = 0;
    hull.cargo_weight_max = 0;
    hull.cargo_rect = 0;
    if (!input->lookup_hull(input->context, input->build_hull_id, &hull) ||
            !hull.component_rects)
        return;
    if (input->slot_dialog) {
        x = (StarsI16)(input->slot_origin.x - 330);
        y = 32;
    } else {
        x = 12;
        y = (StarsI16)(input->arial8_height + 12);
    }
    if (input->slot_rects) {
        for (i = 0; i < (StarsU16)hull.component_count; ++i) {
            packed = hull.component_rects[i];
            input->slot_rects[i].left = (StarsI16)((packed & 0x000fU) * 32U + x);
            input->slot_rects[i].top = (StarsI16)((packed >> 4) * 32U + y);
            input->slot_rects[i].right =
                (StarsI16)(input->slot_rects[i].left + 64);
            input->slot_rects[i].bottom =
                (StarsI16)(input->slot_rects[i].top + 64);
        }
    }
    if (hull.cargo_weight_max && input->cargo_rect) {
        input->cargo_rect->left =
            (StarsI16)(((hull.cargo_rect >> 8) & 0x000fU) * 32U + x);
        input->cargo_rect->top =
            (StarsI16)((hull.cargo_rect >> 12) * 32U + y);
        input->cargo_rect->right =
            (StarsI16)((hull.cargo_rect & 0x000fU) * 32U + x);
        input->cargo_rect->bottom =
            (StarsI16)(((hull.cargo_rect >> 4) & 0x000fU) * 32U + y);
    }
    if (input->plaque) {
        input->plaque->x = (StarsI16)(x + 258);
        input->plaque->y = (StarsI16)(y + 273);
    }
}
