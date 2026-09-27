#include "core.h"

/* NE 10:6B88-6E1E. Evaluate habitability at the nearest terraformable
 * environment to the player's target, without changing the stored planet
 * state. The min/max bounds are the resolved FCanTerraform result. */
StarsI16 stars_pct_planet_opt_value(const StarsPlanetOptValueInput *input,
    StarsI16 fallback_value, StarsPlanetOptDesirability desirability,
    void *context)
{
    StarsI16 trial[3];
    StarsI16 i;
    StarsI16 value;
    if (!input) return 0;
    if (!input->can_terraform)
        return fallback_value;
    for (i = 0; i < 3; ++i) {
        trial[i] = input->environment[i];
        if (input->target[i] == -1 || trial[i] == input->target[i]) continue;
        if (trial[i] < input->target[i]) {
            if (trial[i] < input->terraform_max[i]) {
                value = input->target[i] < input->terraform_max[i] ?
                    input->target[i] : input->terraform_max[i];
                trial[i] = value;
            }
        } else if (input->terraform_min[i] != -1 &&
                input->terraform_min[i] < trial[i]) {
            value = input->terraform_min[i] < input->target[i] ?
                input->target[i] : input->terraform_min[i];
            trial[i] = value;
        }
    }
    if (!desirability) return fallback_value;
    return desirability(context, trial, input->player);
}
