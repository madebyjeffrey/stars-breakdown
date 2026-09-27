#include "core.h"

#define CYBER_PACKET_MINERAL_COUNT 3
#define CYBER_PACKET_ITEM_BASE 14
#define CYBER_PACKET_ITEM_OBJECT 1
#define CYBER_PACKET_QUEUE_MODE 0
#define CYBER_PACKET_MAX_PER_PLANET 7
#define CYBER_PACKET_READY 0x0080
#define CYBER_PACKET_TARGET_NONE 0xffff

static StarsU16 cyber_packet_remainder(const StarsCyberPacketPlanet *planet,
    StarsU16 mineral)
{
    StarsU32 available;
    StarsU32 cost;

    available = planet->available[mineral];
    cost = planet->cost[mineral];
    if (available < cost)
        return 0;
    available -= cost;
    if (available > 0xffffUL)
        return 0xffff;
    return (StarsU16)available;
}

static StarsI16 cyber_packet_best_mineral(
    const StarsCyberPacketPlanet *planet)
{
    StarsI16 best;
    StarsU16 best_remainder;
    StarsU16 remainder;
    StarsU16 mineral;

    best = 0;
    best_remainder = cyber_packet_remainder(planet, 0);
    for (mineral = 1; mineral < CYBER_PACKET_MINERAL_COUNT; ++mineral) {
        remainder = cyber_packet_remainder(planet, mineral);
        if (remainder > best_remainder) {
            best = (StarsI16)mineral;
            best_remainder = remainder;
        }
    }
    return best;
}

static StarsU16 cyber_packet_count(const StarsCyberPacketPlanet *planet,
    StarsU16 minimum_strength)
{
    StarsU16 count;

    if (minimum_strength == 0)
        minimum_strength = 1;
    count = (StarsU16)(planet->packet_strength / minimum_strength);
    if (count > CYBER_PACKET_MAX_PER_PLANET)
        count = CYBER_PACKET_MAX_PER_PLANET;
    return count;
}

void stars_do_cyber_packets(StarsDoCyberPacketsInput *input)
{
    StarsU16 planet_index;
    StarsU16 target_id;
    StarsU16 count;
    StarsI16 mineral;
    StarsCyberPacketPlanet *source;

    if (!input || !input->planets || !input->ops ||
            !input->ops->find_target || !input->ops->queue_item)
        return;
    for (planet_index = 0; planet_index < input->planet_count; ++planet_index) {
        source = &input->planets[planet_index];
        if (source->owner != input->player ||
                (source->packet_flags & CYBER_PACKET_READY) == 0)
            continue;
        count = cyber_packet_count(source, input->minimum_strength);
        if (input->maximum_packets != 0 && count > input->maximum_packets)
            count = input->maximum_packets;
        if (count == 0)
            continue;
        target_id = CYBER_PACKET_TARGET_NONE;
        if (!input->ops->find_target(input->ops->context, source, &target_id) ||
                target_id == CYBER_PACKET_TARGET_NONE || target_id == source->id)
            continue;
        mineral = cyber_packet_best_mineral(source);
        if (!input->ops->queue_item(input->ops->context,
                (StarsI16)(CYBER_PACKET_ITEM_BASE + mineral),
                (StarsI16)count, CYBER_PACKET_ITEM_OBJECT,
                CYBER_PACKET_QUEUE_MODE))
            continue;
        source->packet_strength = (StarsU16)(source->packet_strength -
            count * input->minimum_strength);
        if (input->ops->select_planet)
            input->ops->select_planet(input->ops->context, source->id,
                target_id);
    }
}
