#include "core.h"
#include <string.h>

#define KILL_SHIPS_FLEET_EMPTY 0x0400U
#define KILL_SHIPS_TOKEN_ALIVE 0x0001U

/* NE 1F:7CDE-7EE7. Remove a hull group from a battle fleet, leave the token
 * describing the survivors, and route the killed ships through cargo balance
 * and optional salvage/technology bookkeeping.  The temporary fleet records
 * mirror the original stack copies without exposing their raw offsets. */
void stars_kill_ships(StarsKillShipsInput *input)
{
    StarsKillShipsFleet original;
    StarsKillShipsFleet killed;
    StarsI16 survivors;
    StarsI16 index;

    if (!input || !input->token || !input->fleet || input->killed_count == 0)
        return;
    if (input->fallout && input->ops.mark_techs)
        input->ops.mark_techs(input->context, input->hull_definition,
            input->token->player);

    original = *input->fleet;
    memset(&killed, 0, sizeof killed);
    killed.player = original.player;
    killed.x = original.x;
    killed.y = original.y;
    killed.planet_id = original.planet_id;
    if (input->hull_index < 0 || input->hull_index >= 16)
        return;
    survivors = (StarsI16)((StarsI16)original.ship_counts[input->hull_index] -
        input->killed_count);
    killed.ship_counts[input->hull_index] =
        (StarsU16)input->killed_count;
    input->token->ship_count = survivors;
    if (survivors == 0) {
        input->token->flags = (StarsU16)(input->token->flags &
            (StarsU16)~KILL_SHIPS_TOKEN_ALIVE);
        for (index = 0; index < 16; ++index)
            if (index != input->hull_index &&
                    original.ship_counts[index] != 0U)
                break;
        if (index == 16)
            input->fleet->flags = (StarsU16)((input->fleet->flags &
                (StarsU16)~KILL_SHIPS_FLEET_EMPTY) | KILL_SHIPS_FLEET_EMPTY);
    }

    if ((input->fleet->flags & KILL_SHIPS_FLEET_EMPTY) == 0U) {
        if (input->ops.balance_cargo)
            input->ops.balance_cargo(input->context, &original, &killed);
        *input->fleet = original;
    }
    if (input->fallout && input->ops.create_salvage)
        input->ops.create_salvage(input->context, &killed);
}
