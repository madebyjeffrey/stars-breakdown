/* Genuine C reconstruction of FDoesPrimaryTargetTypeExist (NE 31:56d8-5879). */
#include "core.h"

StarsI16 stars_does_primary_target_type_exist(
    StarsI16 local_player, StarsU16 target_type, StarsU16 attack_players,
    const StarsBattleTargetToken *tokens, StarsU16 token_count)
{
    StarsU16 i;

    if (!tokens || target_type == 0U) return 0;
    for (i = 0U; i < token_count; ++i) {
        const StarsBattleTargetToken *token = &tokens[i];
        if (token->player != local_player && token->player >= 0 &&
            token->player < 16 &&
            (attack_players & (StarsU16)(1U << token->player)) != 0U &&
            (token->active_flags & 1U) != 0U) {
            if (target_type == 1U) return 1;
            if (target_type == 2U) {
                if (token->grobj == 1U) return 1;
            } else if (target_type == 3U || target_type == 6U ||
                       target_type == 7U) {
                if (token->primary_target_type == target_type) return 1;
            } else if (target_type == 4U) {
                if (token->primary_target_type == 4U ||
                    token->primary_target_type == 7U) return 1;
            } else if (target_type == 5U &&
                       token->primary_target_type > 4U) return 1;
        }
    }
    return 0;
}
