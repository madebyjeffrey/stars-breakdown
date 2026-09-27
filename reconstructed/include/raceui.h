#ifndef STARS_RACE_UI_H
#define STARS_RACE_UI_H
#include "runtime.h"
typedef struct StarsButtonTrack {
    StarsU16 window, dc;
    StarsRect rect;
    StarsU16 delay, type, text;
    StarsU8 ticks[4];
    StarsU16 flags;
} StarsButtonTrack;
typedef char stars_button_track_size[sizeof(StarsButtonTrack) == 24 ? 1 : -1];
typedef struct StarsProgressState {
    StarsU16 window;
    StarsI16 percent;
} StarsProgressState;
typedef void (*StarsProgressDraw)(void *, StarsI16, StarsI16);
typedef StarsU16 (*StarsProgressCreate)(void *);
typedef void (*StarsProgressDestroy)(void *, StarsU16);
typedef void (*StarsGaugeFrame)(void *, StarsU16, const StarsRect *, StarsU16);
typedef void (*StarsGaugeFill)(void *, StarsU16, const StarsRect *, StarsU16);
typedef struct StarsProgressDialogOps {
    void (*paint)(void *, StarsU16, StarsI16);
    void (*erase)(void *, StarsU16, StarsU16);
    StarsU16 (*color)(void *, StarsU16, StarsU16);
    void (*init)(void *, StarsU16, StarsI16);
} StarsProgressDialogOps;
typedef void (*StarsProgressGaugePaint)(void *, const StarsRect *, StarsU16);
typedef struct StarsFrameResizeInfo {
    StarsI16 x_top;
    StarsI16 y1;
    StarsI16 y2;
} StarsFrameResizeInfo;
typedef StarsU16 (*StarsGetMenu)(void *, StarsU16);
typedef StarsU16 (*StarsGetSubMenu)(void *, StarsU16, StarsI16);
typedef StarsU32 (*StarsFindPlanetFromId)(void *, StarsI16);
typedef const StarsU8 *(*StarsFindMemory)(void *, StarsU32);
typedef void (*StarsSelectAdjacent)(void *, StarsU16, StarsU16);
typedef struct StarsFindSomethingOps {
    StarsFindPlanetFromId planet_from_id;
    StarsFindMemory memory;
    StarsSelectAdjacent select_planet;
    StarsSelectAdjacent select_fleet;
} StarsFindSomethingOps;
typedef StarsI16 (*StarsFindTurnsOutstanding)(void *);
typedef void (*StarsDiagonalMeasure)(void *, StarsI16, const StarsU8 *,
    StarsI16, StarsI16 *, StarsI16 *);
typedef void (*StarsDiagonalDraw)(void *, StarsI16, StarsI16, StarsI16,
    StarsI16, StarsI16, StarsI16, const StarsU8 *, StarsI16);
void stars_sticky_dialog(StarsRuntime *, StarsU16, StarsPoint *, StarsI16);
void stars_draw_frame(StarsRuntime *, StarsU16, const StarsRect *, StarsI16);
void stars_draw_button(StarsRuntime *, StarsU16, const StarsRect *, StarsU16, StarsI16, StarsU16);
void stars_init_button_track(StarsRuntime *, StarsButtonTrack *, StarsU16, StarsU16,
    const StarsRect *, StarsU16, StarsU16, StarsU16, StarsU16, StarsU16);
StarsI16 stars_track_button(StarsRuntime *, StarsButtonTrack *);
StarsI16 stars_get_mouse_move(StarsRuntime *, StarsPoint *);
StarsI16 stars_get_right_mouse_move(StarsRuntime *, StarsPoint *);
StarsI16 stars_update_progress(StarsProgressState *, StarsI16, StarsU16,
    StarsProgressDraw, void *);
StarsI16 stars_show_progress(StarsProgressState *, StarsProgressCreate,
    StarsProgressDraw, void *);
void stars_hide_progress(StarsProgressState *, StarsProgressDestroy, void *);
StarsI32 stars_draw_gauge(void *, StarsU16, const StarsRect *, StarsU16,
    const StarsI32 *, const StarsU16 *, StarsI32, StarsGaugeFrame,
    StarsGaugeFill, StarsU16, StarsU16);
StarsI16 stars_progress_dialog(StarsProgressState *, StarsU16, StarsU16,
    StarsU16, StarsI16, const StarsProgressDialogOps *, void *);
StarsI16 stars_draw_progress_gauge(const StarsRect *, StarsI16, StarsI16,
    StarsProgressGaugePaint, void *);
StarsU16 stars_hcrs_from_frame_window_pt(StarsI16, StarsI16,
    const StarsFrameResizeInfo *, StarsI16, StarsU16, StarsU16, StarsU16,
    StarsI16 *);
StarsU16 stars_get_a_sub_menu(StarsU16, StarsI16, StarsU16, StarsI16,
    StarsGetMenu, StarsGetSubMenu, void *);
StarsI16 stars_find_something_and_select_it(StarsU8 *,
    const StarsFindSomethingOps *, void *);
typedef StarsU32 (*StarsRestoreFleetFromId)(void *, StarsI16);
typedef StarsU32 (*StarsRestorePlanetFromId)(void *, StarsI16);
typedef StarsI16 (*StarsRestoreNextMessage)(void *, StarsI16);
typedef void (*StarsRestoreSetMessageTitle)(void *, StarsU16);
typedef void (*StarsRestoreInvalidate)(void *, StarsU16, StarsU16);
typedef void (*StarsRestoreFindSelection)(void *);
typedef void (*StarsRestoreAdvanceTutor)(void *);
typedef struct StarsRestoreSelectionOps {
    StarsRestoreFleetFromId fleet_from_id;
    StarsRestorePlanetFromId planet_from_id;
    StarsFindMemory memory;
    StarsSelectAdjacent select_fleet;
    StarsSelectAdjacent select_planet;
    StarsRestoreNextMessage next_message;
    StarsRestoreSetMessageTitle set_message_title;
    StarsRestoreInvalidate invalidate;
    StarsRestoreFindSelection find_selection;
    StarsRestoreAdvanceTutor advance_tutor;
} StarsRestoreSelectionOps;
void stars_restore_selection(StarsU8 *, const StarsRestoreSelectionOps *,
    void *);
typedef StarsU16 (*StarsHostBeginPaint)(void *, StarsU16, StarsU8 *);
typedef void (*StarsHostEndPaint)(void *, StarsU16, StarsU8 *);
typedef void (*StarsHostDrawOptions)(void *, StarsU16, StarsU16, StarsI16);
typedef void (*StarsHostWinHelp)(void *, StarsU16, const StarsU8 *, StarsU16,
    StarsU16);
typedef struct StarsHostOptionsOps {
    StarsHostBeginPaint begin_paint;
    StarsHostEndPaint end_paint;
    StarsHostDrawOptions draw_options;
    StarsGetClientRect get_client_rect;
    StarsFillRect fill_rect;
    StarsSetBackgroundColor set_bk_color;
    StarsEndDialog end_dialog;
    StarsHostWinHelp win_help;
} StarsHostOptionsOps;
StarsI16 stars_host_options_dialog(StarsU8 *, StarsU16, StarsU16, StarsU16,
    StarsU32, const StarsHostOptionsOps *, void *);
typedef StarsI16 (*StarsCheckLogFile)(void *, StarsI16, StarsI16 *);
typedef struct StarsFindTurnsOps {
    StarsCheckLogFile check_log_file;
    StarsTickCount ticks;
} StarsFindTurnsOps;
StarsI16 stars_find_turns_outstanding(StarsU8 *, const StarsFindTurnsOps *,
    void *);
typedef void (*StarsDrawSlotDialog)(void *, StarsU16, StarsU16,
    const StarsRect *, StarsI16);
typedef void (*StarsDrawBuildSelection)(void *, StarsU16, StarsU16, StarsI16);
typedef struct StarsBuildSelectionOps {
    StarsGetClientRect get_client_rect;
    StarsDrawSlotDialog draw_slot;
    StarsDrawBuildSelection draw_component;
} StarsBuildSelectionOps;
void stars_set_build_selection(StarsU8 *, StarsI16,
    const StarsBuildSelectionOps *, void *);
typedef const StarsU8 *(*StarsTitleName)(void *, StarsI16);
typedef const StarsU8 *(*StarsTitleString)(void *, StarsU16);
typedef struct StarsPlanetTitleOps {
    StarsTitleName planet_name;
    StarsTitleName fleet_name;
    StarsTitleString string;
    StarsSetWindowText set_window_text;
} StarsPlanetTitleOps;
void stars_set_planet_title_bar(StarsU8 *, StarsU16,
    const StarsPlanetTitleOps *, void *);
StarsI16 stars_turns_out_safe(StarsU8 *, StarsFindTurnsOutstanding, void *);
void stars_draw_host_options(StarsU16, StarsU16, StarsI16);
StarsI16 stars_diagonal_text_out(const StarsRect *, const StarsU8 *, StarsI16,
    StarsDiagonalMeasure, StarsDiagonalDraw, void *);
StarsU16 stars_printer_font_create(StarsRuntime *, StarsU16, StarsI16,
    StarsI16 *);
void stars_draw_planet_print_dot(StarsRuntime *, StarsU16, StarsI16,
    StarsI16, StarsI16);
StarsI16 stars_dx_stream_text_out(StarsRuntime *, StarsU16, StarsI16 *,
    StarsI16, const StarsU8 *, StarsI16, StarsI16);
StarsI16 stars_string_fits_screen(StarsRuntime *, StarsU16, StarsU16,
    StarsU8 *, StarsI16);
void stars_wrap_text_out(StarsRuntime *, StarsU16, StarsI16 *, StarsI16 *,
    const StarsU8 *, StarsI16, StarsI16, StarsI16, StarsI16 *, StarsI16,
    StarsI16);
void stars_draw_fuzzy_border(StarsRuntime *, StarsU16, const StarsRect *,
    StarsU16, StarsU32, StarsU32);
StarsU16 stars_black_reserved_palette(StarsRuntime *);
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
