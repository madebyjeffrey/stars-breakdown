#include "core.h"

/* Genuine C reconstruction of FEnumCalcArmadaDest, NE 18:3286-3405. */
StarsI16 stars_enum_calc_armada_dest(const StarsArmadaPlanet *source,
    const StarsArmadaPlanet *test, const StarsArmadaDestinationInput *input)
{
    StarsU8 score;
    StarsI32 distance;
    StarsPoint source_point;
    StarsPoint test_point;

    if (!source || !test || !input || !input->ai_planets ||
        !input->plan_points || !input->distance)
        return 0;
    if (source == test && source->segment == test->segment)
        return 0;
    score = input->ai_planets[(StarsU16)(test->id * 0x10 + 10)];
    if (score == 0)
        return 0;
    source_point = input->plan_points[source->id];
    test_point = input->plan_points[test->id];
    distance = input->distance(input->context, source_point, test_point);
    if (distance < 0x9c4L)
        score = (StarsU8)(score + 7U);
    else if (distance < 10000L)
        score = (StarsU8)(score + 5U);
    else if (distance < 0x57e4L)
        score = (StarsU8)(score + 4U);
    else if (distance < 40000L)
        score = (StarsU8)(score + 3U);
    else if (distance < 90000L)
        score = (StarsU8)(score + 2U);
    else if (distance < 250000L)
        score = (StarsU8)(score + 1U);
    if ((score & 0x80U) == 0U ||
        (input->random && input->random(input->context, 4) == 0))
        return (StarsI16)score;
    return 0;
}
