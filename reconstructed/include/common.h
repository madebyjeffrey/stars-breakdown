#ifndef STARS_COMMON_H
#define STARS_COMMON_H
#include "core.h"
/* Near pointers are offsets in the complete DGROUP image. */
StarsU16 stars_number_int(StarsU8 *, StarsI16, StarsU16);
StarsU16 stars_number_long(StarsU8 *, StarsI32, StarsU16);
StarsU16 stars_number_scaled(StarsU8 *, StarsI32, StarsU16);
StarsU16 stars_number_commas(StarsU8 *, StarsU16, StarsI32);
void stars_bound_points(StarsU8 *, StarsU16, StarsU16, StarsI16);
StarsU16 stars_pack_long(StarsU32);
#endif
