#include "core.h"

/* Typed semantic reconstruction of EstMineralsMined, NE 6:5362-5a16.
 * Fixed-point helpers and packed planet/fleet mutation remain explicit
 * callbacks; the estimate, rounding, concentration floor, and depletion
 * decisions stay in ordinary C. */

static StarsI32 mineral_mul(const StarsMineralMinedOps *ops, StarsI32 left,
    StarsI32 right)
{
    return ops->fixed_multiply ? ops->fixed_multiply(ops->context, left,
        right) : left * right;
}

static StarsI32 mineral_div(const StarsMineralMinedOps *ops, StarsI32 value,
    StarsI32 divisor)
{
    return ops->fixed_divide ? ops->fixed_divide(ops->context, value,
        divisor) : value / divisor;
}

static StarsI32 mineral_rem(const StarsMineralMinedOps *ops, StarsI32 value,
    StarsI32 divisor)
{
    return ops->fixed_remainder ? ops->fixed_remainder(ops->context, value,
        divisor) : value % divisor;
}

static StarsU16 mineral_level(StarsU16 concentration)
{
    if (concentration < 5U)
        return 10U;
    if (concentration < 25U)
        return 25U;
    if (concentration < 101U)
        return concentration;
    return 100U;
}

static void mineral_apply_exact(StarsMineralMinedInput *input,
    StarsI16 index, StarsI32 amount, StarsI32 depletion_product)
{
    const StarsMineralMinedOps *ops;
    StarsI32 remaining;
    StarsI32 threshold;
    StarsI32 last_threshold;
    StarsI32 level_divisor;
    StarsI32 percent;
    StarsI32 maximum_percent;
    StarsI32 consumed;
    StarsU16 concentration;

    ops = input->ops;
    input->mineral_weight[index] += amount;
    /* The original routine applies the efficiency factor to the reported
     * mineral amount, but its concentration-depletion loop uses the
     * pre-efficiency product divided by 100.  Keeping these quantities
     * separate matters for an ordinary (non-special-race) populated turn. */
    remaining = mineral_div(ops, depletion_product, 100);
    concentration = input->concentration[index];
    last_threshold = 0;
    maximum_percent = input->percent_level[index] ?
        input->percent_level[index] : 0x100L;
    percent = maximum_percent;
    while (remaining >= 1 && concentration >= 2U) {
        level_divisor = (StarsI32)mineral_level(concentration);
        threshold = mineral_div(ops,
            mineral_div(ops, mineral_mul(ops, percent, 0x30d4L), 0x100L),
            level_divisor);
        last_threshold = threshold;
        if (remaining < threshold)
            break;
        remaining -= threshold;
        --concentration;
        maximum_percent = 0x100L;
        percent = 0x100L;
        input->percent_level[index] = 0;
    }
    if (remaining >= 1 && concentration >= 2U) {
        level_divisor = (StarsI32)mineral_level(concentration);
        threshold = mineral_div(ops, 0x30d4L, level_divisor);
        consumed = last_threshold - remaining;
        if (consumed < 1)
            consumed = 1;
        percent = mineral_div(ops,
            mineral_mul(ops, consumed, 0x100L), threshold);
        if (maximum_percent <= percent)
            percent = maximum_percent - 1L;
        input->percent_level[index] = (StarsU8)percent;
        if (percent == 0)
            --concentration;
    }
    input->concentration[index] = (StarsU8)concentration;
}

void stars_est_minerals_mined(StarsMineralMinedInput *input)
{
    const StarsMineralMinedOps *ops;
    StarsI32 mines;
    StarsI32 product;
    StarsI32 depletion_product;
    StarsI32 amount;
    StarsI32 remainder;
    StarsI16 efficiency;
    StarsI16 index;
    StarsU16 concentration;
    StarsU8 percent;

    if (!input || !input->ops)
        return;
    ops = input->ops;
    efficiency = input->special_race ? 10 : input->efficiency_tenths;
    mines = input->c_mines;
    if (mines == -1) {
        if (input->player < 0 ||
                (!input->special_race && input->operating_mines == 0)) {
            for (index = 0; index < 3; ++index)
                input->output[index] = -1;
            return;
        }
        mines = ops->operating_mines ?
            ops->operating_mines(ops->context) : input->operating_mines;
    } else {
        efficiency = 10;
    }
    for (index = 0; index < 3; ++index) {
        concentration = input->concentration[index];
        if (concentration < 30 && input->force_concentration_floor &&
                (input->c_mines == -1 || input->special_race))
            concentration = 30;
        product = mineral_mul(ops, mines, concentration);
        depletion_product = product;
        if (input->c_mines == -1)
            product = mineral_div(ops, mineral_mul(ops, product, efficiency),
                10);
        remainder = mineral_rem(ops, product, 100);
        amount = mineral_div(ops, product, 100);
        if (remainder != 0 && input->random_rounding) {
            if (!ops->random || ops->random(ops->context, 100) < remainder)
                ++amount;
        }
        input->output[index] = amount;
        if (input->apply) {
            percent = input->percent_level[index];
            if (input->mineral_weight) {
                mineral_apply_exact(input, index, amount, depletion_product);
            } else if (ops->apply_mineral) {
                ops->apply_mineral(ops->context, index, amount,
                    &concentration, &percent);
                input->concentration[index] = concentration;
                input->percent_level[index] = percent;
                while (amount > 0 && concentration >= 2 && ops->can_deplete &&
                        ops->can_deplete(ops->context, index, amount)) {
                    --concentration;
                    percent = 0;
                    if (!ops->can_deplete(ops->context, index, amount))
                        break;
                }
                input->concentration[index] = concentration;
                input->percent_level[index] = percent;
            }
        }
    }
    /* Macinti-style planets include eligible fleets' mine counts in the
     * estimate.  The original recursively called EstMineralsMined for each
     * contributing fleet, then cleared that fleet's mine order when applying
     * the result.  Keep fleet discovery and order mutation as typed host
     * boundaries while retaining the recursive arithmetic here. */
    if (input->c_mines == -1 && input->special_race &&
            ops->fleet_mines) {
        StarsMineralMinedInput fleet_input;
        StarsI32 fleet_mines;
        StarsI16 mineral_index;

        fleet_input = *input;
        fleet_input.c_mines = 0;
        fleet_input.fleet_count = 0;
        for (mineral_index = 0; mineral_index < 3; ++mineral_index)
            fleet_input.output[mineral_index] = 0;
        for (index = 0; index < input->fleet_count; ++index) {
            if (!ops->fleet_mines(ops->context, index, &fleet_mines))
                break;
            if (fleet_mines <= 0)
                continue;
            fleet_input.c_mines = fleet_mines;
            for (mineral_index = 0; mineral_index < 3; ++mineral_index)
                fleet_input.output[mineral_index] = 0;
            stars_est_minerals_mined(&fleet_input);
            for (mineral_index = 0; mineral_index < 3; ++mineral_index) {
                input->output[mineral_index] +=
                    fleet_input.output[mineral_index];
                if (input->apply) {
                    input->concentration[mineral_index] =
                        fleet_input.concentration[mineral_index];
                    input->percent_level[mineral_index] =
                        fleet_input.percent_level[mineral_index];
                }
            }
            if (input->apply && ops->fleet_consumed)
                ops->fleet_consumed(ops->context, index);
        }
    }
}
