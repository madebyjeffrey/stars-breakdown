#include "core.h"

#define IS_ACTIVE_FLAG 0x0200U
#define IS_MIN_DESIGNS 15U
#define IS_CREATE_RETRIES 5

static StarsI16 is_active(const StarsISShdef *design)
{
    return design && (design->active & IS_ACTIVE_FLAG) != 0U;
}

static StarsI16 is_create(const StarsEnsureISShdefsInput *input,
    StarsI16 slot, StarsU16 hull_id, StarsI16 name_id)
{
    return input->create_shdef ? input->create_shdef(input->context, slot,
        hull_id, name_id) : 0;
}

static void is_promote_empty(const StarsEnsureISShdefsInput *input,
    StarsI16 slot)
{
    StarsISShdef copy;
    if (is_active(&input->designs[slot]) ||
            input->designs[slot].existing != 0U)
        return;
    copy = input->designs[slot];
    copy.active = (StarsU16)(copy.active | IS_ACTIVE_FLAG);
    if (input->change_shdef)
        input->change_shdef(input->context, slot, &copy);
}

/* Genuine semantic reconstruction of EnsureISShdefs, NE 20:1938-1E22.
 *
 * The IS personality keeps a small catalogue of fixed ship roles.  Each role
 * has its own technology gate and hull family; slots zero, one, and two may
 * be promoted from an empty design record before their replacement is
 * created.  Randomized names are retried when the design table is full, while
 * the other roles use their fixed name-pool entries.  The original `iroCur`
 * argument is not consulted by this helper, so it is retained as
 * `current_order` for ABI/documentation context but does not affect policy. */
void stars_ensure_is_shdefs(StarsEnsureISShdefsInput *input)
{
    StarsI16 attempt;
    StarsI16 name_id;

    if (!input || !input->designs || input->design_count < IS_MIN_DESIGNS)
        return;
    (void)input->current_order;

    /* The two basic IS combat designs are always refreshed once their active
     * records exist; their names are fixed entries in the IS name table. */
    if (is_active(&input->designs[4]) && input->tech[2] > 4U)
        (void)is_create(input, 4, 1U, 14);
    if (is_active(&input->designs[5]) && input->tech[2] > 6U)
        (void)is_create(input, 5, 3U, 18);

    /* Slot 14 is a randomized hull-6 variant.  The four attempts mirror the
     * original table-full retry loop. */
    if (is_active(&input->designs[14]) && input->tech[1] > 4U &&
            input->tech[4] > 5U && input->tech[3] > 3U &&
            input->tech[2] > 4U) {
        for (attempt = 0; attempt < 4; ++attempt) {
            name_id = input->random_value ?
                (StarsI16)(68 + input->random_value(input->context, 1)) : 68;
            if (is_create(input, 14, 6U, name_id) != 0)
                break;
        }
    }

    /* Slots one and zero are the two core hulls.  An empty record is first
     * activated through the same value-style change service used by the
     * original routine. */
    if (is_active(&input->designs[1]) || input->designs[1].existing == 0U) {
        is_promote_empty(input, 1);
        (void)is_create(input, 1, 1U, 0);
    }
    if (is_active(&input->designs[0]) || input->designs[0].existing == 0U) {
        is_promote_empty(input, 0);
        (void)is_create(input, 0, 4U, 1);
    }
    if ((is_active(&input->designs[2]) || input->designs[2].existing == 0U) &&
            input->tech[3] > 6U && input->tech[4] > 3U) {
        is_promote_empty(input, 2);
        (void)is_create(input, 2, 17U, 15);
    }

    /* Remaining roles are strictly active-gated. */
    if (is_active(&input->designs[6]) && input->tech[3] > 3U &&
            input->tech[2] > 4U && input->tech[5] > 5U)
        (void)is_create(input, 6, 11U, 17);
    if (is_active(&input->designs[2]) && input->tech[1] > 7U &&
            input->tech[4] > 6U && input->tech[3] > 5U &&
            input->tech[2] > 6U)
        (void)is_create(input, 2, 17U, 15);
    if (is_active(&input->designs[3]) && input->tech[1] > 10U &&
            input->tech[4] > 11U && input->tech[3] > 14U &&
            input->tech[2] > 8U)
        (void)is_create(input, 3, 19U, 16);
    if (is_active(&input->designs[9]) && input->tech[1] > 4U &&
            input->tech[4] > 5U && input->tech[3] > 12U &&
            input->tech[2] > 6U) {
        for (attempt = 0; attempt < IS_CREATE_RETRIES; ++attempt) {
            name_id = input->random_value ?
                (StarsI16)(110 + input->random_value(input->context, 4)) : 110;
            if (is_create(input, 9, 9U, name_id) != 0)
                break;
        }
    }
}
