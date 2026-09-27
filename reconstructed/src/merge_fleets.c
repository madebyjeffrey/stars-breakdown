#include <string.h>
#include "core.h"

static StarsU16 merge_word(const StarsU8 *bytes, StarsU16 offset)
{
    return (StarsU16)(bytes[offset] |
        ((StarsU16)bytes[offset + 1U] << 8));
}

static void merge_put_word(StarsU8 *bytes, StarsU16 offset, StarsU16 value)
{
    bytes[offset] = (StarsU8)value;
    bytes[offset + 1U] = (StarsU8)(value >> 8);
}

/* NE 11:C9F6-CAFD. Merge the fixed 124-byte fleet records through explicit
 * service boundaries for cargo balancing, fleet lookup, deletion and report
 * invalidation.  The 16 ship-count words and the no-delete flag are handled
 * here because they are the direct state changes in the original routine. */
void stars_merge_two_fleets(const StarsMergeFleetInput *input)
{
    StarsU8 destination[124];
    StarsU8 deleted[124];
    StarsU16 slot;
    StarsU16 destination_id;
    StarsU16 deleted_id;
    if (!input || !input->destination || !input->deleted) return;
    memcpy(destination, input->destination, sizeof destination);
    memcpy(deleted, input->deleted, sizeof deleted);
    for (slot = 0; slot < 16U; ++slot) {
        StarsU16 offset = (StarsU16)(12U + slot * 2U);
        StarsU16 total = (StarsU16)(merge_word(destination, offset) +
            merge_word(deleted, offset));
        merge_put_word(destination, offset, total);
        merge_put_word(deleted, offset, 0U);
    }
    if (input->cargo_balance)
        input->cargo_balance(input->context, destination, deleted);
    if (input->lookup) {
        input->lookup(input->context, -1, destination);
        input->lookup(input->context, -1, deleted);
    }
    memcpy(input->destination, destination, sizeof destination);
    memcpy(input->deleted, deleted, sizeof deleted);
    destination_id = merge_word(destination, 0U);
    deleted_id = merge_word(deleted, 0U);
    if (input->no_delete == 0) {
        if (input->delete_fleet)
            input->delete_fleet(input->context, (StarsI16)deleted_id, 2,
                (StarsI16)destination_id);
        if (input->invalidate_report)
            input->invalidate_report(input->context, 1, 2);
    } else {
        merge_put_word(input->deleted, 4U,
            (StarsU16)((merge_word(input->deleted, 4U) & 0xfbffU) |
                0x0400U));
    }
}
