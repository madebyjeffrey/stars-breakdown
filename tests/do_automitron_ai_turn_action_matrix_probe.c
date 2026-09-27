#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 callback_counts[10];
    StarsI16 planned;
    StarsI16 visited;
    StarsI16 valid;
    StarsI16 foreign_seen;
} Trace;

static StarsI16 plan(void *opaque,
    const StarsAutomitronTurnFleet *fleet,
    const StarsAutomitronFleetPolicy *policy, StarsI16 turn,
    StarsAutomitronFleetPlan *out)
{
    Trace *trace;

    trace = (Trace *)opaque;
    ++trace->planned;
    if (turn != 150 || policy->total_ships == 0 ||
            fleet->id < 1 || fleet->id > 9)
        return 0;
    out->action = fleet->id;
    out->target_planet = (StarsI16)(100 + fleet->id);
    out->order_kind = (StarsI16)(fleet->id + 1);
    out->append = (StarsI16)(fleet->id & 1);
    out->reason = (StarsI16)(200 + fleet->id);
    return 1;
}

static StarsI16 verify_action(void *opaque,
    const StarsAutomitronTurnFleet *fleet,
    const StarsAutomitronFleetPlan *plan_in, StarsI16 action)
{
    Trace *trace;

    trace = (Trace *)opaque;
    ++trace->callback_counts[action];
    if (fleet->id == action && plan_in->action == action &&
            plan_in->target_planet == 100 + action &&
            plan_in->order_kind == action + 1 &&
            plan_in->append == (action & 1) &&
            plan_in->reason == 200 + action)
        ++trace->valid;
    return 1;
}

#define ACTION_CALLBACK(name, action) \
static StarsI16 name(void *opaque, \
    const StarsAutomitronTurnFleet *fleet, \
    const StarsAutomitronFleetPlan *plan_in) \
{ \
    return verify_action(opaque, fleet, plan_in, action); \
}

ACTION_CALLBACK(clear_task, STARS_AUTOMITRON_FLEET_ACTION_CLEAR_TASK)
ACTION_CALLBACK(move, STARS_AUTOMITRON_FLEET_ACTION_MOVE)
ACTION_CALLBACK(colonize, STARS_AUTOMITRON_FLEET_ACTION_COLONIZE)
ACTION_CALLBACK(wormhole, STARS_AUTOMITRON_FLEET_ACTION_WORMHOLE)
ACTION_CALLBACK(starbase, STARS_AUTOMITRON_FLEET_ACTION_MOVE_STARBASE)
ACTION_CALLBACK(attack, STARS_AUTOMITRON_FLEET_ACTION_ATTACK)
ACTION_CALLBACK(armada, STARS_AUTOMITRON_FLEET_ACTION_TARGET_ARMADA)
ACTION_CALLBACK(transport, STARS_AUTOMITRON_FLEET_ACTION_TRANSPORT)
ACTION_CALLBACK(freighter, STARS_AUTOMITRON_FLEET_ACTION_TARGET_FREIGHTER)

static StarsI16 visit(void *opaque,
    const StarsAutomitronTurnFleet *fleet,
    const StarsAutomitronFleetPolicy *policy, StarsI16 potency)
{
    Trace *trace;

    trace = (Trace *)opaque;
    ++trace->visited;
    if (fleet->owner == 9)
        ++trace->foreign_seen;
    if (policy->total_ships != 1 || potency != 4)
        return 0;
    return 1;
}

int main(void)
{
    StarsAutomitronTurnFleet fleets[10];
    StarsAutomitronAiTurnOps ops;
    StarsAutomitronAiTurnInput input;
    Trace trace;
    StarsI16 action;
    StarsI16 failures;

    memset(fleets, 0, sizeof fleets);
    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    for (action = 1; action <= 9; ++action) {
        fleets[action - 1].id = action;
        fleets[action - 1].owner = 4;
        fleets[action - 1].ship_counts[2] = 1;
    }
    fleets[9].id = 10;
    fleets[9].owner = 9;
    fleets[9].ship_counts[2] = 1;
    ops.plan_fleet_action = plan;
    ops.clear_task = clear_task;
    ops.move = move;
    ops.colonize = colonize;
    ops.goto_wormhole = wormhole;
    ops.move_starbase = starbase;
    ops.attack = attack;
    ops.target_armada = armada;
    ops.transport = transport;
    ops.target_freighter = freighter;
    ops.visit_fleet_typed = visit;
    ops.context = &trace;
    input.turn = 150;
    input.player_id = 4;
    input.fleet_count = 10;
    input.fleets = fleets;
    input.ops = &ops;

    failures = !stars_do_automitron_ai_turn(&input);
    failures += trace.planned != 9 || trace.visited != 9 ||
        trace.valid != 9 || trace.foreign_seen != 0;
    for (action = 1; action <= 9; ++action)
        failures += trace.callback_counts[action] != 1;

    printf("AUTOMITRON_AI_ACTION_MATRIX_CHECKS=20 FAILURES=%d RESULT=%s\n",
        (int)failures, failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
}
