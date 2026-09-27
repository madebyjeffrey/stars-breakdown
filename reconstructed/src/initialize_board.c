#include "core.h"
#include <string.h>

#define BOARD_FLEET_SKIP 0x0400U
#define BOARD_FLEET_CARGO 0x0100U
#define BOARD_FLEET_MISSED 0x0080U
#define BOARD_PLANET_COMBAT_MARK 0x4000U
#define BOARD_TOKEN_STARBASE 1U
#define BOARD_TOKEN_FLEET 2U
#define BOARD_TOKEN_FLAG_ORDERED 0x0011U
#define BOARD_TOKEN_FLAG_SHIELD 0x0008U
#define BOARD_TOKEN_FLAG_NO_INIT 0x0001U
#define BOARD_TOKEN_WARP_MASK 0x000fU
#define BOARD_TOKEN_WARP_REDUCTION 4U
#define BOARD_PLAYER_FLAG_SHIELD 0x000dU
#define BOARD_MAX_TOKENS 255

static StarsI16 board_player_enabled(const StarsInitializeBoardInput *input,
    StarsI16 player)
{
    if (player < 0 || player >= input->player_count) return 0;
    return (StarsI16)((input->active_players &
        ((StarsU16)1U << (StarsU16)player)) != 0);
}

static StarsI16 board_compact_player(const StarsInitializeBoardInput *input,
    StarsI16 player)
{
    StarsI16 index;
    StarsI16 compact = 0;
    for (index = 0; index < player; ++index)
        if (board_player_enabled(input, index)) ++compact;
    return compact;
}

static void board_prepare_token(StarsInitializeBoardInput *input,
    StarsBoardToken *token, StarsI16 player, StarsU8 coordinate,
    StarsI16 design, StarsU16 count, StarsU8 kind, StarsI16 id)
{
    memset(token, 0, sizeof *token);
    token->player = player;
    token->battle_coordinate = coordinate;
    token->design = design;
    token->count = count;
    token->object_kind = kind;
    token->id = id;
    token->init_min = 0xffU;
    token->init_max = 0xffU;
    token->dp_shield = 0;
    if (input->check_initiative)
        input->check_initiative(input->context, token);
}

/* Genuine semantic reconstruction of InitializeBoard, NE 31:45B4-4CA8.
 * This coordinator turns eligible fleets and a defending starbase into the
 * ordered battle-token stream consumed by the combat engine.  It preserves
 * the original distinctions between skipped fleets, missed players, the
 * starbase-first token, cargo dumping, per-design ship tokens, initiative
 * preparation, shield-race flags, and initiative-range aggregation. */
void stars_initialize_board(StarsInitializeBoardInput *input)
{
    StarsI16 fleet_index;
    StarsI16 design;
    StarsI16 token_count = 0;
    StarsI16 dampening = 0;
    StarsI16 compact_player;
    StarsBoardPlanet *planet;
    StarsBoardFleet *fleet;
    StarsBoardToken *token;
    StarsU16 shield_flag;
    StarsI16 min_init = -1;
    StarsI16 max_init = -1;

    if (!input || !input->fleets || !input->tokens) return;
    input->token_count = 0;
    input->missed_players = 0;
    if (input->initiative) memset(input->initiative, 0, 64);
    for (fleet_index = 0; fleet_index < input->fleet_count; ++fleet_index) {
        fleet = &input->fleets[fleet_index];
        if (fleet->planet_id == -1 || !input->find_planet) continue;
        planet = input->find_planet(input->context, fleet->planet_id);
        if (!planet || !planet->is_starbase ||
                !board_player_enabled(input, planet->owner)) continue;
        planet->flags = (StarsU16)((planet->flags & (StarsU16)~BOARD_PLANET_COMBAT_MARK) |
            BOARD_PLANET_COMBAT_MARK);
        if (token_count >= input->token_capacity || token_count >= BOARD_MAX_TOKENS)
            break;
        token = &input->tokens[token_count++];
        compact_player = board_compact_player(input, planet->owner);
        board_prepare_token(input, token, planet->owner,
            (StarsU8)(compact_player + input->battle_coordinate),
            (StarsI16)(planet->design_nibble + 16), 1, BOARD_TOKEN_STARBASE,
            planet->id);
        if (input->check_weapons)
            input->check_weapons(input->context, token, &dampening,
                input->initiative);
        if (token->init_base == 0xffU)
            token->flags = (StarsU16)((token->flags & 0x0fffU) | 0x5000U);
        else token->flags = (StarsU16)((token->flags & 0x0fffU) | 0x3000U);
        token->flags = (StarsU16)((token->flags & 0xfff0U) | 1U);
        token->flags = (StarsU16)((token->flags & 0xff0fU) | 0x10U);
        token->flags = (StarsU16)((token->flags & 0xf0ffU) | 0x0500U);
        token->warp = (StarsU8)(planet->flags >> 4);
    }
    for (fleet_index = 0; fleet_index < input->fleet_count; ++fleet_index) {
        fleet = &input->fleets[fleet_index];
        if (fleet->flags & BOARD_FLEET_SKIP) continue;
        if (fleet->flags & BOARD_FLEET_MISSED) {
            if (fleet->player >= 0 && fleet->player < 16)
                input->missed_players |= (StarsU16)1U << (StarsU16)fleet->player;
            continue;
        }
        if (!(fleet->flags & BOARD_FLEET_CARGO)) continue;
        fleet->flags = (StarsU16)((fleet->flags & (StarsU16)~BOARD_PLANET_COMBAT_MARK) |
            BOARD_PLANET_COMBAT_MARK);
        if (input->dump_cargo) input->dump_cargo(input->context, fleet);
        compact_player = board_compact_player(input, fleet->player);
        for (design = 0; design < 16; ++design) {
            if (!fleet->design_count[design]) continue;
            if (token_count >= input->token_capacity || token_count >= BOARD_MAX_TOKENS)
                break;
            token = &input->tokens[token_count++];
            board_prepare_token(input, token, fleet->player,
                (StarsU8)(compact_player + input->battle_coordinate), design,
                fleet->design_count[design], BOARD_TOKEN_FLEET, fleet->id);
            if (input->check_weapons)
                input->check_weapons(input->context, token, &dampening,
                    input->initiative);
            if (input->check_target)
                input->check_target(input->context, token, fleet, design);
            if (input->speed)
                token->warp = input->speed(input->context, fleet, design,
                    token, input->dump_cargo != 0);
        }
    }
    if (input->randomize) input->randomize(input->context, input->tokens,
        token_count);
    for (fleet_index = 0; fleet_index < token_count; ++fleet_index) {
        token = &input->tokens[fleet_index];
        token->flags |= BOARD_TOKEN_FLAG_ORDERED;
        if (token->init_min == 0xffU)
            token->flags &= (StarsU16)~BOARD_TOKEN_FLAG_NO_INIT;
        shield_flag = input->race_flag ?
            (StarsU16)input->race_flag(input->context, token->player,
                BOARD_PLAYER_FLAG_SHIELD) : 0;
        if (token->dp_shield && shield_flag)
            token->flags |= BOARD_TOKEN_FLAG_SHIELD;
        if (dampening && token->object_kind != BOARD_TOKEN_STARBASE) {
            if (token->warp > BOARD_TOKEN_WARP_REDUCTION)
                token->warp = (StarsU8)(token->warp - BOARD_TOKEN_WARP_REDUCTION);
            else token->warp = 0;
        }
        if (input->record_token)
            input->record_token(input->context, token);
        if (token->init_min != 0xffU &&
                (min_init < 0 || token->init_min < (StarsU8)min_init))
            min_init = token->init_min;
        if (token->init_max != 0xffU &&
                (max_init < 0 || max_init < token->init_max))
            max_init = token->init_max;
    }
    if (input->initiative_min) *input->initiative_min =
        (StarsI16)(min_init < 0 ? 0xff : min_init);
    if (input->initiative_max) *input->initiative_max =
        (StarsI16)(max_init < 0 ? 0xff : max_init);
    input->token_count = token_count;
}
