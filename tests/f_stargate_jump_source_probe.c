#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 damage_result;
    StarsI16 damage_percent;
    StarsI16 messages;
    StarsI16 last_message;
    StarsU16 last_fleet_reference;
    StarsI16 last_fleet_id;
    StarsI16 last_planet;
    StarsI16 last_destination;
    StarsI16 last_slot;
    StarsU32 last_amount;
    StarsI16 random_value;
    StarsI16 random_calls;
    StarsI16 transfers;
} Trace;

static StarsI16 damage(void *context, StarsI16 source, StarsI16 destination,
    StarsI16 distance, StarsI16 mass, StarsI16 *percent)
{
    Trace *trace = (Trace *)context;
    (void)source;
    (void)destination;
    (void)distance;
    (void)mass;
    *percent = trace->damage_percent;
    return trace->damage_result;
}

static StarsI16 random_value(void *context, StarsI16 limit)
{
    Trace *trace = (Trace *)context;
    (void)limit;
    ++trace->random_calls;
    if (trace->random_calls == 1)
        return 0;
    return trace->random_value;
}

static void send_message(void *context, const StarsFStargateMessage *message)
{
    Trace *trace = (Trace *)context;
    ++trace->messages;
    trace->last_message = message->code;
    trace->last_fleet_reference = message->fleet_reference;
    trace->last_fleet_id = message->fleet_id;
    trace->last_planet = message->planet;
    trace->last_destination = message->destination;
    trace->last_slot = message->slot;
    trace->last_amount = message->amount;
}

static void transfer(void *context, StarsFStargateFleet *source,
    StarsFStargateFleet *destination)
{
    Trace *trace = (Trace *)context;
    (void)source;
    (void)destination;
    ++trace->transfers;
}

static void initialize(StarsFStargateFleet *fleet, Trace *trace,
    StarsFStargateJumpInput *input)
{
    memset(fleet, 0, sizeof *fleet);
    memset(trace, 0, sizeof *trace);
    memset(input, 0, sizeof *input);
    fleet->player = 2;
    fleet->id = 41;
    fleet->planet = 9;
    fleet->cargo[0] = 10U;
    fleet->cargo_state[0] = 50U;
    input->fleet = fleet;
    input->destination = 12;
    input->design_mass[0] = 3;
    input->ship_weight[0] = 100;
    input->race_stat_stargate_loss = 0;
    input->stargate_damage = damage;
    input->random_value = random_value;
    input->send_message = send_message;
    input->transfer_cargo_balance = transfer;
    input->context = trace;
}

int main(void)
{
    StarsFStargateFleet fleet;
    StarsFStargateJumpInput input;
    Trace trace;

    initialize(&fleet, &trace, &input);
    trace.damage_result = -2;
    if (stars_f_stargate_jump(&input, 1, 2, 30) != 0 ||
        trace.last_message != STARS_STARGATE_MSG_DAMAGE_ABORT ||
        trace.last_fleet_reference != 0x8029 || trace.last_fleet_id != 41 ||
        trace.last_planet != 9 || trace.last_destination != 12 ||
        trace.last_slot != 0 || trace.last_amount != 0U)
        return 1;

    initialize(&fleet, &trace, &input);
    trace.damage_result = 1;
    trace.damage_percent = 100;
    if (stars_f_stargate_jump(&input, 1, 2, 30) != 0 ||
        trace.last_message != STARS_STARGATE_MSG_EMPTY || fleet.cargo[0] != 0U ||
        trace.last_fleet_reference != 0x8029 || trace.last_fleet_id != 41 ||
        trace.last_planet != 9 || trace.last_destination != 12 ||
        trace.last_slot != 0 || trace.last_amount != 0U)
        return 2;

    initialize(&fleet, &trace, &input);
    trace.damage_result = 1;
    trace.damage_percent = 25;
    trace.random_value = 99;
    if (stars_f_stargate_jump(&input, 1, 2, 30) != 1 ||
        trace.transfers != 1 || fleet.cargo[0] == 0U ||
        (fleet.flags & 0x4000U) == 0U)
        return 3;

    initialize(&fleet, &trace, &input);
    fleet.cargo[0] = 4U;
    fleet.cargo_state[0] = 2660U;
    input.ship_weight[0] = 100;
    trace.damage_result = 1;
    trace.damage_percent = 7;
    trace.random_value = 99;
    if (stars_f_stargate_jump(&input, 1, 2, 30) != 1 ||
        fleet.cargo[0] != 3U || fleet.cargo_state[0] != 7780U)
        return 4;

    puts("F_STARGATE_JUMP_SOURCE=PASS");
    return 0;
}
