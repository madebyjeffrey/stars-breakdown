#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct ProbeRandom {
    StarsU32 value;
} ProbeRandom;

typedef struct ProbeStartRetryRandom {
    StarsI16 calls;
} ProbeStartRetryRandom;

typedef struct ProbeRace {
    StarsI16 types[4];
    StarsU16 flags[4];
    StarsI16 homeworld_stat;
} ProbeRace;

typedef struct ProbeShipLog {
    StarsI16 designs[64];
    StarsI16 add_design[64];
    StarsI16 count;
    StarsI16 starbase_player;
    StarsI16 starbase_planet;
    StarsI16 scanner_player;
    StarsI16 scanner_planet;
    StarsI16 scanner_level;
    StarsI16 special_calls;
    StarsI16 special_player;
    StarsI16 special_planet;
    StarsI16 part_queries[16];
    StarsI16 part_query_count;
} ProbeShipLog;

typedef struct ProbeSpecialContext {
    ProbeRandom random;
    ProbeShipLog *log;
    StarsI16 desirability_calls;
} ProbeSpecialContext;

typedef struct ProbeFileLog {
    StarsI16 opened;
    StarsI16 closed;
    StarsI16 records;
    StarsI16 data_files;
    StarsI16 first_record_type;
    StarsI16 first_record_size;
    StarsU32 packed[4];
} ProbeFileLog;

typedef struct ProbeFinalizeLog {
    StarsI16 set_player;
    StarsI16 reset_log;
    StarsI16 created_windows;
    StarsI16 destroyed_game;
    StarsI16 formatted_name;
    StarsI16 loaded_game;
    StarsI16 sent_message;
} ProbeFinalizeLog;

typedef struct ProbeBattlePlanLog {
    StarsI16 calls;
    StarsI16 templates[10];
    StarsI16 players[10];
} ProbeBattlePlanLog;

typedef struct ProbeWormholeLog {
    StarsI16 created;
    StarsI16 linked;
    StarsI16 set_points;
    StarsI16 validated;
    StarsI16 directions[4];
    StarsPoint points[4];
} ProbeWormholeLog;

typedef struct ProbeWormholeContext {
    ProbeRandom random;
    ProbeWormholeLog log;
} ProbeWormholeContext;

typedef struct ProbeMessageLog {
    StarsI16 count;
    StarsI16 recipients[8];
    StarsI16 messages[8];
} ProbeMessageLog;

static void probe_send_player_message(void *opaque, StarsI16 recipient,
    StarsI16 message, StarsI16 value, StarsI16 extra)
{
    ProbeMessageLog *log;
    (void)value;
    (void)extra;
    log = (ProbeMessageLog *)opaque;
    log->recipients[log->count] = recipient;
    log->messages[log->count] = message;
    ++log->count;
}

static StarsI16 probe_random(void *, StarsI16);

static StarsI16 probe_wormhole_random(void *opaque, StarsI16 limit)
{
    ProbeWormholeContext *context;
    context = (ProbeWormholeContext *)opaque;
    return probe_random(&context->random, limit);
}

static StarsI16 probe_create_wormhole(void *opaque, StarsI16 direction)
{
    ProbeWormholeContext *context;
    context = (ProbeWormholeContext *)opaque;
    context->log.directions[context->log.created] = direction;
    return context->log.created++;
}

static void probe_set_wormhole_point(void *opaque, StarsI16 handle,
    StarsPoint point)
{
    ProbeWormholeContext *context;
    context = (ProbeWormholeContext *)opaque;
    context->log.points[handle] = point;
    ++context->log.set_points;
}

static StarsI16 probe_validate_wormhole(void *opaque, StarsI16 handle)
{
    ProbeWormholeContext *context;
    (void)handle;
    context = (ProbeWormholeContext *)opaque;
    ++context->log.validated;
    return 0;
}

static StarsI16 probe_link_wormholes(void *opaque, StarsI16 first,
    StarsI16 second)
{
    ProbeWormholeContext *context;
    (void)first;
    (void)second;
    context = (ProbeWormholeContext *)opaque;
    ++context->log.linked;
    return 1;
}

static void probe_init_battle_plan(void *opaque, StarsU8 *data,
    StarsI16 template_index, StarsI16 player)
{
    ProbeBattlePlanLog *log;
    log = (ProbeBattlePlanLog *)opaque;
    data[0] = (StarsU8)template_index;
    log->templates[log->calls] = template_index;
    log->players[log->calls] = player;
    ++log->calls;
}

static void probe_set_player(void *opaque, StarsI16 player)
{
    ((ProbeFinalizeLog *)opaque)->set_player = player;
}

static void probe_reset_log(void *opaque)
{
    ++((ProbeFinalizeLog *)opaque)->reset_log;
}

static void probe_create_windows(void *opaque)
{
    ++((ProbeFinalizeLog *)opaque)->created_windows;
}

static void probe_destroy_game(void *opaque)
{
    ++((ProbeFinalizeLog *)opaque)->destroyed_game;
}

static StarsI16 probe_format_load_name(void *opaque, StarsI16 player,
    char *name, StarsI16 capacity)
{
    ProbeFinalizeLog *log;
    (void)player;
    log = (ProbeFinalizeLog *)opaque;
    if (capacity < 5)
        return 0;
    strcpy(name, "SAVE");
    ++log->formatted_name;
    return 1;
}

static StarsI16 probe_load_game(void *opaque, const char *base,
    const char *name)
{
    ProbeFinalizeLog *log;
    (void)base;
    (void)name;
    log = (ProbeFinalizeLog *)opaque;
    ++log->loaded_game;
    return 1;
}

static void probe_send_message(void *opaque, StarsI16 message,
    StarsI16 command, StarsI16 value)
{
    ProbeFinalizeLog *log;
    (void)message;
    (void)command;
    (void)value;
    log = (ProbeFinalizeLog *)opaque;
    ++log->sent_message;
}

static StarsI16 probe_open_file(void *opaque, const char *name)
{
    ProbeFileLog *log;
    log = (ProbeFileLog *)opaque;
    return name != 0 && ++log->opened;
}

static StarsI16 probe_write_record(void *opaque, StarsI16 type,
    StarsI16 size, const void *data, StarsI16 byte_count)
{
    ProbeFileLog *log;
    log = (ProbeFileLog *)opaque;
    if (log->records == 0) {
        log->first_record_type = type;
        log->first_record_size = size;
    }
    if (byte_count == 4 && log->records < 4)
        log->packed[log->records] = *(const StarsU32 *)data;
    ++log->records;
    return 1;
}

static void probe_close_stream(void *opaque)
{
    ProbeFileLog *log;
    log = (ProbeFileLog *)opaque;
    ++log->closed;
}

static StarsI16 probe_write_data_file(void *opaque, const char *name,
    StarsI16 player, StarsI16 mode)
{
    ProbeFileLog *log;
    (void)name;
    (void)player;
    (void)mode;
    log = (ProbeFileLog *)opaque;
    ++log->data_files;
    return 1;
}

static StarsI16 probe_create_ship(void *, StarsI16, StarsI16, StarsI16,
    StarsI16);
static void probe_set_starbase(void *opaque, StarsI16 player,
    StarsI16 planet)
{
    ProbeShipLog *log;
    log = (ProbeShipLog *)opaque;
    log->starbase_player = player;
    log->starbase_planet = planet;
}
static void probe_set_scanner(void *opaque, StarsI16 player,
    StarsI16 planet, StarsI16 level)
{
    ProbeShipLog *log;
    log = (ProbeShipLog *)opaque;
    log->scanner_player = player;
    log->scanner_planet = planet;
    log->scanner_level = level;
}
static StarsI16 probe_create_special_startup(void *opaque, StarsI16 player,
    StarsI16 planet)
{
    ProbeShipLog *log;
    log = (ProbeShipLog *)opaque;
    ++log->special_calls;
    log->special_player = player;
    log->special_planet = planet;
    return 1;
}
static StarsI16 probe_part_available(void *opaque, StarsU16 group,
    StarsU16 subtype, StarsU16 part)
{
    ProbeShipLog *log;
    (void)group;
    (void)subtype;
    log = (ProbeShipLog *)opaque;
    if (log->part_query_count < 16)
        log->part_queries[log->part_query_count++] = (StarsI16)part;
    return part == 4U || part == 0U;
}
static StarsI16 probe_random(void *, StarsI16);

static StarsI16 probe_desirability(void *opaque,
    const StarsGenerateWorldPlanet *planet, StarsI16 player)
{
    ProbeSpecialContext *special;
    (void)planet;
    (void)player;
    special = (ProbeSpecialContext *)opaque;
    ++special->desirability_calls;
    return special->desirability_calls == 1 ? 5 : 10;
}

static StarsI16 probe_create_special_ship(void *opaque, StarsI16 player,
    StarsI16 planet, StarsI16 design, StarsI16 add_design)
{
    ProbeSpecialContext *special;
    special = (ProbeSpecialContext *)opaque;
    return probe_create_ship(special->log, player, planet, design,
        add_design);
}

static StarsI16 probe_create_ship(void *opaque, StarsI16 player,
    StarsI16 planet, StarsI16 design, StarsI16 add_design)
{
    ProbeShipLog *log;
    (void)player;
    (void)planet;
    log = (ProbeShipLog *)opaque;
    log->designs[log->count] = design;
    log->add_design[log->count] = add_design;
    ++log->count;
    return (StarsI16)(design + 20);
}

static StarsI16 probe_race_stat(void *opaque, StarsI16 player,
    StarsI16 stat)
{
    ProbeRace *race;
    race = (ProbeRace *)opaque;
    if (stat == 0x0e)
        return race->types[player];
    if (stat == 7)
        return race->homeworld_stat;
    return 0;
}

static StarsI16 probe_advantage_points(void *opaque, StarsI16 player)
{
    (void)opaque;
    (void)player;
    return 23;
}

static StarsI16 probe_race_flag(void *opaque, StarsI16 player,
    StarsI16 flag)
{
    ProbeRace *race;
    race = (ProbeRace *)opaque;
    return (race->flags[player] & (StarsU16)(1U << (flag & 15))) != 0;
}

static StarsI16 probe_true_max_growth(void *opaque, StarsI16 player)
{
    (void)opaque;
    (void)player;
    return 5;
}

static void probe_randomize_race(void *opaque, StarsI16 player)
{
    ProbeRace *race;
    race = (ProbeRace *)opaque;
    race->types[player] = 9;
}

static StarsI16 probe_random(void *opaque, StarsI16 limit)
{
    ProbeRandom *random;
    random = (ProbeRandom *)opaque;
    random->value = random->value * 1103515245UL + 12345UL;
    return (StarsI16)((random->value >> 8) % (StarsU32)limit);
}

static StarsI16 probe_zero_random(void *opaque, StarsI16 limit)
{
    (void)opaque;
    (void)limit;
    return 0;
}

static StarsI16 probe_special_zero_random(void *opaque, StarsI16 limit)
{
    (void)opaque;
    (void)limit;
    return 0;
}

static StarsI16 probe_start_retry_random(void *opaque, StarsI16 limit)
{
    ProbeStartRetryRandom *random;
    random = (ProbeStartRetryRandom *)opaque;
    ++random->calls;
    if (limit <= 0)
        return 0;
    if (random->calls == 1 || random->calls == 52)
        return 0;
    return 1 % limit;
}

static StarsI16 layout_is_valid(const StarsGenerateWorldLayout *layout,
    StarsI16 minimum_distance)
{
    StarsU8 ids[125];
    StarsI16 i;
    StarsI16 j;
    StarsI32 dx;
    StarsI32 dy;
    StarsI32 distance;

    memset(ids, 0, sizeof ids);

    if (layout->point_count <= 0 || layout->point_count > 999)
        return 0;
    for (i = 0; i < layout->point_count; ++i) {
        if (layout->points[i].x < 0x3f2 || layout->points[i].y < 0x3f2)
            return 0;
        if (layout->point_ids[i] < 0 || layout->point_ids[i] >= 999 ||
                (ids[layout->point_ids[i] >> 3] &
                (StarsU8)(1U << (layout->point_ids[i] & 7))) != 0U)
            return 0;
        ids[layout->point_ids[i] >> 3] = (StarsU8)(
            ids[layout->point_ids[i] >> 3] |
            (StarsU8)(1U << (layout->point_ids[i] & 7)));
        for (j = (StarsI16)(i + 1); j < layout->point_count; ++j) {
            dx = (StarsI32)layout->points[i].x - layout->points[j].x;
            dy = (StarsI32)layout->points[i].y - layout->points[j].y;
            distance = dx * dx + dy * dy;
            if (minimum_distance >= 0 &&
                distance <= (StarsI32)minimum_distance * minimum_distance)
                return 0;
        }
    }
    return 1;
}

int main(void)
{
    StarsGenerateWorldInput input;
    StarsGenerateWorldLayout layout;
    StarsGenerateWorldPlanet planets[12];
    StarsGenerateWorldPlanetInput planet_input;
    StarsGenerateWorldStartsInput starts_input;
    StarsI16 start_planets[4];
    StarsGenerateWorldPlayer players[4];
    StarsGenerateWorldPlayerInput player_input;
    StarsGenerateWorldHomeworldInput homeworld_input;
    StarsGenerateWorldStartupShipsInput startup_input;
    StarsGenerateWorldDesign designs[1];
    StarsGenerateWorldSpecialPlanetInput special_input;
    ProbeRace race;
    ProbeShipLog ship_log;
    ProbeSpecialContext special_context;
    ProbeFileLog file_log;
    StarsGenerateWorldFileInput file_input;
    ProbeFinalizeLog finalize_log;
    StarsGenerateWorldFinalizeInput finalize_input;
    StarsGenerateWorldBattlePlansInput battle_input;
    char load_name[16];
    StarsU8 battle_storage[512];
    StarsU8 battle_counts[2];
    ProbeBattlePlanLog battle_log;
    ProbeWormholeContext wormhole_context;
    StarsGenerateWorldWormholesInput wormhole_input;
    StarsGenerateWorldPlayerStatusInput status_input;
    ProbeMessageLog message_log;
    StarsI16 selected_player;
    StarsI16 single_player_mode;
    StarsI16 special_planet;
    ProbeRandom random;
    ProbeStartRetryRandom retry_random;
    StarsGenerateWorldLayout retry_layout;
    StarsGenerateWorldStartsInput retry_starts_input;
    StarsI16 retry_planets[2];
    StarsI16 failures;
    StarsI16 i;

    memset(&input, 0, sizeof input);
    memset(&layout, 0, sizeof layout);
    random.value = 0x13579bdfUL;
    input.map_size = 1;
    input.density = 1;
    input.minimum_distance = 35;
    input.random_value = probe_random;
    input.context = &random;
    input.layout = &layout;
    failures = !stars_generate_world_layout(&input);
    failures += !layout_is_valid(&layout, input.minimum_distance);
    input.smooth_layout = 1;
    failures += !stars_generate_world_layout(&input);
    failures += !layout_is_valid(&layout, (StarsI16)-1);
    failures += layout.point_count <= 0 || layout.point_count > 999;

    memset(&starts_input, 0, sizeof starts_input);
    memset(start_planets, 0, sizeof start_planets);
    random.value = 0x31415926UL;
    starts_input.layout = &layout;
    starts_input.player_count = 3;
    starts_input.start_distance = 1;
    starts_input.random_value = probe_random;
    starts_input.context = &random;
    starts_input.start_planets = start_planets;
    failures += !stars_generate_world_assign_starts(&starts_input);
    failures += start_planets[0] == start_planets[1] ||
        start_planets[0] == start_planets[2] ||
        start_planets[1] == start_planets[2];

    /* A first pass with two nearly-too-close starts must widen the raw
     * distance band and retry rather than returning failure. */
    memset(&retry_layout, 0, sizeof retry_layout);
    memset(&retry_starts_input, 0, sizeof retry_starts_input);
    memset(&retry_random, 0, sizeof retry_random);
    retry_layout.galaxy_extent = 40;
    retry_layout.point_count = 2;
    retry_layout.points[0].x = 1010;
    retry_layout.points[0].y = 1010;
    retry_layout.points[1].x = 1029;
    retry_layout.points[1].y = 1010;
    retry_starts_input.layout = &retry_layout;
    retry_starts_input.player_count = 2;
    retry_starts_input.start_distance = 1;
    retry_starts_input.random_value = probe_start_retry_random;
    retry_starts_input.context = &retry_random;
    retry_starts_input.start_planets = retry_planets;
    failures += !stars_generate_world_assign_starts(&retry_starts_input);
    failures += retry_random.calls < 53 ||
        retry_planets[0] == retry_planets[1];

    memset(&player_input, 0, sizeof player_input);
    memset(players, 0, sizeof players);
    memset(&race, 0, sizeof race);
    race.types[0] = 1;
    race.types[1] = 2;
    race.types[2] = 3;
    race.types[3] = 9;
    race.flags[0] = (StarsU16)(1U << 8);
    race.flags[1] = (StarsU16)(1U << 0);
    race.flags[2] = (StarsU16)(1U << 0);
    race.flags[3] = (StarsU16)(1U << 0x1e);
    players[0].is_dead = 1;
    players[0].ships_created = 99;
    players[0].salt = 0x12345678L;
    players[0].research_percent = 99;
    players[0].resources_last_year = 0x12345678L;
    players[0].score = 77;
    players[0].relations[0] = 7;
    players[0].relations[1] = 8;
    player_input.player_count = 4;
    player_input.players = players;
    player_input.race_stat = probe_race_stat;
    player_input.race_flag = probe_race_flag;
    player_input.randomize_race = probe_randomize_race;
    player_input.context = &race;
    failures += !stars_generate_world_init_players(&player_input);
    failures += players[0].tech[4] != 5 || players[0].tech[2] != 1;
    failures += players[1].tech[0] != 1 || players[1].tech[1] != 6 ||
        players[1].tech[2] != 2;
    failures += players[2].tech[0] != 1 || players[2].tech[1] != 1 ||
        players[2].tech[2] != 2 || players[2].tech[3] != 2 ||
        players[2].tech[5] != 6;
    failures += players[3].race_type != 9 ||
        players[3].race_stats[0] != 3 || players[3].messages_sent != 4 ||
        players[0].is_dead != 0 || players[0].ships_created != 0 ||
        players[0].salt != 0L || players[0].research_percent != 15 ||
        players[0].resources_last_year != 0L || players[0].score != 0 ||
        players[0].relations[0] != 0 ||
        players[0].relations[1] != 0;

    memset(&homeworld_input, 0, sizeof homeworld_input);
    players[0].environment_min[0] = 20;
    players[0].environment_max[0] = 60;
    players[0].environment_min[1] = 30;
    players[0].environment_max[1] = 30;
    players[0].environment_min[2] = -1;
    players[0].environment_max[2] = -1;
    start_planets[0] = 0;
    homeworld_input.mineral_floor = 30;
    homeworld_input.player_count = 1;
    homeworld_input.planet_count = 12;
    homeworld_input.start_planets = start_planets;
    homeworld_input.planets = planets;
    homeworld_input.players = players;
    homeworld_input.random_value = probe_random;
    homeworld_input.race_flag = probe_race_flag;
    homeworld_input.context = &race;
    random.value = 0x55667788UL;
    planets[0].mineral_concentration[0] = 10;
    planets[0].mineral_concentration[1] = 40;
    failures += !stars_generate_world_assign_homeworlds(&homeworld_input);
    failures += planets[0].owner != 0 || players[0].homeworld != 0 ||
        (planets[0].flags & 0x0400U) == 0U ||
        planets[0].mine_count != 10 || planets[0].factory_count != 10 ||
        planets[0].defense_count != 10 || planets[0].production_slot != 0 ||
        planets[0].population != 250L ||
        planets[0].mineral_concentration[0] != 30 ||
        planets[0].environment[0] != 40 ||
        planets[0].environment[1] != 30 ||
        planets[0].environment[0] != planets[0].environment_original[0];

    /* The enhanced-mineral option scales the initial population through the
     * raw fourth fixed-point resource slot. */
    homeworld_input.world_flags = 0x0020U;
    homeworld_input.true_max_growth = probe_true_max_growth;
    homeworld_input.advantage_points = probe_advantage_points;
    homeworld_input.race_stat = probe_race_stat;
    race.homeworld_stat = 2;
    failures += !stars_generate_world_assign_homeworlds(&homeworld_input);
    failures += planets[0].population != 500L ||
        planets[0].mine_count != 21;

    race.homeworld_stat = 0;
    planets[0].mineral_rate[0] = 100L;
    planets[0].mineral_rate[1] = 200L;
    planets[0].mineral_rate[2] = 300L;
    failures += !stars_generate_world_assign_homeworlds(&homeworld_input);
    failures += planets[0].mineral_rate[0] != 214L ||
        planets[0].mineral_rate[1] != 257L ||
        planets[0].mineral_rate[2] != 357L;

    race.homeworld_stat = 3;
    homeworld_input.world_flags = 0;
    failures += !stars_generate_world_assign_homeworlds(&homeworld_input);
    failures += planets[0].factory_count != 12;
    race.homeworld_stat = 4;
    failures += !stars_generate_world_assign_homeworlds(&homeworld_input);
    failures += planets[0].defense_count != 12;
    race.homeworld_stat = 1;
    planets[0].mineral_concentration[0] = 10;
    planets[0].mineral_concentration[1] = 40;
    planets[0].mineral_concentration[2] = 50;
    failures += !stars_generate_world_assign_homeworlds(&homeworld_input);
    failures += planets[0].mineral_concentration[0] != 47 ||
        planets[0].mineral_concentration[1] != 46 ||
        planets[0].mineral_concentration[2] != 56;
    race.homeworld_stat = 8;
    failures += !stars_generate_world_assign_homeworlds(&homeworld_input);
    failures += planets[0].mine_count != 0 ||
        planets[0].factory_count != 0 || planets[0].defense_count != 0;

    memset(&startup_input, 0, sizeof startup_input);
    memset(&ship_log, 0, sizeof ship_log);
    startup_input.player_count = 4;
    startup_input.map_size = 1;
    startup_input.players = players;
    startup_input.race_flag = probe_race_flag;
    startup_input.create_ship = probe_create_ship;
    startup_input.set_starbase = probe_set_starbase;
    startup_input.set_scanner = probe_set_scanner;
    startup_input.create_special_startup = probe_create_special_startup;
    startup_input.context = &ship_log;
    players[0].race_type = 0;
    players[0].homeworld = 0;
    players[1].race_type = 5;
    players[1].homeworld = 1;
    players[2].race_type = 7;
    players[2].homeworld = 2;
    players[3].race_type = 9;
    players[3].homeworld = 3;
    players[3].tech[3] = 3;
    failures += !stars_generate_world_create_startup_ships(&startup_input);
    failures += players[0].ships_created != 4 ||
        players[1].ships_created != 4 || players[2].ships_created != 5 ||
        players[3].ships_created != 6 || ship_log.count != 18 ||
        ship_log.special_calls != 1 || ship_log.special_player != 2 ||
        ship_log.special_planet != 2;

    /* Race 6 sets the homeworld starbase state after its first ship. */
    memset(&ship_log, 0, sizeof ship_log);
    players[0].race_type = 6;
    players[0].homeworld = 7;
    startup_input.player_count = 1;
    failures += !stars_generate_world_create_startup_ships(&startup_input);
    failures += ship_log.count != 2 || ship_log.special_calls != 1 ||
        players[0].ships_created != 3 ||
        ship_log.starbase_player != 0 ||
        ship_log.starbase_planet != 7;

    /* The post-startup design walk preserves each raw fallback list and
     * stops at the first part accepted by the typed lookup boundary. */
    memset(&ship_log, 0, sizeof ship_log);
    memset(designs, 0, sizeof designs);
    designs[0].part_count = 2;
    designs[0].parts[0].group = 1;
    designs[0].parts[0].subtype = 1;
    designs[0].parts[0].part = 15;
    designs[0].parts[1].group = 0x80;
    designs[0].parts[1].subtype = 1;
    designs[0].parts[1].part = 9;
    startup_input.player_count = 1;
    startup_input.create_special_startup = 0;
    startup_input.designs = designs;
    startup_input.design_count = 1;
    startup_input.environment_special = 'S';
    startup_input.part_available = probe_part_available;
    players[0].race_type = 0;
    players[0].homeworld = 0;
    failures += !stars_generate_world_create_startup_ships(&startup_input);
    failures += designs[0].parts[0].part != 4U ||
        designs[0].parts[1].part != 0U ||
        ship_log.part_query_count != 4 ||
        ship_log.part_queries[0] != 5 || ship_log.part_queries[1] != 4 ||
        ship_log.part_queries[2] != 2 || ship_log.part_queries[3] != 0;

    memset(&ship_log, 0, sizeof ship_log);
    startup_input.player_count = 1;
    startup_input.designs = 0;
    startup_input.design_count = 0;
    startup_input.part_available = 0;
    players[0].race_type = 8;
    players[0].homeworld = 6;
    failures += !stars_generate_world_create_startup_ships(&startup_input);
    failures += players[0].ships_created != 2 ||
        ship_log.scanner_player != 0 || ship_log.scanner_planet != 6 ||
        ship_log.scanner_level != 31;

    memset(&special_input, 0, sizeof special_input);
    memset(planets, 0, sizeof planets);
    memset(&layout, 0, sizeof layout);
    layout.galaxy_extent = 800;
    layout.point_count = 3;
    layout.points[0].x = 1000;
    layout.points[0].y = 1000;
    layout.point_ids[0] = 17;
    layout.points[1].x = 1125;
    layout.points[1].y = 1000;
    layout.point_ids[1] = 3;
    layout.points[2].x = 1300;
    layout.points[2].y = 1000;
    layout.point_ids[2] = 29;
    planets[0].owner = 0;
    planets[1].owner = -1;
    planets[2].owner = -1;
    planets[0].environment[0] = 50;
    planets[0].environment[1] = 50;
    planets[0].environment[2] = 50;
    planets[0].environment_original[0] = 50;
    planets[0].environment_original[1] = 50;
    planets[0].environment_original[2] = 50;
    planets[0].population = 250L;
    planets[0].mineral_rate[0] = 100L;
    planets[0].mineral_rate[1] = 200L;
    planets[0].mineral_rate[2] = 300L;
    planets[1].mineral_concentration[0] = 11;
    planets[1].mineral_concentration[1] = 22;
    planets[1].mineral_concentration[2] = 33;
    ship_log.count = 0;
    special_context.random.value = 0x77889900UL;
    special_context.log = &ship_log;
    special_context.desirability_calls = 0;
    special_input.layout = &layout;
    special_input.planet_count = 3;
    special_input.player = 0;
    special_input.homeworld = 0;
    special_input.planets = planets;
    special_input.random_value = probe_special_zero_random;
    special_input.desirability = probe_desirability;
    special_input.create_ship = probe_create_special_ship;
    special_input.context = &special_context;
    special_input.special_planet = &special_planet;
    failures += !stars_generate_world_create_special_planet(&special_input);
    failures += special_planet != 1 || planets[1].owner != 0 ||
        special_context.desirability_calls != 2 || ship_log.count != 1 ||
        ship_log.designs[0] != 0 || ship_log.add_design[0] != 0 ||
        planets[0].population != 200L || planets[1].population != 100L ||
        planets[1].mineral_rate[0] != 100L ||
        planets[1].mineral_rate[1] != 100L ||
        planets[1].mineral_rate[2] != 100L ||
        planets[1].mine_count != 10 || planets[1].factory_count != 4 ||
        planets[1].defense_count != 0 || planets[1].production_slot != 1 ||
        planets[1].mineral_concentration[0] != 11 ||
        planets[1].mineral_concentration[1] != 22 ||
        planets[1].mineral_concentration[2] != 33;

    memset(&file_log, 0, sizeof file_log);
    memset(&file_input, 0, sizeof file_input);
    file_input.layout = &layout;
    file_input.player_count = 2;
    file_input.base_name = "WORLD";
    file_input.game_state = &layout;
    file_input.open_file = probe_open_file;
    file_input.write_record = probe_write_record;
    file_input.close_stream = probe_close_stream;
    file_input.write_data_file = probe_write_data_file;
    file_input.context = &file_log;
    failures += !stars_generate_world_write_files(&file_input);
    failures += file_log.opened != 1 || file_log.closed != 1 ||
        file_log.records != 5 || file_log.first_record_type != 7 ||
        file_log.first_record_size != 0x40 || file_log.data_files != 3 ||
        ((file_log.packed[1] >> 22) & 0x3ffUL) != 17UL ||
        ((file_log.packed[2] >> 22) & 0x3ffUL) != 3UL;

    memset(&finalize_input, 0, sizeof finalize_input);
    memset(&finalize_log, 0, sizeof finalize_log);
    memset(load_name, 0, sizeof load_name);
    finalize_input.base_name = "WORLD";
    finalize_input.load_name = load_name;
    finalize_input.load_name_capacity = sizeof load_name;
    finalize_input.set_player = probe_set_player;
    finalize_input.reset_message_log = probe_reset_log;
    finalize_input.create_windows = probe_create_windows;
    finalize_input.destroy_game = probe_destroy_game;
    finalize_input.format_load_name = probe_format_load_name;
    finalize_input.load_game = probe_load_game;
    finalize_input.send_message = probe_send_message;
    finalize_input.context = &finalize_log;
    failures += !stars_generate_world_finalize(&finalize_input);
    failures += finalize_log.set_player != -1 ||
        finalize_log.reset_log != 1 || finalize_log.created_windows != 1;
    memset(&finalize_log, 0, sizeof finalize_log);
    finalize_input.hosted_game = 1;
    finalize_input.selected_player = 2;
    failures += !stars_generate_world_finalize(&finalize_input);
    failures += finalize_log.destroyed_game != 1 ||
        finalize_log.formatted_name != 1 || finalize_log.loaded_game != 1 ||
        finalize_log.set_player != 2 || finalize_log.created_windows != 1 ||
        finalize_log.sent_message != 1;
    memset(&finalize_input, 0, sizeof finalize_input);
    finalize_input.batch_mode = 1;
    failures += !stars_generate_world_finalize(&finalize_input);

    memset(&battle_input, 0, sizeof battle_input);
    memset(&battle_log, 0, sizeof battle_log);
    memset(battle_storage, 0, sizeof battle_storage);
    memset(battle_counts, 0, sizeof battle_counts);
    battle_input.player_count = 2;
    battle_input.plan_stride = 256;
    battle_input.plan_size = 36;
    battle_input.plan_storage = battle_storage;
    battle_input.plan_counts = battle_counts;
    planets[0].owner = -1;
    planets[0].mineral_rate[0] = 101L;
    planets[0].mineral_rate[1] = 202L;
    planets[0].mineral_rate[2] = 303L;
    battle_input.planet_count = 1;
    battle_input.planets = planets;
    battle_input.init_plan = probe_init_battle_plan;
    battle_input.context = &battle_log;
    failures += !stars_generate_world_init_battle_plans(&battle_input);
    failures += battle_counts[0] != 5 || battle_counts[1] != 5 ||
        battle_log.calls != 10 || battle_log.templates[0] != 0 ||
        battle_log.templates[4] != 4 || battle_log.players[5] != 1 ||
        battle_storage[256] != 0 || planets[0].mineral_rate[0] != 0L ||
        planets[0].mineral_rate[1] != 0L ||
        planets[0].mineral_rate[2] != 0L;

    memset(&wormhole_context, 0, sizeof wormhole_context);
    memset(&wormhole_input, 0, sizeof wormhole_input);
    wormhole_input.pair_count = 1;
    wormhole_input.galaxy_extent = 200;
    wormhole_input.max_attempts = 100;
    wormhole_input.random_value = probe_wormhole_random;
    wormhole_input.create_wormhole = probe_create_wormhole;
    wormhole_input.set_point = probe_set_wormhole_point;
    wormhole_input.validate_wormhole = probe_validate_wormhole;
    wormhole_input.link_wormholes = probe_link_wormholes;
    wormhole_context.random.value = 0x99aabbccUL;
    wormhole_input.context = &wormhole_context;
    failures += !stars_generate_world_create_wormholes(&wormhole_input);
    failures += wormhole_context.log.created != 2 ||
        wormhole_context.log.linked != 1 ||
        wormhole_context.log.set_points != 2 ||
        wormhole_context.log.validated != 2 ||
        wormhole_context.log.directions[0] > 2 ||
        wormhole_context.log.directions[1] > 2 ||
        wormhole_context.log.points[0].x < 1000 ||
        wormhole_context.log.points[1].x < 1000;

    memset(&status_input, 0, sizeof status_input);
    memset(&message_log, 0, sizeof message_log);
    players[0].is_dead = 1;
    players[0].is_computer = 0;
    players[1].is_computer = 0;
    players[2].is_computer = 1;
    players[2].computer_type = 3;
    players[3].is_computer = 1;
    players[3].computer_type = 7;
    status_input.player_count = 4;
    status_input.players = players;
    status_input.send_message = probe_send_player_message;
    status_input.context = &message_log;
    status_input.selected_player = &selected_player;
    status_input.single_player_mode = &single_player_mode;
    failures += !stars_generate_world_finalize_player_status(&status_input);
    failures += message_log.count != 2 || message_log.messages[0] != 0x117 ||
        message_log.messages[1] != 0x182 || message_log.recipients[1] != 1 ||
        selected_player != 0 || single_player_mode != 0 ||
        players[2].salt != 0x094dabeeL || players[0].relations[1] != 0;
    players[1].is_computer = 1;
    players[3].computer_type = 3;
    memset(&message_log, 0, sizeof message_log);
    failures += !stars_generate_world_finalize_player_status(&status_input);
    failures += message_log.count != 1 || single_player_mode != 1 ||
        players[0].relations[1] != 2 || players[1].salt != 0x094dabeeL;

    memset(&planet_input, 0, sizeof planet_input);
    memset(planets, 0, sizeof planets);
    random.value = 0x2468ace1UL;
    planet_input.planet_count = 12;
    planet_input.random_value = probe_random;
    planet_input.context = &random;
    planet_input.planets = planets;
    failures += !stars_generate_world_init_planets(&planet_input);
    for (i = 0; i < planet_input.planet_count; ++i) {
        failures += planets[i].owner != -1;
        failures += planets[i].environment[0] !=
            planets[i].environment_original[0];
        failures += planets[i].environment[1] !=
            planets[i].environment_original[1];
        failures += planets[i].environment[2] !=
            planets[i].environment_original[2];
    }

    memset(planets, 0, sizeof planets);
    random.value = 0x11223344UL;
    planet_input.world_flags = 1;
    failures += !stars_generate_world_init_planets(&planet_input);
    for (i = 0; i < planet_input.planet_count; ++i)
        failures += planets[i].mineral_concentration[0] != 100 ||
            planets[i].mineral_concentration[1] != 100 ||
            planets[i].mineral_concentration[2] != 100;

    memset(planets, 0, sizeof planets);
    planet_input.world_flags = 0x0020U;
    planet_input.planet_count = 1;
    planet_input.random_value = probe_zero_random;
    failures += !stars_generate_world_init_planets(&planet_input);
    failures += planets[0].mineral_rate[0] != 206L ||
        planets[0].mineral_rate[1] != 206L ||
        planets[0].mineral_rate[2] != 206L;

    {
        FILE *report;
        report = fopen("C:\\GW\\GWR.TXT", "w");
        if (!report) return 3;
        fprintf(report, "GENERATE_WORLD_CHECKS=44 FAILURES=%d RESULT=%s\n",
            (int)failures, failures == 0 ? "PASS" : "FAIL");
        fclose(report);
    }
#ifndef TARGET_NATIVE
    printf("GENERATE_WORLD_CHECKS=44 FAILURES=%d RESULT=%s\n",
        (int)failures, failures == 0 ? "PASS" : "FAIL");
#endif
    return failures == 0 ? 0 : 1;
}
