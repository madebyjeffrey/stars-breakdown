/* Reuse the retained full-turn loader and DoAiTurn coordinator while the
 * selector-3 personality and downstream services are connected. */
#include "core.h"
typedef struct RototillRngDraw {
    StarsI16 limit;
    StarsI16 value;
} RototillRngDraw;
static RototillRngDraw rototill_rng_draws[128];
static StarsU16 rototill_rng_draw_count;
static StarsSeeds *rototill_traced_seeds;
static StarsI16 rototill_trace_random(StarsSeeds *, StarsI16);
#define stars_random rototill_trace_random
#define main turin_binding_original_main
#include "satisfy_orders_turin_binding_e3_probe.c"
#undef main
#undef stars_random

static StarsI16 rototill_trace_random(StarsSeeds *state, StarsI16 limit)
{
    StarsI16 value = stars_random(state, limit);
    if (state == rototill_traced_seeds) {
        if (rototill_rng_draw_count < 128U) {
            rototill_rng_draws[rototill_rng_draw_count].limit = limit;
            rototill_rng_draws[rototill_rng_draw_count].value = value;
            ++rototill_rng_draw_count;
        }
    }
    return value;
}
#define stars_random rototill_trace_random

static StarsU8 rototill_produce_result[4];
static StarsU8 rototill_planet_after_produce[64];
static StarsU16 rototill_planet_after_produce_size;
static StarsU8 rototill_planet10_after_produce[64];
static StarsU16 rototill_planet10_after_produce_size;
static StarsU8 rototill_hst_planet_bodies[TURIN_RETAINED_POINT_COUNT][64];
static StarsU16 rototill_hst_planet_sizes[TURIN_RETAINED_POINT_COUNT];
static StarsU8 rototill_hst_thing_bodies[TURIN_RETAINED_THING_COUNT * 18U];
static StarsI16 rototill_output_tail_draws[4];
static StarsU8 rototill_output_bofs[3][18];

typedef struct RototillOutputBof {
    TurinTrace *trace;
    StarsU16 index;
    StarsU16 size;
} RototillOutputBof;

static StarsU32 rototill_output_tick(void *opaque)
{
    static const StarsU32 ticks[3] = {917UL, 920UL, 921UL};
    RototillOutputBof *output = (RototillOutputBof *)opaque;
    if (output->index >= 3U) {
        ++output->trace->failures;
        return 0UL;
    }
    return ticks[output->index];
}

static void rototill_output_transfer(void *opaque,
    const StarsU8 *bytes, StarsU16 count)
{
    RototillOutputBof *output = (RototillOutputBof *)opaque;
    if (output->index >= 3U || output->size + count > 18U) {
        ++output->trace->failures;
        return;
    }
    memcpy(rototill_output_bofs[output->index] + output->size, bytes, count);
    output->size = (StarsU16)(output->size + count);
}

static void rototill_preview_output_rng_tail(TurinTrace *trace)
{
    StarsSeeds preview = trace->seeds;
    StarsU8 data[0x4ba8U];
    RototillOutputBof output;
    StarsU16 index;
    rototill_output_tail_draws[0] = trace->saved_flag;
    for (index = 1U; index < 4U; ++index)
        rototill_output_tail_draws[index] = stars_random(&preview, 2000);
    memset(data, 0, sizeof data);
    memcpy(data + 0x0a1cU, trace->complete_input.bof, 4U);
    memcpy(data + 0x0070U, trace->complete_input.game, 4U);
    put_u16(data, 0x0082U, 1U);
    put_u16(data, 0x0080U,
        (StarsU16)(rototill_output_tail_draws[0] << 9));
    put_u16(data, 0x07caU, trace->game_flags);
    put_u32(data, 0x1370U, trace->seeds.first);
    put_u32(data, 0x1374U, trace->seeds.second);
    /* PrepareOutputFiles consumed Random(8); the source writer now owns
     * the three following Random(2000) draws. */
    memset(&output, 0, sizeof output);
    output.trace = trace;
    for (index = 0U; index < 3U; ++index) {
        output.index = index;
        output.size = 0U;
        stars_write_bof(data, index < 2U ? (StarsI16)index : -1,
            index < 2U ? 3 : 2, 0, rototill_output_tick,
            rototill_output_transfer, &output);
        if (output.size != 18U)
            ++trace->failures;
    }
    if (get_u32(data, 0x1370U) != preview.first ||
            get_u32(data, 0x1374U) != preview.second)
        ++trace->failures;
}
static StarsU8 rototill_fleet_bodies[TURIN_RETAINED_FLEET_COUNT][128];
static StarsU16 rototill_fleet_body_sizes[TURIN_RETAINED_FLEET_COUNT];
static StarsU8 rototill_order_bodies[TURIN_RETAINED_FLEET_COUNT][18];
static StarsU16 rototill_order_body_sizes[TURIN_RETAINED_FLEET_COUNT];
static StarsU8 rototill_host_fleet_bodies[TURIN_HST_OWNER0_INPUT_FLEET_COUNT][128];
static StarsU16 rototill_host_fleet_body_sizes[TURIN_HST_OWNER0_INPUT_FLEET_COUNT];
static StarsU8 rototill_host_order_bodies[TURIN_HST_OWNER0_INPUT_FLEET_COUNT][18];
static StarsU16 rototill_host_order_body_sizes[TURIN_HST_OWNER0_INPUT_FLEET_COUNT];
static StarsU8 rototill_output_design_bodies[4][160];
static StarsU16 rototill_output_design_sizes[4];
static StarsU8 rototill_m2_player_body[266];
static StarsU16 rototill_m2_player_size;
static StarsU8 rototill_m1_player_body[266];
static StarsU16 rototill_m1_player_size;
static StarsU8 rototill_hst_player_bodies[2][266];
static StarsU16 rototill_hst_player_sizes[2];
static StarsU8 rototill_all_design_bodies[30][160];
static StarsU16 rototill_all_design_sizes[30];
static StarsU16 rototill_all_design_files[30];
static StarsU16 rototill_all_design_owners[30];
static StarsU16 rototill_all_design_banks[30];
static StarsU16 rototill_all_design_slots[30];
static StarsU16 rototill_all_design_count;
static StarsU8 rototill_plan_bodies[20][64];
static StarsU16 rototill_plan_sizes[20];
static StarsU16 rototill_plan_files[20];
static StarsU16 rototill_plan_owners[20];
static StarsU16 rototill_plan_count;
static StarsU16 rototill_plan_file;
static StarsU16 rototill_plan_owner;
static StarsU8 rototill_client_planet_bodies[5][128];
static StarsU16 rototill_client_planet_sizes[5];
static StarsU16 rototill_client_planet_types[5];
static StarsU16 rototill_client_planet_files[5];
static StarsU16 rototill_client_planet_count;
static StarsU8 rototill_m1_message_body[1024];
static StarsU16 rototill_m1_message_size;
static StarsU16 rototill_m1_message_calls;
static StarsU8 rototill_thing_records[5][18];
static StarsU16 rototill_thing_sizes[5];
static StarsU16 rototill_thing_record_count;

typedef struct RototillFleetWriter {
    TurinTrace *trace;
    StarsU16 id;
    StarsU16 owner;
    StarsU16 id_word;
    StarsI16 planet;
    StarsI16 x;
    StarsI16 y;
} RototillFleetWriter;

static void rototill_capture_fleet_record(void *opaque, StarsU16 type,
    StarsU16 length, const StarsU8 *record)
{
    RototillFleetWriter *writer = (RototillFleetWriter *)opaque;
    StarsU8 *body;
    StarsU16 *fleet_size = writer->owner ?
        &rototill_fleet_body_sizes[writer->id] :
        &rototill_host_fleet_body_sizes[writer->id];
    StarsU16 *order_size = writer->owner ?
        &rototill_order_body_sizes[writer->id] :
        &rototill_host_order_body_sizes[writer->id];
    if (type == 16U && length >= 4U && length <= 118U &&
            !*fleet_size) {
        body = writer->owner ? rototill_fleet_bodies[writer->id] :
            rototill_host_fleet_bodies[writer->id];
        put_u16(body, 0U, writer->id_word);
        put_u16(body, 2U, writer->owner);
        put_u16(body, 4U, get_u16(record, 0U));
        put_u16(body, 6U, (StarsU16)writer->planet);
        put_u16(body, 8U, (StarsU16)writer->x);
        put_u16(body, 10U, (StarsU16)writer->y);
        put_u16(body, 12U, get_u16(record, 2U));
        memcpy(body + 14U, record + 4U, (size_t)(length - 4U));
        *fleet_size = (StarsU16)(length + 10U);
    } else if (type == 20U && length == 8U &&
            !*order_size) {
        body = writer->owner ? rototill_order_bodies[writer->id] :
            rototill_host_order_bodies[writer->id];
        memcpy(body, record, length);
        *order_size = length;
    } else {
        ++writer->trace->failures;
    }
}

static StarsU32 rototill_log_tick(void *opaque)
{
    (void)opaque;
    /* Retained BOF salt 1033 minus the next source Random(2000) draw 210. */
    return 823UL;
}

static StarsI16 rototill_log_create(void *opaque, StarsI16 player,
    const StarsU8 *base)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    (void)base;
    if (player != 1 || trace->generated_log_size) {
        ++trace->failures;
        return 0;
    }
    memcpy(trace->rng_file_data + 0x0a1cU, trace->complete_input.bof, 4U);
    memcpy(trace->rng_file_data + 0x0070U, trace->complete_input.game, 4U);
    put_u16(trace->rng_file_data, 0x0082U,
        get_u16(trace->complete_input.bof, 10U));
    put_u16(trace->rng_file_data, 0x0080U,
        (StarsU16)(((get_u16(trace->complete_input.bof, 14U) >> 13) & 7U) << 9));
    put_u32(trace->rng_file_data, 0x1370U, trace->seeds.first);
    put_u32(trace->rng_file_data, 0x1374U, trace->seeds.second);
    put_u16(trace->rng_file_data, 0x07caU, trace->game_flags);
    put_u16(trace->rng_file_data, 0x07ceU, trace->startup_log_flags);
    stars_write_bof(trace->rng_file_data, 1, 1, 0,
        rototill_log_tick, turn_log_transfer, trace);
    trace->seeds.first = get_u32(trace->rng_file_data, 0x1370U);
    trace->seeds.second = get_u32(trace->rng_file_data, 0x1374U);
    if (trace->generated_log_size != 18U ||
            get_u16(trace->generated_log_file, 0U) != 0x2010U ||
            get_u16(trace->generated_log_file, 14U) != 0x8121U ||
            get_u16(trace->generated_log_file, 16U) != 0x0101U)
        ++trace->failures;
    return 1;
}

static void rototill_log_record(void *opaque, StarsU16 type,
    StarsU16 size, const void *body)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    static const StarsU8 header[17] = {
        0x1bU, 0x01U, 0x76U, 0x0aU, 0x6fU, 0x00U,
        0xa5U, 0xdcU, 0xa6U, 0x59U, 0x7aU, 0xc5U,
        0xa5U, 0xdcU, 0xa6U, 0x7aU, 0xf0U
    };
    if (type == 9U && (size != sizeof header ||
            memcmp(body, header, sizeof header) != 0))
        ++trace->failures;
    stars_write_record(trace->rng_file_data, (StarsI16)type, size,
        size ? (const StarsU8 *)body : trace->saved_mutation_stream,
        turn_log_transfer, trace);
    ++trace->generated_log_records;
}

static StarsI16 rototill_write_generated_log(TurinTrace *trace)
{
    StarsWriteLogFileInput input;
    static const StarsU8 environment[11] = {
        0xa5U, 0xdcU, 0xa6U, 0x59U, 0x7aU, 0xc5U,
        0xa5U, 0xdcU, 0xa6U, 0x7aU, 0xf0U
    };
    memset(&input, 0, sizeof input);
    input.player = 1;
    input.current_player = trace->session.player;
    input.player_flags = get_u16(trace->complete_input.views[0].players[1], 6U);
    input.serial = 0x006f0a76UL;
    input.environment = environment;
    input.log = trace->saved_mutation_stream;
    input.log_size = trace->saved_mutation_size;
    input.game_flags = &trace->startup_log_flags;
    input.create_file = rototill_log_create;
    input.write_record = rototill_log_record;
    input.close_stream = turn_log_close;
    input.context = trace;
    if (!stars_write_log_file(&input)) return 0;
    return trace->generated_log_size == 322U &&
        trace->generated_log_closed == 1U &&
        trace->generated_log_records == trace->saved_mutation_records + 2U;
}

static void rototill_history_transfer(void *opaque,
    const StarsU8 *bytes, StarsU16 count)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    if ((StarsU32)trace->generated_history_size + count >
            sizeof trace->generated_history_file) {
        ++trace->failures;
        return;
    }
    if (count)
        memcpy(trace->generated_history_file + trace->generated_history_size,
            bytes, count);
    trace->generated_history_size =
        (StarsU16)(trace->generated_history_size + count);
}

static void rototill_write_generated_history(TurinTrace *trace)
{
    static const StarsU16 types[4] = {32U, 14U, 33U, 0U};
    static const StarsU16 sizes[4] = {4U, 16U, 49U, 0U};
    StarsU16 index;
    if (trace->startup_history_record_count != 4U ||
            trace->generated_history_size != 0U) {
        ++trace->failures;
        return;
    }
    memcpy(trace->rng_file_data + 0x0a1cU, trace->complete_input.bof, 4U);
    memcpy(trace->rng_file_data + 0x0070U, trace->complete_input.game, 4U);
    put_u16(trace->rng_file_data, 0x0082U,
        get_u16(trace->complete_input.bof, 10U));
    put_u16(trace->rng_file_data, 0x0080U,
        (StarsU16)(((get_u16(trace->complete_input.bof, 14U) >> 13) & 7U) << 9));
    put_u32(trace->rng_file_data, 0x1370U, trace->seeds.first);
    put_u32(trace->rng_file_data, 0x1374U, trace->seeds.second);
    put_u16(trace->rng_file_data, 0x07caU, trace->game_flags);
    put_u16(trace->rng_file_data, 0x07ceU, trace->startup_log_flags);
    stars_write_bof(trace->rng_file_data, 1, 4, 0,
        rototill_log_tick, rototill_history_transfer, trace);
    trace->seeds.first = get_u32(trace->rng_file_data, 0x1370U);
    trace->seeds.second = get_u32(trace->rng_file_data, 0x1374U);
    if (trace->generated_history_size != 18U ||
            get_u16(trace->generated_history_file, 14U) != 0xeb41U ||
            get_u16(trace->generated_history_file, 16U) != 0x0104U)
        ++trace->failures;
    for (index = 0U; index < 4U; ++index) {
        if (trace->startup_history_types[index] != types[index] ||
                trace->startup_history_sizes[index] != sizes[index])
            ++trace->failures;
        stars_write_record(trace->rng_file_data, (StarsI16)types[index],
            sizes[index], trace->startup_history_records[index],
            rototill_history_transfer, trace);
    }
    if (trace->generated_history_size != 95U)
        ++trace->failures;
}

typedef struct RototillResearchReplay {
    TurinTrace *trace;
    StarsU16 kinds[7];
    StarsU16 count;
} RototillResearchReplay;

static StarsU16 rototill_research_replay_record(void *opaque,
    StarsU16 type, StarsU16 size, const StarsU8 *body)
{
    RototillResearchReplay *replay = (RototillResearchReplay *)opaque;
    (void)size;
    (void)body;
    if (replay->count >= 7U || type == 34U) {
        ++replay->trace->failures;
        return 0U;
    }
    replay->kinds[replay->count++] = type;
    return 1U;
}

static void rototill_reload_host_input(TurinTrace *trace)
{
    if (trace->complete_input.views[0].players[1][56U] != 0U ||
            trace->complete_input.views[2].players[1][56U] != 15U ||
            soe3_inputs[0].views[0].players[1][56U] != 15U) {
        ++trace->failures;
        return;
    }
    /* EnsureAis/FGenerateTurn reload the full HST player before X2 replay. */
    memcpy(trace->complete_input.views[0].players[1],
        soe3_inputs[0].views[0].players[1], 192U);
    trace->complete_input.views[0].players[1][7U] = 0x67U;
    startup_load_runtime(trace);
    if (!read_retained_m2_shdef_bank(trace, 0)) {
        ++trace->failures;
        return;
    }
}

static void rototill_check_research_reload(TurinTrace *trace)
{
    static const StarsU16 kinds[7] = {27U, 27U, 27U, 27U, 4U, 1U, 29U};
    static const StarsU8 environment[11] = {
        0xa5U, 0xdcU, 0xa6U, 0x59U, 0x7aU, 0xc5U,
        0xa5U, 0xdcU, 0xa6U, 0x7aU, 0xf0U
    };
    RototillResearchReplay replay;
    StarsU8 data[0x09aaU];
    if (trace->complete_input.views[0].players[1][56U] != 15U ||
            trace->complete_input.views[0].players[1][7U] != 0x67U) {
        ++trace->failures;
        return;
    }
    trace->replay_serial = 0x006f0a76UL;
    memcpy(trace->replay_environment, environment, sizeof environment);
    if (!turn_load_generated_log(trace) || trace->generated_log_loads != 1U) {
        ++trace->failures;
        return;
    }
    memset(&replay, 0, sizeof replay);
    memset(data, 0, sizeof data);
    replay.trace = trace;
    put_u16(data, 0x09a4U, trace->saved_mutation_size);
    put_u16(data, 0x09a8U, 1U);
    if (!stars_run_log_file(data, trace->saved_mutation_stream,
            rototill_research_replay_record, &replay) ||
            replay.count != 7U ||
            memcmp(replay.kinds, kinds, sizeof kinds) != 0 ||
            get_u16(data, 0x09a8U) != 1U ||
            trace->complete_input.views[0].players[1][56U] != 15U)
        ++trace->failures;
    if (!trace->failures) {
        StarsU16 planet;
        StarsI16 applied;
        for (planet = 0U; planet < TURIN_RETAINED_POINT_COUNT; ++planet)
            trace->turn_planet_states[planet] =
                trace->complete_input.views[0].planets[planet];
        applied = startup_replay_memory_logs(trace);
        if (!applied || trace->startup_memory_applied != 7U ||
                trace->complete_input.views[0].players[1][56U] != 15U ||
                trace->turn_committed_planet != 24 ||
                trace->turn_committed_queue_count != 1U ||
                trace->turn_produce_queue[0].quantity != 3 ||
                trace->turn_planet_states[24].minerals[3] != 225UL ||
                get_u32(trace->raw_fleets[1], 88U) != 25UL ||
                trace->plord[0][3] != 2U ||
                get_u16(trace->plord[0], 26U) != 26U) {
            fprintf(stderr, "rot replay applied=%d count=%u failures=%d research=%u queue=%u\n",
                applied, trace->startup_memory_applied, trace->failures,
                trace->complete_input.views[0].players[1][56U],
                trace->turn_committed_queue_count);
            ++trace->failures;
        }
    }
}

static void rototill_check_fleet_movement(TurinTrace *trace)
{
    StarsMoveFleetsOps ops;
    StarsMoveFleetsInput input;
    StarsMoveFleet fleets[TURIN_RETAINED_FLEET_COUNT];
    StarsU16 id;
    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(fleets, 0, sizeof fleets);
    ops.get_order = get_order;
    ops.distance = distance;
    ops.movement_limit = movement_limit;
    ops.fuel_use = fuel_use;
    ops.context = trace;
    for (id = 0U; id < TURIN_RETAINED_FLEET_COUNT; ++id) {
        const StarsU8 *raw = trace->raw_fleets[id];
        if (!trace->raw_fleet_present[id] ||
                get_u16(raw, 0U) != id || get_u16(raw, 2U) != 1U) {
            ++trace->failures;
            return;
        }
        fleets[id].id = (StarsI16)id;
        fleets[id].owner = 1;
        fleets[id].x = (StarsI16)get_u16(raw, 8U);
        fleets[id].y = (StarsI16)get_u16(raw, 10U);
        fleets[id].target_id = (StarsI16)get_u16(raw, 6U);
        fleets[id].fuel = (StarsI32)get_u32(raw, 92U);
        fleets[id].fuel_capacity = move_current_capacity(trace, 1, id, 0);
        fleets[id].active = 1;
        fleets[id].warp_level = (StarsI16)(
            (get_u16(trace->plord[id], 10U) >> 4) & 15U);
    }
    input.fleets = fleets;
    input.fleet_count = TURIN_RETAINED_FLEET_COUNT;
    input.post_movement = 0;
    input.ops = &ops;
    stars_move_fleets(&input);
    if (trace->move_fuel_calls != 1 ||
            fleets[0].x != 1313 || fleets[0].y != 1262 ||
            fleets[0].target_id != 26 || fleets[0].fuel != 298L ||
            trace->fuel_distance != 24L)
        ++trace->failures;
    for (id = 1U; id < TURIN_RETAINED_FLEET_COUNT; ++id)
        if (fleets[id].x != 1290 || fleets[id].y != 1258 ||
                fleets[id].target_id != 24)
            ++trace->failures;
    memcpy(trace->turn_moving_fleets, fleets, sizeof fleets);
    run_retained_owner0_move_fleets(trace);
    for (id = 0U; id < trace->complete_input.views[0].fleet_count; ++id) {
        Soe3InputFleet *source = &trace->complete_input.views[0].fleets[id];
        StarsMoveFleet *moving;
        StarsU16 fleet_id;
        StarsU16 flags;
        StarsU32 direction = 0UL;
        StarsI16 dx, dy;
        const StarsU8 *next;
        if (source->owner != 1) continue;
        fleet_id = (StarsU16)(source->id_word & 511U);
        if (fleet_id >= TURIN_RETAINED_FLEET_COUNT) {
            ++trace->failures;
            continue;
        }
        moving = &trace->turn_moving_fleets[fleet_id];
        flags = get_u16(trace->raw_fleets[fleet_id], 4U);
        if (moving->status & 0x2000U) {
            dx = (StarsI16)(moving->x - source->state.x);
            dy = (StarsI16)(moving->y - source->state.y);
            next = trace->plord[fleet_id] + 22U;
            if (dx || dy) {
                while (dx > 127 || dx < -127 || dy > 127 || dy < -127) {
                    dx /= 2; dy /= 2;
                }
                direction = (StarsU32)(dx + 127) |
                    ((StarsU32)(dy + 127) << 8) |
                    ((StarsU32)((get_u16(next, 6U) >> 4) & 15U) << 16) |
                    0x00100000UL;
            }
        }
        source->state.overlap_116[0] = direction;
        flags = (StarsU16)(flags | 0x2800U);
        if (moving->status & 0x2000U)
            flags = (StarsU16)(flags & ~0x2000U);
        source->state.fleet_flags = flags;
        source->state.x = moving->x;
        source->state.y = moving->y;
        source->state.planet = moving->target_id;
        source->state.minerals[4] = (StarsU32)moving->fuel;
        put_u16(trace->raw_fleets[fleet_id], 4U, flags);
        put_u16(trace->raw_fleets[fleet_id], 6U,
            (StarsU16)moving->target_id);
        put_u16(trace->raw_fleets[fleet_id], 8U, (StarsU16)moving->x);
        put_u16(trace->raw_fleets[fleet_id], 10U, (StarsU16)moving->y);
        put_u32(trace->raw_fleets[fleet_id], 92U,
            (StarsU32)moving->fuel);
        put_u32(trace->raw_fleets[fleet_id], 116U, direction);
    }
    for (id = 0U; id < TURIN_HST_OWNER0_INPUT_FLEET_COUNT; ++id) {
        Soe3InputFleet *source = turn_host_fleet(trace, 0U, id);
        StarsMoveFleet *moving = &trace->turn_owner0_moving_fleets[id];
        StarsU16 flags;
        if (!source || !moving->active) continue;
        flags = (StarsU16)(source->state.fleet_flags | 0x2800U);
        if (moving->status & 0x2000U)
            flags = (StarsU16)(flags & ~0x2000U);
        source->state.fleet_flags = flags;
        source->state.x = moving->x;
        source->state.y = moving->y;
        source->state.planet = moving->target_id;
        source->state.minerals[4] = (StarsU32)moving->fuel;
    }
    turn_finish_movement_orders(trace);
    if (trace->plord[0][3] != 1U ||
            get_u16(trace->plord[0], 8U) != 26U ||
            get_u16(trace->plord[0], 10U) != 0x1150U ||
            get_u16(trace->raw_fleets[0], 4U) != 0x0907U ||
            get_u16(trace->raw_fleets[1], 4U) != 0x2907U ||
            get_u16(trace->raw_fleets[2], 4U) != 0x2907U ||
            get_u16(trace->raw_fleets[3], 4U) != 0x2907U)
        ++trace->failures;
    ++trace->turn_move_fleets_calls;
}

static void rototill_write_moved_fleets(TurinTrace *trace)
{
    StarsU16 id;
    for (id = 0U; id < TURIN_RETAINED_FLEET_COUNT; ++id) {
        const Soe3InputFleet *source = turn_host_fleet(trace, 1U, id);
        StarsWriteFleetInput input;
        StarsWriteFleetOps ops;
        RototillFleetWriter writer;
        StarsU8 record[128];
        StarsU16 length;
        StarsU16 i;
        if (!source || trace->plord[id][3] != 1U) {
            ++trace->failures;
            continue;
        }
        memset(&input, 0, sizeof input);
        memset(&ops, 0, sizeof ops);
        memset(&writer, 0, sizeof writer);
        input.fleet_flags = get_u16(trace->raw_fleets[id], 4U);
        for (i = 0U; i < 32U; ++i)
            input.ship_counts[i] = get_u16(trace->raw_fleets[id],
                (StarsU16)(12U + i * 2U));
        for (i = 0U; i < 5U; ++i)
            input.minerals[i] = get_u32(trace->raw_fleets[id],
                (StarsU16)(76U + i * 4U));
        input.design_plan = trace->raw_fleets[id][96U];
        input.cord = trace->plord[id][3];
        input.orders = trace->plord[id] + STARS_AI_PLORD_HEADER_SIZE;
        input.order_count = trace->plord[id][3];
        writer.trace = trace;
        writer.id = id;
        writer.owner = 1U;
        writer.id_word = source->id_word;
        writer.planet = trace->turn_moving_fleets[id].target_id;
        writer.x = trace->turn_moving_fleets[id].x;
        writer.y = trace->turn_moving_fleets[id].y;
        ops.write_record = rototill_capture_fleet_record;
        if (!stars_write_fleet(&input, record, (StarsU16)sizeof record,
                &length, &ops, &writer) ||
                !rototill_fleet_body_sizes[id] ||
                !rototill_order_body_sizes[id])
            ++trace->failures;
    }
}

static void rototill_write_host_fleets(TurinTrace *trace)
{
    StarsU16 id;
    for (id = 0U; id < TURIN_HST_OWNER0_INPUT_FLEET_COUNT; ++id) {
        const Soe3InputFleet *source = turn_host_fleet(trace, 0U, id);
        StarsWriteFleetInput input;
        StarsWriteFleetOps ops;
        RototillFleetWriter writer;
        StarsU8 record[128];
        StarsU16 length;
        StarsU16 i;
        if (!source || source->state.order_count != 1U ||
                !trace->turn_owner0_moving_fleets[id].active) {
            ++trace->failures;
            continue;
        }
        memset(&input, 0, sizeof input);
        memset(&ops, 0, sizeof ops);
        memset(&writer, 0, sizeof writer);
        input.fleet_flags = source->state.fleet_flags;
        for (i = 0U; i < 32U; ++i)
            input.ship_counts[i] = source->state.ship_counts[i];
        for (i = 0U; i < 5U; ++i)
            input.minerals[i] = source->state.minerals[i];
        input.design_plan = (StarsU8)source->state.design_plan;
        input.cord = (StarsU8)source->state.order_count;
        input.orders = source->state.orders;
        input.order_count = source->state.order_count;
        writer.trace = trace;
        writer.id = id;
        writer.owner = 0U;
        writer.id_word = source->id_word;
        writer.planet = trace->turn_owner0_moving_fleets[id].target_id;
        writer.x = trace->turn_owner0_moving_fleets[id].x;
        writer.y = trace->turn_owner0_moving_fleets[id].y;
        ops.write_record = rototill_capture_fleet_record;
        if (!stars_write_fleet(&input, record, (StarsU16)sizeof record,
                &length, &ops, &writer) ||
                !rototill_host_fleet_body_sizes[id] ||
                !rototill_host_order_body_sizes[id])
            ++trace->failures;
    }
}

static void rototill_write_output_designs(TurinTrace *trace)
{
    static const StarsU16 slots[4] = {18U, 20U, 17U, 19U};
    StarsVisPFPlayer views[2];
    StarsVisPFInitInput visibility;
    StarsU16 index;
    StarsU16 owner, slot;
    memset(views, 0, sizeof views);
    memset(&visibility, 0, sizeof visibility);
    for (owner = 0U; owner < 2U; ++owner) {
        const StarsU8 *ships = trace->complete_input.views[0].ship_designs[owner];
        const StarsU8 *bases = owner == 1U ? trace->starbase_designs :
            trace->complete_input.views[0].starbase_designs[owner];
        for (slot = 0U; slot < 16U; ++slot)
            views[owner].design_flags[slot] =
                get_u16(ships + slot * 147U, 123U);
        for (slot = 0U; slot < 10U; ++slot)
            views[owner].starbase_flags[slot] =
                get_u16(bases + slot * 147U, 123U);
    }
    visibility.viewer = 1;
    visibility.players = views;
    visibility.player_count = 2U;
    stars_set_vis_pf_init(&visibility, 0, 0);
    for (index = 0U; index < 4U; ++index) {
        const StarsU8 *raw = trace->starbase_designs +
            (slots[index] - 16U) * 147U;
        StarsWriteShdefInput input;
        StarsWriteShdefOps ops;
        memset(&input, 0, sizeof input);
        memset(&ops, 0, sizeof ops);
        input.flags = views[1].starbase_flags[slots[index] - 16U];
        input.hull_id = get_u16(raw, 0U);
        input.bitmap = (StarsU8)get_u16(raw, 50U);
        input.component_count = raw[122U];
        input.design_points = get_u16(raw, 56U);
        input.turn = get_u16(raw, 125U);
        input.built = get_u32(raw, 127U);
        input.existing = get_u32(raw, 131U);
        input.empty_weight = get_u16(raw, 40U);
        input.component_data = raw + 58U;
        input.component_data_size = (StarsU16)(raw[122U] * 4U);
        input.name = raw + 8U;
        ops.compress_name = startup_compress_name;
        if (!stars_write_rt_shdef(&input,
                rototill_output_design_bodies[index],
                sizeof rototill_output_design_bodies[index],
                &rototill_output_design_sizes[index], &ops, trace))
            ++trace->failures;
    }
}

static void rototill_write_all_designs(TurinTrace *trace)
{
    StarsU16 file, bank, owner, slot;
    rototill_all_design_count = 0U;
    for (file = 0U; file < 3U; ++file) {
        const StarsVisPFPlayer *views = file == 0U ?
            trace->turn_player0_visibility_players :
            (file == 1U ? trace->turn_player1_visibility_players :
            trace->turn_host_visibility_players);
        for (bank = 0U; bank < 2U; ++bank)
            for (owner = 0U; owner < 2U; ++owner)
                for (slot = 0U; slot < (bank ? 10U : 16U); ++slot) {
                    const StarsU8 *raw;
                    StarsU16 flags;
                    StarsWriteShdefInput input;
                    StarsWriteShdefOps ops;
                    StarsU16 index;
                    if (file != 2U && owner != file) continue;
                    flags = bank ? views[owner].starbase_flags[slot] :
                        views[owner].design_flags[slot];
                    if ((flags & 0x0300U) != 0x0100U) continue;
                    if (rototill_all_design_count >= 30U) {
                        ++trace->failures; return;
                    }
                    index = rototill_all_design_count++;
                    raw = bank ? (owner == 1U ? trace->starbase_designs :
                        trace->complete_input.views[0].starbase_designs[owner]) :
                        (owner == 1U ? trace->designs :
                        trace->complete_input.views[0].ship_designs[owner]);
                    raw += slot * 147U;
                    memset(&input, 0, sizeof input);
                    memset(&ops, 0, sizeof ops);
                    input.flags = flags;
                    input.hull_id = get_u16(raw, 0U);
                    input.bitmap = (StarsU8)get_u16(raw, 50U);
                    input.component_count = raw[122U];
                    input.design_points = get_u16(raw, 56U);
                    input.turn = get_u16(raw, 125U);
                    input.built = get_u32(raw, 127U);
                    input.existing = get_u32(raw, 131U);
                    input.empty_weight = get_u16(raw, 40U);
                    input.component_data = raw + 58U;
                    input.component_data_size = (StarsU16)(raw[122U] * 4U);
                    input.name = raw + 8U;
                    ops.compress_name = startup_compress_name;
                    rototill_all_design_files[index] = file;
                    rototill_all_design_owners[index] = owner;
                    rototill_all_design_banks[index] = bank;
                    rototill_all_design_slots[index] = slot;
                    if (!stars_write_rt_shdef(&input,
                            rototill_all_design_bodies[index],
                            sizeof rototill_all_design_bodies[index],
                            &rototill_all_design_sizes[index], &ops, trace))
                        ++trace->failures;
                }
    }
}

static void rototill_capture_plan(void *opaque, StarsU16 type,
    StarsU16 size, const StarsU8 *body)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsU16 index = rototill_plan_count;
    if (type != 30U || index >= 20U || size > 64U) {
        ++trace->failures;
        return;
    }
    rototill_plan_count = (StarsU16)(index + 1U);
    rototill_plan_sizes[index] = size;
    rototill_plan_files[index] = rototill_plan_file;
    rototill_plan_owners[index] = rototill_plan_owner;
    memcpy(rototill_plan_bodies[index], body, size);
}

static void rototill_write_all_plans(TurinTrace *trace)
{
    StarsU16 file, owner, slot;
    const Soe3InputView *world = &trace->complete_input.views[0];
    rototill_plan_count = 0U;
    for (file = 0U; file < 3U; ++file) {
        rototill_plan_file = file;
        for (owner = 0U; owner < 2U; ++owner) {
            if (file != 2U && owner != file) continue;
            rototill_plan_owner = owner;
            for (slot = 0U; slot < 16U; ++slot)
                if (world->battle_plan_present[owner][slot])
                    stars_write_battle_plan(world->battle_plans[owner][slot],
                        original_dgroup, 0, rototill_capture_plan, 0, trace);
        }
    }
}

static void rototill_write_client_planets(TurinTrace *trace)
{
    StarsU16 viewer, planet;
    rototill_client_planet_count = 0U;
    for (viewer = 0U; viewer < 2U; ++viewer)
        for (planet = 0U; planet < TURIN_RETAINED_POINT_COUNT; ++planet) {
            StarsReadPlanetState state =
                trace->turn_client_visibility_planets[viewer][planet];
            StarsU16 index, type;
            if (!(state.flags & 0x0100U)) continue;
            if (rototill_client_planet_count >= 5U) {
                ++trace->failures; return;
            }
            index = rototill_client_planet_count++;
            type = (state.flags & 255U) == 7U ? 13U : 14U;
            if ((state.flags & 255U) == 2U)
                state.flags = (StarsU16)((state.flags & 0xfd00U) | 3U);
            rototill_client_planet_types[index] = type;
            rototill_client_planet_files[index] = viewer;
            if (!stars_write_planet(&state, type, 0U,
                    rototill_client_planet_bodies[index],
                    sizeof rototill_client_planet_bodies[index],
                    &rototill_client_planet_sizes[index], 0, 0))
                ++trace->failures;
        }
}

static void rototill_capture_m1_message(void *opaque, StarsU16 type,
    StarsU16 length, const void *record)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    if (type != 12U || length > sizeof rototill_m1_message_body ||
            rototill_m1_message_calls != 0U) {
        ++trace->failures; return;
    }
    memcpy(rototill_m1_message_body, record, length);
    rototill_m1_message_size = length;
    ++rototill_m1_message_calls;
}

static void rototill_write_m1_messages(TurinTrace *trace)
{
    rototill_m1_message_size = 0U;
    rototill_m1_message_calls = 0U;
    stars_write_player_messages(trace->turn_message_buffer,
        trace->turn_message_bytes, 0, 0, 0U,
        rototill_capture_m1_message, trace);
    if (rototill_m1_message_calls != 1U)
        ++trace->failures;
}

static void rototill_capture_thing(void *opaque, StarsU16 type,
    StarsU16 length, const StarsU8 *record)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsU16 index = rototill_thing_record_count;
    if (type != 43U || index >= 5U || length > 18U) {
        ++trace->failures; return;
    }
    rototill_thing_record_count = (StarsU16)(index + 1U);
    rototill_thing_sizes[index] = length;
    memcpy(rototill_thing_records[index], record, length);
}

static void rototill_write_hst_things(TurinTrace *trace)
{
    StarsThing things[TURIN_RETAINED_THING_COUNT];
    StarsU16 index;
    rototill_thing_record_count = 0U;
    for (index = 0U; index < TURIN_RETAINED_THING_COUNT; ++index)
        memcpy(things[index].raw,
            rototill_hst_thing_bodies + index * 18U, 18U);
    if (!stars_write_thing_records(things, TURIN_RETAINED_THING_COUNT,
            rototill_capture_thing, trace) ||
            rototill_thing_record_count != 5U)
        ++trace->failures;
}

typedef struct RototillOutputStream {
    TurinTrace *trace;
    StarsU16 file;
    StarsU8 data[0x5000U];
    StarsU8 bytes[4096];
    StarsU16 size;
    StarsU16 opened;
} RototillOutputStream;

static StarsU8 rototill_output_files[3][4096];
static StarsU16 rototill_output_file_sizes[3];

static void rototill_stream_transfer(void *opaque,
    const StarsU8 *bytes, StarsU16 count)
{
    RototillOutputStream *stream = (RototillOutputStream *)opaque;
    if ((StarsU32)stream->size + count > sizeof stream->bytes) {
        ++stream->trace->failures;
        return;
    }
    memcpy(stream->bytes + stream->size, bytes, count);
    stream->size = (StarsU16)(stream->size + count);
}

static void rototill_stream_record(RototillOutputStream *stream,
    StarsU16 type, StarsU16 size, const StarsU8 *body)
{
    if (!stream->opened || type > 63U || size > 1023U ||
            (!body && size)) {
        ++stream->trace->failures;
        return;
    }
    stars_write_record(stream->data, (StarsI16)type, size, body,
        rototill_stream_transfer, stream);
}

static void rototill_stream_bof(RototillOutputStream *stream)
{
    rototill_stream_record(stream, 8U, 16U,
        rototill_output_bofs[stream->file] + 2U);
}

static StarsI16 rototill_stream_create(void *opaque,
    StarsU16 type, StarsI16 player)
{
    RototillOutputStream *stream = (RototillOutputStream *)opaque;
    if (stream->opened || (StarsU16)(player == -1 ? 2 : player) != stream->file ||
            type != (player == -1 ? 2U : 3U)) {
        ++stream->trace->failures;
        return 0;
    }
    stream->opened = 1U;
    rototill_stream_bof(stream);
    return 1;
}

static StarsI16 rototill_stream_mark_open(void *opaque,
    StarsU16 type, StarsI16 player, StarsU16 mode)
{
    RototillOutputStream *stream = (RototillOutputStream *)opaque;
    if (!stream->opened || stream->file != 0U ||
            type != 3U || player != 0 || mode != 0x12U) {
        ++stream->trace->failures;
        return 0;
    }
    return 1;
}

static void rototill_stream_mark_record(void *opaque,
    StarsU16 type, const StarsU8 *body, StarsU16 size)
{
    RototillOutputStream *stream = (RototillOutputStream *)opaque;
    if (type != 8U || size != 16U) {
        ++stream->trace->failures; return;
    }
    put_u16(stream->bytes, 16U, get_u16(body, 0U));
}

static StarsI16 rototill_stream_mark_append(void *opaque,
    StarsU16 type, StarsI16 player, StarsU16 mode, StarsI16 value)
{
    RototillOutputStream *stream = (RototillOutputStream *)opaque;
    StarsU16 size = stream->trace->active_file_sizes[1];
    StarsMarkFileInput input;
    StarsMarkFileState state;
    StarsMarkFileOps ops;
    if (stream->file != 0U || stream->opened || type != 0x2003U ||
            player != 0 || mode != 4U || value != 1 ||
            size > sizeof stream->bytes) {
        ++stream->trace->failures;
        return 0;
    }
    memcpy(stream->bytes, stream->trace->active_files[1], size);
    stream->size = size;
    stream->opened = 1U;
    memset(&input, 0, sizeof input);
    memset(&state, 0, sizeof state);
    memset(&ops, 0, sizeof ops);
    input.type = type;
    input.player = player;
    input.mark_mode = mode;
    input.value = value;
    input.game_lid = get_u32(stream->trace->complete_input.game, 0U);
    input.header_type = (StarsU16)(get_u16(stream->bytes, 0U) >> 10);
    input.header_version = get_u16(stream->bytes, 10U);
    input.header_game_lid = get_u32(stream->bytes, 6U);
    input.header_flags = get_u16(stream->bytes, 16U);
    ops.open_file = rototill_stream_mark_open;
    ops.write_record = rototill_stream_mark_record;
    if (!stars_mark_file(&input, &state, &ops, stream) ||
            !state.changed || !state.file_open) {
        ++stream->trace->failures;
        return 0;
    }
    return 1;
}

static void rototill_stream_append_bof(void *opaque,
    StarsI16 player, StarsI16 type, StarsI16 multi)
{
    RototillOutputStream *stream = (RototillOutputStream *)opaque;
    if (player != 0 || type != 3 || multi != 1) {
        ++stream->trace->failures;
        return;
    }
    rototill_stream_bof(stream);
}

static StarsI16 rototill_stream_append(void *opaque, StarsI16 player)
{
    return stars_append_file(player, rototill_stream_mark_append,
        rototill_stream_append_bof, opaque);
}

static StarsI16 rototill_stream_player_predicate(void *opaque, StarsU16 player)
{
    RototillOutputStream *stream = (RototillOutputStream *)opaque;
    return (StarsI16)(stream->file == 2U || player == stream->file);
}

static void rototill_stream_player(void *opaque, StarsU16 player)
{
    RototillOutputStream *stream = (RototillOutputStream *)opaque;
    const StarsU8 *body;
    StarsU16 size;
    if (stream->file == 2U) {
        body = rototill_hst_player_bodies[player];
        size = rototill_hst_player_sizes[player];
    } else if (stream->file == 1U) {
        body = rototill_m2_player_body;
        size = rototill_m2_player_size;
    } else {
        body = rototill_m1_player_body;
        size = rototill_m1_player_size;
    }
    rototill_stream_record(stream, 6U, size, body);
}

static void rototill_stream_messages(void *opaque, StarsI16 player)
{
    RototillOutputStream *stream = (RototillOutputStream *)opaque;
    if (player == 0)
        rototill_stream_record(stream, 12U, rototill_m1_message_size,
            rototill_m1_message_body);
}

static void rototill_stream_planets(void *opaque, StarsI16 player)
{
    RototillOutputStream *stream = (RototillOutputStream *)opaque;
    StarsU16 index;
    if (player == -1) {
        for (index = 0U; index < TURIN_RETAINED_POINT_COUNT; ++index) {
            rototill_stream_record(stream, 13U,
                rototill_hst_planet_sizes[index],
                rototill_hst_planet_bodies[index]);
            if (index == 24U)
                rototill_stream_record(stream, 28U,
                    sizeof rototill_produce_result, rototill_produce_result);
        }
    } else {
        for (index = 0U; index < rototill_client_planet_count; ++index)
            if (rototill_client_planet_files[index] == (StarsU16)player) {
                rototill_stream_record(stream,
                    rototill_client_planet_types[index],
                    rototill_client_planet_sizes[index],
                    rototill_client_planet_bodies[index]);
                if (player == 1 &&
                        rototill_client_planet_types[index] == 13U)
                    rototill_stream_record(stream, 28U,
                        sizeof rototill_produce_result,
                        rototill_produce_result);
            }
    }
}

static StarsI16 rototill_stream_design_predicate(void *opaque,
    StarsU16 owner, StarsU16 bank, StarsU16 slot)
{
    RototillOutputStream *stream = (RototillOutputStream *)opaque;
    StarsU16 index;
    for (index = 0U; index < rototill_all_design_count; ++index)
        if (rototill_all_design_files[index] == stream->file &&
                rototill_all_design_owners[index] == owner &&
                rototill_all_design_banks[index] == bank &&
                rototill_all_design_slots[index] == slot)
            return 1;
    return 0;
}

static void rototill_stream_design(void *opaque,
    StarsU16 owner, StarsU16 bank, StarsU16 slot)
{
    RototillOutputStream *stream = (RototillOutputStream *)opaque;
    StarsU16 index;
    for (index = 0U; index < rototill_all_design_count; ++index)
        if (rototill_all_design_files[index] == stream->file &&
                rototill_all_design_owners[index] == owner &&
                rototill_all_design_banks[index] == bank &&
                rototill_all_design_slots[index] == slot) {
            rototill_stream_record(stream, 26U,
                rototill_all_design_sizes[index],
                rototill_all_design_bodies[index]);
            return;
        }
    ++stream->trace->failures;
}

static StarsI16 rototill_stream_fleet_predicate(void *opaque, StarsU16 index)
{
    RototillOutputStream *stream = (RototillOutputStream *)opaque;
    return (StarsI16)((stream->trace->turn_visibility_fleet_flags
        [stream->file][index] & 0x0100U) != 0U);
}

static void rototill_stream_fleet(void *opaque, StarsU16 index)
{
    RototillOutputStream *stream = (RototillOutputStream *)opaque;
    const Soe3InputFleet *source =
        &stream->trace->complete_input.views[0].fleets[index];
    StarsU16 id = (StarsU16)(source->id_word & 511U);
    if (source->owner == 0 && id < TURIN_HST_OWNER0_INPUT_FLEET_COUNT) {
        rototill_stream_record(stream, 16U,
            rototill_host_fleet_body_sizes[id],
            rototill_host_fleet_bodies[id]);
        rototill_stream_record(stream, 20U,
            rototill_host_order_body_sizes[id],
            rototill_host_order_bodies[id]);
    } else if (source->owner == 1 && id < TURIN_RETAINED_FLEET_COUNT) {
        rototill_stream_record(stream, 16U,
            rototill_fleet_body_sizes[id], rototill_fleet_bodies[id]);
        rototill_stream_record(stream, 20U,
            rototill_order_body_sizes[id], rototill_order_bodies[id]);
    } else ++stream->trace->failures;
}

static void rototill_stream_scores(void *opaque, StarsI16 player)
{
    RototillOutputStream *stream = (RototillOutputStream *)opaque;
    if (player >= 0 && player < 2)
        rototill_stream_record(stream, 45U, 24U,
            stream->trace->turn_score_records[player]);
}

static void rototill_stream_things(void *opaque, StarsI16 player)
{
    RototillOutputStream *stream = (RototillOutputStream *)opaque;
    StarsU16 index;
    if (player != -1) return;
    for (index = 0U; index < rototill_thing_record_count; ++index)
        rototill_stream_record(stream, 43U, rototill_thing_sizes[index],
            rototill_thing_records[index]);
}

static void rototill_stream_plans(void *opaque, StarsI16 owner)
{
    RototillOutputStream *stream = (RototillOutputStream *)opaque;
    StarsU16 index;
    for (index = 0U; index < rototill_plan_count; ++index)
        if (rototill_plan_files[index] == stream->file &&
                rototill_plan_owners[index] == (StarsU16)owner)
            rototill_stream_record(stream, 30U,
                rototill_plan_sizes[index], rototill_plan_bodies[index]);
}

static void rototill_stream_turn(void *opaque, StarsU16 turn)
{
    RototillOutputStream *stream = (RototillOutputStream *)opaque;
    StarsU8 body[2];
    if (turn != 1U) ++stream->trace->failures;
    put_u16(body, 0U, turn);
    rototill_stream_record(stream, 0U, 2U, body);
}

static void rototill_stream_close(void *opaque)
{
    RototillOutputStream *stream = (RototillOutputStream *)opaque;
    if (!stream->opened || stream->size >
            sizeof rototill_output_files[stream->file]) {
        ++stream->trace->failures; return;
    }
    memcpy(rototill_output_files[stream->file],
        stream->bytes, stream->size);
    rototill_output_file_sizes[stream->file] = stream->size;
    stream->opened = 0U;
}

static void rototill_stream_alert(void *opaque, StarsU16 message)
{
    RototillOutputStream *stream = (RototillOutputStream *)opaque;
    (void)message;
    ++stream->trace->failures;
}

static void rototill_write_complete_file(TurinTrace *trace, StarsU16 file)
{
    if (file < 3U) {
        RototillOutputStream stream;
        StarsWriteDataFileInput input;
        StarsWriteDataFileState state;
        StarsWriteDataFileOps ops;
        memset(&stream, 0, sizeof stream);
        memset(&input, 0, sizeof input);
        memset(&state, 0, sizeof state);
        memset(&ops, 0, sizeof ops);
        stream.trace = trace;
        stream.file = file;
        input.player = file == 2U ? -1 : (StarsI16)file;
        input.append = file == 0U;
        input.player_count = 2U;
        input.fleet_count = trace->complete_input.views[0].fleet_count;
        input.turn = 1U;
        ops.create_file = rototill_stream_create;
        ops.append_file = rototill_stream_append;
        ops.write_player_predicate = rototill_stream_player_predicate;
        ops.write_player = rototill_stream_player;
        ops.write_messages = rototill_stream_messages;
        ops.write_planets = rototill_stream_planets;
        ops.write_design_predicate = rototill_stream_design_predicate;
        ops.write_design = rototill_stream_design;
        ops.write_fleet_predicate = rototill_stream_fleet_predicate;
        ops.write_fleet = rototill_stream_fleet;
        ops.write_scores = rototill_stream_scores;
        ops.write_things = rototill_stream_things;
        ops.write_battle_plans = rototill_stream_plans;
        ops.write_turn = rototill_stream_turn;
        ops.close_file = rototill_stream_close;
        ops.alert = rototill_stream_alert;
        if (!stars_write_data_file(&input, &state, &ops, &stream) ||
                stream.opened || rototill_output_file_sizes[file] == 0U)
            ++trace->failures;
    }
}

static void rototill_write_player(TurinTrace *trace, StarsI16 viewer,
    StarsI16 player,
    StarsU8 *record, StarsU16 *record_size)
{
    const StarsVisPFPlayer *view = viewer == -1 ?
        &trace->turn_host_visibility_players[player] :
        (viewer == 1 ? &trace->turn_player1_visibility_players[player] :
        &trace->turn_player0_visibility_players[player]);
    TurnPlayerWriteContext context;
    StarsU8 *data;
    if ((viewer == 1 && trace->turn_player1_visibility_calls != 1U) ||
            (viewer == 0 && trace->turn_player0_visibility_calls != 1U) ||
            (viewer == -1 && trace->turn_host_visibility_calls != 1U)) {
        ++trace->failures;
        return;
    }
    data = (StarsU8 *)calloc(65536U, 1U);
    if (!data) { ++trace->failures; return; }
    memcpy(data, retained_input_decode_prefix, sizeof retained_input_decode_prefix);
    memcpy(data + 0x3000U, trace->complete_input.views[0].players[player], 192U);
    data[0x3001U] = view->design_count;
    put_u16(data, 0x3002U, view->planet_count);
    put_u16(data, 0x3004U, view->fleet_count);
    put_u16(data, 0x3006U, view->detail_flags);
    memset(&context, 0, sizeof context);
    context.trace = trace;
    context.data = data;
    context.record = record;
    context.record_size = record_size;
    stars_write_player_record(data, 0x3000U, 0x4000U,
        turn_capture_current_player, &context);
    if (context.calls != 2U || *record_size > 266U)
        ++trace->failures;
    free(data);
}

static StarsI16 rototill_wormhole_random(void *opaque, StarsI16 limit)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsI16 index = trace->wormhole_move_random_calls;
    StarsI16 value;
    if (index < 0 || index >= TURIN_RETAINED_WORMHOLE_DRAW_COUNT) {
        ++trace->failures;
        return 0;
    }
    value = stars_random(&trace->seeds, limit);
    trace->wormhole_move_random_limits[index] = limit;
    trace->wormhole_move_random_values[index] = value;
    ++trace->wormhole_move_random_calls;
    return value;
}

static StarsI16 rototill_wormhole_validate(void *opaque,
    StarsMoveThing *thing)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsWormholePoint others[TURIN_RETAINED_THING_COUNT - 1U];
    StarsNearestPoint fleets[10];
    StarsNearestPoint planets[32];
    StarsU16 current, index, count = 0U;
    const Soe3InputView *view = &trace->complete_input.views[0];
    current = (StarsU16)(thing - trace->retained_move_things);
    if (current >= TURIN_RETAINED_THING_COUNT ||
            view->fleet_count != 10U) {
        ++trace->failures;
        return 0x0f;
    }
    for (index = 0U; index < TURIN_RETAINED_THING_COUNT; ++index) {
        const StarsMoveThing *other;
        if (index == current) continue;
        other = &trace->retained_move_things[index];
        others[count].x = other->x;
        others[count].y = other->y;
        others[count].identity = other->identity;
        others[count].flags = other->identity;
        ++count;
    }
    for (index = 0U; index < view->fleet_count; ++index) {
        const Soe3InputFleet *fleet = &view->fleets[index];
        StarsU16 id = (StarsU16)(fleet->id_word & 511U);
        if (fleet->owner == 1 && id == 0U) {
            fleets[index].x = (StarsI16)get_u16(trace->raw_fleets[0], 8U);
            fleets[index].y = (StarsI16)get_u16(trace->raw_fleets[0], 10U);
        } else {
            fleets[index].x = fleet->state.x;
            fleets[index].y = fleet->state.y;
        }
    }
    for (index = 0U; index < 32U; ++index) {
        planets[index].x = trace->complete_input.coordinates[index][0];
        planets[index].y = trace->complete_input.coordinates[index][1];
    }
    ++trace->wormhole_move_validate_calls;
    return stars_validate_wormhole_position(thing->x, thing->y,
        thing->partner_identity,
        (StarsI16)get_u16(trace->complete_input.game, 4U),
        others, count, planets, 32U, fleets, view->fleet_count);
}

static void rototill_wormhole_waypoints(void *opaque)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsU16 index;
    for (index = 0U; index < trace->complete_input.views[0].thing_count; ++index) {
        StarsU8 *raw = trace->complete_input.views[0].things[index];
        put_u16(raw, 2U, (StarsU16)trace->retained_move_things[index].x);
        put_u16(raw, 4U, (StarsU16)trace->retained_move_things[index].y);
        put_u16(raw, 6U, trace->retained_move_things[index].packed_destination);
        memcpy(trace->turn_thing_bytes + index * 18U, raw, 18U);
    }
    turn_validate_current_waypoints(trace);
    ++trace->wormhole_move_waypoint_calls;
}

static void rototill_check_wormhole_movement(TurinTrace *trace)
{
    StarsMoveThingsOps ops;
    StarsMoveThingsInput input;
    StarsU16 index;
    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(trace->retained_move_things, 0,
        sizeof trace->retained_move_things);
    for (index = 0U; index < TURIN_RETAINED_THING_COUNT; ++index) {
        const StarsU8 *raw = trace->complete_input.views[0].things[index];
        StarsMoveThing *thing = &trace->retained_move_things[index];
        StarsU16 identity = get_u16(raw, 0U);
        memcpy(trace->turn_thing_bytes + index * 18U, raw, 18U);
        thing->kind = (StarsI16)(identity >> 13);
        thing->owner = (StarsI16)((identity >> 9) & 15U);
        thing->identity = identity;
        thing->partner_identity = get_u16(raw, 12U);
        thing->x = (StarsI16)get_u16(raw, 2U);
        thing->y = (StarsI16)get_u16(raw, 4U);
        thing->packed_destination = get_u16(raw, 6U);
        thing->active = 1;
        if (thing->kind != 2) ++trace->failures;
    }
    ops.random_value = rototill_wormhole_random;
    ops.wormhole_chance = retained_wormhole_chance;
    ops.validate_wormhole = rototill_wormhole_validate;
    ops.validate_waypoints = rototill_wormhole_waypoints;
    ops.context = trace;
    input.things = trace->retained_move_things;
    input.thing_count = TURIN_RETAINED_THING_COUNT;
    input.post_production = 1;
    input.map_size =
        (StarsI16)get_u16(trace->complete_input.game, 4U);
    input.ops = &ops;
    stars_move_things(&input);
    if (trace->wormhole_move_random_calls !=
            TURIN_RETAINED_WORMHOLE_DRAW_COUNT ||
            trace->wormhole_move_validate_calls !=
            TURIN_RETAINED_THING_COUNT ||
            trace->wormhole_move_waypoint_calls != 1)
        ++trace->failures;
    for (index = 0U; index < TURIN_RETAINED_THING_COUNT; ++index) {
        StarsU8 *raw = trace->turn_thing_bytes + index * 18U;
        put_u16(raw, 2U,
            (StarsU16)trace->retained_move_things[index].x);
        put_u16(raw, 4U,
            (StarsU16)trace->retained_move_things[index].y);
        put_u16(raw, 6U,
            trace->retained_move_things[index].packed_destination);
    }
}

static StarsI16 rototill_guess_random(void *opaque, StarsI16 limit)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsI16 index = trace->guess_random_calls;
    StarsI16 value;
    if (index < 0 || index >= 2) { ++trace->failures; return 0; }
    value = stars_random(&trace->seeds, limit);
    trace->guess_random_limits[index] = limit;
    trace->guess_random_values[index] = value;
    ++trace->guess_random_calls;
    return value;
}

static void rototill_check_update_guesses(TurinTrace *trace)
{
    StarsUpdateGuessesInput input;
    StarsU16 planet;
    memset(&input, 0, sizeof input);
    for (planet = 0U; planet < 32U; ++planet) {
        const StarsReadPlanetState *source = &trace->turn_planet_states[planet];
        trace->runtime_guess_planets[planet].owner = source->player;
        trace->runtime_guess_planets[planet].guess_word = source->routing_word;
        trace->runtime_guess_planets[planet].mineral_value =
            source->minerals[3];
        trace->runtime_guess_planets[planet].survival_flags =
            (StarsU16)(source->opaque_20[1] & 0xffffU);
        pack_loaded_planet(trace->guess_runtime_planets[planet], source);
    }
    input.planets = trace->runtime_guess_planets;
    input.planet_count = 32U;
    input.random = rototill_guess_random;
    input.race_stat = retained_guess_race_stat;
    input.calc_pct_survive = retained_guess_calc_survive;
    input.context = trace;
    stars_update_guesses(&input);
    for (planet = 0U; planet < 32U; ++planet)
        trace->turn_planet_states[planet].routing_word =
            trace->runtime_guess_planets[planet].guess_word;
    if (trace->guess_random_calls != 2 || trace->guess_race_calls != 2 ||
            trace->guess_survival_calls != 2 ||
            trace->turn_planet_states[24].routing_word != 0x2047U)
        ++trace->failures;
    if (!stars_write_planet(&trace->turn_planet_states[24], 13U, 1U,
            rototill_planet_after_produce,
            sizeof rototill_planet_after_produce,
            &rototill_planet_after_produce_size, 0, 0))
        ++trace->failures;
    if (!stars_write_planet(&trace->turn_planet_states[10], 13U, 0U,
            rototill_planet10_after_produce,
            sizeof rototill_planet10_after_produce,
            &rototill_planet10_after_produce_size, 0, 0))
        ++trace->failures;
    for (planet = 0U; planet < TURIN_RETAINED_POINT_COUNT; ++planet)
        if (!stars_write_planet(&trace->turn_planet_states[planet],
                13U, 1U, rototill_hst_planet_bodies[planet],
                sizeof rototill_hst_planet_bodies[planet],
                &rototill_hst_planet_sizes[planet], 0, 0))
            ++trace->failures;
}

static StarsI16 rototill_mining_random(void *opaque, StarsI16 limit)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsI16 value;
    StarsI16 index = trace->mining_random_calls;
    if (limit != 100 || index < 0 ||
            index >= TURIN_RETAINED_MINE_DRAW_COUNT) {
        ++trace->failures;
        return 0;
    }
    value = stars_random(&trace->seeds, limit);
    trace->mining_random_limits[index] = limit;
    trace->mining_random_values[index] = value;
    ++trace->mining_random_calls;
    return value;
}

static void rototill_check_full_map_mining(TurinTrace *trace)
{
    StarsU16 planet;
    StarsI16 owned = 0;
    StarsI16 mineral;
    memset(trace->mining_planets, 0, sizeof trace->mining_planets);
    memset(trace->mining_inputs, 0, sizeof trace->mining_inputs);
    memset(trace->mining_stock, 0, sizeof trace->mining_stock);
    memset(&trace->mining_ops, 0, sizeof trace->mining_ops);
    trace->mining_ops.random = rototill_mining_random;
    trace->mining_ops.context = trace;
    for (planet = 0U; planet < TURIN_RETAINED_POINT_COUNT; ++planet) {
        const StarsReadPlanetState *source = &trace->turn_planet_states[planet];
        StarsMineralMinedInput *input;
        StarsCMaxOperableMinesOps mine_ops;
        StarsU8 *raw = trace->mining_planets + planet * 56U;
        pack_loaded_planet(raw, source);
        if (source->player < 0) continue;
        if (owned >= 2) { ++trace->failures; break; }
        trace->mining_runtime_ids[owned] = (StarsU16)source->id;
        input = &trace->mining_inputs[owned];
        input->player = source->player;
        input->special_race = turn_score_resource_stat(trace,
            source->player, 14) == 8;
        input->efficiency_tenths = turn_score_resource_stat(trace,
            source->player, 4);
        memset(&mine_ops, 0, sizeof mine_ops);
        mine_ops.max_mines.calc_max_population = turn_score_max_population;
        mine_ops.max_mines.get_player_stat = turn_score_resource_stat;
        input->operating_mines = stars_cmines_operating(raw, &mine_ops, trace);
        input->c_mines = -1;
        input->force_concentration_floor = (source->flags >> 10) & 1U;
        input->random_rounding = 1;
        input->mineral_weight = trace->mining_stock[owned];
        input->ops = &trace->mining_ops;
        for (mineral = 0; mineral < 3; ++mineral) {
            input->concentration[mineral] =
                source->mineral_concentrations[mineral];
            input->percent_level[mineral] = source->mineral_levels[mineral];
            trace->mining_stock[owned][mineral] =
                (StarsI32)source->minerals[mineral];
        }
        ++owned;
    }
    if (owned == 2)
        stars_mine_minerals(trace->mining_planets,
            TURIN_RETAINED_POINT_COUNT,
            estimate_retained_mining_planet, trace);
    if (owned != 2 || trace->mining_visited != 32U ||
            trace->mining_planet_calls != 2 ||
            trace->mining_random_calls != TURIN_RETAINED_MINE_DRAW_COUNT ||
            trace->mining_stock[1][0] != 539L ||
            trace->mining_stock[1][1] != 965L ||
            trace->mining_stock[1][2] != 506L ||
            trace->mining_inputs[1].percent_level[0] != 247U ||
            trace->mining_inputs[1].percent_level[1] != 241U ||
            trace->mining_inputs[1].percent_level[2] != 247U) {
        fprintf(stderr, "rot mining owned=%d visits=%u calls=%d draws=%d stock=%ld,%ld,%ld levels=%u,%u,%u\n",
            owned, trace->mining_visited, trace->mining_planet_calls,
            trace->mining_random_calls,
            (long)trace->mining_stock[1][0],
            (long)trace->mining_stock[1][1],
            (long)trace->mining_stock[1][2],
            trace->mining_inputs[1].percent_level[0],
            trace->mining_inputs[1].percent_level[1],
            trace->mining_inputs[1].percent_level[2]);
        ++trace->failures;
    }
}

static StarsI16 rototill_is_transport(void *opaque,
    const StarsRototillTurnFleet *fleet)
{
    StarsRototillScoutAdapter *adapter;
    TurinTrace *trace;
    StarsI16 kinds[16];
    StarsU16 index;
    adapter = (StarsRototillScoutAdapter *)opaque;
    trace = (TurinTrace *)adapter->context;
    for (index = 0U; index < 16U; ++index)
        kinds[index] = (StarsI16)get_u16(trace->designs + index * 147U, 0U);
    return stars_f_is_ai_transport(fleet->ship_counts, kinds, 16U,
        (StarsU16)fleet->owner, 0, 0, trace);
}

static StarsI16 rototill_select_fleet(void *opaque,
    const StarsRototillTurnFleet *fleet, StarsI16 player)
{
    TurinTrace *trace;
    trace = (TurinTrace *)opaque;
    if (!fleet || player != 1 || (fleet->id != 0 && fleet->id != 1))
        ++trace->failures;
    return 1;
}

static StarsI16 rototill_refresh_fleet(void *opaque,
    const StarsRototillTurnFleet *fleet, StarsI16 player)
{
    TurinTrace *trace;
    StarsLogFleet before;
    StarsLogFleet after;
    StarsLogChangeFleetState state;
    StarsLogChangeFleetOps log_ops;
    const Soe3InputFleet *original;
    StarsU16 index;
    trace = (TurinTrace *)opaque;
    if (fleet && fleet->id == 1) {
        if (player != 1 || get_u32(trace->raw_fleets[1], 88U) != 25UL)
            ++trace->failures;
        return 1;
    }
    if (!fleet || player != 1 || fleet->id != 0 ||
            fleet->raw_plord != trace->plord[0] ||
            fleet->raw_plord[3] != 2U ||
            get_u16(fleet->raw_plord, 26U) != 26U)
        ++trace->failures;
    original = 0;
    for (index = 0U; index < trace->complete_input.views[2].fleet_count;
            ++index)
        if ((trace->complete_input.views[2].fleets[index].id_word & 511U)
                == (StarsU16)fleet->id)
            original = &trace->complete_input.views[2].fleets[index];
    if (!original)
        return 0;
    memset(&before, 0, sizeof before);
    before.object_id = original->id_word;
    before.order_count = original->state.order_count;
    before.orders = original->state.orders;
    after = before;
    after.order_count = fleet->raw_plord[3];
    after.orders = fleet->raw_plord + 4U;
    memset(&state, 0, sizeof state);
    memset(&log_ops, 0, sizeof log_ops);
    log_ops.write_memory = startup_capture_memory_log;
    stars_log_change_fleet(&before, &after, trace->game_flags,
        &state, &log_ops, trace);
    return 1;
}

static StarsI16 rototill_nearest_colonizable(void *opaque,
    const StarsRototillTurnFleet *fleet, StarsI16 player)
{
    StarsRototillScoutAdapter *adapter;
    TurinTrace *trace;
    StarsPoint point;
    StarsU8 status[TURIN_RETAINED_POINT_COUNT];
    StarsU16 index;
    adapter = (StarsRototillScoutAdapter *)opaque;
    trace = (TurinTrace *)adapter->context;
    if (!fleet || fleet->id != 1 || player != 1 ||
            !adapter->marked_planets)
        ++trace->failures;
    for (index = 0U; index < TURIN_RETAINED_POINT_COUNT; ++index)
        status[index] = adapter->raw_ai_planet_table[index * 16U + 15U];
    point.x = (StarsI16)get_u16(fleet->raw_fleet_record, 8U);
    point.y = (StarsI16)get_u16(fleet->raw_fleet_record, 10U);
    return stars_id_nearest_colonizable_planet(point, fleet->planet_id,
        adapter->race_mode, adapter->turn, 1, player, status,
        adapter->planet_owners, adapter->planet_points,
        TURIN_RETAINED_POINT_COUNT, 0, 0U, 0, trace, 0);
}

static StarsI16 rototill_transfer_supply(void *opaque,
    const StarsRototillTurnFleet *fleet, StarsI16 player)
{
    StarsRototillScoutAdapter *adapter;
    TurinTrace *trace;
    StarsCargoState before;
    StarsI16 transferred;
    StarsLogXfer first;
    StarsLogXfer second;
    StarsU8 *data;
    StarsU8 log[28];
    StarsU16 lane;
    StarsU16 header;
    adapter = (StarsRototillScoutAdapter *)opaque;
    trace = (TurinTrace *)adapter->context;
    if (!fleet || fleet->id != 1 || fleet->planet_id != 24 || player != 1)
        return 0;
    before = trace->supply_fleet;
    transferred = stars_xfer_ai_supply(1, fleet->planet_id, 2,
        fleet->id, 3, 25, change_supply, trace);
    if (transferred != 25)
        return 0;
    data = (StarsU8 *)calloc(0x5262U, 1U);
    if (!data)
        return 0;
    memset(&first, 0, sizeof first);
    memset(&second, 0, sizeof second);
    memset(log, 0, sizeof log);
    first.id = (StarsU16)(fleet->id | (player << 9));
    first.grobj = 2U;
    second.id = (StarsU16)fleet->planet_id;
    second.grobj = 1U;
    for (lane = 0U; lane < 5U; ++lane)
        first.item[lane] = trace->supply_fleet.minerals[lane] -
            before.minerals[lane];
    stars_log_make_valid_xfer(data, log, &first, &second, 0, trace);
    header = get_u16(log, 0U);
    if (get_u16(data, 0x09a4U) != (header & 1023U) + 2U)
        ++trace->failures;
    else
        startup_capture_memory_log(trace, (StarsU16)(header >> 10),
            (StarsU16)(header & 1023U), log + 2U);
    free(data);
    return 1;
}

static StarsI16 rototill_ops_select_fleet(void *opaque,
    const StarsRototillTurnFleet *fleet, StarsI16 player)
{
    StarsRototillScoutAdapter *adapter;
    adapter = (StarsRototillScoutAdapter *)opaque;
    return rototill_select_fleet(adapter->context, fleet, player);
}

static StarsI16 rototill_ops_refresh_fleet(void *opaque,
    const StarsRototillTurnFleet *fleet, StarsI16 player)
{
    StarsRototillScoutAdapter *adapter;
    adapter = (StarsRototillScoutAdapter *)opaque;
    return rototill_refresh_fleet(adapter->context, fleet, player);
}

static StarsI16 rototill_has_teeth(void *opaque,
    const StarsRototillTurnFleet *fleet)
{
    TurinTrace *trace;
    trace = (TurinTrace *)opaque;
    ++trace->fleet_teeth_calls;
    return stars_fleet_might_have_teeth(fleet->ship_counts,
        (StarsU16)fleet->owner << 9, lookup_hull_components, trace);
}

static StarsI16 rototill_ideal_warp(void *opaque,
    const StarsRototillTurnFleet *fleet, StarsI16 target, StarsPoint point)
{
    TurinTrace *trace;
    trace = (TurinTrace *)opaque;
    ++trace->order_warp_calls;
    if (!fleet || fleet->id != 0 || target != 26 ||
            point.x != retained_points[26].x ||
            point.y != retained_points[26].y)
        ++trace->failures;
    return current_ideal_warp(trace, fleet->ship_counts, 1);
}

static StarsI16 rototill_random_value(void *opaque, StarsI16 limit)
{
    TurinTrace *trace;
    StarsI16 value;
    trace = (TurinTrace *)opaque;
    value = stars_random(&trace->seeds, limit);
    if (trace->random_calls == 0 && (limit != 100 || value != 31))
        ++trace->failures;
    ++trace->random_calls;
    return value;
}

static const StarsU8 *rototill_find_wormhole(void *opaque,
    StarsPoint source, StarsU32 distance)
{
    TurinTrace *trace;
    const StarsU8 *things[TURIN_RETAINED_THING_COUNT];
    StarsU16 index;
    trace = (TurinTrace *)opaque;
    ++trace->wormhole_calls;
    for (index = 0U; index < TURIN_RETAINED_THING_COUNT; ++index)
        things[index] = trace->complete_input.views[0].things[index];
    return stars_lpth_worm_find(source, distance, 1, things,
        TURIN_RETAINED_THING_COUNT, 0, rototill_random_value, trace);
}

static StarsI16 rototill_ensure_ai(void *opaque, StarsI16 percent,
    StarsI16 *latest_starbase)
{
    StarsRototillScoutAdapter *adapter;
    TurinTrace *trace;
    StarsIroAiOps iro_ops;
    StarsU8 no_requirements[1];
    StarsU16 index;
    adapter = (StarsRototillScoutAdapter *)opaque;
    trace = (TurinTrace *)adapter->context;
    no_requirements[0] = 0U;
    ++trace->startup_ensure_calls;
    memset(&trace->startup_research, 0, sizeof trace->startup_research);
    for (index = 0U; index < 6U; ++index)
        trace->startup_research.tech[index] =
            trace->complete_input.views[0].players[1][26U + index];
    trace->startup_research.current_tech =
        trace->complete_input.views[0].players[1][57U];
    trace->startup_research.percent_research =
        trace->complete_input.views[0].players[1][56U];
    memset(&iro_ops, 0, sizeof iro_ops);
    iro_ops.ensure_designs = startup_ensure_designs;
    iro_ops.latest_starbase = startup_latest_starbase;
    iro_ops.validate_history = startup_validate_history;
    iro_ops.write_research = startup_write_research;
    trace->startup_research_order = stars_iro_ensure_ai(no_requirements,
        0, latest_starbase, percent, &trace->startup_research, &iro_ops,
        &trace->scout);
    /* IroEnsureAi writes pctResearch in the live rgplr table.  The
     * retained input byte is 15, but Rototill passes zero at turn 0. */
    if (percent != 0 || trace->startup_research.percent_research != 0)
        ++trace->failures;
    trace->complete_input.views[0].players[1][56U] =
        (StarsU8)trace->startup_research.percent_research;
    if (latest_starbase)
        trace->startup_latest_design = *latest_starbase;
    return trace->startup_research_order;
}

typedef struct RototillBasicPlanetContext {
    TurinTrace *trace;
    StarsAiProductionCandidate catalog[128];
    StarsAiProductionCandidate pending[1];
    StarsI32 resources[4];
    StarsI32 costs[4];
    StarsU16 action_calls;
    StarsU16 finish_calls;
    StarsU16 fix_calls;
    StarsU16 blocked_calls;
} RototillBasicPlanetContext;

static StarsI16 rototill_basic_upgrade(void *opaque, StarsU16 id)
{
    RototillBasicPlanetContext *basic = (RototillBasicPlanetContext *)opaque;
    TurinTrace *trace = basic->trace;
    StarsAiUpgradeStarbase starbase;
    StarsUpgradeStarbaseOps ops;
    StarsU16 lane;
    if (id != 24U) ++trace->failures;
    memset(&starbase, 0, sizeof starbase);
    memset(&ops, 0, sizeof ops);
    starbase.family = (StarsU16)(trace->complete_input.views[2].planets[0].opaque_44 & 15UL);
    for (lane = 0U; lane < 3U; ++lane)
        starbase.resource_values[lane] = (StarsU16)
            trace->complete_input.views[2].planets[0].minerals[lane];
    ops.random_value = startup_prepare_random;
    ops.queue_upgrade = startup_queue_upgrade;
    ++basic->action_calls;
    return stars_f_upgrade_ai_starbase(&starbase,
        trace->startup_latest_design, 0, 3U, &ops, trace);
}

static StarsI16 rototill_basic_fling(void *opaque, StarsU16 id)
{
    RototillBasicPlanetContext *basic = (RototillBasicPlanetContext *)opaque;
    StarsAiFlingPlanet planet;
    StarsAiFlingOps ops;
    if (id != 24U) ++basic->trace->failures;
    memset(&planet, 0, sizeof planet);
    memset(&ops, 0, sizeof ops);
    planet.owner = 1;
    ops.random_value = startup_prepare_random;
    ops.queue_item = startup_basic_unexpected_packet;
    ++basic->action_calls;
    return stars_f_ai_fling(&planet, 1U, basic->trace->points[24],
        0U, 1, basic->resources, 0U, &ops, basic->trace);
}

static StarsI16 rototill_basic_scanner(void *opaque, StarsU16 id)
{
    RototillBasicPlanetContext *basic = (RototillBasicPlanetContext *)opaque;
    if (id != 24U) ++basic->trace->failures;
    ++basic->action_calls;
    return stars_f_queue_ai_scanner(basic->pending, 0U, basic->catalog,
        basic->trace->final_queue_catalog.item_count, basic->resources,
        basic->costs, startup_unexpected_defense_add, basic->trace);
}

static StarsI16 rototill_basic_defenses(void *opaque, StarsU16 id)
{
    RototillBasicPlanetContext *basic = (RototillBasicPlanetContext *)opaque;
    if (id != 24U) ++basic->trace->failures;
    ++basic->action_calls;
    return stars_f_queue_ai_defenses(
        (StarsI32)basic->trace->supply_planet.minerals[3],
        get_u32(basic->trace->final_queue_planet, 24U) & 4095UL,
        0U, 9, 9, startup_unexpected_defense_lookup,
        startup_unexpected_defense_add, basic->trace);
}

static StarsI16 rototill_basic_terraforming(void *opaque, StarsU16 id)
{
    RototillBasicPlanetContext *basic = (RototillBasicPlanetContext *)opaque;
    if (id != 24U) ++basic->trace->failures;
    ++basic->action_calls;
    return stars_f_queue_ai_terraforming(3, 0L, 225U,
        basic->pending, 0U, basic->catalog,
        basic->trace->final_queue_catalog.item_count,
        basic->costs, startup_unexpected_defense_add, basic->trace);
}

static void rototill_basic_free(void *opaque, StarsU8 *product)
{
    RototillBasicPlanetContext *basic = (RototillBasicPlanetContext *)opaque;
    if (product != (StarsU8 *)&basic->trace->final_queue_copy)
        ++basic->trace->failures;
}

static void rototill_basic_finish(void *opaque, StarsU16 id,
    StarsI16 changed)
{
    RototillBasicPlanetContext *basic = (RototillBasicPlanetContext *)opaque;
    StarsFinishProductionInput input;
    StarsFinishProductionOps ops;
    if (id != 24U || changed != 0)
        ++basic->trace->failures;
    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    input.planet_bits = get_u32(basic->trace->final_queue_planet, 24U);
    input.game_flags = basic->trace->game_flags;
    input.player = 1;
    input.planet_id = 24;
    input.ai_active = 1;
    input.global_product = (StarsU8 *)&basic->trace->final_queue_copy;
    ops.free_product = rototill_basic_free;
    input.ops = &ops;
    input.context = basic;
    stars_finish_production(&input);
    put_u32(basic->trace->final_queue_planet, 24U, input.planet_bits);
    ++basic->finish_calls;
}

static void rototill_basic_fix_attack(void *opaque)
{
    RototillBasicPlanetContext *basic = (RototillBasicPlanetContext *)opaque;
    StarsU16 turn = get_u16(basic->trace->complete_input.game, 18U);
    StarsU16 size = get_u16(basic->trace->complete_input.game, 4U);
    if (turn >= (StarsU16)(size * 10U + 20U))
        ++basic->trace->failures;
    ++basic->fix_calls;
}

static void rototill_basic_blocked(void *opaque)
{
    RototillBasicPlanetContext *basic = (RototillBasicPlanetContext *)opaque;
    /* The original blocked-queue walker skips this planet before reading
     * its first item when it has no production allocation. */
    if (basic->trace->turn_ai_queue.count != 0U)
        ++basic->trace->failures;
    ++basic->blocked_calls;
}

static StarsI16 rototill_run_basic_tasks(void *opaque, StarsI16 research,
    StarsI16 latest_starbase, StarsI16 player)
{
    StarsRototillScoutAdapter *adapter = (StarsRototillScoutAdapter *)opaque;
    TurinTrace *trace = (TurinTrace *)adapter->context;
    RototillBasicPlanetContext basic;
    StarsAiBasicPlanet planet;
    StarsHandleBasicOps ops;
    StarsGetResourcesAvailableOps resource_ops;
    StarsSeeds upgrade_preview;
    StarsI16 upgrade_draw;
    StarsU16 random_calls_before;
    StarsI16 failures = trace->failures;
    if (research != 0x39e || latest_starbase != 0 || player != 1)
        ++trace->failures;
    startup_basic_prelude(trace, latest_starbase);
    if (trace->basic_keep_calls != 1U ||
            trace->basic_starbase_calls != 1U ||
            trace->basic_starbase_visits != 1U ||
            trace->basic_speed_calls != 1U ||
            trace->plord[0][3] != 2U ||
            get_u16(trace->plord[0], 28U) != 0x1150U)
        ++trace->failures;
    startup_select_current_planet(trace, 24);
    if (get_u32(trace->final_queue_planet, 40U) != 225UL)
        ++trace->failures;
    trace->game_flags = (StarsU16)((trace->game_flags & ~0x20U) |
        (((get_u32(trace->final_queue_planet, 24U) >> 23) & 1UL) << 5));
    startup_build_catalog(trace, &trace->final_queue_catalog,
        &trace->final_queue_copy, trace->game_flags);
    memset(&basic, 0, sizeof basic);
    basic.trace = trace;
    startup_basic_candidates(trace, &trace->final_queue_catalog,
        basic.catalog);
    memset(&resource_ops, 0, sizeof resource_ops);
    resource_ops.estimate_minerals = startup_final_estimate;
    resource_ops.resources_at_planet = startup_final_resources;
    resource_ops.research_percent = startup_final_percent;
    stars_get_resources_available(trace->final_queue_planet,
        basic.resources, 1, get_u32(trace->final_queue_planet, 24U),
        &resource_ops, trace);
    memset(&planet, 0, sizeof planet);
    planet.id = 24U;
    planet.owner = 1;
    planet.research_level = 3;
    planet.mineral_total = 225U;
    memset(&ops, 0, sizeof ops);
    ops.upgrade_starbase = rototill_basic_upgrade;
    ops.fling = rototill_basic_fling;
    ops.scanner = rototill_basic_scanner;
    ops.defenses = rototill_basic_defenses;
    ops.terraforming = rototill_basic_terraforming;
    ops.finish_production = rototill_basic_finish;
    ops.fix_planets_under_attack = rototill_basic_fix_attack;
    ops.add_mines_blocked = rototill_basic_blocked;
    upgrade_preview = trace->seeds;
    upgrade_draw = stars_random(&upgrade_preview, 100);
    random_calls_before = trace->startup_prepare_random_calls;
    stars_handle_basic_ai_tasks(&planet, 1U, 1, latest_starbase,
        &ops, &basic);
    if (basic.action_calls != 5U || basic.finish_calls != 1U ||
            basic.fix_calls != 1U || basic.blocked_calls != 1U ||
            startup_final_percent(trace, 1) != 0 ||
            get_u32(trace->final_queue_planet, 24U) != 0x0000000aUL ||
            (trace->game_flags & 0x20U) != 0U ||
            upgrade_draw != 12 ||
            trace->startup_prepare_random_calls !=
                (StarsU16)(random_calls_before + 1U) ||
            trace->seeds.first != upgrade_preview.first ||
            trace->seeds.second != upgrade_preview.second)
        ++trace->failures;
    return trace->failures == failures;
}

typedef struct RototillQueueBoundary {
    TurinTrace *trace;
    StarsU16 quantity;
    StarsU16 mine_quantity;
    StarsU16 calls;
} RototillQueueBoundary;

static void rototill_queue_cost(void *opaque, StarsU32 item,
    StarsI32 costs[4])
{
    RototillQueueBoundary *boundary = (RototillQueueBoundary *)opaque;
    StarsProductionCostInput input;
    StarsProductionCostOps ops;
    StarsU32 output[4];
    StarsI16 lane;
    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    input.item_variant = (StarsU16)((item >> 17) & 7UL);
    input.item_kind = (StarsU16)((item >> 10) & 127UL);
    input.quantity = (StarsU16)(item & 1023UL);
    input.player = 1;
    input.planet_player = 1;
    input.only_one = 1;
    input.game_flags = boundary->trace->game_flags;
    ops.race_stat = turn_score_resource_stat;
    ops.race_grbit = startup_part_race_bit;
    input.ops = &ops;
    input.context = boundary->trace;
    stars_get_production_costs(&input, output);
    for (lane = 0; lane < 4; ++lane)
        costs[lane] = (StarsI32)output[lane];
}

static StarsI16 rototill_queue_dry_add(void *opaque, StarsI16 kind,
    StarsU16 quantity, StarsI16 now, StarsI16 flags)
{
    RototillQueueBoundary *boundary = (RototillQueueBoundary *)opaque;
    (void)now;
    (void)flags;
    if (kind == 7 && boundary->calls == 0U)
        boundary->quantity = quantity;
    else if (kind == 8 && boundary->calls == 1U)
        boundary->mine_quantity = quantity;
    else
        ++boundary->trace->failures;
    ++boundary->calls;
    return 1;
}

static void rototill_check_queue_boundary(TurinTrace *trace,
    StarsU32 population, StarsI16 no_research,
    StarsI32 expected_resources,
    StarsI16 expected_max_factories, StarsU16 expected_quantity,
    StarsU16 expected_mines)
{
    StarsGetResourcesAvailableOps resource_ops;
    StarsCMaxMinesOps capacity_ops;
    StarsCMaxOperableMinesOps mine_ops;
    StarsCMaxOperableFactoriesOps factory_ops;
    StarsFillProdMinesAndFactoriesInput fill;
    RototillQueueBoundary boundary;
    StarsI32 cost[4];
    StarsU32 selection_flags;
    memset(&boundary, 0, sizeof boundary);
    boundary.trace = trace;
    pack_loaded_planet(trace->final_queue_planet,
        &trace->complete_input.views[2].planets[0]);
    if (get_u32(trace->final_queue_planet, 24U) != 0x0000000aUL)
        ++trace->failures;
    put_u32(trace->final_queue_planet, 40U, population);
    startup_build_catalog(trace, &trace->final_queue_catalog,
        &trace->final_queue_copy, trace->game_flags);
    if (trace->final_queue_catalog.item_count != 18U)
        ++trace->failures;
    memset(&resource_ops, 0, sizeof resource_ops);
    resource_ops.estimate_minerals = startup_final_estimate;
    resource_ops.resources_at_planet = startup_final_resources;
    resource_ops.research_percent = startup_final_percent;
    memset(&fill, 0, sizeof fill);
    selection_flags = get_u32(trace->final_queue_planet, 24U);
    if (no_research)
        selection_flags |= 0x00800000UL;
    stars_get_resources_available(trace->final_queue_planet,
        fill.available, 1, selection_flags,
        &resource_ops, trace);
    memset(&capacity_ops, 0, sizeof capacity_ops);
    capacity_ops.calc_max_population = startup_final_max_population;
    capacity_ops.get_player_stat = turn_score_resource_stat;
    memset(&mine_ops, 0, sizeof mine_ops);
    mine_ops.max_mines = capacity_ops;
    memset(&factory_ops, 0, sizeof factory_ops);
    factory_ops.max_factories = capacity_ops;
    fill.max_mines = stars_cmax_mines(trace->final_queue_planet, 1,
        &capacity_ops, trace);
    fill.mines_operating = stars_cmines_operating(trace->final_queue_planet,
        &mine_ops, trace);
    fill.max_factories = stars_cmax_operable_factories(
        trace->final_queue_planet, 1, 0, &factory_ops, trace);
    fill.factories_operating = stars_cfactories_operating(
        trace->final_queue_planet, &factory_ops, trace);
    fill.global_queue_items = trace->final_queue_catalog.items;
    fill.global_queue_count = trace->final_queue_catalog.item_count;
    fill.get_costs = rototill_queue_cost;
    fill.add_item = rototill_queue_dry_add;
    fill.context = &boundary;
    rototill_queue_cost(&boundary, 0x00021c01UL, cost);
    if (fill.available[3] != expected_resources ||
            cost[2] != 3L || cost[3] != 10L ||
            fill.factories_operating != 10 ||
            fill.max_factories != expected_max_factories ||
            !stars_fill_prod_mines_and_factories(&fill) ||
            boundary.calls != (StarsU16)(expected_mines ? 2U : 1U) ||
            boundary.quantity != expected_quantity ||
            boundary.mine_quantity != expected_mines)
        ++trace->failures;
    if (population == 225UL && !no_research) {
        StarsAiProductionCandidate pending[1], catalog[128];
        StarsAiFlingPlanet fling_planet;
        StarsAiFlingOps fling_ops;
        StarsI32 basic_cost[4];
        StarsU16 index;
        memset(pending, 0, sizeof pending);
        memset(catalog, 0, sizeof catalog);
        memset(&fling_planet, 0, sizeof fling_planet);
        memset(&fling_ops, 0, sizeof fling_ops);
        memset(basic_cost, 0, sizeof basic_cost);
        startup_basic_candidates(trace, &trace->final_queue_catalog, catalog);
        for (index = 0U; index < trace->final_queue_catalog.item_count;
                ++index)
            if (catalog[index].category == 1U &&
                    (catalog[index].subtype == 12U ||
                     (catalog[index].subtype >= 18U &&
                      catalog[index].subtype <= 26U)))
                ++trace->failures;
        if (fill.available[0] != 538L ||
                fill.available[1] != 964L ||
                fill.available[2] != 506L ||
                trace->supply_planet.minerals[3] != 225UL ||
                trace->startup_latest_design != 0)
            ++trace->failures;
        fling_planet.owner = 1;
        fling_ops.random_value = startup_prepare_random;
        fling_ops.queue_item = startup_basic_unexpected_packet;
        if (stars_f_ai_fling(&fling_planet, 1U, trace->points[24],
                0U, 1, fill.available, 0U, &fling_ops, trace) != 0 ||
                stars_f_queue_ai_defenses(225L, 0UL, 0U, 9, 9,
                    startup_unexpected_defense_lookup,
                    startup_unexpected_defense_add, trace) != 0 ||
                stars_f_queue_ai_scanner(pending, 0U, catalog,
                    trace->final_queue_catalog.item_count, fill.available,
                    basic_cost, startup_unexpected_defense_add, trace) != 0 ||
                stars_f_queue_ai_terraforming(3, 0L, 225U,
                    pending, 0U, catalog,
                    trace->final_queue_catalog.item_count, basic_cost,
                    startup_unexpected_defense_add, trace) != 0)
            ++trace->failures;
    }
}

static void rototill_final_select(void *opaque, StarsI16 kind, StarsI16 id)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    if (kind != 1 || id != 24 || trace->turn_ai_queue.count != 0U)
        ++trace->failures;
    startup_select_current_planet(trace, id);
    if (get_u32(trace->final_queue_planet, 40U) != 225UL ||
            (get_u32(trace->final_queue_planet, 24U) & 0x00800000UL) != 0UL)
        ++trace->failures;
    trace->game_flags = (StarsU16)((trace->game_flags & ~0x20U) |
        (((get_u32(trace->final_queue_planet, 24U) >> 23) & 1UL) << 5));
}

static void rototill_final_initialize(void *opaque, StarsU8 *scratch,
    StarsU16 size)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    if (!scratch || size != 256U || trace->turn_ai_queue.count != 0U)
        ++trace->failures;
    trace->turn_ai_queue.records = trace->turn_ai_queue_records;
    trace->turn_ai_queue.capacity = 64U;
    startup_build_catalog(trace, &trace->final_queue_catalog,
        &trace->final_queue_copy, trace->game_flags);
    if (trace->final_queue_catalog.item_count != 18U ||
            trace->final_queue_copy.item_count != 0U)
        ++trace->failures;
}

static StarsI16 rototill_final_add(void *opaque, StarsI16 kind,
    StarsU16 quantity, StarsI16 now, StarsI16 flags)
{
    RototillQueueBoundary *boundary = (RototillQueueBoundary *)opaque;
    TurinTrace *trace = boundary->trace;
    if (kind != 7 || quantity != 3U || now != 1 || flags != 1 ||
            trace->turn_ai_queue.count != 0U)
        ++trace->failures;
    stars_add_item_to_queue(0, &trace->turn_ai_queue, (StarsU16)kind,
        quantity, 1U, 1, 0, trace);
    if (trace->turn_ai_queue.count != 1U ||
            get_u32(trace->turn_ai_queue.records, 0U) != 0x00021c03UL)
        ++trace->failures;
    return 1;
}

static StarsI16 rototill_final_fill(void *opaque, const StarsU8 *planet)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsGetResourcesAvailableOps resource_ops;
    StarsGetProdQCostOps queue_ops;
    StarsCMaxMinesOps capacity_ops;
    StarsCMaxOperableMinesOps mine_ops;
    StarsCMaxOperableFactoriesOps factory_ops;
    StarsFillProdMinesAndFactoriesInput fill;
    RototillQueueBoundary boundary;
    StarsI16 result;
    if (planet != trace->final_queue_planet ||
            get_u32(planet, 40U) != 225UL ||
            startup_final_percent(trace, 1) != 0)
        ++trace->failures;
    memset(&resource_ops, 0, sizeof resource_ops);
    resource_ops.estimate_minerals = startup_final_estimate;
    resource_ops.resources_at_planet = startup_final_resources;
    resource_ops.research_percent = startup_final_percent;
    memset(&queue_ops, 0, sizeof queue_ops);
    queue_ops.get_queue = startup_final_queue;
    queue_ops.get_costs = startup_final_cost;
    memset(&capacity_ops, 0, sizeof capacity_ops);
    capacity_ops.calc_max_population = startup_final_max_population;
    capacity_ops.get_player_stat = turn_score_resource_stat;
    memset(&mine_ops, 0, sizeof mine_ops);
    mine_ops.max_mines = capacity_ops;
    memset(&factory_ops, 0, sizeof factory_ops);
    factory_ops.max_factories = capacity_ops;
    memset(&fill, 0, sizeof fill);
    memset(&boundary, 0, sizeof boundary);
    boundary.trace = trace;
    stars_get_resources_available(planet, fill.available, 1,
        get_u32(planet, 24U), &resource_ops, trace);
    stars_get_prod_q_cost(planet, fill.queued_cost, 1,
        &queue_ops, trace);
    fill.max_mines = stars_cmax_mines(planet, 1, &capacity_ops, trace);
    fill.mines_operating = stars_cmines_operating(planet, &mine_ops, trace);
    fill.max_factories = stars_cmax_operable_factories(planet, 1, 0,
        &factory_ops, trace);
    fill.factories_operating = stars_cfactories_operating(planet,
        &factory_ops, trace);
    fill.global_queue_items = trace->final_queue_catalog.items;
    fill.global_queue_count = trace->final_queue_catalog.item_count;
    fill.get_costs = rototill_queue_cost;
    fill.add_item = rototill_final_add;
    fill.context = &boundary;
    if (fill.available[3] != 32L || fill.queued_cost[3] != 0L ||
            fill.max_factories != 22 || fill.factories_operating != 10)
        ++trace->failures;
    result = stars_fill_prod_mines_and_factories(&fill);
    if (result != 1 || trace->turn_ai_queue.count != 1U)
        ++trace->failures;
    return result;
}

static void rototill_final_free(void *opaque, StarsU8 *product)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    if (product != 0)
        ++trace->failures;
}

static void rototill_final_lookup(void *opaque, StarsI16 id,
    StarsU8 *product)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    if (product != trace->turn_ai_queue.records ||
            (trace->turn_finish_lookup_calls == 0 && id != -1) ||
            (trace->turn_finish_lookup_calls == 1 && id != 24) ||
            trace->turn_finish_lookup_calls >= 2)
        ++trace->failures;
    ++trace->turn_finish_lookup_calls;
}

static void rototill_final_finish(void *opaque, StarsI16 changed)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsFinishProductionInput input;
    StarsFinishProductionOps ops;
    StarsLogPlanet old_planet, new_planet;
    StarsLogChangePlanetState log_state;
    StarsLogChangePlanetOps log_ops;
    StarsU16 lane;
    if (changed != 1 || trace->turn_ai_queue.count != 1U)
        ++trace->failures;
    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    input.write = changed;
    input.game_flags = trace->game_flags;
    input.planet_bits = get_u32(trace->final_queue_planet, 24U);
    input.player = 1;
    input.planet_id = 24;
    input.ai_active = trace->ai_input.ai_active;
    input.global_product = trace->turn_ai_queue.records;
    input.global_count = (StarsU8)trace->turn_ai_queue.count;
    ops.free_product = rototill_final_free;
    ops.lookup_planet = rototill_final_lookup;
    input.ops = &ops;
    input.context = trace;
    stars_finish_production(&input);
    if (input.global_product ||
            input.selected_product != trace->turn_ai_queue.records ||
            trace->turn_finish_lookup_calls != 2)
        ++trace->failures;
    memset(&old_planet, 0, sizeof old_planet);
    old_planet.object_id = 24;
    for (lane = 0U; lane < 4U; ++lane)
        old_planet.minerals[lane] = trace->supply_planet.minerals[lane];
    old_planet.overlap_20 = get_u32(trace->final_queue_planet, 24U);
    old_planet.overlap_44 = get_u32(trace->final_queue_planet, 44U);
    old_planet.overlap_48 = get_u16(trace->final_queue_planet, 48U);
    new_planet = old_planet;
    new_planet.has_production = 1U;
    new_planet.production_count = trace->turn_ai_queue.count;
    new_planet.production = trace->turn_ai_queue.records;
    memset(&log_state, 0, sizeof log_state);
    memset(&log_ops, 0, sizeof log_ops);
    log_ops.write_memory = startup_capture_memory_log;
    stars_log_change_planet(&old_planet, &new_planet,
        trace->game_flags, &log_state, &log_ops, trace);
    if (trace->startup_memory_count != 7U ||
            trace->startup_memory_types[6] != 29U ||
            trace->startup_memory_sizes[6] != 6U ||
            memcmp(trace->startup_memory_records[6],
                "\x18\x00\x03\x1c\x02\x00", 6U) != 0)
        ++trace->failures;
}

static StarsI16 rototill_fill_production_queue(void *opaque)
{
    StarsRototillScoutAdapter *adapter = (StarsRototillScoutAdapter *)opaque;
    TurinTrace *trace = (TurinTrace *)adapter->context;
    StarsProductionQueueOps ops;
    const StarsU8 *planets[1];
    StarsI16 failures = trace->failures;
    trace->turn_committed_planet = 24;
    planets[0] = trace->final_queue_planet;
    memset(&ops, 0, sizeof ops);
    ops.change_selection = rototill_final_select;
    ops.initialize = rototill_final_initialize;
    ops.fill = rototill_final_fill;
    ops.finish = rototill_final_finish;
    stars_fill_production_queue(planets, 1U, &ops, trace);
    return trace->failures == failures;
}

typedef struct RototillProduceBoundary {
    TurinTrace *trace;
    StarsU16 built_factories;
} RototillProduceBoundary;

static StarsI32 rototill_produce_resources(void *opaque,
    const StarsProducePlanet *planet, StarsI16 owner)
{
    RototillProduceBoundary *boundary = (RototillProduceBoundary *)opaque;
    if (!planet || planet->id != 24 || owner != 1 ||
            get_u32(boundary->trace->final_queue_planet, 40U) != 225UL)
        ++boundary->trace->failures;
    return startup_final_resources(boundary->trace,
        boundary->trace->final_queue_planet, owner);
}

static StarsI16 rototill_produce_costs(void *opaque,
    const StarsProducePlanet *planet, const StarsProduceItem *item,
    StarsI32 costs[4])
{
    RototillProduceBoundary *boundary = (RototillProduceBoundary *)opaque;
    RototillQueueBoundary queue_boundary;
    StarsU32 packed;
    if (!planet || planet->id != 24 || !item || item->type != 7 ||
            item->object_kind != 1 || item->quantity < 1)
        ++boundary->trace->failures;
    memset(&queue_boundary, 0, sizeof queue_boundary);
    queue_boundary.trace = boundary->trace;
    packed = ((StarsU32)item->object_kind << 17) |
        ((StarsU32)item->type << 10) | (StarsU16)item->quantity;
    rototill_queue_cost(&queue_boundary, packed, costs);
    if (costs[0] != 0L || costs[1] != 0L || costs[2] != 3L ||
            costs[3] != 10L)
        ++boundary->trace->failures;
    return 1;
}

static StarsI16 rototill_produce_max_installation(void *opaque,
    const StarsBuildObjectPlanet *planet, StarsI16 kind)
{
    RototillProduceBoundary *boundary = (RototillProduceBoundary *)opaque;
    if (!planet || planet->id != 24 || kind != 1)
        ++boundary->trace->failures;
    return 22;
}

static StarsI16 rototill_produce_build_object(void *opaque,
    StarsProducePlanet *planet, StarsI16 group, StarsI16 kind,
    StarsI16 quantity, StarsI32 minerals[3])
{
    RototillProduceBoundary *boundary = (RototillProduceBoundary *)opaque;
    StarsBuildObjectPlanet object_planet;
    StarsBuildObjectInput input;
    StarsI16 result;
    (void)minerals;
    if (!planet || planet->id != 24 || group != 1 || kind != 7 ||
            quantity != 1)
        ++boundary->trace->failures;
    memset(&object_planet, 0, sizeof object_planet);
    memset(&input, 0, sizeof input);
    object_planet.id = planet->id;
    object_planet.player = planet->owner;
    object_planet.installations[1] = planet->factory_count;
    input.planet = &object_planet;
    input.object_group = group;
    input.item = kind;
    input.quantity = quantity;
    input.ops.max_installation = rototill_produce_max_installation;
    input.ops.context = boundary;
    result = stars_build_object(&input);
    planet->factory_count = object_planet.installations[1];
    if (!result)
        ++boundary->trace->failures;
    ++boundary->built_factories;
    return result;
}

static StarsI16 rototill_produce_build_item(void *opaque,
    StarsProducePlanet *planet, StarsProduceItem *item,
    StarsProduceItem *partial, StarsI32 resources[4], StarsI16 *status)
{
    RototillProduceBoundary *boundary = (RototillProduceBoundary *)opaque;
    StarsBuildProductionOps ops;
    StarsBuildProductionInput input;
    StarsI16 index, result;
    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    ops.costs = rototill_produce_costs;
    ops.build_object = rototill_produce_build_object;
    ops.context = boundary;
    input.planet = planet;
    input.item = item;
    input.partial = partial;
    input.status = status;
    input.ops = &ops;
    for (index = 0; index < 4; ++index)
        input.resources[index] = resources[index];
    result = stars_build_production_item(&input);
    for (index = 0; index < 4; ++index)
        resources[index] = input.resources[index];
    return result;
}

static void rototill_check_produce_boundary(TurinTrace *trace)
{
    RototillProduceBoundary boundary;
    StarsProducePlanet planet;
    StarsProduceItem item;
    StarsProduceOps ops;
    StarsProduceInput input;
    StarsU8 serialized[4];
    StarsU16 length = 0U;
    StarsU32 packed;
    StarsU16 lane;
    memset(&boundary, 0, sizeof boundary);
    memset(&planet, 0, sizeof planet);
    memset(&item, 0, sizeof item);
    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    boundary.trace = trace;
    packed = get_u32(trace->turn_ai_queue.records, 0U);
    if (trace->turn_ai_queue.count != 1U || packed != 0x00021c03UL ||
            startup_final_percent(trace, 1) != 0 ||
            trace->complete_input.views[2].players[1][56U] != 15U)
        ++trace->failures;
    planet.id = 24;
    planet.owner = 1;
    planet.research_percent = trace->complete_input.views[2].players[1][56U];
    planet.factory_count = 10;
    for (lane = 0U; lane < 3U; ++lane)
        planet.mineral[lane] = (StarsI32)trace->supply_planet.minerals[lane];
    item.type = (StarsI16)((packed >> 10) & 127UL);
    item.object_kind = (StarsI16)((packed >> 17) & 7UL);
    item.quantity = (StarsI16)(packed & 1023UL);
    item.active = 1;
    planet.items = &item;
    planet.item_count = 1U;
    planet.item_capacity = 1U;
    ops.resources_at_planet = rototill_produce_resources;
    ops.build_item_with_resources = rototill_produce_build_item;
    ops.context = &boundary;
    input.planets = &planet;
    input.planet_count = 1U;
    input.game_flags = 0x0080;
    input.ops = &ops;
    stars_produce(&input);
    if (boundary.built_factories != 2U || planet.factory_count != 12 ||
            planet.item_count != 1U || item.quantity != 1 ||
            item.progress != 89 || planet.last_year_resources != 4L ||
            !stars_write_production_queue_record(&item, 1U, serialized,
                sizeof serialized, &length, 0, 0) || length != 4U ||
            memcmp(serialized, "\x01\x1c\x92\x05", 4U) != 0) {
        ++trace->failures;
    }
    memcpy(rototill_produce_result, serialized,
        sizeof rototill_produce_result);
}

static void rototill_full_produce_mine(void *opaque)
{
    RototillProduceBoundary *boundary = (RototillProduceBoundary *)opaque;
    TurinTrace *trace = boundary->trace;
    StarsU16 owned;
    StarsI16 mineral;
    rototill_check_full_map_mining(trace);
    if (!trace->turn_produce_planets) { ++trace->failures; return; }
    for (owned = 0U; owned < 2U; ++owned) {
        StarsU16 id = trace->mining_runtime_ids[owned];
        for (mineral = 0; mineral < 3; ++mineral) {
            trace->turn_produce_planets[id].mineral[mineral] =
                trace->mining_stock[owned][mineral];
            trace->turn_planet_states[id].mineral_levels[mineral] =
                trace->mining_inputs[owned].percent_level[mineral];
            trace->turn_planet_states[id].mineral_concentrations[mineral] =
                trace->mining_inputs[owned].concentration[mineral];
        }
    }
}

static StarsI32 rototill_full_produce_resources(void *opaque,
    const StarsProducePlanet *planet, StarsI16 owner)
{
    RototillProduceBoundary *boundary = (RototillProduceBoundary *)opaque;
    return turn_produce_resources(boundary->trace, planet, owner);
}

static StarsI16 rototill_full_produce_extra(void *opaque, StarsI16 planet)
{
    RototillProduceBoundary *boundary = (RototillProduceBoundary *)opaque;
    return turn_produce_extra(boundary->trace, planet);
}

static void rototill_full_produce_message(void *opaque, StarsI16 player,
    StarsI16 code, StarsI16 planet, StarsI16 object, StarsI16 value)
{
    RototillProduceBoundary *boundary = (RototillProduceBoundary *)opaque;
    TurinTrace *trace = boundary->trace;
    StarsMessageOps ops;
    if (player != 0 || code != 0x3f || planet != 10 ||
            object != 10 || value != 0)
        ++trace->failures;
    memset(&ops, 0, sizeof ops);
    ops.parameter_count = turn_message_parameter_count;
    ops.player_flags = turn_message_player_flags;
    ops.append_message = turn_append_message;
    if (!stars_send_player_message2(&trace->turn_messages, player, code,
            planet, object, value, &ops, trace) ||
            trace->turn_messages.message_memory != trace->turn_message_bytes)
        ++trace->failures;
    ++trace->turn_produce_message_calls;
}

static void rototill_full_produce_population(void *opaque,
    StarsProducePlanet *planet)
{
    RototillProduceBoundary *boundary = (RototillProduceBoundary *)opaque;
    turn_produce_update_population(boundary->trace, planet);
}

static void rototill_full_produce_research(void *opaque,
    StarsI16 use_pool)
{
    RototillProduceBoundary *boundary = (RototillProduceBoundary *)opaque;
    TurinTrace *trace = boundary->trace;
    StarsU16 planet;
    if (trace->turn_production_commits != 1U ||
            trace->turn_production_planet_visits != 32U || use_pool != 1)
        ++trace->failures;
    ++trace->turn_research_calls;
    for (planet = 0U; planet < 32U; ++planet) {
        const StarsProducePlanet *produced = &trace->turn_produce_planets[planet];
        StarsU8 *player;
        if (produced->owner < 0) continue;
        player = trace->complete_input.views[0].players[produced->owner];
        put_u32(player, 58U, get_u32(player, 58U) +
            (StarsU32)produced->last_year_resources);
    }
    turn_update_current_research(trace, use_pool);
}

static StarsI16 rototill_event_random(void *opaque, StarsI16 limit)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsI16 value = stars_random(&trace->seeds, limit);
    ++trace->random_event_random_calls;
    if (trace->random_event_random_calls > 4)
        ++trace->failures;
    return value;
}

static void rototill_event_meteor(void *opaque)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsMeteorInput input;
    StarsMeteorPlanet planets[32];
    StarsU16 planet, axis;
    memset(&input, 0, sizeof input);
    memset(planets, 0, sizeof planets);
    record_random_event_callback(trace, 1);
    for (planet = 0U; planet < 32U; ++planet) {
        const StarsReadPlanetState *source = &trace->turn_planet_states[planet];
        planets[planet].id = source->id;
        planets[planet].owner = source->player;
        planets[planet].population = source->minerals[3];
        for (axis = 0U; axis < 3U; ++axis) {
            planets[planet].minerals[axis] = source->minerals[axis];
            planets[planet].concentration[axis] =
                source->mineral_concentrations[axis];
            planets[planet].environment[axis] =
                (signed char)source->environment[axis];
            planets[planet].original_environment[axis] =
                (signed char)source->environment_original[axis];
        }
    }
    input.turn = trace->random_event_turn;
    input.player_count = 2;
    input.planets = planets;
    input.planet_count = 32U;
    input.random = rototill_event_random;
    input.send_message = turn_event_meteor_message;
    input.toss_non_auto_build_items = turn_event_meteor_toss;
    input.context = trace;
    stars_meteor_strike(&input);
}

static void rototill_event_climate(void *opaque)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsPlanetaryClimateInput input;
    StarsClimatePlanet planets[32];
    StarsU16 planet, axis;
    memset(&input, 0, sizeof input);
    memset(planets, 0, sizeof planets);
    record_random_event_callback(trace, 2);
    for (planet = 0U; planet < 32U; ++planet) {
        const StarsReadPlanetState *source = &trace->turn_planet_states[planet];
        planets[planet].id = source->id;
        planets[planet].player = source->player;
        planets[planet].climate_metric = source->minerals[3];
        for (axis = 0U; axis < 3U; ++axis) {
            planets[planet].environment[axis] = source->environment[axis];
            planets[planet].original_environment[axis] =
                source->environment_original[axis];
        }
    }
    input.turn = trace->random_event_turn;
    input.planets = planets;
    input.planet_count = 32U;
    input.random = rototill_event_random;
    input.send_message = turn_event_climate_message;
    input.toss_build_items = turn_event_climate_toss;
    input.context = trace;
    stars_planetary_climate_change(&input);
}

static void rototill_event_minerals(void *opaque)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsDiscoverMineralsInput input;
    StarsDiscoverMineralPlanet planets[32];
    StarsU16 planet, axis;
    memset(&input, 0, sizeof input);
    memset(planets, 0, sizeof planets);
    record_random_event_callback(trace, 3);
    for (planet = 0U; planet < 32U; ++planet) {
        const StarsReadPlanetState *source = &trace->turn_planet_states[planet];
        planets[planet].id = (StarsU16)source->id;
        planets[planet].player = source->player;
        for (axis = 0U; axis < 3U; ++axis)
            planets[planet].concentration[axis] =
                source->mineral_concentrations[axis];
    }
    input.turn = trace->random_event_turn;
    input.planets = planets;
    input.planet_count = 32U;
    input.mineral_size =
        (StarsI16)get_u16(trace->complete_input.game, 4U);
    input.random = rototill_event_random;
    input.message = turn_event_mineral_message;
    input.context = trace;
    stars_discover_new_minerals(&input);
}

static void rototill_event_trader(void *opaque)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsMysteryTraderInput input;
    memset(&input, 0, sizeof input);
    record_random_event_callback(trace, 4);
    input.turn = trace->random_event_turn;
    input.player_count = 2;
    input.map_size = (StarsI16)get_u16(trace->complete_input.game, 4U);
    input.random = rototill_event_random;
    input.allocate_thing = turn_event_allocate;
    input.context = trace;
    stars_mystery_trader(&input);
}

static void rototill_full_random_events(void *opaque)
{
    RototillProduceBoundary *boundary = (RototillProduceBoundary *)opaque;
    TurinTrace *trace = boundary->trace;
    trace->random_event_turn =
        (StarsI16)get_u16(trace->complete_input.game, 18U);
    stars_random_events(rototill_event_meteor,
        rototill_event_climate, rototill_event_minerals,
        rototill_event_trader, trace);
    ++trace->turn_produce_random_event_calls;
}

static void rototill_check_full_map_produce(TurinTrace *trace)
{
    RototillProduceBoundary boundary;
    StarsProducePlanet planets[32];
    StarsProduceOps ops;
    StarsProduceInput input;
    StarsU16 planet;
    StarsI16 mineral;
    StarsU8 serialized[4];
    StarsU16 length = 0U;
    memset(&boundary, 0, sizeof boundary);
    memset(planets, 0, sizeof planets);
    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    boundary.trace = trace;
    for (planet = 0U; planet < 32U; ++planet) {
        const StarsReadPlanetState *loaded = &trace->turn_planet_states[planet];
        StarsI16 owner = loaded->player;
        planets[planet].id = loaded->id;
        planets[planet].owner = owner;
        for (mineral = 0; mineral < 3; ++mineral)
            planets[planet].mineral[mineral] =
                (StarsI32)loaded->minerals[mineral];
        if (owner < 0) continue;
        if (owner > 1) { ++trace->failures; return; }
        planets[planet].cheater = (StarsI16)((get_u16(
            trace->complete_input.views[0].players[owner], 84U) >> 2) & 1U);
        planets[planet].research_percent =
            trace->complete_input.views[0].players[owner][56U];
        planets[planet].research_exempt =
            (StarsI16)((loaded->opaque_20[1] >> 23) & 1UL);
        planets[planet].factory_count =
            (StarsI16)((loaded->opaque_20[0] >> 20) & 4095UL);
        planets[planet].mine_count =
            (StarsI16)((loaded->opaque_20[0] >> 8) & 4095UL);
        planets[planet].defense_count =
            (StarsI16)(loaded->opaque_20[1] & 4095UL);
    }
    planets[24].items = trace->turn_committed_queue;
    planets[24].item_count = trace->turn_committed_queue_count;
    planets[24].item_capacity = trace->turn_committed_queue_count;
    trace->turn_produce_planets = planets;
    ops.mine_minerals = rototill_full_produce_mine;
    ops.resources_at_planet = rototill_full_produce_resources;
    ops.planet_extra = rototill_full_produce_extra;
    ops.build_item_with_resources = rototill_produce_build_item;
    ops.send_message = rototill_full_produce_message;
    ops.update_populations = rototill_full_produce_population;
    ops.update_research = rototill_full_produce_research;
    ops.random_events = rototill_full_random_events;
    ops.context = &boundary;
    input.planets = planets;
    input.planet_count = 32U;
    input.game_flags = get_u16(trace->complete_input.game, 16U);
    input.ops = &ops;
    stars_produce(&input);
    ++trace->turn_produce_calls;
    trace->turn_planet_states[24].opaque_20[0] =
        (trace->turn_planet_states[24].opaque_20[0] & 0x000fffffUL) |
        ((StarsU32)planets[24].factory_count << 20);
    if (!stars_write_planet(&trace->turn_planet_states[24], 13U, 1U,
            rototill_planet_after_produce,
            sizeof rototill_planet_after_produce,
            &rototill_planet_after_produce_size, 0, 0))
        ++trace->failures;
    if (trace->turn_produce_message_calls != 1 ||
            trace->turn_produce_resource_calls != 2 ||
            trace->turn_produce_base_resources != 32L ||
            boundary.built_factories != 2U ||
            planets[24].factory_count != 12 ||
            planets[24].mineral[0] != 539L ||
            planets[24].mineral[1] != 965L ||
            planets[24].mineral[2] != 498L ||
            trace->turn_planet_states[24].minerals[3] != 256UL ||
            (trace->turn_planet_states[24].opaque_20[0] & 255UL) != 50UL ||
            planets[24].last_year_resources != 4L ||
            planets[24].item_count != 1U ||
            planets[24].items[0].quantity != 1 ||
            planets[24].items[0].progress != 89 ||
            !stars_write_production_queue_record(planets[24].items,
                planets[24].item_count, serialized, sizeof serialized,
                &length, 0, 0) || length != 4U ||
            memcmp(serialized, "\x01\x1c\x92\x05", 4U) != 0 ||
            trace->turn_production_commits != 1U ||
            trace->turn_production_planet_visits != 32U ||
            trace->turn_produce_population_calls != 2 ||
            trace->turn_research_calls != 1U ||
            trace->turn_produce_random_event_calls != 1U ||
            trace->random_event_callback_calls != 4 ||
            trace->random_event_random_calls != 4) {
        fprintf(stderr, "rot full produce messages=%d resource-calls=%d base=%ld built=%u factory=%d minerals=%ld,%ld,%ld last=%ld count=%u qty=%d progress=%d bytes=%02x%02x%02x%02x failures=%d\n",
            trace->turn_produce_message_calls,
            trace->turn_produce_resource_calls,
            (long)trace->turn_produce_base_resources,
            boundary.built_factories, planets[24].factory_count,
            (long)planets[24].mineral[0],
            (long)planets[24].mineral[1],
            (long)planets[24].mineral[2],
            (long)planets[24].last_year_resources,
            planets[24].item_count, planets[24].items[0].quantity,
            planets[24].items[0].progress,
            serialized[0], serialized[1], serialized[2], serialized[3],
            trace->failures);
        ++trace->failures;
    }
    trace->turn_produce_planets = (StarsProducePlanet *)0;
}

static void run_rototill_personality(void *opaque, StarsI16 player,
    void *planet_table)
{
    TurinTrace *trace;
    StarsRototillAiTurnInput input;
    StarsRototillAiTurnOps ops;
    StarsRototillTurnPlanet planet;
    StarsRototillTurnFleet fleets[TURIN_RETAINED_FLEET_COUNT];
    StarsRototillScoutAdapter scout;
    StarsI16 owners[TURIN_RETAINED_POINT_COUNT];
    StarsU8 status[TURIN_RETAINED_POINT_COUNT];
    StarsU8 raw_planet[56];
    StarsU8 fleet0_first_order[18];
    StarsU16 index;
    StarsU16 saved_queue_count;
    StarsI16 result;

    trace = (TurinTrace *)opaque;
    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    memset(&planet, 0, sizeof planet);
    memset(fleets, 0, sizeof fleets);
    memset(&scout, 0, sizeof scout);
    memset(status, 0, sizeof status);
    for (index = 0U; index < TURIN_RETAINED_POINT_COUNT; ++index)
        owners[index] = -2;
    owners[24] = 1;
    pack_loaded_planet(raw_planet,
        &trace->complete_input.views[2].planets[0]);
    planet.id = (StarsI16)get_u16(raw_planet, 0U);
    planet.raw_planet_record = raw_planet;
    planet.raw_planet_record_size = sizeof raw_planet;
    for (index = 0U; index < TURIN_RETAINED_FLEET_COUNT; ++index) {
        if (!trace->raw_fleet_present[index]) {
            ++trace->failures;
            return;
        }
        fleets[index].id = (StarsI16)index;
        fleets[index].raw_fleet_record = trace->raw_fleets[index];
        fleets[index].raw_fleet_record_size = 124U;
        fleets[index].raw_plord = trace->plord[index];
        fleets[index].raw_plord_bytes = sizeof trace->plord[index];
    }
    memcpy(fleet0_first_order, trace->plord[0] + 4U,
        sizeof fleet0_first_order);
    input.turn = (StarsI16)get_u16(trace->complete_input.game, 18U);
    input.player_id = player;
    input.planet_count = 1;
    input.planets = &planet;
    input.fleet_count = TURIN_RETAINED_FLEET_COUNT;
    input.fleets = fleets;
    input.ops = &ops;
    input.production_inputs_bound = 1;
    input.raw_ai_planet_table = (StarsU8 *)planet_table;
    input.raw_ai_planet_record_count = TURIN_RETAINED_POINT_COUNT;
    input.fleet_unowned_marker_bound = 1;
    input.fleet_scout_bound = 1;
    input.fleet_colonizer_supply_bound = 1;
    input.planet_points = trace->points;
    input.planet_point_count = TURIN_RETAINED_POINT_COUNT;
    input.ship0_first_hull_slot_byte = trace->designs[61U];
    scout.raw_ai_planet_table = (StarsU8 *)planet_table;
    scout.ai_planet_record_count = TURIN_RETAINED_POINT_COUNT;
    scout.planet_points = trace->points;
    scout.planet_count = TURIN_RETAINED_POINT_COUNT;
    scout.planet_owners = owners;
    scout.status_scratch = status;
    scout.status_scratch_count = TURIN_RETAINED_POINT_COUNT;
    scout.race_mode = 3;
    scout.turn = input.turn;
    scout.has_teeth = rototill_has_teeth;
    scout.random_value = rototill_random_value;
    scout.find_wormhole = rototill_find_wormhole;
    scout.ideal_warp = rototill_ideal_warp;
    scout.select_fleet = rototill_select_fleet;
    scout.refresh_fleet = rototill_refresh_fleet;
    scout.context = trace;
    ops.context = &scout;
    ops.ensure_ai = rototill_ensure_ai;
    ops.is_ai_transport = rototill_is_transport;
    ops.select_fleet = rototill_ops_select_fleet;
    ops.refresh_fleet = rototill_ops_refresh_fleet;
    ops.target_scout = stars_rototill_target_scout;
    ops.nearest_colonizable = rototill_nearest_colonizable;
    ops.transfer_supply = rototill_transfer_supply;
    ops.run_basic_tasks = rototill_run_basic_tasks;
    ops.fill_production_queue = rototill_fill_production_queue;
    result = stars_do_rototill_ai_turn(&input);
    rototill_check_produce_boundary(trace);
    saved_queue_count = trace->turn_ai_queue.count;
    trace->turn_ai_queue.count = 0U;
    rototill_check_queue_boundary(trace, 225UL, 0, 32L, 22, 3U, 0U);
    rototill_check_queue_boundary(trace, 250UL, 0, 35L, 25, 3U, 1U);
    rototill_check_queue_boundary(trace, 225UL, 1, 32L, 22, 3U, 0U);
    trace->turn_ai_queue.count = saved_queue_count;
    if (player != 1 || !trace->ai_input.ai_active ||
            trace->session.player != 1 ||
            planet.id != 24 || get_u32(raw_planet, 40U) != 250UL ||
            !result ||
            input.production_items_added != 0 ||
            ((StarsU8 *)planet_table)[24U * 16U + 9U] != 1U ||
            trace->plord[0][3] != 2U ||
            memcmp(trace->plord[0] + 4U, fleet0_first_order,
                sizeof fleet0_first_order) != 0 ||
            get_u16(trace->plord[0], 22U) !=
                (StarsU16)retained_points[26].x ||
            get_u16(trace->plord[0], 24U) !=
                (StarsU16)retained_points[26].y ||
            get_u16(trace->plord[0], 26U) != 26U ||
            get_u16(trace->plord[0], 28U) != 0x1150U ||
            ((StarsU8 *)planet_table)[26U * 16U + 15U] != 4U ||
            trace->raw_fleets[0][98U] != 2U ||
            trace->order_warp_calls != 1U ||
            trace->supply_fleet.minerals[3] != 25UL ||
            trace->supply_planet.minerals[3] != 225UL ||
            trace->random_calls != 1 || trace->wormhole_calls != 0U)
        ++trace->failures;
    ++trace->personality_calls;
}

static void rototill_write_log(void *opaque, StarsI16 player)
{
    TurinTrace *trace;
    trace = (TurinTrace *)opaque;
    if (player != 1 || trace->session.player != player ||
            !trace->ai_input.ai_active)
        ++trace->failures;
    if (!save_generated_mutations(trace) ||
            trace->saved_mutation_records != 7U ||
            !rototill_write_generated_log(trace))
        ++trace->failures;
    ++trace->log_calls;
    trace->runtime_log_present[player] = 1U;
}

static void rototill_write_history(void *opaque, StarsI16 player)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    write_history(opaque, player);
    if (player == 1 && !trace->failures)
        rototill_write_generated_history(trace);
}

static StarsI16 rototill_turn_load_game(void *opaque,
    StarsGenerateTurnState *state)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    if (trace->personality_calls != 1 || trace->failures) return 0;
    rototill_reload_host_input(trace);
    state->player_count = (StarsI16)get_u16(trace->complete_input.game, 8U);
    state->turn = (StarsI16)get_u16(trace->complete_input.bof, 10U);
    return (StarsI16)(!trace->failures && state->player_count == 2 &&
        state->turn == 0);
}

static void rototill_turn_begin(void *opaque,
    StarsGenerateTurnState *state)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    if (!state->loaded || trace->turn_generation_flag_entries) {
        ++trace->failures; return;
    }
    trace->turn_generation_prior_flags = trace->game_flags;
    trace->game_flags = (StarsU16)((trace->game_flags & 0xfdfdU) | 0x0202U);
    ++trace->turn_generation_flag_entries;
}

static void rototill_turn_end(void *opaque,
    StarsGenerateTurnState *state)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    if (!state->output_written || trace->turn_generation_flag_entries != 1U ||
            trace->turn_generation_flag_exits ||
            !rototill_output_file_sizes[0] || !rototill_output_file_sizes[1] ||
            !rototill_output_file_sizes[2]) {
        ++trace->failures; return;
    }
    trace->game_flags = (StarsU16)(trace->game_flags & 0xfdfdU);
    ++trace->turn_generation_flag_exits;
}

static StarsI16 rototill_turn_random(void *opaque, StarsI16 limit)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    return stars_random(&trace->seeds, limit);
}

static StarsI16 rototill_turn_load_log(void *opaque,
    StarsGenerateTurnState *state, StarsI16 player)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    if (state->logs_loaded || player < 0 || player > 1)
        ++trace->failures;
    if (player == 1 && !trace->failures)
        rototill_check_research_reload(trace);
    ++trace->turn_load_log_calls;
    return (StarsI16)!trace->failures;
}

static void rototill_turn_validate_logs(void *opaque,
    StarsGenerateTurnState *state)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    if (!state->logs_loaded || state->player_order_count != 2U ||
            trace->turn_load_log_calls != 2U ||
            trace->generated_log_loads != 2U)
        ++trace->failures;
}

static void rototill_turn_move_fleets(void *opaque,
    StarsGenerateTurnState *state)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    (void)state;
    if (!trace->failures) rototill_check_fleet_movement(trace);
}

static void rototill_turn_produce(void *opaque,
    StarsGenerateTurnState *state)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    (void)state;
    if (!trace->failures) rototill_write_moved_fleets(trace);
    if (!trace->failures) rototill_write_host_fleets(trace);
    if (!trace->failures) rototill_write_output_designs(trace);
    if (!trace->failures) rototill_check_full_map_produce(trace);
}

static void rototill_turn_move_things(void *opaque,
    StarsGenerateTurnState *state, StarsI16 mode)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    if (mode == 0) turn_move_things(trace, state, 0);
    else if (mode == 1 && !trace->failures)
        rototill_check_wormhole_movement(trace);
    else ++trace->failures;
}

static void rototill_turn_update_guesses(void *opaque,
    StarsGenerateTurnState *state)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    (void)state;
    memcpy(rototill_hst_thing_bodies, trace->turn_thing_bytes,
        sizeof rototill_hst_thing_bodies);
    if (!trace->failures) rototill_check_update_guesses(trace);
}

static void rototill_turn_mark_backup(void *opaque,
    StarsGenerateTurnState *state)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsMarkFileInput input;
    StarsMarkFileState marked;
    StarsMarkFileOps ops;
    StarsU16 file;
    if (state->turn != 0U || trace->turn_mark_backup_calls != 0U) {
        ++trace->failures; return;
    }
    for (file = 0U; file < 3U; ++file) {
        trace->active_file_sizes[file] = rototill_input_file_sizes[file];
        memcpy(trace->active_files[file], rototill_input_file_bytes[file],
            trace->active_file_sizes[file]);
    }
    memset(&input, 0, sizeof input);
    memset(&marked, 0, sizeof marked);
    memset(&ops, 0, sizeof ops);
    input.type = 2U; input.player = -1; input.mark_mode = 1U;
    input.game_lid = get_u32(trace->complete_input.game, 0U);
    input.header_type = (StarsU16)(get_u16(trace->active_files[0], 0U) >> 10);
    input.header_version = get_u16(trace->active_files[0], 10U);
    input.header_game_lid = get_u32(trace->active_files[0], 6U);
    input.header_flags = get_u16(trace->active_files[0], 16U);
    ops.open_file = turn_host_mark_open;
    ops.write_record = turn_host_mark_write;
    if (!stars_mark_file(&input, &marked, &ops, trace) ||
            trace->active_file_sizes[0] != rototill_backup_file_sizes[0] ||
            memcmp(trace->active_files[0], rototill_backup_file_bytes[0],
                trace->active_file_sizes[0]) != 0)
        ++trace->failures;
    ++trace->turn_mark_backup_calls;
}

static void rototill_turn_prepare_output(void *opaque,
    StarsGenerateTurnState *state)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsU16 index;
    if (state->turn != 1U || trace->turn_score_updates != 2U ||
            trace->turn_mark_backup_calls != 1U)
        ++trace->failures;
    state->player_file_append[0] = 1U;
    state->player_file_append[1] = 0U;
    for (index = 0U; index < 3U; ++index) {
        StarsU16 size = trace->active_file_sizes[index];
        if (size != rototill_backup_file_sizes[index]) {
            ++trace->failures; continue;
        }
        memcpy(trace->backup_files[index], trace->active_files[index], size);
        trace->backup_file_sizes[index] = size;
        if (memcmp(trace->backup_files[index],
                rototill_backup_file_bytes[index], size) != 0)
            ++trace->failures;
        if (index != 1U) {
            trace->active_file_sizes[index] = 0U;
            ++trace->backup_moves;
        } else ++trace->backup_copies;
    }
    if (trace->runtime_log_present[1] && trace->generated_log_size) {
        trace->backup_log_size = trace->generated_log_size;
        memcpy(trace->backup_log, trace->generated_log_file,
            trace->backup_log_size);
        trace->generated_log_size = 0U;
        trace->runtime_log_present[1] = 0U;
        ++trace->backup_moves;
    } else ++trace->failures;
    trace->saved_flag = stars_random(&trace->seeds, 8);
    if (trace->saved_flag != 6) ++trace->failures;
    put_u16(trace->rng_file_data, 0x0080U,
        (StarsU16)((StarsU16)trace->saved_flag << 9));
    put_u16(trace->complete_input.game, 16U,
        (StarsU16)((get_u16(trace->complete_input.game, 16U) & 0xf1ffU) |
        ((StarsU16)trace->saved_flag << 9)));
    if (!trace->failures) turn_prepare_visibility_init(trace, 1);
    if (!trace->failures) rototill_write_player(trace, 1, 1,
        rototill_m2_player_body, &rototill_m2_player_size);
    if (!trace->failures) turn_prepare_visibility_init(trace, 0);
    if (!trace->failures) rototill_write_player(trace, 0, 0,
        rototill_m1_player_body, &rototill_m1_player_size);
    if (!trace->failures) turn_prepare_visibility_init(trace, -1);
    if (!trace->failures)
        for (index = 0U; index < 2U; ++index)
            rototill_write_player(trace, -1, (StarsI16)index,
                rototill_hst_player_bodies[index],
                &rototill_hst_player_sizes[index]);
    if (!trace->failures) rototill_write_all_designs(trace);
    if (!trace->failures) rototill_write_all_plans(trace);
    if (!trace->failures) rototill_write_client_planets(trace);
    if (!trace->failures) rototill_write_m1_messages(trace);
    if (!trace->failures) rototill_write_hst_things(trace);
    if (!trace->failures) rototill_preview_output_rng_tail(trace);
}

static void rototill_turn_write_data(void *opaque,
    StarsGenerateTurnState *state, StarsI16 player, StarsI16 append)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsU16 file = player < 0 ? 2U : (StarsU16)player;
    (void)state;
    if (file >= 3U || append != (file == 0U)) {
        ++trace->failures; return;
    }
    if (!trace->failures) rototill_write_complete_file(trace, file);
}

static void rototill_run_generate_turn(TurinTrace *trace)
{
    StarsGenerateTurnState state;
    StarsGenerateTurnOps ops;
    memset(&state, 0, sizeof state);
    memset(&ops, 0, sizeof ops);
    state.player_count = 2;
    ops.context = trace;
    ops.destroy_game = startup_host_destroy;
    ops.load_game = rototill_turn_load_game;
    ops.initialize_turn = rototill_turn_begin;
    ops.cleanup = rototill_turn_end;
    ops.random_value = rototill_turn_random;
    ops.load_player_log = rototill_turn_load_log;
    ops.validate_player_logs = rototill_turn_validate_logs;
    ops.validate_waypoints = turn_validate_waypoints;
    ops.do_orders = turn_do_orders;
    ops.unmark_mine_fields = turn_unmark_mine_fields;
    ops.move_things = rototill_turn_move_things;
    ops.move_fleets = rototill_turn_move_fleets;
    ops.thing_decay = turn_thing_decay;
    ops.breed_colonists = turn_breed_colonists;
    ops.produce = rototill_turn_produce;
    ops.fuel_fleets = turn_fuel_fleets;
    ops.sweep_for_mines = turn_sweep_for_mines;
    ops.heal_ships = turn_heal_ships;
    ops.auto_terraform = turn_auto_terraform;
    ops.remote_terraforming = turn_remote_terraform;
    ops.spank_cheaters = turn_spank_cheaters;
    ops.update_guesses = rototill_turn_update_guesses;
    ops.mark_backup = rototill_turn_mark_backup;
    ops.update_scores = turn_update_scores;
    ops.prepare_output_files = rototill_turn_prepare_output;
    ops.write_data_file = rototill_turn_write_data;
    ++trace->startup_turn_shuffle_calls;
    if (!stars_generate_turn(&state, &ops) || !state.loaded ||
            !state.logs_loaded || !state.generated || !state.output_written ||
            state.turn != 1U || trace->turn_validate_calls != 2U ||
            trace->turn_generation_flag_entries != 1U ||
            trace->turn_generation_flag_exits != 1U ||
            trace->turn_waypoint_source_calls != 3U ||
            trace->turn_unmark_mine_calls != 1U ||
            trace->turn_pre_move_things_calls != 1U ||
            trace->turn_thing_decay_calls != 1U ||
            trace->turn_breed_colonists_calls != 1U ||
            trace->turn_fuel_calls != 1U ||
            trace->turn_sweep_for_mines_calls != 1U ||
            trace->turn_heal_calls != 1U ||
            trace->turn_auto_terraform_calls != 1U ||
            trace->turn_remote_terraform_calls != 1U ||
            trace->turn_spank_cheaters_calls != 1U)
        ++trace->failures;
}

int main(int argc, char **argv)
{
    TurinTrace trace;
    StarsU16 saved_flags;
    StarsU16 index;
    static const StarsU16 design_slots[4] = {18U, 20U, 17U, 19U};
    memset(&trace, 0, sizeof trace);
    rototill_traced_seeds = &trace.seeds;
    trace.complete_input = soe3_inputs[0];
    trace.complete_input_loaded = 1;
    /* The original variant audit pins these two decoded player bytes. */
    trace.complete_input.views[0].players[1][7] = 0x67U;
    trace.complete_input.views[2].players[1][7] = 0x67U;
    stars_randomize2(&trace.seeds, 6844UL);
    trace.rototill_seeded_mode = 1;
    trace.scout.context = &trace;
    memset(&trace.ai_input, 0, sizeof trace.ai_input);
    trace.ai_input.players = trace.players;
    trace.ai_input.player_count = 2;
    trace.ai_input.planet_capacity = TURIN_RETAINED_POINT_COUNT;
    trace.ai_input.load_game = load_game;
    trace.ai_input.destroy_game = startup_destroy_game;
    trace.session.player = -1;
    trace.ai_input.current_player = &trace.session.player;
    trace.ai_input.allocate = allocate;
    trace.ai_input.free_memory = free_memory;
    trace.ai_input.compute_design_powers = startup_compute_powers;
    trace.ai_input.mark_planets_under_attack = startup_mark_attacks;
    trace.ai_input.increase_minefield_sizes = startup_increase_minefields;
    trace.ai_input.init_random_planet_list = startup_init_planet_list;
    trace.ai_input.personalities[3] = run_rototill_personality;
    trace.ai_input.write_log = rototill_write_log;
    trace.ai_input.write_history = rototill_write_history;
    trace.ai_input.context = &trace;
    saved_flags = (StarsU16)(trace.game_flags & 0x0010U);
    trace.game_flags |= 0x0010U;
    stars_do_ai_turn(1, 0xffffU, &trace.ai_input);
    trace.game_flags = (StarsU16)((trace.game_flags & 0xffefU) | saved_flags);
    if (trace.personality_calls != 1 || trace.log_calls != 1U ||
            trace.history_calls != 1U ||
            trace.startup_ensure_calls != 1U ||
            trace.startup_research_order != 0x39e ||
            trace.startup_research_writes != 0U ||
            trace.complete_input.views[0].players[1][56U] != 0U ||
            trace.complete_input.views[2].players[1][56U] != 15U ||
            (get_u16(trace.complete_input.views[0].players[1], 4U) >> 12) != 5U ||
            trace.startup_name_count != 4 ||
            trace.startup_name_random_calls != 4 ||
            trace.startup_name_ids[0] != 0x43eU ||
            trace.startup_name_ids[1] != 0x438U ||
            trace.startup_name_ids[2] != 0x43eU ||
            trace.startup_name_ids[3] != 0x434U ||
            trace.startup_memory_count != 7U ||
            trace.startup_memory_types[4] != 4U ||
            trace.startup_memory_types[5] != 1U ||
            trace.ai_allocation_calls != 3U ||
            trace.ai_release_calls != 3U || trace.ai_live_allocations ||
            trace.session.player != -1 || trace.ai_destroy_calls != 1U ||
            trace.ai_destroy_heaps != 9U || trace.ai_reset_messages != 1U ||
            get_u16(trace.input_players[3], 6U) != 0x6797U)
        ++trace.failures;
    if (!trace.failures) rototill_run_generate_turn(&trace);
    if (argc == 2 && strcmp(argv[1], "--dump-designs") == 0 &&
            trace.failures == 0) {
        for (index = 0U; index < 4U; ++index) {
            StarsU16 size;
            StarsU16 byte;
            const StarsU8 *raw;
            if (trace.startup_memory_types[index] != 27U ||
                    trace.startup_memory_sizes[index] < 3U)
                return 3;
            raw = trace.startup_memory_records[index] + 2U;
            size = (StarsU16)(trace.startup_memory_sizes[index] - 2U);
            printf("ROT_DESIGN %u ", design_slots[index]);
            for (byte = 0U; byte < size; ++byte)
                printf("%02x", raw[byte]);
            putchar('\n');
        }
        for (index = 0U; index < trace.startup_memory_count; ++index) {
            StarsU16 byte;
            printf("ROT_MUT %u ", trace.startup_memory_types[index]);
            for (byte = 0U; byte < trace.startup_memory_sizes[index]; ++byte)
                printf("%02x", trace.startup_memory_records[index][byte]);
            putchar('\n');
        }
        printf("ROT_PRODUCE ");
        for (index = 0U; index < sizeof rototill_produce_result; ++index)
            printf("%02x", rototill_produce_result[index]);
        putchar('\n');
        printf("ROT_LOG ");
        for (index = 0U; index < trace.backup_log_size; ++index)
            printf("%02x", trace.backup_log[index]);
        putchar('\n');
        printf("ROT_HIST ");
        for (index = 0U; index < trace.generated_history_size; ++index)
            printf("%02x", trace.generated_history_file[index]);
        putchar('\n');
        printf("ROT_RESEARCH ");
        for (index = 26U; index < 62U; ++index)
            printf("%02x", trace.complete_input.views[0].players[1][index]);
        putchar('\n');
        printf("ROT_PLANET ");
        for (index = 0U; index < rototill_planet_after_produce_size; ++index)
            printf("%02x", rototill_planet_after_produce[index]);
        putchar('\n');
        printf("ROT_PLANET10 ");
        for (index = 0U; index < rototill_planet10_after_produce_size; ++index)
            printf("%02x", rototill_planet10_after_produce[index]);
        putchar('\n');
        printf("ROT_THINGS ");
        for (index = 0U; index < TURIN_RETAINED_THING_COUNT * 18U; ++index)
            printf("%02x", rototill_hst_thing_bodies[index]);
        putchar('\n');
        for (index = 0U; index < TURIN_RETAINED_FLEET_COUNT; ++index) {
            StarsU16 byte;
            printf("ROT_FLEET %u ", index);
            for (byte = 0U; byte < rototill_fleet_body_sizes[index]; ++byte)
                printf("%02x", rototill_fleet_bodies[index][byte]);
            putchar(' ');
            for (byte = 0U; byte < rototill_order_body_sizes[index]; ++byte)
                printf("%02x", rototill_order_bodies[index][byte]);
            putchar('\n');
        }
        for (index = 0U; index < TURIN_HST_OWNER0_INPUT_FLEET_COUNT; ++index) {
            StarsU16 byte;
            printf("ROT_HFLEET %u ", index);
            for (byte = 0U; byte < rototill_host_fleet_body_sizes[index]; ++byte)
                printf("%02x", rototill_host_fleet_bodies[index][byte]);
            putchar(' ');
            for (byte = 0U; byte < rototill_host_order_body_sizes[index]; ++byte)
                printf("%02x", rototill_host_order_bodies[index][byte]);
            putchar('\n');
        }
        for (index = 0U; index < TURIN_RETAINED_POINT_COUNT; ++index) {
            StarsU16 byte;
            printf("ROT_HPLANET %u ", index);
            for (byte = 0U; byte < rototill_hst_planet_sizes[index]; ++byte)
                printf("%02x", rototill_hst_planet_bodies[index][byte]);
            putchar('\n');
        }
        printf("ROT_RNGTAIL %d %d %d %d\n",
            rototill_output_tail_draws[0], rototill_output_tail_draws[1],
            rototill_output_tail_draws[2], rototill_output_tail_draws[3]);
        for (index = 0U; index < 3U; ++index) {
            StarsU16 byte;
            printf("ROT_BOF %u ", index);
            for (byte = 0U; byte < 18U; ++byte)
                printf("%02x", rototill_output_bofs[index][byte]);
            putchar('\n');
        }
        for (index = 0U; index < 4U; ++index) {
            StarsU16 byte;
            printf("ROT_ODSGN %u ", design_slots[index]);
            for (byte = 0U; byte < rototill_output_design_sizes[index]; ++byte)
                printf("%02x", rototill_output_design_bodies[index][byte]);
            putchar('\n');
        }
        printf("ROT_M2PLAYER ");
        for (index = 0U; index < rototill_m2_player_size; ++index)
            printf("%02x", rototill_m2_player_body[index]);
        putchar('\n');
        printf("ROT_M2SCORE ");
        for (index = 0U; index < sizeof trace.turn_score_records[1]; ++index)
            printf("%02x", trace.turn_score_records[1][index]);
        putchar('\n');
        printf("ROT_M1SCORE ");
        for (index = 0U; index < sizeof trace.turn_score_records[0]; ++index)
            printf("%02x", trace.turn_score_records[0][index]);
        putchar('\n');
        printf("ROT_M1PLAYER ");
        for (index = 0U; index < rototill_m1_player_size; ++index)
            printf("%02x", rototill_m1_player_body[index]);
        putchar('\n');
        for (index = 0U; index < 2U; ++index) {
            StarsU16 byte;
            printf("ROT_HPLAYER %u ", index);
            for (byte = 0U; byte < rototill_hst_player_sizes[index]; ++byte)
                printf("%02x", rototill_hst_player_bodies[index][byte]);
            putchar('\n');
        }
        for (index = 0U; index < rototill_all_design_count; ++index) {
            StarsU16 byte;
            printf("ROT_DSGN %u ", rototill_all_design_files[index]);
            for (byte = 0U; byte < rototill_all_design_sizes[index]; ++byte)
                printf("%02x", rototill_all_design_bodies[index][byte]);
            putchar('\n');
        }
        for (index = 0U; index < rototill_plan_count; ++index) {
            StarsU16 byte;
            printf("ROT_PLAN %u ", rototill_plan_files[index]);
            for (byte = 0U; byte < rototill_plan_sizes[index]; ++byte)
                printf("%02x", rototill_plan_bodies[index][byte]);
            putchar('\n');
        }
        for (index = 0U; index < rototill_client_planet_count; ++index) {
            StarsU16 byte;
            printf("ROT_CPLANET %u %u ", rototill_client_planet_files[index],
                rototill_client_planet_types[index]);
            for (byte = 0U; byte < rototill_client_planet_sizes[index]; ++byte)
                printf("%02x", rototill_client_planet_bodies[index][byte]);
            putchar('\n');
        }
        printf("ROT_M1MESSAGE ");
        for (index = 0U; index < rototill_m1_message_size; ++index)
            printf("%02x", rototill_m1_message_body[index]);
        putchar('\n');
        for (index = 0U; index < rototill_thing_record_count; ++index) {
            StarsU16 byte;
            printf("ROT_THING ");
            for (byte = 0U; byte < rototill_thing_sizes[index]; ++byte)
                printf("%02x", rototill_thing_records[index][byte]);
            putchar('\n');
        }
        for (index = 0U; index < 3U; ++index) {
            StarsU16 byte;
            printf("ROT_FILE %u ", index);
            for (byte = 0U; byte < rototill_output_file_sizes[index]; ++byte)
                printf("%02x", rototill_output_files[index][byte]);
            putchar('\n');
        }
    }
    if (argc == 2 && strcmp(argv[1], "--dump-rng") == 0 &&
            trace.failures == 0) {
        for (index = 0U; index < rototill_rng_draw_count; ++index)
            printf("ROT_RNG %u %d %d\n", index + 1U,
                rototill_rng_draws[index].limit,
                rototill_rng_draws[index].value);
    }
    printf("ROTOTILL_COORDINATOR_FAILURES=%d RESULT=%s\n",
           trace.failures, trace.failures ? "FAIL" : "PASS");
    return trace.failures ? 1 : 0;
}
