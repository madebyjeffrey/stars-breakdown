#include "core.h"
#include "common.h"

StarsU16 stars_word(const StarsU8 *data, StarsU16 offset)
{
    return (StarsU16)(data[offset] | ((StarsU16)data[offset + 1U] << 8));
}

void stars_put_word(StarsU8 *data, StarsU16 offset, StarsU16 value)
{
    data[offset] = (StarsU8)(value & 0xffU);
    data[offset + 1U] = (StarsU8)(value >> 8);
}

StarsI16 stars_signed_word(StarsU16 value)
{
    return (StarsI16)value;
}

double stars_get_distance(StarsI16 x1, StarsI16 y1, StarsI16 x2,
    StarsI16 y2)
{
    (void)x1; (void)y1; (void)x2; (void)y2;
    return 0.0;
}

void stars_expand_rect(StarsRect *rect, StarsI16 dx, StarsI16 dy)
{
    (void)rect; (void)dx; (void)dy;
}

void stars_offset_rect(StarsRect *rect, StarsI16 dx, StarsI16 dy)
{
    (void)rect; (void)dx; (void)dy;
}
