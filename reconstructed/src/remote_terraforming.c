#include "core.h"

#define REMOTE_TERRAFORM_MESSAGE 0x15a
#define REMOTE_TERRAFORM_HELP_MESSAGE 300

/* NE 24:4C56-4FAA. Spend each eligible fleet's remote-terraforming points
 * against its destination planet. Each selected axis moves one environment
 * point toward its requested value, clamped to the original 1..99 range;
 * both players receive the resulting desirability report when appropriate. */
void stars_remote_terraform(StarsRemoteTurnInput *input)
{
    StarsU16 fleet_index;
    StarsRemoteTurnFleet *fleet;
    StarsRemoteTurnPlanet *planet;
    StarsI32 remaining;
    StarsI16 help;
    StarsI16 before;
    StarsI16 after;
    StarsI16 choice;
    StarsI16 axis;
    StarsI16 direction;
    StarsI16 concentration;
    StarsI16 message;

    if (!input || !input->fleets || !input->planets ||
            !input->ops.terraforming_amount || !input->ops.best_terraform ||
            !input->ops.desirability)
        return;
    for (fleet_index = 0; fleet_index < input->fleet_count; ++fleet_index) {
        fleet = &input->fleets[fleet_index];
        if ((fleet->status_flags & 0x0400) != 0 || fleet->planet_id == -1 ||
                fleet->planet_id < 0 ||
                (StarsU16)fleet->planet_id >= input->planet_count)
            continue;
        planet = &input->planets[fleet->planet_id];
        if (planet->player == -1)
            continue;
        remaining = input->ops.terraforming_amount(input->context, fleet);
        if (remaining <= 0L)
            continue;
        help = fleet->player == planet->player;
        if (!help && input->ops.friendly)
            help = input->ops.friendly(input->context, fleet->player,
                planet->player);
        if (!help && (planet->status_flags & 0x0200) != 0)
            continue;
        before = input->ops.desirability(input->context, planet,
            planet->player);
        while (remaining > 0L) {
            choice = input->ops.best_terraform(input->context, planet,
                fleet->player, help);
            if (choice == 0)
                break;
            axis = choice < 0 ? (StarsI16)-choice : choice;
            if (axis < 1 || axis > 3)
                break;
            direction = choice < 0 ? -1 : 1;
            concentration = (StarsI16)(planet->mineral_concentration[axis - 1]
                + direction);
            if (concentration > 99)
                concentration = 99;
            if (concentration < 1)
                concentration = 1;
            planet->mineral_concentration[axis - 1] =
                (signed char)concentration;
            --remaining;
        }
        after = input->ops.desirability(input->context, planet,
            planet->player);
        message = help ? REMOTE_TERRAFORM_HELP_MESSAGE :
            REMOTE_TERRAFORM_MESSAGE;
        if (before == after)
            ++message;
        if (input->ops.send_message)
            input->ops.send_message(input->context, fleet->player, message,
                (StarsU16)(fleet->object_token | 0x8000),
                fleet->object_subtoken, planet->id, before, after);
        if (fleet->player != planet->player && before != after &&
                input->ops.send_message)
            input->ops.send_message(input->context, planet->player,
                help ? REMOTE_TERRAFORM_HELP_MESSAGE : REMOTE_TERRAFORM_MESSAGE,
                planet->id, fleet->object_subtoken, planet->id, before, after);
    }
}
