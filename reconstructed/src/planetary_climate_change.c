#include "core.h"

static StarsI16 climate_random(StarsPlanetaryClimateInput *input,
    StarsI16 bound)
{
    if (!input->random || bound <= 0)
        return 0;
    return input->random(input->context, bound);
}

static StarsI16 clamp_climate(StarsI16 value)
{
    if (value < 1)
        return 1;
    if (value > 99)
        return 99;
    return value;
}

/* Genuine C reconstruction of PlanetaryClimateChange, NE 24:5C54-5DCC. */
void stars_planetary_climate_change(StarsPlanetaryClimateInput *input)
{
    StarsClimatePlanet *planet;
    StarsI16 planet_index;
    StarsI16 climate_index;
    StarsI16 amount;
    StarsI16 delta;
    StarsI16 random_value;

    if (!input || !input->planets || input->planet_count == 0U)
        return;
    if (climate_random(input, 20) != 0)
        return;
    planet_index = climate_random(input, (StarsI16)input->planet_count);
    if (planet_index < 0 || planet_index >= (StarsI16)input->planet_count)
        return;
    planet = &input->planets[planet_index];
    if (planet->player != -1 && planet->climate_metric >= 0x33UL &&
            input->turn <= 19)
        return;

    climate_index = climate_random(input, 3);
    if (planet->player != -1 && input->send_message)
        input->send_message(input->context, planet->player, 0x00fdu,
            (StarsU16)planet->id, (StarsU16)planet->id, climate_index);
    amount = climate_random(input, 3) + 3;
    if (amount == 3)
        amount = climate_random(input, 3) + 6;
    random_value = climate_random(input, 2);
    delta = random_value ? (StarsI16)-amount : amount;
    planet->environment[climate_index] = clamp_climate(
        (StarsI16)(planet->environment[climate_index] + delta));
    planet->original_environment[climate_index] = clamp_climate(
        (StarsI16)(planet->original_environment[climate_index] + delta));
    if (input->toss_build_items)
        input->toss_build_items(input->context, planet);
}
