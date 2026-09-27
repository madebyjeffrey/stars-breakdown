#include "core.h"

/* NE 11:AE74-C280. Rebalance the cargo and per-design damage fields when a
 * fleet is split.  The old routine did this with two local _fleet snapshots,
 * 16.16-style long arithmetic helpers, and byte offsets into rgcsh/rgwtMin.
 * The implementation below names those boundaries and keeps the original
 * order: resolve snapshots, account for ship-capacity deltas, rebalance
 * packed design status, then commit conserved cargo between the two new
 * fleets. */

static StarsI32 balance_mul(StarsI32 left, StarsI32 right)
{
    return (StarsI32)((StarsU32)left * (StarsU32)right);
}

static StarsI32 balance_div(StarsI32 numerator, StarsI32 denominator)
{
    StarsU32 magnitude;
    StarsU32 divisor;
    StarsU32 quotient;
    StarsU16 negative;

    if (denominator == 0L) return 0L;
    negative = (StarsU16)((numerator < 0L) != (denominator < 0L));
    magnitude = numerator < 0L ? (StarsU32)0 - (StarsU32)numerator :
        (StarsU32)numerator;
    divisor = denominator < 0L ? (StarsU32)0 - (StarsU32)denominator :
        (StarsU32)denominator;
    quotient = magnitude / divisor;
    return negative ? (StarsI32)((StarsU32)0 - quotient) :
        (StarsI32)quotient;
}

static StarsI32 balance_ceil_positive(StarsI32 numerator, StarsI32 denominator)
{
    if (numerator <= 0L || denominator <= 0L) return 0L;
    return balance_div(numerator + denominator - 1L, denominator);
}

static void balance_add(StarsI32 *value, StarsI32 change)
{
    *value = (StarsI32)((StarsU32)*value + (StarsU32)change);
}

static StarsI16 balance_nonzero_count(StarsI16 count)
{
    return count > 0 ? count : 0;
}

static StarsI32 balance_capacity(const StarsFleetBalanceFleet *fleet,
    StarsI16 design, StarsI16 stat, StarsFleetBalanceWeight weight,
    void *context)
{
    StarsI16 count;
    StarsI16 unit;

    count = balance_nonzero_count(fleet->ship_count[design]);
    if (count == 0 || !weight) return 0L;
    unit = weight(context, fleet->player, design, stat);
    if (unit < 0) unit = 0;
    return balance_mul((StarsI32)count, (StarsI32)unit);
}

static StarsI32 balance_positive_delta_capacity(StarsI16 delta,
    StarsI16 unit)
{
    if (delta <= 0 || unit <= 0) return 0L;
    return balance_mul((StarsI32)delta, (StarsI32)unit);
}

static void balance_status(const StarsFleetBalanceFleet *resolved,
    StarsFleetBalanceFleet * const new_fleets[2],
    StarsI16 delta[2][STARS_FLEET_BALANCE_DESIGNS], StarsI16 design)
{
    StarsI16 donor;
    StarsI16 receiver;
    StarsI16 source_count;
    StarsI16 receiver_count;
    StarsI16 donor_count;
    StarsI32 source_units;
    StarsI32 receiver_units;
    StarsI32 transferred;
    StarsI32 receiver_low;
    StarsI32 receiver_high;
    StarsU16 status;

    if (delta[0][design] == 0 || delta[0][design] != -delta[1][design])
        return;
    donor = delta[0][design] < 0 ? 0 : 1;
    receiver = (StarsI16)(1 - donor);
    source_count = resolved[donor].ship_count[design];
    receiver_count = new_fleets[receiver]->ship_count[design];
    donor_count = new_fleets[donor]->ship_count[design];
    if (source_count < 1) source_count = 0;
    if (receiver_count < 1) receiver_count = 1;
    if (donor_count < 1) donor_count = 1;

    status = resolved[donor].ship_status[design];
    source_units = balance_div(balance_mul((StarsI32)(status &
        STARS_FLEET_BALANCE_STATUS_MASK), (StarsI32)source_count), 100L);
    status = resolved[receiver].ship_status[design];
    receiver_units = balance_div(balance_mul((StarsI32)(status &
        STARS_FLEET_BALANCE_STATUS_MASK),
        (StarsI32)balance_nonzero_count(resolved[receiver].ship_count[design])),
        100L);
    transferred = balance_nonzero_count((StarsI16)(0 - delta[donor][design]));
    if (transferred > source_units) transferred = source_units;

    if (source_units == 0L) {
        if (receiver_units == 0L)
            new_fleets[receiver]->ship_status[design] &=
                (StarsU16)~STARS_FLEET_BALANCE_STATUS_MASK;
        else {
            receiver_low = balance_ceil_positive(receiver_units * 100L,
                (StarsI32)receiver_count);
            new_fleets[receiver]->ship_status[design] =
                (StarsU16)((new_fleets[receiver]->ship_status[design] &
                (StarsU16)~STARS_FLEET_BALANCE_STATUS_MASK) |
                ((StarsU16)receiver_low & STARS_FLEET_BALANCE_STATUS_MASK));
        }
        return;
    }

    if (receiver_units == 0L) {
        receiver_low = balance_ceil_positive(transferred * 100L,
            (StarsI32)receiver_count);
        new_fleets[receiver]->ship_status[design] =
            (StarsU16)((new_fleets[receiver]->ship_status[design] &
            (StarsU16)~STARS_FLEET_BALANCE_STATUS_MASK) |
            ((StarsU16)receiver_low & STARS_FLEET_BALANCE_STATUS_MASK));
    } else {
        receiver_high = balance_ceil_positive(
            balance_mul(transferred, (StarsI32)(resolved[donor].ship_status[
                design] >> 7)) + balance_mul(receiver_units,
                (StarsI32)(resolved[receiver].ship_status[design] >> 7)),
            (StarsI32)receiver_count);
        receiver_low = balance_ceil_positive((transferred + receiver_units) *
            100L, (StarsI32)receiver_count);
        new_fleets[receiver]->ship_status[design] =
            (StarsU16)(((StarsU16)receiver_low &
                STARS_FLEET_BALANCE_STATUS_MASK) |
                ((StarsU16)receiver_high << 7));
    }

    if (transferred == source_units)
        new_fleets[donor]->ship_status[design] = 0U;
    else {
        receiver_low = balance_ceil_positive((source_units - transferred) *
            100L, (StarsI32)donor_count);
        new_fleets[donor]->ship_status[design] =
            (StarsU16)((new_fleets[donor]->ship_status[design] &
            (StarsU16)~STARS_FLEET_BALANCE_STATUS_MASK) |
            ((StarsU16)receiver_low & STARS_FLEET_BALANCE_STATUS_MASK));
    }
}

static StarsI32 balance_sum_minerals(const StarsFleetBalanceFleet *fleet)
{
    StarsI16 resource;
    StarsI32 total;

    total = 0L;
    for (resource = 0; resource < STARS_FLEET_BALANCE_MINERALS; ++resource)
        balance_add(&total, fleet->cargo[resource]);
    return total;
}

static void balance_cargo_side(const StarsFleetBalanceFleet *fleet,
    StarsI32 cargo_capacity, StarsI32 fuel_capacity,
    StarsI32 cargo_gain, StarsI32 fuel_gain, StarsI32 adjustment[5])
{
    StarsI16 resource;
    StarsI32 fuel_transfer;
    StarsI32 mineral_total;
    StarsI32 mineral_transfer;
    StarsI32 remaining;
    StarsI32 share;

    /* The original uses a signed fixed-point threshold before applying the
     * capacity fraction.  For ordinary cargo values this is the same as the
     * positive-load path; retaining the sign check matters for empty and
     * partially initialized fleets. */
    fuel_transfer = 0L;
    if (cargo_capacity != 0L && cargo_gain != 0L && fleet->cargo[4] < 0x0000afc9L)
        fuel_transfer = balance_div(balance_mul(fleet->cargo[4], cargo_gain),
            cargo_capacity);
    adjustment[4] = (StarsI32)((StarsU32)adjustment[4] -
        (StarsU32)fuel_transfer);

    mineral_total = balance_sum_minerals(fleet);
    mineral_transfer = 0L;
    if (fuel_capacity != 0L && fuel_gain != 0L && mineral_total < 0x0000afc9L)
        mineral_transfer = balance_div(balance_mul(mineral_total, fuel_gain),
            fuel_capacity);
    remaining = mineral_transfer;
    if (remaining != 0L && mineral_total != 0L) {
        for (resource = 0; resource < STARS_FLEET_BALANCE_MINERALS;
             ++resource) {
            share = balance_div(balance_mul(fleet->cargo[resource],
                mineral_transfer), mineral_total);
            if (share > remaining) share = remaining;
            if (share < 0L) share = 0L;
            adjustment[resource] = (StarsI32)((StarsU32)adjustment[resource] -
                (StarsU32)share);
            remaining -= share;
        }
        for (resource = 0; resource < STARS_FLEET_BALANCE_MINERALS &&
             remaining > 0L; ++resource) {
            while (remaining > 0L && fleet->cargo[resource] +
                adjustment[resource] > 0L) {
                --adjustment[resource];
                --remaining;
            }
        }
    }
}

void stars_fleet_transfer_cargo_balance(
    StarsFleetTransferCargoBalanceInput *input)
{
    StarsFleetBalanceFleet resolved[2];
    StarsFleetBalanceFleet *new_fleets[2];
    StarsI16 delta[2][STARS_FLEET_BALANCE_DESIGNS];
    StarsI32 total_cargo[2];
    StarsI32 total_fuel[2];
    StarsI32 cargo_gain[2];
    StarsI32 fuel_gain[2];
    StarsI32 adjustment[2][STARS_FLEET_BALANCE_RESOURCES];
    StarsI16 side;
    StarsI16 design;
    StarsI16 resource;
    StarsI16 unit_cargo;
    StarsI16 unit_fuel;
    StarsI16 special;

    if (!input || !input->new_fleets[0] || !input->new_fleets[1]) return;
    new_fleets[0] = input->new_fleets[0];
    new_fleets[1] = input->new_fleets[1];
    special = 0;
    for (side = 0; side < 2; ++side) {
        if (new_fleets[side]->flags & STARS_FLEET_BALANCE_SPECIAL_FLAG) {
            resolved[side].player = input->player;
            resolved[side].flags = 0U;
            for (design = 0; design < STARS_FLEET_BALANCE_DESIGNS; ++design) {
                resolved[side].ship_count[design] = 0;
                resolved[side].ship_status[design] = 0U;
            }
            for (resource = 0; resource < STARS_FLEET_BALANCE_RESOURCES;
                 ++resource) resolved[side].cargo[resource] = 0L;
            special = 1;
        } else if (input->lookup_fleet) {
            if (!input->lookup_fleet(input->context, input->fleet_ids[side],
                &resolved[side])) {
                resolved[side] = *new_fleets[side];
            }
        } else {
            resolved[side] = *new_fleets[side];
        }
        total_cargo[side] = 0L;
        total_fuel[side] = 0L;
        cargo_gain[side] = 0L;
        fuel_gain[side] = 0L;
        for (resource = 0; resource < STARS_FLEET_BALANCE_RESOURCES;
             ++resource) adjustment[side][resource] = 0L;
    }

    for (design = 0; design < STARS_FLEET_BALANCE_DESIGNS; ++design) {
        if (input->max_weight) {
            unit_cargo = input->max_weight(input->context, input->player,
                design, 1);
            unit_fuel = input->max_weight(input->context, input->player,
                design, 2);
        } else {
            unit_cargo = 0;
            unit_fuel = 0;
        }
        for (side = 0; side < 2; ++side) {
            delta[side][design] = (StarsI16)(resolved[side].ship_count[design] -
                new_fleets[side]->ship_count[design]);
            total_cargo[side] += balance_capacity(&resolved[side], design, 1,
                input->max_weight, input->context);
            total_fuel[side] += balance_capacity(&resolved[side], design, 2,
                input->max_weight, input->context);
            cargo_gain[side] += balance_positive_delta_capacity(
                delta[side][design], unit_cargo);
            fuel_gain[side] += balance_positive_delta_capacity(
                delta[side][design], unit_fuel);
        }
        if (!special) balance_status(resolved, new_fleets, delta, design);
    }
    for (side = 0; side < 2; ++side)
        balance_cargo_side(new_fleets[side], total_cargo[side],
            total_fuel[side], cargo_gain[side], fuel_gain[side], adjustment[side]);

    for (side = 0; side < 2; ++side) {
        for (resource = 0; resource < STARS_FLEET_BALANCE_RESOURCES;
             ++resource) {
            balance_add(&new_fleets[side]->cargo[resource],
                adjustment[side][resource]);
            balance_add(&new_fleets[1 - side]->cargo[resource],
                (StarsI32)(0L - adjustment[side][resource]));
        }
    }
}
