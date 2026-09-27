/* Genuine C reconstruction of NthValidEnemyShdef (NE 26:5cf4-5e7f). */
#include "core.h"

const StarsEnemyShipDesign *stars_nth_valid_enemy_design(
    const StarsNthEnemyDesignInput *input, StarsI16 index)
{
    StarsU16 player;
    StarsU16 design;
    StarsU16 limit;

    if (!input || !input->players || index < 0) return 0;
    limit = input->starbase_mode ? 10U : 16U;
    for (player = 0U; player < input->player_count; ++player) {
        const StarsEnemyDesignPlayer *entry = &input->players[player];
        if (player == (StarsU16)input->local_player || !entry->designs) continue;
        for (design = 0U; design < limit && design < entry->design_count; ++design) {
            if (!entry->designs[design].hidden) {
                if (index == 0) return &entry->designs[design];
                --index;
            }
        }
    }
    return 0;
}
