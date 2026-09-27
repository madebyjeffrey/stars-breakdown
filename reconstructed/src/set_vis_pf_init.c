#include "core.h"

static StarsU16 vis_set_detail(StarsU16 flags, StarsU16 level)
{
    return (StarsU16)((flags & 0xfe00U) | 0x0100U | level);
}

static StarsU16 vis_set_player_detail(StarsU16 flags, StarsU16 level)
{
    /* Player bits 3..7 hold the bitmap identity, unlike object detail bytes. */
    return (StarsU16)((flags & 0xfff8U) | 0x0100U | level);
}

static StarsU16 vis_clear_known(StarsU16 flags)
{ return (StarsU16)(flags & 0xfeffU); }

static void vis_add_count(StarsU32 *count, StarsU16 amount)
{ *count += (StarsU32)amount; }

/* NE 15:9654-a0ff. Initialize the visibility masks and counters before the
 * fleet, planet, and thing passes refine them. The original keeps knowledge
 * in bit 8, detail level in bits 0-2, and uses bit 9 as the already-known
 * design guard; those meanings are named here rather than hidden in offsets. */
void stars_set_vis_pf_init(StarsVisPFInitInput *input,
    const StarsVisPFInitOps *ops, void *context)
{
    StarsU32 viewer_mask;
    StarsU16 player;
    StarsU16 slot;
    StarsU16 i;
    StarsU16 owner;
    StarsU16 detail;
    StarsU16 steal;
    StarsI16 scanner;
    StarsI32 mine;
    StarsVisPFPlayer *current;
    StarsVisPFFleet *fleet;
    StarsVisPFPlanet *planet;
    StarsVisPFThing *thing;

    if (!input || !input->players) return;
    viewer_mask = input->viewer == (StarsI16)-1 ? 0UL :
        (StarsU32)1UL << ((StarsU16)input->viewer & 31U);
    for (player = 0; player < input->player_count; ++player) {
        current = &input->players[player];
        current->fleet_count = 0U;
        current->planet_count = 0U;
        current->design_count = 0U;
        if (input->viewer == (StarsI16)-1 ||
                input->viewer == (StarsI16)player || current->dead != 0U)
            current->detail_flags = vis_set_player_detail(current->detail_flags, 7U);
        else
            current->detail_flags = vis_clear_known(current->detail_flags);
        for (slot = 0; slot < 16U; ++slot) {
            if ((input->viewer == (StarsI16)-1 ||
                    input->viewer == (StarsI16)player) &&
                    (current->design_flags[slot] & 0x0200U) == 0U) {
                current->design_flags[slot] = vis_set_detail(
                    current->design_flags[slot], 7U);
                current->design_built[slot] = 0UL;
                ++current->design_count;
            } else {
                current->design_flags[slot] = vis_clear_known(
                    current->design_flags[slot]);
            }
        }
        for (slot = 0; slot < 10U; ++slot) {
            if ((input->viewer == (StarsI16)-1 ||
                    input->viewer == (StarsI16)player) &&
                    (current->starbase_flags[slot] & 0x0200U) == 0U) {
                current->starbase_flags[slot] = vis_set_detail(
                    current->starbase_flags[slot], 7U);
                current->starbase_built[slot] = 0UL;
                current->fleet_count = (StarsU16)((current->fleet_count & 0x0fffU) |
                    ((current->fleet_count + 0x1000U) & 0xf000U));
            } else {
                current->starbase_flags[slot] = vis_clear_known(
                    current->starbase_flags[slot]);
            }
        }
    }

    if (input->planets) {
        for (i = 0; i < input->planet_count; ++i) {
            planet = &input->planets[i];
            if (input->viewer == (StarsI16)-1 ||
                    input->viewer == planet->player) {
                planet->flags = vis_set_detail(planet->flags, 7U);
                if (input->viewer != (StarsI16)-1 &&
                        input->viewer >= 0 &&
                        input->viewer < (StarsI16)input->player_count)
                    ++input->players[(StarsU16)input->viewer].planet_count;
                if ((planet->flags & 0x0200U) != 0U && planet->player >= 0 &&
                        planet->player < (StarsI16)input->player_count &&
                        planet->starbase_slot < 10U)
                    vis_add_count(&input->players[(StarsU16)planet->player]
                        .starbase_built[planet->starbase_slot], 1U);
            } else {
                planet->flags = vis_clear_known(planet->flags);
            }
        }
    }

    for (i = 0; i < input->fleet_count; ++i) {
        fleet = &input->fleets[i];
        fleet->flags = (StarsU16)(fleet->flags & 0x7fffU);
        if (fleet->player < 0 ||
                fleet->player >= (StarsI16)input->player_count ||
                input->viewer == (StarsI16)-1 ||
                input->viewer == fleet->player) {
            if ((fleet->flags & 0x0400U) != 0U) {
                fleet->flags = vis_clear_known(fleet->flags);
                continue;
            }
            fleet->flags = vis_set_detail(fleet->flags, 7U);
            current = fleet->player >= 0 ?
                &input->players[(StarsU16)fleet->player] : 0;
            if (current) {
                current->fleet_count = (StarsU16)((current->fleet_count &
                    0xf000U) | ((current->fleet_count + 1U) & 0x0fffU));
                for (slot = 0; slot < 16U; ++slot)
                    if (fleet->ship_counts[slot] != 0U)
                        vis_add_count(&current->design_built[slot],
                            fleet->ship_counts[slot]);
            }
            if (input->viewer != (StarsI16)-1 && fleet->planet_id >= 0 &&
                    input->planets &&
                    fleet->planet_id < (StarsI16)input->planet_count) {
                steal = 0U;
                scanner = ops && ops->scanner ? ops->scanner(context, fleet,
                    &steal) : 0;
                detail = scanner < 0 ? 1U : 3U;
                if (steal > 1U) detail = 4U;
                planet = &input->planets[(StarsU16)fleet->planet_id];
                if (fleet->mine_special != 0U && planet->player == -1) {
                    mine = ops && ops->mine_from_fleet ?
                        ops->mine_from_fleet(context, fleet) : 0L;
                    if (mine > 0L) detail = 4U;
                }
                if (ops && ops->mark_planet)
                    ops->mark_planet(context, planet->id, input->viewer,
                        (StarsI16)detail);
            }
        } else {
            fleet->flags = vis_clear_known(fleet->flags);
        }
    }

    if (!input->things) return;
    for (i = 0; i < input->thing_count; ++i) {
        thing = &input->things[i];
        owner = (StarsU16)((thing->owner >= 0) ? thing->owner : 0);
        if (thing->kind == 1U) {
            if (input->viewer == (StarsI16)-1 || input->race_stat == 6) {
                thing->dest_x = (StarsU16)(thing->dest_x | 0x8000U);
                if (owner < input->player_count &&
                        (input->players[owner].detail_flags & 0x0100U) == 0U)
                    input->players[owner].detail_flags = vis_set_player_detail(
                        input->players[owner].detail_flags, 3U);
            } else {
                thing->dest_x = (StarsU16)(thing->dest_x & 0x7fffU);
            }
        } else if (thing->kind == 2U) {
            thing->dest_x = (StarsU16)((thing->dest_x & 0xdfffU) |
                (input->viewer == (StarsI16)-1 ? 0x2000U : 0U));
        } else if (thing->kind == 3U) {
            thing->flags = (StarsU16)(thing->flags | 0x0010U);
        } else if (thing->kind == 0U &&
                (thing->target_mask & viewer_mask) != 0UL &&
                owner < input->player_count &&
                (input->players[owner].detail_flags & 0x0100U) == 0U) {
            input->players[owner].detail_flags = vis_set_player_detail(
                input->players[owner].detail_flags, 3U);
        }
    }
}
