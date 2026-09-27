#include <stdio.h>
#include <string.h>
#include "core.h"

#define TURN_FLEETS 2
#define TURN_TABLE_SLOTS 3
#define TURN_ORDER_COUNT 12
#define TURN_EVENT_MOVE 10
#define TURN_EVENT_KILL 11
#define TURN_EVENT_AFTER_MOVE 12

typedef struct TurnState {
    StarsU8 fleet_table[TURN_TABLE_SLOTS * 4];
    StarsU8 fleet_records[TURN_FLEETS][124];
    StarsU8 plord_records[TURN_FLEETS][220];
    StarsKillUsedWaypointFleet kill_views[TURN_FLEETS];
    StarsKillUsedWaypointOrder kill_orders[TURN_FLEETS * TURN_ORDER_COUNT];
    StarsMoveFleet move_fleets[1];
    StarsMoveFleetOrder move_orders[1];
    StarsKillUsedWaypointsPackedInput packed;
    StarsKillUsedWaypointOps kill_ops;
    StarsMoveFleetsOps move_ops;
    StarsMoveFleetsInput move_input;
    StarsI16 delete_calls;
    StarsI16 message_calls;
    StarsI16 kill_calls;
    StarsI16 move_order_calls;
    StarsI16 captured_target;
    StarsI16 captured_x;
    StarsI16 captured_y;
    StarsI16 events[8];
    StarsI16 event_count;
    StarsI16 turn_random_calls;
} TurnState;

static StarsI16 get_order(void *, const StarsMoveFleet *,
    StarsMoveFleetOrder *);
static StarsI32 distance_value(void *, StarsI16, StarsI16, StarsI16,
    StarsI16);
static StarsI32 fuel_use(void *, const StarsMoveFleet *,
    const StarsMoveFleetOrder *, StarsI32);
static void delete_waypoint(void *, StarsKillUsedWaypointFleet *, StarsI16,
    StarsI16);
static void send_message_ex(void *, StarsI16, StarsI16, StarsU16, StarsI16);
static void kill_waypoints(void *);

static void put16(StarsU8 *raw, StarsU16 offset, StarsU16 value)
{
    raw[offset] = (StarsU8)value;
    raw[offset + 1U] = (StarsU8)(value >> 8);
}

static StarsU16 get16(const StarsU8 *raw, StarsU16 offset)
{
    return (StarsU16)(raw[offset] |
        ((StarsU16)raw[offset + 1U] << 8));
}

static void event(TurnState *state, StarsI16 value)
{
    if (state->event_count < 8)
        state->events[state->event_count++] = value;
}

static void put_table_ref(StarsU8 *table, StarsI16 slot, StarsI16 record)
{
    StarsU16 offset = (StarsU16)(slot * 4);
    if (record < 0) {
        put16(table, offset, 0U);
        put16(table, (StarsU16)(offset + 2U), 0U);
    } else {
        put16(table, offset, (StarsU16)(record + 1));
        put16(table, (StarsU16)(offset + 2U), 1U);
    }
}

static void put_order(StarsU8 *raw, StarsI16 index, StarsI16 x,
    StarsI16 y, StarsI16 target, StarsU16 flags)
{
    StarsU16 base = (StarsU16)(4U + (StarsU16)index * 18U);
    put16(raw, base, (StarsU16)x);
    put16(raw, (StarsU16)(base + 2U), (StarsU16)y);
    put16(raw, (StarsU16)(base + 4U), (StarsU16)target);
    put16(raw, (StarsU16)(base + 6U), flags);
}

static void seed(TurnState *state)
{
    StarsI16 index;

    memset(state, 0, sizeof *state);
    for (index = 0; index < TURN_FLEETS; ++index) {
        memset(state->fleet_records[index], 0, 124U);
        memset(state->plord_records[index], 0, 220U);
        put16(state->fleet_records[index], 0U, (StarsU16)(index + 1));
        put16(state->fleet_records[index], 2U, 3U);
        put16(state->fleet_records[index], 98U,
            (StarsU16)(index == 0 ? 4 : 1));
        put16(state->fleet_records[index], 100U, (StarsU16)(index + 1));
    }
    put16(state->fleet_records[1], 8U, 33U);
    put16(state->fleet_records[1], 10U, 44U);
    put_order(state->plord_records[0], 1, 1, 2, 2, 0x0200U);
    put_order(state->plord_records[0], 2, 99, 99, 0, 0U);
    put_table_ref(state->fleet_table, 0, 0);
    put_table_ref(state->fleet_table, 1, 1);
    put_table_ref(state->fleet_table, 2, -1);
    state->packed.fleet_table = state->fleet_table;
    state->packed.fleet_table_count = TURN_TABLE_SLOTS;
    state->packed.fleet_records = &state->fleet_records[0][0];
    state->packed.fleet_record_count = TURN_FLEETS;
    state->packed.plord_records = &state->plord_records[0][0];
    state->packed.plord_record_count = TURN_FLEETS;
    state->packed.fleet_views = state->kill_views;
    state->packed.order_views = state->kill_orders;
    state->packed.workspace_count = TURN_FLEETS;
    state->packed.ops = &state->kill_ops;
    state->move_fleets[0].id = 1;
    state->move_fleets[0].owner = 3;
    state->move_fleets[0].active = 1;
    state->move_fleets[0].x = 0;
    state->move_fleets[0].y = 0;
    state->move_fleets[0].fuel = 100;
    state->move_fleets[0].fuel_capacity = 100;
    state->move_orders[0].kind = 2;
    state->move_orders[0].target_kind = 4;
    state->move_orders[0].target_x = 10;
    state->move_orders[0].target_y = 20;
    state->move_ops.context = state;
    state->move_input.fleets = state->move_fleets;
    state->move_input.fleet_count = 1;
    state->move_input.ops = &state->move_ops;
}

static StarsI16 get_order(void *context, const StarsMoveFleet *fleet,
    StarsMoveFleetOrder *order)
{
    TurnState *state = (TurnState *)context;
    ++state->move_order_calls;
    if (fleet->id != 1)
        return 0;
    *order = state->move_orders[0];
    return 1;
}

static StarsI32 distance_value(void *context, StarsI16 x1, StarsI16 y1,
    StarsI16 x2, StarsI16 y2)
{
    (void)context;
    return (StarsI32)((x2 > x1 ? x2 - x1 : x1 - x2) +
        (y2 > y1 ? y2 - y1 : y1 - y2));
}

static StarsI32 fuel_use(void *context, const StarsMoveFleet *fleet,
    const StarsMoveFleetOrder *order, StarsI32 distance)
{
    (void)context; (void)fleet; (void)order;
    return distance;
}

static void delete_waypoint(void *context, StarsKillUsedWaypointFleet *fleet,
    StarsI16 index, StarsI16 recycle)
{
    TurnState *state = (TurnState *)context;
    (void)index; (void)recycle;
    ++state->delete_calls;
    state->captured_target = fleet->orders[1].target_id;
    state->captured_x = fleet->orders[1].x;
    state->captured_y = fleet->orders[1].y;
    if (fleet->cord > 1) {
        (void)memmove(&fleet->orders[1], &fleet->orders[2],
            (size_t)(fleet->cord - 2) * sizeof(fleet->orders[0]));
        --fleet->cord;
    }
}

static void send_message_ex(void *context, StarsI16 owner, StarsI16 message,
    StarsU16 status_word, StarsI16 fleet_id)
{
    TurnState *state = (TurnState *)context;
    (void)owner; (void)message; (void)status_word; (void)fleet_id;
    ++state->message_calls;
}

static void kill_waypoints(void *context)
{
    TurnState *state = (TurnState *)context;
    ++state->kill_calls;
    event(state, TURN_EVENT_KILL);
    put16(state->fleet_records[0], 8U, (StarsU16)state->move_fleets[0].x);
    put16(state->fleet_records[0], 10U, (StarsU16)state->move_fleets[0].y);
    stars_kill_used_waypoints_packed(&state->packed);
}

static StarsI16 load_game(void *context, StarsGenerateTurnState *state)
{
    TurnState *turn = (TurnState *)context;
    seed(turn);
    turn->kill_ops.delete_waypoint = delete_waypoint;
    turn->kill_ops.send_message_ex = send_message_ex;
    turn->kill_ops.context = turn;
    turn->packed.ops = &turn->kill_ops;
    turn->move_ops.get_order = get_order;
    turn->move_ops.distance = distance_value;
    turn->move_ops.fuel_use = fuel_use;
    turn->move_ops.kill_waypoints = kill_waypoints;
    turn->move_ops.context = turn;
    turn->move_input.ops = &turn->move_ops;
    state->player_count = 1;
    return 1;
}

static StarsI16 load_log(void *context, StarsGenerateTurnState *state,
    StarsI16 player)
{
    (void)context; (void)state; (void)player;
    return 1;
}

static StarsI16 turn_random(void *context, StarsI16 limit)
{
    TurnState *turn = (TurnState *)context;
    ++turn->turn_random_calls;
    return limit == 1 ? 0 : -1;
}

static void stage(void *context, StarsGenerateTurnState *state)
{
    (void)context; (void)state;
}

static void mode(void *context, StarsGenerateTurnState *state, StarsI16 mode)
{
    (void)context; (void)state; (void)mode;
}

static void move_stage(void *context, StarsGenerateTurnState *state)
{
    TurnState *turn = (TurnState *)context;
    (void)state;
    event(turn, TURN_EVENT_MOVE);
    stars_move_fleets(&turn->move_input);
    event(turn, TURN_EVENT_AFTER_MOVE);
}

static void cleanup(void *context, StarsGenerateTurnState *state)
{
    (void)context; (void)state;
}

static void install(TurnState *turn, StarsGenerateTurnOps *ops)
{
    memset(ops, 0, sizeof *ops);
    memset(&turn->kill_ops, 0, sizeof turn->kill_ops);
    memset(&turn->move_ops, 0, sizeof turn->move_ops);
    turn->kill_ops.delete_waypoint = delete_waypoint;
    turn->kill_ops.send_message_ex = send_message_ex;
    turn->kill_ops.context = turn;
    turn->move_ops.get_order = get_order;
    turn->move_ops.distance = distance_value;
    turn->move_ops.fuel_use = fuel_use;
    turn->move_ops.kill_waypoints = kill_waypoints;
    turn->move_ops.context = turn;
    ops->load_game = load_game;
    ops->load_player_log = load_log;
    ops->random_value = turn_random;
    ops->initialize_turn = stage;
    ops->validate_waypoints = stage;
    ops->do_orders = mode;
    ops->move_things = mode;
    ops->move_fleets = move_stage;
    ops->unmark_mine_fields = stage;
    ops->thing_decay = stage;
    ops->breed_colonists = stage;
    ops->produce = stage;
    ops->fuel_fleets = stage;
    ops->sweep_for_mines = stage;
    ops->heal_ships = stage;
    ops->auto_terraform = stage;
    ops->remote_terraforming = stage;
    ops->spank_cheaters = stage;
    ops->update_guesses = stage;
    ops->update_scores = stage;
    ops->mark_backup = stage;
    ops->cleanup = cleanup;
    ops->context = turn;
}

int kill_used_waypoints_turn_probe(void)
{
    TurnState turn;
    StarsGenerateTurnState state;
    StarsGenerateTurnOps ops;
    int failures = 0;
    int move_event = -1;
    int kill_event = -1;
    int after_event = -1;
    StarsI16 index;

    memset(&state, 0, sizeof state);
    install(&turn, &ops);
    if (!stars_generate_turn(&state, &ops))
        return 1;
    for (index = 0; index < turn.event_count; ++index) {
        if (turn.events[index] == TURN_EVENT_MOVE) move_event = index;
        if (turn.events[index] == TURN_EVENT_KILL) kill_event = index;
        if (turn.events[index] == TURN_EVENT_AFTER_MOVE) after_event = index;
    }
    if (!state.generated || !state.output_written || state.turn != 1)
        ++failures;
    if (turn.turn_random_calls != 1 || state.player_order_count != 1 ||
            state.player_order[0] != 0)
        ++failures;
    if (move_event < 0 || kill_event <= move_event ||
            after_event <= kill_event)
        ++failures;
    if (turn.kill_calls != 1 || turn.move_order_calls == 0)
        ++failures;
    if (get16(turn.fleet_records[0], 8U) != 10U ||
            get16(turn.fleet_records[0], 10U) != 20U)
        ++failures;
    if (turn.delete_calls != 0)
        ++failures;
    if (get16(turn.fleet_records[0], 98U) != 4U ||
            get16(turn.plord_records[0], 22U) != 33U ||
            get16(turn.plord_records[0], 24U) != 44U ||
            turn.message_calls != 0)
        ++failures;
    return failures;
}

int main(void)
{
    int failures = kill_used_waypoints_turn_probe();
    if (failures == 0)
        puts("KILL_USED_WAYPOINTS_TURN_CHECKS=6 FAILURES=0 RESULT=PASS");
    else
        puts("KILL_USED_WAYPOINTS_TURN_CHECKS=6 FAILURES=1 RESULT=FAIL");
    return failures != 0;
}
