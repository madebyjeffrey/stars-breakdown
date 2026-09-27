#include "core.h"

#define MYSTERY_TRADER_SPECIAL_A 0x0040
#define MYSTERY_TRADER_SPECIAL_B 0x0080
#define MYSTERY_TRADER_SPECIAL_C 0x0400
#define MYSTERY_TRADER_SPECIAL_D 0x0800

/* NE 24:5EFA-6258. On the original turn schedule, create one mystery-trader
 * object at a random map edge/corner. Its low flags choose the trader's
 * presentation, while later random choices select either a special ability
 * or a technology mask; announce the new object to every player. */
void stars_mystery_trader(StarsMysteryTraderInput *input)
{
    StarsI16 trigger;
    StarsI16 chance;
    StarsI16 flags;
    StarsI16 choice;
    StarsI16 roll;
    StarsI16 index;
    StarsI16 coordinates[5];
    StarsU32 mask;
    StarsMysteryTraderThing *thing;

    if (!input || !input->random || !input->allocate_thing ||
            input->turn <= 0x27)
        return;
    if (input->turn % 100 == 0x47)
        trigger = 2;
    else if (input->turn % 100 == 0x21)
        trigger = 3;
    else if ((input->turn & 0x7f) == 0x31)
        trigger = 4;
    else {
        if ((input->turn & 1) != 0)
            return;
        trigger = 7;
    }
    if (input->random(input->context, trigger) != 0)
        return;
    thing = input->allocate_thing(input->context, 0, 3);
    if (!thing)
        return;
    flags = input->random(input->context, 5);
    thing->flags = (StarsU16)((flags + 8) & 0x0f);
    for (index = 0; index < 4; index += 2)
        coordinates[index] = (StarsI16)(input->random(input->context,
            (StarsI16)(input->map_size * 400 + 0x169)) + 0x3fc);
    if (input->random(input->context, 2) == 0) {
        coordinates[1] = 0x3fc;
        coordinates[3] = (StarsI16)(input->map_size * 400 + 0x564);
    } else {
        coordinates[1] = (StarsI16)(input->map_size * 400 + 0x564);
        coordinates[3] = 0x3fc;
    }
    choice = input->random(input->context, 2);
    thing->x = coordinates[choice];
    thing->y = coordinates[choice == 0 ? 1 : 0];
    thing->endpoints[0] = coordinates[choice + 2];
    thing->endpoints[1] = coordinates[(choice == 0 ? 1 : 0) + 2];
    if (input->turn < 100)
        chance = 5;
    else if (input->turn < 0xfa)
        chance = 3;
    else
        chance = 2;
    if ((thing->flags & 0x0f) < 10)
        ++chance;
    else if ((thing->flags & 0x0f) > 10)
        --chance;
    if (input->random(input->context, 10) < chance) {
        thing->ability = input->random(input->context, 6) == 0 ?
            0x1000 : 0;
    } else {
        roll = input->random(input->context, 13);
        mask = (StarsU32)1UL << roll;
        if (mask == MYSTERY_TRADER_SPECIAL_A || mask == MYSTERY_TRADER_SPECIAL_B ||
                mask == MYSTERY_TRADER_SPECIAL_C || mask == MYSTERY_TRADER_SPECIAL_D) {
            roll = input->random(input->context, 13);
            mask = (StarsU32)1UL << roll;
            if ((input->turn < 0x78 && mask == MYSTERY_TRADER_SPECIAL_B) ||
                    (input->turn < 0x96 && mask == MYSTERY_TRADER_SPECIAL_C) ||
                    (input->turn < 0xb4 && mask == MYSTERY_TRADER_SPECIAL_D))
                if (input->random(input->context, 2) != 0)
                    mask = 0;
        }
        thing->ability = (StarsU16)mask;
    }
    for (index = 0; index < input->player_count; ++index)
        if (input->send_message)
            input->send_message(input->context, index, 299,
                (StarsI16)thing->id);
}
