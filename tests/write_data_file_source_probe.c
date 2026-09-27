#ifndef TARGET_NATIVE
#include <stdio.h>
#endif
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsU16 visible[4];
    StarsU16 visible_count;
    StarsU16 prepares;
    StarsU16 marks;
    StarsU16 creates;
    StarsU16 appends;
    StarsU16 battles;
    StarsU16 players;
    StarsU16 salts;
    StarsU16 messages;
    StarsU16 planets;
    StarsU16 designs;
    StarsU16 fleets;
    StarsU16 plans;
    StarsU16 turns;
    StarsU16 closes;
    StarsU16 alerts;
    StarsU16 alert_id;
    StarsU16 expected_type;
    StarsU16 event_count;
    StarsU16 events[16];
} Trace;

static void record_event(Trace *t, StarsU16 event)
{
    if (t->event_count < 16U)
        t->events[t->event_count++] = event;
    else
        ++t->alerts;
}

static void visible(void *opaque, StarsI16 player)
{
    Trace *t = (Trace *)opaque;
    if (t->visible_count < 4U)
        t->visible[t->visible_count++] = (StarsU16)player;
}

static void prepare(void *opaque, StarsI16 player)
{
    Trace *t = (Trace *)opaque;
    if (player < 0) ++t->alerts;
    ++t->prepares;
}

static void mark(void *opaque, StarsI16 player)
{
    Trace *t = (Trace *)opaque;
    (void)player;
    ++t->marks;
}

static StarsI16 create_file(void *opaque, StarsU16 type, StarsI16 player)
{
    Trace *t = (Trace *)opaque;
    if (t->expected_type != 0U && (type != t->expected_type ||
            ((type == STARS_WRITE_DATA_GAME_FILE && player != -1) ||
             (type == STARS_WRITE_DATA_PLAYER_FILE && player < 0))))
        ++t->alerts;
    ++t->creates;
    return t->expected_type != 0U;
}

static StarsI16 append_file(void *opaque, StarsI16 player)
{
    Trace *t = (Trace *)opaque;
    (void)player;
    ++t->appends;
    return t->expected_type == 0U ? 0 : 1;
}

static void stage(void *opaque, StarsI16 player)
{
    Trace *t = (Trace *)opaque;
    (void)player;
    ++t->battles;
}

static StarsI16 player_predicate(void *opaque, StarsU16 player)
{
    Trace *t = (Trace *)opaque;
    (void)t;
    return (StarsI16)(player != 1U);
}

static void write_player(void *opaque, StarsU16 player)
{
    Trace *t = (Trace *)opaque;
    (void)player;
    ++t->players;
}

static void write_salt(void *opaque, StarsU32 salt_value)
{
    Trace *t = (Trace *)opaque;
    if (salt_value != 0x11223344UL) ++t->alerts;
    ++t->salts;
}

static StarsI16 design_predicate(void *opaque, StarsU16 player,
    StarsU16 bank, StarsU16 slot)
{
    Trace *t = (Trace *)opaque;
    (void)t;
    return (StarsI16)(player == 0U && bank < 2U && slot == 0U);
}

static void write_design(void *opaque, StarsU16 player, StarsU16 bank,
    StarsU16 slot)
{
    Trace *t = (Trace *)opaque;
    if (player != 0U || bank > 1U || slot != 0U) ++t->alerts;
    record_event(t, (StarsU16)(10U + bank));
    ++t->designs;
}

static StarsI16 fleet_predicate(void *opaque, StarsU16 index)
{
    Trace *t = (Trace *)opaque;
    (void)t;
    return (StarsI16)(index == 1U);
}

static void write_fleet(void *opaque, StarsU16 index)
{
    Trace *t = (Trace *)opaque;
    if (index != 1U) ++t->alerts;
    record_event(t, 20U);
    ++t->fleets;
}

static void write_scores(void *opaque, StarsI16 player)
{
    Trace *t = (Trace *)opaque;
    (void)player;
    record_event(t, 30U);
}

static void write_things(void *opaque, StarsI16 player)
{
    Trace *t = (Trace *)opaque;
    (void)player;
    record_event(t, 40U);
}

static void write_turn(void *opaque, StarsU16 turn)
{
    Trace *t = (Trace *)opaque;
    if (turn != 17U) ++t->alerts;
    ++t->turns;
}

static void write_plans(void *opaque, StarsI16 player)
{
    Trace *t = (Trace *)opaque;
    (void)player;
    record_event(t, 50U);
    ++t->plans;
}

static void close_file(void *opaque)
{
    ++((Trace *)opaque)->closes;
}

static void alert(void *opaque, StarsU16 error)
{
    Trace *t = (Trace *)opaque;
    ++t->alerts;
    t->alert_id = error;
}

static void initialize_ops(StarsWriteDataFileOps *ops)
{
    memset(ops, 0, sizeof *ops);
    ops->set_visible_plan_fleet = visible;
    ops->prepare_view = prepare;
    ops->mark_messages = mark;
    ops->mark_planets_lost = mark;
    ops->create_file = create_file;
    ops->append_file = append_file;
    ops->write_battles = stage;
    ops->write_player_predicate = player_predicate;
    ops->write_player = write_player;
    ops->write_salt = write_salt;
    ops->write_messages = stage;
    ops->write_planets = stage;
    ops->write_design_predicate = design_predicate;
    ops->write_design = write_design;
    ops->write_fleet_predicate = fleet_predicate;
    ops->write_fleet = write_fleet;
    ops->write_scores = write_scores;
    ops->write_things = write_things;
    ops->write_battle_plans = write_plans;
    ops->write_turn = write_turn;
    ops->close_file = close_file;
    ops->alert = alert;
}

int main(void)
{
    Trace t;
    StarsWriteDataFileInput input;
    StarsWriteDataFileState state;
    StarsWriteDataFileOps ops;
    static const StarsU16 expected_events[] = {
        10U, 20U, 11U, 30U, 40U, 50U, 50U
    };
    memset(&t, 0, sizeof t);
    t.expected_type = STARS_WRITE_DATA_GAME_FILE;
    initialize_ops(&ops);
    memset(&input, 0, sizeof input);
    input.player = -1;
    input.player_count = 2U;
    input.fleet_count = 3U;
    input.turn = 17U;
    input.salt = 0x11223344UL;
    input.session_flags = 0x0002U;
    if (!stars_write_data_file(&input, &state, &ops, &t) ||
            t.creates != 1U || t.appends != 0U || t.prepares != 0U ||
            t.marks != 2U || t.players != 1U || t.salts != 1U ||
            t.designs != 2U || t.fleets != 1U || t.plans != 2U ||
            t.turns != 1U || t.closes != 1U || t.visible_count != 2U ||
            (StarsI16)t.visible[0] != -1 ||
            (StarsI16)t.visible[1] != -1 || t.alerts != 0U ||
            t.event_count != (StarsU16)(sizeof expected_events /
                sizeof expected_events[0]) ||
            memcmp(t.events, expected_events, sizeof expected_events) != 0)
        return 1;

    memset(&t, 0, sizeof t);
    t.expected_type = STARS_WRITE_DATA_PLAYER_FILE;
    input.player = 1;
    input.append = 1;
    input.salt = 0x11223344UL;
    if (!stars_write_data_file(&input, &state, &ops, &t) ||
            t.creates != 0U || t.appends != 1U || t.prepares != 1U ||
            t.salts != 0U || t.designs != 2U || t.plans != 1U || t.closes != 1U ||
            t.alerts != 0U) return 1;

    memset(&t, 0, sizeof t);
    t.expected_type = STARS_WRITE_DATA_PLAYER_FILE;
    input.session_flags = 0U;
    input.append = 0;
    if (!stars_write_data_file(&input, &state, &ops, &t) ||
            t.creates != 1U || t.appends != 0U || t.prepares != 0U ||
            t.marks != 2U || t.closes != 1U || t.alerts != 0U)
        return 1;

    memset(&t, 0, sizeof t);
    t.expected_type = 0U;
    input.player = 1;
    input.append = 1;
    if (stars_write_data_file(&input, &state, &ops, &t) ||
            t.appends != 1U || t.creates != 1U || t.alerts != 1U ||
            t.alert_id != STARS_WRITE_DATA_ALERT_APPEND || t.closes != 0U)
        return 1;
#ifndef TARGET_NATIVE
    printf("WRITE_DATA_CHECKS=5 FAILURES=0 RESULT=PASS\n");
#endif
    return 0;
}
