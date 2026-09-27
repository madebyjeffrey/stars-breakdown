#include "core.h"

/* Typed semantic reconstruction of IDropPart, NE 26:5476-59f0.  Packed
 * hull-slot records and redraw/update side effects are callback-owned. */

static void normalize_source(StarsDropPartInput *input)
{
    if (input->key_control == 0 && input->key_shift == 0) {
        if (input->source_index >= 0)
            input->source_quantity = (StarsU16)(input->source_quantity | 0x0100);
    } else if (input->key_control == 0) {
        if (input->source_index < 0 ||
                (input->source_quantity >> 8) > 4)
            input->source_quantity = (StarsU16)(input->source_quantity | 0x0400);
    } else if (input->source_index < 0) {
        input->source_quantity = (StarsU16)(input->source_quantity | 0x6400);
    }
}

static StarsI16 drop_part_packed_hit(const StarsDropPartInput *input,
    StarsI16 slot)
{
    const StarsRect *rect;

    if (!input->packed_slot_rects ||
            slot < 0 || slot >= input->packed_slot_rect_count)
        return 0;
    rect = &input->packed_slot_rects[slot];
    return input->point.x >= (StarsI16)rect->left &&
        input->point.x < (StarsI16)rect->right &&
        input->point.y >= (StarsI16)rect->top &&
        input->point.y < (StarsI16)rect->bottom;
}

static void redraw_empty_drop(StarsDropPartInput *input)
{
    StarsRect rect;

    if (input->ops->get_client_rect) {
        input->ops->get_client_rect(input->ops->context, &rect);
        if (input->ops->draw_slot)
            input->ops->draw_slot(input->ops->context, 0, &rect,
                input->source_index);
        rect.top = (StarsU16)input->y_build_info_sum;
        if (input->ops->invalidate_rect)
            input->ops->invalidate_rect(input->ops->context, &rect, 1);
        else if (input->ops->invalidate)
            input->ops->invalidate(input->ops->context);
        if (input->ops->draw_selected_component)
            input->ops->draw_selected_component(input->ops->context, 0, -1);
        if (input->ops->draw_selected_hull)
            input->ops->draw_selected_hull(input->ops->context, 0, -1,
                (const StarsRect *)0);
    } else if (input->ops->invalidate) {
        input->ops->invalidate(input->ops->context);
    }
}

static void redraw_merged_drop(StarsDropPartInput *input)
{
    StarsRect rect;

    if (input->ops->get_client_rect) {
        input->ops->get_client_rect(input->ops->context, &rect);
        rect.top = (StarsU16)input->y_build_info_sum;
        if (input->ops->invalidate_rect)
            input->ops->invalidate_rect(input->ops->context, &rect, 1);
        else if (input->ops->invalidate)
            input->ops->invalidate(input->ops->context);
    } else if (input->ops->invalidate) {
        input->ops->invalidate(input->ops->context);
    }
}

StarsI16 stars_drop_part(StarsDropPartInput *input)
{
    StarsI16 slot;
    StarsDropPartSlot *target;
    StarsDropPartSlot *source;
    StarsU16 target_amount;
    StarsU16 target_capacity;
    StarsU16 source_amount;
    StarsU16 merged_amount;
    StarsU16 moved_amount;
    StarsU16 source_current;

    if (!input || !input->ops)
        return 0;
    normalize_source(input);
    slot = -1;
    if (input->packed_slot_rects) {
        for (slot = 0; slot < input->slot_count; ++slot)
            if (drop_part_packed_hit(input, slot))
                break;
    } else if (input->ops->hit_test) {
        for (slot = 0; slot < input->slot_count; ++slot)
            if (input->ops->hit_test(input->ops->context, slot,
                    &input->point))
                break;
    }
    if (slot >= input->slot_count) {
        if (input->point.x < (StarsI16)(input->client_width >> 1)) {
            if (!input->no_modify && input->source_index >= 0) {
                if (input->slots && input->source_index < input->slot_count) {
                    source = &input->slots[input->source_index];
                    source_current = (StarsU16)(source->quantity >> 8);
                    source_amount = (StarsU16)(input->source_quantity >> 8);
                    moved_amount = source_amount < source_current ?
                        source_amount : source_current;
                    source->quantity = (StarsU16)(
                        ((source_current - moved_amount) << 8) |
                        (source->quantity & 0xff));
                }
                if (input->ops->empty_drop)
                    input->ops->empty_drop(input->ops->context,
                        input->source_index, &input->source_grhst,
                        &input->source_quantity);
                if (input->ops->update_cost)
                    input->ops->update_cost(input->ops->context,
                        input->source_index);
                redraw_empty_drop(input);
                if (input->ops->advance_tutor)
                    input->ops->advance_tutor(input->ops->context);
            }
            return 1;
        }
        return 0;
    }
    target = &input->slots[slot];
    if (target->grhst & 1)
        input->source_quantity = (StarsU16)(input->source_quantity | 0x6400);
    if (slot == input->source_index)
        return 2;
    target_amount = (StarsU16)(target->quantity >> 8);
    target_capacity = target->capacity != 0 ?
        (StarsU16)(target->capacity >> 8) : target_amount;
    source_amount = (StarsU16)(input->source_quantity >> 8);
    if (target_amount < source_amount &&
            (target_amount == 0 ||
             (target->grhst == input->source_grhst &&
              (target->quantity & 0xff) ==
              (input->source_quantity & 0xff))) &&
            (target_amount != 0 || (input->source_grhst & target->grhst))) {
        if (!input->no_modify) {
            merged_amount = (StarsU16)(target_amount + source_amount);
            if (merged_amount > target_capacity)
                merged_amount = target_capacity;
            moved_amount = (StarsU16)(merged_amount - target_amount);
            if (input->slots && input->source_index >= 0 &&
                    input->source_index < input->slot_count) {
                source = &input->slots[input->source_index];
                source_current = (StarsU16)(source->quantity >> 8);
                if (moved_amount > source_current)
                    moved_amount = source_current;
                source->quantity = (StarsU16)(
                    ((source_current - moved_amount) << 8) |
                    (source->quantity & 0xff));
            }
            target->grhst = input->source_grhst;
            target->quantity = (StarsU16)((merged_amount << 8) |
                (input->source_quantity & 0xff));
            if (input->source_index >= 0 && input->ops->update_cost)
                input->ops->update_cost(input->ops->context,
                    input->source_index);
            if (input->ops->set_selection)
                input->ops->set_selection(input->ops->context, slot);
            redraw_merged_drop(input);
            if (input->ops->advance_tutor)
                input->ops->advance_tutor(input->ops->context);
        }
        return -1;
    }
    if (!input->no_modify && input->ops->beep)
        input->ops->beep(input->ops->context);
    return 3;
}
