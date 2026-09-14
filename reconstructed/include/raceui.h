#ifndef STARS_RACE_UI_H
#define STARS_RACE_UI_H
#include "runtime.h"
typedef struct StarsPoint { StarsU16 x, y; } StarsPoint;
typedef struct StarsButtonTrack {
    StarsU16 window, dc;
    StarsRect rect;
    StarsU16 delay, type, text;
    StarsU8 ticks[4];
    StarsU16 flags;
} StarsButtonTrack;
typedef char stars_button_track_size[sizeof(StarsButtonTrack) == 24 ? 1 : -1];
void stars_sticky_dialog(StarsRuntime *, StarsU16, StarsPoint *, StarsI16);
void stars_draw_frame(StarsRuntime *, StarsU16, const StarsRect *, StarsI16);
void stars_draw_button(StarsRuntime *, StarsU16, const StarsRect *, StarsU16, StarsI16, StarsU16);
void stars_init_button_track(StarsRuntime *, StarsButtonTrack *, StarsU16, StarsU16,
    const StarsRect *, StarsU16, StarsU16, StarsU16, StarsU16, StarsU16);
StarsI16 stars_track_button(StarsRuntime *, StarsButtonTrack *);
StarsI16 stars_get_mouse_move(StarsRuntime *, StarsPoint *);
StarsU16 stars_dib_num_colors(const StarsU8 *);
StarsU16 stars_palette_size(const StarsU8 *);
StarsI16 stars_dib_blt(StarsRuntime *, StarsU16, StarsI16, StarsI16, StarsI16, StarsI16,
    StarsU16, StarsI16, StarsI16, StarsI16, StarsI16, StarsU32);
void stars_center_text(StarsRuntime *, StarsU16, StarsI16, StarsI16, StarsU16, StarsI16);
void stars_rect_center_text(StarsRuntime *, StarsU16, const StarsRect *, StarsU16, StarsI16);
void stars_right_text(StarsRuntime *, StarsU16, StarsI16, StarsI16, StarsU16, StarsI16, StarsI16);
void stars_invalidate_advantage(StarsRuntime *, StarsU16);
void stars_draw_advantage(StarsRuntime *, StarsU16, const StarsRect *, StarsU16);
void stars_race_title(StarsRuntime *, StarsU16, StarsI16);
StarsI16 stars_race_wizard(StarsRuntime *, StarsU16, StarsI16, StarsI16);
/* CommandHandler's New Race (0x81) entry slice; not the other menu commands. */
StarsI16 stars_new_race(StarsRuntime *, StarsU16);
StarsI16 stars_race_dialog1(StarsRuntime *, StarsU16, StarsU16, StarsU16, StarsU32);
StarsI16 stars_race_dialog2(StarsRuntime *, StarsU16, StarsU16, StarsU16, StarsU32);
StarsI16 stars_race_dialog3(StarsRuntime *, StarsU16, StarsU16, StarsU16, StarsU32);
StarsI16 stars_race_dialog4(StarsRuntime *, StarsU16, StarsU16, StarsU16, StarsU32);
StarsI16 stars_race_dialog5(StarsRuntime *, StarsU16, StarsU16, StarsU16, StarsU32);
StarsI16 stars_race_dialog6(StarsRuntime *, StarsU16, StarsU16, StarsU16, StarsU32);
void stars_draw_race2(StarsRuntime *, StarsU16, StarsU16, StarsU16);
void stars_draw_race3(StarsRuntime *, StarsU16, StarsU16, StarsI16);
StarsI16 stars_race_hit_test(StarsRuntime *, StarsPoint);
StarsI16 stars_track_race2(StarsRuntime *, StarsU16, StarsPoint, StarsU16);
StarsI16 stars_track_race3(StarsRuntime *, StarsU16, StarsPoint, StarsU16);
#endif
