#include "core.h"
#include <string.h>

static StarsU32 stargate_add(StarsU32 left, StarsU32 right)
{
    return left + right;
}

static StarsI32 stargate_scaled(StarsI32 value, StarsI32 factor,
    StarsI32 divisor)
{
    StarsU32 product;

    if (divisor <= 0 || value <= 0 || factor <= 0)
        return 0;
    /* The original __aFlmul/__aFldiv helpers operate on 32-bit integers;
     * unsigned arithmetic makes the wrap point explicit on the host. */
    product = (StarsU32)value * (StarsU32)factor;
    return (StarsI32)(product / (StarsU32)divisor);
}

static StarsI32 stargate_weighted_damage(StarsI32 cargo_damage,
    StarsU32 units, StarsI32 hull_damage, StarsU32 remaining)
{
    StarsU32 numerator;

    if (remaining == 0U)
        return 0;
    /* The original divides the weighted damage by surviving cargo before
     * applying the 500/ship-weight scale.  Preserve that boundary and the
     * 32-bit intermediate wrap instead of using host floating point. */
    numerator = (StarsU32)cargo_damage * units;
    numerator += (StarsU32)hull_damage * remaining;
    return (StarsI32)(numerator / remaining);
}

static void stargate_message(StarsFStargateJumpInput *input,
    StarsI16 code, StarsI16 slot, StarsU32 amount)
{
    StarsFStargateMessage message;

    if (!input->send_message || !input->fleet)
        return;
    message.player = input->fleet->player;
    message.code = code;
    message.fleet_reference = (StarsU16)(input->fleet->id | 0x8000U);
    message.fleet_id = (StarsI16)input->fleet->id;
    message.planet = input->fleet->planet;
    message.destination = input->destination;
    message.slot = slot;
    message.amount = amount;
    input->send_message(input->context, &message);
}

static void stargate_empty(StarsFStargateJumpInput *input)
{
    if (!input->fleet)
        return;
    input->fleet->flags = (StarsU16)((input->fleet->flags & 0xfbffU) | 0x0400U);
    stargate_message(input, STARS_STARGATE_MSG_EMPTY, 0, 0U);
}

static StarsI16 stargate_random(StarsFStargateJumpInput *input,
    StarsI16 limit)
{
    if (!input->random_value || limit <= 0)
        return 0;
    return input->random_value(input->context, limit);
}

static void stargate_report_loss(StarsFStargateJumpInput *input,
    StarsU32 lost, StarsU32 total)
{
    StarsI16 code;

    if (lost == 0U)
        return;
    if ((lost >> 16) != 0U) {
        stargate_message(input, STARS_STARGATE_MSG_LARGE_LOSS, 0,
            lost);
        return;
    }
    if (lost < (total >> 2))
        code = STARS_STARGATE_MSG_LIGHT_LOSS;
    else if (lost > (total >> 1))
        code = STARS_STARGATE_MSG_HEAVY_LOSS;
    else
        code = STARS_STARGATE_MSG_MEDIUM_LOSS;
    stargate_message(input, code, 0, lost);
}

static void stargate_slot_loss(StarsFStargateJumpInput *input, StarsI16 slot,
    StarsI16 damage, StarsU32 *lost)
{
    StarsU32 remaining;
    StarsU32 damage_cargo;
    StarsU32 units;
    StarsI16 loss_chance;
    StarsI32 ship_weight;
    StarsI32 cargo_damage;
    StarsI32 hull_damage;
    StarsI32 updated_damage;
    StarsI16 unit;

    remaining = input->fleet->cargo[slot];
    damage_cargo = input->fleet->cargo_state[slot];
    loss_chance = input->race_stat_stargate_loss == 7 ? 0 :
        (StarsI16)(damage / 3);
    ship_weight = input->ship_weight[slot];
    if (damage_cargo == 0U)
        units = 0U;
    else {
        units = (StarsU32)(((StarsU32)remaining *
            (damage_cargo & 0xffff007fUL)) / 100U);
        if (units == 0U)
            units = 1U;
    }
    if (loss_chance != 0) {
        for (unit = 0; unit < (StarsI16)input->fleet->cargo[slot]; ++unit) {
            if (stargate_random(input, 100) < loss_chance) {
                --remaining;
                if (units != 0U &&
                    stargate_random(input, 500) < (StarsI16)(damage_cargo >> 7))
                    --units;
            }
        }
        *lost = stargate_add(*lost,
            input->fleet->cargo[slot] - remaining);
    }
    if (remaining != 0U) {
        if (damage_cargo == 0U)
            cargo_damage = 0;
        else {
            cargo_damage = stargate_scaled(ship_weight,
                (StarsI32)(damage_cargo >> 7), 500L);
            if (cargo_damage == 0)
                cargo_damage = 1;
        }
        hull_damage = stargate_scaled(ship_weight, (StarsI32)damage, 100L);
        if (hull_damage == 0)
            hull_damage = 1;
        if (units != 0U && ship_weight <= cargo_damage + hull_damage) {
            *lost = stargate_add(*lost, units);
            remaining -= units;
        }
        if (remaining != 0U) {
            updated_damage = stargate_weighted_damage(cargo_damage, units,
                hull_damage, remaining);
            updated_damage = stargate_scaled(updated_damage, 500L,
                ship_weight);
            if (updated_damage == 0)
                updated_damage = 1;
            input->fleet->cargo_state[slot] =
                (StarsU32)((updated_damage << 7) | 100U);
        }
    }
    input->fleet->cargo[slot] = remaining;
    if (remaining == 0U)
        input->fleet->cargo_state[slot] = 0U;
}

StarsI16 stars_f_stargate_jump(StarsFStargateJumpInput *input,
    StarsI16 source_gate, StarsI16 destination_gate, StarsI16 distance)
{
    StarsU32 total;
    StarsU32 lost;
    StarsU32 units;
    StarsU32 state;
    StarsI16 damage[STARS_STARGATE_CARGO_SLOTS];
    StarsI16 active;
    StarsI16 slot;
    StarsI16 result;
    StarsFStargateFleet original;
    StarsFStargateFleet lost_fleet;

    if (!input || !input->fleet || !input->stargate_damage)
        return 0;
    original = *input->fleet;
    total = 0U;
    lost = 0U;
    active = 0;
    for (slot = 0; slot < STARS_STARGATE_CARGO_SLOTS; ++slot) {
        damage[slot] = 0;
        units = input->fleet->cargo[slot];
        if (units == 0U)
            continue;
        ++active;
        total = stargate_add(total, units);
        result = input->stargate_damage(input->context, source_gate,
            destination_gate, distance, input->design_mass[slot],
            &damage[slot]);
        if (result == -2) {
            stargate_message(input, STARS_STARGATE_MSG_DAMAGE_ABORT, slot, 0U);
            return 0;
        }
        if (result == -1) {
            stargate_message(input, STARS_STARGATE_MSG_DAMAGE_FAIL, 0, 0U);
            return 0;
        }
        if (result == 0)
            --active;
        else if (result == 1)
            input->fleet->flags =
                (StarsU16)((input->fleet->flags & 0xbfffU) | 0x4000U);
    }
    if (active == 0) {
        stargate_empty(input);
        return 0;
    }
    for (slot = 0; slot < STARS_STARGATE_CARGO_SLOTS; ++slot) {
        if (input->fleet->cargo[slot] == 0U || damage[slot] == 0)
            continue;
        if (damage[slot] == 100) {
            lost = stargate_add(lost, input->fleet->cargo[slot]);
            input->fleet->cargo[slot] = 0U;
            input->fleet->cargo_state[slot] = 0U;
            --active;
        } else {
            state = input->fleet->cargo[slot];
            stargate_slot_loss(input, slot, damage[slot], &lost);
            if (input->fleet->cargo[slot] == 0U)
                --active;
            (void)state;
        }
    }
    if (active == 0) {
        stargate_empty(input);
        return 0;
    }
    stargate_report_loss(input, lost, total);
    memset(&lost_fleet, 0, sizeof lost_fleet);
    lost_fleet.player = original.player;
    lost_fleet.flags = (StarsU16)((original.flags & 0xfb00U) | 0x0407U);
    for (slot = 0; slot < STARS_STARGATE_CARGO_SLOTS; ++slot)
        lost_fleet.cargo[slot] = original.cargo[slot] -
            input->fleet->cargo[slot];
    if (lost != 0U && input->transfer_cargo_balance)
        input->transfer_cargo_balance(input->context, input->fleet,
            &lost_fleet);
    /* The original copies its surviving local source snapshot back to lpfl;
     * the second record is only the temporary lost-fleet service record. */
    return 1;
}
