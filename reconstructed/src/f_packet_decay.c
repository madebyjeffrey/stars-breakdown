#include "core.h"

static StarsI16 packet_decay_rate(StarsU16 kind)
{
    if (kind == 1U)
        return 10;
    if (kind == 2U)
        return 25;
    return 50;
}

static StarsI16 packet_clamp_decay(StarsI16 amount, StarsI32 decay,
    StarsI16 minimum)
{
    StarsI32 selected = decay;
    if (selected < (StarsI32)minimum)
        selected = minimum;
    if (selected > (StarsI32)amount)
        selected = amount;
    return (StarsI16)selected;
}

/* Genuine C reconstruction of FPacketDecay, NE 24:6E9C-70C3. */
StarsI16 stars_f_packet_decay(StarsPacketDecayThing *thing,
    StarsI16 percent_rate, const StarsPacketDecayInput *input)
{
    StarsU16 kind;
    StarsI16 rate;
    StarsI16 minimum;
    StarsI16 index;
    StarsI16 decay_amount;
    StarsI32 decay;
    StarsI32 remaining;

    if (!thing)
        return 0;
    kind = (StarsU16)((thing->status_flags >> 14) & 3U);
    if (kind == 0U)
        return 0;
    rate = packet_decay_rate(kind);
    minimum = 10;
    if (input && input->race_stat &&
        input->race_stat(input->context, thing->owner, 0x000e) == 6) {
        rate = (StarsI16)(rate / 2);
        minimum = 5;
    }
    remaining = 0L;
    for (index = 0; index < 3; ++index) {
        if (thing->amounts[index] != 0) {
            decay = ((StarsI32)thing->amounts[index] * rate *
                percent_rate) / 10000L;
            decay_amount = packet_clamp_decay(thing->amounts[index], decay,
                minimum);
            thing->amounts[index] = (StarsI16)(
                thing->amounts[index] - decay_amount);
            remaining += thing->amounts[index];
        }
    }
    if (remaining == 0L) {
        if (input && input->free_thing)
            input->free_thing(input->context, thing);
        return 1;
    }
    thing->status_flags = (StarsU16)((thing->status_flags & 0xc000U) |
        (StarsU16)((remaining + 9L) / 10L));
    return 0;
}
