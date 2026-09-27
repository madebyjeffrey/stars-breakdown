#ifndef TARGET_NATIVE
#include <stdio.h>
#endif
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsLoadGameRecord game[3];
    StarsLoadGameRecord player_file[8];
    StarsLoadGameRecord main_file[16];
    StarsU8 header[64];
    StarsU8 player_data[64];
    StarsU8 plan[4];
    StarsU8 turn_end[16];
    StarsU16 stream;
    StarsU16 index;
    StarsU16 current_file;
    StarsU16 main_count;
    StarsU16 opens[4];
    StarsI16 players[4];
    StarsU16 closes;
    StarsU16 plans;
    StarsU16 battles;
    StarsU16 players_read;
    StarsU16 prepared_turns;
    StarsU16 messages;
    StarsU16 planets;
    StarsU16 snapshot_planets;
    StarsU16 planet_turns;
    StarsU16 production;
    StarsU16 designs;
    StarsU16 fleets;
    StarsU16 scores;
    StarsU16 things;
    StarsU16 battle_plans;
    StarsU16 turns;
    StarsU16 post_loads;
    StarsU16 post_stages;
    StarsU16 logs_loaded;
    StarsU16 logs_run;
    StarsU16 counts_rebuilt;
    StarsU16 mru_updates;
    StarsU16 cleanup_failures;
    StarsU16 alerts;
} Trace;

static StarsI16 open_file(void *opaque, StarsU16 type, StarsI16 player,
    StarsU16 mode)
{
    Trace *t = (Trace *)opaque;
    (void)mode;
    t->opens[t->stream] = type;
    t->players[t->stream] = player;
    t->current_file = type;
    ++t->stream;
    t->index = 0;
    return 1;
}

static StarsI16 next_record(void *opaque, StarsLoadGameRecord *record)
{
    Trace *t = (Trace *)opaque;
    if (t->current_file == 0U) {
        if (t->index >= 3U) return 0;
        *record = t->game[t->index++];
        return 1;
    }
    if (t->current_file == 4U) {
        if (t->index >= 8U) return 0;
        *record = t->player_file[t->index++];
        return 1;
    }
    if (t->index >= (t->main_count ? t->main_count : 13U)) return 0;
    *record = t->main_file[t->index++];
    return 1;
}

static void close_file(void *opaque)
{
    ++((Trace *)opaque)->closes;
}

static StarsI16 decode_header(void *opaque, const StarsU8 *data,
    StarsU16 length, StarsLoadGameState *state)
{
    (void)opaque;
    if (length != 64U || data[0] != 0x42U) return 0;
    state->player_count = 2;
    state->plan_count = 1;
    state->map_size = 400;
    return 1;
}

static void store_plan(void *opaque, StarsU16 index,
    const StarsLoadGamePlan *plan)
{
    Trace *t = (Trace *)opaque;
    if (index != 0U || plan->x != 1005 || plan->y != 10 || plan->id != 7)
        ++t->alerts;
    ++t->plans;
}

static void reset_runtime(void *opaque, StarsU16 players, StarsI16 player)
{
    (void)opaque;
    if (players != 2U || player != -1) return;
}

static StarsI16 decode_player_header(void *opaque, const StarsU8 *data,
    StarsU16 length, StarsLoadGameState *state)
{
    (void)opaque;
    if (!data || length != 64U) return 0;
    state->current_planet_count = 1;
    return 1;
}

static StarsI16 planet_snapshot(void *opaque, StarsI16 player,
    const StarsLoadGameRecord *record)
{
    Trace *t = (Trace *)opaque;
    (void)player;
    (void)record;
    ++t->snapshot_planets;
    return 1;
}

static StarsI16 locate_planet(void *opaque, StarsU16 id, StarsU16 *slot,
    StarsI16 *existing)
{
    (void)opaque;
    if (!slot || !existing || id != 0x42U) return 0;
    *slot = 0;
    *existing = 0;
    return 1;
}

static StarsI16 planet_turn(void *opaque,
    const StarsLoadGamePlanetInput *input)
{
    Trace *t = (Trace *)opaque;
    if (!input || input->id != 0x42U || input->slot != 0U ||
            input->preinitialized != 0 || input->history != 0)
        ++t->alerts;
    ++t->planet_turns;
    return 1;
}

static StarsI16 player_turn(void *opaque,
    const StarsLoadGamePlayerInput *input)
{
    Trace *t = (Trace *)opaque;
    if (!input || input->player_index != 0U || !input->record)
        ++t->alerts;
    ++t->players_read;
    return 1;
}

static void prepare_turn(void *opaque, StarsU16 turn_count)
{
    Trace *t = (Trace *)opaque;
    if (turn_count != 1U) ++t->alerts;
    ++t->prepared_turns;
}

static StarsI16 handle(void *opaque, StarsI16 player,
    const StarsLoadGameRecord *record)
{
    Trace *t = (Trace *)opaque;
    (void)player;
    switch (record->type) {
    case 0x1f: ++t->battles; break;
    case 6: ++t->players_read; break;
    case 0x21: ++t->messages; break;
    case 0x0e: ++t->planets; break;
    case 0x24: ++t->planets; break;
    case 0x1c: ++t->production; break;
    case 0x1a: ++t->designs; break;
    case 0x10: ++t->fleets; break;
    case 0x2d: ++t->scores; break;
    case 0x2b: ++t->things; break;
    case 0x1e: ++t->battle_plans; break;
    default: break;
    }
    return 1;
}

static StarsI16 check_password(void *opaque, StarsI16 player)
{
    (void)opaque;
    (void)player;
    return 1;
}

static void finish_turn(void *opaque, StarsU16 turn, StarsU16 flags,
    StarsU16 count)
{
    Trace *t = (Trace *)opaque;
    if (turn != 12U || flags != 0x0020U || count != 1U) ++t->alerts;
    ++t->turns;
}

static StarsI16 post_load(void *opaque, const StarsU8 *file_name,
    const StarsU8 *extension, const StarsLoadGameState *state)
{
    Trace *t = (Trace *)opaque;
    if (!file_name || !extension || state->turn_count != 1U) return 0;
    ++t->post_loads;
    return 1;
}

static void post_stage(void *opaque, const StarsLoadGameState *state)
{
    Trace *t = (Trace *)opaque;
    if (!state || state->turn_count != 1U) ++t->alerts;
    ++t->post_stages;
}

static StarsI16 load_log(void *opaque, const StarsU8 *file_name,
    const StarsU8 *extension)
{
    Trace *t = (Trace *)opaque;
    if (!file_name || !extension) ++t->alerts;
    ++t->logs_loaded;
    return 1;
}

static StarsI16 run_log(void *opaque, const StarsU8 *file_name,
    const StarsU8 *extension)
{
    Trace *t = (Trace *)opaque;
    if (!file_name || !extension) ++t->alerts;
    ++t->logs_run;
    return 1;
}

static void rebuild_counts(void *opaque, const StarsLoadGameState *state)
{
    Trace *t = (Trace *)opaque;
    if (!state || state->turn_count != 1U) ++t->alerts;
    ++t->counts_rebuilt;
}

static void update_mru(void *opaque, const StarsU8 *file_name,
    const StarsU8 *extension)
{
    Trace *t = (Trace *)opaque;
    if (!file_name || !extension) ++t->alerts;
    ++t->mru_updates;
}

static void cleanup_failure(void *opaque, StarsI16 player)
{
    Trace *t = (Trace *)opaque;
    if (player != -1) ++t->alerts;
    ++t->cleanup_failures;
}

static void alert(void *opaque, StarsI16 message)
{
    (void)message;
    ++((Trace *)opaque)->alerts;
}

int main(void)
{
    Trace t;
    StarsLoadGameState state;
    StarsLoadGameOps ops;
    StarsU32 packed = 5UL | (10UL << 10) | (7UL << 22);
    memset(&t, 0, sizeof t);
    t.header[0] = 0x42;
    t.plan[0] = (StarsU8)packed;
    t.plan[1] = (StarsU8)(packed >> 8);
    t.plan[2] = (StarsU8)(packed >> 16);
    t.plan[3] = (StarsU8)(packed >> 24);
    t.turn_end[10] = 12;
    t.turn_end[14] = 0x20;
    t.game[0].type = 7; t.game[0].length = 64; t.game[0].data = t.header;
    t.game[1].type = 0xfffeU; t.game[1].length = 4; t.game[1].data = t.plan;
    t.game[2].type = 0; t.game[2].length = 0; t.game[2].data = 0;
    t.main_file[0].type = 0x1f; t.main_file[0].data = t.header;
    t.main_file[1].type = 6; t.main_file[1].length = 64;
    t.main_file[1].data = t.player_data;
    t.main_file[2].type = 0x24; t.main_file[2].data = t.header;
    t.main_file[3].type = 0x21; t.main_file[3].data = t.header;
    t.main_file[4].type = 0x24; t.main_file[4].length = 64;
    t.main_file[4].data = t.header;
    t.main_file[5].type = 0x1c; t.main_file[5].data = t.header;
    t.main_file[6].type = 0x1a; t.main_file[6].data = t.header;
    t.main_file[7].type = 0x10; t.main_file[7].data = t.header;
    t.main_file[8].type = 0x2d; t.main_file[8].data = t.header;
    t.main_file[9].type = 0x2b; t.main_file[9].data = t.header;
    t.main_file[10].type = 0x1e; t.main_file[10].data = t.header;
    t.main_file[11].type = 8; t.main_file[11].length = 16; t.main_file[11].data = t.turn_end;
    t.main_file[12].type = 0; t.main_file[12].data = 0;
    t.main_count = 13;
    memset(&ops, 0, sizeof ops);
    ops.open_file = open_file; ops.next_record = next_record;
    ops.close_file = close_file; ops.decode_game_header = decode_header;
    ops.store_plan = store_plan; ops.reset_runtime = reset_runtime;
    ops.decode_player_header = decode_player_header;
    ops.planet_snapshot = planet_snapshot;
    ops.locate_planet = locate_planet; ops.planet_turn = planet_turn;
    ops.player_turn = player_turn; ops.prepare_turn = prepare_turn;
    ops.battle = handle; ops.player_record = handle; ops.messages = handle;
    ops.planet = handle; ops.production = handle; ops.ship_design = handle;
    ops.fleet = handle; ops.score = handle; ops.thing = handle;
    ops.battle_plan = handle; ops.check_password = check_password;
    ops.finish_turn = finish_turn; ops.post_load = post_load;
    ops.alert = alert; ops.cleanup_failure = cleanup_failure;
    ops.notify_thing_warps = post_stage;
    ops.notify_production = post_stage; ops.notify_battles = post_stage;
    ops.load_log = load_log; ops.run_log = run_log;
    ops.rebuild_counts = rebuild_counts; ops.update_mru = update_mru;
    if (!stars_load_game((const StarsU8 *)"game", (const StarsU8 *)"hs",
            &state, &ops, &t)) return 1;
    if (t.opens[0] != 0U || t.opens[1] != 2U || t.closes != 2U ||
            t.plans != 1U || t.battles != 1U || t.players_read != 1U ||
            t.messages != 1U || t.planets != 0U || t.production != 1U ||
            t.designs != 1U || t.fleets != 1U || t.scores != 1U ||
            t.things != 1U || t.battle_plans != 1U || t.turns != 1U ||
            t.prepared_turns != 1U ||
            t.snapshot_planets != 0U || t.planet_turns != 1U ||
            t.post_loads != 1U || t.post_stages != 0U ||
            t.logs_loaded != 1U || t.logs_run != 1U ||
            t.counts_rebuilt != 1U || t.mru_updates != 0U ||
            t.alerts != 0U || state.turn_count != 1U) return 1;
    memset(&t, 0, sizeof t);
    t.header[0] = 0x42;
    t.plan[0] = (StarsU8)packed;
    t.plan[1] = (StarsU8)(packed >> 8);
    t.plan[2] = (StarsU8)(packed >> 16);
    t.plan[3] = (StarsU8)(packed >> 24);
    t.turn_end[10] = 12;
    t.turn_end[14] = 0x20;
    t.game[0].type = 7; t.game[0].length = 64; t.game[0].data = t.header;
    t.game[1].type = 0xfffeU; t.game[1].length = 4; t.game[1].data = t.plan;
    t.game[2].type = 0; t.game[2].data = 0;
    t.player_file[0].type = 0x20; t.player_file[0].length = 64;
    t.player_file[0].data = t.header;
    t.player_file[1].type = 0x0e; t.player_file[1].data = t.header;
    t.player_file[2].type = 6; t.player_file[2].length = 64;
    t.player_file[2].data = t.player_data;
    t.player_file[3].type = 0x1a; t.player_file[3].data = t.header;
    t.player_file[4].type = 0x2d; t.player_file[4].data = t.header;
    t.player_file[5].type = 0x29; t.player_file[5].data = t.header;
    t.player_file[6].type = 0x2b; t.player_file[6].data = t.header;
    t.player_file[7].type = 0; t.player_file[7].data = 0;
    t.main_file[0].type = 6; t.main_file[0].length = 64;
    t.main_file[0].data = t.player_data;
    t.main_file[1].type = 0x24; t.main_file[1].length = 64;
    t.main_file[1].data = t.header;
    t.main_file[2].type = 8; t.main_file[2].length = 16; t.main_file[2].data = t.turn_end;
    t.main_file[3].type = 0; t.main_file[3].data = 0;
    t.main_count = 4;
    if (!stars_load_game((const StarsU8 *)"game", (const StarsU8 *)"p1",
            &state, &ops, &t)) return 1;
    if (t.opens[0] != 0U || t.opens[1] != 4U || t.opens[2] != 0x3003U ||
            t.players[0] != -1 || t.players[1] != 0 || t.players[2] != 0 ||
            t.closes != 3U || t.plans != 1U || t.players_read != 2U ||
            t.planets != 0U || t.snapshot_planets != 1U ||
            t.planet_turns != 1U || t.designs != 1U || t.scores != 1U ||
            t.things != 1U || t.turns != 1U || t.prepared_turns != 1U ||
            t.post_loads != 1U ||
            t.post_stages != 3U || t.logs_loaded != 1U || t.logs_run != 1U ||
            t.counts_rebuilt != 1U || t.mru_updates != 1U ||
            t.alerts != 0U ||
            state.player != 0 || state.turn_count != 1U) return 1;

    /* A physical EOF immediately after a valid turn is normal in the
     * original loader.  This case has no synthetic type-0 record. */
    memset(&t, 0, sizeof t);
    t.header[0] = 0x42;
    t.plan[0] = (StarsU8)packed;
    t.plan[1] = (StarsU8)(packed >> 8);
    t.plan[2] = (StarsU8)(packed >> 16);
    t.plan[3] = (StarsU8)(packed >> 24);
    t.turn_end[10] = 12;
    t.turn_end[14] = 0x20;
    t.game[0].type = 7; t.game[0].length = 64; t.game[0].data = t.header;
    t.game[1].type = 0xfffeU; t.game[1].length = 4; t.game[1].data = t.plan;
    t.game[2].type = 0; t.game[2].data = 0;
    t.main_file[0].type = 8; t.main_file[0].length = 16;
    t.main_file[0].data = t.turn_end;
    t.main_count = 1;
    if (!stars_load_game((const StarsU8 *)"game", (const StarsU8 *)"hs",
            &state, &ops, &t)) return 1;
    if (t.opens[0] != 0U || t.opens[1] != 2U || t.closes != 2U ||
            t.turns != 1U || t.prepared_turns != 1U ||
            t.post_loads != 1U || t.post_stages != 0U ||
            t.logs_loaded != 1U || t.logs_run != 1U ||
            t.counts_rebuilt != 1U || t.mru_updates != 0U ||
            t.alerts != 0U || t.cleanup_failures != 0U) return 1;

    /* A wrong first turn record follows the parser's one-alert cleanup path;
     * cleanup must close the main stream and must not invoke post-load work. */
    memset(&t, 0, sizeof t);
    t.header[0] = 0x42;
    t.plan[0] = (StarsU8)packed;
    t.plan[1] = (StarsU8)(packed >> 8);
    t.plan[2] = (StarsU8)(packed >> 16);
    t.plan[3] = (StarsU8)(packed >> 24);
    t.game[0].type = 7; t.game[0].length = 64; t.game[0].data = t.header;
    t.game[1].type = 0xfffeU; t.game[1].length = 4; t.game[1].data = t.plan;
    t.game[2].type = 0; t.game[2].data = 0;
    t.main_file[0].type = 7; t.main_file[0].data = t.header;
    t.main_count = 1;
    if (stars_load_game((const StarsU8 *)"game", (const StarsU8 *)"hs",
            &state, &ops, &t)) return 1;
    if (t.opens[0] != 0U || t.opens[1] != 2U || t.closes != 2U ||
            t.prepared_turns != 1U || t.cleanup_failures != 1U ||
            t.post_loads != 0U ||
            t.post_stages != 0U || t.alerts != 1U) return 1;
#ifndef TARGET_NATIVE
    printf("LOAD_GAME_CHECKS=19 FAILURES=0 RESULT=PASS\n");
#endif
    return 0;
}
