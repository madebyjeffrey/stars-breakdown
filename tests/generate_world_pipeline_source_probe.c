#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct PipelineTrace {
    StarsU32 random_state;
    StarsI16 ships;
    StarsI16 plans;
    StarsI16 files;
    StarsI16 messages;
} PipelineTrace;

static StarsI16 pipeline_random(void *opaque, StarsI16 limit)
{
    PipelineTrace *trace;
    trace = (PipelineTrace *)opaque;
    trace->random_state = trace->random_state * 1664525UL + 1013904223UL;
    if (limit <= 0)
        return 0;
    return (StarsI16)((trace->random_state >> 8) % (StarsU32)limit);
}

static StarsI16 pipeline_race_stat(void *opaque, StarsI16 player,
    StarsI16 stat)
{
    (void)opaque;
    (void)player;
    (void)stat;
    return 0;
}

static StarsI16 pipeline_race_flag(void *opaque, StarsI16 player,
    StarsI16 flag)
{
    (void)opaque;
    (void)player;
    (void)flag;
    return 0;
}

static StarsI16 pipeline_create_ship(void *opaque, StarsI16 player,
    StarsI16 planet, StarsI16 design, StarsI16 add_design)
{
    PipelineTrace *trace;
    (void)player;
    (void)planet;
    (void)design;
    (void)add_design;
    trace = (PipelineTrace *)opaque;
    ++trace->ships;
    return design;
}

static void pipeline_init_plan(void *opaque, StarsU8 *data,
    StarsI16 plan, StarsI16 player)
{
    PipelineTrace *trace;
    (void)plan;
    (void)player;
    trace = (PipelineTrace *)opaque;
    data[0] = 0xa5;
    ++trace->plans;
}

static StarsI16 pipeline_create_wormhole(void *opaque, StarsI16 direction)
{
    (void)opaque;
    (void)direction;
    return 0;
}

static void pipeline_set_wormhole(void *opaque, StarsI16 handle,
    StarsPoint point)
{
    (void)opaque;
    (void)handle;
    (void)point;
}

static StarsI16 pipeline_validate_wormhole(void *opaque, StarsI16 handle)
{
    (void)opaque;
    (void)handle;
    return 0;
}

static StarsI16 pipeline_link_wormholes(void *opaque, StarsI16 first,
    StarsI16 second)
{
    (void)opaque;
    (void)first;
    (void)second;
    return 1;
}

static void pipeline_message(void *opaque, StarsI16 recipient,
    StarsI16 message, StarsI16 value, StarsI16 extra)
{
    PipelineTrace *trace;
    (void)recipient;
    (void)message;
    (void)value;
    (void)extra;
    trace = (PipelineTrace *)opaque;
    ++trace->messages;
}

static StarsI16 pipeline_open(void *opaque, const char *name)
{
    PipelineTrace *trace;
    (void)name;
    trace = (PipelineTrace *)opaque;
    ++trace->files;
    return 1;
}

static StarsI16 pipeline_write_record(void *opaque, StarsI16 type,
    StarsI16 size, const void *record, StarsI16 count)
{
    (void)opaque;
    (void)type;
    (void)size;
    (void)record;
    (void)count;
    return 1;
}

static void pipeline_close(void *opaque)
{
    (void)opaque;
}

static StarsI16 pipeline_write_data(void *opaque, const char *name,
    StarsI16 player, StarsI16 flags)
{
    (void)opaque;
    (void)name;
    (void)player;
    (void)flags;
    return 1;
}

int main(void)
{
    PipelineTrace trace;
    StarsGenerateWorldPipelineInput pipeline;
    StarsGenerateWorldInput layout_input;
    StarsGenerateWorldLayout layout;
    StarsGenerateWorldPlanetInput planet_input;
    StarsGenerateWorldPlanet planets[128];
    StarsGenerateWorldStartsInput starts_input;
    StarsI16 starts[1];
    StarsGenerateWorldPlayerInput player_input;
    StarsGenerateWorldPlayer players[1];
    StarsGenerateWorldHomeworldInput homeworld_input;
    StarsGenerateWorldStartupShipsInput startup_input;
    StarsGenerateWorldBattlePlansInput battle_input;
    StarsU8 battle_storage[64];
    StarsU8 battle_counts[1];
    StarsGenerateWorldWormholesInput wormhole_input;
    StarsGenerateWorldPlayerStatusInput status_input;
    StarsI16 selected_player;
    StarsI16 single_player_mode;
    StarsGenerateWorldFileInput file_input;
    StarsGenerateWorldFinalizeInput finalize_input;
    StarsI16 failures;

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
    memset(battle_storage, 0, sizeof battle_storage);
    memset(battle_counts, 0, sizeof battle_counts);
    memset(&wormhole_input, 0, sizeof wormhole_input);
    memset(&status_input, 0, sizeof status_input);
    memset(&file_input, 0, sizeof file_input);
    memset(&finalize_input, 0, sizeof finalize_input);

    trace.random_state = 0x12345678UL;
    layout_input.map_size = 1;
    layout_input.density = 1;
    layout_input.minimum_distance = 1;
    layout_input.random_value = pipeline_random;
    layout_input.context = &trace;
    layout_input.layout = &layout;

    planet_input.world_flags = 1;
    planet_input.planet_count = 128;
    planet_input.random_value = pipeline_random;
    planet_input.context = &trace;
    planet_input.planets = planets;

    starts_input.layout = &layout;
    starts_input.player_count = 1;
    starts_input.start_distance = 1;
    starts_input.random_value = pipeline_random;
    starts_input.context = &trace;
    starts_input.start_planets = starts;

    player_input.player_count = 1;
    player_input.players = players;
    player_input.race_stat = pipeline_race_stat;
    player_input.race_flag = pipeline_race_flag;
    player_input.context = &trace;

    homeworld_input.mineral_floor = 30;
    homeworld_input.player_count = 1;
    homeworld_input.planet_count = 128;
    homeworld_input.start_planets = starts;
    homeworld_input.planets = planets;
    homeworld_input.players = players;
    homeworld_input.random_value = pipeline_random;
    homeworld_input.race_flag = pipeline_race_flag;
    homeworld_input.context = &trace;

    startup_input.player_count = 1;
    startup_input.map_size = 1;
    startup_input.players = players;
    startup_input.race_flag = pipeline_race_flag;
    startup_input.create_ship = pipeline_create_ship;
    startup_input.context = &trace;

    battle_input.player_count = 1;
    battle_input.plan_stride = 64;
    battle_input.plan_size = 4;
    battle_input.plan_storage = battle_storage;
    battle_input.plan_counts = battle_counts;
    battle_input.init_plan = pipeline_init_plan;
    battle_input.context = &trace;

    wormhole_input.galaxy_extent = 800;
    wormhole_input.max_attempts = 1;
    wormhole_input.random_value = pipeline_random;
    wormhole_input.create_wormhole = pipeline_create_wormhole;
    wormhole_input.set_point = pipeline_set_wormhole;
    wormhole_input.validate_wormhole = pipeline_validate_wormhole;
    wormhole_input.link_wormholes = pipeline_link_wormholes;
    wormhole_input.context = &trace;

    status_input.player_count = 1;
    status_input.players = players;
    status_input.send_message = pipeline_message;
    status_input.context = &trace;
    status_input.selected_player = &selected_player;
    status_input.single_player_mode = &single_player_mode;

    file_input.layout = &layout;
    file_input.player_count = 1;
    file_input.base_name = "WORLD";
    file_input.game_state = &layout;
    file_input.open_file = pipeline_open;
    file_input.write_record = pipeline_write_record;
    file_input.close_stream = pipeline_close;
    file_input.write_data_file = pipeline_write_data;
    file_input.context = &trace;

    finalize_input.batch_mode = 1;

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

    failures = !stars_generate_world_pipeline(&pipeline);
    failures += layout.point_count <= 0 || planets[starts[0]].owner != 0;
    failures += planets[0].point.x != layout.points[0].x ||
        planets[0].point.y != layout.points[0].y;
    failures += players[0].homeworld != starts[0] || players[0].ships_created != 4;
    failures += battle_counts[0] != 5 || trace.plans != 5;
    failures += trace.files != 1 || selected_player != 0;
    printf("GENERATE_WORLD_PIPELINE_CHECKS=7 FAILURES=%d RESULT=%s\n",
        (int)failures, failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
}
