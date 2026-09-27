#include "core.h"

static StarsI16 hull_component_kind(StarsU16 encoded)
{
    return (StarsI16)(encoded & 0x00ffU);
}

static StarsI16 hull_component_count(StarsU16 encoded)
{
    return (StarsI16)(encoded >> 8);
}

static StarsI16 add_percentage(StarsI16 current, StarsI16 percent)
{
    return (StarsI16)(current + ((100 - current) * percent) / 100);
}

/* Genuine C reconstruction of InitFromHuldef, NE 1F:3CBA-3E7A. */
StarsI16 stars_init_from_huldef(const StarsHullDefinitionInput *input,
    StarsI16 *percent_battle_coverage, StarsHullPartDamage part_damage,
    void *context)
{
    StarsI16 coverage;
    StarsI16 extra;
    StarsI16 index;
    StarsI16 count;
    StarsI16 kind;
    StarsI16 damage;

    if (!input || (!input->components && input->component_count != 0U))
        return 0;
    coverage = 0;
    extra = 0;
    for (index = 0; index < (StarsI16)input->component_count; ++index) {
        kind = hull_component_kind(input->components[index].packed_damage);
        count = hull_component_count(input->components[index].packed_damage);
        if (count == 0)
            continue;
        if ((input->components[index].attributes & 0x0800U) == 0U) {
            if ((input->components[index].attributes & 0x0010U) != 0U &&
                kind == 0x12) {
                for (; count > 0; --count)
                    coverage = add_percentage(coverage, 10);
            }
        } else if (kind == 5 || kind == 6 || kind == 7) {
            extra = (StarsI16)(extra + (kind - 4) * count);
            damage = part_damage ?
                part_damage(context, &input->components[index]) : 0;
            for (; count > 0; --count)
                coverage = add_percentage(coverage, damage);
        }
    }
    if (percent_battle_coverage)
        *percent_battle_coverage = coverage;
    if ((StarsI16)(input->base_value + extra) > 63)
        return 63;
    return (StarsI16)(input->base_value + extra);
}
