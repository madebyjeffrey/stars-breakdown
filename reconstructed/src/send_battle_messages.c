#include "core.h"

#define SBM_FLEET_SKIP 0x0400U
#define SBM_FLEET_MISSED 0x0080U
#define SBM_STARBASE_PRESENT 0x0200U
#define SBM_LOSS_SHIP 0x3fffU
#define SBM_LOSS_SHIP_DESIGN 0x4000U
#define SBM_LOSS_DAMAGED 0x8000U
#define SBM_BATTLE_OBJECT 0x4000
#define SBM_FLEET_OBJECT 0x8000
#define SBM_MAX_PLAYERS 16
#define SBM_MAX_DESIGNS 16

/* Report event identifiers retained as names.  They are protocol values, not
 * arbitrary magic numbers: the original UI/message decoder assigns the
 * corresponding text and report semantics to each one. */
#define SBM_EVENT_PLANET_OBSERVER 0x00f9
#define SBM_EVENT_FLEET_OBSERVER 0x00fa
#define SBM_EVENT_MISSED_FLEET 0x0180
#define SBM_EVENT_STARBASE_LOST 0x008d
#define SBM_EVENT_STARBASE_LOW_YIELD 0x008e
#define SBM_EVENT_STARBASE_OTHER 0x0144
#define SBM_EVENT_SHIP_BASE 0x0091
#define SBM_EVENT_SHIP_BASE_DAMAGED 0x0092
#define SBM_EVENT_SHIP_OTHER_DAMAGED 0x0093
#define SBM_EVENT_SHIP_BOTH_DAMAGED 0x0094
#define SBM_EVENT_SHIP_NONE 0x0095
#define SBM_EVENT_DESIGN_BASE 0x0096
#define SBM_EVENT_DESIGN_BASE_DAMAGED 0x0097
#define SBM_EVENT_DESIGN_OTHER_DAMAGED 0x0098
#define SBM_EVENT_DESIGN_BOTH_DAMAGED 0x0099
#define SBM_EVENT_DESIGN_NONE 0x009a
#define SBM_EVENT_ONE_SIDED 0x009b
#define SBM_EVENT_ONE_SIDED_REVERSE 0x009c
#define SBM_EVENT_MULTI_SIDED 0x009d
#define SBM_EVENT_MULTI_SIDED_REVERSE 0x009e
#define SBM_EVENT_MIXED 0x009f
#define SBM_EVENT_SINGLE_WIN 0x00a0
#define SBM_EVENT_SINGLE_LOSS 0x00a1
#define SBM_EVENT_SINGLE_WIN_WITH_LOSS 0x00a2
#define SBM_EVENT_SINGLE_LOSS_WITH_WIN 0x00a3
#define SBM_EVENT_EQUAL_NO_LOSS 0x00a4
#define SBM_EVENT_EQUAL_LOSS 0x00a5
#define SBM_EVENT_NO_ENEMY 0x00a6
#define SBM_EVENT_EQUAL_SIDE 0x00a7
#define SBM_EVENT_UNKNOWN 0x007e
#define SBM_EVENT_ONE_SHIP 0x0113
#define SBM_EVENT_ONE_SHIP_REVERSE 0x0114
#define SBM_EVENT_ONE_SIDE_ONE_SHIP 0x0115
#define SBM_EVENT_REVERSE_ONE_SHIP 0x0116

static StarsU16 sbm_bit(StarsI16 player)
{
    if (player < 0 || player >= SBM_MAX_PLAYERS) return 0;
    return (StarsU16)(1U << (StarsU16)player);
}

static StarsI16 sbm_same_planet_owner(const StarsBattleMessagePlanet *planet,
    StarsI16 player)
{
    return planet && planet->owner == player;
}

static StarsBattleMessageFleet *sbm_find_fleet(
    StarsSendBattleMessagesInput *input, StarsI16 player, StarsI16 require_missed)
{
    StarsU16 i;
    StarsBattleMessageFleet *fleet;
    for (i = 0; i < input->fleet_count; ++i) {
        fleet = &input->fleets[i];
        if (fleet->player == player &&
                (!require_missed || (fleet->missed_flags & SBM_FLEET_MISSED)))
            return fleet;
    }
    return (StarsBattleMessageFleet *)0;
}

static void sbm_send(StarsSendBattleMessagesInput *input, StarsI16 player,
    StarsI16 event, StarsI16 object, StarsI16 x, StarsI16 y, StarsI16 a,
    StarsI16 b, StarsI16 c, StarsI16 d, StarsI16 e)
{
    if (input->send_message)
        input->send_message(input->context, player, event, object, x, y,
            a, b, c, d, e);
}

static void sbm_learn(StarsSendBattleMessagesInput *input, StarsI16 player,
    StarsI16 x, StarsI16 y, StarsI16 event)
{
    if (input->learn_tech)
        input->learn_tech(input->context, player, x, y, event);
}

static StarsU16 *sbm_row(StarsU16 *losses, StarsI16 player)
{
    return losses + (player * SBM_MAX_DESIGNS);
}

static StarsU16 sbm_row_value(const StarsU16 *row)
{
    StarsI16 design;
    StarsU16 total = 0;
    for (design = 0; design < SBM_MAX_DESIGNS; ++design)
        total = (StarsU16)(total + (row[design] & SBM_LOSS_SHIP));
    return total;
}

static StarsI16 sbm_row_present(const StarsU16 *row)
{
    StarsI16 design;
    for (design = 0; design < SBM_MAX_DESIGNS; ++design)
        if (row[design]) return 1;
    return 0;
}

static StarsI16 sbm_row_flag(const StarsU16 *row, StarsU16 flag)
{
    StarsI16 design;
    for (design = 0; design < SBM_MAX_DESIGNS; ++design)
        if (row[design] & flag) return 1;
    return 0;
}

static StarsI16 sbm_row_design(const StarsU16 *losses, StarsI16 player)
{
    StarsI16 design;
    const StarsU16 *row = sbm_row((StarsU16 *)losses, player);
    for (design = 0; design < SBM_MAX_DESIGNS; ++design)
        if (row[design]) return design;
    return -1;
}

static StarsI16 sbm_pack_design(StarsI16 player, StarsI16 design)
{
    if (player < 0 || design < 0) return 0;
    return (StarsI16)(player * 32 + design);
}

static StarsI16 sbm_count_ships(const StarsSendBattleMessagesInput *input,
    StarsI16 player, StarsI16 design)
{
    StarsU16 i;
    StarsI16 total = 0;
    for (i = 0; i < input->fleet_count; ++i)
        if (!(input->fleets[i].flags & SBM_FLEET_SKIP) &&
                input->fleets[i].player == player) {
            if (design < 0) {
                for (design = 0; design < SBM_MAX_DESIGNS; ++design)
                    total = (StarsI16)(total + input->fleets[i].ship_counts[design]);
                design = -1;
            } else total = (StarsI16)(total + input->fleets[i].ship_counts[design]);
        }
    return total;
}

static void sbm_mark_ship_losses(StarsSendBattleMessagesInput *input)
{
    StarsU16 i;
    StarsI16 design;
    StarsBattleMessageFleet *fleet;
    for (i = 0; i < input->fleet_count; ++i) {
        fleet = &input->fleets[i];
        if (fleet->flags & SBM_FLEET_SKIP) continue;
        for (design = 0; design < SBM_MAX_DESIGNS; ++design)
            if (fleet->ship_counts[design])
                sbm_row(input->player_losses, fleet->player)[design] |=
                    SBM_LOSS_SHIP_DESIGN;
    }
}

static void sbm_special_starbase_report(StarsSendBattleMessagesInput *input,
    StarsI16 recipient, StarsI16 x, StarsI16 y, StarsI16 base_owner,
    StarsI16 base_design, StarsI16 mineral_value, StarsI32 population)
{
    StarsI16 event;
    StarsI16 high_population = (StarsI16)(population >> 16);
    event = recipient == base_owner ?
        ((population < 0 || (population < 1 && mineral_value < 1001)) ?
            SBM_EVENT_STARBASE_LOW_YIELD : SBM_EVENT_STARBASE_LOST) :
        SBM_EVENT_STARBASE_OTHER;
    sbm_send(input, recipient, event, input->battle_id | SBM_BATTLE_OBJECT,
        x, y, (StarsI16)(base_owner * 32 + base_design + 16), mineral_value,
        high_population, 0, 0);
}

/* The ship/design branches classify the two sides from the loss matrix.  A
 * zero row means the starbase is standing in for that side; the original
 * encodes that fallback as base-owner plus base-design. */
static void sbm_two_side_report(StarsSendBattleMessagesInput *input,
    StarsI16 recipient, StarsI16 x, StarsI16 y, StarsI16 base_owner,
    StarsI16 base_design, StarsI16 use_design_counts)
{
    StarsI16 own = recipient;
    StarsI16 other = -1;
    StarsI16 player;
    StarsI16 own_design;
    StarsI16 other_design;
    StarsU16 *own_row;
    StarsU16 *other_row;
    StarsU16 own_value;
    StarsU16 other_value;
    StarsU16 own_count;
    StarsU16 other_count;
    StarsI16 event;
    StarsI16 own_damaged;
    StarsI16 other_damaged;
    StarsI16 own_dead;
    StarsI16 other_dead;
    StarsI16 own_pack;
    StarsI16 other_pack;

    for (player = 0; player < SBM_MAX_PLAYERS; ++player)
        if (player != own && sbm_row_value(sbm_row(input->player_losses,
                player))) { other = player; break; }
    own_row = sbm_row_present(sbm_row(input->player_losses, own)) ?
        sbm_row(input->player_losses, own) : (StarsU16 *)0;
    other_row = other < 0 ? (StarsU16 *)0 : sbm_row(input->player_losses, other);
    own_value = sbm_row_value(own_row);
    other_value = other_row ? sbm_row_value(other_row) : 0;
    own_dead = own_row && ((own_value != 0) || input->starbase_died);
    other_dead = other_row ? other_value != 0 : input->starbase_died;
    own_damaged = own_row ? sbm_row_flag(own_row, SBM_LOSS_DAMAGED) :
        input->starbase_damaged;
    other_damaged = other_row ? sbm_row_flag(other_row, SBM_LOSS_DAMAGED) :
        input->starbase_damaged;
    if (!use_design_counts) {
        if (own_dead) event = other_damaged ? SBM_EVENT_SHIP_BOTH_DAMAGED :
            SBM_EVENT_SHIP_BASE;
        else if (other_dead) event = own_damaged ?
            SBM_EVENT_SHIP_OTHER_DAMAGED : SBM_EVENT_SHIP_BASE_DAMAGED;
        else event = SBM_EVENT_SHIP_NONE;
    } else {
        if (own_dead) event = other_damaged ? SBM_EVENT_DESIGN_BOTH_DAMAGED :
            SBM_EVENT_DESIGN_BASE;
        else if (other_dead) event = own_damaged ?
            SBM_EVENT_DESIGN_OTHER_DAMAGED : SBM_EVENT_DESIGN_BASE_DAMAGED;
        else event = SBM_EVENT_DESIGN_NONE;
    }
    own_design = sbm_row_design(input->player_losses, own);
    other_design = other < 0 ? -1 : sbm_row_design(input->player_losses, other);
    own_pack = own_row && own_value ? sbm_pack_design(own, own_design) :
        sbm_pack_design(base_owner, base_design + 16);
    other_pack = other_row && other_value ? sbm_pack_design(other, other_design) :
        sbm_pack_design(base_owner, base_design + 16);
    if (use_design_counts) {
        own_count = (StarsU16)(own_value & 0x1fffU);
        other_count = (StarsU16)(other_value & 0x1fffU);
        own_count = (StarsU16)(own_count + sbm_count_ships(input, own,
            own_design));
        other_count = (StarsU16)(other_count + sbm_count_ships(input, other,
            other_design));
    } else {
        own_count = own_value;
        other_count = other_value;
    }
    sbm_send(input, recipient, event, input->battle_id | SBM_BATTLE_OBJECT,
        x, y, own_pack, own_count, other_pack, other_count, 0);
    if (event == SBM_EVENT_SHIP_NONE || event == SBM_EVENT_DESIGN_NONE)
        sbm_learn(input, recipient, x, y, 0x00ef);
}

static void sbm_generic_report(StarsSendBattleMessagesInput *input,
    StarsI16 recipient, StarsI16 x, StarsI16 y, StarsI16 base_owner,
    StarsI16 base_design, StarsI16 cplr)
{
    StarsI16 other_player = -1;
    StarsI16 player;
    StarsI16 own_losses = (StarsI16)sbm_row_value(sbm_row(
        input->player_losses, recipient));
    StarsI16 other_losses = 0;
    StarsI16 own_initial = own_losses;
    StarsI16 other_initial;
    StarsI16 own_ships = (StarsI16)(own_losses +
        sbm_count_ships(input, recipient, -1));
    StarsI16 other_ships = 0;
    StarsI16 event;
    StarsI16 other_tag = 0x30;
    StarsI16 own_pack = 0;
    StarsI16 other_pack = 0;
    const StarsU16 *row;

    for (player = 0; player < SBM_MAX_PLAYERS; ++player) {
        row = sbm_row(input->player_losses, player);
        if (player != recipient && sbm_row_present(row)) {
            other_player = player;
            other_losses = (StarsI16)sbm_row_value(row);
            other_pack = sbm_pack_design(player, sbm_row_design(
                input->player_losses, player));
            break;
        }
    }
    other_initial = other_losses;
    other_ships = other_losses + sbm_count_ships(input, other_player, -1);
    if (base_owner == recipient) {
        ++own_ships;
        ++own_initial;
    } else if (base_owner >= 0) {
        ++other_ships;
        ++other_initial;
        other_tag = (StarsI16)(base_owner | 0x30);
        if (other_player < 0) other_pack = sbm_pack_design(base_owner,
            base_design + 16);
    }
    if (cplr != 2) {
        /* Non-two-player battles use the compact cplr/count report family. */
        event = SBM_EVENT_UNKNOWN;
        if (own_initial == 0) {
            if (other_ships == other_initial)
                sbm_send(input, recipient, SBM_EVENT_EQUAL_NO_LOSS,
                    input->battle_id | SBM_BATTLE_OBJECT, x, y, cplr,
                    own_ships, 0, 0, 0);
            else event = SBM_EVENT_EQUAL_LOSS;
        } else if (other_initial == 0 && own_ships == own_initial) {
            sbm_send(input, recipient, SBM_EVENT_NO_ENEMY,
                input->battle_id | SBM_BATTLE_OBJECT, x, y, cplr,
                own_ships, other_ships, 0, 0);
        } else event = SBM_EVENT_UNKNOWN;
        if (event == SBM_EVENT_EQUAL_LOSS || event == SBM_EVENT_UNKNOWN)
            sbm_send(input, recipient, event, input->battle_id | SBM_BATTLE_OBJECT,
                x, y, cplr, own_initial, own_ships, other_initial, other_ships);
        if (own_initial != own_ships &&
                (!input->find_planet || base_owner < 0 || base_owner == recipient))
            sbm_learn(input, recipient, x, y, 0x00ef);
        return;
    }
    /* In a two-player report the first value identifies the opposing side;
     * the remaining values distinguish initial losses from current survivors. */
    other_tag = (StarsI16)(other_player >= 0 ? other_player | 0x30 : 0x30);
    if (base_owner >= 0 && base_owner != recipient)
        other_tag = (StarsI16)(base_owner | 0x30);
    if (own_ships == 1)
        own_pack = sbm_pack_design(recipient, sbm_row_design(
            input->player_losses, recipient));
    if (other_ships == 1)
        other_pack = sbm_pack_design(other_player, other_player < 0 ? -1 :
            sbm_row_design(input->player_losses, other_player));
    if (base_owner == recipient && own_ships == 1)
        own_pack = sbm_pack_design(base_owner, base_design + 16);
    if (base_owner >= 0 && base_owner != recipient && other_ships == 1)
        other_pack = sbm_pack_design(base_owner, base_design + 16);
    if (other_ships == other_initial) {
        if (other_initial == 1)
            event = own_initial == 0 ? SBM_EVENT_SINGLE_WIN :
                SBM_EVENT_SINGLE_WIN_WITH_LOSS;
        else if (own_initial == 0)
            event = own_ships == 1 ? SBM_EVENT_ONE_SHIP : SBM_EVENT_ONE_SIDED;
        else event = SBM_EVENT_MULTI_SIDED;
    } else if (own_initial == own_ships) {
        if (own_initial == 1)
            event = other_initial == 0 ? SBM_EVENT_SINGLE_LOSS :
                SBM_EVENT_SINGLE_LOSS_WITH_WIN;
        else if (other_initial == 0)
            event = other_ships == 1 ? SBM_EVENT_ONE_SHIP_REVERSE :
                SBM_EVENT_ONE_SIDED_REVERSE;
        else event = SBM_EVENT_MULTI_SIDED_REVERSE;
    } else if (own_ships == 1) event = SBM_EVENT_ONE_SIDE_ONE_SHIP;
    else if (other_ships == 1) event = SBM_EVENT_REVERSE_ONE_SHIP;
    else event = SBM_EVENT_MIXED;
    sbm_send(input, recipient, event, input->battle_id | SBM_BATTLE_OBJECT,
        x, y, other_tag,
        (event == SBM_EVENT_ONE_SHIP || event == SBM_EVENT_ONE_SIDE_ONE_SHIP) ?
            own_pack : own_ships,
        (event == SBM_EVENT_ONE_SHIP_REVERSE ||
            event == SBM_EVENT_REVERSE_ONE_SHIP) ? other_pack : other_ships,
        own_initial, other_initial);
    if (own_initial != own_ships &&
            (!input->find_planet || base_owner < 0 || base_owner == recipient))
        sbm_learn(input, recipient, x, y, 0x00ef);
}

/* Genuine semantic reconstruction of SendBattleMessages, NE 31:9C0E-AE05.
 * It first records which ship designs were present, then walks every player
 * and sends only the report that player is entitled to see.  Spectators get a
 * fleet/planet discovery message, missed fleets get a separate notification,
 * and participants receive starbase, two-side ship/design, or aggregate
 * battle-result reports.  The small technology callbacks preserve the
 * original post-report research notification without hiding it in a numeric
 * side effect. */
void stars_send_battle_messages(StarsSendBattleMessagesInput *input)
{
    StarsBattleMessageFleet *first;
    StarsBattleMessagePlanet *planet = (StarsBattleMessagePlanet *)0;
    StarsI16 x;
    StarsI16 y;
    StarsI16 base_owner = -1;
    StarsI16 base_design = 0;
    StarsI16 player;
    StarsBattleMessageFleet *fleet;
    StarsI16 special_race = 0;

    if (!input || !input->fleets || !input->player_losses ||
            input->player_count < 0) return;
    first = &input->fleets[0];
    if (first->planet_id < 0) {
        x = first->x;
        y = first->y;
    } else {
        x = -1;
        y = first->planet_id;
        if (input->find_planet) planet = input->find_planet(input->context, y);
        if (planet && planet->owner >= 0) {
            if ((planet->flags & SBM_STARBASE_PRESENT) || input->starbase_died) {
                base_owner = planet->owner;
                base_design = (StarsI16)(planet->starbase_design & 0x0fU);
            }
            if (input->starbase_died && input->race_stat &&
                    input->race_stat(input->context, planet->owner, 0x0e) == 8) {
                special_race = 1;
                if (input->uninhabit) input->uninhabit(input->context, planet);
            }
        }
    }
    sbm_mark_ship_losses(input);
    for (player = 0; player < input->player_count && player < SBM_MAX_PLAYERS;
            ++player) {
        if (!(input->battle_player_mask & sbm_bit(player))) {
            if (sbm_same_planet_owner(planet, player)) {
                sbm_send(input, player, SBM_EVENT_PLANET_OBSERVER,
                    planet->id, planet->id, 0, 0, 0, 0, 0, 0);
                sbm_learn(input, player, x, y, 0x00f1);
            } else if (input->spectator_players & sbm_bit(player)) {
                fleet = sbm_find_fleet(input, player, 0);
                if (fleet) {
                    sbm_send(input, player, SBM_EVENT_FLEET_OBSERVER,
                        fleet->id | SBM_FLEET_OBJECT, fleet->x, fleet->y,
                        0, 0, 0, 0, 0);
                    sbm_learn(input, player, x, y, 0x00f0);
                }
            }
            if (input->missed_players & sbm_bit(player)) {
                fleet = sbm_find_fleet(input, player, 1);
                if (fleet)
                    sbm_send(input, player, SBM_EVENT_MISSED_FLEET,
                        fleet->id | SBM_FLEET_OBJECT, x, y, 0, 0, 0, 0, 0);
            }
            continue;
        }
        if (special_race) {
            sbm_special_starbase_report(input, player, x, y, base_owner,
                base_design, planet ? (StarsI16)planet->mineral_value : 0,
                planet ? planet->population : 0);
        } else if (input->battle_player_count == 2 && input->ships_involved == 2) {
            sbm_two_side_report(input, player, x, y, base_owner, base_design, 0);
        } else if (input->battle_player_count == 2 &&
                input->ship_designs_involved == 2) {
            sbm_two_side_report(input, player, x, y, base_owner, base_design, 1);
        } else {
            sbm_generic_report(input, player, x, y, base_owner, base_design,
                input->battle_player_count);
        }
    }
}
