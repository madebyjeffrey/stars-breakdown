#include "core.h"

static StarsI16 abs_i16(StarsI16 value)
{
    return value < 0 ? (StarsI16)-value : value;
}

static StarsI16 find_terraform_part(const StarsCanTerraformInput *input,
    StarsU16 first, StarsU16 last, StarsI16 *dp, StarsI16 *cost)
{
    StarsI16 subtype;

    for (subtype = (StarsI16)last; subtype >= (StarsI16)first; --subtype)
        if (input->lookup_part(input->context, 0x2000U,
                (StarsU16)subtype, dp, cost))
            return 1;
    return 0;
}

/* NE 0A:8022-8732. Resolve the best available terraforming parts, form the
 * legal range on each environment axis, and narrow it to the useful side of
 * the current value when requested by the caller. */
StarsI16 stars_can_terraform_lppl(const StarsCanTerraformInput *input,
    StarsI16 *environment_min, StarsI16 *environment_max,
    StarsI16 *environment_cost, StarsI16 help)
{
    StarsI16 saved_player;
    StarsI16 axis;
    StarsI16 dp;
    StarsI16 cost;
    StarsI16 first_dp;
    StarsI16 first_cost;
    StarsI16 adjustment[3];
    StarsI16 found;

    if (!input || !input->planet || !input->current_player ||
            !environment_min || !environment_max || !environment_cost ||
            !input->lookup_part)
        return 0;
    saved_player = *input->current_player;
    if (*input->current_player == -1)
        *input->current_player = input->planet->player;
    found = 0;
    first_dp = 0;
    first_cost = 0;
    if (find_terraform_part(input, 0U, 7U, &first_dp, &first_cost)) {
        found = 1;
        for (axis = 0; axis < 3; ++axis) {
            adjustment[axis] = first_dp;
            environment_cost[axis] = first_cost;
        }
    } else {
        for (axis = 0; axis < 3; ++axis) adjustment[axis] = 0;
    }
    if (find_terraform_part(input, 8U, 11U, &dp, &cost) &&
            adjustment[0] < dp) {
        found = 1;
        adjustment[0] = dp;
        environment_cost[0] = cost;
    }
    if (find_terraform_part(input, 12U, 15U, &dp, &cost) &&
            adjustment[1] < dp) {
        found = 1;
        adjustment[1] = dp;
        environment_cost[1] = cost;
    }
    if (find_terraform_part(input, 16U, 19U, &dp, &cost) &&
            adjustment[2] < dp) {
        found = 1;
        adjustment[2] = dp;
        environment_cost[2] = cost;
    }
    if (!found) {
        *input->current_player = saved_player;
        return 0;
    }
    for (axis = 0; axis < 3; ++axis) {
        StarsI16 minimum;
        StarsI16 maximum;
        StarsI16 center;
        StarsI16 current;
        StarsI16 current_distance;
        StarsI16 min_distance;
        StarsI16 max_distance;

        if (adjustment[axis] == 0 ||
                input->race_environment_enabled[axis] == -1) {
            environment_min[axis] = -1;
            environment_max[axis] = -1;
            continue;
        }
        minimum = (StarsI16)input->planet->original_environment[axis] -
            adjustment[axis];
        maximum = (StarsI16)input->planet->original_environment[axis] +
            adjustment[axis];
        current = input->planet->environment[axis];
        if (minimum < current)
            environment_min[axis] = minimum < 1 ? 1 : minimum;
        else
            environment_min[axis] = -1;
        if (current < maximum)
            environment_max[axis] = maximum < 100 ? maximum : 99;
        else
            environment_max[axis] = -1;
        center = input->race_center[axis];
        if (!help) {
            current_distance = abs_i16((StarsI16)(current - center));
            min_distance = environment_min[axis] == -1 ? 0 :
                abs_i16((StarsI16)(environment_min[axis] - center));
            max_distance = environment_max[axis] == -1 ? 0 :
                abs_i16((StarsI16)(environment_max[axis] - center));
            if (current_distance < min_distance ||
                    current_distance < max_distance) {
                if (min_distance < max_distance)
                    environment_min[axis] = -1;
                else
                    environment_max[axis] = -1;
            } else {
                environment_min[axis] = -1;
                environment_max[axis] = -1;
            }
        } else if (current == center) {
            environment_min[axis] = -1;
            environment_max[axis] = -1;
        } else if (center < current) {
            environment_max[axis] = -1;
            if (environment_min[axis] != -1 &&
                    center > environment_min[axis])
                environment_min[axis] = center;
        } else {
            environment_min[axis] = -1;
            if (environment_max[axis] != -1 &&
                    environment_max[axis] > center)
                environment_max[axis] = center;
        }
    }
    *input->current_player = saved_player;
    for (axis = 0; axis < 3; ++axis)
        if (environment_min[axis] != -1 || environment_max[axis] != -1)
            return 1;
    return 0;
}
