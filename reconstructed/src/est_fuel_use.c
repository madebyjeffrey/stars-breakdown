#include "core.h"

#define EST_FUEL_NO_ENGINE 99999L
#define EST_FUEL_SCOOP_ENGINE 10
#define EST_FUEL_RACE_DISCOUNT 15L
#define EST_FUEL_PERCENT 100L
#define EST_FUEL_SCALE 2000L
#define EST_FUEL_RANGE_SCALE 1000L
#define EST_FUEL_FLAG_SCOOP 0x0040U

static StarsI32 est_fuel_from_double(double value)
{
    /* EstFuelUse calls the original fixed-point runtime for these products.
     * Keep the intermediate wide, as the compiler's signed 32-bit multiply
     * would otherwise overflow before the division. */
    if (value >= 2147483647.0)
        return (StarsI32)2147483647L;
    if (value <= -2147483648.0)
        return (StarsI32)(-2147483647L - 1L);
    return (StarsI32)value;
}

static StarsI32 est_fuel_mul_div(StarsI32 left, StarsI32 right,
    StarsI32 divisor)
{
    if (divisor == 0)
        return 0;
    return est_fuel_from_double((double)left * (double)right /
        (double)divisor);
}

static StarsI32 est_fuel_cost_term(StarsI32 hull_fuel_weight,
    StarsU32 cargo_count, StarsI32 used, StarsI32 fuel, StarsI32 travel)
{
    double weight;
    double fuel_distance;

    /* The raw routine has a fast fixed-point path for small products and a
     * floating-point path for large products.  Both represent this same
     * truncating quotient; using double here also makes the large path safe
     * without relying on signed-overflow behavior. */
    weight = (double)hull_fuel_weight * (double)cargo_count +
        (double)used;
    fuel_distance = (double)fuel * (double)travel;
    return est_fuel_from_double(weight * fuel_distance /
        (double)EST_FUEL_SCALE);
}

static StarsI32 est_fuel_slot_value(const StarsEstFuelInput *input,
    const StarsEstFuelSlot *slot)
{
    StarsI32 fuel;

    if (!slot->engine_available)
        return EST_FUEL_NO_ENGINE;
    fuel = slot->fuel_used[input->warp];
    if (input->race_grbit)
        fuel -= est_fuel_mul_div(fuel, EST_FUEL_RACE_DISCOUNT,
            EST_FUEL_PERCENT);
    return fuel;
}

static StarsI32 est_fuel_total_weight(const StarsEstFuelFleet *fleet)
{
    StarsI16 index;
    StarsI32 total;

    total = 0;
    for (index = 0; index < 4; ++index)
        total += fleet->mineral_weight[index];
    return total;
}

static StarsI32 est_fuel_distance(StarsEstFuelInput *input)
{
    if (input->travel != -1L)
        return input->travel;
    if (input->range_only)
        return 1000L;
    if (input->distance)
        return input->distance(input->context, input->order, input->warp,
            input->fleet->player, input->fleet->destination);
    return 0;
}

/* Genuine source reconstruction of EstFuelUse, NE 0B:9FE4-A6FF.  The
 * original processes ship slots in increasing engine-fuel order, consumes
 * available mineral weight against each hull's capacity, and converts the
 * resulting fixed-point fuel use into either fuel cost or reachable range. */
StarsI32 stars_est_fuel_use(StarsEstFuelInput *input)
{
    StarsI32 fuel_values[STARS_EST_FUEL_SLOTS];
    StarsI32 remaining_weight;
    StarsI32 total_fuel;
    StarsI32 travel;
    StarsI32 current_fuel;
    StarsI32 next_fuel;
    StarsI32 capacity;
    StarsI32 used;
    StarsI16 slot;
    StarsI16 found;
    StarsI16 index;

    if (!input || !input->fleet)
        return 0;
    if (input->game_flags)
        *input->game_flags = (StarsU16)(*input->game_flags &
            (StarsU16)~EST_FUEL_FLAG_SCOOP);
    if (input->warp == -1 && input->warp_from_order)
        input->warp = input->warp_from_order(input->context,
            input->order, input->fleet->player);
    if (input->warp < 0 || input->warp >= STARS_EST_FUEL_WARPS)
        return 0;
    for (slot = 0; slot < STARS_EST_FUEL_SLOTS; ++slot) {
        fuel_values[slot] = est_fuel_slot_value(input,
            &input->fleet->slots[slot]);
        if (input->fleet->slots[slot].cargo_count != 0U &&
            input->fleet->slots[slot].engine_id == EST_FUEL_SCOOP_ENGINE &&
            input->game_flags)
            *input->game_flags = (StarsU16)(*input->game_flags |
                EST_FUEL_FLAG_SCOOP);
    }
    remaining_weight = est_fuel_total_weight(input->fleet);
    total_fuel = 0;
    current_fuel = 0;
    travel = est_fuel_distance(input);
    for (;;) {
        next_fuel = EST_FUEL_NO_ENGINE + 1L;
        found = 0;
        for (slot = 0; slot < STARS_EST_FUEL_SLOTS; ++slot) {
            if (input->fleet->slots[slot].cargo_count == 0U)
                continue;
            if (fuel_values[slot] == current_fuel) {
                capacity = est_fuel_mul_div(
                    (StarsI32)input->fleet->slots[slot].cargo_count,
                    input->fleet->slots[slot].ship_stat, 1L);
                used = capacity;
                if (used > remaining_weight)
                    used = remaining_weight;
                if (used < 0)
                    used = 0;
                remaining_weight -= used;
                total_fuel += est_fuel_cost_term(
                    input->fleet->slots[slot].hull_fuel_weight,
                    input->fleet->slots[slot].cargo_count, used,
                    current_fuel, travel);
            } else if (fuel_values[slot] > current_fuel &&
                       fuel_values[slot] < next_fuel) {
                next_fuel = fuel_values[slot];
                found = 1;
            }
        }
        if (!found)
            break;
        current_fuel = next_fuel;
    }
    if (!input->range_only)
        total_fuel += 9L;
    total_fuel /= 10L;
    if (!input->range_only)
        return total_fuel;
    if (total_fuel == 0)
        return 1000000000L;
    if (total_fuel < 100001L)
        return est_fuel_mul_div(input->fleet->cargo_weight, 1000L,
            total_fuel);
    index = (StarsI16)(total_fuel / 1000L);
    return index == 0 ? 0 : (input->fleet->cargo_weight / index);
}
