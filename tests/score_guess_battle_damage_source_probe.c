#include <stdio.h>
#include <string.h>
#include "core.h"

static StarsI32 damage(void *context, const StarsScoreGuessToken *shooter,
    const StarsScoreGuessToken *target, StarsU8 coordinate, StarsI16 reverse)
{
    (void)context; (void)shooter; (void)target; (void)coordinate;
    return reverse ? 5L : 3L;
}

int main(void)
{
    StarsScoreGuessToken source;
    StarsScoreGuessToken target;
    StarsScoreGuessInput input;
    memset(&source, 0, sizeof source);
    memset(&target, 0, sizeof target);
    memset(&input, 0, sizeof input);
    source.owner = 0;
    source.battle_coordinate = 0x11U;
    source.md_descriptor = 1U;
    target.owner = 1;
    target.battle_coordinate = 0x12U;
    target.md_grobj = 1U;
    target.md_descriptor = (StarsU16)(7U << 12);
    target.active_flags = 1U;
    input.source = &source;
    input.targets = &target;
    input.target_count = 1U;
    input.attack_players = 2U;
    input.primary_attack = 1;
    input.damage = damage;
    if (stars_score_guess_battle_damage(&input, 0x11U) != 5L)
        return 1;
    source.md_descriptor = (StarsU16)(1U | (1U << 8));
    if (stars_score_guess_battle_damage(&input, 0x11U) != -3L)
        return 2;
    target.active_flags = 0U;
    if (stars_score_guess_battle_damage(&input, 0x11U) != 0L)
        return 3;
    puts("SCORE_GUESS_BATTLE_DAMAGE_SOURCE=PASS");
    return 0;
}
