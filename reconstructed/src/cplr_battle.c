#include "core.h"

#define CPLR_FLEET_SKIP 0x0400U
#define CPLR_FLEET_BATTLE 0x0100U
#define CPLR_FLEET_SEEN 0x0800U
#define CPLR_FLEET_LIMITED 0x1000U
#define CPLR_FLEET_LIMIT_MARK 0x0080U
#define CPLR_TARGET_ALLIES 1U
#define CPLR_TARGET_ALL_PLAYERS 2U
#define CPLR_TARGET_EXCEPT_SELF 3U
#define CPLR_TARGET_FIRST_PLAYER 4U
#define CPLR_MAX_BATTLE_SHIPS 255
#define CPLR_MAX_PLAYERS 16

static StarsU16 cplr_player_bit(StarsI16 player)
{
    if (player < 0 || player >= CPLR_MAX_PLAYERS) return 0;
    return (StarsU16)1U << (StarsU16)player;
}

static void cplr_add_target(StarsCplrBattleInput *input,
    StarsI16 owner, StarsU8 target, StarsU16 *mask)
{
    StarsI16 player;
    StarsI16 relationship;
    if (target == 0U) return;
    if (target == CPLR_TARGET_EXCEPT_SELF) {
        *mask = (StarsU16)(input->player_count >= 16 ? 0xffffU :
            ((StarsU16)1U << (StarsU16)input->player_count) - 1U);
        *mask = (StarsU16)(*mask & (StarsU16)~cplr_player_bit(owner));
        return;
    }
    if (target >= CPLR_TARGET_FIRST_PLAYER) {
        *mask |= cplr_player_bit((StarsI16)(target - CPLR_TARGET_FIRST_PLAYER));
        return;
    }
    for (player = 0; player < input->player_count; ++player) {
        if (player == owner) continue;
        relationship = input->relation ? input->relation(input->context,
            owner, player) : 0;
        if (relationship == 2 ||
                (target == CPLR_TARGET_ALL_PLAYERS && relationship == 0))
            *mask |= cplr_player_bit(player);
    }
}

static StarsI16 cplr_popcount(StarsU16 value)
{
    StarsI16 count = 0;
    while (value) {
        if (value & 1U) ++count;
        value >>= 1;
    }
    return count;
}

/* Genuine semantic reconstruction of CplrBattle, NE 31:2952-339B.
 * Before battle resolution the original identifies every participating
 * player, expands battle-plan target rules into player bit masks, computes
 * the transitive attack closure, folds qualifying allied players into that
 * closure, and marks nonparticipants as spectators.  It also marks fleets
 * for combat and caps the participating ship records at 255, preserving the
 * original reserve-fleet behavior when the cap is exceeded. */
StarsI16 stars_cplr_battle(StarsCplrBattleInput *input)
{
    StarsI16 fleet_index;
    StarsI16 player;
    StarsI16 other;
    StarsI16 design;
    StarsI16 owner;
    StarsI16 total_ships = 0;
    StarsU16 active = 0;
    StarsU16 conflict = 0;
    StarsU16 mask;
    StarsU16 old_conflict;
    StarsU16 *attacks;
    StarsCplrBattleFleet *fleet;

    if (!input || !input->fleets || !input->attack_masks ||
            !input->player_mask || !input->spectator_mask) return 0;
    attacks = input->attack_masks;
    for (player = 0; player < input->player_count && player < CPLR_MAX_PLAYERS; ++player)
        attacks[player] = 0;
    *input->player_mask = 0;
    *input->spectator_mask = 0;
    if (input->planet_is_starbase && input->planet_owner >= 0 &&
            input->planet_has_teeth) {
        owner = input->planet_owner;
        active |= cplr_player_bit(owner);
        cplr_add_target(input, owner, input->planet_target, &attacks[owner]);
    } else if (input->planet_owner >= 0) {
        *input->spectator_mask |= cplr_player_bit(input->planet_owner);
    }
    for (fleet_index = 0; fleet_index < input->fleet_count; ++fleet_index) {
        fleet = &input->fleets[fleet_index];
        if (fleet->flags & CPLR_FLEET_SKIP) continue;
        owner = fleet->player;
        active |= cplr_player_bit(owner);
        if (fleet->has_teeth && fleet->plan_attacker && fleet->plan_target)
            cplr_add_target(input, owner, fleet->plan_target, &attacks[owner]);
        fleet->flags |= (CPLR_FLEET_SEEN | CPLR_FLEET_BATTLE);
    }
    for (player = 0; player < input->player_count && player < CPLR_MAX_PLAYERS; ++player)
        conflict |= (StarsU16)(active & attacks[player]);
    /* Original returns here when no active player is targeted. The initial
     * fleet seen/battle marks remain set; spectator cleanup is reached only
     * on the combat branch. */
    if (conflict == 0U) return 0;
    old_conflict = 0xffffU;
    while (conflict != old_conflict) {
        old_conflict = conflict;
        for (player = 0; player < input->player_count && player < CPLR_MAX_PLAYERS; ++player) {
            if (!(conflict & cplr_player_bit(player))) continue;
            for (other = 0; other < input->player_count && other < CPLR_MAX_PLAYERS; ++other)
                if (attacks[other] & cplr_player_bit(player))
                    conflict |= cplr_player_bit(other);
        }
    }
    for (player = 0; player < input->player_count && player < CPLR_MAX_PLAYERS; ++player) {
        if (!(active & cplr_player_bit(player)) ||
                (conflict & cplr_player_bit(player))) continue;
        mask = attacks[player];
        for (other = 0; other < input->player_count && other < CPLR_MAX_PLAYERS; ++other) {
            if (other == player || !(conflict & cplr_player_bit(other))) continue;
            if (input->relation && input->relation(input->context, player, other) == 1)
                mask |= attacks[other];
        }
        if (mask & conflict) {
            attacks[player] = mask;
            conflict |= cplr_player_bit(player);
        } else {
            *input->spectator_mask |= cplr_player_bit(player);
        }
    }
    *input->spectator_mask |= (StarsU16)(active & (StarsU16)~conflict);
    for (fleet_index = 0; fleet_index < input->fleet_count; ++fleet_index) {
        fleet = &input->fleets[fleet_index];
        if (!(conflict & cplr_player_bit(fleet->player))) {
            fleet->flags &= (StarsU16)~CPLR_FLEET_BATTLE;
            continue;
        }
        for (design = 0; design < 16; ++design)
            if (fleet->ships[design] && (!input->counted_ship ||
                    input->counted_ship(input->context, fleet->player, design)))
                total_ships += fleet->ships[design];
    }
    if (total_ships > CPLR_MAX_BATTLE_SHIPS) {
        total_ships = 0;
        for (fleet_index = 0; fleet_index < input->fleet_count; ++fleet_index) {
            fleet = &input->fleets[fleet_index];
            if (!(conflict & cplr_player_bit(fleet->player))) continue;
            for (design = 0; design < 16; ++design) {
                if (!fleet->ships[design]) continue;
                if (total_ships + fleet->ships[design] > CPLR_MAX_BATTLE_SHIPS) {
                    fleet->flags = (StarsU16)((fleet->flags &
                        (StarsU16)~CPLR_FLEET_BATTLE) | CPLR_FLEET_LIMITED);
                    fleet->flags |= CPLR_FLEET_LIMIT_MARK;
                } else total_ships += fleet->ships[design];
            }
        }
    }
    *input->player_mask = conflict;
    return cplr_popcount(conflict);
}
