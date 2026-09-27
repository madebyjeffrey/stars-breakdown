/* Genuine C reconstruction of DoThingInteractions (NE 23:0B3A-1A95).
 * The original is a post-move pass over mine-field thing records.  The game
 * tables are represented explicitly here; callbacks cover the original
 * message, RNG, design, research, fleet-stat, and trader services. */
#include "core.h"
#include <string.h>

static StarsU32 thing_sum3(const StarsU32 *values)
{
    return values[0] + values[1] + values[2];
}

static StarsI16 thing_random(const StarsThingInteractionOps *ops, void *context,
    StarsI16 limit)
{
    StarsI16 value;
    if (!ops || !ops->random_value || limit <= 0) return 0;
    value = ops->random_value(context, limit);
    if (value < 0) return 0;
    return (StarsI16)(value % limit);
}

static StarsI16 thing_fleet_owner(const StarsThingInteractionFleet *fleet)
{
    return fleet ? fleet->player : (StarsI16)-1;
}

static void thing_send2(const StarsThingInteractionOps *ops, void *context,
    StarsI16 player, StarsI16 message, StarsU32 object,
    const StarsThingInteractionFleet *fleet, StarsI16 value)
{
    StarsU16 word = 0;
    if (ops && ops->fleet_word && fleet)
        word = ops->fleet_word(context, fleet);
    if (ops && ops->send_message2)
        ops->send_message2(context, player, message, object, word, value);
}

static void thing_spend(StarsU32 *minerals, StarsU32 amount)
{
    StarsI16 index;
    StarsU32 part;
    for (index = 2; index >= 0 && amount != 0U; --index) {
        part = minerals[index] < amount ? minerals[index] : amount;
        minerals[index] -= part;
        amount -= part;
    }
}

static StarsI16 thing_min_technology(const StarsU8 *technology)
{
    StarsI16 index;
    StarsI16 result = 0;
    for (index = 1; index < 6; ++index)
        if (technology[index] < technology[result]) result = index;
    return result;
}

static StarsI16 thing_has_all_technology(const StarsU8 *technology,
    StarsI16 threshold)
{
    StarsI16 index;
    for (index = 0; index < 6; ++index)
        if ((StarsI16)technology[index] < threshold) return 0;
    return 1;
}

static StarsI16 thing_pick_random_target(const StarsThingInteractionOps *ops,
    void *context, StarsU32 targets, StarsU32 trader_bits)
{
    StarsI16 attempts;
    StarsI16 bit;
    StarsU32 candidate = targets;
    if (candidate == 0U) candidate = (StarsU32)1U << thing_random(ops, context, 13);
    attempts = 25;
    while ((candidate & trader_bits) != 0U && attempts-- > 0)
        candidate = (StarsU32)1U << thing_random(ops, context, 13);
    if ((candidate & trader_bits) == 0U) return (StarsI16)candidate;
    bit = 0x1000;
    return bit;
}

static StarsI16 thing_choose_design(const StarsThingInteractionOps *ops,
    void *context, StarsThingInteractionPlayer *player, StarsI16 owner,
    StarsI16 turn, StarsI16 *quality, StarsThingDesignProfile *profile)
{
    StarsI16 template_index;
    StarsI16 design;
    StarsI16 candidate;
    template_index = thing_random(ops, context, (StarsI16)(turn > 100 ? 3 : 4));
    if (template_index > 0) template_index = (StarsI16)
        (thing_random(ops, context, 2) + 1);
    if (!ops || !ops->get_design_profile || !ops->find_design) return -1;
    memset(profile, 0, sizeof *profile);
    ops->get_design_profile(context, owner, template_index, profile);
    design = ops->find_design(context, owner, profile);
    candidate = design < 0 ? 0 : design;
    if (design < 0)
        while (candidate < 16 &&
                (player->design_flags[candidate] & 0x0200U) == 0U)
            ++candidate;
    if (candidate < 0 || candidate >= 16) return -1;
    *quality = (StarsI16)(thing_random(ops, context, 3) == 0 ? 2 : 1);
    if (turn > 100) *quality = (StarsI16)(*quality +
        thing_random(ops, context, (StarsI16)(turn / 100 + 1)));
    if (template_index > 0)
        *quality = (StarsI16)(*quality +
            thing_random(ops, context, (StarsI16)(*quality + 1)));
    if (*quality > 5) *quality = 5;
    return candidate;
}

static void thing_mine_for_fleet(StarsThingInteractionThing *thing,
    StarsThingInteractionFleet *fleet, StarsThingInteractionPlayer *players,
    const StarsThingInteractionOps *ops, void *context, StarsI16 turn)
{
    StarsI16 owner = thing_fleet_owner(fleet);
    StarsI16 threshold;
    StarsI16 index;
    StarsI16 level_sum = 0;
    StarsI16 design;
    StarsI16 quantity;
    StarsU32 total = thing_sum3(fleet->minerals);
    StarsU32 mask;
    StarsU16 selected;
    StarsI16 message;
    StarsThingDesignProfile profile;
    StarsThingInteractionFleet created;

    if (owner < 0 || owner >= 16) return;
    if (total < 5000U && (fleet->overlap & 0x2000U) == 0U) {
        thing_send2(ops, context, owner, 0x108,
            (StarsU32)fleet->id | 0x8000U, fleet, (StarsI16)fleet->id);
        return;
    }
    threshold = players[owner].dead ? 10 : 0x1a;
    if ((thing->visited_players & ((StarsU32)1U << owner)) != 0U) {
        thing_send2(ops, context, owner, 0x118,
            (StarsU32)fleet->id | 0x8000U, fleet, (StarsI16)fleet->id);
        return;
    }
    thing->visited_players |= (StarsU32)1U << owner;
    if (ops && ops->remove_message)
        ops->remove_message(context, owner, 0x4e,
            (StarsU32)fleet->id | 0x8000U);
    fleet->overlap = (StarsU16)((fleet->overlap & 0xfbffU) | 0x0400U);
    for (index = 0; index < 6; ++index)
        if ((StarsI16)players[owner].technology[index] >= threshold) ++level_sum;
    mask = players[owner].trader_bits;
    if (thing->target_players == 0U ||
            (thing->target_players & mask) != 0U) {
        if (thing_has_all_technology(players[owner].technology, threshold)) {
            if (thing_random(ops, context, 5) != 0) {
                mask = (StarsU32)thing_pick_random_target(ops, context,
                    thing->target_players, mask);
                selected = (StarsU16)mask;
                message = ops && ops->give_trader_part ?
                    ops->give_trader_part(context, selected, owner,
                        &selected) : 0x10e;
                mask = selected;
                thing_send2(ops, context, owner, message, mask, fleet, 0);
            } else {
                thing_send2(ops, context, owner, 0x10e, 0xffffffffUL,
                    fleet, 0);
            }
            return;
        }
        if (ops && ops->develop_cost && ops->update_research) {
            StarsU8 next[6];
            StarsU32 cost;
            index = thing_min_technology(players[owner].technology);
            memcpy(next, players[owner].technology, sizeof next);
            next[index] = (StarsU8)(next[index] + 1U);
            cost = ops->develop_cost(context, owner, next);
            players[owner].technology[index] = next[index];
            if (cost > 0U) cost += cost;
            if (ops->update_research) ops->update_research(context, owner);
            (void)cost;
        }
        return;
    }
    mask = (StarsU32)thing_pick_random_target(ops, context,
        thing->target_players, players[owner].trader_bits);
    if (mask == 0x1000U &&
            (players[owner].detail_flags & 0x0200U) == 0U) {
        design = thing_choose_design(ops, context, &players[owner], owner,
            turn, &quantity, &profile);
        if (design >= 0 && players[owner].fleet_count < 0x200U &&
                ops && ops->new_fleet) {
            memset(&created, 0, sizeof created);
            if (ops->new_fleet(context, owner, fleet->planet, fleet->x,
                    fleet->y, design, (StarsU16)quantity, &created)) {
                created.x = fleet->x;
                created.y = fleet->y;
                created.overlap |= 0x2000U;
                created.ship_count = (StarsU16)quantity;
                players[owner].design_built[design] += (StarsU32)quantity;
                players[owner].design_exist[design] += (StarsU32)quantity;
                created.design_counts[design] = (StarsU16)quantity;
                if (ops->update_design)
                    ops->update_design(context, owner, design);
                if (ops->fleet_stat) {
                    StarsU32 stat = ops->fleet_stat(context, &created, 1);
                    created.minerals[1] = stat;
                }
                thing_send2(ops, context, owner, 0x14f,
                    (StarsU32)created.id | 0x8000U, fleet, quantity);
            }
        } else {
            thing_send2(ops, context, owner, 0x150, 0xffffffffUL,
                fleet, 0);
        }
    } else {
        selected = (StarsU16)mask;
        message = ops && ops->give_trader_part ?
            ops->give_trader_part(context, selected, owner, &selected) : 0x10e;
        mask = selected;
        thing_send2(ops, context, owner, message, mask, fleet, 0);
    }
    (void)level_sum;
}

static void thing_mine_for_planet(StarsThingInteractionThing *thing,
    StarsThingInteractionPlanet *planet, StarsThingInteractionPlayer *players,
    const StarsThingInteractionOps *ops, void *context, StarsI16 turn)
{
    StarsI16 owner = planet->owner;
    StarsI16 threshold;
    StarsI16 index;
    StarsI16 sum = 0;
    StarsU32 amount;
    StarsU32 mask;
    StarsI32 dx, dy, distance;
    if (owner < 0 || owner >= 16) return;
    if ((planet->overlap & 0x0200U) == 0U ||
            (players[owner].detail_flags & 0x0200U) == 0U ||
            players[owner].technology_stage <= 1U ||
            (thing->visited_players & ((StarsU32)1U << owner)) != 0U)
        return;
    dx = (StarsI32)planet->x - thing->x;
    dy = (StarsI32)planet->y - thing->y;
    distance = dx * dx + dy * dy - 6;
    if (distance >= 10001) return;
    amount = thing_sum3(planet->minerals);
    threshold = players[owner].technology_stage == 2U ? 3500 : 5000;
    if (amount < (StarsU32)threshold) return;
    if (thing->target_players == 0U) {
        for (index = 0; index < 6; ++index) sum = (StarsI16)(sum +
            players[owner].technology[index]);
        threshold = players[owner].dead ? 10 : 0x1a;
        if (sum >= (StarsI16)(threshold * 6 - 6)) return;
        for (index = 0; index < 6; ++index) ++players[owner].technology[
            thing_min_technology(players[owner].technology)];
        amount = (StarsU32)(players[owner].technology_stage == 2U ? 3500 : 5000);
    } else {
        mask = thing_pick_random_target(ops, context, thing->target_players,
            players[owner].trader_bits);
        players[owner].trader_bits |= mask;
        amount = (StarsU32)(players[owner].technology_stage == 2U ? 3500 : 5000);
    }
    thing->visited_players |= (StarsU32)1U << owner;
    thing_spend(planet->minerals, amount);
    (void)turn;
}

void stars_do_thing_interactions(StarsI16 post_move,
    StarsThingInteractionThing *things, StarsU16 thing_count,
    StarsThingInteractionFleet *fleets, StarsU16 fleet_count,
    StarsThingInteractionPlanet *planets, StarsU16 planet_count,
    StarsThingInteractionPlayer *players, const StarsThingInteractionOps *ops,
    void *context, StarsI16 turn)
{
    StarsU16 thing_index;
    StarsU16 fleet_index;
    StarsU16 planet_index;
    StarsThingInteractionThing *thing;
    if (!post_move || !things || !fleets || !planets || !players) return;
    for (thing_index = 0; thing_index < thing_count; ++thing_index) {
        thing = &things[thing_index];
        if ((thing->overlap & 0x6000U) != 0x6000U) continue;
        for (fleet_index = 0; fleet_index < fleet_count; ++fleet_index) {
            if (fleets[fleet_index].x != thing->x ||
                    fleets[fleet_index].y != thing->y ||
                    (fleets[fleet_index].overlap & 0x0400U) != 0U)
                continue;
            thing_mine_for_fleet(thing, &fleets[fleet_index], players, ops,
                context, turn);
        }
        for (planet_index = 0; planet_index < planet_count; ++planet_index)
            thing_mine_for_planet(thing, &planets[planet_index], players, ops,
                context, turn);
    }
}
