#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct InitNewGameTrace {
    StarsI16 random_values[16];
    StarsI16 random_limits[16];
    StarsI16 random_count;
    StarsI16 victory_index;
    StarsI16 victory_value;
    StarsI16 victory_calls;
} InitNewGameTrace;

static StarsI16 probe_random(void *opaque, StarsI16 limit)
{
    InitNewGameTrace *trace = (InitNewGameTrace *)opaque;
    StarsI16 call = trace->random_count;
    trace->random_limits[call] = limit;
    ++trace->random_count;
    return trace->random_values[call];
}

static StarsI16 probe_set_victory(void *opaque, StarsI16 index,
    StarsI16 value)
{
    InitNewGameTrace *trace = (InitNewGameTrace *)opaque;
    trace->victory_index = index;
    trace->victory_value = value;
    ++trace->victory_calls;
    return 0;
}

static int run_case(StarsI16 map_size, StarsI16 ai_level,
    const StarsI16 *random_values, StarsI16 random_count,
    StarsI16 expected_players, StarsI16 expected_random_calls,
    StarsU8 expected_type)
{
    StarsI16 players = 99;
    StarsU8 types[20];
    StarsInitNewGamePlrInput input;
    InitNewGameTrace trace;
    StarsI16 i;

    (void)memset(&trace, 0, sizeof(trace));
    (void)memset(types, 0, sizeof(types));
    trace.random_count = 0;
    for (i = 0; i < random_count; ++i)
        trace.random_values[i] = random_values[i];
    types[0] = 0x55U;
    input.step_max_so_far = 0;
    input.ai_level = ai_level;
    input.map_size = map_size;
    input.player_count = &players;
    input.read_only = 0U;
    input.player_types = types;
    input.random_value = probe_random;
    input.set_victory_value = probe_set_victory;
    input.context = &trace;

    stars_init_new_game_plr(&input);
    if (players != expected_players) return 1;
    if (trace.random_count != expected_random_calls) return 2;
    if (trace.victory_calls != 1 || trace.victory_index != 9 ||
            trace.victory_value != map_size * 2) return 3;
    if (types[0] != 0x55U || types[1] != expected_type) return 4;
    return 0;
}

int main(void)
{
    static const StarsI16 map0_easy[] = { 0 };
    static const StarsI16 map0_expert[] = { 0 };
    static const StarsI16 map1_expert[] = { 0 };
    static const StarsI16 map1_hard[] = { 0 };
    static const StarsI16 map2_hard[] = { 1, 0 };
    static const StarsI16 map3_expert[] = { 0, 1 };
    static const StarsI16 map4_expert[] = { 0, 2 };
    StarsI16 players;
    StarsU8 types[4];
    StarsInitNewGamePlrInput input;
    int failures = 0;

    if (run_case(0, 0, map0_easy, 1, 2, 0, 0x0bU)) ++failures;
    if (run_case(0, 3, map0_expert, 1, 3, 2, (StarsU8)'c')) ++failures;
    if (run_case(1, 3, map1_expert, 1, 5, 4, (StarsU8)'c')) ++failures;
    if (run_case(1, 2, map1_hard, 1, 4, 3, (StarsU8)'S')) ++failures;
    if (run_case(2, 2, map2_hard, 2, 6, 6, (StarsU8)'S')) ++failures;
    if (run_case(3, 3, map3_expert, 2, 15, 15, (StarsU8)'c')) ++failures;
    if (run_case(4, 3, map4_expert, 2, 11, 11, (StarsU8)'c')) ++failures;

    (void)memset(types, 0xaa, sizeof(types));
    players = 7;
    input.step_max_so_far = 1;
    input.ai_level = 3;
    input.map_size = 0;
    input.player_count = &players;
    input.read_only = 0U;
    input.player_types = types;
    input.random_value = probe_random;
    input.set_victory_value = probe_set_victory;
    input.context = 0;
    {
        InitNewGameTrace trace;
        (void)memset(&trace, 0, sizeof(trace));
        input.context = &trace;
        stars_init_new_game_plr(&input);
        if (players != 7 || trace.victory_calls != 1 ||
                trace.victory_index != 9 || trace.victory_value != 0)
            ++failures;
    }

    if (failures == 0)
        puts("INIT_NEW_GAME_PLR_SOURCE=PASS");
    else
        puts("INIT_NEW_GAME_PLR_SOURCE=FAIL");
    return failures;
}
