#include "core.h"

/* Typed semantic reconstruction of TransferStuff, NE 11:4faa-5f98.  The
 * packed fleet/planet records, dialog, and application refreshes are exposed
 * as typed operations so the orchestration remains visible without replaying
 * bytes or inventing record layouts. */

StarsI16 stars_transfer_stuff(StarsTransferInput *input)
{
    StarsTransferOps const *ops;
    StarsTransferObject *objects[2];
    StarsTransferObject split;
    StarsI16 valid_hulls[16];
    StarsI16 valid_count;
    StarsI16 i;
    StarsI16 empty_fleet;
    StarsI16 split_requested;
    StarsI16 original_second_type;

    if (!input || !input->ops)
        return 0;
    ops = input->ops;
    original_second_type = input->second.object_type;
    split_requested = 0;
    objects[0] = &input->first;
    objects[1] = &input->second;
    split.object_type = 2;
    split.object_id = -1;
    split.has_cargo = 0;
    split.is_selected_planet = 0;
    for (i = 0; i < 2; ++i) {
        if (objects[i]->object_type == 4) {
            if (input->mode == 1) {
                if (ops->split_fleet && !ops->split_fleet(ops->context,
                        &input->first, &split))
                    return 0;
                split.object_type = 2;
                input->second = split;
                objects[1] = &input->second;
                split_requested = 1;
            } else if (ops->jettison) {
                /* The mode-0 sentinel is the jettison destination; the
                 * source fleet is the first transfer record. */
                ops->jettison(ops->context, i == 1 ? &input->first :
                    objects[i]);
            }
        } else {
            if (ops->lookup_object &&
                    !ops->lookup_object(ops->context,
                        objects[i]->object_type, objects[i]->object_id,
                        objects[i], 0))
                return 0;
            if (ops->prepare_object &&
                    !ops->prepare_object(ops->context, objects[i],
                        input->mode))
                return 0;
        }
    }
    valid_count = 0;
    if (input->mode == 1 && ops->enumerate_hulls)
        valid_count = ops->enumerate_hulls(ops->context, &input->first,
            valid_hulls, 16);
    (void)valid_count;
    if (ops->advance_tutor)
        ops->advance_tutor(ops->context);
    if (ops->show_dialog)
        input->dialog_result = ops->show_dialog(ops->context);
    if (!input->dialog_result) {
        if (input->mode != 1 || !split_requested ||
                original_second_type != 4) {
            input->mode = -1;
            return 0;
        }
        if (ops->delete_fleet)
            ops->delete_fleet(ops->context, &input->second);
        if (ops->cancel_memory)
            ops->cancel_memory(ops->context);
        input->mode = -1;
        return 0;
    }
    if (input->mode == 1) {
        empty_fleet = input->second.has_cargo == 0;
        if (empty_fleet && split_requested && original_second_type == 4) {
            if (ops->delete_fleet)
                ops->delete_fleet(ops->context, &input->second);
            if (ops->cancel_memory)
                ops->cancel_memory(ops->context);
            input->mode = -1;
            return 0;
        }
        if (ops->balance)
            ops->balance(ops->context, &input->first, &input->second);
    }
    if (ops->lookup_object) {
        for (i = 0; i < 2; ++i) {
            if (!ops->lookup_object(ops->context, objects[i]->object_type,
                    objects[i]->object_id, objects[i], 1) &&
                    objects[i]->object_type != 4)
                continue;
            if (ops->refresh_object)
                ops->refresh_object(ops->context, objects[i]->object_type,
                    objects[i]->object_id);
        }
    }
    if (ops->refresh_selection)
        ops->refresh_selection(ops->context, input->first.object_type);
    if (ops->invalidate_window)
        ops->invalidate_window(ops->context, input->second.object_type);
    if (input->scanner_bits != 0 && ops->invalidate_window)
        ops->invalidate_window(ops->context, input->scanner_mode);
    input->mode = -1;
    return 0;
}
