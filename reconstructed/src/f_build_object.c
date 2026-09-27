#include "core.h"

#define BUILD_PLANET_STARBASE 0x0200U
#define BUILD_MESSAGE_INSTALL_MINES 0x37
#define BUILD_MESSAGE_INSTALL_FACTORIES 0x35
#define BUILD_MESSAGE_INSTALL_DEFENSES 0x39
#define BUILD_MESSAGE_TERRAFORM 0x7b
#define BUILD_MESSAGE_PACKET_NO_WARP 0xd1
#define BUILD_MESSAGE_PACKET_NO_SPACE 0xd2
#define BUILD_MESSAGE_PACKET_CREATED 0xd3
#define BUILD_MESSAGE_PACKET_UPDATED 0xd4
#define BUILD_MESSAGE_RANDOMIZE 0x11b
#define BUILD_MESSAGE_SPECIAL 0x7c
#define BUILD_RACE_STAT_PACKET 0x0e
#define BUILD_PACKET_WARP_BITS 4
#define BUILD_PACKET_MAX 0x3ff

static void build_message(const StarsBuildObjectInput *input,
    StarsI16 message, StarsI16 a, StarsI16 b, StarsI16 c, StarsI16 d,
    StarsI16 e, StarsI16 f, StarsI16 g, StarsI16 h)
{
    if (input->ops.send_message)
        input->ops.send_message(input->ops.context, input->planet->player,
            message, a, b, c, d, e, f, g, h);
}

static StarsI16 bounded_add(StarsI16 value, StarsI16 delta,
    StarsI16 minimum, StarsI16 maximum)
{
    StarsI16 result;
    result = (StarsI16)(value + delta);
    if (result < minimum) result = minimum;
    if (result > maximum) result = maximum;
    return result;
}

static StarsI16 build_installation(StarsBuildObjectInput *input,
    StarsI16 kind, StarsI16 message)
{
    StarsI16 maximum;
    StarsI16 available;
    StarsI16 quantity;
    StarsI16 old_messages;
    StarsI16 old_count;

    if (!input->ops.max_installation) return 0;
    maximum = input->ops.max_installation(input->ops.context, input->planet,
        kind);
    old_count = input->planet->installations[kind];
    available = (StarsI16)(maximum - old_count);
    quantity = input->quantity;
    if (quantity > available) quantity = available;
    if (quantity < 1) return 0;
    input->planet->installations[kind] = (StarsI16)(old_count + quantity);
    old_messages = 0;
    if (input->ops.remove_message)
        old_messages = input->ops.remove_message(input->ops.context,
            input->planet->player, message, input->planet->id);
    if ((StarsI16)(quantity + old_messages) < 2) {
        build_message(input, message, input->planet->id, input->planet->id,
            0, 0, 0, 0, 0, 0);
    } else {
        build_message(input, (StarsI16)(message + 1), input->planet->id,
            (StarsI16)(quantity + old_messages), input->planet->id,
            0, 0, 0, 0, 0);
    }
    return 1;
}

static StarsI16 build_terraform(StarsBuildObjectInput *input)
{
    StarsI16 count;
    StarsI16 direction;
    StarsI16 axis;
    StarsI16 new_value;

    count = input->quantity;
    while (count > 0) {
        direction = 0;
        if (input->ops.best_terraform)
            direction = input->ops.best_terraform(input->ops.context,
                input->planet, 1);
        if (direction == 0) break;
        axis = direction < 0 ? (StarsI16)(-direction - 1) :
            (StarsI16)(direction - 1);
        if (axis < 0 || axis >= 3) break;
        new_value = bounded_add(input->planet->environment[axis],
            direction < 0 ? -1 : 1, 1, 99);
        input->planet->environment[axis] = new_value;
        build_message(input, BUILD_MESSAGE_TERRAFORM, input->planet->id,
            input->planet->id, direction > 0 ? 1 : 0, axis,
            new_value, 0, 0, 0);
        --count;
    }
    return count == input->quantity ? 0 : 1;
}

static StarsI16 build_packets(StarsBuildObjectInput *input)
{
    StarsI16 warp;
    StarsI16 range;
    StarsI16 race_stat;
    StarsI16 rate;
    StarsI16 tier;
    StarsI16 i;
    StarsI16 amount;
    StarsBuildObjectPacket packet;

    warp = 0;
    range = 0;
    if (input->ops.race_stat)
        race_stat = input->ops.race_stat(input->ops.context,
            input->planet->player, BUILD_RACE_STAT_PACKET);
    else
        race_stat = 0;
    if (!input->ops.warp_machine ||
            input->ops.warp_machine(input->ops.context, input->planet,
                &range) == 0) {
        build_message(input, BUILD_MESSAGE_PACKET_NO_WARP, input->planet->id,
            input->planet->id, 0, 0, 0, 0, 0, 0);
        return 0;
    }
    if (input->planet->starbase_packets <= 0) {
        build_message(input, BUILD_MESSAGE_PACKET_NO_SPACE, input->planet->id,
            input->planet->id, 0, 0, 0, 0, 0, 0);
        return 0;
    }
    if (input->item == 6) input->item = 0x11;
    rate = input->item == 0x11 ? (race_stat == 6 ? 25 : 40) :
        (race_stat == 6 ? 70 : 100);
    tier = (StarsI16)(input->item - 0x0e);
    if (tier < 0) tier = 0;
    amount = (StarsI16)(rate * input->quantity);
    if (amount > 0x7ff8) amount = 0x7ff8;
    for (i = 0; i < 3; ++i)
        if (input->item == 0x11 || i == tier) {
            packet.strength[i] = amount;
        } else {
            packet.strength[i] = 0;
        }
    packet.player = input->planet->player;
    packet.planet_id = input->planet->id;
    packet.warp = warp;
    packet.reach = (StarsI16)(range + 4);
    packet.range = range;
    if (input->ops.find_packet && input->ops.find_packet(
            input->ops.context, packet.warp, packet.reach, &packet)) {
        if (input->ops.update_packet)
            input->ops.update_packet(input->ops.context, &packet,
                packet.strength[0], packet.strength[1], packet.strength[2]);
        build_message(input, BUILD_MESSAGE_PACKET_UPDATED, input->planet->id,
            input->planet->id, input->planet->starbase_packets - 1,
            0, 0, 0, 0, 0);
    } else {
        if (input->ops.create_packet)
            input->ops.create_packet(input->ops.context, &packet,
                packet.strength[0], packet.strength[1], packet.strength[2],
                packet.warp, packet.reach);
        if (input->planet->starbase_packets > 0)
            --input->planet->starbase_packets;
        build_message(input, BUILD_MESSAGE_PACKET_CREATED, input->planet->id,
            input->planet->id, input->planet->starbase_packets,
            0, 0, 0, 0, 0);
    }
    return 1;
}

static StarsI16 build_special(StarsBuildObjectInput *input)
{
    StarsI16 i;
    StarsI16 value;

    if (input->item == 0x0d) {
        for (i = 0; i < input->player_count; ++i)
            if (input->ops.send_message)
                input->ops.send_message(input->ops.context, i,
                    BUILD_MESSAGE_RANDOMIZE, input->planet->id,
                    input->planet->id, 0, 0, 0, 0, 0, 0);
        for (i = 0; i < 3; ++i) {
            value = input->ops.random_value ?
                input->ops.random_value(input->ops.context, 50) : 0;
            value = (StarsI16)(value + (input->ops.random_value ?
                input->ops.random_value(input->ops.context, 50) : 0) + 1);
            input->planet->environment[i] = value;
            input->planet->mineral_concentration[i] = (StarsI16)(25 +
                (input->ops.random_value ? input->ops.random_value(
                input->ops.context, 40) : 0) + (input->ops.random_value ?
                input->ops.random_value(input->ops.context, 40) : 0));
        }
        return 1;
    }
    if (input->item == STARS_BUILD_SPECIAL_SCANNER) {
        if (input->ops.select_scanner)
            input->ops.select_scanner(input->ops.context,
                input->planet->player, input->planet->id, input->quantity);
        return 1;
    }
    if (input->item >= STARS_BUILD_SPECIAL_FIRST &&
            input->item <= STARS_BUILD_SPECIAL_LAST) {
        build_message(input, BUILD_MESSAGE_SPECIAL, input->planet->id,
            input->planet->id, (StarsI16)-32768,
            (StarsI16)(input->item - STARS_BUILD_SPECIAL_FIRST), 0, 0, 0, 0);
        if (input->ops.build_special)
            input->ops.build_special(input->ops.context,
                input->planet->player, (StarsI16)(input->item -
                STARS_BUILD_SPECIAL_FIRST), input->quantity);
        return 1;
    }
    return 0;
}

StarsI16 stars_build_object(StarsBuildObjectInput *input)
{
    StarsI16 design;

    if (!input || !input->planet || input->quantity < 1) return 0;
    if (input->object_group == STARS_BUILD_OBJECT_FLEET) {
        if (input->item > 15) {
            design = (StarsI16)(input->item - 0x10);
            if (!input->ops.can_build || !input->ops.can_build(
                    input->ops.context, input->planet->player, design, 1))
                return 0;
            if (input->planet->flags & BUILD_PLANET_STARBASE) {
                if (input->ops.kill_queued_ships)
                    input->ops.kill_queued_ships(input->ops.context,
                        input->planet->player, input->planet->id, design);
                if (input->ops.kill_queued_packets &&
                        input->planet->starbase_warp < 1)
                    input->ops.kill_queued_packets(input->ops.context,
                        input->planet->player, input->planet->id, 0);
            }
            input->planet->starbase_hull = design;
            if (input->ops.build_starbase)
                input->ops.build_starbase(input->ops.context,
                    input->planet->player, input->planet->id, design);
            return 1;
        }
        if (!(input->planet->flags & BUILD_PLANET_STARBASE) ||
                !input->ops.can_build || !input->ops.can_build(
                input->ops.context, input->planet->player, input->item, 0)) {
            build_message(input, 0x4f, input->planet->id, input->item,
                0, 0, 0, 0, 0, 0);
            return 0;
        }
        if (input->ops.build_ship)
            input->ops.build_ship(input->ops.context, input->planet->player,
                input->planet->id, input->item, input->quantity);
        return 1;
    }
    if (input->object_group != STARS_BUILD_OBJECT_PLANET) return 0;
    switch (input->item) {
    case 0:
    case 8:
        return build_installation(input, STARS_BUILD_INSTALLATION_MINES,
            BUILD_MESSAGE_INSTALL_MINES);
    case 1:
    case 7:
        return build_installation(input,
            STARS_BUILD_INSTALLATION_FACTORIES,
            BUILD_MESSAGE_INSTALL_FACTORIES);
    case 2:
    case 9:
        return build_installation(input, STARS_BUILD_INSTALLATION_DEFENSES,
            BUILD_MESSAGE_INSTALL_DEFENSES);
    case 3:
    case 0x0b:
        return 1;
    case 4:
    case 5:
    case 0x0c:
        return build_terraform(input);
    case 6:
    case 0x0e:
    case 0x0f:
    case 0x10:
    case 0x11:
        return build_packets(input);
    case 0x0d:
        return build_special(input);
    case 0x1b:
    case 0x12:
    case 0x13:
    case 0x14:
    case 0x15:
    case 0x16:
    case 0x17:
    case 0x18:
    case 0x19:
    case 0x1a:
        return build_special(input);
    case 0x0a:
        return 1;
    default:
        return 0;
    }
}
