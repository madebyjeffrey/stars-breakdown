#include "core.h"

#define THING_DECAY_PACKET_KIND 1
#define THING_DECAY_FLEET_HIDDEN 0x0400
#define THING_DECAY_FLEET_MARKED 0x1000
#define THING_DECAY_PACKET_CLEAR 0x4000
#define THING_DECAY_MINE_ACTIVE 0x01

static void thing_decay_clear_fleet_marks(StarsThingDecayInput *input)
{
    StarsU16 index;
    if (!input || !input->fleets)
        return;
    for (index = 0; index < input->fleet_count; ++index)
        input->fleets[index].status_flags = (StarsU16)
            (input->fleets[index].status_flags &
             (StarsU16)~THING_DECAY_FLEET_MARKED);
}

static void thing_decay_packet(StarsThingDecayInput *input,
    StarsThingDecayThing *thing)
{
    StarsI16 index;
    StarsI32 decrement;
    StarsI32 total;

    if ((thing->packet_flags & 0x3c00U) != 0U) {
        if (input->ops.packet_decay &&
                input->ops.packet_decay(input->context, thing, 100))
            input->ops.free_thing(input->context, thing);
        return;
    }
    if ((thing->packet_flags & THING_DECAY_PACKET_CLEAR) != 0U) {
        thing->packet_flags = (StarsU16)(thing->packet_flags &
            (StarsU16)~THING_DECAY_PACKET_CLEAR);
        return;
    }
    total = 0L;
    for (index = 0; index < 3; ++index) {
        if (thing->packet_resources[index] == 0L)
            continue;
        decrement = thing->packet_resources[index] / 10L;
        if (decrement < 10L)
            decrement = 10L;
        thing->packet_resources[index] -= decrement;
        if (thing->packet_resources[index] < 0L)
            thing->packet_resources[index] = 0L;
        total += thing->packet_resources[index];
    }
    if (total == 0L) {
        input->ops.free_thing(input->context, thing);
        return;
    }
    thing->strength = (thing->strength & (StarsI32)0xc000L) |
        ((total + 9L) / 10L & 0x3fffL);
}

static void thing_decay_mine(StarsThingDecayInput *input,
    StarsThingDecayThing *thing)
{
    StarsU16 index;
    StarsI16 stat;
    StarsI16 planet_count;
    StarsI32 loss;
    StarsI32 scaled_loss;
    StarsThingDecayFleet *fleet;

    stat = input->ops.race_stat ? input->ops.race_stat(input->context,
        thing->owner, 0x0e) : 0;
    if ((thing->active & THING_DECAY_MINE_ACTIVE) != 0) {
        for (index = 0; index < input->fleet_count; ++index) {
            fleet = &input->fleets[index];
            if ((fleet->status_flags & THING_DECAY_FLEET_HIDDEN) != 0 ||
                    (fleet->status_flags & THING_DECAY_FLEET_MARKED) != 0)
                continue;
            if ((StarsI32)(fleet->x - thing->x) *
                    (StarsI32)(fleet->x - thing->x) +
                    (StarsI32)(fleet->y - thing->y) *
                    (StarsI32)(fleet->y - thing->y) <= thing->radius) {
                if (input->ops.travel_through_mines)
                    input->ops.travel_through_mines(input->context, fleet,
                        thing);
                fleet->status_flags = (StarsU16)(fleet->status_flags |
                    THING_DECAY_FLEET_MARKED);
            }
        }
    }
    planet_count = input->ops.planets_in_circle ?
        input->ops.planets_in_circle(input->context, thing->x, thing->y,
            thing->radius) : 0;
    loss = (StarsI32)(((stat != 5 ? 4 : 1) * planet_count) + 2);
    if (loss > 50L)
        loss = 50L;
    if ((thing->active & THING_DECAY_MINE_ACTIVE) != 0)
        loss += 25L;
    scaled_loss = thing->radius * loss / 100L;
    if (scaled_loss < loss)
        scaled_loss = loss;
    if (thing->mine_type != 2 && scaled_loss < 10L)
        scaled_loss = 10L;
    if (scaled_loss < thing->strength)
        thing->strength -= scaled_loss;
    else
        input->ops.free_thing(input->context, thing);
}

/* NE 24:70C6-7637. Clear per-turn mine-contact marks, decay packet cargo,
 * and reduce active minefield strength according to nearby planets and the
 * mine owner’s race trait. Freed things are removed through the supplied
 * callback, mirroring the original compacting allocator. */
void stars_thing_decay(StarsThingDecayInput *input)
{
    StarsU16 index;
    StarsThingDecayThing *thing;

    if (!input || !input->things || !input->ops.free_thing)
        return;
    thing_decay_clear_fleet_marks(input);
    for (index = 0; index < input->thing_count; ++index) {
        thing = &input->things[index];
        if (!thing->active)
            continue;
        if (thing->kind == THING_DECAY_PACKET_KIND)
            thing_decay_packet(input, thing);
        else if ((thing->kind & 0x1fff) == 0)
            thing_decay_mine(input, thing);
    }
}
