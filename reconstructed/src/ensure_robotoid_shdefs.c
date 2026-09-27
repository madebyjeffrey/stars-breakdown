#include "core.h"

#define ROBOTOID_ACTIVE_FLAG 0x0200U
#define ROBOTOID_MAX_DESIGNS 16U
#define ROBOTOID_CREATE_RETRIES 5
#define ROBOTOID_SPECIAL_NAME (-1)

/* Design slots are stable roles in the Robotoid production plan.  The names
 * are intentionally semantic; their numeric values are the original string
 * pool selectors, not magic values invented by this reconstruction. */
#define ROBOTOID_SLOT_MINER_0 2
#define ROBOTOID_SLOT_MINER_LAST 5
#define ROBOTOID_SLOT_ESCORT_0 6
#define ROBOTOID_SLOT_ESCORT_LAST 7
#define ROBOTOID_SLOT_RAIDER_0 9
#define ROBOTOID_SLOT_RAIDER_LAST 10
#define ROBOTOID_SLOT_WAR_0 11
#define ROBOTOID_SLOT_WAR_LAST 13
#define ROBOTOID_SLOT_STARBASE 14
#define ROBOTOID_SLOT_HEAVY_RAIDER 15

static StarsI16 robotoid_random(const StarsEnsureRobotoidShdefsInput *input,
    StarsI16 limit)
{
    StarsI16 value;
    if (!input->random_value || limit <= 0)
        return 0;
    value = input->random_value(input->context, limit);
    if (value < 0 || value >= limit)
        return 0;
    return value;
}

static StarsI16 robotoid_is_active(const StarsRobotoidShdef *design)
{
    return design && (design->active & ROBOTOID_ACTIVE_FLAG) != 0U;
}

static StarsI16 robotoid_due(const StarsEnsureRobotoidShdefsInput *input,
    StarsI16 slot, StarsI16 recycle_turns)
{
    const StarsRobotoidShdef *design;
    const StarsRobotoidShdef *previous;
    if (!input->designs || slot < 0 || (StarsU16)slot >= input->design_count)
        return 0;
    design = &input->designs[slot];
    if (!robotoid_is_active(design))
        return 0;
    if (slot == ROBOTOID_SLOT_MINER_0 || slot == ROBOTOID_SLOT_ESCORT_0 ||
            slot == ROBOTOID_SLOT_RAIDER_0 || slot == ROBOTOID_SLOT_WAR_0)
        return 1;
    previous = &input->designs[slot - 1];
    return !robotoid_is_active(previous) &&
        input->turn - previous->turn_created > recycle_turns;
}

static StarsI16 robotoid_create_retry(
    const StarsEnsureRobotoidShdefsInput *input, StarsI16 slot,
    StarsU16 hull_id, StarsI16 name_base, StarsI16 name_count)
{
    StarsI16 attempt;
    StarsI16 name_id;
    if (!input->create_shdef)
        return 0;
    for (attempt = 0; attempt < ROBOTOID_CREATE_RETRIES; ++attempt) {
        name_id = name_base;
        if (name_count > 0)
            name_id = (StarsI16)(name_base +
                robotoid_random(input, name_count));
        if (input->create_shdef(input->context, slot, hull_id, name_id) != 0)
            return 1;
    }
    return 0;
}

/* Genuine semantic reconstruction of EnsureRobotoidShdefs, NE 18:20AE-288E.
 *
 * The original routine is not a general design generator.  It is a periodic
 * maintenance pass for the Robotoid personality: technology gates enable
 * several fixed design slots, an inactive predecessor and an elapsed-turn
 * interval make replacement designs eligible, and each creation is retried
 * when the design table has no free slot.  The final block upgrades the
 * starter design once the Robotoid has reached the required technology and
 * the starter has never been built.  The callbacks represent the original
 * FCreateAiShdef/FChangeAiShdef services while keeping the decision rules in
 * ordinary, readable C. */
void stars_ensure_robotoid_shdefs(StarsEnsureRobotoidShdefsInput *input)
{
    StarsI16 slot;
    StarsI16 name_base;
    StarsI16 created;
    StarsRobotoidShdef starter;

    if (!input || !input->designs ||
            input->design_count < ROBOTOID_MAX_DESIGNS)
        return;

    /* Slots 11..13 are the ordinary Robotoid war-fleet progression.  Early
     * technology uses hull 11; later technology picks one of six hull-31
     * variants. */
    for (slot = ROBOTOID_SLOT_WAR_0; slot <= ROBOTOID_SLOT_WAR_LAST; ++slot) {
        if (input->tech[2] > 1U &&
                input->tech[3] >= (StarsU8)((slot - 11) * 3 + 4) &&
                robotoid_due(input, slot, 14)) {
            if (input->tech[3] < 10U) {
                name_base = (slot == ROBOTOID_SLOT_WAR_0) ? 14 : 15;
                (void)robotoid_create_retry(input, slot, 11U, name_base, 0);
            } else {
                (void)robotoid_create_retry(input, slot, 31U, 8, 6);
            }
        }
    }

    /* Slot 14 is the starbase-support design.  Its preferred hull is tried
     * five times before ordinary hull-6 variants are considered. */
    if (robotoid_is_active(&input->designs[ROBOTOID_SLOT_STARBASE]) &&
            input->tech[1] > 4U && input->tech[4] > 5U &&
            input->tech[3] > 5U && input->tech[2] > 5U &&
            input->tech[0] > 1U) {
        created = 0;
        for (slot = 0; slot < ROBOTOID_CREATE_RETRIES && !created; ++slot)
            created = input->create_shdef ? input->create_shdef(
                input->context, ROBOTOID_SLOT_STARBASE, 29U,
                ROBOTOID_SPECIAL_NAME) : 0;
        if (!created)
            (void)robotoid_create_retry(input, ROBOTOID_SLOT_STARBASE, 6U,
                16, 4);
    }

    /* Slot 15 is the heavy-raider role.  The fixed hull-29 design is kept as
     * the first choice, with hull-6 alternates after a failed allocation. */
    if (robotoid_is_active(&input->designs[ROBOTOID_SLOT_HEAVY_RAIDER]) &&
            input->tech[4] > 9U && input->tech[3] > 7U &&
            input->tech[2] > 8U && input->tech[1] > 13U) {
        if (!input->create_shdef || input->create_shdef(input->context,
                ROBOTOID_SLOT_HEAVY_RAIDER, 29U, ROBOTOID_SPECIAL_NAME) == 0)
            (void)robotoid_create_retry(input, ROBOTOID_SLOT_HEAVY_RAIDER, 6U,
                20, 4);
    }

    /* Slots 2..5 are four mineral/escort variants.  The even and odd slots
     * use separate four-name pools, which is why the source tracks parity. */
    for (slot = ROBOTOID_SLOT_MINER_0; slot <= ROBOTOID_SLOT_MINER_LAST; ++slot) {
        if (input->tech[1] > 9U && input->tech[3] > 9U &&
                input->tech[2] > 8U && input->tech[0] > 5U &&
                robotoid_due(input, slot, 12)) {
            name_base = ((slot - ROBOTOID_SLOT_MINER_0) & 1) ? 4 : 0;
            (void)robotoid_create_retry(input, slot, 31U, name_base, 4);
        }
    }

    /* Slots 6 and 7 are late-game raider escorts.  They require the full
     * combat technology gate and are recycled less frequently than miners. */
    for (slot = ROBOTOID_SLOT_ESCORT_0; slot <= ROBOTOID_SLOT_ESCORT_LAST; ++slot) {
        if (input->tech[5] > 3U && input->tech[4] > 9U &&
                input->tech[3] > 11U && input->tech[2] > 11U &&
                input->tech[0] > 5U && input->tech[1] > 14U &&
                robotoid_due(input, slot, 20)) {
            name_base = (slot == ROBOTOID_SLOT_ESCORT_0) ? 27 : 31;
            (void)robotoid_create_retry(input, slot, 9U, name_base, 4);
        }
    }

    /* Slots 9 and 10 are the simple raider pair.  The original first tries a
     * shared special bitmap/name, then falls back once to the matching hull-19
     * name; there is no retry loop around that fallback. */
    for (slot = ROBOTOID_SLOT_RAIDER_0; slot <= ROBOTOID_SLOT_RAIDER_LAST; ++slot) {
        if (input->tech[1] > 13U && robotoid_due(input, slot, 15)) {
            name_base = (slot == ROBOTOID_SLOT_RAIDER_0) ? 36 : 37;
            if (!input->create_shdef || input->create_shdef(input->context,
                    slot, 9U, ROBOTOID_SPECIAL_NAME) == 0)
                if (input->create_shdef)
                    (void)input->create_shdef(input->context, slot, 19U,
                        name_base);
        }
    }

    /* Once every design role has been inspected, the starter can be promoted
     * to the Robotoid's hull-5 role.  Copying the compact record before the
     * callback mirrors FChangeAiShdef's value-style update. */
    starter = input->designs[0];
    if (starter.hull_id != 5U && starter.existing == 0U &&
            input->starter_hull != 0U && input->starter_existing == 0U &&
            input->tech[5] > 3U && input->tech[4] > 4U &&
            input->tech[3] > 5U && input->tech[2] > 5U &&
            input->tech[0] > 5U) {
        starter.active = (StarsU16)(starter.active | ROBOTOID_ACTIVE_FLAG);
        starter.hull_id = 5U;
        if (input->change_shdef)
            input->change_shdef(input->context, 0, &starter);
        if (input->create_shdef)
            (void)input->create_shdef(input->context, 0, 5U,
                ROBOTOID_SPECIAL_NAME);
    }
}
