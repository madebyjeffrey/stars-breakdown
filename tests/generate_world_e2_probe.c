#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct E2Trace {
    StarsI16 ships;
    StarsI16 plans;
    StarsI16 files;
    StarsI16 records;
    StarsI16 data_files;
    StarsI16 closed;
    StarsI16 set_player;
    StarsI16 reset_log;
    StarsI16 created_windows;
    StarsI16 first_type;
    StarsI16 first_size;
    StarsI16 last_type;
    StarsI16 last_size;
    StarsI16 player_record;
    StarsI16 order[32];
    StarsI16 order_count;
} E2Trace;

static void put16(StarsU8 *raw, StarsU16 offset, StarsU16 value)
{
    raw[offset] = (StarsU8)value;
    raw[(StarsU16)(offset + 1U)] = (StarsU8)(value >> 8);
}

static void put32(StarsU8 *raw, StarsU16 offset, StarsU32 value)
{
    raw[offset] = (StarsU8)value;
    raw[(StarsU16)(offset + 1U)] = (StarsU8)(value >> 8);
    raw[(StarsU16)(offset + 2U)] = (StarsU8)(value >> 16);
    raw[(StarsU16)(offset + 3U)] = (StarsU8)(value >> 24);
}

static StarsI16 check(StarsI16 condition, StarsI16 *checks)
{
    ++*checks;
    return condition ? 0 : 1;
}

static StarsI16 e2_race_stat(void *context, StarsI16 player,
    StarsI16 stat)
{
    (void)context;
    (void)player;
    (void)stat;
    return 0;
}

static StarsI16 e2_race_flag(void *context, StarsI16 player,
    StarsI16 flag)
{
    (void)context;
    (void)player;
    (void)flag;
    return 0;
}

static StarsI16 e2_create_ship(void *context, StarsI16 player,
    StarsI16 planet, StarsI16 design, StarsI16 add_design)
{
    E2Trace *trace;
    (void)player;
    (void)planet;
    (void)add_design;
    trace = (E2Trace *)context;
    ++trace->ships;
    return design;
}

static void e2_init_plan(void *context, StarsU8 *data,
    StarsI16 plan, StarsI16 player)
{
    E2Trace *trace;
    trace = (E2Trace *)context;
    data[0] = (StarsU8)(plan + player);
    ++trace->plans;
}

static StarsI16 e2_create_wormhole(void *context, StarsI16 direction)
{
    (void)context;
    (void)direction;
    return 0;
}

static void e2_set_wormhole(void *context, StarsI16 handle,
    StarsPoint point)
{
    (void)context;
    (void)handle;
    (void)point;
}

static StarsI16 e2_validate_wormhole(void *context, StarsI16 handle)
{
    (void)context;
    (void)handle;
    return 0;
}

static StarsI16 e2_link_wormholes(void *context, StarsI16 first,
    StarsI16 second)
{
    (void)context;
    (void)first;
    (void)second;
    return 1;
}

static void e2_message(void *context, StarsI16 recipient,
    StarsI16 message, StarsI16 value, StarsI16 extra)
{
    (void)context;
    (void)recipient;
    (void)message;
    (void)value;
    (void)extra;
}

static StarsI16 e2_open(void *context, const char *name)
{
    E2Trace *trace;
    (void)name;
    trace = (E2Trace *)context;
    ++trace->files;
    return 1;
}

static StarsI16 e2_write_record(void *context, StarsI16 type,
    StarsI16 size, const void *record, StarsI16 count)
{
    E2Trace *trace;
    (void)record;
    trace = (E2Trace *)context;
    if (trace->records == 0) {
        trace->first_type = type;
        trace->first_size = size;
    }
    trace->last_type = type;
    trace->last_size = size;
    if (size == 2)
        trace->player_record = *(const StarsI16 *)record;
    (void)count;
    ++trace->records;
    return 1;
}

static void e2_close(void *context)
{
    ++((E2Trace *)context)->closed;
}

static StarsI16 e2_write_data(void *context, const char *name,
    StarsI16 player, StarsI16 flags)
{
    E2Trace *trace;
    (void)name;
    (void)player;
    (void)flags;
    trace = (E2Trace *)context;
    ++trace->data_files;
    return 1;
}

static void e2_set_player(void *context, StarsI16 player)
{
    ((E2Trace *)context)->set_player = player;
}

static void e2_reset_log(void *context)
{
    ++((E2Trace *)context)->reset_log;
}

static void e2_create_windows(void *context)
{
    ++((E2Trace *)context)->created_windows;
}

int main(void)
{
    static StarsU8 raw[64];
    StarsSeeds seeds;
    StarsGenerateWorldGameState state;
    StarsGenerateWorldPipelineInput pipeline;
    StarsGenerateWorldInput layout_input;
    StarsGenerateWorldLayout layout;
    StarsGenerateWorldPlanetInput planet_input;
    static StarsGenerateWorldPlanet planets[999];
    StarsGenerateWorldStartsInput starts_input;
    StarsI16 starts[1];
    StarsGenerateWorldPlayerInput player_input;
    StarsGenerateWorldPlayer players[1];
    StarsGenerateWorldHomeworldInput homeworld_input;
    StarsGenerateWorldStartupShipsInput startup_input;
    StarsGenerateWorldBattlePlansInput battle_input;
    static StarsU8 plan_storage[320];
    static StarsU8 plan_counts[1];
    StarsGenerateWorldWormholesInput wormhole_input;
    StarsGenerateWorldPlayerStatusInput status_input;
    StarsI16 selected_player;
    StarsI16 single_player_mode;
    StarsGenerateWorldFileInput file_input;
    StarsGenerateWorldFinalizeInput finalize_input;
    E2Trace trace;
    StarsI16 i;
    StarsI16 checks;
    StarsI16 failures;
    StarsU32 prior_first;

    memset(raw, 0, sizeof raw);
    put32(raw, 0, 0x89abcdefUL);
    put16(raw, 4, 1U);
    put16(raw, 6, 1U);
    put16(raw, 8, 1U);
    put16(raw, 10, 128U);
    put16(raw, 12, 1U);
    put16(raw, 16, 0x0001U);
    put16(raw, 18, 7U);
    stars_randomize2(&seeds, 0x12345678UL);
    memset(&state, 0, sizeof state);
    memset(&trace, 0, sizeof trace);
    memset(&pipeline, 0, sizeof pipeline);
    memset(&layout_input, 0, sizeof layout_input);
    memset(&layout, 0, sizeof layout);
    memset(&planet_input, 0, sizeof planet_input);
    memset(planets, 0, sizeof planets);
    memset(&starts_input, 0, sizeof starts_input);
    memset(starts, 0, sizeof starts);
    memset(&player_input, 0, sizeof player_input);
    memset(players, 0, sizeof players);
    memset(&homeworld_input, 0, sizeof homeworld_input);
    memset(&startup_input, 0, sizeof startup_input);
    memset(&battle_input, 0, sizeof battle_input);
    memset(plan_storage, 0, sizeof plan_storage);
    memset(plan_counts, 0, sizeof plan_counts);
    memset(&wormhole_input, 0, sizeof wormhole_input);
    memset(&status_input, 0, sizeof status_input);
    memset(&file_input, 0, sizeof file_input);
    memset(&finalize_input, 0, sizeof finalize_input);
    for (i = 0; i < 3; ++i)
        players[0].environment_max[i] = -1;

    checks = 0;
    failures = 0;
    failures += check(stars_generate_world_read_game_state(raw, &seeds,
        &state), &checks);
    layout_input.map_size = state.map_size;
    layout_input.density = state.density;
    layout_input.minimum_distance = state.start_distance;
    layout_input.world_flags = state.generation_flags;
    layout_input.random_value = stars_generate_world_random_from_state;
    layout_input.context = &state;
    layout_input.layout = &layout;

    planet_input.world_flags = state.generation_flags;
    planet_input.planet_count = state.planet_max;
    planet_input.random_value = stars_generate_world_random_from_state;
    planet_input.context = &state;
    planet_input.planets = planets;
    starts_input.layout = &layout;
    starts_input.player_count = state.player_count;
    starts_input.start_distance = state.start_distance;
    starts_input.random_value = stars_generate_world_random_from_state;
    starts_input.context = &state;
    starts_input.start_planets = starts;
    player_input.world_flags = state.generation_flags;
    player_input.player_count = state.player_count;
    player_input.players = players;
    player_input.race_stat = e2_race_stat;
    player_input.race_flag = e2_race_flag;
    player_input.context = &state;
    homeworld_input.world_flags = state.generation_flags;
    homeworld_input.mineral_floor = 30;
    homeworld_input.player_count = state.player_count;
    homeworld_input.planet_count = state.planet_max;
    homeworld_input.start_planets = starts;
    homeworld_input.planets = planets;
    homeworld_input.players = players;
    homeworld_input.random_value = stars_generate_world_random_from_state;
    homeworld_input.race_flag = e2_race_flag;
    homeworld_input.context = &state;
    startup_input.player_count = state.player_count;
    startup_input.map_size = state.map_size;
    startup_input.players = players;
    startup_input.race_flag = e2_race_flag;
    startup_input.create_ship = e2_create_ship;
    startup_input.context = &trace;
    battle_input.player_count = state.player_count;
    battle_input.plan_stride = 320;
    battle_input.plan_size = 64;
    battle_input.plan_storage = plan_storage;
    battle_input.plan_counts = plan_counts;
    battle_input.planet_count = state.planet_max;
    battle_input.planets = planets;
    battle_input.init_plan = e2_init_plan;
    battle_input.context = &trace;
    wormhole_input.pair_count = 0;
    wormhole_input.galaxy_extent = 1600;
    wormhole_input.max_attempts = 1;
    wormhole_input.random_value = stars_generate_world_random_from_state;
    wormhole_input.create_wormhole = e2_create_wormhole;
    wormhole_input.set_point = e2_set_wormhole;
    wormhole_input.validate_wormhole = e2_validate_wormhole;
    wormhole_input.link_wormholes = e2_link_wormholes;
    wormhole_input.context = &state;
    status_input.player_count = state.player_count;
    status_input.players = players;
    status_input.send_message = e2_message;
    status_input.context = &trace;
    status_input.selected_player = &selected_player;
    status_input.single_player_mode = &single_player_mode;
    file_input.layout = &layout;
    file_input.player_count = state.player_count;
    file_input.base_name = "WORLD";
    file_input.game_state = raw;
    file_input.open_file = e2_open;
    file_input.write_record = e2_write_record;
    file_input.close_stream = e2_close;
    file_input.write_data_file = e2_write_data;
    file_input.context = &trace;
    finalize_input.batch_mode = 0;
    finalize_input.hosted_game = 0;
    finalize_input.set_player = e2_set_player;
    finalize_input.reset_message_log = e2_reset_log;
    finalize_input.create_windows = e2_create_windows;
    finalize_input.context = &trace;
    pipeline.layout = &layout_input;
    pipeline.planets = &planet_input;
    pipeline.starts = &starts_input;
    pipeline.players = &player_input;
    pipeline.homeworlds = &homeworld_input;
    pipeline.startup_ships = &startup_input;
    pipeline.battle_plans = &battle_input;
    pipeline.wormholes = &wormhole_input;
    pipeline.player_status = &status_input;
    pipeline.files = &file_input;
    pipeline.finalize = &finalize_input;
    pipeline.special_planet_count = 0;

    prior_first = state.random.first;
    failures += check(stars_generate_world_pipeline(&pipeline), &checks);
    failures += check(state.random.first != prior_first, &checks);
    failures += check(layout.point_count > 0 && layout.point_count <= 999,
        &checks);
    failures += check(planets[starts[0]].owner == 0 &&
        players[0].homeworld == starts[0], &checks);
    failures += check(players[0].ships_created == 4 && trace.ships == 4,
        &checks);
    failures += check(plan_counts[0] == 5 && trace.plans == 5, &checks);
    failures += check(trace.files == 1 && trace.closed == 1 &&
        trace.data_files == 2, &checks);
    failures += check(trace.records == layout.point_count + 2 &&
        trace.first_type == 7 && trace.first_size == 0x40, &checks);
    failures += check(trace.last_type == 0 && trace.last_size == 2 &&
        trace.player_record == 1, &checks);
    failures += check(trace.set_player == -1 && trace.reset_log == 1 &&
        trace.created_windows == 1, &checks);
    failures += check(selected_player == 0 && single_player_mode == 1,
        &checks);
    failures += check(state.game_id == 0x89abcdefUL && state.turn == 7,
        &checks);

    printf("GENERATE_WORLD_E2_CHECKS=%d FAILURES=%d RESULT=%s\n",
        (int)checks, (int)failures, failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
}
