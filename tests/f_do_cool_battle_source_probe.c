#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 allocated;
    StarsI16 initialized;
    StarsI16 regen;
    StarsI16 moved;
    StarsI16 attacked;
    StarsI16 advanced;
    StarsI16 reported;
    StarsI16 released;
    StarsI16 report_round;
    StarsI16 report_ex_calls;
    StarsCoolBattleReportSummary report_summary;
    StarsI16 initiative_calls;
    StarsI16 last_initiative;
    StarsI16 starbase_enabled;
    StarsI16 starbase_calls;
    StarsI16 header_calls;
    StarsI16 header_id;
    StarsI16 header_tokens;
    StarsI16 header_planet;
    StarsPoint header_point;
    StarsI16 event_calls;
    StarsI16 move_events;
    StarsI16 attack_events;
    StarsU8 last_event_coordinate;
    StarsU8 last_event_state;
    StarsI16 deactivate_on_attack;
    StarsI16 token_regen;
    StarsI16 token_move;
    StarsI16 token_attack;
    StarsU16 allocation_bytes;
    StarsI16 allocation_type;
} Trace;
static StarsI16 allocate(void *c, StarsU16 bytes, StarsI16 allocation_type)
{
    Trace *t = (Trace *)c;
    ++t->allocated;
    t->allocation_bytes = bytes;
    t->allocation_type = allocation_type;
    return 1;
}
static StarsI16 initialize(void *c, const StarsCoolBattleFleet *fleets,
    StarsI16 fleet_count, StarsI16 player_count, StarsU16 player_mask,
    StarsCoolBattleToken *token_buffer, StarsI16 token_capacity,
    StarsI16 *tokens, StarsI16 *rounds, StarsI16 *initiative_min,
    StarsI16 *initiative_max)
{
    Trace *t = (Trace *)c;
    (void)fleets;
    (void)player_count;
    (void)player_mask;
    if (token_buffer && token_capacity >= 4) {
        token_buffer[0].active = 1;
        token_buffer[0].player = 0;
        token_buffer[0].init_min = 2;
        token_buffer[0].init_max = 4;
        token_buffer[0].dp_shield = 1;
        token_buffer[1].active = 1;
        token_buffer[1].player = 1;
        token_buffer[1].init_min = 2;
        token_buffer[1].init_max = 4;
    }
    if (fleet_count != 2)
        return 0;
    *tokens = 4;
    *rounds = 1;
    *initiative_min = 2;
    *initiative_max = 4;
    ++t->initialized;
    return 1;
}
static StarsI16 round_step(void *c, StarsI16 round, StarsI16 tokens)
{
    (void)c;
    (void)round;
    (void)tokens;
    return 1;
}
static StarsI16 regen(void *c, StarsI16 round, StarsI16 tokens)
{
    Trace *t = (Trace *)c;
    (void)round;
    (void)tokens;
    ++t->regen;
    return round_step(c, round, tokens);
}
static StarsI16 move(void *c, StarsI16 round, StarsI16 tokens)
{
    Trace *t = (Trace *)c;
    (void)round;
    (void)tokens;
    ++t->moved;
    return 1;
}
static StarsI16 attack(void *c, StarsI16 round, StarsI16 tokens)
{
    Trace *t = (Trace *)c;
    (void)round;
    (void)tokens;
    ++t->attacked;
    return 1;
}
static StarsI16 regen_token(void *c, StarsI16 round,
    StarsCoolBattleToken *token)
{
    Trace *t = (Trace *)c;
    (void)round;
    (void)token;
    ++t->token_regen;
    return 1;
}
static StarsI16 move_token(void *c, StarsI16 round,
    StarsCoolBattleToken *token)
{
    Trace *t = (Trace *)c;
    (void)round;
    (void)token;
    ++t->token_move;
    return 1;
}
static StarsI16 move_range(void *c, StarsU16 attack_mask,
    StarsU8 *preferred)
{
    (void)c;
    (void)attack_mask;
    *preferred = 0xffU;
    return 1;
}
static StarsI32 move_score(void *c, StarsU16 coordinate, StarsI16 primary,
    StarsU16 attack_mask)
{
    (void)c;
    (void)primary;
    (void)attack_mask;
    return coordinate == 0x11U ? 0L : 10L;
}
static void board_check_initiative(void *c, StarsBoardToken *token)
{
    (void)c;
    token->init_base = 2U;
    token->init_min = 2U;
    token->init_max = 4U;
}
static StarsI16 attack_token(void *c, StarsI16 round, StarsI16 initiative,
    StarsI16 index, StarsCoolBattleToken *token, StarsU16 surviving)
{
    Trace *t = (Trace *)c;
    (void)round;
    (void)initiative;
    (void)index;
    (void)token;
    (void)surviving;
    ++t->token_attack;
    if (t->deactivate_on_attack && index == 1)
        token->active = 0;
    return 1;
}
static StarsI16 write_header(void *c, const StarsCoolBattleHeader *header)
{
    Trace *t = (Trace *)c;
    ++t->header_calls;
    t->header_id = header->battle_id;
    t->header_tokens = header->token_count;
    t->header_planet = header->planet_id;
    t->header_point = header->point;
    return 1;
}
static StarsI16 write_event(void *c, const StarsCoolBattleEvent *event)
{
    Trace *t = (Trace *)c;
    ++t->event_calls;
    if (event->kind == 1)
        ++t->move_events;
    if (event->kind == 2)
        ++t->attack_events;
    t->last_event_coordinate = event->battle_coordinate;
    t->last_event_state = event->movement_state;
    return 1;
}
static StarsI16 defending_starbase(void *c, StarsI16 planet_id,
    StarsU16 player_mask)
{
    Trace *t = (Trace *)c;
    ++t->starbase_calls;
    (void)player_mask;
    if (t->starbase_enabled && planet_id == 10)
        return 1;
    return -1;
}
static StarsI16 attack_initiative(void *c, StarsI16 round,
    StarsI16 initiative, StarsI16 tokens, StarsU16 surviving)
{
    Trace *t = (Trace *)c;
    (void)round;
    (void)tokens;
    (void)surviving;
    ++t->initiative_calls;
    t->last_initiative = initiative;
    return 1;
}
static StarsI16 initiative_available(void *c, StarsI16 initiative)
{
    (void)c;
    return initiative == 4 || initiative == 2;
}
static StarsU16 advance_round(void *c, StarsI16 round, StarsU16 surviving)
{
    Trace *t = (Trace *)c;
    (void)round;
    ++t->advanced;
    return surviving == 3 ? 1 : surviving;
}
static StarsI16 report(void *c, StarsI16 round)
{
    Trace *t = (Trace *)c;
    ++t->reported;
    t->report_round = round;
    return 1;
}
static StarsI16 report_ex(void *c,
    const StarsCoolBattleReportSummary *summary)
{
    Trace *t = (Trace *)c;
    ++t->report_ex_calls;
    t->report_summary = *summary;
    return 1;
}
static void release(void *c)
{
    Trace *t = (Trace *)c;
    ++t->released;
}
int main(void)
{
    StarsCoolBattleFleet fleets[2];
    StarsCoolBattleInput input;
    StarsU16 attack_masks[2];
    StarsCoolBattleToken tokens[4];
    StarsU8 log_buffer[70];
    StarsU8 log_staging[64];
    StarsBoardFleet board_fleets[2];
    StarsBoardToken board_tokens[2];
    StarsInitializeBoardInput board;
    StarsU8 board_initiative[64];
    StarsI16 board_min;
    StarsI16 board_max;
    Trace trace;
    memset(&fleets, 0, sizeof fleets);
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    memset(attack_masks, 0, sizeof attack_masks);
    memset(tokens, 0, sizeof tokens);
    memset(log_buffer, 0, sizeof log_buffer);
    memset(log_staging, 0, sizeof log_staging);
    memset(board_fleets, 0, sizeof board_fleets);
    memset(board_tokens, 0, sizeof board_tokens);
    memset(&board, 0, sizeof board);
    memset(board_initiative, 0, sizeof board_initiative);
    tokens[0].battle_coordinate = 0x21U;
    tokens[0].movement_flags = 0x00a0U;
    input.fleets = fleets;
    input.fleet_count = 2;
    input.player_count = 2;
    input.player_mask = 3;
    input.spectator_mask = 0x80;
    input.attack_masks = attack_masks;
    attack_masks[0] = 2;
    attack_masks[1] = 1;
    input.ops.allocate = allocate;
    input.ops.initialize = initialize;
    input.ops.regen_shields = regen;
    input.ops.move_tokens = move;
    input.ops.defending_starbase = defending_starbase;
    input.ops.attack_tokens = attack;
    input.ops.write_header = write_header;
    input.ops.write_event = write_event;
    input.ops.advance_round = advance_round;
    input.ops.send_messages = report;
    input.ops.send_messages_ex = report_ex;
    input.ops.release = release;
    input.ops.context = &trace;
    fleets[0].owner = 0;
    fleets[0].planet_id = 10;
    fleets[0].point.x = 12;
    fleets[0].point.y = 14;
    fleets[0].ship_count = 2;
    fleets[1].owner = 1;
    fleets[1].ship_count = 2;
    trace.starbase_enabled = 1;
    if (!stars_f_do_cool_battle(&input) || trace.allocated != 1 ||
            trace.initialized != 1 || trace.regen != 0 || trace.moved != 1 ||
            trace.attacked != 1 || trace.advanced != 1 || trace.reported != 0 ||
            trace.report_ex_calls != 1 ||
            trace.report_summary.battle_id != 0 ||
            trace.report_summary.player_count != 2 ||
            trace.report_summary.player_mask != 3 ||
            trace.report_summary.spectator_mask != 0x80 ||
            trace.report_summary.active_ship_count != 5 ||
            trace.report_summary.active_player_mask != 3 ||
            trace.report_summary.round_count != 1 ||
            trace.released != 1 || trace.report_round != 0 ||
            input.token_count != 4 || input.round_count != 1 ||
            input.surviving_players != 1 || input.active_player_mask != 3 ||
            input.active_ship_count != 5 || trace.starbase_calls != 1 ||
            trace.header_calls != 1 || trace.header_id != 0 ||
            trace.header_tokens != 4 || trace.header_planet != 10 ||
            trace.header_point.x != 12 || trace.header_point.y != 14 ||
            trace.event_calls != 2 || trace.move_events != 1 ||
            trace.attack_events != 1 ||
            trace.last_event_coordinate != 0xffU ||
            trace.last_event_state != 0U ||
            trace.allocation_bytes != STARS_COOL_BATTLE_LOG_BYTES ||
            trace.allocation_type != STARS_COOL_BATTLE_ALLOC_TYPE)
        return 1;
    input.ops.send_messages_ex = 0;
    memset(&trace, 0, sizeof trace);
    input.player_mask = 3;
    input.token_count = 0;
    input.round_count = 0;
    input.surviving_players = 0;
    attack_masks[0] = 0;
    attack_masks[1] = 2;
    if (!stars_f_do_cool_battle(&input) || trace.regen != 0 ||
            trace.moved != 0 || trace.attacked != 0 || trace.advanced != 0 ||
            trace.report_round != 0 || input.surviving_players != 2 ||
            trace.reported != 1 || trace.released != 1)
        return 1;
    memset(&trace, 0, sizeof trace);
    input.player_mask = 3;
    input.token_count = 0;
    input.round_count = 0;
    input.surviving_players = 0;
    input.initiative_min = 0;
    input.initiative_max = 0;
    attack_masks[0] = 2;
    attack_masks[1] = 1;
    input.ops.attack_initiative = attack_initiative;
    input.ops.initiative_available = initiative_available;
    if (!stars_f_do_cool_battle(&input) || trace.initiative_calls != 2 ||
            trace.last_initiative != 2 || trace.attacked != 0 ||
            trace.advanced != 1 || input.round_count != 1)
        return 1;
    memset(&trace, 0, sizeof trace);
    fleets[1].active = 1;
    input.player_mask = 3;
    attack_masks[0] = 0;
    attack_masks[1] = 0;
    input.ops.attack_initiative = 0;
    if (!stars_f_do_cool_battle(&input) || input.active_player_mask != 1 ||
            input.active_ship_count != 2 || input.surviving_players != 1 ||
            trace.regen != 0 || trace.reported != 1)
        return 1;
    memset(&trace, 0, sizeof trace);
    memset(tokens, 0, sizeof tokens);
    tokens[0].battle_coordinate = 0x21U;
    tokens[0].movement_flags = 0x00a0U;
    input.player_mask = 3;
    input.tokens = tokens;
    input.token_capacity = 4;
    input.ops.regen_token = regen_token;
    input.ops.move_token = move_token;
    input.ops.attack_token = attack_token;
    input.ops.attack_initiative = 0;
    input.ops.initiative_available = initiative_available;
    input.battle_id = 40;
    input.log_buffer = log_buffer;
    input.log_capacity = sizeof log_buffer;
    input.log_battle_buffer = log_staging;
    input.log_battle_capacity = sizeof log_staging;
    fleets[1].active = 0;
    fleets[0].active = 1;
    attack_masks[0] = 2;
    attack_masks[1] = 1;
    if (!stars_f_do_cool_battle(&input) || trace.token_regen != 0 ||
            trace.token_move != 2 || trace.token_attack != 4 ||
            input.active_player_mask != 3 ||
            trace.last_event_coordinate != 0x21U ||
            trace.last_event_state != 5U || input.log_used != 50U ||
            log_buffer[0] != 40U || log_buffer[1] != 0U ||
            log_buffer[2] != 2U || log_buffer[3] != 4U ||
            log_buffer[4] != 3U || log_buffer[5] != 0U ||
            log_buffer[6] != 50U || log_buffer[7] != 0U ||
            log_buffer[8] != 10U || log_buffer[9] != 0U ||
            log_buffer[10] != 14U || log_buffer[11] != 0U ||
            log_buffer[12] != 12U || log_buffer[13] != 0U ||
            log_buffer[14] != 0U || log_buffer[15] != 0x21U ||
            log_buffer[18] != 0x50U || log_buffer[19] != 0U ||
            log_buffer[20] != 1U || log_buffer[26] != 1U ||
            log_buffer[32] != 0U || log_buffer[33] != 0x21U ||
            log_buffer[48] != 0x50U || log_buffer[49] != 0U)
        return 1;
    memset(&trace, 0, sizeof trace);
    input.ops.send_messages_ex = report_ex;
    input.battle_id = 43;
    input.log_used = 5U;
    if (!stars_f_do_cool_battle(&input))
        return 1;
    if (input.log_used != 55U || trace.report_ex_calls != 1 ||
            trace.report_summary.record_bytes != 50U ||
            log_buffer[0] != 40U || log_buffer[1] != 0U ||
            log_buffer[2] != 2U || log_buffer[3] != 4U ||
            log_buffer[4] != 3U || log_buffer[5] != 43U ||
            log_buffer[6] != 0U || log_buffer[7] != 2U ||
            log_buffer[8] != 4U || log_buffer[9] != 3U ||
            log_buffer[10] != 0U || log_buffer[11] != 50U ||
            log_buffer[12] != 0U)
        return 1;
    input.ops.send_messages_ex = 0;
    memset(&trace, 0, sizeof trace);
    input.tokens = 0;
    input.battle_id = 41;
    input.log_used = 50U;
    input.log_capacity = sizeof log_buffer;
    if (!stars_f_do_cool_battle(&input) || input.log_used != 64U ||
            log_buffer[50] != 41U || log_buffer[51] != 0U ||
            log_buffer[52] != 2U || log_buffer[53] != 4U ||
            log_buffer[54] != 3U || log_buffer[55] != 0U ||
            log_buffer[56] != 14U || log_buffer[57] != 0U)
        return 1;
    input.battle_id = 42;
    if (!stars_f_do_cool_battle(&input) || input.log_used != 64U ||
            input.log_overflow != 1 || log_buffer[64] != 0xffU ||
            log_buffer[65] != 0xffU)
        return 1;
    input.tokens = tokens;
    input.log_buffer = 0;
    input.log_capacity = 0;
    input.log_battle_buffer = 0;
    input.log_battle_capacity = 0;
    memset(&trace, 0, sizeof trace);
    memset(tokens, 0, sizeof tokens);
    tokens[0].battle_coordinate = 0x00U;
    tokens[0].object_kind = 2U;
    tokens[0].warp = 8U;
    tokens[1].battle_coordinate = 0x33U;
    tokens[1].object_kind = 1U;
    input.ops.move_token = 0;
    input.ops.move_range = move_range;
    input.ops.move_score = move_score;
    if (!stars_f_do_cool_battle(&input) ||
            tokens[0].battle_coordinate != 0x11U ||
            (tokens[0].movement_flags & 0x10U) == 0U ||
            tokens[1].battle_coordinate != 0x33U)
        return 1;
    input.ops.move_token = move_token;
    input.ops.move_range = 0;
    input.ops.move_score = 0;
    memset(&trace, 0, sizeof trace);
    board_fleets[0].player = 0;
    board_fleets[0].id = 20;
    board_fleets[0].planet_id = -1;
    board_fleets[0].flags = 0x0100U;
    board_fleets[0].design_count[0] = 1U;
    board_fleets[1].player = 1;
    board_fleets[1].id = 21;
    board_fleets[1].planet_id = -1;
    board_fleets[1].flags = 0x0100U;
    board_fleets[1].design_count[0] = 1U;
    board.fleets = board_fleets;
    board.fleet_count = 2;
    board.tokens = board_tokens;
    board.token_capacity = 2;
    board.initiative = board_initiative;
    board.initiative_min = &board_min;
    board.initiative_max = &board_max;
    board.check_initiative = board_check_initiative;
    input.ops.initialize = 0;
    input.board = &board;
    input.tokens = tokens;
    input.token_capacity = 2;
    input.log_buffer = 0;
    input.log_battle_buffer = 0;
    fleets[0].active = 0;
    fleets[1].active = 0;
    input.player_mask = 3;
    attack_masks[0] = 2;
    attack_masks[1] = 1;
    if (!stars_f_do_cool_battle(&input) || board.token_count != 2 ||
            input.token_count != 2 || input.active_player_mask != 3 ||
            tokens[0].id != 20 || tokens[0].design != 0 ||
            tokens[0].init_base != 2U || tokens[0].target_flags != 0U ||
            tokens[1].id != 21 ||
            trace.token_move != 2 || trace.token_attack != 4 ||
            input.surviving_players != 1)
        return 1;
    input.ops.initialize = initialize;
    input.board = 0;
    memset(&trace, 0, sizeof trace);
    memset(tokens, 0, sizeof tokens);
    input.token_capacity = 4;
    trace.deactivate_on_attack = 1;
    if (!stars_f_do_cool_battle(&input) || trace.token_attack != 1 ||
            trace.advanced != 0 || input.surviving_players != 1)
        return 1;
    memset(&trace, 0, sizeof trace);
    input.token_capacity = 3;
    if (stars_f_do_cool_battle(&input) != -1 || trace.released != 1)
        return 1;
    puts("F_DO_COOL_BATTLE_CHECKS=58 FAILURES=0 RESULT=PASS");
    return 0;
}
