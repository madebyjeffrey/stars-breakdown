#include "core.h"

#define TURIN_ACTIVE_FLAG 0x0200U
#define TURIN_MIN_DESIGNS 16U
#define TURIN_CREATE_RETRIES 5
#define TURIN_SPECIAL_NAME (-1)

static StarsI16 turin_active(const StarsTurinDroneShdef *design)
{
    return design && (design->active & TURIN_ACTIVE_FLAG) != 0U;
}

static StarsI16 turin_create(const StarsEnsureTurinDroneShdefsInput *input,
    StarsI16 slot, StarsU16 hull, StarsI16 name_id)
{
    return input->create_shdef ? input->create_shdef(input->context, slot,
        hull, name_id) : 0;
}

static void turin_promote_empty(const StarsEnsureTurinDroneShdefsInput *input,
    StarsI16 slot, StarsU16 replacement_hull)
{
    StarsTurinDroneShdef copy;
    if (turin_active(&input->designs[slot]) ||
            input->designs[slot].existing != 0U)
        return;
    copy = input->designs[slot];
    copy.active = (StarsU16)(copy.active | TURIN_ACTIVE_FLAG);
    if (copy.hull_id != replacement_hull && input->change_shdef)
        input->change_shdef(input->context, slot, &copy);
}

/* Genuine semantic reconstruction of EnsureTurinDroneShdefs, NE 18:58BA-5F32.
 *
 * Turin Drone designs are maintained as named roles rather than generated
 * from one universal formula.  The early slots are permitted to be either
 * active or empty, in which case an empty slot is promoted before its design
 * is created.  Later roles are only refreshed when their active bit is set.
 * The callbacks replace the original FCreateAiShdef and FChangeAiShdef calls;
 * the role numbers, hull families, technology gates, and bounded retries are
 * kept explicit so the inferred personality policy remains reviewable. */
void stars_ensure_turin_drone_shdefs(StarsEnsureTurinDroneShdefsInput *input)
{
    StarsI16 attempt;
    StarsI16 name_id;

    if (!input || !input->designs || input->design_count < TURIN_MIN_DESIGNS)
        return;

    /* Fuel/transport roles. */
    if (turin_active(&input->designs[8]) && input->tech[2] > 4U &&
            input->tech[3] > 7U)
        (void)turin_create(input, 8, 12U, 12);
    if (turin_active(&input->designs[9]) && input->tech[2] > 6U &&
            input->tech[3] > 10U)
        (void)turin_create(input, 9, 13U, 15);
    if (turin_active(&input->designs[10]) && input->tech[1] > 4U &&
            input->tech[4] > 4U && input->tech[3] > 3U &&
            input->tech[2] > 4U) {
        for (attempt = 0; attempt < 4; ++attempt) {
            name_id = input->random_value ?
                input->random_value(input->context, 1) : 0;
            if (turin_create(input, 10, 6U, name_id) != 0)
                break;
        }
    }

    /* The three core combat roles can be created after an empty slot is
     * promoted.  This is why they use active-or-empty tests in the original. */
    if ((turin_active(&input->designs[1]) || input->designs[1].existing == 0U)) {
        turin_promote_empty(input, 1, 11U);
        (void)turin_create(input, 1, 15U, 0);
    }
    if ((turin_active(&input->designs[0]) || input->designs[0].existing == 0U)) {
        turin_promote_empty(input, 0, 5U);
        (void)turin_create(input, 0, 5U, 1);
    }
    if ((turin_active(&input->designs[2]) || input->designs[2].existing == 0U) &&
            input->tech[3] > 6U && input->tech[4] > 3U) {
        turin_promote_empty(input, 2, 22U);
        (void)turin_create(input, 2, 22U, 17);
    }

    /* Mining and scouting support roles. */
    if ((turin_active(&input->designs[12]) ||
            input->designs[12].existing == 0U) && input->tech[3] > 3U &&
            input->tech[5] > 3U)
        (void)turin_create(input, 12, 11U, 14);
    if (turin_active(&input->designs[13]) && input->tech[1] > 7U &&
            input->tech[4] > 6U && input->tech[3] > 5U)
        (void)turin_create(input, 13, 18U, 13);
    if (turin_active(&input->designs[14]) && input->tech[1] > 10U &&
            input->tech[4] > 11U && input->tech[3] > 14U &&
            input->tech[2] > 8U)
        (void)turin_create(input, 14, 18U, 13);
    if (turin_active(&input->designs[4]) && input->tech[1] > 4U &&
            input->tech[4] > 5U && input->tech[3] > 12U &&
            input->tech[2] > 6U) {
        for (attempt = 0; attempt < TURIN_CREATE_RETRIES; ++attempt) {
            name_id = input->random_value ?
                input->random_value(input->context, 4) : 0;
            if (turin_create(input, 4, 9U, name_id) != 0)
                break;
        }
    }
    if (turin_active(&input->designs[15]) && input->tech[1] > 4U &&
            input->tech[4] > 5U && input->tech[3] > 12U &&
            input->tech[2] > 6U) {
        for (attempt = 0; attempt < TURIN_CREATE_RETRIES; ++attempt)
            if (turin_create(input, 15, 12U, 12) != 0)
                break;
    }
}
