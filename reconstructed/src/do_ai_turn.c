#include "core.h"
#include <string.h>

#define AI_TURN_ALLOC_TAG 6U
#define AI_TURN_DATA_BYTES 0x2000UL
#define AI_TURN_DATA_VERSION 2U
#define AI_TURN_PERSONALITY_SHIFT 13
#define AI_TURN_NO_PERSONALITY 0xffffU

static void ai_turn_free(StarsDoAiTurnInput *input, void *planet_table,
    void *planet_index, void *ai_data)
{
    if (input->free_memory) {
        if (planet_index) input->free_memory(input->context, planet_index,
            AI_TURN_ALLOC_TAG);
        if (planet_table) input->free_memory(input->context, planet_table,
            AI_TURN_ALLOC_TAG);
        if (ai_data) input->free_memory(input->context, ai_data,
            AI_TURN_ALLOC_TAG);
    }
}

/* Genuine semantic reconstruction of DoAiTurn, NE 18:0000-02EE.
 * It marks the AI phase, reloads the requested player's game state, rejects
 * dead players, allocates the temporary planet/index/data tables, runs the
 * shared design/attack/minefield/random-list preparation, dispatches the
 * selected personality, writes the log and history records, and releases all
 * temporary tables on both success and allocation failure. */
void stars_do_ai_turn(StarsI16 player, StarsU16 requested_personality,
    StarsDoAiTurnInput *input)
{
    void *planet_table = (void *)0;
    void *planet_index = (void *)0;
    void *ai_data = (void *)0;
    StarsU16 personality;
    StarsI16 loaded;
    StarsI16 dead = 0;
    StarsI16 saved_player;
    StarsU16 i;
    if (!input || !input->players || player < 0 ||
            (StarsU16)player >= input->player_count) return;
    saved_player = input->current_player ? *input->current_player : 0;
    input->ai_active = 1;
    if (input->destroy_game) input->destroy_game(input->context);
    loaded = input->load_game ? input->load_game(input->context, player,
        requested_personality) : 0;
    if (!loaded) {
        input->ai_active = 0;
        if (input->current_player) *input->current_player = saved_player;
        return;
    }
    dead = input->players[player].dead;
    if (!dead && input->allocate) {
        planet_table = input->allocate(input->context,
            (StarsU32)input->planet_capacity * 16UL, AI_TURN_ALLOC_TAG);
        planet_index = input->allocate(input->context,
            (StarsU32)input->planet_capacity * 4UL, AI_TURN_ALLOC_TAG);
        ai_data = input->allocate(input->context, AI_TURN_DATA_BYTES,
            AI_TURN_ALLOC_TAG);
        if (planet_table && planet_index && ai_data) {
            /* Original clears AI planet records before any preparation. */
            memset(planet_table, 0, (size_t)input->planet_capacity * 16U);
            ((StarsU8 *)ai_data)[0] = AI_TURN_DATA_VERSION;
            ((StarsU8 *)ai_data)[1] = 0;
            if (input->compute_design_powers)
                input->compute_design_powers(input->context);
            if (input->mark_planets_under_attack)
                input->mark_planets_under_attack(input->context);
            if (input->increase_minefield_sizes)
                input->increase_minefield_sizes(input->context);
            if (input->init_random_planet_list)
                input->init_random_planet_list(input->context);
            if (requested_personality != AI_TURN_NO_PERSONALITY)
                input->players[player].personality = requested_personality;
            personality = (StarsU16)(input->players[player].personality >>
                AI_TURN_PERSONALITY_SHIFT);
            for (i = 0; i < 8; ++i)
                if (personality == i && input->personalities[i])
                    input->personalities[i](input->context, player,
                        planet_table);
        }
    }
    if (input->write_log) input->write_log(input->context, player);
    if (input->write_history) input->write_history(input->context, player);
    ai_turn_free(input, planet_table, planet_index, ai_data);
    input->ai_active = 0;
    if (input->current_player) *input->current_player = saved_player;
}
