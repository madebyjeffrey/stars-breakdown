#ifndef STARS_UI_INTERNAL_H
#define STARS_UI_INTERNAL_H
#include "runtime_internal.h"
#include "raceui.h"
StarsI16 stars_half(StarsU16);
StarsRect stars_rect(StarsU16, StarsU16, StarsU16, StarsU16);
int stars_point_in_rect(const StarsRect *, StarsPoint);
StarsU32 stars_text_extent(StarsRuntime *, StarsU16, const StarsU8 *, StarsU16);
void stars_text_out(StarsRuntime *, StarsU16, StarsU16, StarsU16, const StarsU8 *, StarsU16);
void stars_pat_blt(StarsRuntime *, StarsU16, StarsU16, StarsU16, StarsU16, StarsU16, StarsU32);
void stars_window_position(StarsRuntime *, StarsU16, StarsU16, StarsU16, StarsU16, StarsU16, StarsU16);
StarsRect stars_read_rect(const StarsU8 *, StarsU16);
void stars_write_rect(StarsU8 *, StarsU16, const StarsRect *);
StarsU16 stars_select(StarsRuntime *, StarsU16, StarsU16);
void stars_fill_rect(StarsRuntime *, StarsU16, const StarsRect *, StarsU16);
void stars_screen_point(StarsRuntime *, StarsU16, void *);
StarsI16 stars_signed_byte(StarsU8);
StarsRect stars_race_rect(StarsRuntime *, StarsU16);
void stars_set_race_rect(StarsRuntime *, StarsU16, StarsRect);
#endif
