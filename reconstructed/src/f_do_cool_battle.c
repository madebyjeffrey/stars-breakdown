#include "core.h"
#include <string.h>

#define STARS_COOL_BATTLE_EVENT_MOVE 1
#define STARS_COOL_BATTLE_EVENT_ATTACK 2
#define STARS_COOL_BATTLE_HEADER_BYTES 14U
#define STARS_COOL_BATTLE_RECORD_BYTES 6U
#define STARS_COOL_BATTLE_TOKEN_BYTES 29U

/* Typed semantic reconstruction of FDoCoolBattle's battle-lifecycle
 * orchestration, NE 31:8BCC-9917.  Token packing, movement geometry, and
 * individual FAttack/FDamageTok semantics are deliberately callback-owned;
 * this layer preserves their original order and termination protocol. */

static StarsI16 cool_battle_player_count(StarsU16 mask)
{
    StarsI16 count;

    count = 0;
    while (mask != 0) {
        if ((mask & 1U) != 0)
            ++count;
        mask = (StarsU16)(mask >> 1);
    }
    return count;
}

static void cool_battle_collect_participants(
    const StarsCoolBattleFleet *fleets, StarsI16 fleet_count,
    StarsU16 *active_players, StarsI16 *active_ships)
{
    StarsI16 fleet;

    *active_players = 0;
    *active_ships = 0;
    for (fleet = 0; fleet < fleet_count; ++fleet) {
        /* The packed battle flag is represented by active: zero is the
         * normal fleet state, while a nonzero value is the raw inactive bit. */
        if (fleets[fleet].active == 0 && fleets[fleet].ship_count != 0) {
            *active_ships = (StarsI16)(*active_ships +
                fleets[fleet].ship_count);
            if (fleets[fleet].owner >= 0 && fleets[fleet].owner < 16)
                *active_players = (StarsU16)(*active_players |
                    (StarsU16)(1U << fleets[fleet].owner));
        }
    }
}

static StarsU16 cool_battle_token_players(const StarsCoolBattleToken *tokens,
    StarsI16 token_count)
{
    StarsI16 index;
    StarsU16 players;

    players = 0;
    for (index = 0; index < token_count; ++index) {
        if (tokens[index].active && tokens[index].player >= 0 &&
                tokens[index].player < 16)
            players = (StarsU16)(players |
                (StarsU16)(1U << tokens[index].player));
    }
    return players;
}

static void cool_battle_put_word(StarsU8 *buffer, StarsU16 offset,
    StarsU16 value)
{
    buffer[offset] = (StarsU8)(value & 0xffU);
    buffer[offset + 1U] = (StarsU8)(value >> 8);
}

static StarsU16 cool_battle_get_word(const StarsU8 *buffer, StarsU16 offset)
{
    return (StarsU16)(buffer[offset] |
        ((StarsU16)buffer[offset + 1U] << 8));
}

static void cool_battle_load_packed_token(const StarsU8 *raw,
    StarsCoolBattleToken *token)
{
    token->id = (StarsI16)cool_battle_get_word(raw, 0);
    token->player = (StarsI16)raw[2];
    token->object_kind = raw[3];
    token->design = (StarsI16)raw[4];
    token->battle_coordinate = raw[5];
    token->init_base = raw[6];
    token->init_min = raw[7];
    token->init_max = raw[8];
    token->target_token = raw[9];
    token->pct_cloak = raw[10];
    token->pct_jam = raw[11];
    token->pct_battle_crit = raw[12];
    token->pct_capture = raw[13];
    token->pct_beam_def = raw[14];
    token->weight = cool_battle_get_word(raw, 15);
    token->dp_shield = cool_battle_get_word(raw, 17);
    token->count = cool_battle_get_word(raw, 19);
    token->defense_value = cool_battle_get_word(raw, 21);
    token->target_flags = cool_battle_get_word(raw, 23);
    token->movement_packed = cool_battle_get_word(raw, 25);
    token->warp = (StarsU8)((token->movement_packed >> 8) & 0x0fU);
    token->movement_flags = cool_battle_get_word(raw, 27);
    token->active = (StarsU16)(token->movement_flags & 1U);
}

static void cool_battle_store_packed_token(StarsU8 *raw,
    const StarsCoolBattleToken *token)
{
    StarsU16 movement_packed;
    StarsU16 movement_flags;

    cool_battle_put_word(raw, 0, (StarsU16)token->id);
    raw[2] = (StarsU8)token->player;
    raw[3] = token->object_kind;
    raw[4] = (StarsU8)token->design;
    raw[5] = token->battle_coordinate;
    raw[6] = token->init_base;
    raw[7] = token->init_min;
    raw[8] = token->init_max;
    raw[9] = token->target_token;
    raw[10] = token->pct_cloak;
    raw[11] = token->pct_jam;
    raw[12] = token->pct_battle_crit;
    raw[13] = token->pct_capture;
    raw[14] = token->pct_beam_def;
    cool_battle_put_word(raw, 15, token->weight);
    cool_battle_put_word(raw, 17, token->dp_shield);
    cool_battle_put_word(raw, 19, token->count);
    cool_battle_put_word(raw, 21, token->defense_value);
    cool_battle_put_word(raw, 23, token->target_flags);
    movement_packed = (StarsU16)(token->movement_packed & 0xf0ffU);
    movement_packed = (StarsU16)(movement_packed |
        (StarsU16)((token->warp & 0x0fU) << 8));
    cool_battle_put_word(raw, 25, movement_packed);
    movement_flags = (StarsU16)(token->movement_flags & (StarsU16)~1U);
    if (token->active)
        movement_flags = (StarsU16)(movement_flags | 1U);
    cool_battle_put_word(raw, 27, movement_flags);
}

static StarsU8 *cool_battle_work_buffer(StarsCoolBattleInput *input)
{
    return input->log_battle_buffer ? input->log_battle_buffer :
        input->log_buffer;
}

static StarsU16 cool_battle_work_capacity(const StarsCoolBattleInput *input)
{
    return input->log_battle_buffer ? input->log_battle_capacity :
        input->log_capacity;
}

static StarsI16 cool_battle_write_header_buffer(StarsCoolBattleInput *input,
    const StarsCoolBattleHeader *header)
{
    StarsU8 *buffer;
    StarsU16 capacity;
    StarsU16 start;

    buffer = cool_battle_work_buffer(input);
    capacity = cool_battle_work_capacity(input);
    if (!buffer)
        return 1;
    start = input->log_battle_start;
    if (start > capacity ||
            (StarsU16)(capacity - start) <
                STARS_COOL_BATTLE_HEADER_BYTES)
        return 0;
    cool_battle_put_word(buffer, start,
        (StarsU16)header->battle_id);
    buffer[start + 2U] = (StarsU8)header->player_count;
    buffer[start + 3U] = (StarsU8)header->token_count;
    cool_battle_put_word(buffer, (StarsU16)(start + 4U),
        header->player_mask);
    cool_battle_put_word(buffer, (StarsU16)(start + 6U), 0U);
    cool_battle_put_word(buffer, (StarsU16)(start + 8U),
        (StarsU16)header->planet_id);
    cool_battle_put_word(buffer, (StarsU16)(start + 10U),
        (StarsU16)header->point.y);
    cool_battle_put_word(buffer, (StarsU16)(start + 12U),
        (StarsU16)header->point.x);
    input->log_used = (StarsU16)(start + STARS_COOL_BATTLE_HEADER_BYTES);
    return 1;
}

static StarsI16 cool_battle_write_event(StarsCoolBattleInput *input,
    StarsI16 kind, StarsI16 round, StarsI16 initiative, StarsI16 token_index,
    const StarsCoolBattleToken *token)
{
    StarsCoolBattleEvent event;
    StarsU8 *buffer;
    StarsU16 capacity;
    StarsU16 packed;

    buffer = cool_battle_work_buffer(input);
    capacity = cool_battle_work_capacity(input);
    if (buffer && token) {
        if (input->log_used > capacity ||
                (StarsU16)(capacity - input->log_used) <
                    STARS_COOL_BATTLE_RECORD_BYTES)
            return 0;
        buffer[input->log_used] = (StarsU8)token_index;
        buffer[input->log_used + 1U] = token->battle_coordinate;
        buffer[input->log_used + 2U] = 0U;
        buffer[input->log_used + 3U] = 0U;
        packed = (StarsU16)(round & 0x0f);
        packed = (StarsU16)(packed |
            (StarsU16)(((token->movement_flags >> 5) & 0x0fU) << 4));
        packed = (StarsU16)(packed |
            (StarsU16)(((StarsU16)token_index & 0xffU) << 8));
        cool_battle_put_word(buffer,
            (StarsU16)(input->log_used + 4U), packed);
        input->log_used = (StarsU16)(input->log_used +
            STARS_COOL_BATTLE_RECORD_BYTES);
    }
    if (!input->ops.write_event)
        return 1;
    event.kind = kind;
    event.round = round;
    event.initiative = initiative;
    event.token_index = token_index;
    event.battle_coordinate = token ? token->battle_coordinate : 0xffU;
    event.movement_state = token ? (StarsU8)((token->movement_flags >> 5) &
        0x0fU) : 0U;
    return input->ops.write_event(input->ops.context, &event);
}

static StarsU16 cool_battle_filter_attackers(StarsU16 surviving,
    const StarsU16 *attack_masks, StarsI16 player_count)
{
    StarsI16 player;

    if (!attack_masks)
        return surviving;
    if (player_count > 16)
        player_count = 16;
    for (player = 0; player < player_count; ++player) {
        if ((surviving & (StarsU16)(1U << player)) != 0U &&
                (surviving & attack_masks[player]) == 0U)
            surviving = (StarsU16)(surviving &
                (StarsU16)~(StarsU16)(1U << player));
    }
    return surviving;
}

static StarsI16 cool_battle_move_token(StarsCoolBattleInput *input,
    StarsI16 round, StarsI16 index)
{
    StarsCoolBattleToken *token;
    StarsDxyMoveToken dxy_token;
    StarsDxyMoveInput dxy_input;
    StarsU16 attack_mask;

    token = &input->tokens[index];
    if (input->ops.move_token)
        return input->ops.move_token(input->ops.context, round, token);
    if (!input->ops.move_range || !input->ops.move_score)
        return 1;
    dxy_token.battle_coordinate = token->battle_coordinate;
    dxy_token.object_kind = token->object_kind;
    dxy_token.movement_flags = (StarsU8)token->movement_flags;
    attack_mask = 0U;
    if (input->attack_masks && token->player >= 0 && token->player < 16)
        attack_mask = input->attack_masks[token->player];
    dxy_input.token = &dxy_token;
    dxy_input.speed = stars_dxy_from_spd_round(token->warp, round);
    dxy_input.attack_mask = attack_mask;
    dxy_input.move_range = input->ops.move_range;
    dxy_input.score_damage = input->ops.move_score;
    dxy_input.random = input->ops.move_random;
    dxy_input.same_square = input->ops.move_same_square;
    dxy_input.context = input->ops.context;
    if (!stars_dxy_move_tok_to(&dxy_input))
        return 0;
    token->battle_coordinate = dxy_token.battle_coordinate;
    token->movement_flags = (StarsU16)dxy_token.movement_flags;
    return 1;
}

static void cool_battle_copy_board_token(StarsCoolBattleToken *destination,
    const StarsBoardToken *source)
{
    destination->id = source->id;
    destination->player = source->player;
    destination->design = source->design;
    destination->active = (StarsU16)(source->flags & 1U);
    destination->count = source->count;
    destination->object_kind = source->object_kind;
    destination->battle_coordinate = source->battle_coordinate;
    destination->target_token = 0U;
    destination->pct_cloak = 0U;
    destination->pct_jam = 0U;
    destination->pct_battle_crit = 0U;
    destination->pct_capture = 0U;
    destination->pct_beam_def = 0U;
    destination->weight = 0U;
    destination->defense_value = 0U;
    destination->movement_packed = (StarsU16)((source->warp & 0x0fU) << 8);
    destination->movement_flags = source->flags;
    destination->init_min = source->init_min;
    destination->init_max = source->init_max;
    destination->init_base = source->init_base;
    destination->warp = source->warp;
    destination->dp_shield = source->dp_shield;
    destination->target_flags = source->target_flags;
}

static StarsI16 cool_battle_load_tokens(StarsCoolBattleInput *input,
    StarsI16 token_count)
{
    StarsI16 index;
    StarsU8 *raw;

    if (input->packed_tokens) {
        if (input->packed_token_stride < STARS_COOL_BATTLE_TOKEN_BYTES ||
                input->packed_token_capacity < (StarsU16)token_count)
            return 0;
        for (index = 0; index < token_count; ++index) {
            raw = input->packed_tokens +
                index * input->packed_token_stride;
            cool_battle_load_packed_token(raw, &input->tokens[index]);
        }
        return 1;
    }
    if (!input->ops.load_token)
        return 1;
    if (!input->tokens)
        return 0;
    for (index = 0; index < token_count; ++index)
        if (!input->ops.load_token(input->ops.context, index,
                &input->tokens[index]))
            return 0;
    return 1;
}

static StarsI16 cool_battle_store_tokens(StarsCoolBattleInput *input,
    StarsI16 token_count)
{
    StarsI16 index;
    StarsU8 *raw;

    if (input->packed_tokens) {
        if (input->packed_token_stride < STARS_COOL_BATTLE_TOKEN_BYTES ||
                input->packed_token_capacity < (StarsU16)token_count)
            return 0;
        for (index = 0; index < token_count; ++index) {
            raw = input->packed_tokens +
                index * input->packed_token_stride;
            cool_battle_store_packed_token(raw, &input->tokens[index]);
        }
        return 1;
    }
    if (!input->ops.store_token)
        return 1;
    if (!input->tokens)
        return 0;
    for (index = 0; index < token_count; ++index)
        if (!input->ops.store_token(input->ops.context, index,
                &input->tokens[index]))
            return 0;
    return 1;
}

static StarsI16 cool_battle_initialize_board(StarsCoolBattleInput *input,
    StarsI16 *tokens, StarsI16 *rounds, StarsI16 *initiative_min,
    StarsI16 *initiative_max)
{
    StarsI16 index;
    StarsInitializeBoardInput *board;

    board = input->board;
    if (!board || !input->tokens || !board->tokens)
        return 0;
    board->active_players = input->player_mask;
    board->player_count = input->player_count;
    board->battle_coordinate = (StarsI16)(((input->player_count - 1) *
        input->player_count) / 2);
    stars_initialize_board(board);
    if (board->token_count < 0 ||
            board->token_count > input->token_capacity)
        return 0;
    for (index = 0; index < board->token_count; ++index)
        cool_battle_copy_board_token(&input->tokens[index],
            &board->tokens[index]);
    *tokens = board->token_count;
    *rounds = 0;
    if (board->initiative_min)
        *initiative_min = *board->initiative_min;
    if (board->initiative_max)
        *initiative_max = *board->initiative_max;
    return 1;
}

static StarsI16 cool_battle_attack(StarsCoolBattleInput *input,
    StarsI16 round, StarsI16 tokens, StarsU16 surviving)
{
    StarsI16 index;
    StarsI16 initiative;
    const StarsCoolBattleOps *ops;

    ops = &input->ops;
    if (ops->attack_initiative || ops->attack_token) {
        initiative = input->initiative_max;
        if (initiative > 255)
            initiative = 255;
        while (initiative >= input->initiative_min) {
            if (!ops->initiative_available ||
                    ops->initiative_available(ops->context, initiative)) {
                if (ops->attack_token && input->tokens) {
                    for (index = (StarsI16)(tokens - 1); index >= 0; --index) {
                        /* FDoCoolBattle's inner loop rebuilds this mask before
                         * each candidate.  FAttack can deactivate the token
                         * just visited, so the remaining candidates must not
                         * get a turn once only one player remains. */
                        if (cool_battle_player_count((StarsU16)(surviving &
                                cool_battle_token_players(input->tokens,
                                    tokens))) <= 1)
                            break;
                        if (input->tokens[index].active &&
                                input->tokens[index].init_min <= initiative &&
                                initiative <= input->tokens[index].init_max &&
                                (!cool_battle_write_event(input,
                                    STARS_COOL_BATTLE_EVENT_ATTACK, round,
                                    initiative, index,
                                    &input->tokens[index]) ||
                                !ops->attack_token(ops->context, round,
                                    initiative, index, &input->tokens[index],
                                    surviving)))
                            return 0;
                    }
                } else if (ops->attack_initiative &&
                        (!cool_battle_write_event(input,
                            STARS_COOL_BATTLE_EVENT_ATTACK, round,
                            initiative, -1, (const StarsCoolBattleToken *)0) ||
                        !ops->attack_initiative(ops->context, round,
                            initiative, tokens, surviving)))
                    return 0;
            }
            --initiative;
        }
        return 1;
    }
    if (ops->attack_tokens &&
            (!cool_battle_write_event(input, STARS_COOL_BATTLE_EVENT_ATTACK,
                round, -1, -1, (const StarsCoolBattleToken *)0) ||
            !ops->attack_tokens(ops->context, round, tokens)))
        return 0;
    return 1;
}

StarsI16 stars_f_do_cool_battle(StarsCoolBattleInput *input)
{
    StarsI16 i;
    StarsI16 round;
    StarsI16 tokens;
    StarsI16 rounds;
    StarsU16 surviving;
    StarsU16 active_players;
    StarsI16 active_ships;
    StarsI16 initiative_min;
    StarsI16 initiative_max;
    StarsI16 starbase_owner;
    StarsI16 token_store_failed;
    StarsU16 serialized_bytes;
    StarsCoolBattleHeader header;
    StarsCoolBattleReportSummary report_summary;
    const StarsCoolBattleOps *ops;

    if (!input || !input->fleets || input->fleet_count < 0 ||
            input->player_count < 0 ||
            (!input->ops.initialize && !input->board))
        return -1;
    input->log_overflow = 0;
    serialized_bytes = 0U;
    input->log_append_start = input->log_buffer ? input->log_used : 0U;
    if (input->log_battle_buffer) {
        input->log_battle_start = 0U;
        input->log_used = 0U;
    } else {
        input->log_battle_start = input->log_buffer ? input->log_used : 0U;
    }
    ops = &input->ops;
    if (ops->allocate && !ops->allocate(ops->context,
            STARS_COOL_BATTLE_LOG_BYTES, STARS_COOL_BATTLE_ALLOC_TYPE))
        return -1;
    tokens = 0;
    rounds = 0;
    token_store_failed = 0;
    initiative_min = input->initiative_min;
    initiative_max = input->initiative_max;
    if (ops->initialize) {
        i = ops->initialize(ops->context, input->fleets,
            input->fleet_count, input->player_count, input->player_mask,
            input->tokens, input->token_capacity, &tokens, &rounds,
            &initiative_min, &initiative_max);
    } else {
        i = cool_battle_initialize_board(input, &tokens, &rounds,
            &initiative_min, &initiative_max);
    }
    if (!i) {
        if (ops->release)
            ops->release(ops->context);
        return -1;
    }
    if (input->tokens && (input->token_capacity < 0 ||
            tokens < 0 || tokens > input->token_capacity)) {
        if (ops->release)
            ops->release(ops->context);
        return -1;
    }
    if (!cool_battle_load_tokens(input, tokens)) {
        if (ops->release)
            ops->release(ops->context);
        return -1;
    }
    header.battle_id = input->battle_id;
    header.player_count = input->player_count;
    header.token_count = tokens;
    header.planet_id = input->fleets[0].planet_id;
    header.point = input->fleets[0].point;
    header.player_mask = input->player_mask;
    header.record_bytes = 0U;
    ++input->battle_id;
    if (!cool_battle_write_header_buffer(input, &header) ||
            (ops->write_header && !ops->write_header(ops->context, &header))) {
        if (ops->release)
            ops->release(ops->context);
        return -1;
    }
    cool_battle_collect_participants(input->fleets, input->fleet_count,
        &active_players, &active_ships);
    if (input->tokens)
        active_players = cool_battle_token_players(input->tokens, tokens);
    starbase_owner = -1;
    if (ops->defending_starbase && input->fleet_count > 0)
        starbase_owner = ops->defending_starbase(ops->context,
            input->fleets[0].planet_id, input->player_mask);
    if (starbase_owner >= 0 && starbase_owner < 16 &&
            (input->player_mask & (StarsU16)(1U << starbase_owner)) != 0U) {
        active_players = (StarsU16)(active_players |
            (StarsU16)(1U << starbase_owner));
        ++active_ships;
    }
    input->active_player_mask = active_players;
    input->active_ship_count = active_ships;
    input->initiative_min = initiative_min;
    input->initiative_max = initiative_max;
    if (active_ships != 0)
        surviving = (StarsU16)(input->player_mask & active_players);
    else
        surviving = input->player_mask;
    for (round = 0; round < 16 && cool_battle_player_count(surviving) > 1;
            ++round) {
        if (input->tokens)
            surviving = (StarsU16)(input->player_mask &
                cool_battle_token_players(input->tokens, tokens));
        else
            surviving = cool_battle_filter_attackers(surviving,
                input->attack_masks, input->player_count);
        if (cool_battle_player_count(surviving) <= 1)
            break;
        if (input->tokens && ops->regen_token) {
            for (i = 0; i < tokens; ++i) {
                if (round != 0 && input->tokens[i].active &&
                        input->tokens[i].dp_shield != 0 &&
                        !ops->regen_token(ops->context, round,
                            &input->tokens[i]))
                    break;
            }
            if (i < tokens) break;
        } else if (ops->regen_shields && round != 0 &&
                !ops->regen_shields(ops->context, round, tokens))
            break;
        if (input->tokens && (ops->move_token || ops->move_range)) {
            for (i = 0; i < tokens; ++i) {
                if (!input->tokens[i].active)
                    continue;
                if (!cool_battle_move_token(input, round, i))
                    break;
                if (!cool_battle_store_tokens(input, tokens)) {
                    token_store_failed = 1;
                    break;
                }
                if (!cool_battle_write_event(input,
                        STARS_COOL_BATTLE_EVENT_MOVE, round, -1, i,
                        &input->tokens[i]))
                    break;
            }
            if (i < tokens) break;
        } else if (ops->move_tokens &&
                (!cool_battle_write_event(input, STARS_COOL_BATTLE_EVENT_MOVE,
                    round, -1, -1, (const StarsCoolBattleToken *)0) ||
                !ops->move_tokens(ops->context, round, tokens)))
            break;
        if (input->tokens) {
            surviving = (StarsU16)(input->player_mask &
                cool_battle_token_players(input->tokens, tokens));
            surviving = cool_battle_filter_attackers(surviving,
                input->attack_masks, input->player_count);
            if (cool_battle_player_count(surviving) <= 1)
                break;
        }
        if (!cool_battle_attack(input, round, tokens, surviving))
            break;
        if (!cool_battle_store_tokens(input, tokens)) {
            token_store_failed = 1;
            break;
        }
        if (input->tokens) {
            surviving = (StarsU16)(input->player_mask &
                cool_battle_token_players(input->tokens, tokens));
            if (cool_battle_player_count(surviving) <= 1)
                break;
        }
        if (ops->advance_round)
            surviving = ops->advance_round(ops->context, round, surviving);
        if (surviving == 0)
            break;
    }
    input->token_count = tokens;
    input->round_count = round;
    if (!cool_battle_store_tokens(input, tokens))
        token_store_failed = 1;
    input->surviving_players = surviving;
    if (cool_battle_work_buffer(input)) {
        StarsU8 *work_buffer;

        work_buffer = cool_battle_work_buffer(input);
        serialized_bytes = (StarsU16)(input->log_used -
            input->log_battle_start);
        cool_battle_put_word(work_buffer,
            (StarsU16)(input->log_battle_start + 6U), serialized_bytes);
        if (input->log_battle_buffer && input->log_buffer) {
            if (input->log_append_start <= input->log_capacity &&
                    (StarsU16)(input->log_capacity -
                        input->log_append_start) >= serialized_bytes) {
                memmove(input->log_buffer + input->log_append_start,
                    work_buffer + input->log_battle_start, serialized_bytes);
                input->log_used = (StarsU16)(input->log_append_start +
                    serialized_bytes);
            } else {
                if (input->log_append_start <= input->log_capacity &&
                        (StarsU16)(input->log_capacity -
                            input->log_append_start) >= 2U)
                    cool_battle_put_word(input->log_buffer,
                        input->log_append_start, 0xffffU);
                input->log_used = input->log_append_start;
                input->log_overflow = 1;
            }
        }
    }
    i = token_store_failed ? 0 : 1;
    report_summary.battle_id = header.battle_id;
    report_summary.player_count = input->player_count;
    report_summary.player_mask = input->player_mask;
    report_summary.spectator_mask = input->spectator_mask;
    report_summary.active_ship_count = input->active_ship_count;
    report_summary.active_player_mask = input->active_player_mask;
    report_summary.round_count = round;
    report_summary.record_bytes = serialized_bytes;
    if (ops->send_messages_ex) {
        if (!ops->send_messages_ex(ops->context, &report_summary))
            i = 0;
    } else if (ops->send_messages &&
            !ops->send_messages(ops->context, round)) {
        i = 0;
    }
    if (ops->release)
        ops->release(ops->context);
    return i;
}
