#include "core.h"

enum {
    FILTER_MESSAGE_MAX_ID = 0x07ff,
    FILTER_PAIR_FIRST = 0x35,
    FILTER_PAIR_SECOND = 0x36,
    FILTER_PAIR_THIRD = 0x37,
    FILTER_PAIR_FOURTH = 0x38,
    FILTER_PAIR_FIFTH = 0x39,
    FILTER_PAIR_SIXTH = 0x3a,
    FILTER_FOUR_MESSAGE_GROUP_FIRST = 0x2b,
    FILTER_FOUR_MESSAGE_GROUP_LAST = 0x2e,
    FILTER_FIVE_MESSAGE_GROUP_FIRST = 0x60,
    FILTER_FIVE_MESSAGE_GROUP_LAST = 0x64,
    FILTER_FIVE_MESSAGE_GROUP_SECOND_FIRST = 0x6a,
    FILTER_FIVE_MESSAGE_GROUP_SECOND_LAST = 0x6e,
    FILTER_EIGHTEEN_MESSAGE_GROUP_FIRST = 0x91,
    FILTER_EIGHTEEN_MESSAGE_GROUP_LAST = 0xa8
};

static void filter_set_bit(StarsU8 *bits, StarsI16 id, StarsI16 enabled)
{
    StarsU16 byte_index;
    StarsU8 mask;
    if (!bits || id < 0 || id > FILTER_MESSAGE_MAX_ID)
        return;
    byte_index = (StarsU16)id >> 3;
    mask = (StarsU8)(1U << ((StarsU16)id & 7U));
    if (enabled) bits[byte_index] = (StarsU8)(bits[byte_index] | mask);
    else bits[byte_index] = (StarsU8)(bits[byte_index] & (StarsU8)~mask);
}

static void filter_set_range(StarsU8 *bits, StarsI16 first, StarsI16 last,
    StarsI16 enabled)
{
    StarsI16 id;
    for (id = first; id <= last; ++id)
        filter_set_bit(bits, id, enabled);
}

static StarsI16 filter_is_one_of(StarsI16 id, StarsI16 first,
    StarsI16 second)
{
    return (StarsI16)(id == first || id == second);
}

/* Genuine semantic reconstruction of SetFilteringGroups, NE 7:A018-A8E3.
 * The first bit is always the requested message.  Several UI checkboxes
 * represent a paired message or a contiguous report family, so the original
 * routine updates those companion IDs at the same time. */
void stars_set_filtering_group(StarsMessageState *state, StarsI16 message_id,
    StarsI16 enabled)
{
    StarsU8 *bits;
    StarsI16 companion;

    if (!state || message_id < 0 || message_id > FILTER_MESSAGE_MAX_ID)
        return;
    bits = state->filtered;
    filter_set_bit(bits, message_id, enabled != 0);

    if (filter_is_one_of(message_id, FILTER_PAIR_FIRST,
            FILTER_PAIR_SECOND)) {
        companion = (StarsI16)(message_id ^ 3);
        filter_set_bit(bits, companion, enabled != 0);
    } else if (filter_is_one_of(message_id, FILTER_PAIR_THIRD,
            FILTER_PAIR_FOURTH)) {
        companion = (StarsI16)(message_id ^ 0x0f);
        filter_set_bit(bits, companion, enabled != 0);
    } else if (filter_is_one_of(message_id, FILTER_PAIR_FIFTH,
            FILTER_PAIR_SIXTH)) {
        companion = (StarsI16)(message_id ^ 3);
        filter_set_bit(bits, companion, enabled != 0);
    } else if (message_id >= FILTER_FOUR_MESSAGE_GROUP_FIRST &&
            message_id <= FILTER_FOUR_MESSAGE_GROUP_LAST) {
        filter_set_range(bits, FILTER_FOUR_MESSAGE_GROUP_FIRST,
            FILTER_FOUR_MESSAGE_GROUP_LAST, enabled != 0);
    } else if (filter_is_one_of(message_id, 0x2f, 0x30)) {
        companion = (StarsI16)(message_id ^ 0x1f);
        filter_set_bit(bits, companion, enabled != 0);
    } else if (filter_is_one_of(message_id, 0x42, 0x43) ||
            filter_is_one_of(message_id, 0x44, 0x45) ||
            filter_is_one_of(message_id, 0x46, 0x47) ||
            filter_is_one_of(message_id, 0x4a, 0x4b) ||
            filter_is_one_of(message_id, 0x4c, 0x4d)) {
        companion = (StarsI16)(message_id ^ 1);
        filter_set_bit(bits, companion, enabled != 0);
    } else if (filter_is_one_of(message_id, 0x48, 0x49)) {
        companion = (StarsI16)(message_id ^ 1);
        filter_set_bit(bits, companion, enabled != 0);
    } else if (message_id >= FILTER_FIVE_MESSAGE_GROUP_FIRST &&
            message_id <= FILTER_FIVE_MESSAGE_GROUP_LAST) {
        filter_set_range(bits, FILTER_FIVE_MESSAGE_GROUP_FIRST,
            FILTER_FIVE_MESSAGE_GROUP_LAST, enabled != 0);
    } else if (message_id >= FILTER_FIVE_MESSAGE_GROUP_SECOND_FIRST &&
            message_id <= FILTER_FIVE_MESSAGE_GROUP_SECOND_LAST) {
        filter_set_range(bits, FILTER_FIVE_MESSAGE_GROUP_SECOND_FIRST,
            FILTER_FIVE_MESSAGE_GROUP_SECOND_LAST, enabled != 0);
    } else if (filter_is_one_of(message_id, 0x79, 0x7a)) {
        companion = (StarsI16)(message_id ^ 3);
        filter_set_bit(bits, companion, enabled != 0);
    } else if (message_id > 0x90 &&
            message_id <= FILTER_EIGHTEEN_MESSAGE_GROUP_LAST) {
        filter_set_range(bits, FILTER_EIGHTEEN_MESSAGE_GROUP_FIRST,
            FILTER_EIGHTEEN_MESSAGE_GROUP_LAST, enabled != 0);
    }
}
