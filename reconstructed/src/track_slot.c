#include "core.h"

static StarsI16 track_slot_in_rect(const StarsRect *rect, StarsI16 x,
    StarsI16 y)
{
    return x >= rect->left && x < rect->right &&
        y >= rect->top && y < rect->bottom;
}

static StarsI16 track_slot_hit(const StarsTrackSlotInput *input)
{
    StarsU16 index;

    for (index = 0; index < input->part_count &&
            index < STARS_TRACK_SLOT_MAX; ++index) {
        if (track_slot_in_rect(&input->slot_rect[index], input->x,
                input->y))
            return (StarsI16)index;
    }
    return -1;
}

/* Genuine C reconstruction of FTrackSlot, NE 1A:306A-3A8F.  The original
 * routine combines a slot hit test with several USER/GDI tracking loops.  A
 * caller supplies those platform effects through callbacks; the decisions
 * about what is selectable, inspectable, draggable, or a popup remain here. */
StarsI16 stars_track_slot(StarsTrackSlotInput *input)
{
    StarsI16 slot;
    StarsI16 direction;
    StarsTrackSlotPart part;

    if (!input || input->part_count == 0)
        return 0;

    if (!input->right_button && input->slot_dialog != 0) {
        if (track_slot_in_rect(&input->spin_rect[0], input->x, input->y))
            direction = -1;
        else if (track_slot_in_rect(&input->spin_rect[1], input->x, input->y))
            direction = 1;
        else
            direction = 0;
        if (direction != 0) {
            if (input->ops && input->ops->track_spin) {
                while (input->ops->track_spin(input->context, input->window,
                        direction, &input->current_bitmap) != 0)
                    ;
            }
            return 1;
        }
    }

    if (input->list_box) {
        slot = -1;
        if (!input->ops || !input->ops->read_list_part ||
            !input->ops->read_list_part(input->context, input->window,
                &slot, &part))
            return 0;
    }
    else {
        slot = track_slot_hit(input);
        if (slot < 0)
            return 0;
        part = input->slot_part[slot];
        if (part.kind == STARS_TRACK_PART_EMPTY) {
            if (input->ops && input->ops->select_slot)
                input->ops->select_slot(input->context, slot);
            return 0;
        }
    }

    if (input->list_box) {
        if (part.group == 1U && part.kind == 0U && input->hull_id != 0x0eU) {
            if (input->ops && input->ops->show_popup)
                input->ops->show_popup(input->context, input->window, input->x,
                    input->y, STARS_TRACK_POPUP_EMPTY);
            return 1;
        }
        if (part.group == STARS_TRACK_PART_STARBASE && part.kind == 1U &&
            input->hull_id != 0x0fU) {
            if (input->ops && input->ops->show_popup)
                input->ops->show_popup(input->context, input->window, input->x,
                    input->y, STARS_TRACK_POPUP_STARBASE);
            return 1;
        }
        if (part.group == STARS_TRACK_PART_CARGO && part.kind == 0U &&
            ((part.hull_flags >> 6) & 0x0fU) != 0U) {
            if (input->ops && input->ops->show_popup)
                input->ops->show_popup(input->context, input->window, input->x,
                    input->y, STARS_TRACK_POPUP_RESTRICTED);
            return 1;
        }
    }

    if (input->ops && input->ops->select_slot)
        input->ops->select_slot(input->context, slot);

    if (!input->right_button) {
        if (input->build_mode == 4U) {
            if (input->ops && input->ops->drag_part)
                input->ops->drag_part(input->context, input->window, slot,
                    &part, input->x, input->y);
            return 1;
        }
        return 0;
    }

    if (input->ops && input->ops->inspect_part)
        input->ops->inspect_part(input->context, input->window, input->x,
            input->y, &part);
    return 1;
}
