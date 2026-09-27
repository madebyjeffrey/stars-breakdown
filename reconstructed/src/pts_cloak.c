#include "core.h"

/* NE 11:3170-3376. Convert a hull-special descriptor into cloak points. */
StarsI16 stars_cpts_cloak_from_lphs(const StarsCptsCloakInput *input)
{
    StarsI16 points;
    StarsU16 subtype;
    StarsU16 count;

    if (!input)
        return 0;
    subtype = (StarsU16)(input->packed_count_type & 0xffU);
    count = (StarsU16)(input->packed_count_type >> 8);
    points = 0;
    if (count != 0U) {
        if (input->group == 1U && subtype == 8U)
            points = 0x14;
        else if (input->group == 2U && subtype == 6U)
            points = 0x28;
        else if (input->group == 4U && subtype == 4U)
            points = 0x46;
        else if (input->group == 4U && subtype == 6U)
            points = 0x14;
        else if (input->group == 8U && subtype == 7U)
            points = 0x32;
        else if (input->group == 8U && subtype == 9U)
            points = 0x28;
        else if (input->group == 0x10U && subtype == 0x12U)
            points = 0x14;
        else if (input->group == 0x80U && subtype == 6U)
            points = 0x3c;
        else if (input->group == 0x80U && subtype == 7U)
            points = 0x32;
        else if (input->group == 0x800U && subtype < 5U &&
                input->part_points)
            points = input->part_points(input->context, input->group,
                subtype);
        else if (input->group == 0x1000U && subtype == 4U)
            points = 0x14;
        if (count > 1U)
            points = (StarsI16)(points * count);
    }
    return points;
}
