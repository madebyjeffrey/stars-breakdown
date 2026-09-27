/* Semantic reconstruction of the AI-turn preparation routine. */
#include "core.h"

static StarsI16 progress_value(StarsU16 player, StarsU16 count)
{
    if (!count) return 0;
    return (StarsI16)((0x154L * (player + 1U)) / count);
}

/* NE 5:56BC-5899. AI turns are run only once per loaded game state; the
 * caller supplies the game/file services while the flag transitions remain
 * in the reconstructed source. */
void stars_ensure_ais(StarsEnsureAisState *state,
    const StarsEnsureAisOps *ops, void *context)
{
    StarsU16 original_ai_mode;
    StarsU16 restore_file_mode;
    StarsU16 i;
    StarsU16 did_ai;
    StarsU16 handle;
    StarsU16 saved_file_error_silent;

    if (!state || !ops || (state->flags & 0x0400U)) return;
    saved_file_error_silent = state->file_error_silent;
    original_ai_mode = (StarsU16)((state->flags >> 4) & 1U);
    restore_file_mode = (StarsU16)((state->flags >> 3) & 1U);
    if (!restore_file_mode && ops->destroy_game && ops->load_game) {
        ops->destroy_game(context, state->base_path, 0U);
        ops->load_game(context, state->base_path, 0x03f0U);
    }
    state->flags = (StarsU16)((state->flags & 0xffefU) | 0x0010U);
    state->file_error_silent = 1U;
    did_ai = 0U;
    for (i = 0U; i < state->player_count && i < 16U; ++i) {
        if (ops->update_progress)
            ops->update_progress(context, progress_value(i,
                state->player_count));
        if (state->player_flags[i] & 0x0200U) {
            did_ai = 1U;
            state->flags = (StarsU16)((state->flags & 0xfff5U) | 10U);
            handle = ops->open_file ? ops->open_file(context,
                (StarsI16)i, 0x20U) : 0U;
            state->flags = (StarsU16)((state->flags & 0xfff5U) |
                (restore_file_mode << 3));
            if (!handle) {
                if (ops->do_ai_turn)
                    ops->do_ai_turn(context, (StarsI16)i,
                        state->player_flags[i]);
            } else if (ops->close_file) {
                ops->close_file(context, (StarsI16)handle);
            }
        }
    }
    state->flags = (StarsU16)((state->flags & 0xffefU) |
        (original_ai_mode << 4));
    if (did_ai && ops->destroy_game && ops->load_game) {
        ops->destroy_game(context, state->base_path, 0U);
        ops->load_game(context, state->base_path, 0x03f4U);
    }
    state->file_error_silent = saved_file_error_silent;
    state->flags = (StarsU16)((state->flags & 0xfbffU) | 0x0400U);
}
