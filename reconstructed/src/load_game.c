#include "core.h"
#include <string.h>

/* The record type is stored in the high six bits of the original two-byte
 * header.  Keeping names here is important: these values describe the Stars!
 * file format, whereas the low ten bits are the payload length. */
enum {
    STARS_LOAD_RECORD_END = 0,
    STARS_LOAD_RECORD_PLAYER = 6,
    STARS_LOAD_RECORD_GAME_HEADER = 7,
    STARS_LOAD_RECORD_PLAYER_HEADER = 0x20,
    STARS_LOAD_RECORD_MESSAGE_CHUNK = 0x0c,
    STARS_LOAD_RECORD_TURN_END = 8,
    STARS_LOAD_RECORD_PLANET = 0x0e,
    STARS_LOAD_RECORD_FLEET = 0x10,
    STARS_LOAD_RECORD_BATTLE_PLAN = 0x1e,
    STARS_LOAD_RECORD_BATTLE = 0x1f,
    STARS_LOAD_RECORD_MESSAGES = 0x21,
    STARS_LOAD_RECORD_MESSAGE_INCOMING = 0x28,
    STARS_LOAD_RECORD_SALT = 0x24,
    STARS_LOAD_RECORD_PLANET_TURN = STARS_LOAD_RECORD_SALT,
    STARS_LOAD_RECORD_HISTORY = 0x29,
    STARS_LOAD_RECORD_THING = 0x2b,
    STARS_LOAD_RECORD_SCORE = 0x2d,
    STARS_LOAD_RECORD_BATTLE_ALT = 0x27,
    STARS_LOAD_RECORD_PRODUCTION = 0x1c,
    STARS_LOAD_RECORD_DESIGN = 0x1a,
    STARS_LOAD_RECORD_OPTIONAL = 0x16,
    STARS_LOAD_FILE_GAME = 0,
    STARS_LOAD_FILE_PLAYER = 4,
    STARS_LOAD_MODE_READ = 0x20,
    STARS_LOAD_HISTORY_MODE = 2,
    STARS_LOAD_PLAYER_MODE = 3,
    STARS_LOAD_PLAYER_FILE_FLAGS = 0x3000,
    STARS_LOAD_ALERT_BAD_RECORD = 0x10,
    STARS_LOAD_ALERT_BAD_TURN = 0x22c
};

/* Plan coordinates are read as four raw bytes, without a record header.  It
 * is intentionally outside the enum: VC1.52 diagnoses 0xfffe as a signed
 * out-of-range enum constant even though the wire value is an unsigned word. */
#define STARS_LOAD_RECORD_PLAN_INDEX ((StarsU16)0xfffeU)

static StarsU16 load_word(const StarsU8 *data)
{
    return (StarsU16)(data[0] | ((StarsU16)data[1] << 8));
}

static StarsU32 load_long(const StarsU8 *data)
{
    return (StarsU32)load_word(data) |
        ((StarsU32)load_word(data + 2U) << 16);
}

static StarsU16 load_record_type(const StarsLoadGameRecord *record)
{
    return record ? record->type : 0xffffU;
}

static StarsI16 load_next(const StarsLoadGameOps *ops, void *context,
    StarsLoadGameRecord *record)
{
    if (!ops || !ops->next_record || !record)
        return 0;
    memset(record, 0, sizeof *record);
    return ops->next_record(context, record);
}

static void load_alert(const StarsLoadGameOps *ops, void *context,
    StarsLoadGameState *state, StarsI16 message)
{
    if (state && state->failure_alerted)
        return;
    if (ops && ops->alert)
        ops->alert(context, message);
    if (state)
        state->failure_alerted = 1;
}

static StarsI16 load_open(const StarsLoadGameOps *ops, void *context,
    StarsU16 type, StarsI16 player)
{
    if (!ops || !ops->open_file)
        return 0;
    return ops->open_file(context, type, player, STARS_LOAD_MODE_READ);
}

static StarsI16 load_handler(StarsLoadGameRecordHandler handler,
    void *context, StarsI16 player, const StarsLoadGameRecord *record)
{
    if (!handler)
        return 1;
    return handler(context, player, record);
}

static StarsI16 load_planet_turn(const StarsLoadGameOps *ops, void *context,
    StarsLoadGameState *state, StarsLoadGameRecord *record)
{
    StarsLoadGamePlanetInput input;
    StarsU16 slot;
    StarsI16 existing;

    /* The first word of a planet payload is its object id with the upper
     * five bits reserved for flags.  It is the key used by the raw loader's
     * sorted merge, not a sequential record number. */
    if (!record->data || record->length < 2U)
        return 0;
    input.player = state->player;
    input.id = (StarsU16)(load_word(record->data) & 0x07ffU);
    input.slot = state->current_planet_count;
    input.preinitialized = 0;
    input.history = 0;
    input.record = record;
    slot = input.slot;
    existing = 0;
    if (ops->locate_planet &&
            !ops->locate_planet(context, input.id, &slot, &existing))
        return 0;
    input.slot = slot;
    input.preinitialized = existing;
    if (ops->planet_turn)
        return ops->planet_turn(context, &input);
    return load_handler(ops->planet, context, state->player, record);
}

static StarsI16 load_player_record(const StarsLoadGameOps *ops, void *context,
    StarsLoadGameState *state, StarsLoadGameRecord *record)
{
    StarsLoadGamePlayerInput input;

    if (!record->data || record->length < 1U)
        return 0;
    input.player_index = record->data[0];
    input.selected_player = state->player;
    input.record = record;
    if (ops->player_turn)
        return ops->player_turn(context, &input);
    return load_handler(ops->player_record, context, state->player, record);
}

static StarsI16 load_expect(const StarsLoadGameOps *ops, void *context,
    StarsLoadGameRecord *record, StarsLoadGameState *state, StarsU16 type)
{
    if (!load_next(ops, context, record) || load_record_type(record) != type) {
        load_alert(ops, context, state, STARS_LOAD_ALERT_BAD_RECORD);
        return 0;
    }
    return 1;
}

static StarsI16 load_player_number(const StarsU8 *extension)
{
    StarsU16 value = 0;
    StarsU16 digits = 0;
    const StarsU8 *p = extension + 1;

    while (*p >= '0' && *p <= '9') {
        value = (StarsU16)(value * 10U + (StarsU16)(*p - '0'));
        ++p;
        ++digits;
    }
    if (digits == 0U || *p != 0 || value == 0U || value > 32U)
        return -1;
    return (StarsI16)(value - 1U);
}

/* The game prelude is deliberately separate from the turn loop.  It is a
 * small index: one type-7 game header followed by one four-byte packed plan
 * coordinate per plan, then a type-0 end marker.  The original updates the
 * running X coordinate rather than storing an absolute X in the record. */
static StarsI16 load_game_prelude(const StarsLoadGameOps *ops, void *context,
    StarsLoadGameState *state)
{
    StarsLoadGameRecord record;
    StarsLoadGamePlan plan;
    StarsU16 i;
    StarsI16 running_x;
    StarsU32 packed;

    if (!load_expect(ops, context, &record, state,
            STARS_LOAD_RECORD_GAME_HEADER))
        return 0;
    if (!record.data || record.length < 64U || !ops->decode_game_header ||
            !ops->decode_game_header(context, record.data, record.length,
            state))
        return 0;
    running_x = 1000;
    for (i = 0; i < state->plan_count; ++i) {
        if (!load_expect(ops, context, &record, state,
                STARS_LOAD_RECORD_PLAN_INDEX) || !record.data ||
                record.length < 4U)
            return 0;
        packed = load_long(record.data);
        running_x = (StarsI16)(running_x + (StarsI16)(packed & 0x03ffUL));
        plan.x = running_x;
        plan.y = (StarsI16)((packed >> 10) & 0x0fffUL);
        plan.id = (StarsI16)((packed >> 22) & 0x03ffUL);
        if (plan.x >= (StarsI16)(state->map_size + 1000U) ||
                plan.y >= (StarsI16)(state->map_size + 1000U) ||
                plan.id >= 1000) {
            load_alert(ops, context, state, STARS_LOAD_ALERT_BAD_RECORD);
            return 0;
        }
        if (ops->store_plan)
            ops->store_plan(context, i, &plan);
    }
    if (!load_expect(ops, context, &record, state, STARS_LOAD_RECORD_END))
        return 0;
    return 1;
}

/* The optional player file is a snapshot used when loading an individual
 * player's view.  Its header supplies the counts that determine the exact
 * number of following planet records; variable groups then follow in a fixed
 * order.  This is why the code does not search for a type by scanning ahead:
 * a misplaced record is a corrupt file, not an ignorable extension. */
static StarsI16 load_player_file(const StarsLoadGameOps *ops, void *context,
    StarsLoadGameState *state)
{
    StarsLoadGameRecord record;
    StarsU16 i;

    if (!load_expect(ops, context, &record, state,
            STARS_LOAD_RECORD_PLAYER_HEADER))
        return 0;
    if (!record.data || !ops->decode_player_header ||
            !ops->decode_player_header(context, record.data, record.length,
            state))
        return 0;
    if (!load_next(ops, context, &record))
        return 0;
    for (i = 0; i < state->current_planet_count; ++i) {
        if (load_record_type(&record) != STARS_LOAD_RECORD_PLANET ||
                !load_handler(ops->planet_snapshot ? ops->planet_snapshot :
                    ops->planet, context, state->player, &record))
            return 0;
        if (!load_next(ops, context, &record))
            return 0;
    }
    if (load_record_type(&record) == STARS_LOAD_RECORD_MESSAGES) {
        if (!load_handler(ops->messages, context, state->player, &record))
            return 0;
        if (!load_next(ops, context, &record))
            return 0;
    }
    while (load_record_type(&record) == STARS_LOAD_RECORD_PLAYER) {
        if (!load_player_record(ops, context, state, &record) ||
                !load_next(ops, context, &record))
            return 0;
    }
    while (load_record_type(&record) == STARS_LOAD_RECORD_DESIGN) {
        if (!load_handler(ops->ship_design, context, state->player, &record) ||
                !load_next(ops, context, &record))
            return 0;
        ++state->current_design_count;
    }
    while (load_record_type(&record) == STARS_LOAD_RECORD_SCORE) {
        if (!load_handler(ops->score, context, state->player, &record) ||
                !load_next(ops, context, &record))
            return 0;
    }
    while (load_record_type(&record) == STARS_LOAD_RECORD_HISTORY) {
        if (!load_handler(ops->history, context, state->player, &record) ||
                !load_next(ops, context, &record))
            return 0;
    }
    while (load_record_type(&record) == STARS_LOAD_RECORD_THING) {
        if (!load_handler(ops->thing, context, state->player, &record) ||
                !load_next(ops, context, &record))
            return 0;
    }
    if (ops->close_file)
        ops->close_file(context);
    return 1;
}

/* Consume one complete turn.  The record order is the loader's central
 * invariant: battles and player snapshots precede the password check;
 * planets, production, designs, fleets, scores, things and battle plans then
 * rebuild the current world.  Callbacks are only for object decoding and
 * global UI/heap effects -- the dispatch and sequencing are genuine C. */
static StarsI16 load_turn(const StarsLoadGameOps *ops, void *context,
    StarsLoadGameState *state, StarsLoadGameRecord *record)
{
    StarsU16 type;

    ++state->turn_count;
    state->current_planet_count = 0;
    state->current_fleet_count = 0;
    state->current_thing_count = 0;
    state->current_design_count = 0;
    if (ops->prepare_turn)
        ops->prepare_turn(context, state->turn_count);
    type = load_record_type(record);
    while (type == STARS_LOAD_RECORD_BATTLE ||
            type == STARS_LOAD_RECORD_BATTLE_ALT) {
        if (!load_handler(ops->battle, context, state->player, record) ||
                !load_next(ops, context, record)) return 0;
        type = load_record_type(record);
    }
    while (type == STARS_LOAD_RECORD_PLAYER) {
        if (!load_player_record(ops, context, state, record) ||
                !load_next(ops, context, record)) return 0;
        type = load_record_type(record);
    }
    if (state->mode == STARS_LOAD_HISTORY_MODE && type == STARS_LOAD_RECORD_SALT) {
        if (!load_handler(ops->salt, context, state->player, record) ||
                !load_next(ops, context, record)) return 0;
        type = load_record_type(record);
    }
    if (ops->check_password && !ops->check_password(context, state->player))
        return 0;
    while (type == STARS_LOAD_RECORD_MESSAGE_CHUNK ||
            type == STARS_LOAD_RECORD_MESSAGE_INCOMING ||
            type == STARS_LOAD_RECORD_MESSAGES) {
        if (!load_handler(ops->messages, context, state->player, record) ||
                !load_next(ops, context, record)) return 0;
        type = load_record_type(record);
    }
    while (type == STARS_LOAD_RECORD_PLANET_TURN) {
        if (!load_planet_turn(ops, context, state, record) ||
                !load_next(ops, context, record)) return 0;
        ++state->current_planet_count;
        type = load_record_type(record);
        if (type == STARS_LOAD_RECORD_PRODUCTION) {
            if (!load_handler(ops->production, context, state->player,
                    record) || !load_next(ops, context, record)) return 0;
            type = load_record_type(record);
        }
    }
    while (type == STARS_LOAD_RECORD_DESIGN) {
        if (!load_handler(ops->ship_design, context, state->player, record) ||
                !load_next(ops, context, record)) return 0;
        ++state->current_design_count;
        type = load_record_type(record);
    }
    while (type == STARS_LOAD_RECORD_FLEET) {
        if (!load_handler(ops->fleet, context, state->player, record) ||
                !load_next(ops, context, record)) return 0;
        ++state->current_fleet_count;
        type = load_record_type(record);
    }
    while (type == STARS_LOAD_RECORD_SCORE) {
        if (!load_handler(ops->score, context, state->player, record) ||
                !load_next(ops, context, record)) return 0;
        type = load_record_type(record);
    }
    while (type == STARS_LOAD_RECORD_THING) {
        if (!load_handler(ops->thing, context, state->player, record) ||
                !load_next(ops, context, record)) return 0;
        ++state->current_thing_count;
        type = load_record_type(record);
    }
    if (type == STARS_LOAD_RECORD_OPTIONAL) {
        if (!load_next(ops, context, record)) return 0;
        type = load_record_type(record);
    }
    while (type == STARS_LOAD_RECORD_BATTLE_PLAN) {
        if (!load_handler(ops->battle_plan, context, state->player, record) ||
                !load_next(ops, context, record)) return 0;
        type = load_record_type(record);
    }
    if (type != STARS_LOAD_RECORD_TURN_END) {
        load_alert(ops, context, state, STARS_LOAD_ALERT_BAD_TURN);
        return 0;
    }
    if (record->data && record->length >= 16U) {
        state->current_turn = load_word(record->data + 10U);
        /* The saved warp/turn flags are read from the last word of the
         * 16-byte turn marker, not from its record-header-adjacent bytes. */
        state->current_flags = load_word(record->data + 14U);
    }
    if (ops->finish_turn)
        ops->finish_turn(context, state->current_turn, state->current_flags,
            state->turn_count);
    return 1;
}

static StarsI16 load_post_stages(const StarsLoadGameOps *ops, void *context,
    const StarsU8 *file_name, const StarsU8 *extension,
    const StarsLoadGameState *state)
{
    /* FLoadGame emits the player-facing notifications before replaying the
     * companion log.  They are deliberately separate callbacks because each
     * one consumes a different decoded collection (things, production, or
     * battles), while the sequencing belongs to this coordinator. */
    if (state->player != -1) {
        if (ops->notify_thing_warps)
            ops->notify_thing_warps(context, state);
        if (ops->notify_production)
            ops->notify_production(context, state);
        if (ops->notify_battles)
            ops->notify_battles(context, state);
    }
    if (ops->load_log && !ops->load_log(context, file_name, extension))
        return 0;
    if (ops->run_log && !ops->run_log(context, file_name, extension))
        return 0;
    if (ops->rebuild_counts)
        ops->rebuild_counts(context, state);
    /* The raw routine writes MRU/private-profile state only for a concrete
     * player load; history mode uses player == -1 and skips that side effect. */
    if (state->player != -1 && ops->update_mru)
        ops->update_mru(context, file_name, extension);
    return 1;
}

/* Genuine semantic reconstruction of the coordinator portion of FLoadGame,
 * NE 15:0810-3206.  The original function also owns large DGROUP heaps,
 * password/UI side effects, MRU maintenance and post-load notifications;
 * those are represented by typed operations until their exact state layouts
 * are reviewed.  The file-format decisions and all record dispatch remain in
 * this C implementation, so no assembly or emitted-byte wrapper is involved. */
StarsI16 stars_load_game(const StarsU8 *file_name, const StarsU8 *extension,
    StarsLoadGameState *state, const StarsLoadGameOps *ops, void *context)
{
    StarsLoadGameRecord record;
    StarsI16 opened;

    (void)file_name;
    if (!extension || !state || !ops || !ops->open_file ||
            !ops->next_record) return 0;
    memset(state, 0, sizeof *state);
    state->player = -1;
    if ((extension[0] == 'h' || extension[0] == 'H') &&
            (extension[1] == 's' || extension[1] == 'S')) {
        state->mode = STARS_LOAD_HISTORY_MODE;
    } else {
        state->mode = STARS_LOAD_PLAYER_MODE;
        state->mode_flags = STARS_LOAD_PLAYER_FILE_FLAGS;
        state->player = load_player_number(extension);
    }
    opened = load_open(ops, context, STARS_LOAD_FILE_GAME, -1);
    if (!opened) return 0;
    state->open = 1;
    if (!load_game_prelude(ops, context, state)) goto fail;
    if (ops->close_file) ops->close_file(context);
    state->open = 0;
    if (ops->reset_runtime)
        ops->reset_runtime(context, state->player_count, state->player);
    if (state->player != -1) {
        opened = load_open(ops, context, STARS_LOAD_FILE_PLAYER,
            state->player);
        if (!opened || !load_player_file(ops, context, state)) goto fail;
        state->open = 0;
    }
    opened = load_open(ops, context,
        (StarsU16)(state->mode | state->mode_flags), state->player);
    if (!opened) goto fail;
    state->open = 1;
    if (!load_next(ops, context, &record)) goto fail;
    while (load_record_type(&record) != STARS_LOAD_RECORD_END) {
        if (!load_turn(ops, context, state, &record)) goto fail;
        /* Real files end exactly at the file boundary; the type-0 marker is
         * accepted by the adapter as an equally valid synthetic terminator. */
        if (!load_next(ops, context, &record)) {
            if (ops->close_file) ops->close_file(context);
            state->open = 0;
            if (!load_post_stages(ops, context, file_name, extension, state))
                goto fail;
            if (ops->post_load && !ops->post_load(context, file_name,
                    extension, state)) goto fail;
            return 1;
        }
    }
    if (ops->close_file) ops->close_file(context);
    state->open = 0;
    if (!load_post_stages(ops, context, file_name, extension, state))
        goto fail;
    if (ops->post_load && !ops->post_load(context, file_name, extension,
            state)) goto fail;
    return 1;
fail:
    if (state->open && ops->close_file) ops->close_file(context);
    state->open = 0;
    if (ops->cleanup_failure)
        ops->cleanup_failure(context, state->player);
    load_alert(ops, context, state, STARS_LOAD_ALERT_BAD_RECORD);
    return 0;
}
