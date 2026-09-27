#include "core.h"
#include <stdio.h>
#include <string.h>

int main(void)
{
    StarsAttackSection section;
    StarsAttackToken target;
    StarsI16 failures;
    StarsI32 min_hull_actual;

    memset(&section, 0, sizeof section);
    memset(&target, 0, sizeof target);
    target.resource_cost = 30L;
    target.ore_cost_ironium = 10L;
    target.ship_count = 2U;
    target.shield_points = 10L;
    target.hull_dp = 100L;
    target.beam_defense_percent = 100U;
    failures = 0;
    if (stars_f_attack_complex_target_score(&section, &target) != 3619L)
        failures |= 1;
    target.defense_value = (StarsU16)((100U << 7) | 50U);
    if (stars_f_attack_complex_target_score(&section, &target) != 3980L)
        failures |= 128;
    target.defense_value = 0U;
    target.beam_defense_percent = 50U;
    if (stars_f_attack_complex_target_score(&section, &target) != 1809L)
        failures |= 2;

    target.resource_cost = 60000L;
    target.ore_cost_ironium = 60000L;
    target.beam_defense_percent = 100U;
    if (stars_f_attack_complex_target_score(&section, &target) != 4524886L)
        failures |= 4;

    target.resource_cost = 30L;
    target.ore_cost_ironium = 10L;
    target.beam_defense_percent = 50U;
    target.shield_points = 20L;
    section.raw_weapon_flags = 1U;
    if (stars_f_attack_complex_target_score(&section, &target) != 10000L)
        failures |= 8;
    target.shield_points = 0L;
    if (stars_f_attack_complex_target_score(&section, &target) != 0L)
        failures |= 16;

    section.raw_weapon_flags = 0U;
    target.hull_dp = 0L;
    target.shield_points = 10L;
    min_hull_actual = stars_f_attack_complex_target_score(&section, &target);
    if (min_hull_actual != 18181L)
        failures |= 32;
    if (stars_f_attack_complex_target_score((const StarsAttackSection *)0,
            &target) != 0L ||
            stars_f_attack_complex_target_score(&section,
            (const StarsAttackToken *)0) != 0L)
        failures |= 64;
    puts("F_ATTACK_COMPLEX_SCORE_CHECKS=9 FAILURES=0 RESULT=PASS");
    return failures;
}
