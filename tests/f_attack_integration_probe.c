#include <stdio.h>
#include <string.h>
#include "core.h"

/* Raw Win16 views recovered from _tok (29 bytes), _hul (123 bytes), and the
 * six-byte battle record.  The probe deliberately keeps the packed boundary
 * in the adapter instead of treating the typed reconstruction as storage. */
typedef struct RawBattle {
    unsigned char tok[29];
    unsigned char hul[123];
    unsigned char record[6];
    StarsDamageApplyFleet fleet;
    StarsI16 damage_calls;
    StarsI16 killed;
    StarsI16 records;
    StarsU32 last_damage;
} RawBattle;

static StarsU16 get_word(const unsigned char *p, StarsI16 off)
{
    return (StarsU16)((StarsU16)p[off] | ((StarsU16)p[off + 1] << 8));
}

static void put_word(unsigned char *p, StarsI16 off, StarsU16 value)
{
    p[off] = (unsigned char)(value & 0xffU);
    p[off + 1] = (unsigned char)(value >> 8);
}

static StarsDamageApplyFleet *find_fleet(void *context, StarsI16 id)
{
    RawBattle *raw = (RawBattle *)context;
    (void)id;
    return &raw->fleet;
}

static void kill_ships(void *context, StarsDamageApplyToken *token,
    StarsI16 count, StarsDamageApplyFleet *fleet)
{
    RawBattle *raw = (RawBattle *)context;
    (void)token;
    (void)fleet;
    raw->killed = (StarsI16)(raw->killed + count);
}

static void record_damage(void *context, StarsI16 id, StarsU16 flags,
    StarsU32 damage, StarsU16 record_flags)
{
    RawBattle *raw = (RawBattle *)context;
    (void)flags;
    (void)record_flags;
    put_word(raw->record, 0, (StarsU16)id);
    put_word(raw->record, 2, (StarsU16)damage);
    raw->record[4] = (unsigned char)(record_flags & 0xffU);
    ++raw->records;
    raw->last_damage = damage;
}

static StarsI16 apply_damage(void *context, StarsI16 attacker,
    StarsI16 target_index, StarsI16 section, StarsI32 amount, StarsI16 mode)
{
    RawBattle *raw = (RawBattle *)context;
    StarsDamageApplyToken token;
    StarsFDamageTokInput damage;
    StarsU32 beam;
    StarsU16 before_count;
    (void)attacker;
    (void)section;
    if (target_index != 0) return 0;
    memset(&token, 0, sizeof token);
    token.id = (StarsI16)get_word(raw->tok, 0);
    token.player = (StarsI16)raw->tok[2];
    token.ship_count = get_word(raw->tok, 19);
    token.dp_shield = get_word(raw->tok, 17);
    token.hull_damage = get_word(raw->hul, 56);
    before_count = token.ship_count;
    beam = (StarsU32)amount;
    memset(&damage, 0, sizeof damage);
    damage.token = &token;
    damage.beam_damage = &beam;
    damage.weapon_flags = (StarsU16)mode;
    damage.find_fleet = find_fleet;
    damage.kill_ships = kill_ships;
    damage.record = record_damage;
    damage.context = raw;
    ++raw->damage_calls;
    if (!stars_f_damage_tok(&damage)) return 0;
    put_word(raw->tok, 17, token.dp_shield);
    put_word(raw->tok, 19, token.ship_count);
    raw->fleet.ship_count = token.ship_count;
    if (before_count == token.ship_count && amount != 0L) return 0;
    return 1;
}

static int run_case(void)
{
    RawBattle raw;
    StarsAttackSection section;
    StarsAttackToken target;
    StarsAttackInput attack;
    StarsI16 result;

    memset(&raw, 0, sizeof raw);
    memset(&section, 0, sizeof section);
    memset(&target, 0, sizeof target);
    memset(&attack, 0, sizeof attack);

    /* _tok: id 0, owner 2, battle coordinate 5, dpShield 17, csh 19,
     * packed target field 23, active flags 27.  _hul.dp is at 56. */
    put_word(raw.tok, 0, 17U);
    raw.tok[2] = 1U;
    raw.tok[5] = 0x11U;
    put_word(raw.tok, 17, 3U);
    put_word(raw.tok, 19, 2U);
    put_word(raw.tok, 23, 2U);
    put_word(raw.tok, 27, 1U);
    put_word(raw.hul, 0, 4U);
    put_word(raw.hul, 56, 10U);

    target.owner = (StarsI16)raw.tok[2];
    target.battle_coordinate = raw.tok[5];
    target.active_flags = get_word(raw.tok, 27);
    target.target_mask = get_word(raw.tok, 23);
    target.battle_rating = 3;
    target.beam_defense_percent = 100U;

    section.init = 4;
    section.weapon_mode = 0;
    section.target_slots = 2U;
    section.battle_rating = 5;
    section.base_damage = 80L;
    section.active = 1U;
    section.beam_passes = 1;

    attack.attacker_index = 9;
    attack.attacker_owner = 0;
    attack.attacker_coordinate = 0x11U;
    attack.init = 4;
    attack.attack_players = (StarsU16)(1U << 1);
    attack.section_count = 1;
    attack.sections = &section;
    attack.target_count = 1;
    attack.targets = &target;
    attack.ops.damage_token = apply_damage;
    attack.ops.context = &raw;
    attack.battle_record = raw.record;

    result = stars_f_attack(&attack);
    if (!result || raw.damage_calls != 1 || raw.killed != 2 ||
            get_word(raw.tok, 17) != 0U || get_word(raw.tok, 19) != 0U ||
            get_word(raw.hul, 56) != 10U || raw.records != 2 ||
            get_word(raw.record, 0) != 17U || raw.last_damage != 74UL ||
            raw.record[2] != 1U || raw.record[3] != 0U || raw.record[5] != 0U ||
            attack.hit_count != 1 || attack.first_hit_target != 0)
        return 1;

    /* A second direct call uses the same packed token/hull adapter to prove
     * the shield-only branch preserves the residual beam and raw hull field. */
    put_word(raw.tok, 17, 4U);
    put_word(raw.tok, 19, 2U);
    {
        StarsDamageApplyToken token;
        StarsFDamageTokInput damage;
        StarsU32 beam = 6UL;
        memset(&token, 0, sizeof token);
        token.id = (StarsI16)get_word(raw.tok, 0);
        token.ship_count = get_word(raw.tok, 19);
        token.dp_shield = get_word(raw.tok, 17);
        token.hull_damage = get_word(raw.hul, 56);
        memset(&damage, 0, sizeof damage);
        damage.token = &token;
        damage.beam_damage = &beam;
        damage.shields_only = 1;
        damage.record = record_damage;
        damage.context = &raw;
        if (stars_f_damage_tok(&damage) != 0 || beam != 0UL ||
                token.dp_shield != 1U || token.hull_damage != 10U)
            return 1;
    }
    return 0;
}

int main(void)
{
    int failures = run_case();
#ifndef INTEGRATION_NATIVE
    printf("F_ATTACK_INTEGRATION_CHECKS=2 FAILURES=%d RESULT=%s\n",
        failures, failures ? "FAIL" : "PASS");
#endif
    return failures;
}
