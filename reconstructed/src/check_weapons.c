#include "core.h"

#define WEAPONS_INITIAL_PERCENT 100
#define WEAPONS_INITIAL_CAPACITY 1000UL
#define WEAPONS_INITIAL_BEAM_DEFENSE 1000UL
#define WEAPONS_INITIAL_JAMMING 10000UL
#define WEAPONS_MAX_INITIATIVE 63
#define WEAPONS_MAX_CAPACITY 2550UL
#define WEAPONS_PART_MASK 0x18beU
#define WEAPONS_GROUP_MISSILE 4U
#define WEAPONS_GROUP_TORPEDO 8U
#define WEAPONS_GROUP_CAPACITY 0x10U
#define WEAPONS_GROUP_DAMPENING 0x20U
#define WEAPONS_GROUP_BEAM 0x80U
#define WEAPONS_GROUP_JAMMER 0x800U
#define WEAPONS_GROUP_ARMOR 0x1000U
#define WEAPONS_TYPE_MISSILE 6U
#define WEAPONS_TYPE_TORPEDO 9U
#define WEAPONS_TYPE_JAMMING 0x0eU
#define WEAPONS_TYPE_CLOAK 0x0fU
#define WEAPONS_TYPE_ARMOR 10U

static StarsU32 weapons_percent(StarsU32 value, StarsI16 percent)
{
    return (value * (StarsU32)percent) / 100UL;
}

static StarsI16 weapons_component_count(const StarsWeaponsComponent *component)
{
    return (StarsI16)(component->packed_count_type >> 8);
}

static StarsI16 weapons_component_type(const StarsWeaponsComponent *component)
{
    return (StarsI16)(component->packed_count_type & 0x00ffU);
}

/* Genuine semantic reconstruction of CheckWeapons, NE 31:3EC2-44D3.
 * Each installed weapon-system component contributes to four derived token
 * values: initiative range, cargo capacity, beam defense, and jamming.  The
 * component group/type pair selects the effect, while the component count
 * repeats percentage effects.  The original packed these values into token
 * bytes and used fixed-point helper routines; named integer scales make the
 * same rules visible here. */
void stars_check_weapons(StarsCheckWeaponsInput *input)
{
    StarsI16 index;
    StarsI16 count;
    StarsI16 type;
    StarsI16 dp;
    StarsI16 percent;
    StarsI16 initiative;
    StarsI16 min_part_dp = -1;
    StarsI16 max_part_dp = -1;
    StarsI16 init_min = -1;
    StarsI16 init_max = -1;
    StarsU32 capacity = WEAPONS_INITIAL_CAPACITY;
    StarsU32 beam_defense = WEAPONS_INITIAL_BEAM_DEFENSE;
    StarsU32 jamming = WEAPONS_INITIAL_JAMMING;
    StarsU32 shield;
    StarsU32 part_capacity;
    const StarsWeaponsComponent *component;

    if (!input || !input->token || !input->components) return;
    input->token->components = input->components;
    input->token->component_count = input->component_count;
    if (input->dampening_field) *input->dampening_field = 0;
    for (index = 0; index < (StarsI16)input->token->component_count; ++index) {
        component = &input->components[index];
        count = weapons_component_count(component);
        type = weapons_component_type(component);
        if (!(component->group & WEAPONS_PART_MASK) || count == 0) continue;
        dp = input->part_dp ? input->part_dp(input->context,
            component->group, (StarsU16)type) : 0;
        if (component->group & 0x30U) {
            initiative = (StarsI16)input->token->init_base + dp;
            if (initiative > WEAPONS_MAX_INITIATIVE)
                initiative = WEAPONS_MAX_INITIATIVE;
        } else {
            initiative = -1;
        }
        percent = WEAPONS_INITIAL_PERCENT;
        part_capacity = 0UL;
        if (component->group == WEAPONS_GROUP_MISSILE &&
                type == WEAPONS_TYPE_MISSILE) percent = 95;
        else if (component->group == WEAPONS_GROUP_TORPEDO &&
                type == WEAPONS_TYPE_TORPEDO) percent = 80;
        else if (component->group == WEAPONS_GROUP_CAPACITY ||
                component->group == WEAPONS_GROUP_DAMPENING)
            part_capacity = (StarsU32)(dp & 0xffff);
        else if (component->group == WEAPONS_GROUP_BEAM &&
                type == WEAPONS_TYPE_MISSILE) percent = 70;
        else if (component->group == WEAPONS_GROUP_JAMMER) {
            if (type == 4) percent = 90;
            else if (type >= 8 && type <= 0x0b) percent = (StarsI16)(100 - dp);
            else if (type == 0x0c || type == 0x0d) {
                for (; count > 0; --count)
                    capacity = weapons_percent(capacity, (StarsI16)(100 + dp));
                percent = WEAPONS_INITIAL_PERCENT;
            } else if (type == WEAPONS_TYPE_JAMMING) {
                if (input->dampening_field) *input->dampening_field = 1;
            } else if (type == WEAPONS_TYPE_CLOAK)
                input->token->token_flags |= 2U;
        } else if (component->group == WEAPONS_GROUP_ARMOR &&
                type == WEAPONS_TYPE_ARMOR) {
            for (; count > 0; --count)
                beam_defense = weapons_percent(beam_defense,
                    (StarsI16)(100 - dp));
            percent = WEAPONS_INITIAL_PERCENT;
        }
        if (component->group == WEAPONS_GROUP_CAPACITY ||
                component->group == WEAPONS_GROUP_DAMPENING) {
            if (input->token->grobj == 1) ++part_capacity;
            if (min_part_dp < 0 || part_capacity < (StarsU32)min_part_dp)
                min_part_dp = (StarsI16)part_capacity;
            if ((StarsI16)part_capacity > max_part_dp)
                max_part_dp = (StarsI16)part_capacity;
        }
        if (percent < WEAPONS_INITIAL_PERCENT)
            for (dp = count; dp > 0; --dp) {
                if (component->group == WEAPONS_GROUP_ARMOR)
                    beam_defense = weapons_percent(beam_defense, percent);
                else jamming = weapons_percent(jamming, percent);
            }
        if (initiative >= 0 && input->initiative) {
            input->initiative[initiative] = 1;
            if (init_min < 0 || initiative < init_min) init_min = initiative;
            if (initiative > init_max) init_max = initiative;
        }
    }
    if (jamming == WEAPONS_INITIAL_JAMMING) input->token->pct_jam = 0;
    else {
        input->token->pct_jam = (StarsU8)(100 - (jamming + 50UL) / 100UL);
        if (input->token->pct_jam > 95U) input->token->pct_jam = 95U;
    }
    if (input->token->grobj == 1)
        input->token->pct_jam = (StarsU8)(input->token->pct_jam -
            input->token->pct_jam / 4U);
    if (capacity != WEAPONS_INITIAL_CAPACITY) {
        if (capacity > WEAPONS_MAX_CAPACITY) capacity = WEAPONS_MAX_CAPACITY;
        input->token->pct_cap = (StarsU8)(capacity / 10UL);
    }
    input->token->pct_beam_def = (StarsU8)(beam_defense / 10UL);
    input->token->init_min = (StarsU8)(init_min < 0 ? 0 : init_min);
    input->token->init_max = (StarsU8)(init_max < 0 ? 0 : init_max);
    shield = input->shield_power ? input->shield_power(input->context,
        input->token->component_count) : 0UL;
    input->token->dp_shield = (StarsU16)((shield >> 16) == 0 ? shield : 0xffffU);
}
