#include "core.h"

#define UPDATE_POP_MESSAGE_DECLINE 0x25
#define UPDATE_POP_MESSAGE_LOSS 0x26
#define UPDATE_POP_MESSAGE_EMPTY 0x23
#define UPDATE_POP_MESSAGE_RELEASED 0x40
#define UPDATE_POP_RACE_STAT 0x0e

/* NE 18:50A0-532C. Apply one population tick to every active planet, report
 * a negative change using the original fixed-point split, and uninhabit
 * planets whose population and reserve have both reached zero. */
void stars_update_populations(StarsUpdatePopulationInput *input)
{
    StarsU16 index;
    StarsUpdatePopulationPlanet *planet;
    StarsI32 change;
    StarsI16 desirability;
    StarsI16 message;
    StarsI16 race_stat;
    StarsI16 low_change;
    StarsI16 high_change;

    if (!input || !input->planets)
        return;
    for (index = 0; index < input->planet_count; ++index) {
        planet = &input->planets[index];
        change = 0L;
        if (planet->player != -1 &&
                (planet->population_word != 0L || planet->reserve != 0L) &&
                input->ops.change_population)
            change = input->ops.change_population(input->context, planet, 1);
        if (planet->player != -1 && change != 0L && change < 0x10000L &&
                change < 0L && planet->reserve > -1L &&
                (planet->reserve > 0L || planet->population_word != 0L)) {
            low_change = (StarsI16)change;
            high_change = (StarsI16)(change >> 16);
            desirability = input->ops.desirability ?
                input->ops.desirability(input->context, planet) : 0;
            if (desirability < 0) {
                if (input->ops.send_message)
                    input->ops.send_message(input->context, planet->player,
                        UPDATE_POP_MESSAGE_DECLINE,
                        (StarsI16)(planet->population_word - low_change),
                        (StarsI16)(planet->reserve - high_change));
            } else if (input->ops.send_message) {
                input->ops.send_message(input->context, planet->player,
                    UPDATE_POP_MESSAGE_LOSS, (StarsI16)(-low_change),
                    (StarsI16)(-high_change));
            }
        }
        if (planet->player != -1 && planet->population_word == 0L &&
                planet->reserve == 0L) {
            race_stat = input->ops.race_stat ?
                input->ops.race_stat(input->context, planet->player,
                    UPDATE_POP_RACE_STAT) : 0;
            message = change < 0L ? UPDATE_POP_MESSAGE_EMPTY :
                UPDATE_POP_MESSAGE_RELEASED;
            message = (StarsI16)(message + (race_stat == 8));
            if (input->ops.send_message)
                input->ops.send_message(input->context, planet->player,
                    message, 0, 0);
            if (input->ops.uninhabit)
                input->ops.uninhabit(input->context, planet);
        } else if (planet->player == -1 && input->ops.uninhabit)
            input->ops.uninhabit(input->context, planet);
    }
}
