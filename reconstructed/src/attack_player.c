/* Genuine C reconstruction of FAttackPlayer (NE 31:ae06-aefa). */
#include "core.h"

StarsI16 stars_attack_player(StarsU16 plan_behavior, StarsI16 relation,
    StarsI16 target_player)
{
    if (plan_behavior == 0U) return 0;
    if (plan_behavior == 1U) return relation == 2 ? 1 : 0;
    if (plan_behavior == 2U) return relation != 1 ? 1 : 0;
    if (plan_behavior == 3U) return 1;
    return target_player == (StarsI16)(plan_behavior - 4U) ? 1 : 0;
}
