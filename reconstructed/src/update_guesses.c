#include "core.h"

static StarsU16 update_guess_base(StarsU32 value, StarsGuessRandom random,
    void *context)
{
    StarsU32 quarter;
    StarsU32 eighth;
    StarsI32 estimate;
    StarsI16 perturbation;

    quarter = value >> 2;
    eighth = value >> 3;
    perturbation = random ? random(context, (StarsI16)quarter) : 0;
    estimate = (StarsI32)((value - eighth + (StarsI32)perturbation) >> 2);
    if (estimate < 1L)
        estimate = 1L;
    else if (estimate > 0xffaL)
        estimate = 0xffaL;
    return (StarsU16)estimate;
}

/* Genuine C reconstruction of UpdateGuesses, NE 24:532C-55B2. */
void stars_update_guesses(StarsUpdateGuessesInput *input)
{
    StarsU16 index;
    StarsU16 low_guess;
    StarsI16 stat;
    StarsI16 survive;
    StarsI16 high_guess;
    StarsGuessPlanet *planet;

    if (!input || !input->planets)
        return;
    for (index = 0; index < input->planet_count; ++index) {
        planet = &input->planets[index];
        if (planet->mineral_value == 0UL &&
            (planet->mineral_value >> 16) == 0UL) {
            planet->guess_word = 0;
            continue;
        }
        stat = input->race_stat ?
            input->race_stat(input->context, planet->owner, 0x000e) : 0;
        if (stat == 8)
            low_guess = 0;
        else
            low_guess = update_guess_base(planet->mineral_value,
                input->random, input->context);
        planet->guess_word = (StarsU16)((planet->guess_word & 0xf000U) |
            (low_guess & 0x0fffU));
        if ((planet->survival_flags & 0x0fffU) == 0U) {
            planet->guess_word = (StarsU16)(planet->guess_word & 0x0fffU);
        } else {
            survive = input->calc_pct_survive ?
                input->calc_pct_survive(input->context, planet) : 0;
            high_guess = (StarsI16)((104 - survive) / 6);
            if (high_guess < 1)
                high_guess = 1;
            else if (high_guess > 15)
                high_guess = 15;
            planet->guess_word = (StarsU16)((planet->guess_word & 0x0fffU) |
                ((StarsU16)high_guess << 12));
        }
    }
}
