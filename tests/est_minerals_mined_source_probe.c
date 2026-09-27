#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 random_calls;
    StarsI16 apply_calls;
    StarsI16 deplete_calls;
    StarsI16 fleet_calls;
    StarsI16 fleet_consumed_calls;
    StarsI32 operating_mines;
} Trace;
static StarsI32 multiply(void *context, StarsI32 left, StarsI32 right)
{
    (void)context; return left * right;
}
static StarsI32 divide_fixed(void *context, StarsI32 value, StarsI32 divisor)
{
    (void)context; return value / divisor;
}
static StarsI32 remainder_fixed(void *context, StarsI32 value,
    StarsI32 divisor)
{
    (void)context; return value % divisor;
}
static StarsI16 random_value(void *context, StarsI16 limit)
{
    (void)limit; ++((Trace *)context)->random_calls; return 0;
}
static StarsI32 operating_mines(void *context)
{
    return ((Trace *)context)->operating_mines;
}
static StarsI16 can_deplete(void *context, StarsI16 index, StarsI32 amount)
{
    (void)index; (void)amount; ++((Trace *)context)->deplete_calls;
    return ((Trace *)context)->deplete_calls == 1;
}
static void apply_mineral(void *context, StarsI16 index, StarsI32 amount,
    StarsU16 *concentration, StarsU8 *percent)
{
    (void)index; (void)amount; ++((Trace *)context)->apply_calls;
    *concentration = (StarsU16)*concentration;
    *percent = *percent;
}
static StarsI16 fleet_mines(void *context, StarsI16 index, StarsI32 *mines)
{
    Trace *trace = (Trace *)context;
    ++trace->fleet_calls;
    if (index != 0)
        return 0;
    *mines = 5;
    return 1;
}
static void fleet_consumed(void *context, StarsI16 index)
{
    (void)index;
    ++((Trace *)context)->fleet_consumed_calls;
}
static void install(StarsMineralMinedOps *ops, Trace *trace)
{
    memset(ops, 0, sizeof *ops);
    ops->fixed_multiply = multiply;
    ops->fixed_divide = divide_fixed;
    ops->fixed_remainder = remainder_fixed;
    ops->random = random_value;
    ops->operating_mines = operating_mines;
    ops->can_deplete = can_deplete;
    ops->apply_mineral = apply_mineral;
    ops->fleet_mines = fleet_mines;
    ops->fleet_consumed = fleet_consumed;
    ops->context = trace;
}
int main(void)
{
    StarsMineralMinedInput input;
    StarsMineralMinedOps ops;
    Trace trace;

    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    install(&ops, &trace);
    input.player = -1;
    input.c_mines = -1;
    input.ops = &ops;
    stars_est_minerals_mined(&input);
    if (input.output[0] != -1 || input.output[1] != -1 ||
            input.output[2] != -1)
        return 1;
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    install(&ops, &trace);
    trace.operating_mines = 10;
    input.player = 0;
    input.special_race = 1;
    input.c_mines = -1;
    input.force_concentration_floor = 1;
    input.random_rounding = 1;
    input.apply = 1;
    input.concentration[0] = 10;
    input.concentration[1] = 41;
    input.concentration[2] = 0;
    input.percent_level[0] = 5;
    input.ops = &ops;
    stars_est_minerals_mined(&input);
    if (input.output[0] != 3 || input.output[1] != 5 ||
            input.output[2] != 3 || trace.random_calls != 1 ||
            trace.apply_calls != 3 || trace.deplete_calls < 1)
        return 1;
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    install(&ops, &trace);
    trace.operating_mines = 10;
    input.player = 0;
    input.special_race = 1;
    input.c_mines = -1;
    input.fleet_count = 2;
    input.apply = 1;
    input.concentration[0] = 100;
    input.concentration[1] = 100;
    input.concentration[2] = 100;
    input.ops = &ops;
    stars_est_minerals_mined(&input);
    if (input.output[0] != 14 || input.output[1] != 15 ||
            input.output[2] != 15 || trace.fleet_calls != 2 ||
            trace.fleet_consumed_calls != 1 || input.concentration[0] != 99)
        return 2;
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    install(&ops, &trace);
    {
        StarsI32 weight[3];
        memset(weight, 0, sizeof weight);
        input.player = 0;
        input.c_mines = 10;
        input.apply = 1;
        input.concentration[0] = 50;
        input.percent_level[0] = 100;
        input.mineral_weight = weight;
        input.ops = &ops;
        stars_est_minerals_mined(&input);
        if (input.output[0] != 5 || weight[0] != 5 ||
                input.concentration[0] != 50 ||
                input.percent_level[0] != 94)
            return 3;
        memset(weight, 0, sizeof weight);
        input.percent_level[0] = 0;
        stars_est_minerals_mined(&input);
        if (input.output[0] != 5 || weight[0] != 5 ||
                input.percent_level[0] != 250)
            return 4;
    }
    puts("EST_MINERALS_MINED_CHECKS=29 FAILURES=0 RESULT=PASS");
    return 0;
}
