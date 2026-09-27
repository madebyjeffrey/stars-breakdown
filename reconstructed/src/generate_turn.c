#include "core.h"

#define TURN_PROGRESS_LOAD 0x168
#define TURN_PROGRESS_LOGS 0x172
#define TURN_PROGRESS_ORDERS 0x1b8
#define TURN_PROGRESS_PRE_MOVE 0x212
#define TURN_PROGRESS_FLEETS 0x226
#define TURN_PROGRESS_DECAY 0x28a
#define TURN_PROGRESS_PRODUCE 700
#define TURN_PROGRESS_POST_PRODUCE 0x2ee
#define TURN_PROGRESS_FUEL 0x302
#define TURN_PROGRESS_FINISH 0x352
#define TURN_PROGRESS_SCORE 0x354
#define TURN_PROGRESS_FILES 0x356
#define TURN_PROGRESS_DONE 1000
#define TURN_MAX_PLAYERS 16

/* FGenerateTurn, after serial checks: repair an occupied ship design's
 * first slot to an engine, retaining a nonzero installed quantity. */
void stars_prepare_turn_design(StarsU8 *design)
{
    StarsU16 flags, group;
    if (!design) return;
    flags = (StarsU16)(design[123] | ((StarsU16)design[124] << 8));
    group = (StarsU16)(design[58] | ((StarsU16)design[59] << 8));
    if (!(flags & 0x0200U) && group != 1U) {
        design[58] = 1U; design[59] = 0U; design[60] = 1U;
        if (!design[61]) design[61] = 1U;
    }
}

/* Original pre-waypoint pass clears moved/follow flags, then marks a
 * single fleet-target order for the subsequent follow-resolution loop. */
StarsU16 stars_prepare_turn_fleet_flags(StarsU16 flags, StarsU16 orders,
    StarsU16 first_order_flags)
{
    flags &= 0x3fffU;
    if (orders == 1U && ((first_order_flags >> 8) & 15U) == 2U)
        flags |= 0x8000U;
    return flags;
}

static void turn_progress(const StarsGenerateTurnOps *ops, StarsI16 value)
{
    if (ops && ops->progress) ops->progress(ops->context, value);
}

static void turn_stage(const StarsGenerateTurnOps *ops,
    StarsGenerateTurnStage stage, StarsGenerateTurnState *state)
{
    if (stage) stage(ops->context, state);
}

static void turn_mode_stage(const StarsGenerateTurnOps *ops,
    StarsGenerateTurnModeStage stage, StarsGenerateTurnState *state,
    StarsI16 mode)
{
    if (stage) stage(ops->context, state, mode);
}

static void turn_cleanup(const StarsGenerateTurnOps *ops,
    StarsGenerateTurnState *state)
{
    if (ops && ops->cleanup) ops->cleanup(ops->context, state);
}

/* FGenerateTurn initializes abStack_1c to [0..cPlayer-1], then performs
 * the original forward Fisher-Yates shuffle. Random(1) is intentional: the
 * RNG advances both seeds even though the only returned value is zero. */
static StarsI16 turn_shuffle_players(StarsGenerateTurnState *state,
    const StarsGenerateTurnOps *ops)
{
    StarsI16 player;
    StarsI16 offset;
    StarsI16 selected;
    StarsU8 temporary;

    if (state->player_count < 0 || state->player_count > TURN_MAX_PLAYERS)
        return 0;
    if (state->player_count > 0 && !ops->random_value)
        return 0;
    for (player = 0; player < state->player_count; ++player)
        state->player_order[player] = (StarsU8)player;
    for (player = 0; player < state->player_count; ++player) {
        offset = ops->random_value(ops->context,
            (StarsI16)(state->player_count - player));
        if (offset < 0 || offset >= state->player_count - player)
            return 0;
        selected = (StarsI16)(player + offset);
        if (selected != player) {
            temporary = state->player_order[selected];
            state->player_order[selected] = state->player_order[player];
            state->player_order[player] = temporary;
        }
    }
    state->player_order_count = state->player_count;
    return 1;
}

StarsI16 stars_generate_turn(StarsGenerateTurnState *state,
    const StarsGenerateTurnOps *ops)
{
    StarsI16 player;
    StarsI16 log_index;

    if (!state || !ops || !ops->load_game) return 0;
    state->loaded = 0;
    state->logs_loaded = 0;
    state->generated = 0;
    state->output_written = 0;
    state->interrupted = 0;
    state->player_order_count = 0;
    for (player = 0; player < TURN_MAX_PLAYERS; ++player)
        state->player_file_append[player] = 0U;
    turn_stage(ops, ops->destroy_game, state);
    turn_progress(ops, TURN_PROGRESS_LOAD);
    if (!ops->load_game(ops->context, state)) {
        turn_cleanup(ops, state);
        turn_progress(ops, TURN_PROGRESS_DONE);
        return 0;
    }
    state->loaded = 1;
    turn_stage(ops, ops->initialize_turn, state);
    if (!turn_shuffle_players(state, ops)) {
        turn_cleanup(ops, state);
        turn_progress(ops, TURN_PROGRESS_DONE);
        return 0;
    }
    turn_progress(ops, TURN_PROGRESS_LOGS);
    for (log_index = 0; log_index < state->player_count; ++log_index) {
        player = (StarsI16)state->player_order[log_index];
        if (ops->load_player_log && !ops->load_player_log(ops->context,
                state, player)) {
            turn_cleanup(ops, state);
            turn_progress(ops, TURN_PROGRESS_DONE);
            return 0;
        }
        turn_progress(ops, (StarsI16)(TURN_PROGRESS_LOGS +
            (60 * (log_index + 1)) / (state->player_count + 1)));
    }
    state->logs_loaded = 1;
    turn_stage(ops, ops->validate_player_logs, state);
    turn_stage(ops, ops->validate_waypoints, state);
    turn_progress(ops, TURN_PROGRESS_ORDERS);
    turn_mode_stage(ops, ops->do_orders, state, 0);
    turn_progress(ops, TURN_PROGRESS_PRE_MOVE);

    turn_stage(ops, ops->unmark_mine_fields, state);
    turn_mode_stage(ops, ops->move_things, state, 0);
    turn_progress(ops, TURN_PROGRESS_FLEETS);
    turn_stage(ops, ops->move_fleets, state);
    turn_progress(ops, TURN_PROGRESS_DECAY);
    turn_stage(ops, ops->thing_decay, state);
    turn_stage(ops, ops->breed_colonists, state);
    turn_progress(ops, TURN_PROGRESS_PRODUCE);
    turn_stage(ops, ops->produce, state);
    turn_progress(ops, TURN_PROGRESS_POST_PRODUCE);
    turn_mode_stage(ops, ops->move_things, state, 1);
    turn_progress(ops, TURN_PROGRESS_FUEL);
    turn_stage(ops, ops->fuel_fleets, state);
    turn_mode_stage(ops, ops->do_orders, state, 1);
    turn_stage(ops, ops->sweep_for_mines, state);
    turn_stage(ops, ops->heal_ships, state);
    turn_stage(ops, ops->auto_terraform, state);
    turn_stage(ops, ops->remote_terraforming, state);
    turn_progress(ops, TURN_PROGRESS_FINISH);
    turn_stage(ops, ops->spank_cheaters, state);
    turn_stage(ops, ops->validate_waypoints, state);
    turn_stage(ops, ops->update_guesses, state);
    turn_progress(ops, TURN_PROGRESS_SCORE);
    turn_stage(ops, ops->mark_backup, state);
    ++state->turn;
    turn_progress(ops, TURN_PROGRESS_FILES);
    turn_stage(ops, ops->update_scores, state);
    turn_stage(ops, ops->prepare_output_files, state);
    if (ops->write_player_file) {
        for (player = 0; player < state->player_count; ++player)
            ops->write_player_file(ops->context, state, player, 0);
        ops->write_player_file(ops->context, state, -1, 0);
    }
    if (ops->write_data_file) {
        for (player = 0; player < state->player_count; ++player)
            ops->write_data_file(ops->context, state, player,
                (StarsI16)(state->player_file_append[player] != 0U));
        ops->write_data_file(ops->context, state, -1, 0);
    }
    state->output_written = 1;
    state->generated = 1;
    turn_progress(ops, TURN_PROGRESS_DONE);
    turn_cleanup(ops, state);
    return 1;
}
