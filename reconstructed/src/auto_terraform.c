#include "core.h"

#define AUTO_TERRAFORM_RACE_STAT 0x0e
#define AUTO_TERRAFORM_MESSAGE_CLIMATE 0x15c
#define AUTO_TERRAFORM_MESSAGE_DESIRABILITY 0x156

/* NE 24:48F6-4C55. For each player with the relevant environmental trait,
 * occasionally move a planet's original climate one step toward that
 * player's preferred value. Then apply the normal terraforming result and
 * publish the resulting desirability. The callbacks represent the original
 * player, planet, random-number, and message globals. */
void stars_auto_terraform(StarsAutoTerraformInput *input)
{
    StarsU8 eligible[16];
    StarsI16 player;
    StarsI16 axis;
    StarsI16 target;
    StarsI16 random_value;
    StarsI16 min[3];
    StarsI16 max[3];
    StarsI16 cost[3];
    StarsI16 desirability;
    StarsI16 race_stat;
    StarsU16 index;
    StarsAutoTerraformPlanet *planet;

    if (!input || !input->planets || !input->ops.race_stat)
        return;
    for (player = 0; player < input->player_count && player < 16; ++player) {
        race_stat = input->ops.race_stat(input->context, player,
            AUTO_TERRAFORM_RACE_STAT);
        eligible[player] = (StarsU8)(race_stat == 3);
    }
    if (player == 0)
        return;
    for (player = 0; player < input->player_count && player < 16; ++player)
        if (eligible[player])
            break;
    if (player == input->player_count || player == 16)
        return;

    for (index = 0; index < input->planet_count; ++index) {
        planet = &input->planets[index];
        /* The raw routine also tests an unowned flag-cleanup case here, but
         * this loop is already restricted to owned planets, so that branch
         * cannot execute. */
        if (planet->player == -1 || planet->player < 0 ||
                planet->player >= input->player_count ||
                planet->player >= 16 || !eligible[planet->player])
            continue;

        axis = input->ops.random ? input->ops.random(input->context, 3) : 0;
        if (axis >= 0 && axis < 3 && input->ops.race_target &&
                input->ops.random) {
            target = input->ops.race_target(input->context, planet->player,
                axis);
            if (target != -1 && target != planet->original_environment[axis] &&
                    input->ops.random(input->context, 10) == 0) {
                if (planet->reserve > 0L ||
                        (planet->reserve >= -1L && planet->population > 999L))
                    random_value = 0;
                else
                    random_value = input->ops.random(input->context, 1000);
                if (planet->reserve > 0L ||
                        (planet->reserve >= -1L && planet->population > 999L) ||
                        random_value < planet->population) {
                    if (target < planet->original_environment[axis])
                        --planet->original_environment[axis];
                    else
                        ++planet->original_environment[axis];
                    if (input->ops.send_message)
                        input->ops.send_message(input->context, planet->player,
                            AUTO_TERRAFORM_MESSAGE_CLIMATE, planet->id, axis);
                }
            }
        }

        if (input->ops.can_terraform && input->ops.can_terraform(
                input->context, planet, min, max, cost, 1)) {
            for (axis = 0; axis < 3; ++axis) {
                if (min[axis] == -1) {
                    if (max[axis] != -1)
                        planet->environment[axis] = (signed char)max[axis];
                } else
                    planet->environment[axis] = (signed char)min[axis];
            }
            desirability = input->ops.desirability ?
                input->ops.desirability(input->context, planet,
                    planet->player) : 0;
            if (input->ops.send_message)
                input->ops.send_message(input->context, planet->player,
                    AUTO_TERRAFORM_MESSAGE_DESIRABILITY, planet->id,
                    desirability);
        }
    }
}
