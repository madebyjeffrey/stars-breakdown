/* Semantic reconstruction of NE 15:5964-704D (FWriteDataFile).
 *
 * FWriteDataFile is a save coordinator.  The difficult-looking original
 * body is mostly a sequence of visibility checks around the far-pointer game
 * arrays, followed by a fixed stream order.  This source retains that order
 * in C and gives the array-dependent checks typed predicates; it does not
 * manufacture a replacement stream from literal bytes.
 */
#include "core.h"

static StarsI16 write_data_open(const StarsWriteDataFileInput *input,
    const StarsWriteDataFileOps *ops, void *context,
    StarsWriteDataFileState *state)
{
    StarsI16 opened;
    if (input->append && ops->append_file) {
        opened = ops->append_file(context, input->player);
        if (opened) {
            state->used_append = 1;
            state->open = 1;
            return 1;
        }
    }
    if (!ops->create_file) return 0;
    opened = ops->create_file(context, state->file_type, input->player);
    if (opened) {
        state->open = 1;
        return 1;
    }
    return 0;
}

static void write_data_close(const StarsWriteDataFileOps *ops, void *context,
    StarsWriteDataFileState *state)
{
    if (state->open && ops && ops->close_file)
        ops->close_file(context);
    state->open = 0;
}

/* NE 15:5964-704D.  Set up visibility, open or append the destination, then
 * emit the same logical sections as the executable: battles, player records,
 * optional game salt, messages, planets, the first ship-design bank, fleets,
 * the second design bank, scores, things, battle plans, and the final type-0
 * turn marker. */
StarsI16 stars_write_data_file(const StarsWriteDataFileInput *input,
    StarsWriteDataFileState *state, const StarsWriteDataFileOps *ops,
    void *context)
{
    StarsU16 player;
    StarsU16 slot;
    StarsU16 bank_limit;
    StarsU16 start;
    StarsU16 end;
    StarsU16 message;
    if (!input || !state || !ops || input->player_count > 32U)
        return 0;
    state->player = input->player;
    state->append = input->append;
    state->file_type = input->player == -1 ? STARS_WRITE_DATA_GAME_FILE :
        STARS_WRITE_DATA_PLAYER_FILE;
    state->player_start = input->player == -1 ? 0U : (StarsU16)input->player;
    state->player_end = input->player == -1 ? input->player_count :
        (StarsU16)(input->player + 1);
    state->open = 0;
    state->used_append = 0;
    if (input->player < -1 || state->player_end > input->player_count)
        return 0;

    if (ops->set_visible_plan_fleet)
        ops->set_visible_plan_fleet(context, input->player);
    if (input->player != -1 && (input->session_flags & 0x0002U) &&
            ops->prepare_view)
        ops->prepare_view(context, input->player);
    if (ops->mark_messages)
        ops->mark_messages(context, input->player);
    if (ops->mark_planets_lost)
        ops->mark_planets_lost(context, input->player);

    if (!write_data_open(input, ops, context, state)) {
        if (input->append) message = STARS_WRITE_DATA_ALERT_APPEND;
        else if (input->player == -1) message = STARS_WRITE_DATA_ALERT_GAME;
        else message = STARS_WRITE_DATA_ALERT_PLAYER;
        if (ops->alert) ops->alert(context, message);
        if (ops->set_visible_plan_fleet)
            ops->set_visible_plan_fleet(context, -1);
        return 0;
    }

    if (ops->write_battles)
        ops->write_battles(context, input->player);
    if (ops->write_player_predicate && ops->write_player) {
        for (player = 0U; player < input->player_count; ++player)
            if (ops->write_player_predicate(context, player))
                ops->write_player(context, player);
    }
    if (input->player == -1 && input->salt != 0UL && ops->write_salt)
        ops->write_salt(context, input->salt);
    if (ops->write_messages)
        ops->write_messages(context, input->player);
    if (ops->write_planets)
        ops->write_planets(context, input->player);

    /* The normal player designs occupy sixteen slots and precede fleets;
     * the extra ten-slot table follows fleets in the original stream. */
    if (ops->write_design_predicate && ops->write_design) {
        start = 0U;
        end = input->player_count;
        for (player = start; player < end; ++player) {
            state->design_bank = 0U;
            bank_limit = 16U;
            for (slot = 0U; slot < bank_limit; ++slot) {
                state->design_slot = slot;
                if (ops->write_design_predicate(context, player,
                        state->design_bank, slot))
                    ops->write_design(context, player,
                        state->design_bank, slot);
            }
        }
    }
    if (ops->write_fleet_predicate && ops->write_fleet) {
        for (state->fleet_index = 0U; state->fleet_index < input->fleet_count;
                ++state->fleet_index)
            if (ops->write_fleet_predicate(context, state->fleet_index))
                ops->write_fleet(context, state->fleet_index);
    }
    if (ops->write_design_predicate && ops->write_design) {
        start = 0U;
        end = input->player_count;
        for (player = start; player < end; ++player) {
            state->design_bank = 1U;
            bank_limit = 10U;
            for (slot = 0U; slot < bank_limit; ++slot) {
                state->design_slot = slot;
                if (ops->write_design_predicate(context, player,
                        state->design_bank, slot))
                    ops->write_design(context, player,
                        state->design_bank, slot);
            }
        }
    }
    if (ops->write_scores)
        ops->write_scores(context, input->player);
    if (ops->write_things)
        ops->write_things(context, input->player);
    if (ops->write_battle_plans) {
        start = state->player_start;
        end = state->player_end;
        for (player = start; player < end; ++player)
            ops->write_battle_plans(context, (StarsI16)player);
    }
    if (ops->write_turn)
        ops->write_turn(context, input->turn);
    write_data_close(ops, context, state);
    if (ops->set_visible_plan_fleet)
        ops->set_visible_plan_fleet(context, -1);
    return 1;
}
