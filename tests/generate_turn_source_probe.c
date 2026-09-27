#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 events[128];
    StarsI16 event_count;
    StarsI16 stages;
    StarsI16 modes;
    StarsI16 logs;
    StarsI16 player_count;
    StarsI16 log_order[3];
    StarsI16 random_limits[3];
    StarsI16 random_values[3];
    StarsI16 random_calls;
    StarsI16 writes;
    StarsI16 cleanup;
    StarsI16 fail_log;
    StarsI16 append_errors;
} Trace;

enum {
    EV_LOAD_GAME = 1, EV_INITIALIZE, EV_RANDOM_BASE = 10,
    EV_LOAD_LOG_BASE = 20, EV_VALIDATE = 30, EV_VALIDATE_LOGS, EV_DO_ORDER_BASE = 40,
    EV_UNMARK, EV_MOVE_THINGS_BASE = 50, EV_MOVE_FLEETS, EV_DECAY,
    EV_BREED, EV_PRODUCE, EV_FUEL, EV_SWEEP, EV_HEAL, EV_AUTO_TERRAFORM,
    EV_REMOTE_TERRAFORM, EV_SPANK, EV_UPDATE_GUESSES, EV_MARK_BACKUP,
    EV_UPDATE_SCORES, EV_PREPARE_OUTPUT, EV_WRITE_PLAYER_BASE = 80, EV_WRITE_DATA_BASE = 100,
    EV_CLEANUP, EV_DESTROY
};

static void record(Trace *t, StarsI16 event)
{ if (t->event_count < 128) t->events[t->event_count++] = event; }

static StarsI16 expect_events(const Trace *t, const StarsI16 *expected,
    StarsI16 count)
{
    StarsI16 i;
    if (t->event_count != count) {
        fprintf(stderr, "event count: got %d expected %d\n",
            t->event_count, count);
        return 0;
    }
    for (i = 0; i < count; ++i) {
        if (t->events[i] != expected[i]) {
            fprintf(stderr, "event %d: got %d expected %d\n", i,
                t->events[i], expected[i]);
            return 0;
        }
    }
    return 1;
}

static StarsI16 load_game(void *p, StarsGenerateTurnState *state)
{ Trace *t = (Trace *)p; record(t, EV_LOAD_GAME); state->player_count = t->player_count ? t->player_count : 3; return 1; }
static StarsI16 load_log(void *p, StarsGenerateTurnState *state,
    StarsI16 player)
{ Trace *t = (Trace *)p; (void)state; record(t, (StarsI16)(EV_LOAD_LOG_BASE + player)); if (t->logs < 3) t->log_order[t->logs] = player; ++t->logs; return !(t->fail_log && player == 1); }
static StarsI16 random_value(void *p, StarsI16 limit)
{ Trace *t = (Trace *)p; StarsI16 i = t->random_calls++; record(t, (StarsI16)(EV_RANDOM_BASE + limit)); if (i >= 3) return -1; t->random_limits[i] = limit; return t->random_values[i]; }
#define STAGE_CALLBACK(name, event) \
    static void name(void *p, StarsGenerateTurnState *state) \
    { Trace *t = (Trace *)p; (void)state; ++t->stages; record(t, event); }
STAGE_CALLBACK(destroy_game, EV_DESTROY)
STAGE_CALLBACK(initialize_turn, EV_INITIALIZE)
STAGE_CALLBACK(validate_waypoints, EV_VALIDATE)
STAGE_CALLBACK(validate_player_logs, EV_VALIDATE_LOGS)
STAGE_CALLBACK(move_fleets, EV_MOVE_FLEETS)
STAGE_CALLBACK(unmark_mine_fields, EV_UNMARK)
STAGE_CALLBACK(thing_decay, EV_DECAY)
STAGE_CALLBACK(breed_colonists, EV_BREED)
STAGE_CALLBACK(produce, EV_PRODUCE)
STAGE_CALLBACK(fuel_fleets, EV_FUEL)
STAGE_CALLBACK(sweep_for_mines, EV_SWEEP)
STAGE_CALLBACK(heal_ships, EV_HEAL)
STAGE_CALLBACK(auto_terraform, EV_AUTO_TERRAFORM)
STAGE_CALLBACK(remote_terraforming, EV_REMOTE_TERRAFORM)
STAGE_CALLBACK(spank_cheaters, EV_SPANK)
STAGE_CALLBACK(update_guesses, EV_UPDATE_GUESSES)
STAGE_CALLBACK(update_scores, EV_UPDATE_SCORES)
STAGE_CALLBACK(prepare_output_files, EV_PREPARE_OUTPUT)
static void mark_backup(void *p, StarsGenerateTurnState *state)
{
    Trace *t = (Trace *)p;
    ++t->stages; record(t, EV_MARK_BACKUP);
    state->player_file_append[0] = 1U;
    state->player_file_append[2] = 1U;
}
#undef STAGE_CALLBACK
static void mode(void *p, StarsGenerateTurnState *state, StarsI16 value)
{ Trace *t = (Trace *)p; (void)state; ++t->modes; record(t, (StarsI16)(EV_DO_ORDER_BASE + value)); }
static void move_things(void *p, StarsGenerateTurnState *state,
    StarsI16 value)
{ Trace *t = (Trace *)p; (void)state; ++t->modes; record(t, (StarsI16)(EV_MOVE_THINGS_BASE + value)); }
static void write_file(void *p, StarsGenerateTurnState *state,
    StarsI16 player, StarsI16 missing)
{ Trace *t = (Trace *)p; (void)state; (void)missing; ++t->writes; record(t, (StarsI16)(EV_WRITE_PLAYER_BASE + player + 1)); }
static void write_data(void *p, StarsGenerateTurnState *state,
    StarsI16 player, StarsI16 missing)
{ Trace *t = (Trace *)p; (void)state; if (missing != (player == 0 || player == 2)) ++t->append_errors; ++t->writes; record(t, (StarsI16)(EV_WRITE_DATA_BASE + player + 1)); }
static void progress(void *p, StarsI16 value)
{ (void)p; (void)value; }
static void cleanup(void *p, StarsGenerateTurnState *state)
{ Trace *t = (Trace *)p; (void)state; ++t->cleanup; record(t, EV_CLEANUP); }

static void install(StarsGenerateTurnOps *ops)
{
    memset(ops, 0, sizeof(*ops));
    ops->load_game = load_game; ops->load_player_log = load_log;
    ops->destroy_game = destroy_game;
    ops->initialize_turn = initialize_turn;
    ops->validate_waypoints = validate_waypoints;
    ops->validate_player_logs = validate_player_logs;
    ops->do_orders = mode; ops->move_things = move_things;
    ops->move_fleets = move_fleets; ops->unmark_mine_fields = unmark_mine_fields;
    ops->thing_decay = thing_decay; ops->breed_colonists = breed_colonists;
    ops->produce = produce; ops->fuel_fleets = fuel_fleets;
    ops->sweep_for_mines = sweep_for_mines; ops->heal_ships = heal_ships;
    ops->auto_terraform = auto_terraform;
    ops->remote_terraforming = remote_terraforming;
    ops->spank_cheaters = spank_cheaters;
    ops->update_guesses = update_guesses;
    ops->update_scores = update_scores; ops->mark_backup = mark_backup;
    ops->prepare_output_files = prepare_output_files;
    ops->write_player_file = write_file; ops->write_data_file = write_data;
    ops->progress = progress; ops->cleanup = cleanup;
    ops->random_value = random_value;
}

int main(void)
{
    StarsGenerateTurnOps ops;
    StarsGenerateTurnState state;
    Trace trace;
    StarsU8 design[147];
    static const StarsI16 success_events[] = {
        EV_DESTROY, EV_LOAD_GAME, EV_INITIALIZE, EV_RANDOM_BASE + 3,
        EV_RANDOM_BASE + 2, EV_RANDOM_BASE + 1,
        EV_LOAD_LOG_BASE + 2, EV_LOAD_LOG_BASE + 0, EV_LOAD_LOG_BASE + 1,
        EV_VALIDATE_LOGS, EV_VALIDATE, EV_DO_ORDER_BASE + 0, EV_UNMARK,
        EV_MOVE_THINGS_BASE + 0, EV_MOVE_FLEETS, EV_DECAY, EV_BREED,
        EV_PRODUCE, EV_MOVE_THINGS_BASE + 1, EV_FUEL,
        EV_DO_ORDER_BASE + 1, EV_SWEEP, EV_HEAL, EV_AUTO_TERRAFORM,
        EV_REMOTE_TERRAFORM, EV_SPANK, EV_VALIDATE, EV_UPDATE_GUESSES,
        EV_MARK_BACKUP, EV_UPDATE_SCORES, EV_PREPARE_OUTPUT, EV_WRITE_PLAYER_BASE + 1,
        EV_WRITE_PLAYER_BASE + 2, EV_WRITE_PLAYER_BASE + 3,
        EV_WRITE_PLAYER_BASE + 0, EV_WRITE_DATA_BASE + 1,
        EV_WRITE_DATA_BASE + 2, EV_WRITE_DATA_BASE + 3,
        EV_WRITE_DATA_BASE + 0, EV_CLEANUP
    };
    static const StarsI16 failed_log_events[] = {
        EV_DESTROY, EV_LOAD_GAME, EV_INITIALIZE, EV_RANDOM_BASE + 3,
        EV_RANDOM_BASE + 2, EV_RANDOM_BASE + 1,
        EV_LOAD_LOG_BASE + 2, EV_LOAD_LOG_BASE + 0,
        EV_LOAD_LOG_BASE + 1, EV_CLEANUP
    };

    memset(design, 0, sizeof design);
    design[58] = 4U; design[60] = 9U;
    stars_prepare_turn_design(design);
    if (design[58] != 1U || design[59] || design[60] != 1U || design[61] != 1U) return 20;
    design[58] = 4U; design[61] = 7U;
    stars_prepare_turn_design(design);
    if (design[60] != 1U || design[61] != 7U) return 21;
    design[58] = 4U; design[124] = 2U;
    stars_prepare_turn_design(design);
    if (design[58] != 4U) return 22;
    design[58] = 1U; design[124] = 0U; design[60] = 9U;
    stars_prepare_turn_design(design);
    if (design[60] != 9U) return 23;
    if (stars_prepare_turn_fleet_flags(0xc407U, 1U, 0x0100U) != 0x0407U ||
            stars_prepare_turn_fleet_flags(0x4407U, 1U, 0x0200U) != 0x8407U ||
            stars_prepare_turn_fleet_flags(0xc407U, 2U, 0x0200U) != 0x0407U) return 24;
    memset(&state, 0, sizeof state); memset(&trace, 0, sizeof trace);
    install(&ops); ops.context = &trace;
    trace.random_values[0] = 2; trace.random_values[1] = 1;
    trace.random_values[2] = 0;
    if (!stars_generate_turn(&state, &ops)) return 1;
    if (trace.append_errors || !state.generated || !state.output_written || state.turn != 1 ||
            trace.logs != 3 || trace.writes != 8 || trace.cleanup != 1 ||
            trace.modes != 4 || trace.stages < 15 ||
            trace.random_calls != 3 || state.player_order_count != 3 ||
            state.player_order[0] != 2 || state.player_order[1] != 0 ||
            state.player_order[2] != 1 || trace.log_order[0] != 2 ||
            trace.log_order[1] != 0 || trace.log_order[2] != 1 ||
            trace.random_limits[0] != 3 || trace.random_limits[1] != 2 ||
            trace.random_limits[2] != 1 ||
            !expect_events(&trace, success_events,
                (StarsI16)(sizeof success_events / sizeof success_events[0])))
        return 2;
    memset(&state, 0, sizeof state); memset(&trace, 0, sizeof trace);
    trace.fail_log = 1; trace.random_values[0] = 2;
    trace.random_values[1] = 1; trace.random_values[2] = 0;
    ops.context = &trace;
    if (stars_generate_turn(&state, &ops) != 0 || state.generated ||
            trace.logs != 3 || trace.writes != 0 || trace.cleanup != 1 ||
            trace.random_calls != 3 || trace.log_order[2] != 1) return 3;
    if (!expect_events(&trace, failed_log_events,
            (StarsI16)(sizeof failed_log_events / sizeof failed_log_events[0])))
        return 7;
    memset(&state, 0, sizeof state); memset(&trace, 0, sizeof trace);
    trace.player_count = 2; trace.random_values[0] = 1;
    trace.random_values[1] = 0; ops.context = &trace;
    if (!stars_generate_turn(&state, &ops) || trace.append_errors || trace.random_calls != 2 ||
            trace.random_limits[0] != 2 || trace.random_limits[1] != 1 ||
            state.player_order_count != 2 || state.player_order[0] != 1 ||
            state.player_order[1] != 0 || trace.logs != 2 ||
            trace.log_order[0] != 1 || trace.log_order[1] != 0) return 4;
    memset(&state, 0, sizeof state); memset(&trace, 0, sizeof trace);
    trace.random_values[0] = 3; ops.context = &trace;
    if (stars_generate_turn(&state, &ops) != 0 || state.generated ||
            trace.random_calls != 1 || trace.logs != 0 ||
            trace.cleanup != 1 || trace.writes != 0) return 5;
    memset(&state, 0, sizeof state); memset(&trace, 0, sizeof trace);
    install(&ops); ops.context = &trace; ops.random_value = 0;
    if (stars_generate_turn(&state, &ops) != 0 || state.generated ||
            trace.random_calls != 0 || trace.logs != 0 ||
            trace.cleanup != 1 || trace.writes != 0) return 6;
    puts("GENERATE_TURN_SOURCE=PASS");
    return 0;
}
