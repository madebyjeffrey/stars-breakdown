#include "core.h"

/* NE 0A:5DD2-5FCB. Pick the environmental axis whose legal adjustment
 * produces the greatest desirability per point changed. */
StarsI16 stars_best_terraform(const StarsBestTerraformInput *input,
    StarsI16 help)
{
    StarsI16 saved_player;
    StarsI16 min[3];
    StarsI16 max[3];
    StarsI16 cost[3];
    StarsI16 candidate[3];
    StarsI32 score[3];
    StarsI16 base;
    StarsI16 value;
    StarsI16 i;
    StarsI16 best;
    signed char saved_env;

    if (!input || !input->planet || !input->current_player ||
            !input->can_terraform || !input->desirability)
        return 0;
    saved_player = *input->current_player;
    *input->current_player = input->planet->player;
    if (*input->current_player == -1) {
        *input->current_player = saved_player;
        return 0;
    }
    if (!input->can_terraform(input->context, input->planet, help, min, max,
            cost)) {
        *input->current_player = saved_player;
        return 0;
    }
    base = input->desirability(input->context, input->planet,
        input->planet->player);
    for (i = 0; i < 3; ++i) {
        if (min[i] == -1) {
            if (max[i] == -1) {
                score[i] = 0;
                candidate[i] = -1;
                continue;
            }
            candidate[i] = max[i];
        } else
            candidate[i] = min[i];
        saved_env = input->planet->env[i];
        input->planet->env[i] = (signed char)candidate[i];
        value = input->desirability(input->context, input->planet,
            input->planet->player);
        input->planet->env[i] = saved_env;
        value = (StarsI16)(value - base);
        if (value < 0) value = (StarsI16)-value;
        value = (StarsI16)(value * 100);
        saved_env = (signed char)(input->planet->env[i] - candidate[i]);
        if (saved_env < 0) saved_env = (signed char)-saved_env;
        score[i] = saved_env ? (StarsI32)value / saved_env + 1L : 0L;
    }
    best = 0;
    for (i = 1; i < 3; ++i)
        if (score[best] < score[i]) best = i;
    value = min[best] == -1 ? (StarsI16)(best + 1) :
        (StarsI16)(-(best + 1));
    *input->current_player = saved_player;
    return value;
}
