#include "core.h"

#define METEOR_RACE_STAT 0x0e
#define METEOR_MESSAGE_BASE 0x83
#define METEOR_MESSAGE_OWNER_BASE 0x87

static void meteor_swap(StarsI16 *a, StarsI16 *b)
{
    StarsI16 value;
    value = *a; *a = *b; *b = value;
}

/* NE 24:560E-5EFA. Occasionally strike an eligible planet. The event
 * announces a severity-scaled mineral disturbance, damages owned population
 * unless the owner's race is immune, changes up to three climate axes, and
 * removes non-automatic production items afterward. */
void stars_meteor_strike(StarsMeteorInput *input)
{
    StarsI16 severity;
    StarsI16 axis;
    StarsI16 index;
    StarsI16 choice;
    StarsI16 value;
    StarsI16 message_order[3];
    StarsI16 mineral_order[3];
    StarsI16 delta;
    StarsU32 impact[3];
    StarsU32 population_loss;
    StarsMeteorPlanet *planet;

    if (!input || !input->random || !input->planets ||
            input->random(input->context, 20) != 0 || input->planet_count == 0)
        return;
    index = input->random(input->context, (StarsI16)input->planet_count);
    if (index < 0 || (StarsU16)index >= input->planet_count)
        return;
    planet = &input->planets[index];
    if (input->turn <= 9)
        return;
    if (!((planet->owner == -1 || planet->population == 0UL ||
                planet->population < 51UL) || input->turn > 19))
        return;
    severity = input->random(input->context, 4);
    for (axis = 0; axis < 3; ++axis)
        message_order[axis] = axis;
    for (axis = 0; axis < 3; ++axis) {
        choice = input->random(input->context, 3);
        if (choice >= 0 && choice < 3)
            meteor_swap(&message_order[axis], &message_order[choice]);
    }
    for (axis = 0; axis < 3; ++axis)
        mineral_order[axis] = axis;
    for (axis = 0; axis < 3; ++axis) {
        impact[axis] = (StarsU32)(input->random(input->context, 250) + 50);
    }
    for (axis = 0; axis < 2; ++axis) {
        choice = input->random(input->context, (StarsI16)(3 - axis));
        if (choice >= 0 && choice < 3 - axis)
            meteor_swap(&mineral_order[axis], &mineral_order[axis + choice]);
    }
    for (index = 0; index < input->player_count; ++index) {
        value = METEOR_MESSAGE_BASE;
        if (index == planet->owner && input->race_stat &&
                input->race_stat(input->context, index, METEOR_RACE_STAT) != 8)
            value = METEOR_MESSAGE_OWNER_BASE;
        value = (StarsI16)(value + severity);
        if (input->send_message)
            input->send_message(input->context, index, value, planet->id,
                impact);
    }
    if (planet->owner != -1 && input->race_stat &&
            input->race_stat(input->context, planet->owner,
                METEOR_RACE_STAT) != 8) {
        population_loss = (StarsU32)(planet->population *
            (StarsU32)(severity * 20 + 25) / 100UL);
        planet->population -= population_loss;
    }
    for (index = 0; index <= severity && index < 3; ++index) {
        axis = mineral_order[index];
        value = (StarsI16)(input->random(input->context, 17000) + 3000);
        impact[axis] += (StarsU32)value;
        value = (StarsI16)(planet->concentration[axis] +
            input->random(input->context, 50) + 50);
        if (severity == 3)
            value = (StarsI16)(value + input->random(input->context, 15) + 15);
        if (value > 200)
            value = 200;
        planet->concentration[axis] = (StarsU8)value;
    }
    for (axis = 0; axis < 3; ++axis)
        planet->minerals[axis] += impact[axis] >> 4;
    for (index = 0; index <= severity && index < 3; ++index) {
        delta = (StarsI16)(input->random(input->context, 3) + 3);
        if (severity == 3)
            delta = (StarsI16)(delta + input->random(input->context, 3) + 3);
        if (input->random(input->context, 2) != 0)
            delta = (StarsI16)-delta;
        axis = index;
        value = (StarsI16)(planet->environment[axis] + delta);
        if (value < 1) value = 1;
        else if (value > 99) value = 99;
        planet->environment[axis] = (signed char)value;
        value = (StarsI16)(planet->original_environment[axis] + delta);
        if (value < 1) value = 1;
        else if (value > 99) value = 99;
        planet->original_environment[axis] = (signed char)value;
    }
    if (input->toss_non_auto_build_items)
        input->toss_non_auto_build_items(input->context, planet);
}
