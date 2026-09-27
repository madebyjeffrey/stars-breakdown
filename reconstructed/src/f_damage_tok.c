#include "core.h"

#define DAMAGE_RECORD_SHIELD 0x0004U
#define DAMAGE_RECORD_DEATH 0x0001U
#define DAMAGE_STARBASE 1U
#define DAMAGE_ARMOR_FLAG 0x0080U
#define DAMAGE_SHIELD_SCALE 500UL
#define DAMAGE_MAX_STARBASE 500UL
#define DAMAGE_MINIMUM 1UL

static StarsU32 damage_minimum(StarsU32 left, StarsU32 right)
{
    return left < right ? left : right;
}

/* Genuine semantic reconstruction of FDamageTok, NE 31:81D4-8B28.
 * Damage is applied in layers.  Active shields absorb up to shield strength
 * times ship count first; shield-only calls stop after recording that layer.
 * Remaining damage is then routed through the starbase or fleet path, where
 * casualties, loss counters, residual beam damage, torpedo packets, and the
 * compact battle record are updated. */
StarsI16 stars_f_damage_tok(StarsFDamageTokInput *input)
{
    StarsU32 beam;
    StarsU32 shield_capacity;
    StarsU32 shield_damage;
    StarsU32 total_damage;
    StarsU32 hull_per_ship;
    StarsU32 casualty_count;
    StarsU32 remaining_packets;
    StarsI16 killed;
    StarsDamageApplyPlanet *planet;
    StarsDamageApplyFleet *fleet;
    StarsU16 record_flags = 0;

    if (!input || !input->token || !input->beam_damage) return 0;
    beam = *input->beam_damage;
    shield_capacity = (StarsU32)input->token->dp_shield *
        (StarsU32)input->token->ship_count;
    shield_damage = damage_minimum(beam, shield_capacity);
    if (shield_damage) {
        input->token->dp_shield = (StarsU16)(input->token->dp_shield -
            shield_damage / (input->token->ship_count ?
                input->token->ship_count : 1U));
        beam -= shield_damage;
        record_flags |= DAMAGE_RECORD_SHIELD;
        if (input->record)
            input->record(input->context, input->token->id,
                input->weapon_flags, shield_damage, record_flags);
    }
    if ((!beam && !input->torpedo_damage) || input->shields_only) {
        *input->beam_damage = beam;
        return 0;
    }
    total_damage = beam + input->torpedo_damage;
    if (input->token->object_kind == DAMAGE_STARBASE) {
        planet = input->find_planet ? input->find_planet(input->context,
            input->token->id) : (StarsDamageApplyPlanet *)0;
        if (!planet) return 0;
        if (input->token->defense_value)
            total_damage += (StarsU32)input->token->hull_damage *
                input->token->defense_value / DAMAGE_SHIELD_SCALE;
        if (total_damage >= planet->starbase_damage) {
            planet->starbase_damage = planet->starbase_limit;
            input->token->ship_count = 0;
            record_flags |= DAMAGE_RECORD_DEATH;
            if (input->kill_queued_ships)
                input->kill_queued_ships(input->context, planet);
            if (input->kill_queued_packets)
                input->kill_queued_packets(input->context, planet);
        } else planet->starbase_damage = (StarsU16)(planet->starbase_damage -
            total_damage);
        *input->beam_damage = 0;
        if (input->record)
            input->record(input->context, input->token->id,
                input->weapon_flags, total_damage, record_flags);
        return 1;
    }
    fleet = input->find_fleet ? input->find_fleet(input->context,
        input->token->id) : (StarsDamageApplyFleet *)0;
    if (!fleet) return 0;
    hull_per_ship = input->token->hull_damage ? input->token->hull_damage : 1UL;
    casualty_count = (total_damage + hull_per_ship - 1UL) / hull_per_ship;
    if (casualty_count > input->token->ship_count)
        casualty_count = input->token->ship_count;
    killed = (StarsI16)casualty_count;
    if (killed && input->kill_ships)
        input->kill_ships(input->context, input->token, killed, fleet);
    input->token->ship_count = (StarsU16)(input->token->ship_count - casualty_count);
    if (input->torpedo_packets) {
        remaining_packets = *input->torpedo_packets;
        if (remaining_packets > casualty_count)
            remaining_packets -= casualty_count;
        else remaining_packets = 0;
        *input->torpedo_packets = remaining_packets;
    }
    *input->beam_damage = 0;
    if (input->record)
        input->record(input->context, input->token->id,
            input->weapon_flags, total_damage, record_flags);
    return 1;
}
