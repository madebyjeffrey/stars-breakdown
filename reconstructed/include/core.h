/* Reviewed semantic core. The state parameter replaces original DGROUP globals;
 * this host-test interface is not the original far-call ABI. C89 compatible. */
#ifndef STARS_CORE_H
#define STARS_CORE_H
#include <limits.h>
typedef unsigned char StarsU8;
typedef unsigned short StarsU16;
typedef signed short StarsI16;
#if UINT_MAX == 0xffffffffU
typedef unsigned int StarsU32;
typedef signed int StarsI32;
#elif ULONG_MAX == 0xffffffffUL
typedef unsigned long StarsU32;
typedef signed long StarsI32;
#else
#error A 32-bit unsigned integer type is required
#endif
typedef char stars_core_byte_width[(UCHAR_MAX == 255) ? 1 : -1];
typedef char stars_core_word_width[(USHRT_MAX == 65535) ? 1 : -1];
typedef struct StarsSeeds { StarsU32 first, second; } StarsSeeds;

typedef void (*StarsFakeTbWndProc)(void *, StarsU16, StarsU16, StarsU16,
    StarsI32);
typedef void (*StarsFakeShowTooltip)(void *, StarsI16);
typedef StarsI32 (*StarsFakeCallWindowProc)(void *, StarsU32, StarsU16,
    StarsU16, StarsU16, StarsI32);
typedef struct StarsFakeProcOps {
    StarsFakeTbWndProc toolbar;
    StarsFakeShowTooltip show_tooltip;
    StarsFakeCallWindowProc call_window;
} StarsFakeProcOps;
StarsI32 stars_fake_combo_proc(StarsU32, StarsU16, StarsU16, StarsU16, StarsI32,
    const StarsFakeProcOps *, void *);
StarsI32 stars_fake_ce_proc(StarsU32, StarsU16, StarsU16, StarsU16, StarsI32,
    const StarsFakeProcOps *, void *);
typedef struct StarsRect StarsRect;
typedef struct StarsPoint { StarsI16 x, y; } StarsPoint;

typedef void (*StarsTbCreateRadar)(void *, StarsU16, StarsI16, StarsI16,
    StarsI16, StarsI16);
typedef void (*StarsTbInitializeRadar)(void *, StarsU16, StarsI16);
typedef void (*StarsTbInstallProxy)(void *, StarsU16);
typedef StarsU16 (*StarsTbBeginPaint)(void *, StarsU16);
typedef void (*StarsTbGetClientRect)(void *, StarsU16, StarsRect *);
typedef void (*StarsTbPatBlt)(void *, StarsU16, StarsI16, StarsI16,
    StarsI16, StarsI16, StarsU32);
typedef void (*StarsTbDrawToolbar)(void *, StarsU16, const StarsRect *);
typedef void (*StarsTbEndPaint)(void *, StarsU16, StarsU16);
typedef void (*StarsTbFillRect)(void *, StarsU16, const StarsRect *, StarsU16);
typedef void (*StarsTbSetColor)(void *, StarsU16, StarsU32);
typedef void (*StarsTbSetCursor)(void *, StarsU16);
typedef void (*StarsTbPostMessage)(void *, StarsU16, StarsU16, StarsU16,
    StarsU32);
typedef void (*StarsTbShowTooltip)(void *, StarsI16, const StarsRect *);
typedef void (*StarsTbMapPoints)(void *, StarsU16, StarsU16, StarsPoint *,
    StarsU16);
typedef StarsI16 (*StarsTbGetMouseMove)(void *, StarsPoint *);
typedef StarsI16 (*StarsTbPointInRect)(void *, const StarsRect *, StarsPoint);
typedef StarsU16 (*StarsTbGetDc)(void *, StarsU16);
typedef void (*StarsTbReleaseDc)(void *, StarsU16, StarsU16);
typedef void (*StarsTbSelectPalette)(void *, StarsU16, StarsU16);
typedef void (*StarsTbRealizePalette)(void *, StarsU16);
typedef void (*StarsTbSetCapture)(void *, StarsU16);
typedef void (*StarsTbReleaseCapture)(void *);
typedef void (*StarsTbExecuteButton)(void *, StarsI16, StarsI16);
typedef StarsI16 (*StarsTbButtonDown)(void *, StarsI16);
typedef void (*StarsTbDrawButton)(void *, StarsU16, StarsPoint, StarsI16,
    StarsI16);
typedef void (*StarsTbTerminateFocus)(void *, StarsI16);
typedef void (*StarsTbMessageBeep)(void *, StarsU16);
typedef StarsI32 (*StarsTbDefaultProc)(void *, StarsU16, StarsU16, StarsU16,
    StarsU32);
typedef struct StarsTbWndOps {
    StarsTbCreateRadar create_radar;
    StarsTbInitializeRadar initialize_radar;
    StarsTbInstallProxy install_combo_proxy;
    StarsTbInstallProxy install_edit_proxy;
    StarsTbBeginPaint begin_paint;
    StarsTbGetClientRect get_client_rect;
    StarsTbPatBlt pat_blt;
    StarsTbDrawToolbar draw_toolbar;
    StarsTbEndPaint end_paint;
    StarsTbFillRect fill_rect;
    StarsTbSetColor set_background_color;
    StarsTbSetCursor set_cursor;
    StarsTbPostMessage post_message;
    StarsTbShowTooltip show_tooltip;
    StarsTbMapPoints map_points;
    StarsTbGetMouseMove get_mouse_move;
    StarsTbPointInRect point_in_rect;
    StarsTbGetDc get_dc;
    StarsTbReleaseDc release_dc;
    StarsTbSelectPalette select_palette;
    StarsTbRealizePalette realize_palette;
    StarsTbSetCapture set_capture;
    StarsTbReleaseCapture release_capture;
    StarsTbExecuteButton execute_button;
    StarsTbButtonDown button_down;
    StarsTbDrawButton draw_button;
    StarsTbTerminateFocus terminate_focus;
    StarsTbMessageBeep message_beep;
    StarsTbDefaultProc default_proc;
} StarsTbWndOps;
typedef struct StarsTbWndState {
    StarsU16 toolbar_window;
    StarsU16 radar_window;
    StarsU16 font;
    StarsU16 hand_cursor;
    StarsU16 button_face_brush;
    StarsU32 button_face_color;
    StarsU16 palette;
    StarsI16 arial8_height;
    StarsI16 radar_view;
    StarsI16 window_layout;
    StarsI16 last_x;
    StarsI16 last_y;
    StarsI16 real_combo_proc;
    StarsI16 real_edit_proc;
    StarsU16 real_combo_segment;
    StarsU16 real_edit_segment;
    const StarsI16 *buttons;
    StarsU16 button_count;
    const StarsTbWndOps *ops;
    void *context;
} StarsTbWndState;
StarsI32 stars_tb_wnd_proc(StarsTbWndState *, StarsU16, StarsU16, StarsU16,
    StarsU32);
typedef StarsU16 (*StarsDrawBitmapSelectObject)(void *, StarsU16, StarsU16);
typedef void (*StarsDrawBitmapPatBlt)(void *, StarsU16, StarsI16, StarsI16,
    StarsI16, StarsI16, StarsU32);
typedef void (*StarsDrawBitmapDibBlt)(void *, StarsU16, StarsI16, StarsI16,
    StarsI16, StarsI16, StarsU16, StarsI16, StarsI16, StarsI16, StarsI16,
    StarsU32);
typedef struct StarsDrawBitmapButtonOps {
    StarsDrawBitmapSelectObject select_object;
    StarsDrawBitmapPatBlt pat_blt;
    StarsDrawBitmapDibBlt dib_blt;
} StarsDrawBitmapButtonOps;
typedef struct StarsDrawBitmapButtonInput {
    StarsU16 hdc;
    StarsPoint point;
    StarsI16 button;
    StarsI16 pressed;
    StarsI16 arial8_height;
    StarsU16 button_face_brush;
    StarsU16 button_shadow_brush;
    StarsU16 button_highlight_brush;
    StarsU16 toolbar_bitmap;
    StarsDrawBitmapButtonOps ops;
    void *context;
} StarsDrawBitmapButtonInput;
void stars_draw_bitmap_button(const StarsDrawBitmapButtonInput *);

/* ExecuteButton presents the three scanner filter popups through one
 * semantic menu boundary.  The callback owns Win16 menu construction and
 * localization; the reconstructed routine owns the mask transitions and
 * scanner invalidation around the user's row choice. */
typedef StarsI16 (*StarsExecuteButtonPopup)(void *, StarsU16, StarsI16,
    StarsI16, StarsU16, StarsU16, StarsI16);
typedef void (*StarsExecuteButtonCursor)(void *, StarsU16, StarsPoint *);
typedef void (*StarsExecuteButtonInvalidate)(void *, StarsU16);
typedef void (*StarsExecuteButtonCommand)(void *, StarsU16);
typedef void (*StarsExecuteButtonAdvanceTutor)(void *);
typedef struct StarsExecuteButtonOps {
    StarsExecuteButtonPopup popup_menu;
    StarsExecuteButtonCursor get_cursor_pos;
    StarsExecuteButtonCursor screen_to_client;
    StarsExecuteButtonInvalidate invalidate;
    StarsExecuteButtonCommand command_handler;
    StarsExecuteButtonAdvanceTutor advance_tutor;
} StarsExecuteButtonOps;
typedef struct StarsExecuteButtonInput {
    StarsU16 game_bits;
    StarsU16 game_bits2;
    StarsU16 grbit_scan;
    StarsU16 grbit_scan_mines;
    StarsU16 grbit_scan_ship;
    StarsU16 grbit_scan_enemy_ship;
    StarsI16 scan_zoom;
    StarsU16 toolbar_window;
    StarsU16 frame_window;
    StarsU16 scanner_window;
    const StarsU16 *ship_definition_flags;
    StarsU16 ship_definition_count;
    StarsExecuteButtonOps ops;
    void *context;
} StarsExecuteButtonInput;
void stars_execute_button(StarsExecuteButtonInput *, StarsI16, StarsI16);

typedef StarsI16 (*StarsAiRandom)(void *, StarsI16);
typedef StarsI16 (*StarsAiRaceStat)(void *, StarsI16, StarsI16);
StarsI16 stars_enum_calc_miner_dest(StarsU32, StarsU32, const StarsU8 *,
    StarsU16, StarsAiRandom, void *);
StarsI16 stars_enum_calc_colonist_drop(StarsU32, StarsU32, const StarsU8 *,
    StarsU16, StarsI16, StarsAiRaceStat, void *);

/* The pickup enumerator only consumes these fields from the 56-byte planet
 * record.  Keeping the offsets explicit preserves the original layout while
 * giving the reconstructed routine ordinary typed C fields. */
typedef struct StarsEnumPickupPlanet {
    StarsU16 id;
    StarsI16 player;
    StarsU16 overlap_flags;
    StarsU8 reserved[34];
    StarsU16 mineral_threshold;
    StarsI16 pickup_count;
    StarsU8 tail[12];
} StarsEnumPickupPlanet;
StarsI16 stars_enum_pick_up(const StarsEnumPickupPlanet *,
    const StarsEnumPickupPlanet *, StarsI16);
typedef struct StarsAiMineThing {
    StarsU8 raw[18];
} StarsAiMineThing;
typedef char stars_ai_mine_thing_size[(sizeof(StarsAiMineThing) == 18) ? 1 : -1];
void stars_increase_ai_minefield_sizes(StarsAiMineThing *, StarsU16);
typedef StarsI32 (*StarsAiChangeCargo)(void *, StarsI16, StarsI16, StarsI16,
    StarsI32);
StarsI16 stars_xfer_ai_troopers(StarsI16, StarsI16, StarsI16,
    StarsAiChangeCargo, void *);
StarsI16 stars_xfer_ai_supply(StarsI16, StarsI16, StarsI16, StarsI16,
    StarsI16, StarsI16, StarsAiChangeCargo, void *);
typedef const StarsU8 *(*StarsLookupAiFleetHull)(void *, StarsI16, StarsU16);
StarsU32 stars_ul_fleet_power(const StarsU8 *, StarsLookupAiFleetHull, void *);
typedef void (*StarsChangeAiMainObject)(void *, StarsI16, StarsI16);
typedef StarsI16 (*StarsPlanetDesirability)(void *, const StarsU8 *, StarsI16);
typedef void (*StarsInitProduction)(void *, void *);
typedef StarsI16 (*StarsLatestStarbase)(void *);
StarsI16 stars_i_build_cyber_starbase(const StarsU8 *, StarsI16,
    StarsPlanetDesirability, StarsChangeAiMainObject, StarsInitProduction,
    StarsLatestStarbase, StarsLatestStarbase, void *);
typedef StarsU16 (*StarsFindIdealWarp)(void *, const StarsU8 *, StarsI16);
typedef void (*StarsKillQueuedShips)(void *, const StarsU8 *);
typedef void (*StarsKillQueuedMassPackets)(void *, const StarsU8 *);
typedef struct StarsDestroyIshdefSbOps {
    StarsKillQueuedShips kill_ships;
    StarsKillQueuedMassPackets kill_mass_packets;
} StarsDestroyIshdefSbOps;
void stars_destroy_all_ishdef_sb(StarsU8 *, StarsU16, StarsI16, StarsI16,
    const StarsDestroyIshdefSbOps *, void *);

typedef StarsI16 (*StarsEnsureMaidAi)(void *, StarsI16);
typedef void (*StarsHandleMaidAiTasks)(void *, StarsI16, void *, StarsI16,
    StarsI32 *, StarsI32 *);
typedef void (*StarsFillMaidProduction)(void *);
typedef struct StarsMaidAiOps {
    StarsEnsureMaidAi ensure_ai;
    StarsHandleMaidAiTasks handle_tasks;
    StarsFillMaidProduction fill_queue;
} StarsMaidAiOps;
void stars_do_maid_ai_turn(StarsI16, void *, StarsI16 *,
    const StarsMaidAiOps *, void *);
StarsI16 stars_potent_is_war_fleet(const StarsU16 *, StarsI16, StarsU16);
/* A player record is a complete byte span; these routines read only the
 * recorded fields at offsets 0x7D, 0x110, 0x35A, and 0x3ED..0x3EF. */
StarsI16 stars_ishdef_ai_sb_latest_of(const StarsU8 *);
StarsI16 stars_ishdef_ai_sb_latest(const StarsU8 *);
StarsI16 stars_is_turin_drone_ai_attack(const StarsI16 *, const StarsI16 *);
StarsI16 stars_dx_of_btn(StarsI16, StarsI16);

typedef StarsU16 (*StarsGetDialogItem)(void *, StarsU16, StarsU16);
typedef void (*StarsEnableDialogWindow)(void *, StarsU16, StarsU16);
typedef struct StarsZipButtonOps {
    StarsGetDialogItem get_item;
    StarsEnableDialogWindow enable;
} StarsZipButtonOps;
void stars_enable_zip_prod_buttons(StarsU16, StarsI16,
    const StarsU8 *, StarsU16, const StarsZipButtonOps *, void *);
typedef void (*StarsSetFocusWindow)(void *, StarsU16);
typedef struct StarsEnableVcrButtonOps {
    StarsGetDialogItem get_item;
    StarsEnableDialogWindow enable;
    StarsSetFocusWindow set_focus;
} StarsEnableVcrButtonOps;
void stars_enable_vcr_buttons(StarsU16, StarsI16, StarsI16,
    const StarsEnableVcrButtonOps *, void *);
typedef StarsU32 (*StarsGetMineralColor)(void *, StarsI16);
typedef void (*StarsSetMineralTextColor)(void *, StarsU16, StarsU32);
typedef const StarsU8 *(*StarsFormatMineralLong)(void *, StarsI32, StarsI16 *);
typedef void (*StarsMineralRightText)(void *, StarsU16, StarsI16, StarsI16,
    const StarsU8 *, StarsI16, StarsI16);
typedef struct StarsDrawMineralItemOps {
    StarsGetMineralColor get_color;
    StarsSetMineralTextColor set_text_color;
    StarsFormatMineralLong format_long;
    StarsMineralRightText right_text;
    const StarsU8 *double_dash;
} StarsDrawMineralItemOps;
void stars_draw_mineral_item(StarsU16, StarsI16, StarsI16, StarsI16,
    StarsI32, const StarsDrawMineralItemOps *, void *);

typedef struct StarsScanSelection {
    StarsI16 x;
    StarsI16 y;
    StarsI16 grobj;
    StarsI16 grobj_full;
    StarsI16 planet;
    StarsI16 fleet;
    StarsI16 waypoint;
    StarsI16 thing;
} StarsScanSelection;
typedef void (*StarsFindNearestScanObject)(void *, StarsI16, StarsI16,
    StarsI16, StarsScanSelection *);
typedef void (*StarsChangeScanSelection)(void *, const StarsScanSelection *,
    StarsI16);
typedef struct StarsScanWaypointOps {
    StarsFindNearestScanObject find_nearest;
    StarsChangeScanSelection change_selection;
} StarsScanWaypointOps;
StarsI16 stars_set_scan_waypoint(StarsI16, StarsI16, const StarsU8 *, StarsU16,
    const StarsScanWaypointOps *, void *);
StarsI32 stars_fake_edit_proc(StarsU16, StarsU16, StarsU16, StarsI32, StarsU32,
    StarsFakeCallWindowProc, void *);
typedef void (*StarsEstimateMineralsMined)(void *, const StarsU8 *, StarsI32 *,
    StarsI16, StarsI16);
typedef StarsI16 (*StarsGetResourcesAtPlanet)(void *, const StarsU8 *,
    StarsI16);
typedef StarsI16 (*StarsGetResearchPercent)(void *, StarsI16);
typedef struct StarsGetResourcesAvailableOps {
    StarsEstimateMineralsMined estimate_minerals;
    StarsGetResourcesAtPlanet resources_at_planet;
    StarsGetResearchPercent research_percent;
} StarsGetResourcesAvailableOps;
void stars_get_resources_available(const StarsU8 *, StarsI32 *, StarsI16,
    StarsU32, const StarsGetResourcesAvailableOps *, void *);
typedef const StarsU8 *(*StarsGetPlanetProductionQueue)(void *,
    const StarsU8 *, StarsU16 *);
typedef void (*StarsGetProductionCosts)(void *, const StarsU8 *,
    const StarsU8 *, StarsI32 *, StarsI16, StarsI16);
typedef struct StarsGetProdQCostOps {
    StarsGetPlanetProductionQueue get_queue;
    StarsGetProductionCosts get_costs;
} StarsGetProdQCostOps;
void stars_get_prod_q_cost(const StarsU8 *, StarsI32 *, StarsI16,
    const StarsGetProdQCostOps *, void *);
typedef struct StarsMineralScanFleet {
    StarsU32 identity;
    StarsI16 owner;
    StarsPoint point;
} StarsMineralScanFleet;
typedef struct StarsMineralScanThing {
    StarsU32 identity;
    StarsI16 index;
    StarsPoint point;
} StarsMineralScanThing;
typedef StarsI16 (*StarsMineralScanPopup)(void *, StarsU16, StarsI16,
    StarsI16, StarsU16, const StarsU32 *, const StarsU16 *, StarsI16);
typedef void (*StarsMineralScanChange)(void *, const StarsScanSelection *,
    StarsI16);
typedef void (*StarsMineralScanRedraw)(void *, StarsI16, StarsI16);
typedef void (*StarsMineralScanMainSelection)(void *, StarsI16, StarsI16);
typedef struct StarsPopupMineralScanChoicesInput {
    StarsU16 window;
    StarsI16 x;
    StarsI16 y;
    StarsI16 player;
    StarsI16 planet_owner;
    StarsScanSelection selection;
    const StarsMineralScanFleet *fleets;
    StarsU16 fleet_count;
    const StarsMineralScanThing *things;
    StarsU16 thing_count;
    StarsMineralScanPopup popup_menu;
    StarsMineralScanChange change_selection;
    StarsMineralScanRedraw redraw_selection;
    StarsMineralScanMainSelection change_main_selection;
    void *context;
} StarsPopupMineralScanChoicesInput;
void stars_popup_mineral_scan_choices(
    StarsPopupMineralScanChoicesInput *);
typedef StarsI16 (*StarsHtMineOtherAtSelection)(void *);
typedef struct StarsHtMineWindowInput {
    StarsU16 window;
    StarsI16 x;
    StarsI16 y;
    StarsI16 client_left;
    StarsI16 client_top;
    StarsI16 client_right;
    StarsI16 client_bottom;
    StarsI16 font_height;
    StarsScanSelection selection;
    StarsU16 selected_thing_kind;
    StarsI16 planet_found;
    StarsI16 planet_owner;
    StarsU16 planet_flags;
    StarsI16 player;
    StarsHtMineOtherAtSelection other_at_selection;
    void *context;
} StarsHtMineWindowInput;
StarsI16 stars_ht_mine_window(const StarsHtMineWindowInput *);

/* MineWndProc keeps the small mineral-window message loop separate from the
 * Win16 global address map.  The callbacks below are the actual window/GDI
 * services and game actions used by the original procedure. */
typedef struct StarsMineWndThing {
    StarsU32 overlap_0;
    StarsU8 cargo_value;
} StarsMineWndThing;
typedef void (*StarsMineWndGetText)(void *, StarsU16, StarsU8 *, StarsU16);
typedef StarsU16 (*StarsMineWndCreateWindow)(void *, const StarsU8 *,
    const StarsU8 *, StarsU32, StarsI16, StarsI16, StarsI16, StarsI16,
    StarsU16, StarsU16, StarsU16);
typedef StarsI32 (*StarsMineWndSendMessage)(void *, StarsU16, StarsU16,
    StarsU16, StarsU32);
typedef void (*StarsMineWndSetTitleBar)(void *, StarsU16);
typedef StarsU16 (*StarsMineWndBeginPaint)(void *, StarsU16, StarsRect *);
typedef void (*StarsMineWndGetClientRect)(void *, StarsU16, StarsRect *);
typedef void (*StarsMineWndDrawFrame)(void *, StarsU16, const StarsRect *,
    StarsU16);
typedef StarsU32 (*StarsMineWndSetColor)(void *, StarsU16, StarsU32);
typedef StarsI32 (*StarsMineWndTextExtent)(void *, StarsU16, const StarsU8 *,
    StarsI16);
typedef void (*StarsMineWndCenterText)(void *, StarsU16, const StarsRect *,
    const StarsU8 *, StarsI16);
typedef void (*StarsMineWndDrawArrow)(void *, StarsU16, const StarsRect *,
    StarsI16);
typedef void (*StarsMineWndDrawSurvey)(void *, StarsU16, const StarsRect *);
typedef StarsI16 (*StarsMineWndOtherAtSelection)(void *);
typedef void (*StarsMineWndEndPaint)(void *, StarsU16, StarsRect *);
typedef void (*StarsMineWndFillRect)(void *, StarsU16, const StarsRect *,
    StarsU16);
typedef void (*StarsMineWndGetCursorPos)(void *, StarsPoint *);
typedef void (*StarsMineWndScreenToClient)(void *, StarsU16, StarsPoint *);
typedef StarsI16 (*StarsMineWndPointInRect)(void *, const StarsRect *,
    StarsPoint);
typedef StarsI16 (*StarsMineWndHitTest)(void *, StarsU16, StarsI16, StarsI16);
typedef void (*StarsMineWndSetCursor)(void *, StarsU16);
typedef void (*StarsMineWndWriteMem)(void *, StarsU16, StarsU16, StarsU32);
typedef void (*StarsMineWndClick)(void *, StarsI16, StarsI16, StarsU16,
    StarsU16);
typedef StarsI32 (*StarsMineWndDefaultProc)(void *, StarsU16, StarsU16,
    StarsU16, StarsU32);
typedef struct StarsMineWndOps {
    StarsMineWndGetText get_text;
    StarsMineWndCreateWindow create_window;
    StarsMineWndSendMessage send_message;
    StarsMineWndSetTitleBar set_title_bar;
    StarsMineWndBeginPaint begin_paint;
    StarsMineWndGetClientRect get_client_rect;
    StarsMineWndDrawFrame draw_frame;
    StarsMineWndSetColor set_text_color;
    StarsMineWndSetColor set_background_color;
    StarsMineWndTextExtent text_extent;
    StarsMineWndCenterText center_text;
    StarsMineWndDrawArrow draw_arrow;
    StarsMineWndDrawSurvey draw_survey;
    StarsMineWndOtherAtSelection other_at_selection;
    StarsMineWndEndPaint end_paint;
    StarsMineWndFillRect fill_rect;
    StarsMineWndGetCursorPos get_cursor_pos;
    StarsMineWndScreenToClient screen_to_client;
    StarsMineWndPointInRect point_in_rect;
    StarsMineWndHitTest hit_test;
    StarsMineWndSetCursor set_cursor;
    StarsMineWndWriteMem write_mem;
    StarsMineWndClick click;
    StarsMineWndDefaultProc default_proc;
} StarsMineWndOps;
typedef struct StarsMineWndInput {
    StarsU16 combo_window;
    StarsU16 instance;
    StarsU16 font;
    StarsI16 font_height;
    StarsU32 text_color;
    StarsU32 background_color;
    StarsU16 button_face_brush;
    StarsU16 hand_cursor;
    StarsU16 arrow_help_cursor;
    const StarsU8 *button_class;
    const StarsU8 *title;
    StarsMineWndThing *selected_thing;
    StarsI16 selected_thing_index;
    StarsU16 popup_window;
    const StarsMineWndOps *ops;
    void *context;
} StarsMineWndInput;
StarsI32 stars_mine_wnd_proc(StarsMineWndInput *, StarsU16, StarsU16,
    StarsU16, StarsU32);
typedef StarsI16 (*StarsResearchTrackStep)(void *, StarsI16, StarsI16);
typedef void (*StarsResearchShowFutureTech)(void *, StarsI16);
typedef void (*StarsResearchShowTechNote)(void *, StarsI16);
typedef StarsI16 (*StarsResearchRaceFlag)(void *, StarsI16);
typedef StarsI32 (*StarsResearchProjectSpending)(void *, StarsI16);
typedef void (*StarsResearchRedraw)(void *);
typedef struct StarsFTrackResearchDlgInput {
    StarsU16 window;
    StarsI16 x;
    StarsI16 y;
    StarsI16 fkb;
    StarsI16 spin_top_left;
    StarsI16 spin_top_top;
    StarsI16 spin_top_right;
    StarsI16 spin_top_bottom;
    StarsI16 spin_bottom_left;
    StarsI16 spin_bottom_top;
    StarsI16 spin_bottom_right;
    StarsI16 spin_bottom_bottom;
    StarsI16 future_top;
    StarsI16 future_count;
    StarsI16 note_top;
    StarsI16 resource_left;
    StarsI16 resource_right;
    StarsI16 font_height;
    StarsI16 *research_percent;
    StarsI32 *research_budget;
    StarsResearchTrackStep track_step;
    StarsResearchShowFutureTech show_future_tech;
    StarsResearchShowTechNote show_tech_note;
    StarsResearchRaceFlag race_flag;
    StarsResearchProjectSpending project_spending;
    StarsResearchRedraw redraw;
    void *context;
} StarsFTrackResearchDlgInput;
StarsI16 stars_f_track_research_dlg(StarsFTrackResearchDlgInput *);

/* ResearchDlg owns the research choices and the dialog geometry.  Win16
 * controls, localized strings, player storage, and tutor state remain service
 * callbacks; the message routing, packed-tech mapping, layout arithmetic, and
 * commit behavior are ordinary C. */
typedef StarsU16 (*StarsResearchDlgBeginPaint)(void *, StarsU16, StarsRect *);
typedef void (*StarsResearchDlgEndPaint)(void *, StarsU16, StarsRect *);
typedef void (*StarsResearchDlgGetClientRect)(void *, StarsU16, StarsRect *);
typedef void (*StarsResearchDlgGetWindowRect)(void *, StarsU16, StarsRect *);
typedef void (*StarsResearchDlgFillRect)(void *, StarsU16, const StarsRect *,
    StarsU16);
typedef void (*StarsResearchDlgGetCursorPos)(void *, StarsPoint *);
typedef void (*StarsResearchDlgScreenToClient)(void *, StarsU16, StarsPoint *);
typedef void (*StarsResearchDlgSetCursor)(void *, StarsU16);
typedef void (*StarsResearchDlgSetBackground)(void *, StarsU16, StarsU32);
typedef void (*StarsResearchDlgCheckRadio)(void *, StarsU16, StarsU16,
    StarsU16, StarsU16);
typedef StarsU16 (*StarsResearchDlgGetDc)(void *, StarsU16);
typedef StarsU16 (*StarsResearchDlgSelectObject)(void *, StarsU16, StarsU16);
typedef void (*StarsResearchDlgReleaseDc)(void *, StarsU16, StarsU16);
typedef StarsI16 (*StarsResearchDlgGetString)(void *, StarsU16, StarsU8 *,
    StarsU16);
typedef const StarsU8 *(*StarsResearchDlgCompressedString)(void *, StarsU16);
typedef StarsI32 (*StarsResearchDlgTextExtent)(void *, StarsU16,
    const StarsU8 *, StarsI16);
typedef StarsU16 (*StarsResearchDlgGetDialogItem)(void *, StarsU16, StarsU16);
typedef void (*StarsResearchDlgSetWindowText)(void *, StarsU16, const StarsU8 *);
typedef void (*StarsResearchDlgSetWindowPos)(void *, StarsU16, StarsU16,
    StarsI16, StarsI16, StarsI16, StarsI16, StarsU16);
typedef StarsI32 (*StarsResearchDlgSendMessage)(void *, StarsU16, StarsU16,
    StarsU16, StarsU32);
typedef void (*StarsResearchDlgComboAddString)(void *, StarsU16,
    const StarsU8 *);
typedef StarsI16 (*StarsResearchDlgButtonChecked)(void *, StarsU16, StarsU16);
typedef void (*StarsResearchDlgSticky)(void *, StarsU16, StarsPoint *, StarsI16);
typedef void (*StarsResearchDlgEndDialog)(void *, StarsU16, StarsI16);
typedef void (*StarsResearchDlgWinHelp)(void *, StarsU16, const StarsU8 *,
    StarsU16);
typedef void (*StarsResearchDlgDraw)(void *, StarsU16, StarsU16,
    const StarsRect *, StarsI16);
typedef StarsI16 (*StarsResearchDlgTrack)(void *, StarsU16, StarsI16,
    StarsI16, StarsU16);
typedef StarsI16 (*StarsResearchDlgPlanetOwner)(void *, StarsI16);
typedef StarsU32 (*StarsResearchDlgResourcesAtPlanet)(void *, StarsI16,
    StarsI16);
typedef StarsI32 (*StarsResearchDlgProjectedBudget)(void *, StarsI16);
typedef void (*StarsResearchDlgCommit)(void *, StarsI16, StarsI16, StarsI16);
typedef void (*StarsResearchDlgWriteMem)(void *, StarsU16, StarsU16,
    const StarsU8 *, StarsU16);
typedef void (*StarsResearchDlgTutorMark)(void *, StarsU16);
typedef void (*StarsResearchDlgAdvanceTutor)(void *);
typedef struct StarsResearchDlgState {
    StarsI16 pct_research;
    StarsI16 current_technology;
    StarsI16 future_technology;
    StarsI16 original_pct_research;
    StarsI16 original_current_technology;
    StarsI16 original_future_technology;
    StarsI16 future_technology_count;
    StarsI16 y_future_technology;
    StarsI16 y_technology_note;
    StarsI16 dx_res_left;
    StarsI16 dx_res_right;
    StarsI16 dx_res_radio;
    StarsI16 dx_res_string_right;
    StarsI16 arial8_height;
    StarsI16 player_id;
    StarsI16 planet_count;
    StarsI16 tutor_active;
    StarsU16 font_arial8;
    StarsU16 button_face_brush;
    StarsU32 button_face_color;
    StarsU16 help_cursor;
    const StarsU8 *help_file;
    StarsPoint sticky_position;
    StarsI32 resource_total;
    StarsI32 research_budget;
} StarsResearchDlgState;
typedef struct StarsResearchDlgOps {
    StarsResearchDlgBeginPaint begin_paint;
    StarsResearchDlgEndPaint end_paint;
    StarsResearchDlgGetClientRect get_client_rect;
    StarsResearchDlgGetWindowRect get_window_rect;
    StarsResearchDlgFillRect fill_rect;
    StarsResearchDlgGetCursorPos get_cursor_pos;
    StarsResearchDlgScreenToClient screen_to_client;
    StarsResearchDlgSetCursor set_cursor;
    StarsResearchDlgSetBackground set_background_color;
    StarsResearchDlgCheckRadio check_radio_button;
    StarsResearchDlgGetDc get_dc;
    StarsResearchDlgSelectObject select_object;
    StarsResearchDlgReleaseDc release_dc;
    StarsResearchDlgGetString get_string;
    StarsResearchDlgCompressedString get_compressed_string;
    StarsResearchDlgTextExtent get_text_extent;
    StarsResearchDlgGetDialogItem get_dialog_item;
    StarsResearchDlgSetWindowText set_window_text;
    StarsResearchDlgSetWindowPos set_window_pos;
    StarsResearchDlgSendMessage send_message;
    StarsResearchDlgComboAddString combo_add_string;
    StarsResearchDlgButtonChecked button_checked;
    StarsResearchDlgSticky sticky_position;
    StarsResearchDlgEndDialog end_dialog;
    StarsResearchDlgWinHelp win_help;
    StarsResearchDlgDraw draw_dialog;
    StarsResearchDlgTrack track_research;
    StarsResearchDlgPlanetOwner planet_owner;
    StarsResearchDlgResourcesAtPlanet resources_at_planet;
    StarsResearchDlgProjectedBudget projected_budget;
    StarsResearchDlgCommit commit;
    StarsResearchDlgWriteMem write_mem;
    StarsResearchDlgTutorMark tutor_mark;
    StarsResearchDlgAdvanceTutor advance_tutor;
} StarsResearchDlgOps;
typedef struct StarsResearchDlgInput {
    StarsResearchDlgState *state;
    const StarsResearchDlgOps *ops;
    void *context;
} StarsResearchDlgInput;
StarsI16 stars_research_dlg(StarsResearchDlgInput *, StarsU16, StarsU16,
    StarsU16, StarsU32);

typedef struct StarsSelectAdjFleetRecord {
    StarsI16 id;
    StarsI16 player;
    StarsPoint point;
} StarsSelectAdjFleetRecord;
typedef void (*StarsSelectAdjFleetInvalidate)(void *, StarsI16, StarsI16);
typedef void (*StarsSelectAdjFleetChangeScan)(void *, StarsPoint, StarsU16);
typedef void (*StarsSelectAdjFleetRedraw)(void *, StarsI16, StarsI16);
typedef void (*StarsSelectAdjFleetChangeMain)(void *, StarsU16, StarsI16);
typedef void (*StarsSelectAdjFleetCenter)(void *, StarsPoint, StarsI16);
typedef void (*StarsSelectAdjFleetDrawScroll)(void *);
typedef void (*StarsSelectAdjFleetSetTitle)(void *, StarsU16);
typedef void (*StarsSelectAdjFleetSetDropDown)(void *, StarsI16);
typedef struct StarsSelectAdjFleetOps {
    StarsSelectAdjFleetInvalidate invalidate_report;
    StarsSelectAdjFleetChangeScan change_scan_selection;
    StarsSelectAdjFleetRedraw redraw_scan_selection;
    StarsSelectAdjFleetChangeMain change_main_object;
    StarsSelectAdjFleetCenter center_scanner;
    StarsSelectAdjFleetDrawScroll draw_scroll_bar;
    StarsSelectAdjFleetInvalidate invalidate_mine_window;
    StarsSelectAdjFleetSetTitle set_mineral_title;
    StarsSelectAdjFleetSetDropDown set_fleet_dropdown;
} StarsSelectAdjFleetOps;
typedef struct StarsSelectAdjFleetInput {
    const StarsSelectAdjFleetRecord *fleets;
    StarsU16 fleet_count;
    const StarsU16 *player_fleet_order;
    StarsU16 player_fleet_count;
    StarsI16 player;
    StarsU16 report_cached;
    StarsU16 selection_kind;
    StarsI16 selected_fleet_id;
    StarsPoint selected_point;
    StarsU16 mine_window;
    const StarsSelectAdjFleetOps *ops;
    void *context;
} StarsSelectAdjFleetInput;
void stars_select_adj_fleet(StarsI16, StarsI16, StarsSelectAdjFleetInput *);
typedef struct StarsChangeMainPlanet {
    StarsI16 id;
    StarsPoint point;
} StarsChangeMainPlanet;
typedef struct StarsChangeMainFleet {
    StarsI16 id_planet;
    StarsI16 player;
    StarsPoint point;
    StarsI16 planet_slot;
    StarsU16 order_max;
    StarsU16 present;
} StarsChangeMainFleet;
typedef struct StarsChangeMainSelection {
    StarsI16 grobj;
    StarsI16 id;
    StarsI16 grobj_full;
    StarsPoint point;
    StarsI16 planet_id;
    StarsI16 active_waypoint;
    StarsI16 scan_waypoint;
    StarsU16 fleet_overlap;
    StarsChangeMainFleet fleet;
} StarsChangeMainSelection;
typedef StarsI16 (*StarsChangeMainLookupPlanet)(void *, StarsI16,
    StarsChangeMainPlanet *);
typedef StarsI16 (*StarsChangeMainLookupFleet)(void *, StarsI16,
    StarsChangeMainFleet *);
typedef void (*StarsChangeMainCopyFleet)(void *,
    const StarsChangeMainFleet *, StarsChangeMainFleet *);
typedef void (*StarsChangeMainInvalidateReport)(void *, StarsI16, StarsI16);
typedef void (*StarsChangeMainFill)(void *);
typedef void (*StarsChangeMainFillBattle)(void *, StarsI16);
typedef void (*StarsChangeMainFillShip)(void *, StarsI16);
typedef void (*StarsChangeMainSend)(void *, StarsU16, StarsU16, StarsU16,
    StarsU32);
typedef void (*StarsChangeMainShowWindow)(void *, StarsU16, StarsI16);
typedef void (*StarsChangeMainDrawPlanShip)(void *, StarsU16, StarsU16);
typedef void (*StarsChangeMainInvalidateRect)(void *, StarsU16, StarsI16);
typedef void (*StarsChangeMainSetTitle)(void *, StarsU16);
typedef void (*StarsChangeMainAdvanceTutor)(void *);
typedef struct StarsChangeMainObjInput {
    StarsI16 f_ai;
    StarsI16 current_player;
    StarsI16 fleet_count;
    const StarsChangeMainFleet *fleets;
    const StarsPoint *planet_points;
    StarsU16 scan_flags;
    StarsU16 tutorial_flags;
    StarsU16 overlap_flags;
    StarsChangeMainSelection *selection;
    StarsU16 planet_window;
    StarsU16 toolbar_window;
    StarsU16 planet_production_window;
    StarsU16 order_edit_window;
    StarsU16 ship_dropdown_window;
    StarsU16 battle_dropdown_window;
    StarsU16 ship_list_window;
    StarsU16 fleet_component_window;
    StarsU16 report_combo_window;
    const StarsU16 *button_windows;
    const StarsU16 *order_dropdown_windows;
    StarsRect *reference_rects;
    StarsU16 reference_rect_count;
    StarsChangeMainLookupPlanet lookup_planet;
    StarsChangeMainLookupFleet lookup_fleet;
    StarsChangeMainCopyFleet copy_fleet;
    StarsChangeMainInvalidateReport invalidate_report;
    StarsChangeMainFill fill_planet_production;
    StarsChangeMainFill fill_orders;
    StarsChangeMainFill fill_fleet_components;
    StarsChangeMainFillBattle fill_battle;
    StarsChangeMainFillShip fill_ship;
    StarsChangeMainSend send_message;
    StarsChangeMainShowWindow show_window;
    StarsChangeMainDrawPlanShip draw_plan_ship;
    StarsChangeMainInvalidateRect invalidate_rect;
    StarsChangeMainSetTitle set_title;
    StarsChangeMainAdvanceTutor advance_tutor;
    void *context;
} StarsChangeMainObjInput;
void stars_change_main_obj_sel(StarsI16, StarsI16, StarsChangeMainObjInput *);
typedef struct StarsPlanetClickTile {
    StarsI16 top;
    StarsI16 height;
    StarsI16 kind;
    StarsU16 reserved_6;
    StarsU16 reserved_8;
    StarsU16 layout_flags;
    StarsU16 reserved_c;
    StarsU16 reserved_e;
} StarsPlanetClickTile;
typedef struct StarsPlanetClickButton {
    StarsU16 flags;
} StarsPlanetClickButton;
typedef StarsI16 (*StarsPlanetClickPointInRect)(void *, const StarsRect *,
    StarsPoint);
typedef StarsU16 (*StarsPlanetClickGetDc)(void *, StarsU16);
typedef void (*StarsPlanetClickReleaseDc)(void *, StarsU16, StarsU16);
typedef void (*StarsPlanetClickCapture)(void *, StarsU16);
typedef void (*StarsPlanetClickReleaseCapture)(void *);
typedef void (*StarsPlanetClickDrawBorder)(void *, StarsU16, const StarsRect *);
typedef StarsI16 (*StarsPlanetClickMouseMove)(void *, StarsPoint *);
typedef void (*StarsPlanetClickOffsetRect)(void *, StarsRect *, StarsI16,
    StarsI16);
typedef void (*StarsPlanetClickInitButton)(void *, StarsPlanetClickButton *,
    StarsU16, const StarsRect *, StarsU16);
typedef StarsI16 (*StarsPlanetClickTrackButton)(void *,
    StarsPlanetClickButton *);
typedef void (*StarsPlanetClickOrders)(void *, StarsPoint, StarsI16,
    StarsI16, StarsI16);
typedef void (*StarsPlanetClickReflow)(void *, StarsI16, StarsI16, StarsI16);
typedef struct StarsPlanetClickInput {
    StarsI16 selected_kind;
    StarsI16 arial8_height;
    StarsU16 planet_window;
    StarsPlanetClickTile *planet_tiles;
    StarsI16 planet_tile_count;
    StarsPlanetClickTile *fleet_tiles;
    StarsI16 fleet_tile_count;
    StarsPlanetClickPointInRect point_in_rect;
    StarsPlanetClickGetDc get_dc;
    StarsPlanetClickReleaseDc release_dc;
    StarsPlanetClickCapture capture;
    StarsPlanetClickReleaseCapture release_capture;
    StarsPlanetClickDrawBorder draw_border;
    StarsPlanetClickMouseMove get_mouse_move;
    StarsPlanetClickOffsetRect offset_rect;
    StarsPlanetClickInitButton init_button;
    StarsPlanetClickTrackButton track_button;
    StarsPlanetClickOrders click_ship_orders;
    StarsPlanetClickOrders click_planet_orders;
    StarsPlanetClickReflow reflow_column;
    void *context;
} StarsPlanetClickInput;
void stars_planet_click(StarsI16, StarsI16, StarsI16, StarsI16,
    StarsPlanetClickInput *);
typedef struct StarsClickPlanet {
    StarsU16 id;
    StarsU16 owner;
    StarsU16 flags;
    StarsU16 mineral_weight_low;
    StarsU16 mineral_weight_high;
    StarsU16 mine_factory_bits_low;
    StarsU16 mine_factory_bits_high;
    StarsU16 scanner_bits_low;
    StarsU16 scanner_bits_high;
    StarsU16 defense_count;
    StarsU8 population_values[24];
    StarsU8 current_population_value;
    StarsU8 starbase_slot;
    StarsU8 reserved_2b;
    StarsU16 starbase_hull;
    StarsU16 resource_value;
    StarsI16 research_percent;
} StarsClickPlanet;
typedef struct StarsClickPlanetPopup {
    StarsU16 kind;
    StarsU16 planet_id;
    StarsI16 planet_variable;
    StarsI16 planet_value;
    StarsI16 planet_minimum;
    StarsI16 planet_maximum;
    StarsI16 player_value;
    StarsI16 player_minimum;
    StarsI16 player_maximum;
    StarsU8 part[8];
    StarsU32 mineral_values[4];
    const StarsU8 *text;
    void *fleet;
} StarsClickPlanetPopup;
typedef StarsI16 (*StarsClickPlanetPointInRect)(void *, const StarsRect *,
    StarsPoint);
typedef const StarsU8 *(*StarsClickPlanetString)(void *, StarsU16);
typedef void (*StarsClickPlanetInitButton)(void *, StarsPlanetClickButton *,
    StarsU16, const StarsRect *, StarsU16, StarsI16, const StarsU8 *);
typedef StarsI16 (*StarsClickPlanetTrackButton)(void *, StarsPlanetClickButton *);
typedef StarsI16 (*StarsClickPlanetRaceStat)(void *, StarsI16, StarsI16);
typedef StarsI16 (*StarsClickPlanetResources)(void *, const StarsClickPlanet *,
    StarsI16);
typedef StarsI16 (*StarsClickPlanetCapacity)(void *, const StarsClickPlanet *,
    StarsI16, StarsI16);
typedef StarsI16 (*StarsClickPlanetWarpMachine)(void *, const StarsClickPlanet *,
    StarsI16 *);
typedef void (*StarsClickPlanetPartLookup)(void *, StarsU8 *);
typedef void (*StarsClickPlanetShowPopup)(void *, StarsU16, StarsPoint,
    const StarsClickPlanetPopup *);
typedef void (*StarsClickPlanetEstimate)(void *, const StarsClickPlanet *,
    StarsU32[4]);
typedef StarsI16 (*StarsClickPlanetMulDiv)(void *, StarsI16, StarsI16,
    StarsI16);
typedef void (*StarsClickPlanetOrders)(void *, StarsPoint, StarsI16,
    StarsI16, StarsI16);
typedef struct StarsClickPlanetOps {
    StarsClickPlanetPointInRect point_in_rect;
    StarsClickPlanetString get_string;
    StarsClickPlanetInitButton init_button;
    StarsClickPlanetTrackButton track_button;
    StarsClickPlanetRaceStat get_race_stat;
    StarsClickPlanetResources resources_at_planet;
    StarsClickPlanetCapacity max_capacity;
    StarsClickPlanetWarpMachine warp_machine;
    StarsClickPlanetPartLookup lookup_part;
    StarsClickPlanetPartLookup best_defense_part;
    StarsClickPlanetPartLookup best_scanner_part;
    StarsClickPlanetEstimate estimate_minerals;
    StarsClickPlanetMulDiv muldiv;
    StarsClickPlanetOrders click_ship_orders;
    StarsClickPlanetOrders click_planet_orders;
    StarsClickPlanetShowPopup popup;
} StarsClickPlanetOps;
typedef struct StarsClickPlanetInput {
    StarsU16 selected_kind;
    StarsI16 current_player;
    StarsI16 arial8_height;
    StarsU16 planet_window;
    StarsU16 hand_cursor;
    StarsU16 help_cursor;
    StarsRect *reference_rects;
    StarsU16 reference_rect_count;
    StarsClickPlanet *planet;
    StarsClickPlanetPopup popup_state;
    StarsClickPlanetOps ops;
    void *context;
} StarsClickPlanetInput;
StarsU16 stars_click_in_planet_orders(StarsPoint, StarsI16, StarsI16,
    StarsI16, StarsClickPlanetInput *);
struct StarsRect;
typedef StarsI32 (*StarsFleetGaugeGetStat)(void *, const void *, StarsI16);
typedef StarsI16 (*StarsFleetGaugeIdealWarp)(void *, const void *, StarsI16);
typedef void (*StarsFleetGaugeSelectFont)(void *, StarsU16, StarsU16);
typedef StarsI32 (*StarsFleetGaugeDrawGauge)(void *, StarsU16,
    const struct StarsRect *, StarsI16, const StarsI32 *, const StarsU16 *, StarsI32);
typedef StarsU16 (*StarsFleetGaugeSetBkMode)(void *, StarsU16, StarsU16);
typedef StarsI16 (*StarsFleetGaugeGetString)(void *, StarsU16, StarsU8 *);
typedef StarsI16 (*StarsFleetGaugeFormatText)(void *, StarsU8 *, StarsU16,
    StarsU16, StarsI32, StarsI32, StarsI16);
typedef StarsI32 (*StarsFleetGaugeTextExtent)(void *, StarsU16,
    const StarsU8 *, StarsI16);
typedef void (*StarsFleetGaugeCenterText)(void *, StarsU16, const struct StarsRect *,
    const StarsU8 *, StarsI16);
typedef struct StarsDrawFleetGaugeOps {
    StarsFleetGaugeGetStat get_fleet_stat;
    StarsFleetGaugeIdealWarp find_ideal_warp;
    StarsFleetGaugeSelectFont select_font;
    StarsFleetGaugeDrawGauge draw_gauge;
    StarsFleetGaugeSetBkMode set_bk_mode;
    StarsFleetGaugeGetString get_string;
    StarsFleetGaugeFormatText format_text;
    StarsFleetGaugeTextExtent text_extent;
    StarsFleetGaugeCenterText center_text;
} StarsDrawFleetGaugeOps;
typedef struct StarsDrawFleetGaugeInput {
    StarsU16 hdc;
    const struct StarsRect *rect;
    const void *fleet;
    const void *selected_fleet;
    const StarsI32 *mineral_weights;
    StarsU16 active_warp_word;
    StarsI32 active_engine_value;
    StarsI16 ideal_warp_ignore_scoops;
    StarsU16 font_arial8;
    StarsU16 mineral_brushes[5];
    StarsDrawFleetGaugeOps ops;
    void *context;
} StarsDrawFleetGaugeInput;
void stars_draw_fleet_gauge(StarsDrawFleetGaugeInput *, StarsI16);
typedef StarsI16 (*StarsFleetBitmapLookupPrimary)(void *, const void *, StarsI16 *);
typedef StarsU16 (*StarsFleetBitmapSelectObject)(void *, StarsU16, StarsU16);
typedef void (*StarsFleetBitmapPatternBlt)(void *, StarsU16, StarsI16, StarsI16,
    StarsI16, StarsI16, StarsU32);
typedef void (*StarsFleetBitmapSelectPalette)(void *, StarsU16, StarsU16,
    StarsI16);
typedef void (*StarsFleetBitmapRealizePalette)(void *, StarsU16);
typedef void (*StarsFleetBitmapDrawDib)(void *, StarsU16, StarsI16, StarsI16,
    StarsI16, StarsI16, StarsU16, StarsI16, StarsI16, StarsI16, StarsI16,
    StarsU32);
typedef StarsI16 (*StarsFleetBitmapFormatCount)(void *, StarsU8 *, StarsU16,
    StarsI16);
typedef StarsI32 (*StarsFleetBitmapTextExtent)(void *, StarsU16,
    const StarsU8 *, StarsI16);
typedef void (*StarsFleetBitmapTextOut)(void *, StarsU16, StarsI16, StarsI16,
    const StarsU8 *, StarsI16);
typedef void (*StarsFleetBitmapSetTextColor)(void *, StarsU16, StarsU32);
typedef struct StarsDrawFleetBitmapOps {
    StarsFleetBitmapLookupPrimary lookup_primary;
    StarsFleetBitmapSelectObject select_object;
    StarsFleetBitmapPatternBlt pattern_blt;
    StarsFleetBitmapSelectPalette select_palette;
    StarsFleetBitmapRealizePalette realize_palette;
    StarsFleetBitmapDrawDib draw_dib;
    StarsFleetBitmapFormatCount format_count;
    StarsFleetBitmapTextExtent text_extent;
    StarsFleetBitmapTextOut text_out;
    StarsFleetBitmapSetTextColor set_text_color;
} StarsDrawFleetBitmapOps;
typedef struct StarsDrawFleetBitmapInput {
    const void *fleet;
    StarsU16 hdc;
    StarsI16 x;
    StarsI16 y;
    StarsI16 draw_frame;
    StarsI16 bitmap_index;
    StarsI16 differences;
    StarsI16 shrink;
    StarsI16 race_bitmap;
    StarsI16 ship_count;
    StarsU16 brush_shadow;
    StarsU16 brush_highlight;
    StarsU16 palette;
    StarsU16 ship_dibs[5];
    StarsU16 ship_dibs_small[5];
    StarsU16 race_dib_full;
    StarsU16 race_dib_small;
    StarsU16 font_arial7;
    StarsDrawFleetBitmapOps ops;
    void *context;
} StarsDrawFleetBitmapInput;
void stars_draw_fleet_bitmap(StarsDrawFleetBitmapInput *);
typedef StarsI32 (*StarsUpdateOrdersSendMessage)(void *, StarsU16, StarsU16,
    StarsU16, const StarsU8 *);
typedef StarsI32 (*StarsUpdateOrdersFleetStat)(void *, const void *, StarsI16);
typedef StarsI16 (*StarsUpdateOrdersGetString)(void *, StarsU16, StarsU8 *,
    StarsU16);
typedef StarsI16 (*StarsUpdateOrdersGetPlayerName)(void *, StarsI16, StarsU8 *,
    StarsU16);
typedef StarsI16 (*StarsUpdateOrdersFormatText)(void *, StarsU8 *, StarsU16,
    StarsU16, StarsI32, StarsI32, StarsI16);
typedef void (*StarsUpdateOrdersSetWindowText)(void *, StarsU16,
    const StarsU8 *);
typedef struct StarsUpdateOrdersOps {
    StarsUpdateOrdersSendMessage send_message;
    StarsUpdateOrdersFleetStat get_fleet_stat;
    StarsUpdateOrdersGetString get_string;
    StarsUpdateOrdersGetPlayerName get_player_name;
    StarsUpdateOrdersFormatText format_text;
    StarsUpdateOrdersSetWindowText set_window_text;
} StarsUpdateOrdersOps;
typedef struct StarsUpdateOrdersInput {
    StarsU16 order_dropdowns[3];
    StarsU16 order_edit;
    const void *fleet;
    StarsI32 order_values[5];
    StarsU16 active_warp_nibble;
    StarsU16 active_mode6_selection;
    StarsU16 active_mode7_selection;
    StarsU16 active_mode9_selection;
    const StarsU8 *mineral_names[5];
    StarsI16 player_count;
    StarsI16 player_id;
    StarsUpdateOrdersOps ops;
    void *context;
} StarsUpdateOrdersInput;
void stars_update_orders_dds(StarsI16, StarsUpdateOrdersInput *);
typedef void (*StarsGetAiEnumPoint)(void *, const StarsU8 *, StarsPoint *);
typedef StarsI16 (*StarsAiEnumPredicate)(void *, const StarsU8 *,
    const StarsU8 *);
typedef struct StarsAiEnumOps {
    StarsGetAiEnumPoint get_point;
    StarsAiEnumPredicate predicate;
} StarsAiEnumOps;
const StarsU8 *stars_lpfl_find_closest_enum(const StarsU8 *,
    const StarsU8 *const *, const StarsU16 *, StarsU16, StarsU16 *,
    const StarsAiEnumOps *, void *);
const StarsU8 *stars_lppl_find_closest_enum(const StarsU8 *,
    const StarsU8 *const *, StarsU16, const StarsAiEnumOps *, void *);
const StarsU8 *stars_lppl_find_best_enum(const StarsU8 *,
    const StarsU8 *const *, StarsU16, const StarsAiEnumOps *, void *);
StarsI16 stars_id_random_planet_nearby(StarsPoint, StarsI16, StarsI16,
    const StarsPoint *, const StarsU16 *, StarsU16, StarsAiRandom, void *);
typedef struct StarsNeedMineralsPlanet {
    StarsU16 id;
    StarsI16 owner;
    StarsU16 order_flags;
    StarsU16 status_flags;
    StarsI32 minerals[3];
} StarsNeedMineralsPlanet;
typedef StarsI16 (*StarsNeedMineralsWarp)(void *,
    const StarsNeedMineralsPlanet *, StarsI16 *);
typedef StarsI32 (*StarsNeedMineralsDistance)(void *, StarsPoint, StarsPoint);
typedef struct StarsNeedMineralsInput {
    const StarsNeedMineralsPlanet *source;
    const StarsNeedMineralsPlanet *test;
    StarsI16 player;
    const StarsU16 *source_ai_flags;
    const StarsU16 *test_ai_flags;
    const StarsPoint *planet_points;
    StarsU16 planet_count;
    StarsNeedMineralsWarp warp;
    StarsNeedMineralsDistance distance_squared;
    double warp_scale;
    void *context;
} StarsNeedMineralsInput;
StarsI16 stars_f_enum_need_minerals(const StarsNeedMineralsInput *);
struct StarsRect;
typedef StarsU16 (*StarsBrowserBeginPaint)(void *, StarsU16,
    struct StarsRect *);
typedef void (*StarsBrowserGetClientRect)(void *, StarsU16, struct StarsRect *);
typedef void (*StarsBrowserDrawComponent)(void *, StarsU16, StarsU16,
    StarsI16, StarsI16);
typedef void (*StarsBrowserEndPaint)(void *, StarsU16, struct StarsRect *);
typedef void (*StarsBrowserFillRect)(void *, StarsU16,
    const struct StarsRect *,
    StarsU32);
typedef void (*StarsBrowserGetCursorPos)(void *, StarsPoint *);
typedef void (*StarsBrowserScreenToClient)(void *, StarsU16, StarsPoint *);
typedef void (*StarsBrowserCopyHull)(void *, StarsU16, StarsU16);
typedef void (*StarsBrowserNormalizeHull)(void *, StarsU16);
typedef void (*StarsBrowserSetCursor)(void *, StarsU16);
typedef void (*StarsBrowserPopup)(void *, StarsU16, StarsI16, StarsI16);
typedef StarsI32 (*StarsBrowserDefaultProc)(void *, StarsU16, StarsU16,
    StarsU16, StarsU32);
typedef struct StarsBrowserWndOps {
    StarsBrowserBeginPaint begin_paint;
    StarsBrowserGetClientRect get_client_rect;
    StarsBrowserDrawComponent draw_component;
    StarsBrowserEndPaint end_paint;
    StarsBrowserFillRect fill_rect;
    StarsBrowserGetCursorPos get_cursor_pos;
    StarsBrowserScreenToClient screen_to_client;
    StarsBrowserCopyHull copy_hull;
    StarsBrowserNormalizeHull normalize_hull;
    StarsBrowserSetCursor set_cursor;
    StarsBrowserPopup popup;
    StarsBrowserDefaultProc default_proc;
} StarsBrowserWndOps;
typedef struct StarsBrowserWndInput {
    StarsU16 browser_grhst;
    StarsU16 browser_id;
    StarsU16 slot_dialog;
    StarsI16 arial_height;
    StarsU32 button_face_brush;
    StarsU16 help_cursor;
    StarsU16 popup_kind;
    const StarsBrowserWndOps *ops;
    void *context;
} StarsBrowserWndInput;
StarsI32 stars_browser_wnd_proc(StarsBrowserWndInput *, StarsU16, StarsU16,
    StarsU16, StarsU32);
typedef void (*StarsBrowserDlgSticky)(void *, StarsU16, StarsPoint *, StarsI16);
typedef StarsU16 (*StarsBrowserDlgGetSubMenu)(void *, StarsU16, StarsI16);
typedef void (*StarsBrowserDlgCheckMenu)(void *, StarsU16, StarsU16,
    StarsU16);
typedef void (*StarsBrowserDlgSetBkColor)(void *, StarsU16, StarsU32);
typedef void (*StarsBrowserDlgGetClientRect)(void *, StarsU16, StarsRect *);
typedef void (*StarsBrowserDlgFillRect)(void *, StarsU16, const StarsRect *,
    StarsU16);
typedef StarsU16 (*StarsBrowserDlgBeginPaint)(void *, StarsU16, StarsRect *);
typedef void (*StarsBrowserDlgEndPaint)(void *, StarsU16, StarsRect *);
typedef StarsU16 (*StarsBrowserDlgGetMetric)(void *, StarsU16);
typedef void (*StarsBrowserDlgSetWindowPos)(void *, StarsU16, StarsI16,
    StarsI16, StarsI16, StarsI16, StarsU16);
typedef StarsU16 (*StarsBrowserDlgGetDc)(void *, StarsU16);
typedef void (*StarsBrowserDlgSelectObject)(void *, StarsU16, StarsU16);
typedef void (*StarsBrowserDlgReleaseDc)(void *, StarsU16, StarsU16);
typedef StarsI16 (*StarsBrowserDlgGetItemText)(void *, StarsU16, StarsU16,
    StarsU8 *, StarsU16);
typedef StarsI32 (*StarsBrowserDlgTextExtent)(void *, StarsU16,
    const StarsU8 *, StarsI16);
typedef StarsU16 (*StarsBrowserDlgCreateWindow)(void *, const StarsU8 *,
    StarsU32, StarsI16, StarsI16, StarsI16, StarsI16, StarsU16);
typedef const StarsU8 *(*StarsBrowserDlgCompressedString)(void *, StarsU16);
typedef StarsI32 (*StarsBrowserDlgSendMessage)(void *, StarsU16, StarsU16,
    StarsU16, StarsU32);
typedef StarsI16 (*StarsBrowserDlgLookupPart)(void *, StarsU16, StarsU16);
typedef StarsI16 (*StarsBrowserDlgShouldHide)(void *, StarsU16, StarsU16,
    StarsU16);
typedef StarsU16 (*StarsBrowserDlgGroupFromSelection)(void *, StarsI16);
typedef StarsU16 (*StarsBrowserDlgStepGroup)(void *, StarsU16, StarsI16);
typedef StarsI16 (*StarsBrowserDlgButtonChecked)(void *, StarsU16, StarsU16);
typedef void (*StarsBrowserDlgInvalidate)(void *, StarsU16);
typedef void (*StarsBrowserDlgEndDialog)(void *, StarsU16, StarsI16);
typedef void (*StarsBrowserDlgAdvanceTutor)(void *);
typedef StarsI32 (*StarsBrowserDlgDefaultProc)(void *, StarsU16, StarsU16,
    StarsU16, StarsU32);
typedef struct StarsBrowserDlgState {
    StarsPoint sticky_position;
    StarsU16 browser_window;
    StarsU16 browser_child_window;
    StarsU16 frame_window;
    StarsU16 instance;
    StarsU16 font_arial8;
    StarsU16 button_face_brush;
    StarsU32 button_face_color;
    StarsI16 arial8_height;
    StarsI16 arial10_height;
    StarsU16 hull_group;
    StarsU16 item_word;
    StarsU16 component_valid;
    StarsU16 browser_valid;
    StarsU16 game_bits;
} StarsBrowserDlgState;
typedef struct StarsBrowserDlgOps {
    StarsBrowserDlgSticky sticky_position;
    StarsBrowserDlgGetSubMenu get_submenu;
    StarsBrowserDlgCheckMenu check_menu_item;
    StarsBrowserDlgGetClientRect get_client_rect;
    StarsBrowserDlgFillRect fill_rect;
    StarsBrowserDlgSetBkColor set_background_color;
    StarsBrowserDlgGetMetric get_system_metrics;
    StarsBrowserDlgSetWindowPos set_window_pos;
    StarsBrowserDlgGetDc get_dc;
    StarsBrowserDlgSelectObject select_object;
    StarsBrowserDlgReleaseDc release_dc;
    StarsGetDialogItem get_dialog_item;
    StarsBrowserDlgGetItemText get_item_text;
    StarsBrowserDlgTextExtent get_text_extent;
    StarsBrowserDlgCreateWindow create_window;
    StarsBrowserDlgCompressedString get_compressed_string;
    StarsBrowserDlgSendMessage send_message;
    StarsBrowserDlgLookupPart lookup_part;
    StarsBrowserDlgShouldHide should_hide;
    StarsBrowserDlgGroupFromSelection group_from_selection;
    StarsBrowserDlgStepGroup step_group;
    StarsBrowserDlgButtonChecked button_checked;
    StarsBrowserDlgInvalidate invalidate;
    StarsBrowserDlgEndDialog end_dialog;
    StarsBrowserDlgAdvanceTutor advance_tutor;
    StarsBrowserDlgBeginPaint begin_paint;
    StarsBrowserDlgEndPaint end_paint;
    StarsBrowserDlgDefaultProc default_proc;
} StarsBrowserDlgOps;
typedef struct StarsBrowserDlgInput {
    StarsBrowserDlgState *state;
    const StarsBrowserDlgOps *ops;
    void *context;
} StarsBrowserDlgInput;
StarsI16 stars_browser_dlg(StarsBrowserDlgInput *, StarsU16, StarsU16,
    StarsU16, StarsU32);
typedef StarsI16 (*StarsGetAiHullCategory)(void *, StarsI16, StarsU16);
typedef const StarsU8 *(*StarsLookupAiPlanet)(void *, StarsI16);
typedef void (*StarsMarkAiPlanet)(void *, StarsI16);
typedef struct StarsMarkPlanetsUnderAttackOps {
    StarsGetAiHullCategory hull_category;
    StarsLookupAiPlanet lookup_planet;
    StarsMarkAiPlanet mark_planet;
} StarsMarkPlanetsUnderAttackOps;
void stars_mark_planets_under_attack(const StarsU8 *const *, StarsU16,
    StarsI16, const StarsMarkPlanetsUnderAttackOps *, void *);
typedef StarsI16 (*StarsAiBigStarbase)(void *, const StarsU8 *);
StarsI16 stars_idpl_find_closest_starbase(StarsPoint, const StarsU8 *const *,
    const StarsPoint *, StarsU16, StarsI16, StarsI16, StarsAiBigStarbase,
    void *);
typedef StarsI16 (*StarsGetWormholeMovePercent)(void *, const StarsU8 *);
const StarsU8 *stars_lpth_worm_find(StarsPoint, StarsU32, StarsI16,
    const StarsU8 *const *, StarsU16, StarsGetWormholeMovePercent,
    StarsAiRandom, void *);
typedef void (*StarsPrepareUnknownPlanetStatus)(void *, StarsU8 *, StarsU16);
typedef const StarsU8 *(*StarsFindAiWormhole)(void *, StarsPoint, StarsU32);
StarsI16 stars_id_nearest_unknown_planet(StarsPoint, StarsU8 *,
    const StarsPoint *, StarsU16, StarsI16, StarsI16,
    StarsPrepareUnknownPlanetStatus, StarsFindAiWormhole, StarsAiRandom,
    void *, const StarsU8 **);
StarsI16 stars_id_nearest_unknown_planet_strided(StarsPoint, const StarsU8 *,
    StarsU16, StarsU16, const StarsPoint *, StarsU16, StarsI16, StarsI16,
    StarsPrepareUnknownPlanetStatus, StarsFindAiWormhole, StarsAiRandom,
    void *, const StarsU8 **);
StarsI16 stars_itb_from_ppt(StarsPoint *, const StarsI16 *, StarsU16,
    StarsI16);
StarsI16 stars_f_fleet_in_field(StarsPoint, StarsPoint, StarsU32);
typedef StarsI16 (*StarsGetAiDesignWeight)(void *, StarsU16, StarsU16);
typedef StarsI16 (*StarsAiDesignSpecial)(void *, StarsU16, StarsU16);
StarsI16 stars_f_is_ai_transport(const StarsI16 *, const StarsI16 *, StarsU16,
    StarsU16, StarsGetAiDesignWeight, StarsAiDesignSpecial, void *);
StarsI16 stars_f_is_ai_attack(const StarsI16 *, const StarsI16 *, StarsU16,
    StarsU16, StarsGetAiDesignWeight, StarsAiDesignSpecial, void *);
StarsI16 stars_pt_to_scan(StarsI16, StarsI16);
StarsI16 stars_scan_to_pt(StarsI16, StarsI16);
StarsI32 stars_fetch_score_x_val(const StarsU8 *, StarsI16);
typedef void (*StarsSelectPalette)(void *, StarsU16, StarsU16);
typedef void (*StarsRealizePalette)(void *, StarsU16);
typedef StarsI16 (*StarsToolbarButtonDown)(void *, StarsI16);
typedef void (*StarsDrawToolbarButton)(void *, StarsU16, StarsPoint,
    StarsI16, StarsI16);
typedef struct StarsDrawToolbarOps {
    StarsSelectPalette select_palette;
    StarsRealizePalette realize_palette;
    StarsToolbarButtonDown button_down;
    StarsDrawToolbarButton draw_button;
} StarsDrawToolbarOps;
void stars_draw_toolbar(StarsU16, StarsU16, const StarsI16 *, StarsU16,
    StarsI16, const StarsDrawToolbarOps *, void *);
typedef void (*StarsPopupSelectFont)(void *, StarsU16, StarsU16);
typedef void (*StarsPopupWrapText)(void *, StarsU16, StarsI16 *, StarsI16 *,
    const StarsU8 *, StarsI16, StarsI16, StarsI16, StarsI16 *, StarsI16,
    StarsI16);
typedef struct StarsPopupTextOps {
    StarsPopupSelectFont select_font;
    StarsPopupWrapText wrap_text;
} StarsPopupTextOps;
StarsPoint stars_display_string(StarsU16, StarsI16, StarsI16, const StarsU8 *,
    StarsU16, StarsI16, const StarsPopupTextOps *, void *);
typedef struct StarsHullComponent {
    StarsU16 attributes;
    StarsU16 packed_damage;
} StarsHullComponent;
StarsI16 stars_hull_has_teeth(const StarsHullComponent *, StarsU16);
typedef const StarsHullComponent *(*StarsLookupFleetHull)(void *, StarsU16,
    StarsU16, StarsU16 *);
StarsI16 stars_fleet_might_have_teeth(const StarsI16 *, StarsU16,
    StarsLookupFleetHull, void *);
StarsI16 stars_dz_from_brc_brc(StarsU8, StarsU8);
StarsI16 stars_dxy_from_spd_round(StarsU16, StarsI16);
/* Token storage retains the original 29-byte battle-token layout.  The hull
 * lookup callback supplies the selected far-table record and segment
 * selector that the original DGROUP tables would have produced. */
typedef struct StarsToken {
    StarsU8 raw[29];
} StarsToken;
typedef StarsI16 (*StarsBattleRandom)(void *, StarsI16);
void stars_randomize_tok_order(StarsToken *, StarsU16, StarsBattleRandom, void *);
typedef struct StarsWeaponsComponent {
    StarsU16 group;
    StarsU16 packed_count_type;
} StarsWeaponsComponent;
typedef StarsI16 (*StarsWeaponsPartDp)(void *, StarsU16, StarsU16);
typedef StarsU32 (*StarsWeaponsShieldPower)(void *, StarsU16);
typedef struct StarsCheckWeaponsToken {
    StarsU8 init_base;
    StarsU8 grobj;
    StarsU8 pct_jam;
    StarsU8 pct_cap;
    StarsU8 pct_beam_def;
    StarsU8 init_min;
    StarsU8 init_max;
    StarsU16 token_flags;
    StarsU16 dp_shield;
    const StarsWeaponsComponent *components;
    StarsU16 component_count;
} StarsCheckWeaponsToken;
typedef struct StarsCheckWeaponsInput {
    StarsCheckWeaponsToken *token;
    StarsU16 component_count;
    const StarsWeaponsComponent *components;
    StarsI16 *dampening_field;
    StarsU8 *initiative;
    StarsWeaponsPartDp part_dp;
    StarsWeaponsShieldPower shield_power;
    void *context;
} StarsCheckWeaponsInput;
void stars_check_weapons(StarsCheckWeaponsInput *);
typedef struct StarsBoardPlanet {
    StarsI16 id;
    StarsI16 owner;
    StarsI16 is_starbase;
    StarsU16 design_nibble;
    StarsU8 battle_coordinate;
    StarsU16 flags;
} StarsBoardPlanet;
typedef struct StarsBoardFleet {
    StarsI16 id;
    StarsI16 player;
    StarsI16 planet_id;
    StarsU16 flags;
    StarsU16 design_count[16];
    StarsU8 battle_coordinate;
} StarsBoardFleet;
typedef struct StarsBoardToken {
    StarsI16 id;
    StarsI16 player;
    StarsI16 design;
    StarsU16 count;
    StarsU8 object_kind;
    StarsU8 battle_coordinate;
    StarsU16 flags;
    StarsU8 init_min;
    StarsU8 init_max;
    StarsU8 init_base;
    StarsU8 warp;
    StarsU16 dp_shield;
    StarsU16 target_flags;
} StarsBoardToken;
typedef StarsBoardPlanet *(*StarsBoardFindPlanet)(void *, StarsI16);
typedef StarsI16 (*StarsBoardDumpCargo)(void *, StarsBoardFleet *);
typedef void (*StarsBoardCheckInitiative)(void *, StarsBoardToken *);
typedef void (*StarsBoardCheckWeapons)(void *, StarsBoardToken *, StarsI16 *, StarsU8 *);
typedef void (*StarsBoardCheckTarget)(void *, StarsBoardToken *, StarsBoardFleet *, StarsI16);
typedef StarsU8 (*StarsBoardSpeed)(void *, StarsBoardFleet *, StarsI16,
    StarsBoardToken *, StarsI16);
typedef StarsI16 (*StarsBoardRaceFlag)(void *, StarsI16, StarsU16);
typedef void (*StarsBoardRandomize)(void *, StarsBoardToken *, StarsI16);
typedef void (*StarsBoardRecordToken)(void *, const StarsBoardToken *);
typedef struct StarsInitializeBoardInput {
    StarsBoardFleet *fleets;
    StarsI16 fleet_count;
    StarsBoardPlanet *planets;
    StarsI16 planet_count;
    StarsI16 player_count;
    StarsI16 battle_coordinate;
    StarsU16 active_players;
    StarsBoardToken *tokens;
    StarsI16 token_capacity;
    StarsI16 token_count;
    StarsU8 *initiative;
    StarsI16 *initiative_min;
    StarsI16 *initiative_max;
    StarsU16 missed_players;
    StarsBoardFindPlanet find_planet;
    StarsBoardDumpCargo dump_cargo;
    StarsBoardCheckInitiative check_initiative;
    StarsBoardCheckWeapons check_weapons;
    StarsBoardCheckTarget check_target;
    StarsBoardSpeed speed;
    StarsBoardRaceFlag race_flag;
    StarsBoardRandomize randomize;
    StarsBoardRecordToken record_token;
    void *context;
} StarsInitializeBoardInput;
void stars_initialize_board(StarsInitializeBoardInput *);
typedef StarsI16 (*StarsMoveRange)(void *, StarsU16, StarsU8 *);
typedef StarsI32 (*StarsMoveScore)(void *, StarsU16, StarsI16, StarsU16);
typedef StarsI16 (*StarsMoveRandom)(void *, StarsI16);
typedef StarsI16 (*StarsMoveSameSquare)(void *, StarsU16, StarsI16);
typedef struct StarsDxyMoveToken {
    StarsU8 battle_coordinate;
    StarsU8 object_kind;
    StarsU8 movement_flags;
} StarsDxyMoveToken;
typedef struct StarsDxyMoveInput {
    StarsDxyMoveToken *token;
    StarsI16 speed;
    StarsU16 attack_mask;
    StarsMoveRange move_range;
    StarsMoveScore score_damage;
    StarsMoveRandom random;
    StarsMoveSameSquare same_square;
    void *context;
} StarsDxyMoveInput;
StarsI16 stars_dxy_move_tok_to(StarsDxyMoveInput *);
typedef struct StarsCplrBattleFleet {
    StarsI16 player;
    StarsI16 planet_id;
    StarsU16 flags;
    StarsU8 plan_attacker;
    StarsU8 plan_target;
    StarsI16 has_teeth;
    StarsU16 ships[16];
} StarsCplrBattleFleet;
typedef StarsI16 (*StarsCplrRelation)(void *, StarsI16, StarsI16);
typedef StarsI16 (*StarsCplrCountedShip)(void *, StarsI16, StarsI16);
typedef struct StarsCplrBattleInput {
    StarsCplrBattleFleet *fleets;
    StarsI16 fleet_count;
    StarsI16 player_count;
    StarsI16 planet_owner;
    StarsI16 planet_is_starbase;
    StarsI16 planet_has_teeth;
    StarsU8 planet_target;
    StarsU16 *attack_masks;
    StarsU16 *player_mask;
    StarsU16 *spectator_mask;
    StarsCplrRelation relation;
    StarsCplrCountedShip counted_ship;
    void *context;
} StarsCplrBattleInput;
StarsI16 stars_cplr_battle(StarsCplrBattleInput *);
typedef struct StarsDamageEstimateToken {
    StarsU8 battle_coordinate;
    StarsU8 object_kind;
    StarsU8 ship_count;
    StarsU8 range;
    StarsU8 pct_capacity;
    StarsU8 pct_beam_defense;
    StarsU8 pct_battle_computer;
    StarsU16 dp_shield;
    StarsU16 hull_damage;
    StarsU16 battle_computer;
} StarsDamageEstimateToken;
typedef struct StarsDamageEstimateComponent {
    StarsU16 group;
    StarsU16 packed_count_type;
    StarsU16 fuel_max;
    StarsU16 damage_rating;
    StarsU16 flags;
} StarsDamageEstimateComponent;
typedef StarsI16 (*StarsDamageEstimatePartDp)(void *, StarsU16, StarsU16);
typedef StarsI32 (*StarsDamageEstimateTorpHit)(void *, StarsI32,
    const StarsDamageEstimateToken *, StarsI16, StarsI16);
typedef struct StarsDpFromPtokInput {
    StarsDamageEstimateToken *source;
    const StarsDamageEstimateToken *target;
    const StarsDamageEstimateComponent *components;
    StarsU16 component_count;
    StarsU8 source_coordinate;
    StarsU8 target_coordinate;
    StarsI16 proximity;
    StarsDamageEstimatePartDp part_dp;
    StarsDamageEstimateTorpHit torp_hit;
    void *context;
} StarsDpFromPtokInput;
StarsI32 stars_dp_from_ptok_brc_to_brc(StarsDpFromPtokInput *);
typedef struct StarsDamageApplyToken {
    StarsI16 player;
    StarsI16 id;
    StarsI16 design;
    StarsU16 ship_count;
    StarsU16 dp_shield;
    StarsU16 hull_damage;
    StarsU16 battle_computer;
    StarsU16 flags;
    StarsU8 object_kind;
    StarsU8 defense_value;
} StarsDamageApplyToken;
typedef struct StarsDamageApplyFleet {
    StarsI16 player;
    StarsI16 id;
    StarsU16 ship_count;
    StarsU16 flags;
    StarsU16 design_counts[16];
} StarsDamageApplyFleet;
typedef struct StarsDamageApplyPlanet {
    StarsI16 id;
    StarsI16 owner;
    StarsU16 starbase_damage;
    StarsU16 starbase_limit;
    StarsU16 flags;
} StarsDamageApplyPlanet;
typedef StarsDamageApplyPlanet *(*StarsDamageFindPlanet)(void *, StarsI16);
typedef StarsDamageApplyFleet *(*StarsDamageFindFleet)(void *, StarsI16);
typedef StarsI16 (*StarsDamageRaceStat)(void *, StarsI16, StarsU16);
typedef void (*StarsDamageKillShips)(void *, StarsDamageApplyToken *, StarsI16,
    StarsDamageApplyFleet *);
typedef void (*StarsDamageKillQueue)(void *, StarsDamageApplyPlanet *);
typedef void (*StarsDamageRecord)(void *, StarsI16, StarsU16, StarsU32,
    StarsU16);
typedef struct StarsFDamageTokInput {
    StarsDamageApplyToken *token;
    StarsU32 *beam_damage;
    StarsU32 torpedo_damage;
    StarsU16 weapon_flags;
    StarsI16 shields_only;
    StarsU32 *torpedo_packets;
    StarsDamageFindPlanet find_planet;
    StarsDamageFindFleet find_fleet;
    StarsDamageRaceStat race_stat;
    StarsDamageKillShips kill_ships;
    StarsDamageKillQueue kill_queued_ships;
    StarsDamageKillQueue kill_queued_packets;
    StarsDamageRecord record;
    void *context;
} StarsFDamageTokInput;
StarsI16 stars_f_damage_tok(StarsFDamageTokInput *);
typedef const StarsU8 *(*StarsLookupTokenHull)(void *, StarsU16, StarsU16,
    StarsU16, StarsU16 *);
typedef const StarsU8 *(*StarsResolveFarHull)(void *, StarsU16, StarsU16);
const StarsU8 *stars_lpshdef_from_tok(const StarsToken *,
    StarsLookupTokenHull, void *);
const StarsU8 *stars_lpshdef_from_tok_globals(const StarsToken *,
    const StarsU8 *, StarsResolveFarHull, void *);
typedef StarsU32 (*StarsTokenShieldPower)(void *, const StarsU8 *, StarsU16);
void stars_regen_shield(StarsToken *, StarsLookupTokenHull,
    StarsTokenShieldPower, void *);
typedef struct StarsKillShipsToken {
    StarsI16 player;
    StarsI16 ship_count;
    StarsU16 flags;
} StarsKillShipsToken;
typedef struct StarsKillShipsFleet {
    StarsI16 player;
    StarsI16 x;
    StarsI16 y;
    StarsI16 planet_id;
    StarsU16 flags;
    StarsU16 ship_counts[16];
    StarsU32 cargo[4];
} StarsKillShipsFleet;
typedef void (*StarsKillShipsMarkTechs)(void *, const void *, StarsI16);
typedef void (*StarsKillShipsBalanceCargo)(void *, StarsKillShipsFleet *,
    StarsKillShipsFleet *);
typedef void (*StarsKillShipsCreateSalvage)(void *, const StarsKillShipsFleet *);
typedef struct StarsKillShipsOps {
    StarsKillShipsMarkTechs mark_techs;
    StarsKillShipsBalanceCargo balance_cargo;
    StarsKillShipsCreateSalvage create_salvage;
} StarsKillShipsOps;
typedef struct StarsKillShipsInput {
    StarsKillShipsToken *token;
    StarsI16 killed_count;
    StarsI16 hull_index;
    StarsKillShipsFleet *fleet;
    StarsI16 fallout;
    const void *hull_definition;
    StarsKillShipsOps ops;
    void *context;
} StarsKillShipsInput;
void stars_kill_ships(StarsKillShipsInput *);
typedef StarsI16 (*StarsCreateSalvageRaceGrbit)(void *, StarsI16, StarsI16);
typedef StarsI32 (*StarsCreateSalvageOreCost)(void *, StarsI16, StarsI16,
    StarsI16, StarsI16);
typedef void (*StarsCreateSalvageDrop)(void *, const StarsU32 *, StarsI16,
    StarsI16, StarsI16);
typedef void (*StarsCreateSalvageAddPlanetMineral)(void *, void *, StarsI16,
    StarsI32);
typedef struct StarsCreateSalvageOps {
    StarsCreateSalvageRaceGrbit race_grbit;
    StarsCreateSalvageOreCost ore_cost;
    StarsCreateSalvageDrop drop_salvage;
    StarsCreateSalvageAddPlanetMineral add_planet_mineral;
} StarsCreateSalvageOps;
typedef struct StarsCreateSalvageInput {
    const StarsKillShipsFleet *fleet;
    void *planet;
    StarsU16 planet_depleted;
    StarsCreateSalvageOps ops;
    void *context;
} StarsCreateSalvageInput;
void stars_create_salvage(StarsCreateSalvageInput *);
typedef struct StarsUpdatePopulationPlanet {
    StarsI16 player;
    StarsU16 id;
    StarsI32 population_word;
    StarsI32 reserve;
} StarsUpdatePopulationPlanet;
typedef StarsI32 (*StarsUpdatePopulationChange)(void *,
    StarsUpdatePopulationPlanet *, StarsI16);
typedef StarsI16 (*StarsUpdatePopulationDesirability)(void *,
    const StarsUpdatePopulationPlanet *);
typedef void (*StarsUpdatePopulationMessage)(void *, StarsI16, StarsU16,
    StarsI16, StarsI16);
typedef StarsI16 (*StarsUpdatePopulationRaceStat)(void *, StarsI16, StarsI16);
typedef void (*StarsUpdatePopulationUninhabit)(void *,
    StarsUpdatePopulationPlanet *);
typedef struct StarsUpdatePopulationOps {
    StarsUpdatePopulationChange change_population;
    StarsUpdatePopulationDesirability desirability;
    StarsUpdatePopulationMessage send_message;
    StarsUpdatePopulationRaceStat race_stat;
    StarsUpdatePopulationUninhabit uninhabit;
} StarsUpdatePopulationOps;
typedef struct StarsUpdatePopulationInput {
    StarsUpdatePopulationPlanet *planets;
    StarsU16 planet_count;
    StarsUpdatePopulationOps ops;
    void *context;
} StarsUpdatePopulationInput;
void stars_update_populations(StarsUpdatePopulationInput *);
typedef struct StarsAutoTerraformPlanet {
    StarsI16 player;
    StarsU16 id;
    signed char environment[3];
    signed char original_environment[3];
    StarsI32 population;
    StarsI32 reserve;
} StarsAutoTerraformPlanet;
typedef StarsI16 (*StarsAutoTerraformRandom)(void *, StarsI16);
typedef StarsI16 (*StarsAutoTerraformRaceStat)(void *, StarsI16, StarsI16);
typedef StarsI16 (*StarsAutoTerraformRaceTarget)(void *, StarsI16, StarsI16);
typedef StarsI16 (*StarsAutoTerraformCan)(void *,
    const StarsAutoTerraformPlanet *, StarsI16 *, StarsI16 *, StarsI16 *,
    StarsI16);
typedef StarsI16 (*StarsAutoTerraformDesirability)(void *,
    const StarsAutoTerraformPlanet *, StarsI16);
typedef void (*StarsAutoTerraformMessage)(void *, StarsI16, StarsI16,
    StarsU16, StarsI16);
typedef struct StarsAutoTerraformOps {
    StarsAutoTerraformRandom random;
    StarsAutoTerraformRaceStat race_stat;
    StarsAutoTerraformRaceTarget race_target;
    StarsAutoTerraformCan can_terraform;
    StarsAutoTerraformDesirability desirability;
    StarsAutoTerraformMessage send_message;
} StarsAutoTerraformOps;
typedef struct StarsAutoTerraformInput {
    StarsAutoTerraformPlanet *planets;
    StarsU16 planet_count;
    StarsI16 player_count;
    StarsAutoTerraformOps ops;
    void *context;
} StarsAutoTerraformInput;
void stars_auto_terraform(StarsAutoTerraformInput *);
typedef struct StarsRemoteTurnFleet {
    StarsI16 player;
    StarsI16 planet_id;
    StarsU16 object_token;
    StarsU16 object_subtoken;
    StarsU16 status_flags;
} StarsRemoteTurnFleet;
typedef struct StarsRemoteTurnPlanet {
    StarsI16 player;
    StarsU16 id;
    StarsU16 status_flags;
    /* Legacy name: original rgMinConc[3..5], i.e. environment axes. */
    signed char mineral_concentration[3];
} StarsRemoteTurnPlanet;
typedef StarsI32 (*StarsRemoteTurnAmount)(void *, const StarsRemoteTurnFleet *);
typedef StarsI16 (*StarsRemoteTurnFriendly)(void *, StarsI16, StarsI16);
typedef StarsI16 (*StarsRemoteTurnBest)(void *, StarsRemoteTurnPlanet *,
    StarsI16, StarsI16);
typedef StarsI16 (*StarsRemoteTurnDesirability)(void *,
    const StarsRemoteTurnPlanet *, StarsI16);
typedef void (*StarsRemoteTurnMessage)(void *, StarsI16, StarsI16,
    StarsU16, StarsU16, StarsU16, StarsI16, StarsI16);
typedef struct StarsRemoteTurnOps {
    StarsRemoteTurnAmount terraforming_amount;
    StarsRemoteTurnFriendly friendly;
    StarsRemoteTurnBest best_terraform;
    StarsRemoteTurnDesirability desirability;
    StarsRemoteTurnMessage send_message;
} StarsRemoteTurnOps;
typedef struct StarsRemoteTurnInput {
    StarsRemoteTurnFleet *fleets;
    StarsU16 fleet_count;
    StarsRemoteTurnPlanet *planets;
    StarsU16 planet_count;
    StarsRemoteTurnOps ops;
    void *context;
} StarsRemoteTurnInput;
void stars_remote_terraform(StarsRemoteTurnInput *);
typedef struct StarsTransitBreedFleet {
    StarsI16 player;
    StarsU16 id;
    StarsI16 destination;
    StarsU16 status_flags;
    StarsI16 growth_rate;
    StarsU32 colonists;
} StarsTransitBreedFleet;
typedef struct StarsTransitBreedPlanet {
    StarsI16 player;
    StarsI16 id;
    StarsU32 population;
} StarsTransitBreedPlanet;
typedef StarsI16 (*StarsTransitBreedRaceStat)(void *, StarsI16, StarsI16);
typedef StarsI16 (*StarsTransitBreedRandom)(void *, StarsI16);
typedef StarsI32 (*StarsTransitBreedTransfer)(void *,
    StarsTransitBreedFleet *, StarsI32);
typedef StarsTransitBreedPlanet *(*StarsTransitBreedLookup)(void *, StarsI16);
typedef void (*StarsTransitBreedMessage)(void *, StarsI16, StarsI16,
    StarsU16, StarsU16, StarsU16, StarsI16, StarsI16, StarsI16);
typedef struct StarsTransitBreedOps {
    StarsTransitBreedRaceStat race_stat;
    StarsTransitBreedRandom random;
    StarsTransitBreedTransfer transfer_colonists;
    StarsTransitBreedLookup planet_from_id;
    StarsTransitBreedMessage send_message;
} StarsTransitBreedOps;
typedef struct StarsTransitBreedInput {
    StarsTransitBreedFleet *fleets;
    StarsU16 fleet_count;
    StarsI16 player_count;
    StarsTransitBreedOps ops;
    void *context;
} StarsTransitBreedInput;
void stars_breed_colonists_in_transit(StarsTransitBreedInput *);
typedef struct StarsThingDecayFleet {
    StarsI16 x;
    StarsI16 y;
    StarsU16 status_flags;
} StarsThingDecayFleet;
typedef struct StarsThingDecayThing {
    StarsU16 kind;
    StarsU16 packet_flags;
    StarsI16 owner;
    StarsI16 x;
    StarsI16 y;
    StarsI32 radius;
    StarsI32 strength;
    StarsI32 packet_resources[3];
    StarsI16 mine_type;
    StarsU8 active;
} StarsThingDecayThing;
typedef StarsI16 (*StarsThingDecayRaceStat)(void *, StarsI16, StarsI16);
typedef StarsI16 (*StarsThingDecayPacket)(void *, StarsThingDecayThing *,
    StarsI16);
typedef void (*StarsThingDecayTravel)(void *, StarsThingDecayFleet *,
    StarsThingDecayThing *);
typedef StarsI16 (*StarsThingDecayPlanetsInCircle)(void *, StarsI16, StarsI16,
    StarsI32);
typedef void (*StarsThingDecayFree)(void *, StarsThingDecayThing *);
typedef struct StarsThingDecayOps {
    StarsThingDecayRaceStat race_stat;
    StarsThingDecayPacket packet_decay;
    StarsThingDecayTravel travel_through_mines;
    StarsThingDecayPlanetsInCircle planets_in_circle;
    StarsThingDecayFree free_thing;
} StarsThingDecayOps;
typedef struct StarsThingDecayInput {
    StarsThingDecayFleet *fleets;
    StarsU16 fleet_count;
    StarsThingDecayThing *things;
    StarsU16 thing_count;
    StarsThingDecayOps ops;
    void *context;
} StarsThingDecayInput;
void stars_thing_decay(StarsThingDecayInput *);
typedef struct StarsSweepMinesFleet {
    StarsI16 player;
    StarsI16 id;
    StarsI16 x;
    StarsI16 y;
    StarsU16 status_flags;
} StarsSweepMinesFleet;
typedef struct StarsSweepMinesPlanet {
    StarsI16 player;
    StarsI16 id;
    StarsI16 x;
    StarsI16 y;
    StarsI32 sweep_power;
} StarsSweepMinesPlanet;
typedef struct StarsSweepMinesThing {
    StarsU16 kind;
    StarsI16 owner;
    StarsI16 x;
    StarsI16 y;
    StarsI32 detection_radius;
    StarsI32 remaining_strength;
    StarsI16 mine_type;
    StarsU32 owner_mask;
    StarsU8 active;
} StarsSweepMinesThing;
typedef StarsI32 (*StarsSweepMinesFleetPower)(void *,
    const StarsSweepMinesFleet *);
typedef StarsI32 (*StarsSweepMinesPlanetPower)(void *,
    const StarsSweepMinesPlanet *);
typedef StarsI16 (*StarsSweepMinesCanAttack)(void *, StarsI16, StarsI16);
typedef void (*StarsSweepMinesMessage)(void *, StarsI16, StarsI16, StarsI16,
    StarsI32, StarsI16);
typedef void (*StarsSweepMinesFree)(void *, StarsSweepMinesThing *);
typedef struct StarsSweepMinesOps {
    StarsSweepMinesFleetPower fleet_power;
    StarsSweepMinesPlanetPower planet_power;
    StarsSweepMinesCanAttack can_attack;
    StarsSweepMinesMessage send_message;
    StarsSweepMinesFree free_thing;
} StarsSweepMinesOps;
typedef struct StarsSweepMinesInput {
    StarsSweepMinesFleet *fleets;
    StarsU16 fleet_count;
    StarsSweepMinesPlanet *planets;
    StarsU16 planet_count;
    StarsSweepMinesThing *things;
    StarsU16 thing_count;
    StarsSweepMinesOps ops;
    void *context;
} StarsSweepMinesInput;
void stars_sweep_for_mines(StarsSweepMinesInput *);
typedef struct StarsFuelShipDesign {
    StarsI32 fuel_component; /* Anti-Matter Generator count per ship. */
    StarsI16 hull_kind;
} StarsFuelShipDesign;
typedef struct StarsFuelFleet {
    StarsI16 player;
    StarsI16 destination;
    StarsU16 status_flags;
    StarsU16 ship_counts[16];
    StarsFuelShipDesign designs[16];
    StarsI32 fuel;
    StarsI32 base_fuel;
} StarsFuelFleet;
typedef StarsI32 (*StarsFuelCurrent)(void *, const StarsFuelFleet *);
typedef StarsI16 (*StarsFuelCanRefuel)(void *, const StarsFuelFleet *);
typedef void (*StarsFuelSet)(void *, StarsFuelFleet *, StarsI32);
typedef struct StarsFuelOps {
    StarsFuelCurrent current_fuel; /* Maximum fuel capacity, not current cargo. */
    StarsFuelCanRefuel can_refuel;
    StarsFuelSet set_fuel;
} StarsFuelOps;
typedef struct StarsFuelInput {
    StarsFuelFleet *fleets;
    StarsU16 fleet_count;
    StarsFuelOps ops;
    void *context;
} StarsFuelInput;
void stars_fuel_fleets(StarsFuelInput *);

/* Semantic input for EstFuelUse (NE 0B:9FE4-A6FF). A slot carries ship
 * count, design mass, cargo capacity, and the selected engine's fuel table.
 * Legacy member names are retained; these are integer weights/counts.
 * engine_available includes the original required-engine-count check. */
#define STARS_EST_FUEL_SLOTS 16
#define STARS_EST_FUEL_WARPS 12
typedef struct StarsEstFuelSlot {
    StarsU32 cargo_count; /* Number of ships of this design. */
    StarsI32 ship_stat; /* WtMaxShdefStat(grStatCargo), per ship. */
    StarsI32 hull_fuel_weight; /* Current SHDEF mass at offset 40. */
    StarsI32 fuel_used[STARS_EST_FUEL_WARPS];
    StarsI16 engine_available;
    StarsI16 engine_id;
} StarsEstFuelSlot;
typedef StarsI16 (*StarsEstFuelWarpFromOrder)(void *, StarsI16, StarsI16);
typedef StarsI32 (*StarsEstFuelDistance)(void *, StarsI16, StarsI16,
    StarsI16, StarsI16);
typedef struct StarsEstFuelFleet {
    StarsI16 player;
    StarsI16 destination;
    StarsI16 order_count;
    StarsI32 cargo_weight; /* Available fleet fuel, used for range estimates. */
    StarsI32 mineral_weight[4];
    StarsEstFuelSlot slots[STARS_EST_FUEL_SLOTS];
} StarsEstFuelFleet;
typedef struct StarsEstFuelInput {
    StarsEstFuelFleet *fleet;
    StarsI16 order;
    StarsI16 warp;
    StarsI32 travel;
    StarsI16 range_only;
    StarsI16 race_grbit;
    StarsU16 *game_flags;
    StarsEstFuelWarpFromOrder warp_from_order;
    StarsEstFuelDistance distance;
    void *context;
} StarsEstFuelInput;
StarsI32 stars_est_fuel_use(StarsEstFuelInput *);
typedef struct StarsTechLearnPlayer {
    StarsU16 dead_flags;
    StarsU32 trader_flags;
    StarsI16 tech_level[6];
    StarsU32 research[6];
} StarsTechLearnPlayer;
typedef StarsI16 (*StarsTechLearnRandom)(void *, StarsI16);
typedef StarsI16 (*StarsTechLearnGivePart)(void *, StarsU32, StarsI16,
    StarsU16 *);
typedef StarsI32 (*StarsTechLearnCost)(void *, StarsI16, StarsI16, StarsI16);
typedef void (*StarsTechLearnTraderMessage)(void *, StarsI16, StarsI16,
    StarsU16, StarsI16, StarsI16);
typedef void (*StarsTechLearnResearchMessage)(void *, StarsI16, StarsI16,
    StarsI16, StarsI16, StarsI16, StarsI32);
typedef void (*StarsTechLearnMarkEvent)(void *, StarsI16);
typedef struct StarsTechLearnInput {
    StarsTechLearnPlayer *player;
    StarsI16 player_id;
    StarsI16 x;
    StarsI16 y;
    StarsI16 message_id;
    StarsU8 trader_chance[13];
    StarsU8 battle_threshold[6];
    StarsI16 half_cost;
    StarsU16 *goto_id;
    StarsTechLearnRandom random;
    StarsTechLearnGivePart give_trader_part;
    StarsTechLearnCost tech_level_cost;
    StarsTechLearnTraderMessage send_trader_message;
    StarsTechLearnResearchMessage send_research_message;
    StarsTechLearnMarkEvent mark_event;
    void *context;
} StarsTechLearnInput;
StarsI16 stars_itech_learn_a_tech(StarsTechLearnInput *);
typedef struct StarsMysteryTraderThing {
    StarsU16 id;
    StarsU16 flags;
    StarsI16 x;
    StarsI16 y;
    StarsI16 endpoints[2];
    StarsU16 ability;
} StarsMysteryTraderThing;
typedef StarsMysteryTraderThing *(*StarsMysteryTraderAllocate)(void *,
    StarsI16, StarsI16);
typedef StarsI16 (*StarsMysteryTraderRandom)(void *, StarsI16);
typedef void (*StarsMysteryTraderMessage)(void *, StarsI16, StarsI16,
    StarsI16);
typedef struct StarsMysteryTraderInput {
    StarsI16 turn;
    StarsI16 map_size;
    StarsI16 player_count;
    StarsMysteryTraderAllocate allocate_thing;
    StarsMysteryTraderRandom random;
    StarsMysteryTraderMessage send_message;
    void *context;
} StarsMysteryTraderInput;
void stars_mystery_trader(StarsMysteryTraderInput *);
typedef struct StarsMeteorPlanet {
    StarsI16 id;
    StarsI16 owner;
    StarsU32 population;
    StarsU32 minerals[3];
    StarsU8 concentration[3];
    signed char environment[3];
    signed char original_environment[3];
} StarsMeteorPlanet;
typedef StarsI16 (*StarsMeteorRandom)(void *, StarsI16);
typedef StarsI16 (*StarsMeteorRaceStat)(void *, StarsI16, StarsI16);
typedef void (*StarsMeteorMessage)(void *, StarsI16, StarsI16, StarsI16,
    const StarsU32[3]);
typedef void (*StarsMeteorTossProduction)(void *, StarsMeteorPlanet *);
typedef struct StarsMeteorInput {
    StarsI16 turn;
    StarsI16 player_count;
    StarsMeteorPlanet *planets;
    StarsU16 planet_count;
    StarsMeteorRandom random;
    StarsMeteorRaceStat race_stat;
    StarsMeteorMessage send_message;
    StarsMeteorTossProduction toss_non_auto_build_items;
    void *context;
} StarsMeteorInput;
void stars_meteor_strike(StarsMeteorInput *);
typedef struct StarsHealFleet {
    StarsI16 player;
    StarsI16 destination;
    StarsU16 status_flags;
    StarsU16 ship_presence[16];
    StarsU16 packed_ship_condition[16];
    StarsI16 hull_kind[16];
} StarsHealFleet;
typedef struct StarsHealPlanet {
    StarsI16 id;
    StarsI16 owner;
    StarsU16 starbase_damage;
    StarsU16 starbase_present;
    StarsU16 starbase_blocked;
    StarsU16 starbase_has_cargo;
} StarsHealPlanet;
typedef StarsI16 (*StarsHealRaceStat)(void *, StarsI16, StarsI16);
typedef struct StarsHealInput {
    StarsHealFleet *fleets;
    StarsU16 fleet_count;
    StarsHealPlanet *planets;
    StarsU16 planet_count;
    StarsHealRaceStat race_stat;
    void *context;
} StarsHealInput;
void stars_heal_ships(StarsHealInput *);
StarsI16 stars_f_can_kill_tok(const StarsToken *, const StarsToken *,
    StarsLookupTokenHull, void *);
typedef struct StarsHullPart {
    StarsU16 kind;
    StarsU16 packed_count_type;
} StarsHullPart;
StarsI16 stars_hull_has_bombs(const StarsHullPart *, StarsU16);
typedef const StarsHullPart *(*StarsLookupBombHull)(void *, StarsU16,
    StarsU16, StarsU16 *);
StarsI16 stars_fleet_has_bombs(const StarsI16 *, StarsU16,
    StarsLookupBombHull, void *);
typedef StarsU16 (*StarsGetHullType)(void *, StarsU16, StarsU16);
StarsI16 stars_fleet_has_teeth(const StarsI16 *, StarsU16,
    StarsLookupFleetHull, StarsGetHullType, void *);
StarsI16 stars_f_colonizer(const StarsI16 *, StarsU16, StarsGetHullType,
    void *);
StarsI16 stars_f_scout(const StarsI16 *, StarsU16, StarsGetHullType, void *);
StarsI16 stars_potent_rob_war_fleet(const StarsU16 *, StarsI16,
    StarsU16);
typedef struct StarsCheckTargetFleet {
    StarsU16 flags;
    StarsI16 plan;
    StarsI16 ship_design;
} StarsCheckTargetFleet;
typedef StarsU8 *(*StarsCheckTargetShipDefinition)(void *, StarsI16,
    StarsI16);
typedef const StarsU8 *(*StarsCheckTargetBattlePlan)(void *, StarsI16,
    StarsI16);
typedef StarsI16 (*StarsCheckTargetHullPredicate)(void *, const StarsU8 *);
typedef StarsI16 (*StarsCheckTargetMaxStat)(void *, const StarsU8 *, StarsI16);
typedef struct StarsCheckTargetInput {
    StarsCheckTargetShipDefinition ship_definition;
    StarsCheckTargetBattlePlan battle_plan;
    StarsCheckTargetHullPredicate has_teeth;
    StarsCheckTargetHullPredicate has_bombs;
    StarsCheckTargetHullPredicate fuel_tanker;
    StarsCheckTargetMaxStat max_stat;
    void *context;
} StarsCheckTargetInput;
void stars_check_target(StarsToken *, const StarsCheckTargetFleet *,
    const StarsCheckTargetInput *);
typedef struct StarsDeleteBattleFleet {
    StarsI16 player;
    StarsU8 plan;
} StarsDeleteBattleFleet;
typedef StarsDeleteBattleFleet *(*StarsDeleteBattleFleetAt)(void *, StarsI16);
typedef const StarsU8 *(*StarsDeleteFormatIds)(void *, StarsU16);
typedef StarsI16 (*StarsDeleteAlert)(void *, const StarsU8 *, StarsI16);
typedef struct StarsDeleteBattlePlanInput {
    StarsI16 current_player;
    StarsI16 fleet_count;
    StarsDeleteBattleFleetAt fleet_at;
    StarsU8 *plan_count;
    StarsU8 *plans;
    StarsDeleteFormatIds format_ids;
    StarsDeleteAlert alert;
    void *context;
} StarsDeleteBattlePlanInput;
StarsI16 stars_delete_battle_plan(StarsI16, StarsI16,
    StarsDeleteBattlePlanInput *);
typedef struct StarsBattlePlan {
    StarsU16 words[18];
    StarsU8 name[32];
} StarsBattlePlan;
typedef void (*StarsBattlePlansSimple)(void *, StarsU16);
typedef void (*StarsBattlePlansEnable)(void *, StarsU16, StarsI16);
typedef void (*StarsBattlePlansListReset)(void *, StarsU16);
typedef void (*StarsBattlePlansListAdd)(void *, StarsU16, StarsI16,
    const StarsU8 *);
typedef void (*StarsBattlePlansListSelect)(void *, StarsU16, StarsI16);
typedef StarsI16 (*StarsBattlePlansListSelection)(void *, StarsU16);
typedef void (*StarsBattlePlansSetChoice)(void *, StarsU16, StarsI16);
typedef StarsI16 (*StarsBattlePlansGetChoice)(void *, StarsU16);
typedef void (*StarsBattlePlansSetCheck)(void *, StarsU16, StarsI16);
typedef StarsI16 (*StarsBattlePlansGetCheck)(void *, StarsU16);
typedef void (*StarsBattlePlansLog)(void *, const StarsBattlePlan *);
typedef StarsI16 (*StarsBattlePlansDelete)(void *, StarsI16, StarsI16);
typedef StarsI16 (*StarsBattlePlansNewName)(void *, StarsBattlePlan *);
typedef void (*StarsBattlePlansBattleRefresh)(void *, StarsI16);
typedef void (*StarsBattlePlansHelp)(void *, StarsU16, StarsU16);
typedef struct StarsBattlePlansOps {
    StarsBattlePlansSimple sticky_dialog;
    StarsBattlePlansEnable enable_control;
    StarsBattlePlansListReset reset_list;
    StarsBattlePlansListAdd add_list_item;
    StarsBattlePlansListSelect select_list_item;
    StarsBattlePlansListSelection get_list_selection;
    StarsBattlePlansSetChoice set_choice;
    StarsBattlePlansGetChoice get_choice;
    StarsBattlePlansSetCheck set_check;
    StarsBattlePlansGetCheck get_check;
    StarsBattlePlansLog log_change;
    StarsBattlePlansDelete delete_plan;
    StarsBattlePlansNewName new_plan_name;
    StarsBattlePlansBattleRefresh refresh_battle_dropdown;
    StarsBattlePlansSimple advance_tutor;
    StarsBattlePlansHelp win_help;
} StarsBattlePlansOps;
typedef struct StarsBattlePlansInput {
    StarsU16 hwnd;
    StarsI16 player;
    StarsI16 player_count;
    StarsU16 game_flags;
    StarsI16 selected_object_kind;
    StarsI16 selected_plan;
    StarsI16 plan_count;
    StarsBattlePlan *plans;
    StarsBattlePlan edit_plan;
    StarsI16 edit_plan_index;
    StarsI16 dirty;
    StarsU16 list_plans;
    StarsU16 list_attackers;
    StarsU16 list_target_players;
    StarsU16 list_tactics;
    StarsU16 list_friendly_tactics;
    StarsU16 check_manual;
    StarsU16 delete_control;
    StarsU16 new_control;
    StarsU16 button_face_brush;
    StarsU32 button_face_color;
    StarsU16 help_file;
    const StarsBattlePlansOps *ops;
    void *context;
} StarsBattlePlansInput;
StarsI16 stars_battle_plans_dialog(StarsU16, StarsU16, StarsU16,
    StarsI32, StarsBattlePlansInput *);
typedef struct StarsSalvageThing {
    StarsI16 x;
    StarsI16 y;
    StarsU16 flags;
    StarsU16 value;
    StarsU32 minerals[3];
} StarsSalvageThing;
typedef StarsI16 (*StarsSalvageRandom)(void *, StarsI16);
typedef StarsSalvageThing *(*StarsSalvageNewThing)(void *, StarsI16,
    StarsI16);
typedef struct StarsDropSalvageInput {
    StarsSalvageThing **thing;
    StarsU32 minerals[3];
    StarsI16 player;
    StarsPoint point;
    const StarsPoint *plan_points;
    StarsI16 plan_count;
    StarsSalvageRandom random;
    StarsSalvageNewThing new_thing;
    void *context;
} StarsDropSalvageInput;
void stars_drop_salvage(StarsDropSalvageInput *);
typedef struct StarsOrdersPlanet {
    StarsU16 id;
    StarsI16 player;
    StarsU16 flags;
    StarsU8 middle[44];
    StarsU16 turn;
    StarsU8 tail[4];
} StarsOrdersPlanet;
typedef char stars_orders_planet_size[(sizeof(StarsOrdersPlanet) == 56) ? 1 : -1];
typedef void (*StarsOrdersAction)(void *, StarsI16);
typedef void (*StarsOrdersSetBattleId)(void *, StarsU16);
typedef struct StarsDoOrdersInput {
    StarsOrdersPlanet *planets;
    StarsI16 planet_count;
    StarsU16 game_turn;
    StarsOrdersSetBattleId set_battle_id;
    StarsOrdersAction do_battles;
    StarsOrdersAction do_thing_interactions;
    StarsOrdersAction satisfy_orders;
    StarsOrdersAction drop_colonists;
    StarsOrdersAction update_research;
    StarsOrdersAction transfer_to_others;
    void *context;
} StarsDoOrdersInput;
void stars_do_orders(StarsI16, StarsDoOrdersInput *);
typedef struct StarsArmadaPlanet {
    StarsI16 id;
    StarsI16 player;
    StarsU16 segment;
} StarsArmadaPlanet;
typedef StarsI32 (*StarsArmadaDistance)(void *, StarsPoint, StarsPoint);
typedef StarsI16 (*StarsArmadaRandom)(void *, StarsI16);
typedef StarsU16 (*StarsArmadaPlayerFlags)(void *, StarsI16);
typedef struct StarsArmadaDestinationInput {
    const StarsU8 *ai_planets;
    const StarsPoint *plan_points;
    StarsArmadaDistance distance;
    StarsArmadaRandom random;
    StarsArmadaPlayerFlags player_flags;
    void *context;
} StarsArmadaDestinationInput;
StarsI16 stars_enum_calc_armada_dest(const StarsArmadaPlanet *,
    const StarsArmadaPlanet *, const StarsArmadaDestinationInput *);
StarsI16 stars_enum_calc_armada_human_dest(const StarsArmadaPlanet *,
    const StarsArmadaPlanet *, const StarsArmadaDestinationInput *);
typedef const StarsU8 *(*StarsDumpCargoBattlePlan)(void *, StarsI16,
    StarsI16);
typedef StarsU8 *(*StarsDumpCargoPlanet)(void *, StarsI16);
typedef void (*StarsDumpCargoSalvage)(void *, const StarsU8 *, StarsI16,
    const StarsPoint *);
typedef void (*StarsDumpCargoAddMinerals)(void *, StarsU8 *, const StarsU8 *);
typedef struct StarsDumpCargoInput {
    StarsU16 fleet_flags;
    StarsI16 planet;
    StarsPoint point;
    StarsU8 plan_index;
    StarsU8 minerals[12];
    StarsDumpCargoBattlePlan battle_plan;
    StarsDumpCargoPlanet find_planet;
    StarsDumpCargoSalvage drop_salvage;
    StarsDumpCargoAddMinerals add_minerals;
    void *context;
} StarsDumpCargoInput;
StarsI16 stars_dump_cargo(StarsDumpCargoInput *);
typedef struct StarsMoveOrder {
    StarsPoint point;
    StarsI16 id;
    StarsU16 flags;
    StarsU8 reserved[10];
} StarsMoveOrder;
/* A PLORD allocation has a four-byte header followed by 18-byte order
 * records.  The first-order repair used by several AI personalities changes
 * only the allocation count, the first record's low order-kind nibble, and
 * the first record's packed repair value. */
#define STARS_AI_PLORD_HEADER_SIZE 4U
#define STARS_AI_PLORD_ORDER_SIZE 18U
StarsI16 stars_ai_set_first_order_kind(StarsU8 *plord, StarsU16 kind);
StarsI16 stars_ai_repair_first_order(StarsU8 *plord);
StarsI16 stars_ai_move_order_to_plord(StarsU8 *plord, StarsU16 capacity,
    const StarsMoveOrder *, StarsI16);
typedef void (*StarsSelectAiFleet)(void *, StarsI16, StarsI16);
typedef StarsI16 (*StarsRefreshAiFleet)(void *);
typedef struct StarsMoveAiFleetState {
    StarsMoveOrder *orders;
    StarsU16 count;
    StarsU16 capacity;
    StarsSelectAiFleet select_fleet;
    StarsRefreshAiFleet refresh_fleet;
} StarsMoveAiFleetState;
StarsI16 stars_f_move_ai_fleet(const StarsU8 *, const StarsMoveOrder *,
    StarsI16, StarsMoveAiFleetState *, void *);
typedef StarsI16 (*StarsFindClosestStarbase)(void *, const StarsU8 *, StarsI16);
typedef void (*StarsGetPlanetPoint)(void *, StarsI16, StarsPoint *);
typedef StarsI16 (*StarsMoveAiFleet)(void *, const StarsU8 *,
    const StarsMoveOrder *, StarsI16);
StarsI16 stars_f_colonize_ai_fleet(StarsU8 *, const StarsPoint *, StarsI16,
    StarsChangeAiMainObject, StarsFindIdealWarp, StarsMoveAiFleet, void *);
StarsI16 stars_f_goto_wormhole_ai_fleet(StarsU8 *, const StarsU8 *,
    StarsChangeAiMainObject, StarsFindIdealWarp, StarsMoveAiFleet, void *);
StarsI32 stars_score_from_give_take_tactic(StarsI32, StarsI32, StarsI16);
typedef struct StarsMoveToStarbaseOps {
    StarsFindClosestStarbase find_closest;
    StarsGetPlanetPoint get_point;
    StarsMoveAiFleet move_fleet;
} StarsMoveToStarbaseOps;
StarsI16 stars_f_move_to_nearest_starbase(const StarsU8 *, StarsI16,
    const StarsMoveToStarbaseOps *, void *);
typedef void (*StarsQuickBuildDefenses)(void *, const StarsU8 *, void *);
void stars_fix_planets_under_attack(const StarsU8 *const *, StarsU16,
    const StarsU8 *, void *, StarsQuickBuildDefenses, void *);
typedef void (*StarsSendOrderMessage)(void *, StarsU16, StarsU16, StarsU16,
    StarsU32);
typedef void (*StarsUpdateOrderWindow)(void *, StarsU16);
typedef void (*StarsUpdateOrderDropdowns)(void *, StarsI16);
typedef struct StarsSetOrdersLbSelOps {
    StarsSendOrderMessage send_message;
    StarsUpdateOrderWindow update_window;
    StarsUpdateOrderDropdowns update_dropdowns;
} StarsSetOrdersLbSelOps;
void stars_set_orders_lb_sel(StarsU16, StarsI16, StarsU16,
    const StarsSetOrdersLbSelOps *, void *);
typedef const StarsU8 *(*StarsGetOrderLocationName)(void *, StarsI16,
    StarsI16, StarsI16, StarsI16);
typedef void (*StarsClearOrdersList)(void *, StarsU16);
typedef void (*StarsAddOrderText)(void *, StarsU16, const StarsU8 *);
typedef void (*StarsSelectOrderRow)(void *, StarsI16);
typedef void (*StarsFillOrdersDrawPlanShip)(void *, StarsU16, StarsU16);
typedef struct StarsFillOrdersOps {
    StarsClearOrdersList clear_list;
    StarsGetOrderLocationName get_location_name;
    StarsAddOrderText add_text;
    StarsSelectOrderRow select_row;
    StarsFillOrdersDrawPlanShip draw_plan_ship;
} StarsFillOrdersOps;
void stars_fill_orders_lb(StarsU16, const StarsU8 *, StarsU16, StarsI16,
    StarsI16, const StarsFillOrdersOps *, void *);
typedef const StarsU8 *(*StarsGetBattleDropdownText)(void *, StarsI16, StarsI16);
typedef struct StarsFillBattleDropdownOps {
    StarsClearOrdersList clear_list;
    StarsGetBattleDropdownText get_text;
    StarsAddOrderText add_text;
    StarsSelectOrderRow select_row;
} StarsFillBattleDropdownOps;
void stars_fill_battle_dropdown(StarsU16, StarsI16, StarsU16, StarsI16,
    const StarsFillBattleDropdownOps *, void *);
typedef void (*StarsMakeDirectory)(void *, const StarsU8 *);
void stars_create_backup_dir(StarsU8 *, const StarsU8 *, StarsU16, StarsU16,
    const StarsU8 *, const StarsU8 *, const StarsU8 *, const StarsU8 *,
    StarsMakeDirectory, void *);
const StarsU8 *stars_nth_valid_shdef(const StarsU8 *, StarsU16, StarsI16,
    const StarsU8 *, StarsU16, StarsI16);
typedef void (*StarsGetDropoffPoint)(void *, StarsU16, StarsPoint *);
typedef struct StarsDropoffOps {
    StarsGetDropoffPoint get_point;
} StarsDropoffOps;
StarsI16 stars_f_enum_drop_off_stage1(const StarsU8 *, const StarsU8 *,
    StarsU16, StarsI16, const StarsU8 *, const StarsDropoffOps *, void *);
StarsI16 stars_f_enum_drop_off_stage2(const StarsU8 *, const StarsU8 *,
    StarsU16, StarsI16, const StarsU8 *, const StarsDropoffOps *, void *);
void stars_mine_minerals(const StarsU8 *, StarsU16,
    StarsEstimateMineralsMined, void *);
void stars_unmark_mine_fields(StarsU8 *, StarsU16);
void stars_create_ship(StarsI16 *, StarsU8 *, StarsU16, StarsI16, StarsI16);
typedef void (*StarsChangeProductionSelection)(void *, StarsI16, StarsI16);
typedef void (*StarsInitializeProduction)(void *, StarsU8 *, StarsU16);
typedef StarsI16 (*StarsFillPlanetProduction)(void *, const StarsU8 *);
typedef void (*StarsFinishProduction)(void *, StarsI16);
typedef struct StarsProductionQueueOps {
    StarsChangeProductionSelection change_selection;
    StarsInitializeProduction initialize;
    StarsFillPlanetProduction fill;
    StarsFinishProduction finish;
} StarsProductionQueueOps;
void stars_fill_production_queue(const StarsU8 *const *, StarsU16,
    const StarsProductionQueueOps *, void *);
typedef struct StarsAiProductionQueue {
    StarsU8 *records;
    StarsU16 count;
    StarsU16 capacity;
} StarsAiProductionQueue;
typedef StarsU16 (*StarsAiItemCapacity)(void *, StarsU16, StarsU16);
typedef void (*StarsInitAiProduction)(void *, StarsU8 *);
typedef void (*StarsFinishAiProduction)(void *, StarsI16);
typedef struct StarsAddItemToQueueOps {
    StarsAiItemCapacity item_capacity;
    StarsInitAiProduction initialize;
    StarsFinishAiProduction finish;
} StarsAddItemToQueueOps;
void stars_add_item_to_queue(StarsAiProductionQueue *,
    StarsAiProductionQueue *, StarsU16, StarsU16, StarsU16, StarsI16,
    const StarsAddItemToQueueOps *, void *);
typedef void (*StarsEnsureAiStarbaseDesigns)(void *);
typedef void (*StarsValidateAiStarbaseHistory)(void *);
typedef void (*StarsWriteAiResearchState)(void *, StarsU8, StarsI16);
typedef struct StarsIroAiOps {
    StarsEnsureAiStarbaseDesigns ensure_designs;
    StarsLatestStarbase latest_starbase;
    StarsValidateAiStarbaseHistory validate_history;
    StarsWriteAiResearchState write_research;
} StarsIroAiOps;
typedef struct StarsIroAiState {
    StarsU8 tech[6];
    StarsU8 current_tech;
    StarsI16 percent_research;
} StarsIroAiState;
StarsI16 stars_iro_ensure_ai(StarsU8 *, StarsI16, StarsI16 *, StarsI16,
    StarsIroAiState *, const StarsIroAiOps *, void *);
typedef StarsI16 (*StarsIsAiTransport)(void *, const StarsU8 *);
typedef void (*StarsSetAiFleetSpeed)(void *, const StarsU8 *, StarsI16,
    StarsI16, const StarsU8 *const *);
typedef struct StarsKeepFleetsMovingOps {
    StarsIsAiTransport is_transport;
    StarsSetAiFleetSpeed set_speed;
} StarsKeepFleetsMovingOps;
void stars_keep_fleets_moving(const StarsU8 *const *, StarsU16, StarsI16,
    StarsI16, const StarsU8 *const *, StarsU16,
    const StarsKeepFleetsMovingOps *, void *);
StarsI16 stars_f_should_planet_build_colonizer(StarsPoint, StarsI16,
    const StarsPoint *, const StarsU8 *, StarsU16, StarsAiRandom, void *);
void stars_init_random_planet_list(const StarsU8 *const *, StarsU16, StarsI16,
    StarsU16, const StarsU8 **, StarsU16 *, StarsAiRandom, void *);
typedef struct StarsAiPart {
    StarsU16 tech_group;
    StarsU16 attributes;
    const StarsU8 *component;
} StarsAiPart;
typedef StarsI16 (*StarsLookupAiPart)(void *, StarsU16, StarsU16,
    StarsAiPart *);
StarsI16 stars_f_get_ai_part(StarsU16, const StarsU8 *, StarsU16,
    const StarsU16 *, StarsLookupAiPart, StarsAiPart *, void *);
typedef struct StarsAiShdefWork {
    StarsU16 hull_id;
    StarsU16 part_count;
    StarsU16 part_groups[64];
    StarsU16 part_attributes[64];
    StarsU16 part_ids[64];
    StarsU16 flags;
    StarsU16 turn;
    StarsU32 existing;
    StarsU32 built;
    StarsI16 name_base;
    StarsI16 name_length;
    StarsI16 bitmap_id;
    char name[32];
} StarsAiShdefWork;
typedef StarsI16 (*StarsLookupAiHull)(void *, StarsU16, StarsU16,
    StarsAiShdefWork *);
typedef StarsI16 (*StarsLookupAiShdefPart)(void *, StarsU16, StarsAiPart *);
typedef void (*StarsNameAiShdef)(void *, StarsAiShdefWork *, StarsI16,
    StarsI16, StarsI16);
typedef StarsI16 (*StarsPublishAiShdef)(void *, StarsI16,
    const StarsAiShdefWork *);
StarsI16 stars_f_create_ai_shdef(StarsI16, StarsU16, const StarsU8 *,
    StarsLookupAiHull, StarsLookupAiShdefPart, StarsNameAiShdef,
    StarsPublishAiShdef, void *);
typedef StarsI16 (*StarsAiGetName)(void *, StarsI16, char *, StarsU16);
typedef StarsI16 (*StarsAiBitmapInUse)(void *, StarsU16, StarsU16);
typedef StarsI16 (*StarsAiNameInUse)(void *, StarsU16, const char *);
typedef void (*StarsAiFormatName)(void *, char *, StarsU16, StarsI16,
    StarsI16);
typedef struct StarsPickAiNameOps {
    StarsAiRandom random_value;
    StarsAiGetName get_name;
    StarsAiBitmapInUse bitmap_in_use;
    StarsAiNameInUse name_in_use;
    StarsAiFormatName format_name;
} StarsPickAiNameOps;
void stars_pick_ai_name_and_bitmap(StarsAiShdefWork *, StarsI16, StarsI16,
    StarsI16, const StarsPickAiNameOps *, void *);
typedef StarsU16 (*StarsAiOldShdefFlags)(void *, StarsI16);
typedef void (*StarsAiSetShdefFlags)(void *, StarsI16, StarsU16);
typedef void (*StarsAiChangeShdefCount)(void *, StarsI16, StarsI16);
typedef void (*StarsAiUpdateShdefCost)(void *, StarsAiShdefWork *);
typedef void (*StarsAiLogShdefChange)(void *, StarsI16,
    const StarsAiShdefWork *);
typedef struct StarsChangeAiShdefOps {
    StarsAiOldShdefFlags old_flags;
    StarsAiSetShdefFlags set_flags;
    StarsAiChangeShdefCount change_count;
    StarsAiUpdateShdefCost update_cost;
    StarsAiLogShdefChange log_change;
    StarsI16 player_design_count;
} StarsChangeAiShdefOps;
StarsI16 stars_f_change_ai_shdef(StarsAiShdefWork *, StarsI16, StarsU16,
    const StarsChangeAiShdefOps *, void *);
typedef struct StarsAiBlockedMinePlanet {
    StarsU16 id;
    StarsU16 production_flags;
    StarsI16 current_eta;
    StarsI16 queued_eta;
    StarsU16 existing_mines;
    StarsU16 max_mines;
    StarsI16 mining_race_stat;
    StarsI32 resource_units;
    StarsI32 mine_cost;
} StarsAiBlockedMinePlanet;
typedef StarsI16 (*StarsAiBlockedMineResources)(void *, StarsU16);
typedef void (*StarsAiBlockedMineQueue)(void *, StarsU16, StarsI16,
    StarsI16);
typedef void (*StarsAiBlockedMineMark)(void *, StarsU16, StarsI16);
typedef struct StarsAddBlockedMinesOps {
    StarsAiBlockedMineResources resources_at_planet;
    StarsAiBlockedMineQueue add_queue_item;
    StarsAiBlockedMineMark mark_queue;
} StarsAddBlockedMinesOps;
void stars_add_mines_to_blocked_queues(StarsAiBlockedMinePlanet *const *,
    StarsU16, const StarsAddBlockedMinesOps *, void *);
typedef struct StarsAiTargetFleet {
    StarsU16 id;
    StarsU16 planet_id;
    StarsPoint point;
    StarsI32 combat_power;
    StarsU8 human;
    StarsU8 in_transit;
} StarsAiTargetFleet;
typedef struct StarsAiTargetPlanet {
    StarsU16 id;
    StarsPoint point;
    StarsI16 owner;
    StarsU8 has_attack_fleet;
} StarsAiTargetPlanet;
typedef StarsI16 (*StarsAiMoveTarget)(void *, StarsU16, StarsPoint, StarsU16);
typedef StarsI16 (*StarsAiRetreatTarget)(void *, StarsU16);
typedef struct StarsTargetAttackOps {
    StarsAiRandom random_value;
    StarsAiMoveTarget move_target;
    StarsAiRetreatTarget retreat_to_base;
} StarsTargetAttackOps;
StarsI16 stars_id_target_attack(const StarsAiTargetFleet *,
    const StarsAiTargetFleet *, StarsU16, const StarsAiTargetFleet *, StarsU16,
    StarsI16, const StarsAiTargetPlanet *, StarsU16, StarsI16,
    const StarsTargetAttackOps *, void *);
typedef struct StarsAiQuickDefensePlanet {
    StarsU16 id;
    StarsI32 resources[3];
    StarsI32 production_threshold;
    StarsI16 current_mine_capacity;
    StarsI16 defense_capacity;
} StarsAiQuickDefensePlanet;
typedef StarsI16 (*StarsAiDefenseQueue)(void *, StarsU16, StarsI16,
    StarsI16);
typedef void (*StarsAiDefenseFinish)(void *, StarsU16, StarsI16);
typedef struct StarsQuickDefenseOps {
    StarsAiDefenseQueue add_queue_item;
    StarsAiDefenseFinish finish_production;
} StarsQuickDefenseOps;
void stars_quick_build_defenses(const StarsAiQuickDefensePlanet *,
    const StarsQuickDefenseOps *, void *);
typedef StarsI16 (*StarsAiFleetInField)(void *, const StarsU8 *,
    const StarsU8 *);
typedef StarsI16 (*StarsAiFleetRandom)(void *, StarsI16);
typedef StarsI16 (*StarsAiFleetRaceStat)(void *, StarsI16, StarsI16);
typedef StarsU16 (*StarsAiBestWarp)(void *, const StarsU8 *,
    const StarsMoveOrder *);
typedef StarsI16 (*StarsAiFleetCurrentSpeed)(void *, const StarsU8 *);
typedef void (*StarsAiFleetSetSpeed)(void *, const StarsU8 *, StarsU16);
typedef struct StarsSetAiFleetIdealSpeedOps {
    StarsAiFleetInField fleet_in_field;
    StarsAiFleetRandom random_value;
    StarsAiFleetRaceStat race_stat;
    StarsAiBestWarp best_warp;
    StarsAiFleetCurrentSpeed current_speed;
    StarsAiFleetSetSpeed set_speed;
} StarsSetAiFleetIdealSpeedOps;
void stars_set_ai_fleet_ideal_speed(const StarsU8 *, StarsI16, StarsI16,
    const StarsU8 *const *, StarsU16, StarsI16,
    const StarsSetAiFleetIdealSpeedOps *, void *);
typedef StarsU32 (*StarsAiFleetDistance)(void *, const StarsU8 *,
    const StarsU8 *);
typedef void (*StarsClearAiTask)(void *, const StarsU8 *, StarsI16);
typedef StarsI16 (*StarsMoveAiOrder)(void *, const StarsU8 *,
    const StarsMoveOrder *, StarsI16);
typedef struct StarsFindBuddyOps {
    StarsAiFleetDistance distance;
    StarsAiFleetRandom random_value;
    StarsClearAiTask clear_task;
    StarsMoveAiOrder move_order;
} StarsFindBuddyOps;
StarsI16 stars_f_find_buddy_and_join_up(const StarsU8 *, StarsI16, StarsI16,
    StarsI32, StarsI32, const StarsU8 *const *, const StarsPoint *, StarsU16,
    StarsI16, const StarsFindBuddyOps *, void *);
StarsI16 stars_f_should_we_build_colonizers(StarsI16, StarsI16, StarsI16,
    StarsI16, StarsI16, const StarsU8 *const *, StarsU16,
    const StarsU8 *, const StarsU16 *, const StarsU32 *,
    const StarsU8 *, const StarsI16 *, StarsU16, StarsAiRandom, void *,
    StarsI16 *);
typedef StarsI16 (*StarsFindNearestAiObject)(void *, StarsPoint, StarsI16 *);
typedef StarsI16 (*StarsMoveNearestAiFleet)(void *, const StarsU8 *,
    const StarsMoveOrder *, StarsI16);
typedef struct StarsMoveNearestPlanetOps {
    StarsFindNearestAiObject find_nearest;
    StarsMoveNearestAiFleet move_fleet;
} StarsMoveNearestPlanetOps;
void stars_move_to_nearest_planet_or_enemy(const StarsU8 *, StarsI16,
    StarsPoint, StarsI32, const StarsU8 *const *, const StarsPoint *, StarsU16,
    StarsI16, const StarsMoveNearestPlanetOps *, void *);
typedef struct StarsAiStarbaseDesign {
    StarsU8 active;
    StarsU16 age;
    StarsU32 cost_a;
    StarsU32 cost_b;
} StarsAiStarbaseDesign;
typedef void (*StarsEnsureMacintiDesign)(void *, StarsI16, StarsI16);
void stars_ensure_macinti_starbase_designs(StarsU8 *, StarsU16,
    const StarsAiStarbaseDesign *, StarsU16, StarsU32, StarsU32,
    StarsEnsureMacintiDesign, void *);
typedef struct StarsAiUpgradeStarbase {
    StarsU16 family;
    StarsU16 recycle_state;
    StarsU16 resource_values[3];
    StarsI16 planet_capacity;
} StarsAiUpgradeStarbase;
typedef StarsI16 (*StarsAiQueueStarbaseUpgrade)(void *, StarsI16);
typedef struct StarsUpgradeStarbaseOps {
    StarsAiRandom random_value;
    StarsAiQueueStarbaseUpgrade queue_upgrade;
} StarsUpgradeStarbaseOps;
StarsI16 stars_f_upgrade_ai_starbase(StarsAiUpgradeStarbase *, StarsI16,
    StarsI16, StarsU16, const StarsUpgradeStarbaseOps *, void *);
typedef struct StarsAiQueueStarbasePlanet {
    StarsU16 id;
    StarsI16 owner;
    StarsU16 starbase_family;
    StarsI16 mining_level;
    StarsU16 mineral_total;
    StarsU8 ai_flags;
    StarsU8 history_present;
} StarsAiQueueStarbasePlanet;
typedef StarsI16 (*StarsAiIsCyberStarbase)(void *, StarsU16);
typedef StarsI16 (*StarsAiQueueStarbase)(void *, StarsU16, StarsI16);
typedef struct StarsQueueAiStarbaseOps {
    StarsAiIsCyberStarbase is_cyber;
    StarsAiQueueStarbase queue_starbase;
} StarsQueueAiStarbaseOps;
void stars_queue_ai_starbase(const StarsAiQueueStarbasePlanet *, StarsU16,
    StarsI16, StarsI16, const StarsQueueAiStarbaseOps *, void *);
typedef StarsI16 (*StarsAiScoutHasTeeth)(void *, const StarsAiTargetFleet *);
typedef struct StarsScoutTargetOps {
    StarsAiScoutHasTeeth has_teeth;
    StarsAiMoveTarget move_target;
} StarsScoutTargetOps;
StarsI16 stars_id_target_scout(const StarsAiTargetFleet *,
    const StarsAiTargetFleet *, StarsU16, StarsI16,
    const StarsAiTargetPlanet *, StarsU16, StarsU16 *,
    const StarsScoutTargetOps *, void *);
typedef struct StarsAiFlingPlanet {
    StarsU16 id;
    StarsPoint point;
    StarsI16 owner;
    StarsU16 turn_seen;
    StarsU16 flags;
    StarsU16 mineral_values[3];
} StarsAiFlingPlanet;
typedef StarsI16 (*StarsAiFlingQueue)(void *, StarsI16, StarsI16);
typedef struct StarsAiFlingOps {
    StarsAiRandom random_value;
    StarsAiFlingQueue queue_item;
} StarsAiFlingOps;
StarsI16 stars_f_ai_fling(const StarsAiFlingPlanet *, StarsU16,
    StarsPoint, StarsU16, StarsI16, const StarsI32 *, StarsU16,
    const StarsAiFlingOps *, void *);
typedef struct StarsAiBasicPlanet {
    StarsU16 id;
    StarsI16 owner;
    StarsI16 research_level;
    StarsU16 mineral_total;
    StarsU8 ai_marker;
} StarsAiBasicPlanet;
typedef StarsI16 (*StarsAiBasicAction)(void *, StarsU16);
typedef struct StarsHandleBasicOps {
    void (*keep_fleets_moving)(void *);
    void (*queue_starbases)(void *, StarsI16);
    StarsAiBasicAction upgrade_starbase;
    StarsAiBasicAction fling;
    StarsAiBasicAction scanner;
    StarsAiBasicAction defenses;
    StarsAiBasicAction terraforming;
    void (*finish_production)(void *, StarsU16, StarsI16);
    void (*fix_planets_under_attack)(void *);
    void (*add_mines_blocked)(void *);
} StarsHandleBasicOps;
void stars_handle_basic_ai_tasks(const StarsAiBasicPlanet *, StarsU16, StarsI16,
    StarsI16, const StarsHandleBasicOps *, void *);
typedef StarsI32 (*StarsAiFreighterTargetScore)(void *,
    const StarsAiTargetPlanet *, StarsI16);
typedef struct StarsFreighterTargetOps {
    StarsAiFreighterTargetScore score_target;
    StarsAiMoveTarget move_target;
} StarsFreighterTargetOps;
StarsI16 stars_id_target_freighter(const StarsAiTargetFleet *,
    const StarsAiTargetPlanet *, StarsU16, StarsU16, StarsI16, StarsI16,
    const StarsFreighterTargetOps *, void *, StarsU16 *);
typedef struct StarsAiHistoryEntry {
    StarsU16 planet_id;
    StarsI16 level;
    StarsU16 fleets[8];
    StarsU16 fleet_count;
} StarsAiHistoryEntry;
typedef struct StarsAiHistoryPlanet {
    StarsU16 id;
    StarsI16 owner;
    StarsU8 active_starbase;
    StarsU16 population;
    StarsU16 concentrations[3];
    StarsU16 stored[3];
} StarsAiHistoryPlanet;
typedef struct StarsAiHistoryTransport {
    StarsU16 id;
    StarsU16 destination;
} StarsAiHistoryTransport;
typedef StarsI16 (*StarsAiIsTransport)(void *, StarsU16);
typedef struct StarsValidateHistoryOps {
    StarsAiIsTransport is_transport;
} StarsValidateHistoryOps;
void stars_validate_starbase_history(StarsAiHistoryEntry *, StarsU16 *,
    StarsU16, StarsI16, StarsU16, const StarsAiHistoryPlanet *, StarsU16,
    const StarsAiHistoryTransport *, StarsU16, const StarsValidateHistoryOps *,
    void *);
typedef void (*StarsCreateAiStarbase)(void *, StarsI16, StarsI16);
void stars_ensure_ai_starbase_designs(StarsI16,
    const StarsU8 *, const StarsAiStarbaseDesign *,
    const StarsCreateAiStarbase, void *);
typedef struct StarsAiShdefStatus {
    StarsU8 active;
    StarsU32 existing;
    StarsU16 turn;
} StarsAiShdefStatus;
typedef void (*StarsRecycleAiShdef)(void *, StarsI16);
StarsI16 stars_check_ai_shdef_status(StarsI16, StarsI16, StarsU16,
    StarsI16 *, StarsU8 *, StarsI16, const StarsAiShdefStatus *,
    StarsRecycleAiShdef, void *);
typedef StarsI16 (*StarsAiQueueHasType)(void *, StarsI16);
typedef void (*StarsAiQueueAddItem)(void *, StarsI16, StarsI16);
StarsI16 stars_f_queue_ai_defenses(StarsI32, StarsU32, StarsU16,
    StarsI16, StarsI16, StarsAiQueueHasType, StarsAiQueueAddItem, void *);
typedef struct StarsAiProductionCandidate {
    StarsU8 category;
    StarsU16 subtype;
    StarsU16 quantity;
    StarsI16 queue_type;
    StarsI32 costs[4];
} StarsAiProductionCandidate;
StarsI16 stars_f_queue_ai_scanner(const StarsAiProductionCandidate *, StarsU16,
    StarsAiProductionCandidate *, StarsU16, const StarsI32 *,
    StarsI32 *, StarsAiQueueAddItem, void *);
StarsI16 stars_f_queue_ai_terraforming(StarsI16, StarsI32, StarsU16,
    const StarsAiProductionCandidate *, StarsU16,
    StarsAiProductionCandidate *, StarsU16, StarsI32 *,
    StarsAiQueueAddItem, void *);
typedef void (*StarsSplitAiFleet)(void *, StarsU8 *, const StarsU16 *);
void stars_split_out_shdefs(StarsU8 *, StarsU8 *const *, StarsU16,
    StarsI16, StarsU16, StarsSplitAiFleet, void *);
typedef StarsI16 (*StarsAiPlanetOptValue)(void *, StarsI16);
typedef const StarsU8 *(*StarsAiFindWormhole)(void *, StarsPoint, StarsU32);
typedef struct StarsNearestColonizableOps {
    StarsAiPlanetOptValue planet_opt_value;
    StarsAiFindWormhole find_wormhole;
} StarsNearestColonizableOps;
/* owners: -2 means no loaded planet; -1 means known unowned. */
StarsI16 stars_id_nearest_colonizable_planet(StarsPoint, StarsI16, StarsI16,
    StarsI16, StarsI16, StarsI16, StarsU8 *, const StarsI16 *,
    const StarsPoint *, StarsU16, const StarsI16 *, StarsU16,
    const StarsNearestColonizableOps *, void *, const StarsU8 **);
typedef struct StarsAiFreighterThing {
    StarsU16 id;
    StarsU16 flags;
    StarsPoint point;
    StarsU8 destination_class;
    StarsI16 cargo[3];
} StarsAiFreighterThing;
typedef void (*StarsTransferAiSupply)(void *, StarsI16, StarsI16,
    StarsI16, StarsI16);
StarsI16 stars_f_salvage_target_freighter(const StarsAiFreighterThing *,
    StarsU16, StarsPoint, StarsI16, StarsI16, StarsI16, StarsI16, StarsI32, StarsI32,
    StarsI16, StarsI16 *, StarsTransferAiSupply, void *);
typedef struct StarsAiShdefFleet {
    StarsI16 player;
    StarsI16 planet;
    StarsPoint point;
    StarsI32 mine_value;
    StarsU16 ship_counts[16];
} StarsAiShdefFleet;
typedef void (*StarsMergeAiFleet)(void *, StarsAiShdefFleet *,
    StarsAiShdefFleet *);
void stars_merge_all_shdefs(StarsU16, StarsAiShdefFleet *, StarsU16,
    StarsI16, StarsMergeAiFleet, void *);
typedef struct StarsAiStarbaseWork {
    StarsU16 hull_id;
    StarsU16 flags;
    StarsI16 bitmap_id;
    char name[32];
    StarsU16 component_count;
    StarsU16 groups[64];
    StarsU16 tech_levels[64];
} StarsAiStarbaseWork;
typedef StarsI16 (*StarsCreateAiShdefWork)(void *, StarsI16, StarsI16,
    StarsI16, StarsAiStarbaseWork *);
typedef void (*StarsChangeAiShdefWork)(void *, StarsI16,
    const StarsAiStarbaseWork *);
StarsI16 stars_f_create_ai_starbase(StarsI16, StarsI16, StarsI16, StarsI16,
    StarsI16, StarsAiStarbaseWork *, StarsCreateAiShdefWork,
    StarsChangeAiShdefWork, const StarsPickAiNameOps *, void *);

void stars_randomize(StarsSeeds *state, StarsU32 seed);
void stars_randomize2(StarsSeeds *state, StarsU32 seed);
StarsI16 stars_random(StarsSeeds *state, StarsI16 count);
typedef struct StarsRandomSeedDialogOps {
    const StarsU8 *(*text)(void *, StarsU16, StarsU16);
    void (*randomize)(void *, StarsU32);
    void (*end)(void *, StarsU16, StarsI16);
} StarsRandomSeedDialogOps;
StarsI16 stars_random_seed_dialog(StarsU16, StarsU16, StarsU16, StarsU32,
    const StarsRandomSeedDialogOps *, void *);
StarsU32 stars_next_file_xor(StarsSeeds *state);
void stars_get_file_seeds(const StarsSeeds *state, StarsU32 *first, StarsU32 *second);
void stars_set_file_seeds(StarsSeeds *state, StarsU32 first, StarsU32 second);
void stars_set_file_xor_stream(StarsSeeds *state, StarsU32 game_id,
    StarsI16 salt, StarsI16 turn, StarsI16 player, StarsI16 crippled);
StarsU32 stars_distance_squared(StarsI16 x1, StarsI16 y1, StarsI16 x2, StarsI16 y2);
/* Rectangle fields retain their raw 16-bit representation. */
struct StarsRect { StarsU16 left, top, right, bottom; };
typedef struct StarsDrawShipPlanetThing {
    StarsU16 flags;
    StarsPoint point;
} StarsDrawShipPlanetThing;
typedef StarsI16 (*StarsDrawShipPlanetGetText)(void *, StarsU16, StarsU8 *,
    StarsU16);
typedef StarsI16 (*StarsDrawShipPlanetGetPlanetName)(void *, StarsI16,
    StarsU8 *, StarsU16);
typedef StarsI16 (*StarsDrawShipPlanetDrawTile)(void *, StarsU16,
    StarsU16, const StarsU8 *, StarsRect *);
typedef void (*StarsDrawShipPlanetShowWindow)(void *, StarsU16, StarsI16);
typedef void (*StarsDrawShipPlanetEnableWindow)(void *, StarsU16, StarsI16);
typedef void (*StarsDrawShipPlanetSetWindowText)(void *, StarsU16,
    const StarsU8 *);
typedef void (*StarsDrawShipPlanetSetWindowPos)(void *, StarsU16, StarsI16,
    StarsI16, StarsI16, StarsI16, StarsU16);
typedef struct StarsDrawShipPlanetOps {
    StarsDrawShipPlanetGetText get_text;
    StarsDrawShipPlanetGetPlanetName get_planet_name;
    StarsDrawShipPlanetDrawTile draw_tile;
    StarsDrawShipPlanetShowWindow show_window;
    StarsDrawShipPlanetEnableWindow enable_window;
    StarsDrawShipPlanetSetWindowText set_window_text;
    StarsDrawShipPlanetSetWindowPos set_window_pos;
} StarsDrawShipPlanetOps;
typedef struct StarsDrawShipPlanetInput {
    StarsU16 hdc;
    StarsU16 tile_flags;
    StarsI16 planet_id;
    StarsI16 object_x;
    StarsI16 object_y;
    StarsI16 selected_planet_player;
    StarsI16 current_player;
    StarsU16 grid_bit;
    StarsI16 arial8_height;
    StarsU16 button_planet;
    StarsU16 button_thing;
    StarsDrawShipPlanetThing *things;
    StarsU16 thing_count;
    StarsRect tile_rect;
    StarsDrawShipPlanetOps ops;
    void *context;
} StarsDrawShipPlanetInput;
void stars_draw_ship_planet(StarsDrawShipPlanetInput *);

/* Semantic adapter for DrawPlanetMinSum (NE 0A:12B2).  The original routine
 * paints a planet's three mineral rows and the mine/factory summaries.  The
 * adapter keeps the planet fields and UI effects explicit so the caller can
 * provide the legacy DGROUP and Win16 services without embedding addresses. */
typedef struct StarsPlanetMinSumPlanet {
    StarsI32 minerals[3];
    StarsU32 mineral_capacity_bits;
} StarsPlanetMinSumPlanet;
typedef struct StarsPlanetMinSumTile {
    StarsU16 flags;
} StarsPlanetMinSumTile;
typedef const StarsU8 *(*StarsPlanetMinSumString)(void *, StarsU16);
typedef StarsI16 (*StarsPlanetMinSumGetText)(void *, StarsU16, StarsU8 *,
    StarsU16);
typedef StarsI16 (*StarsPlanetMinSumDrawTile)(void *, StarsU16,
    const StarsPlanetMinSumTile *, StarsRect *, const StarsU8 *);
typedef StarsU16 (*StarsPlanetMinSumSelectObject)(void *, StarsU16, StarsU16);
typedef void (*StarsPlanetMinSumSetTextColor)(void *, StarsU16, StarsU32);
typedef void (*StarsPlanetMinSumTextOut)(void *, StarsU16, StarsI16, StarsI16,
    const StarsU8 *, StarsU16);
typedef void (*StarsPlanetMinSumPatBlt)(void *, StarsU16, StarsI16, StarsI16,
    StarsI16, StarsI16, StarsU32);
typedef void (*StarsPlanetMinSumSetReference)(void *, StarsU16,
    const StarsRect *);
typedef void (*StarsPlanetMinSumRightText)(void *, StarsU16, StarsI16,
    StarsI16, const StarsU8 *, StarsU16, StarsI16);
typedef StarsI16 (*StarsPlanetMinSumRaceStat)(void *, StarsI16, StarsI16);
typedef StarsI16 (*StarsPlanetMinSumCapacity)(void *,
    const StarsPlanetMinSumPlanet *, StarsI16, StarsI16);
typedef StarsI16 (*StarsPlanetMinSumOperating)(void *,
    const StarsPlanetMinSumPlanet *);
typedef StarsI16 (*StarsPlanetMinSumFormat)(void *, StarsU8 *, StarsU16,
    const StarsU8 *, StarsI32, StarsI16);
typedef struct StarsPlanetMinSumOps {
    StarsPlanetMinSumString compressed_string;
    StarsPlanetMinSumGetText get_text;
    StarsPlanetMinSumDrawTile draw_tile;
    StarsPlanetMinSumSelectObject select_object;
    StarsPlanetMinSumSetTextColor set_text_color;
    StarsPlanetMinSumTextOut text_out;
    StarsPlanetMinSumPatBlt pat_blt;
    StarsPlanetMinSumSetReference set_reference;
    StarsPlanetMinSumRightText right_text;
    StarsPlanetMinSumRaceStat race_stat;
    StarsPlanetMinSumCapacity max_mines;
    StarsPlanetMinSumOperating operating_mines;
    StarsPlanetMinSumCapacity max_factories;
    StarsPlanetMinSumFormat format;
} StarsPlanetMinSumOps;
typedef struct StarsPlanetMinSumInput {
    StarsU16 hdc;
    StarsPlanetMinSumTile tile;
    const StarsPlanetMinSumPlanet *planet;
    StarsI16 player;
    StarsI16 arial8_height;
    StarsI16 max_mineral_width;
    StarsU16 normal_font;
    StarsU16 bold_font;
    StarsU16 highlight_brush;
    StarsU32 text_color;
    StarsU32 mineral_colors[3];
    const StarsU8 *mineral_labels[3];
    const StarsU8 *quantity_format;
    const StarsU8 *special_mines_format;
    const StarsU8 *capacity_format;
    const StarsU8 *factory_label;
    const StarsU8 *special_factory_label;
    StarsRect *reference_rects;
    StarsPlanetMinSumOps ops;
    void *context;
} StarsPlanetMinSumInput;
void stars_draw_planet_min_sum(StarsPlanetMinSumInput *);
typedef StarsI16 (*StarsDrawShipCargoGetText)(void *, StarsU16, StarsU8 *,
    StarsU16);
typedef StarsI16 (*StarsDrawShipCargoDrawTile)(void *, StarsU16, StarsU16,
    const StarsU8 *, StarsRect *);
typedef void (*StarsDrawShipCargoSelectFont)(void *, StarsU16, StarsU16);
typedef StarsI32 (*StarsDrawShipCargoTextExtent)(void *, StarsU16,
    const StarsU8 *, StarsI16);
typedef void (*StarsDrawShipCargoTextOut)(void *, StarsU16, StarsI16,
    StarsI16, const StarsU8 *, StarsI16);
typedef void (*StarsDrawShipCargoSetTextColor)(void *, StarsU16, StarsU32);
typedef void (*StarsDrawShipCargoSetReference)(void *, StarsU16,
    const StarsRect *);
typedef void (*StarsDrawShipCargoDrawGauge)(void *, StarsU16,
    const StarsRect *, const void *, StarsI16);
typedef StarsI16 (*StarsDrawShipCargoFormatValue)(void *, StarsU8 *, StarsU16,
    StarsI32);
typedef void (*StarsDrawShipCargoRightText)(void *, StarsU16, StarsI16,
    StarsI16, const StarsU8 *, StarsI16, StarsI16);
typedef struct StarsDrawShipCargoOps {
    StarsDrawShipCargoGetText get_text;
    StarsDrawShipCargoDrawTile draw_tile;
    StarsDrawShipCargoSelectFont select_font;
    StarsDrawShipCargoTextExtent text_extent;
    StarsDrawShipCargoTextOut text_out;
    StarsDrawShipCargoSetTextColor set_text_color;
    StarsDrawShipCargoSetReference set_reference;
    StarsDrawShipCargoDrawGauge draw_gauge;
    StarsDrawShipCargoFormatValue format_value;
    StarsDrawShipCargoRightText right_text;
} StarsDrawShipCargoOps;
typedef struct StarsDrawShipCargoInput {
    StarsU16 hdc;
    StarsU16 tile_flags;
    StarsU16 grid_bit;
    StarsI16 arial8_height;
    StarsI16 max_mineral_quantity_width;
    StarsU16 normal_font;
    StarsU16 bold_font;
    StarsU32 mineral_colors[3];
    StarsU32 button_text_color;
    const void *planet;
    const StarsI32 *resource_values;
    const StarsU8 *mineral_names[3];
    StarsRect tile_rect;
    StarsDrawShipCargoOps ops;
    void *context;
} StarsDrawShipCargoInput;
void stars_draw_ship_cargo(StarsDrawShipCargoInput *);
typedef StarsI16 (*StarsDrawFleetCompGetText)(void *, StarsU16, StarsU8 *,
    StarsU16);
typedef StarsI16 (*StarsDrawFleetCompDrawTile)(void *, StarsU16, StarsU16,
    const StarsU8 *, StarsRect *);
typedef StarsI32 (*StarsDrawFleetCompTextExtent)(void *, StarsU16,
    const StarsU8 *, StarsI16);
typedef void (*StarsDrawFleetCompTextOut)(void *, StarsU16, StarsI16,
    StarsI16, const StarsU8 *, StarsI16);
typedef void (*StarsDrawFleetCompRightText)(void *, StarsU16, StarsI16,
    StarsI16, const StarsU8 *, StarsI16, StarsI16);
typedef StarsI16 (*StarsDrawFleetCompGetClientHeight)(void *, StarsU16);
typedef void (*StarsDrawFleetCompSetWindowPos)(void *, StarsU16, StarsI16,
    StarsI16, StarsI16, StarsI16, StarsU16);
typedef void (*StarsDrawFleetCompShowWindow)(void *, StarsU16, StarsI16);
typedef void (*StarsDrawFleetCompEnableWindow)(void *, StarsU16, StarsI16);
typedef void (*StarsDrawFleetCompSelectFont)(void *, StarsU16, StarsU16);
typedef StarsI16 (*StarsDrawFleetCompFindIdealWarp)(void *, const void *,
    StarsI16);
typedef StarsI32 (*StarsDrawFleetCompEstimateFuel)(void *, const void *,
    StarsI16);
typedef StarsI16 (*StarsDrawFleetCompFormatFuel)(void *, StarsU8 *,
    StarsU16, StarsI32);
typedef StarsI16 (*StarsDrawFleetCompFormatPercent)(void *, StarsU8 *,
    StarsU16, StarsI16);
typedef StarsI16 (*StarsDrawFleetCompCloakPercent)(void *, const void *);
typedef StarsI16 (*StarsDrawFleetCompCanSplit)(void *, StarsI32);
typedef StarsI16 (*StarsDrawFleetCompCanSplitAll)(void *, StarsI32);
typedef StarsI16 (*StarsDrawFleetCompCanMerge)(void *, const void *);
typedef struct StarsDrawFleetCompOps {
    StarsDrawFleetCompGetText get_text;
    StarsDrawFleetCompDrawTile draw_tile;
    StarsDrawFleetCompTextExtent text_extent;
    StarsDrawFleetCompTextOut text_out;
    StarsDrawFleetCompRightText right_text;
    StarsDrawFleetCompGetClientHeight get_client_height;
    StarsDrawFleetCompSetWindowPos set_window_pos;
    StarsDrawFleetCompShowWindow show_window;
    StarsDrawFleetCompEnableWindow enable_window;
    StarsDrawFleetCompSelectFont select_font;
    StarsDrawFleetCompFindIdealWarp find_ideal_warp;
    StarsDrawFleetCompEstimateFuel estimate_fuel;
    StarsDrawFleetCompFormatFuel format_fuel;
    StarsDrawFleetCompFormatPercent format_percent;
    StarsDrawFleetCompCloakPercent cloak_percent;
    StarsDrawFleetCompCanSplit can_split;
    StarsDrawFleetCompCanSplitAll can_split_all;
    StarsDrawFleetCompCanMerge can_merge;
} StarsDrawFleetCompOps;
typedef struct StarsDrawFleetCompInput {
    StarsU16 hdc;
    StarsU16 tile_flags;
    StarsRect tile_rect;
    StarsU16 grid_bit;
    StarsI16 arial8_height;
    StarsI16 ship_dropdown_height;
    StarsU16 font_normal;
    StarsU16 font_bold;
    StarsU16 fleet_component_list;
    StarsU16 battle_dropdown;
    StarsU16 buttons[3];
    StarsI16 fleet_component_width;
    StarsI16 battle_dropdown_width;
    StarsI16 list_client_height;
    StarsU16 ship_counts[16];
    const void *fleet;
    StarsDrawFleetCompOps ops;
    void *context;
} StarsDrawFleetCompInput;
void stars_draw_fleet_comp(StarsDrawFleetCompInput *);
typedef struct StarsShipOrdersWaypoint {
    StarsI16 x;
    StarsI16 y;
    StarsU16 warp_word;
} StarsShipOrdersWaypoint;
typedef StarsI16 (*StarsDrawShipOrdersGetText)(void *, StarsU16, StarsU8 *,
    StarsU16);
typedef StarsI16 (*StarsDrawShipOrdersDrawTile)(void *, StarsU16, StarsU16,
    const StarsU8 *, StarsRect *);
typedef StarsI32 (*StarsDrawShipOrdersTextExtent)(void *, StarsU16,
    const StarsU8 *, StarsI16);
typedef void (*StarsDrawShipOrdersTextOut)(void *, StarsU16, StarsI16,
    StarsI16, const StarsU8 *, StarsI16);
typedef void (*StarsDrawShipOrdersRightText)(void *, StarsU16, StarsI16,
    StarsI16, const StarsU8 *, StarsI16, StarsI16);
typedef void (*StarsDrawShipOrdersSelectFont)(void *, StarsU16, StarsU16);
typedef void (*StarsDrawShipOrdersFillRect)(void *, StarsU16,
    const StarsRect *, StarsU16);
typedef void (*StarsDrawShipOrdersSetTextColor)(void *, StarsU16, StarsU32);
typedef StarsI16 (*StarsDrawShipOrdersGetLocation)(void *, StarsI16,
    const StarsShipOrdersWaypoint *, StarsU8 *, StarsU16);
typedef StarsI16 (*StarsDrawShipOrdersGetDistance)(void *,
    const StarsShipOrdersWaypoint *, const StarsShipOrdersWaypoint *,
    StarsU8 *, StarsU16);
typedef StarsI16 (*StarsDrawShipOrdersGetEta)(void *, const void *, StarsI16,
    StarsU8 *, StarsU16);
typedef StarsI16 (*StarsDrawShipOrdersGetWarp)(void *, const void *, StarsI16);
typedef StarsI32 (*StarsDrawShipOrdersFuelUse)(void *, const void *, StarsI16);
typedef StarsI16 (*StarsDrawShipOrdersFormatNumber)(void *, StarsU8 *,
    StarsU16, StarsU16, StarsI32);
typedef StarsI16 (*StarsDrawShipOrdersFindIdealWarp)(void *, const void *,
    StarsI16);
typedef StarsI32 (*StarsDrawShipOrdersFuelAvailable)(void *, const void *);
typedef void (*StarsDrawShipOrdersDrawGauge)(void *, StarsU16,
    const StarsRect *, const void *);
typedef void (*StarsDrawShipOrdersSetReference)(void *, StarsU16,
    const StarsRect *);
typedef void (*StarsDrawShipOrdersSendReport)(void *, StarsU16, StarsU16);
typedef void (*StarsDrawShipOrdersDrawDiamond)(void *, StarsU16,
    const StarsRect *, StarsU16);
typedef void (*StarsDrawShipOrdersSetWindowPos)(void *, StarsU16, StarsI16,
    StarsI16, StarsI16, StarsI16, StarsU16);
typedef void (*StarsDrawShipOrdersShowWindow)(void *, StarsU16, StarsI16);
typedef StarsI16 (*StarsDrawShipOrdersGetClientHeight)(void *, StarsU16);
typedef struct StarsDrawShipOrdersOps {
    StarsDrawShipOrdersGetText get_text;
    StarsDrawShipOrdersDrawTile draw_tile;
    StarsDrawShipOrdersTextExtent text_extent;
    StarsDrawShipOrdersTextOut text_out;
    StarsDrawShipOrdersRightText right_text;
    StarsDrawShipOrdersSelectFont select_font;
    StarsDrawShipOrdersFillRect fill_rect;
    StarsDrawShipOrdersSetTextColor set_text_color;
    StarsDrawShipOrdersGetLocation get_location;
    StarsDrawShipOrdersGetDistance get_distance;
    StarsDrawShipOrdersGetEta get_eta;
    StarsDrawShipOrdersGetWarp get_warp;
    StarsDrawShipOrdersFuelUse fuel_use;
    StarsDrawShipOrdersFormatNumber format_number;
    StarsDrawShipOrdersFindIdealWarp find_ideal_warp;
    StarsDrawShipOrdersFuelAvailable fuel_available;
    StarsDrawShipOrdersDrawGauge draw_gauge;
    StarsDrawShipOrdersSetReference set_reference;
    StarsDrawShipOrdersSendReport send_report;
    StarsDrawShipOrdersDrawDiamond draw_diamond;
    StarsDrawShipOrdersSetWindowPos set_window_pos;
    StarsDrawShipOrdersShowWindow show_window;
    StarsDrawShipOrdersGetClientHeight get_client_height;
} StarsDrawShipOrdersOps;
typedef struct StarsDrawShipOrdersInput {
    StarsU16 hdc;
    StarsU16 tile_flags;
    StarsRect tile_rect;
    StarsU16 grid_bit;
    StarsI16 arial8_height;
    StarsI16 ship_list_width;
    StarsI16 ship_dropdown_height;
    StarsU16 ship_list;
    StarsU16 report_combo;
    StarsU16 normal_font;
    StarsU16 bold_font;
    StarsU16 button_face_brush;
    StarsU16 diamond_brush;
    StarsI16 active_waypoint;
    StarsI16 cord;
    StarsI16 report_selection;
    StarsI32 fuel_available;
    StarsI16 fuel_waypoint;
    const void *fleet;
    const StarsShipOrdersWaypoint *waypoints;
    StarsU16 waypoint_count;
    StarsDrawShipOrdersOps ops;
    void *context;
} StarsDrawShipOrdersInput;
void stars_draw_ship_orders(StarsDrawShipOrdersInput *);
typedef StarsI32 (*StarsWayPtOrdersSendMessage)(void *, StarsU16,
    StarsU16, StarsU16, StarsU32);
typedef StarsI16 (*StarsWayPtOrdersIsVisible)(void *, StarsU16);
typedef void (*StarsWayPtOrdersShowWindow)(void *, StarsU16, StarsI16);
typedef void (*StarsWayPtOrdersSetWindowPos)(void *, StarsU16, StarsI16,
    StarsI16, StarsI16, StarsI16, StarsU16);
typedef void (*StarsWayPtOrdersEnableWindow)(void *, StarsU16, StarsI16);
typedef StarsI16 (*StarsWayPtOrdersGetText)(void *, StarsU16, StarsU8 *,
    StarsU16);
typedef StarsI16 (*StarsWayPtOrdersDrawTile)(void *, StarsU16, StarsU16,
    const StarsU8 *, StarsRect *);
typedef StarsI32 (*StarsWayPtOrdersTextExtent)(void *, StarsU16,
    const StarsU8 *, StarsI16);
typedef void (*StarsWayPtOrdersSelectFont)(void *, StarsU16, StarsU16);
typedef void (*StarsWayPtOrdersFillRect)(void *, StarsU16, const StarsRect *,
    StarsU16);
typedef void (*StarsWayPtOrdersDrawText)(void *, StarsU16, const StarsU8 *,
    StarsI16, const StarsRect *, StarsU16);
typedef void (*StarsWayPtOrdersSetTextColor)(void *, StarsU16, StarsU32);
typedef void (*StarsWayPtOrdersSetReference)(void *, StarsU16,
    const StarsRect *);
typedef void (*StarsWayPtOrdersDrawDiamond)(void *, StarsU16,
    const StarsRect *, StarsU16);
typedef void (*StarsWayPtOrdersDrawGauge)(void *, StarsU16, const StarsRect *,
    const void *, StarsI16);
typedef StarsI16 (*StarsWayPtOrdersFormat)(void *, StarsU8 *, StarsU16,
    StarsU16, StarsI32);
typedef void (*StarsWayPtOrdersDrawMinerals)(void *, StarsU16, StarsI16,
    StarsI16, StarsI16, StarsI16);
typedef struct StarsWayPtOrdersOps {
    StarsWayPtOrdersSendMessage send_message;
    StarsWayPtOrdersIsVisible is_visible;
    StarsWayPtOrdersShowWindow show_window;
    StarsWayPtOrdersSetWindowPos set_window_pos;
    StarsWayPtOrdersEnableWindow enable_window;
    StarsWayPtOrdersGetText get_text;
    StarsWayPtOrdersDrawTile draw_tile;
    StarsWayPtOrdersTextExtent text_extent;
    StarsWayPtOrdersSelectFont select_font;
    StarsWayPtOrdersFillRect fill_rect;
    StarsWayPtOrdersDrawText draw_text;
    StarsWayPtOrdersSetTextColor set_text_color;
    StarsWayPtOrdersSetReference set_reference;
    StarsWayPtOrdersDrawDiamond draw_diamond;
    StarsWayPtOrdersDrawGauge draw_gauge;
    StarsWayPtOrdersFormat format_number;
    StarsWayPtOrdersDrawMinerals draw_minerals;
} StarsWayPtOrdersOps;
typedef struct StarsWayPtOrdersInput {
    StarsU16 hdc;
    StarsU16 tile_flags;
    StarsRect tile_rect;
    StarsU16 grid_bit;
    StarsI16 arial8_height;
    StarsI16 arial7_height;
    StarsI16 arial6_height;
    StarsI16 ship_dropdown_height;
    StarsI16 order_edit_width;
    StarsI16 dropdown_width[3];
    StarsU16 dropdown[3];
    StarsU16 order_edit;
    StarsU16 normal_font;
    StarsU16 bold_font;
    StarsU16 small_font;
    StarsU16 tiny_font;
    StarsU16 button_face_brush;
    StarsU16 diamond_brush;
    StarsU32 button_text_color;
    StarsU32 disabled_text_color;
    StarsI16 active_order_kind;
    StarsI16 selected_player;
    StarsI16 destination_player;
    StarsI16 race_can_scan;
    StarsI16 mineral_density;
    StarsI32 mines;
    StarsI32 layer_mines;
    StarsI16 has_starbase_design;
    StarsI16 starbase_tech_complete;
    StarsI16 has_starbase_order;
    const void *destination_planet;
    const void *fleet;
    StarsI16 dropdown_selection[3];
    StarsU16 report_label_ids[5];
    StarsU16 report_selection;
    StarsWayPtOrdersOps ops;
    void *context;
} StarsWayPtOrdersInput;
void stars_draw_ship_waypoint_orders(StarsWayPtOrdersInput *);
typedef StarsU16 (*StarsDrawProductionGetStock)(void *, StarsU16);
typedef void (*StarsDrawProductionFill)(void *, StarsU16, const StarsRect *,
    StarsU16);
typedef void (*StarsDrawProductionInflate)(void *, StarsRect *, StarsI16,
    StarsI16);
typedef StarsU32 (*StarsDrawProductionSetTextColor)(void *, StarsU16,
    StarsU32);
typedef StarsU16 (*StarsDrawProductionSetBkMode)(void *, StarsU16, StarsU16);
typedef StarsU16 (*StarsDrawProductionSelectFont)(void *, StarsU16, StarsU16);
typedef StarsI16 (*StarsDrawProductionTextExtent)(void *, StarsU16,
    const StarsU8 *, StarsU16);
typedef void (*StarsDrawProductionTextOut)(void *, StarsU16, StarsI16,
    StarsI16, const StarsU8 *, StarsU16);
typedef StarsU16 (*StarsDrawProductionGetString)(void *, StarsU16, StarsU8 *,
    StarsU16);
typedef void (*StarsDrawProductionRightText)(void *, StarsU16, StarsI16,
    StarsI16, const StarsU8 *, StarsU16, StarsI16);
typedef struct StarsDrawProductionOps {
    StarsDrawProductionGetStock get_stock_object;
    StarsDrawProductionFill fill_rect;
    StarsDrawProductionInflate inflate_rect;
    StarsDrawProductionSetTextColor set_text_color;
    StarsDrawProductionSetBkMode set_bk_mode;
    StarsDrawProductionSelectFont select_font;
    StarsDrawProductionTextExtent text_extent;
    StarsDrawProductionTextOut text_out;
    StarsDrawProductionGetString get_string;
    StarsDrawProductionRightText right_text;
} StarsDrawProductionOps;
typedef struct StarsDrawProductionInput {
    StarsU16 hdc;
    StarsRect rect;
    const StarsU8 *text;
    StarsI16 inflate;
    StarsI16 selected;
    StarsI16 listbox;
    StarsU16 window_brush;
    StarsU16 blue_brush;
    StarsU16 gray_brush;
    StarsU16 red_brush;
    StarsU16 green_brush;
    StarsU16 button_face_brush;
    StarsU32 window_color;
    StarsU32 blank_color;
    StarsU32 hash_color;
    StarsU32 ampersand_color;
    StarsU32 other_color;
    StarsU16 italic_font;
    StarsU16 normal_font;
    const StarsDrawProductionOps *ops;
    void *context;
} StarsDrawProductionInput;
void stars_draw_production_item(const StarsDrawProductionInput *);
typedef StarsU16 (*StarsDrawProductionDlgGetDc)(void *, StarsU16);
typedef void (*StarsDrawProductionDlgReleaseDc)(void *, StarsU16, StarsU16);
typedef void (*StarsDrawProductionDlgSelectFont)(void *, StarsU16, StarsU16);
typedef void (*StarsDrawProductionDlgSetBkColor)(void *, StarsU16, StarsU32);
typedef void (*StarsDrawProductionDlgSetTextColor)(void *, StarsU16, StarsU32);
typedef StarsI16 (*StarsDrawProductionDlgTextExtent)(void *, StarsU16,
    const StarsU8 *, StarsI16);
typedef void (*StarsDrawProductionDlgTextOut)(void *, StarsU16, StarsI16,
    StarsI16, const StarsU8 *, StarsI16);
typedef void (*StarsDrawProductionDlgRightText)(void *, StarsU16, StarsI16,
    StarsI16, const StarsU8 *, StarsI16, StarsI16);
typedef const StarsU8 *(*StarsDrawProductionDlgString)(void *, StarsU16);
typedef void (*StarsDrawProductionDlgRect)(void *, StarsU16, StarsRect *);
typedef StarsU16 (*StarsDrawProductionDlgItem)(void *, StarsU16, StarsU16);
typedef StarsI16 (*StarsDrawProductionDlgSelection)(void *, StarsU16);
typedef StarsI16 (*StarsDrawProductionDlgCosts)(void *, StarsI16,
    StarsI32[4]);
typedef StarsI16 (*StarsDrawProductionDlgPackedCosts)(void *, StarsI16,
    StarsI16, StarsU32, StarsI32[4]);
typedef StarsI16 (*StarsDrawProductionDlgEta)(void *, StarsI16, StarsI16);
typedef const StarsU8 *(*StarsDrawProductionDlgEtaFormat)(void *, StarsU16);
typedef StarsI16 (*StarsDrawProductionDlgFormatNumber)(void *, StarsI32,
    StarsU8 *, StarsU16);
typedef StarsU32 (*StarsDrawProductionDlgGetMineralColor)(void *, StarsU16);
typedef void (*StarsDrawProductionDlgDiamond)(void *, StarsU16,
    const StarsRect *);
typedef struct StarsDrawProductionDlgOps {
    StarsDrawProductionDlgGetDc get_dc;
    StarsDrawProductionDlgReleaseDc release_dc;
    StarsDrawProductionDlgSelectFont select_font;
    StarsDrawProductionDlgSetBkColor set_background_color;
    StarsDrawProductionDlgSetTextColor set_text_color;
    StarsDrawProductionDlgTextExtent text_extent;
    StarsDrawProductionDlgTextOut text_out;
    StarsDrawProductionDlgRightText right_text;
    StarsDrawProductionDlgString compressed_string;
    StarsDrawProductionDlgString get_label;
    StarsDrawProductionDlgRect get_client_rect;
    StarsDrawProductionDlgRect get_window_rect;
    StarsDrawProductionDlgRect screen_to_client;
    StarsDrawProductionDlgItem get_dialog_item;
    StarsDrawProductionDlgSelection selection;
    StarsDrawProductionDlgCosts get_costs;
    StarsDrawProductionDlgPackedCosts get_packed_costs;
    StarsDrawProductionDlgEta get_eta;
    StarsDrawProductionDlgEtaFormat get_eta_format;
    StarsDrawProductionDlgFormatNumber format_number;
    StarsDrawProductionDlgGetMineralColor get_mineral_color;
    StarsDrawProductionDlgDiamond draw_diamond;
    void *context;
} StarsDrawProductionDlgOps;
typedef struct StarsDrawProductionDlgInput {
    StarsU16 hwnd;
    StarsU16 hdc;
    StarsRect client;
    StarsI16 line_height;
    StarsI16 max_mineral_width;
    StarsU16 normal_font;
    StarsU16 bold_font;
    StarsU32 face_color;
    StarsU32 text_color;
    StarsRect *diamond_rect;
    /* These are the recovered packed _prod arrays behind the two listboxes.
     * The counts are the byte-3 item counts, not capacities. */
    const StarsU32 *source_items;
    StarsU16 source_item_count;
    const StarsU32 *planet_items;
    StarsU16 planet_item_count;
    /* Recovered rgszMinerals table at DGROUP:04cc.  It contains six near
     * pointers; the dialog uses entries 0, 1, 2, and 5. */
    const StarsU8 *const *mineral_labels;
    StarsU16 mineral_label_count;
    const StarsDrawProductionDlgOps *ops;
} StarsDrawProductionDlgInput;
void stars_draw_production_dlg(StarsDrawProductionDlgInput *, StarsI16);
typedef struct StarsPlanetProductionData {
    StarsI16 has_production;
    StarsI16 queue_count;
    StarsI16 selected_item;
    StarsU8 planet_name[64];
    StarsU8 eta[96];
} StarsPlanetProductionData;
typedef struct StarsPlanetProductionTile { StarsU16 flags; } StarsPlanetProductionTile;
typedef const StarsU8 *(*StarsPlanetProductionString)(void *, StarsU16);
typedef StarsI16 (*StarsPlanetProductionDrawTile)(void *, StarsU16,
    StarsPlanetProductionTile *, const StarsU8 *, StarsRect *);
typedef void (*StarsPlanetProductionShow)(void *, StarsU16, StarsU16);
typedef void (*StarsPlanetProductionEnable)(void *, StarsU16, StarsU16);
typedef void (*StarsPlanetProductionSetPos)(void *, StarsU16, StarsU16,
    StarsI16, StarsI16, StarsI16, StarsI16, StarsU16);
typedef void (*StarsPlanetProductionGetRect)(void *, StarsU16, StarsRect *);
typedef StarsI16 (*StarsPlanetProductionSelection)(void *, StarsU16);
typedef void (*StarsPlanetProductionSelectFont)(void *, StarsU16, StarsU16);
typedef StarsI16 (*StarsPlanetProductionExtent)(void *, StarsU16,
    const StarsU8 *, StarsI16);
typedef void (*StarsPlanetProductionText)(void *, StarsU16, StarsI16,
    StarsI16, const StarsU8 *, StarsI16);
typedef void (*StarsPlanetProductionRight)(void *, StarsU16, StarsI16,
    StarsI16, const StarsU8 *, StarsI16, StarsI16);
typedef void (*StarsPlanetProductionButton)(void *, StarsU16, const StarsRect *,
    StarsI16, StarsI16);
typedef struct StarsPlanetProductionOps {
    StarsPlanetProductionString compressed_string;
    StarsPlanetProductionDrawTile draw_tile;
    StarsPlanetProductionShow show_window;
    StarsPlanetProductionEnable enable_window;
    StarsPlanetProductionSetPos set_window_pos;
    StarsPlanetProductionGetRect get_client_rect;
    StarsPlanetProductionSelection selection;
    StarsPlanetProductionSelectFont select_font;
    StarsPlanetProductionExtent text_extent;
    StarsPlanetProductionText text_out;
    StarsPlanetProductionRight right_text;
    StarsPlanetProductionButton draw_button;
    void *context;
} StarsPlanetProductionOps;
typedef struct StarsPlanetProductionInput {
    StarsU16 hdc;
    StarsI16 line_height;
    StarsI16 compact_rows;
    StarsU16 normal_font;
    StarsU16 bold_font;
    StarsU16 list_window;
    StarsU16 buttons[2];
    StarsRect *reference_rect;
    StarsPlanetProductionTile *tile;
    const StarsPlanetProductionData *data;
    const StarsPlanetProductionOps *ops;
} StarsPlanetProductionInput;
void stars_draw_planet_production(StarsPlanetProductionInput *);
typedef struct StarsZipProductionQueue {
    StarsU8 name[13];
    StarsU8 valid;
    StarsU8 no_research;
    StarsU8 item_count;
    StarsU16 items[12];
} StarsZipProductionQueue;
typedef StarsU16 (*StarsZipGetDialogItem)(void *, StarsU16, StarsU16);
typedef void (*StarsZipGetClientRect)(void *, StarsU16, StarsRect *);
typedef void (*StarsZipInvalidateRect)(void *, StarsU16, const StarsRect *,
    StarsU16);
typedef void (*StarsZipSendMessage)(void *, StarsU16, StarsU16, StarsU16,
    const StarsU8 *);
typedef const StarsU8 *(*StarsZipCompressedString)(void *, StarsU16);
typedef void (*StarsZipGetString)(void *, StarsU16, StarsU8 *, StarsU16);
typedef void (*StarsZipCopyString)(void *, StarsU8 *, const StarsU8 *);
typedef void (*StarsZipFormatString)(void *, StarsU8 *, const StarsU8 *,
    const StarsU8 *, StarsU16);
typedef struct StarsFillZipProdInput {
    StarsI16 static_y;
    StarsI16 line_height;
    StarsZipGetDialogItem get_dialog_item;
    StarsZipGetClientRect get_client_rect;
    StarsZipInvalidateRect invalidate_rect;
    StarsZipSendMessage send_message;
    StarsZipCompressedString compressed_string;
    StarsZipGetString get_string;
    StarsZipCopyString copy_string;
    StarsZipFormatString format_string;
    void *context;
} StarsFillZipProdInput;
void stars_fill_zip_prod_lb(StarsU16, const StarsZipProductionQueue *,
    const StarsFillZipProdInput *);
typedef void (*StarsZipProdDlgGetWindowRect)(void *, StarsU16, StarsRect *);
typedef void (*StarsZipProdDlgGetClientRect)(void *, StarsU16, StarsRect *);
typedef void (*StarsZipProdDlgSetWindowPos)(void *, StarsU16, StarsI16,
    StarsI16, StarsI16, StarsI16, StarsU16);
typedef StarsU16 (*StarsZipProdDlgBeginPaint)(void *, StarsU16, StarsRect *);
typedef void (*StarsZipProdDlgEndPaint)(void *, StarsU16, StarsRect *);
typedef void (*StarsZipProdDlgMapWindowPoints)(void *, StarsU16, StarsU16,
    StarsRect *, StarsU16);
typedef void (*StarsZipProdDlgExpandRect)(void *, StarsRect *, StarsI16,
    StarsI16);
typedef void (*StarsZipProdDlgDrawFrame)(void *, StarsU16, const StarsRect *,
    StarsI16);
typedef void (*StarsZipProdDlgSelectObject)(void *, StarsU16, StarsU16);
typedef void (*StarsZipProdDlgSetBkColor)(void *, StarsU16, StarsU32);
typedef void (*StarsZipProdDlgFillRect)(void *, StarsU16, const StarsRect *,
    StarsU16);
typedef void (*StarsZipProdDlgTextOut)(void *, StarsU16, StarsI16, StarsI16,
    const StarsU8 *, StarsI16);
typedef void (*StarsZipProdDlgSetWindowText)(void *, StarsU16,
    const StarsU8 *);
typedef StarsI32 (*StarsZipProdDlgSendMessage)(void *, StarsU16, StarsU16,
    StarsU16, StarsU32);
typedef void (*StarsZipProdDlgCheckRadio)(void *, StarsU16, StarsU16,
    StarsU16, StarsU16);
typedef void (*StarsZipProdDlgEnableButtons)(void *, StarsU16, StarsI16);
typedef void (*StarsZipProdDlgFillList)(void *, StarsU16,
    const StarsZipProductionQueue *);
typedef StarsI16 (*StarsZipProdDlgRename)(void *, StarsI16, StarsU8 *,
    StarsU16);
typedef StarsI16 (*StarsZipProdDlgGetProduction)(void *, StarsI16,
    StarsU16 *, StarsU16 *);
typedef StarsU16 (*StarsZipProdDlgResourceTech)(void *);
typedef const StarsU8 *(*StarsZipProdDlgCompressed)(void *, StarsU16);
typedef StarsI16 (*StarsZipProdDlgGetString)(void *, StarsU16, StarsU8 *,
    StarsU16);
typedef void (*StarsZipProdDlgFormatDefault)(void *, StarsU8 *,
    const StarsU8 *, StarsI16);
typedef void (*StarsZipProdDlgGetDialogText)(void *, StarsU16, StarsU16,
    StarsU8 *, StarsU16);
typedef void (*StarsZipProdDlgSetFocus)(void *, StarsU16);
typedef void (*StarsZipProdDlgSticky)(void *, StarsU16, StarsPoint *, StarsI16);
typedef void (*StarsZipProdDlgEndDialog)(void *, StarsU16, StarsU16);
typedef void (*StarsZipProdDlgWinHelp)(void *, StarsU16, const StarsU8 *,
    StarsU16);
typedef void (*StarsZipProdDlgAdvanceTutor)(void *);
typedef struct StarsZipProdDlgState {
    StarsZipProductionQueue queues[5];
    StarsPoint sticky_position;
    StarsU8 work[80];
    StarsU16 dialog_window;
    StarsU16 frame_window;
    StarsU16 button_face_brush;
    StarsU32 button_face_color;
    StarsI16 arial8_height;
    StarsI16 static_y;
    StarsI16 selected_queue;
    StarsI16 resource_tech;
    StarsI16 queue_count;
    StarsI16 game_bits;
    StarsI16 help_id;
    StarsU16 tech_label_id[4];
    StarsI16 dirty;
} StarsZipProdDlgState;
typedef struct StarsZipProdDlgOps {
    StarsZipProdDlgGetClientRect get_client_rect;
    StarsZipProdDlgGetWindowRect get_window_rect;
    StarsZipProdDlgSetWindowPos set_window_pos;
    StarsZipProdDlgBeginPaint begin_paint;
    StarsZipProdDlgEndPaint end_paint;
    StarsZipProdDlgMapWindowPoints map_window_points;
    StarsZipProdDlgExpandRect expand_rect;
    StarsZipProdDlgDrawFrame draw_frame;
    StarsZipProdDlgSelectObject select_object;
    StarsZipProdDlgSetBkColor set_background_color;
    StarsZipProdDlgFillRect fill_rect;
    StarsZipProdDlgTextOut text_out;
    StarsZipProdDlgSetWindowText set_window_text;
    StarsZipProdDlgSendMessage send_message;
    StarsZipProdDlgCheckRadio check_radio_button;
    StarsZipProdDlgEnableButtons enable_buttons;
    StarsZipProdDlgFillList fill_list;
    StarsZipProdDlgRename rename_dialog;
    StarsZipProdDlgGetProduction get_production;
    StarsZipProdDlgResourceTech resource_tech;
    StarsZipProdDlgCompressed compressed_string;
    StarsZipProdDlgGetString get_string;
    StarsZipProdDlgFormatDefault format_default;
    StarsZipGetDialogItem get_dialog_item;
    StarsZipProdDlgGetDialogText get_dialog_text;
    StarsZipProdDlgSetFocus set_focus;
    StarsZipProdDlgSticky sticky_position;
    StarsZipProdDlgEndDialog end_dialog;
    StarsZipProdDlgWinHelp win_help;
    StarsZipProdDlgAdvanceTutor advance_tutor;
    void *context;
} StarsZipProdDlgOps;
typedef struct StarsZipProdDlgInput {
    StarsZipProdDlgState *state;
    const StarsZipProdDlgOps *ops;
} StarsZipProdDlgInput;
StarsI16 stars_zip_prod_dlg(StarsZipProdDlgInput *, StarsU16, StarsU16,
    StarsU16, StarsU32);
/* The production list stores its count at byte 3 and packed _prod words
 * beginning at byte 4.  The dialog only consumes these recorded fields. */
typedef struct StarsProductionList {
    StarsU8 flags[3];
    StarsU8 item_count;
    StarsU32 items[128];
} StarsProductionList;
typedef struct StarsProductionPlanet {
    StarsU16 id;
    StarsU32 flags;
} StarsProductionPlanet;
typedef const StarsU8 *(*StarsProductionPlanetName)(void *, StarsU16);
typedef const StarsU8 *(*StarsProductionCompressedString)(void *, StarsU16);
typedef void (*StarsProductionFormatTitle)(void *, StarsU8 *, const StarsU8 *,
    const StarsU8 *);
typedef void (*StarsProductionFillSource)(void *, StarsU16, StarsI16);
typedef void (*StarsProductionFillPlanet)(void *, StarsU16,
    const StarsProductionList *, const StarsProductionPlanet *);
typedef void (*StarsProductionSetWindowText)(void *, StarsU16, const StarsU8 *);
typedef StarsU16 (*StarsProductionGetDialogItem)(void *, StarsU16, StarsU16);
typedef void (*StarsProductionSendMessage)(void *, StarsU16, StarsU16,
    StarsU16, StarsU32);
typedef struct StarsInitializeProductionDlgInput {
    const StarsProductionList *global_queue;
    const StarsProductionPlanet *selected_planet;
    StarsProductionPlanetName planet_name;
    StarsProductionCompressedString compressed_string;
    StarsProductionFormatTitle format_title;
    StarsProductionFillSource fill_source;
    StarsProductionFillPlanet fill_planet;
    StarsProductionSetWindowText set_window_text;
    StarsProductionGetDialogItem get_dialog_item;
    StarsProductionSendMessage send_message;
    void *context;
} StarsInitializeProductionDlgInput;
void stars_initialize_production_dlg(StarsU16,
    const StarsInitializeProductionDlgInput *);
typedef void (*StarsInitProductionAdvanceTutor)(void *);
typedef struct StarsInitProductionInput {
    StarsProductionList *available;
    StarsProductionList *queue;
    const StarsU32 *ship_items;
    StarsU16 ship_item_count;
    StarsU16 factories_current;
    StarsU16 factories_max;
    StarsU16 mines_current;
    StarsU16 mines_max;
    StarsU16 defenses_current;
    StarsU16 defenses_max;
    /* Legacy fields retained for callers; eligibility belongs in ship_items
     * and starbase_items, and automatic choices derive from race_stat. */
    StarsU16 starbase_present;
    StarsU16 starbase_cargo_capacity;
    StarsU16 warp_gate_count; /* nonzero means positive mass-driver warp */
    StarsU16 terraform_count;
    StarsU16 race_stat;
    StarsU16 research_mask; /* legacy, no longer overrides race rules */
    StarsU16 tutorial_player;
    StarsInitProductionAdvanceTutor advance_tutor;
    void *context;
    /* Eligible starbase design words (group 2, indices 16..25), in slot order.
     * ship_items likewise contains eligible group-2 ship design words. */
    const StarsU32 *starbase_items;
    StarsU16 starbase_item_count;
    StarsI16 planetary_part_14_lookup;
    StarsU16 scanner_type;
    StarsU16 game_flags;
} StarsInitProductionInput;
void stars_init_production(StarsInitProductionInput *);
typedef void (*StarsProdCommandFinish)(void *, StarsI16);
typedef void (*StarsProdCommandSticky)(void *, StarsU16);
typedef void (*StarsProdCommandEnd)(void *, StarsU16, StarsI16);
typedef void (*StarsProdCommandHelp)(void *, StarsU16, StarsU16);
typedef void (*StarsProdCommandToggle)(void *, StarsI16);
typedef void (*StarsProdCommandFill)(void *, StarsU16,
    const StarsProductionList *);
typedef void (*StarsProdCommandSelect)(void *, StarsI16, StarsI16);
typedef void (*StarsProdCommandInitialize)(void *);
typedef void (*StarsProdCommandNavigate)(void *, StarsI16, StarsI16);
typedef void (*StarsProdCommandDraw)(void *, StarsU16);
typedef void (*StarsProdCommandInvalidate)(void *, StarsU16);
typedef struct StarsProdCommandOps {
    StarsProdCommandFinish finish;
    StarsProdCommandSticky sticky;
    StarsProdCommandEnd end_dialog;
    StarsProdCommandHelp help;
    StarsProdCommandToggle toggle_research;
    StarsProdCommandFill fill_queue;
    StarsProdCommandSelect select_item;
    StarsProdCommandInitialize initialize;
    StarsProdCommandNavigate navigate;
    StarsProdCommandDraw draw;
    StarsProdCommandInvalidate invalidate;
    void *context;
} StarsProdCommandOps;
typedef struct StarsProdCommandInput {
    StarsU16 dialog;
    StarsU16 message;
    StarsU32 parameter;
    StarsI16 selected_index;
    StarsI16 shift_down;
    StarsI16 control_down;
    StarsProductionList *queue;
    const StarsProductionList *catalog;
    const StarsProdCommandOps *ops;
} StarsProdCommandInput;
void stars_prod_command_handler(StarsProdCommandInput *);
typedef StarsU16 (*StarsProductionDlgBeginPaint)(void *, StarsU16, StarsRect *);
typedef void (*StarsProductionDlgEndPaint)(void *, StarsU16, StarsRect *);
typedef void (*StarsProductionDlgGetClientRect)(void *, StarsU16, StarsRect *);
typedef void (*StarsProductionDlgFillRect)(void *, StarsU16, const StarsRect *,
    StarsU16);
typedef StarsU16 (*StarsProductionDlgGetItem)(void *, StarsU16, StarsU16);
typedef void (*StarsProductionDlgEnable)(void *, StarsU16, StarsU16);
typedef void (*StarsProductionDlgSetPos)(void *, StarsU16, StarsU16,
    StarsI16, StarsI16, StarsI16, StarsI16, StarsU16);
typedef void (*StarsProductionDlgGetWindowRect)(void *, StarsU16, StarsRect *);
typedef void (*StarsProductionDlgMapRect)(void *, StarsU16, StarsRect *);
typedef StarsI16 (*StarsProductionDlgPointInRect)(void *, const StarsRect *,
    StarsPoint);
typedef void (*StarsProductionDlgGetCursor)(void *, StarsPoint *);
typedef void (*StarsProductionDlgSetCursor)(void *, StarsU16);
typedef void (*StarsProductionDlgDraw)(void *, StarsU16, StarsU16,
    const StarsRect *, StarsI16);
typedef void (*StarsProductionDlgDrawItem)(void *, StarsU32, StarsI16);
typedef void (*StarsProductionDlgSetMeasure)(void *, StarsU32, StarsI16);
typedef void (*StarsProductionDlgCommand)(void *, StarsU16, StarsU16,
    StarsU32);
typedef void (*StarsProductionDlgPopup)(void *, StarsU16, StarsI16, StarsI16);
typedef StarsI16 (*StarsProductionDlgPopupMenu)(void *, StarsU16, StarsI16,
    StarsI16);
typedef StarsI16 (*StarsProductionDlgPopupMenuItems)(void *, StarsU16,
    StarsI16, StarsI16, const StarsU8 *const *, StarsU16, const StarsU8 *);
typedef void (*StarsProductionDlgSelectZip)(void *, StarsU16, StarsI16);
typedef void (*StarsProductionDlgOpenZipDialog)(void *, StarsU16);
typedef StarsI16 (*StarsProductionDlgEditZipDialog)(void *, StarsU16,
    StarsZipProductionQueue *, StarsU16);
typedef void (*StarsProductionDlgSticky)(void *, StarsU16, StarsI16);
typedef StarsI16 (*StarsProductionDlgCopyString)(void *, StarsU16, StarsU8 *,
    StarsU16);
typedef const StarsU8 *(*StarsProductionDlgCompressedString)(void *,
    StarsU16);
typedef struct StarsProductionDlgState {
    StarsU16 hwnd;
    StarsU16 button_face_brush;
    StarsU32 button_face_color;
    StarsU16 help_cursor;
    StarsI16 arial8_height;
    StarsI16 selected_player_planets;
    StarsI16 compact_mode;
    StarsI16 zip_valid_count;
    StarsZipProductionQueue *zip_queues;
    StarsU16 zip_queue_capacity;
    StarsRect diamond;
    StarsPoint sticky_position;
    StarsU8 popup_text[64];
} StarsProductionDlgState;
/* The original ProductionDlg consumes the five-record vrgZipProd table
 * directly.  Keep the raw 40-byte Win16 record boundary explicit at the
 * caller seam before the typed dialog state is used. */
void stars_production_dlg_decode_zip_queues(StarsZipProductionQueue *,
    StarsU16, const StarsU8 *, StarsU16);
typedef struct StarsProductionDlgOps {
    StarsProductionDlgBeginPaint begin_paint;
    StarsProductionDlgEndPaint end_paint;
    StarsProductionDlgGetClientRect get_client_rect;
    StarsProductionDlgFillRect fill_rect;
    StarsProductionDlgGetItem get_dialog_item;
    StarsProductionDlgEnable enable_window;
    StarsProductionDlgSetPos set_window_pos;
    StarsProductionDlgGetWindowRect get_window_rect;
    StarsProductionDlgMapRect screen_to_client;
    StarsProductionDlgPointInRect point_in_rect;
    StarsProductionDlgGetCursor get_cursor_pos;
    StarsProductionDlgSetCursor set_cursor;
    StarsProductionDlgDraw draw_dialog;
    StarsProductionDlgDrawItem draw_focus_item;
    StarsProductionDlgDrawItem draw_list_item;
    StarsProductionDlgSetMeasure set_measure_height;
    StarsProductionDlgCommand command_handler;
    StarsProductionDlgPopup popup;
    StarsProductionDlgPopupMenu popup_menu;
    StarsProductionDlgPopupMenuItems popup_menu_items;
    StarsProductionDlgSelectZip select_zip;
    StarsProductionDlgOpenZipDialog open_zip_dialog;
    StarsProductionDlgEditZipDialog edit_zip_dialog;
    StarsProductionDlgSticky sticky_position;
    StarsProductionDlgCommand initialize_dialog;
    StarsProductionDlgCopyString copy_popup_string;
    StarsProductionDlgCompressedString get_compressed_string;
    void *context;
} StarsProductionDlgOps;
typedef struct StarsProductionDlgInput {
    StarsProductionDlgState *state;
    const StarsProductionDlgOps *ops;
} StarsProductionDlgInput;
StarsI16 stars_production_dlg(StarsProductionDlgInput *, StarsU16, StarsU16,
    StarsU16, StarsU32);
typedef struct StarsFillBuildDesign {
    StarsI16 visible;
    StarsI16 queued;
    StarsI16 has_hull;
    StarsI16 has_components;
    StarsU8 name[64];
} StarsFillBuildDesign;
typedef void (*StarsFillBuildSend)(void *, StarsU16, StarsU16, StarsU16,
    const StarsU8 *);
typedef void (*StarsFillBuildEnable)(void *, StarsU16, StarsU16);
typedef StarsU16 (*StarsFillBuildItem)(void *, StarsU16, StarsU16);
typedef StarsI16 (*StarsFillBuildPartName)(void *, StarsI16, StarsI16,
    StarsU8 *, StarsU16);
typedef StarsI16 (*StarsFillBuildPlayerName)(void *, StarsI16, StarsU8 *,
    StarsU16);
typedef StarsI16 (*StarsFillBuildQueued)(void *, StarsI16, StarsI16 *);
typedef StarsI16 (*StarsFillBuildCount)(void *, StarsU16);
typedef const StarsU8 *(*StarsFillBuildString)(void *, StarsU16);
typedef void (*StarsFillBuildRect)(void *, StarsU16, StarsRect *);
typedef void (*StarsFillBuildPosition)(void *, StarsU16, StarsU16, StarsI16,
    StarsI16, StarsI16, StarsI16, StarsU16);
typedef struct StarsFillBuildInput {
    StarsU16 dropdown;
    StarsU16 slot_dialog;
    StarsI16 starbase_mode;
    StarsI16 mode;
    StarsI16 current_player;
    StarsI16 player_count;
    StarsI16 line_height;
    StarsI16 parent_width;
    const StarsFillBuildDesign *designs;
    const StarsFillBuildDesign *player_designs;
    StarsU16 designs_per_player;
    const StarsU8 *packed_designs;
    StarsI16 packed_design_stride;
    const StarsU8 *packed_player_designs;
    StarsI16 packed_player_design_stride;
    StarsFillBuildSend send_message;
    StarsFillBuildEnable enable_window;
    StarsFillBuildItem get_dialog_item;
    StarsFillBuildPartName part_name;
    StarsFillBuildPlayerName player_name;
    StarsFillBuildQueued queued;
    StarsFillBuildCount list_count;
    StarsFillBuildString compressed_string;
    StarsFillBuildRect get_window_rect;
    StarsFillBuildPosition set_window_pos;
    void *context;
} StarsFillBuildInput;
void stars_fill_build_dropdown(const StarsFillBuildInput *);
typedef struct StarsEstimateProdItem {
    StarsU32 packed;
    StarsU16 quantity;
    StarsI16 progress_kind;
    StarsI16 max_progress;
} StarsEstimateProdItem;
typedef struct StarsEstimateProdPlanet {
    StarsI32 minerals[3];
    StarsI32 resources;
    StarsI16 research_percent;
    StarsU32 flags;
    StarsI16 population;
    StarsU16 mines, factories;
} StarsEstimateProdPlanet;
/* Return newly mined quantities; the scheduler adds them to its local stock. */
typedef void (*StarsEstimateProdMined)(void *, StarsEstimateProdPlanet *,
    StarsI32[3]);
typedef StarsI32 (*StarsEstimateProdResource)(void *,
    const StarsEstimateProdPlanet *);
typedef StarsI16 (*StarsEstimateProdBuild)(void *, StarsEstimateProdPlanet *,
    const StarsEstimateProdItem *, StarsEstimateProdItem *, const StarsI32[3],
    StarsI16, StarsI16 *);
/* Mutable simulation boundary for CBuildProdItem, including resources. */
typedef StarsI16 (*StarsEstimateProdBuildMutable)(void *, StarsEstimateProdPlanet *,
    StarsEstimateProdItem *, StarsEstimateProdItem *, StarsI32[4],
    StarsI16, StarsI16 *);
typedef void (*StarsEstimateProdPopulation)(void *, StarsEstimateProdPlanet *,
    StarsI16);
typedef struct StarsEstimateProdInput {
    StarsEstimateProdPlanet *planet;
    StarsEstimateProdItem *queue;
    StarsU16 queue_count;
    StarsEstimateProdItem partial_item; /* Legacy; simulation starts empty. */
    StarsI16 item_index;
    StarsI16 *first_turn;
    StarsI16 *last_turn;
    StarsEstimateProdMined estimate_mined;
    StarsEstimateProdResource resources_at_planet;
    StarsEstimateProdBuild build_item;
    StarsEstimateProdPopulation change_population;
    void *context;
    StarsEstimateProdBuildMutable build_item_mutable;
} StarsEstimateProdInput;
void stars_estimate_item_prod_schedule(StarsEstimateProdInput *);
typedef struct StarsHullDefinitionInput {
    StarsU16 base_value;
    const StarsHullComponent *components;
    StarsU16 component_count;
} StarsHullDefinitionInput;
typedef StarsI16 (*StarsHullPartDamage)(void *, const StarsHullComponent *);
StarsI16 stars_init_from_huldef(const StarsHullDefinitionInput *, StarsI16 *,
    StarsHullPartDamage, void *);
typedef struct StarsRetargetMinerCandidate {
    StarsI16 planet_id;
    StarsPoint point;
    StarsU8 weight_a;
    StarsU8 weight_b;
    StarsU8 weight_c;
} StarsRetargetMinerCandidate;
typedef struct StarsRetargetMinerInput {
    StarsI16 current_planet;
    StarsPoint fleet_point;
    const StarsRetargetMinerCandidate *candidates;
    StarsU16 candidate_count;
} StarsRetargetMinerInput;
typedef StarsI16 (*StarsRetargetMinerMove)(void *, const StarsPoint *,
    const StarsMoveOrder *, StarsI16);
StarsI16 stars_f_retarget_miner(const StarsRetargetMinerInput *,
    StarsRetargetMinerMove, void *);
typedef struct StarsClimatePlanet {
    StarsI16 id;
    StarsI16 player;
    StarsU32 climate_metric;
    StarsI16 environment[3];
    StarsI16 original_environment[3];
} StarsClimatePlanet;
typedef StarsI16 (*StarsClimateRandom)(void *, StarsI16);
typedef void (*StarsClimateMessage)(void *, StarsI16, StarsU16, StarsU16,
    StarsU16, StarsI16);
typedef void (*StarsClimateTossBuildItems)(void *, StarsClimatePlanet *);
typedef struct StarsPlanetaryClimateInput {
    StarsClimatePlanet *planets;
    StarsU16 planet_count;
    StarsI16 turn;
    StarsClimateRandom random;
    StarsClimateMessage send_message;
    StarsClimateTossBuildItems toss_build_items;
    void *context;
} StarsPlanetaryClimateInput;
void stars_planetary_climate_change(StarsPlanetaryClimateInput *);
typedef struct StarsBattleUnitToken {
    StarsI16 owner;
    StarsU16 design;
    StarsU16 hull_category;
    StarsU32 battle_units;
} StarsBattleUnitToken;
typedef struct StarsBattleUnitsInput {
    StarsI16 local_player;
    const StarsBattleUnitToken *tokens;
    StarsU16 token_count;
} StarsBattleUnitsInput;
typedef StarsU16 (*StarsBattleUnitHullCategory)(void *, StarsI16, StarsU16);
StarsU32 stars_c_battle_units(const StarsBattleUnitsInput *, StarsU16,
    StarsBattleUnitHullCategory, void *);
typedef void (*StarsRenameGetClientRect)(void *, StarsU16, StarsRect *);
typedef void (*StarsRenameFillRect)(void *, StarsU16, const StarsRect *,
    StarsU32);
typedef void (*StarsRenameSetBkColor)(void *, StarsU16, StarsU32);
typedef const StarsU8 *(*StarsRenameGetString)(void *, StarsU16);
typedef void (*StarsRenameSetWindowText)(void *, StarsU16, const StarsU8 *);
typedef void (*StarsRenameSetWindowPos)(void *, StarsU16, StarsI16, StarsI16,
    StarsU16);
typedef void (*StarsRenameSendDlgItemMessage)(void *, StarsU16, StarsU16,
    StarsU16, StarsU16, StarsU32);
typedef StarsU16 (*StarsRenameGetDialogItem)(void *, StarsU16, StarsU16);
typedef void (*StarsRenameGetDialogText)(void *, StarsU16, StarsU16,
    StarsU8 *, StarsU16);
typedef void (*StarsRenameStickyPosition)(void *, StarsU16, StarsPoint *,
    StarsI16);
typedef void (*StarsRenameEndDialog)(void *, StarsU16, StarsU16);
typedef void (*StarsRenameWinHelp)(void *, StarsU16, const StarsU8 *, StarsU16);
typedef struct StarsRenameZipInput {
    StarsU16 existing_order_dialog;
    StarsPoint sticky_position;
    StarsU32 button_face_brush;
    StarsU32 button_face_color;
    StarsU8 work[14];
    const StarsU8 *help_file;
    StarsRenameGetClientRect get_client_rect;
    StarsRenameFillRect fill_rect;
    StarsRenameSetBkColor set_background_color;
    StarsRenameGetString get_string;
    StarsRenameSetWindowText set_window_text;
    StarsRenameSetWindowPos set_window_pos;
    StarsRenameSendDlgItemMessage send_dialog_item_message;
    StarsRenameGetDialogItem get_dialog_item;
    StarsRenameGetDialogText get_dialog_text;
    StarsRenameStickyPosition sticky_position_call;
    StarsRenameEndDialog end_dialog;
    StarsRenameWinHelp win_help;
    void *context;
} StarsRenameZipInput;
StarsI16 stars_rename_zip_dlg(StarsU16, StarsU16, StarsU16, StarsU32,
    StarsRenameZipInput *);
typedef StarsI16 (*StarsRenameFitsScreen)(void *, const StarsU8 *, StarsU16);
typedef void (*StarsRenameGetWindowText)(void *, StarsU16, StarsU8 *, StarsU16);
typedef StarsU32 (*StarsRenameSendMessage)(void *, StarsU16, StarsU16,
    StarsU16, StarsU32);
typedef struct StarsRenameInput {
    StarsPoint sticky_position;
    StarsU32 button_face_brush;
    StarsU32 button_face_color;
    StarsU8 work[250];
    const StarsU8 *help_file;
    StarsU16 edit_update;
    StarsRenameGetClientRect get_client_rect;
    StarsRenameFillRect fill_rect;
    StarsRenameSetBkColor set_background_color;
    StarsRenameGetString get_string;
    StarsRenameSetWindowText set_window_text;
    StarsRenameSetWindowPos set_window_pos;
    StarsRenameSendDlgItemMessage send_dialog_item_message;
    StarsRenameGetDialogItem get_dialog_item;
    StarsRenameGetDialogText get_dialog_text;
    StarsRenameGetWindowText get_window_text;
    StarsRenameSendMessage send_message;
    StarsRenameFitsScreen fits_screen;
    StarsRenameStickyPosition sticky_position_call;
    StarsRenameEndDialog end_dialog;
    StarsRenameWinHelp win_help;
    void *context;
} StarsRenameInput;
StarsI16 stars_rename_dlg(StarsU16, StarsU16, StarsU16, StarsU32,
    StarsRenameInput *);
typedef struct StarsLinkFleet {
    StarsI16 x, y;
    StarsU16 flags;
    struct StarsLinkFleet *next;
} StarsLinkFleet;
void stars_link_fleet_records(StarsLinkFleet **, StarsU16, StarsU16 *);
typedef struct StarsDoBattlesFleet {
    StarsU16 flags;
    StarsU16 has_next;
} StarsDoBattlesFleet;
typedef void *(*StarsDoBattlesAlloc)(void *, StarsU16, StarsU16);
typedef void (*StarsDoBattlesFree)(void *, void *, StarsU16);
typedef void (*StarsDoBattlesLinkFleets)(void *, StarsI16);
typedef StarsI16 (*StarsDoBattlesCplrBattle)(void *,
    const StarsDoBattlesFleet *, StarsU16 *, StarsU16 *, StarsU16 *);
typedef void (*StarsDoBattlesCoolBattle)(void *, const StarsDoBattlesFleet *,
    StarsI16, const StarsU16 *, StarsU16, StarsU16);
typedef void (*StarsDoBattlesBombing)(void *);
typedef struct StarsDoBattlesInput {
    StarsDoBattlesFleet *fleets;
    StarsU16 fleet_count;
    StarsDoBattlesLinkFleets link_fleets;
    StarsDoBattlesAlloc allocate;
    StarsDoBattlesFree free_memory;
    StarsDoBattlesCplrBattle cplr_battle;
    StarsDoBattlesCoolBattle cool_battle;
    StarsDoBattlesBombing do_bombing;
    void *battle_table;
    StarsU8 *battle_current;
    void *context;
} StarsDoBattlesInput;
void stars_do_battles(StarsI16, StarsDoBattlesInput *);
typedef struct StarsTooltipState {
    StarsU16 window;
    StarsU16 visible;
    StarsI16 current_id;
    StarsU32 last_tick;
    StarsRect rectangle;
    StarsI16 width;
} StarsTooltipState;
typedef StarsI16 (*StarsTooltipGetString)(void *, StarsI16, StarsU8 *);
typedef StarsU16 (*StarsTooltipGetDC)(void *, StarsU16);
typedef StarsU16 (*StarsTooltipSelectObject)(void *, StarsU16, StarsU16);
typedef StarsU32 (*StarsTooltipTextExtent)(void *, StarsU16, const StarsU8 *,
    StarsI16);
typedef StarsU32 (*StarsTooltipGetTick)(void *);
typedef void (*StarsTooltipSetWindowPos)(void *, StarsU16, StarsI16, StarsI16,
    StarsI16, StarsI16, StarsU16);
typedef void (*StarsTooltipWndProc)(void *, StarsU16, StarsU16, StarsU16,
    StarsU32);
typedef struct StarsTooltipOps {
    StarsU16 font;
    StarsI16 font_height;
    StarsTooltipGetString get_string;
    StarsTooltipGetDC get_dc;
    StarsTooltipSelectObject select_object;
    StarsTooltipTextExtent text_extent;
    StarsTooltipGetTick get_tick;
    StarsTooltipSetWindowPos set_window_pos;
    StarsTooltipWndProc window_proc;
    StarsU16 (*is_visible)(void *, StarsU16);
    void (*destroy_window)(void *, StarsU16);
    void (*release_dc)(void *, StarsU16, StarsU16);
    StarsU16 (*create_window)(void *, StarsI16, StarsI16);
    void (*invalidate_window)(void *, StarsU16, StarsU16);
    void *context;
} StarsTooltipOps;
void stars_show_tooltip(StarsI16, const StarsRect *, StarsTooltipState *,
    const StarsTooltipOps *);
typedef StarsU16 (*StarsTooltipWndBeginPaint)(void *, StarsU16, StarsRect *);
typedef void (*StarsTooltipWndEndPaint)(void *, StarsU16, const StarsRect *);
typedef void (*StarsTooltipWndClientRect)(void *, StarsU16, StarsRect *);
typedef void (*StarsTooltipWndFillRect)(void *, StarsU16, const StarsRect *,
    StarsU16);
typedef void (*StarsTooltipWndFrameRect)(void *, StarsU16, const StarsRect *,
    StarsU16);
typedef StarsU16 (*StarsTooltipWndSelectObject)(void *, StarsU16, StarsU16);
typedef StarsI16 (*StarsTooltipWndSetBkMode)(void *, StarsU16, StarsI16);
typedef void (*StarsTooltipWndSetTextColor)(void *, StarsU16, StarsU32);
typedef void (*StarsTooltipWndTextOut)(void *, StarsU16, StarsI16, StarsI16,
    const StarsU8 *, StarsI16);
typedef StarsI16 (*StarsTooltipWndTimer)(void *, StarsU16, StarsU16, StarsU16);
typedef StarsU32 (*StarsTooltipWndTick)(void *);
typedef void (*StarsTooltipWndCursor)(void *, StarsPoint *);
typedef StarsI16 (*StarsTooltipWndPointInRect)(void *, const StarsRect *,
    StarsPoint);
typedef void (*StarsTooltipWndMapPoint)(void *, StarsU16, StarsPoint *);
typedef void (*StarsTooltipWndPosition)(void *, StarsU16, StarsU16, StarsI16,
    StarsI16, StarsI16, StarsI16, StarsU16);
typedef void (*StarsTooltipWndUpdate)(void *, StarsU16);
typedef StarsI32 (*StarsTooltipWndDefaultProc)(void *, StarsU16, StarsU16,
    StarsU16, StarsU32);
typedef struct StarsTooltipWndOps {
    StarsTooltipWndBeginPaint begin_paint;
    StarsTooltipWndEndPaint end_paint;
    StarsTooltipWndClientRect get_client_rect;
    StarsTooltipWndFillRect fill_rect;
    StarsTooltipWndFrameRect frame_rect;
    StarsTooltipWndSelectObject select_object;
    StarsTooltipWndSetBkMode set_background_mode;
    StarsTooltipWndSetTextColor set_text_color;
    StarsTooltipWndTextOut text_out;
    StarsTooltipGetString get_string;
    StarsTooltipWndTimer set_timer;
    StarsTooltipWndTimer kill_timer;
    StarsTooltipWndTick get_tick;
    StarsTooltipWndCursor get_cursor_pos;
    StarsTooltipWndPointInRect point_in_rect;
    StarsTooltipWndMapPoint screen_to_client;
    StarsTooltipWndMapPoint client_to_screen;
    StarsTooltipWndPosition set_window_pos;
    StarsTooltipWndUpdate update_window;
    StarsU16 (*is_visible)(void *, StarsU16);
    void (*destroy_window)(void *, StarsU16);
    StarsTooltipWndDefaultProc default_proc;
    void *context;
} StarsTooltipWndOps;
typedef struct StarsTooltipWndInput {
    StarsU16 tooltip_window;
    StarsI16 tooltip_id;
    StarsI16 timer_id;
    StarsU32 first_visible_tick;
    StarsU32 last_visible_tick;
    StarsRect tooltip_rect;
    StarsU16 frame_window;
    StarsU16 window_frame_brush;
    StarsU16 tooltip_brush;
    StarsU16 font;
    StarsU32 text_color;
    StarsI16 font_height;
    StarsI16 tip_width;
    StarsI16 screen_width;
    StarsTooltipWndOps ops;
} StarsTooltipWndInput;
StarsI32 stars_tooltip_wnd_proc(StarsTooltipWndInput *, StarsU16, StarsU16,
    StarsU16, StarsU32);
typedef struct StarsAutoFleetOrder {
    StarsU16 flags;
    StarsU32 target_token;
} StarsAutoFleetOrder;
typedef struct StarsAutoFleet {
    StarsI16 player;
    StarsPoint point;
    StarsU16 flags;
    StarsU32 token;
    StarsU8 plan_index;
    StarsAutoFleetOrder *order;
} StarsAutoFleet;
typedef struct StarsAutoFleetPlanet {
    StarsI16 player;
} StarsAutoFleetPlanet;
typedef StarsI32 (*StarsAutoFleetMineValue)(void *, const StarsAutoFleet *);
typedef StarsI16 (*StarsAutoFleetRaceStat)(void *, StarsI16, StarsI16);
typedef struct StarsAutoFleetOrderInput {
    StarsAutoFleet *fleet;
    const StarsAutoFleetPlanet *planet;
    StarsAutoFleet *other_fleets;
    StarsU16 other_fleet_count;
    StarsAutoFleetMineValue mine_value;
    StarsAutoFleetRaceStat race_stat;
    void *context;
} StarsAutoFleetOrderInput;
void stars_auto_fleet_order(StarsAutoFleetOrderInput *);
typedef struct StarsPacketDecayThing {
    StarsU16 status_flags;
    StarsI16 owner;
    StarsI16 amounts[3];
} StarsPacketDecayThing;
typedef StarsI16 (*StarsPacketDecayRaceStat)(void *, StarsI16, StarsI16);
typedef void (*StarsPacketDecayFree)(void *, StarsPacketDecayThing *);
typedef struct StarsPacketDecayInput {
    StarsPacketDecayRaceStat race_stat;
    StarsPacketDecayFree free_thing;
    void *context;
} StarsPacketDecayInput;
StarsI16 stars_f_packet_decay(StarsPacketDecayThing *, StarsI16,
    const StarsPacketDecayInput *);
typedef struct StarsTorpHitToken { StarsU16 jam_percent; } StarsTorpHitToken;
typedef StarsI16 (*StarsTorpHitRandom)(void *, StarsI16);
StarsI32 stars_c_torp_hit(StarsI32, const StarsTorpHitToken *, StarsI16,
    StarsI16, StarsTorpHitRandom, void *);
typedef struct StarsShipDropdownFleet {
    StarsI16 id;
    StarsI16 owner;
    StarsI16 planet_id;
    StarsPoint point;
} StarsShipDropdownFleet;
typedef struct StarsShipDropdownThing {
    StarsI16 id;
    StarsI16 kind;
    StarsI16 owner;
    StarsPoint point;
} StarsShipDropdownThing;
typedef struct StarsFillShipSelection {
    StarsU16 selected_object_kind;
    StarsI16 selected_id;
    StarsPoint selected_fleet_point;
    const StarsPoint *planet_points;
} StarsFillShipSelection;
typedef void (*StarsShipDropdownGetName)(void *, StarsI16, StarsU8 *, StarsU16);
typedef void (*StarsShipDropdownSend)(void *, StarsU16, StarsU16, StarsU16,
    const StarsU8 *);
typedef struct StarsFillShipDropdownInput {
    StarsU16 dropdown;
    StarsI16 current_player;
    const StarsFillShipSelection *selection;
    const StarsShipDropdownFleet *fleets;
    StarsU16 fleet_count;
    const StarsShipDropdownThing *things;
    StarsU16 thing_count;
    StarsShipDropdownGetName fleet_name;
    StarsShipDropdownGetName thing_name;
    StarsShipDropdownSend send_message;
    void *context;
} StarsFillShipDropdownInput;
void stars_fill_ship_dd(StarsI16, const StarsFillShipDropdownInput *);
typedef struct StarsAdjacentPlanet {
    StarsI16 id;
    StarsI16 owner;
    StarsU16 visibility_flags;
} StarsAdjacentPlanet;
typedef void (*StarsSelectAdjacentChangeScan)(void *,
    const StarsScanSelection *, StarsI16);
typedef void (*StarsSelectAdjacentRedrawScan)(void *, StarsI16, StarsI16);
typedef void (*StarsSelectAdjacentChangeMain)(void *, StarsI16, StarsI16);
typedef void (*StarsSelectAdjacentCenter)(void *, StarsPoint, StarsI16);
typedef void (*StarsSelectAdjacentDrawBar)(void *);
typedef struct StarsSelectAdjacentOps {
    void (*invalidate_report)(void *, StarsI16, StarsI16);
    StarsSelectAdjacentChangeScan change_scan;
    StarsSelectAdjacentRedrawScan redraw_scan;
    StarsSelectAdjacentChangeMain change_main;
    StarsSelectAdjacentCenter center_scan;
    StarsSelectAdjacentDrawBar draw_scroll_bar;
    void (*invalidate_mine)(void *, StarsU16, StarsU16);
    void (*set_mineral_title)(void *);
    void *context;
} StarsSelectAdjacentOps;
typedef struct StarsSelectAdjacentInput {
    StarsAdjacentPlanet *planets;
    StarsU16 planet_count;
    const StarsPoint *planet_points;
    const StarsI16 *player_planet_ids;
    StarsU16 player_planet_count;
    StarsI16 current_player;
    StarsI16 selected_planet;
    StarsU16 mine_window;
    StarsU16 report_cached;
} StarsSelectAdjacentInput;
void stars_select_adj_planet(StarsI16, StarsI16, StarsSelectAdjacentInput *,
    const StarsSelectAdjacentOps *);
typedef struct StarsGuessPlanet {
    StarsI16 owner;
    StarsU16 guess_word;
    StarsU32 mineral_value;
    StarsU16 survival_flags;
} StarsGuessPlanet;
typedef StarsI16 (*StarsGuessRandom)(void *, StarsI16);
typedef StarsI16 (*StarsGuessRaceStat)(void *, StarsI16, StarsI16);
typedef StarsI16 (*StarsGuessPctSurvive)(void *, const StarsGuessPlanet *);
typedef struct StarsUpdateGuessesInput {
    StarsGuessPlanet *planets;
    StarsU16 planet_count;
    StarsGuessRandom random;
    StarsGuessRaceStat race_stat;
    StarsGuessPctSurvive calc_pct_survive;
    void *context;
} StarsUpdateGuessesInput;
void stars_update_guesses(StarsUpdateGuessesInput *);
void stars_expand_rect(StarsRect *rect, StarsI16 dx, StarsI16 dy);
void stars_offset_rect(StarsRect *rect, StarsI16 dx, StarsI16 dy);
StarsI16 stars_compare_long(StarsU32 first, StarsU32 second);
StarsI16 stars_validate_serial_number(const StarsU8 *, StarsSeeds *, StarsU32 *);
/* dgroup points to a complete 65536-byte near-address space. */
StarsI16 stars_get_race_stat(const StarsU8 *dgroup, StarsU16 player, StarsI16 index);
/* These routines retain the original DGROUP offsets and unchecked stack index. */
void stars_push_random(StarsU8 *dgroup, StarsU32 first, StarsU32 second);
void stars_pop_random(StarsU8 *dgroup);
/* Requires each original word access to remain inside the 64 KiB segment.
 * Retains unsigned end-pointer comparison, even for a negative count. */
void stars_xor_file_buffer(StarsU8 *dgroup, StarsU16 buffer, StarsI16 count);
/* Codec tables are read from original DGROUP; see codec.md for input limits. */
StarsU16 stars_nybble_from_char(const StarsU8 *dgroup, StarsU8 ch);
StarsU8 stars_char_from_nybble(const StarsU8 *dgroup, StarsI16 code);
int stars_compress_user_string(const StarsU8 *dgroup, const StarsU8 *input,
    StarsU8 *output, StarsI16 *capacity);
int stars_decompress_user_string(const StarsU8 *dgroup, const StarsU8 *input,
    StarsI16 length, StarsU8 *output, const StarsI16 *capacity);
void stars_add_back_trailing_spaces(StarsU8 *data, StarsU16 cursor_slot, StarsU16 end);
void stars_chop_trailing_spaces(StarsU8 *data, StarsU16 start, StarsU16 end_slot);
void stars_chop_last_word(StarsU8 *data, StarsU16 start, StarsU16 end_slot);
void stars_int_to_roman(StarsU8 *data, StarsI16 value, StarsU16 output);
StarsI16 stars_parse_numbers(StarsU8 *data, StarsU16 input, StarsU16 output, StarsI16 maximum);
/* pointer_slot holds an offset:selector pointing into data; selector retained. */
StarsU32 stars_get_line(StarsU8 *data, StarsU16 pointer_slot);
void stars_set_victory_check(StarsU8 *data, StarsU16 game, StarsI16 index, StarsI16 enabled);
StarsI16 stars_get_victory_check(const StarsU8 *data, StarsU16 game, StarsI16 index);
StarsI16 stars_get_victory_value(const StarsU8 *data, StarsU16 game, StarsI16 index, StarsI16 raw);
/* Setter requires a valid victory-field index, 0 through 9. */
StarsI16 stars_set_victory_value(StarsU8 *data, StarsU16 game, StarsI16 index, StarsI16 value);
/* Valid template indices 0..4; destination is a valid 36-byte region. */
void stars_init_battle_plan(StarsU8 *data, StarsU16 output, StarsI16 template_index, StarsI16 plan_id);
StarsU32 stars_computer_player_pointer(StarsU16 code_selector, StarsI16 race, StarsI16 level);
void stars_init_new_game3(void);
typedef StarsI16 (*StarsInitNewGameRandom)(void *, StarsI16);
typedef StarsI16 (*StarsInitNewGameSetVC)(void *, StarsI16, StarsI16);
typedef struct StarsInitNewGamePlrInput {
    StarsI16 step_max_so_far;
    StarsI16 ai_level;
    StarsI16 map_size;
    StarsI16 *player_count;
    StarsU16 read_only;
    StarsU8 *player_types;
    StarsInitNewGameRandom random_value;
    StarsInitNewGameSetVC set_victory_value;
    void *context;
} StarsInitNewGamePlrInput;
void stars_init_new_game_plr(StarsInitNewGamePlrInput *);
/* Stat setter index must be 0..15. All memory images are complete 64 KiB. */
StarsI16 stars_set_race_stat(StarsU8 *data, StarsU16 player, StarsI16 index, StarsI16 value);
StarsI16 stars_get_race_trait(const StarsU8 *data, StarsU16 player, StarsI16 index);
void stars_set_race_trait(StarsU8 *data, StarsU16 player, StarsI16 index, StarsI16 enabled);
StarsU16 stars_race_checksum(const StarsU8 *data, StarsU16 player);
void stars_bounds_check_player(StarsU8 *data, StarsU16 player);
/* Valid environmental ranges and nonzero selected half-width required.
 * Planet and global player array refer to the same supplied data image. */
StarsI16 stars_planet_desirability(const StarsU8 *data, StarsU16 planet, StarsI16 player_index);
/* Ordered, nondegenerate ranges or invalid triples repaired to immunity.
 * Player record must be disjoint from player zero or exactly player zero.
 * See innate-habitability.md for the bounded x87 rounding model. */
StarsI32 stars_innate_race_habitability(StarsU8 *data, StarsU16 player);
/* Same record/FPU domain as innate habitability, after bounds correction. */
StarsI16 stars_race_advantage_points(StarsU8 *data, StarsU16 player);
StarsI16 stars_true_max_growth(const StarsU8 *data, StarsI16 player_index);
/* strings is a complete image of NE segment 3; see game-strings.md. */
StarsU16 stars_get_compressed_string(StarsU8 *data, const StarsU8 *strings, StarsI16 id);
StarsI16 stars_copy_game_string(StarsU8 *data, const StarsU8 *strings, StarsI16 id, StarsU16 output);
/* Existing initialized record, static template and RNG seeds in data.
 * Name must terminate within its 32-byte field; see random-race.md. */
void stars_create_random_race(StarsU8 *data, const StarsU8 *strings, StarsU16 player);
/* Exact-count stream transfers, or nonlocal abort on failure. These are
 * integration boundaries for RgFromStream/RgToStream, not Windows APIs. */
typedef void (*StarsReadTransfer)(void *context, StarsU8 *output, StarsU16 count);
typedef void (*StarsWriteTransfer)(void *context, const StarsU8 *input, StarsU16 count);
void stars_read_record(StarsU8 *data, StarsReadTransfer read, void *context);
void stars_write_record(StarsU8 *data, StarsI16 type, StarsU16 count, const StarsU8 *input,
    StarsWriteTransfer write, void *context);
/* Fixed original strings; result must fit 50-byte temporary. Automatic
 * pluralization of trimmed single lowercase e needs saved-SI context (U019). */
StarsU16 stars_player_name(StarsU8 *data, const StarsU8 *strings, StarsI16 index,
    StarsI16 capital, StarsI16 plural, StarsI16 article, StarsI16 grammar, StarsU16 player);
/* Disjoint, contiguous records/buffers. At decoded length 32 the terminator
 * can extend beyond a name field; see documented writable-span requirements. */
void stars_read_player_record(StarsU8 *data, const StarsU8 *strings, StarsU16 player, StarsU16 input);
void stars_write_player_record(StarsU8 *data, StarsU16 player, StarsU16 storage,
    StarsWriteTransfer write, void *context);
typedef StarsU32 (*StarsTickCount)(void *context);
void stars_write_bof(StarsU8 *data, StarsI16 player, StarsI16 type, StarsI16 multi,
    StarsTickCount ticks, StarsWriteTransfer write, void *context);
/* Stream integration boundaries. abort must not return. Error callbacks stand
 * for the still-unreconstructed FileError / formatted write-error alert.
 * memory resolves a contiguous far source span; no selector crossing allowed. */
typedef struct StarsStreamApi {
    StarsI16 (*open)(void *context, const StarsU8 *name, StarsU16 mode, StarsU16 *error);
    StarsI16 (*close)(void *context, StarsI16 handle);
    StarsU16 (*read)(void *context, StarsI16 handle, StarsU8 *output, StarsU16 count);
    StarsU16 (*write)(void *context, StarsI16 handle, const StarsU8 *input, StarsU16 count);
    StarsTickCount ticks;
    const StarsU8 *(*memory)(void *context, StarsU16 selector, StarsU16 offset);
    void (*file_error)(void *context, StarsI16 id);
    void (*write_error)(void *context);
    void (*abort)(void *context, StarsU16 environment, StarsI16 value);
} StarsStreamApi;
typedef StarsU32 (*StarsFileLength)(void *context, StarsI16 handle);
StarsI16 stars_copy_file(const StarsU8 *, const StarsU8 *,
    const StarsStreamApi *, void *, StarsFileLength);
void stars_stream_open(StarsU8 *data, StarsU16 name, StarsI16 mode,
    const StarsStreamApi *api, void *context);
void stars_stream_close(StarsU8 *data, const StarsStreamApi *api, void *context);
void stars_stream_read(StarsU8 *data, StarsU8 *output, StarsU16 count,
    const StarsStreamApi *api, void *context);
void stars_stream_write(StarsU8 *data, const StarsU8 *input, StarsU16 count,
    const StarsStreamApi *api, void *context);
/* Filename tables must match the verified original; bounded, contiguous
 * NUL-terminated base and output buffers, without overlap. */
void stars_set_file_name(StarsU8 *data, StarsU16 type, StarsI16 player);
typedef StarsI16 (*StarsPathAccess)(void *, const StarsU8 *, StarsI16);
typedef StarsI16 (*StarsOpenGameFile)(void *, StarsU16, StarsI16, StarsU16);
typedef void (*StarsCloseGameFile)(void *);
typedef void (*StarsReadGameRecord)(void *);
typedef void (*StarsWriteGameRecord)(void *, StarsU16, StarsU16, const StarsU8 *);
typedef StarsI16 (*StarsMarkGameFile)(void *, StarsU16, StarsI16, StarsU16, StarsI16);
typedef void (*StarsWriteBofRecord)(void *, StarsI16, StarsI16, StarsI16);
typedef void (*StarsEndDialog)(void *, StarsU16, StarsI16);
typedef void (*StarsWinHelp)(void *, StarsU16, const StarsU8 *, StarsU16, StarsU16);
typedef void *(*StarsMakeProcInstance)(void *, StarsU16);
typedef StarsI16 (*StarsDialogBox)(void *, StarsU16, StarsU16, StarsU16, void *);
typedef void (*StarsFreeProcInstance)(void *, void *);
typedef void (*StarsWriteLogFile)(void *, const StarsU8 *, StarsI16);
typedef void (*StarsWriteHistFile)(void *, StarsI16);
typedef struct StarsPromptSaveOps {
    StarsMakeProcInstance make_proc;
    StarsDialogBox dialog_box;
    StarsFreeProcInstance free_proc;
    StarsWriteLogFile write_log;
    StarsWriteHistFile write_history;
} StarsPromptSaveOps;
typedef void (*StarsVisiblePlanFleetStep)(void *, StarsI16);
typedef void (*StarsVisibleProgress)(void *, StarsI16);
typedef struct StarsVisiblePlanFleetOps {
    StarsVisiblePlanFleetStep initialize;
    StarsVisiblePlanFleetStep fleets;
    StarsVisiblePlanFleetStep planets;
    StarsVisiblePlanFleetStep things;
    StarsVisiblePlanFleetStep finish;
    StarsVisibleProgress progress;
} StarsVisiblePlanFleetOps;
typedef struct StarsVisibleFinishPlayer {
    StarsU8 shdef_count;
    StarsU16 detail_flags;
    StarsU16 fleet_flags[16];
    StarsU32 fleet_known[16];
    StarsU16 starbase_flags[10];
    StarsU32 starbase_known[10];
    StarsU16 fleet_count; /* Low 12 fleet count; high 4 visible starbase designs. */
} StarsVisibleFinishPlayer;
typedef struct StarsVisibleFinishInput {
    StarsI16 viewer;
    StarsI16 race_stat;
    StarsU16 player_count;
    StarsVisibleFinishPlayer *players;
} StarsVisibleFinishInput;
void stars_set_visible_pf_finish(const StarsVisibleFinishInput *);
typedef struct StarsTechLevelCostInput {
    const char *colonist_counts;
    const StarsU32 *level_costs;
    StarsU16 level_count;
    StarsU16 game_flags;
    StarsI16 tech;
    StarsI16 level;
    StarsI16 player;
} StarsTechLevelCostInput;
typedef StarsI16 (*StarsTechRaceStat)(void *, StarsI16, StarsI16);
typedef struct StarsTechLevelCostOps {
    StarsTechRaceStat get_race_stat;
} StarsTechLevelCostOps;
StarsI32 stars_get_tech_level_cost(const StarsTechLevelCostInput *,
    const StarsTechLevelCostOps *, void *);
StarsI16 stars_file_bogus_long(const StarsU8 *data, StarsU32 serial);
StarsI16 stars_file_valid_serial_long(const StarsU8 *data, StarsU32 serial);
typedef StarsI16 (*StarsSerialValidator)(void *, StarsU32);
StarsI16 stars_player_also_cheater(const StarsU8 *, const StarsU8 *, StarsI16,
    StarsI16, StarsSerialValidator, void *);
typedef StarsU16 (*StarsGetFocus)(void *);
typedef void (*StarsSendCharacter)(void *, StarsU16, StarsU16, StarsU16,
    StarsU32);
StarsI16 stars_handle_char(StarsU16, StarsU16, StarsU16, StarsU16, StarsU16,
    StarsU32, StarsGetFocus, StarsSendCharacter, void *);
typedef void (*StarsGetClientRect)(void *, StarsU16, StarsRect *);
typedef void (*StarsFillRect)(void *, StarsU16, const StarsRect *, StarsU16);
typedef void (*StarsSetBackgroundColor)(void *, StarsU16, StarsU32);
StarsI16 stars_order_info_dialog(StarsU16, StarsU16, StarsU16, StarsU32,
    StarsU16, StarsU32, StarsGetClientRect, StarsFillRect,
    StarsSetBackgroundColor, StarsEndDialog, void *);
typedef StarsU16 (*StarsHandleGetActiveWindow)(void *);
typedef void (*StarsHandleDestroyWindow)(void *, StarsU16);
typedef void (*StarsHandleSendMessage)(void *, StarsU16, StarsU16,
    StarsU16, StarsU32);
typedef StarsU16 (*StarsHandleGetFocus)(void *);
typedef StarsU16 (*StarsHandleGetParent)(void *, StarsU16);
typedef void (*StarsHandleTerminateToolbarFocus)(void *, StarsU16);
typedef void (*StarsHandleGetCursorPos)(void *, StarsI16 *, StarsI16 *);
typedef StarsU16 (*StarsHandleWindowFromPoint)(void *, StarsI16, StarsI16);
typedef StarsU16 (*StarsHandleGetDlgItem)(void *, StarsU16, StarsU16);
typedef StarsI16 (*StarsHandleGetKeyState)(void *, StarsU16);
typedef void (*StarsHandleInvalidateRect)(void *, StarsU16);
typedef StarsI16 (*StarsHandleIsButtonDown)(void *, StarsU16);
typedef void (*StarsHandleExecuteButton)(void *, StarsU16, StarsU16);
typedef void (*StarsHandleDeleteCurWayPoint)(void *, StarsU16);
typedef void (*StarsHandleLookupFleet)(void *, StarsI16, StarsU8 *);
typedef void (*StarsHandleDrawPlanShip)(void *, StarsU16, StarsU16);
typedef void (*StarsHandleExecuteReportClick)(void *, StarsI16, StarsI16,
    StarsU16, StarsU16, StarsI16);
typedef struct StarsHandleKeyOps {
    StarsHandleGetActiveWindow get_active_window;
    StarsHandleDestroyWindow destroy_window;
    StarsHandleSendMessage send_message;
    StarsHandleGetFocus get_focus;
    StarsHandleGetParent get_parent;
    StarsHandleTerminateToolbarFocus terminate_toolbar_focus;
    StarsHandleGetCursorPos get_cursor_pos;
    StarsHandleWindowFromPoint window_from_point;
    StarsHandleGetDlgItem get_dialog_item;
    StarsHandleGetKeyState get_key_state;
    StarsHandleInvalidateRect invalidate_rect;
    StarsHandleIsButtonDown is_button_down;
    StarsHandleExecuteButton execute_button;
    StarsHandleDeleteCurWayPoint delete_current_waypoint;
    StarsHandleLookupFleet lookup_fleet;
    StarsHandleDrawPlanShip draw_plan_ship;
    StarsHandleExecuteReportClick execute_report_click;
} StarsHandleKeyOps;
StarsI16 stars_handle_key(StarsU8 *, StarsU16, StarsU16, StarsU16, StarsU32,
    const StarsHandleKeyOps *, void *);
typedef StarsI16 (*StarsBatchOpen)(void *, const StarsU8 *, StarsU16);
typedef StarsU16 (*StarsBatchFileLength)(void *, StarsI16);
typedef StarsU32 (*StarsBatchAllocate)(void *, StarsU16, StarsI16);
typedef StarsU8 *(*StarsBatchMemory)(void *, StarsU32);
typedef StarsU16 (*StarsBatchRead)(void *, StarsI16, StarsU8 *, StarsU16);
typedef void (*StarsBatchClose)(void *, StarsI16);
typedef struct StarsBatchOps {
    StarsBatchOpen open;
    StarsBatchFileLength file_length;
    StarsBatchAllocate allocate;
    StarsBatchMemory memory;
    StarsBatchRead read;
    StarsBatchClose close;
} StarsBatchOps;
StarsI16 stars_set_up_batch_processing(StarsU8 *, const StarsBatchOps *,
    void *);
typedef void (*StarsReadIniSettings)(void *);
typedef StarsU16 (*StarsInitCreateWindow)(void *, const StarsU8 *,
    const StarsU8 *, StarsU32, StarsI16, StarsI16, StarsI16, StarsI16,
    StarsU16);
typedef StarsU16 (*StarsInitLoadAccelerator)(void *, StarsU16, StarsU16);
typedef void (*StarsInitShowWindow)(void *, StarsU16, StarsU16);
typedef struct StarsInitInstanceOps {
    StarsReadIniSettings read_ini_settings;
    StarsInitCreateWindow create_window;
    StarsInitLoadAccelerator load_accelerator;
    StarsInitShowWindow show_window;
} StarsInitInstanceOps;
StarsI16 stars_init_instance(StarsU8 *, StarsI16, const StarsInitInstanceOps *,
    void *);
/* WinMain keeps the recovered startup state explicit.  The message and
 * Win16 entry points are callbacks so the parser and dispatch policy remain
 * ordinary C source while the platform adapter supplies USER/instance work. */
typedef struct StarsWinMainMessage {
    StarsU16 hwnd;
    StarsU16 message;
    StarsU16 wparam;
    StarsU32 lparam;
} StarsWinMainMessage;
typedef struct StarsWinMainState {
    StarsU16 instance;
    StarsU16 frame;
    StarsU16 title;
    StarsU16 accel_frame;
    StarsU16 accel_title;
    StarsU16 ini_flags;
    StarsU16 global_low_flags;
    StarsU16 global_high_flags;
    StarsI16 turn_generation_count;
    StarsU16 vtimer_auto_generate;
    StarsU32 salt_last;
    char base[512];
    char pass_last[16];
    StarsU8 tutor_zero[44];
    StarsU8 vtimer_zero[10];
} StarsWinMainState;
typedef StarsI16 (*StarsWinMainInitMdi)(void *);
typedef StarsU32 (*StarsWinMainTick)(void *);
typedef void (*StarsWinMainRandomize2)(void *, StarsU32);
typedef StarsI16 (*StarsWinMainCreateStuff)(void *);
typedef StarsI16 (*StarsWinMainSystemColors)(void *);
typedef StarsI16 (*StarsWinMainInitInstance)(void *, StarsI16,
    StarsWinMainState *);
typedef StarsI16 (*StarsWinMainSetupBatch)(void *);
typedef StarsU32 (*StarsWinMainSaltFromString)(void *, const StarsU8 *);
typedef void (*StarsWinMainAlert)(void *, StarsU16, StarsI16);
typedef void (*StarsWinMainPostMessage)(void *, StarsU16, StarsU16,
    StarsU16, StarsU32);
typedef StarsI16 (*StarsWinMainGetMessage)(void *, StarsWinMainMessage *);
typedef StarsI16 (*StarsWinMainIsIconic)(void *, StarsU16);
typedef StarsI16 (*StarsWinMainTranslateAccelerator)(void *, StarsU16,
    StarsU16, const StarsWinMainMessage *);
typedef void (*StarsWinMainTranslateMessage)(void *, const StarsWinMainMessage *);
typedef StarsI16 (*StarsWinMainHandleKey)(void *, StarsU16, StarsU16,
    StarsU16, StarsU32);
typedef StarsI16 (*StarsWinMainHandleChar)(void *, StarsU16, StarsU16,
    StarsU32);
typedef void (*StarsWinMainDispatchMessage)(void *, const StarsWinMainMessage *);
typedef void (*StarsWinMainFreeStuff)(void *);
typedef struct StarsWinMainOps {
    StarsWinMainInitMdi init_mdi;
    StarsWinMainTick get_tick;
    StarsWinMainRandomize2 randomize2;
    StarsWinMainCreateStuff create_stuff;
    StarsWinMainSystemColors get_system_colors;
    StarsWinMainInitInstance init_instance;
    StarsWinMainSetupBatch set_up_batch_processing;
    StarsWinMainSaltFromString salt_from_string;
    StarsWinMainAlert alert_ids;
    StarsWinMainPostMessage post_message;
    StarsWinMainGetMessage get_message;
    StarsWinMainIsIconic is_iconic;
    StarsWinMainTranslateAccelerator translate_accelerator;
    StarsWinMainTranslateMessage translate_message;
    StarsWinMainHandleKey handle_key;
    StarsWinMainHandleChar handle_char;
    StarsWinMainDispatchMessage dispatch_message;
    StarsWinMainFreeStuff free_stuff;
} StarsWinMainOps;
StarsI16 stars_win_main(StarsWinMainState *, StarsU16, StarsU16,
    const StarsU8 *, StarsI16, const StarsWinMainOps *, void *);
typedef struct StarsTitleState {
    StarsU16 screen_colors;
    StarsU16 dib_title;
    StarsU16 palette_splash;
    StarsU16 buttons[4];
    StarsU16 button_face;
    StarsU16 font_arial8;
    StarsU16 font_arial10;
    StarsU16 icon_stars;
    StarsU16 frame;
    StarsU16 title;
    StarsU16 instance;
    StarsU16 freeing_title;
    StarsU16 global_high_flags;
    StarsI16 exit_value;
    StarsI16 id_player;
    StarsU32 game_lid;
    StarsU16 game_flags;
    StarsI16 planet_present;
    StarsI16 dy_arial8;
    const StarsU8 *base_name;
    char work[128];
} StarsTitleState;
typedef StarsU16 (*StarsTitleLoadResource)(void *, StarsU16);
typedef StarsU16 (*StarsTitlePaletteFromDib)(void *, StarsU16);
typedef void (*StarsTitleUnlockResource)(void *, StarsU16);
typedef void (*StarsTitleFreeResource)(void *, StarsU16);
typedef void (*StarsTitleGetClientRect)(void *, StarsU16, StarsRect *);
typedef const StarsU8 *(*StarsTitleGetString)(void *, StarsU16);
typedef StarsU16 (*StarsTitleCreateWindow)(void *, const StarsU8 *,
    const StarsU8 *, StarsU32, StarsI16, StarsI16, StarsI16, StarsI16,
    StarsU16, StarsU16, StarsU16);
typedef StarsI16 (*StarsTitleAccess)(void *, const StarsU8 *, StarsI16);
typedef void (*StarsTitleEnableWindow)(void *, StarsU16, StarsI16);
typedef void (*StarsTitleSendMessage)(void *, StarsU16, StarsU16,
    StarsU16, StarsU32);
typedef StarsI16 (*StarsTitleIsIconic)(void *, StarsU16);
typedef StarsU16 (*StarsTitleBeginPaint)(void *, StarsU16, StarsRect *);
typedef void (*StarsTitleEndPaint)(void *, StarsU16, const StarsRect *);
typedef StarsU16 (*StarsTitleSelectObject)(void *, StarsU16, StarsU16);
typedef void (*StarsTitleSetTextColor)(void *, StarsU16, StarsU32);
typedef void (*StarsTitleDrawStars)(void *, StarsU16, const StarsRect *);
typedef StarsU16 (*StarsTitleCreateFont)(void *, StarsI16, const StarsU8 *);
typedef void (*StarsTitleDeleteObject)(void *, StarsU16);
typedef void (*StarsTitleSetBkMode)(void *, StarsU16, StarsI16);
typedef void (*StarsTitleDrawCentered)(void *, StarsU16, const StarsRect *,
    const StarsU8 *, StarsI16);
typedef StarsU16 (*StarsTitleSelectPalette)(void *, StarsU16, StarsU16, StarsI16);
typedef StarsU16 (*StarsTitleRealizePalette)(void *, StarsU16);
typedef StarsI16 (*StarsTitleSystemMetric)(void *, StarsU16);
typedef void (*StarsTitleDibBlt)(void *, StarsU16, StarsI16, StarsI16,
    StarsI16, StarsI16, StarsU16, StarsI16, StarsI16, StarsI16, StarsI16,
    StarsU32);
typedef void *(*StarsTitleLocalAlloc)(void *, StarsU16, StarsU16);
typedef void (*StarsTitleLocalFree)(void *, void *);
typedef void (*StarsTitleVersion)(void *, char *, StarsU16);
typedef void (*StarsTitleGetWindowRect)(void *, StarsU16, StarsRect *);
typedef StarsU16 (*StarsTitleGetDc)(void *, StarsU16);
typedef void (*StarsTitleReleaseDc)(void *, StarsU16, StarsU16);
typedef void (*StarsTitleDrawIcon)(void *, StarsU16, StarsI16, StarsI16,
    StarsU16);
typedef void (*StarsTitleSetFocus)(void *, StarsU16);
typedef void (*StarsTitleNewGameWizard)(void *, StarsU16, StarsU16);
typedef void (*StarsTitleDestroyWindow)(void *, StarsU16);
typedef void (*StarsTitleShowWindow)(void *, StarsU16, StarsU16);
typedef StarsI16 (*StarsTitleOpenGame)(void *, StarsU16, StarsI16);
typedef void (*StarsTitleInitializeMenu)(void *, StarsU16);
typedef void (*StarsTitlePostMessage)(void *, StarsU16, StarsU16,
    StarsU16, StarsU32);
typedef void (*StarsTitleStartTutor)(void *, StarsI16);
typedef void (*StarsTitleWriteIni)(void *);
typedef void (*StarsTitlePostQuit)(void *, StarsI16);
typedef void (*StarsTitleExitWindows)(void *, StarsI16);
typedef StarsU32 (*StarsTitleDefaultProc)(void *, StarsU16, StarsU16,
    StarsU16, StarsU32);
typedef void (*StarsTitleInvalidateRect)(void *, StarsU16, StarsI16);
typedef struct StarsTitleOps {
    StarsTitleLoadResource load_big_resource;
    StarsTitlePaletteFromDib palette_from_dib;
    StarsTitleUnlockResource unlock_resource;
    StarsTitleFreeResource free_resource;
    StarsTitleGetClientRect get_client_rect;
    StarsTitleGetString get_string;
    StarsTitleCreateWindow create_window;
    StarsTitleAccess access;
    StarsTitleEnableWindow enable_window;
    StarsTitleSendMessage send_message;
    StarsTitleIsIconic is_iconic;
    StarsTitleBeginPaint begin_paint;
    StarsTitleEndPaint end_paint;
    StarsTitleSelectObject select_object;
    StarsTitleSetTextColor set_text_color;
    StarsTitleDrawStars draw_stars;
    StarsTitleCreateFont create_font;
    StarsTitleDeleteObject delete_object;
    StarsTitleSetBkMode set_bk_mode;
    StarsTitleDrawCentered draw_centered;
    StarsTitleSelectPalette select_palette;
    StarsTitleRealizePalette realize_palette;
    StarsTitleSystemMetric system_metric;
    StarsTitleDibBlt dib_blt;
    StarsTitleLocalAlloc local_alloc;
    StarsTitleLocalFree local_free;
    StarsTitleVersion version;
    StarsTitleGetWindowRect get_window_rect;
    StarsTitleGetDc get_dc;
    StarsTitleReleaseDc release_dc;
    StarsTitleDrawIcon draw_icon;
    StarsTitleSetFocus set_focus;
    StarsTitleNewGameWizard new_game_wizard;
    StarsTitleDestroyWindow destroy_window;
    StarsTitleShowWindow show_window;
    StarsTitleOpenGame open_game;
    StarsTitleInitializeMenu initialize_menu;
    StarsTitlePostMessage post_message;
    StarsTitleStartTutor start_tutor;
    StarsTitleWriteIni write_ini;
    StarsTitlePostQuit post_quit;
    StarsTitleExitWindows exit_windows;
    StarsTitleDefaultProc default_proc;
    StarsTitleInvalidateRect invalidate_rect;
} StarsTitleOps;
StarsU32 stars_title_wnd_proc(StarsTitleState *, StarsU16, StarsU16,
    StarsU16, StarsU32, const StarsTitleOps *, void *);
typedef struct StarsCreateStuffState {
    StarsU16 display_flags;
    StarsU8 player_defaults[192];
    StarsU16 regions[2];
    StarsU16 brushes[34];
    StarsU16 pens[11];
    StarsU16 pattern_brushes[6];
    StarsU16 cursors[11];
    StarsU16 bitmaps[8];
    StarsU16 dib_planets;
    StarsU16 dib_things;
    StarsU16 dib_toolbar;
    StarsU16 dib_ships[5];
    StarsU16 dib_ships_t[5];
    StarsU16 dib_inventory[7];
    StarsU16 dib_races;
    StarsU16 dib_races_t;
    StarsU16 dib_races_x;
    StarsU16 dib_plaque;
    StarsU16 icons[10];
    StarsU32 log_buffer;
    StarsU32 message_buffer;
    StarsU32 misc_buffer;
    StarsU32 planet_buffer;
    StarsU32 fleet_buffer;
    StarsU32 proc_instances[8];
    StarsU16 palette_ships;
} StarsCreateStuffState;
typedef StarsI16 (*StarsCreateStuffMetric)(void *, StarsU16);
typedef void (*StarsCreateStuffCopyDefaults)(void *, StarsU8 *, StarsU16);
typedef StarsU16 (*StarsCreateStuffRegion)(void *, StarsI16, StarsI16,
    StarsI16, StarsI16);
typedef StarsU16 (*StarsCreateStuffBrush)(void *, StarsU32);
typedef StarsU16 (*StarsCreateStuffPen)(void *, StarsI16, StarsI16, StarsU32);
typedef StarsU16 (*StarsCreateStuffLoadBitmap)(void *, StarsU16, StarsU16);
typedef StarsU16 (*StarsCreateStuffPatternBrush)(void *, StarsU16);
typedef void (*StarsCreateStuffDeleteObject)(void *, StarsU16);
typedef StarsU16 (*StarsCreateStuffLoadCursor)(void *, StarsU16, StarsU16);
typedef StarsU16 (*StarsCreateStuffLoadDib)(void *, StarsU16);
typedef StarsU16 (*StarsCreateStuffPaletteFromDib)(void *, StarsU16);
typedef StarsU16 (*StarsCreateStuffLoadIcon)(void *, StarsU16, StarsU16);
typedef StarsU32 (*StarsCreateStuffAllocate)(void *, StarsU16, StarsU16);
typedef StarsU32 (*StarsCreateStuffMakeProc)(void *, StarsU16, StarsU16);
typedef void (*StarsCreateStuffDiskSerial)(void *);
typedef void (*StarsCreateStuffAlert)(void *, StarsU16, StarsI16);
typedef struct StarsCreateStuffOps {
    StarsCreateStuffMetric get_system_metric;
    StarsCreateStuffCopyDefaults copy_player_defaults;
    StarsCreateStuffRegion create_region;
    StarsCreateStuffBrush get_brush;
    StarsCreateStuffPen create_pen;
    StarsCreateStuffLoadBitmap load_bitmap;
    StarsCreateStuffPatternBrush create_pattern_brush;
    StarsCreateStuffDeleteObject delete_object;
    StarsCreateStuffLoadCursor load_cursor;
    StarsCreateStuffLoadDib load_dib;
    StarsCreateStuffPaletteFromDib palette_from_dib;
    StarsCreateStuffLoadIcon load_icon;
    StarsCreateStuffAllocate allocate_far;
    StarsCreateStuffMakeProc make_proc_instance;
    StarsCreateStuffDiskSerial get_disk_serial;
    StarsCreateStuffAlert alert_ids;
} StarsCreateStuffOps;
StarsI16 stars_create_stuff(StarsCreateStuffState *, StarsU16,
    const StarsCreateStuffOps *, void *);
/* The original InitMDIApp registers eleven Win16 window classes.  Keep the
 * class records typed while representing the original far references as
 * selector/offset words; the USER calls remain explicit host boundaries. */
typedef struct StarsMdiFarRef {
    StarsU16 offset;
    StarsU16 selector;
} StarsMdiFarRef;
typedef struct StarsMdiClassSpec {
    StarsU16 style;
    StarsMdiFarRef window_proc;
    StarsU16 class_extra;
    StarsU16 window_extra;
    StarsU16 instance;
    StarsU16 icon;
    StarsU16 cursor;
    StarsU16 background;
    StarsMdiFarRef menu_name;
    StarsMdiFarRef class_name;
} StarsMdiClassSpec;
typedef StarsU16 (*StarsMdiLoadCursor)(void *, StarsU16, StarsU16);
typedef StarsU16 (*StarsMdiGetStockObject)(void *, StarsU16);
typedef StarsU16 (*StarsMdiRegisterClass)(void *, const StarsMdiClassSpec *);
typedef struct StarsInitMdiOps {
    StarsMdiLoadCursor load_cursor;
    StarsMdiGetStockObject get_stock_object;
    StarsMdiRegisterClass register_class;
} StarsInitMdiOps;
StarsI16 stars_init_mdi_app(StarsU16, const StarsInitMdiOps *, void *);

/* CommandHandler is the frame-level command router.  The original routine
 * mostly sequences application services and USER calls; keep those calls as
 * typed boundaries so the reconstructed source remains usable by both a
 * native VC1.52 harness and a host test without reproducing Win16 handles. */
typedef struct StarsCommandState {
    StarsI16 player;
    StarsU16 game_present;
    StarsU16 tutor_active;
    StarsU16 scanner_window;
    StarsU16 browser_window;
    StarsU16 report_kind;
    StarsU16 title_window;
    StarsU16 timer_id;
    StarsU16 order_edit_focus;
    StarsU16 selected_object_kind;
    StarsU16 layout;
    StarsU16 scan_zoom;
    StarsU16 scan_flags;
    StarsU16 dirty;
    StarsU16 overlap_mode;
    StarsU16 new_turn_available;
    StarsU16 salt_valid;
    StarsU16 salt_current;
    StarsU16 fleet_count;
    StarsU16 enemy_fleet_count;
    StarsU16 planet_count;
    StarsU16 battle_count;
} StarsCommandState;
typedef void (*StarsCommandVoid)(void *);
typedef void (*StarsCommandDump)(void *, StarsU16);
typedef void (*StarsCommandMenuSelection)(void *, StarsU16);
typedef void (*StarsCommandWaypoint)(void *, StarsU16);
typedef StarsI16 (*StarsCommandConfirm)(void *);
typedef StarsI16 (*StarsCommandGame)(void *, StarsU16, StarsU16);
typedef StarsI16 (*StarsCommandDialog)(void *, StarsU16, StarsU16);
typedef void (*StarsCommandValue)(void *, StarsU16);
typedef void (*StarsCommandMessage)(void *, StarsU16, StarsU16);
typedef void (*StarsCommandScanZoom)(void *, StarsU16, StarsU16);
typedef void (*StarsCommandHelp)(void *, StarsU16);
typedef struct StarsCommandOps {
    StarsCommandMenuSelection set_menu_selection;
    StarsCommandDump dump;
    StarsCommandWaypoint delete_waypoint;
    StarsCommandConfirm confirm_end_tutor;
    StarsCommandGame open_game;
    StarsCommandGame new_game;
    StarsCommandVoid write_ini;
    StarsCommandVoid destroy_game;
    StarsCommandVoid initialize_menu;
    StarsCommandVoid create_children;
    StarsCommandVoid start_tutor;
    StarsCommandVoid advance_tutor;
    StarsCommandValue show_tutor;
    StarsCommandVoid ensure_ais;
    StarsCommandVoid generate_turn;
    StarsCommandDialog dialog;
    StarsCommandVoid refit;
    StarsCommandValue ensure_tile_size;
    StarsCommandValue invalidate_frame;
    StarsCommandMessage post_message;
    StarsCommandValue close_frame;
    StarsCommandValue toggle_toolbar;
    StarsCommandScanZoom set_scan_zoom;
    StarsCommandHelp help;
    StarsCommandValue select_report;
    StarsCommandValue toggle_scanner_grid;
    StarsCommandValue restore_selection;
    StarsCommandValue find_dialog;
    StarsCommandValue load_mru_game;
} StarsCommandOps;
void stars_mdi_command_handler(StarsCommandState *, StarsU16, StarsU16,
    const StarsCommandOps *, void *);
typedef struct StarsCreateChildState {
    StarsI16 player;
    const StarsU8 *base_path;
    StarsU16 frame;
    StarsU16 instance;
    StarsU16 scanner;
    StarsU16 mine;
    StarsU16 planet;
    StarsU16 toolbar;
    StarsU16 message;
    StarsI16 mine_width;
    StarsI16 mine_height;
    StarsI16 x_scan_top;
    StarsI16 y_scan_top;
    const StarsU8 *scan_class;
    const StarsU8 *mine_class;
    const StarsU8 *planet_class;
    const StarsU8 *toolbar_class;
    const StarsU8 *message_class;
} StarsCreateChildState;
typedef void (*StarsCreateChildTitle)(void *, StarsI16, const StarsU8 *,
    StarsU8 *, StarsU16);
typedef StarsU16 (*StarsCreateChildWindow)(void *, const StarsU8 *, StarsI16,
    StarsI16, StarsI16, StarsI16, StarsU16, StarsU16);
typedef void (*StarsCreateChildInvalidate)(void *, StarsU16);
typedef void (*StarsCreateChildSetText)(void *, StarsU16, const StarsU8 *);
typedef void (*StarsCreateChildDestroy)(void *, StarsU16);
typedef void (*StarsCreateChildScanBars)(void *, StarsU16);
typedef void (*StarsCreateChildRefit)(void *);
typedef struct StarsCreateChildOps {
    StarsCreateChildTitle make_title;
    StarsCreateChildWindow create_window;
    StarsCreateChildInvalidate invalidate;
    StarsCreateChildSetText set_window_text;
    StarsCreateChildDestroy destroy_window;
    StarsCreateChildScanBars set_scan_scroll_bars;
    StarsCreateChildRefit refit_frame_children;
} StarsCreateChildOps;
void stars_create_child_windows(StarsCreateChildState *,
    const StarsCreateChildOps *, void *);
typedef struct StarsRefitState {
    StarsU16 frame;
    StarsU16 scanner;
    StarsU16 mine;
    StarsU16 planet;
    StarsU16 toolbar;
    StarsU16 message;
    StarsI16 iconic;
    StarsI16 layout;
    StarsU16 color_flags;
    StarsI16 dx;
    StarsI16 dy;
    StarsI16 dx_plan_want;
    StarsI16 dy_msg_want;
    StarsI16 dy_min_want;
    StarsI16 dx2_plan_want;
    StarsI16 dy2_msg_want;
    StarsI16 dy2_min_want;
    StarsI16 x_top;
    StarsI16 y1;
    StarsI16 y2;
    StarsI16 dy_arial8;
} StarsRefitState;
typedef void (*StarsRefitMoveWindow)(void *, StarsU16, StarsI16, StarsI16,
    StarsI16, StarsI16, StarsI16);
typedef StarsU16 (*StarsRefitGetSubMenu)(void *, StarsU16, StarsU16);
typedef void (*StarsRefitCheckMenuItem)(void *, StarsU16, StarsU16, StarsU16);
typedef struct StarsRefitOps {
    StarsRefitMoveWindow move_window;
    StarsRefitGetSubMenu get_sub_menu;
    StarsRefitCheckMenuItem check_menu_item;
} StarsRefitOps;
void stars_refit_frame_children(StarsRefitState *, const StarsRefitOps *,
    void *);
typedef struct StarsEnsureAisState {
    StarsU16 flags;
    StarsU16 file_error_silent;
    StarsU16 player_count;
    StarsU16 player_flags[16];
    const StarsU8 *base_path;
} StarsEnsureAisState;
typedef void (*StarsEnsureAisGame)(void *, const StarsU8 *, StarsU16);
typedef StarsU16 (*StarsEnsureAisOpen)(void *, StarsI16, StarsU16);
typedef void (*StarsEnsureAisClose)(void *, StarsI16);
typedef void (*StarsEnsureAisTurn)(void *, StarsI16, StarsU16);
typedef void (*StarsEnsureAisProgress)(void *, StarsI16);
typedef struct StarsEnsureAisOps {
    StarsEnsureAisGame destroy_game;
    StarsEnsureAisGame load_game;
    StarsEnsureAisOpen open_file;
    StarsEnsureAisClose close_file;
    StarsEnsureAisTurn do_ai_turn;
    StarsEnsureAisProgress update_progress;
} StarsEnsureAisOps;
void stars_ensure_ais(StarsEnsureAisState *, const StarsEnsureAisOps *,
    void *);
typedef struct StarsInitializeMenuState {
    StarsU16 frame;
    StarsU16 menu;
    const StarsU8 *base;
    StarsU16 game_flags;
    StarsU16 salt_low;
    StarsU16 salt_high;
    StarsU16 color_flags;
    StarsU16 scan_flags;
    StarsU16 scanner;
    StarsU16 scan_zoom;
    StarsU16 window_layout;
    const StarsU8 *mru[9];
} StarsInitializeMenuState;
typedef StarsU16 (*StarsMenuGetMenu)(void *, StarsU16);
typedef StarsU16 (*StarsMenuGetSubMenu)(void *, StarsU16, StarsU16);
typedef void (*StarsMenuDelete)(void *, StarsU16, StarsU16, StarsU16);
typedef void (*StarsMenuInsert)(void *, StarsU16, StarsU16, StarsU16,
    StarsU16, const StarsU8 *);
typedef void (*StarsMenuEnable)(void *, StarsU16, StarsU16, StarsU16);
typedef void (*StarsMenuCheck)(void *, StarsU16, StarsU16, StarsU16);
typedef StarsI16 (*StarsMenuCount)(void *, StarsU16);
typedef void (*StarsMenuDraw)(void *, StarsU16);
typedef struct StarsInitializeMenuOps {
    StarsMenuGetMenu get_menu;
    StarsMenuGetSubMenu get_sub_menu;
    StarsMenuDelete delete_menu;
    StarsMenuInsert insert_menu;
    StarsMenuEnable enable_menu;
    StarsMenuCheck check_menu;
    StarsMenuCount menu_item_count;
    StarsMenuDraw draw_menu_bar;
} StarsInitializeMenuOps;
void stars_initialize_menu(StarsInitializeMenuState *,
    const StarsInitializeMenuOps *, void *);
typedef struct StarsInvertPoint {
    StarsI16 x;
    StarsI16 y;
} StarsInvertPoint;
typedef struct StarsInvertState {
    StarsI16 layout;
    StarsI16 dx;
    StarsI16 dy;
    StarsI16 x_top;
    StarsI16 y1;
    StarsI16 y2;
    StarsI16 dy_arial8;
} StarsInvertState;
typedef void (*StarsInvertPatBlt)(void *, StarsU16, StarsI16, StarsI16,
    StarsI16, StarsI16, StarsU32);
typedef struct StarsInvertOps {
    StarsInvertPatBlt pat_blt;
} StarsInvertOps;
StarsInvertPoint stars_invert_pane_border(StarsInvertState *, StarsU16,
    StarsI16, StarsInvertPoint, const StarsInvertPoint *,
    const StarsInvertOps *, void *);
typedef struct StarsVerifyTurnsState {
    StarsI16 id_player;
    StarsU16 player_count;
    StarsU16 plan_count;
    StarsU16 game_flags;
    StarsU16 global_flags;
    StarsU32 tick_last;
    StarsU16 player_flags[16];
    StarsU16 player_dead[16];
    StarsI16 output[16];
    StarsU16 col_drop;
    StarsU16 xfer_full;
    StarsU16 message_index;
} StarsVerifyTurnsState;
typedef void *(*StarsVerifyAllocate)(void *, StarsU32, StarsU16);
typedef void (*StarsVerifyFree)(void *, void *, StarsU16);
typedef StarsI16 (*StarsVerifyCheckLog)(void *, StarsI16, StarsI16 *);
typedef StarsI16 (*StarsVerifyLoadLog)(void *, StarsI16);
typedef StarsI16 (*StarsVerifyRunLog)(void *);
typedef StarsU32 (*StarsVerifyTick)(void *);
typedef struct StarsVerifyTurnsOps {
    StarsVerifyAllocate allocate;
    StarsVerifyFree free_memory;
    StarsVerifyCheckLog check_log;
    StarsVerifyLoadLog load_log;
    StarsVerifyRunLog run_log;
    StarsVerifyTick get_tick;
} StarsVerifyTurnsOps;
void stars_verify_turns(StarsVerifyTurnsState *, const StarsVerifyTurnsOps *,
    void *);
typedef struct StarsBringUpHostState {
    StarsU16 gd_low_flags;
    StarsU16 gd_high_flags;
    StarsU16 ini_flags;
    StarsU16 frame;
    StarsU16 instance;
    StarsU16 title_class;
    StarsU16 title_text;
    StarsU16 title;
    StarsU16 freeing_title;
    StarsU16 pass_count;
    StarsU16 timer_id;
    StarsU16 timer_type;
} StarsBringUpHostState;
typedef StarsU32 (*StarsHostMakeProc)(void *, StarsU16);
typedef StarsI16 (*StarsHostDialog)(void *, StarsU16, StarsU16, StarsU32);
typedef void (*StarsHostFreeProc)(void *, StarsU32);
typedef void (*StarsHostMarkFile)(void *, StarsI16, StarsI16, StarsI16,
    StarsI16);
typedef void (*StarsHostShowWindow)(void *, StarsU16, StarsU16);
typedef void (*StarsHostGame)(void *);
typedef StarsI16 (*StarsHostMetric)(void *, StarsU16);
typedef StarsU16 (*StarsHostCreateTitle)(void *, StarsU16, StarsU16,
    StarsU32, StarsI16, StarsI16, StarsI16, StarsI16, StarsU16);
typedef StarsI16 (*StarsHostAsyncKey)(void *, StarsU16);
typedef StarsU16 (*StarsHostSetTimer)(void *, StarsU16, StarsU16, StarsU32,
    StarsU32);
typedef void (*StarsHostTimerProc)(void *, StarsU16, StarsU16, StarsU16,
    StarsU32);
typedef struct StarsBringUpHostOps {
    StarsHostMarkFile mark_file;
    StarsHostShowWindow show_window;
    StarsHostMakeProc make_proc_instance;
    StarsHostDialog dialog_box;
    StarsHostFreeProc free_proc_instance;
    StarsHostGame destroy_game;
    StarsHostGame show_progress;
    StarsHostGame hide_progress;
    StarsHostGame ensure_ais;
    StarsHostGame generate_turn;
    StarsHostMetric get_system_metric;
    StarsHostCreateTitle create_title;
    StarsHostAsyncKey get_async_key_state;
    StarsHostSetTimer set_timer;
    StarsHostTimerProc host_timer_proc;
} StarsBringUpHostOps;
void stars_bring_up_host_dialog(StarsBringUpHostState *,
    const StarsBringUpHostOps *, void *);
typedef struct StarsDrawHostState {
    StarsU16 frame;
    StarsU16 player_count;
    StarsI16 output[16];
    StarsU16 dead_flags[16];
    StarsU16 global_flags;
    StarsU16 font;
    StarsU16 blue_brush;
    StarsU32 button_face;
    StarsU32 window_text;
    StarsI16 dy_arial8;
    StarsI16 turn;
    StarsU32 tick_last;
    const StarsU8 *turn_format;
} StarsDrawHostState;
typedef const StarsU8 *(*StarsDrawHostCompressed)(void *, StarsU16);
typedef StarsU16 (*StarsDrawHostString)(void *, StarsU16, StarsU8 *, StarsU16);
typedef const StarsU8 *(*StarsDrawHostPlayerName)(void *, StarsI16);
typedef StarsU16 (*StarsDrawHostGetDc)(void *, StarsU16);
typedef void (*StarsDrawHostReleaseDc)(void *, StarsU16, StarsU16);
typedef StarsU16 (*StarsDrawHostSetBkMode)(void *, StarsU16, StarsU16);
typedef StarsU32 (*StarsDrawHostSetBkColor)(void *, StarsU16, StarsU32);
typedef void (*StarsDrawHostSelectFont)(void *, StarsU16, StarsU16);
typedef void (*StarsDrawHostSetTextColor)(void *, StarsU16, StarsU32);
typedef void (*StarsDrawHostSetRect)(void *, StarsRect *, StarsI16, StarsI16,
    StarsI16, StarsI16);
typedef void (*StarsDrawHostOffsetRect)(void *, StarsRect *, StarsI16,
    StarsI16);
typedef void (*StarsDrawHostDiamond)(void *, StarsU16, const StarsRect *,
    StarsU16);
typedef void (*StarsDrawHostRightText)(void *, StarsU16, StarsI16, StarsI16,
    const StarsU8 *, StarsU16, StarsI16);
typedef void (*StarsDrawHostTextOut)(void *, StarsU16, StarsI16, StarsI16,
    const StarsU8 *, StarsU16);
typedef StarsI16 (*StarsDrawHostTextExtent)(void *, StarsU16, const StarsU8 *,
    StarsU16);
typedef StarsU16 (*StarsDrawHostDlgItem)(void *, StarsU16, StarsU16);
typedef void (*StarsDrawHostSetWindowText)(void *, StarsU16, const StarsU8 *);
typedef StarsU32 (*StarsDrawHostTick)(void *);
typedef struct StarsDrawHostOps {
    StarsDrawHostCompressed compressed;
    StarsDrawHostString get_string;
    StarsDrawHostPlayerName player_name;
    StarsDrawHostGetDc get_dc;
    StarsDrawHostReleaseDc release_dc;
    StarsDrawHostSetBkMode set_bk_mode;
    StarsDrawHostSetBkColor set_bk_color;
    StarsDrawHostSelectFont select_font;
    StarsDrawHostSetTextColor set_text_color;
    StarsDrawHostSetRect set_rect;
    StarsDrawHostOffsetRect offset_rect;
    StarsDrawHostDiamond draw_diamond;
    StarsDrawHostRightText right_text;
    StarsDrawHostTextOut text_out;
    StarsDrawHostTextExtent text_extent;
    StarsDrawHostDlgItem get_dialog_item;
    StarsDrawHostSetWindowText set_window_text;
    StarsDrawHostTick get_tick;
} StarsDrawHostOps;
void stars_draw_host_dialog(StarsDrawHostState *, StarsU16, const StarsDrawHostOps *,
    void *);
typedef struct StarsHostTimerState {
    StarsU16 processing_timer;
    StarsU16 timer_type;
    StarsU16 timer_id;
    StarsU16 player;
    StarsU16 frame;
    StarsU16 global_flags;
    StarsU16 ini_flags;
    StarsI16 exit_value;
    StarsU16 dialog_id;
    StarsU16 file_error_silent;
    const StarsU8 *player_format;
    const StarsU8 *waiting_format;
    const StarsU8 *waiting_suffix;
    const StarsU8 *loaded_title;
} StarsHostTimerState;
typedef StarsI16 (*StarsHostTimerNewTurn)(void *, StarsU16);
typedef void (*StarsHostTimerKill)(void *, StarsU16, StarsU16);
typedef void (*StarsHostTimerDestroy)(void *);
typedef StarsI16 (*StarsHostTimerLoad)(void *, const StarsU8 *);
typedef void (*StarsHostTimerChildren)(void *);
typedef void (*StarsHostTimerFlash)(void *, StarsU16, StarsU16);
typedef void (*StarsHostTimerBeep)(void *, StarsU16);
typedef StarsI16 (*StarsHostTimerOutstanding)(void *);
typedef const StarsU8 *(*StarsHostTimerText)(void *, StarsU16);
typedef void (*StarsHostTimerSetText)(void *, StarsU16, const StarsU8 *);
typedef StarsU16 (*StarsHostTimerWindow)(void *, StarsU16, StarsU16);
typedef void (*StarsHostTimerInvalidate)(void *, StarsU16);
typedef void (*StarsHostTimerProgress)(void *);
typedef void (*StarsHostTimerEnsure)(void *);
typedef void (*StarsHostTimerGenerate)(void *);
typedef void (*StarsHostTimerQuit)(void *, StarsI16);
typedef StarsU16 (*StarsHostTimerDialogItem)(void *, StarsU16, StarsU16);
typedef void (*StarsHostTimerEnable)(void *, StarsU16, StarsU16);
typedef StarsU16 (*StarsHostTimerSetTimer)(void *, StarsU16, StarsU16,
    StarsU32, StarsU32);
typedef void (*StarsHostTimerAlert)(void *, StarsU16, StarsI16);
typedef struct StarsHostTimerOps {
    StarsHostTimerNewTurn new_turn_available;
    StarsHostTimerKill kill_timer;
    StarsHostTimerDestroy destroy_game;
    StarsHostTimerLoad load_game;
    StarsHostTimerChildren create_children;
    StarsHostTimerSetTimer set_timer;
    StarsHostTimerFlash flash_window;
    StarsHostTimerBeep message_beep;
    StarsHostTimerOutstanding turns_outstanding;
    StarsHostTimerText text;
    StarsHostTimerSetText set_window_text;
    StarsHostTimerWindow get_window;
    StarsHostTimerInvalidate invalidate;
    StarsHostTimerProgress show_progress;
    StarsHostTimerProgress hide_progress;
    StarsHostTimerEnsure ensure_ais;
    StarsHostTimerGenerate generate_turn;
    StarsHostTimerQuit post_quit;
    StarsHostTimerDialogItem get_dialog_item;
    StarsHostTimerEnable enable_window;
    StarsHostTimerAlert alert;
} StarsHostTimerOps;
void stars_host_timer_proc(StarsHostTimerState *, StarsU16, const StarsHostTimerOps *,
    void *);
typedef struct StarsHostModePlayer {
    StarsU16 detail;
    StarsU32 salt;
} StarsHostModePlayer;
typedef struct StarsHostModeState {
    StarsU16 timer_id;
    StarsU16 processing_timer;
    StarsU16 player_count;
    StarsU16 global_low_flags;
    StarsU16 global_high_flags;
    StarsU16 auto_generate;
    StarsU16 force_mode;
    StarsU16 frame;
    StarsU16 instance;
    StarsU16 hand_cursor;
    StarsU16 button_brush;
    StarsU32 button_face_color;
    StarsU16 player_id;
    StarsI16 pass_count;
    StarsI16 exit_value;
    StarsU16 message_id;
    StarsI16 dy_arial8;
    StarsU32 tick_last;
    const StarsU8 *game_name;
    const StarsU8 *base_name;
    const StarsU8 *help_file;
    StarsHostModePlayer players[16];
} StarsHostModeState;
typedef StarsU16 (*StarsHostModeBeginPaint)(void *, StarsU16, StarsRect *);
typedef void (*StarsHostModeEndPaint)(void *, StarsU16, const StarsRect *);
typedef void (*StarsHostModeFillRect)(void *, StarsU16, const StarsRect *, StarsU16);
typedef void (*StarsHostModeGetClientRect)(void *, StarsU16, StarsRect *);
typedef StarsU32 (*StarsHostModeSetBkColor)(void *, StarsU16, StarsU32);
typedef void (*StarsHostModeDraw)(void *, StarsU16, StarsU16);
typedef void (*StarsHostModeSticky)(void *, StarsU16, StarsU16);
typedef void (*StarsHostModeSetText)(void *, StarsU16, const StarsU8 *);
typedef void (*StarsHostModeEnable)(void *, StarsU16, StarsU16);
typedef StarsU16 (*StarsHostModeGetItem)(void *, StarsU16, StarsU16);
typedef StarsU16 (*StarsHostModeGetWindow)(void *, StarsU16, StarsU16);
typedef StarsU16 (*StarsHostModeSetTimer)(void *, StarsU16, StarsU16, StarsU32);
typedef void (*StarsHostModeKillTimer)(void *, StarsU16, StarsU16);
typedef void (*StarsHostModeGetCursor)(void *, StarsI16 *, StarsI16 *);
typedef void (*StarsHostModeMapPoint)(void *, StarsU16, StarsI16 *, StarsI16 *);
typedef void (*StarsHostModeSetCursor)(void *, StarsU16);
typedef StarsU16 (*StarsHostModeCreateMenu)(void *);
typedef void (*StarsHostModeAppendMenu)(void *, StarsU16, StarsU16,
    const StarsU8 *);
typedef void (*StarsHostModeTrackMenu)(void *, StarsU16, StarsU16, StarsI16,
    StarsI16, StarsU16);
typedef void (*StarsHostModeDestroyMenu)(void *, StarsU16);
typedef StarsU16 (*StarsHostModePeekMessage)(void *, StarsU16, StarsU16,
    StarsU16, StarsU16, StarsU16 *);
typedef StarsI16 (*StarsHostModeAsyncKey)(void *, StarsU16);
typedef StarsI16 (*StarsHostModeOutstanding)(void *);
typedef void (*StarsHostModeMarkFile)(void *, StarsI16, StarsI16, StarsI16,
    StarsI16);
typedef void (*StarsHostModeEnsure)(void *);
typedef void (*StarsHostModePostQuit)(void *, StarsI16);
typedef void (*StarsHostModeEndDialog)(void *, StarsU16, StarsI16);
typedef StarsI16 (*StarsHostModeAlert)(void *, StarsU16, StarsI16);
typedef StarsU16 (*StarsHostModeGetFocus)(void *);
typedef StarsI16 (*StarsHostModeMessageBox)(void *, StarsU16, const StarsU8 *,
    const StarsU8 *, StarsU16);
typedef StarsI16 (*StarsHostModePasswordCheck)(void *);
typedef StarsU32 (*StarsHostModeMakeProc)(void *, StarsU16);
typedef StarsI16 (*StarsHostModeDialogBox)(void *, StarsU16, StarsU16, StarsU32);
typedef void (*StarsHostModeFreeProc)(void *, StarsU32);
typedef void (*StarsHostModeWinHelp)(void *, StarsU16, const StarsU8 *,
    StarsU16, StarsU16);
typedef StarsI16 (*StarsHostModeOptions)(void *, StarsU16);
typedef struct StarsHostModeOps {
    StarsHostModeBeginPaint begin_paint;
    StarsHostModeEndPaint end_paint;
    StarsHostModeFillRect fill_rect;
    StarsHostModeGetClientRect get_client_rect;
    StarsHostModeSetBkColor set_bk_color;
    StarsHostModeDraw draw_dialog;
    StarsHostModeSticky sticky_position;
    StarsHostModeSetText set_window_text;
    StarsHostModeEnable enable_window;
    StarsHostModeGetItem get_dialog_item;
    StarsHostModeGetWindow get_window;
    StarsHostModeSetTimer set_timer;
    StarsHostModeKillTimer kill_timer;
    StarsHostModeGetCursor get_cursor_pos;
    StarsHostModeMapPoint screen_to_client;
    StarsHostModeMapPoint client_to_screen;
    StarsHostModeSetCursor set_cursor;
    StarsHostModeCreateMenu create_popup_menu;
    StarsHostModeAppendMenu append_menu;
    StarsHostModeTrackMenu track_popup_menu;
    StarsHostModeDestroyMenu destroy_menu;
    StarsHostModePeekMessage peek_message;
    StarsHostModeAsyncKey get_async_key_state;
    StarsHostModeOutstanding turns_outstanding;
    StarsHostModeMarkFile mark_file;
    StarsHostModeEnsure ensure_ais;
    StarsHostModePostQuit post_quit;
    StarsHostModeEndDialog end_dialog;
    StarsHostModeAlert alert;
    StarsHostModeGetFocus get_focus;
    StarsHostModeMessageBox message_box;
    StarsHostModePasswordCheck check_password;
    StarsHostModeMakeProc make_proc;
    StarsHostModeDialogBox dialog_box;
    StarsHostModeFreeProc free_proc;
    void (*set_focus)(void *, StarsU16);
    StarsHostModeWinHelp win_help;
    StarsHostModeOptions options_dialog;
    const StarsU8 *(*string)(void *, StarsU16);
    const StarsU8 *(*compressed)(void *, StarsU16);
} StarsHostModeOps;
StarsI16 stars_host_mode_dialog(StarsHostModeState *, StarsU16, StarsU16,
    StarsU16, StarsU32, const StarsHostModeOps *, void *);
typedef struct StarsOpenGameState {
    StarsU16 ini_flags;
    StarsU32 serial_number;
    StarsU32 ini_lid;
    StarsI16 ini_turn;
    StarsU32 game_lid;
    StarsI16 game_turn;
    StarsI16 player;
    StarsU16 planet_count;
    StarsU16 frame;
    StarsU16 title;
    StarsU16 freeing_title;
    StarsU16 orders_visible;
    StarsU16 global_flags;
    StarsI16 file_error_silent;
    StarsI16 selection_object;
    StarsI16 selection_waypoint;
    StarsI16 selection_id;
    char selected_path[512];
    char startup_path[512];
    char base_path[512];
    char race_path[512];
} StarsOpenGameState;
typedef StarsI16 (*StarsOpenGameDialog)(void *, StarsU16, StarsU8 *,
    StarsU16);
typedef StarsI16 (*StarsOpenGameClassify)(void *, const StarsU8 *, StarsI16);
typedef void (*StarsOpenGameSetWork)(void *, StarsI16, StarsI16);
typedef void (*StarsOpenGameDestroy)(void *);
typedef StarsI16 (*StarsOpenGameLoad)(void *, const StarsU8 *, const StarsU8 *);
typedef void (*StarsOpenGameCreateChildren)(void *);
typedef void (*StarsOpenGameDestroyWindow)(void *, StarsU16);
typedef void (*StarsOpenGameHost)(void *);
typedef void (*StarsOpenGameSendMessage)(void *, StarsU16, StarsU16,
    StarsU16, StarsU32);
typedef void (*StarsOpenGameFindSelect)(void *);
typedef void (*StarsOpenGameRaceWizard)(void *, StarsU16);
typedef struct StarsOpenGameOps {
    StarsOpenGameDialog open_dialog;
    StarsOpenGameClassify was_race_file;
    StarsOpenGameSetWork set_work_from_date;
    StarsOpenGameDestroy destroy_game;
    StarsOpenGameLoad load_game;
    StarsOpenGameCreateChildren create_children;
    StarsOpenGameDestroyWindow destroy_window;
    StarsOpenGameHost bring_up_host;
    StarsOpenGameSendMessage send_message;
    StarsOpenGameFindSelect find_select;
    StarsOpenGameRaceWizard race_wizard;
} StarsOpenGameOps;
StarsI16 stars_open_game(StarsOpenGameState *, StarsU16, StarsI16,
    const StarsOpenGameOps *, void *);
typedef struct StarsFrameState {
    StarsU16 timer_id;
    StarsU16 timer_type;
    StarsU16 window_layout;
    StarsI16 dx;
    StarsI16 dy;
    StarsI16 x_top;
    StarsI16 y1;
    StarsI16 y2;
    StarsI16 dx_plan_want;
    StarsI16 dy_msg_want;
    StarsI16 dy_min_want;
    StarsI16 dx2_plan_want;
    StarsI16 dy2_msg_want;
    StarsI16 dy2_min_want;
    StarsI16 dy_arial8;
    StarsI16 dy_sys_font;
    StarsI16 dy_sbar;
    StarsU16 frame;
    StarsU16 scanner;
    StarsU16 message_window;
    StarsU16 planet;
    StarsU16 title;
    StarsU16 instance;
    StarsU16 hbr_shadow;
    StarsU16 hbr_hilite;
    StarsU16 hbr_desktop;
    StarsU16 hbr_face;
    StarsU16 hbr_50;
    StarsU16 hicon_host;
    StarsU16 hicon_stars;
    StarsU16 hicon_wait;
    StarsU16 palette;
    StarsU16 freeing_title;
    StarsI16 player;
    StarsU32 game_lid;
    StarsU16 global_low_flags;
    StarsU16 global_high_flags;
    StarsU16 ini_flags;
    StarsI16 file_error_silent;
    StarsU32 serial_number;
    StarsU32 ini_lid;
    StarsI16 ini_turn;
    StarsI16 game_turn;
    StarsU16 planet_count;
    StarsI16 selected_object;
    StarsI16 selected_planet;
    StarsI16 selected_id;
    StarsInvertPoint selected_point;
    StarsU16 tutorial_flags;
    StarsI16 turn_generation_count;
    StarsU8 machine_config[11];
    StarsU8 current_environment[11];
    StarsI16 exit_value;
    StarsU16 batch_active;
    char serial_text[128];
    const StarsU8 *base_name;
} StarsFrameState;
typedef StarsI16 (*StarsFrameTextMetrics)(void *, StarsU16);
typedef void (*StarsFrameVoid)(void *);
typedef void (*StarsFrameDestroyWindow)(void *, StarsU16);
typedef StarsI16 (*StarsFrameMarkFile)(void *, StarsI16, StarsI16, StarsI16,
    StarsI16);
typedef void (*StarsFramePostQuit)(void *, StarsI16);
typedef void (*StarsFrameExitWindows)(void *, StarsI16);
typedef void (*StarsFrameRefit)(void *);
typedef StarsU16 (*StarsFrameBeginPaint)(void *, StarsU16, StarsRect *);
typedef void (*StarsFrameEndPaint)(void *, StarsU16, const StarsRect *);
typedef StarsU16 (*StarsFrameGetDc)(void *, StarsU16);
typedef void (*StarsFrameReleaseDc)(void *, StarsU16, StarsU16);
typedef void (*StarsFrameKillTimer)(void *, StarsU16, StarsU16);
typedef void (*StarsFrameFillRect)(void *, StarsU16, const StarsRect *, StarsU16);
typedef StarsI16 (*StarsFrameIsIconic)(void *, StarsU16);
typedef StarsU16 (*StarsFrameSelectObject)(void *, StarsU16, StarsU16);
typedef void (*StarsFramePatBlt)(void *, StarsU16, StarsI16, StarsI16,
    StarsI16, StarsI16, StarsU32);
typedef void (*StarsFrameDrawIcon)(void *, StarsU16, StarsI16, StarsI16,
    StarsU16);
typedef void (*StarsFrameGetRect)(void *, StarsU16, StarsRect *);
typedef void (*StarsFrameExcludeClip)(void *, StarsU16, const StarsRect *);
typedef void (*StarsFrameMapWindow)(void *, StarsU16, StarsU16, StarsRect *);
typedef void (*StarsFrameGetCursor)(void *, StarsI16 *, StarsI16 *);
typedef void (*StarsFrameMapPoint)(void *, StarsU16, StarsI16 *, StarsI16 *);
typedef StarsI16 (*StarsFramePointInRect)(void *, const StarsRect *, StarsI16,
    StarsI16);
typedef StarsU16 (*StarsFrameCursorFromPoint)(void *, StarsInvertPoint *,
    StarsI16 *);
typedef void (*StarsFrameSetCursor)(void *, StarsU16);
typedef StarsU32 (*StarsFrameDefaultProc)(void *, StarsU16, StarsU16,
    StarsU16, StarsU32);
typedef void (*StarsFrameSendMessage)(void *, StarsU16, StarsU16, StarsU16,
    StarsU32);
typedef void (*StarsFrameChangeProduction)(void *, StarsI16);
typedef StarsU16 (*StarsFrameFindStarbase)(void *, StarsI16, StarsU16);
typedef void (*StarsFrameShipCommand)(void *, StarsU16, StarsU16, StarsU16);
typedef void (*StarsFrameSelectAdjacent)(void *, StarsI16, StarsU16);
typedef void (*StarsFrameCommand)(void *, StarsU16, StarsU16);
typedef StarsI16 (*StarsFrameNewTurn)(void *, StarsI16);
typedef StarsI16 (*StarsFrameAlert)(void *, StarsU16, StarsI16);
typedef StarsI16 (*StarsFrameLoadGame)(void *, const StarsU8 *, StarsU16);
typedef void (*StarsFrameCreateChildren)(void *);
typedef void (*StarsFrameScanToPoint)(void *, StarsInvertPoint *, StarsI16);
typedef void (*StarsFrameInitializeMenu)(void *, StarsU16);
typedef void (*StarsFrameAdvanceTutor)(void *);
typedef void (*StarsFrameShowTutor)(void *, StarsI16);
typedef void (*StarsFrameVerifyTurns)(void *);
typedef void (*StarsFrameEnsureAis)(void *);
typedef void (*StarsFrameGenerateTurn)(void *);
typedef void (*StarsFrameGenerateNewGame)(void *, const StarsU8 *);
typedef StarsI16 (*StarsFrameLoadLog)(void *, const StarsU8 *);
typedef void (*StarsFrameRunLog)(void *);
typedef StarsI16 (*StarsFrameTurnsSafe)(void *);
typedef StarsI16 (*StarsFrameNextBatch)(void *, StarsU8 *, StarsU16);
typedef void (*StarsFrameClearFile)(void *, StarsI16);
typedef void (*StarsFrameOutput)(void *, StarsI16, const StarsU8 *);
typedef StarsU16 (*StarsFrameSelectPalette)(void *, StarsU16, StarsU16, StarsU16);
typedef StarsI16 (*StarsFrameRealizePalette)(void *, StarsU16);
typedef void (*StarsFrameGetSystemColors)(void *);
typedef StarsI16 (*StarsFrameSystemMetric)(void *, StarsU16);
typedef StarsU16 (*StarsFrameCreateWindow)(void *, StarsU16, StarsU16,
    StarsU32, StarsI16, StarsI16, StarsI16, StarsI16, StarsU16);
typedef void (*StarsFrameShowWindow)(void *, StarsU16, StarsU16);
typedef StarsU32 (*StarsFrameMakeProc)(void *, StarsU16);
typedef StarsI16 (*StarsFrameDialogBox)(void *, StarsU16, StarsU16, StarsU32);
typedef void (*StarsFrameFreeProc)(void *, StarsU32);
typedef StarsI16 (*StarsFrameValidSerial)(void *, const StarsU8 *, StarsU32 *);
typedef StarsI16 (*StarsFrameCompareEnvironment)(void *, const StarsU8 *,
    const StarsU8 *, StarsU16);
typedef void (*StarsFrameWriteIni)(void *);
typedef void (*StarsFrameOutputString)(void *, StarsI16, const StarsU8 *);
typedef StarsI16 (*StarsFrameMouseMove)(void *, StarsInvertPoint *);
typedef StarsInvertPoint (*StarsFrameInvert)(void *, StarsU16, StarsI16,
    StarsInvertPoint, const StarsInvertPoint *);
typedef void (*StarsFrameCapture)(void *, StarsU16);
typedef void (*StarsFrameReleaseCapture)(void *);
typedef void (*StarsFrameSetNcSize)(void *, StarsU32, StarsI16, StarsI16);
typedef StarsI16 (*StarsFrameTimerCount)(void *);
typedef struct StarsFrameOps {
    StarsFrameTextMetrics create_fonts;
    StarsFrameTextMetrics get_text_metrics;
    StarsFrameVoid init_tiles;
    StarsFrameVoid ensure_tile_size;
    StarsFrameVoid write_ini;
    StarsFrameDestroyWindow destroy_window;
    StarsFrameMarkFile mark_file;
    StarsFrameVoid destroy_game;
    StarsFramePostQuit post_quit;
    StarsFrameExitWindows exit_windows;
    StarsFrameRefit refit;
    StarsFrameGetDc get_dc;
    StarsFrameReleaseDc release_dc;
    StarsFrameKillTimer kill_timer;
    StarsFrameBeginPaint begin_paint;
    StarsFrameEndPaint end_paint;
    StarsFrameFillRect fill_rect;
    StarsFrameIsIconic is_iconic;
    StarsFrameSelectObject select_object;
    StarsFramePatBlt pat_blt;
    StarsFrameDrawIcon draw_icon;
    StarsFrameGetRect get_client_rect;
    StarsFrameExcludeClip exclude_clip_rect;
    StarsFrameMapWindow map_window_points;
    StarsFrameGetCursor get_cursor_pos;
    StarsFrameMapPoint screen_to_client;
    StarsFramePointInRect point_in_rect;
    StarsFrameCursorFromPoint cursor_from_point;
    StarsFrameSetCursor set_cursor;
    StarsFrameDefaultProc default_proc;
    StarsFrameSendMessage send_message;
    StarsFrameChangeProduction change_production;
    StarsFrameFindStarbase find_starbase;
    StarsFrameShipCommand ship_command;
    StarsFrameSelectAdjacent select_planet;
    StarsFrameSelectAdjacent select_fleet;
    StarsFrameCommand command_handler;
    StarsFrameNewTurn new_turn_available;
    StarsFrameAlert alert;
    StarsFrameLoadGame load_game;
    StarsFrameCreateChildren create_children;
    StarsFrameScanToPoint scan_to_point;
    StarsFrameInitializeMenu initialize_menu;
    StarsFrameAdvanceTutor advance_tutor;
    StarsFrameShowTutor show_tutor;
    StarsFrameVerifyTurns verify_turns;
    StarsFrameEnsureAis ensure_ais;
    StarsFrameGenerateTurn generate_turn;
    StarsFrameGenerateNewGame generate_new_game;
    StarsFrameLoadLog load_log;
    StarsFrameRunLog run_log;
    StarsFrameTurnsSafe turns_safe;
    StarsFrameNextBatch next_batch_line;
    StarsFrameClearFile clear_file;
    StarsFrameOutput output;
    StarsFrameSelectPalette select_palette;
    StarsFrameRealizePalette realize_palette;
    StarsFrameGetSystemColors get_system_colors;
    StarsFrameSystemMetric system_metric;
    StarsFrameCreateWindow create_window;
    StarsFrameShowWindow show_window;
    StarsFrameMakeProc make_proc;
    StarsFrameDialogBox dialog_box;
    StarsFrameFreeProc free_proc;
    StarsFrameValidSerial valid_serial;
    StarsFrameCompareEnvironment compare_environment;
    StarsFrameMouseMove mouse_move;
    StarsFrameInvert invert_border;
    StarsFrameCapture set_capture;
    StarsFrameReleaseCapture release_capture;
    StarsFrameSetNcSize set_nc_size;
    StarsFrameTimerCount timer_count;
    StarsFrameOutputString output_string;
} StarsFrameOps;
StarsU32 stars_frame_wnd_proc(StarsFrameState *, StarsU16, StarsU16,
    StarsU16, StarsU32, const StarsFrameOps *, void *);
typedef void (*StarsDestroyTutorWindow)(void *, StarsU16);
typedef void (*StarsRestoreTutorGame)(void *);
typedef StarsU32 (*StarsTutorTick)(void *);
typedef void (*StarsTutorRandomize2)(void *, StarsU32);
typedef struct StarsEndTutorOps {
    StarsDestroyTutorWindow destroy_window;
    StarsRestoreTutorGame restore_game;
    StarsTutorTick get_tick;
    StarsTutorRandomize2 randomize2;
} StarsEndTutorOps;
void stars_end_tutor(StarsU8 *, StarsU8 *, StarsI16,
    const StarsEndTutorOps *, void *);
typedef void (*StarsRestoreCheckMenu)(void *, StarsU16, StarsU16, StarsU16);
typedef StarsU16 (*StarsRestoreSubMenu)(void *, StarsU16, StarsU16);
typedef void (*StarsRestoreRefit)(void *);
typedef void (*StarsRestoreGameInvalidate)(void *, StarsU16, StarsU16);
typedef struct StarsRestoreGameStateInput {
    StarsU16 grbit_scan;
    StarsI16 scan_zoom;
    StarsU16 tutor_grbit_scan;
    StarsI16 tutor_scan_zoom;
    StarsU8 zip_prod_current[13][2];
    StarsU8 zip_prod_saved[13][2];
    StarsU16 zip_current_valid;
    StarsU16 zip_saved_valid;
    StarsU16 tutor_zpq[13];
    StarsU16 tutor_zip_valid;
    StarsU16 game_grbits;
    StarsU16 frame_window;
    StarsI16 report_sort;
    StarsI16 tutor_report_sort;
    StarsRestoreSubMenu get_sub_menu;
    StarsRestoreCheckMenu check_menu;
    StarsRestoreRefit refit_children;
    StarsRestoreGameInvalidate invalidate_report;
    void *context;
} StarsRestoreGameStateInput;
void stars_restore_game_state(StarsRestoreGameStateInput *);
typedef StarsU16 (*StarsShowMainGetDlgItem)(void *, StarsU16, StarsU16);
typedef void (*StarsShowMainWindow)(void *, StarsU16, StarsI16);
typedef const StarsU8 *(*StarsShowMainGetString)(void *, StarsI16);
typedef void (*StarsShowMainSetText)(void *, StarsU16, StarsU16,
    const StarsU8 *);
typedef struct StarsShowMainControlsInput {
    StarsU16 hwnd;
    StarsI16 show;
    StarsShowMainGetDlgItem get_dialog_item;
    StarsShowMainWindow show_window;
    StarsShowMainGetString get_string;
    StarsShowMainSetText set_dialog_item_text;
    void *context;
} StarsShowMainControlsInput;
void stars_show_main_controls(StarsShowMainControlsInput *);
typedef StarsI16 (*StarsBuildPartLookup)(void *, StarsU16, StarsU16,
    StarsU16 *);
typedef void (*StarsBuildPartSend)(void *, StarsU16, StarsU16,
    const StarsU8 *);
typedef struct StarsFillBuildPartsInput {
    StarsU16 hwnd_list;
    StarsU16 component_groups;
    StarsI16 starbase_mode;
    StarsBuildPartLookup lookup_part;
    StarsBuildPartSend send_message;
    void *context;
} StarsFillBuildPartsInput;
void stars_fill_build_parts_lb(StarsFillBuildPartsInput *);
typedef void *(*StarsShipBuilderNthShdef)(void *, StarsU16);
typedef void *(*StarsShipBuilderMakeProc)(void *, StarsU16);
typedef StarsI16 (*StarsShipBuilderDialogBox)(void *, StarsU16, StarsU16,
    StarsU16, void *);
typedef void (*StarsShipBuilderFreeProc)(void *, void *);
typedef void (*StarsShipBuilderFillProduction)(void *, StarsU16,
    const void *, const void *);
typedef struct StarsShipBuilderInput {
    StarsPoint dialog_size;
    StarsI16 dialog_font_height;
    StarsU16 display_flags;
    StarsU16 instance;
    StarsU16 frame_window;
    StarsU16 production_list;
    StarsU16 selected_object_type;
    const void *selected_production;
    void *ship_definition;
    StarsShipBuilderNthShdef nth_valid_shdef;
    StarsShipBuilderMakeProc make_proc_instance;
    StarsShipBuilderDialogBox dialog_box;
    StarsShipBuilderFreeProc free_proc_instance;
    StarsShipBuilderFillProduction fill_production;
    void *context;
} StarsShipBuilderInput;
StarsI16 stars_ship_builder(StarsShipBuilderInput *);
typedef struct StarsFleetComponent {
    StarsI16 quantity;
    StarsU16 packed_mass;
} StarsFleetComponent;
typedef void (*StarsFleetCompFormat)(void *, StarsU16, StarsU16, StarsI16,
    StarsI16, StarsU8 *, StarsU16);
typedef void (*StarsFleetCompSend)(void *, StarsU16, StarsU16,
    const StarsU8 *);
typedef struct StarsFillFleetCompInput {
    StarsU16 hwnd_list;
    StarsFleetComponent components[16];
    StarsFleetCompFormat format_entry;
    StarsFleetCompSend send_message;
    void *context;
} StarsFillFleetCompInput;
void stars_fill_fleet_comp_lb(StarsFillFleetCompInput *);
typedef void (*StarsScanGetClientRect)(void *, StarsU16, StarsRect *);
typedef void (*StarsScanExcludeClip)(void *, StarsU16, StarsI16, StarsI16,
    StarsI16, StarsI16);
typedef StarsU16 (*StarsScanGetStockObject)(void *, StarsU16);
typedef StarsU16 (*StarsScanSelectObject)(void *, StarsU16, StarsU16);
typedef StarsU16 (*StarsScanSetRop2)(void *, StarsU16, StarsU16);
typedef void (*StarsScanMoveTo)(void *, StarsU16, StarsI16, StarsI16);
typedef void (*StarsScanLineTo)(void *, StarsU16, StarsI16, StarsI16);
typedef void (*StarsScanSelectClip)(void *, StarsU16, StarsU16);
typedef struct StarsDrawScanXorLinesInput {
    StarsU16 hdc;
    StarsU16 scanner_window;
    StarsPoint points[4];
    StarsI16 point_count;
    StarsI16 scrollbar_height;
    StarsU16 ship_pen;
    StarsU16 huge_region;
    StarsScanGetClientRect get_client_rect;
    StarsScanExcludeClip exclude_clip_rect;
    StarsScanGetStockObject get_stock_object;
    StarsScanSelectObject select_object;
    StarsScanSetRop2 set_rop2;
    StarsScanMoveTo move_to;
    StarsScanLineTo line_to;
    StarsScanSelectClip select_clip_region;
    void *context;
} StarsDrawScanXorLinesInput;
void stars_draw_scan_xor_lines(StarsDrawScanXorLinesInput *);
typedef struct StarsResearchPlanet {
    StarsI16 owner;
    StarsI16 production_active;
} StarsResearchPlanet;
typedef StarsU32 (*StarsResearchResources)(void *, StarsU16, StarsI16);
typedef StarsU32 (*StarsResearchProduction)(void *, StarsU16, StarsI16);
typedef struct StarsProjectedResearchInput {
    StarsI32 percentage;
    StarsU8 *player_percentage;
    StarsI16 player;
    const StarsResearchPlanet *planets;
    StarsU16 planet_count;
    StarsResearchResources resources_at_planet;
    StarsResearchProduction production_schedule;
    void *context;
} StarsProjectedResearchInput;
StarsI32 stars_projected_research_spending(StarsProjectedResearchInput *);
typedef struct StarsSaveProdQueueItem {
    StarsU8 no_research;
    StarsU8 count;
} StarsSaveProdQueueItem;
typedef StarsU16 (*StarsSaveGetSubMenu)(void *, StarsU16, StarsU16);
typedef void (*StarsSaveCheckMenu)(void *, StarsU16, StarsU16, StarsU16);
typedef void (*StarsSaveRefitChildren)(void *);
typedef void (*StarsSaveInvalidateReport)(void *, StarsU16, StarsU16);
typedef void (*StarsSaveDestroyGame)(void *);
typedef struct StarsSaveGameStateInput {
    StarsU16 current_grbit_scan;
    StarsI16 current_scan_zoom;
    StarsU16 tutor_grbit_scan;
    StarsI16 tutor_scan_zoom;
    StarsU16 tutor_flags;
    StarsU16 tutor_zpq[13];
    StarsU16 tutor_zip_valid;
    StarsU16 display_flags;
    StarsU16 frame_window;
    StarsSaveProdQueueItem current_queue[13];
    StarsSaveProdQueueItem saved_queue[13];
    StarsU16 current_valid;
    StarsI16 report_sort;
    StarsI16 report_ascending;
    StarsI16 tutor_report_sort;
    StarsU32 game_lid;
    StarsU16 game_flags;
    StarsSaveGetSubMenu get_sub_menu;
    StarsSaveCheckMenu check_menu;
    StarsSaveRefitChildren refit_children;
    StarsSaveInvalidateReport invalidate_report;
    StarsSaveDestroyGame destroy_game;
    void *context;
} StarsSaveGameStateInput;
void stars_save_game_state(StarsSaveGameStateInput *);
typedef StarsI16 (*StarsTutorConfirmEnd)(void *, const StarsU8 *);
typedef void (*StarsTutorEndCallback)(void *, StarsI16);
StarsI16 stars_ask_kill_tutor(StarsU16, const StarsU8 *,
    StarsTutorConfirmEnd, StarsTutorEndCallback, void *);
typedef const StarsU8 *(*StarsTokenHullLookup)(void *, const StarsU8 *);
typedef void (*StarsSetBattlePlayer)(void *, StarsI16);
typedef StarsI16 (*StarsInitHull)(void *, const StarsU8 *, StarsU8 *);
typedef struct StarsCheckInitiativeOps {
    StarsTokenHullLookup lookup_hull;
    StarsSetBattlePlayer set_player;
    StarsInitHull init_hull;
} StarsCheckInitiativeOps;
void stars_check_initiative(StarsU8 *, const StarsCheckInitiativeOps *, void *);
typedef StarsI32 (*StarsCalcPlanetMaxPop)(void *, StarsU16, StarsI16);
typedef StarsI16 (*StarsGetPlayerStat)(void *, StarsI16, StarsI16);
typedef struct StarsCMaxMinesOps {
    StarsCalcPlanetMaxPop calc_max_population;
    StarsGetPlayerStat get_player_stat;
} StarsCMaxMinesOps;
typedef struct StarsCalcPlanetMaxPopInput {
    StarsI16 planet_owner;
    StarsI16 player;
    StarsI16 race_stat;
    StarsI16 race_grbit;
    StarsI32 desirability;
    StarsI32 starbase_population;
    StarsU16 has_starbase;
} StarsCalcPlanetMaxPopInput;
StarsI32 stars_calc_planet_max_pop(const StarsCalcPlanetMaxPopInput *);
StarsI16 stars_cmax_mines(const StarsU8 *, StarsI16,
    const StarsCMaxMinesOps *, void *);
typedef StarsI32 (*StarsChangePlanetPopulation)(void *, const StarsU8 *, StarsI16);
typedef struct StarsCMaxOperableMinesOps {
    StarsCMaxMinesOps max_mines;
    StarsChangePlanetPopulation change_population;
} StarsCMaxOperableMinesOps;
StarsI16 stars_cmax_operable_mines(const StarsU8 *, StarsI16, StarsI16,
    const StarsCMaxOperableMinesOps *, void *);
StarsI16 stars_cmines_operating(const StarsU8 *,
    const StarsCMaxOperableMinesOps *, void *);
typedef struct StarsCMaxOperableFactoriesOps {
    StarsCMaxMinesOps max_factories;
    StarsChangePlanetPopulation change_population;
} StarsCMaxOperableFactoriesOps;
StarsI16 stars_cmax_factories(const StarsU8 *, StarsI16,
    const StarsCMaxMinesOps *, void *);
StarsI16 stars_cmax_operable_factories(const StarsU8 *, StarsI16, StarsI16,
    const StarsCMaxOperableFactoriesOps *, void *);
StarsI16 stars_cfactories_operating(const StarsU8 *,
    const StarsCMaxOperableFactoriesOps *, void *);
typedef StarsI16 (*StarsPctPlanetDesirability)(void *, const StarsU8 *,
    StarsI16);
typedef struct StarsCMaxDefensesOps {
    StarsPctPlanetDesirability planet_desirability;
    StarsGetPlayerStat get_player_stat;
} StarsCMaxDefensesOps;
StarsI16 stars_cmax_defenses(const StarsU8 *, StarsI16,
    const StarsCMaxDefensesOps *, void *);
typedef struct StarsCMaxOperableDefensesOps {
    StarsCMaxDefensesOps max_defenses;
    StarsChangePlanetPopulation change_population;
} StarsCMaxOperableDefensesOps;
StarsI16 stars_cmax_operable_defenses(const StarsU8 *, StarsI16, StarsI16,
    const StarsCMaxOperableDefensesOps *, void *);
typedef StarsI16 (*StarsGetPlayerByte)(void *, StarsI16, StarsU16);
typedef struct StarsCResourcesAtPlanetOps {
    StarsCalcPlanetMaxPop calc_max_population;
    StarsPctPlanetDesirability planet_desirability;
    StarsGetPlayerStat get_player_stat;
    StarsGetPlayerByte get_player_byte;
    StarsCalcPlanetMaxPop factory_calc_max_population;
    StarsGetPlayerStat factory_get_player_stat;
    StarsChangePlanetPopulation factory_change_population;
} StarsCResourcesAtPlanetOps;
StarsI16 stars_cresources_at_planet(const StarsU8 *, StarsI16,
    const StarsCResourcesAtPlanetOps *, void *);
typedef struct StarsWarpMAItem {
    StarsU32 identifier;
    StarsU16 flags;
} StarsWarpMAItem;
typedef StarsU16 (*StarsGetWarpMAItems)(void *, StarsI16, StarsU16,
    StarsWarpMAItem *, StarsU16);
typedef StarsI16 (*StarsGetWarpMARaceByte)(void *, StarsI16, StarsU16,
    StarsU16);
StarsI16 stars_iwarp_ma_from_planet(const StarsU8 *, StarsI16, StarsI16 *,
    const StarsGetWarpMAItems, const StarsGetWarpMARaceByte, void *);
typedef StarsU16 (*StarsGetStargateItems)(void *, StarsI16, StarsU16,
    StarsWarpMAItem *, StarsU16);
typedef StarsI16 (*StarsLookupStargateFuel)(void *, StarsU32, StarsU16);
StarsI16 stars_stargate_range_from_planet(const StarsU8 *, StarsI16, StarsI16,
    const StarsGetStargateItems, const StarsLookupStargateFuel, void *);
StarsI16 stars_fprod_is_terra(const StarsU8 *);
typedef StarsI16 (*StarsCanTerraform)(void *, const StarsU8 *, StarsI16 *,
    StarsI16 *, StarsI16 *, StarsI16);
StarsI16 stars_ipct_can_terraform(const StarsU8 *, StarsCanTerraform, void *);
typedef void (*StarsFreePlanetProduction)(void *, StarsI16, StarsU32);
typedef struct StarsUninhabitPlanetOps {
    StarsGetPlayerStat get_player_stat;
    StarsFreePlanetProduction free_production;
} StarsUninhabitPlanetOps;
void stars_uninhabit_planet(StarsU8 *, const StarsUninhabitPlanetOps *, void *);
typedef struct StarsCloakSection {
    StarsU16 group;
    StarsU16 flags;
} StarsCloakSection;
typedef StarsI16 (*StarsGetRaceGrbit)(void *, StarsI16, StarsI16);
typedef StarsI16 (*StarsGetCloakPartPoints)(void *, StarsU16, StarsU16);
typedef struct StarsPctCloakOps {
    StarsGetRaceGrbit get_race_grbit;
    StarsGetPlayerStat get_player_stat;
    StarsGetCloakPartPoints get_part_points;
} StarsPctCloakOps;
StarsI16 stars_pct_cloak_from_huldef(const StarsU8 *, StarsI16, StarsI16 *,
    const StarsCloakSection *, StarsU16, const StarsPctCloakOps *, void *);
StarsI16 stars_pct_planet_capacity(const StarsU8 *, StarsI16,
    StarsCalcPlanetMaxPop, void *);
typedef struct StarsAdjacentStarbasePlanet {
    StarsI16 id;
    StarsI16 owner;
    StarsU16 flags;
    StarsU16 hull_id;
} StarsAdjacentStarbasePlanet;
typedef StarsU16 (*StarsGetHullCargoMax)(void *, StarsU16);
StarsI16 stars_id_find_adj_starbase(const StarsAdjacentStarbasePlanet *,
    StarsU16, StarsI16, StarsI16, StarsI16, StarsGetHullCargoMax, void *);
typedef struct StarsProductionEtaInput {
    StarsI16 eta_first;
    StarsI16 eta_last;
    StarsI16 item_index;
    StarsU16 production_count;
    StarsU16 item_phase;
    StarsU16 item_stage;
} StarsProductionEtaInput;
typedef void (*StarsProductionEtaString)(void *, StarsU16, StarsU8 *, StarsU16);
typedef void (*StarsProductionEtaFormat)(void *, StarsU16, StarsI16,
    StarsI16, StarsU8 *, StarsU16);
typedef struct StarsProductionEtaOps {
    StarsProductionEtaString get_string;
    StarsProductionEtaFormat format;
} StarsProductionEtaOps;
StarsI16 stars_psz_production_eta(const StarsProductionEtaInput *,
    StarsI16 *, StarsI16 *, StarsU8 *, StarsU16,
    const StarsProductionEtaOps *, void *);
typedef struct StarsCheckLogRecord {
    StarsU16 type;
    StarsU16 length;
    StarsU16 first_word;
} StarsCheckLogRecord;
typedef struct StarsCheckLogFileInput {
    StarsU16 opened;
    StarsI16 open_error;
    const StarsCheckLogRecord *records;
    StarsU16 record_count;
} StarsCheckLogFileInput;
typedef struct StarsCheckLogFileState {
    StarsI16 memory_current;
    StarsI16 memory_previous;
    StarsU16 current_type;
    StarsU16 current_first_word;
} StarsCheckLogFileState;
StarsI16 stars_check_log_file(const StarsCheckLogFileInput *,
    StarsCheckLogFileState *, StarsI16 *);
typedef struct StarsPlanetOptValueInput {
    StarsI16 player;
    StarsI16 environment[3];
    StarsI16 target[3];
    StarsI16 terraform_min[3];
    StarsI16 terraform_max[3];
    StarsU16 can_terraform;
} StarsPlanetOptValueInput;
typedef StarsI16 (*StarsPlanetOptDesirability)(void *, const StarsI16 *,
    StarsI16);
StarsI16 stars_pct_planet_opt_value(const StarsPlanetOptValueInput *,
    StarsI16, StarsPlanetOptDesirability, void *);
typedef struct StarsGetEtaInput {
    StarsU16 hdc;
    StarsU16 cord;
    StarsU16 order_flags;
    StarsU16 order_words[5];
    StarsU32 fuel_use;
    StarsU32 fuel_available;
    const StarsU8 *dash_text;
} StarsGetEtaInput;
typedef void (*StarsGetFleetEtaText)(void *, StarsU16, StarsU8 *, StarsU16);
typedef void (*StarsSetEtaColor)(void *, StarsU16);
typedef struct StarsGetEtaOps {
    StarsGetFleetEtaText get_text;
    StarsSetEtaColor set_color;
} StarsGetEtaOps;
StarsI16 stars_get_eta(const StarsGetEtaInput *, StarsU8 *, StarsU16,
    StarsI16 *, const StarsGetEtaOps *, void *);
typedef struct StarsFleetWp0Input {
    StarsU16 cord;
    StarsU16 order_flags;
    StarsU16 order_words[5];
} StarsFleetWp0Input;
StarsI16 stars_fdest_is_wp0(const StarsFleetWp0Input *);
typedef struct StarsReportPlanetRow {
    StarsI16 player;
    StarsU16 status;
} StarsReportPlanetRow;
typedef struct StarsReportFleetRow {
    StarsI16 player;
    StarsU16 present;
} StarsReportFleetRow;
typedef StarsI16 (*StarsReportIndexCompare)(void *, StarsI16, StarsI16,
    StarsI16);
typedef StarsI16 (*StarsReportBattleCount)(void *);
typedef struct StarsSortReportCacheInput {
    StarsI16 report;
    StarsI16 sort_column;
    StarsI16 current_sort_column;
    StarsI16 current_subsort;
    StarsI16 current_ascending;
    StarsU16 report_window;
    StarsI16 cached;
    StarsI16 player;
    const StarsReportPlanetRow *planets;
    StarsI16 planet_count;
    const StarsReportFleetRow *fleets;
    StarsI16 fleet_count;
    StarsReportIndexCompare compare;
    StarsReportBattleCount battle_count;
} StarsSortReportCacheInput;
typedef struct StarsSortReportCacheState {
    StarsI16 previous_sort_column;
    StarsI16 previous_subsort;
    StarsI16 previous_ascending;
    StarsI16 cached;
    StarsI16 rows;
    StarsU16 battle_cached;
    StarsI16 indices[1025];
} StarsSortReportCacheState;
void stars_sort_report_cache(const StarsSortReportCacheInput *,
    StarsSortReportCacheState *);
/* ICompReport compares two already-resolved report rows.  The original
 * routine obtains names, percentages, battle counts, and ship statistics from
 * several far-pointer record families before doing the final comparison.
 * Keeping those lookups in the row adapter makes the comparison itself a
 * normal C operation and avoids exposing the original packed records to every
 * caller. */
#define STARS_ICOMP_REPORT_COLUMNS 16
typedef struct StarsICompReportRow {
    StarsI16 valid[STARS_ICOMP_REPORT_COLUMNS];
    StarsI32 value[STARS_ICOMP_REPORT_COLUMNS];
    const StarsU8 *text[STARS_ICOMP_REPORT_COLUMNS];
    StarsI32 tie_value[STARS_ICOMP_REPORT_COLUMNS];
} StarsICompReportRow;
typedef StarsI16 (*StarsICompReportTextCompare)(void *,
    const StarsU8 *, const StarsU8 *);
typedef struct StarsICompReportInput {
    StarsI16 report_kind;
    StarsI16 sort_column;
    StarsI16 subsort;
    StarsI16 ascending;
    StarsI16 previous_sort_column;
    StarsI16 previous_subsort;
    StarsI16 previous_ascending;
    const StarsICompReportRow *left;
    const StarsICompReportRow *right;
    StarsICompReportTextCompare compare_text;
    void *context;
} StarsICompReportInput;
StarsI16 stars_i_comp_report(const StarsICompReportInput *);
typedef struct StarsZipOrder {
    StarsU16 valid;
    StarsU16 words[5];
    const StarsU8 *name;
} StarsZipOrder;
typedef StarsU16 (*StarsZipOrderDlgBeginPaint)(void *, StarsU16, StarsRect *);
typedef void (*StarsZipOrderDlgEndPaint)(void *, StarsU16, StarsRect *);
typedef void (*StarsZipOrderDlgGetClientRect)(void *, StarsU16, StarsRect *);
typedef void (*StarsZipOrderDlgFillRect)(void *, StarsU16, const StarsRect *,
    StarsU16);
typedef StarsU16 (*StarsZipOrderDlgGetDialogItem)(void *, StarsU16, StarsU16);
typedef void (*StarsZipOrderDlgGetWindowRect)(void *, StarsU16, StarsRect *);
typedef void (*StarsZipOrderDlgScreenToClient)(void *, StarsU16, StarsRect *);
typedef void (*StarsZipOrderDlgExpandRect)(void *, StarsRect *, StarsI16,
    StarsI16);
typedef void (*StarsZipOrderDlgDrawFrame)(void *, StarsU16, const StarsRect *,
    StarsI16);
typedef StarsU16 (*StarsZipOrderDlgSelectObject)(void *, StarsU16, StarsU16);
typedef void (*StarsZipOrderDlgSetBackground)(void *, StarsU16, StarsU32);
typedef void (*StarsZipOrderDlgSetTextColor)(void *, StarsU16, StarsU32);
typedef StarsI16 (*StarsZipOrderDlgGetString)(void *, StarsU16, StarsU8 *,
    StarsU16);
typedef const StarsU8 *(*StarsZipOrderDlgCompressedString)(void *, StarsU16);
typedef StarsI32 (*StarsZipOrderDlgTextExtent)(void *, StarsU16,
    const StarsU8 *, StarsI16);
typedef void (*StarsZipOrderDlgTextOut)(void *, StarsU16, StarsI16, StarsI16,
    const StarsU8 *, StarsI16);
typedef void (*StarsZipOrderDlgRightText)(void *, StarsU16, StarsI16,
    StarsI16, const StarsU8 *, StarsI16);
typedef void (*StarsZipOrderDlgSetWindowText)(void *, StarsU16, const StarsU8 *);
typedef void (*StarsZipOrderDlgShowWindow)(void *, StarsU16, StarsI16);
typedef void (*StarsZipOrderDlgCheckRadio)(void *, StarsU16, StarsU16,
    StarsU16, StarsU16);
typedef void (*StarsZipOrderDlgEnableButtons)(void *, StarsU16, StarsI16);
typedef void (*StarsZipOrderDlgInvalidate)(void *, StarsU16);
typedef StarsI16 (*StarsZipOrderDlgRename)(void *, StarsI16, StarsU8 *,
    StarsU16);
typedef void (*StarsZipOrderDlgFormatDefault)(void *, StarsU8 *, StarsI16);
typedef void (*StarsZipOrderDlgCopyOrder)(void *, StarsI16);
typedef void (*StarsZipOrderDlgDeleteOrder)(void *, StarsI16);
typedef void (*StarsZipOrderDlgSetFocus)(void *, StarsU16);
typedef void (*StarsZipOrderDlgSticky)(void *, StarsU16, StarsPoint *, StarsI16);
typedef void (*StarsZipOrderDlgEndDialog)(void *, StarsU16, StarsI16);
typedef void (*StarsZipOrderDlgWinHelp)(void *, StarsU16, const StarsU8 *,
    StarsU16);
typedef void (*StarsZipOrderDlgAdvanceTutor)(void *);
typedef struct StarsZipOrderDlgState {
    StarsU16 valid[4];
    StarsU16 words[4][5];
    StarsU8 names[4][64];
    StarsI16 selected;
    StarsI16 arial8_height;
    StarsU16 font_arial8;
    StarsU32 button_face_color;
    StarsU16 button_face_brush;
    StarsPoint sticky_position;
    const StarsU8 *help_file;
    StarsU16 frame_window;
    StarsI16 tutor_active;
    StarsI16 dirty;
    const StarsU8 *resource_caption;
    const StarsU8 *resource_labels[5];
    StarsU32 resource_colors[5];
} StarsZipOrderDlgState;
typedef struct StarsZipOrderDlgOps {
    StarsZipOrderDlgBeginPaint begin_paint;
    StarsZipOrderDlgEndPaint end_paint;
    StarsZipOrderDlgGetClientRect get_client_rect;
    StarsZipOrderDlgFillRect fill_rect;
    StarsZipOrderDlgGetDialogItem get_dialog_item;
    StarsZipOrderDlgGetWindowRect get_window_rect;
    StarsZipOrderDlgScreenToClient screen_to_client;
    StarsZipOrderDlgExpandRect expand_rect;
    StarsZipOrderDlgDrawFrame draw_frame;
    StarsZipOrderDlgSelectObject select_object;
    StarsZipOrderDlgSetBackground set_background_color;
    StarsZipOrderDlgSetTextColor set_text_color;
    StarsZipOrderDlgGetString get_string;
    StarsZipOrderDlgCompressedString compressed_string;
    StarsZipOrderDlgTextExtent text_extent;
    StarsZipOrderDlgTextOut text_out;
    StarsZipOrderDlgRightText right_text;
    StarsZipOrderDlgSetWindowText set_window_text;
    StarsZipOrderDlgShowWindow show_window;
    StarsZipOrderDlgCheckRadio check_radio_button;
    StarsZipOrderDlgEnableButtons enable_buttons;
    StarsZipOrderDlgInvalidate invalidate;
    StarsZipOrderDlgRename rename_dialog;
    StarsZipOrderDlgFormatDefault format_default;
    StarsZipOrderDlgCopyOrder copy_order;
    StarsZipOrderDlgDeleteOrder delete_order;
    StarsZipOrderDlgSetFocus set_focus;
    StarsZipOrderDlgSticky sticky_position;
    StarsZipOrderDlgEndDialog end_dialog;
    StarsZipOrderDlgWinHelp win_help;
    StarsZipOrderDlgAdvanceTutor advance_tutor;
    void *context;
} StarsZipOrderDlgOps;
StarsI16 stars_zip_order_dlg(StarsZipOrderDlgState *, StarsU16, StarsU16,
    StarsU16, StarsU32, const StarsZipOrderDlgOps *);
typedef StarsI16 (*StarsTaskCompressed)(void *, StarsU16, StarsU8 *, StarsU16);
typedef StarsI16 (*StarsTaskLocalized)(void *, StarsU16, StarsU8 *, StarsU16);
typedef struct StarsTaskNameInput {
    StarsU16 cord;
    StarsU16 orders[9];
    StarsU16 next_orders[9];
    const StarsZipOrder *zip_orders;
    StarsTaskCompressed compressed;
    StarsTaskLocalized localized;
    const StarsU8 *dash_text;
    const StarsU8 *special_names[3];
    const StarsU8 *suffix_names[2];
    void *context;
} StarsTaskNameInput;
StarsI16 stars_get_task_name(const StarsTaskNameInput *, StarsU8 *, StarsU16,
    StarsI16 *);
typedef StarsI16 (*StarsDestinationCompressed)(void *, StarsU16, StarsU8 *,
    StarsU16);
typedef StarsI16 (*StarsDestinationLocation)(void *, StarsU16, StarsI16,
    StarsI16, StarsI16, StarsU8 *, StarsU16);
typedef void (*StarsDestinationColor)(void *, StarsU16, StarsU16);
typedef struct StarsDestinationNameInput {
    StarsU16 cord;
    StarsU16 orders[9];
    StarsU16 next_orders[9];
    StarsU16 hdc;
    StarsDestinationCompressed compressed;
    StarsDestinationLocation location;
    StarsDestinationColor set_color;
    const StarsU8 *dash_text;
    void *context;
} StarsDestinationNameInput;
StarsI16 stars_get_destination_name(const StarsDestinationNameInput *,
    StarsU8 *, StarsU16);
typedef struct StarsBattleRecord {
    StarsU16 marker;
    StarsU16 length;
} StarsBattleRecord;
typedef struct StarsBattleBlock {
    const StarsBattleRecord *records;
    StarsU16 record_count;
    StarsU16 ib_top;
    const struct StarsBattleBlock *next;
} StarsBattleBlock;
StarsI16 stars_count_battles(const StarsBattleBlock *);
typedef StarsI32 (*StarsMineSweepPower)(void *, StarsI16, StarsU16);
typedef struct StarsMineSweepFleetInput {
    StarsI16 player;
    StarsI16 mine_sweep_counts[16];
    StarsMineSweepPower power;
    void *context;
} StarsMineSweepFleetInput;
StarsI32 stars_mine_sweep_from_fleet(const StarsMineSweepFleetInput *);
typedef struct StarsMineSweepComponent {
    StarsU16 type;
    StarsU16 encoded_value;
} StarsMineSweepComponent;
typedef StarsI32 (*StarsMineSweepComponentPower)(void *, StarsU16, StarsU16,
    StarsU16);
typedef struct StarsMineSweepHullInput {
    StarsU16 hull_definition;
    const StarsMineSweepComponent *components;
    StarsU16 component_count;
    StarsMineSweepComponentPower component_power;
    void *context;
} StarsMineSweepHullInput;
StarsI32 stars_mine_sweep_from_hull(const StarsMineSweepHullInput *);
typedef struct StarsMineFleetPart {
    StarsU16 group;
    StarsU16 subtype;
} StarsMineFleetPart;
typedef struct StarsMineFleetDesign {
    const StarsMineFleetPart *parts;
    StarsU16 part_count;
} StarsMineFleetDesign;
typedef StarsI32 (*StarsMineFleetPartDp)(void *, StarsU16, StarsU16);
typedef struct StarsMineFleetInput {
    StarsI16 player;
    const StarsI16 *design_counts;
    const StarsMineFleetDesign *designs;
    StarsMineFleetPartDp part_dp;
    void *context;
} StarsMineFleetInput;
StarsI32 stars_mine_from_fleet(const StarsMineFleetInput *);
typedef struct StarsDiscoverMineralPlanet {
    StarsI16 player;
    StarsU16 id;
    StarsU8 concentration[3];
} StarsDiscoverMineralPlanet;
typedef StarsI16 (*StarsDiscoverRandom)(void *, StarsI16);
typedef void (*StarsDiscoverMessage)(void *, StarsI16, StarsU16, StarsU16,
    StarsI16);
typedef struct StarsDiscoverMineralsInput {
    StarsI16 mineral_size;
    StarsI16 turn;
    StarsDiscoverMineralPlanet *planets;
    StarsU16 planet_count;
    StarsDiscoverRandom random;
    StarsDiscoverMessage message;
    void *context;
} StarsDiscoverMineralsInput;
void stars_discover_new_minerals(StarsDiscoverMineralsInput *);
typedef struct StarsMdTargetToken {
    StarsU8 grobj;
    StarsU16 overlap_23;
} StarsMdTargetToken;
StarsI16 stars_is_target_of_md_target(const StarsMdTargetToken *, StarsI16);
typedef struct StarsMacArmadaDesign {
    StarsU16 ai_controlled;
    StarsU16 hull_type;
} StarsMacArmadaDesign;
typedef struct StarsPotentMacFleetInput {
    StarsI16 ship_counts[10];
    StarsMacArmadaDesign designs[10];
    StarsI16 potency_threshold;
} StarsPotentMacFleetInput;
StarsI16 stars_potent_mac_war_fleet(const StarsPotentMacFleetInput *, StarsI16 *);
typedef struct StarsAutoTrackOrder {
    StarsU16 flags;
    StarsI16 target_fleet;
    StarsI16 x;
    StarsI16 y;
} StarsAutoTrackOrder;
typedef struct StarsAutoTrackFleet {
    StarsI16 player;
    StarsU16 present;
    StarsU16 cord;
    StarsAutoTrackOrder *orders;
    StarsU16 order_count;
} StarsAutoTrackFleet;
typedef struct StarsNoAutoTrackInput {
    StarsI16 target_player;
    StarsI16 target_fleet;
    StarsI16 target_x;
    StarsI16 target_y;
    StarsAutoTrackFleet *fleets;
    StarsU16 fleet_count;
} StarsNoAutoTrackInput;
void stars_no_auto_track_fleet(StarsNoAutoTrackInput *);
typedef StarsI16 (*StarsNearestScanObject)(void *, StarsPoint, StarsU16,
    StarsPoint *, StarsU16 *);
typedef void (*StarsScanToLogical)(void *, StarsPoint *);
typedef struct StarsNearWaypointInput {
    StarsU16 selected_kind;
    StarsI16 logical_input;
    StarsPoint selected_point;
    StarsPoint query_point;
    StarsU16 selected_waypoint_count;
    const StarsPoint *waypoints;
    StarsNearestScanObject nearest;
    StarsScanToLogical scan_to_logical;
    void *context;
} StarsNearWaypointInput;
StarsI16 stars_near_a_waypoint(StarsNearWaypointInput *);
typedef struct StarsDesignValueInput {
    StarsU16 token_count;
    StarsU32 hull_dp;
    StarsU32 displacement_dp;
    StarsU16 has_displacement;
} StarsDesignValueInput;
StarsI32 stars_design_value_from_token(const StarsDesignValueInput *);
typedef struct StarsButtonDownInput {
    StarsU16 scan_flags;
    StarsU16 mine_mode;
} StarsButtonDownInput;
StarsI16 stars_is_button_down(const StarsButtonDownInput *, StarsI16);
typedef void (*StarsToolbarGetWindowText)(void *, StarsU16, StarsU8 *, StarsU16);
typedef void (*StarsToolbarSendMessage)(void *, StarsU16, StarsU16,
    StarsU16, StarsU32);
typedef void (*StarsToolbarInvalidateRect)(void *, StarsU16);
typedef void (*StarsToolbarSetFocus)(void *, StarsU16);
typedef void (*StarsToolbarSetWindowText)(void *, StarsU16, const StarsU8 *);
typedef struct StarsTerminateToolbarFocusInput {
    StarsU16 cancel;
    StarsU16 radar;
    StarsU16 toolbar;
    StarsU16 scanner;
    StarsU16 frame;
    StarsU16 grbits2;
    StarsU16 scan_flags;
    StarsI16 radar_view;
    StarsToolbarGetWindowText get_window_text;
    StarsToolbarSetWindowText set_window_text;
    StarsToolbarSendMessage send_message;
    StarsToolbarInvalidateRect invalidate_rect;
    StarsToolbarSetFocus set_focus;
    void *context;
} StarsTerminateToolbarFocusInput;
void stars_terminate_toolbar_focus(StarsTerminateToolbarFocusInput *);
typedef struct StarsBattleDataRecord {
    StarsU16 id;
    StarsU16 cb_data;
} StarsBattleDataRecord;
typedef struct StarsBattleDataBlock StarsBattleDataBlock;
struct StarsBattleDataBlock {
    StarsU16 ib_top;
    const StarsBattleDataRecord *records;
    StarsU16 record_count;
    const StarsBattleDataBlock *next;
};
typedef struct StarsBattleDataInput {
    const StarsBattleDataBlock *first;
} StarsBattleDataInput;
const StarsBattleDataRecord *stars_btl_data_get(const StarsBattleDataInput *,
    StarsI16);
typedef struct StarsMessageHitTestInput {
    StarsRect title_rect;
    StarsPoint point;
    StarsI16 current_message;
    StarsU16 message_count;
    StarsU16 message_flags;
    StarsU16 game_flags;
    const StarsU8 *sent;
    const StarsU8 *filtered;
} StarsMessageHitTestInput;
StarsI16 stars_message_hit_test(const StarsMessageHitTestInput *);
typedef void (*StarsLockSelectObject)(void *, StarsU16, StarsU16);
typedef void (*StarsLockPatBlt)(void *, StarsU16, StarsI16, StarsI16,
    StarsI16, StarsI16, StarsU32);
typedef struct StarsDrawLockLightInput {
    StarsU16 hdc;
    StarsRect rect;
    StarsI16 full_redraw;
    StarsU16 button_face;
    StarsU16 button_shadow;
    StarsU16 button_hilite;
    StarsFillRect fill_rect;
    StarsLockSelectObject select_object;
    StarsLockPatBlt pat_blt;
    void *context;
} StarsDrawLockLightInput;
void stars_draw_lock_light(StarsDrawLockLightInput *);
typedef void (*StarsScanOrientation)(void *, StarsI16, StarsI16,
    StarsPoint *, StarsPoint *);
typedef struct StarsScanFleetOrientationInput {
    StarsI16 fleet_player;
    StarsI16 local_player;
    StarsI16 fleet_cord;
    StarsU16 own_order_flags;
    StarsI16 own_order_x;
    StarsI16 own_order_y;
    StarsU16 enemy_position;
    StarsU16 enemy_flags;
    StarsScanOrientation orientation;
    void *context;
} StarsScanFleetOrientationInput;
void stars_get_scan_fleet_orientation(const StarsScanFleetOrientationInput *,
    StarsPoint *, StarsPoint *);
typedef struct StarsDxDyOrientationInput {
    StarsI16 scan_zoom;
} StarsDxDyOrientationInput;
void stars_get_dx_dy_orientation(StarsI16, StarsI16,
    const StarsDxDyOrientationInput *, StarsPoint *, StarsPoint *);
typedef StarsI16 (*StarsScanScrollToPoint)(void *, StarsI16);
typedef void (*StarsScanSetScrollRange)(void *, StarsU16, StarsU16,
    StarsU16, StarsU16, StarsU16);
typedef struct StarsSetScanScrollBarsInput {
    StarsI16 in_scroll_set;
    StarsI16 d_gal_inv;
    StarsI16 dy_sbar;
    StarsI16 client_right;
    StarsI16 client_bottom;
    StarsI16 d_scan_page;
    StarsI16 d_scan_inc;
    StarsScanScrollToPoint scan_to_point;
    StarsScanSetScrollRange set_scroll_range;
    void *context;
} StarsSetScanScrollBarsInput;
void stars_set_scan_scroll_bars(StarsU16, StarsSetScanScrollBarsInput *);
typedef StarsI16 (*StarsScanPointInRect)(void *, StarsRect, StarsPoint);
typedef void (*StarsCenterPointScan)(void *, StarsPoint, StarsI16);
typedef struct StarsEnsurePointOnScreenInput {
    StarsI16 dy_sbar;
    StarsI16 x_scan_top;
    StarsI16 y_scan_top;
    StarsI16 d_gal_inv;
    StarsI16 client_right;
    StarsI16 client_bottom;
    StarsScanScrollToPoint scan_to_point;
    StarsScanPointInRect point_in_rect;
    StarsCenterPointScan center_point;
    void *context;
} StarsEnsurePointOnScreenInput;
StarsI16 stars_ensure_point_on_screen(StarsPoint, StarsI16,
    const StarsEnsurePointOnScreenInput *);
/* CShipsScanVis consumes the fleet fields at their original offsets, while
 * the hull table lookup is supplied explicitly instead of through DGROUP. */
typedef struct StarsShipsScanFleet {
    StarsI16 player;
    StarsU16 cord;
    StarsU16 order_flags;
    StarsU16 overlap_flags;
    StarsU16 ship_counts[16];
    StarsU16 ship_designs[16];
} StarsShipsScanFleet;
typedef StarsU16 (*StarsShipsScanHullCategory)(void *, StarsI16, StarsU16);
typedef struct StarsShipsScanInput {
    StarsU16 scan_flags;
    StarsI16 local_player;
    StarsU16 own_ship_mask;
    StarsU16 enemy_category_mask;
    StarsShipsScanHullCategory hull_category;
    void *context;
} StarsShipsScanInput;
StarsU32 stars_c_ships_scan_vis(const StarsShipsScanFleet *,
    const StarsShipsScanInput *);
typedef void (*StarsDrawScanBitBlt)(void *, StarsU16, StarsI16, StarsI16,
    StarsI16, StarsI16, StarsU16, StarsI16, StarsI16, StarsU32);
typedef StarsU32 (*StarsDrawScanPlayerColor)(void *, StarsI16);
typedef struct StarsDrawScanFleetCountInput {
    StarsShipsScanFleet *fleets;
    StarsU16 fleet_count;
    StarsShipsScanInput scan;
    StarsU16 numbers_bitmap;
    StarsScanSelectObject select_object;
    StarsSetBackgroundColor set_color;
    StarsDrawScanBitBlt bit_blt;
    StarsDrawScanPlayerColor player_color;
    void *context;
} StarsDrawScanFleetCountInput;
void stars_draw_scan_fleet_count(StarsI16, StarsI16, StarsU16, StarsU16,
    StarsDrawScanFleetCountInput *);
typedef struct StarsDrawCbItemInput StarsDrawCbItemInput;
typedef void (*StarsDrawCbSendMessage)(void *, StarsU16, StarsU16, StarsU16,
    StarsU32);
typedef void (*StarsDrawCbProduction)(void *, StarsU16, StarsRect *,
    StarsU8 *, StarsI16, StarsI16, StarsI16);
typedef void (*StarsDrawCbFocus)(void *, const StarsDrawCbItemInput *,
    StarsI16);
struct StarsDrawCbItemInput {
    StarsU16 hwnd_item;
    StarsU16 item_id;
    StarsU16 item_state;
    StarsU16 hdc;
    StarsRect item_rect;
    StarsU16 fleet_comp_list;
    StarsU16 planet_prod_list;
    StarsU8 *work_text;
    StarsDrawCbSendMessage send_message;
    StarsDrawCbProduction draw_production;
    StarsDrawCbFocus handle_focus;
    void *context;
};
void stars_draw_cb_entire_item(StarsDrawCbItemInput *, StarsI16);
typedef struct StarsPanicDialogInput {
    StarsU16 hwnd;
    StarsU16 message;
    StarsU16 wparam;
    StarsU32 lparam;
    StarsU16 button_face;
    StarsU32 color_button_face;
    StarsU16 tutor_help_id;
    const StarsU8 *help_file;
    StarsGetClientRect get_client_rect;
    StarsFillRect fill_rect;
    StarsSetBackgroundColor set_background_color;
    StarsEndDialog end_dialog;
    StarsWinHelp win_help;
    void *context;
} StarsPanicDialogInput;
StarsI16 stars_panic_dialog(const StarsPanicDialogInput *);
typedef void (*StarsTutorDialogSticky)(void *, StarsU16, StarsPoint *, StarsI16);
typedef StarsU16 (*StarsTutorDialogGetSubMenu)(void *, StarsU16, StarsU16);
typedef void (*StarsTutorDialogCheckMenu)(void *, StarsU16, StarsU16, StarsU16);
typedef void (*StarsTutorDialogDrawText)(void *, StarsU16);
typedef void (*StarsTutorDialogPostMessage)(void *, StarsU16, StarsU16,
    StarsU16, StarsU32);
typedef void (*StarsTutorDialogSetWindowPos)(void *, StarsU16, StarsU16,
    StarsI16, StarsI16, StarsI16, StarsI16, StarsU16);
typedef StarsU32 (*StarsTutorDialogMakeProc)(void *, StarsU16);
typedef StarsI16 (*StarsTutorDialogBox)(void *, StarsU16, StarsU16,
    StarsU16, StarsU32);
typedef void (*StarsTutorDialogFreeProc)(void *, StarsU32);
typedef void (*StarsTutorDialogShow)(void *, StarsI16);
typedef void (*StarsTutorDialogEndTutor)(void *, StarsI16);
typedef const StarsU8 *(*StarsTutorDialogFormat)(void *, StarsU16);
typedef StarsI16 (*StarsTutorDialogAlert)(void *, const StarsU8 *, StarsI16);
typedef struct StarsTutorDialogOps {
    StarsTutorDialogSticky sticky_position;
    StarsTutorDialogGetSubMenu get_sub_menu;
    StarsTutorDialogCheckMenu check_menu;
    StarsTutorDialogDrawText draw_text;
    StarsGetClientRect get_client_rect;
    StarsFillRect fill_rect;
    StarsEndDialog end_dialog;
    StarsTutorDialogPostMessage post_message;
    StarsTutorDialogSetWindowPos set_window_pos;
    StarsTutorDialogMakeProc make_proc;
    StarsTutorDialogBox dialog_box;
    StarsTutorDialogFreeProc free_proc;
    StarsTutorDialogShow show_tutor;
    StarsTutorDialogEndTutor end_tutor;
    StarsTutorDialogFormat format_ids;
    StarsTutorDialogAlert alert;
    StarsWinHelp win_help;
} StarsTutorDialogOps;
typedef struct StarsTutorDialogInput {
    StarsU16 hwnd;
    StarsU16 message;
    StarsU16 wparam;
    StarsU32 lparam;
    StarsU16 frame_window;
    StarsU16 instance;
    StarsU16 *tutor_window;
    StarsU16 *tutor_flags;
    StarsU16 tutor_help_id;
    StarsPoint *sticky_point;
    const StarsU8 *help_file;
    StarsU16 button_face_brush;
    const StarsTutorDialogOps *ops;
    void *context;
} StarsTutorDialogInput;
StarsI16 stars_tutor_dlg(const StarsTutorDialogInput *);
StarsI16 stars_attack_player(StarsU16 plan_behavior, StarsI16 relation,
    StarsI16 target_player);
typedef struct StarsEnemyShipDesign {
    StarsU16 hidden;
} StarsEnemyShipDesign;
typedef struct StarsEnemyDesignPlayer {
    const StarsEnemyShipDesign *designs;
    StarsU16 design_count;
} StarsEnemyDesignPlayer;
typedef struct StarsNthEnemyDesignInput {
    const StarsEnemyDesignPlayer *players;
    StarsU16 player_count;
    StarsI16 local_player;
    StarsI16 starbase_mode;
} StarsNthEnemyDesignInput;
const StarsEnemyShipDesign *stars_nth_valid_enemy_design(
    const StarsNthEnemyDesignInput *, StarsI16);
typedef struct StarsProductionItem {
    StarsU32 packed;
} StarsProductionItem;
typedef void (*StarsFreeProductionItems)(void *, StarsProductionItem *);
typedef struct StarsTossBuildItemsInput {
    StarsProductionItem *items;
    StarsU16 item_count;
    StarsFreeProductionItems free_items;
    void *context;
} StarsTossBuildItemsInput;
void stars_toss_non_auto_build_items(StarsTossBuildItemsInput *);
typedef struct StarsBattleTargetToken {
    StarsI16 player;
    StarsU16 grobj;
    StarsU16 primary_target_type;
    StarsU16 active_flags;
} StarsBattleTargetToken;
StarsI16 stars_does_primary_target_type_exist(
    StarsI16 local_player, StarsU16 target_type, StarsU16 attack_players,
    const StarsBattleTargetToken *, StarsU16);
StarsI16 stars_should_part_be_hidden(StarsI16 local_player, StarsU16 group,
    StarsU16 subtype, StarsU16 trader_flags);
typedef struct StarsBattleKillToken {
    StarsI16 player;
} StarsBattleKillToken;
typedef struct StarsBattleKillEntry {
    StarsU16 token_index;
    StarsU16 kills;
} StarsBattleKillEntry;
typedef struct StarsBattleKillGroup {
    const StarsBattleKillEntry *entries;
    StarsU16 entry_count;
} StarsBattleKillGroup;
StarsU32 stars_battle_kills(const StarsBattleKillToken *, StarsU16,
    const StarsBattleKillGroup *, StarsU16, StarsI16, StarsI16);
typedef struct StarsTerraComponent {
    StarsU16 type;
    StarsU16 encoded_amount;
} StarsTerraComponent;
typedef struct StarsTerraDesign {
    const StarsTerraComponent *components;
    StarsU16 component_count;
} StarsTerraDesign;
typedef struct StarsTerraFleetInput {
    StarsI16 component_counts[16];
    StarsTerraDesign designs[16];
} StarsTerraFleetInput;
StarsI32 stars_terra_from_fleet(const StarsTerraFleetInput *);
typedef struct StarsColonistDropRecord {
    StarsI16 fleet;
    StarsI16 player;
    StarsI16 planet;
    StarsI32 colonists;
    StarsU16 active;
} StarsColonistDropRecord;
typedef struct StarsQueueColonistDropInput {
    StarsI16 fleet;
    StarsI16 player;
    StarsI16 planet;
    StarsI32 colonists;
    StarsColonistDropRecord *records;
    StarsI16 record_count;
    StarsI16 record_capacity;
} StarsQueueColonistDropInput;
StarsI32 stars_queue_colonist_drop(StarsQueueColonistDropInput *);
typedef struct StarsQueuedProductionPlanet {
    StarsI16 player;
    StarsU16 build_enabled;
    StarsU16 has_production;
    StarsU16 space_dock;
    const StarsU32 *items;
    StarsU16 item_count;
} StarsQueuedProductionPlanet;
typedef struct StarsQueuedProductionInput {
    StarsI16 design;
    StarsI16 space_docks_only;
    StarsI16 current_player;
    const StarsQueuedProductionPlanet *planets;
    StarsU16 planet_count;
} StarsQueuedProductionInput;
StarsI16 stars_count_queued_production(const StarsQueuedProductionInput *,
    StarsI16 *);
typedef void (*StarsFillPlanetProdGetString)(void *, StarsU16, StarsU8 *,
    StarsU16);
typedef void (*StarsFillPlanetProdName)(void *, StarsU32, StarsU8 *, StarsU16);
typedef void (*StarsFillPlanetProdEstimate)(void *, const void *,
    const StarsU32 *, StarsU16, StarsI16 *, StarsI16 *);
typedef void (*StarsFillPlanetProdFormat)(void *, StarsU8, StarsU16,
    const StarsU8 *, StarsU8 *, StarsU16);
typedef void (*StarsFillPlanetProdSend)(void *, StarsU16, StarsU16,
    const StarsU8 *);
typedef struct StarsFillPlanetProdQueue {
    const StarsU32 *items;
    StarsU16 item_count;
} StarsFillPlanetProdQueue;
typedef struct StarsFillPlanetProdInput {
    StarsU16 hwnd;
    StarsU16 default_hwnd;
    StarsU16 production_dialog_open;
    const void *planet;
    const StarsFillPlanetProdQueue *selected_queue;
    const StarsFillPlanetProdQueue *queue;
    StarsU8 *work;
    StarsU16 work_capacity;
    StarsFillPlanetProdGetString get_string;
    StarsFillPlanetProdName name_item;
    StarsFillPlanetProdEstimate estimate_schedule;
    StarsFillPlanetProdFormat format_line;
    StarsFillPlanetProdSend send_message;
    void *context;
} StarsFillPlanetProdInput;
void stars_fill_planet_production_list(const StarsFillPlanetProdInput *);
typedef void (*StarsPacketResourceQuery)(void *, StarsU32[3]);
typedef StarsI16 (*StarsPacketQueueItem)(void *, StarsI16, StarsI16,
    StarsI16, StarsI16);
typedef struct StarsAddPacketQueueInput {
    StarsU32 available[3];
    StarsU32 cost[3];
    StarsPacketQueueItem add_item;
    void *context;
} StarsAddPacketQueueInput;
StarsI16 stars_add_packet_to_queue(StarsAddPacketQueueInput *);
typedef void (*StarsSetWindowText)(void *, StarsU16, const StarsU8 *);
typedef StarsU16 (*StarsAboutGetDC)(void *, StarsU16);
typedef void (*StarsAboutSelectObject)(void *, StarsU16, StarsU32);
typedef void (*StarsAboutSetBkMode)(void *, StarsU16, StarsU16);
typedef void (*StarsAboutSetTextColor)(void *, StarsU16, StarsU32);
typedef void (*StarsAboutIntersectClip)(void *, StarsU16, StarsI16, StarsI16,
    StarsU16, StarsU16);
typedef void (*StarsAboutDrawCentered)(void *, StarsU16, const StarsRect *,
    const StarsU8 *, StarsI16);
typedef void (*StarsAboutOffsetRect)(void *, StarsRect *, StarsI16, StarsI16);
typedef void (*StarsAboutSelectClip)(void *, StarsU16, StarsU16);
typedef void (*StarsAboutReleaseDC)(void *, StarsU16, StarsU16);
typedef StarsU16 (*StarsAboutSetTimer)(void *, StarsU16, StarsU16, StarsU32);
typedef void (*StarsAboutKillTimer)(void *, StarsU16, StarsU16);
typedef void (*StarsAboutShowOrder)(void *, StarsU16, StarsU16, StarsU16);
typedef const StarsU8 *(*StarsAboutTutorialString)(void *, StarsI16);
typedef const StarsU8 *(*StarsAboutVersion)(void *);
typedef struct StarsAboutOps {
    StarsGetDialogItem get_dialog_item;
    StarsAboutVersion version;
    StarsSetWindowText set_window_text;
    StarsAboutSetTimer set_timer;
    StarsAboutKillTimer kill_timer;
    StarsAboutShowOrder show_order;
    StarsEndDialog end_dialog;
    StarsGetClientRect get_client_rect;
    StarsAboutGetDC get_dc;
    StarsAboutSelectObject select_object;
    StarsAboutSetBkMode set_bk_mode;
    StarsSetBackgroundColor set_bk_color;
    StarsAboutSetTextColor set_text_color;
    StarsAboutIntersectClip intersect_clip;
    StarsAboutTutorialString tutorial_string;
    StarsAboutDrawCentered draw_centered;
    StarsAboutOffsetRect offset_rect;
    StarsFillRect fill_rect;
    StarsAboutSelectClip select_clip;
    StarsAboutReleaseDC release_dc;
} StarsAboutOps;
StarsI16 stars_about(StarsU8 *, StarsU16, StarsU16, StarsU16, StarsU32,
    const StarsAboutOps *, void *);
/* plan_order contains the 21 original rgidPlan selector bytes into the
 * serial/environment frame: serial bytes 0..3, environment bytes 4..14,
 * generated bytes 15..20. The output is 28 characters plus a NUL. */
void stars_format_serial_and_env(StarsSeeds *, StarsU32, const StarsU8 *,
    const StarsU8 *, StarsU8 *);
/* Safe host-facing wrapper: output_capacity must include the NUL byte and
 * every plan selector must refer to one of the 21 initialized frame bytes. */
StarsI16 stars_format_serial_and_env_checked(StarsSeeds *, StarsU32,
    const StarsU8 *, const StarsU8 *, StarsU8 *, StarsU16);
StarsI16 stars_serial_and_env_from_string(StarsSeeds *, const StarsU8 *,
    StarsU32 *, StarsU8 *, const StarsU8 *, StarsSerialValidator, void *);
StarsI16 stars_get_file_status(StarsU8 *data, StarsU16 type, StarsI16 player,
    StarsPathAccess access, void *context);
StarsI16 stars_new_turn_available(StarsU8 *, StarsI16, StarsOpenGameFile,
    StarsCloseGameFile, void *);
StarsI16 stars_check_file(StarsU8 *, StarsU16, StarsI16, StarsU16,
    StarsOpenGameFile, StarsReadGameRecord, StarsCloseGameFile, void *);
/* FLoadGame is a coordinator around the record decoders below.  The original
 * routine keeps the current record in DGROUP and advances it with ReadRt;
 * this interface makes that cursor explicit so the semantic source can be
 * exercised without reproducing the original global address map. */
typedef struct StarsLoadGameRecord {
    StarsU16 type;
    StarsU16 length;
    const StarsU8 *data;
} StarsLoadGameRecord;
typedef struct StarsLoadGamePlan {
    StarsI16 x;
    StarsI16 y;
    StarsI16 id;
} StarsLoadGamePlan;
/* A turn planet is not just another record.  FLoadGame extracts its 11-bit
 * object id, looks for that id in the already sorted planet array, and then
 * asks FReadPlanet to decode into either the existing slot or an insertion
 * slot.  Keeping those facts in a typed view prevents the reconstruction from
 * hiding the merge behind an opaque byte callback. */
typedef struct StarsLoadGamePlanetInput {
    StarsI16 player;
    StarsU16 id;
    StarsU16 slot;
    StarsI16 preinitialized;
    StarsI16 history;
    const StarsLoadGameRecord *record;
} StarsLoadGamePlanetInput;
/* Type-6 records carry the destination player in payload byte zero.  It is
 * independent of the player selected by the filename, so the loader must not
 * silently pass only state->player to a decoder. */
typedef struct StarsLoadGamePlayerInput {
    StarsU16 player_index;
    StarsI16 selected_player;
    const StarsLoadGameRecord *record;
} StarsLoadGamePlayerInput;
typedef struct StarsLoadGameState {
    StarsU16 player_count;
    StarsU16 plan_count;
    StarsU16 map_size;
    StarsI16 player;
    StarsU16 mode;
    StarsU16 mode_flags;
    StarsU16 turn_count;
    StarsU16 current_turn;
    StarsU16 current_flags;
    StarsU16 current_planet_count;
    StarsU16 current_fleet_count;
    StarsU16 current_thing_count;
    StarsU16 current_design_count;
    StarsI16 open;
    /* Internal guard: one corrupt stream produces one user-facing alert even
     * when both the record parser and the outer cleanup path observe it. */
    StarsI16 failure_alerted;
} StarsLoadGameState;
typedef StarsI16 (*StarsLoadGameOpen)(void *, StarsU16, StarsI16, StarsU16);
typedef StarsI16 (*StarsLoadGameNext)(void *, StarsLoadGameRecord *);
typedef void (*StarsLoadGameClose)(void *);
typedef StarsI16 (*StarsLoadGameDecodeHeader)(void *, const StarsU8 *,
    StarsU16, StarsLoadGameState *);
typedef void (*StarsLoadGamePlanRecord)(void *, StarsU16,
    const StarsLoadGamePlan *);
typedef void (*StarsLoadGameReset)(void *, StarsU16, StarsI16);
typedef StarsI16 (*StarsLoadGamePlayerHeader)(void *, const StarsU8 *,
    StarsU16, StarsLoadGameState *);
typedef StarsI16 (*StarsLoadGameRecordHandler)(void *, StarsI16,
    const StarsLoadGameRecord *);
typedef StarsI16 (*StarsLoadGameLocatePlanet)(void *, StarsU16, StarsU16 *,
    StarsI16 *);
typedef StarsI16 (*StarsLoadGamePlanetTurn)(void *,
    const StarsLoadGamePlanetInput *);
typedef StarsI16 (*StarsLoadGamePlayerTurn)(void *,
    const StarsLoadGamePlayerInput *);
typedef void (*StarsLoadGamePrepareTurn)(void *, StarsU16);
typedef StarsI16 (*StarsLoadGamePassword)(void *, StarsI16);
typedef void (*StarsLoadGameTurn)(void *, StarsU16, StarsU16, StarsU16);
typedef StarsI16 (*StarsLoadGamePostLoad)(void *, const StarsU8 *,
    const StarsU8 *, const StarsLoadGameState *);
typedef void (*StarsLoadGameCleanup)(void *, StarsI16);
typedef void (*StarsLoadGameAlert)(void *, StarsI16);
typedef void (*StarsLoadGamePostStage)(void *, const StarsLoadGameState *);
typedef StarsI16 (*StarsLoadGameLogStage)(void *, const StarsU8 *,
    const StarsU8 *);
typedef void (*StarsLoadGameMruStage)(void *, const StarsU8 *,
    const StarsU8 *);
typedef struct StarsLoadGameOps {
    StarsLoadGameOpen open_file;
    StarsLoadGameNext next_record;
    StarsLoadGameClose close_file;
    StarsLoadGameDecodeHeader decode_game_header;
    StarsLoadGamePlanRecord store_plan;
    StarsLoadGameReset reset_runtime;
    StarsLoadGamePlayerHeader decode_player_header;
    StarsLoadGameRecordHandler player_record;
    StarsLoadGameRecordHandler planet;
    /* planet_snapshot handles the history-form player-file planets. */
    StarsLoadGameRecordHandler planet_snapshot;
    /* locate_planet and planet_turn expose the sorted-array merge performed
     * for each current-turn planet.  The older planet callback remains a
     * compatibility fallback for small adapters. */
    StarsLoadGameLocatePlanet locate_planet;
    StarsLoadGamePlanetTurn planet_turn;
    StarsLoadGamePlayerTurn player_turn;
    StarsLoadGamePrepareTurn prepare_turn;
    StarsLoadGameRecordHandler production;
    StarsLoadGameRecordHandler ship_design;
    StarsLoadGameRecordHandler fleet;
    StarsLoadGameRecordHandler score;
    StarsLoadGameRecordHandler salt;
    StarsLoadGameRecordHandler history;
    StarsLoadGameRecordHandler thing;
    StarsLoadGameRecordHandler battle;
    StarsLoadGameRecordHandler battle_plan;
    StarsLoadGameRecordHandler messages;
    StarsLoadGamePassword check_password;
    StarsLoadGameTurn finish_turn;
    /* post_load covers log replay, derived counters, notifications and MRU;
     * cleanup_failure covers DestroyCurGame/stream cleanup at the boundary. */
    StarsLoadGamePostLoad post_load;
    StarsLoadGameCleanup cleanup_failure;
    StarsLoadGameAlert alert;
    /* These optional stages correspond to the observable work after the
     * final turn: notifications, log replay, derived player counts, and MRU
     * persistence.  They are separate so a semantic caller can test each
     * obligation without receiving a monolithic byte-oriented callback. */
    StarsLoadGamePostStage notify_thing_warps;
    StarsLoadGamePostStage notify_production;
    StarsLoadGamePostStage notify_battles;
    StarsLoadGameLogStage load_log;
    StarsLoadGameLogStage run_log;
    StarsLoadGamePostStage rebuild_counts;
    StarsLoadGameMruStage update_mru;
} StarsLoadGameOps;
StarsI16 stars_load_game(const StarsU8 *, const StarsU8 *,
    StarsLoadGameState *, const StarsLoadGameOps *, void *);
/* FLoadLogFile reads the companion log's 16-byte turn header, copies the
 * length-prefixed memory-log image, and leaves type-0x28 message records to a
 * typed consumer.  The source records are already selected by the platform
 * adapter; this keeps resource handles out of the C record semantics. */
typedef struct StarsLoadLogFileState {
    StarsU16 memory_current;
    StarsI16 memory_previous;
    StarsU16 version;
    StarsU16 file_turn;
    StarsU16 file_flags;
    StarsU16 log_length;
    StarsU16 messages;
} StarsLoadLogFileState;
typedef StarsI16 (*StarsLoadLogHeader)(void *, StarsU16, StarsU16);
typedef StarsI16 (*StarsLoadLogSerial)(void *, const StarsLoadGameRecord *);
typedef StarsI16 (*StarsLoadLogMessage)(void *, const StarsLoadGameRecord *);
typedef void (*StarsLoadLogFileError)(void *, StarsI16);
typedef void (*StarsLoadLogDirty)(void *);
typedef struct StarsLoadLogFileInput {
    const StarsLoadGameRecord *records;
    StarsU16 record_count;
    StarsU32 game_id;
    StarsU16 game_turn;
    StarsU16 game_turn_flags;
    StarsU8 *log;
    StarsU16 log_capacity;
    StarsLoadLogHeader accept_header;
    StarsLoadLogSerial serial;
    StarsLoadLogMessage message;
    StarsLoadLogFileError file_error;
    StarsLoadLogDirty dirty_game;
    void *context;
} StarsLoadLogFileInput;
StarsI16 stars_load_log_file(const StarsLoadLogFileInput *,
    StarsLoadLogFileState *);
/* Runtime-log record kinds.  These names are deliberately kept beside the
 * decoder: the original source used numeric record types, but each value is
 * an operation on a different part of the current game. */
#define STARS_LOG_RT_NONE              0x00U
#define STARS_LOG_RT_TRANSFER_BYTE     0x01U
#define STARS_LOG_RT_TRANSFER_WORD     0x02U
#define STARS_LOG_RT_DELETE_ORDER      0x03U
#define STARS_LOG_RT_INSERT_ORDER      0x04U
#define STARS_LOG_RT_REPLACE_ORDER     0x05U
#define STARS_LOG_RT_FLEET_FLAG        0x0aU
#define STARS_LOG_RT_ORDER_FLAG        0x0bU
#define STARS_LOG_RT_TRANSFER_BALANCE  0x17U
#define STARS_LOG_RT_SPLIT_FLEET       0x18U
#define STARS_LOG_RT_TRANSFER_LONG     0x19U
#define STARS_LOG_RT_SHIP_DESIGN       0x1bU
#define STARS_LOG_RT_PRODUCTION        0x1dU
#define STARS_LOG_RT_BATTLE_PLAN       0x1eU
#define STARS_LOG_RT_RESEARCH          0x22U
#define STARS_LOG_RT_PLANET_SCAN       0x23U
#define STARS_LOG_RT_PLAYER_SALT       0x24U
#define STARS_LOG_RT_MERGE_FLEET       0x25U
#define STARS_LOG_RT_RELATIONS         0x26U
#define STARS_LOG_RT_FLEET_PLAN        0x2aU
#define STARS_LOG_RT_THING             0x2bU
#define STARS_LOG_RT_FLEET_NAME        0x2cU
#define STARS_LOG_RT_PLAYER_SETTINGS   0x2eU
#define STARS_LOG_ORDER_DELETE         0U
#define STARS_LOG_ORDER_INSERT         1U
#define STARS_LOG_ORDER_REPLACE        2U

typedef struct StarsRunLogTransfer {
    StarsU16 source_id;
    StarsU16 target_id;
    StarsU8 source_kind;
    StarsU8 target_kind;
    StarsI32 delta[5];
} StarsRunLogTransfer;
typedef struct StarsRunLogOrder {
    StarsU16 fleet_id;
    StarsU16 index;
    StarsU16 operation;
    StarsI16 delete_following;
    StarsU8 data[18];
    StarsU16 data_size;
} StarsRunLogOrder;
typedef struct StarsRunLogFleetFlag {
    StarsU16 fleet_id;
    StarsU16 value;
} StarsRunLogFleetFlag;
typedef struct StarsRunLogOrderFlag {
    StarsU16 fleet_id;
    StarsU16 index;
    StarsU16 value;
} StarsRunLogOrderFlag;
typedef struct StarsRunLogTransferBalance {
    StarsU16 first_fleet;
    StarsU16 second_fleet;
    StarsI16 delta[16];
    StarsU16 mask;
} StarsRunLogTransferBalance;
typedef struct StarsRunLogFleetGroup {
    StarsU16 base_fleet;
    const StarsU16 *fleets;
    StarsU16 fleet_count;
} StarsRunLogFleetGroup;
typedef struct StarsRunLogDesignChange {
    StarsU16 player;
    StarsU16 slot;
    StarsU16 action;
    const StarsU8 *design_payload;
    StarsU16 design_payload_size;
} StarsRunLogDesignChange;
typedef struct StarsRunLogProduction {
    StarsU16 planet_id;
    const StarsU8 *entries;
    StarsU16 entry_count;
} StarsRunLogProduction;
typedef struct StarsRunLogBattlePlan {
    StarsU16 player;
    StarsU16 plan_index;
    StarsU16 deleted;
    const StarsU8 *payload;
    StarsU16 payload_size;
} StarsRunLogBattlePlan;
typedef struct StarsRunLogResearch {
    StarsU8 research_percent;
    StarsU8 technology;
} StarsRunLogResearch;
typedef struct StarsRunLogPlanetScan {
    StarsU16 planet_id;
    StarsU16 remote_terraform;
    StarsU16 scanner_range;
    StarsU16 scanner_mines;
    StarsU16 scanner_radius;
} StarsRunLogPlanetScan;
typedef struct StarsRunLogSalt {
    StarsU32 value;
} StarsRunLogSalt;
typedef struct StarsRunLogRelations {
    const StarsU8 *data;
    StarsU16 player_count;
} StarsRunLogRelations;
typedef struct StarsRunLogFleetPlan {
    StarsU16 fleet_id;
    StarsU16 plan_index;
} StarsRunLogFleetPlan;
typedef struct StarsRunLogThing {
    StarsU16 thing_id;
    StarsU8 owner;
} StarsRunLogThing;
typedef struct StarsRunLogFleetName {
    StarsU16 fleet_id;
    StarsU8 encoding_size;
    const StarsU8 *encoding;
} StarsRunLogFleetName;
typedef struct StarsRunLogPlayerBytes {
    const StarsU8 *data;
    StarsU16 size;
} StarsRunLogPlayerBytes;
typedef StarsI16 (*StarsRunLogVoid)(void *, const void *);
typedef StarsI16 (*StarsRunLogOrderOp)(void *, const StarsRunLogOrder *);
typedef StarsI16 (*StarsRunLogFleetFlagOp)(void *,
    const StarsRunLogFleetFlag *);
typedef StarsI16 (*StarsRunLogOrderFlagOp)(void *,
    const StarsRunLogOrderFlag *);
typedef StarsI16 (*StarsRunLogTransferBalanceOp)(void *,
    const StarsRunLogTransferBalance *);
typedef StarsI16 (*StarsRunLogFleetGroupOp)(void *,
    const StarsRunLogFleetGroup *);
typedef StarsI16 (*StarsRunLogDesignOp)(void *, const StarsRunLogDesignChange *);
typedef StarsI16 (*StarsRunLogProductionOp)(void *, const StarsRunLogProduction *);
typedef StarsI16 (*StarsRunLogBattlePlanOp)(void *, const StarsRunLogBattlePlan *);
typedef StarsI16 (*StarsRunLogResearchOp)(void *, const StarsRunLogResearch *);
typedef StarsI16 (*StarsRunLogPlanetScanOp)(void *, const StarsRunLogPlanetScan *);
typedef StarsI16 (*StarsRunLogSaltOp)(void *, const StarsRunLogSalt *);
typedef StarsI16 (*StarsRunLogRelationsOp)(void *, const StarsRunLogRelations *);
typedef StarsI16 (*StarsRunLogFleetPlanOp)(void *, const StarsRunLogFleetPlan *);
typedef StarsI16 (*StarsRunLogThingOp)(void *, const StarsRunLogThing *);
typedef StarsI16 (*StarsRunLogNameOp)(void *, const StarsRunLogFleetName *);
typedef StarsI16 (*StarsRunLogBytesOp)(void *, const StarsRunLogPlayerBytes *);
typedef struct StarsRunLogRecordOps {
    StarsRunLogVoid transfer;
    StarsRunLogOrderOp order;
    StarsRunLogFleetFlagOp fleet_flag;
    StarsRunLogOrderFlagOp order_flag;
    StarsRunLogTransferBalanceOp transfer_balance;
    StarsRunLogFleetGroupOp split_fleet;
    StarsRunLogFleetGroupOp merge_fleet;
    StarsRunLogDesignOp design;
    StarsRunLogProductionOp production;
    StarsRunLogBattlePlanOp battle_plan;
    StarsRunLogResearchOp research;
    StarsRunLogPlanetScanOp planet_scan;
    StarsRunLogSaltOp salt;
    StarsRunLogRelationsOp relations;
    StarsRunLogFleetPlanOp fleet_plan;
    StarsRunLogThingOp thing;
    StarsRunLogNameOp fleet_name;
    StarsRunLogBytesOp player_settings;
} StarsRunLogRecordOps;
typedef struct StarsRunLogRecordInput {
    StarsU16 record_type;
    StarsU16 payload_size;
    const StarsU8 *payload;
    StarsU16 player;
    StarsU16 player_count;
    StarsU16 game_flags;
} StarsRunLogRecordInput;
/* Decode one memory-log record and present its state change through typed
 * callbacks.  The callback layer owns DGROUP lookups and heap mutation; this
 * routine owns record validation, little-endian decoding, signed deltas, and
 * the meaning of every record kind. */
StarsI16 stars_run_log_record(const StarsRunLogRecordInput *,
    const StarsRunLogRecordOps *, void *);
/* FWriteDataFile's fixed save order.  Collection predicates are callbacks
 * because the original arrays are far-pointer DGROUP objects; the coordinator
 * still owns which players, design banks, fleets, and turn marker are visited
 * and when append falls back to creating a fresh file. */
#define STARS_WRITE_DATA_GAME_FILE       2U
#define STARS_WRITE_DATA_PLAYER_FILE     3U
#define STARS_WRITE_DATA_ALERT_GENERIC   0x10U
#define STARS_WRITE_DATA_ALERT_GAME      0x13U
#define STARS_WRITE_DATA_ALERT_PLAYER    0x215U
#define STARS_WRITE_DATA_ALERT_APPEND    0x216U
typedef struct StarsWriteDataFileInput {
    StarsI16 player;
    StarsI16 append;
    StarsU16 player_count;
    StarsU16 fleet_count;
    StarsU16 turn;
    StarsU32 salt;
    const StarsU8 *file_base;
    StarsU16 session_flags;
} StarsWriteDataFileInput;
typedef struct StarsWriteDataFileState {
    StarsI16 player;
    StarsI16 append;
    StarsU16 file_type;
    StarsU16 player_start;
    StarsU16 player_end;
    StarsU16 design_bank;
    StarsU16 design_slot;
    StarsU16 fleet_index;
    StarsI16 open;
    StarsI16 used_append;
} StarsWriteDataFileState;
typedef void (*StarsWriteDataView)(void *, StarsI16);
typedef void (*StarsWriteDataMark)(void *, StarsI16);
typedef StarsI16 (*StarsWriteDataCreate)(void *, StarsU16, StarsI16);
typedef StarsI16 (*StarsWriteDataAppend)(void *, StarsI16);
typedef StarsI16 (*StarsWriteDataPlayerPredicate)(void *, StarsU16);
typedef void (*StarsWriteDataPlayer)(void *, StarsU16);
typedef StarsI16 (*StarsWriteDataDesignPredicate)(void *, StarsU16,
    StarsU16, StarsU16);
typedef void (*StarsWriteDataDesign)(void *, StarsU16, StarsU16, StarsU16);
typedef StarsI16 (*StarsWriteDataFleetPredicate)(void *, StarsU16);
typedef void (*StarsWriteDataFleet)(void *, StarsU16);
typedef void (*StarsWriteDataStage)(void *, StarsI16);
typedef void (*StarsWriteDataSalt)(void *, StarsU32);
typedef void (*StarsWriteDataTurn)(void *, StarsU16);
typedef void (*StarsWriteDataClose)(void *);
typedef void (*StarsWriteDataAlert)(void *, StarsU16);
typedef struct StarsWriteDataFileOps {
    StarsWriteDataView set_visible_plan_fleet;
    StarsWriteDataView prepare_view;
    StarsWriteDataMark mark_messages;
    StarsWriteDataMark mark_planets_lost;
    StarsWriteDataCreate create_file;
    StarsWriteDataAppend append_file;
    StarsWriteDataStage write_battles;
    StarsWriteDataPlayerPredicate write_player_predicate;
    StarsWriteDataPlayer write_player;
    StarsWriteDataSalt write_salt;
    StarsWriteDataStage write_messages;
    StarsWriteDataStage write_planets;
    StarsWriteDataDesignPredicate write_design_predicate;
    StarsWriteDataDesign write_design;
    StarsWriteDataFleetPredicate write_fleet_predicate;
    StarsWriteDataFleet write_fleet;
    StarsWriteDataStage write_scores;
    StarsWriteDataStage write_things;
    StarsWriteDataStage write_battle_plans;
    StarsWriteDataTurn write_turn;
    StarsWriteDataClose close_file;
    StarsWriteDataAlert alert;
} StarsWriteDataFileOps;
StarsI16 stars_write_data_file(const StarsWriteDataFileInput *,
    StarsWriteDataFileState *, const StarsWriteDataFileOps *, void *);
/* Battle blocks are stored as a 14-byte prefix, zero or more 29-byte
 * participant records, and an optional tail.  WriteBattles preserves that
 * image but emits it in the old record-size limits. */
#define STARS_BATTLE_PREFIX_BYTES       14U
#define STARS_BATTLE_EVENT_BYTES        29U
#define STARS_BATTLE_RECORD_LIMIT       0x400U
#define STARS_BATTLE_CONTINUATION       0x27U
#define STARS_BATTLE_PRIMARY            0x1fU
typedef struct StarsWriteBattleEvent {
    const StarsU8 *data;
    StarsU16 size;
    StarsU16 player;
    StarsU16 object_id;
    StarsU16 design_slot;
    StarsU16 is_design;
} StarsWriteBattleEvent;
typedef struct StarsWriteBattleBlock {
    StarsU32 visibility;
    StarsU16 planet_id;
    const StarsU8 *data;
    StarsU16 size;
    const StarsWriteBattleEvent *events;
    StarsU16 event_count;
} StarsWriteBattleBlock;
typedef void (*StarsWriteBattleMarkPlayer)(void *, StarsU16);
typedef void (*StarsWriteBattleMarkPlanet)(void *, StarsU16, StarsI16);
typedef void (*StarsWriteBattleMarkObject)(void *, StarsU16, StarsU16);
typedef void (*StarsWriteBattleRecord)(void *, StarsU16, StarsU16,
    const StarsU8 *);
typedef struct StarsWriteBattlesOps {
    StarsWriteBattleMarkPlayer mark_player;
    StarsWriteBattleMarkPlanet mark_planet;
    StarsWriteBattleMarkObject mark_design;
    StarsWriteBattleMarkObject mark_fleet;
    StarsWriteBattleRecord write_record;
} StarsWriteBattlesOps;
typedef struct StarsWriteBattlesInput {
    StarsI16 player;
    StarsI16 log_is_current;
    const StarsWriteBattleBlock *blocks;
    StarsU16 block_count;
} StarsWriteBattlesInput;
/* Filter battle blocks for a viewer, expose their referenced objects, and
 * emit legal-sized primary/continuation records. */
StarsI16 stars_write_battles(const StarsWriteBattlesInput *,
    const StarsWriteBattlesOps *, void *);
typedef StarsU16 (*StarsReadShdefHull)(void *, StarsU16, StarsU8 *, StarsU16);
/* Part callback: eight-byte adapter record, mass word at offset 4; return
 * lookup/technology status (zero means unresolved). The item is the low
 * byte of HS.wFlags, distinct from the high-byte installed quantity. */
typedef StarsI16 (*StarsReadShdefPart)(void *, StarsU16, StarsU16,
    StarsU8 *, StarsU16);
typedef void (*StarsReadShdefCost)(void *, StarsU8 *);
/* Receives compressed bytes without the length prefix. Return decoded
 * character count through the final argument; output has room for 33 bytes. */
typedef StarsI16 (*StarsReadShdefString)(void *, const StarsU8 *, StarsU16,
    StarsU8 *, StarsU16 *);
typedef struct StarsReadShdefOps {
    StarsReadShdefHull lookup_hull;
    StarsReadShdefPart lookup_part;
    StarsReadShdefCost update_cost;
    StarsReadShdefString decode_string;
} StarsReadShdefOps;
/* Read one variable-length runtime ship design into the 147-byte design
 * array.  design points at the complete design table (up to sixteen entries),
 * while record is the serialized _rtshdef prefix and optional component/name
 * tail. */
StarsI16 stars_read_shdef(const StarsU8 *, StarsU16, StarsU8 *, StarsU16,
    StarsI16, const StarsReadShdefOps *, void *);
typedef StarsI16 (*StarsWriteShdefCompress)(void *, const StarsU8 *,
    StarsU8 *, StarsI16 *);
typedef void (*StarsWriteShdefRecord)(void *, StarsU16, StarsU16,
    const StarsU8 *);
typedef struct StarsWriteShdefInput {
    StarsU16 flags;
    StarsU16 hull_id;
    StarsU8 bitmap;
    StarsU8 component_count;
    StarsU16 design_points;
    StarsU16 turn;
    StarsU32 built;
    StarsU32 existing;
    StarsU16 empty_weight;
    const StarsU8 *component_data;
    StarsU16 component_data_size;
    const StarsU8 *name;
} StarsWriteShdefInput;
typedef struct StarsWriteShdefOps {
    StarsWriteShdefCompress compress_name;
    StarsWriteShdefRecord write_record;
} StarsWriteShdefOps;
StarsI16 stars_write_rt_shdef(const StarsWriteShdefInput *, StarsU8 *,
    StarsU16, StarsU16 *, const StarsWriteShdefOps *, void *);
/* Packed planet record view for FReadPlanet.  The payload begins at the
 * encoded record's first byte after the two header words. */
typedef struct StarsReadPlanetInput {
    StarsU16 id_player_word;
    StarsU16 flags_word;
    StarsU16 header_flags;
    StarsU16 game_turn;
    StarsU16 previous_flags;
    StarsU16 previous_turn;
    StarsU16 history;
    StarsU16 preinitialized;
    const StarsU8 *payload;
    StarsU16 payload_size;
} StarsReadPlanetInput;
typedef struct StarsReadPlanetState {
    StarsI16 id;
    StarsI16 player;
    StarsU16 flags;
    StarsU16 turn;
    StarsU8 mineral_levels[3];
    StarsU8 mineral_concentrations[3];
    StarsU8 environment_original[3];
    StarsU8 environment[3];
    StarsU16 routing_word;
    StarsU32 minerals[4];
    StarsU32 opaque_20[2];
    StarsU32 opaque_44;
    StarsU16 opaque_48;
} StarsReadPlanetState;
typedef StarsI16 (*StarsReadPlanetRaceStat)(void *, StarsI16, StarsI16);
typedef StarsI16 (*StarsReadPlanetValue)(void *,
    const StarsReadPlanetState *, StarsI16);
typedef void (*StarsReadPlanetSend)(void *, StarsI16, StarsI16, StarsI16,
    StarsI16, StarsI16);
typedef struct StarsReadPlanetOps {
    StarsReadPlanetRaceStat race_stat;
    StarsReadPlanetValue optimal_value;
    StarsReadPlanetValue desirability;
    StarsI16 (*true_max_growth)(void *, StarsI16);
    StarsReadPlanetSend send_message;
} StarsReadPlanetOps;
StarsI16 stars_read_planet(const StarsReadPlanetInput *, StarsReadPlanetState *,
    StarsI16, const StarsReadPlanetOps *, void *);
StarsI16 stars_write_planet(const StarsReadPlanetState *, StarsU16, StarsU16,
    StarsU8 *, StarsU16, StarsU16 *, StarsWriteGameRecord, void *);
typedef struct StarsReadFleetInput {
    StarsU16 fleet_flags;
    StarsU16 ship_bitmap;
    const StarsU16 *ship_words;
    const StarsU8 *ship_bytes;
    StarsU16 mineral_bitmap;
    const StarsU8 *mineral_payload;
    StarsU16 mineral_payload_size;
    StarsU16 header_type;
    StarsU16 short_record_count;
    const StarsU8 *tail_payload;
    StarsU16 tail_payload_size;
    StarsI16 planet;
    StarsI16 x;
    StarsI16 y;
    StarsI16 game_turn;
    StarsU16 planet_count;
    const StarsI16 *planet_x;
    const StarsI16 *planet_y;
    const StarsU8 *name_payload;
    StarsU16 name_payload_size;
} StarsReadFleetInput;
typedef struct StarsReadFleetState {
    StarsU16 fleet_flags;
    StarsU16 ship_counts[32];
    StarsU32 minerals[5];
    StarsU32 overlap_116[2];
    StarsU32 overlap_44[2];
    StarsU16 design_plan;
    StarsU16 order_count;
    StarsI16 planet;
    StarsI16 x;
    StarsI16 y;
    StarsU8 orders[32 * 18];
    char name[33];
} StarsReadFleetState;
typedef StarsI16 (*StarsReadFleetDecompress)(void *, const StarsU8 *, StarsU16,
    StarsU8 *, StarsU16 *);
typedef void (*StarsReadFleetAlert)(void *, StarsI16);
typedef struct StarsReadFleetOps {
    StarsReadFleetDecompress decompress_name;
    StarsReadFleetAlert alert;
} StarsReadFleetOps;
StarsI16 stars_read_fleet(const StarsReadFleetInput *, StarsReadFleetState *,
    const StarsReadFleetOps *, void *);
typedef void (*StarsWriteFleetName)(void *, const StarsU8 *);
/* Host-facing inputs for WriteFleet.  ship_weights is the already-resolved
 * per-design weight table that the original routine reads through the player
 * record; the serializer itself retains the original width-selection rules. */
typedef struct StarsWriteFleetInput {
    StarsU16 fleet_flags;
    StarsU16 ship_counts[32];
    StarsU32 minerals[5];
    StarsU32 overlap_116[2];
    const StarsU32 *ship_weights;
    StarsU8 design_plan;
    StarsU8 cord;
    const StarsU8 *orders;
    StarsU16 order_count;
    const StarsU8 *name;
} StarsWriteFleetInput;
typedef struct StarsWriteFleetOps {
    StarsWriteGameRecord write_record;
    StarsWriteFleetName write_name;
} StarsWriteFleetOps;
StarsI16 stars_write_fleet(const StarsWriteFleetInput *, StarsU8 *, StarsU16,
    StarsU16 *, const StarsWriteFleetOps *, void *);
typedef StarsI16 (*StarsOpenFileAlert)(void *, StarsI16);
typedef struct StarsOpenFileInput {
    StarsU8 *data;
    StarsU16 type;
    StarsI16 player;
    StarsU16 mode;
    StarsU32 game_lid;
    StarsU32 game_id;
    StarsU16 game_turn;
    StarsU16 game_flags;
    StarsU16 gd_flags;
    StarsU16 file_error_silent;
    StarsU16 header_type;
    StarsU16 header_version;
    StarsU16 header_turn;
    StarsU16 header_flags;
    StarsU32 header_game_lid;
} StarsOpenFileInput;
typedef struct StarsOpenFileState {
    StarsU16 game_turn;
    StarsU16 game_flags;
    StarsU16 gd_flags;
    StarsU16 file_error_silent;
    StarsU16 opened;
} StarsOpenFileState;
typedef struct StarsOpenFileOps {
    StarsOpenGameFile open_file;
    StarsCloseGameFile close_file;
    StarsOpenFileAlert alert;
} StarsOpenFileOps;
StarsI16 stars_open_file(const StarsOpenFileInput *, StarsOpenFileState *,
    const StarsOpenFileOps *, void *);
typedef void (*StarsMarkFileWrite)(void *, StarsU16, const StarsU8 *, StarsU16);
typedef struct StarsMarkFileInput {
    StarsU16 type;
    StarsI16 player;
    StarsU16 mark_mode;
    StarsI16 value;
    StarsU32 game_lid;
    StarsU16 header_type;
    StarsU16 header_version;
    StarsU32 header_game_lid;
    StarsU16 header_flags;
    StarsU16 player_record_flags;
    StarsU16 player_record_type;
} StarsMarkFileInput;
typedef struct StarsMarkFileState {
    StarsU16 header_flags;
    StarsU16 player_record_flags;
    StarsU16 changed;
    StarsU16 file_open;
} StarsMarkFileState;
typedef struct StarsMarkFileOps {
    StarsOpenGameFile open_file;
    StarsCloseGameFile close_file;
    StarsOpenFileAlert alert;
    StarsMarkFileWrite write_record;
} StarsMarkFileOps;
StarsI16 stars_mark_file(const StarsMarkFileInput *, StarsMarkFileState *,
    const StarsMarkFileOps *, void *);
typedef StarsU8 *(*StarsNextBattleBlock)(void *, StarsU8 *, StarsU16,
    StarsU16 *, StarsU16 *);
/* Update the compact battle-history subrecords in a chain of far blocks.
 * next_block supplies the next contiguous host span and its original top
 * marker; no host pointer is stored in the serialized records. */
void stars_update_battle_records(StarsU8 *, StarsU16, StarsU16,
    StarsNextBattleBlock, void *);
typedef void (*StarsDestroyGameHeap)(void *, StarsU16);
typedef StarsI16 (*StarsDestroyGamePrompt)(void *);
typedef void (*StarsDestroyGameWindow)(void *, StarsU16);
typedef void (*StarsDestroyGameVoid)(void *);
typedef struct StarsDestroyGameState {
    StarsU16 message_entry_open;
    StarsI16 player;
    StarsU16 dirty;
    StarsU16 selected_kind;
    StarsI16 selected_id;
    StarsU32 game_lid;
    StarsU16 browser_window;
    StarsU16 report_window;
    StarsU16 popup_window;
    StarsU16 active_window;
    StarsU16 orders_visible;
    StarsU16 selected_full_kind;
    StarsI16 selected_planet;
    StarsI16 selected_fleet;
    StarsI16 selected_waypoint;
    StarsU16 selected_fleet_order;
    StarsU16 selected_planet_production;
    StarsU16 player_count;
    StarsU16 planet_count;
    StarsU16 fleet_count;
    StarsU16 thing_count;
    StarsU16 thing_capacity;
    StarsU16 score_count;
    StarsU16 battle_template;
    StarsU16 battle_log;
    StarsU16 battle_current;
    StarsU16 battle_report_cached[4];
    StarsU16 design_table[16];
    StarsU16 design_hulls[16];
    StarsU16 design_costs[16];
    StarsU16 battle_plan_table[16];
    StarsU8 battle_plan_counts[16];
    StarsU16 control_widths[3];
    StarsU32 salt;
    StarsU32 last_tick;
    StarsU16 scan_flags;
    StarsU32 game_flags;
    StarsU16 game_flags2;
    StarsI16 memory_log_current, memory_log_previous, message_current;
    StarsU16 ai_data_present;
    StarsU16 planet_capacity, turn;
    StarsPoint selected_point;
} StarsDestroyGameState;
typedef struct StarsDestroyGameOps {
    StarsDestroyGameHeap reset_heap;
    StarsDestroyGameVoid finish_message_entry;
    StarsDestroyGamePrompt prompt_save;
    StarsDestroyGameVoid reset_messages;
    StarsDestroyGameWindow destroy_window;
    StarsDestroyGameVoid save_selection;
} StarsDestroyGameOps;
void stars_destroy_cur_game(StarsDestroyGameState *, const StarsDestroyGameOps *,
    void *);
void stars_write_orders(const StarsU8 *, StarsU16, StarsWriteGameRecord, void *);
void stars_write_runtime_string(const StarsU8 *, const StarsU8 *,
    StarsWriteGameRecord, void *);
StarsI16 stars_append_file(StarsI16, StarsMarkGameFile, StarsWriteBofRecord, void *);
void stars_write_battle_plan(const StarsU8 *, const StarsU8 *, StarsI16,
    StarsWriteGameRecord, StarsWriteGameRecord, void *);
typedef void (*StarsWriteBattlePlanLog)(void *, const StarsU8 *, StarsI16);
typedef void (*StarsAdvanceTutor)(void *);
typedef struct StarsLogChangeBtlplanOps {
    StarsWriteBattlePlanLog write_battle_plan;
    StarsAdvanceTutor advance_tutor;
} StarsLogChangeBtlplanOps;
void stars_log_change_btlplan(StarsU8 *, const StarsU8 *,
    const StarsLogChangeBtlplanOps *, void *);
typedef void (*StarsWriteMemoryLog)(void *, StarsU16, StarsU16,
    const StarsU8 *);
/* _logxferf: two 16-bit identifiers, two object nibbles, and sixteen
 * 16-bit item values.  The layout is the original 36-byte near record. */
typedef struct StarsLogXferf {
    StarsU16 id;
    StarsU16 grobj;
    StarsU16 item[16];
} StarsLogXferf;
typedef char stars_log_xferf_size[(sizeof(StarsLogXferf) == 36) ? 1 : -1];
void stars_log_make_valid_xferf(const StarsLogXferf *, const StarsLogXferf *,
    StarsWriteMemoryLog, void *);
/* _logxfer: five signed 32-bit item deltas in the original 24-byte record. */
typedef struct StarsLogXfer {
    StarsU16 id;
    StarsU16 grobj;
    StarsU32 item[5];
} StarsLogXfer;
typedef char stars_log_xfer_size[(sizeof(StarsLogXfer) == 24) ? 1 : -1];
/* The fleet fields consumed by LogChangeFleet. The original _fleet is much
 * larger and contains far pointers and overlapping bitfields; exposing only
 * the fields this logger compares keeps those meanings named instead of
 * scattering offsets through the reconstruction. */
typedef struct StarsLogFleet {
    StarsU16 object_id;
    StarsU16 flags;
    StarsU16 ship_counts[16];
    StarsU32 minerals[5];
    StarsU16 plan;
    StarsU16 field_60;
    StarsU16 order_count;
    const StarsU8 *orders;
} StarsLogFleet;
typedef struct StarsLogChangeFleetState {
    StarsU16 valid_xfer;
    StarsLogXfer xfer;
    StarsU16 valid_xferf;
    StarsLogXferf xferf;
} StarsLogChangeFleetState;
typedef StarsI16 (*StarsLogFleetPreviousOrder)(void *, StarsU16,
    StarsU16);
typedef struct StarsLogChangeFleetOps {
    StarsWriteMemoryLog write_memory;
    StarsLogFleetPreviousOrder previous_order_replace;
} StarsLogChangeFleetOps;
void stars_log_change_fleet(const StarsLogFleet *, const StarsLogFleet *,
    StarsU16, StarsLogChangeFleetState *, const StarsLogChangeFleetOps *,
    void *);
/* The planet fields consumed by LogChangePlanet. Production entries are the
 * original four-byte queue words; the logger does not interpret their inner
 * fields, only their count and byte image. */
typedef struct StarsLogPlanet {
    StarsI16 object_id;
    StarsU32 minerals[4];
    StarsU16 has_production;
    StarsU16 production_count;
    const StarsU8 *production;
    StarsU32 overlap_20;
    StarsU32 overlap_44;
    StarsU16 overlap_48;
} StarsLogPlanet;
typedef struct StarsLogChangePlanetState {
    StarsU16 valid_xfer;
    StarsLogXfer xfer;
} StarsLogChangePlanetState;
typedef StarsI16 (*StarsLogPlanetPreviousProduction)(void *, StarsI16);
typedef struct StarsLogChangePlanetOps {
    StarsWriteMemoryLog write_memory;
    StarsLogPlanetPreviousProduction previous_production;
} StarsLogChangePlanetOps;
void stars_log_change_planet(const StarsLogPlanet *, const StarsLogPlanet *,
    StarsU16, StarsLogChangePlanetState *, const StarsLogChangePlanetOps *,
    void *);
/* _thing is an 18-byte record; LogChangeThing consumes idFull and the three
 * 16-bit words beginning at offset 8, leaving the remaining fields opaque. */
typedef struct StarsThing { StarsU8 raw[18]; } StarsThing;
typedef char stars_thing_size[(sizeof(StarsThing) == 18) ? 1 : -1];
StarsI16 stars_write_thing_records(const StarsThing *, StarsU16,
    StarsWriteGameRecord, void *);
/* FWriteDataFile writes 24-byte _scorex records only to a player log. */
#define STARS_SCOREX_RECORD_BYTES 24U
typedef struct StarsWriteScoreRecordsInput {
    const StarsU8 *records;
    const StarsU8 *visible_players;
    const StarsU8 *dead_players;
    StarsU16 player_count;
    StarsI16 file_player;
    StarsU16 game_flags;
    StarsU16 turn;
} StarsWriteScoreRecordsInput;
StarsI16 stars_write_score_records(const StarsWriteScoreRecordsInput *,
    StarsWriteGameRecord, void *);
/* Host-facing semantic view of the 18-byte thing record used by
 * DoThingInteractions.  The raw executable stores the visited-player mask at
 * offset 0x0c and the trader/target mask at offset 0x0e; keeping those words
 * explicit prevents the interaction routine from becoming another offset
 * soup while retaining the original bit meanings. */
typedef struct StarsThingInteractionThing {
    StarsI16 x;
    StarsI16 y;
    StarsU16 overlap;
    StarsU32 visited_players;
    StarsU32 target_players;
} StarsThingInteractionThing;
typedef struct StarsThingInteractionFleet {
    StarsI16 x;
    StarsI16 y;
    StarsI16 player;
    StarsI16 planet;
    StarsU16 id;
    StarsU16 overlap;
    StarsU32 minerals[3];
    StarsU16 ship_count;
    StarsU16 design_counts[16];
} StarsThingInteractionFleet;
typedef struct StarsThingInteractionPlanet {
    StarsI16 x;
    StarsI16 y;
    StarsI16 owner;
    StarsU16 overlap;
    StarsU16 id;
    StarsU32 minerals[3];
} StarsThingInteractionPlanet;
typedef struct StarsThingInteractionPlayer {
    StarsU8 technology[6];
    StarsU32 trader_bits;
    StarsU16 detail_flags;
    StarsU16 technology_stage;
    StarsU16 dead;
    StarsU16 fleet_count;
    StarsU16 design_flags[16];
    StarsU32 design_built[16];
    StarsU32 design_exist[16];
} StarsThingInteractionPlayer;
typedef struct StarsThingDesignProfile {
    StarsU8 bytes[73];
} StarsThingDesignProfile;
typedef StarsI16 (*StarsThingRandom)(void *, StarsI16);
typedef void (*StarsThingSendMessage2)(void *, StarsI16, StarsI16,
    StarsU32, StarsU16, StarsI16);
typedef void (*StarsThingSendMessage)(void *, StarsI16, StarsI16, StarsI16,
    StarsU16, StarsI16, StarsI16, StarsI16, StarsI16, StarsI16);
typedef void (*StarsThingRemoveMessage)(void *, StarsI16, StarsI16, StarsU32);
typedef StarsU16 (*StarsThingFleetWord)(void *,
    const StarsThingInteractionFleet *);
typedef StarsU32 (*StarsThingDevelopCost)(void *, StarsI16, const StarsU8 *);
typedef void (*StarsThingUpdateResearch)(void *, StarsI16);
typedef void (*StarsThingGetDesignProfile)(void *, StarsI16, StarsI16,
    StarsThingDesignProfile *);
typedef StarsI16 (*StarsThingFindDesign)(void *, StarsI16,
    const StarsThingDesignProfile *);
typedef StarsI16 (*StarsThingNewFleet)(void *, StarsI16, StarsI16,
    StarsI16, StarsI16, StarsI16, StarsU16, StarsThingInteractionFleet *);
typedef void (*StarsThingUpdateDesign)(void *, StarsI16, StarsI16);
typedef StarsU32 (*StarsThingFleetStat)(void *,
    const StarsThingInteractionFleet *, StarsI16);
typedef StarsI16 (*StarsThingGiveTraderPart)(void *, StarsU16, StarsI16,
    StarsU16 *);
typedef struct StarsThingInteractionOps {
    StarsThingRandom random_value;
    StarsThingSendMessage2 send_message2;
    StarsThingSendMessage send_message;
    StarsThingRemoveMessage remove_message;
    StarsThingFleetWord fleet_word;
    StarsThingDevelopCost develop_cost;
    StarsThingUpdateResearch update_research;
    StarsThingGetDesignProfile get_design_profile;
    StarsThingFindDesign find_design;
    StarsThingNewFleet new_fleet;
    StarsThingUpdateDesign update_design;
    StarsThingFleetStat fleet_stat;
    StarsThingGiveTraderPart give_trader_part;
} StarsThingInteractionOps;
void stars_do_thing_interactions(StarsI16, StarsThingInteractionThing *,
    StarsU16, StarsThingInteractionFleet *, StarsU16,
    StarsThingInteractionPlanet *, StarsU16, StarsThingInteractionPlayer *,
    const StarsThingInteractionOps *, void *, StarsI16);
typedef struct StarsFleetNameRecord { char *name; } StarsFleetNameRecord;
typedef StarsFleetNameRecord *(*StarsFindFleetName)(void *, StarsI16);
typedef char *(*StarsAllocateFleetName)(void *, StarsU16);
typedef void (*StarsFreeFleetName)(void *, char *);
typedef StarsI16 (*StarsCompressFleetName)(void *, const StarsU8 *,
    StarsU8 *, StarsI16 *);
typedef struct StarsLogChangeNameOps {
    StarsFindFleetName find_fleet;
    StarsAllocateFleetName allocate_name;
    StarsFreeFleetName free_name;
    StarsCompressFleetName compress_name;
    StarsWriteMemoryLog write_memory;
    StarsAdvanceTutor advance_tutor;
} StarsLogChangeNameOps;
void stars_log_change_name(StarsI16, StarsI16, const StarsU8 *, StarsU16,
    const StarsLogChangeNameOps *, void *);
typedef StarsU16 (*StarsWriteShdefPayload)(void *, const StarsU8 *,
    StarsU8 *, StarsU16);
typedef struct StarsLogChangeShdefInput {
    StarsU16 game_flags;
    StarsU16 tutor_flags;
    StarsI16 player;
    StarsU16 design_flags;
    const StarsU8 *design_payload;
    StarsU16 design_payload_size;
} StarsLogChangeShdefInput;
typedef struct StarsLogChangeShdefOps {
    StarsWriteShdefPayload write_design;
    StarsWriteMemoryLog write_memory;
    StarsAdvanceTutor advance_tutor;
} StarsLogChangeShdefOps;
StarsI16 stars_log_change_shdef(const StarsLogChangeShdefInput *,
    const StarsLogChangeShdefOps *, void *);
typedef StarsI16 (*StarsLookupDefensePart)(void *, StarsU8 *);
StarsI16 stars_get_best_defense_part(StarsU8 *, StarsLookupDefensePart, void *);
void stars_show_tutor(StarsU16 *, StarsU16 *, StarsI16, StarsInitShowWindow,
    void *);
typedef StarsU8 *(*StarsFindMutableFleet)(void *, StarsI16);
typedef struct StarsLogSplitFleetOps {
    StarsWriteMemoryLog write_memory;
    StarsFindMutableFleet find_fleet;
} StarsLogSplitFleetOps;
void stars_log_split_fleet(StarsU8 *, StarsI16, const StarsLogSplitFleetOps *,
    void *);
StarsI16 stars_get_prev_log_rt(const StarsU8 *, const StarsU8 *, StarsU8 *,
    StarsU8 *);
typedef StarsU16 (*StarsRunLogRecord)(void *, StarsU16, StarsU16,
    const StarsU8 *);
StarsI16 stars_run_log_file(StarsU8 *, const StarsU8 *, StarsRunLogRecord,
    void *);
typedef StarsI16 (*StarsEnumLogRecord)(void *, StarsU16, StarsU16,
    const StarsU8 *, void *);
void stars_enum_log_rts(const StarsU8 *, const StarsU8 *, StarsEnumLogRecord,
    void *, void *);
void stars_log_merge_fleet(StarsU8 *, StarsI16, StarsWriteMemoryLog, void *);
typedef struct StarsLogChangeRelationsOps {
    StarsWriteMemoryLog write_memory;
    StarsAdvanceTutor advance_tutor;
} StarsLogChangeRelationsOps;
void stars_log_change_relations(StarsU8 *, const StarsU8 *, const StarsU8 *,
    const StarsLogChangeRelationsOps *, void *);
/* Unpack a type-0x1e battle-plan payload into a 36-byte plan record. The
 * input and destination spans are the original record layouts. */
void stars_unpack_battle_plan(const StarsU8 *, const StarsU8 *, StarsU8 *, StarsI16);
StarsI16 stars_ask_save_dialog(StarsU16, StarsU16, StarsU16, StarsI32,
    const StarsU8 *, StarsEndDialog, StarsWinHelp, void *);
void stars_prompt_save_game(StarsU8 *, const StarsPromptSaveOps *, void *);
void stars_set_visible_plan_fleet(StarsU8 *, StarsI16,
    const StarsVisiblePlanFleetOps *, void *);
typedef struct StarsVisPFPlayer {
    StarsU16 dead;
    StarsU16 detail_flags;
    StarsU16 fleet_count;
    StarsU16 planet_count;
    StarsU8 design_count;
    StarsU16 design_flags[16];
    StarsU32 design_built[16];
    StarsU16 starbase_flags[10];
    StarsU32 starbase_built[10];
} StarsVisPFPlayer;
typedef struct StarsVisPFFleet {
    StarsI16 player;
    StarsI16 planet_id;
    StarsU16 flags;
    StarsU16 ship_counts[16];
    StarsI16 scanner_result;
    StarsU16 scanner_steal;
    StarsI32 mine_strength;
    StarsU16 mine_special;
} StarsVisPFFleet;
typedef struct StarsVisPFPlanet {
    StarsI16 player;
    StarsU16 flags;
    StarsU16 starbase_slot;
    StarsU16 id;
} StarsVisPFPlanet;
typedef struct StarsVisPFThing {
    StarsU16 kind;
    StarsI16 owner;
    StarsU16 dest_x;
    StarsU16 flags;
    StarsU32 owner_mask;
    StarsU32 target_mask;
} StarsVisPFThing;
typedef void (*StarsVisPFMarkPlanet)(void *, StarsU16, StarsI16, StarsI16);
typedef StarsI16 (*StarsVisPFScanner)(void *, const StarsVisPFFleet *,
    StarsU16 *);
typedef StarsI32 (*StarsVisPFMine)(void *, const StarsVisPFFleet *);
typedef struct StarsVisPFInitOps {
    StarsVisPFMarkPlanet mark_planet;
    StarsVisPFScanner scanner;
    StarsVisPFMine mine_from_fleet;
} StarsVisPFInitOps;
typedef struct StarsVisPFInitInput {
    StarsI16 viewer;
    StarsI16 race_stat;
    StarsVisPFPlayer *players;
    StarsU16 player_count;
    StarsVisPFPlanet *planets;
    StarsU16 planet_count;
    StarsVisPFFleet *fleets;
    StarsU16 fleet_count;
    StarsVisPFThing *things;
    StarsU16 thing_count;
} StarsVisPFInitInput;
void stars_set_vis_pf_init(StarsVisPFInitInput *,
    const StarsVisPFInitOps *, void *);
typedef struct StarsVisThingTarget {
    StarsU16 kind;
    StarsI16 owner;
    StarsI16 x;
    StarsI16 y;
    StarsU16 dest_x;
    StarsU32 bits_2;
    StarsU32 bits_4;
    StarsU32 bits_8;
    StarsU32 range_squared;
    /* Raw kind-0 concealment gate, kept as a named 32-bit comparison. */
    StarsU32 concealment_threshold;
} StarsVisThingTarget;
typedef struct StarsVisThingFleet {
    StarsU16 id;
    StarsI16 x;
    StarsI16 y;
    StarsI16 planet_id;
    StarsU16 flags;
    StarsI16 cloak_percent;
} StarsVisThingFleet;
typedef struct StarsVisThingPlanet {
    StarsU16 id;
    StarsI16 x;
    StarsI16 y;
    StarsI16 owner;
    StarsU16 flags;
    StarsU32 starbase_count;
    StarsI32 starbase_delta;
} StarsVisThingPlanet;
typedef void (*StarsVisThingMarkFleet)(void *, StarsU16, StarsI16);
typedef void (*StarsVisThingMarkPlanet)(void *, StarsU16, StarsI16,
    StarsI16);
typedef StarsI16 (*StarsVisThingCloak)(void *, const StarsVisThingFleet *);
typedef StarsI16 (*StarsVisThingRandom)(void *, StarsI16);
typedef struct StarsSetVisPFThingsOps {
    StarsVisThingMarkFleet mark_fleet;
    StarsVisThingMarkPlanet mark_planet;
    StarsVisThingCloak cloak_percent;
    StarsVisThingRandom random_value;
} StarsSetVisPFThingsOps;
typedef struct StarsSetVisPFThingsInput {
    StarsI16 viewer;
    StarsI16 race_stat;
    StarsU16 *player_detail_flags;
    StarsU16 player_count;
    StarsVisThingTarget *things;
    StarsU16 thing_count;
    StarsVisThingFleet *fleets;
    StarsU16 fleet_count;
    StarsVisThingPlanet *planets;
    StarsU16 planet_count;
} StarsSetVisPFThingsInput;
void stars_set_vis_pf_things(StarsSetVisPFThingsInput *,
    const StarsSetVisPFThingsOps *, void *);
/* Inputs returned by GetCachedFleetScannerRange/PctCloakFromLpfl for
 * SetVisPFFleets.  The executable's fleet record is substantially larger;
 * this view names only the geometry, visibility, and scanner fields used by
 * the procedure. */
typedef struct StarsVisFleetScanFleet {
    StarsU16 id;
    StarsI16 player;
    StarsI16 x;
    StarsI16 y;
    StarsI16 planet_id;
    StarsU16 flags;
    StarsI16 cloak_percent;
    StarsU16 scanner_range;
    StarsI16 planet_range;
    StarsI16 detection_percent;
    StarsU16 scanner_bits;
} StarsVisFleetScanFleet;
typedef void (*StarsVisFleetMarkFleet)(void *, StarsU16, StarsI16);
typedef void (*StarsVisFleetMarkPlanet)(void *, StarsU16, StarsI16,
    StarsI16);
typedef StarsU16 (*StarsVisFleetScanner)(void *,
    const StarsVisFleetScanFleet *, StarsI16 *, StarsI16 *, StarsU16 *);
typedef StarsI16 (*StarsVisFleetCloak)(void *,
    const StarsVisFleetScanFleet *);
typedef struct StarsSetVisPFFleetsOps {
    StarsVisFleetMarkFleet mark_fleet;
    StarsVisFleetMarkPlanet mark_planet;
    StarsVisFleetScanner scanner;
    StarsVisFleetCloak cloak_percent;
} StarsSetVisPFFleetsOps;
typedef struct StarsSetVisPFFleetsInput {
    StarsI16 viewer;
    StarsU16 *player_detail_flags;
    StarsU16 player_count;
    StarsVisFleetScanFleet *fleets;
    StarsU16 fleet_count;
    StarsVisThingTarget *things;
    StarsU16 thing_count;
    StarsVisThingPlanet *planets;
    StarsU16 planet_count;
} StarsSetVisPFFleetsInput;
void stars_set_vis_pf_fleets(StarsSetVisPFFleetsInput *,
    const StarsSetVisPFFleetsOps *, void *);
/* Planet-centered visibility input used by SetVisPFPlanets. */
typedef struct StarsVisPlanetScanPlanet {
    StarsU16 id;
    StarsI16 x;
    StarsI16 y;
    StarsI16 owner;
    StarsU16 flags;
    StarsU16 starbase_slot;
    StarsU32 starbase_count;
    StarsI32 starbase_delta;
    StarsI16 scanner_range;
    StarsI16 deep_range;
    StarsI16 stargate_range;
} StarsVisPlanetScanPlanet;
typedef void (*StarsVisPlanetMarkFleet)(void *, StarsU16, StarsI16);
typedef void (*StarsVisPlanetMarkPlanet)(void *, StarsU16, StarsI16,
    StarsI16);
typedef StarsI16 (*StarsVisPlanetRaceStat)(void *, StarsI16, StarsI16);
typedef StarsI16 (*StarsVisPlanetScanner)(void *,
    const StarsVisPlanetScanPlanet *, StarsI16 *);
typedef StarsI16 (*StarsVisPlanetStargate)(void *,
    const StarsVisPlanetScanPlanet *, StarsI16, StarsI16);
typedef StarsI16 (*StarsVisPlanetCloak)(void *,
    const StarsVisFleetScanFleet *);
typedef void (*StarsVisPlanetProgress)(void *, StarsI16);
typedef struct StarsSetVisPFPlanetsOps {
    StarsVisPlanetMarkFleet mark_fleet;
    StarsVisPlanetMarkPlanet mark_planet;
    StarsVisPlanetRaceStat race_stat;
    StarsVisPlanetScanner scanner;
    StarsVisPlanetStargate stargate_range;
    StarsVisPlanetCloak cloak_percent;
    StarsVisPlanetProgress progress;
} StarsSetVisPFPlanetsOps;
typedef struct StarsSetVisPFPlanetsInput {
    StarsI16 viewer;
    StarsI16 race_stat;
    StarsU16 *starbase_design_flags;
    StarsI16 *stargate_ranges;
    StarsU16 *player_detail_flags;
    StarsU16 player_count;
    StarsVisPlanetScanPlanet *planets;
    StarsU16 planet_count;
    StarsVisFleetScanFleet *fleets;
    StarsU16 fleet_count;
    StarsVisThingTarget *things;
    StarsU16 thing_count;
} StarsSetVisPFPlanetsInput;
void stars_set_vis_pf_planets(StarsSetVisPFPlanetsInput *,
    const StarsSetVisPFPlanetsOps *, void *);
void stars_ensure_cas_hdefs(StarsI16);
void stars_cancel_mem_rt(StarsU8 *, StarsI16);
typedef void (*StarsSetMessageTitle)(void *, StarsU16);
void stars_dirty_game(StarsU8 *, StarsI16, StarsSetMessageTitle, void *);
typedef void (*StarsLogOverflow)(void *);
typedef struct StarsWriteMemRtOps {
    StarsSetMessageTitle set_message_title;
    StarsLogOverflow log_overflow;
} StarsWriteMemRtOps;
void stars_log_change_thing(StarsU8 *, StarsU8 *, StarsU16 *, StarsLogXfer *,
    const StarsThing *, const StarsThing *, const StarsWriteMemRtOps *, void *);
void stars_log_make_valid_xfer(StarsU8 *, StarsU8 *, const StarsLogXfer *,
    const StarsLogXfer *, const StarsWriteMemRtOps *, void *);
void stars_write_mem_rt(StarsU8 *, StarsU8 *, StarsI16, StarsI16,
    const StarsU8 *, const StarsWriteMemRtOps *, void *);
typedef StarsU32 (*StarsGetFleetStat)(void *, const StarsU8 *, StarsU16);
StarsU32 stars_get_fuel_free(const StarsU8 *, StarsGetFleetStat, void *);
StarsU32 stars_get_cargo_free(const StarsU8 *, StarsGetFleetStat, void *);
StarsI16 stars_fuel_tanker(const StarsU8 *);
typedef StarsI16 (*StarsPtToScan)(void *, StarsI16);
void stars_logical_to_scan(StarsU8 *, StarsPtToScan, void *);
typedef StarsI16 (*StarsScanToPt)(void *, StarsI16);
void stars_scan_to_logical(StarsU8 *, StarsScanToPt, void *);
typedef StarsI16 (*StarsGetMessage)(void *, StarsI16, StarsU8 *);
StarsI16 stars_idm_get_message_n(StarsI16, StarsGetMessage, void *);
typedef const StarsU8 *(*StarsFormatMessage)(void *, StarsI16,
    const StarsU8 *);
const StarsU8 *stars_psz_get_message_n(StarsI16, StarsGetMessage,
    StarsFormatMessage, StarsU8 *, void *);
StarsI16 stars_can_split(const StarsU8 *, StarsI32);
StarsI16 stars_can_split_all(const StarsU8 *, StarsI32);
/* Host representation of the active fleet-pointer list used by FCanMerge.
 * present==0 is the original null far-pointer sentinel. */
typedef struct StarsMergeFleet {
    StarsI16 player;
    StarsI16 x;
    StarsI16 y;
    StarsU16 ship_counts[16];
    StarsI16 present;
} StarsMergeFleet;
StarsI16 stars_can_merge(const StarsMergeFleet *, const StarsMergeFleet *,
    StarsU16, StarsI16);
typedef void (*StarsMergeCargoBalance)(void *, StarsU8 *, StarsU8 *);
typedef void (*StarsMergeLookup)(void *, StarsI16, StarsU8 *);
typedef void (*StarsMergeDelete)(void *, StarsI16, StarsI16, StarsI16);
typedef void (*StarsMergeInvalidate)(void *, StarsI16, StarsI16);
typedef struct StarsMergeFleetInput {
    StarsU8 *destination;
    StarsU8 *deleted;
    StarsI16 no_delete;
    StarsMergeCargoBalance cargo_balance;
    StarsMergeLookup lookup;
    StarsMergeDelete delete_fleet;
    StarsMergeInvalidate invalidate_report;
    void *context;
} StarsMergeFleetInput;
void stars_merge_two_fleets(const StarsMergeFleetInput *);
typedef struct StarsDropDownFleet {
    StarsU16 id;
    StarsI16 x;
    StarsI16 y;
} StarsDropDownFleet;
typedef struct StarsSetFleetDropDownOps {
    StarsHandleSendMessage send_message;
    StarsHandleDrawPlanShip draw_plan_ship;
} StarsSetFleetDropDownOps;
void stars_set_fleet_dropdown_sel(StarsU16, StarsU16, StarsI16, StarsU16,
    StarsI16, StarsI16, const StarsDropDownFleet *, StarsU16,
    const StarsSetFleetDropDownOps *, void *);
typedef struct StarsShipComponentStat {
    StarsU16 group;
    StarsU16 packed_count_type;
} StarsShipComponentStat;
typedef struct StarsHullStat {
    StarsU16 fuel_max;
    StarsU16 cargo_max;
} StarsHullStat;
typedef struct StarsShipDesignStat {
    StarsU16 component_count;
    const StarsShipComponentStat *components;
} StarsShipDesignStat;
StarsI16 stars_wt_max_ship_design_stat(const StarsShipDesignStat *,
    const StarsHullStat *, StarsI16);
typedef struct StarsFleetStatEntry {
    StarsI16 ship_count;
    const StarsShipDesignStat *design;
    const StarsHullStat *hull;
} StarsFleetStatEntry;
StarsI32 stars_get_fleet_stat(const StarsFleetStatEntry *, StarsU16,
    StarsU16, StarsI16);
typedef struct StarsJettisonRecord {
    StarsU16 fleet_id;
    StarsU8 flags;
    StarsU8 mineral_mask;
    StarsI16 byte_amounts[5];
    StarsI32 long_amounts[5];
} StarsJettisonRecord;
typedef StarsI16 (*StarsLookupJettisonFleet)(void *, StarsU16,
    StarsI16 *, StarsI16 *);
StarsI16 stars_enum_calc_jettison(const StarsJettisonRecord *, StarsI16,
    StarsU32 *, StarsU16, StarsLookupJettisonFleet, void *);
typedef struct StarsTransferEndpoint {
    StarsI16 group;
    StarsU16 id;
    void *object;
} StarsTransferEndpoint;
typedef StarsI32 (*StarsChangeCargo)(void *, StarsI16, StarsU16, StarsI16,
    StarsI32, void *);
StarsI32 stars_xfer_supply(StarsI16, StarsI32,
    const StarsTransferEndpoint *, StarsChangeCargo, void *);
typedef struct StarsTransferFullRecord {
    StarsU16 source_id;
    StarsU16 destination_id;
    StarsU8 source_group;
    StarsU8 destination_group;
    StarsU32 quantities[5];
} StarsTransferFullRecord;
typedef StarsI16 (*StarsTransferLookupOwner)(void *, StarsI16, StarsU16,
    StarsI16 *);
typedef StarsI32 (*StarsTransferApplyCargo)(void *, StarsI16, StarsU16,
    StarsI16, StarsI32);
typedef struct StarsTransferMessage {
    StarsI16 recipient;
    StarsI16 event;
    StarsU16 source_tag;
    StarsU16 destination_tag;
    StarsI16 supply;
    StarsI32 requested;
    StarsI32 accepted;
} StarsTransferMessage;
typedef void (*StarsTransferSendMessage)(void *, const StarsTransferMessage *);
typedef struct StarsTransferToOthersInput {
    StarsTransferFullRecord *records;
    StarsU16 record_count;
    StarsTransferLookupOwner lookup_owner;
    StarsTransferApplyCargo apply_cargo;
    StarsTransferSendMessage send_message;
    void *context;
} StarsTransferToOthersInput;
void stars_transfer_to_others(StarsTransferToOthersInput *);

/* FleetTransferCargoBalance works on the two newly-created fleet records
 * while looking up the corresponding pre-transfer records.  The original
 * _fleet layout mixes signed ship counts, packed per-design status words, and
 * five signed cargo dwords.  These named fields keep that representation
 * visible without making callers depend on the original private structure. */
#define STARS_FLEET_BALANCE_DESIGNS 16
#define STARS_FLEET_BALANCE_RESOURCES 5
#define STARS_FLEET_BALANCE_MINERALS 4
#define STARS_FLEET_BALANCE_STATUS_MASK 0x007fU
#define STARS_FLEET_BALANCE_SPECIAL_FLAG 0x0400U
typedef struct StarsFleetBalanceFleet {
    StarsI16 player;
    StarsU16 flags;
    StarsI16 ship_count[STARS_FLEET_BALANCE_DESIGNS];
    StarsU16 ship_status[STARS_FLEET_BALANCE_DESIGNS];
    StarsI32 cargo[STARS_FLEET_BALANCE_RESOURCES];
} StarsFleetBalanceFleet;
typedef StarsI16 (*StarsFleetBalanceLookup)(void *, StarsI16,
    StarsFleetBalanceFleet *);
typedef StarsI16 (*StarsFleetBalanceWeight)(void *, StarsI16, StarsI16,
    StarsI16);
typedef struct StarsFleetTransferCargoBalanceInput {
    StarsFleetBalanceFleet *new_fleets[2];
    StarsI16 fleet_ids[2];
    StarsI16 player;
    StarsFleetBalanceLookup lookup_fleet;
    StarsFleetBalanceWeight max_weight;
    void *context;
} StarsFleetTransferCargoBalanceInput;
void stars_fleet_transfer_cargo_balance(
    StarsFleetTransferCargoBalanceInput *);

/* ShipCommandProc is the command router for the ship pane.  Window handles
 * and notification codes are inputs, while the host supplies the original
 * pane/fleet side effects through one named action boundary. */
#define STARS_SHIP_COMMAND_BUTTONS 11
#define STARS_SHIP_COMMAND_ORDERS 3
#define STARS_SHIP_COMMAND_NOTIFY_CLICK 0
#define STARS_SHIP_COMMAND_NOTIFY_CHANGE 1
#define STARS_SHIP_COMMAND_NOTIFY_EDIT 0x0300
#define STARS_SHIP_ACTION_SELECT_NEXT_FLEET 1
#define STARS_SHIP_ACTION_SELECT_PREVIOUS_FLEET 2
#define STARS_SHIP_ACTION_RENAME_FLEET 3
#define STARS_SHIP_ACTION_SELECT_PLANET 4
#define STARS_SHIP_ACTION_TRANSFER_TO_PLANET 5
#define STARS_SHIP_ACTION_TRANSFER_FROM_ORBIT 6
#define STARS_SHIP_ACTION_SELECT_ORBITING_FLEET 7
#define STARS_SHIP_ACTION_TRANSFER_FLEET 8
#define STARS_SHIP_ACTION_TRANSFER_ALL 9
#define STARS_SHIP_ACTION_SPLIT_FLEET 10
#define STARS_SHIP_ACTION_MERGE_FLEETS 11
#define STARS_SHIP_ACTION_DRAW 12
#define STARS_SHIP_ACTION_SET_SCAN_WAYPOINT 13
#define STARS_SHIP_ACTION_COMPONENT_POPUP 14
#define STARS_SHIP_ACTION_SELECT_BATTLE_PLAN 15
#define STARS_SHIP_ACTION_UPDATE_REPORT 16
#define STARS_SHIP_ACTION_SET_ORDER_KIND 17
#define STARS_SHIP_ACTION_SET_ORDER_TARGET 18
#define STARS_SHIP_ACTION_SET_ORDER_VALUE 19
typedef struct StarsShipCommandState {
    StarsU16 button[STARS_SHIP_COMMAND_BUTTONS];
    StarsU16 ship_dropdown;
    StarsU16 ship_list;
    StarsU16 component_list;
    StarsU16 battle_dropdown;
    StarsU16 report_combo;
    StarsU16 order_dropdown[STARS_SHIP_COMMAND_ORDERS];
    StarsU16 order_edit;
    StarsI16 fleet_id;
    StarsI16 planet_id;
    StarsI16 order_index;
    StarsI16 order_kind;
    StarsI16 order_values[3];
    StarsU16 report_flags;
    StarsU16 scan_flags;
} StarsShipCommandState;
typedef void (*StarsShipCommandAction)(void *, StarsI16, StarsI16);
typedef StarsI32 (*StarsShipCommandSelection)(void *, StarsU16);
typedef StarsI32 (*StarsShipCommandEditValue)(void *, StarsU16);
typedef struct StarsShipCommandOps {
    StarsShipCommandAction action;
    StarsShipCommandSelection selection;
    StarsShipCommandEditValue edit_value;
} StarsShipCommandOps;
typedef struct StarsShipCommandInput {
    StarsU16 frame_window;
    StarsU16 mine_window;
    StarsU16 scanner_window;
    StarsU16 message_window;
    StarsU16 player;
    StarsShipCommandState *state;
    const StarsShipCommandOps *ops;
    void *context;
} StarsShipCommandInput;
void stars_ship_command_proc(StarsU16, StarsU16, StarsU32,
    StarsShipCommandInput *);

/* ClickInShipOrders maps the ship-pane reference rectangles to transfer,
 * order-edit, popup, and gauge-drag actions. */
#define STARS_CLICK_SHIP_REGIONS 19
#define STARS_CLICK_REGION_FLEET_FUEL 1
#define STARS_CLICK_REGION_FUEL_TRANSFER 2
#define STARS_CLICK_REGION_PLANET_TRANSFER 3
#define STARS_CLICK_REGION_ORBIT_TRANSFER 4
#define STARS_CLICK_REGION_ORDER_KIND 0
#define STARS_CLICK_REGION_ORDER_TARGET 18
#define STARS_CLICK_REGION_MASS_WARP 15
#define STARS_CLICK_REGION_POPUP 12
#define STARS_CLICK_ACTION_TRANSFER_PLANET 1
#define STARS_CLICK_ACTION_TRANSFER_ORBIT 2
#define STARS_CLICK_ACTION_SET_ORDER_KIND 3
#define STARS_CLICK_ACTION_SET_ORDER_TARGET 4
#define STARS_CLICK_ACTION_DRAG_FUEL 5
#define STARS_CLICK_ACTION_DRAG_MASS_WARP 6
#define STARS_CLICK_ACTION_DRAG_ORDER 7
#define STARS_CLICK_ACTION_SHOW_ORDER_POPUP 8
#define STARS_CLICK_ACTION_SHOW_ZIP_POPUP 9
#define STARS_CLICK_ACTION_REFRESH 10
typedef struct StarsClickShipOrdersState {
    StarsRect reference[STARS_CLICK_SHIP_REGIONS];
    StarsI16 selected_object;
    StarsI16 selected_planet;
    StarsI16 selected_fleet;
    StarsI16 order_kind;
    StarsI16 order_values[3];
    StarsI16 scan_object;
    StarsI16 scan_planet;
    StarsI32 fuel;
    StarsI16 mass_warp;
} StarsClickShipOrdersState;
typedef void (*StarsClickShipAction)(void *, StarsI16, StarsI32);
typedef StarsI32 (*StarsClickShipDrag)(void *, StarsI16, StarsI32);
typedef StarsI16 (*StarsClickShipPopup)(void *, StarsI16, StarsI16,
    StarsI16);
typedef struct StarsClickShipOrdersOps {
    StarsClickShipAction action;
    StarsClickShipDrag drag;
    StarsClickShipPopup popup;
} StarsClickShipOrdersOps;
typedef struct StarsClickShipOrdersInput {
    StarsClickShipOrdersState *state;
    const StarsClickShipOrdersOps *ops;
    void *context;
} StarsClickShipOrdersInput;
StarsU16 stars_click_ship_orders(StarsPoint, StarsI16, StarsI16, StarsI16,
    StarsClickShipOrdersInput *);

/* DrawShipScanPath consumes the selected scanner object and emits the
 * geometric line/arrow/route primitives that the original GDI routine draws.
 * Keeping geometry separate from the host DC makes the direction and route
 * rules testable without replaying emitted bytes. */
#define STARS_SCAN_PATH_MAX_ORDERS 64
#define STARS_SCAN_PATH_KIND_NONE 0
#define STARS_SCAN_PATH_KIND_PLANET 1
#define STARS_SCAN_PATH_KIND_FLEET 2
#define STARS_SCAN_PATH_KIND_THING 8
#define STARS_SCAN_PATH_FLAG_STARBASE 0x0080U
typedef struct StarsDrawShipScanPathState {
    StarsI16 selected_kind;
    StarsI16 scan_kind;
    StarsI16 scan_thing;
    StarsU16 orders_visible;
    StarsU16 scanner_flags;
    StarsU16 drawing_busy;
    StarsPoint selected_point;
    StarsPoint transit_point;
    StarsI16 transit_valid;
    StarsI16 transit_type;
    StarsI16 route_count;
    StarsPoint route_points[STARS_SCAN_PATH_MAX_ORDERS];
    StarsI16 planet_route_count;
    StarsPoint planet_route[STARS_SCAN_PATH_MAX_ORDERS];
} StarsDrawShipScanPathState;
typedef void (*StarsScanPathLine)(void *, StarsU16, StarsPoint, StarsPoint,
    StarsI16);
typedef void (*StarsScanPathState)(void *, StarsU16, StarsI16);
typedef struct StarsDrawShipScanPathOps {
    StarsU16 (*acquire_dc)(void *, StarsU16);
    void (*release_dc)(void *, StarsU16, StarsU16);
    StarsScanPathLine line;
    StarsScanPathState set_pen;
    StarsScanPathState set_mode;
} StarsDrawShipScanPathOps;
typedef struct StarsDrawShipScanPathInput {
    StarsU16 scanner_window;
    StarsI16 scrollbar_height;
    StarsI16 client_left;
    StarsI16 client_top;
    StarsI16 client_right;
    StarsI16 client_bottom;
    StarsDrawShipScanPathState *state;
    const StarsDrawShipScanPathOps *ops;
    void *context;
} StarsDrawShipScanPathInput;
void stars_draw_ship_scan_path(StarsU16, StarsI16,
    StarsDrawShipScanPathInput *);
typedef void (*StarsRelationsSticky)(void *, StarsU16, StarsI16);
typedef void (*StarsRelationsPaint)(void *, StarsU16);
typedef void (*StarsRelationsFillBackground)(void *, StarsU16);
typedef void (*StarsRelationsLogChange)(void *, StarsI16);
typedef void (*StarsRelationsInvalidate)(void *, StarsU16);
typedef void (*StarsRelationsSetRadio)(void *, StarsU16, StarsU16, StarsU16);
typedef void (*StarsRelationsAddName)(void *, StarsI16);
typedef void (*StarsRelationsSelectRow)(void *, StarsI16);
typedef StarsI16 (*StarsRelationsGetSelection)(void *);
typedef void (*StarsRelationsEndDialog)(void *, StarsI16);
typedef void (*StarsRelationsHelp)(void *, StarsU16);
typedef struct StarsRelationsOps {
    StarsRelationsSticky sticky_dialog;
    StarsRelationsPaint paint;
    StarsRelationsFillBackground fill_background;
    StarsRelationsLogChange log_change;
    StarsRelationsInvalidate invalidate_scanner;
    StarsRelationsSetRadio set_radio;
    StarsRelationsAddName add_name;
    StarsRelationsSelectRow select_row;
    StarsRelationsGetSelection get_selection;
    StarsRelationsEndDialog end_dialog;
    StarsRelationsHelp help;
} StarsRelationsOps;
typedef struct StarsRelationsInput {
    StarsU16 hwnd;
    StarsI16 player;
    StarsI16 player_count;
    StarsU8 *relation_values;
    StarsU16 scanner_window;
    StarsI16 scan_group;
    StarsU16 list_control;
    StarsU16 background_brush;
    StarsU16 help_file;
    StarsU16 dirty;
    const StarsRelationsOps *ops;
    void *context;
} StarsRelationsInput;
StarsI16 stars_relations_dialog(StarsU16, StarsU16, StarsU16, StarsI32,
    StarsRelationsInput *);
/* Host representation of the cargo-bearing portions of planet, packet, and
 * fleet records.  A planet or fleet uses minerals; a packet uses cargo and
 * thing_capacity.  The original routine resolves a null object through a
 * lookup and commits it through store. */
typedef struct StarsCargoState {
    StarsU32 minerals[5];
    StarsU16 cargo[3];
    StarsU16 thing_capacity;
    StarsU16 fleet_flags;
    StarsU16 owner;
    StarsU16 ship_counts[16];
} StarsCargoState;
typedef StarsCargoState *(*StarsLookupCargo)(void *, StarsI16, StarsU16);
typedef void (*StarsStoreCargo)(void *, StarsI16, StarsU16,
    const StarsCargoState *);
typedef StarsU32 (*StarsCargoFree)(void *, const StarsCargoState *);
typedef struct StarsChgCargoOps {
    StarsLookupCargo lookup;
    StarsStoreCargo store;
    StarsCargoFree cargo_free;
    StarsCargoFree fuel_free;
} StarsChgCargoOps;
StarsI32 stars_change_cargo(StarsI16, StarsU16, StarsI16, StarsI32,
    StarsCargoState *, const StarsChgCargoOps *, void *);
typedef struct StarsTransferButton {
    StarsI16 supply;
    StarsU16 flags;
    StarsU16 button_state;
    StarsRect rect;
} StarsTransferButton;
typedef StarsI32 (*StarsTransferAvailability)(void *, StarsU16, StarsI16,
    StarsI16);
void stars_update_xfer_buttons(StarsI16, StarsI16, StarsTransferButton *,
    StarsU16, StarsTransferAvailability, void *);
typedef StarsU16 (*StarsAcquireDC)(void *, StarsU16);
typedef void (*StarsReleaseDC)(void *, StarsU16, StarsU16);
typedef void (*StarsPaintTransfer)(void *, StarsU16, StarsU16, StarsU16,
    StarsU16, StarsU16, StarsU16);
typedef void (*StarsDrawTransferButton)(void *, StarsU16, const StarsRect *,
    StarsU16);
typedef void (*StarsDrawTransferSide)(void *, StarsU16, const StarsRect *,
    const StarsCargoState *, StarsI16);
typedef struct StarsTransferSide {
    StarsI16 group;
    const StarsCargoState *state;
} StarsTransferSide;
typedef struct StarsDrawXferOps {
    StarsAcquireDC acquire_dc;
    StarsReleaseDC release_dc;
    StarsPaintTransfer paint_transfer;
    StarsDrawTransferButton draw_button;
    StarsDrawTransferSide draw_ships;
    StarsDrawTransferSide draw_cargo;
    StarsDrawTransferSide draw_planet;
    StarsDrawTransferSide draw_thing;
} StarsDrawXferOps;
void stars_draw_xfer_dlg(StarsU16, StarsU16, const StarsRect *, StarsI16,
    StarsI16, StarsI16, const StarsTransferButton *, StarsU16,
    const StarsTransferSide *, const StarsDrawXferOps *, void *);
StarsI16 stars_setup_xfer_buttons(const StarsRect *, StarsI16, StarsI16,
    StarsI16, const StarsI16 *, const StarsU16 *, StarsTransferButton *,
    StarsU16, StarsTransferAvailability, void *);
typedef const char *(*StarsThingLabel)(void *, StarsI16);
typedef void (*StarsTransferColor)(void *, StarsU16, StarsU16);
typedef void (*StarsTransferFont)(void *, StarsU16, StarsU16);
typedef void (*StarsTransferFrame)(void *, StarsU16, const StarsRect *, StarsU16);
typedef void (*StarsTransferCenteredText)(void *, StarsU16, const StarsRect *,
    const char *, StarsI16);
typedef void (*StarsTransferRightText)(void *, StarsU16, StarsI16, StarsI16,
    const char *, StarsI16, StarsI16);
typedef void (*StarsTransferGauge)(void *, StarsU16, const StarsRect *,
    const StarsCargoState *, StarsI16);
typedef struct StarsDrawThingXferOps {
    StarsThingLabel thing_name;
    StarsThingLabel label;
    StarsTransferColor set_text_color;
    StarsTransferColor set_background_color;
    StarsTransferFont select_font;
    StarsTransferFrame draw_frame;
    StarsTransferCenteredText centered_text;
    StarsTransferRightText right_text;
    StarsTransferGauge draw_gauge;
} StarsDrawThingXferOps;
void stars_draw_thing_xfer_side(StarsU16, const StarsRect *,
    const StarsCargoState *, StarsU16, StarsI16, StarsI16,
    StarsU16, StarsU16, const StarsDrawThingXferOps *, void *);
typedef void (*StarsTransferMineral)(void *, StarsU16, const StarsRect *,
    const StarsCargoState *, StarsI16, StarsI16);
typedef struct StarsDrawFleetCargoXferOps {
    StarsThingLabel fleet_name;
    StarsThingLabel label;
    StarsTransferColor set_text_color;
    StarsTransferColor set_background_color;
    StarsTransferFont select_font;
    StarsTransferFrame draw_frame;
    StarsTransferCenteredText centered_text;
    StarsTransferRightText right_text;
    StarsTransferGauge draw_gauge;
    StarsTransferMineral draw_mineral;
} StarsDrawFleetCargoXferOps;
void stars_draw_fleet_cargo_xfer_side(StarsU16, const StarsRect *,
    const StarsCargoState *, StarsU16, StarsU16, StarsI16, StarsI16,
    StarsU16, StarsU16, const StarsDrawFleetCargoXferOps *, void *);
typedef const char *(*StarsFormatShipCount)(void *, StarsU16);
typedef struct StarsDrawFleetShipsXferOps {
    StarsThingLabel fleet_name;
    StarsThingLabel ship_label;
    StarsTransferColor set_text_color;
    StarsTransferColor set_background_color;
    StarsTransferFont select_font;
    StarsTransferFrame draw_frame;
    StarsTransferCenteredText centered_text;
    StarsTransferRightText right_text;
    StarsFormatShipCount format_count;
} StarsDrawFleetShipsXferOps;
void stars_draw_fleet_ships_xfer_side(StarsU16, const StarsRect *,
    const StarsCargoState *, StarsU16, const StarsU16 *, StarsU16, StarsI16,
    StarsI16, StarsU16, StarsU16, const StarsDrawFleetShipsXferOps *, void *);
typedef void (*StarsTransferMineralValue)(void *, StarsU16, const StarsRect *,
    StarsI32, StarsI16);
typedef struct StarsDrawPlanetXferOps {
    StarsThingLabel planet_name;
    StarsThingLabel unknown_name;
    StarsThingLabel label;
    StarsThingLabel format_value;
    StarsTransferColor set_text_color;
    StarsTransferColor set_background_color;
    StarsTransferFont select_font;
    StarsTransferFrame draw_frame;
    StarsTransferCenteredText centered_text;
    StarsTransferRightText right_text;
    StarsTransferMineralValue draw_value;
} StarsDrawPlanetXferOps;
void stars_draw_planet_xfer_side(StarsU16, const StarsRect *,
    const StarsCargoState *, StarsI16, StarsI16, StarsI16, StarsU16, StarsU16,
    StarsU16, const StarsDrawPlanetXferOps *, void *);
void stars_get_xfer_left_right_rects(const StarsRect *, StarsRect *,
    StarsRect *, StarsI16);
StarsI16 stars_is_popup_hull_type(StarsU16, const StarsU8 *, StarsU16,
    const StarsU16 *);
typedef void (*StarsFrameRect)(void *, StarsU16, const StarsRect *, StarsU16);
void stars_handle_focus_state(StarsU16, StarsU16, const StarsRect *, StarsU16,
    StarsI16, StarsFrameRect, void *);
typedef void (*StarsEnableWindow)(void *, StarsU16, StarsU16);
void stars_enable_zip_buttons(StarsU16, StarsI16, const StarsU8 *,
    StarsGetDialogItem, StarsEnableWindow, void *);
typedef void (*StarsRandomEvent)(void *);
void stars_random_events(StarsRandomEvent, StarsRandomEvent,
    StarsRandomEvent, StarsRandomEvent, void *);
StarsI16 stars_enum_our_starbase(const StarsU8 *, StarsU16);
StarsI16 stars_enum_calc_enemy_fleet(const StarsU8 *, StarsU16);
StarsI16 stars_enum_calc_enemy_planet(const StarsU8 *, StarsU16);
StarsI16 stars_empty_bmp_from_grhst(const StarsU8 *, StarsI16);
StarsI16 stars_check_research(StarsU8 *, StarsI16, StarsI16, StarsI16);
typedef void (*StarsChangeMainObject)(void *, StarsI16, StarsU16);
typedef void (*StarsLookupFleet)(void *, StarsI16, StarsU8 *);
void stars_clear_ai_current_task(StarsU8 *, const StarsU8 *, StarsI16,
    StarsChangeMainObject, StarsLookupFleet, void *);
typedef StarsI16 (*StarsGetString)(void *, StarsU16, StarsU8 *);
typedef StarsI16 (*StarsChooseSaveFile)(void *, const StarsU8 *,
    const StarsU8 *, StarsU8 *, StarsU16, StarsU16 *);
typedef struct StarsGetNewGameNameOps {
    StarsGetString get_string;
    StarsChooseSaveFile choose_file;
} StarsGetNewGameNameOps;
StarsI16 stars_get_new_game_name(StarsU8 *, StarsU16,
    const StarsGetNewGameNameOps *, void *);
typedef StarsI16 (*StarsRaceDialogHitTest)(void *, StarsPoint);
typedef void (*StarsInitButtonTrack)(void *, StarsU16, StarsU16, StarsU16,
    StarsU16, StarsU16);
typedef StarsI16 (*StarsTrackButton)(void *, StarsU16, StarsU16 *);
typedef StarsI16 (*StarsGetVcValue)(void *, StarsI16, StarsI16);
typedef StarsI16 (*StarsSetVcValue)(void *, StarsI16, StarsI16);
typedef void (*StarsDrawNewGame3)(void *, StarsU16, StarsU16, StarsI16);
typedef struct StarsTrackNewGameDlg3Ops {
    StarsRaceDialogHitTest hit_test;
    StarsInitButtonTrack init_track;
    StarsTrackButton track_button;
    StarsGetVcValue get_value;
    StarsSetVcValue set_value;
    StarsDrawNewGame3 draw;
} StarsTrackNewGameDlg3Ops;
StarsI16 stars_track_new_game_dlg3(StarsU16, StarsPoint, StarsI16,
    const StarsTrackNewGameDlg3Ops *, void *);
typedef StarsU16 (*StarsNewGameDlg3BeginPaint)(void *, StarsU16);
typedef void (*StarsNewGameDlg3EndPaint)(void *, StarsU16, StarsU16);
typedef void (*StarsNewGameDlg3Draw)(void *, StarsU16, StarsU16, StarsI16);
typedef void (*StarsNewGameDlg3GetClientRect)(void *, StarsU16, StarsRect *);
typedef void (*StarsNewGameDlg3FillRect)(void *, StarsU16, const StarsRect *,
    StarsU16);
typedef StarsU16 (*StarsNewGameDlg3GetDialogItem)(void *, StarsU16, StarsU16);
typedef void (*StarsNewGameDlg3SetBackground)(void *, StarsU16, StarsU32);
typedef void (*StarsNewGameDlg3GetCursorPos)(void *, StarsPoint *);
typedef void (*StarsNewGameDlg3ScreenToClient)(void *, StarsU16, StarsPoint *);
typedef StarsI16 (*StarsNewGameDlg3HitTest)(void *, StarsPoint);
typedef void (*StarsNewGameDlg3SetCursor)(void *, StarsU16);
typedef void (*StarsNewGameDlg3SetTitle)(void *, StarsU16, StarsI16);
typedef StarsI16 (*StarsNewGameDlg3GetVCCheck)(void *, StarsI16);
typedef void (*StarsNewGameDlg3EnableWindow)(void *, StarsU16, StarsU16);
typedef StarsU32 (*StarsNewGameDlg3SendMessage)(void *, StarsU16, StarsU16,
    StarsU16, StarsU32);
typedef StarsI16 (*StarsNewGameDlg3GetButtonChecked)(void *, StarsU16,
    StarsU16);
typedef void (*StarsNewGameDlg3SetVCCheck)(void *, StarsI16, StarsI16);
typedef void (*StarsNewGameDlg3StickyPosition)(void *, StarsU16, StarsPoint *,
    StarsI16);
typedef void (*StarsNewGameDlg3EndDialog)(void *, StarsU16, StarsI16);
typedef void (*StarsNewGameDlg3WinHelp)(void *, StarsU16, const StarsU8 *,
    StarsU16, StarsU16);
typedef StarsI16 (*StarsNewGameDlg3Track)(void *, StarsU16, StarsPoint,
    StarsI16);
typedef struct StarsNewGameDlg3Ops {
    StarsNewGameDlg3BeginPaint begin_paint;
    StarsNewGameDlg3EndPaint end_paint;
    StarsNewGameDlg3Draw draw;
    StarsNewGameDlg3GetClientRect get_client_rect;
    StarsNewGameDlg3FillRect fill_rect;
    StarsNewGameDlg3GetDialogItem get_dialog_item;
    StarsNewGameDlg3SetBackground set_background_color;
    StarsNewGameDlg3GetCursorPos get_cursor_pos;
    StarsNewGameDlg3ScreenToClient screen_to_client;
    StarsNewGameDlg3HitTest hit_test;
    StarsNewGameDlg3SetCursor set_cursor;
    StarsNewGameDlg3SetTitle set_title;
    StarsNewGameDlg3GetVCCheck get_vc_check;
    StarsNewGameDlg3EnableWindow enable_window;
    StarsNewGameDlg3SendMessage send_message;
    StarsNewGameDlg3GetButtonChecked is_button_checked;
    StarsNewGameDlg3SetVCCheck set_vc_check;
    StarsNewGameDlg3StickyPosition sticky_position;
    StarsNewGameDlg3EndDialog end_dialog;
    StarsNewGameDlg3WinHelp win_help;
    StarsNewGameDlg3Track track;
} StarsNewGameDlg3Ops;
typedef struct StarsNewGameDlg3Input {
    StarsU16 button_face_brush;
    StarsU32 button_face_color;
    StarsU16 hand_cursor;
    const StarsU8 *help_file;
    StarsPoint *sticky_point;
    const StarsU16 *race_button_ids;
    StarsU16 race_button_count;
    StarsU16 read_only;
    const StarsNewGameDlg3Ops *ops;
    void *context;
} StarsNewGameDlg3Input;
StarsI16 stars_new_game_dlg3(StarsU16, StarsU16, StarsU16, StarsU32,
    StarsNewGameDlg3Input *);
typedef StarsU16 (*StarsDrawNewGame3GetDc)(void *, StarsU16);
typedef void (*StarsDrawNewGame3ReleaseDc)(void *, StarsU16, StarsU16);
typedef StarsU16 (*StarsDrawNewGame3SetBkMode)(void *, StarsU16, StarsU16);
typedef StarsU32 (*StarsDrawNewGame3SetBkColor)(void *, StarsU16, StarsU32);
typedef void (*StarsDrawNewGame3GetRect)(void *, StarsU16, StarsRect *);
typedef void (*StarsDrawNewGame3MapRect)(void *, StarsU16, StarsRect *);
typedef void (*StarsDrawNewGame3SelectFont)(void *, StarsU16, StarsU16);
typedef StarsI16 (*StarsDrawNewGame3Extent)(void *, StarsU16, const StarsU8 *,
    StarsI16);
typedef StarsI16 (*StarsDrawNewGame3String)(void *, StarsU16, StarsU8 *,
    StarsU16);
typedef StarsI16 (*StarsDrawNewGame3Value)(void *, StarsI16, StarsI16);
typedef StarsI16 (*StarsDrawNewGame3Format)(void *, StarsU8 *, StarsU16,
    StarsI16);
typedef void (*StarsDrawNewGame3Text)(void *, StarsU16, StarsI16, StarsI16,
    const StarsU8 *, StarsI16);
typedef void (*StarsDrawNewGame3Right)(void *, StarsU16, StarsI16, StarsI16,
    const StarsU8 *, StarsI16, StarsI16);
typedef void (*StarsDrawNewGame3Button)(void *, StarsU16, const StarsRect *,
    StarsU16);
typedef struct StarsDrawNewGame3Ops {
    StarsDrawNewGame3GetDc get_dc;
    StarsDrawNewGame3ReleaseDc release_dc;
    StarsDrawNewGame3SetBkMode set_bk_mode;
    StarsDrawNewGame3SetBkColor set_bk_color;
    StarsDrawNewGame3GetRect get_client_rect;
    StarsDrawNewGame3GetRect get_window_rect;
    StarsDrawNewGame3MapRect map_window_rect;
    StarsDrawNewGame3SelectFont select_font;
    StarsDrawNewGame3Extent text_extent;
    StarsDrawNewGame3String get_string;
    StarsDrawNewGame3Value get_value;
    StarsDrawNewGame3Format format_value;
    StarsDrawNewGame3Text text_out;
    StarsDrawNewGame3Right right_text;
    StarsDrawNewGame3Button draw_button;
    void *context;
} StarsDrawNewGame3Ops;
typedef struct StarsDrawNewGame3Input {
    StarsU16 hwnd;
    StarsU16 hdc;
    StarsI16 line_height;
    StarsI16 read_only;
    StarsU16 bold_font;
    StarsU16 normal_font;
    StarsU16 button_style;
    StarsU32 button_face_color;
    signed char column_widths[9][2];
    /* The original stores this as an 18-byte signed table. */
    const signed char *packed_column_widths;
    StarsU16 packed_column_width_count;
    StarsRect *reference_rects;
    StarsI16 *reference_count;
    const StarsDrawNewGame3Ops *ops;
} StarsDrawNewGame3Input;
void stars_draw_new_game3(StarsDrawNewGame3Input *, StarsI16);
typedef struct StarsDrawNewGame2Player {
    StarsU8 type;
    StarsI16 active;
    StarsI16 dead;
    StarsI16 race;
    StarsU8 name[32];
    StarsU8 file_name[16];
} StarsDrawNewGame2Player;
typedef StarsU16 (*StarsDrawNewGame2GetDc)(void *, StarsU16);
typedef void (*StarsDrawNewGame2ReleaseDc)(void *, StarsU16, StarsU16);
typedef void (*StarsDrawNewGame2SelectFont)(void *, StarsU16, StarsU16);
typedef StarsU16 (*StarsDrawNewGame2SetBkMode)(void *, StarsU16, StarsU16);
typedef StarsI16 (*StarsDrawNewGame2Extent)(void *, StarsU16, const StarsU8 *,
    StarsI16);
typedef StarsI16 (*StarsDrawNewGame2String)(void *, StarsU16, StarsU8 *,
    StarsU16);
typedef const StarsU8 *(*StarsDrawNewGame2Compressed)(void *, StarsU16);
typedef const StarsU8 *(*StarsDrawNewGame2AiLabel)(void *, StarsU16);
typedef StarsI16 (*StarsDrawNewGame2Format)(void *, StarsU8 *, StarsU16,
    StarsU16, const StarsU8 *, const StarsU8 *);
typedef void (*StarsDrawNewGame2Text)(void *, StarsU16, StarsI16, StarsI16,
    const StarsU8 *, StarsI16);
typedef void (*StarsDrawNewGame2Right)(void *, StarsU16, StarsI16, StarsI16,
    const StarsU8 *, StarsI16, StarsI16);
typedef void (*StarsDrawNewGame2Color)(void *, StarsU16, StarsU32);
typedef void (*StarsDrawNewGame2Diamond)(void *, StarsU16, const StarsRect *);
typedef struct StarsDrawNewGame2Ops {
    StarsDrawNewGame2GetDc get_dc;
    StarsDrawNewGame2ReleaseDc release_dc;
    StarsDrawNewGame2SelectFont select_font;
    StarsDrawNewGame2SetBkMode set_bk_mode;
    StarsDrawNewGame2Extent text_extent;
    StarsDrawNewGame2String get_string;
    StarsDrawNewGame2Compressed compressed_string;
    StarsDrawNewGame2AiLabel ai_group_label;
    StarsDrawNewGame2AiLabel ai_level_label;
    StarsDrawNewGame2Format format_text;
    StarsDrawNewGame2Text text_out;
    StarsDrawNewGame2Right right_text;
    StarsDrawNewGame2Color set_text_color;
    StarsDrawNewGame2Diamond draw_diamond;
    void *context;
} StarsDrawNewGame2Ops;
typedef struct StarsDrawNewGame2Input {
    StarsU16 hwnd;
    StarsU16 hdc;
    StarsI16 line_height;
    StarsI16 read_only;
    StarsI16 player_count;
    StarsU32 game_flags;
    StarsU32 text_color;
    StarsU32 dead_color;
    StarsU16 bold_font;
    StarsU16 blue_brush;
    StarsI16 diamond_x;
    StarsDrawNewGame2Player *players;
    /* The original keeps the editable page in packed parallel tables: one
     * byte per row for the player kind/race and thirteen bytes per saved race
     * filename.  These optional views let the renderer consume those tables
     * directly instead of requiring a host-sized shadow record. */
    const StarsU8 *packed_player_types;
    StarsU16 packed_player_type_count;
    const StarsU8 *packed_race_files;
    StarsU16 packed_race_file_count;
    StarsU16 packed_race_file_stride;
    const StarsDrawNewGame2Ops *ops;
} StarsDrawNewGame2Input;
void stars_draw_new_game2(StarsDrawNewGame2Input *, StarsI16);

/* Typed view of KillUsedWaypoints (NE 11:189A).  The original walks packed
 * 18-byte PLORD records; these fields retain the observable order semantics
 * while keeping far-pointer lookup and deletion in the caller-owned model. */
#define STARS_KILL_WP_MAX_ORDERS 32
typedef struct StarsKillUsedWaypointOrder {
    StarsI16 x;
    StarsI16 y;
    StarsI16 target_id;
    StarsU16 flags;
    StarsU8 payload[10];
} StarsKillUsedWaypointOrder;
typedef struct StarsKillUsedWaypointFleet {
    StarsI16 id;
    StarsI16 owner;
    StarsI16 x;
    StarsI16 y;
    StarsI16 cord;
    StarsI16 status_flags;
    StarsI16 id_planet;
    StarsKillUsedWaypointOrder *orders;
} StarsKillUsedWaypointFleet;
typedef struct StarsKillUsedWaypointPlanet {
    StarsI16 owner;
    StarsU16 status_flags;
} StarsKillUsedWaypointPlanet;
typedef StarsKillUsedWaypointFleet *(*StarsKillUsedWaypointFindFleet)(void *, StarsI16);
typedef StarsKillUsedWaypointPlanet *(*StarsKillUsedWaypointFindPlanet)(void *, StarsI16);
typedef void (*StarsKillUsedWaypointDelete)(void *, StarsKillUsedWaypointFleet *,
    StarsI16, StarsI16);
typedef void (*StarsKillUsedWaypointMessage)(void *, StarsI16, StarsI16,
    StarsI16);
/* FSendPlrMsg also receives the fleet status word used by this procedure.
 * Keep the legacy callback for small callers, but expose the full packed
 * message boundary when a probe or integration supplies it. */
typedef void (*StarsKillUsedWaypointMessageEx)(void *, StarsI16, StarsI16,
    StarsU16, StarsI16);
typedef struct StarsKillUsedWaypointOps {
    StarsKillUsedWaypointFindFleet find_fleet;
    StarsKillUsedWaypointFindPlanet find_planet;
    StarsKillUsedWaypointDelete delete_waypoint;
    StarsKillUsedWaypointMessage send_message;
    StarsKillUsedWaypointMessageEx send_message_ex;
    void *context;
} StarsKillUsedWaypointOps;
typedef struct StarsKillUsedWaypointsInput {
    StarsKillUsedWaypointFleet *fleets;
    StarsU16 fleet_count;
    const StarsKillUsedWaypointOps *ops;
} StarsKillUsedWaypointsInput;
void stars_kill_used_waypoints(StarsKillUsedWaypointsInput *);

/* Packed live-table adapter for KillUsedWaypoints.  The original rglpfl is a
 * far-pointer table and each _fleet carries a far lpplord at offset 100.  A
 * host/native fixture represents those pointers as 1-based record references:
 * a four-byte table entry has record-index+1 in its low word and a nonzero
 * segment surrogate in its high word; a zero pair is the original null
 * terminator.  This keeps the enumeration boundary explicit without making
 * the reconstructed source depend on a host pointer size. */
#define STARS_KILL_WP_PACKED_FLEET_SIZE 124U
#define STARS_KILL_WP_PACKED_FLEET_REF_SIZE 4U
#define STARS_KILL_WP_PACKED_PLORD_HEADER_SIZE 4U
#define STARS_KILL_WP_PACKED_ORDER_SIZE 18U
#define STARS_KILL_WP_PACKED_ORDER_COUNT 12U
#define STARS_KILL_WP_PACKED_PLORD_SIZE \
    (STARS_KILL_WP_PACKED_PLORD_HEADER_SIZE + \
     STARS_KILL_WP_PACKED_ORDER_SIZE * STARS_KILL_WP_PACKED_ORDER_COUNT)
typedef struct StarsKillUsedWaypointsPackedInput {
    StarsU8 *fleet_table;
    StarsU16 fleet_table_count;
    StarsU8 *fleet_records;
    StarsU16 fleet_record_count;
    StarsU8 *plord_records;
    StarsU16 plord_record_count;
    StarsKillUsedWaypointFleet *fleet_views;
    StarsKillUsedWaypointOrder *order_views;
    StarsU16 workspace_count;
    const StarsKillUsedWaypointOps *ops;
    StarsU16 enumerated_count;
    StarsU16 malformed;
} StarsKillUsedWaypointsPackedInput;
void stars_kill_used_waypoints_packed(StarsKillUsedWaypointsPackedInput *);

/* Typed fuel-planning view of LFuelUseToWaypoint (NE 0B:A9F4). */
typedef struct StarsFuelWaypointOrder {
    StarsI16 x;
    StarsI16 y;
    StarsI16 target_id;
    StarsU16 flags;
} StarsFuelWaypointOrder;
typedef struct StarsFuelWaypointFleet {
    StarsI16 owner;
    StarsI32 cargo[16];
    StarsFuelWaypointOrder *orders;
} StarsFuelWaypointFleet;
typedef StarsI32 (*StarsFuelWaypointDistance)(void *, StarsI16, StarsI16,
    StarsI16, StarsI16);
typedef StarsI32 (*StarsFuelWaypointEstimate)(void *,
    const StarsFuelWaypointFleet *, StarsI16, StarsI16, StarsI32, StarsI16);
typedef StarsI32 (*StarsFuelWaypointRamScoop)(void *,
    const StarsFuelWaypointFleet *, StarsI16, StarsI32);
typedef StarsI16 (*StarsFuelWaypointCargoHull)(void *,
    const StarsFuelWaypointFleet *, StarsI16);
typedef StarsI16 (*StarsFuelWaypointPlanet)(void *, StarsI16, StarsI16 *,
    StarsU16 *, StarsI16 *);
typedef struct StarsFuelWaypointOps {
    StarsFuelWaypointDistance distance;
    StarsFuelWaypointEstimate estimate_fuel;
    StarsFuelWaypointRamScoop ram_scoop_gain;
    StarsFuelWaypointCargoHull cargo_hull_type;
    StarsFuelWaypointPlanet planet_info;
    void *context;
    /* Original DGetDistance is floating point; integer callback retained
     * for callers whose distances are exactly integral. */
    double (*precise_distance)(void *, StarsI16, StarsI16, StarsI16, StarsI16);
} StarsFuelWaypointOps;
typedef struct StarsFuelUseToWaypointInput {
    const StarsFuelWaypointFleet *fleet;
    StarsI16 waypoint_count;
    StarsI16 max_cargo;
    const StarsFuelWaypointOps *ops;
} StarsFuelUseToWaypointInput;
StarsI32 stars_fuel_use_to_waypoint(StarsFuelUseToWaypointInput *);

/* Typed drawing/window view of DrawPlanShipBitmap (NE 0A:3336). */
typedef StarsI16 (*StarsPlanShipBitmapName)(void *, StarsI16, StarsU8 *,
    StarsU16);
typedef StarsI16 (*StarsPlanShipBitmapTile)(void *, StarsU16,
    StarsI16, StarsI16, StarsRect *, const StarsU8 *);
typedef void (*StarsPlanShipBitmapShow)(void *, StarsU16, StarsI16);
typedef void (*StarsPlanShipBitmapPosition)(void *, StarsU16, StarsU16,
    StarsI16, StarsI16, StarsI16, StarsI16, StarsU16);
typedef StarsU16 (*StarsPlanShipBitmapObject)(void *, StarsU16, StarsU16);
/* The raw DrawFleetBitmap call has a recovered positional tail after the
 * fleet identity.  Keep those arguments explicit until the callee's full
 * prototype is recovered; the legacy draw_fleet callback remains available
 * for callers that only need identity dispatch. */
typedef StarsU16 (*StarsPlanShipBitmapFleetEx)(void *, StarsU16, StarsI16,
    StarsI16, StarsI16, StarsI16, StarsI16, StarsI16, StarsI16, StarsI16,
    StarsI16);
typedef void (*StarsPlanShipBitmapPattern)(void *, StarsU16, StarsI16,
    StarsI16, StarsI16, StarsI16, StarsU32);
typedef void (*StarsPlanShipBitmapBlt)(void *, StarsU16, StarsI16, StarsI16,
    StarsI16, StarsI16, StarsU16, StarsI16, StarsI16, StarsI16, StarsI16,
    StarsU32);
typedef void (*StarsPlanShipBitmapPalette)(void *, StarsU16, StarsU16);
typedef struct StarsPlanShipBitmapOps {
    StarsPlanShipBitmapName planet_name;
    StarsPlanShipBitmapName fleet_name;
    StarsPlanShipBitmapTile draw_tile;
    StarsPlanShipBitmapShow show_window;
    StarsPlanShipBitmapPosition set_window_position;
    StarsPlanShipBitmapObject draw_fleet;
    StarsPlanShipBitmapFleetEx draw_fleet_ex;
    StarsPlanShipBitmapObject select_object;
    StarsPlanShipBitmapPattern pattern_blt;
    StarsPlanShipBitmapBlt dib_blt;
    StarsPlanShipBitmapPalette select_palette;
    StarsPlanShipBitmapPalette realize_palette;
    void *context;
} StarsPlanShipBitmapOps;
typedef struct StarsDrawPlanShipBitmapInput {
    StarsU16 hdc;
    StarsI16 object_kind;
    StarsI16 object_id;
    StarsI16 tile_flags;
    StarsI16 global_display_flags;
    StarsI16 arial8_height;
    StarsU16 button_handles[7];
    StarsU16 shadow_brush;
    StarsU16 hilite_brush;
    StarsU16 planet_dib;
    StarsU16 palette;
    /* Optional raw Win16 _sel view.  When present, the complete 226-byte
     * record supplies grobj/id at offsets 4/8 before the typed dispatch. */
    const StarsU8 *packed_selection_record;
    StarsU16 packed_selection_size;
    const StarsPlanShipBitmapOps *ops;
} StarsDrawPlanShipBitmapInput;
void stars_draw_plan_ship_bitmap(StarsDrawPlanShipBitmapInput *);

/* Typed route-planning view of AutoRouteFleet (NE 11:1E52). */
typedef struct StarsAutoRouteOrder {
    StarsI16 x;
    StarsI16 y;
    StarsI16 target_id;
    StarsU16 flags;
} StarsAutoRouteOrder;
typedef struct StarsAutoRouteFleet {
    StarsI16 owner;
    StarsI16 x;
    StarsI16 y;
    StarsI16 cord;
    StarsI32 fuel_capacity;
    StarsI32 mineral_weights[4];
    StarsI32 cargo[16];
    StarsI32 cargo_hull_capacity[16];
    StarsAutoRouteOrder *orders;
} StarsAutoRouteFleet;
typedef struct StarsAutoRoutePlanet {
    StarsI16 id;
    StarsI16 owner;
    StarsI16 x;
    StarsI16 y;
    StarsI16 route_target_id;
    StarsU16 status_flags;
    StarsI16 stargate;
    StarsI16 hull_cargo_max;
} StarsAutoRoutePlanet;
typedef StarsI16 (*StarsAutoRouteIdealWarp)(void *,
    const StarsAutoRouteFleet *, StarsI16);
typedef StarsI32 (*StarsAutoRouteDistance)(void *, StarsI16, StarsI16,
    StarsI16, StarsI16);
typedef StarsAutoRoutePlanet *(*StarsAutoRouteFindPlanet)(void *, StarsI16);
typedef StarsI16 (*StarsAutoRouteGateDamage)(void *, StarsI16, StarsI16,
    StarsI32, StarsI32, StarsI16 *);
typedef StarsI32 (*StarsAutoRouteEstimateFuel)(void *,
    const StarsAutoRouteFleet *, StarsI16, StarsI16, StarsI32, StarsI16);
typedef struct StarsAutoRouteOps {
    StarsAutoRouteIdealWarp ideal_warp;
    StarsAutoRouteDistance distance;
    StarsAutoRouteFindPlanet find_planet;
    StarsAutoRouteGateDamage gate_damage;
    StarsAutoRouteEstimateFuel estimate_fuel;
    void *context;
} StarsAutoRouteOps;
typedef struct StarsAutoRouteInput {
    StarsAutoRouteFleet *fleet;
    const StarsAutoRoutePlanet *source;
    const StarsAutoRouteOps *ops;
} StarsAutoRouteInput;
void stars_auto_route_fleet(StarsAutoRouteInput *);

/* Bounded typed dispatch view of DrawMineSurvey (NE 06:065A). */
typedef struct StarsDrawMineSurveyRect {
    StarsI16 left;
    StarsI16 top;
    StarsI16 right;
    StarsI16 bottom;
} StarsDrawMineSurveyRect;
typedef StarsU16 (*StarsDrawMineSurveyCreateDc)(void *, StarsU16);
typedef void (*StarsDrawMineSurveyDeleteDc)(void *, StarsU16);
typedef void (*StarsDrawMineSurveyColor)(void *, StarsU16, StarsU32);
typedef void (*StarsDrawMineSurveyMode)(void *, StarsU16, StarsU16);
typedef StarsU16 (*StarsDrawMineSurveyGetMode)(void *, StarsU16);
typedef StarsU32 (*StarsDrawMineSurveyGetColor)(void *, StarsU16);
typedef StarsI16 (*StarsDrawMineSurveyFleet)(void *, StarsU16,
    StarsDrawMineSurveyRect *, StarsI16);
typedef void (*StarsDrawMineSurveyFleetSummary)(void *, StarsU16,
    StarsDrawMineSurveyRect *, StarsI16, StarsU32, StarsI16, StarsI16);
typedef StarsI16 (*StarsDrawMineSurveyPlanet)(void *, StarsU16,
    StarsDrawMineSurveyRect *, StarsI16);
typedef StarsI16 (*StarsDrawMineSurveyLookupPlanet)(void *, StarsI16);
typedef StarsI16 (*StarsDrawMineSurveyThing)(void *, StarsU16,
    StarsDrawMineSurveyRect *, StarsI16);
typedef StarsI16 (*StarsDrawMineSurveyUnknown)(void *, StarsU16,
    StarsDrawMineSurveyRect *);
/* Decoded values consumed by the detailed planet and thing rows.  These are
 * deliberately semantic views of the packed records, not byte-replay
 * buffers: the implementation derives the owner/kind and wormhole flags from
 * the same recorded bit fields used by the original routine. */
typedef struct StarsDrawMineSurveyPlanetInfo {
    StarsI16 owner;
    StarsU16 survey_detail;
    StarsI16 survey_turn;
    StarsI16 current_turn;
    StarsI16 environment[3];
    StarsI16 original_environment[3];
    StarsI16 mineral_concentration[3];
    StarsI32 minerals[3];
} StarsDrawMineSurveyPlanetInfo;
typedef struct StarsDrawMineSurveyPlanetSummary {
    StarsI16 owner;
    StarsI16 owner_known;
    StarsI16 survey_detail;
    StarsI16 detailed_rows;
    StarsI16 turns_since_survey;
    StarsI16 environment[3];
    StarsI16 original_environment[3];
    StarsI16 mineral_concentration[3];
    StarsI32 minerals[3];
} StarsDrawMineSurveyPlanetSummary;
typedef void (*StarsDrawMineSurveyPlanetDetails)(void *, StarsU16,
    StarsDrawMineSurveyRect *, StarsI16,
    const StarsDrawMineSurveyPlanetSummary *);
typedef struct StarsDrawMineSurveyThingInfo {
    StarsU16 id_full;
    StarsPoint point;
    StarsU16 destination_word;
    StarsU16 mine_type;
    StarsU16 mine_count;
    StarsU16 amounts[3];
    StarsU32 visibility_mask;
} StarsDrawMineSurveyThingInfo;
typedef struct StarsDrawMineSurveyThingSummary {
    StarsI16 kind;
    StarsI16 id;
    StarsI16 owner;
    StarsPoint point;
    StarsI16 destination_known;
    StarsI16 destination_id;
    StarsI16 range_code;
    StarsI16 mine_type;
    StarsI16 mine_count;
    StarsU16 amounts[3];
    StarsU32 visibility_mask;
} StarsDrawMineSurveyThingSummary;
typedef void (*StarsDrawMineSurveyThingDetails)(void *, StarsU16,
    StarsDrawMineSurveyRect *, StarsI16,
    const StarsDrawMineSurveyThingSummary *);
/* Exact outer-panel PATBLT geometry recovered from DrawMineSurvey.  The
 * brush role is 0 for the button-shadow brush and 1 for the button-highlight
 * brush; the ROP is retained as a value because the original uses both the
 * frame raster operation and the interior fill operation. */
typedef struct StarsDrawMineSurveyPattern {
    StarsI16 x;
    StarsI16 y;
    StarsI16 width;
    StarsI16 height;
    StarsU32 rop;
    StarsI16 brush_role;
} StarsDrawMineSurveyPattern;
typedef void (*StarsDrawMineSurveyPatternBlt)(void *, StarsU16,
    const StarsDrawMineSurveyPattern *);
typedef struct StarsDrawMineSurveyOps {
    StarsDrawMineSurveyCreateDc create_compatible_dc;
    StarsDrawMineSurveyDeleteDc delete_dc;
    StarsDrawMineSurveyColor set_background;
    StarsDrawMineSurveyColor set_text_color;
    StarsDrawMineSurveyGetMode get_background_mode;
    StarsDrawMineSurveyGetColor get_background_color;
    StarsDrawMineSurveyGetColor get_text_color;
    StarsDrawMineSurveyMode set_background_mode;
    StarsDrawMineSurveyFleet draw_fleet_panel;
    StarsDrawMineSurveyFleetSummary draw_fleet_summary;
    StarsDrawMineSurveyPlanet draw_planet_panel;
    StarsDrawMineSurveyLookupPlanet lookup_planet;
    StarsDrawMineSurveyThing draw_thing_panel;
    StarsDrawMineSurveyUnknown draw_unknown_planet;
    StarsDrawMineSurveyPlanetDetails draw_planet_details;
    StarsDrawMineSurveyThingDetails draw_thing_details;
    StarsDrawMineSurveyPatternBlt draw_panel_pattern;
    void *context;
} StarsDrawMineSurveyOps;
typedef struct StarsDrawMineSurveyInput {
    StarsU16 hdc;
    StarsDrawMineSurveyRect rect;
    StarsI16 selected_kind;
    StarsI16 selected_full_kind;
    StarsI16 selected_fleet;
    StarsI16 selected_planet;
    StarsI16 selected_thing;
    const StarsU16 *fleet_ship_counts;
    StarsI16 fleet_status;
    StarsI16 fleet_cord;
    StarsU16 base_color;
    StarsU16 text_color;
    const StarsDrawMineSurveyPlanetInfo *planet_info;
    const StarsDrawMineSurveyThingInfo *thing_info;
    /* Optional recovered raw-record views.  These are the native Win16
     * storage boundaries: _fleet is 124 bytes, _planet 56 bytes, and _thing
     * 18 bytes.  Typed inputs remain valid for callers that already decoded
     * the records. */
    const StarsU8 *packed_fleet_record;
    StarsU16 packed_fleet_size;
    const StarsU8 *packed_planet_record;
    StarsU16 packed_planet_size;
    const StarsU8 *packed_thing_record;
    StarsU16 packed_thing_size;
    StarsI16 packed_survey_turn;
    StarsI16 packed_current_turn;
    const StarsDrawMineSurveyOps *ops;
} StarsDrawMineSurveyInput;
void stars_draw_mine_survey(StarsDrawMineSurveyInput *);

typedef StarsU16 (*StarsNewGameDlgBeginPaint)(void *, StarsU16, StarsRect *);
typedef void (*StarsNewGameDlgEndPaint)(void *, StarsU16, StarsRect *);
typedef void (*StarsNewGameDlgGetControlRect)(void *, StarsU16, StarsRect *);
typedef void (*StarsNewGameDlgMapPoints)(void *, StarsU16, StarsU16,
    StarsRect *, StarsU16);
typedef void (*StarsNewGameDlgExpandRect)(void *, StarsRect *, StarsI16,
    StarsI16);
typedef void (*StarsNewGameDlgDrawFrame)(void *, StarsU16, const StarsRect *,
    StarsI16);
typedef void (*StarsNewGameDlgSelectObject)(void *, StarsU16, StarsU16);
typedef void (*StarsNewGameDlgSetBkColor)(void *, StarsU16, StarsU32);
typedef void (*StarsNewGameDlgFillRect)(void *, StarsU16, const StarsRect *,
    StarsU16);
typedef StarsI16 (*StarsNewGameDlgGetString)(void *, StarsU16, StarsU8 *,
    StarsU16);
typedef void (*StarsNewGameDlgTextOut)(void *, StarsU16, StarsI16, StarsI16,
    const StarsU8 *, StarsI16);
typedef void (*StarsNewGameDlgSetTitle)(void *, StarsU16, StarsI16);
typedef void (*StarsNewGameDlgCheckRadio)(void *, StarsU16, StarsU16,
    StarsU16, StarsU16);
typedef void (*StarsNewGameDlgSetWindowText)(void *, StarsU16,
    const StarsU8 *);
typedef void (*StarsNewGameDlgSendMessage)(void *, StarsU16, StarsU16,
    StarsU16, StarsU32);
typedef void (*StarsNewGameDlgEnableWindow)(void *, StarsU16, StarsU16);
typedef StarsI16 (*StarsNewGameDlgButtonChecked)(void *, StarsU16, StarsU16);
typedef void (*StarsNewGameDlgGetText)(void *, StarsU16, StarsU16, StarsU8 *,
    StarsU16);
typedef StarsI16 (*StarsNewGameDlgRaceButton)(void *, StarsU16);
typedef void (*StarsNewGameDlgDefaultName)(void *, StarsU8 *, StarsU16);
typedef void (*StarsNewGameDlgSticky)(void *, StarsU16, StarsPoint *, StarsI16);
typedef void (*StarsNewGameDlgEndDialog)(void *, StarsU16, StarsI16);
typedef void (*StarsNewGameDlgWinHelp)(void *, StarsU16, const StarsU8 *,
    StarsU16);
typedef struct StarsNewGameDlgState {
    StarsI16 map_size;
    StarsI16 density;
    StarsI16 start_distance;
    StarsU16 options;
    StarsU8 game_name[32];
    StarsU16 race_button_ids[4];
    StarsU16 button_face_brush;
    StarsU32 button_face_color;
    StarsI16 arial8_height;
    StarsI16 read_only;
    StarsPoint sticky_position;
    StarsU16 help_id;
} StarsNewGameDlgState;
typedef struct StarsNewGameDlgResourceControl {
    StarsU16 id;
    StarsI16 x;
    StarsI16 y;
    StarsI16 width;
    StarsI16 height;
} StarsNewGameDlgResourceControl;
typedef struct StarsNewGameDlgResourceContract {
    StarsU16 resource_id;
    StarsI16 width;
    StarsI16 height;
    StarsI16 font_size;
    StarsI16 control_count;
    StarsNewGameDlgResourceControl controls[27];
} StarsNewGameDlgResourceContract;
void stars_new_game_dlg_resource_contract(
    StarsNewGameDlgResourceContract *);
typedef struct StarsNewGameDlgOps {
    StarsNewGameDlgBeginPaint begin_paint;
    StarsNewGameDlgEndPaint end_paint;
    StarsNewGameDlgGetControlRect get_control_rect;
    StarsNewGameDlgMapPoints map_points;
    StarsNewGameDlgExpandRect expand_rect;
    StarsNewGameDlgDrawFrame draw_frame;
    StarsNewGameDlgSelectObject select_object;
    StarsNewGameDlgSetBkColor set_background_color;
    StarsNewGameDlgFillRect fill_rect;
    StarsNewGameDlgGetString get_string;
    StarsNewGameDlgTextOut text_out;
    StarsNewGameDlgSetTitle set_title;
    StarsNewGameDlgCheckRadio check_radio_button;
    StarsGetDialogItem get_dialog_item;
    StarsNewGameDlgSetWindowText set_window_text;
    StarsNewGameDlgSendMessage send_message;
    StarsNewGameDlgEnableWindow enable_window;
    StarsNewGameDlgButtonChecked is_button_checked;
    StarsNewGameDlgGetText get_dialog_text;
    StarsNewGameDlgRaceButton race_button_index;
    StarsNewGameDlgDefaultName default_name;
    StarsNewGameDlgSticky sticky_position;
    StarsNewGameDlgEndDialog end_dialog;
    StarsNewGameDlgWinHelp win_help;
    void *context;
} StarsNewGameDlgOps;
typedef struct StarsNewGameDlgInput {
    StarsNewGameDlgState *state;
    /* Optional recovered 64-byte packed _game record. */
    StarsU8 *packed_game_record;
    const StarsNewGameDlgOps *ops;
    void *context;
} StarsNewGameDlgInput;
StarsI16 stars_new_game_dlg(StarsNewGameDlgInput *, StarsU16, StarsU16,
    StarsU16, StarsU32);

/* SimpleNewGameDlg is the compact map/setup dialog used before the full race
 * wizard.  It shares the ordinary dialog drawing/message services with
 * NewGameDlg but keeps the selected turn and race-template transitions
 * explicit. */
typedef StarsI16 (*StarsSimpleNewGameRaceWizard)(void *, StarsU16);
typedef void (*StarsSimpleNewGameRaceCopy)(void *, StarsI16);
typedef void (*StarsSimpleNewGameListReset)(void *, StarsU16);
typedef void (*StarsSimpleNewGameListString)(void *, StarsU16, StarsU16,
    const StarsU8 *);
typedef StarsI16 (*StarsSimpleNewGameListCount)(void *, StarsU16);
typedef StarsI16 (*StarsSimpleNewGameSelection)(void *, StarsU16);
typedef void (*StarsSimpleNewGameFocus)(void *, StarsU16);
typedef struct StarsSimpleNewGameState {
    StarsI16 density;
    StarsI16 map_size;
    StarsI16 turn;
    StarsPoint sticky_position;
    StarsU16 button_face_brush;
    StarsU32 button_face_color;
    StarsI16 arial8_height;
} StarsSimpleNewGameState;
typedef struct StarsSimpleNewGameResourceControl {
    StarsU16 id;
    StarsI16 x;
    StarsI16 y;
    StarsI16 width;
    StarsI16 height;
    StarsI16 text_length;
    StarsU16 text_checksum;
} StarsSimpleNewGameResourceControl;
typedef struct StarsSimpleNewGameResourceContract {
    StarsU16 resource_id;
    StarsI16 width;
    StarsI16 height;
    StarsI16 font_size;
    StarsI16 control_count;
    StarsSimpleNewGameResourceControl controls[16];
} StarsSimpleNewGameResourceContract;
void stars_simple_new_game_resource_contract(
    StarsSimpleNewGameResourceContract *);
typedef struct StarsSimpleNewGameInput {
    StarsSimpleNewGameState *state;
    const StarsNewGameDlgOps *ops;
    StarsSimpleNewGameRaceWizard race_wizard;
    StarsSimpleNewGameRaceCopy load_old_race;
    StarsSimpleNewGameRaceCopy load_new_race;
    StarsSimpleNewGameRaceCopy save_new_race;
    StarsSimpleNewGameListReset reset_list;
    StarsSimpleNewGameListString add_list_string;
    StarsSimpleNewGameListCount list_count;
    StarsSimpleNewGameSelection get_selection;
    StarsSimpleNewGameFocus set_focus;
    /* Optional original 0xc0-byte temporary player and destination array.
     * A successful custom-race wizard copies the complete temporary record
     * into the selected new-player slot. */
    StarsU8 *packed_player_record;
    StarsU8 *packed_player_records;
    StarsU16 packed_player_stride;
    void *context;
} StarsSimpleNewGameInput;
StarsI16 stars_simple_new_game_dlg(StarsSimpleNewGameInput *, StarsU16,
    StarsU16, StarsU16, StarsU32);

/* NewGameWizard keeps the Win16 dialog/resource layer at callback boundaries.
 * The state machine, player-type encoding, and relation-group repair remain
 * ordinary typed C. */
typedef struct StarsNewGameWizardPlayer {
    StarsI16 active;
    StarsI16 dead;
    StarsI16 race;
    StarsI16 ai_level;
    /* Randomized computer-race selection receives the raw all-ones salt. */
    StarsI32 salt;
    /* The five-bit relation-group value written into the packed player flags. */
    StarsU8 relation_group;
    /* When valid, the original packed player word is available: bit 9 is the
     * raw relation-exclusion marker and bits 3..7 hold the five-bit group. */
    StarsU16 packed_flags;
    StarsI16 packed_flags_valid;
    StarsU8 name[32];
    StarsU8 display_name[32];
} StarsNewGameWizardPlayer;
typedef StarsI16 (*StarsNewGameWizardDialog)(void *, StarsU16, StarsU16,
    StarsI16, StarsI16);
typedef StarsI16 (*StarsNewGameWizardGameName)(void *, StarsU8 *, StarsU16);
typedef void (*StarsNewGameWizardInitPlayers)(void *, StarsI16, StarsI16);
typedef void (*StarsNewGameWizardSimpleSetup)(void *, StarsI16);
typedef void (*StarsNewGameWizardCopyPlayer)(void *, StarsI16, StarsI16);
typedef void (*StarsNewGameWizardComputerPlayer)(void *, StarsI16, StarsI16,
    StarsI16, StarsI16);
typedef void (*StarsNewGameWizardFinalizePlayer)(void *, StarsI16,
    StarsU8, StarsI16, StarsI16);
typedef StarsI16 (*StarsNewGameWizardAdvantage)(void *, StarsI16);
typedef StarsI16 (*StarsNewGameWizardRandom)(void *, StarsI16);
typedef void (*StarsNewGameWizardPlayerName)(void *, StarsI16);
typedef void (*StarsNewGameWizardSetPlayerCount)(void *, StarsI16);
typedef void (*StarsNewGameWizardAssignRelations)(void *, StarsI16);
typedef void (*StarsNewGameWizardLifecycle)(void *);
typedef void (*StarsNewGameWizardSetBits)(void *, StarsU16);
typedef struct StarsNewGameWizardOps {
    StarsNewGameWizardDialog dialog;
    StarsNewGameWizardGameName get_game_name;
    StarsNewGameWizardInitPlayers init_players;
    StarsNewGameWizardSimpleSetup prepare_simple;
    StarsNewGameWizardCopyPlayer copy_player;
    StarsNewGameWizardComputerPlayer configure_computer;
    StarsNewGameWizardFinalizePlayer finalize_player;
    StarsNewGameWizardAdvantage advantage_points;
    StarsNewGameWizardRandom random_value;
    StarsNewGameWizardPlayerName ensure_player_name;
    StarsNewGameWizardPlayerName ensure_display_name;
    StarsNewGameWizardSetPlayerCount set_player_count;
    StarsNewGameWizardAssignRelations assign_relations;
    StarsNewGameWizardLifecycle save_destroy_restore;
    StarsNewGameWizardLifecycle close_title;
    StarsNewGameWizardLifecycle generate_world;
    StarsNewGameWizardLifecycle bring_up_host;
    StarsNewGameWizardLifecycle start_tutor;
    StarsNewGameWizardSetBits set_game_bits;
    void *context;
} StarsNewGameWizardOps;
typedef struct StarsNewGameWizardResourceControl {
    StarsU16 id;
    StarsI16 x;
    StarsI16 y;
    StarsI16 width;
    StarsI16 height;
    StarsI16 text_length;
    StarsU16 text_checksum;
} StarsNewGameWizardResourceControl;
typedef struct StarsNewGameWizardResourcePage {
    StarsU16 resource_id;
    StarsI16 width;
    StarsI16 height;
    StarsI16 font_size;
    StarsI16 control_count;
    StarsNewGameWizardResourceControl controls[27];
} StarsNewGameWizardResourcePage;
typedef struct StarsNewGameWizardResourceContract {
    StarsNewGameWizardResourcePage pages[3];
} StarsNewGameWizardResourceContract;
void stars_new_game_wizard_resource_contract(
    StarsNewGameWizardResourceContract *);
typedef struct StarsNewGameWizardInput {
    StarsU16 hwnd;
    StarsI16 read_only;
    StarsU16 saved_game_bits;
    StarsU16 *game_bits;
    StarsI16 current_player_count;
    const StarsNewGameWizardPlayer *current_players;
    StarsNewGameWizardPlayer *players;
    StarsU8 *player_types;
    StarsNewGameWizardPlayer *temporary_players;
    const StarsNewGameWizardPlayer *default_player;
    StarsI16 ai_level;
    StarsI16 selected_player;
    StarsI16 simple_dialog_result;
    StarsI16 simple_game_name_mode;
    StarsI16 game_player_count;
    StarsI16 host_player;
    /* When set, relation_group fields use the typed repair algorithm. */
    StarsI16 relation_groups_valid;
    StarsU8 game_name[256];
    /* Optional packed Win16 _player array.  The wizard writes only the
     * fields it owns and leaves the copied race/design bytes untouched. */
    StarsU8 *packed_players;
    StarsU16 packed_player_stride;
    const StarsNewGameWizardOps *ops;
} StarsNewGameWizardInput;
StarsI16 stars_new_game_wizard(StarsNewGameWizardInput *);
typedef void (*StarsNewGameDlg2Paint)(void *, StarsU16, StarsU16);
typedef void (*StarsNewGameDlg2Erase)(void *, StarsU16, StarsU16);
typedef void (*StarsNewGameDlg2Simple)(void *, StarsU16);
typedef StarsI16 (*StarsNewGameDlg2Popup)(void *, StarsU16, StarsU8,
    StarsI16, StarsI16, StarsI16);
typedef StarsI16 (*StarsNewGameDlg2RaceAction)(void *, StarsU16, StarsI16,
    StarsU8);
typedef StarsI16 (*StarsNewGameDlg2PasswordCheck)(void *, StarsI16, StarsU8);
typedef void (*StarsNewGameDlg2Help)(void *, StarsU16);
typedef void (*StarsNewGameDlg2Alert)(void *, StarsI16, StarsI16);
typedef void (*StarsNewGameDlg2Sticky)(void *, StarsU16, StarsI16);
typedef void (*StarsNewGameDlg2EndDialog)(void *, StarsU16, StarsI16);
typedef void (*StarsNewGameDlg2Invalidate)(void *, StarsU16);
typedef StarsU16 (*StarsNewGameDlg2GetDialogItem)(void *, StarsU16, StarsU16);
typedef void (*StarsNewGameDlg2SetBackground)(void *, StarsU16, StarsU32);
typedef struct StarsNewGameDlg2Ops {
    StarsNewGameDlg2Paint paint;
    StarsNewGameDlg2Erase erase_background;
    StarsNewGameDlg2Simple set_title_and_layout;
    StarsNewGameDlg2Simple set_cursor;
    StarsNewGameDlg2Popup track_popup;
    StarsNewGameDlg2RaceAction create_race;
    StarsNewGameDlg2RaceAction open_game;
    StarsNewGameDlg2RaceAction edit_race;
    StarsNewGameDlg2PasswordCheck check_password;
    StarsNewGameDlg2Help help;
    StarsNewGameDlg2Alert alert;
    StarsNewGameDlg2Sticky sticky_position;
    StarsNewGameDlg2EndDialog end_dialog;
    StarsNewGameDlg2Invalidate invalidate;
    StarsNewGameDlg2Simple set_focus;
    StarsNewGameDlg2GetDialogItem get_dialog_item;
    StarsNewGameDlg2SetBackground set_background_color;
    void *context;
} StarsNewGameDlg2Ops;
typedef struct StarsNewGameDlg2Input {
    StarsI16 read_only;
    StarsI16 x_diamond;
    StarsI16 diamond_width;
    StarsI16 row_top;
    StarsI16 row_height;
    StarsI16 row_height_active;
    StarsU8 *player_types;
    StarsI16 player_type_count;
    const StarsU16 *race_button_ids;
    StarsI16 race_button_count;
    StarsU16 button_face_brush;
    StarsU32 button_face_color;
    /* Optional recovered packed race state.  Successful Create/Open/Edit
     * callbacks populate the temporary buffers and this procedure commits
     * the selected 0xc0-byte player and 13-byte DOS race-file slots. */
    StarsU8 *packed_player_record;
    StarsU8 *packed_players;
    StarsU16 packed_player_stride;
    StarsU8 *race_file;
    StarsU8 *race_files;
    StarsU16 race_file_stride;
    const StarsNewGameDlg2Ops *ops;
} StarsNewGameDlg2Input;
StarsI16 stars_new_game_dlg2(StarsU16, StarsU16, StarsU16, StarsI32,
    StarsNewGameDlg2Input *);
typedef struct StarsNewGameDlg2ResourceControl {
    StarsU16 id;
    StarsI16 x;
    StarsI16 y;
    StarsI16 width;
    StarsI16 height;
    StarsI16 text_length;
    StarsU16 text_checksum;
} StarsNewGameDlg2ResourceControl;
typedef struct StarsNewGameDlg2ResourceContract {
    StarsU16 resource_id;
    StarsI16 width;
    StarsI16 height;
    StarsI16 font_size;
    StarsI16 control_count;
    StarsNewGameDlg2ResourceControl controls[5];
} StarsNewGameDlg2ResourceContract;
void stars_new_game_dlg2_resource_contract(
    StarsNewGameDlg2ResourceContract *);
typedef void (*StarsPlanetWndCreate)(void *, StarsU16);
typedef void (*StarsPlanetWndPaint)(void *, StarsU16, StarsU16);
typedef StarsI32 (*StarsPlanetWndColor)(void *, StarsU16, StarsU16,
    StarsI32);
typedef StarsU16 (*StarsPlanetWndCursor)(void *, StarsU16, StarsI16,
    StarsI16);
typedef void (*StarsPlanetWndSetCursor)(void *, StarsU16);
typedef void (*StarsPlanetWndDrawItem)(void *, StarsI32);
typedef void (*StarsPlanetWndMeasureItem)(void *, StarsI32, StarsI16);
/* PlanetWndProc passes the selected planet and combobox ordinal to the
 * FLookupOrbitingXfer boundary.  The callback owns the packed 124-byte fleet
 * / 18-byte thing scan and _xfer construction, then exposes only the _xfer
 * id and grobj fields consumed by TransferStuff or SelectAdjFleet. */
typedef StarsI16 (*StarsPlanetWndFindTransfer)(void *, StarsI16, StarsI16,
    StarsI16 *, StarsI16 *);
typedef StarsI16 (*StarsPlanetWndGetSelection)(void *);
typedef void (*StarsPlanetWndTransfer)(void *, StarsI16, StarsI16,
    StarsI16, StarsI16);
typedef void (*StarsPlanetWndSelect)(void *, StarsI16, StarsI16);
typedef StarsI16 (*StarsPlanetWndTrySelect)(void *, StarsI16, StarsI16);
typedef void (*StarsPlanetWndSimple)(void *);
typedef void (*StarsPlanetWndShipCommand)(void *, StarsU16, StarsU16,
    StarsI32);
typedef void (*StarsPlanetWndDropdown)(void *, StarsI16);
typedef void (*StarsPlanetWndBuildShip)(void *, StarsPoint);
typedef void (*StarsPlanetWndProduction)(void *, StarsI16);
typedef StarsI16 (*StarsPlanetWndConfirm)(void *);
typedef void (*StarsPlanetWndClick)(void *, StarsI16, StarsI16, StarsU16,
    StarsI16);
typedef void (*StarsPlanetWndSetActive)(void *, StarsU16);
typedef StarsU16 (*StarsPlanetWndBeginPaint)(void *, StarsU16, StarsRect *);
typedef void (*StarsPlanetWndEndPaint)(void *, StarsU16, StarsRect *);
typedef void (*StarsPlanetWndGetClientRect)(void *, StarsU16, StarsRect *);
typedef void (*StarsPlanetWndFillRect)(void *, StarsU16, const StarsRect *,
    StarsU16);
typedef void (*StarsPlanetWndGetCursorPos)(void *, StarsPoint *);
typedef void (*StarsPlanetWndScreenToClient)(void *, StarsU16, StarsPoint *);
typedef StarsI16 (*StarsPlanetWndPointInRect)(void *, const StarsRect *,
    StarsPoint);
typedef StarsU16 (*StarsPlanetWndOrderCursor)(void *, StarsPoint, StarsI16,
    StarsI16, StarsI16);
typedef void (*StarsPlanetWndSetCursorValue)(void *, StarsU16);
typedef void (*StarsPlanetWndSetMinimumSize)(void *, StarsI32, StarsI16,
    StarsI16);
typedef StarsI16 (*StarsPlanetWndGetDrawAction)(void *, StarsI32);
typedef void (*StarsPlanetWndDrawItemMode)(void *, StarsI32, StarsI16);
typedef struct StarsPlanetWndProcOps {
    StarsPlanetWndCreate create_controls;
    StarsPlanetWndPaint paint_planet_ship;
    StarsPlanetWndColor color_control;
    StarsPlanetWndCursor cursor_hit_test;
    StarsPlanetWndSetCursor set_cursor;
    StarsPlanetWndDrawItem draw_combo_item;
    StarsPlanetWndMeasureItem measure_combo_item;
    StarsPlanetWndSimple set_minimum_size;
    StarsPlanetWndSimple set_focus_frame;
    StarsPlanetWndShipCommand ship_command;
    StarsPlanetWndDropdown ship_dropdown_changed;
    StarsPlanetWndFindTransfer find_transfer;
    StarsPlanetWndGetSelection get_dropdown_selection;
    StarsPlanetWndTransfer transfer;
    StarsPlanetWndTrySelect try_select_adj_planet;
    StarsPlanetWndSelect select_adj_planet;
    StarsPlanetWndSelect select_adj_starbase;
    StarsPlanetWndBuildShip build_ship;
    StarsPlanetWndProduction change_production;
    StarsPlanetWndConfirm confirm_production;
    StarsPlanetWndClick planet_click;
    StarsPlanetWndSetActive set_active;
    StarsPlanetWndSimple advance_tutor;
    StarsPlanetWndSimple default_window_proc;
    StarsPlanetWndBeginPaint begin_paint;
    StarsPlanetWndEndPaint end_paint;
    StarsPlanetWndGetClientRect get_client_rect;
    StarsPlanetWndFillRect fill_rect;
    StarsPlanetWndGetCursorPos get_cursor_pos;
    StarsPlanetWndScreenToClient screen_to_client;
    StarsPlanetWndPointInRect point_in_rect;
    StarsPlanetWndOrderCursor click_ship_orders;
    StarsPlanetWndOrderCursor click_planet_orders;
    StarsPlanetWndSetCursorValue set_cursor_value;
    StarsPlanetWndSetMinimumSize set_minimum_size_exact;
    StarsPlanetWndGetDrawAction get_draw_item_action;
    StarsPlanetWndDrawItemMode draw_item_focus;
    StarsPlanetWndDrawItemMode draw_item_entire;
    void *context;
} StarsPlanetWndProcOps;
typedef StarsU16 (*StarsPlanetWndCreateWindow)(void *, StarsU16, StarsU16,
    const StarsU8 *, StarsU32, StarsI16, StarsI16, StarsI16, StarsI16,
    StarsU16);
typedef void (*StarsPlanetWndSetFont)(void *, StarsU16, StarsU16);
typedef void (*StarsPlanetWndSetEditLimit)(void *, StarsU16, StarsU16);
typedef const StarsU8 *(*StarsPlanetWndGetString)(void *, StarsU16);
typedef void (*StarsPlanetWndGetRect)(void *, StarsU16, StarsRect *);
typedef StarsI32 (*StarsPlanetWndGetLong)(void *, StarsU16, StarsI16);
typedef void (*StarsPlanetWndSetLong)(void *, StarsU16, StarsI16, StarsI32);
typedef struct StarsPlanetWndLayoutOps {
    StarsPlanetWndCreateWindow create_window;
    StarsPlanetWndSetFont set_font;
    StarsPlanetWndSetEditLimit set_edit_limit;
    StarsPlanetWndGetString get_compressed_string;
    StarsPlanetWndGetRect get_client_rect;
    StarsPlanetWndGetLong get_window_long;
    StarsPlanetWndSetLong set_window_long;
    StarsU16 font_ariel8;
    StarsU16 font_ariel8_bold;
    StarsI16 ariel8_height;
    StarsI32 fake_edit_proc;
    void *context;
} StarsPlanetWndLayoutOps;
typedef struct StarsPlanetWndLayoutState {
    StarsU16 order_dropdowns[3];
    StarsU16 order_edit;
    StarsU16 battle_dropdown;
    StarsU16 ship_dropdown;
    StarsU16 ship_list;
    StarsU16 fleet_component_list;
    StarsU16 planet_production_list;
    StarsU16 buttons[13];
    StarsU16 report_combo;
    StarsI16 ship_dropdown_height;
    StarsI32 real_edit_proc;
} StarsPlanetWndLayoutState;
typedef struct StarsPlanetWndProcInput {
    StarsU16 frame_window;
    StarsU16 rep_combo;
    StarsU16 ship_dropdown;
    StarsU16 planet_production_list;
    StarsU16 button_handles[13];
    StarsI16 selected_kind;
    StarsI16 selected_planet;
    StarsI16 selected_fleet;
    StarsI16 shift_down;
    StarsI16 ariel8_height;
    StarsI16 frame_dx;
    StarsI16 frame_dy;
    StarsI16 title_bar_height;
    StarsU16 button_face_brush;
    const StarsPlanetWndProcOps *ops;
    const StarsPlanetWndLayoutOps *layout_ops;
    StarsPlanetWndLayoutState *layout_state;
} StarsPlanetWndProcInput;
void stars_planet_wnd_create_layout(StarsU16,
    StarsPlanetWndProcInput *);
StarsI32 stars_planet_wnd_proc(StarsU16, StarsU16, StarsU16, StarsI32,
    StarsPlanetWndProcInput *);
typedef StarsI16 (*StarsMineClickHitTest)(void *, StarsI16, StarsI16);
typedef StarsI16 (*StarsMineClickPreparePopup)(void *, StarsI16);
typedef struct StarsMineClickPopupData {
    StarsI16 hit;
    StarsI16 popup_kind;
    StarsI16 popup_string_id;
    StarsI16 planet_id;
    StarsI16 planet_owner;
    StarsI16 planet_detail;
    StarsI16 planet_var;
    StarsI16 planet_value;
    StarsI16 planet_min;
    StarsI16 planet_max;
    StarsI16 player_value;
    StarsI16 player_min;
    StarsI16 player_max;
    StarsI16 terraform_min[3];
    StarsI16 terraform_max[3];
    StarsI32 mineral_output[3];
    StarsI16 derived_minerals;
    StarsI16 derived_terraform;
    StarsI16 fleet_id;
    StarsI16 fleet_owner;
    StarsI16 fleet_planet;
    StarsI16 fleet_ship_count;
    StarsI16 object_id;
    StarsI16 object_owner;
    StarsI16 object_kind;
    StarsI16 object_x;
    StarsI16 object_y;
} StarsMineClickPopupData;
typedef StarsI16 (*StarsMineClickPreparePopupData)(void *, StarsI16,
    const StarsMineClickPopupData *);
typedef StarsI16 (*StarsMineClickResolvePopup)(void *,
    StarsMineClickPopupData *);
typedef StarsI16 (*StarsMineClickPopupString)(void *, StarsI16, StarsI16);
typedef void (*StarsMineClickShowPopup)(void *, StarsI16, StarsI16, StarsI16);
typedef StarsI16 (*StarsMineClickThresholdMenu)(void *, StarsI16 *, StarsI16,
    StarsI16);
typedef void (*StarsMineClickSetThreshold)(void *, StarsI16);
typedef StarsI16 (*StarsMineClickScannerVisible)(void *);
typedef void (*StarsMineClickAction)(void *, StarsI16);
typedef void (*StarsMineClickInvalidate)(void *);
typedef struct StarsMineClickOps {
    StarsMineClickHitTest hit_test;
    StarsMineClickPreparePopup prepare_popup;
    StarsMineClickPreparePopupData prepare_popup_data;
    StarsMineClickShowPopup show_popup;
    StarsMineClickThresholdMenu threshold_menu;
    StarsMineClickSetThreshold set_threshold;
    StarsMineClickScannerVisible scanner_visible;
    StarsMineClickAction popup_mineral_choices;
    StarsMineClickAction advance_scan_selection;
    StarsMineClickAction popup_fleet_cargo;
    StarsMineClickInvalidate invalidate_mine;
    StarsMineClickAction set_default_popup;
    void *context;
    StarsMineClickResolvePopup resolve_minerals;
    StarsMineClickResolvePopup resolve_terraform;
    StarsMineClickPopupString popup_string;
} StarsMineClickOps;
typedef struct StarsMineClickInput {
    StarsI16 current_graph_max;
    StarsI16 scanner_mode;
    const StarsU8 *packed_planet_record;
    StarsU16 packed_planet_size;
    const StarsU8 *packed_fleet_record;
    StarsU16 packed_fleet_size;
    const StarsU8 *packed_thing_record;
    StarsU16 packed_thing_size;
    const StarsMineClickOps *ops;
} StarsMineClickInput;
void stars_mine_click(StarsI16, StarsI16, StarsI16, StarsI16,
    StarsMineClickInput *);
typedef struct StarsUpdatePlayerScore {
    StarsI32 total;
    StarsI16 planets;
    StarsI32 ship_value;
    StarsI16 tech_count; /* Total levels stored in score history. */
    StarsI16 technology_levels[6]; /* Current player levels for victory gates. */
    StarsI32 resources;
    StarsI16 dead;
    StarsI16 rank;
    StarsU16 achievement_flags;
    StarsU8 achievement_count;
} StarsUpdatePlayerScore;
typedef StarsI16 (*StarsUpdatePlayerScoreCalculate)(void *, StarsI16,
    StarsUpdatePlayerScore *);
typedef StarsI16 (*StarsUpdatePlayerScoreVictoryValue)(void *, StarsI16);
typedef StarsI16 (*StarsUpdatePlayerScoreVictoryCheck)(void *, StarsI16);
typedef void (*StarsUpdatePlayerScoreRecord)(void *, StarsI16,
    const StarsUpdatePlayerScore *);
typedef void (*StarsUpdatePlayerScoreMarkDead)(void *, StarsI16);
typedef void (*StarsUpdatePlayerScoreMessage)(void *, StarsI16, StarsI16,
    StarsU16);
typedef void (*StarsUpdatePlayerScoreDirty)(void *);
typedef struct StarsUpdatePlayerScoreOps {
    StarsUpdatePlayerScoreCalculate calculate;
    StarsUpdatePlayerScoreVictoryValue victory_value;
    StarsUpdatePlayerScoreVictoryCheck victory_check;
    StarsUpdatePlayerScoreRecord record;
    StarsUpdatePlayerScoreMarkDead mark_dead;
    StarsUpdatePlayerScoreMessage send_message;
    StarsUpdatePlayerScoreDirty mark_dirty;
    void *context;
} StarsUpdatePlayerScoreOps;
typedef struct StarsUpdatePlayerScoreInput {
    StarsI16 player_count;
    StarsI16 planet_count;
    StarsI16 turn;
    StarsUpdatePlayerScore *players;
    const StarsUpdatePlayerScoreOps *ops;
} StarsUpdatePlayerScoreInput;
void stars_update_player_scores(StarsUpdatePlayerScoreInput *);
typedef struct StarsPlanetStatsData {
    StarsI32 mineral_rate_x100;
    StarsI16 resources_available;
    StarsI16 resources_total;
    StarsI16 scanner_range;
    StarsI16 scanner_width;
    StarsI16 defense_capacity;
    StarsI16 best_defense_available;
    StarsI16 survival_percent;
    StarsI16 scanner_available;
    StarsI16 special_scanner;
} StarsPlanetStatsData;
typedef StarsI16 (*StarsPlanetStatsLoadData)(void *, const void *,
    StarsPlanetStatsData *);
/* Optional service-owned enrichment after the tile has been accepted.  The
 * original reads the selected player and packed planet through race/resource,
 * scanner, defense, and survival services between tile drawing and the seven
 * value rows. */
typedef void (*StarsPlanetStatsResolveData)(void *, const void *, StarsI16,
    StarsPlanetStatsData *);
typedef StarsI16 (*StarsPlanetStatsDrawTile)(void *, StarsU16,
    const void *, StarsRect *);
typedef void (*StarsPlanetStatsSelectFont)(void *, StarsU16, StarsI16);
typedef StarsI16 (*StarsPlanetStatsMeasureLabel)(void *, StarsU16, StarsI16);
typedef void (*StarsPlanetStatsDrawLabel)(void *, StarsU16, StarsI16,
    StarsI16, StarsI16);
typedef void (*StarsPlanetStatsDrawValue)(void *, StarsU16, StarsI16,
    StarsI16, StarsI16, StarsI16, const StarsPlanetStatsData *);
/* Optional typed formatting path.  The legacy draw_value callback remains
 * available for callers that already own localization and text rendering;
 * callers that want this procedure to expose its value strings provide both
 * callbacks below. */
typedef StarsI16 (*StarsPlanetStatsFormatValue)(void *, StarsI16,
    const StarsPlanetStatsData *, StarsU8 *, StarsU16);
typedef void (*StarsPlanetStatsDrawFormattedValue)(void *, StarsU16,
    StarsI16, StarsI16, StarsI16, StarsI16, const StarsU8 *, StarsI16);
typedef void (*StarsPlanetStatsSetReference)(void *, StarsI16,
    const StarsRect *);
typedef void (*StarsPlanetStatsSeparator)(void *, StarsU16, StarsI16,
    StarsI16, StarsI16);
typedef struct StarsPlanetStatsOps {
    StarsPlanetStatsDrawTile draw_tile;
    StarsPlanetStatsSelectFont select_font;
    StarsPlanetStatsMeasureLabel measure_label;
    StarsPlanetStatsDrawLabel draw_label;
    StarsPlanetStatsDrawValue draw_value;
    StarsPlanetStatsFormatValue format_value;
    StarsPlanetStatsDrawFormattedValue draw_formatted_value;
    StarsPlanetStatsSetReference set_reference;
    StarsPlanetStatsSeparator draw_separator;
    StarsPlanetStatsLoadData load_data;
    StarsPlanetStatsResolveData resolve_data;
    void *context;
} StarsPlanetStatsOps;
typedef struct StarsPlanetStatsInput {
    StarsU16 hdc;
    const void *tile;
    StarsPlanetStatsData data;
    StarsI16 font_normal;
    StarsI16 font_bold;
    StarsI16 line_height;
    StarsI16 hidden;
    StarsI16 player;
    const StarsPlanetStatsOps *ops;
} StarsPlanetStatsInput;
void stars_draw_planet_stats(StarsPlanetStatsInput *);

/* Production bridge for the recovered 56-byte packed planet view used by
 * DrawPlanetStats. Service values outside the record remain typed sidecars;
 * the record supplies the scanner/defense and mineral-rate words. */
#define STARS_PLANET_STATS_PLANET_SIZE 56
typedef struct StarsPlanetStatsPackedInput {
    const StarsU8 *planet_record;
    StarsU16 planet_size;
    StarsI16 resources_available;
    StarsI16 resources_total;
    StarsI16 scanner_width;
    StarsI16 best_defense_available;
    StarsI16 survival_percent;
    StarsI16 scanner_available;
    StarsI16 special_scanner;
    StarsPlanetStatsInput stats;
} StarsPlanetStatsPackedInput;
void stars_draw_planet_stats_packed(StarsPlanetStatsPackedInput *);
typedef struct StarsPlanetPopInfo {
    StarsI16 owner;
    StarsI16 selected_player;
    StarsI32 mineral_rate;
    StarsI16 habitability_class;
    StarsI32 max_population;
    StarsI16 desirability_tenths;
    StarsI32 population_change;
    StarsI16 foreign_environment_class;
} StarsPlanetPopInfo;
typedef struct StarsPlanetPopSegment {
    StarsI16 kind;
    StarsI16 string_id;
    StarsI32 value1;
    StarsI32 value2;
    StarsI16 bold;
} StarsPlanetPopSegment;
typedef void (*StarsPlanetPopPrepareSegment)(void *, StarsPlanetPopSegment *);
typedef StarsI16 (*StarsPlanetPopLookup)(void *, StarsI16,
    StarsPlanetPopInfo *);
typedef void (*StarsPlanetPopSelectFont)(void *, StarsU16, StarsI16);
typedef void (*StarsPlanetPopStream)(void *, StarsU16, StarsI16 *, StarsI16,
    const StarsPlanetPopSegment *, StarsI16);
typedef void (*StarsPlanetPopWrap)(void *, StarsU16, StarsI16 *, StarsI16 *,
    const StarsPlanetPopSegment *, StarsI16, StarsI16);
typedef StarsI32 (*StarsPlanetPopMaxPopulation)(void *, StarsI16, StarsI16);
typedef StarsI16 (*StarsPlanetPopDesirability)(void *, StarsI16, StarsI16);
typedef StarsI32 (*StarsPlanetPopChangePopulation)(void *, StarsI16);
typedef struct StarsPlanetPopInfoOps {
    StarsPlanetPopLookup lookup_planet;
    StarsPlanetPopSelectFont select_font;
    StarsPlanetPopStream stream_text;
    StarsPlanetPopWrap wrap_text;
    StarsPlanetPopMaxPopulation max_population;
    StarsPlanetPopDesirability desirability;
    StarsPlanetPopChangePopulation change_population;
    StarsPlanetPopPrepareSegment prepare_segment;
    void *context;
} StarsPlanetPopInfoOps;
typedef struct StarsPlanetPopInfoInput {
    StarsU16 hdc;
    StarsI16 planet_id;
    StarsI16 viewer_player;
    StarsI16 print_mode;
    StarsI16 line_height;
    StarsI16 font_normal;
    StarsI16 font_bold;
    /* Optional production view of the recovered 56-byte _planet record.
     * Service values not represented by that record remain explicit sidecars;
     * the packed view takes precedence over lookup_planet when valid. */
    const StarsU8 *packed_planet_record;
    StarsU16 packed_planet_size;
    StarsI16 packed_selected_player;
    StarsI16 packed_habitability_class;
    StarsI32 packed_max_population;
    StarsI16 packed_desirability_tenths;
    StarsI16 packed_foreign_environment_class;
    const StarsPlanetPopInfoOps *ops;
} StarsPlanetPopInfoInput;
StarsPoint stars_display_planet_pop_info(StarsPlanetPopInfoInput *);
typedef struct StarsPlanetStateInfoData {
    StarsI16 planet_id;
    StarsI16 planet_var;
    StarsI16 planet_value;
    StarsI16 player_min;
    StarsI16 player_max;
    StarsI16 player_value;
    StarsI16 planet_min;
    StarsI16 planet_max;
    StarsI16 player_value_secondary;
    StarsI16 planet_max_secondary;
    StarsI16 planet_min_secondary;
    StarsI16 player_value_min_secondary;
    StarsI16 difference_center;
    StarsI16 planet_min_offset;
    StarsI16 planet_max_offset;
    StarsI16 environment_index;
} StarsPlanetStateInfoData;
typedef struct StarsPlanetStateInfoSegment {
    StarsI16 string_id;
    StarsI16 kind;
    StarsI16 value;
    StarsI16 bold;
} StarsPlanetStateInfoSegment;
typedef void (*StarsPlanetStatePrepareSegment)(void *,
    StarsPlanetStateInfoSegment *);
typedef StarsI16 (*StarsPlanetStateLookup)(void *, StarsPlanetStateInfoData *);
typedef void (*StarsPlanetStateSelectFont)(void *, StarsU16, StarsI16);
typedef void (*StarsPlanetStateStream)(void *, StarsU16, StarsI16 *, StarsI16,
    const StarsPlanetStateInfoSegment *, StarsI16);
typedef void (*StarsPlanetStateWrap)(void *, StarsU16, StarsI16 *, StarsI16 *,
    const StarsPlanetStateInfoSegment *, StarsI16, StarsI16);
typedef StarsI16 (*StarsPlanetStateEnvironment)(void *, StarsI16, StarsI16,
    StarsI16, StarsI16);
typedef StarsI16 (*StarsPlanetStateDesirability)(void *, StarsI16, StarsI16,
    StarsI16, StarsI16);
typedef struct StarsPlanetStateInfoOps {
    StarsPlanetStateLookup lookup;
    StarsPlanetStateSelectFont select_font;
    StarsPlanetStateStream stream_text;
    StarsPlanetStateWrap wrap_text;
    StarsPlanetStateEnvironment environment;
    StarsPlanetStateDesirability desirability;
    StarsPlanetStatePrepareSegment prepare_segment;
    void *context;
} StarsPlanetStateInfoOps;
typedef struct StarsPlanetStateInfoInput {
    StarsU16 hdc;
    StarsI16 print_mode;
    StarsI16 line_height;
    StarsI16 font_normal;
    StarsI16 font_bold;
    StarsPlanetStateInfoData data;
    /* Optional production views of the recovered _planet and _popupdata
     * records. Both must be complete before they replace typed data. */
    const StarsU8 *packed_planet_record;
    StarsU16 packed_planet_size;
    const StarsU8 *packed_popupdata;
    StarsU16 packed_popupdata_size;
    StarsI16 packed_environment_index;
    const StarsPlanetStateInfoOps *ops;
} StarsPlanetStateInfoInput;
StarsPoint stars_display_planet_state_info(StarsPlanetStateInfoInput *);
typedef struct StarsPopupMeasureCase {
    StarsU16 hwnd;
    StarsU16 hdc;
    StarsI16 popup_kind;
    StarsI16 line_height;
    StarsI16 line_height_large;
    StarsI16 print_mode;
    StarsI16 value1;
    StarsI16 value2;
} StarsPopupMeasureCase;
typedef StarsI16 (*StarsPopupClientToScreen)(void *, StarsU16, StarsPoint *);
typedef StarsU16 (*StarsPopupGetDc)(void *, StarsU16);
typedef StarsU16 (*StarsPopupWindowSelectFont)(void *, StarsU16, StarsU16);
typedef StarsPoint (*StarsPopupMeasure)(void *, const StarsPopupMeasureCase *);
typedef void (*StarsPopupUpdateSlots)(void *);
typedef StarsI16 (*StarsPopupGetMetric)(void *, StarsI16);
typedef StarsU16 (*StarsPopupCreateWindow)(void *, StarsU16, StarsU32,
    StarsI16, StarsI16, StarsI16, StarsI16);
typedef void (*StarsPopupReleaseDc)(void *, StarsU16, StarsU16);
typedef void (*StarsPopupSendMessage)(void *, StarsU16, StarsU16, StarsU16);
typedef void (*StarsPopupSetCapture)(void *, StarsU16);
typedef struct StarsPopupOps {
    StarsPopupClientToScreen client_to_screen;
    StarsPopupGetDc get_dc;
    StarsPopupWindowSelectFont select_font;
    StarsPopupMeasure measure;
    StarsPopupUpdateSlots update_slots;
    StarsPopupGetMetric get_system_metric;
    StarsPopupCreateWindow create_window;
    StarsPopupReleaseDc release_dc;
    StarsPopupSendMessage send_message;
    StarsPopupSetCapture set_capture;
    void *context;
} StarsPopupOps;
typedef struct StarsPopupInput {
    StarsU16 hwnd;
    StarsI16 x;
    StarsI16 y;
    StarsI16 popup_kind;
    StarsI16 planet_id;
    StarsU16 font_normal;
    StarsI16 line_height;
    StarsI16 line_height_large;
    StarsI16 slot_extra_rows;
    const StarsPopupOps *ops;
} StarsPopupInput;
void stars_popup(StarsPopupInput *);
typedef struct StarsPopupMenuText {
    StarsI16 source_kind;
    StarsI16 id;
} StarsPopupMenuText;
typedef StarsI16 (*StarsPopupMenuClientToScreen)(void *, StarsU16,
    StarsPoint *);
typedef StarsU16 (*StarsPopupMenuCreate)(void *);
typedef const StarsU8 *(*StarsPopupMenuResolveText)(void *,
    const StarsPopupMenuText *);
typedef void (*StarsPopupMenuAppend)(void *, StarsU16, StarsU16, StarsU16,
    const StarsU8 *);
typedef void (*StarsPopupMenuTrack)(void *, StarsU16, StarsU16, StarsI16,
    StarsI16, StarsU16);
typedef void (*StarsPopupMenuDestroy)(void *, StarsU16);
typedef StarsI16 (*StarsPopupMenuPeek)(void *, StarsU16, StarsU16, StarsU16,
    StarsU16, StarsU32 *);
typedef struct StarsPopupMenuOps {
    StarsPopupMenuClientToScreen client_to_screen;
    StarsPopupMenuCreate create_menu;
    StarsPopupMenuResolveText resolve_text;
    StarsPopupMenuAppend append_menu;
    StarsPopupMenuTrack track_popup;
    StarsPopupMenuDestroy destroy_menu;
    StarsPopupMenuPeek peek_command;
    void *context;
} StarsPopupMenuOps;
StarsI16 stars_popup_menu(StarsU16, StarsI16, StarsI16, StarsI16,
    const StarsU32 *, const StarsU8 *const *, StarsI16, StarsI16,
    const StarsPopupMenuOps *);
typedef struct StarsDropPartSlot {
    StarsU16 grhst;
    StarsU16 quantity;
    /* Packed capacity from the selected hull definition. */
    StarsU16 capacity;
} StarsDropPartSlot;
typedef StarsI16 (*StarsDropPartHitTest)(void *, StarsI16,
    const StarsPoint *);
typedef void (*StarsDropPartEmptyDrop)(void *, StarsI16, StarsU16 *,
    StarsU16 *);
typedef void (*StarsDropPartUpdate)(void *, StarsI16);
typedef void (*StarsDropPartGetClientRect)(void *, StarsRect *);
typedef void (*StarsDropPartDrawSlot)(void *, StarsI16, const StarsRect *,
    StarsI16);
typedef void (*StarsDropPartDrawSelection)(void *, StarsI16, StarsI16);
typedef void (*StarsDropPartDrawHull)(void *, StarsI16, StarsI16,
    const StarsRect *);
typedef void (*StarsDropPartInvalidateRect)(void *, const StarsRect *,
    StarsI16);
typedef void (*StarsDropPartInvalidate)(void *);
typedef void (*StarsDropPartSelect)(void *, StarsI16);
typedef void (*StarsDropPartBeep)(void *);
typedef void (*StarsDropPartAdvanceTutor)(void *);
typedef struct StarsDropPartOps {
    StarsDropPartHitTest hit_test;
    StarsDropPartEmptyDrop empty_drop;
    StarsDropPartUpdate update_cost;
    StarsDropPartGetClientRect get_client_rect;
    StarsDropPartDrawSlot draw_slot;
    StarsDropPartDrawSelection draw_selected_component;
    StarsDropPartDrawHull draw_selected_hull;
    StarsDropPartInvalidateRect invalidate_rect;
    StarsDropPartInvalidate invalidate;
    StarsDropPartSelect set_selection;
    StarsDropPartBeep beep;
    StarsDropPartAdvanceTutor advance_tutor;
    void *context;
} StarsDropPartOps;
typedef struct StarsDropPartInput {
    StarsPoint point;
    StarsI16 client_width;
    StarsU16 key_control;
    StarsU16 key_shift;
    StarsU16 source_grhst;
    StarsU16 source_quantity;
    StarsI16 source_index;
    StarsI16 no_modify;
    StarsI16 selected_slot;
    StarsI16 y_build_info_sum;
    StarsDropPartSlot *slots;
    StarsI16 slot_count;
    const StarsRect *packed_slot_rects;
    StarsI16 packed_slot_rect_count;
    const StarsDropPartOps *ops;
} StarsDropPartInput;
StarsI16 stars_drop_part(StarsDropPartInput *);
typedef StarsI32 (*StarsMineralFixedMultiply)(void *, StarsI32, StarsI32);
typedef StarsI32 (*StarsMineralFixedDivide)(void *, StarsI32, StarsI32);
typedef StarsI32 (*StarsMineralFixedRemainder)(void *, StarsI32, StarsI32);
typedef StarsI16 (*StarsMineralRandom)(void *, StarsI16);
typedef StarsI32 (*StarsMineralOperatingMines)(void *);
typedef StarsI16 (*StarsMineralCanDeplete)(void *, StarsI16, StarsI32);
typedef void (*StarsMineralApply)(void *, StarsI16, StarsI32, StarsU16 *,
    StarsU8 *);
typedef StarsI16 (*StarsMineralFleetMines)(void *, StarsI16, StarsI32 *);
typedef void (*StarsMineralFleetConsumed)(void *, StarsI16);
typedef struct StarsMineralMinedOps {
    StarsMineralFixedMultiply fixed_multiply;
    StarsMineralFixedDivide fixed_divide;
    StarsMineralFixedRemainder fixed_remainder;
    StarsMineralRandom random;
    StarsMineralOperatingMines operating_mines;
    StarsMineralCanDeplete can_deplete;
    StarsMineralApply apply_mineral;
    StarsMineralFleetMines fleet_mines;
    StarsMineralFleetConsumed fleet_consumed;
    void *context;
} StarsMineralMinedOps;
typedef struct StarsMineralMinedInput {
    StarsI16 player;
    StarsI16 special_race;
    StarsI16 efficiency_tenths;
    StarsI32 operating_mines;
    StarsI32 c_mines;
    StarsU8 concentration[3];
    StarsU8 percent_level[3];
    StarsI16 force_concentration_floor;
    StarsI16 random_rounding;
    StarsI16 apply;
    StarsI16 fleet_count;
    StarsI32 output[3];
    /* Optional typed view of the packed rgwtMin array.  When present, the
     * apply path performs the original concentration/percentage depletion
     * arithmetic directly; callback-only callers retain the service boundary
     * used by earlier probes. */
    StarsI32 *mineral_weight;
    const StarsMineralMinedOps *ops;
} StarsMineralMinedInput;
void stars_est_minerals_mined(StarsMineralMinedInput *);
typedef struct StarsTransferObject {
    StarsI16 object_type;
    StarsI16 object_id;
    StarsI16 has_cargo;
    StarsI16 is_selected_planet;
} StarsTransferObject;
typedef StarsI16 (*StarsTransferPrepare)(void *, StarsTransferObject *,
    StarsI16);
typedef void (*StarsTransferJettison)(void *, StarsTransferObject *);
typedef StarsI16 (*StarsTransferSplit)(void *, StarsTransferObject *,
    StarsTransferObject *);
typedef StarsI16 (*StarsTransferEnumerateHulls)(void *,
    const StarsTransferObject *, StarsI16 *, StarsI16);
typedef StarsI16 (*StarsTransferDialog)(void *);
typedef void (*StarsTransferBalance)(void *, StarsTransferObject *,
    StarsTransferObject *);
typedef void (*StarsTransferDeleteFleet)(void *, StarsTransferObject *);
typedef void (*StarsTransferCancelMemory)(void *);
typedef void (*StarsTransferRefresh)(void *, StarsI16);
typedef void (*StarsTransferInvalidate)(void *, StarsI16);
typedef void (*StarsTransferAdvanceTutor)(void *);
/* TransferStuff first resolves each packed object through FLookupObject and
 * later commits the live planet/fleet/thing record with its specialized
 * negative-id lookup.  The update flag distinguishes those two boundaries. */
typedef StarsI16 (*StarsTransferLookupObject)(void *, StarsI16, StarsI16,
    StarsTransferObject *, StarsI16);
typedef void (*StarsTransferRefreshObject)(void *, StarsI16, StarsI16);
typedef struct StarsTransferOps {
    StarsTransferPrepare prepare_object;
    StarsTransferJettison jettison;
    StarsTransferSplit split_fleet;
    StarsTransferEnumerateHulls enumerate_hulls;
    StarsTransferDialog show_dialog;
    StarsTransferBalance balance;
    StarsTransferDeleteFleet delete_fleet;
    StarsTransferCancelMemory cancel_memory;
    StarsTransferRefresh refresh_selection;
    StarsTransferInvalidate invalidate_window;
    StarsTransferAdvanceTutor advance_tutor;
    void *context;
    StarsTransferLookupObject lookup_object;
    StarsTransferRefreshObject refresh_object;
} StarsTransferOps;
typedef struct StarsTransferInput {
    StarsTransferObject first;
    StarsTransferObject second;
    StarsI16 mode;
    StarsI16 dialog_result;
    StarsI16 scanner_mode;
    StarsI16 scanner_bits;
    const StarsTransferOps *ops;
} StarsTransferInput;
StarsI16 stars_transfer_stuff(StarsTransferInput *);
typedef struct StarsPlanetShipListState {
    StarsI16 has_object;
    StarsI16 tile_changed;
    StarsI16 tile_ship_mode;
    StarsI16 selected_player;
    StarsI16 fleet_owner;
    StarsI16 transfer_kind;
    StarsI16 transfer_found;
    StarsI16 transfer_index;
    StarsI16 combo_selection;
    StarsI16 tile_width;
    StarsI16 details_hidden;
    StarsI16 mode_flag;
    StarsI16 button_enabled[3];
} StarsPlanetShipListState;
typedef StarsI16 (*StarsPlanetShipListDrawTile)(void *, StarsU16,
    StarsI16, StarsRect *);
typedef void (*StarsPlanetShipListShow)(void *, StarsI16, StarsI16);
typedef void (*StarsPlanetShipListResetRefs)(void *);
typedef void (*StarsPlanetShipListSetCombo)(void *, StarsI16, StarsI16,
    StarsI16, StarsI16);
typedef void (*StarsPlanetShipListSetComboEx)(void *, StarsI16, StarsI16,
    StarsI16, StarsI16, StarsU16);
typedef void (*StarsPlanetShipListSetButton)(void *, StarsI16, StarsI16,
    StarsI16, StarsI16, StarsI16);
typedef StarsI32 (*StarsPlanetShipListComboSelection)(void *, StarsU16);
typedef void (*StarsPlanetShipListEnable)(void *, StarsI16, StarsI16);
typedef StarsI16 (*StarsPlanetShipListLookupTransfer)(void *, StarsI16,
    StarsI16, StarsI16, StarsI16 *);
typedef void (*StarsPlanetShipListDrawGauge)(void *, StarsI16,
    const StarsRect *);
typedef void (*StarsPlanetShipListFill)(void *, const StarsRect *);
/* The original draws fleet and thing detail rows differently after the
 * transfer lookup.  Keep that branch typed while retaining fill_details for
 * callers that own the complete localized/GDI implementation. */
typedef void (*StarsPlanetShipListDrawDetails)(void *, const StarsRect *,
    StarsI16, StarsI16);
typedef struct StarsPlanetShipListOps {
    StarsPlanetShipListDrawTile draw_tile;
    StarsPlanetShipListShow show_control;
    StarsPlanetShipListResetRefs reset_references;
    StarsPlanetShipListSetCombo set_combo_position;
    StarsPlanetShipListSetComboEx set_combo_position_ex;
    StarsPlanetShipListSetButton set_button_position;
    StarsPlanetShipListComboSelection combo_selection;
    StarsPlanetShipListEnable enable_button;
    StarsPlanetShipListLookupTransfer lookup_transfer;
    StarsPlanetShipListDrawGauge draw_gauge;
    StarsPlanetShipListFill fill_details;
    StarsPlanetShipListDrawDetails draw_details;
    void *context;
} StarsPlanetShipListOps;
typedef struct StarsPlanetShipListInput {
    StarsU16 hdc;
    StarsI16 title_object_id;
    StarsI16 line_height;
    StarsI16 combo_height;
    StarsI16 button_width;
    StarsI16 player_id;
    StarsPlanetShipListState state;
    /* Optional production views of the original selection and transfer
     * records. Valid raw records take precedence over typed state and the
     * transfer lookup callback remains the fallback for incomplete views. */
    const StarsU8 *packed_selection_record;
    StarsU16 packed_selection_size;
    const StarsU8 *packed_transfer_record;
    StarsU16 packed_transfer_size;
    const StarsPlanetShipListOps *ops;
} StarsPlanetShipListInput;
void stars_draw_planet_ship_list(StarsPlanetShipListInput *);
typedef StarsI16 (*StarsTrackXferHitTest)(void *, StarsI16,
    const StarsPoint *);
typedef void (*StarsTrackXferCapture)(void *, StarsU16);
typedef StarsI16 (*StarsTrackXferMouseMove)(void *, StarsPoint *);
typedef StarsI32 (*StarsTrackXferQuantity)(void *, StarsI16, StarsI16);
typedef StarsI32 (*StarsTrackXferChangeCargo)(void *, StarsI16, StarsI32);
typedef StarsI32 (*StarsTrackXferSupply)(void *, StarsI16, StarsI32);
/* mdXferDlg updates the active and opposite packed records directly; it is
 * not the same service call as XferSupply used by the ordinary cargo path. */
typedef StarsI32 (*StarsTrackXferDialogTransfer)(void *, StarsI16,
    StarsI32);
typedef StarsI16 (*StarsTrackXferTrackButton)(void *, StarsI16);
typedef void (*StarsTrackXferDraw)(void *, StarsU16, StarsI16);
typedef void (*StarsTrackXferUpdate)(void *);
typedef struct StarsTrackXferOps {
    StarsTrackXferHitTest hit_test;
    StarsTrackXferCapture set_capture;
    StarsTrackXferCapture release_capture;
    StarsTrackXferMouseMove mouse_move;
    StarsTrackXferQuantity quantity;
    StarsTrackXferChangeCargo change_cargo;
    StarsTrackXferSupply supply;
    StarsTrackXferDialogTransfer dialog_transfer;
    StarsTrackXferTrackButton track_button;
    StarsTrackXferDraw draw_dialog;
    StarsTrackXferUpdate update_buttons;
    void *context;
} StarsTrackXferOps;
typedef struct StarsTrackXferInput {
    StarsU16 hwnd;
    StarsU16 hdc;
    StarsPoint point;
    StarsI16 button_count;
    StarsI16 source_mode;
    StarsI16 source_side;
    StarsI16 cargo_kind;
    StarsI16 object_kind;
    StarsI16 owner_matches;
    StarsI16 md_xfer_dialog;
    StarsI16 player_id;
    StarsI16 fkb;
    StarsI16 button_index;
    StarsI32 total_cargo;
    StarsI32 other_cargo;
    /* Geometry of the selected transfer button.  The original drag path
     * scales against (right-left-2), not an abstract 0..100 percentage. */
    StarsRect drag_rect;
    StarsI16 drag_rect_valid;
    const StarsTrackXferOps *ops;
} StarsTrackXferInput;
StarsI16 stars_track_xfer(StarsTrackXferInput *);
#define STARS_DRAW_POPUP_DATA_SIZE 22
typedef struct StarsDrawPopupPayload {
    /* Complete 22-byte _popupdata union view.  Pointer members are retained
     * as raw Win16 far-pointer words; the dispatcher never dereferences them
     * because ownership and address-space validity belong to the callback. */
    StarsU8 raw[STARS_DRAW_POPUP_DATA_SIZE];
    StarsI16 gr_popup;
    StarsI32 rgi[5];
    StarsI16 id_planet;
    StarsI16 planet_var;
    StarsI16 planet_value;
    StarsI16 planet_minimum;
    StarsI16 planet_maximum;
    StarsI16 player_value;
    StarsI16 player_minimum;
    StarsI16 player_maximum;
    StarsI16 id_plan;
    StarsI16 c_max;
    StarsI16 c_current;
    StarsI16 c_operate;
    StarsI16 factory;
    StarsI16 dx_out;
    StarsU32 psz_near;
    StarsI16 player;
    StarsU32 fleet_far;
    StarsI16 red_damage;
    StarsI16 dx_damage;
    StarsU16 grbit;
    StarsU8 part[8];
    StarsU32 shdef_far;
    StarsI16 show_damage;
    StarsI16 hide_counts;
    StarsI16 token;
    StarsI16 summary;
    StarsI16 token_index;
} StarsDrawPopupPayload;
typedef struct StarsDrawPopupCase {
    StarsU16 hwnd;
    StarsU16 hdc;
    StarsI16 popup_kind;
    StarsRect client_rect;
    StarsRect slot_rect;
    StarsI16 value1;
    StarsI16 value2;
    StarsI16 starbase_mode;
    StarsDrawPopupPayload payload;
} StarsDrawPopupCase;
#define STARS_DRAW_POPUP_TEXT_TITLE 1
#define STARS_DRAW_POPUP_TEXT_LABEL 2
#define STARS_DRAW_POPUP_TEXT_VALUE 3
#define STARS_DRAW_POPUP_TEXT_HULL_NAME 4
#define STARS_DRAW_POPUP_TEXT_PLANET_NAME 5
#define STARS_DRAW_POPUP_TEXT_FLEET_NAME 6
#define STARS_DRAW_POPUP_TEXT_SEPARATOR 7
#define STARS_DRAW_POPUP_FORMAT_NONE 0
#define STARS_DRAW_POPUP_FORMAT_I16 1
#define STARS_DRAW_POPUP_FORMAT_I16_PAIR 2
#define STARS_DRAW_POPUP_FORMAT_LONG 3
#define STARS_DRAW_POPUP_FORMAT_LONG_PAIR 4
#define STARS_DRAW_POPUP_FORMAT_PERCENT_PAIR 5
#define STARS_DRAW_POPUP_ANCHOR_LEFT 0
#define STARS_DRAW_POPUP_ANCHOR_RIGHT 1
#define STARS_DRAW_POPUP_ANCHOR_CENTER 2
#define STARS_DRAW_POPUP_FONT_NORMAL 0
#define STARS_DRAW_POPUP_FONT_BOLD 1
typedef struct StarsDrawPopupText {
    StarsI16 popup_kind;
    StarsI16 row;
    StarsI16 role;
    StarsI16 string_id;
    StarsI16 format_kind;
    StarsI32 value1;
    StarsI32 value2;
    StarsI16 visible;
    StarsI16 bold;
    StarsI16 text_color;
    StarsI16 hull_index;
    StarsI16 x;
    StarsI16 y;
    StarsI16 anchor;
    StarsI16 font_kind;
    StarsI16 background_mode;
} StarsDrawPopupText;
typedef StarsI16 (*StarsDrawPopupGetClientRect)(void *, StarsU16,
    StarsRect *);
typedef StarsU32 (*StarsDrawPopupSetColor)(void *, StarsU16, StarsU32);
typedef StarsU16 (*StarsDrawPopupSetMode)(void *, StarsU16, StarsU16);
typedef StarsU16 (*StarsDrawPopupSelectFont)(void *, StarsU16, StarsU16);
typedef StarsI16 (*StarsDrawPopupResolveString)(void *, StarsI16,
    StarsU8 *, StarsU16);
typedef StarsI16 (*StarsDrawPopupFormatText)(void *,
    const StarsDrawPopupText *, StarsU8 *, StarsU16);
typedef StarsI16 (*StarsDrawPopupMeasureText)(void *,
    const StarsDrawPopupText *);
typedef void (*StarsDrawPopupEmitText)(void *, const StarsDrawPopupText *,
    const StarsU8 *, StarsI16);
typedef void (*StarsDrawPopupDrawSeparator)(void *, StarsU16, StarsI16,
    StarsI16, StarsI16, StarsI16, StarsU16);
typedef StarsI16 (*StarsDrawPopupHullVisible)(void *, StarsI16, StarsI16,
    StarsI16);
typedef StarsI16 (*StarsDrawPopupDecorateHull)(void *, StarsI16, StarsI16,
    StarsU8 *, StarsU16);
typedef void (*StarsDrawPopupDrawCase)(void *, const StarsDrawPopupCase *);
typedef struct StarsDrawPopupOps {
    StarsDrawPopupGetClientRect get_client_rect;
    StarsDrawPopupSetColor set_background_color;
    StarsDrawPopupSetColor set_text_color;
    StarsDrawPopupSetMode set_background_mode;
    StarsDrawPopupSelectFont select_font;
    StarsDrawPopupResolveString resolve_string;
    StarsDrawPopupFormatText format_text;
    StarsDrawPopupMeasureText measure_text;
    StarsDrawPopupEmitText emit_text;
    StarsDrawPopupDrawSeparator draw_separator;
    StarsDrawPopupHullVisible hull_visible;
    StarsDrawPopupDecorateHull decorate_hull;
    StarsDrawPopupDrawCase draw_mineral;
    StarsDrawPopupDrawCase draw_fleet_name;
    StarsDrawPopupDrawCase draw_fleet;
    StarsDrawPopupDrawCase draw_coordinates;
    StarsDrawPopupDrawCase draw_planet_state;
    StarsDrawPopupDrawCase draw_zip_order;
    StarsDrawPopupDrawCase draw_planet_population;
    StarsDrawPopupDrawCase draw_factory_mine;
    StarsDrawPopupDrawCase draw_component;
    StarsDrawPopupDrawCase draw_string;
    StarsDrawPopupDrawCase draw_resource;
    StarsDrawPopupDrawCase draw_slot_hull;
    StarsDrawPopupDrawCase draw_default;
    void *context;
} StarsDrawPopupOps;
typedef struct StarsDrawPopupInput {
    StarsU16 hwnd;
    StarsU16 hdc;
    StarsI16 popup_kind;
    /* Optional production view of the original 22-byte _popupdata record.
     * When present and large enough, its selector word takes precedence over
     * the legacy typed popup_kind field. */
    const StarsU8 *packed_popupdata;
    StarsU16 packed_popupdata_size;
    StarsU16 font_normal;
    StarsU16 font_bold;
    StarsI16 arial8_height;
    StarsI16 slot_starbase_mode;
    /* Optional raw views used by the source-owned localization/filtering
     * boundary. The popup union contains only far-pointer words, so callers
     * provide the resolved fleet and hull records explicitly. */
    const StarsU8 *packed_fleet_record;
    StarsU16 packed_fleet_size;
    const StarsU8 *packed_shdef_record;
    StarsU16 packed_shdef_size;
    StarsI16 coordinate_valid;
    StarsI16 coordinate_planet;
    StarsI16 coordinate_x;
    StarsI16 coordinate_y;
    const StarsDrawPopupOps *ops;
} StarsDrawPopupInput;
void stars_draw_popup(StarsDrawPopupInput *);
typedef void (*StarsMergeFleetsGetClientRect)(void *, StarsU16, StarsRect *);
typedef void (*StarsMergeFleetsFillRect)(void *, StarsU16, const StarsRect *,
    StarsU16);
typedef void (*StarsMergeFleetsSetBackground)(void *, StarsU16, StarsU32);
typedef StarsU16 (*StarsMergeFleetsGetDialogItem)(void *, StarsU16, StarsU16);
typedef void (*StarsMergeFleetsStickyPosition)(void *, StarsU16, StarsPoint *,
    StarsI16);
typedef StarsI16 (*StarsMergeFleetsGetName)(void *, StarsI16, StarsU8 *,
    StarsU16);
typedef StarsI16 (*StarsMergeFleetsIsSpecial)(void *, StarsI16);
typedef void (*StarsMergeFleetsAppendSpecial)(void *, StarsU8 *);
typedef void (*StarsMergeFleetsAddString)(void *, StarsU16, const StarsU8 *);
typedef StarsU32 (*StarsMergeFleetsSendMessage)(void *, StarsU16, StarsU16,
    StarsU16, StarsU32);
typedef void (*StarsMergeFleetsAdvanceTutor)(void *);
typedef StarsI16 (*StarsMergeFleetsConfirm)(void *);
typedef void (*StarsMergeFleetsEndDialog)(void *, StarsU16, StarsI16);
typedef void (*StarsMergeFleetsWinHelp)(void *, StarsU16, const StarsU8 *,
    StarsU16, StarsU16);
typedef struct StarsMergeFleetsOps {
    StarsMergeFleetsGetClientRect get_client_rect;
    StarsMergeFleetsFillRect fill_rect;
    StarsMergeFleetsSetBackground set_background_color;
    StarsMergeFleetsGetDialogItem get_dialog_item;
    StarsMergeFleetsStickyPosition sticky_position;
    StarsMergeFleetsGetName get_fleet_name;
    StarsMergeFleetsIsSpecial is_special;
    StarsMergeFleetsAppendSpecial append_special;
    StarsMergeFleetsAddString add_string;
    StarsMergeFleetsSendMessage send_message;
    StarsMergeFleetsAdvanceTutor advance_tutor;
    StarsMergeFleetsConfirm confirm_merge;
    StarsMergeFleetsEndDialog end_dialog;
    StarsMergeFleetsWinHelp win_help;
} StarsMergeFleetsOps;
typedef struct StarsMergeFleetsInput {
    StarsU16 button_face_brush;
    StarsU32 button_face_color;
    StarsPoint *sticky_point;
    const StarsU8 *help_file;
    StarsU16 list_control_id;
    StarsI16 fleet_count;
    StarsI16 *merge_indices;
    const StarsI16 *fleet_ids;
    StarsI16 selected_fleet_id;
    StarsU16 tutor_active;
    const StarsMergeFleetsOps *ops;
    void *context;
} StarsMergeFleetsInput;
StarsI16 stars_merge_fleets_dlg(StarsU16, StarsU16, StarsU16, StarsU32,
    StarsMergeFleetsInput *);
typedef void (*StarsZipOrderSelectFont)(void *, StarsU16, StarsU16);
typedef StarsI16 (*StarsZipOrderGetString)(void *, StarsI16, StarsU8 *, StarsU16);
typedef void (*StarsZipOrderCenterText)(void *, StarsU16, StarsI16, StarsI16,
    const StarsU8 *, StarsI16);
typedef void (*StarsZipOrderStreamText)(void *, StarsU16, StarsI16 *, StarsI16,
    const StarsU8 *, StarsI16, StarsI16);
typedef struct StarsZipOrderInfoOps {
    StarsZipOrderSelectFont select_font;
    StarsZipOrderGetString get_string;
    StarsZipOrderCenterText center_text;
    StarsZipOrderStreamText stream_text;
} StarsZipOrderInfoOps;
typedef struct StarsZipOrderInfoInput {
    StarsU16 font_normal;
    StarsU16 font_bold;
    StarsI16 arial8_height;
    const StarsZipOrderInfoOps *ops;
    void *context;
} StarsZipOrderInfoInput;
StarsPoint stars_pt_display_zip_ord_info(StarsU16, StarsI16, StarsI16,
    const StarsZipOrderInfoInput *);
typedef void (*StarsDrawPlanShipShowWindow)(void *, StarsU16, StarsI16);
typedef void (*StarsDrawPlanShipGetClientRect)(void *, StarsU16, StarsRect *);
typedef StarsU32 (*StarsDrawPlanShipSetColor)(void *, StarsU16, StarsU32);
typedef StarsU16 (*StarsDrawPlanShipSelectFont)(void *, StarsU16, StarsU16);
typedef StarsU16 (*StarsDrawPlanShipGetDc)(void *, StarsU16);
typedef void (*StarsDrawPlanShipReleaseDc)(void *, StarsU16, StarsU16);
typedef StarsI16 (*StarsDrawPlanShipGetString)(void *, StarsI16, StarsU8 *,
    StarsU16);
typedef void (*StarsDrawPlanShipDiagonalText)(void *, StarsU16,
    const StarsRect *, const StarsU8 *, StarsI16);
typedef void (*StarsDrawPlanShipRecordDraw)(void *, StarsU16, StarsI16,
    StarsU16, StarsU16);
typedef struct StarsDrawPlanShipRecord {
    StarsU32 draw_mask;
    StarsU16 flags;
} StarsDrawPlanShipRecord;
typedef struct StarsDrawPlanShipOps {
    StarsDrawPlanShipShowWindow show_window;
    StarsDrawPlanShipGetClientRect get_client_rect;
    StarsDrawPlanShipSetColor set_background_color;
    StarsDrawPlanShipSetColor set_text_color;
    StarsDrawPlanShipSelectFont select_font;
    StarsDrawPlanShipGetDc get_dc;
    StarsDrawPlanShipReleaseDc release_dc;
    StarsDrawPlanShipGetString get_string;
    StarsDrawPlanShipDiagonalText diagonal_text;
    StarsDrawPlanShipRecordDraw draw_record;
} StarsDrawPlanShipOps;
typedef struct StarsDrawPlanShipInput {
    StarsI16 selected_id;
    StarsI16 selected_object_kind;
    StarsU16 player_dead;
    StarsU16 hwnd_planet;
    StarsU16 font_normal;
    StarsU32 button_face_color;
    StarsU32 button_text_color;
    StarsU16 *button_windows;
    StarsU16 *order_dropdown_windows;
    StarsU16 order_edit_window;
    StarsU16 ship_dropdown_window;
    StarsU16 battle_dropdown_window;
    StarsU16 ship_list_window;
    StarsU16 fleet_component_list_window;
    StarsU16 planet_production_list_window;
    StarsU16 report_combo_window;
    StarsRect *reference_rects;
    StarsU16 reference_count;
    StarsU16 button_count;
    StarsU16 order_dropdown_count;
    StarsU16 draw_flags;
    StarsU16 selected_value;
    StarsDrawPlanShipRecord *fleet_records;
    StarsU16 fleet_record_count;
    StarsDrawPlanShipRecord *planet_records;
    StarsU16 planet_record_count;
    const StarsDrawPlanShipOps *ops;
    void *context;
} StarsDrawPlanShipInput;
void stars_draw_plan_ship(StarsU16, StarsU16, StarsDrawPlanShipInput *);
typedef StarsU16 (*StarsTransferDlgBeginPaint)(void *, StarsU16);
typedef void (*StarsTransferDlgEndPaint)(void *, StarsU16, StarsU16);
typedef void (*StarsTransferDlgDraw)(void *, StarsU16, StarsU16,
    const StarsRect *, StarsI16);
typedef void (*StarsTransferDlgGetClientRect)(void *, StarsU16, StarsRect *);
typedef void (*StarsTransferDlgFillRect)(void *, StarsU16, const StarsRect *,
    StarsU16);
typedef void (*StarsTransferDlgStickyPosition)(void *, StarsU16, StarsPoint *,
    StarsI16);
typedef StarsI16 (*StarsTransferDlgGetString)(void *, StarsI16, StarsU8 *,
    StarsU16);
typedef void (*StarsTransferDlgSetWindowText)(void *, StarsU16, const StarsU8 *);
typedef void (*StarsTransferDlgSetWindowPos)(void *, StarsU16, StarsU16,
    StarsI16, StarsI16, StarsI16, StarsI16, StarsU16);
typedef StarsI16 (*StarsTransferDlgGetSystemMetric)(void *, StarsU16);
typedef StarsU16 (*StarsTransferDlgGetDialogItem)(void *, StarsU16, StarsU16);
typedef void (*StarsTransferDlgGetWindowRect)(void *, StarsU16, StarsRect *);
typedef void (*StarsTransferDlgScreenToClient)(void *, StarsU16, StarsPoint *);
typedef void (*StarsTransferDlgSetupButtons)(void *, const StarsRect *);
typedef void (*StarsTransferDlgAdvanceTutor)(void *);
typedef void (*StarsTransferDlgEndDialog)(void *, StarsU16, StarsI16);
typedef void (*StarsTransferDlgWinHelp)(void *, StarsU16, const StarsU8 *,
    StarsU16, StarsU16);
typedef StarsI16 (*StarsTransferDlgTrack)(void *, StarsU16, StarsPoint,
    StarsU16);
typedef struct StarsTransferDlgOps {
    StarsTransferDlgBeginPaint begin_paint;
    StarsTransferDlgEndPaint end_paint;
    StarsTransferDlgDraw draw;
    StarsTransferDlgGetClientRect get_client_rect;
    StarsTransferDlgFillRect fill_rect;
    StarsTransferDlgStickyPosition sticky_position;
    StarsTransferDlgGetString get_string;
    StarsTransferDlgSetWindowText set_window_text;
    StarsTransferDlgSetWindowPos set_window_pos;
    StarsTransferDlgGetSystemMetric get_system_metric;
    StarsTransferDlgGetDialogItem get_dialog_item;
    StarsTransferDlgGetWindowRect get_window_rect;
    StarsTransferDlgScreenToClient screen_to_client;
    StarsTransferDlgSetupButtons setup_buttons;
    StarsTransferDlgAdvanceTutor advance_tutor;
    StarsTransferDlgEndDialog end_dialog;
    StarsTransferDlgWinHelp win_help;
    StarsTransferDlgTrack track;
} StarsTransferDlgOps;
typedef struct StarsTransferDlgInput {
    StarsU16 button_face_brush;
    StarsU16 mode;
    StarsI16 valid_hulls;
    StarsPoint *sticky_point;
    const StarsU8 *help_file;
    StarsU16 control_ids[3];
    StarsU16 tutor_active;
    const StarsTransferDlgOps *ops;
    void *context;
} StarsTransferDlgInput;
StarsI16 stars_transfer_dlg(StarsU16, StarsU16, StarsU16, StarsU32,
    StarsTransferDlgInput *);
typedef StarsI16 (*StarsFormatTitle)(void *, StarsU8 *, const StarsU8 *,
    StarsI16);
void stars_set_ngw_title(StarsU8 *, StarsU16, StarsI16, StarsGetString,
    StarsFormatTitle, StarsSetWindowText, void *);
typedef void (*StarsTutorError)(void *, StarsI16);
typedef StarsI16 (*StarsCheckSelection)(void *, StarsI16, StarsU16);
typedef StarsI16 (*StarsNameDigitFlag)(void *, StarsU8);
typedef void (*StarsAppendDefaultName)(void *, StarsU8 *, StarsU16);
typedef void (*StarsFitNameToScreen)(void *, StarsU8 *, StarsU16);
typedef struct StarsMakeNewNameOps {
    StarsNameDigitFlag digit_flag;
    StarsAppendDefaultName append_default;
    StarsFitNameToScreen fit;
} StarsMakeNewNameOps;
void stars_make_new_name(StarsU8 *, StarsU16, const StarsMakeNewNameOps *, void *);
StarsI16 stars_check_fleet_wp(StarsU8 *, const StarsU8 *, const StarsU8 *,
    StarsU16, StarsI16, StarsU16, StarsI16, StarsU16, StarsU16,
    StarsTutorError, StarsCheckSelection, void *);
StarsI16 stars_check_planet_route(StarsU8 *, const StarsU8 *, StarsI16);
StarsI16 stars_check_laying_wp(StarsU8 *, const StarsU8 *, const StarsU8 *,
    StarsU16, StarsI16, StarsI16, StarsI16, StarsTutorError,
    StarsCheckSelection, void *);
typedef StarsI16 (*StarsCompareText)(void *, const StarsU8 *, const StarsU8 *);
StarsI16 stars_check_fleet_name(StarsU8 *, const StarsU8 *, StarsI16,
    StarsGetString, StarsCompareText, StarsTutorError, void *);
StarsI16 stars_check_scanner(StarsU8 *, StarsI16, StarsI16);
typedef const StarsU8 *(*StarsFormatIds)(void *, StarsI16);
typedef void (*StarsAlertText)(void *, const StarsU8 *, StarsI16);
void stars_tutor_error(StarsU8 *, StarsI16, StarsFormatIds, StarsAlertText,
    void *);
StarsI16 stars_check_template(StarsU8 *, const StarsU8 *, StarsI16);
StarsI16 stars_check_summary(StarsU8 *, StarsU16, StarsI16,
    const StarsU8 *, const StarsU8 *);
typedef StarsI16 (*StarsMessageNext)(void *, StarsI16);
typedef StarsI16 (*StarsMessageId)(void *, StarsI16);
typedef void (*StarsSetFilteringGroups)(void *, StarsI16, StarsI16);
StarsI16 stars_check_messages(StarsU8 *, StarsI16, StarsI16, StarsI16,
    const StarsU8 *, StarsMessageNext, StarsMessageId,
    StarsSetFilteringGroups, void *);
StarsI16 stars_check_selection(StarsU8 *, StarsU16, StarsI16,
    const StarsU8 *);
StarsI16 stars_check_zip(StarsU8 *, StarsI16, const StarsU8 *, StarsI16,
    StarsGetString, StarsCompareText, StarsTutorError, void *);
StarsI16 stars_check_cargo(StarsU8 *, const StarsU8 *, StarsI16, StarsI16,
    StarsI16, StarsI16, StarsTutorError, StarsCheckSelection, void *);
typedef StarsI32 (*StarsSendBuilderMessage)(void *, StarsU16, StarsU16);
StarsI16 stars_check_ship_builder(StarsU8 *, StarsU16, StarsI16, StarsI16,
    StarsI16, StarsGetDialogItem, StarsSendBuilderMessage, void *);
StarsI16 stars_check_builder_part(StarsU8 *, StarsU16, StarsI16, StarsI16,
    const StarsU8 *, const StarsU8 *, StarsU16, StarsTutorError, void *);
StarsI16 stars_check_queue(StarsU8 *, StarsI16, StarsI16, StarsI16,
    StarsU16, StarsU16, StarsU16, StarsU16, const StarsU8 *,
    const StarsU8 *, StarsU16, StarsTutorError, StarsCheckSelection, void *);
StarsI16 stars_check_xfer_wp(StarsU8 *, const StarsU8 *, const StarsU8 *,
    StarsU16, StarsI16, StarsI16, StarsU16, const StarsU8 *, StarsTutorError,
    StarsCheckSelection, void *);
typedef StarsI16 (*StarsColonizeCargoCheck)(void *, const StarsU8 *, StarsI16);
StarsI16 stars_check_colonize_wp(StarsU8 *, const StarsU8 *, const StarsU8 *,
    StarsU16, StarsI16, StarsU16, StarsColonizeCargoCheck, StarsTutorError,
    void *);
StarsI16 stars_check_patrol_wp(StarsU8 *, const StarsU8 *, const StarsU8 *,
    StarsU16, StarsI16, StarsI16, StarsU16, StarsU16, StarsU16,
    StarsTutorError, void *);
StarsI16 stars_check_battle_plan(StarsU8 *, StarsI16, StarsU16, StarsU16,
    StarsU16, StarsU16, StarsU16, StarsU16, StarsU16, StarsU16);
StarsI16 stars_ok_merge_dialog(StarsU8 *, const StarsU8 *, StarsTutorError,
    void *);
/* base contains the 64-byte tutor-string index pages, env_offsets contains
 * little-endian compressed-stream offsets per page, and resource_chars is the
 * decoded character table. */
StarsI16 stars_cch_tutor_string(StarsU8 *, StarsI16, StarsI16,
    const StarsU8 *, const StarsU8 *, const StarsU8 *, const StarsU8 *);
typedef StarsI16 (*StarsTutorialShipCheck)(void *, StarsI16, StarsI16);
typedef StarsI16 (*StarsTutorialPartCheck)(void *, StarsI16, StarsU16,
    StarsU16, StarsU16);
StarsI16 stars_tutorial_enabled_ship_builder(StarsU8 *, StarsI16,
    StarsI16, StarsU16, StarsU16, StarsU16, const StarsU8 *, const StarsU8 *,
    StarsTutorialShipCheck, StarsTutorialPartCheck, StarsCompareText,
    StarsTutorError, void *);
/* environment is a near-offset token for the new native recovery scope.
 * It must be distinct from any active scope and disjoint from application data.
 * The old data[006E] token is restored on normal and recovered-error returns. */
StarsI16 stars_create_file(StarsU8 *data, StarsU16 type, StarsI16 player,
    StarsU16 forced_name, StarsU16 environment, const StarsStreamApi *api, void *context);
typedef struct StarsAlertApi {
    StarsU16 (*focus)(void *context);
    StarsI16 (*message)(void *context, StarsU16 owner, const StarsU8 *text,
        const StarsU8 *caption, StarsI16 type);
    void (*output)(void *context, StarsI16 channel, const StarsU8 *text);
    StarsU16 (*format_id)(void *context, StarsI16 id);
} StarsAlertApi;
/* OutputSz and PszFormatIds remain explicit game-helper boundaries. */
StarsI16 stars_alert_string(StarsU8 *data, StarsU16 text, StarsI16 type,
    const StarsAlertApi *api, void *context);
void stars_file_error(StarsU8 *data, StarsI16 id, const StarsAlertApi *api, void *context);
StarsI16 stars_bad_file_error(StarsI16 id);
typedef struct StarsDiskIdentity {
    StarsI16 present;
    StarsU16 serial_word_a;
    StarsU16 serial_word_b;
    const StarsU8 *label;
    StarsU8 free_nibble;
} StarsDiskIdentity;
StarsU16 stars_get_disk_serial_number(const StarsDiskIdentity *, StarsU8 *);
typedef struct StarsBitmapSource {
    StarsI32 width;
    StarsI32 height;
    StarsU16 planes;
    StarsU16 bits;
    const StarsU8 *pixels;
    StarsU32 pixel_bytes;
} StarsBitmapSource;
StarsU32 stars_dib_from_bitmap(const StarsBitmapSource *, StarsU32,
    StarsU16, const StarsU8 *, StarsU16, StarsU8 *, StarsU32);
typedef struct StarsMineThing { StarsU16 raw; } StarsMineThing;
void stars_get_mine_field_counts(const StarsMineThing *, StarsU16, StarsI16,
    StarsU16, StarsI16 *, StarsI16 *);
typedef struct StarsSelectionArrowOps {
    StarsU16 (*create_dc)(void *, StarsU16);
    StarsU16 (*select_object)(void *, StarsU16, StarsU16);
    void (*bit_blt)(void *, StarsU16, StarsI16, StarsI16, StarsI16, StarsI16,
        StarsU16, StarsI16, StarsI16, StarsU32);
    void (*delete_dc)(void *, StarsU16);
} StarsSelectionArrowOps;
void stars_draw_selection_arrow(StarsU16, const StarsRect *, StarsI16,
    StarsU16, const StarsSelectionArrowOps *, void *);
typedef struct StarsDiamondOps {
    StarsU16 (*select_object)(void *, StarsU16, StarsU16);
    void (*pat_blt)(void *, StarsU16, StarsI16, StarsI16, StarsI16, StarsI16, StarsU32);
} StarsDiamondOps;
void stars_draw_diamond(StarsU16, const StarsRect *, StarsU16, StarsU16, StarsU16,
    const StarsDiamondOps *, void *);
typedef struct StarsMinePoint { StarsI16 x, y; StarsU16 present; } StarsMinePoint;
typedef struct StarsMineScanSelection {
    StarsI16 planet, fleet, thing;
    StarsMinePoint point;
} StarsMineScanSelection;
StarsI16 stars_other_stuff_at_scan_selection(const StarsMineScanSelection *,
    const StarsMinePoint *, StarsU16, const StarsMinePoint *, StarsU16);
typedef struct StarsMineralBarsOps {
    void (*get_client_rect)(void *, StarsU16, StarsRect *);
    StarsU16 (*get_dc)(void *, StarsU16);
    StarsU16 (*select_font)(void *, StarsU16, StarsU16);
    StarsI16 (*text_width)(void *, StarsU16, StarsU16, StarsU16);
    StarsI16 (*planet_attr_length)(void *);
    void (*release_dc)(void *, StarsU16, StarsU16);
    void (*invalidate_rect)(void *, StarsU16, const StarsRect *, StarsI16);
} StarsMineralBarsOps;
void stars_invalidate_mineral_bars(StarsU16, StarsU16, StarsI16,
    const StarsMineralBarsOps *, void *);
typedef struct StarsMineralTitleState {
    StarsU16 grobj_full, grobj;
    StarsI16 planet, fleet, thing, player;
    StarsU16 thing_flags;
    StarsU16 thing_kind_flags, thing_cargo;
    StarsI16 thing_owner, race_stat;
} StarsMineralTitleState;
typedef struct StarsMineralTitleOps {
    void (*get_string)(void *, StarsU16, StarsU8 *, StarsU16);
    void (*get_name)(void *, StarsI16, StarsI16, StarsU8 *, StarsU16);
    void (*set_title)(void *, const StarsU8 *);
    void (*invalidate)(void *, StarsU16, StarsI16);
    void (*send_message)(void *, StarsU16, StarsU16, StarsU16);
    void (*get_client_rect)(void *, StarsU16, StarsRect *);
    void (*set_window_pos)(void *, StarsU16, StarsI16, StarsI16, StarsI16,
        StarsI16, StarsU16);
} StarsMineralTitleOps;
void stars_set_mineral_title_bar(StarsU16, StarsU16, StarsI16,
    const StarsMineralTitleState *, const StarsMineralTitleOps *, void *);
typedef struct StarsMessageState {
    StarsU16 message_memory, message_count, player_message_count;
    StarsI16 current_message;
    StarsU16 view_filtered;
    StarsU8 filtered[256], sent[256];
} StarsMessageState;
#define STARS_DECORATE_SOUND_DISABLED 0x0004U
void stars_set_filtering_group(StarsMessageState *, StarsI16, StarsI16);

/* MessageWndProc is the controller for the message child window.  These
 * records keep Win16 handles and the original DGROUP state explicit while
 * leaving USER/GDI and game-navigation services at typed boundaries. */
enum {
    STARS_MESSAGE_WM_CREATE = 1,
    STARS_MESSAGE_WM_SIZE = 5,
    STARS_MESSAGE_WM_PAINT = 0x000f,
    STARS_MESSAGE_WM_ERASE_BACKGROUND = 0x0014,
    STARS_MESSAGE_WM_CONTROL_COLOR = 0x0019,
    STARS_MESSAGE_WM_SETCURSOR = 0x0020,
    STARS_MESSAGE_WM_GETMINMAXINFO = 0x0024,
    STARS_MESSAGE_WM_KEYDOWN = 0x0100,
    STARS_MESSAGE_WM_CHAR = 0x0102,
    STARS_MESSAGE_WM_COMMAND = 0x0111,
    STARS_MESSAGE_WM_LBUTTONDOWN = 0x0201,
    STARS_MESSAGE_WM_LBUTTONDOUBLECLICK = 0x0203,
    STARS_MESSAGE_BUTTON_PREVIOUS = 0,
    STARS_MESSAGE_BUTTON_ACTION = 1,
    STARS_MESSAGE_BUTTON_NEXT = 2,
    STARS_MESSAGE_BUTTON_DROPDOWN = 3,
    STARS_MESSAGE_KEY_HOME = 0x24,
    STARS_MESSAGE_KEY_END = 0x23,
    STARS_MESSAGE_KEY_UP = 0x26,
    STARS_MESSAGE_KEY_DOWN = 0x28,
    STARS_MESSAGE_CHAR_ACCEPT = 0x000d,
    STARS_MESSAGE_CHAR_TOGGLE_FILTER = 0x002b,
    STARS_MESSAGE_CHAR_TOGGLE_VIEW = 0x002d,
    STARS_MESSAGE_BUTTON_WIDTH = 0x2c,
    STARS_MESSAGE_LAST_BUTTON_WIDTH = 0x32,
    STARS_MESSAGE_SET_FONT = 0x0030,
    STARS_MESSAGE_ADD_STRING = 0x0403,
    STARS_MESSAGE_SELECT_STRING = 0x040e,
    STARS_MESSAGE_SET_LIMIT = 0x0415,
    STARS_MESSAGE_EDIT_LIMIT = 0x03c8,
    STARS_MESSAGE_CONTROL_COLOR_SCROLL = 6,
    STARS_MESSAGE_MODE_SEND = 0x0100,
    STARS_MESSAGE_FLAG_TITLE_DIRTY = 0x1000,
    STARS_MESSAGE_FLAG_TUTOR = 0x0800,
    STARS_MESSAGE_DEFAULT_PLAYER_NAME_STRING = 0x0549,
    STARS_MESSAGE_MINMAX_WIDTH_EXTRA = 0x00c6,
    STARS_MESSAGE_MINMAX_HEIGHT_EXTRA = 0x0016,
    STARS_MESSAGE_OBJECT_PLANET = 1,
    STARS_MESSAGE_OBJECT_FLEET = 2,
    STARS_MESSAGE_OBJECT_PART = 4,
    STARS_MESSAGE_OBJECT_BATTLE = 6,
    STARS_MESSAGE_OBJECT_SCAN_POINT = 10,
    STARS_MESSAGE_OBJECT_REPORT = 11
};
#define STARS_MESSAGE_CHILD_STYLE_BUTTON 0x40000000UL
#define STARS_MESSAGE_CHILD_STYLE_DROPDOWN 0x40200003UL
#define STARS_MESSAGE_CHILD_STYLE_EDIT 0x40800044UL
#define STARS_MESSAGE_CHILD_STYLE_SCROLL 0x40a00844UL
typedef void (*StarsMessageCreateWindow)(void *, StarsU16 *, StarsU16,
    StarsU32, StarsI16, StarsI16, StarsI16, StarsI16, StarsU16);
typedef void (*StarsMessageSendChild)(void *, StarsU16, StarsU16, StarsU16,
    StarsU32);
typedef void (*StarsMessageSetPosition)(void *, StarsU16, StarsI16, StarsI16,
    StarsI16, StarsI16, StarsU16);
typedef void (*StarsMessageShowChild)(void *, StarsU16, StarsI16);
typedef StarsI32 (*StarsMessageDefaultProc)(void *, StarsU16, StarsU16,
    StarsU16, StarsU32);
typedef StarsI16 (*StarsMessageAsyncKeyState)(void *, StarsU16);
typedef void (*StarsMessageGetPoint)(void *, StarsPoint *);
typedef void (*StarsMessageScreenToClient)(void *, StarsU16, StarsPoint *);
typedef void (*StarsMessageSetCursor)(void *, StarsU16);
typedef void (*StarsMessageGetClientRect)(void *, StarsU16, StarsRect *);
typedef void (*StarsMessageFillRect)(void *, StarsU16, const StarsRect *,
    StarsU16);
typedef void (*StarsMessageSetColor)(void *, StarsU16, StarsU32);
typedef StarsU16 (*StarsMessageBeginPaint)(void *, StarsU16);
typedef void (*StarsMessageEndPaint)(void *, StarsU16, StarsU16);
typedef void (*StarsMessageDrawFrame)(void *, StarsU16, const StarsRect *);
typedef void (*StarsMessageDrawTitle)(void *, StarsU16, const StarsRect *,
    const StarsU8 *, StarsI16);
typedef void (*StarsMessageDrawBody)(void *, StarsU16, const StarsRect *,
    const StarsU8 *, StarsI16);
typedef StarsI16 (*StarsMessageHitTest)(void *, StarsPoint);
typedef void (*StarsMessageSetText)(void *, StarsU16, const StarsU8 *);
typedef StarsI16 (*StarsMessagePlayerName)(void *, StarsI16, StarsU8 *,
    StarsU16);
typedef void (*StarsMessageFormatTitle)(void *, StarsI16, StarsU8 *, StarsU16);
typedef void (*StarsMessageSetMsgTitle)(void *, StarsU16);
typedef void (*StarsMessageFinishEntry)(void *, StarsI16);
typedef StarsI16 (*StarsMessageMoveSelection)(void *, StarsI16);
typedef StarsI16 (*StarsMessageGetId)(void *, StarsI16);
typedef void (*StarsMessageDirtyGame)(void *, StarsI16);
typedef void (*StarsMessageAdvanceTutor)(void *);
typedef void (*StarsMessageObjectAction)(void *, StarsI16, StarsI16,
    StarsI16, StarsI16);
typedef void (*StarsMessagePostCommand)(void *, StarsU16, StarsU16);
typedef void (*StarsMessageSetFocus)(void *, StarsU16);
typedef struct StarsMessageWndOps {
    StarsMessageCreateWindow create_window;
    StarsMessageSendChild send_child_message;
    StarsMessageSetPosition set_position;
    StarsMessageShowChild show_child;
    StarsMessageDefaultProc default_proc;
    StarsMessageAsyncKeyState async_key_state;
    StarsMessageGetPoint get_cursor_pos;
    StarsMessageScreenToClient screen_to_client;
    StarsMessageSetCursor set_cursor;
    StarsMessageGetClientRect get_client_rect;
    StarsMessageFillRect fill_rect;
    StarsMessageSetColor set_background_color;
    StarsMessageBeginPaint begin_paint;
    StarsMessageEndPaint end_paint;
    StarsMessageDrawFrame draw_frame;
    StarsMessageDrawTitle draw_title;
    StarsMessageDrawBody draw_body;
    StarsMessageHitTest hit_test;
    StarsMessageSetText set_window_text;
    StarsMessagePlayerName player_name;
    StarsMessageFormatTitle format_title;
    StarsMessageSetMsgTitle set_msg_title;
    StarsMessageFinishEntry finish_entry;
    StarsMessageMoveSelection next_selection;
    StarsMessageMoveSelection previous_selection;
    StarsMessageGetId message_id;
    StarsMessageDirtyGame dirty_game;
    StarsMessageAdvanceTutor advance_tutor;
    StarsMessageObjectAction object_action;
    StarsMessagePostCommand post_command;
    StarsMessageSetFocus set_focus;
} StarsMessageWndOps;
typedef struct StarsMessageWndState {
    StarsMessageState messages;
    StarsU16 buttons[4];
    StarsU16 dropdown;
    StarsU16 edit;
    StarsU16 scroll;
    StarsU16 font;
    StarsU16 hand_cursor;
    StarsU16 button_brush;
    StarsU32 button_face_color;
    StarsI16 player_count;
    StarsI16 send_mode;
    StarsI16 ai_active;
    StarsI16 dirty;
    StarsI16 tutor_active;
    StarsI16 object_kind;
    StarsI16 object_id;
    StarsI16 object_x;
    StarsI16 object_y;
    StarsI16 object_parameter;
    StarsI16 ship_dropdown_height;
    StarsI16 arial8_height;
    StarsI16 win_frame_width;
    StarsRect message_text;
    StarsRect message_title;
    const StarsU8 *button_class;
    const StarsU8 *combo_class;
    const StarsU8 *edit_class;
    const StarsU8 *scroll_class;
    const StarsMessageWndOps *ops;
    void *context;
} StarsMessageWndState;
StarsI32 stars_message_wnd_proc(StarsMessageWndState *, StarsU16, StarsU16,
    StarsU16, StarsU32);
typedef StarsU16 (*StarsDecorateCreateCompatibleDc)(void *, StarsU16);
typedef StarsU16 (*StarsDecorateSelectObject)(void *, StarsU16, StarsU16);
typedef void (*StarsDecorateDeleteDc)(void *, StarsU16);
typedef StarsU32 (*StarsDecorateSetColor)(void *, StarsU16, StarsU32);
typedef void (*StarsDecorateBitBlt)(void *, StarsU16, StarsI16, StarsI16,
    StarsI16, StarsI16, StarsU16, StarsI16, StarsI16, StarsU32);
typedef void (*StarsDecoratePatBlt)(void *, StarsU16, StarsI16, StarsI16,
    StarsI16, StarsI16, StarsU32);
typedef StarsI16 (*StarsDecorateMessageId)(void *, StarsI16);
typedef struct StarsDecorateMsgTitleOps {
    StarsDecorateCreateCompatibleDc create_compatible_dc;
    StarsDecorateSelectObject select_object;
    StarsDecorateDeleteDc delete_dc;
    StarsDecorateSetColor set_text_color;
    StarsDecorateSetColor set_background_color;
    StarsDecorateBitBlt bit_blt;
    StarsDecoratePatBlt pat_blt;
    StarsDecorateMessageId message_id;
} StarsDecorateMsgTitleOps;
typedef struct StarsDecorateMsgTitleInput {
    StarsU16 hdc;
    StarsRect rect;
    StarsI16 current_message;
    StarsI16 message_count;
    StarsI16 view_filtered;
    StarsU8 *sent_bits;
    StarsU8 *filtered_bits;
    StarsU16 message_bit_bytes;
    StarsU16 game_flags;
    StarsU16 mono_bitmap;
    StarsU16 message_bitmap;
    StarsU16 shadow_brush;
    StarsU16 highlight_brush;
    const StarsDecorateMsgTitleOps *ops;
    void *context;
} StarsDecorateMsgTitleInput;
void stars_decorate_msg_title_bar(StarsDecorateMsgTitleInput *);
enum {
    STARS_MSG_DLG_PAINT = 0x000f,
    STARS_MSG_DLG_ERASE_BACKGROUND = 0x0014,
    STARS_MSG_DLG_CONTROL_COLOR = 0x0019,
    STARS_MSG_DLG_INIT_DIALOG = 0x0110,
    STARS_MSG_DLG_COMMAND = 0x0111,
    STARS_MSG_DLG_EDIT_CONTROL = 0x010c,
    STARS_MSG_DLG_ACCEPT = 1,
    STARS_MSG_DLG_CANCEL = 2,
    STARS_MSG_DLG_HELP = 0x0076,
    STARS_MSG_DLG_COMBO_SELECT = 0x0415,
    STARS_MSG_DLG_EDIT_CONTROL_TYPE = 6,
    STARS_MSG_DLG_HELP_COMMAND = 0x0dbc,
    STARS_MSG_DLG_INVALID_SERIAL_STRING = 0x04cd,
    STARS_MSG_DLG_ALERT_ERROR = 0x0010,
    STARS_MSG_DLG_DRAW_TEXT_FLAGS = 0x0810
};
typedef StarsU16 (*StarsMsgDlgBeginPaint)(void *, StarsU16);
typedef void (*StarsMsgDlgEndPaint)(void *, StarsU16, StarsU16);
typedef void (*StarsMsgDlgGetClientRect)(void *, StarsU16, StarsRect *);
typedef void (*StarsMsgDlgGetTextRect)(void *, StarsU16, StarsU16, StarsRect *);
typedef StarsU16 (*StarsMsgDlgGetDialogItem)(void *, StarsU16, StarsU16);
typedef StarsU16 (*StarsMsgDlgSelectFont)(void *, StarsU16, StarsU16);
typedef void (*StarsMsgDlgSetColor)(void *, StarsU16, StarsU32);
typedef StarsI16 (*StarsMsgDlgGetString)(void *, StarsU16, StarsU8 *, StarsU16);
typedef void (*StarsMsgDlgDrawText)(void *, StarsU16, const StarsU8 *, StarsI16,
    const StarsRect *, StarsU16);
typedef void (*StarsMsgDlgFillRect)(void *, StarsU16, const StarsRect *, StarsU16);
typedef void (*StarsMsgDlgSendItemMessage)(void *, StarsU16, StarsU16,
    StarsU16, StarsU16, StarsU32);
typedef void (*StarsMsgDlgSetWindowText)(void *, StarsU16, const StarsU8 *);
typedef void (*StarsMsgDlgStickyPosition)(void *, StarsU16, StarsPoint *, StarsI16);
typedef StarsI16 (*StarsMsgDlgGetItemText)(void *, StarsU16, StarsU16,
    StarsU8 *, StarsU16);
typedef StarsI16 (*StarsMsgDlgValidateSerial)(void *, const StarsU8 *);
typedef void (*StarsMsgDlgFormatError)(void *, StarsU16, StarsU8 *, StarsU16);
typedef void (*StarsMsgDlgAlert)(void *, const StarsU8 *, StarsI16);
typedef void (*StarsMsgDlgSetFocus)(void *, StarsU16);
typedef void (*StarsMsgDlgEndDialog)(void *, StarsU16, StarsI16);
typedef void (*StarsMsgDlgWinHelp)(void *, StarsU16, StarsU16, StarsU16);
typedef struct StarsMsgDlgOps {
    StarsMsgDlgBeginPaint begin_paint;
    StarsMsgDlgEndPaint end_paint;
    StarsMsgDlgGetClientRect get_client_rect;
    StarsMsgDlgGetTextRect get_text_rect;
    StarsMsgDlgGetDialogItem get_dialog_item;
    StarsMsgDlgSelectFont select_font;
    StarsMsgDlgSetColor set_background_color;
    StarsMsgDlgSetColor set_text_color;
    StarsMsgDlgGetString get_string;
    StarsMsgDlgDrawText draw_text;
    StarsMsgDlgFillRect fill_rect;
    StarsMsgDlgSendItemMessage send_item_message;
    StarsMsgDlgSetWindowText set_window_text;
    StarsMsgDlgStickyPosition sticky_position;
    StarsMsgDlgGetItemText get_item_text;
    StarsMsgDlgValidateSerial validate_serial;
    StarsMsgDlgFormatError format_error;
    StarsMsgDlgAlert alert;
    StarsMsgDlgSetFocus set_focus;
    StarsMsgDlgEndDialog end_dialog;
    StarsMsgDlgWinHelp win_help;
} StarsMsgDlgOps;
typedef struct StarsMsgDlgInput {
    StarsU16 hwnd;
    StarsU16 message;
    StarsU16 wparam;
    StarsU32 lparam;
    StarsU16 edit_control;
    StarsU16 font;
    StarsU16 button_brush;
    StarsU32 button_face_color;
    StarsU16 help_file;
    StarsU16 invalid_serial_string;
    const StarsMsgDlgOps *ops;
    void *context;
} StarsMsgDlgInput;
StarsI16 stars_msg_dlg(const StarsMsgDlgInput *);
enum {
    STARS_SET_MSG_TITLE_INCOMING_EMPTY = 0,
    STARS_SET_MSG_TITLE_INCOMING_LIST = 1,
    STARS_SET_MSG_TITLE_OUTGOING_COUNT = 2,
    STARS_SET_MSG_TITLE_BUTTON_PREVIOUS = 0,
    STARS_SET_MSG_TITLE_BUTTON_ACTION = 1,
    STARS_SET_MSG_TITLE_BUTTON_NEXT = 2,
    STARS_SET_MSG_TITLE_BUTTON_DROPDOWN = 3,
    STARS_SET_MSG_TITLE_OBJECT_PLANET = 1,
    STARS_SET_MSG_TITLE_OBJECT_FLEET = 2,
    STARS_SET_MSG_TITLE_OBJECT_SPECIAL = 3,
    STARS_SET_MSG_TITLE_OBJECT_WAYPOINT = 4,
    STARS_SET_MSG_TITLE_OBJECT_PLANETARY = 5,
    STARS_SET_MSG_TITLE_OBJECT_THING = 6,
    STARS_SET_MSG_TITLE_OBJECT_MASS = 7,
    STARS_SET_MSG_TITLE_OBJECT_BATTLE = 8,
    STARS_SET_MSG_TITLE_OBJECT_OWNER = 9,
    STARS_SET_MSG_TITLE_OBJECT_POINT = 10,
    STARS_SET_MSG_TITLE_OBJECT_HOME = 11,
    STARS_SET_MSG_TITLE_GOTO_BATTLE_ID = 0x0800,
    STARS_SET_MSG_TITLE_SEND_MODE_FLAG = 0x0100
};
#define STARS_SET_MSG_TITLE_GOTO_NONE 0xffffU
#define STARS_SET_MSG_TITLE_GOTO_SPECIAL 0xfffeU
#define STARS_SET_MSG_TITLE_GOTO_PLANETARY 0xfffdU
#define STARS_SET_MSG_TITLE_GOTO_THING 0xfffcU
#define STARS_SET_MSG_TITLE_GOTO_OWNER 0xfffbU
#define STARS_SET_MSG_TITLE_GOTO_POINT 0xfffaU
#define STARS_SET_MSG_TITLE_GOTO_HOME 0xfff9U
typedef struct StarsSetMsgOutgoingView {
    StarsI16 recipient;
    StarsI16 stored_length;
    const StarsU8 *payload;
} StarsSetMsgOutgoingView;
typedef struct StarsSetMsgIncomingView {
    StarsU16 goto_word;
    StarsI16 parameter0;
    StarsI16 parameter1;
} StarsSetMsgIncomingView;
typedef void (*StarsSetMsgShowControl)(void *, StarsU16, StarsI16);
typedef void (*StarsSetMsgEnableControl)(void *, StarsU16, StarsI16);
typedef void (*StarsSetMsgText)(void *, StarsU16, const StarsU8 *);
typedef void (*StarsSetMsgTitleText)(void *, const StarsU8 *);
typedef void (*StarsSetMsgInvalidate)(void *);
typedef void (*StarsSetMsgPosition)(void *, StarsU16, StarsI16, StarsI16,
    StarsI16, StarsI16, StarsU16);
typedef void (*StarsSetMsgFormatTitle)(void *, StarsI16, StarsI16, StarsI16,
    StarsI16, StarsI16, StarsU8 *, StarsU16);
typedef StarsI16 (*StarsSetMsgGetOutgoing)(void *, StarsI16,
    StarsSetMsgOutgoingView *);
typedef StarsI16 (*StarsSetMsgDecodeText)(void *, const StarsU8 *, StarsI16,
    StarsU8 *, StarsU16);
typedef void (*StarsSetMsgSetRecipient)(void *, StarsI16);
typedef void (*StarsSetMsgSetEditor)(void *, const StarsU8 *);
typedef StarsI16 (*StarsSetMsgGetIncoming)(void *, StarsI16,
    StarsSetMsgIncomingView *);
typedef StarsI16 (*StarsSetMsgMessageId)(void *, StarsI16);
typedef StarsI16 (*StarsSetMsgFleetExists)(void *, StarsI16);
typedef void (*StarsSetMsgObject)(void *, StarsI16, StarsI16, StarsI16,
    StarsI16);
typedef struct StarsSetMsgTitleOps {
    StarsSetMsgShowControl show_control;
    StarsSetMsgEnableControl enable_control;
    StarsSetMsgText set_control_text;
    StarsSetMsgTitleText set_title;
    StarsSetMsgInvalidate invalidate;
    StarsSetMsgPosition set_position;
    StarsSetMsgFormatTitle format_title;
    StarsSetMsgGetOutgoing get_outgoing;
    StarsSetMsgDecodeText decode_text;
    StarsSetMsgSetRecipient set_recipient;
    StarsSetMsgSetEditor set_editor;
    StarsSetMsgGetIncoming get_incoming;
    StarsSetMsgMessageId message_id;
    StarsSetMsgFleetExists fleet_exists;
    StarsSetMsgObject set_object;
} StarsSetMsgTitleOps;
typedef struct StarsSetMsgTitleInput {
    StarsU16 hwnd;
    StarsU16 edit_control;
    StarsU16 dropdown_control;
    StarsU16 message_window;
    StarsU16 controls[4];
    StarsU16 scroll_control;
    StarsI16 ai_active;
    StarsI16 dirty;
    StarsI16 send_mode;
    StarsI16 host_mode;
    StarsI16 turn;
    StarsI16 current_message;
    StarsI16 incoming_count;
    StarsI16 current_outgoing;
    StarsI16 outgoing_count;
    StarsI16 incoming_recipient;
    StarsI16 message_view_filtered;
    StarsU8 *filtered_bits;
    StarsU16 filtered_bit_bytes;
    StarsRect message_text_rect;
    StarsI16 ship_dropdown_height;
    const StarsSetMsgTitleOps *ops;
    void *context;
} StarsSetMsgTitleInput;
void stars_set_msg_title(StarsSetMsgTitleInput *);

/* The editor stores at most 999 visible characters.  A plain-text record
 * includes its terminating byte, while a compressed record contains only
 * the encoded bytes.  The sign of stored_length distinguishes those two
 * representations; a negative value represents -(visible_length + 1). */
#define STARS_FINISH_MESSAGE_TEXT_LIMIT 999
#define STARS_FINISH_MESSAGE_PAYLOAD_CAPACITY 1000
typedef struct StarsFinishPlayerMessage {
    StarsI16 sender;
    StarsI16 recipient;
    StarsI16 message_id;
    StarsI16 stored_length;
    StarsU8 payload[STARS_FINISH_MESSAGE_PAYLOAD_CAPACITY];
} StarsFinishPlayerMessage;
typedef StarsI16 (*StarsFinishCompressUserString)(void *, const StarsU8 *,
    StarsU16, StarsU8 *, StarsU16 *);
typedef StarsI16 (*StarsFinishMessageRecipient)(void *);
typedef void (*StarsFinishMessageDirty)(void *);
typedef struct StarsFinishPlrMsgEntryInput {
    StarsFinishPlayerMessage *entries;
    StarsU16 count;
    StarsU16 capacity;
    StarsI16 current;
    StarsI16 direction;
    StarsI16 sender;
    StarsI16 message_id;
    const StarsU8 *editor_text;
    StarsU16 editor_length;
    StarsFinishCompressUserString compress;
    StarsFinishMessageRecipient recipient;
    StarsFinishMessageDirty mark_dirty;
    void *context;
} StarsFinishPlrMsgEntryInput;
StarsI16 stars_finish_plr_msg_entry(StarsFinishPlrMsgEntryInput *);
enum {
    STARS_READ_PLAYER_MESSAGE_KIND = 0x0028,
    STARS_READ_MESSAGE_BUFFER_KIND = 0x000c,
    STARS_READ_RECORD_KIND_SHIFT = 10,
    STARS_READ_RECORD_LENGTH_MASK = 0x03ff,
    STARS_READ_MESSAGE_ID_MASK = 0x01ff,
    STARS_READ_MESSAGE_MASK_SHIFT = 9,
    STARS_READ_MESSAGE_MASK_BITS = 7,
    STARS_READ_SENT_BIT_BYTES = 64,
    STARS_READ_PLAYER_MESSAGE_PAYLOAD_CAPACITY = 1024
};
typedef struct StarsReadMessageStreamRecord {
    StarsU16 kind;
    StarsU16 bytes;
    const StarsU8 *data;
} StarsReadMessageStreamRecord;
typedef struct StarsReadIncomingPlayerMessage {
    StarsU16 bytes;
    StarsU8 payload[STARS_READ_PLAYER_MESSAGE_PAYLOAD_CAPACITY];
} StarsReadIncomingPlayerMessage;
typedef StarsU16 (*StarsReadMessageParameterCount)(void *, StarsI16);
typedef StarsI16 (*StarsReadMessageSelectFirst)(void *, StarsI16);
typedef struct StarsReadPlayerMessagesInput {
    const StarsReadMessageStreamRecord *message_records;
    StarsU16 message_record_count;
    const StarsReadMessageStreamRecord *incoming_records;
    StarsU16 incoming_record_count;
    StarsU8 *message_buffer;
    StarsU16 message_capacity;
    StarsU16 message_bytes;
    StarsU16 message_count;
    StarsU8 *sent_bits;
    StarsU16 sent_bit_bytes;
    StarsReadIncomingPlayerMessage *incoming;
    StarsU16 incoming_capacity;
    StarsU16 incoming_count;
    StarsI16 current_message;
    StarsReadMessageParameterCount parameter_count;
    StarsReadMessageSelectFirst select_first;
    void *context;
} StarsReadPlayerMessagesInput;
StarsI16 stars_read_player_messages(StarsReadPlayerMessagesInput *);
typedef struct StarsMessageOps {
    StarsI16 (*message_id)(void *, StarsI16);
    StarsU16 (*parameter_count)(void *, StarsI16);
    StarsU16 (*player_flags)(void *, StarsI16);
    void (*append_message)(void *, const StarsU8 *, StarsU16);
    void (*prepend_message)(void *, const StarsU8 *, StarsU16);
} StarsMessageOps;
typedef struct StarsMessageBig {
    StarsI16 message;
    StarsI16 goto_id;
    StarsI16 parameters[7];
} StarsMessageBig;
StarsI16 stars_get_message_big(const StarsU8 *, StarsU16, StarsU16, StarsI16,
    const StarsMessageOps *, void *, StarsMessageBig *);
StarsI16 stars_remove_player_message(StarsU8 *, StarsU16, StarsI16, StarsI16,
    StarsI16);
StarsI16 stars_find_player_message(const StarsU8 *, StarsU16, StarsI16,
    StarsI16, StarsI16);
typedef struct StarsMessageResetState {
    StarsU16 message_memory;
    StarsI16 current_message;
    StarsU16 message_count;
    StarsI16 send_current;
    StarsU8 sent[49];
    StarsU8 filtered[49];
    void *incoming_list;
    StarsU16 incoming_segment;
    void *outgoing_list;
    StarsU16 outgoing_segment;
    StarsU16 incoming_count;
    StarsU16 outgoing_count;
} StarsMessageResetState;
void stars_reset_messages(StarsMessageResetState *);
typedef struct StarsPlayerMessage {
    StarsI16 from;
    StarsI16 to;
} StarsPlayerMessage;
void stars_mark_players_that_sent_messages(StarsI16, const StarsPlayerMessage *,
    StarsU16, StarsU16 *, StarsU16);
typedef struct StarsOutgoingPlayerMessage {
    StarsI16 from;
    StarsI16 to;
    StarsI16 compressed_length;
    const StarsU8 *data;
} StarsOutgoingPlayerMessage;
typedef void (*StarsWriteMessageRecord)(void *, StarsU16, StarsU16,
    const void *);
void stars_write_player_messages(const StarsU8 *, StarsU16, StarsI16,
    const StarsOutgoingPlayerMessage *, StarsU16, StarsWriteMessageRecord,
    void *);
typedef void *(*StarsLookupPlanet)(void *, StarsU16);
typedef void (*StarsMarkLostPlanet)(void *, void *, StarsI16, StarsI16);
typedef struct StarsPlanetLossOps {
    StarsLookupPlanet lookup;
    StarsMarkLostPlanet mark;
} StarsPlanetLossOps;
void stars_mark_planets_player_lost(const StarsU8 *, StarsU16, StarsI16,
    const StarsPlanetLossOps *, void *);
StarsI16 stars_message_next(const StarsMessageState *, StarsI16,
    const StarsMessageOps *, void *);
StarsI16 stars_message_previous(const StarsMessageState *, StarsI16,
    const StarsMessageOps *, void *);
StarsI16 stars_package_player_message(const StarsMessageState *, StarsU8 *, StarsI16,
    StarsI16, StarsI16, const StarsI16 *, const StarsMessageOps *, void *);
StarsI16 stars_send_player_message(StarsMessageState *, StarsI16, StarsI16, StarsI16,
    const StarsI16 *, const StarsMessageOps *, void *);
StarsI16 stars_send_player_message_prepend(StarsMessageState *, StarsI16, StarsI16,
    StarsI16, const StarsI16 *, const StarsMessageOps *, void *);
StarsI16 stars_send_player_message2_xgen(StarsMessageState *, StarsI16, StarsI16,
    StarsI16, StarsI16, StarsI16, const StarsMessageOps *, void *);
StarsI16 stars_send_player_message2(StarsMessageState *, StarsI16, StarsI16, StarsI16,
    StarsI16, StarsI16, const StarsMessageOps *, void *);
typedef void (*StarsPrintMapGetWindowText)(void *, StarsU16, StarsU8 *,
    StarsU16);
typedef void (*StarsPrintMapSendMessage)(void *, StarsU16, StarsU16,
    StarsU16, StarsU32);
typedef void (*StarsPrintMapSetBkColor)(void *, StarsU16, StarsU32);
typedef void (*StarsPrintMapSticky)(void *, StarsU16, StarsU16);
typedef void (*StarsPrintMapBeep)(void *, StarsU16);
typedef struct StarsPrintMapDlgInput {
    StarsU16 page[2];
    StarsU16 button_face_brush;
    StarsU32 button_face_color;
    StarsU16 font_arial8;
    const StarsU8 *help_file;
    StarsGetClientRect get_client_rect;
    StarsFillRect fill_rect;
    StarsPrintMapSetBkColor set_background_color;
    StarsGetDialogItem get_dialog_item;
    StarsPrintMapSendMessage send_message;
    StarsSetWindowText set_window_text;
    StarsPrintMapGetWindowText get_window_text;
    StarsSetFocusWindow set_focus;
    StarsFormatIds format_ids;
    StarsAlertText alert;
    StarsWinHelp win_help;
    StarsPrintMapSticky sticky_dialog;
    StarsEndDialog end_dialog;
    StarsPrintMapBeep message_beep;
    void *context;
} StarsPrintMapDlgInput;
StarsI16 stars_print_map_dlg(StarsPrintMapDlgInput *, StarsU16, StarsU16,
    StarsU16, StarsU32);
typedef void (*StarsMassGaugeSelectObject)(void *, StarsU16, StarsU16);
typedef StarsI32 (*StarsMassGaugeDrawGauge)(void *, StarsU16,
    const StarsRect *, StarsI16, StarsU16, StarsI32);
typedef StarsU16 (*StarsMassGaugeSetBkMode)(void *, StarsU16, StarsU16);
typedef StarsI16 (*StarsMassGaugeFormat)(void *, StarsI32, StarsU8 *, StarsU16);
typedef void (*StarsMassGaugeTextExtent)(void *, StarsU16, const StarsU8 *,
    StarsI16);
typedef void (*StarsMassGaugeCenterText)(void *, StarsU16, const StarsRect *,
    const StarsU8 *, StarsI16);
typedef struct StarsDrawMassWarpGaugeInput {
    StarsU16 hdc;
    const StarsRect *rect;
    StarsI16 best;
    StarsI16 current;
    StarsU16 font_arial8;
    StarsU16 brush_yellow;
    StarsU16 brush_red;
    StarsU16 brush_purple;
    StarsMassGaugeSelectObject select_object;
    StarsMassGaugeDrawGauge draw_gauge;
    StarsMassGaugeSetBkMode set_bk_mode;
    StarsMassGaugeFormat format_value;
    StarsMassGaugeTextExtent get_text_extent;
    StarsMassGaugeCenterText center_text;
    void *context;
} StarsDrawMassWarpGaugeInput;
void stars_draw_mass_warp_gauge(StarsDrawMassWarpGaugeInput *);
typedef struct StarsUpdateSlotGlobalsHull {
    StarsU8 component_count;
    const StarsU8 *component_rects;
    StarsU16 cargo_weight_max;
    StarsU16 cargo_rect;
} StarsUpdateSlotGlobalsHull;
typedef StarsI16 (*StarsUpdateSlotLookupHull)(void *, StarsU16,
    StarsUpdateSlotGlobalsHull *);
typedef struct StarsUpdateSlotGlobalsInput {
    StarsU16 has_build_design;
    StarsU16 build_hull_id;
    StarsU16 slot_dialog;
    StarsI16 arial8_height;
    StarsPoint slot_origin;
    StarsRect *slot_rects;
    StarsRect *cargo_rect;
    StarsPoint *plaque;
    StarsUpdateSlotLookupHull lookup_hull;
    void *context;
} StarsUpdateSlotGlobalsInput;
void stars_update_slot_globals(StarsUpdateSlotGlobalsInput *);
typedef struct StarsTutorialResource {
    const StarsU8 *data;
    StarsU16 size;
} StarsTutorialResource;
typedef StarsU16 (*StarsTutorialFindResource)(void *, StarsU16, StarsU16);
typedef StarsU16 (*StarsTutorialLoadResource)(void *, StarsU16);
typedef const StarsTutorialResource *(*StarsTutorialLockResource)(void *,
    StarsU16);
typedef void (*StarsTutorialUnlockResource)(void *, StarsU16);
typedef void (*StarsTutorialFreeResource)(void *, StarsU16);
typedef StarsI16 (*StarsTutorialMakeFilename)(void *, StarsU8 *, StarsU16);
typedef StarsI16 (*StarsTutorialOpenStream)(void *, const StarsU8 *, StarsU16);
typedef void (*StarsTutorialWriteStream)(void *, const StarsU8 *, StarsU16);
typedef void (*StarsTutorialCloseStream)(void *);
typedef struct StarsWriteTutorialMFileInput {
    StarsI16 turn;
    StarsTutorialFindResource find_resource;
    StarsTutorialLoadResource load_resource;
    StarsTutorialLockResource lock_resource;
    StarsTutorialUnlockResource unlock_resource;
    StarsTutorialFreeResource free_resource;
    StarsTutorialMakeFilename make_filename;
    StarsTutorialOpenStream open_stream;
    StarsTutorialWriteStream write_stream;
    StarsTutorialCloseStream close_stream;
    void *context;
} StarsWriteTutorialMFileInput;
StarsI16 stars_write_tutorial_m_file(const StarsWriteTutorialMFileInput *);
typedef struct StarsJammerComponent {
    StarsU16 group;
    StarsU16 packed_count_type;
} StarsJammerComponent;
typedef StarsI16 (*StarsJammerPartDp)(void *, StarsU16, StarsU16);
typedef struct StarsPctJammerFromHulInput {
    StarsU16 hull_id;
    const StarsJammerComponent *components;
    StarsU16 component_count;
    StarsJammerPartDp part_dp;
    void *context;
} StarsPctJammerFromHulInput;
StarsI16 stars_pct_jammer_from_hul(const StarsPctJammerFromHulInput *);
typedef struct StarsStartupFleetState {
    StarsU8 bytes[124];
} StarsStartupFleetState;
typedef StarsStartupFleetState *(*StarsStartupCreateFleet)(void *, StarsI16,
    StarsI16);
typedef StarsI32 (*StarsStartupFleetStat)(void *, const StarsStartupFleetState *,
    StarsU16);
typedef struct StarsCreateStartupShipInput {
    StarsI16 player;
    StarsI16 planet;
    StarsI16 design_index;
    StarsU16 add_design;
    StarsU8 *design_count;
    StarsU8 *design_table;
    StarsU16 design_capacity;
    const StarsU16 *design_words;
    StarsU16 design_word_count;
    StarsU8 design_hull_id;
    StarsStartupCreateFleet create_fleet;
    StarsStartupFleetStat fleet_stat;
    void *context;
} StarsCreateStartupShipInput;
StarsI16 stars_create_startup_ship(const StarsCreateStartupShipInput *);
typedef StarsI16 (*StarsTutorWorldString)(void *, StarsU16, StarsU8 *, StarsU16);
typedef void (*StarsTutorWorldFormatNames)(void *, StarsU8 *, StarsU16);
typedef void (*StarsTutorWorldUnlink)(void *, StarsU16, StarsI16);
typedef void (*StarsTutorWorldRandomize)(void *, StarsU32);
typedef void (*StarsTutorWorldGenerate)(void *, StarsI16);
typedef struct StarsCreateTutorWorldInput {
    StarsU16 *player_count;
    StarsU16 *density;
    StarsU16 *size;
    StarsU16 *start_distance;
    StarsU16 *game_flags;
    StarsU32 *game_lid;
    StarsU8 *victory_conditions;
    StarsU8 *player0;
    StarsU8 *player1;
    const StarsU8 *default_player;
    const StarsU8 *competitor_player;
    StarsU8 *game_name;
    StarsU8 *player0_name;
    StarsU8 *player0_names;
    StarsU8 *player1_name;
    StarsTutorWorldString get_string;
    StarsTutorWorldFormatNames format_names;
    StarsTutorWorldUnlink unlink_file;
    StarsTutorWorldRandomize randomize;
    StarsTutorWorldGenerate generate_world;
    void *context;
} StarsCreateTutorWorldInput;
void stars_create_tutor_world(const StarsCreateTutorWorldInput *);
typedef void (*StarsKillQueuedFree)(void *, StarsU32 *);
typedef void (*StarsKillQueuedRefresh)(void *, StarsI16, const StarsU32 *,
    StarsU16);
typedef struct StarsKillQueuedMassPacketsInput {
    StarsI16 planet_id;
    StarsU32 *production_records;
    StarsU16 *production_count;
    StarsU16 selected_group;
    StarsI16 selected_planet_id;
    StarsKillQueuedFree free_production;
    StarsKillQueuedRefresh refresh_production;
    void *context;
} StarsKillQueuedMassPacketsInput;
void stars_kill_queued_mass_packets(StarsKillQueuedMassPacketsInput *);
void stars_kill_queued_ships(StarsKillQueuedMassPacketsInput *);
typedef StarsU16 (*StarsDrawDlgStock)(void *, StarsU16);
typedef void (*StarsDrawDlgFill)(void *, StarsU16, const StarsRect *, StarsU16);
typedef void (*StarsDrawDlgSend)(void *, StarsU16, StarsU16, StarsU16,
    StarsU32);
typedef void (*StarsDrawDlgPalette)(void *, StarsU16, StarsU16);
typedef void (*StarsDrawDlgRealizePalette)(void *, StarsU16);
typedef void (*StarsDrawDlgBitmap)(void *, StarsU16, StarsI16, StarsI16,
    StarsI16, StarsI16, StarsU16, StarsI16, StarsI16, StarsI16, StarsI16,
    StarsU32);
typedef StarsU32 (*StarsDrawDlgTextColor)(void *, StarsU16, StarsU32);
typedef StarsU16 (*StarsDrawDlgBkMode)(void *, StarsU16, StarsU16);
typedef void (*StarsDrawDlgTextOut)(void *, StarsU16, StarsI16, StarsI16,
    const StarsU8 *, StarsU16);
typedef void (*StarsDrawDlgFocus)(void *, const StarsRect *, StarsI16);
typedef struct StarsDrawDlgLBEntireItemInput {
    StarsU16 hdc;
    StarsU16 hwnd_item;
    StarsU16 item_id;
    StarsU16 item_state;
    StarsRect item_rect;
    StarsI16 inflate;
    StarsI16 arial8_height;
    StarsU16 palette;
    StarsU32 window_color;
    StarsU16 glyph_x;
    StarsU16 glyph_y;
    const StarsU8 *text;
    StarsDrawDlgStock get_stock_object;
    StarsDrawDlgFill fill_rect;
    StarsDrawDlgSend send_message;
    StarsDrawDlgPalette select_palette;
    StarsDrawDlgRealizePalette realize_palette;
    StarsDrawDlgBitmap dib_blt;
    StarsDrawDlgTextColor set_text_color;
    StarsDrawDlgBkMode set_bk_mode;
    StarsDrawDlgTextOut text_out;
    StarsDrawDlgFocus handle_focus;
    void *context;
} StarsDrawDlgLBEntireItemInput;
void stars_draw_dlg_lb_entire_item(const StarsDrawDlgLBEntireItemInput *);
typedef StarsI16 (*StarsWriteLogCreate)(void *, StarsI16, const StarsU8 *);
typedef void (*StarsWriteLogMemory)(void *, StarsU16, StarsU16,
    const StarsU8 *);
typedef void (*StarsWriteLogRecord)(void *, StarsU16, StarsU16, const void *);
typedef void (*StarsWriteLogAlert)(void *, StarsU16, StarsI16);
typedef void (*StarsWriteLogClose)(void *);
typedef void (*StarsWriteLogDirty)(void *);
typedef struct StarsWriteLogFileInput {
    const StarsU8 *file_base;
    StarsI16 player;
    StarsI16 current_player;
    StarsU16 player_flags;
    StarsU16 previous_log_type;
    const StarsU8 *current_queue;
    const StarsU8 *previous_queue;
    StarsU16 queue_count_byte;
    StarsU32 serial;
    const StarsU8 *environment;
    const StarsU8 *log;
    StarsU16 log_size;
    const StarsOutgoingPlayerMessage *messages;
    StarsU16 message_count;
    StarsU16 *game_flags;
    StarsWriteLogCreate create_file;
    StarsWriteLogMemory write_memory;
    StarsWriteLogRecord write_record;
    StarsWriteLogAlert alert;
    StarsWriteLogClose close_stream;
    StarsWriteLogDirty dirty_game;
    void *context;
} StarsWriteLogFileInput;
StarsI16 stars_write_log_file(const StarsWriteLogFileInput *);
typedef struct StarsHistoryPlanet {
    StarsU16 owner_flags;
    const void *record;
} StarsHistoryPlanet;
typedef struct StarsHistoryDesign {
    StarsU32 flags;
    const void *record;
} StarsHistoryDesign;
typedef struct StarsHistoryScore {
    StarsU16 turn;
    const void *record;
} StarsHistoryScore;
typedef struct StarsHistoryPlayer {
    StarsU16 detection_flags;
    StarsU16 fleet_count;
    const void *record;
    StarsHistoryDesign designs[16];
    StarsHistoryDesign enemy_designs[10];
    const StarsHistoryScore *scores;
    StarsU16 score_count;
} StarsHistoryPlayer;
typedef StarsI16 (*StarsHistoryCreate)(void *, StarsI16);
typedef void (*StarsHistoryPlanetWrite)(void *, StarsU16, const void *);
typedef void (*StarsHistoryPlayerWrite)(void *, const void *);
typedef void (*StarsHistoryDesignWrite)(void *, const void *);
typedef void (*StarsHistoryScoreWrite)(void *, const void *);
typedef void (*StarsHistoryRecordWrite)(void *, StarsU16, StarsU16,
    const void *);
typedef void (*StarsHistoryClose)(void *);
typedef void (*StarsHistoryAlert)(void *, StarsU16, StarsI16);
typedef struct StarsWriteHistFileInput {
    StarsI16 player;
    StarsU16 planet_count;
    StarsU16 game_player_count;
    StarsU16 game_turn;
    const StarsHistoryPlanet *planets;
    const StarsHistoryPlayer *players;
    StarsU16 message_bits_count;
    const void *message_bits;
    const StarsU8 *ai_data;
    StarsU32 ai_size;
    StarsHistoryCreate create_file;
    StarsHistoryPlanetWrite write_planet;
    StarsHistoryRecordWrite write_record;
    StarsHistoryPlayerWrite write_player;
    StarsHistoryDesignWrite write_design;
    StarsHistoryScoreWrite write_score;
    StarsHistoryAlert alert;
    StarsHistoryClose close_stream;
    void *context;
} StarsWriteHistFileInput;
StarsI16 stars_write_hist_file(const StarsWriteHistFileInput *);
/* DumpUniverse's report-facing state.  The original routine writes a map
 * file from the live planet index and reports either success or failure. */
typedef struct StarsDumpUniversePlanet {
    StarsI16 id;
    StarsI16 x;
    StarsI16 y;
} StarsDumpUniversePlanet;
typedef const StarsU8 *(*StarsDumpUniversePlanetName)(void *, StarsI16);
typedef StarsI16 (*StarsDumpUniverseOpen)(void *, const StarsU8 *, StarsU16);
typedef void (*StarsDumpUniverseWrite)(void *, const StarsU8 *, StarsU16);
typedef void (*StarsDumpUniverseClose)(void *);
typedef void (*StarsDumpUniverseAlert)(void *, StarsU16, StarsI16);
typedef struct StarsDumpUniverseInput {
    StarsU32 game_id;
    StarsI16 player;
    const StarsU8 *base_name;
    const StarsDumpUniversePlanet *planets;
    StarsU16 planet_count;
    StarsDumpUniversePlanetName planet_name;
    StarsDumpUniverseOpen open_stream;
    StarsDumpUniverseWrite write_stream;
    StarsDumpUniverseClose close_stream;
    StarsDumpUniverseAlert alert;
    StarsU16 *file_error_silent;
    void *context;
} StarsDumpUniverseInput;
void stars_dump_universe(const StarsDumpUniverseInput *);
typedef void (*StarsDrawTileFrame)(void *, StarsU16, const StarsRect *,
    StarsU16);
typedef void (*StarsDrawTileSelect)(void *, StarsU16, StarsU16);
typedef void (*StarsDrawTileColor)(void *, StarsU16, StarsU32);
typedef void (*StarsDrawTileTitle)(void *, StarsU16, const StarsRect *,
    const StarsU8 *);
typedef void (*StarsDrawTileButton)(void *, StarsU16, const StarsRect *,
    StarsU16);
typedef void (*StarsDrawTilePatBlt)(void *, StarsU16, StarsI16, StarsI16,
    StarsI16, StarsI16, StarsU32);
typedef struct StarsDrawTileNCInput {
    StarsU16 hdc;
    StarsU16 tile_flags;
    StarsI16 y_top;
    StarsI16 dy_full;
    StarsI16 arial8_height;
    StarsU16 font_arial8;
    StarsU32 text_color;
    StarsU32 face_color;
    StarsU16 button_shadow_brush;
    const StarsU8 *title;
    StarsRect *rect;
    StarsDrawTileFrame draw_frame;
    StarsDrawTileSelect select_object;
    StarsDrawTileColor set_text_color;
    StarsDrawTileColor set_background_color;
    StarsDrawTileTitle center_title;
    StarsDrawTileButton draw_button;
    StarsDrawTilePatBlt pat_blt;
    void *context;
} StarsDrawTileNCInput;
StarsI16 stars_draw_tile_nc(StarsDrawTileNCInput *);

/* Semantic inputs for DrawPlanetStarbase (NE 0A:22CC-2AFA).  The tile
 * renderer owns the common frame, while these records expose the starbase
 * values and the small set of drawing services used for its six detail rows. */
typedef struct StarsPlanetStarbaseTile {
    StarsU16 flags;
} StarsPlanetStarbaseTile;
typedef struct StarsPlanetStarbaseData {
    StarsI16 has_starbase;
    StarsI16 hull_id;
    StarsI16 starbase_design;
    StarsU16 cargo_capacity;
    StarsU16 defense;
    StarsU16 mass_warp_packed;
    StarsI16 planet_id;
    StarsI16 player_id;
    StarsI16 destination_id;
    StarsU8 hull_class[32];
} StarsPlanetStarbaseData;
typedef const StarsU8 *(*StarsPlanetStarbaseString)(void *, StarsU16);
typedef StarsI16 (*StarsPlanetStarbaseGetText)(void *, StarsU16, StarsU8 *,
    StarsU16);
typedef StarsI16 (*StarsPlanetStarbaseDrawTile)(void *, StarsU16,
    const StarsPlanetStarbaseTile *, const StarsU8 *, StarsRect *);
typedef void (*StarsPlanetStarbaseSelectFont)(void *, StarsU16, StarsU16);
typedef void (*StarsPlanetStarbaseTextOut)(void *, StarsU16, StarsI16,
    StarsI16, const StarsU8 *, StarsI16);
typedef void (*StarsPlanetStarbaseRightText)(void *, StarsU16, StarsI16,
    StarsI16, const StarsU8 *, StarsI16, StarsI16);
typedef StarsI32 (*StarsPlanetStarbaseTextExtent)(void *, StarsU16,
    const StarsU8 *, StarsI16);
typedef void (*StarsPlanetStarbaseSetColor)(void *, StarsU16, StarsU32);
typedef void (*StarsPlanetStarbasePatBlt)(void *, StarsU16, StarsI16,
    StarsI16, StarsI16, StarsI16, StarsU32);
typedef void (*StarsPlanetStarbaseSetReference)(void *, StarsU16,
    const StarsRect *);
typedef void (*StarsPlanetStarbaseButton)(void *, StarsU16, const StarsRect *,
    StarsI16, StarsI16);
typedef void (*StarsPlanetStarbaseGauge)(void *, StarsU16, const StarsRect *,
    StarsI16, StarsI16);
typedef StarsI32 (*StarsPlanetStarbaseShield)(void *, StarsI16, StarsI16);
typedef StarsI16 (*StarsPlanetStarbaseWarp)(void *, StarsI16, StarsI16 *);
typedef StarsI16 (*StarsPlanetStarbaseGetPlanetName)(void *, StarsI16,
    StarsU8 *, StarsU16);
typedef StarsI16 (*StarsPlanetStarbaseFormat)(void *, StarsU8 *, StarsU16,
    StarsI32, StarsI16);
typedef struct StarsPlanetStarbaseOps {
    StarsPlanetStarbaseString compressed_string;
    StarsPlanetStarbaseGetText get_text;
    StarsPlanetStarbaseDrawTile draw_tile;
    StarsPlanetStarbaseSelectFont select_font;
    StarsPlanetStarbaseTextOut text_out;
    StarsPlanetStarbaseRightText right_text;
    StarsPlanetStarbaseTextExtent text_extent;
    StarsPlanetStarbaseSetColor set_text_color;
    StarsPlanetStarbasePatBlt pat_blt;
    StarsPlanetStarbaseSetReference set_reference;
    StarsPlanetStarbaseButton draw_button;
    StarsPlanetStarbaseGauge draw_gauge;
    StarsPlanetStarbaseShield shield_value;
    StarsPlanetStarbaseWarp warp_value;
    StarsPlanetStarbaseGetPlanetName get_planet_name;
    StarsPlanetStarbaseFormat format_value;
    void *context;
} StarsPlanetStarbaseOps;
typedef struct StarsPlanetStarbaseInput {
    StarsU16 hdc;
    StarsI16 line_height;
    StarsU16 normal_font;
    StarsU16 bold_font;
    StarsU32 highlight_color;
    StarsI16 button_enabled;
    StarsU16 button_highlight_brush;
    StarsPlanetStarbaseTile *tile;
    const StarsPlanetStarbaseData *data;
    StarsRect *reference_rects;
    const StarsPlanetStarbaseOps *ops;
} StarsPlanetStarbaseInput;
/* Packed planet/starbase view used before DrawPlanetStarbase receives its
 * typed display data.  The raw offsets and record strides are the recovered
 * Win16 storage boundary: _planet is 56 bytes, _shdef is 147 bytes, and
 * _huldef is 143 bytes. */
typedef struct StarsPlanetStarbasePackedInput {
    const StarsU8 *planet;
    const StarsU8 *starbase_designs;
    const StarsU8 *starbase_hulls;
    StarsU16 design_count;
    StarsU16 hull_count;
} StarsPlanetStarbasePackedInput;
StarsI16 stars_decode_planet_starbase(
    const StarsPlanetStarbasePackedInput *, StarsPlanetStarbaseData *);
void stars_draw_planet_starbase(StarsPlanetStarbaseInput *);

typedef struct StarsDeleteWpFarInput {
    StarsU16 cord;
    StarsU8 *iord_mac;
    StarsU8 *records;
} StarsDeleteWpFarInput;
void stars_delete_wp_far(StarsDeleteWpFarInput *, StarsU16, StarsU16);

typedef void (*StarsDeleteCurBeep)(void *, StarsU16);
typedef void (*StarsDeleteCurRedraw)(void *, StarsU16, StarsU16);
typedef void (*StarsDeleteCurLookupFleet)(void *, StarsI16, StarsU8 *);
typedef void (*StarsDeleteCurFindNearest)(void *, StarsPoint, StarsU16,
    StarsScanSelection *);
typedef void (*StarsDeleteCurChangeSelection)(void *, const StarsScanSelection *,
    StarsI16);
typedef void (*StarsDeleteCurLogicalToScan)(void *, StarsPoint *);
typedef void (*StarsDeleteCurBoundPoints)(void *, StarsRect *,
    const StarsPoint *, StarsI16);
typedef void (*StarsDeleteCurInvalidate)(void *, StarsU16, const StarsRect *,
    StarsI16);
typedef struct StarsDeleteCurWayPointOps {
    StarsDeleteCurBeep message_beep;
    StarsDeleteCurRedraw redraw_scan_selection;
    StarsDeleteCurLookupFleet lookup_fleet;
    StarsDeleteCurFindNearest find_nearest;
    StarsDeleteCurChangeSelection change_selection;
    StarsDeleteCurLogicalToScan logical_to_scan;
    StarsDeleteCurBoundPoints bound_points;
    StarsDeleteCurInvalidate invalidate_rect;
} StarsDeleteCurWayPointOps;
typedef struct StarsDeleteCurWayPointInput {
    StarsU16 cord;
    StarsI16 active_waypoint;
    StarsU8 *iord_mac;
    StarsU8 *records;
    StarsU8 *fleet;
    StarsU16 scanner_window;
    StarsU16 scan_flags;
    const StarsDeleteCurWayPointOps *ops;
    void *context;
} StarsDeleteCurWayPointInput;
void stars_delete_cur_waypoint(StarsDeleteCurWayPointInput *, StarsI16);

typedef struct StarsIdealWarpEngine {
    StarsI16 fuel_used[12];
    StarsI16 ore_cost[14]; /* Legacy layout only; ideal warp reads fuel_used. */
} StarsIdealWarpEngine;
typedef struct StarsIdealWarpDesign {
    StarsU16 engine_count;
    StarsU8 engine_flags[10];
    StarsU8 engine_ids[10];
} StarsIdealWarpDesign;
typedef const StarsIdealWarpEngine *(*StarsLookupIdealWarpEngine)(void *,
    StarsU16);
typedef struct StarsFindIdealWarpInput {
    StarsI16 ship_counts[16];
    StarsIdealWarpDesign designs[16];
    StarsLookupIdealWarpEngine lookup_engine;
    void *context;
} StarsFindIdealWarpInput;
StarsI16 stars_find_ideal_warp(const StarsFindIdealWarpInput *, StarsI16);

typedef void (*StarsFakeListCursorPos)(void *, StarsU16, StarsPoint *);
typedef void (*StarsFakeListScreenToClient)(void *, StarsU16, StarsPoint *);
typedef void (*StarsFakeListSetCursor)(void *, StarsU16);
typedef StarsI32 (*StarsFakeListCallWindowProc)(void *, StarsU32, StarsU16,
    StarsU16, StarsU32);
typedef void (*StarsFakeListTrackSlot)(void *, StarsU16, StarsU16, StarsI16,
    StarsU16, StarsI16, StarsI16);
typedef StarsU32 (*StarsFakeListSendMessage)(void *, StarsU16, StarsU16,
    StarsU16, StarsU32);
typedef void (*StarsFakeListLookupPart)(void *, StarsU8 *, StarsU16);
typedef void (*StarsFakeListPopup)(void *, StarsU16, StarsU16, StarsI16);
typedef struct StarsFakeListProcOps {
    StarsFakeListCursorPos get_cursor_pos;
    StarsFakeListScreenToClient screen_to_client;
    StarsFakeListSetCursor set_cursor;
    StarsFakeListCallWindowProc call_window_proc;
    StarsFakeListTrackSlot track_slot;
    StarsFakeListSendMessage send_message;
    StarsFakeListLookupPart lookup_part;
    StarsFakeListPopup popup;
} StarsFakeListProcOps;
typedef struct StarsFakeListProcInput {
    StarsU16 build_mode;
    StarsU16 hand_cursor;
    StarsU8 work[2];
    StarsU8 global_part[4];
    StarsU16 global_popup;
    StarsU32 real_list_proc;
    const StarsFakeListProcOps *ops;
    void *context;
} StarsFakeListProcInput;
StarsI32 stars_fake_list_proc(StarsFakeListProcInput *, StarsU16, StarsU16,
    StarsU16, StarsU32);

typedef void (*StarsFinishProductionFree)(void *, StarsU8 *);
typedef void (*StarsFinishProductionLookup)(void *, StarsI16, StarsU8 *);
typedef void (*StarsFinishProductionFill)(void *);
typedef void (*StarsFinishProductionDraw)(void *, StarsU16, StarsU16);
typedef void (*StarsFinishProductionTutor)(void *);
typedef struct StarsFinishProductionOps {
    StarsFinishProductionFree free_product;
    StarsFinishProductionLookup lookup_planet;
    StarsFinishProductionFill fill_list;
    StarsFinishProductionDraw draw_ship;
    StarsFinishProductionTutor advance_tutor;
} StarsFinishProductionOps;
typedef struct StarsFinishProductionInput {
    StarsI16 write;
    StarsU32 planet_bits;
    StarsU16 game_flags;
    StarsI16 player;
    StarsI16 planet_id;
    StarsI16 ai_active;
    StarsU16 tutor_flags;
    StarsU8 *selected_product;
    StarsU8 *global_product;
    StarsU8 global_count;
    const StarsFinishProductionOps *ops;
    void *context;
} StarsFinishProductionInput;
void stars_finish_production(StarsFinishProductionInput *);

typedef void (*StarsChangeProductionInitialize)(void *, StarsU8 *, StarsU16);
typedef StarsI16 (*StarsChangeProductionDialog)(void *, StarsU8 *, StarsU16);
typedef void (*StarsChangeProductionRelease)(void *);
typedef void (*StarsChangeProductionEndDialog)(void *, StarsU16, StarsI16);
typedef void (*StarsChangeProductionFinish)(void *, StarsI16);
typedef void (*StarsChangeProductionDraw)(void *, StarsU16, StarsU16);
typedef void (*StarsChangeProductionFree)(void *, StarsU8 *);
typedef const StarsU8 *(*StarsChangeProductionFormat)(void *, StarsU16);
typedef void (*StarsChangeProductionAlert)(void *, const StarsU8 *, StarsI16);
typedef struct StarsChangeProductionOps {
    StarsChangeProductionInitialize initialize;
    StarsChangeProductionDialog dialog;
    StarsChangeProductionRelease release_dialog_proc;
    StarsChangeProductionEndDialog end_dialog;
    StarsChangeProductionFinish finish;
    StarsChangeProductionDraw draw_ship;
    StarsChangeProductionFree free_product;
    StarsChangeProductionFormat format_ids;
    StarsChangeProductionAlert alert;
} StarsChangeProductionOps;
typedef struct StarsChangeProductionInput {
    StarsI16 clear;
    StarsU8 *product_buffer;
    StarsU16 product_buffer_size;
    StarsU8 *global_product;
    StarsU16 dialog_window;
    StarsI16 dialog_up;
    StarsI16 selected_object;
    StarsI16 exception;
    void *saved_environment;
    void *current_environment;
    const StarsChangeProductionOps *ops;
    void *context;
} StarsChangeProductionInput;
StarsI16 stars_change_production(StarsChangeProductionInput *);

typedef void (*StarsPopupWindowGetRect)(void *, StarsU16, StarsRect *);
typedef void (*StarsPopupWindowFillRect)(void *, StarsU16, const StarsRect *,
    StarsU16);
typedef StarsU16 (*StarsPopupWindowBeginPaint)(void *, StarsU16, StarsRect *);
typedef void (*StarsPopupWindowDraw)(void *, StarsU16, StarsU16);
typedef void (*StarsPopupWindowEndPaint)(void *, StarsU16, const StarsRect *);
typedef void (*StarsPopupWindowDestroy)(void *, StarsU16);
typedef void (*StarsPopupWindowReleaseCapture)(void *);
typedef void (*StarsPopupWindowAdvanceTutor)(void *);
typedef StarsI32 (*StarsPopupWindowDefaultProc)(void *, StarsU16, StarsU16,
    StarsU16, StarsU32);
typedef struct StarsPopupWndProcOps {
    StarsPopupWindowGetRect get_client_rect;
    StarsPopupWindowFillRect fill_rect;
    StarsPopupWindowBeginPaint begin_paint;
    StarsPopupWindowDraw draw_popup;
    StarsPopupWindowEndPaint end_paint;
    StarsPopupWindowDestroy destroy_window;
    StarsPopupWindowReleaseCapture release_capture;
    StarsPopupWindowAdvanceTutor advance_tutor;
    StarsPopupWindowDefaultProc default_proc;
} StarsPopupWndProcOps;
typedef struct StarsPopupWndProcInput {
    StarsU16 popup_window;
    StarsU16 popup_kind;
    StarsU16 button_face_brush;
    StarsU16 game_flags;
    StarsU16 tutor_flags;
    const StarsPopupWndProcOps *ops;
    void *context;
} StarsPopupWndProcInput;
StarsI32 stars_popup_wnd_proc(StarsPopupWndProcInput *, StarsU16, StarsU16,
    StarsU16, StarsU32);

typedef StarsI16 (*StarsHScrollGetMetric)(void *, StarsU16);
typedef void (*StarsHScrollSetPos)(void *, StarsU16, StarsU16, StarsI16,
    StarsI16);
typedef void (*StarsHScrollSetRange)(void *, StarsU16, StarsU16, StarsI16,
    StarsI16, StarsI16);
typedef void (*StarsHScrollSetWindowPos)(void *, StarsU16, StarsU16, StarsI16,
    StarsI16, StarsI16, StarsI16, StarsU16);
typedef struct StarsReportViewState {
    StarsU16 report_window;
    StarsU16 hscroll_window;
    StarsU16 fields;
    StarsU16 field_first;
    StarsU16 rows_visible;
    StarsU16 column_scroll;
    StarsU32 visible_fields;
    StarsU8 column_width[64];
    StarsI16 report_kind;
    StarsI16 row_first;
    StarsI16 row_count;
} StarsReportViewState;
typedef struct StarsSetHScrollBarOps {
    StarsGetClientRect get_client_rect;
    StarsHScrollGetMetric get_system_metrics;
    StarsHScrollSetPos set_scroll_pos;
    StarsHScrollSetRange set_scroll_range;
    StarsHScrollSetWindowPos set_window_pos;
} StarsSetHScrollBarOps;
typedef struct StarsSetHScrollBarInput {
    StarsReportViewState *view;
    StarsU16 report_window;
    StarsI16 arial8_height;
    const StarsSetHScrollBarOps *ops;
    void *context;
} StarsSetHScrollBarInput;
void stars_set_h_scroll_bar(StarsSetHScrollBarInput *);

typedef void (*StarsInvalidateReportRect)(void *, StarsU16,
    const StarsRect *, StarsI16);
typedef void (*StarsInvalidateReportScrollRange)(void *, StarsU16, StarsU16,
    StarsI16, StarsI16, StarsI16);
typedef void (*StarsInvalidateReportSort)(void *, StarsI16, StarsI16);
typedef struct StarsInvalidateReportRecord {
    StarsI16 report;
    StarsI16 sort_column;
    StarsI16 cached;
    StarsU16 vertical_scroll;
    StarsI16 rows;
    StarsI16 rows_visible;
} StarsInvalidateReportRecord;
typedef struct StarsInvalidateReportOps {
    StarsGetClientRect get_client_rect;
    StarsInvalidateReportRect invalidate_rect;
    StarsInvalidateReportScrollRange set_scroll_range;
    StarsInvalidateReportSort sort_cache;
} StarsInvalidateReportOps;
typedef struct StarsInvalidateReportInput {
    StarsI16 report;
    StarsI16 reload;
    StarsU16 game_view_flags;
    StarsI16 ai_mode;
    StarsU16 report_window;
    StarsI16 arial8_height;
    StarsInvalidateReportRecord **current;
    StarsInvalidateReportRecord *planet_report;
    StarsInvalidateReportRecord *fleet_report;
    void **report_ids;
    const StarsInvalidateReportOps *ops;
    void *context;
} StarsInvalidateReportInput;
void stars_invalidate_report(StarsInvalidateReportInput *);

typedef const StarsU8 *(*StarsTutorTitleFormat)(void *);
typedef void (*StarsTutorFormatTitle)(void *, StarsU8 *, const StarsU8 *,
    StarsI16);
typedef StarsI16 (*StarsTutorTaskDone)(void *);
typedef void (*StarsTutorErrorCallback)(void *, StarsI16);
typedef void (*StarsTutorEnd)(void *, StarsI16);
typedef void (*StarsTutorShow)(void *, StarsI16);
typedef void (*StarsTutorGetWindowRect)(void *, StarsU16, StarsRect *);
typedef void (*StarsTutorScreenToClient)(void *, StarsU16, StarsPoint *);
typedef void (*StarsTutorExpandRect)(void *, StarsRect *, StarsI16, StarsI16);
typedef struct StarsAdvanceTutorState {
    StarsU16 flags;
    StarsI16 idt;
    StarsI16 idt_bold;
    StarsI16 idh;
    StarsI16 ids_error;
    StarsU16 window;
    StarsI16 arial8_height;
} StarsAdvanceTutorState;
typedef struct StarsAdvanceTutorOps {
    StarsTutorTaskDone task_done;
    StarsTutorTitleFormat title_format;
    StarsTutorFormatTitle format_title;
    StarsSetWindowText set_window_text;
    StarsTutorShow show_tutor;
    StarsTutorGetWindowRect get_window_rect;
    StarsTutorScreenToClient screen_to_client;
    StarsTutorExpandRect expand_rect;
    StarsInvalidateReportRect invalidate_rect;
    StarsTutorErrorCallback tutor_error;
    StarsTutorEnd end_tutor;
} StarsAdvanceTutorOps;
void stars_advance_tutor(StarsAdvanceTutorState *, const StarsAdvanceTutorOps *,
    void *);
typedef StarsU16 (*StarsDrawTutorBeginPaint)(void *, StarsU16, StarsRect *);
typedef void (*StarsDrawTutorEndPaint)(void *, StarsU16, const StarsRect *);
typedef StarsU16 (*StarsDrawTutorSelectObject)(void *, StarsU16, StarsU16);
typedef void (*StarsDrawTutorSetColor)(void *, StarsU16, StarsU32);
typedef void (*StarsDrawTutorPatBlt)(void *, StarsU16, StarsI16, StarsI16,
    StarsI16, StarsI16, StarsU32);
typedef void (*StarsDrawTutorGetWindowRect)(void *, StarsU16, StarsRect *);
typedef void (*StarsDrawTutorScreenToClient)(void *, StarsU16, StarsPoint *);
typedef StarsI16 (*StarsDrawTutorGetString)(void *, StarsU8 *, StarsI16);
typedef StarsI16 (*StarsDrawTutorHasBreak)(void *, StarsU8);
typedef void (*StarsDrawTutorWrapText)(void *, StarsU16, StarsI16 *,
    StarsI16 *, const StarsU8 *, StarsI16, StarsI16, StarsI16,
    StarsI16 *, StarsI16, StarsI16);
typedef struct StarsDrawTutorOps {
    StarsDrawTutorBeginPaint begin_paint;
    StarsDrawTutorEndPaint end_paint;
    StarsDrawTutorSelectObject select_object;
    StarsDrawTutorSetColor set_background_color;
    StarsDrawTutorSetColor set_text_color;
    StarsDrawTutorPatBlt pat_blt;
    StarsGetClientRect get_client_rect;
    StarsDrawTutorGetWindowRect get_window_rect;
    StarsDrawTutorScreenToClient screen_to_client;
    StarsGetDialogItem get_dialog_item;
    StarsDrawTutorGetString get_string;
    StarsDrawTutorHasBreak has_break;
    StarsDrawTutorWrapText wrap_text;
    StarsFillRect fill_rect;
} StarsDrawTutorOps;
typedef struct StarsDrawTutorInput {
    StarsU16 hwnd;
    StarsI16 arial8_height;
    StarsU16 arial8_font;
    StarsU16 button_face_brush;
    StarsU16 button_shadow_brush;
    StarsU16 button_highlight_brush;
    StarsU32 button_face_color;
    StarsU32 button_text_color;
    StarsI16 idt;
    StarsI16 idt_bold;
    const StarsDrawTutorOps *ops;
    void *context;
} StarsDrawTutorInput;
void stars_draw_tutor_text(const StarsDrawTutorInput *);
typedef StarsU32 (*StarsStartTutorMakeProc)(void *, StarsU16);
typedef StarsI16 (*StarsStartTutorMetric)(void *, StarsU16);
typedef StarsI16 (*StarsStartTutorGetString)(void *, StarsU16, StarsU8 *,
    StarsU16);
typedef void (*StarsStartTutorAppendString)(void *, StarsU8 *, StarsU16);
typedef StarsI16 (*StarsStartTutorAccess)(void *, const StarsU8 *, StarsI16);
typedef StarsI16 (*StarsStartTutorOpenGame)(void *, StarsU16, StarsI16);
typedef void (*StarsStartTutorCreateChildren)(void *);
typedef void (*StarsStartTutorCreateWorld)(void *);
typedef void (*StarsStartTutorCreateDialog)(void *, StarsU16, StarsU16,
    StarsU16, StarsU32);
typedef void (*StarsStartTutorInitializeMenu)(void *, StarsI16);
typedef void (*StarsStartTutorPostMessage)(void *, StarsU16, StarsU16,
    StarsU16, StarsU32);
typedef void (*StarsStartTutorWindow)(void *, StarsU16);
typedef void (*StarsStartTutorShowWindow)(void *, StarsU16, StarsI16);
typedef void (*StarsStartTutorShowTutor)(void *, StarsI16);
typedef void (*StarsStartTutorInvalidate)(void *, StarsU16, StarsU16);
typedef void (*StarsStartTutorAdvance)(void *);
typedef StarsI16 (*StarsStartTutorTaskDone)(void *);
typedef void (*StarsStartTutorEnd)(void *, StarsI16);
typedef StarsI16 (*StarsStartTutorAlert)(void *, StarsU16, StarsI16);
typedef struct StarsStartTutorState {
    StarsU16 gd_flags;
    StarsU16 gd_bits2;
    StarsU16 ini_flags;
    StarsU32 game_lid;
    StarsU16 grbit_scan;
    StarsI16 scan_zoom;
    StarsU16 zip_valid;
    StarsU8 misc_state[26];
    StarsU16 tutor_flags;
    StarsI16 tutor_idt;
    StarsI16 tutor_idt_bold;
    StarsI16 tutor_idh;
    StarsI16 tutor_ids_error;
    StarsU16 tutor_window;
    StarsU32 tutor_proc;
    StarsU16 freeing_title;
    StarsU16 title_window;
    StarsU16 frame_window;
    StarsU16 instance;
    StarsU8 *base_name;
    StarsU16 base_capacity;
} StarsStartTutorState;
typedef struct StarsStartTutorOps {
    StarsStartTutorMakeProc make_proc;
    StarsStartTutorMetric get_system_metric;
    StarsStartTutorGetString get_string;
    StarsStartTutorAppendString append_string;
    StarsStartTutorAccess access_file;
    StarsStartTutorOpenGame open_game;
    StarsStartTutorCreateChildren create_children;
    StarsStartTutorCreateWorld create_world;
    StarsStartTutorCreateDialog create_dialog;
    StarsStartTutorInitializeMenu initialize_menu;
    StarsStartTutorPostMessage post_message;
    StarsStartTutorWindow destroy_window;
    StarsStartTutorShowWindow show_window;
    StarsStartTutorShowTutor show_tutor;
    StarsStartTutorInvalidate invalidate_rect;
    StarsStartTutorAdvance advance_tutor;
    StarsStartTutorTaskDone task_done;
    StarsStartTutorEnd end_tutor;
    StarsStartTutorAlert alert_ids;
    void (*save_game_state)(void *);
    void (*free_proc)(void *, StarsU32);
} StarsStartTutorOps;
void stars_start_tutor(StarsStartTutorState *, StarsI16,
    const StarsStartTutorOps *, void *);

typedef StarsI16 (*StarsCtrScanToPoint)(void *, StarsI16);
typedef StarsI16 (*StarsCtrPointToScan)(void *, StarsI16);
typedef void (*StarsCtrSetScrollPos)(void *, StarsU16, StarsU16, StarsI16,
    StarsI16);
typedef void (*StarsCtrScrollScanner)(void *, StarsI16, StarsI16);
typedef void (*StarsCtrInvalidate)(void *, StarsU16, StarsI16);
typedef struct StarsCtrPointScanInput {
    StarsU16 scanner_window;
    StarsPoint point;
    StarsI16 scroll;
    StarsI16 dy_sbar;
    StarsI16 scan_zoom;
    StarsI16 gal_size;
    StarsI16 x_scan_top;
    StarsI16 y_scan_top;
    StarsCtrScanToPoint scan_to_point;
    StarsCtrPointToScan point_to_scan;
    StarsCtrSetScrollPos set_scroll_pos;
    StarsCtrScrollScanner scroll_scanner;
    StarsCtrInvalidate invalidate;
    StarsGetClientRect get_client_rect;
    void *context;
} StarsCtrPointScanInput;
void stars_ctr_point_scan(StarsCtrPointScanInput *);
typedef struct StarsNextObjectFleet {
    StarsI16 id;
    StarsI16 player;
    StarsPoint point;
    StarsU16 valid;
} StarsNextObjectFleet;
typedef struct StarsNextObjectScan {
    StarsPoint point;
    StarsI16 fleet_index;
    StarsI16 object_kind;
    StarsI16 planet_id;
} StarsNextObjectScan;
typedef struct StarsGetNextObjHereInput {
    StarsNextObjectScan *scan;
    StarsI16 selected_kind;
    StarsI16 selected_id;
    StarsPoint selected_point;
    StarsU16 selected_full_flags;
    StarsI16 selected_planet_player;
    StarsI16 selected_planet_id;
    StarsI16 current_player;
    StarsI16 fleet_count;
    const StarsNextObjectFleet *fleets;
    StarsI16 only_ours;
} StarsGetNextObjHereInput;
StarsI16 stars_get_next_obj_here(StarsGetNextObjHereInput *);
typedef StarsI16 (*StarsScrollScannerVisible)(void *, StarsU16);
typedef StarsU16 (*StarsScrollScannerGetDc)(void *, StarsU16);
typedef void (*StarsScrollScannerReleaseDc)(void *, StarsU16, StarsU16);
typedef void (*StarsScrollScannerDraw)(void *, StarsU16, StarsU16,
    const StarsRect *);
typedef void (*StarsScrollScannerUpdate)(void *, StarsU16);
typedef void (*StarsScrollScannerScroll)(void *, StarsU16, StarsI16,
    StarsI16, const StarsRect *, const StarsRect *);
typedef void (*StarsScrollScannerValidate)(void *, StarsU16,
    const StarsRect *);
typedef struct StarsScrollScannerOps {
    StarsScrollScannerVisible visible;
    StarsScrollScannerGetDc get_dc;
    StarsGetClientRect get_client_rect;
    StarsScrollScannerDraw draw;
    StarsScrollScannerUpdate update;
    StarsScrollScannerScroll scroll_window;
    StarsScrollScannerValidate validate_rect;
    StarsScrollScannerReleaseDc release_dc;
} StarsScrollScannerOps;
typedef struct StarsScrollScannerInput {
    StarsU16 scanner_window;
    StarsU16 browser_window;
    StarsI16 dialog_up;
    StarsU16 view_flags;
    StarsI16 dy_sbar;
    const StarsScrollScannerOps *ops;
    void *context;
} StarsScrollScannerInput;
void stars_scroll_scanner(StarsI16, StarsI16, StarsScrollScannerInput *);

/* ScannerWndProc is a message router around the scanner's typed state.  The
 * original routine reads a large Win16/DGROUP object graph; these callbacks
 * keep those window, drawing, selection, and popup services explicit while
 * retaining the original message decisions in ordinary C. */
enum {
    STARS_SCANNER_WM_CREATE = 1,
    STARS_SCANNER_WM_SIZE = 5,
    STARS_SCANNER_WM_PAINT = 0x000f,
    STARS_SCANNER_WM_SETCURSOR = 0x0020,
    STARS_SCANNER_WM_CHAR = 0x0102,
    STARS_SCANNER_WM_HSCROLL = 0x0114,
    STARS_SCANNER_WM_VSCROLL = 0x0115,
    STARS_SCANNER_WM_LBUTTONDOWN = 0x0201,
    STARS_SCANNER_WM_LBUTTONDOUBLECLICK = 0x0203,
    STARS_SCANNER_WM_RBUTTONDOWN = 0x0204,
    STARS_SCANNER_WM_MBUTTONDOWN = 0x0207,
    STARS_SCANNER_WM_ACTIVATEAPP = 0x0222,
    STARS_SCANNER_KEY_MINUS = 0x002d,
    STARS_SCANNER_KEY_V_LOWER = 0x0076,
    STARS_SCANNER_KEY_V_UPPER = 0x0056,
    STARS_SCANNER_MOUSE_CONTROL = 0x0004,
    STARS_SCANNER_MOUSE_SHIFT = 0x0008,
    STARS_SCANNER_SCAN_ADD = 0x0010,
    STARS_SCANNER_BEHAVIOR_WARP = 0x0100,
    STARS_SCANNER_BEHAVIOR_ROUTE = 0x2000,
    STARS_SCANNER_CURSOR_SCROLLBAR = 0x7f00,
    STARS_SCANNER_CURSOR_SCANNER = 1,
    STARS_SCANNER_CURSOR_ADD = 2,
    STARS_SCANNER_CURSOR_OPEN_GRAB = 3
};
typedef struct StarsScannerPopupChoice {
    StarsI16 kind;
    StarsI16 id;
    StarsI16 owner;
} StarsScannerPopupChoice;
typedef StarsU16 (*StarsScannerBeginPaint)(void *, StarsU16, StarsRect *);
typedef void (*StarsScannerEndPaint)(void *, StarsU16, StarsU16, StarsRect *);
typedef void (*StarsScannerGetRect)(void *, StarsU16, StarsRect *);
typedef void (*StarsScannerDraw)(void *, StarsU16, const StarsRect *);
typedef void (*StarsScannerPostCommand)(void *, StarsU16, StarsI16);
typedef void (*StarsScannerFocus)(void *, StarsU16);
typedef void (*StarsScannerGetPoint)(void *, StarsPoint *);
typedef void (*StarsScannerMapPoint)(void *, StarsU16, StarsPoint *);
typedef StarsI16 (*StarsScannerPointInRect)(void *, const StarsRect *, StarsPoint);
typedef StarsU16 (*StarsScannerLoadCursor)(void *, StarsU16);
typedef void (*StarsScannerSetCursor)(void *, StarsU16);
typedef StarsU16 (*StarsScannerKeyState)(void *, StarsU16);
typedef void (*StarsScannerCrosshair)(void *, StarsU16, StarsPoint);
typedef void (*StarsScannerSetScrollPos)(void *, StarsU16, StarsI16, StarsI16);
typedef StarsI16 (*StarsScannerGetScrollPos)(void *, StarsU16, StarsI16);
typedef void (*StarsScannerScroll)(void *, StarsI16, StarsI16);
typedef void (*StarsScannerTransform)(void *, StarsPoint *);
typedef void (*StarsScannerFindNearest)(void *, StarsPoint, StarsI16,
    StarsScanSelection *);
typedef StarsI16 (*StarsScannerNearWaypoint)(void *, StarsPoint, StarsI16);
typedef StarsI16 (*StarsScannerDragWaypoint)(void *, StarsPoint);
typedef void (*StarsScannerAddWaypoint)(void *, StarsPoint,
    const StarsScanSelection *);
typedef void (*StarsScannerMeasure)(void *, const StarsScanSelection *,
    StarsPoint);
typedef StarsI16 (*StarsScannerPopupMenu)(void *, StarsU16, StarsPoint,
    const StarsScanSelection *, StarsScannerPopupChoice *);
typedef void (*StarsScannerPopupBelow)(void *, StarsU16, StarsPoint,
    const StarsScanSelection *);
typedef void (*StarsScannerChangeScan)(void *, const StarsScanSelection *,
    StarsI16);
typedef StarsI16 (*StarsScannerNextObject)(void *, const StarsScanSelection *,
    StarsI16);
typedef StarsI16 (*StarsScannerCanSelect)(void *, StarsI16, StarsI16,
    StarsI16);
typedef void (*StarsScannerChangeMain)(void *, StarsI16, StarsI16);
typedef void (*StarsScannerRedrawScan)(void *, StarsI16, StarsI16);
typedef void (*StarsScannerTogglePlanet)(void *, StarsI16, StarsI16);
typedef StarsI16 (*StarsScannerWarpMode)(void *);
typedef StarsI32 (*StarsScannerDefaultProc)(void *, StarsU16, StarsU16,
    StarsU16, StarsU32);
typedef struct StarsScannerWndOps {
    StarsScannerBeginPaint begin_paint;
    StarsScannerEndPaint end_paint;
    StarsScannerGetRect get_client_rect;
    StarsScannerDraw draw_scrollbar;
    StarsScannerDraw draw_scanner;
    StarsScannerPostCommand post_command;
    StarsScannerFocus set_focus;
    StarsScannerGetPoint get_cursor_pos;
    StarsScannerMapPoint screen_to_client;
    StarsScannerPointInRect point_in_rect;
    StarsScannerLoadCursor load_cursor;
    StarsScannerSetCursor set_cursor;
    StarsScannerKeyState get_async_key_state;
    StarsScannerCrosshair draw_crosshair;
    StarsScannerSetScrollPos set_scroll_pos;
    StarsScannerGetScrollPos get_scroll_pos;
    StarsScannerScroll scroll_scanner;
    StarsScannerTransform logical_to_scan;
    StarsScannerTransform scan_to_logical;
    StarsScannerFindNearest find_nearest;
    StarsScannerNearWaypoint near_waypoint;
    StarsScannerDragWaypoint drag_waypoint;
    StarsScannerAddWaypoint add_waypoint;
    StarsScannerMeasure measuring_tape;
    StarsScannerPopupMenu popup_menu;
    StarsScannerPopupBelow popup_below;
    StarsScannerChangeScan change_scan_selection;
    StarsScannerNextObject next_object_here;
    StarsScannerCanSelect can_select;
    StarsScannerChangeMain change_main_object;
    StarsScannerRedrawScan redraw_scan_selection;
    StarsScannerTogglePlanet toggle_planet_route;
    StarsScannerWarpMode warp_from_planet;
    StarsScannerDefaultProc default_proc;
} StarsScannerWndOps;
typedef struct StarsScannerWndState {
    StarsU16 frame_window;
    StarsU16 scanner_window;
    StarsU16 active_window;
    StarsI16 x_scan_top, y_scan_top;
    StarsI16 scan_increment, scan_page;
    StarsI16 scrollbar_height;
    StarsI16 zoom;
    StarsU16 fleets_present, drawing_busy;
    StarsU16 behavior_flags, scan_flags;
    StarsI16 current_player;
    StarsI16 selected_kind, selected_id;
    StarsI16 selected_planet;
    StarsPoint selected_point, current_object_point;
    StarsScanSelection selection;
    const StarsScannerWndOps *ops;
    void *context;
} StarsScannerWndState;
StarsI32 stars_scanner_wnd_proc(StarsScannerWndState *, StarsU16, StarsU16,
    StarsU16, StarsU32);

/* FHandleWayPointDrag owns the drag loop and order-record transition.  The
 * callbacks below are the Win16 mouse/GDI and fleet-table services that the
 * original routine calls while preserving the target-object and duplicate-
 * waypoint decisions in the reconstructed source. */
#define STARS_WAYPOINT_DRAG_MAX_ORDERS 64
typedef StarsI16 (*StarsWaypointDragMouseMove)(void *, StarsPoint *);
typedef void (*StarsWaypointDragTransform)(void *, StarsPoint *);
typedef StarsI16 (*StarsWaypointDragNear)(void *, StarsPoint, StarsI16);
typedef void (*StarsWaypointDragFind)(void *, StarsPoint, StarsI16,
    StarsScanSelection *);
typedef void (*StarsWaypointDragXor)(void *, StarsU16, const StarsPoint *,
    StarsI16);
typedef void (*StarsWaypointDragBar)(void *, StarsU16, const StarsPoint *,
    const StarsScanSelection *);
typedef StarsU16 (*StarsWaypointDragGetDc)(void *, StarsU16);
typedef void (*StarsWaypointDragReleaseDc)(void *, StarsU16, StarsU16);
typedef void (*StarsWaypointDragCapture)(void *, StarsU16);
typedef void (*StarsWaypointDragReleaseCapture)(void *);
typedef void (*StarsWaypointDragSetCursor)(void *, StarsU16);
typedef void (*StarsWaypointDragRect)(void *, StarsU16, StarsRect *);
typedef StarsI16 (*StarsWaypointDragConfirmDelete)(void *, StarsI16);
typedef void (*StarsWaypointDragDelete)(void *, StarsI16);
typedef void (*StarsWaypointDragRedraw)(void *, StarsI16, StarsI16);
typedef void (*StarsWaypointDragLine)(void *, StarsU16, StarsPoint,
    StarsPoint);
typedef StarsU16 (*StarsWaypointDragWarp)(void *, StarsI16,
    const StarsPoint *);
typedef void (*StarsWaypointDragFleet)(void *);
typedef void (*StarsWaypointDragChangeScan)(void *, const StarsScanSelection *,
    StarsI16);
typedef void (*StarsWaypointDragBound)(void *, StarsRect *,
    const StarsPoint *, StarsI16);
typedef void (*StarsWaypointDragDraw)(void *, StarsU16, const StarsRect *);
typedef void (*StarsWaypointDragInvalidate)(void *, StarsU16);
typedef void (*StarsWaypointDragTitle)(void *, StarsU16);
typedef struct StarsWaypointDragOps {
    StarsWaypointDragTransform logical_to_scan;
    StarsWaypointDragTransform scan_to_logical;
    StarsWaypointDragMouseMove get_mouse_move;
    StarsWaypointDragNear near_waypoint;
    StarsWaypointDragFind find_nearest;
    StarsWaypointDragXor draw_xor_lines;
    StarsWaypointDragBar draw_bar;
    StarsWaypointDragGetDc get_dc;
    StarsWaypointDragReleaseDc release_dc;
    StarsWaypointDragCapture set_capture;
    StarsWaypointDragReleaseCapture release_capture;
    StarsWaypointDragSetCursor set_cursor;
    StarsWaypointDragRect get_client_rect;
    StarsWaypointDragConfirmDelete confirm_delete;
    StarsWaypointDragDelete delete_waypoint;
    StarsWaypointDragRedraw redraw_scan_selection;
    StarsWaypointDragLine draw_starbase_line;
    StarsWaypointDragWarp warp_best;
    StarsWaypointDragFleet refresh_fleet;
    StarsWaypointDragChangeScan change_scan_selection;
    StarsWaypointDragBound bound_points;
    StarsWaypointDragDraw draw_scanner;
    StarsWaypointDragInvalidate invalidate_mine;
    StarsWaypointDragTitle set_mineral_title;
} StarsWaypointDragOps;
typedef struct StarsWaypointDragState {
    StarsU16 scanner_window, mine_window;
    StarsI16 scrollbar_height, active_waypoint, order_count;
    StarsU16 scan_flags;
    StarsPoint order_points[STARS_WAYPOINT_DRAG_MAX_ORDERS];
    StarsI16 order_kinds[STARS_WAYPOINT_DRAG_MAX_ORDERS];
    StarsI16 order_ids[STARS_WAYPOINT_DRAG_MAX_ORDERS];
    const StarsWaypointDragOps *ops;
    void *context;
} StarsWaypointDragState;
StarsI16 stars_handle_waypoint_drag(StarsPoint, StarsWaypointDragState *);

/* IWarpBestForWaypoint chooses a legal warp for one fleet order.  The
 * callbacks expose fleet fuel/statistics, planet knowledge, and gate/cargo
 * policy while the bounded warp search and distance rounding stay in C. */
typedef StarsI16 (*StarsWarpBestIdeal)(void *, StarsI16);
typedef StarsI16 (*StarsWarpBestPlanet)(void *, StarsPoint, StarsI16 *, StarsI16 *);
/* Evaluate fuel to the current waypoint at the supplied trial warp. */
typedef StarsI32 (*StarsWarpBestFuel)(void *, StarsI16);
typedef StarsI32 (*StarsWarpBestStat)(void *, StarsI16);
typedef StarsI16 (*StarsWarpBestMinWarp)(void *);
typedef StarsI16 (*StarsWarpBestSpecial)(void *, StarsI16);
typedef StarsI16 (*StarsWarpBestGate)(void *, StarsPoint, StarsPoint);
typedef StarsI16 (*StarsWarpBestCargo)(void *, StarsI16);
typedef StarsI16 (*StarsWarpBestDistance)(void *, StarsPoint, StarsPoint);
typedef struct StarsWarpBestOps {
    StarsWarpBestIdeal ideal_warp;
    StarsWarpBestPlanet nearest_planet;
    StarsWarpBestFuel fuel_to_waypoint;
    StarsWarpBestStat fleet_stat;
    StarsWarpBestMinWarp minimum_warp;
    StarsWarpBestSpecial special_order;
    StarsWarpBestGate can_use_gate;
    StarsWarpBestCargo has_cargo_hull;
    StarsWarpBestDistance distance_term;
} StarsWarpBestOps;
typedef struct StarsWarpBestState {
    StarsI16 order_warp, order_kind, previous_warp;
    StarsI16 order_index, order_count, fleet_player, current_player;
    StarsI16 ai_mode;
    StarsPoint previous_point, order_point, destination;
    const StarsWarpBestOps *ops;
    void *context;
    StarsI32 fuel_on_board;
    StarsI16 target_kind;
    StarsI16 colony_hull;
    /* True only for an owned destination whose base hull is not 32. */
    StarsI16 destination_disables_ai_limit;
} StarsWarpBestState;
StarsI16 stars_iwarp_best_for_waypoint(StarsWarpBestState *);

/* DrawRadarCircle accumulates radar discs so the scanner can paint them once
 * after visibility culling.  The typed record makes covered-circle and
 * replacement behavior explicit instead of preserving the original stack
 * aliases and GDI calls. */
#define STARS_RADAR_CIRCLE_MAX 32
typedef void (*StarsRadarCircleEllipse)(void *, StarsU16, const StarsRect *);
typedef StarsI16 (*StarsRadarCircleIntersect)(void *, const StarsRect *,
    const StarsRect *);
typedef void (*StarsRadarCircleMode)(void *, StarsU16, StarsI16);
typedef StarsU32 (*StarsRadarCircleColor)(void *, StarsU16, StarsU32);
typedef struct StarsRadarCircleOps {
    StarsRadarCircleIntersect intersect;
    StarsRadarCircleMode set_mode;
    StarsRadarCircleColor set_background;
    StarsRadarCircleEllipse ellipse;
} StarsRadarCircleOps;
typedef struct StarsRadarCircleState {
    StarsU16 hdc;
    StarsRect clip;
    StarsI16 covered, hollow;
    StarsI16 current, capacity;
    StarsI16 center_x[STARS_RADAR_CIRCLE_MAX];
    StarsI16 center_y[STARS_RADAR_CIRCLE_MAX];
    StarsI16 radius[STARS_RADAR_CIRCLE_MAX];
    const StarsRadarCircleOps *ops;
    void *context;
} StarsRadarCircleState;
void stars_draw_radar_circle(StarsRadarCircleState *, const StarsRect *);

/* DrawBuildSelComp renders the selected ship component summary.  Component
 * lookup, localized description construction, and GDI text are callback
 * boundaries; the selected-slot branches, cost scaling, spacing, and DC
 * ownership stay in the reconstructed routine. */
typedef struct StarsBuildSelCompRecord {
    StarsU16 group;
    StarsU16 flags;
    StarsI16 level;
    StarsI16 mass;
} StarsBuildSelCompRecord;
typedef StarsU16 (*StarsBuildSelCompGetDc)(void *, StarsU16);
typedef void (*StarsBuildSelCompReleaseDc)(void *, StarsU16, StarsU16);
typedef void (*StarsBuildSelCompRect)(void *, StarsU16, StarsRect *);
typedef void (*StarsBuildSelCompFill)(void *, StarsU16, const StarsRect *);
typedef void (*StarsBuildSelCompSetFont)(void *, StarsU16, StarsU16);
typedef StarsU32 (*StarsBuildSelCompSetColor)(void *, StarsU16, StarsU32);
typedef StarsI16 (*StarsBuildSelCompResolveDialog)(void *, StarsU16,
    StarsBuildSelCompRecord *);
typedef StarsI16 (*StarsBuildSelCompResolveSlot)(void *, StarsI16,
    StarsBuildSelCompRecord *, StarsBuildSelCompRecord *);
typedef void (*StarsBuildSelCompDescription)(void *, StarsU16, StarsI16,
    StarsI16, const StarsBuildSelCompRecord *);
typedef void (*StarsBuildSelCompHeader)(void *, StarsU16, StarsI16,
    const StarsBuildSelCompRecord *);
typedef void (*StarsBuildSelCompMineral)(void *, StarsU16, StarsI16, StarsI16,
    StarsI32);
typedef void (*StarsBuildSelCompMass)(void *, StarsU16, StarsI16, StarsI32);
typedef void (*StarsBuildSelCompEnd)(void *, StarsU16, StarsU16);
typedef struct StarsBuildSelCompOps {
    StarsBuildSelCompGetDc get_dc;
    StarsBuildSelCompReleaseDc release_dc;
    StarsBuildSelCompRect get_client_rect;
    StarsBuildSelCompFill fill_background;
    StarsBuildSelCompSetFont select_font;
    StarsBuildSelCompSetColor set_text_color;
    StarsBuildSelCompSetColor set_background_color;
    StarsBuildSelCompResolveDialog resolve_dialog_selection;
    StarsBuildSelCompResolveSlot resolve_slot;
    StarsBuildSelCompDescription draw_description;
    StarsBuildSelCompHeader draw_header;
    StarsBuildSelCompMineral draw_mineral_cost;
    StarsBuildSelCompMass draw_mass_cost;
    StarsBuildSelCompEnd restore;
} StarsBuildSelCompOps;
typedef struct StarsBuildSelCompInput {
    StarsU16 window;
    StarsU16 slot_dialog;
    StarsU16 hdc;
    StarsI16 selected_slot;
    StarsI16 starbase_mode;
    StarsI16 build_info_y;
    StarsI16 arial8_height;
    StarsI16 max_mineral_width;
    StarsI16 player;
    StarsI16 technology_level;
    StarsI16 mineral_cost[3];
    StarsI16 mass_cost;
    StarsU32 button_face_color;
    StarsU32 window_text_color;
    StarsU32 mineral_color[3];
    const StarsBuildSelCompOps *ops;
    void *context;
} StarsBuildSelCompInput;
void stars_draw_build_sel_comp(StarsBuildSelCompInput *, StarsI16);

/* DrawSlotDlg owns the build-slot painting order.  The callbacks represent
 * bitmap/font/GDI details and localized labels; slot range selection, cargo
 * panel mode, empty/occupied slot dispatch, highlighting, plaque gating, and
 * DC cleanup remain in the C reconstruction. */
#define STARS_DRAW_SLOT_MAX 16
typedef struct StarsDrawSlotRecord {
    StarsU16 graphics;
    StarsU16 flags;
    StarsU16 bitmap;
    StarsI16 level;
} StarsDrawSlotRecord;
typedef StarsU16 (*StarsDrawSlotGetDc)(void *, StarsU16);
typedef void (*StarsDrawSlotReleaseDc)(void *, StarsU16, StarsU16);
typedef void (*StarsDrawSlotFleetBitmap)(void *, StarsU16, StarsI16,
    StarsI16, StarsU16);
typedef void (*StarsDrawSlotSpin)(void *, StarsU16, StarsI16);
typedef void (*StarsDrawSlotCargo)(void *, StarsU16, const StarsRect *,
    StarsI16, StarsI16);
typedef void (*StarsDrawSlotFill)(void *, StarsU16, const StarsRect *,
    StarsI16);
typedef void (*StarsDrawSlotEmpty)(void *, StarsU16, const StarsRect *,
    const StarsDrawSlotRecord *);
typedef void (*StarsDrawSlotPart)(void *, StarsU16, const StarsRect *,
    const StarsDrawSlotRecord *);
typedef void (*StarsDrawSlotSelected)(void *, StarsU16, const StarsRect *);
typedef void (*StarsDrawSlotPlaque)(void *, StarsU16, const StarsRect *,
    StarsI16, StarsI16, StarsI16, StarsI16);
typedef void (*StarsDrawSlotRestore)(void *, StarsU16, StarsU16);
typedef struct StarsDrawSlotOps {
    StarsDrawSlotGetDc get_dc;
    StarsDrawSlotReleaseDc release_dc;
    StarsDrawSlotFleetBitmap draw_fleet_bitmap;
    StarsDrawSlotSpin draw_spin_buttons;
    StarsDrawSlotCargo draw_cargo_panel;
    StarsDrawSlotFill fill_slot;
    StarsDrawSlotEmpty draw_empty_slot;
    StarsDrawSlotPart draw_part_slot;
    StarsDrawSlotSelected draw_selected_frame;
    StarsDrawSlotPlaque draw_plaque;
    StarsDrawSlotRestore restore;
} StarsDrawSlotOps;
typedef struct StarsDrawSlotInput {
    StarsU16 window;
    StarsU16 hdc;
    StarsI16 mode;
    StarsI16 draw_index;
    StarsI16 starbase_mode;
    StarsI16 popup_window;
    StarsI16 global_planet;
    StarsI16 cargo_max;
    StarsI16 hull_id;
    StarsI16 selected_slot;
    StarsI16 slot_count;
    StarsI16 built_count;
    StarsI16 existing_count;
    StarsRect cargo_rect;
    StarsRect plaque_rect;
    StarsRect slot_rect[STARS_DRAW_SLOT_MAX];
    StarsDrawSlotRecord slots[STARS_DRAW_SLOT_MAX];
    const StarsDrawSlotOps *ops;
    void *context;
} StarsDrawSlotInput;
void stars_draw_slot_dlg(StarsDrawSlotInput *);

/* FTrackSlot uses the same slot records for the painted grid and the list-box
 * proxy.  The callbacks isolate USER/GDI tracking while the C routine keeps
 * the original hit-test, selection, popup, and drag/drop decisions. */
#define STARS_TRACK_SLOT_MAX 32
#define STARS_TRACK_PART_EMPTY 0x0000U
#define STARS_TRACK_PART_COMPONENT 0x0100U
#define STARS_TRACK_PART_CARGO 0x0800U
#define STARS_TRACK_PART_STARBASE 0x1000U
#define STARS_TRACK_POPUP_EMPTY 0x010cU
#define STARS_TRACK_POPUP_STARBASE 0x010dU
#define STARS_TRACK_POPUP_RESTRICTED 0x0186U

typedef struct StarsTrackSlotPart {
    StarsU16 group;
    StarsU16 kind;
    StarsU16 hull_flags;
    StarsU16 component;
} StarsTrackSlotPart;
typedef StarsI16 (*StarsTrackSlotSpin)(void *, StarsU16, StarsI16, StarsU16 *);
typedef void (*StarsTrackSlotSelect)(void *, StarsI16);
typedef StarsI16 (*StarsTrackSlotListPart)(void *, StarsU16, StarsI16 *,
    StarsTrackSlotPart *);
typedef void (*StarsTrackSlotPopup)(void *, StarsU16, StarsI16, StarsI16,
    StarsU16);
typedef StarsI16 (*StarsTrackSlotDrag)(void *, StarsU16, StarsI16,
    const StarsTrackSlotPart *, StarsI16, StarsI16);
typedef void (*StarsTrackSlotInspect)(void *, StarsU16, StarsI16, StarsI16,
    const StarsTrackSlotPart *);
typedef struct StarsTrackSlotOps {
    StarsTrackSlotSpin track_spin;
    StarsTrackSlotSelect select_slot;
    StarsTrackSlotListPart read_list_part;
    StarsTrackSlotPopup show_popup;
    StarsTrackSlotDrag drag_part;
    StarsTrackSlotInspect inspect_part;
} StarsTrackSlotOps;
typedef struct StarsTrackSlotInput {
    StarsU16 window;
    StarsU16 slot_dialog;
    StarsU16 build_mode;
    StarsU16 hull_id;
    StarsU16 part_count;
    StarsU16 current_bitmap;
    StarsI16 x;
    StarsI16 y;
    StarsI16 right_button;
    StarsI16 list_box;
    StarsRect spin_rect[2];
    StarsRect slot_rect[STARS_TRACK_SLOT_MAX];
    StarsTrackSlotPart slot_part[STARS_TRACK_SLOT_MAX];
    const StarsTrackSlotOps *ops;
    void *context;
} StarsTrackSlotInput;
StarsI16 stars_track_slot(StarsTrackSlotInput *);

/* DrawBuildSelHull renders the textual hull summary.  Row kinds make the
 * output testable without tying the reconstruction to a particular GDI font
 * or localization resource. */
#define STARS_HULL_ROW_TITLE 1
#define STARS_HULL_ROW_COST 2
#define STARS_HULL_ROW_WEIGHT 3
#define STARS_HULL_ROW_ARMOR 4
#define STARS_HULL_ROW_POWER 5
#define STARS_HULL_ROW_CLOAK_JAMMER 6
#define STARS_HULL_ROW_SPEED 7
#define STARS_HULL_ROW_SCANNER 8
#define STARS_HULL_ROW_POPUP 9
typedef StarsU16 (*StarsDrawHullGetDc)(void *, StarsU16);
typedef void (*StarsDrawHullReleaseDc)(void *, StarsU16, StarsU16);
typedef void (*StarsDrawHullFill)(void *, StarsU16, const StarsRect *);
typedef void (*StarsDrawHullLine)(void *, StarsU16, StarsI16, StarsU16,
    StarsI32, StarsI32, StarsI16);
typedef void (*StarsDrawHullRestore)(void *, StarsU16, StarsU16);
typedef struct StarsDrawHullOps {
    StarsDrawHullGetDc get_dc;
    StarsDrawHullReleaseDc release_dc;
    StarsDrawHullFill fill_background;
    StarsDrawHullLine draw_line;
    StarsDrawHullRestore restore;
} StarsDrawHullOps;
typedef struct StarsDrawHullInput {
    StarsU16 window;
    StarsU16 hdc;
    StarsI16 mode;
    StarsI16 starbase_mode;
    StarsI16 popup_window;
    StarsI16 popup_kind;
    StarsI16 popup_max;
    StarsI16 popup_owner;
    StarsI16 tech_flags;
    StarsI16 race_special;
    StarsI16 race_can_discount;
    StarsI16 hull_id;
    StarsI16 has_hull;
    StarsI16 draw_y;
    StarsI16 line_height;
    StarsRect client_rect;
    StarsRect draw_rect;
    StarsI32 mineral_cost[4];
    StarsI32 empty_weight;
    StarsI32 armor;
    StarsI32 shield;
    StarsI32 power;
    StarsI32 cloak;
    StarsI32 jammer;
    StarsI32 speed;
    StarsI32 scanner_range;
    StarsI32 scanner_width;
    StarsI32 scanner_height;
    StarsI32 popup_distance;
    StarsI32 popup_units;
    StarsI32 vcr_damage;
    StarsI32 vcr_shield;
    const StarsDrawHullOps *ops;
    void *context;
} StarsDrawHullInput;
void stars_draw_build_sel_hull(StarsDrawHullInput *);

/* SlotDlg is the build-window controller.  Win16 control handles and the
 * original design-store operations are explicit so the dispatcher can be
 * tested independently of USER/GDI and the packed design records. */
enum {
    STARS_SLOT_WM_PAINT = 0x000f,
    STARS_SLOT_WM_ERASE_BACKGROUND = 0x0014,
    STARS_SLOT_WM_CONTROL_COLOR = 0x0019,
    STARS_SLOT_WM_SETCURSOR = 0x0020,
    STARS_SLOT_WM_DRAW_ITEM = 0x002b,
    STARS_SLOT_WM_MEASURE_ITEM = 0x002c,
    STARS_SLOT_WM_INIT_DIALOG = 0x0110,
    STARS_SLOT_WM_COMMAND = 0x0111,
    STARS_SLOT_CONTROL_PARTS = 0x080c,
    STARS_SLOT_CONTROL_DELETE = 0x0817,
    STARS_SLOT_CONTROL_ACCEPT_EDIT = 0x0818,
    STARS_SLOT_CONTROL_DESIGNS = 0x081a,
    STARS_SLOT_CONTROL_EDIT = 0x081b,
    STARS_SLOT_CONTROL_STARBASE = 0x0810,
    STARS_SLOT_CONTROL_BUILD_MODE = 0x0812,
    STARS_SLOT_CONTROL_BUILD_MODE_LAST = 0x0815,
    STARS_SLOT_CONTROL_COPY = 0x0816,
    STARS_SLOT_COMMAND_CLICK = 0,
    STARS_SLOT_COMMAND_CHANGE = 1,
    STARS_SLOT_COMMAND_EDIT = 0x0400,
    STARS_SLOT_MODE_COPY = 4
};
enum {
    STARS_SLOT_ACTION_INIT = 1,
    STARS_SLOT_ACTION_FILL_PARTS,
    STARS_SLOT_ACTION_FILL_DESIGNS,
    STARS_SLOT_ACTION_UPDATE_GLOBALS,
    STARS_SLOT_ACTION_INVALIDATE,
    STARS_SLOT_ACTION_SET_SELECTION,
    STARS_SLOT_ACTION_ENABLE_CONTROLS,
    STARS_SLOT_ACTION_SHOW_CONTROLS,
    STARS_SLOT_ACTION_ENTER_COPY,
    STARS_SLOT_ACTION_DELETE_DESIGN,
    STARS_SLOT_ACTION_LOG_DESIGN,
    STARS_SLOT_ACTION_COMMIT_EDIT,
    STARS_SLOT_ACTION_CANCEL_EDIT,
    STARS_SLOT_ACTION_CLOSE,
    STARS_SLOT_ACTION_HELP,
    STARS_SLOT_ACTION_ADVANCE_TUTOR,
    STARS_SLOT_ACTION_PAINT,
    STARS_SLOT_ACTION_ERASE,
    STARS_SLOT_ACTION_CONTROL_COLOR,
    STARS_SLOT_ACTION_DRAW_ITEM,
    STARS_SLOT_ACTION_MEASURE_ITEM,
    STARS_SLOT_ACTION_TRACK_MOUSE,
    STARS_SLOT_ACTION_SET_EDIT_TEXT,
    STARS_SLOT_ACTION_REDRAW_HULL
};
typedef struct StarsSlotDialogState {
    StarsU16 hwnd;
    StarsU16 parts_control;
    StarsU16 designs_control;
    StarsU16 edit_control;
    StarsI16 mode;
    StarsI16 starbase_mode;
    StarsI16 hull_copy;
    StarsI16 edit_guard;
    StarsI16 dialog_open;
    StarsI16 selection;
    StarsI16 design_valid;
    StarsI16 design_empty;
    StarsI16 design_exists;
    StarsI16 design_queued;
    StarsI16 hull_valid;
    StarsI16 hull_first_part;
    StarsI16 design_index;
    StarsI16 race_special;
    StarsI16 tutorial_active;
    StarsI16 player_design_count;
    StarsI16 turn;
    StarsU16 part_mask;
    StarsU16 starbase_part_mask;
    StarsU16 help_file;
    StarsU8 class_name[32];
} StarsSlotDialogState;
typedef void (*StarsSlotAction)(void *, StarsU16, StarsI16, StarsI16);
typedef StarsI16 (*StarsSlotSelection)(void *, StarsU16);
typedef StarsI16 (*StarsSlotResolveDesign)(void *, StarsI16, StarsI16,
    StarsSlotDialogState *);
typedef StarsI16 (*StarsSlotTutorial)(void *, StarsI16);
typedef StarsI16 (*StarsSlotCheckQueued)(void *, StarsU16,
    const StarsSlotDialogState *, StarsI16);
typedef StarsI16 (*StarsSlotGetText)(void *, StarsU16, StarsU8 *, StarsU16);
typedef StarsI16 (*StarsSlotFitsText)(void *, const StarsU8 *, StarsI16);
typedef void (*StarsSlotSetText)(void *, StarsU16, const StarsU8 *);
typedef StarsI16 (*StarsSlotTrackMouse)(void *, StarsU16, StarsI16, StarsI16,
    StarsI16, StarsI16);
typedef StarsI32 (*StarsSlotDefaultProc)(void *, StarsU16, StarsU16,
    StarsU16, StarsU32);
typedef struct StarsSlotDialogOps {
    StarsSlotAction action;
    StarsSlotSelection selection;
    StarsSlotResolveDesign resolve_design;
    StarsSlotTutorial tutorial_allowed;
    StarsSlotCheckQueued check_queued;
    StarsSlotGetText get_text;
    StarsSlotFitsText fits_text;
    StarsSlotSetText set_text;
    StarsSlotTrackMouse track_mouse;
    StarsSlotDefaultProc default_proc;
} StarsSlotDialogOps;
typedef struct StarsSlotDialogInput {
    StarsSlotDialogState *state;
    const StarsSlotDialogOps *ops;
    void *context;
} StarsSlotDialogInput;
StarsI32 stars_slot_dlg(StarsSlotDialogInput *, StarsU16, StarsU16,
    StarsU16, StarsU32);

enum {
    STARS_TUTOR_CHECK_SELECTION = 1,
    STARS_TUTOR_CHECK_MESSAGES,
    STARS_TUTOR_CHECK_QUEUE,
    STARS_TUTOR_CHECK_RESEARCH,
    STARS_TUTOR_CHECK_FLEET_WP,
    STARS_TUTOR_CHECK_CARGO,
    STARS_TUTOR_CHECK_XFER_WP,
    STARS_TUTOR_CHECK_COLONIZE_WP,
    STARS_TUTOR_CHECK_SUMMARY,
    STARS_TUTOR_CHECK_SCANNER,
    STARS_TUTOR_CHECK_ZIP,
    STARS_TUTOR_CHECK_SHIP_BUILDER,
    STARS_TUTOR_CHECK_BUILDER_PART,
    STARS_TUTOR_CHECK_TEMPLATE,
    STARS_TUTOR_CHECK_PLANET_ROUTE,
    STARS_TUTOR_CHECK_PATROL,
    STARS_TUTOR_CHECK_BATTLE_PLAN,
    STARS_TUTOR_VALUE_FLEET_CORD,
    STARS_TUTOR_VALUE_PRODUCT_COUNT,
    STARS_TUTOR_VALUE_REPORT_SORT,
    STARS_TUTOR_VALUE_FLEET_PRESENT,
    STARS_TUTOR_VALUE_HULL_PART
};
typedef StarsI16 (*StarsTutorTaskCheck)(void *, StarsU16,
    const StarsI16 [6]);
typedef StarsI16 (*StarsTutorTaskValue)(void *, StarsU16, StarsI16, StarsI16);
typedef struct StarsTutorTaskState {
    StarsI16 turn;
    StarsI16 idt;
    StarsI16 idt_bold;
    StarsI16 idh;
    StarsU16 flags;
    StarsI16 pct_res;
    StarsI16 xfer_dialog;
    StarsI16 slot_dialog;
    StarsI16 browser;
    StarsI16 zip_dialog;
    StarsI16 player_designs;
} StarsTutorTaskState;
typedef struct StarsTutorTaskOps {
    StarsTutorTaskCheck check;
    StarsTutorTaskValue value;
    void *context;
} StarsTutorTaskOps;
StarsI16 stars_tutor_task_done(StarsTutorTaskState *,
    const StarsTutorTaskOps *);
typedef StarsU16 (*StarsDrawScannerBarGetDc)(void *, StarsU16);
typedef void (*StarsDrawScannerBarReleaseDc)(void *, StarsU16, StarsU16);
typedef void (*StarsDrawScannerBarSelectObject)(void *, StarsU16, StarsU16);
typedef StarsU16 (*StarsDrawScannerBarSetMode)(void *, StarsU16, StarsU16);
typedef StarsU32 (*StarsDrawScannerBarSetColor)(void *, StarsU16, StarsU32);
typedef StarsI16 (*StarsDrawScannerBarTextExtent)(void *, StarsU16,
    const StarsU8 *, StarsI16);
typedef void (*StarsDrawScannerBarTextOut)(void *, StarsU16, StarsI16,
    StarsI16, const StarsU8 *, StarsI16, const StarsRect *);
typedef StarsI16 (*StarsDrawScannerBarString)(void *, StarsU16, StarsU8 *,
    StarsU16);
typedef StarsI16 (*StarsDrawScannerBarFormat)(void *, StarsI32, StarsU8 *,
    StarsU16);
typedef const StarsU8 *(*StarsDrawScannerBarObjectName)(void *, StarsI16,
    StarsI16);
typedef const StarsU8 *(*StarsDrawScannerBarDistance)(void *, StarsPoint,
    StarsPoint);
typedef const StarsU8 *(*StarsDrawScannerBarLocation)(void *, StarsI16,
    StarsI16, StarsPoint);
typedef void (*StarsDrawScannerBarPattern)(void *, StarsU16, const StarsRect *,
    StarsI16);
typedef struct StarsDrawScannerBarState {
    StarsU16 object_flags;
    StarsU16 object_full_flags;
    StarsI16 object_id;
    StarsI16 waypoint;
    StarsI16 planet_id;
    StarsI16 fleet_id;
    StarsI16 thing_id;
    StarsPoint point;
    StarsPoint target;
    const StarsU8 *label;
} StarsDrawScannerBarState;
typedef struct StarsDrawScannerBarOps {
    StarsDrawScannerBarGetDc get_dc;
    StarsDrawScannerBarReleaseDc release_dc;
    StarsGetClientRect get_client_rect;
    StarsDrawScannerBarSelectObject select_object;
    StarsDrawScannerBarSetMode set_background_mode;
    StarsDrawScannerBarSetColor set_background_color;
    StarsDrawScannerBarSetColor set_text_color;
    StarsDrawScannerBarTextExtent text_extent;
    StarsDrawScannerBarTextOut text_out;
    StarsDrawScannerBarString get_string;
    StarsDrawScannerBarFormat format_number;
    StarsDrawScannerBarObjectName get_object_name;
    StarsDrawScannerBarDistance get_distance;
    StarsDrawScannerBarLocation get_location;
    StarsDrawScannerBarPattern draw_lock_light;
    StarsDrawScannerBarPattern fill_background;
} StarsDrawScannerBarOps;
typedef struct StarsDrawScannerBarInput {
    StarsU16 scanner_window;
    StarsI16 scrollbar_height;
    StarsU16 font;
    StarsU16 button_face_brush;
    StarsU16 button_highlight_brush;
    StarsU32 button_face_color;
    StarsU32 button_text_color;
    StarsI16 full_redraw;
    StarsDrawScannerBarState selection;
    const StarsDrawScannerBarState *bar;
    const StarsDrawScannerBarOps *ops;
    void *context;
} StarsDrawScannerBarInput;
void stars_draw_scanner_sbar(StarsU16, const StarsRect *,
    StarsDrawScannerBarInput *);
typedef struct StarsDrawScannerPlanet {
    StarsI16 id;
    StarsI16 owner;
    StarsPoint logical;
    StarsI16 scanner_range;
    StarsI16 near_range;
    StarsI16 marker_kind;
    StarsU16 known;
} StarsDrawScannerPlanet;
typedef struct StarsDrawScannerFleet {
    StarsI16 id;
    StarsI16 owner;
    StarsI16 planet_id;
    StarsPoint logical;
    StarsI16 scanner_range;
    StarsI16 near_range;
    StarsU16 visible_ships;
    StarsU16 flags;
    const StarsPoint *path;
    StarsU16 path_count;
} StarsDrawScannerFleet;
typedef struct StarsDrawScannerThing {
    StarsI16 id;
    StarsI16 owner;
    StarsI16 kind;
    StarsPoint logical;
    StarsI16 radius;
    StarsU16 flags;
} StarsDrawScannerThing;
typedef struct StarsDrawScannerSelection {
    StarsI16 kind;
    StarsI16 id;
    StarsPoint logical;
    StarsU16 full_kind;
} StarsDrawScannerSelection;
typedef StarsU16 (*StarsDrawScannerGetDc)(void *, StarsU16);
typedef void (*StarsDrawScannerReleaseDc)(void *, StarsU16, StarsU16);
typedef void (*StarsDrawScannerClientRect)(void *, StarsU16, StarsRect *);
typedef void (*StarsDrawScannerSetClip)(void *, StarsU16, const StarsRect *);
typedef StarsI16 (*StarsDrawScannerTransform)(void *, StarsI16);
typedef void (*StarsDrawScannerLinkFleets)(void *);
typedef void (*StarsDrawScannerFill)(void *, StarsU16, const StarsRect *);
typedef void (*StarsDrawScannerRadar)(void *, StarsU16, const StarsRect *,
    StarsI16);
typedef void (*StarsDrawScannerPlanetMark)(void *, StarsU16,
    const StarsDrawScannerPlanet *, StarsPoint, StarsI16);
typedef void (*StarsDrawScannerFleetMark)(void *, StarsU16,
    const StarsDrawScannerFleet *, StarsPoint, StarsI16);
typedef void (*StarsDrawScannerThingMark)(void *, StarsU16,
    const StarsDrawScannerThing *, StarsPoint, StarsI16);
typedef void (*StarsDrawScannerPath)(void *, StarsU16,
    const StarsPoint *, StarsU16, StarsI16);
typedef void (*StarsDrawScannerCount)(void *, StarsU16,
    const StarsDrawScannerFleet *, StarsPoint);
typedef struct StarsDrawScannerOps {
    StarsDrawScannerGetDc get_dc;
    StarsDrawScannerReleaseDc release_dc;
    StarsDrawScannerClientRect get_client_rect;
    StarsDrawScannerSetClip set_clip;
    StarsDrawScannerSetClip reset_clip;
    StarsDrawScannerTransform point_to_scan;
    StarsDrawScannerTransform scan_to_point;
    StarsDrawScannerLinkFleets link_fleets;
    StarsDrawScannerFill fill_background;
    StarsDrawScannerRadar draw_radar_circle;
    StarsDrawScannerPlanetMark draw_planet;
    StarsDrawScannerFleetMark draw_fleet;
    StarsDrawScannerThingMark draw_thing;
    StarsDrawScannerPath draw_path;
    StarsDrawScannerCount draw_fleet_count;
} StarsDrawScannerOps;
typedef struct StarsDrawScannerInput {
    StarsU16 scanner_window;
    StarsI16 scrollbar_height;
    StarsI16 scan_zoom;
    StarsI16 galaxy_inverse;
    StarsI16 x_scan_top;
    StarsI16 y_scan_top;
    StarsI16 player;
    StarsI16 radar_percent;
    StarsU16 view_flags;
    StarsU16 mine_flags;
    StarsU16 hdc;
    StarsDrawScannerSelection selection;
    const StarsDrawScannerPlanet *planets;
    StarsU16 planet_count;
    const StarsDrawScannerFleet *fleets;
    StarsU16 fleet_count;
    const StarsDrawScannerThing *things;
    StarsU16 thing_count;
    const StarsDrawScannerOps *ops;
    void *context;
} StarsDrawScannerInput;
StarsI16 stars_draw_scanner(const StarsRect *, StarsDrawScannerInput *);
typedef StarsI16 (*StarsRedrawScanVisible)(void *, StarsU16);
typedef StarsU16 (*StarsRedrawScanGetDc)(void *, StarsU16);
typedef void (*StarsRedrawScanReleaseDc)(void *, StarsU16, StarsU16);
typedef void (*StarsRedrawScanDrawPath)(void *, StarsU16, StarsI16);
typedef void (*StarsRedrawScanDraw)(void *, StarsU16, const StarsRect *);
typedef void (*StarsRedrawScanLogicalToScan)(void *, StarsPoint *);
typedef struct StarsRedrawScanInput {
    StarsU16 scanner_window;
    StarsU16 visible;
    StarsI16 scan_zoom;
    StarsU16 scan_flags;
    StarsScanSelection *selection;
    StarsPoint selected_point;
    StarsI16 selected_id;
    StarsI16 selected_kind;
    StarsI16 selected_full;
    StarsRedrawScanVisible is_visible;
    StarsRedrawScanGetDc get_dc;
    StarsRedrawScanReleaseDc release_dc;
    StarsRedrawScanDrawPath draw_ship_path;
    StarsRedrawScanDraw draw_scanner;
    StarsRedrawScanLogicalToScan logical_to_scan;
    void *context;
} StarsRedrawScanInput;
void stars_redraw_scan_selection(StarsU16, StarsI16,
    StarsRedrawScanInput *);
typedef struct StarsWayPointScan {
    StarsPoint point;
    StarsU16 grobj;
    StarsU16 grobj_full;
    StarsI16 idpl;
    StarsI16 ifl;
    StarsI16 iwp;
    StarsI16 ith;
} StarsWayPointScan;
typedef StarsI16 (*StarsWayPointScanDistance)(void *, StarsI16);
typedef StarsI16 (*StarsWayPointReallocate)(void *, StarsMoveOrder **,
    StarsU16);
typedef void (*StarsWayPointBeep)(void *, StarsU16);
typedef void (*StarsWayPointDrawPath)(void *, StarsU16, StarsI16);
typedef StarsU16 (*StarsWayPointGetDc)(void *, StarsU16);
typedef void (*StarsWayPointReleaseDc)(void *, StarsU16, StarsU16);
typedef StarsU16 (*StarsWayPointWarpBest)(void *, StarsU16,
    const StarsMoveOrder *);
typedef void (*StarsWayPointRefreshFleet)(void *);
typedef void (*StarsWayPointChangeSelection)(void *,
    const StarsWayPointScan *, StarsI16);
typedef void (*StarsWayPointLogicalToScan)(void *, StarsPoint *);
typedef void (*StarsWayPointBoundPoints)(void *, StarsRect *,
    const StarsPoint *, StarsI16);
typedef void (*StarsWayPointInvalidate)(void *, StarsU16, const StarsRect *,
    StarsI16);
typedef void (*StarsWayPointAlertLimit)(void *, StarsU16, StarsI16);
typedef struct StarsAddWayPointInput {
    StarsMoveOrder *orders;
    StarsU16 order_count;
    StarsU16 order_capacity;
    StarsI16 active_waypoint;
    StarsU16 scan_flags;
    StarsU16 scanner_window;
    const StarsU16 *fleet_ids;
    StarsU16 fleet_count;
    const StarsU16 *thing_ids;
    StarsU16 thing_count;
    StarsWayPointScanDistance scan_to_point;
    StarsWayPointReallocate reallocate_orders;
    StarsWayPointBeep message_beep;
    StarsWayPointAlertLimit alert_limit;
    StarsWayPointGetDc get_dc;
    StarsWayPointDrawPath draw_ship_path;
    StarsWayPointReleaseDc release_dc;
    StarsWayPointWarpBest warp_best;
    StarsWayPointDrawPath redraw_selection;
    StarsWayPointRefreshFleet lookup_fleet;
    StarsWayPointChangeSelection change_selection;
    StarsWayPointLogicalToScan logical_to_scan;
    StarsWayPointBoundPoints bound_points;
    StarsWayPointInvalidate invalidate_rect;
    void *context;
} StarsAddWayPointInput;
StarsI16 stars_add_waypoint(StarsAddWayPointInput *, StarsPoint,
    StarsWayPointScan *);
typedef StarsI16 (*StarsChangeSelFindNearest)(void *, StarsPoint, StarsU16,
    StarsWayPointScan *);
typedef void (*StarsChangeSelRedraw)(void *, StarsU16, StarsI16);
typedef StarsI16 (*StarsChangeSelThing)(void *, StarsI16, StarsPoint *);
typedef void (*StarsChangeSelFillOrders)(void *);
typedef void (*StarsChangeSelOrderRow)(void *, StarsI16);
typedef void (*StarsChangeSelUpdateOrders)(void *, StarsI16);
typedef void (*StarsChangeSelEnsurePoint)(void *, StarsPoint, StarsI16);
typedef void (*StarsChangeSelScrollBar)(void *, StarsU16);
typedef void (*StarsChangeSelTitle)(void *, StarsU16);
typedef StarsU16 (*StarsChangeSelGetDc)(void *, StarsU16);
typedef void (*StarsChangeSelDrawScanner)(void *, StarsU16, const StarsRect *);
typedef void (*StarsChangeSelReleaseDc)(void *, StarsU16, StarsU16);
typedef void (*StarsChangeSelDrawShip)(void *, StarsU16, StarsU16);
typedef void (*StarsChangeSelAdvanceTutor)(void *);
typedef struct StarsChangeScanSelInput {
    StarsScanSelection *selection;
    StarsI16 active_waypoint;
    StarsU16 scanner_window;
    StarsU16 mine_window;
    StarsU16 player;
    StarsU16 tutorial_flags;
    StarsU16 special_radius;
    StarsChangeSelFindNearest find_nearest;
    StarsChangeSelRedraw redraw_selection;
    StarsChangeSelThing get_thing;
    StarsChangeSelFillOrders fill_orders;
    StarsChangeSelOrderRow set_order_row;
    StarsChangeSelUpdateOrders update_orders;
    StarsChangeSelEnsurePoint ensure_point;
    StarsChangeSelScrollBar draw_scroll_bar;
    StarsChangeSelTitle set_title;
    StarsChangeSelGetDc get_dc;
    StarsChangeSelDrawScanner draw_scanner;
    StarsChangeSelReleaseDc release_dc;
    StarsWayPointLogicalToScan logical_to_scan;
    StarsChangeSelDrawShip draw_ship;
    StarsChangeSelAdvanceTutor advance_tutor;
    void *context;
} StarsChangeScanSelInput;
void stars_change_scan_selection(StarsChangeScanSelInput *,
    StarsWayPointScan *, StarsI16);
typedef struct StarsMeasureBar {
    StarsPoint point;
    StarsI16 id;
    StarsU16 kind;
    const StarsU8 *label;
} StarsMeasureBar;
typedef StarsI16 (*StarsMeasureMouseMove)(void *, StarsPoint *);
typedef void (*StarsMeasureTransform)(void *, StarsPoint *);
typedef StarsI16 (*StarsMeasureFindNearest)(void *, StarsPoint, StarsU16,
    StarsWayPointScan *);
typedef StarsU16 (*StarsMeasureGetDc)(void *, StarsU16);
typedef StarsU16 (*StarsMeasureSelectPen)(void *, StarsU16, StarsU16);
typedef StarsU16 (*StarsMeasureSetRop)(void *, StarsU16, StarsU16);
typedef void (*StarsMeasureCapture)(void *, StarsU16);
typedef void (*StarsMeasureReleaseCapture)(void *);
typedef void (*StarsMeasureReleaseDc)(void *, StarsU16, StarsU16);
typedef void (*StarsMeasureLine)(void *, StarsU16, StarsI16, StarsI16);
typedef void (*StarsMeasureDrawBar)(void *, StarsU16,
    const StarsMeasureBar *);
typedef StarsI16 (*StarsMeasureShiftDown)(void *);
typedef void (*StarsMeasureNoObjectText)(void *, StarsU8 *, StarsU16);
typedef struct StarsMeasureTapeInput {
    StarsU16 scanner_window;
    StarsI16 client_right;
    StarsI16 client_bottom;
    StarsI16 scrollbar_height;
    StarsMeasureMouseMove mouse_move;
    StarsMeasureTransform logical_to_scan;
    StarsMeasureTransform scan_to_logical;
    StarsMeasureFindNearest find_nearest;
    StarsMeasureGetDc get_dc;
    StarsMeasureSelectPen select_pen;
    StarsMeasureSetRop set_rop;
    StarsMeasureCapture capture;
    StarsMeasureReleaseCapture release_capture;
    StarsMeasureReleaseDc release_dc;
    StarsMeasureLine move_to;
    StarsMeasureLine line_to;
    StarsMeasureDrawBar draw_bar;
    StarsMeasureShiftDown shift_down;
    StarsMeasureNoObjectText no_object_text;
    const StarsU16 *fleet_ids;
    StarsU16 fleet_count;
    const StarsU16 *thing_ids;
    StarsU16 thing_count;
    void *context;
} StarsMeasureTapeInput;
StarsI16 stars_handle_measuring_tape(StarsMeasureTapeInput *,
    StarsWayPointScan *, StarsPoint);
typedef void (*StarsResourceSelectFont)(void *, StarsU16, StarsU16);
typedef const StarsU8 *(*StarsResourceString)(void *, StarsU16);
typedef void (*StarsResourceCenterText)(void *, StarsU16, StarsI16, StarsI16,
    const StarsU8 *, StarsU16);
typedef void (*StarsResourceFormatPercent)(void *, StarsI16, StarsU8 *,
    StarsU16);
typedef void (*StarsResourceWrapText)(void *, StarsU16, StarsI16 *, StarsI16 *,
    const StarsU8 *, StarsI16, StarsI16, StarsI16, StarsI16 *, StarsI16);
typedef struct StarsDisplayResourceInfoInput {
    StarsU16 font_small;
    StarsU16 font_large;
    StarsI16 width;
    StarsI16 print;
    StarsI16 line_height;
    StarsI16 race_stat;
    StarsI16 planet_id;
    StarsI16 planet_variable;
    StarsI16 planet_value;
    StarsI16 resource_text_value;
    StarsU16 hdc;
    StarsResourceSelectFont select_font;
    StarsResourceString get_string;
    StarsResourceCenterText center_text;
    StarsResourceFormatPercent format_percent;
    StarsResourceWrapText wrap_text;
    void *context;
} StarsDisplayResourceInfoInput;
StarsPoint stars_display_resource_info(const StarsDisplayResourceInfoInput *);
typedef void (*StarsDisplayComponentSelectFont)(void *, StarsU16, StarsU16);
typedef void (*StarsDisplayComponentSelectBrush)(void *, StarsU16, StarsU16);
typedef void (*StarsDisplayComponentSelectPen)(void *, StarsU16, StarsU16);
typedef void (*StarsDisplayComponentSetColor)(void *, StarsU16, StarsU32);
typedef StarsI16 (*StarsDisplayComponentString)(void *, StarsU16, StarsU8 *,
    StarsU16);
typedef StarsI16 (*StarsDisplayComponentFormat)(void *, StarsI32, StarsU8 *,
    StarsU16);
typedef StarsI16 (*StarsDisplayComponentTextExtent)(void *, StarsU16,
    const StarsU8 *, StarsI16);
typedef void (*StarsDisplayComponentTextOut)(void *, StarsU16, StarsI16,
    StarsI16, const StarsU8 *, StarsI16);
typedef void (*StarsDisplayComponentRightText)(void *, StarsU16, StarsI16,
    StarsI16, const StarsU8 *, StarsI16);
typedef void (*StarsDisplayComponentCenterText)(void *, StarsU16, StarsI16,
    StarsI16, const StarsU8 *, StarsI16);
typedef void (*StarsDisplayComponentWrapText)(void *, StarsU16, StarsRect *,
    const StarsU8 *, StarsI16);
typedef void (*StarsDisplayComponentFrame)(void *, StarsU16,
    const StarsRect *, StarsI16);
typedef void (*StarsDisplayComponentFill)(void *, StarsU16, StarsI16,
    StarsI16, StarsI16, StarsI16, StarsU32);
typedef void (*StarsDisplayComponentImage)(void *, StarsU16, StarsI16,
    StarsI16, StarsI16);
typedef StarsI32 (*StarsDisplayComponentCost)(void *, StarsI16,
    const void *);
typedef void (*StarsDisplayComponentGraph)(void *, StarsU16, const StarsRect *,
    StarsI16, StarsI16, StarsI16);
typedef struct StarsDisplayComponentPart {
    StarsU16 present;
    StarsI16 kind;
    StarsI16 technology;
    StarsI16 armor_damage;
    StarsI16 engine_damage;
    StarsU16 fuel_max;
    StarsI16 hull_id;
    StarsI16 image_id;
    StarsI16 mass;
    StarsI16 resistance;
    StarsU16 hull_flags;
    StarsU16 component_flags;
    StarsU16 mineral_cost[6];
    const StarsU8 *name;
} StarsDisplayComponentPart;
typedef struct StarsDisplayComponentOps {
    StarsDisplayComponentSelectFont select_font;
    StarsDisplayComponentSelectBrush select_brush;
    StarsDisplayComponentSelectPen select_pen;
    StarsDisplayComponentSetColor set_text_color;
    StarsDisplayComponentString get_string;
    StarsDisplayComponentFormat format_number;
    StarsDisplayComponentTextExtent text_extent;
    StarsDisplayComponentTextOut text_out;
    StarsDisplayComponentRightText right_text;
    StarsDisplayComponentCenterText center_text;
    StarsDisplayComponentWrapText wrap_text;
    StarsDisplayComponentFrame draw_frame;
    StarsDisplayComponentFill fill_pattern;
    StarsDisplayComponentImage draw_image;
    StarsDisplayComponentCost cost_of_developing;
    StarsDisplayComponentGraph draw_graph;
} StarsDisplayComponentOps;
typedef struct StarsDisplayComponentInput {
    StarsU16 hdc;
    StarsI16 width;
    StarsI16 height;
    StarsI16 arial6_height;
    StarsI16 arial7_height;
    StarsI16 arial8_height;
    StarsI16 arial10_height;
    StarsI16 max_mineral_width;
    StarsU16 font_arial6;
    StarsU16 font_arial7;
    StarsU16 font_arial8;
    StarsU16 font_arial8_bold;
    StarsU16 font_arial10;
    StarsU16 brush_shadow;
    StarsU16 brush_highlight;
    StarsU16 brush_gray;
    StarsU16 brush_pattern;
    StarsU16 pen_dark_blue;
    StarsU16 pen_radar;
    StarsU32 color_black;
    StarsU32 color_disabled;
    StarsU32 color_button_text;
    StarsU32 color_button_highlight;
    StarsU16 game_flags;
    StarsU16 popup_flags;
    StarsI16 player;
    StarsI16 tech_level;
    const StarsU16 *player_technology;
    const StarsDisplayComponentPart *part;
    const StarsDisplayComponentOps *ops;
    void *context;
} StarsDisplayComponentInput;
void stars_display_component_info(const StarsDisplayComponentInput *);
typedef void (*StarsFactorySelectFont)(void *, StarsU16, StarsU16);
typedef const StarsU8 *(*StarsFactoryString)(void *, StarsU16);
typedef const StarsU8 *(*StarsFactoryPlanetName)(void *, StarsI16);
typedef void (*StarsFactoryFormatTitle)(void *, StarsU16, StarsU16,
    StarsU8 *, StarsU16);
typedef void (*StarsFactoryFormatValue)(void *, StarsI16, StarsU16,
    StarsU8 *, StarsU16);
typedef void (*StarsFactoryFormatPercent)(void *, StarsI16, StarsU8 *,
    StarsU16);
typedef void (*StarsFactoryCenterText)(void *, StarsU16, StarsI16, StarsI16,
    const StarsU8 *, StarsU16);
typedef void (*StarsFactoryWrapText)(void *, StarsU16, StarsI16 *, StarsI16 *,
    const StarsU8 *, StarsI16, StarsI16, StarsI16, StarsI16 *, StarsI16);
typedef struct StarsDisplayFactoryMineInfoInput {
    StarsU16 font_small;
    StarsU16 font_large;
    StarsI16 width;
    StarsI16 print;
    StarsI16 line_height;
    StarsI16 race_stat;
    StarsI16 planet_id;
    StarsI16 planet_variable;
    StarsI16 planet_value;
    StarsI16 planet_max;
    StarsU16 hdc;
    StarsFactorySelectFont select_font;
    StarsFactoryString get_string;
    StarsFactoryPlanetName get_planet_name;
    StarsFactoryFormatTitle format_title;
    StarsFactoryFormatValue format_value;
    StarsFactoryFormatPercent format_percent;
    StarsFactoryCenterText center_text;
    StarsFactoryWrapText wrap_text;
    void *context;
} StarsDisplayFactoryMineInfoInput;
StarsPoint stars_display_factory_mine_info(
    const StarsDisplayFactoryMineInfoInput *);
typedef StarsI16 (*StarsReportHeaderString)(void *, StarsU16, StarsU8 *,
    StarsU16);
typedef StarsI16 (*StarsReportTextExtent)(void *, StarsU16, const StarsU8 *,
    StarsU16);
typedef struct StarsReportColumnHeaderInput {
    StarsU16 hdc;
    StarsReportHeaderString get_string;
    StarsReportTextExtent text_extent;
    void *context;
} StarsReportColumnHeaderInput;
StarsI16 stars_report_column_header(const StarsReportColumnHeaderInput *,
    StarsI16, StarsI16, StarsU8 *);
typedef void (*StarsDrawReportSelectObject)(void *, StarsU16, StarsU16);
typedef void (*StarsDrawReportSetBkMode)(void *, StarsU16, StarsU16);
typedef void (*StarsDrawReportPatBlt)(void *, StarsU16, StarsI16, StarsI16,
    StarsI16, StarsI16, StarsU32);
typedef void (*StarsDrawReportFillRect)(void *, StarsU16, const StarsRect *,
    StarsU16);
typedef void (*StarsDrawReportTextOut)(void *, StarsU16, StarsI16, StarsI16,
    const StarsU8 *, StarsI16);
typedef void (*StarsDrawReportCenterText)(void *, StarsU16, StarsI16,
    StarsI16, const StarsU8 *, StarsI16);
typedef void (*StarsDrawReportFrame)(void *, StarsU16, const StarsRect *,
    StarsU16);
typedef StarsI16 (*StarsDrawReportHeader)(void *, StarsI16, StarsI16,
    StarsU8 *, StarsU16);
typedef void (*StarsDrawReportItem)(void *, StarsU16, const StarsRect *,
    StarsI16, StarsI16, StarsI16);
typedef struct StarsDrawReportOps {
    StarsDrawReportSelectObject select_object;
    StarsDrawReportSetBkMode set_background_mode;
    StarsDrawReportPatBlt pat_blt;
    StarsDrawReportFillRect fill_rect;
    StarsDrawReportTextOut text_out;
    StarsDrawReportCenterText center_text;
    StarsDrawReportFrame draw_frame;
    StarsDrawReportHeader header_width;
    StarsDrawReportItem draw_item;
} StarsDrawReportOps;
typedef struct StarsDrawReportInput {
    StarsReportViewState *view;
    StarsI16 arial8_height;
    StarsU16 font;
    StarsU16 shadow_brush;
    StarsU16 button_face_brush;
    StarsU16 fill_cells;
    const StarsDrawReportOps *ops;
    void *context;
} StarsDrawReportInput;
void stars_draw_report(StarsU16, StarsU16, const StarsRect *,
    StarsDrawReportInput *);

typedef void (*StarsReportDlgGetWindowRect)(void *, StarsU16, StarsRect *);
typedef StarsU16 (*StarsReportDlgGetDc)(void *, StarsU16);
typedef void (*StarsReportDlgSelectObject)(void *, StarsU16, StarsU16);
typedef void (*StarsReportDlgReleaseDc)(void *, StarsU16, StarsU16);
typedef void (*StarsReportDlgSetWindowPos)(void *, StarsU16, StarsI16,
    StarsI16, StarsI16, StarsI16, StarsU16);
typedef void (*StarsReportDlgSticky)(void *, StarsU16, StarsPoint *, StarsI16);
typedef StarsU16 (*StarsReportDlgCreateScroll)(void *, const StarsU8 *,
    StarsU32, StarsU16);
typedef void (*StarsReportDlgAdvanceTutor)(void *);
typedef void (*StarsReportDlgSetScrollPos)(void *, StarsU16, StarsU16,
    StarsI16, StarsI16);
typedef void (*StarsReportDlgSetScrollRange)(void *, StarsU16, StarsU16,
    StarsI16, StarsI16, StarsI16);
typedef StarsI16 (*StarsReportDlgGetScrollPos)(void *, StarsU16, StarsU16);
typedef void (*StarsReportDlgScrollWindow)(void *, StarsU16, StarsI16,
    StarsI16, const StarsRect *);
typedef StarsU16 (*StarsReportDlgGetMetric)(void *, StarsU16);
typedef StarsU16 (*StarsReportDlgBeginPaint)(void *, StarsU16, StarsRect *);
typedef void (*StarsReportDlgEndPaint)(void *, StarsU16, StarsRect *);
typedef void (*StarsReportDlgFillRect)(void *, StarsU16, const StarsRect *,
    StarsU16);
typedef void (*StarsReportDlgDestroyWindow)(void *, StarsU16);
typedef StarsU16 (*StarsReportDlgGetSubMenu)(void *, StarsU16, StarsI16);
typedef void (*StarsReportDlgCheckMenuItem)(void *, StarsU16, StarsU16,
    StarsU16);
typedef void (*StarsReportDlgSetMinTrack)(void *, StarsU32, StarsI16,
    StarsI16);
typedef void (*StarsReportDlgInvalidate)(void *, StarsU16);
typedef void (*StarsReportDlgUpdateWindow)(void *, StarsU16);
typedef StarsI32 (*StarsReportDlgDefaultProc)(void *, StarsU16, StarsU16,
    StarsU16, StarsU32);
typedef StarsI16 (*StarsReportDlgHeaderWidth)(void *, StarsI16, StarsI16,
    StarsU16, StarsU8 *, StarsU16);
typedef void (*StarsReportDlgSortCache)(void *, StarsI16, StarsI16);
typedef void (*StarsReportDlgSetHScroll)(void *);
typedef void (*StarsReportDlgDraw)(void *, StarsU16, StarsU16,
    const StarsRect *);
typedef void (*StarsReportDlgDrawView)(void *, StarsU16, StarsU16,
    const StarsRect *, const StarsReportViewState *);
typedef void (*StarsReportDlgColumnPopup)(void *, StarsPoint, StarsI16,
    StarsI16);
typedef void (*StarsReportDlgExecuteClick)(void *, StarsPoint, StarsI16,
    StarsI16, StarsI16);
typedef void (*StarsReportDlgClearCurrent)(void *);
typedef struct StarsReportDlgState StarsReportDlgState;
typedef StarsI16 (*StarsReportDlgLoadView)(void *, const void *,
    StarsReportDlgState *);
struct StarsReportDlgState {
    StarsReportViewState view;
    struct StarsReportPopupState *popup;
    StarsPoint dialog_position;
    StarsPoint dialog_size;
    StarsU16 frame_window;
    StarsU16 vscroll_window;
    StarsU16 instance;
    StarsU16 font_arial8;
    StarsU16 button_face_brush;
    StarsI16 arial8_height;
    StarsI16 sort_column;
    StarsI16 subsort;
    StarsI16 ascending;
    StarsU16 game_bits;
    StarsU16 browser_valid;
    StarsU16 current_valid;
};
typedef struct StarsReportDlgOps {
    StarsReportDlgGetWindowRect get_window_rect;
    StarsGetClientRect get_client_rect;
    StarsReportDlgGetDc get_dc;
    StarsReportDlgSelectObject select_object;
    StarsReportDlgReleaseDc release_dc;
    StarsReportDlgSetWindowPos set_window_pos;
    StarsReportDlgSticky sticky_position;
    StarsReportDlgCreateScroll create_scroll;
    StarsReportDlgAdvanceTutor advance_tutor;
    StarsReportDlgGetMetric get_system_metrics;
    StarsReportDlgSetScrollPos set_scroll_pos;
    StarsReportDlgSetScrollRange set_scroll_range;
    StarsReportDlgGetScrollPos get_scroll_pos;
    StarsReportDlgScrollWindow scroll_window;
    StarsReportDlgBeginPaint begin_paint;
    StarsReportDlgEndPaint end_paint;
    StarsReportDlgFillRect fill_rect;
    StarsReportDlgDestroyWindow destroy_window;
    StarsReportDlgGetSubMenu get_submenu;
    StarsReportDlgCheckMenuItem check_menu_item;
    StarsReportDlgSetMinTrack set_min_track;
    StarsReportDlgInvalidate invalidate;
    StarsReportDlgUpdateWindow update_window;
    StarsReportDlgDefaultProc default_proc;
    StarsReportDlgHeaderWidth header_width;
    StarsReportDlgSortCache sort_cache;
    StarsReportDlgSetHScroll set_h_scroll_bar;
    StarsReportDlgDraw draw_report;
    StarsReportDlgDrawView draw_report_view;
    StarsReportDlgColumnPopup column_popup;
    StarsReportDlgExecuteClick execute_click;
    StarsReportDlgClearCurrent clear_current;
    StarsReportDlgLoadView load_view;
} StarsReportDlgOps;
typedef struct StarsReportDlgInput {
    StarsReportDlgState *state;
    const void *packed_view;
    const StarsReportDlgOps *ops;
    void *context;
} StarsReportDlgInput;
StarsI32 stars_report_dlg(StarsReportDlgInput *, StarsU16, StarsU16,
    StarsU16, StarsU32);

/* Production bridge for the recovered 54-byte `_rpt` view. */
#define STARS_REPORT_DLG_VIEW_SIZE 54
typedef struct StarsReportDlgPackedInput {
    const StarsU8 *view_record;
    StarsU16 view_size;
    StarsReportDlgInput report;
} StarsReportDlgPackedInput;
StarsI32 stars_report_dlg_packed(StarsReportDlgPackedInput *, StarsU16,
    StarsU16, StarsU16, StarsU32);

/* ExecuteReportClick translates a report cell into the corresponding fleet,
 * planet, battle, scan, transfer, or popup action.  The packed report rows and
 * GlobalPD union are represented by typed records and callbacks. */
typedef struct StarsExecuteReportClickFleet {
    StarsI16 id;
    StarsI16 planet_id;
    StarsPoint point;
    StarsU16 flags;
} StarsExecuteReportClickFleet;
typedef struct StarsExecuteReportClickPlanet {
    StarsI16 id;
    StarsU16 has_starbase;
    StarsU16 starbase_hull;
    StarsU16 starbase_design_index;
    StarsU16 defense_count;
    StarsU16 flags;
    /* Original _planet.overlap_4 bits used by popup branches in addition to
     * the service/resource flags represented above. */
    StarsU16 raw_status_flags;
    StarsI16 mineral_concentration[3];
    StarsI16 mineral_value[4];
    StarsI16 player_value[4];
    StarsU32 mineral_weight;
    StarsI16 resources;
    StarsI16 research_percent;
} StarsExecuteReportClickPlanet;
typedef struct StarsExecuteReportClickBattle {
    StarsU16 id;
    StarsPoint point;
} StarsExecuteReportClickBattle;
#define STARS_EXECUTE_REPORT_CLICK_POPUP_SIZE 22
typedef struct StarsExecuteReportClickPopup {
    /* GlobalPD is an overlapping 22-byte union.  Preserve its raw bytes and
     * pointer/part overlays at this boundary; callers own address validity. */
    StarsU8 raw[STARS_EXECUTE_REPORT_CLICK_POPUP_SIZE];
    StarsI32 rgi[5];
    StarsU8 part_bytes[8];
    StarsU32 fleet_far;
    StarsU32 starbase_fleet_far;
    StarsU16 resource_part;
    StarsI16 kind;
    StarsI16 planet_id;
    StarsI16 planet_variable;
    StarsI16 planet_value;
    StarsI16 planet_minimum;
    StarsI16 planet_maximum;
    StarsI16 player_value;
    StarsI16 player_minimum;
    StarsI16 player_maximum;
    StarsI16 mineral_index;
    StarsI16 mineral_value;
    StarsU16 part;
    StarsU16 starbase_hull;
} StarsExecuteReportClickPopup;
typedef StarsU16 (*StarsExecuteReportClickGetDc)(void *, StarsU16);
typedef void (*StarsExecuteReportClickReleaseDc)(void *, StarsU16, StarsU16);
typedef StarsI16 (*StarsExecuteReportClickGetFleet)(void *, StarsI16, StarsI16,
    StarsExecuteReportClickFleet *);
typedef StarsI16 (*StarsExecuteReportClickGetPlanet)(void *, StarsI16,
    StarsExecuteReportClickPlanet *);
typedef StarsI16 (*StarsExecuteReportClickGetBattle)(void *, StarsI16,
    StarsExecuteReportClickBattle *);
typedef void (*StarsExecuteReportClickSelectFleet)(void *, StarsI16);
typedef void (*StarsExecuteReportClickSelectPlanet)(void *, StarsI16);
typedef void (*StarsExecuteReportClickInvalidate)(void *, StarsI16, StarsI16);
typedef StarsI16 (*StarsExecuteReportClickDestIsWp0)(void *,
    const StarsExecuteReportClickFleet *);
typedef void (*StarsExecuteReportClickSendMessage)(void *, StarsU16, StarsU16,
    StarsU16, StarsU32);
typedef void (*StarsExecuteReportClickSetScanWp)(void *, StarsI16);
typedef void (*StarsExecuteReportClickTransfer)(void *, StarsI16, StarsI16,
    StarsI16, StarsI16, StarsI16);
typedef void (*StarsExecuteReportClickShowPopup)(void *, StarsPoint,
    const StarsExecuteReportClickPopup *);
typedef void (*StarsExecuteReportClickBeep)(void *);
typedef StarsI16 (*StarsExecuteReportClickNearest)(void *, StarsPoint,
    StarsI16, StarsI16 *);
typedef void (*StarsExecuteReportClickChangeScan)(void *, StarsPoint, StarsI16,
    StarsI16);
typedef void (*StarsExecuteReportClickEnsurePoint)(void *, StarsPoint);
typedef void (*StarsExecuteReportClickBattleVcr)(void *, StarsU16);
typedef void (*StarsExecuteReportClickCenterScan)(void *, StarsPoint);
typedef void (*StarsExecuteReportClickChangeProduction)(void *);
typedef StarsI16 (*StarsExecuteReportClickMaxValue)(void *,
    const StarsExecuteReportClickPlanet *, StarsI16);
typedef StarsI16 (*StarsExecuteReportClickBestDefense)(void *, StarsU16 *);
typedef void (*StarsExecuteReportClickEstimateMinerals)(void *,
    const StarsExecuteReportClickPlanet *, StarsI32 *);
typedef struct StarsExecuteReportClickState {
    StarsU16 report_window;
    StarsU16 ship_list_window;
    StarsU16 production_window;
    StarsU16 vcr_window;
    StarsI16 transfer_dialog;
    StarsI16 player_id;
    StarsI16 enemy_fleet_report;
    StarsI16 enemy_fleet_rows;
    StarsI16 current_enemy_fleet_row;
    StarsU16 visible_fields_low;
    StarsU16 visible_fields_high;
    StarsI16 first_field;
    StarsI16 field_width[16];
    StarsI16 selected_scan_x;
    StarsI16 selected_scan_y;
    StarsI16 selected_planet_id;
    StarsI16 selected_fleet_id;
} StarsExecuteReportClickState;
typedef struct StarsExecuteReportClickOps {
    StarsExecuteReportClickGetDc get_dc;
    StarsExecuteReportClickReleaseDc release_dc;
    StarsExecuteReportClickGetFleet get_fleet;
    StarsExecuteReportClickGetPlanet get_planet;
    StarsExecuteReportClickGetBattle get_battle;
    StarsExecuteReportClickSelectFleet select_fleet;
    StarsExecuteReportClickSelectPlanet select_planet;
    StarsExecuteReportClickInvalidate invalidate_report;
    StarsExecuteReportClickDestIsWp0 dest_is_wp0;
    StarsExecuteReportClickSendMessage send_message;
    StarsExecuteReportClickSetScanWp set_scan_waypoint;
    StarsExecuteReportClickTransfer transfer;
    StarsExecuteReportClickShowPopup popup;
    StarsExecuteReportClickBeep message_beep;
    StarsExecuteReportClickNearest find_nearest;
    StarsExecuteReportClickChangeScan change_scan_selection;
    StarsExecuteReportClickEnsurePoint ensure_point_on_screen;
    StarsExecuteReportClickBattleVcr battle_vcr;
    StarsExecuteReportClickCenterScan center_scan;
    StarsExecuteReportClickChangeProduction change_production;
    StarsExecuteReportClickMaxValue max_mines;
    StarsExecuteReportClickMaxValue max_operable_mines;
    StarsExecuteReportClickMaxValue max_factories;
    StarsExecuteReportClickMaxValue max_operable_factories;
    StarsExecuteReportClickBestDefense best_defense;
    StarsExecuteReportClickEstimateMinerals estimate_minerals;
    /* Optional raw Win16 GlobalPD owner/part views.  They are copied but
     * never dereferenced by the reconstructed dispatcher. */
    StarsU32 globalpd_fleet_far;
    StarsU32 globalpd_starbase_fleet_far;
    StarsU16 globalpd_starbase_part;
    const StarsU8 *globalpd_defense_part;
    StarsU16 globalpd_defense_part_size;
    /* Legacy caller field; original resource popup derives the part alias
     * from iPlanetVar and preserves the planet ID at union offset +2. */
    StarsU16 globalpd_resource_part;
    /* Optional persistent original 22-byte GlobalPD storage. Popup kinds
     * overwrite only their assigned union fields; other bytes survive. */
    StarsU8 *globalpd_raw;
    StarsU16 globalpd_raw_size;
    /* Optional original vlprgidFleet -> rglpfl pointer chain. Entries in
     * packed_fleet_far_table are four-byte offset:selector far pointers. */
    const StarsU16 *packed_fleet_row_ids;
    StarsU16 packed_fleet_row_count;
    const StarsU8 *packed_fleet_far_table;
    StarsU16 packed_fleet_far_table_size;
    /* Optional rgshdef far-pointer table, one four-byte entry per player.
     * The starbase design slot adds 0x93 bytes to the base offset. */
    const StarsU8 *packed_shdef_far_table;
    StarsU16 packed_shdef_far_table_size;
    /* Optional production raw-record lookup seam.  When present, these
     * records take precedence over the legacy typed lookup callbacks. */
    const StarsU8 *packed_fleet_record;
    StarsU16 packed_fleet_size;
    const StarsU8 *packed_planet_record;
    StarsU16 packed_planet_size;
    const StarsU8 *packed_battle_record;
    StarsU16 packed_battle_size;
    StarsI16 packed_starbase_hull;
    StarsI16 packed_resources;
    StarsI16 packed_research_percent;
    void *context;
} StarsExecuteReportClickOps;
void stars_execute_report_click(StarsExecuteReportClickState *, StarsPoint,
    StarsI16, StarsI16, StarsI16, const StarsExecuteReportClickOps *);

/* Typed report-row data consumed by DrawReportItem.  The original routine
 * reads packed planet, fleet, and battle records through several global far
 * arrays.  These records retain the values that affect its rendering while
 * keeping the reconstruction independent of those original offsets. */
#define STARS_REPORT_ITEM_SHIP_TYPES 16
#define STARS_REPORT_ITEM_BATTLE_TYPES 256
#define STARS_REPORT_ITEM_MINERALS 4

typedef struct StarsReportItemPlanet {
    StarsI16 id;
    StarsI16 player;
    StarsU16 inhabited;
    StarsI32 population;
    StarsI32 max_population;
    StarsI32 mineral_weight[3];
    StarsI32 estimated_minerals[3];
    StarsI16 mineral_concentration[3];
    StarsI16 capacity_percent;
    StarsI16 desirability;
    StarsI16 optimal_value;
    StarsI16 current_installations[3];
    StarsI16 operable_installations[3];
    StarsI16 defense_survival_percent;
    StarsI16 resources;
    StarsI16 research_percent;
    StarsI16 warp_speed;
    StarsI16 stargate;
    StarsI16 next_planet[2];
    StarsU16 brush_status[3];
    const StarsU8 *name;
    const StarsU8 *owner_name;
    const StarsU8 *next_name[2];
    const StarsU8 *production_name;
    const StarsU8 *desirability_label;
} StarsReportItemPlanet;

typedef struct StarsReportItemFleet {
    StarsI16 id;
    StarsI16 player;
    StarsI16 planet_id;
    StarsPoint position;
    StarsI16 cord;
    StarsI32 minerals[STARS_REPORT_ITEM_MINERALS];
    StarsI32 fuel;
    StarsI32 weight;
    StarsI16 fuel_safe;
    StarsI16 cloak_percent;
    StarsI16 plan;
    StarsI16 primary_design;
    StarsI16 primary_count;
    StarsU16 ship_counts[STARS_REPORT_ITEM_SHIP_TYPES];
    StarsI32 combat_ship_count;
    StarsI32 ship_category_count[6];
    StarsU16 scanner_percent;
    StarsU16 scanner_visible;
    StarsU16 task_color;
    const StarsU8 *name;
    const StarsU8 *location_name;
    const StarsU8 *destination_name;
    const StarsU8 *eta_name;
    const StarsU8 *task_name;
    const StarsU8 *plan_name;
    const StarsU8 *primary_name;
} StarsReportItemFleet;

typedef struct StarsReportItemBattle {
    StarsI16 planet_id;
    StarsPoint position;
    StarsI32 cplr;
    StarsI32 units[STARS_REPORT_ITEM_BATTLE_TYPES];
    StarsI32 kills[2];
    const StarsU8 *location_name;
} StarsReportItemBattle;

typedef const StarsReportItemPlanet *(*StarsReportItemPlanetAt)(void *,
    StarsI16);
typedef const StarsReportItemFleet *(*StarsReportItemFleetAt)(void *,
    StarsI16, StarsI16);
typedef const StarsReportItemBattle *(*StarsReportItemBattleAt)(void *,
    StarsI16);
typedef StarsI16 (*StarsReportItemFormat)(void *, StarsI32, StarsU8 *,
    StarsU16);
typedef StarsI16 (*StarsReportItemTextExtent)(void *, StarsU16,
    const StarsU8 *, StarsU16);
typedef void (*StarsReportItemSelectObject)(void *, StarsU16, StarsU16);
typedef void (*StarsReportItemSetColor)(void *, StarsU16, StarsU32);
typedef void (*StarsReportItemSetMode)(void *, StarsU16, StarsU16);
typedef void (*StarsReportItemExtText)(void *, StarsU16, StarsI16, StarsI16,
    StarsU16, const StarsRect *, const StarsU8 *, StarsU16);
typedef void (*StarsReportItemRightText)(void *, StarsU16, StarsI16,
    StarsI16, const StarsU8 *, StarsI16, StarsI16);
typedef void (*StarsReportItemTextOut)(void *, StarsU16, StarsI16, StarsI16,
    const StarsU8 *, StarsU16);
typedef void (*StarsReportItemCenterText)(void *, StarsU16, StarsI16,
    StarsI16, const StarsU8 *, StarsU16);
typedef void (*StarsReportItemFillRect)(void *, StarsU16, const StarsRect *,
    StarsU16);
typedef void (*StarsReportItemDrawMineral)(void *, StarsU16, StarsI16,
    StarsI16, StarsI16, StarsI32);
typedef void (*StarsReportItemDrawProduction)(void *, StarsU16,
    const StarsRect *, const StarsReportItemPlanet *);
typedef struct StarsDrawReportItemOps {
    StarsReportItemPlanetAt planet_at;
    StarsReportItemFleetAt fleet_at;
    StarsReportItemBattleAt battle_at;
    StarsReportItemFormat format_number;
    StarsReportItemFormat format_percent;
    StarsReportItemTextExtent text_extent;
    StarsReportItemSelectObject select_object;
    StarsReportItemSetColor set_text_color;
    StarsReportItemSetMode set_background_mode;
    StarsReportItemExtText ext_text_out;
    StarsReportItemRightText right_text_out;
    StarsReportItemTextOut text_out;
    StarsReportItemCenterText center_text;
    StarsReportItemFillRect fill_rect;
    StarsReportItemDrawMineral draw_mineral;
    StarsReportItemDrawProduction draw_production;
} StarsDrawReportItemOps;
typedef struct StarsDrawReportItemInput {
    StarsI16 player;
    StarsU16 font;
    StarsU16 selection_kind;
    StarsI16 selected_planet;
    StarsI16 selected_fleet;
    StarsI16 selected_scan_fleet;
    StarsPoint selected_scan_point;
    StarsU16 brush_planet_cargo;
    StarsU16 brush_planet_warp;
    StarsU16 brush_planet_gate;
    StarsU16 color_black;
    StarsU16 color_selected;
    StarsU16 color_warning;
    StarsU16 color_disabled;
    const StarsU8 *double_dash;
    const StarsDrawReportItemOps *ops;
    void *context;
} StarsDrawReportItemInput;
void stars_draw_report_item(StarsU16, StarsRect *, StarsI16, StarsI16,
    StarsI16, StarsDrawReportItemInput *);
typedef StarsU16 (*StarsScoreXBeginPaint)(void *, StarsU16, StarsRect *);
typedef void (*StarsScoreXEndPaint)(void *, StarsU16, StarsRect *);
typedef void (*StarsScoreXDraw)(void *, StarsU16, StarsU16);
typedef void (*StarsScoreXGetRect)(void *, StarsU16, StarsRect *);
typedef void (*StarsScoreXFillRect)(void *, StarsU16, const StarsRect *,
    StarsU16);
typedef void (*StarsScoreXGetCursor)(void *, StarsPoint *);
typedef void (*StarsScoreXMapCursor)(void *, StarsU16, StarsPoint *);
typedef void (*StarsScoreXSetCursor)(void *, StarsU16);
typedef void (*StarsScoreXInit)(void *, StarsU16, StarsU16);
typedef void (*StarsScoreXSticky)(void *, StarsU16, StarsI16);
typedef void (*StarsScoreXAdvanceTutor)(void *);
typedef void (*StarsScoreXEndDialog)(void *, StarsU16, StarsI16);
typedef void (*StarsScoreXHelp)(void *, StarsU16, const StarsU8 *, StarsU16,
    StarsU16);
typedef void (*StarsScoreXInvalidate)(void *, StarsU16);
typedef StarsI16 (*StarsScoreXString)(void *, StarsU16, StarsU8 *, StarsU16);
typedef StarsI16 (*StarsScoreXPopup)(void *, StarsU16, StarsI16, StarsI16,
    StarsU16, const StarsU8 *, const StarsU16 *, StarsI16, StarsI16);
typedef struct StarsScoreXOps {
    StarsScoreXBeginPaint begin_paint;
    StarsScoreXEndPaint end_paint;
    StarsScoreXDraw draw_history;
    StarsScoreXDraw draw_score;
    StarsScoreXDraw draw_victory;
    StarsScoreXGetRect get_client_rect;
    StarsScoreXFillRect fill_rect;
    StarsScoreXGetCursor get_cursor_pos;
    StarsScoreXMapCursor screen_to_client;
    StarsScoreXSetCursor set_cursor;
    StarsScoreXInit init_dialog;
    StarsScoreXSticky sticky_position;
    StarsScoreXAdvanceTutor advance_tutor;
    StarsScoreXEndDialog end_dialog;
    StarsScoreXHelp help;
    StarsScoreXInvalidate invalidate;
    StarsScoreXString get_string;
    StarsScoreXPopup popup_menu;
} StarsScoreXOps;
typedef struct StarsScoreXInput {
    StarsU16 mode;
    StarsU16 history_selection;
    StarsU16 history_flags;
    StarsU16 in_dialog;
    StarsU16 dialog_window;
    StarsU16 tutor_active;
    StarsI16 font_height;
    StarsI16 font_height_large;
    StarsU16 hand_cursor;
    StarsU16 button_face_brush;
    const StarsU8 *help_file;
    const StarsScoreXOps *ops;
    void *context;
} StarsScoreXInput;
StarsI16 stars_score_x_dlg(StarsScoreXInput *, StarsU16, StarsU16,
    StarsU16, StarsU32);
typedef StarsU16 (*StarsInitScoreGetDC)(void *, StarsU16);
typedef void (*StarsInitScoreSelectObject)(void *, StarsU16, StarsU16);
typedef StarsI16 (*StarsInitScoreTextExtent)(void *, StarsU16,
    const StarsU8 *, StarsU16);
typedef const StarsU8 *(*StarsInitScoreString)(void *, StarsU16);
typedef void (*StarsInitScoreReleaseDC)(void *, StarsU16, StarsU16);
typedef void (*StarsInitScoreGetRect)(void *, StarsU16, StarsRect *);
typedef StarsU16 (*StarsInitScoreGetDialogItem)(void *, StarsU16, StarsU16);
typedef void (*StarsInitScoreMapWindowPoints)(void *, StarsU16, StarsU16,
    StarsRect *, StarsU16);
typedef void (*StarsInitScoreOffsetRect)(void *, StarsRect *, StarsI16,
    StarsI16);
typedef void (*StarsInitScoreSetWindowPos)(void *, StarsU16, StarsI16,
    StarsI16, StarsI16, StarsI16, StarsU16);
typedef void (*StarsInitScoreSetWindowText)(void *, StarsU16,
    const StarsU8 *);
typedef struct StarsInitScoreDlgInput {
    StarsI16 victory_mode;
    StarsI16 player_count;
    StarsI16 line_height;
    StarsU16 font_arial8;
    StarsI16 *score_x;
    StarsInitScoreGetDC get_dc;
    StarsInitScoreSelectObject select_object;
    StarsInitScoreTextExtent text_extent;
    StarsInitScoreString get_string;
    StarsInitScoreReleaseDC release_dc;
    StarsInitScoreGetRect get_window_rect;
    StarsInitScoreGetRect get_client_rect;
    StarsInitScoreGetDialogItem get_dialog_item;
    StarsInitScoreMapWindowPoints map_window_points;
    StarsInitScoreOffsetRect offset_rect;
    StarsInitScoreSetWindowPos set_window_pos;
    StarsInitScoreSetWindowText set_window_text;
    void *context;
} StarsInitScoreDlgInput;
void stars_init_score_dlg(StarsU16, const StarsInitScoreDlgInput *);
typedef struct StarsDrawVcPlayerState {
    StarsU16 detection_flags;
    StarsU16 dead_flags;
    StarsU16 score_flags;
} StarsDrawVcPlayerState;
typedef const StarsU8 *(*StarsDrawVcPlayerName)(void *, StarsI16);
typedef StarsI16 (*StarsDrawVcGetValue)(void *, StarsI16, StarsI16);
typedef StarsI16 (*StarsDrawVcCheck)(void *, StarsI16);
typedef StarsI16 (*StarsDrawVcFormat)(void *, StarsI16, StarsU8 *, StarsU16);
typedef StarsI16 (*StarsDrawVcGetString)(void *, StarsI16, StarsU8 *, StarsU16);
typedef StarsU16 (*StarsDrawVcSelectObject)(void *, StarsU16, StarsU16);
typedef void (*StarsDrawVcSetColor)(void *, StarsU16, StarsU32);
typedef void (*StarsDrawVcSetMode)(void *, StarsU16, StarsU16);
typedef void (*StarsDrawVcPatBlt)(void *, StarsU16, StarsI16, StarsI16,
    StarsI16, StarsI16, StarsU32);
typedef void (*StarsDrawVcTextOut)(void *, StarsU16, StarsI16, StarsI16,
    const StarsU8 *, StarsU16);
typedef StarsI16 (*StarsDrawVcTextExtent)(void *, StarsU16, const StarsU8 *,
    StarsU16);
typedef StarsU16 (*StarsDrawVcCreateDC)(void *, StarsU16);
typedef void (*StarsDrawVcDeleteDC)(void *, StarsU16);
typedef void (*StarsDrawVcBitBlt)(void *, StarsU16, StarsI16, StarsI16,
    StarsI16, StarsI16, StarsU16, StarsI16, StarsI16, StarsU32);
typedef struct StarsDrawVcReportInput {
    StarsI16 player_count;
    StarsI16 plan_max;
    StarsI16 line_height;
    StarsI16 score_x;
    StarsU16 font_arial8_small;
    StarsU16 font_arial8_large;
    StarsU16 brush_button_shadow;
    StarsU16 bitmap_mono;
    StarsU32 color_button_shadow;
    const StarsU8 *percent_suffix;
    const StarsDrawVcPlayerState *players;
    StarsDrawVcPlayerName player_name;
    StarsDrawVcGetString get_string;
    StarsDrawVcGetValue get_value;
    StarsDrawVcCheck get_check;
    StarsDrawVcFormat format_value;
    StarsDrawVcSelectObject select_object;
    StarsDrawVcSetColor set_text_color;
    StarsDrawVcSetColor set_background_color;
    StarsDrawVcSetMode set_background_mode;
    StarsDrawVcPatBlt pat_blt;
    StarsDrawVcTextOut text_out;
    StarsDrawVcTextExtent text_extent;
    StarsDrawVcCreateDC create_compatible_dc;
    StarsDrawVcDeleteDC delete_dc;
    StarsDrawVcBitBlt bit_blt;
    void *context;
} StarsDrawVcReportInput;
void stars_draw_vc_report(StarsU16, const StarsDrawVcReportInput *);
typedef struct StarsDrawScorePlayerState {
    StarsU16 detection_flags;
    StarsU16 dead_flags;
    StarsU16 score_flags;
    StarsI32 planet_count;
    StarsI32 starbase_count;
    StarsU16 ship_counts[3];
    StarsI16 tech_levels;
    StarsI32 resources;
    StarsI32 score;
    StarsI16 overlap_value;
} StarsDrawScorePlayerState;
typedef StarsI16 (*StarsDrawScoreFormatLong)(void *, StarsI32, StarsU8 *,
    StarsU16);
typedef void (*StarsDrawScoreRightText)(void *, StarsU16, StarsI16, StarsI16,
    const StarsU8 *, StarsU16, StarsI16);
typedef struct StarsDrawScoreReportInput {
    StarsI16 player_count;
    StarsI16 line_height;
    StarsI16 score_x;
    StarsU16 font_arial8_small;
    StarsU16 font_arial8_large;
    StarsU16 brush_button_shadow;
    StarsU32 color_button_shadow;
    const StarsDrawScorePlayerState *players;
    StarsDrawVcPlayerName player_name;
    StarsDrawVcGetString get_string;
    StarsDrawVcCheck get_check;
    StarsDrawScoreFormatLong format_long;
    StarsDrawVcSelectObject select_object;
    StarsDrawVcSetColor set_text_color;
    StarsDrawVcSetMode set_background_mode;
    StarsDrawVcPatBlt pat_blt;
    StarsDrawVcTextOut text_out;
    StarsDrawVcTextExtent text_extent;
    StarsDrawScoreRightText right_text;
    void *context;
} StarsDrawScoreReportInput;
void stars_draw_score_report(StarsU16, const StarsDrawScoreReportInput *);
typedef struct StarsDrawHistoryPoint {
    StarsU16 turn;
    StarsI32 value;
} StarsDrawHistoryPoint;
typedef struct StarsDrawHistorySeries {
    const StarsDrawHistoryPoint *points;
    StarsU16 point_count;
    StarsU32 color;
} StarsDrawHistorySeries;
typedef const StarsU8 *(*StarsDrawHistoryPlayerName)(void *, StarsI16);
typedef StarsI32 (*StarsDrawHistoryValue)(void *, StarsI16, StarsI16);
typedef StarsI16 (*StarsDrawHistoryFormat)(void *, StarsI32, StarsU8 *, StarsU16);
typedef StarsI16 (*StarsDrawHistoryGetString)(void *, StarsI16, StarsU8 *, StarsU16);
typedef void (*StarsDrawHistoryExpandRect)(void *, StarsRect *, StarsI16, StarsI16);
typedef void (*StarsDrawHistorySetRect)(void *, StarsRect *, StarsI16, StarsI16,
    StarsI16, StarsI16);
typedef void (*StarsDrawHistoryDiamond)(void *, StarsU16, const StarsRect *, StarsU16);
typedef void (*StarsDrawHistoryRectCenterText)(void *, StarsU16,
    const StarsRect *, const StarsU8 *, StarsI16);
typedef void (*StarsDrawHistoryCenterText)(void *, StarsU16, StarsI16, StarsI16,
    const StarsU8 *, StarsU16);
typedef void (*StarsDrawHistoryRightText)(void *, StarsU16, StarsI16, StarsI16,
    const StarsU8 *, StarsU16, StarsI16);
typedef StarsU16 (*StarsDrawHistoryCreatePen)(void *, StarsU16, StarsI16, StarsU32);
typedef void (*StarsDrawHistoryDeleteObject)(void *, StarsU16);
typedef void (*StarsDrawHistoryMoveTo)(void *, StarsU16, StarsI16, StarsI16);
typedef void (*StarsDrawHistoryLineTo)(void *, StarsU16, StarsI16, StarsI16);
typedef void (*StarsDrawHistorySetPixel)(void *, StarsU16, StarsI16, StarsI16,
    StarsU32);
typedef struct StarsDrawHistoryReportInput {
    StarsU16 score_dialog;
    StarsI16 player_count;
    StarsI16 game_turn;
    StarsI16 current_player;
    StarsI16 report_type;
    StarsI16 arial8_height;
    StarsI16 arial10_height;
    StarsU16 font_arial8;
    StarsU16 font_arial10;
    StarsU16 brush_blue;
    StarsU16 brush_button_hilite;
    StarsU16 brush_button_shadow;
    StarsU32 color_button_text;
    const StarsDrawHistorySeries *series;
    StarsDrawHistoryPlayerName player_name;
    StarsDrawHistoryValue get_value;
    StarsDrawHistoryGetString get_string;
    StarsDrawHistoryFormat format_short;
    StarsDrawHistoryFormat format_long;
    StarsGetClientRect get_client_rect;
    StarsDrawHistoryExpandRect expand_rect;
    StarsDrawHistorySetRect set_rect;
    StarsDrawHistoryDiamond draw_diamond;
    StarsDrawHistoryRectCenterText center_rect_text;
    StarsDrawVcSelectObject select_object;
    StarsDrawVcSetColor set_text_color;
    StarsDrawVcSetMode set_background_mode;
    StarsDrawVcPatBlt pat_blt;
    StarsDrawVcTextOut text_out;
    StarsDrawHistoryCenterText center_text;
    StarsDrawHistoryRightText right_text;
    StarsDrawHistoryCreatePen create_pen;
    StarsDrawHistoryDeleteObject delete_object;
    StarsDrawHistoryMoveTo move_to;
    StarsDrawHistoryLineTo line_to;
    StarsDrawHistorySetPixel set_pixel;
    void *context;
} StarsDrawHistoryReportInput;
void stars_draw_history_report(StarsU16, const StarsDrawHistoryReportInput *);
typedef struct StarsReportPopupState {
    StarsI16 report;
    StarsI16 field_count;
    StarsI16 sort_column;
    StarsI16 subsort;
    StarsI16 ascending;
    StarsU32 visible_mask;
} StarsReportPopupState;
typedef StarsI16 (*StarsReportPopupHeader)(void *, StarsI16, StarsI16,
    StarsU16, StarsU8 *, StarsU16);
typedef StarsI16 (*StarsReportPopupString)(void *, StarsI16, StarsU8 *, StarsU16);
typedef StarsI16 (*StarsReportPopupSortLabel)(void *, StarsI16, StarsI16,
    StarsU8 *, StarsU16);
typedef const StarsU8 *(*StarsReportPopupCompressed)(void *, StarsI16);
typedef StarsI16 (*StarsReportPopupMenu)(void *, StarsU16, StarsI16, StarsI16,
    StarsU16, const StarsU8 *const *, StarsI16);
typedef void (*StarsReportPopupSortCache)(void *, StarsI16, StarsI16);
typedef void (*StarsReportPopupScroll)(void *);
typedef void (*StarsReportPopupInvalidate)(void *, StarsU16);
typedef struct StarsReportColumnPopupInput {
    StarsU16 report_window;
    StarsReportPopupState *state;
    StarsI16 column;
    StarsI16 right_button;
    StarsU16 hdc;
    StarsReportPopupHeader get_header;
    StarsReportPopupString get_string;
    StarsReportPopupSortLabel get_sort_label;
    StarsReportPopupCompressed get_compressed_string;
    StarsReportPopupMenu popup_menu;
    StarsReportPopupSortCache sort_cache;
    StarsReportPopupScroll set_scroll_bar;
    StarsReportPopupInvalidate invalidate;
    StarsInitScoreGetDC get_dc;
    StarsInitScoreReleaseDC release_dc;
    void *context;
} StarsReportColumnPopupInput;
void stars_report_column_popup(StarsI16, StarsI16,
    const StarsReportColumnPopupInput *);
typedef struct StarsRemoteTerraformPlanet {
    StarsI16 player;
} StarsRemoteTerraformPlanet;
typedef StarsI16 (*StarsRemoteTerraformBest)(void *,
    const StarsRemoteTerraformPlanet *, StarsI16);
typedef struct StarsBestRemoteTerraformInput {
    StarsRemoteTerraformPlanet *planet;
    StarsU8 *players;
    StarsI16 player_count;
    StarsRemoteTerraformBest best_terraform;
    void *context;
} StarsBestRemoteTerraformInput;
StarsI16 stars_best_remote_terraform(StarsBestRemoteTerraformInput *,
    StarsI16, StarsI16);
typedef struct StarsFindNamePlanet { StarsI16 id; StarsPoint point;
    const StarsU8 *name; } StarsFindNamePlanet;
typedef struct StarsFindNameFleet { StarsI16 id; StarsI16 player; StarsPoint point;
    StarsI16 index; StarsU16 valid; const StarsU8 *name; } StarsFindNameFleet;
typedef StarsI16 (*StarsFindNameLookupFleet)(void *, StarsU16,
    StarsFindNameFleet *);
typedef StarsI16 (*StarsFindNameNearest)(void *, StarsPoint, StarsI16,
    StarsScanSelection *);
typedef void (*StarsFindNameChange)(void *, const StarsScanSelection *,
    StarsI16);
typedef void (*StarsFindNameEnsure)(void *, StarsPoint, StarsI16);
typedef void (*StarsFindNameUpdate)(void *, StarsU16);
typedef void (*StarsFindNameSend)(void *, StarsU16, StarsU16, StarsU16,
    StarsU32);
typedef struct StarsFSelectSzInput {
    const StarsU8 *name;
    const StarsFindNamePlanet *planets;
    StarsI16 planet_count;
    const StarsFindNameFleet *fleets;
    StarsI16 fleet_count;
    StarsI16 current_player;
    const StarsU8 *prefix;
    StarsI16 prefix_length;
    StarsU16 scanner_window;
    StarsFindNameLookupFleet lookup_fleet;
    StarsFindNameNearest find_nearest;
    StarsFindNameChange change_selection;
    StarsFindNameEnsure ensure_point;
    StarsFindNameUpdate update_window;
    StarsFindNameSend send_message;
    void *context;
} StarsFSelectSzInput;
StarsI16 stars_f_select_sz(const StarsFSelectSzInput *);
typedef void (*StarsFindDialogSticky)(void *, StarsU16, StarsPoint *,
    StarsI16);
typedef void (*StarsFindDialogItemMessage)(void *, StarsU16, StarsU16,
    StarsU16, StarsU16, StarsU32);
typedef void (*StarsFindDialogGetText)(void *, StarsU16, StarsU16, StarsU8 *,
    StarsU16);
typedef StarsU16 (*StarsFindDialogGetItem)(void *, StarsU16, StarsU16);
typedef StarsI16 (*StarsFindDialogSelect)(void *, const StarsU8 *);
typedef const StarsU8 *(*StarsFindDialogFormatIds)(void *, StarsU16);
typedef void (*StarsFindDialogAlert)(void *, const StarsU8 *, StarsI16);
typedef void (*StarsFindDialogSetFocus)(void *, StarsU16);
typedef struct StarsFindDlgOps {
    StarsGetClientRect get_client_rect;
    StarsFillRect fill_rect;
    StarsSetBackgroundColor set_background_color;
    StarsFindDialogSticky sticky_position;
    StarsFindDialogItemMessage send_item_message;
    StarsFindDialogGetText get_item_text;
    StarsFindDialogGetItem get_item;
    StarsFindDialogSelect select_name;
    StarsFindDialogFormatIds format_ids;
    StarsFindDialogAlert alert;
    StarsFindDialogSetFocus set_focus;
    StarsEndDialog end_dialog;
    StarsWinHelp win_help;
} StarsFindDlgOps;
typedef struct StarsFindDlgInput {
    StarsU16 button_face_brush;
    StarsU32 button_face_color;
    StarsPoint *sticky_point;
    const StarsU8 *help_file;
    const StarsFindDlgOps *ops;
    void *context;
} StarsFindDlgInput;
StarsI16 stars_find_dlg(StarsU16, StarsU16, StarsU16, StarsU32,
    const StarsFindDlgInput *);
typedef void (*StarsNewPlanSetWindowPos)(void *, StarsU16, StarsU16,
    StarsI16, StarsI16, StarsI16, StarsI16, StarsU16);
typedef void (*StarsNewPlanSetItemText)(void *, StarsU16, StarsU16,
    const StarsU8 *);
typedef void (*StarsNewPlanGetItemText)(void *, StarsU16, StarsU16,
    StarsU8 *, StarsU16);
typedef void (*StarsNewPlanSetBackgroundColor)(void *, StarsU16, StarsU32);
typedef struct StarsNewPlanNameDlgOps {
    StarsGetClientRect get_client_rect;
    StarsFillRect fill_rect;
    StarsNewPlanSetBackgroundColor set_background_color;
    StarsNewPlanSetWindowPos set_window_pos;
    StarsFindDialogItemMessage send_item_message;
    StarsNewPlanSetItemText set_item_text;
    StarsNewPlanGetItemText get_item_text;
    StarsEndDialog end_dialog;
    StarsWinHelp win_help;
} StarsNewPlanNameDlgOps;
typedef struct StarsNewPlanNameDlgInput {
    StarsU16 button_face_brush;
    StarsU32 button_face_color;
    StarsPoint *sticky_point;
    StarsU8 *plan_name;
    StarsI16 *dirty_plan;
    const StarsU8 *help_file;
    const StarsNewPlanNameDlgOps *ops;
    void *context;
} StarsNewPlanNameDlgInput;
StarsI16 stars_new_plan_name_dlg(StarsU16, StarsU16, StarsU16, StarsU32,
    const StarsNewPlanNameDlgInput *);

typedef StarsI16 (*StarsCloakPartPoints)(void *, StarsU16, StarsU16);
typedef struct StarsCptsCloakInput {
    StarsU16 group;
    StarsU16 packed_count_type;
    StarsCloakPartPoints part_points;
    void *context;
} StarsCptsCloakInput;
StarsI16 stars_cpts_cloak_from_lphs(const StarsCptsCloakInput *);

typedef struct StarsFleetCloakComponent {
    StarsU16 group;
    StarsU16 packed_count_type;
} StarsFleetCloakComponent;
typedef struct StarsFleetCloakDesign {
    StarsU32 hull_weight;
    const StarsFleetCloakComponent *components;
    StarsU16 component_count;
} StarsFleetCloakDesign;
typedef StarsI16 (*StarsFleetCloakPartPoints)(void *, StarsU16, StarsU16);
typedef StarsI16 (*StarsFleetCloakRaceStat)(void *, StarsI16, StarsI16);
typedef struct StarsPctCloakFromLpflInput {
    StarsI16 player;
    const StarsI16 *ship_counts;
    const StarsFleetCloakDesign *designs;
    const StarsU32 *mineral_weights;
    StarsFleetCloakPartPoints part_points;
    StarsFleetCloakRaceStat race_stat;
    void *context;
} StarsPctCloakFromLpflInput;
StarsI16 stars_pct_cloak_from_lpfl(const StarsPctCloakFromLpflInput *);

typedef struct StarsIshdefProductionPlanet {
    StarsI16 player;
    StarsU16 flags;
    StarsU16 starbase_design;
    StarsU32 *production_records;
    StarsU16 *production_count;
} StarsIshdefProductionPlanet;
typedef StarsI16 (*StarsIshdefSpaceDock)(void *, StarsI16, StarsU16);
typedef void (*StarsIshdefFreeProduction)(void *,
    StarsIshdefProductionPlanet *);
typedef void (*StarsIshdefLookupPlanet)(void *, StarsU16,
    StarsIshdefProductionPlanet *);
typedef void (*StarsIshdefFillProduction)(void *,
    StarsIshdefProductionPlanet *);
typedef struct StarsRemoveIshdefQueuesInput {
    StarsIshdefProductionPlanet *planets;
    StarsU16 planet_count;
    StarsI16 player;
    StarsI16 space_docks_only;
    StarsIshdefSpaceDock is_space_dock;
    StarsIshdefFreeProduction free_production;
    StarsIshdefLookupPlanet lookup_selected;
    StarsIshdefFillProduction fill_selected;
    StarsIshdefProductionPlanet *selected_planet;
    StarsU16 selected_id;
    StarsI16 selected_is_planet;
    void *context;
} StarsRemoveIshdefQueuesInput;
void stars_remove_ishdef_from_all_queues(StarsRemoveIshdefQueuesInput *,
    StarsI16);
typedef struct StarsDestroyIshdefFleet {
    StarsU16 id;
    StarsI16 player;
    StarsU16 flags;
    StarsI16 ship_counts[16];
} StarsDestroyIshdefFleet;
typedef void (*StarsDestroyIshdefTransfer)(void *,
    StarsDestroyIshdefFleet *, StarsDestroyIshdefFleet *);
typedef void (*StarsDestroyIshdefDelete)(void *, StarsU16, StarsI16,
    StarsI16);
typedef void (*StarsDestroyIshdefRefresh)(void *, StarsU16);
typedef void (*StarsDestroyIshdefInvalidate)(void *, StarsU16);
typedef void (*StarsDestroyIshdefStarbase)(void *, StarsI16, StarsI16);
typedef void (*StarsDestroyIshdefReport)(void *, StarsI16, StarsI16);
typedef void (*StarsDestroyIshdefQueues)(void *, StarsI16, StarsI16);
typedef struct StarsDestroyIshdefInput {
    StarsDestroyIshdefFleet *fleets;
    StarsU16 fleet_count;
    StarsI16 player;
    StarsDestroyIshdefTransfer transfer_balance;
    StarsDestroyIshdefDelete delete_fleet;
    StarsDestroyIshdefRefresh refresh_selection;
    StarsDestroyIshdefInvalidate invalidate_window;
    StarsDestroyIshdefStarbase destroy_starbase_design;
    StarsDestroyIshdefReport invalidate_report;
    StarsDestroyIshdefQueues remove_from_queues;
    StarsU16 selected_fleet_id;
    StarsI16 selected_is_fleet;
    void *context;
} StarsDestroyIshdefInput;
void stars_destroy_all_ishdef(StarsDestroyIshdefInput *, StarsI16);

typedef struct StarsLayMinesComponent {
    StarsU16 kind;
    StarsU16 packed_count_type;
} StarsLayMinesComponent;
typedef struct StarsLayMinesDesign {
    StarsU16 hull_kind;
    const StarsLayMinesComponent *components;
    StarsU16 component_count;
} StarsLayMinesDesign;
typedef StarsI32 (*StarsLayMinesPartPower)(void *, StarsU16, StarsU16);
typedef struct StarsCLayMinesInput {
    StarsU8 component_counts[16];
    const StarsLayMinesDesign *designs;
    StarsU16 design_count;
    StarsLayMinesPartPower part_power;
    void *context;
} StarsCLayMinesInput;
StarsI32 stars_c_lay_mines_from_lpfl(const StarsCLayMinesInput *, StarsI16,
    StarsI16);

typedef struct StarsMarkTechPart {
    StarsU16 group;
    StarsU16 packed_count_type;
} StarsMarkTechPart;
typedef struct StarsMarkTechHull {
    StarsU16 hull_id;
    const StarsMarkTechPart *parts;
    StarsU16 part_count;
} StarsMarkTechHull;
typedef StarsI16 (*StarsMarkTechLookup)(void *, StarsU16, StarsU16,
    StarsU8 *);
typedef struct StarsMarkTechsInput {
    StarsU8 tech_battle[6];
    StarsU8 tech_trader[12];
    const StarsMarkTechHull *hull;
    StarsMarkTechLookup lookup;
    void *context;
} StarsMarkTechsInput;
void stars_mark_techs_seen(StarsMarkTechsInput *);
/* Decode one original 147-byte SHDEF and run the recovered technology
 * marking service over its actual hull and occupied part slots. */
StarsI16 stars_mark_techs_seen_shdef(const StarsU8 *, StarsU16,
    StarsMarkTechsInput *);

typedef StarsI32 (*StarsDevelopingLevelCost)(void *, StarsI16, StarsI16,
    StarsI16);
typedef struct StarsCostDevelopingInput {
    signed char current[6];
    const StarsU8 *target;
    const StarsU32 *base_costs;
    StarsU16 game_flags;
    StarsI16 player;
    StarsDevelopingLevelCost level_cost;
    void *context;
} StarsCostDevelopingInput;
StarsI32 stars_cost_of_developing_item(StarsCostDevelopingInput *);

typedef void (*StarsProductionNameGetString)(void *, StarsU16, StarsU8 *);
typedef struct StarsProductionNameInput {
    StarsU32 packed;
    const StarsU8 *const *base_ship_names;
    const StarsU8 *const *player_ship_names;
    const StarsU16 *base_ship_flags;
    const StarsU16 *player_ship_flags;
    const StarsI16 *base_ship_hulls;
    const StarsI16 *player_ship_hulls;
    StarsU16 selected_planet_flags;
    StarsU16 selected_starbase_hull;
    const StarsU8 *const *planetary_names;
    const StarsU8 *starbase_suffix;
    const StarsU8 *ship_suffix;
    StarsProductionNameGetString get_string;
    StarsU8 *output;
    void *context;
} StarsProductionNameInput;
StarsU8 *stars_name_production_item(const StarsProductionNameInput *);
typedef void (*StarsFillProdName)(void *, StarsU32, StarsU8 *);
typedef void (*StarsFillProdAppend)(void *, StarsU8 *);
typedef void (*StarsFillProdSend)(void *, StarsU16, StarsU16,
    const StarsU8 *);
typedef struct StarsFillProdSourceInput {
    StarsU16 hwnd;
    const StarsProductionItem *items;
    StarsU16 item_count;
    StarsFillProdName name;
    StarsFillProdAppend append_item_suffix;
    StarsFillProdSend send_message;
    void *context;
} StarsFillProdSourceInput;
void stars_fill_production_source_list(const StarsFillProdSourceInput *);
typedef struct StarsTerraformPlanet {
    StarsI16 player;
    signed char env[3];
} StarsTerraformPlanet;
typedef StarsI16 (*StarsTerraformCan)(void *, const StarsTerraformPlanet *,
    StarsI16, StarsI16 *, StarsI16 *, StarsI16 *);
typedef StarsI16 (*StarsTerraformDesirability)(void *,
    const StarsTerraformPlanet *, StarsI16);
typedef struct StarsBestTerraformInput {
    StarsTerraformPlanet *planet;
    StarsI16 *current_player;
    StarsTerraformCan can_terraform;
    StarsTerraformDesirability desirability;
    void *context;
} StarsBestTerraformInput;
StarsI16 stars_best_terraform(const StarsBestTerraformInput *, StarsI16);
typedef struct StarsCanTerraformPlanet {
    StarsI16 player;
    signed char environment[3];
    signed char original_environment[3];
} StarsCanTerraformPlanet;
typedef StarsI16 (*StarsCanTerraformPart)(void *, StarsU16, StarsU16,
    StarsI16 *, StarsI16 *);
typedef struct StarsCanTerraformInput {
    StarsCanTerraformPlanet *planet;
    StarsI16 *current_player;
    signed char race_center[3];
    signed char race_environment_enabled[3];
    StarsCanTerraformPart lookup_part;
    void *context;
} StarsCanTerraformInput;
StarsI16 stars_can_terraform_lppl(const StarsCanTerraformInput *,
    StarsI16 *, StarsI16 *, StarsI16 *, StarsI16);
typedef struct StarsStargateArmor {
    StarsI16 fuel_max;
    StarsI16 defense;
} StarsStargateArmor;
typedef StarsI16 (*StarsStargateArmorLookup)(void *, StarsI16,
    StarsStargateArmor *);
typedef struct StarsStargateDamageInput {
    StarsStargateArmorLookup lookup;
    void *context;
} StarsStargateDamageInput;
StarsI16 stars_calc_stargate_damage(const StarsStargateDamageInput *,
    StarsI16, StarsI16, StarsI16, StarsI16, StarsI16 *);

/* Source-level model of FStargateJump (NE 17:0CFE-152D).  The original
 * routine works on a packed _fleet and delegates the game-specific lookups
 * to globals.  These records expose the same cargo slots and state changes
 * without reproducing that packed representation. */
#define STARS_STARGATE_CARGO_SLOTS 16
enum {
    STARS_STARGATE_MSG_DAMAGE_ABORT = 0xe4,
    STARS_STARGATE_MSG_DAMAGE_FAIL = 0xe3,
    STARS_STARGATE_MSG_EMPTY = 0xe7,
    STARS_STARGATE_MSG_LIGHT_LOSS = 0xe8,
    STARS_STARGATE_MSG_MEDIUM_LOSS = 0xe9,
    STARS_STARGATE_MSG_HEAVY_LOSS = 0xea,
    STARS_STARGATE_MSG_LARGE_LOSS = 0xeb
};
typedef struct StarsFStargateFleet {
    StarsI16 player;
    StarsU16 id;
    StarsI16 planet;
    StarsU16 flags;
    StarsU32 cargo[STARS_STARGATE_CARGO_SLOTS];
    StarsU32 cargo_state[STARS_STARGATE_CARGO_SLOTS];
    /* The packed _fleet keeps the five resource dwords at offset 76.  The
     * stargate routine does not inspect them directly, but its real
     * FleetTransferCargoBalance handoff mutates them when ship capacity is
     * lost. */
    StarsI32 resources[STARS_FLEET_BALANCE_RESOURCES];
} StarsFStargateFleet;
typedef struct StarsFStargateMessage {
    StarsI16 player;
    StarsI16 code;
    StarsU16 fleet_reference;
    StarsI16 fleet_id;
    StarsI16 planet;
    StarsI16 destination;
    StarsI16 slot;
    StarsU32 amount;
} StarsFStargateMessage;
typedef StarsI16 (*StarsFStargateDamage)(void *, StarsI16, StarsI16,
    StarsI16, StarsI16, StarsI16 *);
typedef StarsI16 (*StarsFStargateRandom)(void *, StarsI16);
typedef void (*StarsFStargateMessageSend)(void *, const StarsFStargateMessage *);
typedef void (*StarsFStargateTransfer)(void *, StarsFStargateFleet *,
    StarsFStargateFleet *);
typedef struct StarsFStargateJumpInput {
    StarsFStargateFleet *fleet;
    StarsI16 destination;
    StarsI16 design_mass[STARS_STARGATE_CARGO_SLOTS];
    StarsI32 ship_weight[STARS_STARGATE_CARGO_SLOTS];
    StarsI16 race_stat_stargate_loss;
    StarsFStargateDamage stargate_damage;
    StarsFStargateRandom random_value;
    StarsFStargateMessageSend send_message;
    StarsFStargateTransfer transfer_cargo_balance;
    void *context;
} StarsFStargateJumpInput;
StarsI16 stars_f_stargate_jump(StarsFStargateJumpInput *, StarsI16, StarsI16,
    StarsI16);
typedef struct StarsTileSizeEntry {
    StarsI16 kind;
    StarsI16 size;
} StarsTileSizeEntry;
typedef void (*StarsTileSizeReflow)(void *, StarsI16, StarsI16, StarsI16);
typedef struct StarsEnsureTileSizeInput {
    StarsI16 small_tiles;
    StarsI16 tile_flag;
    StarsI16 arial8_height;
    StarsI16 *selected_object;
    StarsTileSizeEntry columns[6];
    StarsTileSizeEntry rows[7];
    StarsTileSizeReflow reflow;
    void *context;
} StarsEnsureTileSizeInput;
void stars_ensure_tile_size(StarsEnsureTileSizeInput *);
typedef struct StarsReflowTile {
    StarsI16 top;
    StarsI16 height;
    StarsU16 object_flags;
    StarsU16 column_flags;
} StarsReflowTile;
typedef void (*StarsReflowFill)(void *, StarsU16, const StarsRect *, StarsU16);
typedef void (*StarsReflowDraw)(void *, StarsU16, StarsU16);
typedef void (*StarsReflowRelease)(void *, StarsU16, StarsU16);
typedef struct StarsReflowColumnInput {
    StarsI16 selected_object;
    StarsI16 arial8_height;
    StarsU16 hwnd_planet;
    StarsU16 hdc;
    StarsU16 button_brush;
    StarsReflowTile columns[6];
    StarsReflowTile rows[7];
    StarsReflowFill fill_rect;
    StarsReflowDraw draw_plan_ship;
    StarsReflowRelease release_dc;
    void *context;
} StarsReflowColumnInput;
void stars_reflow_column(StarsReflowColumnInput *, StarsI16, StarsI16,
    StarsI16);
typedef StarsI16 (*StarsTruePartRaceStat)(void *, StarsI16, StarsI16);
typedef StarsI16 (*StarsTruePartRaceGrbit)(void *, StarsI16, StarsI16);
typedef struct StarsTruePart {
    StarsU16 group;
    StarsU16 subtype;
    StarsU16 base_costs[4];
    signed char technology[6];
} StarsTruePart;
typedef struct StarsTruePartCostInput {
    const signed char *player_technology;
    StarsU16 *game_flags;
    StarsTruePartRaceStat race_stat;
    StarsTruePartRaceGrbit race_grbit;
    void *context;
} StarsTruePartCostInput;
void stars_get_true_part_cost(const StarsTruePartCostInput *, StarsI16,
    const StarsTruePart *, StarsU16[4]);
/* Reprice a packed 147-byte SHDEF from the original part segment.  A built
 * design resolves its physical parts without player visibility gates; the
 * owner's technology and race still determine each part's true cost. */
typedef struct StarsShdefRepriceInput {
    const StarsU8 *part_segment;
    StarsU16 part_segment_size;
    const signed char *player_technology;
    StarsU16 *game_flags;
    StarsTruePartRaceStat race_stat;
    StarsTruePartRaceGrbit race_grbit;
    void *context;
    StarsI16 player;
} StarsShdefRepriceInput;
StarsI16 stars_reprice_shdef_cost(const StarsShdefRepriceInput *,
    const StarsU8 *, StarsU16, StarsU8 *);
StarsI16 stars_initialize_shdef_cost(const StarsShdefRepriceInput *,
    const StarsU8 *, StarsU16, StarsU8 *);

/* Semantic inputs for GetProductionCosts (NE 27:3F20-4F40).  The original
 * routine reads packed production words and far tables.  These records keep
 * the same four cost lanes and component/hull identity visible while the
 * table lookups and technology adjustments remain explicit callbacks. */
#define STARS_PRODUCTION_HULL_COMPONENTS 8
typedef struct StarsProductionHullComponent {
    StarsU16 present;
    StarsU16 group;
    StarsU16 subtype;
    StarsU16 technology_level;
} StarsProductionHullComponent;
typedef struct StarsProductionHull {
    StarsU16 hull_id;
    StarsU16 hidden_flags;
    StarsU16 base_costs[4];
    StarsU16 component_count;
    StarsProductionHullComponent components[STARS_PRODUCTION_HULL_COMPONENTS];
} StarsProductionHull;
typedef StarsI16 (*StarsProductionRaceStat)(void *, StarsI16, StarsI16);
typedef StarsI16 (*StarsProductionRaceGrbit)(void *, StarsI16, StarsI16);
typedef StarsI16 (*StarsProductionLookupPart)(void *, StarsU16, StarsU16,
    StarsTruePart *);
typedef void (*StarsProductionPartCost)(void *, StarsI16,
    const StarsTruePart *, StarsU16[4]);
typedef StarsI16 (*StarsProductionLookupHull)(void *, StarsI16, StarsU16,
    StarsI16, StarsProductionHull *);
typedef struct StarsProductionCostOps {
    StarsProductionRaceStat race_stat;
    StarsProductionRaceGrbit race_grbit;
    StarsProductionLookupPart lookup_part;
    StarsProductionPartCost part_cost;
    StarsProductionLookupHull lookup_hull;
} StarsProductionCostOps;
typedef struct StarsProductionCostInput {
    StarsI16 planet_player;
    StarsU16 planet_flags;
    StarsU16 game_flags;
    const StarsProductionHull *planet_hull;
    StarsI16 player;
    StarsU16 item_kind;
    StarsU16 item_variant;
    StarsU16 item_index;
    StarsU16 quantity;
    StarsI16 only_one;
    const StarsProductionCostOps *ops;
    void *context;
} StarsProductionCostInput;
void stars_get_production_costs(const StarsProductionCostInput *, StarsU32[4]);
typedef struct StarsFleetTargetOrder {
    StarsU16 target;
    StarsU16 flags;
} StarsFleetTargetOrder;
typedef struct StarsFleetTargetRecord {
    StarsU16 token;
    StarsI16 x;
    StarsI16 y;
    StarsFleetTargetOrder *orders;
    StarsU16 order_count;
} StarsFleetTargetRecord;
typedef struct StarsNearestTarget {
    StarsU16 grobj_full;
    StarsU16 planet_id;
    StarsU16 fleet_index;
} StarsNearestTarget;
typedef StarsI16 (*StarsFindNearestTarget)(void *, StarsI16, StarsI16,
    StarsU16, StarsNearestTarget *);
typedef struct StarsFleetOrdersTargetInput {
    StarsFleetTargetRecord *fleets;
    StarsU16 fleet_count;
    StarsFindNearestTarget find_nearest;
    void *context;
} StarsFleetOrdersTargetInput;
void stars_fleet_orders_change_target(StarsFleetOrdersTargetInput *,
    StarsFleetTargetRecord *);
typedef StarsI16 (*StarsQueuedShipCount)(void *, StarsU16, StarsI16,
    StarsI16 *);
typedef void (*StarsQueuedShipFormat)(void *, StarsU16, StarsU32, StarsI16,
    StarsI16, StarsU16, StarsI16, StarsU8 *);
typedef StarsI16 (*StarsQueuedShipMessage)(void *, StarsU16, const StarsU8 *,
    StarsU16, StarsU16);
typedef StarsU16 (*StarsQueuedShipGetFocus)(void *);
typedef void (*StarsQueuedShipFocus)(void *, StarsU16);
typedef void (*StarsQueuedShipRemove)(void *, StarsU16, StarsI16);
typedef void (*StarsQueuedShipDestroy)(void *, StarsU16, StarsI16);
typedef void (*StarsQueuedShipInvalidate)(void *, StarsU16);
typedef struct StarsCheckQueuedShipInput {
    StarsU16 hwnd;
    StarsU16 scanner_window;
    StarsU16 message_window;
    StarsU16 design_id;
    StarsU32 existing_count;
    StarsU16 hull_class;
    StarsI16 player;
    StarsI16 edit_mode;
    StarsI16 starbase_mode;
    StarsQueuedShipCount queued_count;
    StarsQueuedShipFormat format_warning;
    StarsQueuedShipMessage message_box;
    StarsQueuedShipGetFocus get_focus;
    StarsQueuedShipFocus set_focus;
    StarsQueuedShipRemove remove_from_queues;
    StarsQueuedShipDestroy destroy_design;
    StarsQueuedShipInvalidate invalidate;
    void *context;
} StarsCheckQueuedShipInput;
StarsI16 stars_check_queued_ship(const StarsCheckQueuedShipInput *);

/* The turn routine keeps research as unsigned 16.16 fixed-point values.  The
 * packed current_technology byte stores the selected technology in its low
 * nibble and the selection mode in its high nibble.  These fields are named
 * here so callers do not have to recreate the original player-record offsets.
 */
typedef struct StarsResearchPlayer {
    StarsI16 id;
    StarsU16 dead_flags;
    StarsU8 current_technology;
    StarsU8 current_mode;
    StarsU8 technology_level[6];
    StarsU32 research[6];
    StarsU32 research_last_year;
} StarsResearchPlayer;
typedef StarsI16 (*StarsResearchRaceValue)(void *, StarsI16, StarsI16);
typedef StarsU32 (*StarsResearchLevelCost)(void *, StarsI16, StarsI16,
    StarsI16);
/* Return 1 for a real component, -1 for a hidden/unavailable component, and
 * 0 when the component list for a group is exhausted.  On return 1, fill the
 * component's six technology requirements. */
typedef StarsI16 (*StarsResearchLookupPart)(void *, StarsU16, StarsU16,
    StarsU8[6]);
typedef void (*StarsResearchMessage)(void *, StarsI16, StarsI16, StarsI16,
    StarsI16, StarsI16, StarsI16);
typedef void (*StarsResearchPoolMessage)(void *, StarsI16, StarsI16,
    StarsI16, StarsI16);
typedef struct StarsResearchOps {
    StarsResearchRaceValue race_stat;
    StarsResearchRaceValue race_grbit;
    StarsResearchLevelCost level_cost;
    StarsResearchLookupPart lookup_part;
    StarsResearchMessage send_message;
    StarsResearchPoolMessage send_pool_message;
} StarsResearchOps;
typedef struct StarsResearchInput {
    StarsResearchPlayer *players;
    StarsU16 player_count;
    StarsI16 use_pool;
    StarsU16 game_flags;
    const StarsResearchOps *ops;
    void *context;
} StarsResearchInput;
void stars_update_research_status(StarsResearchInput *);
typedef StarsU16 (*StarsDrawResearchGetDc)(void *, StarsU16);
typedef void (*StarsDrawResearchReleaseDc)(void *, StarsU16, StarsU16);
typedef void (*StarsDrawResearchFont)(void *, StarsU16, StarsU16);
typedef StarsU32 (*StarsDrawResearchColor)(void *, StarsU16, StarsU32);
typedef StarsU16 (*StarsDrawResearchString)(void *, StarsU16, StarsU8 *,
    StarsU16);
typedef const StarsU8 *(*StarsDrawResearchCompressed)(void *, StarsU16);
typedef StarsI16 (*StarsDrawResearchTextExtent)(void *, StarsU16,
    const StarsU8 *, StarsU16);
typedef void (*StarsDrawResearchText)(void *, StarsU16, StarsI16, StarsI16,
    const StarsU8 *, StarsU16);
typedef void (*StarsDrawResearchRightText)(void *, StarsU16, StarsI16,
    StarsI16, const StarsU8 *, StarsU16, StarsI16);
typedef void (*StarsDrawResearchFrame)(void *, StarsU16, const StarsRect *,
    StarsI16);
typedef void (*StarsDrawResearchFill)(void *, StarsU16, StarsI16, StarsI16,
    StarsI16, StarsI16, StarsU32);
typedef void (*StarsDrawResearchSetRect)(void *, StarsRect *, StarsI16,
    StarsI16, StarsI16, StarsI16);
typedef void (*StarsDrawResearchInvalidate)(void *, StarsU16,
    const StarsRect *, StarsI16);
typedef StarsI16 (*StarsDrawResearchRaceFlag)(void *, StarsI16, StarsI16);
typedef StarsU32 (*StarsDrawResearchLevelCost)(void *, StarsI16, StarsI16,
    StarsI16);
typedef StarsI16 (*StarsDrawResearchPart)(void *, StarsU16, StarsU16,
    StarsU8[6], const StarsU8 **);
typedef StarsU16 (*StarsDrawResearchGetItem)(void *, StarsU16, StarsU16);
typedef void (*StarsDrawResearchPosition)(void *, StarsU16, StarsU16,
    StarsI16, StarsI16, StarsI16, StarsI16, StarsU16);
typedef void (*StarsDrawResearchButton)(void *, StarsU16, const StarsRect *,
    StarsU16);
typedef struct StarsDrawResearchOps {
    StarsDrawResearchGetDc get_dc;
    StarsDrawResearchReleaseDc release_dc;
    StarsDrawResearchFont select_font;
    StarsDrawResearchColor set_text_color;
    StarsDrawResearchColor set_bk_color;
    StarsDrawResearchString get_string;
    StarsDrawResearchCompressed compressed;
    StarsDrawResearchTextExtent text_extent;
    StarsDrawResearchText text_out;
    StarsDrawResearchText center_text;
    StarsDrawResearchRightText right_text;
    StarsDrawResearchFrame draw_frame;
    StarsDrawResearchFill fill_rect;
    StarsDrawResearchSetRect set_rect;
    StarsDrawResearchInvalidate invalidate_rect;
    StarsDrawResearchRaceFlag race_flag;
    StarsDrawResearchLevelCost level_cost;
    StarsDrawResearchPart lookup_part;
    StarsDrawResearchGetItem get_dialog_item;
    StarsDrawResearchPosition set_window_pos;
    StarsDrawResearchButton draw_button;
} StarsDrawResearchOps;
typedef struct StarsDrawResearchInput {
    StarsU16 hwnd;
    StarsU16 hdc;
    StarsRect paint_rect;
    StarsI16 draw_flags;
    StarsI16 player;
    StarsI16 dy_arial8;
    StarsI16 dx_res_left;
    StarsI16 dx_res_right;
    StarsI16 dx_res_string_right;
    StarsI16 current_technology;
    StarsI16 technology_level[6];
    StarsU32 research[6];
    StarsU32 research_last_year;
    StarsU32 resource_budget;
    StarsU32 resource_total;
    StarsI16 global_research_percent;
    StarsU16 game_flags;
    StarsU16 window_brush;
    StarsU32 button_face;
    StarsU32 window_text;
    StarsU16 font;
    StarsU16 dialog_item;
    StarsRect spin_top;
    StarsRect spin_bottom;
    StarsI16 future_tech_count;
    StarsU16 future_tech_ids[8];
    const StarsDrawResearchOps *ops;
    void *context;
} StarsDrawResearchInput;
void stars_draw_research_dialog(StarsDrawResearchInput *);

typedef struct StarsSpeedComponent {
    StarsU16 group;
    StarsU16 type;
    StarsU16 count;
} StarsSpeedComponent;
typedef struct StarsSpeedDesign {
    StarsU16 empty_weight;
    const StarsSpeedComponent *components;
    StarsU16 component_count;
} StarsSpeedDesign;
typedef struct StarsSpeedFleet {
    StarsI16 player;
    StarsU32 cargo_weight;
} StarsSpeedFleet;
typedef struct StarsSpeedToken {
    StarsU16 weight;
    StarsU16 random_speed_bits;
} StarsSpeedToken;
typedef struct StarsSpeedEngine {
    StarsU16 engine_id;
    StarsU16 fuel_used[10];
} StarsSpeedEngine;
typedef StarsI16 (*StarsSpeedRaceStat)(void *, StarsI16, StarsI16);
typedef StarsU32 (*StarsSpeedHullStat)(void *, const StarsSpeedDesign *,
    StarsI16);
typedef StarsU32 (*StarsSpeedFleetStat)(void *, const StarsSpeedFleet *,
    StarsI16);
typedef StarsI16 (*StarsSpeedRandom)(void *, StarsI16);
typedef StarsI16 (*StarsSpeedEngineLookup)(void *, StarsU16,
    StarsSpeedEngine *);
typedef struct StarsSpeedInput {
    const StarsSpeedFleet *fleet;
    const StarsSpeedDesign *design;
    StarsSpeedToken *token;
    StarsI16 dump_cargo;
    StarsSpeedRaceStat race_stat;
    StarsSpeedHullStat hull_stat;
    StarsSpeedFleetStat fleet_stat;
    StarsSpeedRandom random;
    StarsSpeedEngineLookup engine_lookup;
    void *context;
} StarsSpeedInput;
StarsI16 stars_speed_of_ship(StarsSpeedInput *);

/* Semantic view of the battle-token fields consumed by DzMoveRangeToConsider.
 * A battle position is encoded in one byte, while the movement phase and
 * targeting class are packed into the original token; exposing those values
 * directly makes the range policy readable to callers. */
typedef struct StarsMoveRangeToken {
    StarsI16 owner;
    StarsU8 battle_coordinate;
    StarsU8 targeting_class;
    StarsU8 movement_limit;
    StarsU8 movement_phase;
    StarsU8 md_grobj;
    StarsU16 md_descriptor;
    StarsU16 active_flags;
    StarsU16 shield_points;
} StarsMoveRangeToken;
typedef StarsI32 (*StarsMoveRangeDamage)(void *,
    const StarsMoveRangeToken *, const StarsMoveRangeToken *);
typedef struct StarsMoveRangeInput {
    const StarsMoveRangeToken *self;
    const StarsMoveRangeToken *targets;
    StarsU16 target_count;
    StarsU16 attack_players;
    StarsI16 primary_targets_exist;
    StarsU16 cargo_capacity;
    StarsMoveRangeDamage damage_projection;
    void *context;
} StarsMoveRangeInput;
StarsI16 stars_dz_move_range_to_consider(const StarsMoveRangeInput *,
    StarsU8 *candidate_coordinate);

typedef struct StarsScoreGuessToken {
    StarsI16 owner;
    StarsU8 battle_coordinate;
    StarsU8 initiative_phase;
    StarsU8 md_grobj;
    StarsU16 md_descriptor;
    StarsU16 active_flags;
} StarsScoreGuessToken;
typedef StarsI32 (*StarsScoreGuessDamage)(void *,
    const StarsScoreGuessToken *, const StarsScoreGuessToken *, StarsU8,
    StarsI16);
typedef struct StarsScoreGuessInput {
    const StarsScoreGuessToken *source;
    const StarsScoreGuessToken *targets;
    StarsU16 target_count;
    StarsU16 attack_players;
    StarsI16 primary_attack;
    StarsScoreGuessDamage damage;
    void *context;
} StarsScoreGuessInput;
StarsI32 stars_score_guess_battle_damage(const StarsScoreGuessInput *,
    StarsU8);

typedef struct StarsAttackFleetPlanet {
    StarsI16 max_mines;
    StarsI16 operating_mines;
    StarsI16 max_factories;
    StarsI16 operating_factories;
} StarsAttackFleetPlanet;
typedef StarsI16 (*StarsAttackFleetRandom)(void *, StarsI16);
typedef void (*StarsAttackFleetQueue)(void *, StarsI16, StarsI16, StarsI16,
    StarsI16);
typedef struct StarsAttackFleetInput {
    const StarsAttackFleetPlanet *planet;
    StarsI16 best_destroyer;
    StarsI16 best_battle;
    StarsI16 best_starbase_defender;
    const StarsU16 *battle_design_flags;
    StarsU16 battle_design_count;
    StarsAttackFleetRandom random;
    StarsAttackFleetQueue add_to_queue;
    void *context;
} StarsAttackFleetInput;
StarsI16 stars_add_attack_fleet(const StarsAttackFleetInput *);

typedef struct StarsCheaterFleet {
    StarsI16 id;
    StarsI16 owner;
    StarsU16 flags;
    StarsU32 cargo[3];
} StarsCheaterFleet;
typedef struct StarsCheaterPlanet {
    StarsI16 id;
    StarsI16 owner;
    StarsU32 mine_count;
    StarsU32 mineral_stock[3];
} StarsCheaterPlanet;
typedef StarsI16 (*StarsCheaterRandom)(void *, StarsI16);
typedef void (*StarsCheaterMessage)(void *, StarsI16, StarsI16, StarsI16,
    StarsI32, StarsI32, StarsI16);
typedef struct StarsSpankCheatersInput {
    StarsI16 turn;
    StarsI16 player_count;
    const StarsU8 *dead_players;
    StarsCheaterFleet *fleets;
    StarsU16 fleet_count;
    StarsCheaterPlanet *planets;
    StarsU16 planet_count;
    StarsCheaterRandom random;
    StarsCheaterMessage send_message;
    void *context;
} StarsSpankCheatersInput;
void stars_spank_the_cheaters(StarsSpankCheatersInput *);

/* Semantic view of SendBattleMessages.  The original routine turns the
 * battle ledger into player-specific report messages.  These records expose
 * the fields needed by that policy instead of requiring callers to recreate
 * the packed _fleet and _planet offsets. */
typedef struct StarsBattleMessageFleet {
    StarsI16 id;
    StarsI16 player;
    StarsI16 planet_id;
    StarsI16 x;
    StarsI16 y;
    StarsU16 flags;
    StarsU16 ship_counts[16];
    StarsU16 missed_flags;
} StarsBattleMessageFleet;
typedef struct StarsBattleMessagePlanet {
    StarsI16 id;
    StarsI16 owner;
    StarsU16 flags;
    StarsU16 starbase_design;
    StarsU16 mineral_value;
    StarsI32 population;
} StarsBattleMessagePlanet;
typedef void (*StarsBattleMessageSend)(void *, StarsI16, StarsI16,
    StarsI16, StarsI16, StarsI16, StarsI16, StarsI16, StarsI16, StarsI16,
    StarsI16);
typedef StarsBattleMessagePlanet *(*StarsBattleMessageFindPlanet)(void *,
    StarsI16);
typedef StarsI16 (*StarsBattleMessageRaceStat)(void *, StarsI16, StarsI16);
typedef void (*StarsBattleMessageUninhabit)(void *,
    StarsBattleMessagePlanet *);
typedef void (*StarsBattleMessageLearnTech)(void *, StarsI16, StarsI16,
    StarsI16, StarsI16);
typedef struct StarsSendBattleMessagesInput {
    StarsBattleMessageFleet *fleets;
    StarsU16 fleet_count;
    StarsI16 player_count;
    StarsU16 battle_player_mask;
    StarsI16 battle_player_count;
    StarsI16 battle_id;
    StarsU16 *player_losses;
    StarsI16 ships_involved;
    StarsI16 ship_designs_involved;
    StarsU16 spectator_players;
    StarsU16 missed_players;
    StarsU16 starbase_died;
    StarsU16 starbase_damaged;
    StarsBattleMessageFindPlanet find_planet;
    StarsBattleMessageRaceStat race_stat;
    StarsBattleMessageUninhabit uninhabit;
    StarsBattleMessageLearnTech learn_tech;
    StarsBattleMessageSend send_message;
    void *context;
} StarsSendBattleMessagesInput;
void stars_send_battle_messages(StarsSendBattleMessagesInput *);

/* Semantic bombing model.  DoBombing combines same-planet bomb fleets,
 * applies the planet's survival/defense envelope, updates population,
 * infrastructure and environment, and emits the owner-facing report pair. */
typedef struct StarsDoBombingFleet {
    StarsI16 id;
    StarsI16 player;
    StarsI16 planet_id;
    StarsU16 flags;
} StarsDoBombingFleet;
typedef struct StarsDoBombingPlanet {
    StarsI16 id;
    StarsI16 owner;
    StarsU16 flags;
    StarsI32 population;
    StarsI32 mineral_stock;
    StarsI16 buildings;
    StarsI16 defense_count;
    StarsI16 operable_defenses;
    StarsI16 defense_damage;
    signed char environment[3];
    signed char original_environment[3];
} StarsDoBombingPlanet;
typedef struct StarsDoBombingDamage {
    StarsI32 population;
    StarsI32 minimum_population;
    StarsI32 smart_population;
    StarsI32 buildings;
    StarsI32 terraforming;
    StarsI16 multiple_fleets;
} StarsDoBombingDamage;
typedef StarsI16 (*StarsDoBombingAttack)(void *,
    const StarsDoBombingFleet *, StarsI16);
typedef StarsI16 (*StarsDoBombingCalculate)(void *,
    const StarsDoBombingFleet *, StarsDoBombingDamage *);
typedef void (*StarsDoBombingSurvival)(void *, const StarsDoBombingPlanet *,
    StarsU16 *, StarsU16 *);
typedef StarsI16 (*StarsDoBombingRandom)(void *, StarsI16);
typedef void (*StarsDoBombingMessage)(void *, StarsI16, StarsI16, StarsI16,
    StarsI16, StarsI16, StarsI16, StarsI16, StarsI16, StarsI16);
typedef void (*StarsDoBombingUninhabit)(void *, StarsDoBombingPlanet *);
typedef struct StarsDoBombingInput {
    StarsDoBombingFleet *fleets;
    StarsU16 fleet_count;
    StarsDoBombingPlanet *planets;
    StarsU16 planet_count;
    StarsDoBombingAttack attack;
    StarsDoBombingCalculate calculate;
    StarsDoBombingSurvival survival;
    StarsDoBombingRandom random;
    StarsDoBombingMessage send_message;
    StarsDoBombingUninhabit uninhabit;
    void *context;
} StarsDoBombingInput;
void stars_do_bombing(StarsDoBombingInput *);

typedef struct StarsBattleVcrRecord {
    StarsI16 battle_id;
    StarsU16 token_count;
    StarsU16 next_record;
} StarsBattleVcrRecord;
typedef void *(*StarsBattleVcrAllocate)(void *, StarsU16, StarsU16);
typedef void (*StarsBattleVcrFree)(void *, void *, StarsU16);
typedef StarsI16 (*StarsBattleVcrSetBoard)(void *, StarsI16);
typedef void (*StarsBattleVcrTutor)(void *);
typedef void (*StarsBattleVcrDialog)(void *);
typedef void (*StarsBattleVcrAlert)(void *, StarsI16);
typedef struct StarsBattleVcrInput {
    StarsI16 battle_id;
    StarsU16 view_flags;
    StarsI16 round;
    StarsI16 board_width;
    StarsI16 board_square;
    StarsI16 current_step;
    StarsI16 current_vcr_step;
    StarsI16 current_vcr_round;
    const StarsBattleVcrRecord *records;
    StarsU16 record_count;
    void *token_workspace;
    void *damage_workspace;
    StarsBattleVcrAllocate allocate;
    StarsBattleVcrFree free_memory;
    StarsBattleVcrSetBoard set_board;
    StarsBattleVcrTutor advance_tutor;
    StarsBattleVcrDialog show_dialog;
    StarsBattleVcrAlert alert;
    void *context;
} StarsBattleVcrInput;
void stars_battle_vcr(StarsBattleVcrInput *);

typedef struct StarsVcrTokenState {
    StarsI16 ship_count;
    StarsI16 shield;
    StarsU16 battle_square;
    StarsU16 flags;
    StarsU16 target;
} StarsVcrTokenState;
typedef struct StarsVcrStepRecord {
    StarsU16 token_index;
    StarsI16 ship_loss;
    StarsI16 shield_damage;
    StarsU16 destination_square;
    StarsU16 packed_flags;
    StarsU16 round;
} StarsVcrStepRecord;
typedef void (*StarsVcrRegenerateShield)(void *, StarsVcrTokenState *);
typedef void (*StarsVcrEnableButtons)(void *, StarsI16, StarsI16);
typedef struct StarsSetVcrBoardInput {
    StarsVcrTokenState *tokens;
    const StarsVcrTokenState *initial_tokens;
    StarsU16 token_count;
    const StarsVcrStepRecord *steps;
    StarsU16 step_count;
    StarsI16 current_step;
    StarsI16 round;
    StarsI16 focus_token;
    StarsU16 focus_square;
    StarsVcrRegenerateShield regenerate_shield;
    StarsVcrEnableButtons enable_buttons;
    void *context;
} StarsSetVcrBoardInput;
StarsI16 stars_set_vcr_board(StarsI16, StarsSetVcrBoardInput *);

typedef struct StarsVcrStatToken {
    StarsI16 ship_count;
    StarsI16 hull_dp;
    StarsI16 shield;
    StarsU16 displacement;
} StarsVcrStatToken;
typedef struct StarsVcrStatRecord {
    StarsU16 token_index;
    StarsI16 ship_loss;
    StarsU16 packed_damage;
    StarsU16 displacement;
} StarsVcrStatRecord;
typedef struct StarsGetVcrStatsInput {
    const StarsVcrStatToken *tokens;
    const StarsVcrStatRecord *records;
    StarsU16 token_count;
    StarsU16 record_count;
    const StarsI32 *design_values;
} StarsGetVcrStatsInput;
void stars_get_vcr_stats(const StarsGetVcrStatsInput *, StarsI16,
    StarsI32 *, StarsU16 *, StarsI32 *, StarsI16 *);

typedef void (*StarsVcrDlgSimple)(void *);
typedef void (*StarsVcrDlgWindow)(void *, StarsU16);
typedef void (*StarsVcrDlgTimer)(void *, StarsI16, StarsI16);
typedef StarsI16 (*StarsVcrDlgPopup)(void *, StarsI16, StarsI16, StarsU16);
typedef void (*StarsVcrDlgDraw)(void *, StarsI16, StarsI16);
typedef StarsI16 (*StarsVcrDlgStep)(void *, StarsI16);
typedef void (*StarsVcrDlgFocus)(void *, StarsU16);
typedef struct StarsVcrDlgOps {
    StarsVcrDlgSimple paint;
    StarsVcrDlgWindow fill_background;
    StarsVcrDlgSimple set_hand_cursor;
    StarsVcrDlgWindow resize;
    StarsVcrDlgTimer set_timer;
    StarsVcrDlgSimple kill_timer;
    StarsVcrDlgSimple end_dialog;
    StarsVcrDlgSimple help;
    StarsVcrDlgPopup popup;
    StarsVcrDlgDraw draw;
    StarsVcrDlgStep set_step;
    StarsVcrDlgFocus focus_square;
    StarsVcrDlgSimple refresh_speed;
} StarsVcrDlgOps;
typedef struct StarsVcrDlgInput {
    StarsU16 hwnd;
    StarsI16 current_step;
    StarsI16 final_step;
    StarsI16 speed;
    StarsI16 focus_token;
    StarsU16 focus_square;
    StarsI16 board_square;
    StarsI16 animating;
    StarsVcrDlgOps ops;
    void *context;
} StarsVcrDlgInput;
StarsI16 stars_vcr_dlg(StarsU16, StarsU16, StarsU16, StarsU32,
    StarsVcrDlgInput *);

typedef struct StarsVcrMenuToken {
    StarsI16 player;
    StarsI16 design;
    StarsI16 ship_count;
    StarsU16 battle_square;
} StarsVcrMenuToken;
typedef void (*StarsVcrMenuFormat)(void *, const StarsVcrMenuToken *,
    StarsI16, StarsU8 *, StarsU16);
typedef StarsI16 (*StarsVcrMenuPopup)(void *, StarsU16, StarsI16, StarsI16,
    StarsU16, const StarsU8 *, StarsI16);
typedef struct StarsPopupVcrMenuInput {
    const StarsVcrMenuToken *tokens;
    const StarsVcrStepRecord *steps;
    StarsU16 token_count;
    StarsU16 step_count;
    StarsI16 current_step;
    StarsI16 focused_token;
    StarsVcrMenuFormat format;
    StarsVcrMenuPopup popup;
    void *context;
} StarsPopupVcrMenuInput;
StarsI16 stars_popup_vcr_menu(StarsU16, StarsI16, StarsI16, StarsU16,
    StarsPopupVcrMenuInput *);

typedef struct StarsAnimateAttackRecord {
    StarsU16 attacker_token;
    StarsU16 target_token;
    StarsU16 flags;
} StarsAnimateAttackRecord;
typedef struct StarsAnimateAttackToken {
    StarsU16 battle_square;
} StarsAnimateAttackToken;
typedef void (*StarsAnimateAttackLine)(void *, StarsU16, StarsI16, StarsI16,
    StarsI16, StarsI16, StarsI16, StarsI16);
typedef void (*StarsAnimateAttackIcon)(void *, StarsU16, StarsI16, StarsI16,
    StarsI16);
typedef void (*StarsAnimateAttackAnimation)(void *, StarsU16, StarsI16,
    StarsI16, StarsI16, StarsI16, StarsI16, StarsI16);
typedef struct StarsAnimateAttackOps {
    StarsAnimateAttackLine draw_line;
    StarsAnimateAttackIcon draw_icon;
    StarsAnimateAttackAnimation animate_icon;
} StarsAnimateAttackOps;
typedef struct StarsAnimateAttackInput {
    const StarsAnimateAttackToken *tokens;
    const StarsAnimateAttackRecord *records;
    StarsU16 token_count;
    StarsU16 record_count;
    StarsU16 battle_square_size;
    StarsI16 animate;
    StarsAnimateAttackOps ops;
    void *context;
} StarsAnimateAttackInput;
void stars_animate_attack(StarsU16, StarsAnimateAttackInput *);

typedef struct StarsDrawVcrToken {
    StarsI16 player;
    StarsI16 design;
    StarsI16 ship_count;
    StarsU16 battle_square;
} StarsDrawVcrToken;
typedef void (*StarsDrawVcrStats)(void *, const StarsDrawVcrToken *,
    StarsI32 *, StarsU16 *, StarsI16 *);
typedef void (*StarsDrawVcrLabel)(void *, const StarsDrawVcrToken *, StarsU8 *,
    StarsU16);
typedef void (*StarsDrawVcrFleet)(void *, StarsU16, const StarsDrawVcrToken *);
typedef void (*StarsDrawVcrText)(void *, StarsU16, StarsI16, StarsI16,
    const StarsU8 *);
typedef void (*StarsDrawVcrAttack)(void *, StarsU16);
typedef struct StarsDrawVcrOps {
    StarsDrawVcrStats get_stats;
    StarsDrawVcrLabel format_label;
    StarsDrawVcrFleet draw_fleet;
    StarsDrawVcrText draw_text;
    StarsDrawVcrAttack draw_attack;
} StarsDrawVcrOps;
typedef struct StarsDrawVcrInput {
    const StarsDrawVcrToken *tokens;
    StarsU16 token_count;
    StarsU16 board_square_size;
    StarsDrawVcrOps ops;
    void *context;
} StarsDrawVcrInput;
void stars_draw_vcr(StarsU16, StarsI16, StarsI16, StarsDrawVcrInput *);

typedef struct StarsAiTurnPlayer {
    StarsI16 dead;
    StarsU16 personality;
} StarsAiTurnPlayer;
typedef void *(*StarsAiTurnAllocate)(void *, StarsU32, StarsU16);
typedef void (*StarsAiTurnFree)(void *, void *, StarsU16);
typedef StarsI16 (*StarsAiTurnLoad)(void *, StarsI16, StarsI16);
typedef void (*StarsAiTurnSimple)(void *);
typedef void (*StarsAiTurnPersonality)(void *, StarsI16, void *);
typedef void (*StarsAiTurnLog)(void *, StarsI16);
typedef struct StarsDoAiTurnInput {
    StarsAiTurnPlayer *players;
    StarsU16 player_count;
    StarsU16 planet_capacity;
    StarsI16 player;
    StarsU16 requested_personality;
    StarsAiTurnLoad load_game;
    StarsAiTurnSimple destroy_game;
    StarsAiTurnAllocate allocate;
    StarsAiTurnFree free_memory;
    StarsAiTurnSimple compute_design_powers;
    StarsAiTurnSimple mark_planets_under_attack;
    StarsAiTurnSimple increase_minefield_sizes;
    StarsAiTurnSimple init_random_planet_list;
    StarsAiTurnPersonality personalities[8];
    StarsAiTurnLog write_log;
    StarsAiTurnLog write_history;
    StarsI16 ai_active;
    void *context;
    StarsI16 *current_player; /* Shared idPlayer; loader publishes, coordinator restores. */
} StarsDoAiTurnInput;
void stars_do_ai_turn(StarsI16, StarsU16, StarsDoAiTurnInput *);

/* The Robotoid personality maintains a fixed set of ship-design roles.  The
 * real program stores these in the global _shdef table; this view exposes only
 * the fields EnsureRobotoidShdefs uses when deciding whether a role is due. */
typedef struct StarsRobotoidShdef {
    StarsU16 active;
    StarsI16 turn_created;
    StarsU16 existing;
    StarsU16 hull_id;
} StarsRobotoidShdef;
typedef StarsI16 (*StarsRobotoidRandom)(void *, StarsI16);
typedef StarsI16 (*StarsRobotoidCreateShdef)(void *, StarsI16, StarsU16,
    StarsI16);
typedef void (*StarsRobotoidChangeShdef)(void *, StarsI16,
    const StarsRobotoidShdef *);
typedef struct StarsEnsureRobotoidShdefsInput {
    StarsRobotoidShdef *designs;
    StarsU16 design_count;
    StarsU8 tech[6];
    StarsI16 turn;
    StarsU16 starter_hull;
    StarsU16 starter_existing;
    StarsRobotoidRandom random_value;
    StarsRobotoidCreateShdef create_shdef;
    StarsRobotoidChangeShdef change_shdef;
    void *context;
} StarsEnsureRobotoidShdefsInput;
void stars_ensure_robotoid_shdefs(StarsEnsureRobotoidShdefsInput *);

typedef struct StarsRobotoidTurnPlanet {
    StarsI16 id;
    StarsI16 owner;
    StarsI16 last_seen_turns;
    StarsU16 starbase_flags;
    StarsU16 visibility_flags;
    StarsU16 packet_strength;
    StarsI32 mineral_rate;
    StarsI32 mineral_concentration;
    StarsI16 production_slot_open;
    StarsI32 resources_available[4];
    StarsI32 production_queue_cost[4];
    StarsI32 hull_cost[4];
} StarsRobotoidTurnPlanet;
typedef struct StarsRobotoidTurnFleet {
    StarsI16 id;
    StarsI16 owner;
    StarsI16 planet_id;
    StarsU8 warship_count;
    StarsI16 cord;
    StarsI16 order_kind;
    StarsU8 *raw_plord;
    StarsU8 ship_counts[16];
} StarsRobotoidTurnFleet;
typedef StarsI16 (*StarsRobotoidMergeShdefs)(void *, StarsI16);
typedef StarsI16 (*StarsRobotoidEnsureAi)(void *, StarsI16, StarsI16 *);
typedef StarsI16 (*StarsRobotoidCheckDesignStatus)(void *, StarsI16, StarsI16,
    StarsI16, StarsI16 *, StarsU8[16]);
typedef StarsI16 (*StarsRobotoidDesignStatusInfo)(void *, StarsI16,
    StarsI16 *, StarsI16 *);
typedef StarsI16 (*StarsRobotoidSplitShdefs)(void *, const StarsU8[16]);
typedef StarsI16 (*StarsRobotoidEnsureShdefs)(void *);
typedef StarsI16 (*StarsRobotoidCountWarFleet)(void *,
    const StarsRobotoidTurnFleet *, StarsI16);
typedef StarsI16 (*StarsRobotoidMarkPlanet)(void *,
    const StarsRobotoidTurnPlanet *, StarsI16);
typedef StarsI16 (*StarsRobotoidMarkPlanetTyped)(void *,
    const StarsRobotoidTurnPlanet *, StarsI16, StarsI16);
typedef StarsI16 (*StarsRobotoidRunProduction)(void *,
    const StarsRobotoidTurnPlanet *, StarsI16);
typedef StarsI16 (*StarsRobotoidQueueProduction)(void *,
    const StarsRobotoidTurnPlanet *, StarsI16, StarsI16);
typedef StarsI16 (*StarsRobotoidRunFleets)(void *, StarsI16);
typedef StarsI16 (*StarsRobotoidRunBasicTasks)(void *, StarsI16);
typedef StarsI16 (*StarsRobotoidRunBasicTasksWithResearch)(void *, StarsI16,
    StarsI16, StarsI16);
typedef StarsI16 (*StarsRobotoidFillProductionQueue)(void *);
typedef struct StarsRobotoidFleetPolicy {
    StarsI16 total_ships;
    StarsI16 colonizer_ships;
    StarsI16 transport_ships;
    StarsI16 armed_ships;
    StarsI16 support_design_ships;
    StarsI16 attack_design_ships;
    StarsI16 needs_order_repair;
} StarsRobotoidFleetPolicy;
typedef StarsI16 (*StarsRobotoidVisitFleetTyped)(void *,
    const StarsRobotoidTurnFleet *, const StarsRobotoidFleetPolicy *,
    const StarsU8[4]);
/* The raw procedure repairs an empty first order before its later
 * colonizer/transport/attack routing.  Keep the mutation behind a typed
 * boundary until the packed PLORD view is complete. */
typedef StarsI16 (*StarsRobotoidRepairOrder)(void *,
    const StarsRobotoidTurnFleet *, const StarsRobotoidFleetPolicy *,
    const StarsU8[4]);
#define STARS_ROBOTOID_FLEET_ACTION_NONE 0
#define STARS_ROBOTOID_FLEET_ACTION_CLEAR_TASK 1
#define STARS_ROBOTOID_FLEET_ACTION_MOVE 2
#define STARS_ROBOTOID_FLEET_ACTION_COLONIZE 3
#define STARS_ROBOTOID_FLEET_ACTION_WORMHOLE 4
#define STARS_ROBOTOID_FLEET_ACTION_MOVE_STARBASE 5
#define STARS_ROBOTOID_FLEET_ACTION_ATTACK 6
#define STARS_ROBOTOID_FLEET_ACTION_TARGET_ARMADA 7
#define STARS_ROBOTOID_FLEET_ACTION_TRANSPORT 8
#define STARS_ROBOTOID_FLEET_ACTION_TARGET_FREIGHTER 9
typedef struct StarsRobotoidFleetPlan {
    StarsI16 action;
    StarsI16 target_planet;
    StarsI16 order_kind;
    StarsI16 append;
    StarsI16 reason;
} StarsRobotoidFleetPlan;
typedef StarsI16 (*StarsRobotoidPlanFleetAction)(void *,
    const StarsRobotoidTurnFleet *, const StarsRobotoidFleetPolicy *,
    StarsI16, StarsRobotoidFleetPlan *);
typedef StarsI16 (*StarsRobotoidApplyFleetAction)(void *,
    const StarsRobotoidTurnFleet *, const StarsRobotoidFleetPlan *);
typedef StarsI16 (*StarsRobotoidFleetAction)(void *,
    const StarsRobotoidTurnFleet *, const StarsRobotoidFleetPlan *);
typedef struct StarsRobotoidAiTurnOps {
    StarsRobotoidMergeShdefs merge_shdefs;
    StarsRobotoidEnsureAi ensure_ai;
    StarsRobotoidCheckDesignStatus check_design_status;
    StarsRobotoidDesignStatusInfo design_status_info;
    StarsRobotoidSplitShdefs split_shdefs;
    StarsRobotoidEnsureShdefs ensure_shdefs;
    StarsRobotoidCountWarFleet count_war_fleet;
    StarsRobotoidMarkPlanet mark_planet;
    StarsRobotoidMarkPlanetTyped mark_planet_typed;
    StarsRobotoidRunProduction run_production;
    StarsRobotoidQueueProduction queue_production;
    StarsRobotoidRunFleets run_fleets;
    StarsRobotoidRepairOrder repair_order;
    StarsRobotoidPlanFleetAction plan_fleet_action;
    StarsRobotoidApplyFleetAction apply_fleet_action;
    StarsRobotoidFleetAction clear_task;
    StarsRobotoidFleetAction move;
    StarsRobotoidFleetAction colonize;
    StarsRobotoidFleetAction goto_wormhole;
    StarsRobotoidFleetAction move_starbase;
    StarsRobotoidFleetAction attack;
    StarsRobotoidFleetAction target_armada;
    StarsRobotoidFleetAction transport;
    StarsRobotoidFleetAction target_freighter;
    StarsRobotoidVisitFleetTyped visit_fleet_typed;
    StarsRobotoidRunBasicTasks run_basic_tasks;
    StarsRobotoidRunBasicTasksWithResearch run_basic_tasks_with_research;
    StarsRobotoidFillProductionQueue fill_production_queue;
    void *context;
} StarsRobotoidAiTurnOps;
typedef struct StarsRobotoidAiTurnInput {
    StarsI16 turn;
    StarsI16 player_id;
    StarsI16 planet_count;
    const StarsRobotoidTurnPlanet *planets;
    StarsI16 fleet_count;
    const StarsRobotoidTurnFleet *fleets;
    const StarsRobotoidAiTurnOps *ops;
    StarsU8 armada_potency[4];
    /* Shared CheckAiShdefStatus output consumed by late-turn splitting. */
    StarsU8 split_status_mask[16];
    StarsI16 war_fleet_count;
    StarsI16 hostile_planet_count;
    StarsI16 production_queued;
    StarsI16 production_hulls_queued;
    StarsI16 research_order;
    StarsI16 latest_starbase_design;
} StarsRobotoidAiTurnInput;
StarsI16 stars_do_robotoid_ai_turn(StarsRobotoidAiTurnInput *);

typedef struct StarsCyberTurnPlanet {
    StarsI16 id;
    StarsI16 owner;
    StarsU16 habitat_kind;
    StarsU32 minerals[3];
    /* Signed production rates are distinct from concentration values in the
     * packed planet record and participate in the low-mineral predicate. */
    StarsI32 mineral_rates[3];
    StarsU16 starbase_flags;
    StarsU16 packet_strength;
    StarsU16 visibility_flags;
    StarsI32 resources_available[4];
    StarsI32 production_queue_cost[4];
} StarsCyberTurnPlanet;
typedef struct StarsCyberTurnFleet {
    StarsI16 id;
    StarsI16 owner;
    StarsI16 planet_id;
    StarsI16 cord;
    StarsI16 order_kind;
    StarsU8 *raw_plord;
    StarsU8 ship_counts[16];
    StarsU16 raw_plord_bytes;
} StarsCyberTurnFleet;
typedef StarsI16 (*StarsCyberMergeShdefs)(void *, StarsI16);
typedef StarsI16 (*StarsCyberEnsureAi)(void *, StarsI16, StarsI16 *);
typedef StarsI16 (*StarsCyberCheckDesignStatus)(void *, StarsI16, StarsI16,
    StarsI16, StarsI16 *);
typedef StarsI16 (*StarsCyberCheckDesignStatusEx)(void *, StarsI16, StarsI16,
    StarsI16, StarsI16 *, StarsU8[16]);
typedef StarsI16 (*StarsCyberSplitShdefs)(void *, const StarsU8[16]);
typedef StarsI16 (*StarsCyberEnsureShdefs)(void *, StarsI16);
typedef StarsI16 (*StarsCyberTurnMarkPlanet)(void *,
    const StarsCyberTurnPlanet *, StarsU16, StarsI16);
typedef StarsI16 (*StarsCyberMarkHostilePlanet)(void *,
    const StarsCyberTurnPlanet *, StarsI16, StarsI16);
typedef StarsI16 (*StarsCyberVisitFleet)(void *,
    const StarsCyberTurnFleet *, StarsI16);
typedef StarsI16 (*StarsCyberVisitProduction)(void *,
    const StarsCyberTurnPlanet *, StarsI16);
typedef StarsI16 (*StarsCyberRunBasicTasks)(void *, StarsI16);
typedef StarsI16 (*StarsCyberRunBasicTasksWithResearch)(void *, StarsI16,
    StarsI16, StarsI16);
typedef StarsI16 (*StarsCyberRunPackets)(void *, StarsI16);
/* The original turn passes the prepared planet table and packet-pressure
 * state into the packet-routing phase.  Keep the legacy player-only hook as
 * a fallback for callers that have not yet materialized that service. */
typedef StarsI16 (*StarsCyberRunPacketsEx)(void *, StarsI16,
    StarsCyberTurnPlanet *, StarsI16, StarsI16);
typedef StarsI16 (*StarsCyberFillProductionQueue)(void *);
typedef struct StarsCyberFleetPolicy {
    StarsI16 total_ships;
    StarsI16 colonizer_ships;
    StarsI16 transport_ships;
    StarsI16 armed_ships;
    StarsI16 auxiliary_design_ships;
    StarsI16 support_design_ships;
    StarsI16 attack_design_ships;
    StarsI16 needs_order_repair;
} StarsCyberFleetPolicy;
typedef StarsI16 (*StarsCyberVisitFleetTyped)(void *,
    const StarsCyberTurnFleet *, const StarsCyberFleetPolicy *,
    StarsU8);
typedef StarsI16 (*StarsCyberRepairOrder)(void *,
    const StarsCyberTurnFleet *, const StarsCyberFleetPolicy *, StarsU8);
/* DoCyberFreighter consumes the same typed fleet policy and armada potency
 * prepared by DoCyberAiTurn before the general fleet action is planned. */
typedef StarsI16 (*StarsCyberRunFreighterEx)(void *,
    const StarsCyberTurnFleet *, const StarsCyberFleetPolicy *, StarsU8);
#define STARS_CYBER_FLEET_ACTION_NONE 0
#define STARS_CYBER_FLEET_ACTION_CLEAR_TASK 1
#define STARS_CYBER_FLEET_ACTION_MOVE 2
#define STARS_CYBER_FLEET_ACTION_COLONIZE 3
#define STARS_CYBER_FLEET_ACTION_WORMHOLE 4
#define STARS_CYBER_FLEET_ACTION_MOVE_STARBASE 5
#define STARS_CYBER_FLEET_ACTION_ATTACK 6
#define STARS_CYBER_FLEET_ACTION_TARGET_ARMADA 7
#define STARS_CYBER_FLEET_ACTION_TRANSPORT 8
#define STARS_CYBER_FLEET_ACTION_TARGET_FREIGHTER 9
typedef struct StarsCyberFleetPlan {
    StarsI16 action;
    StarsI16 target_planet;
    StarsI16 order_kind;
    StarsI16 append;
    StarsI16 reason;
} StarsCyberFleetPlan;
typedef StarsI16 (*StarsCyberPlanFleetAction)(void *,
    const StarsCyberTurnFleet *, const StarsCyberFleetPolicy *, StarsU8,
    StarsCyberFleetPlan *);
typedef StarsI16 (*StarsCyberApplyFleetAction)(void *,
    const StarsCyberTurnFleet *, const StarsCyberFleetPlan *);
typedef StarsI16 (*StarsCyberFleetAction)(void *,
    const StarsCyberTurnFleet *, const StarsCyberFleetPlan *);
typedef struct StarsCyberAiTurnOps {
    StarsCyberMergeShdefs merge_shdefs;
    StarsCyberEnsureAi ensure_ai;
    StarsCyberCheckDesignStatus check_design_status;
    StarsCyberCheckDesignStatusEx check_design_status_ex;
    StarsCyberSplitShdefs split_shdefs;
    StarsCyberEnsureShdefs ensure_shdefs;
    StarsCyberTurnMarkPlanet mark_planet;
    StarsCyberMarkHostilePlanet mark_hostile_planet;
    StarsCyberVisitFleet visit_fleet;
    StarsCyberVisitProduction visit_production;
    StarsCyberRepairOrder repair_order;
    StarsCyberPlanFleetAction plan_fleet_action;
    StarsCyberApplyFleetAction apply_fleet_action;
    StarsCyberFleetAction clear_task;
    StarsCyberFleetAction move;
    StarsCyberFleetAction colonize;
    StarsCyberFleetAction goto_wormhole;
    StarsCyberFleetAction move_starbase;
    StarsCyberFleetAction attack;
    StarsCyberFleetAction target_armada;
    StarsCyberFleetAction transport;
    StarsCyberFleetAction target_freighter;
    StarsCyberVisitFleetTyped visit_fleet_typed;
    StarsCyberRunFreighterEx run_freighter_ex;
    StarsCyberRunBasicTasks run_basic_tasks;
    StarsCyberRunBasicTasksWithResearch run_basic_tasks_with_research;
    StarsCyberRunPacketsEx run_packets_ex;
    StarsCyberRunPackets run_packets;
    StarsCyberFillProductionQueue fill_production_queue;
    void *context;
} StarsCyberAiTurnOps;
typedef struct StarsCyberAiTurnInput {
    StarsI16 turn;
    StarsI16 player_id;
    StarsI16 planet_count;
    StarsCyberTurnPlanet *planets;
    StarsI16 fleet_count;
    const StarsCyberTurnFleet *fleets;
    const StarsCyberAiTurnOps *ops;
    StarsU8 armada_potency[4];
    StarsI16 packet_pressure;
    StarsI16 low_mineral_planets;
    StarsI16 production_affordable_count;
    StarsI16 production_blocked_count;
    StarsI16 research_order;
    StarsI16 latest_starbase_design;
    StarsU8 *raw_ai_planet_table;
    StarsU16 raw_ai_planet_record_count;
} StarsCyberAiTurnInput;
StarsI16 stars_do_cyber_ai_turn(StarsCyberAiTurnInput *);

typedef struct StarsAutomitronTurnPlanet {
    StarsI16 id;
    StarsI16 owner;
    StarsI16 planet_class;
    StarsU16 starbase_flags;
    StarsU16 visibility_flags;
    StarsI16 desirability;
    StarsI16 optimal_value;
    StarsI32 mineral_rate;
    StarsI32 mineral_concentration;
    StarsI16 production_slot_open;
    StarsI32 resources_available[4];
    StarsI32 production_queue_cost[4];
    StarsI32 hull_cost[4];
    /* Caller-verified absence of the original queue item that bypasses the
     * colonizer seed branch.  Zero leaves this branch unbound/disabled. */
    StarsI16 colonizer_queue_eligible;
    /* Caller-verified absence of the queue item that bypasses the original
     * early tech/colonizer/support-design queue branches. */
    StarsI16 support_queue_eligible;
    /* Optional live PLPROD header plus four-byte queue items. When bound,
     * its original bypass scan supersedes the caller eligibility flags. */
    const StarsU8 *raw_prod_queue;
    StarsU16 raw_prod_queue_size;
    /* Optional live sixteen-byte vlpbAiPlanet[id] record. */
    StarsU8 *raw_ai_planet;
    StarsU16 raw_ai_planet_size;
} StarsAutomitronTurnPlanet;
typedef struct StarsAutomitronTurnFleet {
    StarsI16 id;
    StarsI16 owner;
    StarsI16 planet_id;
    StarsI16 cord;
    StarsI16 order_kind;
    StarsU8 *raw_plord;
    StarsU16 raw_plord_bytes;
    StarsU8 ship_counts[16];
    /* Optional live 124-byte _fleet record for direct turn-flag writes. */
    StarsU8 *raw_fleet_record;
    StarsU16 raw_fleet_size;
    StarsI32 fuel;
    StarsI16 is_ai_transport;
} StarsAutomitronTurnFleet;
typedef StarsI16 (*StarsAutomitronMergeShdefs)(void *, StarsI16);
typedef StarsI16 (*StarsAutomitronEnsureAi)(void *, StarsI16, StarsI16 *);
typedef StarsI16 (*StarsAutomitronCheckDesignStatus)(void *, StarsI16,
    StarsI16, StarsI16, StarsI16 *);
typedef StarsI16 (*StarsAutomitronCheckDesignStatusEx)(void *, StarsI16,
    StarsI16, StarsI16, StarsI16 *, StarsU8[16]);
typedef StarsI16 (*StarsAutomitronSplitShdefs)(void *, const StarsU8[16]);
typedef StarsI16 (*StarsAutomitronEnsureShdefs)(void *, StarsI16);
typedef StarsI16 (*StarsAutomitronVisitPlanet)(void *,
    const StarsAutomitronTurnPlanet *, StarsI16);
typedef StarsI16 (*StarsAutomitronPlanetValue)(void *,
    const StarsAutomitronTurnPlanet *, StarsI16);
typedef StarsI16 (*StarsAutomitronVisitFleet)(void *,
    const StarsAutomitronTurnFleet *, StarsI16);
/* Typed view of the raw hull-slot aggregates consulted by the Automitron
 * fleet policy.  The slot groups remain explicit until the packed design
 * records behind the higher-level role predicates are recovered. */
typedef struct StarsAutomitronFleetPolicy {
    StarsI16 total_ships;
    StarsI16 slot_1_ships;
    StarsI16 slot_2_3_ships;
    StarsI16 slot_6_ships;
    StarsI16 slot_9_10_ships;
    StarsI16 needs_order_repair;
} StarsAutomitronFleetPolicy;
typedef StarsI16 (*StarsAutomitronVisitFleetTyped)(void *,
    const StarsAutomitronTurnFleet *, const StarsAutomitronFleetPolicy *,
    StarsI16);
/* The raw Automitron loop repairs a cord-one fleet whose first order kind is
 * zero before its typed fleet handling continues. */
typedef StarsI16 (*StarsAutomitronRepairOrder)(void *,
    const StarsAutomitronTurnFleet *, const StarsAutomitronFleetPolicy *,
    const StarsU8[4]);
#define STARS_AUTOMITRON_FLEET_ACTION_NONE 0
#define STARS_AUTOMITRON_FLEET_ACTION_CLEAR_TASK 1
#define STARS_AUTOMITRON_FLEET_ACTION_MOVE 2
#define STARS_AUTOMITRON_FLEET_ACTION_COLONIZE 3
#define STARS_AUTOMITRON_FLEET_ACTION_WORMHOLE 4
#define STARS_AUTOMITRON_FLEET_ACTION_MOVE_STARBASE 5
#define STARS_AUTOMITRON_FLEET_ACTION_ATTACK 6
#define STARS_AUTOMITRON_FLEET_ACTION_TARGET_ARMADA 7
#define STARS_AUTOMITRON_FLEET_ACTION_TRANSPORT 8
#define STARS_AUTOMITRON_FLEET_ACTION_TARGET_FREIGHTER 9
typedef struct StarsAutomitronFleetPlan {
    StarsI16 action;
    StarsI16 target_planet;
    StarsI16 order_kind;
    StarsI16 append;
    StarsI16 reason;
    /* Exact caller-resolved _order bytes for FMoveAiFleet fallback when the
     * target-search callback has already built an order. */
    StarsI16 packed_order_valid;
    StarsMoveOrder packed_order;
} StarsAutomitronFleetPlan;
typedef StarsI16 (*StarsAutomitronPlanFleetAction)(void *,
    const StarsAutomitronTurnFleet *, const StarsAutomitronFleetPolicy *,
    StarsI16, StarsAutomitronFleetPlan *);
typedef StarsI16 (*StarsAutomitronApplyFleetAction)(void *,
    const StarsAutomitronTurnFleet *, const StarsAutomitronFleetPlan *);
typedef StarsI16 (*StarsAutomitronFleetAction)(void *,
    const StarsAutomitronTurnFleet *, const StarsAutomitronFleetPlan *);
typedef StarsI16 (*StarsAutomitronRefreshPackedOrder)(void *,
    const StarsAutomitronTurnFleet *);
typedef StarsI16 (*StarsAutomitronQueueProduction)(void *,
    const StarsAutomitronTurnPlanet *, StarsI16, StarsI16);
typedef StarsI16 (*StarsAutomitronRandom)(void *, StarsI16);
typedef StarsI16 (*StarsAutomitronAddRawQueueItem)(void *,
    const StarsAutomitronTurnPlanet *, StarsI16, StarsI16, StarsI16, StarsI16);
typedef StarsI16 (*StarsAutomitronIsPotentWarFleet)(void *,
    const StarsAutomitronTurnFleet *, StarsI16);
typedef StarsI16 (*StarsAutomitronInitProduction)(void *,
    const StarsAutomitronTurnPlanet *);
typedef StarsI16 (*StarsAutomitronFinishProduction)(void *,
    const StarsAutomitronTurnPlanet *, StarsI16);
typedef StarsI16 (*StarsAutomitronGetProductionBudget)(void *,
    const StarsAutomitronTurnPlanet *, StarsI32[4], StarsI32[4]);
typedef StarsI16 (*StarsAutomitronGetHullCost)(void *,
    StarsI16, StarsI16, StarsI32[4]);
typedef StarsI16 (*StarsAutomitronRunBasicTasks)(void *, StarsI16, StarsI16,
    StarsI16);
typedef StarsI16 (*StarsAutomitronFillProductionQueue)(void *);
typedef struct StarsAutomitronAiTurnOps {
    StarsAutomitronMergeShdefs merge_shdefs;
    StarsAutomitronEnsureAi ensure_ai;
    StarsAutomitronCheckDesignStatus check_design_status;
    StarsAutomitronCheckDesignStatusEx check_design_status_ex;
    StarsAutomitronSplitShdefs split_shdefs;
    StarsAutomitronEnsureShdefs ensure_shdefs;
    StarsAutomitronVisitPlanet visit_planet;
    StarsAutomitronPlanetValue pct_planet_opt_value;
    StarsAutomitronPlanetValue pct_planet_desirability;
    StarsAutomitronVisitFleet visit_fleet;
    StarsAutomitronVisitFleetTyped visit_fleet_typed;
    StarsAutomitronRepairOrder repair_order;
    StarsAutomitronPlanFleetAction plan_fleet_action;
    StarsAutomitronApplyFleetAction apply_fleet_action;
    StarsAutomitronFleetAction clear_task;
    StarsAutomitronFleetAction move;
    StarsAutomitronFleetAction colonize;
    StarsAutomitronFleetAction goto_wormhole;
    StarsAutomitronFleetAction move_starbase;
    StarsAutomitronFleetAction attack;
    StarsAutomitronFleetAction target_armada;
    StarsAutomitronFleetAction transport;
    StarsAutomitronFleetAction target_freighter;
    StarsAutomitronRefreshPackedOrder refresh_packed_order;
    StarsAutomitronQueueProduction queue_production;
    StarsAutomitronRandom random;
    StarsAutomitronAddRawQueueItem add_raw_queue_item;
    StarsAutomitronIsPotentWarFleet is_potent_war_fleet;
    StarsAutomitronInitProduction init_production;
    StarsAutomitronFinishProduction finish_production;
    StarsAutomitronGetProductionBudget get_production_budget;
    StarsAutomitronGetHullCost get_hull_cost;
    StarsAutomitronRunBasicTasks run_basic_tasks;
    StarsAutomitronFillProductionQueue fill_production_queue;
    void *context;
} StarsAutomitronAiTurnOps;
typedef struct StarsAutomitronAiTurnInput {
    StarsI16 turn;
    StarsI16 player_id;
    StarsI16 planet_count;
    const StarsAutomitronTurnPlanet *planets;
    StarsI16 fleet_count;
    const StarsAutomitronTurnFleet *fleets;
    const StarsAutomitronAiTurnOps *ops;
    StarsU8 armada_potency[4];
    StarsI16 unowned_viable_count;
    StarsI16 owned_bad_count;
    StarsI16 production_queued;
    StarsI16 production_hulls_queued;
    StarsI16 research_order;
    StarsI16 latest_starbase_design;
    /* Live rgshdef[6] flag word and queue-table state are caller bindings,
     * not inferred from the reduced planet/fleet records. */
    StarsI16 colonizer_seed_inputs_bound;
    StarsU16 colonizer_design_flags;
    StarsI16 colonizer_seed_queued;
    StarsI16 support_design_queued;
    /* Live early-queue inputs: AI-data group count, owned planet count,
     * technology at index 2, and whether design 1 already exists. */
    StarsI16 early_queue_inputs_bound;
    StarsI16 ai_data_group_count;
    StarsI16 owned_planet_count;
    StarsI16 tech_level_two;
    StarsI16 design_one_exists;
    StarsI16 tech_support_queued;
    StarsI16 initial_colonizer_queued;
    /* Live game.cPlanMax and rgshdef existence counts for the two five-hull
     * batches. Budget and hull costs are retrieved through ops per original
     * call, so earlier queue items can affect the second batch. */
    StarsI16 hull_batch_inputs_bound;
    StarsI16 game_planet_max;
    StarsI32 is_design_existing;
    StarsI32 attack_design_existing;
    /* Live rgshdef[0].hul.rghs[0] low byte and FIsAiTransport result are
     * supplied by the caller for the scout low-fuel order gate. */
    StarsI16 scout_fuel_gate_bound;
    StarsU8 hull0_first_slot_byte;
    /* Optional live vlpbAiPlanet table, indexed by planet ID at 16 bytes
     * per record. A planet's explicit raw_ai_planet pointer takes precedence. */
    StarsU8 *raw_ai_planet_table;
    StarsU16 raw_ai_planet_record_count;
} StarsAutomitronAiTurnInput;
StarsI16 stars_do_automitron_ai_turn(StarsAutomitronAiTurnInput *);

typedef struct StarsRototillTurnPlanet {
    StarsI16 id;
    StarsI16 owner;
    StarsI16 planet_class;
    StarsU16 visibility_flags;
    StarsU8 mineral_concentration[3];
    StarsI16 optimal_value;
    StarsI16 desirability;
    StarsU16 productive;
    /* Live production predicate from the packed planet mineral words. */
    StarsI16 mineral_rate_high;
    StarsU16 mineral_reserve;
    StarsU8 *raw_ai_planet;
    StarsU16 raw_ai_planet_size;
    const StarsU8 *raw_prod_queue;
    StarsU16 raw_prod_queue_size;
    /* Optional original 56-byte _planet; when bound, packed identity,
     * ownership, class, flags, concentrations, and mineral gate take
     * precedence over the scalar snapshot above. */
    const StarsU8 *raw_planet_record;
    StarsU16 raw_planet_record_size;
} StarsRototillTurnPlanet;
typedef struct StarsRototillTurnFleet {
    StarsI16 id;
    StarsI16 owner;
    StarsI16 planet_id;
    StarsI16 cord;
    StarsI16 order_kind;
    StarsI16 ship_counts[16];
    /* Optional mutable original 124-byte _fleet; typed counts and status
     * are decoded from it, and the original fMark bit is cleared in place. */
    StarsU8 *raw_fleet_record;
    StarsU16 raw_fleet_record_size;
    /* Optional four-byte PLORD header plus 18-byte orders. Needed when a
     * fleet's current idPlanet is -1 and its second order supplies target. */
    StarsU8 *raw_plord;
    StarsU16 raw_plord_bytes;
} StarsRototillTurnFleet;
typedef StarsI16 (*StarsRototillEnsureAi)(void *, StarsI16, StarsI16 *);
typedef StarsI16 (*StarsRototillEnsureShdefs)(void *, StarsI16);
typedef StarsI16 (*StarsRototillRunBasicTasks)(void *, StarsI16, StarsI16,
    StarsI16);
typedef StarsI16 (*StarsRototillFillProductionQueue)(void *);
typedef StarsI16 (*StarsRototillVisitPlanet)(void *,
    const StarsRototillTurnPlanet *, StarsU16, StarsI16);
typedef StarsI16 (*StarsRototillPlanetValue)(void *,
    const StarsRototillTurnPlanet *, StarsI16);
typedef StarsI16 (*StarsRototillProductionStart)(void *,
    const StarsRototillTurnPlanet *);
typedef StarsI16 (*StarsRototillCurrentProdQueue)(void *,
    const StarsRototillTurnPlanet *, const StarsU8 **, StarsU16 *);
typedef StarsI16 (*StarsRototillProductionFinish)(void *,
    const StarsRototillTurnPlanet *, StarsI16);
typedef StarsI16 (*StarsRototillAddQueueItem)(void *,
    const StarsRototillTurnPlanet *, StarsI16, StarsI16, StarsI16, StarsI16);
typedef StarsI16 (*StarsRototillVisitFleet)(void *,
    const StarsRototillTurnFleet *, StarsI16);
typedef struct StarsRototillFleetPolicy {
    StarsI16 total_ships;
    StarsI16 colonizer_ships;
    StarsI16 transport_ships;
    StarsI16 armed_ships;
    StarsI16 needs_order_repair;
} StarsRototillFleetPolicy;
typedef StarsI16 (*StarsRototillVisitFleetTyped)(void *,
    const StarsRototillTurnFleet *, const StarsRototillFleetPolicy *,
    StarsI16);
typedef StarsI16 (*StarsRototillRepairOrder)(void *,
    const StarsRototillTurnFleet *, StarsI16);
typedef StarsI16 (*StarsRototillClearAiTask)(void *,
    const StarsRototillTurnFleet *, StarsI16);
typedef StarsI16 (*StarsRototillIsAiTransport)(void *,
    const StarsRototillTurnFleet *);
typedef StarsI16 (*StarsRototillRaceStat)(void *, StarsI16, StarsI16);
typedef StarsI16 (*StarsRototillNearestStarbase)(void *,
    const StarsRototillTurnFleet *, StarsI16);
typedef StarsI16 (*StarsRototillTargetScout)(void *,
    const StarsRototillTurnFleet *, StarsI16);
typedef StarsI16 (*StarsRototillNearestColonizable)(void *,
    const StarsRototillTurnFleet *, StarsI16);
typedef StarsI16 (*StarsRototillTransferSupply)(void *,
    const StarsRototillTurnFleet *, StarsI16);
typedef void (*StarsRototillColonizeFleet)(void *,
    const StarsRototillTurnFleet *, StarsI16);
typedef struct StarsRototillAiTurnOps {
    StarsRototillEnsureAi ensure_ai;
    StarsRototillEnsureShdefs ensure_shdefs;
    StarsRototillVisitPlanet visit_planet;
    StarsRototillPlanetValue pct_planet_opt_value;
    StarsRototillPlanetValue pct_planet_desirability;
    StarsRototillProductionStart init_production;
    /* Read lpplProdGlob after InitProduction; supersedes the pre-init
     * planet queue snapshot for bypass and subsequent queue callbacks. */
    StarsRototillCurrentProdQueue current_prod_queue;
    StarsRototillProductionFinish finish_production;
    StarsRototillAddQueueItem add_raw_queue_item;
    StarsRototillVisitFleet visit_fleet;
    StarsRototillVisitFleetTyped visit_fleet_typed;
    StarsRototillRepairOrder repair_order;
    /* Occupied-destination route reset service order: select, mutate the
     * bound packed fleet/PLORD, refresh, then clear AI task zero. */
    StarsRototillRepairOrder select_fleet;
    StarsRototillRepairOrder refresh_fleet;
    StarsRototillClearAiTask clear_ai_current_task;
    StarsRototillIsAiTransport is_ai_transport;
    StarsRototillRaceStat race_stat;
    StarsRototillNearestStarbase move_nearest_starbase;
    /* IdTargetScout is reached after the caller's packed fleet gates and
     * selection. The service owns target search and FMoveAiFleet. */
    StarsRototillTargetScout target_scout;
    StarsRototillNearestColonizable nearest_colonizable;
    StarsRototillTransferSupply transfer_supply;
    StarsRototillColonizeFleet colonize_fleet;
    StarsRototillRunBasicTasks run_basic_tasks;
    StarsRototillFillProductionQueue fill_production_queue;
    void *context;
} StarsRototillAiTurnOps;
typedef struct StarsRototillAiTurnInput {
    StarsI16 turn;
    StarsI16 player_id;
    StarsI16 planet_count;
    const StarsRototillTurnPlanet *planets;
    StarsI16 fleet_count;
    const StarsRototillTurnFleet *fleets;
    const StarsRototillAiTurnOps *ops;
    StarsI16 current_order;
    StarsI16 latest_starbase_design;
    StarsI16 unowned_viable_count;
    StarsI16 owned_bad_count;
    StarsI16 productive_count;
    StarsU16 mineral_score;
    StarsI16 production_inputs_bound;
    StarsI32 design_one_existing;
    StarsI16 production_items_added;
    StarsI16 design_one_queued;
    StarsI16 queue_bypass_count;
    /* Optional live vlpbAiPlanet table, indexed by planet ID with 16-byte
     * records. Per-planet raw_ai_planet takes precedence. */
    StarsU8 *raw_ai_planet_table;
    StarsU16 raw_ai_planet_record_count;
    /* Opt in only when the caller supplies the complete planet list and
     * live AI table for the original unowned-destination marker branch. */
    StarsI16 fleet_unowned_marker_bound;
    /* Requires mutable packed fleet/PLORD and the three reset services. */
    StarsI16 fleet_occupied_repair_bound;
    /* NE 20:~24F5 transport reroute: target planet owner/point, mutable
     * selected fleet/PLORD, and live AI table are required. */
    StarsI16 fleet_transport_reroute_bound;
    const StarsPoint *planet_points;
    StarsU16 planet_point_count;
    /* Bounded second-sweep ship-slot-1 fallback. Hull byte <=1 skips
     * FMoveToNearestStarbase and reaches first PLORD kind 5 directly. */
    StarsI16 fleet_first_kind_deep_space_bound;
    StarsU8 ship1_first_hull_slot_byte;
    /* Also bind owned/foreign current-planet mineral and starbase gates;
     * a hull byte >1 requires the nearest-starbase service. */
    StarsI16 fleet_first_kind_selected_planet_bound;
    /* Opt-in source scout dispatch for ship slot 0. The caller supplies
     * FIsAiTransport, IdTargetScout, and selected-fleet services. */
    StarsI16 fleet_scout_bound;
    /* Second-sweep ship-slot-1 owned-planet supply branch. The nearest
     * colonizable lookup precedes XferAiSupply. Positive targets also use
     * the colonization callback when the marker branch is bound. */
    StarsI16 fleet_colonizer_supply_bound;
    /* A nonnegative nearest-colonizable target calls FColonizeAiFleet and
     * then writes 4 to the live AI-planet table's byte 15 for that target. */
    StarsI16 fleet_colonizer_marker_bound;
    StarsU8 ship0_first_hull_slot_byte;
} StarsRototillAiTurnInput;
StarsI16 stars_do_rototill_ai_turn(StarsRototillAiTurnInput *);
typedef StarsI16 (*StarsRototillScoutIdealWarp)(void *,
    const StarsRototillTurnFleet *, StarsI16, StarsPoint);
typedef StarsI16 (*StarsRototillEnsureScoutPlord)(void *,
    const StarsRototillTurnFleet *, StarsU16, StarsU8 **, StarsU16 *);
typedef StarsI16 (*StarsRototillBestHomeScoutTarget)(void *, StarsI16);
typedef StarsI16 (*StarsRototillScoutHasTeeth)(void *,
    const StarsRototillTurnFleet *);
typedef StarsU32 (*StarsRototillScoutFuelCapacity)(void *,
    const StarsRototillTurnFleet *);
typedef struct StarsRototillScoutEnemy {
    StarsI16 id;
    StarsPoint point;
    StarsI16 human;
    StarsI16 attack_reserved;
} StarsRototillScoutEnemy;
typedef struct StarsRototillScoutAdapter {
    StarsU8 *raw_ai_planet_table;
    StarsU16 ai_planet_record_count;
    const StarsPoint *planet_points;
    StarsU16 planet_count;
    /* -2 means absent from the loaded player-view planet table. */
    const StarsI16 *planet_owners;
    StarsU8 *status_scratch;
    StarsU16 status_scratch_count;
    const StarsI16 *blocked_targets;
    StarsU16 blocked_target_count;
    StarsI16 race_mode;
    StarsI16 turn;
    StarsI16 marked_planets;
    StarsU16 enemy_candidate_count;
    const StarsRototillScoutEnemy *enemy_candidates;
    StarsI16 only_humans;
    StarsRototillScoutHasTeeth has_teeth;
    StarsRototillScoutFuelCapacity fuel_capacity;
    StarsRototillRepairOrder move_to_starbase;
    const StarsI16 *attack_planet_ids;
    StarsU16 attack_planet_count;
    StarsAiPlanetOptValue planet_opt_value;
    StarsAiRandom random_value;
    StarsFindAiWormhole find_wormhole;
    /* IdTargetScout fallback after every planet status is known: use the
     * original best-enumeration service from the home planet, if present. */
    StarsI16 home_planet_id;
    StarsRototillBestHomeScoutTarget best_home_target;
    StarsRototillScoutIdealWarp ideal_warp;
    StarsRototillEnsureScoutPlord ensure_plord_capacity;
    StarsRototillRepairOrder select_fleet;
    StarsRototillRepairOrder refresh_fleet;
    void *context;
} StarsRototillScoutAdapter;
/* Bounded IdTargetScout routes through enemy candidates and the live
 * 16-byte AI planet table, optional wormhole/home fallback, and packed
 * FMoveAiFleet order writer. */
StarsI16 stars_rototill_target_scout(void *,
    const StarsRototillTurnFleet *, StarsI16);

/* Compact semantic view used by the FAttack reconstruction.  It keeps the
 * packed battle token and hull fields at the callback boundary, but makes the
 * original section matching, player mask, target-mode, and damage policy
 * explicit. */
typedef struct StarsAttackSection {
    StarsI16 init;
    /* If init is negative, derive the raw init byte from the attacker's
     * base plus the weapon-part delta.  The original uses an unsigned
     * 16-bit temporary and only caps the result at 63, preserving underflow
     * before the cap. */
    StarsI16 init_base;
    StarsI16 init_delta;
    /* Optional raw _hs boundary.  FAttack resolves non-empty 0x10/0x20
     * hull components through FLookupPart before matching initiative. */
    StarsU16 part_group;
    StarsU16 part_item;
    StarsU16 part_count;
    StarsI16 weapon_mode;
    StarsI16 target_slot;
    StarsU16 target_slots;
    /* The packed attacker target field contains two four-bit modes.  The
     * first selection pass uses mode zero and the second uses mode one. */
    StarsU16 target_mode_masks[2];
    StarsI16 battle_rating;
    /* When enabled, use the raw DzFromBrcBrc range gate before the
     * normalized battle-rating fallback. */
    StarsU16 range_enabled;
    StarsU8 target_range;
    StarsI32 base_damage;
    StarsI32 fuel_factor;
    StarsU16 cargo_flags;
    /* Optional raw complex-beam scoring inputs.  The original computes a
     * target value from the selected hull's resource/ironium cost and the
     * target's shield/hull capacity before choosing a best target. */
    /* Nonzero selects the raw complex target-score ranking for this section;
     * zero retains the ordinary typed best-damage ranking. */
    StarsU16 raw_scaling;
    StarsU16 raw_weapon_flags;
    /* Retained raw dispatch metadata. The beam uStack44 class is derived from
     * raw_part_dp by FAttack; the packed component policy bit stays separate. */
    StarsU16 raw_damage_flags;
    StarsU16 raw_capacity_percent;
    StarsU16 raw_part_dp;
    StarsU16 raw_distance;
    StarsU16 active;
    StarsI16 beam_passes;
    /* The packed weapon-part flag selects the simple beam path, which
     * broadcasts one damage event to every eligible target. */
    StarsU16 beam_broadcast;
    StarsI32 torpedo_count;
    StarsI16 torpedo_base_percent;
    StarsI16 battle_computer_percent;
    StarsU16 torpedo_weapon_flags;
    StarsI16 torpedo_passes;
    StarsI32 torpedo_shield_damage;
    StarsI32 torpedo_hull_damage;
    /* Raw group-0x20 record word at offset 54 used by FAttack's torpedo
     * allocation math. The _torp view names this field dp; the raw decompiler
     * inferred the overlapping _hul name wtFuelMax at the same offset. */
    StarsU16 raw_torpedo_power;
} StarsAttackSection;
/* Exact four-byte `_hs` slot view: the low/high bytes of packed_item_count
 * are iItem and cItem in the original hull record. */
typedef struct StarsAttackPackedPartSlot {
    StarsU16 group;
    StarsU8 item;
    StarsU8 count;
} StarsAttackPackedPartSlot;
typedef char stars_attack_packed_part_slot_size[
    (sizeof(StarsAttackPackedPartSlot) == 4) ? 1 : -1];
typedef struct StarsAttackToken {
    StarsI16 owner;
    StarsU8 battle_coordinate;
    StarsU16 active_flags;
    StarsU16 target_mask;
    StarsI16 battle_rating;
    StarsU16 beam_defense_percent;
    StarsU16 torpedo_jam_percent;
    StarsI32 resource_cost;
    StarsI32 ore_cost_ironium;
    StarsI32 shield_points;
    StarsI32 hull_dp;
    /* Raw `_tok.dv`: low seven bits are pctSh and the upper nine bits are
     * pctDp.  FAttack uses both packed components to reduce effective hull
     * capacity before complex-target scoring. */
    StarsU16 defense_value;
    StarsU16 ship_count;
} StarsAttackToken;
typedef StarsI16 (*StarsAttackDamageToken)(void *, StarsI16, StarsI16,
    StarsI16, StarsI32, StarsI16);
/* Raw FAttack forwards the mutable beam value and the other original
 * FDamageTok arguments. The callback may mutate beam damage and torpedo packet
 * count; the ordinary callback remains the compatibility fallback. */
typedef StarsI16 (*StarsAttackRawDamageToken)(void *, StarsI16, StarsI16,
    StarsI16, StarsI32 *, StarsI32, StarsU16, StarsI16, StarsU32 *);
typedef StarsI16 (*StarsAttackLookupPart)(void *, StarsU16, StarsU16,
    StarsU16, StarsAttackSection *);
/* Optional direct view of the selected original part record. `record_size`
 * must cover the returned record; FAttack reads the little-endian beam or
 * torpedo words at offsets 52, 54, and 56. */
typedef const StarsU8 *(*StarsAttackLookupPartRecord)(void *, StarsU16,
    StarsU16, StarsU16 *);
/* Re-read a target after attack damage mutates the live battle token. Raw
 * complex FAttack rescans all candidates, so a one-time const snapshot is
 * insufficient for faithful target ranking. Return nonzero on success. */
typedef StarsI16 (*StarsAttackReadTarget)(void *, StarsI16,
    StarsAttackToken *);
typedef struct StarsAttackOps {
    StarsAttackDamageToken damage_token;
    StarsAttackRawDamageToken raw_damage_token;
    StarsAttackLookupPart lookup_part;
    StarsAttackLookupPartRecord lookup_part_record;
    StarsTorpHitRandom torpedo_random;
    void *context;
    StarsAttackReadTarget read_target;
} StarsAttackOps;
typedef struct StarsAttackInput {
    StarsI16 attacker_index;
    /* Raw `_tok.csh`; used with component count and beam `_beam.dp` to seed
     * FAttack's aggregate `dpMain` residual. */
    StarsU16 attacker_ship_count;
    StarsI16 attacker_owner;
    StarsU8 attacker_coordinate;
    /* Original attacker `_tok.grobj`; FAttack adds one to beam dRangeMax
     * only when this is 1 (planet/starbase attacker). */
    StarsU8 attacker_object_kind;
    StarsI16 init;
    StarsU16 attack_players;
    StarsI16 section_count;
    const StarsAttackSection *sections;
    StarsI16 target_count;
    const StarsAttackToken *targets;
    const StarsAttackPackedPartSlot *packed_parts;
    StarsU16 packed_part_count;
    StarsAttackOps ops;
    /* Optional six-byte battle record.  FAttack owns the hit-count field and
     * first-hit token field; the battle coordinator owns the other fields. */
    StarsU8 *battle_record;
    StarsI16 first_hit_target;
    StarsI16 first_hit_section;
    StarsI16 hit_count;
} StarsAttackInput;
StarsI16 stars_f_attack(StarsAttackInput *);
StarsI16 stars_lookup_torpedo_dp(StarsU16);
StarsI16 stars_lookup_torpedo_init(StarsU16);
StarsI16 stars_lookup_torpedo_range(StarsU16);
StarsI16 stars_lookup_beam_dp(StarsU16);
StarsI16 stars_lookup_beam_init(StarsU16);
StarsI16 stars_lookup_beam_range(StarsU16);
StarsI32 stars_f_attack_complex_target_score(const StarsAttackSection *,
    const StarsAttackToken *);

typedef struct StarsCoolBattleFleet {
    StarsI16 id;
    StarsI16 owner;
    StarsI16 planet_id;
    StarsU16 active;
    StarsU16 ship_count;
    StarsPoint point;
} StarsCoolBattleFleet;
typedef struct StarsCoolBattleToken {
    StarsI16 id;
    StarsI16 player;
    StarsI16 design;
    StarsU16 active;
    StarsU16 count;
    StarsU8 object_kind;
    StarsU8 battle_coordinate;
    StarsU8 target_token;
    StarsU8 pct_cloak;
    StarsU8 pct_jam;
    StarsU8 pct_battle_crit;
    StarsU8 pct_capture;
    StarsU8 pct_beam_def;
    StarsU16 weight;
    StarsU16 defense_value;
    StarsU16 movement_packed;
    /* The low bit is the raw active-token bit.  Bits 5..8 are the
     * movement/log state carried into a six-byte battle record. */
    StarsU16 movement_flags;
    StarsU8 init_min;
    StarsU8 init_max;
    StarsU8 init_base;
    StarsU8 warp;
    StarsU16 dp_shield;
    StarsU16 target_flags;
} StarsCoolBattleToken;
#define STARS_COOL_BATTLE_LOG_BYTES 0xffc8U
#define STARS_COOL_BATTLE_ALLOC_TYPE 0x000b
typedef StarsI16 (*StarsCoolBattleAllocate)(void *, StarsU16, StarsI16);
typedef StarsI16 (*StarsCoolBattleInitialize)(void *,
    const StarsCoolBattleFleet *, StarsI16, StarsI16, StarsU16,
    StarsCoolBattleToken *, StarsI16, StarsI16 *, StarsI16 *,
    StarsI16 *, StarsI16 *);
typedef StarsI16 (*StarsCoolBattleRound)(void *, StarsI16, StarsI16);
typedef StarsI16 (*StarsCoolBattleTokenRound)(void *, StarsI16,
    StarsCoolBattleToken *);
typedef StarsI16 (*StarsCoolBattleTokenLoad)(void *, StarsI16,
    StarsCoolBattleToken *);
typedef StarsI16 (*StarsCoolBattleTokenStore)(void *, StarsI16,
    const StarsCoolBattleToken *);
typedef StarsI16 (*StarsCoolBattleMoveRange)(void *, StarsU16, StarsU8 *);
typedef StarsI32 (*StarsCoolBattleMoveScore)(void *, StarsU16, StarsI16,
    StarsU16);
typedef StarsI16 (*StarsCoolBattleMoveRandom)(void *, StarsI16);
typedef StarsI16 (*StarsCoolBattleMoveSameSquare)(void *, StarsU16, StarsI16);
typedef StarsI16 (*StarsCoolBattleInitiativeAvailable)(void *, StarsI16);
typedef StarsI16 (*StarsCoolBattleDefendingStarbase)(void *, StarsI16,
    StarsU16);
typedef StarsI16 (*StarsCoolBattleInitiative)(void *, StarsI16, StarsI16,
    StarsI16, StarsU16);
typedef StarsI16 (*StarsCoolBattleTokenInitiative)(void *, StarsI16, StarsI16,
    StarsI16, StarsCoolBattleToken *, StarsU16);
typedef struct StarsCoolBattleHeader {
    StarsI16 battle_id;
    StarsI16 player_count;
    StarsI16 token_count;
    StarsI16 planet_id;
    StarsPoint point;
    StarsU16 player_mask;
    StarsU16 record_bytes;
} StarsCoolBattleHeader;
typedef StarsI16 (*StarsCoolBattleWriteHeader)(void *,
    const StarsCoolBattleHeader *);
typedef struct StarsCoolBattleEvent {
    StarsI16 kind;
    StarsI16 round;
    StarsI16 initiative;
    StarsI16 token_index;
    /* Semantic view of the record fields; a log writer may serialize these
     * into the original six-byte battle-record layout. */
    StarsU8 battle_coordinate;
    StarsU8 movement_state;
} StarsCoolBattleEvent;
typedef StarsI16 (*StarsCoolBattleWriteEvent)(void *,
    const StarsCoolBattleEvent *);
typedef StarsU16 (*StarsCoolBattleAdvance)(void *, StarsI16, StarsU16);
typedef StarsI16 (*StarsCoolBattleReport)(void *, StarsI16);
typedef struct StarsCoolBattleReportSummary {
    StarsI16 battle_id;
    StarsI16 player_count;
    StarsU16 player_mask;
    StarsU16 spectator_mask;
    StarsI16 active_ship_count;
    StarsU16 active_player_mask;
    StarsI16 round_count;
    StarsU16 record_bytes;
} StarsCoolBattleReportSummary;
typedef StarsI16 (*StarsCoolBattleReportEx)(void *,
    const StarsCoolBattleReportSummary *);
typedef void (*StarsCoolBattleRelease)(void *);
typedef struct StarsCoolBattleOps {
    StarsCoolBattleAllocate allocate;
    StarsCoolBattleInitialize initialize;
    StarsCoolBattleRound regen_shields;
    StarsCoolBattleRound move_tokens;
    StarsCoolBattleTokenRound regen_token;
    StarsCoolBattleTokenRound move_token;
    /* Optional packed _tok bridge.  Load occurs after initialization; store
     * is called after a round's token mutations and before reporting. */
    StarsCoolBattleTokenLoad load_token;
    StarsCoolBattleTokenStore store_token;
    /* If move_token is absent, these callbacks feed the genuine
     * DxyMoveTokTo reconstruction for each typed token. */
    StarsCoolBattleMoveRange move_range;
    StarsCoolBattleMoveScore move_score;
    StarsCoolBattleMoveRandom move_random;
    StarsCoolBattleMoveSameSquare move_same_square;
    StarsCoolBattleDefendingStarbase defending_starbase;
    StarsCoolBattleRound attack_tokens;
    StarsCoolBattleInitiativeAvailable initiative_available;
    StarsCoolBattleInitiative attack_initiative;
    StarsCoolBattleTokenInitiative attack_token;
    StarsCoolBattleWriteHeader write_header;
    StarsCoolBattleWriteEvent write_event;
    StarsCoolBattleAdvance advance_round;
    StarsCoolBattleReport send_messages;
    StarsCoolBattleReportEx send_messages_ex;
    StarsCoolBattleRelease release;
    void *context;
} StarsCoolBattleOps;
typedef struct StarsCoolBattleInput {
    const StarsCoolBattleFleet *fleets;
    StarsI16 fleet_count;
    StarsI16 player_count;
    StarsU16 player_mask;
    StarsU16 spectator_mask;
    const StarsU16 *attack_masks;
    StarsCoolBattleToken *tokens;
    StarsI16 token_capacity;
    StarsCoolBattleOps ops;
    StarsI16 token_count;
    StarsI16 round_count;
    StarsI16 initiative_min;
    StarsI16 initiative_max;
    StarsU16 surviving_players;
    StarsU16 active_player_mask;
    StarsI16 active_ship_count;
    StarsI16 battle_id;
    /* Optional production bridge for the recovered 29-byte `_tok` array. */
    StarsU8 *packed_tokens;
    StarsU16 packed_token_stride;
    StarsU16 packed_token_capacity;
    StarsInitializeBoardInput *board;
    /* Optional little-endian battle-log storage.  When present, the
     * reconstruction writes the original 14-byte header and six-byte event
     * records directly in C. */
    StarsU8 *log_buffer;
    StarsU16 log_capacity;
    StarsU16 log_used;
    StarsU16 log_battle_start;
    StarsU8 *log_battle_buffer;
    StarsU16 log_battle_capacity;
    StarsU16 log_append_start;
    StarsI16 log_overflow;
} StarsCoolBattleInput;
StarsI16 stars_f_do_cool_battle(StarsCoolBattleInput *);
/* Seed FAttack's attacker context from the token supplied to the
 * FDoCoolBattle per-token callback. Weapon sections, targets, and services
 * remain callback-owned. */
StarsI16 stars_attack_bind_cool_battle_token(StarsAttackInput *, StarsI16,
    StarsI16, StarsU16, const StarsCoolBattleToken *);
StarsI16 stars_attack_bind_raw_hull_slots(StarsAttackInput *, const StarsU8 *,
    StarsU16, StarsU16, StarsAttackPackedPartSlot *, StarsU16);
StarsI16 stars_attack_bind_cool_battle_hull(StarsAttackInput *, StarsI16,
    StarsI16, StarsU16, const StarsCoolBattleToken *, const StarsU8 *,
    StarsU16, StarsU16, StarsAttackPackedPartSlot *, StarsU16);
StarsI16 stars_attack_bind_cool_battle_design_hull(StarsAttackInput *,
    StarsI16, StarsI16, StarsU16, const StarsCoolBattleToken *,
    StarsLookupTokenHull, void *, StarsAttackPackedPartSlot *, StarsU16);
StarsI16 stars_attack_bind_cool_battle_global_hull(StarsAttackInput *,
    StarsI16, StarsI16, StarsU16, const StarsCoolBattleToken *,
    const StarsU8 *, StarsResolveFarHull, void *,
    StarsAttackPackedPartSlot *, StarsU16);

typedef struct StarsArmadaTargetFleet {
    StarsU16 id;
    StarsI16 planet_id;
    StarsPoint point;
    StarsU16 order_kind;
    StarsU16 order_target;
    StarsU8 ship_counts[16];
    StarsI32 minerals;
    StarsI32 troopers;
} StarsArmadaTargetFleet;
typedef struct StarsArmadaTargetPlanet {
    StarsU16 id;
    StarsI16 owner;
    StarsU16 starbase;
    StarsU16 turn_seen;
    StarsPoint point;
    StarsI32 minerals;
    StarsI32 population;
} StarsArmadaTargetPlanet;
typedef StarsI16 (*StarsArmadaLookupPlanet)(void *, StarsU16,
    StarsArmadaTargetPlanet *);
typedef StarsI16 (*StarsArmadaFindDestination)(void *,
    const StarsArmadaTargetPlanet *, StarsI16, StarsArmadaTargetPlanet *);
typedef StarsI16 (*StarsArmadaFindEnemyFleet)(void *,
    const StarsArmadaTargetFleet *, StarsArmadaTargetFleet *);
typedef StarsI16 (*StarsArmadaMove)(void *, StarsArmadaTargetFleet *,
    StarsU16, StarsU16, StarsPoint);
typedef void (*StarsArmadaTransfer)(void *, StarsU16, StarsI16, StarsI16);
typedef void (*StarsArmadaClearTask)(void *, StarsArmadaTargetFleet *,
    StarsI16);
typedef struct StarsIdTargetArmadaOps {
    StarsArmadaRandom random;
    StarsArmadaLookupPlanet lookup_planet;
    StarsArmadaFindDestination find_destination;
    StarsArmadaFindEnemyFleet find_enemy_fleet;
    StarsArmadaMove move;
    StarsArmadaTransfer transfer_supply;
    StarsArmadaTransfer transfer_troopers;
    StarsArmadaClearTask clear_task;
    void *context;
} StarsIdTargetArmadaOps;
StarsI16 stars_id_target_armada(StarsArmadaTargetFleet *, StarsI16,
    StarsU16, StarsU16, const StarsArmadaTargetPlanet *, StarsU16,
    const StarsIdTargetArmadaOps *);

typedef struct StarsTurinDroneShdef {
    StarsU16 active;
    StarsU16 existing;
    StarsU16 hull_id;
} StarsTurinDroneShdef;
typedef StarsI16 (*StarsTurinRandom)(void *, StarsI16);
typedef StarsI16 (*StarsTurinCreateShdef)(void *, StarsI16, StarsU16,
    StarsI16);
typedef void (*StarsTurinChangeShdef)(void *, StarsI16,
    const StarsTurinDroneShdef *);
typedef struct StarsEnsureTurinDroneShdefsInput {
    StarsTurinDroneShdef *designs;
    StarsU16 design_count;
    StarsU8 tech[6];
    StarsTurinRandom random_value;
    StarsTurinCreateShdef create_shdef;
    StarsTurinChangeShdef change_shdef;
    void *context;
} StarsEnsureTurinDroneShdefsInput;
void stars_ensure_turin_drone_shdefs(StarsEnsureTurinDroneShdefsInput *);

/* The first, table-independent half of DoTurinDroneAiTurn prepares the
 * Turin design roles and annotates planets before its live fleet/production
 * loops dispatch through the shared AI helpers. */
typedef struct StarsTurinTurnPlanet {
    StarsI16 id;
    StarsI16 owner;
    StarsU16 visibility_flags;
    StarsU8 mineral_concentration[3];
    StarsI32 current_minerals[4];
    StarsI16 desirability;
    StarsI16 production_slot_open;
    StarsI32 resources_available[4];
    StarsI32 production_queue_cost[4];
    StarsI32 hull_cost[4];
    StarsI16 production_queue_has_ship;
    StarsU16 packed_flags;
} StarsTurinTurnPlanet;
typedef struct StarsTurinTurnFleet {
    StarsI16 id;
    StarsI16 owner;
    StarsI16 planet_id;
    StarsI16 cord;
    StarsI16 order_kind;
    StarsU8 *raw_plord;
    StarsI16 ship_counts[16];
    StarsI32 current_cargo[5];
} StarsTurinTurnFleet;
/* Decode packed _planet/_fleet state and variable-length PLPROD records.
 * Production costs, desirability, and selected designs remain services. */
StarsI16 stars_turin_planet_from_packed(const StarsU8 *, StarsU16,
    StarsTurinTurnPlanet *);
StarsI16 stars_turin_fleet_from_packed(const StarsU8 *, StarsU16,
    StarsI16, StarsU8 *, StarsTurinTurnFleet *);
StarsI16 stars_turin_ship_queue_present(const StarsU8 *, StarsU16,
    StarsI16 *);
typedef struct StarsTurinFleetPolicy {
    StarsI16 total_ships;
    StarsI16 colonizer_ships;
    StarsI16 transport_ships;
    StarsI16 armed_ships;
    StarsI16 support_design_ships;
    StarsI16 attack_design_ships;
    StarsI16 needs_order_repair;
} StarsTurinFleetPolicy;
typedef StarsI16 (*StarsTurinMergeShdefs)(void *, StarsI16);
typedef StarsI16 (*StarsTurinMergeNeeded)(void *, StarsI16);
typedef StarsI16 (*StarsTurinEnsureAi)(void *, StarsI16, StarsI16 *);
typedef StarsI16 (*StarsTurinCheckDesignStatus)(void *, StarsI16, StarsI16,
    StarsI16, StarsI16 *, StarsU8[16]);
typedef StarsI16 (*StarsTurinSplitShdefs)(void *, const StarsU8[16]);
typedef StarsI16 (*StarsTurinEnsureShdefs)(void *, StarsI16);
typedef StarsI16 (*StarsTurinPlanetOpt)(void *, const StarsTurinTurnPlanet *,
    StarsI16);
typedef StarsI16 (*StarsTurinPlanetDesirability)(void *,
    const StarsTurinTurnPlanet *, StarsI16);
typedef StarsI16 (*StarsTurinPrepareProduction)(void *,
    const StarsTurinTurnPlanet *, StarsI16);
typedef StarsI16 (*StarsTurinVisitPlanet)(void *,
    const StarsTurinTurnPlanet *, StarsI16);
typedef StarsI16 (*StarsTurinVisitFleets)(void *, StarsI16);
typedef StarsI16 (*StarsTurinQueueProduction)(void *,
    const StarsTurinTurnPlanet *, StarsI16, StarsI16);
typedef StarsI16 (*StarsTurinQueueProductionFull)(void *,
    const StarsTurinTurnPlanet *, StarsI16, StarsI16, StarsI16, StarsI16);
typedef StarsI16 (*StarsTurinFinishProduction)(void *,
    const StarsTurinTurnPlanet *, StarsI16);
typedef StarsI16 (*StarsTurinPotentRobWarFleet)(void *,
    const StarsTurinTurnFleet *, StarsI16);
typedef StarsI16 (*StarsTurinHandleTasks)(void *, StarsI16, StarsI16);
typedef StarsI16 (*StarsTurinFillQueue)(void *);
typedef StarsI16 (*StarsTurinVisitFleetTyped)(void *,
    const StarsTurinTurnFleet *, const StarsTurinFleetPolicy *,
    const StarsU8[4]);
typedef StarsI16 (*StarsTurinRepairOrder)(void *,
    const StarsTurinTurnFleet *, const StarsTurinFleetPolicy *,
    const StarsU8[4]);
#define STARS_TURIN_FLEET_ACTION_NONE 0
#define STARS_TURIN_FLEET_ACTION_CLEAR_TASK 1
#define STARS_TURIN_FLEET_ACTION_MOVE 2
#define STARS_TURIN_FLEET_ACTION_COLONIZE 3
#define STARS_TURIN_FLEET_ACTION_WORMHOLE 4
#define STARS_TURIN_FLEET_ACTION_MOVE_STARBASE 5
#define STARS_TURIN_FLEET_ACTION_ATTACK 6
#define STARS_TURIN_FLEET_ACTION_TARGET_ARMADA 7
#define STARS_TURIN_FLEET_ACTION_TRANSPORT 8
#define STARS_TURIN_LOCAL_SUPPLY_COLONISTS 25
#define STARS_TURIN_FLEET_ACTION_TARGET_FREIGHTER 9
typedef struct StarsTurinFleetPlan {
    StarsI16 action;
    StarsI16 target_planet;
    StarsI16 order_kind;
    StarsI16 append;
    StarsI16 reason;
} StarsTurinFleetPlan;
StarsI16 stars_turin_plan_local_transport(
    const StarsTurinTurnFleet *, const StarsTurinTurnPlanet *, StarsI16,
    StarsTurinFleetPlan *);
typedef StarsI16 (*StarsTurinFleetPoint)(void *,
    const StarsTurinTurnFleet *, StarsPoint *);
typedef StarsI16 (*StarsTurinFleetHasTeeth)(void *,
    const StarsTurinTurnFleet *);
typedef StarsI16 (*StarsTurinIdealWarp)(void *,
    const StarsTurinTurnFleet *, StarsI16, StarsPoint);
typedef struct StarsTurinScoutAdapter {
    const StarsPoint *planet_points;
    const StarsU8 *ai_planet_records;
    StarsU32 ai_planet_records_size;
    StarsU16 planet_count;
    StarsU16 ai_planet_record_stride;
    StarsU16 unknown_status_offset;
    StarsU16 order_capacity;
    StarsU16 enemy_candidate_count;
    StarsI16 status_ready;
    StarsTurinFleetPoint fleet_point;
    StarsTurinFleetHasTeeth fleet_has_teeth;
    StarsTurinIdealWarp ideal_warp;
    StarsFindAiWormhole find_wormhole;
    StarsAiRandom random_value;
    void *context;
} StarsTurinScoutAdapter;
/* Concrete no-enemy IdTargetScout fallback used when a Turin fleet with
 * teeth selects the nearest unknown planet.  Wormhole diversions and enemy
 * candidates remain the responsibility of other route adapters. */
StarsI16 stars_turin_plan_scout_planet(void *,
    const StarsTurinTurnFleet *, const StarsTurinFleetPolicy *, StarsI16,
    StarsTurinFleetPlan *);
StarsI16 stars_turin_apply_scout_planet(void *,
    const StarsTurinTurnFleet *, const StarsTurinFleetPlan *);
typedef StarsI16 (*StarsTurinPlanFleetAction)(void *,
    const StarsTurinTurnFleet *, const StarsTurinFleetPolicy *, StarsI16,
    StarsTurinFleetPlan *);
typedef StarsI16 (*StarsTurinApplyFleetAction)(void *,
    const StarsTurinTurnFleet *, const StarsTurinFleetPlan *);
typedef StarsI16 (*StarsTurinFleetAction)(void *,
    const StarsTurinTurnFleet *, const StarsTurinFleetPlan *);
typedef struct StarsTurinDroneAiTurnOps {
    StarsTurinMergeShdefs merge_shdefs;
    StarsTurinMergeNeeded merge_needed;
    StarsTurinEnsureAi ensure_ai;
    StarsTurinCheckDesignStatus check_design_status;
    StarsTurinSplitShdefs split_shdefs;
    StarsTurinEnsureShdefs ensure_shdefs;
    StarsTurinPlanetOpt planet_opt;
    StarsTurinPlanetDesirability planet_desirability;
    StarsTurinPrepareProduction prepare_production;
    StarsTurinVisitPlanet visit_planet;
    StarsTurinVisitFleets visit_fleets;
    StarsTurinQueueProduction queue_production;
    StarsTurinQueueProductionFull queue_production_full;
    StarsTurinFinishProduction finish_production;
    StarsTurinRandom random_value;
    StarsTurinPotentRobWarFleet potent_rob_war_fleet;
    StarsTurinHandleTasks handle_tasks;
    StarsTurinFillQueue fill_queue;
    StarsTurinRepairOrder repair_order;
    StarsTurinPlanFleetAction plan_fleet_action;
    StarsTurinApplyFleetAction apply_fleet_action;
    StarsTurinFleetAction clear_task;
    StarsTurinFleetAction move;
    StarsTurinFleetAction colonize;
    StarsTurinFleetAction goto_wormhole;
    StarsTurinFleetAction move_starbase;
    StarsTurinFleetAction attack;
    StarsTurinFleetAction target_armada;
    StarsTurinFleetAction transport;
    StarsTurinFleetAction target_freighter;
    StarsTurinVisitFleetTyped visit_fleet_typed;
    void *context;
    /* Original first fleet pass, after planets and before action dispatch. */
    StarsTurinVisitFleets prepare_fleets;
} StarsTurinDroneAiTurnOps;
typedef struct StarsTurinDroneAiTurnInput {
    StarsI16 turn;
    StarsI16 player_id;
    StarsI16 player_tech_level;
    StarsI16 player_tech_level_valid;
    StarsI16 planet_count;
    StarsTurinTurnPlanet *planets;
    StarsI16 fleet_count;
    const StarsTurinTurnFleet *fleets;
    const StarsTurinDroneAiTurnOps *ops;
    StarsU8 armada_potency[4];
    /* Shared CheckAiShdefStatus output consumed by SplitOutShdefs. */
    StarsU8 split_status_mask[16];
    StarsI16 unowned_good_count;
    StarsI16 owned_bad_count;
    StarsI16 unowned_viable_count;
    StarsI16 unowned_opt_marked_count;
    StarsI16 design_selection[8];
    StarsI16 research_order;
    StarsI16 latest_starbase_design;
    StarsI16 production_queued;
    StarsI16 production_hulls_queued;
    /* When valid, the four raw five-item design loops use these selected
     * design table counts and per-design GetTrueHullCost results. Roles are
     * indexed like design_selection: 0=(6..7), 3=(4..5), 5=15, 7=(10..11). */
    StarsI16 production_designs_valid;
    StarsU16 plan_max;
    StarsU32 design_existing[8];
    StarsI32 design_hull_cost[8][4];
    StarsI16 design_15_inactive;
    StarsI16 base_design_inactive;
    StarsI16 base_design_hull_id;
    StarsU32 base_design_existing;
    StarsU32 base_design_built;
    /* Typed results of the raw AI/player tables for the group-8..9
     * production decision. The status total is CheckAiShdefStatus's
     * saturated return value, not the selected design's own count. */
    StarsI16 production_policy_valid;
    StarsI16 ai_planet_scale;
    StarsI16 player_planet_count;
    StarsI16 player_tech_2;
    StarsI16 design_8_9_existing_total;
    StarsI16 design_1_policy_valid;
    StarsU32 design_1_existing;
    StarsI16 minelayer_policy_valid;
    StarsI16 design_12_eligible;
    StarsI16 support_policy_valid;
    /* Optional live vlpbAiPlanet table: 16 bytes per planet ID. */
    StarsU8 *raw_ai_planet;
    StarsU32 raw_ai_planet_size;
    /* Viable enemy planets encountered so far in the second planet pass. */
    StarsI16 enemy_viable_count;
} StarsTurinDroneAiTurnInput;
StarsI16 stars_do_turin_drone_ai_turn(StarsTurinDroneAiTurnInput *);

typedef struct StarsISShdef {
    StarsU16 active;
    StarsU16 existing;
    StarsU16 hull_id;
} StarsISShdef;
typedef StarsI16 (*StarsISRandom)(void *, StarsI16);
typedef StarsI16 (*StarsISCreateShdef)(void *, StarsI16, StarsU16, StarsI16);
typedef void (*StarsISChangeShdef)(void *, StarsI16, const StarsISShdef *);
typedef struct StarsEnsureISShdefsInput {
    StarsISShdef *designs;
    StarsU16 design_count;
    StarsU8 tech[6];
    StarsI16 current_order;
    StarsISRandom random_value;
    StarsISCreateShdef create_shdef;
    StarsISChangeShdef change_shdef;
    void *context;
} StarsEnsureISShdefsInput;
void stars_ensure_is_shdefs(StarsEnsureISShdefsInput *);

typedef StarsI16 (*StarsMacArmadaPotency)(void *,
    const StarsArmadaTargetFleet *, StarsI16, StarsI16 *);
typedef struct StarsTargetMacArmadaOps {
    StarsArmadaRandom random;
    StarsMacArmadaPotency potency;
    StarsArmadaLookupPlanet lookup_planet;
    StarsArmadaFindDestination find_destination;
    StarsArmadaFindEnemyFleet find_enemy_fleet;
    StarsArmadaMove move;
    StarsArmadaClearTask clear_task;
    void *context;
} StarsTargetMacArmadaOps;
void stars_target_mac_armada(StarsArmadaTargetFleet *, StarsI16, StarsU16,
    StarsU16, const StarsU8 *, const StarsArmadaTargetPlanet *, StarsU16,
    const StarsTargetMacArmadaOps *);

typedef struct StarsMacFreighterFleet {
    StarsU16 id;
    StarsI16 planet_id;
    StarsPoint point;
    StarsI32 cargo[3];
    StarsI32 fleet_capacity;
} StarsMacFreighterFleet;
typedef struct StarsMacFreighterPlanet {
    StarsU16 id;
    StarsI16 owner;
    StarsPoint point;
    StarsI32 minerals[3];
    StarsI16 capacity_percent;
    StarsU8 ai_marker;
} StarsMacFreighterPlanet;
typedef StarsI32 (*StarsMacFreighterResources)(void *,
    const StarsMacFreighterPlanet *);
typedef void (*StarsMacFreighterTransfer)(void *, StarsU16, StarsI16,
    StarsI16, StarsI32);
typedef StarsI16 (*StarsMacFreighterMove)(void *, StarsMacFreighterFleet *,
    StarsU16, StarsU16, StarsPoint);
typedef struct StarsIdTargetMacFreighterOps {
    StarsMacFreighterResources resources;
    StarsMacFreighterTransfer transfer_supply;
    StarsMacFreighterMove move;
    void *context;
} StarsIdTargetMacFreighterOps;
StarsI16 stars_id_target_mac_freighter(StarsMacFreighterFleet *, StarsI16,
    StarsU16, const StarsMacFreighterPlanet *, StarsU16,
    const StarsIdTargetMacFreighterOps *);

typedef struct StarsMacintiShdef {
    StarsU16 active;
    StarsU16 existing;
    StarsI16 turn_created;
    StarsU16 hull_id;
} StarsMacintiShdef;
typedef StarsI16 (*StarsMacintiRandom)(void *, StarsI16);
typedef StarsI16 (*StarsMacintiPartAvailable)(void *, StarsU16, StarsU16);
typedef StarsI16 (*StarsMacintiCreateShdef)(void *, StarsI16, StarsU16,
    StarsI16);
typedef void (*StarsMacintiChangeShdef)(void *, StarsI16,
    const StarsMacintiShdef *);
typedef struct StarsEnsureMacintiShdefsInput {
    StarsMacintiShdef *designs;
    StarsU16 design_count;
    StarsU8 tech[6];
    StarsI16 turn;
    StarsI16 advanced_player;
    StarsMacintiRandom random_value;
    StarsMacintiPartAvailable part_available;
    StarsMacintiCreateShdef create_shdef;
    StarsMacintiChangeShdef change_shdef;
    void *context;
} StarsEnsureMacintiShdefsInput;
void stars_ensure_macinti_shdefs(StarsEnsureMacintiShdefsInput *);

typedef StarsI16 (*StarsScannerFindObject)(void *, StarsPoint, StarsU16 *);
typedef StarsI16 (*StarsScannerRandom)(void *, StarsI16);
typedef struct StarsIdGetBestScannerDestInput {
    StarsPoint source_point;
    StarsU16 map_size;
    StarsI16 warp_distance;
    const StarsMacFreighterPlanet *planets;
    StarsU16 planet_count;
    StarsScannerFindObject find_object;
    StarsScannerRandom random_value;
    void *context;
} StarsIdGetBestScannerDestInput;
StarsI16 stars_id_get_best_scanner_dest(StarsI16,
    const StarsIdGetBestScannerDestInput *);

/* The Cyber AI keeps a fixed table of design roles.  The original routine
 * passes pointers into its role/template tables to FCreateAiShdef; the
 * reconstruction exposes those pointers as stable template indices so the
 * surrounding resource layer can resolve them without carrying raw addresses
 * through the API. */
typedef struct StarsCyberAiShdef {
    StarsU16 active;
    StarsU16 existing;
    StarsI16 turn_created;
    StarsU16 hull_id;
} StarsCyberAiShdef;
typedef StarsI16 (*StarsCyberRandom)(void *, StarsI16);
typedef StarsI16 (*StarsCyberCreateShdef)(void *, StarsI16, StarsU16,
    StarsU16);
typedef void (*StarsCyberChangeShdef)(void *, StarsI16,
    const StarsCyberAiShdef *);
typedef struct StarsEnsureCyberAiShdefsInput {
    StarsCyberAiShdef *designs;
    StarsU16 design_count;
    StarsU8 tech[6];
    StarsI16 turn;
    StarsI16 ai_level;
    StarsCyberRandom random_value;
    StarsCyberCreateShdef create_shdef;
    StarsCyberChangeShdef change_shdef;
    void *context;
} StarsEnsureCyberAiShdefsInput;
void stars_ensure_cyber_ai_shdefs(StarsI16, StarsEnsureCyberAiShdefsInput *);
typedef struct StarsCyberPacketPlanet {
    StarsU16 id;
    StarsI16 owner;
    StarsU16 packet_strength;
    StarsU16 packet_flags;
    StarsU16 warp_distance;
    StarsU32 available[3];
    StarsU32 cost[3];
} StarsCyberPacketPlanet;
typedef StarsI16 (*StarsCyberPacketFindTarget)(void *,
    const StarsCyberPacketPlanet *, StarsU16 *);
typedef StarsI16 (*StarsCyberPacketQueueItem)(void *, StarsI16, StarsI16,
    StarsI16, StarsI16);
typedef void (*StarsCyberPacketSelect)(void *, StarsU16, StarsU16);
typedef struct StarsDoCyberPacketsOps {
    StarsCyberPacketFindTarget find_target;
    StarsCyberPacketQueueItem queue_item;
    StarsCyberPacketSelect select_planet;
    void *context;
} StarsDoCyberPacketsOps;
typedef struct StarsDoCyberPacketsInput {
    StarsCyberPacketPlanet *planets;
    StarsU16 planet_count;
    StarsI16 player;
    StarsU16 minimum_strength;
    StarsU16 maximum_packets;
    const StarsDoCyberPacketsOps *ops;
} StarsDoCyberPacketsInput;
void stars_do_cyber_packets(StarsDoCyberPacketsInput *);

/* Typed view of the planet fields consumed by FEnumPktAttack.  packet_word
 * is the original overlap_18 word: its low twelve bits are packet strength
 * and its high nibble is the range/age adjustment. */
typedef struct StarsPacketAttackPlanet {
    StarsU16 id;
    StarsI16 owner;
    StarsU16 status_flags;
    StarsU16 starbase_hull_class;
    StarsU16 packet_word;
} StarsPacketAttackPlanet;
typedef StarsI16 (*StarsPacketAttackRaceStat)(void *, StarsI16);
typedef StarsI16 (*StarsPacketAttackWarp)(void *,
    const StarsPacketAttackPlanet *, StarsI16 *);
typedef double (*StarsPacketAttackDistance)(void *, StarsU16, StarsU16);
typedef struct StarsFEnumPktAttackInput {
    const StarsPacketAttackPlanet *source;
    const StarsPacketAttackPlanet *test;
    StarsI16 player;
    const StarsU16 *ai_planet_flags;
    StarsU16 planet_count;
    StarsI32 ai_packet_weight;
    double warp_distance_scale;
    double gated_distance_exponent;
    double ordinary_distance_exponent;
    double score_divisor;
    StarsPacketAttackRaceStat race_stat;
    StarsPacketAttackWarp warp;
    StarsPacketAttackDistance distance;
    void *context;
} StarsFEnumPktAttackInput;
StarsI16 stars_f_enum_pkt_attack(const StarsFEnumPktAttackInput *);

typedef struct StarsTargetCyberFleet {
    StarsU16 id;
    StarsI16 planet_id;
    StarsI16 cord;
    StarsPoint point;
    StarsU16 order_kind;
    StarsU16 order_target;
    StarsPoint order_point;
    StarsU8 ship_counts[16];
} StarsTargetCyberFleet;
typedef struct StarsTargetCyberPlanet {
    StarsU16 id;
    StarsI16 owner;
    StarsU16 status_flags;
    StarsU16 turn_seen;
    StarsPoint point;
} StarsTargetCyberPlanet;
typedef StarsI16 (*StarsCyberLookupPlanet)(void *, StarsU16,
    StarsTargetCyberPlanet *);
typedef StarsI16 (*StarsCyberFindDestination)(void *,
    const StarsTargetCyberPlanet *, StarsI16, StarsTargetCyberPlanet *);
typedef StarsI16 (*StarsCyberFindEnemyFleet)(void *,
    const StarsTargetCyberFleet *, StarsTargetCyberFleet *);
typedef void (*StarsCyberMoveFleet)(void *, StarsTargetCyberFleet *,
    StarsU16, StarsU16, StarsPoint);
typedef void (*StarsCyberClearTask)(void *, StarsTargetCyberFleet *, StarsI16);
typedef void (*StarsCyberSelectObject)(void *, StarsI16, StarsU16);
typedef void (*StarsCyberMoveNearest)(void *, StarsTargetCyberFleet *,
    StarsU16);
typedef void (*StarsCyberMarkPlanet)(void *, StarsU16);
typedef struct StarsTargetCyberArmadaOps {
    StarsCyberRandom random_value;
    StarsCyberLookupPlanet lookup_planet;
    StarsCyberFindDestination find_destination;
    StarsCyberFindEnemyFleet find_enemy_fleet;
    StarsCyberMoveFleet move_fleet;
    StarsCyberClearTask clear_task;
    StarsCyberSelectObject select_object;
    StarsCyberMoveNearest move_nearest;
    StarsCyberMarkPlanet mark_planet;
    void *context;
} StarsTargetCyberArmadaOps;
void stars_target_cyber_armada(StarsTargetCyberFleet *, StarsI16,
    StarsU16, StarsI16, const StarsI16 *, const StarsTargetCyberArmadaOps *);

typedef struct StarsCyberFreighterFleet {
    StarsU16 id;
    StarsI16 planet_id;
    StarsPoint point;
    StarsI32 available_cargo;
    StarsI32 cargo_by_mineral[3];
    StarsU16 moving_flags;
} StarsCyberFreighterFleet;
typedef struct StarsCyberFreighterPlanet {
    StarsU16 id;
    StarsI16 owner;
    StarsU16 status_flags;
    StarsPoint point;
    StarsI32 mineral_total;
    /* Raw rgwtMin fields used by DoCyberFreighter's owned-planet gate. */
    StarsI16 mining_rate;
    StarsI32 mineral_stock;
} StarsCyberFreighterPlanet;
typedef struct StarsCyberFreighterPlanState {
    StarsU8 flags;
} StarsCyberFreighterPlanState;
typedef StarsI16 (*StarsCyberFreighterLookupPlanet)(void *, StarsI16,
    StarsCyberFreighterPlanet *);
typedef StarsI16 (*StarsCyberFreighterFindNearest)(void *, StarsPoint,
    StarsCyberFreighterPlanet *);
typedef StarsI16 (*StarsCyberFreighterFindTarget)(void *,
    const StarsCyberFreighterPlanet *, StarsI16,
    StarsCyberFreighterPlanet *);
typedef StarsI16 (*StarsCyberFreighterRaceStat)(void *, StarsI16);
typedef void (*StarsCyberFreighterTransfer)(void *, StarsI16, StarsI16,
    StarsI16, StarsI16, StarsI32);
typedef void (*StarsCyberFreighterRefreshFleet)(void *, StarsI16);
typedef void (*StarsCyberFreighterMoveStarbase)(void *,
    StarsCyberFreighterFleet *);
typedef StarsU16 (*StarsCyberFreighterIdealWarp)(void *,
    const StarsCyberFreighterFleet *, StarsI16);
typedef StarsI16 (*StarsCyberFreighterMove)(void *,
    StarsCyberFreighterFleet *, StarsU16, StarsU16, StarsPoint, StarsU16);
typedef struct StarsDoCyberFreighterOps {
    StarsCyberFreighterLookupPlanet lookup_planet;
    StarsCyberFreighterFindNearest find_nearest;
    StarsCyberFreighterFindTarget find_target;
    StarsCyberFreighterRaceStat race_stat;
    StarsCyberFreighterTransfer transfer_supply;
    StarsCyberFreighterRefreshFleet refresh_fleet;
    StarsCyberFreighterMoveStarbase move_to_starbase;
    StarsCyberFreighterIdealWarp ideal_warp;
    StarsCyberFreighterMove move_fleet;
    void *context;
} StarsDoCyberFreighterOps;
void stars_do_cyber_freighter(StarsCyberFreighterFleet *,
    StarsCyberFreighterPlanState *, StarsI16,
    const StarsDoCyberFreighterOps *);

/* The production filler consumes the four budgets produced by
 * GetResourcesAvailable after subtracting GetProdQCost.  The original keeps
 * production records as packed words; this view exposes the packed value only
 * at the boundary and gives the reconstructed policy typed names. */
#define STARS_PRODUCTION_RESOURCE_COUNT 4
typedef void (*StarsFillProdGetCosts)(void *, StarsU32, StarsI32[4]);
typedef StarsI16 (*StarsFillProdAddItem)(void *, StarsI16, StarsU16,
    StarsI16, StarsI16);
typedef struct StarsFillProdMinesAndFactoriesInput {
    StarsI32 available[STARS_PRODUCTION_RESOURCE_COUNT];
    StarsI32 queued_cost[STARS_PRODUCTION_RESOURCE_COUNT];
    const StarsU32 *planet_queue_items;
    StarsU16 planet_queue_count;
    const StarsU32 *global_queue_items;
    StarsU16 global_queue_count;
    StarsI16 max_mines;
    StarsI16 mines_operating;
    StarsI16 max_factories;
    StarsI16 factories_operating;
    StarsI16 research_type;
    StarsU16 game_flags;
    StarsI16 turn;
    StarsU32 current_order;
    StarsU8 technology[6];
    StarsFillProdGetCosts get_costs;
    StarsFillProdAddItem add_item;
    void *context;
} StarsFillProdMinesAndFactoriesInput;
StarsI16 stars_fill_prod_mines_and_factories(
    StarsFillProdMinesAndFactoriesInput *);

/* Semantic view of FBuildObject.  The original routine updates packed
 * planet/fleet records and delegates allocation and notification to the
 * surrounding game.  This view keeps the policy in C while leaving those
 * storage-owning operations behind typed callbacks. */
#define STARS_BUILD_INSTALLATION_MINES 0
#define STARS_BUILD_INSTALLATION_FACTORIES 1
#define STARS_BUILD_INSTALLATION_DEFENSES 2
#define STARS_BUILD_OBJECT_PLANET 1
#define STARS_BUILD_OBJECT_FLEET 2
#define STARS_BUILD_PACKET_FIRST 6
#define STARS_BUILD_PACKET_LAST 0x11
#define STARS_BUILD_SPECIAL_FIRST 0x12
#define STARS_BUILD_SPECIAL_LAST 0x1a
#define STARS_BUILD_SPECIAL_SCANNER 0x1b

typedef struct StarsBuildObjectPlanet {
    StarsI16 id;
    StarsI16 player;
    StarsU16 flags;
    StarsI16 installations[3];
    StarsI16 environment[3];
    StarsI16 mineral_concentration[3];
    StarsI16 resource_level;
    StarsI16 starbase_hull;
    StarsI16 starbase_warp;
    StarsI16 starbase_packets;
} StarsBuildObjectPlanet;

typedef struct StarsBuildObjectPacket {
    StarsI16 player;
    StarsI16 planet_id;
    StarsI16 warp;
    StarsI16 reach;
    StarsI16 range;
    StarsI16 strength[3];
} StarsBuildObjectPacket;

typedef StarsI16 (*StarsBuildObjectCanBuild)(void *, StarsI16, StarsI16,
    StarsI16);
typedef void (*StarsBuildObjectBuildShip)(void *, StarsI16, StarsI16,
    StarsI16, StarsI16);
typedef void (*StarsBuildObjectBuildStarbase)(void *, StarsI16, StarsI16,
    StarsI16);
typedef StarsI16 (*StarsBuildObjectMaxInstallation)(void *,
    const StarsBuildObjectPlanet *, StarsI16);
typedef void (*StarsBuildObjectMessage)(void *, StarsI16, StarsI16,
    StarsI16, StarsI16, StarsI16, StarsI16, StarsI16, StarsI16, StarsI16,
    StarsI16);
typedef StarsI16 (*StarsBuildObjectRemoveMessage)(void *, StarsI16, StarsI16,
    StarsI16);
typedef StarsI16 (*StarsBuildObjectBestTerraform)(void *,
    const StarsBuildObjectPlanet *, StarsI16);
typedef StarsI16 (*StarsBuildObjectRaceStat)(void *, StarsI16, StarsI16);
typedef StarsI16 (*StarsBuildObjectWarpMachine)(void *,
    const StarsBuildObjectPlanet *, StarsI16 *);
typedef StarsI16 (*StarsBuildObjectRandom)(void *, StarsI16);
typedef StarsI16 (*StarsBuildObjectFindPacket)(void *, StarsI16, StarsI16,
    StarsBuildObjectPacket *);
typedef void (*StarsBuildObjectCreatePacket)(void *, StarsBuildObjectPacket *,
    StarsI16, StarsI16, StarsI16, StarsI16, StarsI16);
typedef void (*StarsBuildObjectUpdatePacket)(void *, StarsBuildObjectPacket *,
    StarsI16, StarsI16, StarsI16);
typedef void (*StarsBuildObjectSpecial)(void *, StarsI16, StarsI16,
    StarsI16);
typedef struct StarsBuildObjectOps {
    StarsBuildObjectCanBuild can_build;
    StarsBuildObjectBuildShip build_ship;
    StarsBuildObjectBuildStarbase build_starbase;
    StarsBuildObjectMaxInstallation max_installation;
    StarsBuildObjectMessage send_message;
    StarsBuildObjectRemoveMessage remove_message;
    StarsBuildObjectBestTerraform best_terraform;
    StarsBuildObjectRaceStat race_stat;
    StarsBuildObjectWarpMachine warp_machine;
    StarsBuildObjectRandom random_value;
    StarsBuildObjectFindPacket find_packet;
    StarsBuildObjectCreatePacket create_packet;
    StarsBuildObjectUpdatePacket update_packet;
    StarsBuildObjectSpecial build_special;
    StarsBuildObjectSpecial select_scanner;
    StarsBuildObjectSpecial kill_queued_ships;
    StarsBuildObjectSpecial kill_queued_packets;
    void *context;
} StarsBuildObjectOps;
typedef struct StarsBuildObjectInput {
    StarsBuildObjectPlanet *planet;
    StarsI16 object_group;
    StarsI16 item;
    StarsI16 quantity;
    StarsI16 player_count;
    StarsBuildObjectOps ops;
} StarsBuildObjectInput;
StarsI16 stars_build_object(StarsBuildObjectInput *);

/* Typed text-file interface for GenNewGameFromFile.  The file parser owns
 * syntax, ranges, victory encoding, duplicate-name repair, and bitmap-id
 * repair.  Race loading and world generation remain callbacks because they
 * own the surrounding game records in the original program. */
#define STARS_GAME_FILE_MAX_PLAYERS 16
#define STARS_GAME_FILE_NAME_SIZE 32
#define STARS_GAME_FILE_PATH_SIZE 80
#define STARS_GAME_FILE_BASE_SIZE 128

typedef struct StarsGameFilePlayer {
    StarsU8 race_file[STARS_GAME_FILE_PATH_SIZE];
    StarsU8 name[STARS_GAME_FILE_NAME_SIZE];
    StarsI16 computer_race;
    StarsI16 difficulty;
    StarsI16 bitmap_id;
    StarsI16 computer;
} StarsGameFilePlayer;
typedef struct StarsGameFileState {
    StarsU8 game_name[STARS_GAME_FILE_NAME_SIZE];
    StarsU8 base_name[STARS_GAME_FILE_BASE_SIZE];
    StarsI16 map_size;
    StarsI16 density;
    StarsI16 start_distance;
    StarsU32 random_seed;
    StarsU16 options;
    StarsI16 player_count;
    StarsI16 victory_enabled[10];
    StarsI16 victory_value[10];
    StarsGameFilePlayer players[STARS_GAME_FILE_MAX_PLAYERS];
} StarsGameFileState;
typedef StarsI16 (*StarsGameFileLoadRace)(void *, StarsI16, const StarsU8 *);
typedef StarsI16 (*StarsGameFileChooseComputerRace)(void *, StarsI16,
    StarsI16, StarsI16);
typedef StarsI16 (*StarsGameFileAdvantagePoints)(void *, StarsI16);
typedef void (*StarsGameFileResetPlayer)(void *, StarsI16);
typedef StarsI16 (*StarsGameFileRandom)(void *, StarsI16);
typedef void (*StarsGameFileGetName)(void *, StarsI16, StarsU8 *, StarsU16);
typedef void (*StarsGameFileSetName)(void *, StarsI16, const StarsU8 *,
    StarsU16);
typedef void (*StarsGameFileGenerateWorld)(void *, StarsGameFileState *);
typedef void (*StarsGameFileError)(void *, StarsI16);
typedef struct StarsGameFileOps {
    StarsGameFileLoadRace load_race;
    StarsGameFileChooseComputerRace choose_computer_race;
    StarsGameFileAdvantagePoints advantage_points;
    StarsGameFileResetPlayer reset_player;
    StarsGameFileRandom random_value;
    StarsGameFileGetName get_name;
    StarsGameFileSetName set_name;
    StarsGameFileGenerateWorld generate_world;
    StarsGameFileError error;
    void *context;
} StarsGameFileOps;
StarsI16 stars_gen_new_game_from_file(const StarsU8 *, StarsU16,
    StarsGameFileState *, const StarsGameFileOps *);

/* Turn-generation coordinator.  The original FGenerateTurn is mostly an
 * ordered transaction over shared global subsystems.  This state/operation
 * view makes that order explicit and keeps each subsystem's storage behind a
 * typed stage callback. */
typedef struct StarsGenerateTurnState {
    StarsI16 player_count;
    StarsI16 turn;
    StarsI16 loaded;
    StarsI16 logs_loaded;
    StarsI16 generated;
    StarsI16 output_written;
    StarsI16 interrupted;
    StarsU16 game_flags;
    StarsU8 player_order[16];
    StarsI16 player_order_count;
    /* Backup phase records X-log absence before moving files. */
    StarsU8 player_file_append[16];
} StarsGenerateTurnState;
typedef StarsI16 (*StarsGenerateTurnLoad)(void *, StarsGenerateTurnState *);
typedef StarsI16 (*StarsGenerateTurnPlayerLoad)(void *,
    StarsGenerateTurnState *, StarsI16);
typedef StarsI16 (*StarsGenerateTurnRandom)(void *, StarsI16);
typedef void (*StarsGenerateTurnStage)(void *, StarsGenerateTurnState *);
typedef void (*StarsGenerateTurnModeStage)(void *, StarsGenerateTurnState *,
    StarsI16);
typedef void (*StarsGenerateTurnProgress)(void *, StarsI16);
typedef void (*StarsGenerateTurnPlayerWrite)(void *, StarsGenerateTurnState *,
    StarsI16, StarsI16);
typedef void (*StarsGenerateTurnCleanup)(void *, StarsGenerateTurnState *);
typedef struct StarsGenerateTurnOps {
    StarsGenerateTurnLoad load_game;
    StarsGenerateTurnPlayerLoad load_player_log;
    StarsGenerateTurnStage initialize_turn;
    StarsGenerateTurnStage validate_waypoints;
    StarsGenerateTurnModeStage do_orders;
    StarsGenerateTurnModeStage move_things;
    StarsGenerateTurnStage move_fleets;
    StarsGenerateTurnStage unmark_mine_fields;
    StarsGenerateTurnStage thing_decay;
    StarsGenerateTurnStage breed_colonists;
    StarsGenerateTurnStage produce;
    StarsGenerateTurnStage fuel_fleets;
    StarsGenerateTurnStage sweep_for_mines;
    StarsGenerateTurnStage heal_ships;
    StarsGenerateTurnStage auto_terraform;
    StarsGenerateTurnStage remote_terraforming;
    StarsGenerateTurnStage spank_cheaters;
    StarsGenerateTurnStage update_guesses;
    StarsGenerateTurnStage update_scores;
    StarsGenerateTurnStage mark_backup;
    StarsGenerateTurnPlayerWrite write_player_file;
    StarsGenerateTurnPlayerWrite write_data_file;
    StarsGenerateTurnProgress progress;
    StarsGenerateTurnCleanup cleanup;
    void *context;
    StarsGenerateTurnRandom random_value;
    StarsGenerateTurnStage validate_player_logs;
    StarsGenerateTurnStage prepare_output_files;
    /* FGenerateTurn destroys the previous loaded game before host load. */
    StarsGenerateTurnStage destroy_game;
} StarsGenerateTurnOps;
StarsI16 stars_generate_turn(StarsGenerateTurnState *,
    const StarsGenerateTurnOps *);
void stars_prepare_turn_design(StarsU8 *);
StarsU16 stars_prepare_turn_fleet_flags(StarsU16, StarsU16, StarsU16);

typedef struct StarsMoveThing {
    StarsI16 kind;
    StarsI16 owner;
    StarsU16 identity;
    StarsU16 partner_identity;
    StarsI16 x;
    StarsI16 y;
    StarsI16 target_x;
    StarsI16 target_y;
    StarsI16 target_planet;
    StarsI16 warp_level;
    /* Packed wormhole destination/movement flags from _thing + 6. */
    StarsU16 packed_destination;
    StarsI16 range;
    StarsI16 strength[3];
    StarsI16 active;
} StarsMoveThing;
typedef struct StarsMovePlanet {
    StarsI16 id;
    StarsI16 owner;
    StarsI16 x;
    StarsI16 y;
    StarsI16 warp;
    StarsI16 resource_level;
    StarsI16 environment[3];
    StarsI16 mineral_concentration[3];
} StarsMovePlanet;
typedef StarsI16 (*StarsMoveThingRandom)(void *, StarsI16);
typedef StarsI16 (*StarsMoveWormholeChance)(void *, const StarsMoveThing *);
typedef StarsI16 (*StarsMoveWormholeValidate)(void *, StarsMoveThing *);
typedef StarsI16 (*StarsMoveDistance)(void *, StarsI16, StarsI16,
    StarsI16, StarsI16);
typedef StarsI16 (*StarsMovePacketDecay)(void *, StarsMoveThing *, StarsI16);
typedef StarsI16 (*StarsMoveWarpMachine)(void *, const StarsMovePlanet *,
    StarsI16 *);
typedef StarsI16 (*StarsMoveRaceStat)(void *, StarsI16, StarsI16);
typedef StarsI16 (*StarsMovePlanetSurvival)(void *, const StarsMovePlanet *);
typedef StarsI16 (*StarsMoveApplyPacket)(void *, StarsMovePlanet *,
    StarsI16, StarsI16, StarsI16, StarsI16);
typedef void (*StarsMoveMessage)(void *, StarsI16, StarsI16,
    const StarsMoveThing *, StarsI16, StarsI16);
typedef StarsMovePlanet *(*StarsMoveFindPlanet)(void *, StarsI16);
typedef void (*StarsMoveRemoveThing)(void *, StarsMoveThing *);
typedef void (*StarsMoveValidateWaypoints)(void *);
typedef struct StarsMoveThingsOps {
    StarsMoveThingRandom random_value;
    StarsMoveWormholeChance wormhole_chance;
    StarsMoveWormholeValidate validate_wormhole;
    StarsMoveDistance distance;
    StarsMovePacketDecay packet_decay;
    StarsMoveWarpMachine warp_machine;
    StarsMoveRaceStat race_stat;
    StarsMovePlanetSurvival planet_survival;
    StarsMoveApplyPacket apply_packet;
    StarsMoveMessage send_message;
    StarsMoveFindPlanet find_planet;
    StarsMoveRemoveThing remove_thing;
    StarsMoveValidateWaypoints validate_waypoints;
    void *context;
} StarsMoveThingsOps;
typedef struct StarsMoveThingsInput {
    StarsMoveThing *things;
    StarsU16 thing_count;
    StarsI16 post_production;
    StarsI16 map_size;
    const StarsMoveThingsOps *ops;
} StarsMoveThingsInput;
void stars_move_things(StarsMoveThingsInput *);

/* Typed adapter view of the fleet movement pass. */
typedef struct StarsMoveFleetOrder {
    StarsI16 kind;
    StarsI16 target_kind;
    StarsI16 target_id;
    StarsI16 target_x;
    StarsI16 target_y;
    StarsI16 warp_level;
    StarsI16 colonist_cargo;
    StarsI32 cargo[4];
} StarsMoveFleetOrder;
typedef struct StarsMoveFleet {
    StarsI16 id;
    StarsI16 owner;
    StarsI16 x;
    StarsI16 y;
    StarsI16 target_x;
    StarsI16 target_y;
    StarsI16 target_id;
    StarsI16 active;
    StarsI16 status;
    StarsI16 moving;
    StarsI16 warp_level;
    StarsI16 has_ram_scoop;
    StarsI16 gate_allowed;
    StarsI16 auto_track;
    StarsI32 fuel;
    StarsI32 fuel_capacity;
    StarsI32 move_left;
} StarsMoveFleet;
typedef StarsI16 (*StarsMoveFleetRandom)(void *, StarsI16);
typedef StarsI16 (*StarsMoveFleetGetOrder)(void *, const StarsMoveFleet *,
    StarsMoveFleetOrder *);
typedef StarsI32 (*StarsMoveFleetDistance)(void *, StarsI16, StarsI16,
    StarsI16, StarsI16);
typedef StarsI32 (*StarsMoveFleetMovementLimit)(void *,
    const StarsMoveFleet *, const StarsMoveFleetOrder *, StarsI32,
    StarsI16);
typedef StarsI32 (*StarsMoveFleetFuelUse)(void *, const StarsMoveFleet *,
    const StarsMoveFleetOrder *, StarsI32);
typedef StarsI32 (*StarsMoveFleetFuelFree)(void *, const StarsMoveFleet *);
typedef StarsI32 (*StarsMoveFleetFuelGain)(void *, const StarsMoveFleet *,
    StarsI32);
typedef void (*StarsMoveFleetCargo)(void *, StarsMoveFleet *, StarsI16,
    StarsI32);
typedef StarsI16 (*StarsMoveFleetCanGate)(void *, const StarsMoveFleet *);
typedef StarsI16 (*StarsMoveFleetGateJump)(void *, StarsMoveFleet *,
    StarsI16, StarsI16, StarsI32);
typedef StarsI16 (*StarsMoveFleetMines)(void *, StarsMoveFleet *, StarsI32 *);
typedef void (*StarsMoveFleetTransfer)(void *, StarsMoveFleet *,
    StarsMoveFleet *);
typedef StarsI16 (*StarsMoveFleetRaceStat)(void *, StarsI16, StarsI16);
typedef StarsI16 (*StarsMoveFleetRaceGrbit)(void *, StarsI16, StarsI16);
typedef StarsI16 (*StarsMoveFleetFindPlanet)(void *, StarsI16, StarsI16);
typedef StarsI16 (*StarsMoveFleetNearest)(void *, StarsI16, StarsI16);
typedef void (*StarsMoveFleetMessage)(void *, StarsI16, StarsI16,
    StarsI16, StarsI16, StarsI16, StarsI16);
typedef void (*StarsMoveFleetNoTrack)(void *, StarsMoveFleet *);
typedef void (*StarsMoveFleetKillWaypoints)(void *);
typedef struct StarsMoveFleetsOps {
    StarsMoveFleetRandom random_value;
    StarsMoveFleetGetOrder get_order;
    StarsMoveFleetDistance distance;
    StarsMoveFleetMovementLimit movement_limit;
    StarsMoveFleetFuelUse fuel_use;
    StarsMoveFleetFuelFree fuel_free;
    StarsMoveFleetFuelGain fuel_gain;
    StarsMoveFleetCargo change_cargo;
    StarsMoveFleetCanGate can_jumpgate;
    StarsMoveFleetGateJump stargate_jump;
    StarsMoveFleetMines travel_mines;
    StarsMoveFleetTransfer transfer_cargo;
    StarsMoveFleetRaceStat race_stat;
    StarsMoveFleetRaceGrbit race_grbit;
    StarsMoveFleetFindPlanet find_planet;
    StarsMoveFleetNearest nearest_object;
    StarsMoveFleetMessage send_message;
    StarsMoveFleetNoTrack no_auto_track;
    StarsMoveFleetKillWaypoints kill_waypoints;
    void *context;
} StarsMoveFleetsOps;
typedef struct StarsMoveFleetsInput {
    StarsMoveFleet *fleets;
    StarsU16 fleet_count;
    StarsI16 post_movement;
    const StarsMoveFleetsOps *ops;
} StarsMoveFleetsInput;
void stars_move_fleets(StarsMoveFleetsInput *);

/* Typed resource-transfer slice of SatisfyOrders (NE 23:6798).  The original
 * routine walks the packed PLORD queue and services the same five resource
 * channels for fleets, planets, and salvage/thing destinations.  Keep the
 * storage and object lookup outside this adapter; those records are owned by
 * the turn engine. */
#define STARS_SATISFY_RESOURCE_MINERAL_0 0
#define STARS_SATISFY_RESOURCE_MINERAL_1 1
#define STARS_SATISFY_RESOURCE_MINERAL_2 2
#define STARS_SATISFY_RESOURCE_COLONISTS 3
#define STARS_SATISFY_RESOURCE_FUEL 4
#define STARS_SATISFY_RESOURCE_COUNT 5
#define STARS_SATISFY_ORDER_TRANSFER 2
#define STARS_SATISFY_ORDER_MINE 3
#define STARS_SATISFY_ORDER_MERGE 4
/* Original task 5 enters the fleet-scrap/transfer branch at 10b0:~8207.
 * The packed scrap_state path covers SO-E1's planet-accounting case. The
 * battle_plan callback below is a legacy partial-adapter seam and does not
 * reconstruct the full branch; SO-E3's complete full-turn comparison remains
 * open. */
#define STARS_SATISFY_ORDER_SCRAP 5
#define STARS_SATISFY_ORDER_BATTLE_PLAN STARS_SATISFY_ORDER_SCRAP
#define STARS_SATISFY_ORDER_LAY_MINES 6
#define STARS_SATISFY_ORDER_AUTO_ROUTE 8
#define STARS_SATISFY_ORDER_TRANSFER_PLAYER 9
#define STARS_SATISFY_FLEET_CAN_MINE 0x2000U
#define STARS_SATISFY_TARGET_PLANET 1
#define STARS_SATISFY_TARGET_FLEET 2
#define STARS_SATISFY_TARGET_POINT 4
#define STARS_SATISFY_TARGET_THING 8
#define STARS_SATISFY_RESOURCE_MODE_STAT_PERCENT 5
#define STARS_SATISFY_RESOURCE_MODE_STAT_PERCENT_ALT 6
#define STARS_SATISFY_MESSAGE_TRANSFER 0x2d
#define STARS_SATISFY_MESSAGE_TRANSFER_COLONISTS 0x2e
#define STARS_SATISFY_MESSAGE_FLEET_TRANSFER 0x2b
#define STARS_SATISFY_MESSAGE_FLEET_TRANSFER_COLONISTS 0x2c
#define STARS_SATISFY_MESSAGE_FOREIGN_FLEET_COLONISTS 0x155
#define STARS_SATISFY_MESSAGE_SHORTAGE 0x11f
#define STARS_SATISFY_MESSAGE_FLEET_SHORTAGE 0x120
#define STARS_SATISFY_MESSAGE_POINT_SHORTAGE 0x123
#define STARS_SATISFY_MESSAGE_COLONISTS_NO_UNOWNED_DROP 0x55
#define STARS_SATISFY_MESSAGE_COLONISTS_RACE_BLOCKED 0x56
#define STARS_SATISFY_MESSAGE_COLONISTS_PLANET_BLOCKED 0x135
#define STARS_SATISFY_MESSAGE_MINE_NO_PLANET 0x77
#define STARS_SATISFY_MESSAGE_MINE_EMPTY 0x75
#define STARS_SATISFY_MESSAGE_MINE_BLOCKED 0x76
#define STARS_SATISFY_MINE_NO_PLANET (-1)
#define STARS_SATISFY_MINE_NO_SOURCE 0
#define STARS_SATISFY_MINE_HOSTILE_PLANET 1
#define STARS_SATISFY_MINE_SUCCESS 2
#define STARS_SATISFY_MESSAGE_MERGE_OK 0xf7
#define STARS_SATISFY_MESSAGE_MERGE_ENEMY 0xf6
#define STARS_SATISFY_MESSAGE_MERGE_INVALID 0xf5
#define STARS_SATISFY_MESSAGE_LAY_MINES 0xc3
#define STARS_SATISFY_MESSAGE_LAY_MINES_EMPTY 0xbf
#define STARS_SATISFY_MESSAGE_AUTO_ROUTE 0x127
#define STARS_SATISFY_MESSAGE_AUTO_ROUTE_NO_PATH 0x128
#define STARS_SATISFY_MESSAGE_TRANSFER_PLAYER_INVALID 0x148
#define STARS_SATISFY_MESSAGE_TRANSFER_PLAYER_NO_SHIP 0x149
#define STARS_SATISFY_MESSAGE_TRANSFER_PLAYER_NO_DESIGN 0x14a
#define STARS_SATISFY_MESSAGE_TRANSFER_PLAYER_BLOCKED 0x14c
#define STARS_SATISFY_MESSAGE_TRANSFER_PLAYER_CREATED 0x14d
#define STARS_SATISFY_MESSAGE_TRANSFER_PLAYER_RECEIVED 0x14e
#define STARS_SATISFY_MESSAGE_FUEL_AVAILABLE 0x3c
#define STARS_SATISFY_MESSAGE_FUEL_CAPACITY 0x3d
#define STARS_SATISFY_MESSAGE_FUEL_FAILED_LOAD 0x126
#define STARS_SATISFY_MESSAGE_TRANSFER_FUTILE 0x11e
#define STARS_SATISFY_MESSAGE_SET_AMOUNT 0x121
#define STARS_SATISFY_MESSAGE_SET_COLONISTS 0x122
#define STARS_SATISFY_MESSAGE_COLONISTS_DEEP_SPACE 0x165
#define STARS_SATISFY_FUEL_MESSAGE_NONE 0
#define STARS_SATISFY_FUEL_MESSAGE_FAILED_LOAD 1
#define STARS_SATISFY_FUEL_MESSAGE_CAPACITY 2
#define STARS_SATISFY_FUEL_MESSAGE_AVAILABLE 3
#define STARS_SATISFY_FAILURE_SET_AMOUNT 1
#define STARS_SATISFY_FAILURE_SET_COLONISTS 2
#define STARS_SATISFY_FAILURE_COLONISTS_DEEP_SPACE 3
#define STARS_SATISFY_FAILURE_TRANSFER_FUTILE 4
#define STARS_SATISFY_PLAYER_TRANSFER_INVALID_TARGET 0
#define STARS_SATISFY_PLAYER_TRANSFER_NO_SHIP 1
#define STARS_SATISFY_PLAYER_TRANSFER_NO_DESIGN 2
#define STARS_SATISFY_PLAYER_TRANSFER_CREATED 3
#define STARS_SATISFY_PLAYER_TRANSFER_BLOCKED 4
typedef struct StarsSatisfyOrder {
    StarsI16 kind;
    StarsI16 phase_mask;
    StarsI16 target_kind;
    StarsI16 target_id;
    StarsI16 source_cord;
    StarsI16 target_owner;
    StarsI16 target_starbase_flags;
    /* Nonzero selects raw auto-route success message 0x127; zero selects
     * the alternate/no-path message 0x128. */
    StarsI16 auto_route_message_variant;
    StarsI16 target_present;
    StarsI16 target_inactive;
    /* Typed planet state used by the enemy-colonist destination branch. */
    StarsU16 target_planet_flags;
    StarsI16 colonist_race_stat;
    StarsI16 repeat_count;
    /* Set when the original resource-resolution branch also performs its
     * player technology-discovery side effect. */
    StarsI16 technology_flag;
    /* The player-transfer message carries the newly-created fleet's raw
     * identity separately from the logical order target.  The caller fills
     * this after allocation because the original obtains it from LpflNew. */
    StarsI16 message_target_id;
    StarsU16 message_target_word;
    /* Caller-resolved x coordinate used by ordinary FSendPlrMsg transfer
     * frames; the reduced order record does not own waypoint coordinates. */
    StarsI16 message_target_x;
    /* The packed PLORD resource nibble.  Modes 5 and 6 derive the requested
     * amount from LGetFleetStat during the even (pass 2/4) passes. */
    StarsU8 resource_mode[STARS_SATISFY_RESOURCE_COUNT];
    StarsI32 requested[STARS_SATISFY_RESOURCE_COUNT];
    StarsI16 active;
} StarsSatisfyOrder;
typedef struct StarsSatisfyFleet {
    StarsI16 id;
    StarsI16 owner;
    /* Raw idPlanet used by the auto-route message payload. */
    StarsI16 planet_id;
    StarsU16 flags;
    StarsU16 turn_flags;
    StarsI32 cargo[STARS_SATISFY_RESOURCE_COUNT];
    StarsI32 capacity[STARS_SATISFY_RESOURCE_COUNT];
    /* Pre-resolved WFromLpfl value used by raw FSendPlrMsg2 layouts. */
    StarsU16 message_word;
    StarsSatisfyOrder *orders;
    StarsU16 order_count;
} StarsSatisfyFleet;
typedef StarsI32 (*StarsSatisfyAvailable)(void *,
    const StarsSatisfyFleet *, StarsI16);
typedef StarsI32 (*StarsSatisfyFleetStat)(void *, const StarsSatisfyFleet *,
    StarsI16);
typedef StarsI32 (*StarsSatisfyTargetCapacity)(void *,
    const StarsSatisfyOrder *, StarsI16);
typedef StarsI32 (*StarsSatisfyTransfer)(void *, const StarsSatisfyFleet *,
    const StarsSatisfyOrder *, StarsI16, StarsI32);
typedef StarsI16 (*StarsSatisfyMine)(void *, const StarsSatisfyFleet *,
    const StarsSatisfyOrder *);
typedef StarsI16 (*StarsSatisfyMerge)(void *, StarsSatisfyFleet *,
    const StarsSatisfyOrder *);
typedef StarsI16 (*StarsSatisfyBattlePlan)(void *, StarsSatisfyFleet *,
    const StarsSatisfyOrder *);
/* A successful scrap service has applied the original mineral, planet,
 * technology, and message effects. The adapter then retires the fleet. */
typedef StarsI16 (*StarsSatisfyScrap)(void *, StarsSatisfyFleet *,
    const StarsSatisfyOrder *);
typedef StarsI16 (*StarsSatisfyRoute)(void *, StarsSatisfyFleet *,
    const StarsSatisfyOrder *);
typedef StarsI16 (*StarsSatisfyPlayerTransfer)(void *, StarsSatisfyFleet *,
    const StarsSatisfyOrder *);
typedef void (*StarsSatisfyPlayerTransferMessage)(void *, StarsI16,
    StarsI16, const StarsSatisfyFleet *, const StarsSatisfyOrder *);
typedef StarsI16 (*StarsSatisfyLayMines)(void *, StarsSatisfyFleet *,
    const StarsSatisfyOrder *);
typedef void (*StarsSatisfyColonistDrop)(void *, const StarsSatisfyFleet *,
    const StarsSatisfyOrder *, StarsI32);
typedef void (*StarsSatisfyMessage)(void *, StarsI16, StarsI16, StarsI16,
    StarsI16, StarsI32);
typedef void (*StarsSatisfyOrderDone)(void *, StarsSatisfyFleet *,
    StarsSatisfyOrder *, StarsI16);
/* The original routine performs these side effects after the order-local
 * accounting.  Keep them as explicit service boundaries so the packed
 * adapter can be compared without inventing storage for the full game
 * globals. */
typedef struct StarsSatisfyTechnologyOutcome {
    StarsI16 result;
    StarsI16 message_code;
    StarsI16 result_id;
    StarsI32 amount;
    /* The turn caller resolves the full FSendPlrMsg frame from its live
     * planet/design state.  Preserve that recipient and eight-word frame
     * when a technology result is emitted, rather than guessing from the
     * reduced resource-order view. */
    StarsI16 raw_recipient;
    StarsI16 raw_payload[8];
} StarsSatisfyTechnologyOutcome;
typedef void (*StarsSatisfyTechnology)(void *, StarsSatisfyFleet *,
    const StarsSatisfyOrder *, StarsI32);
typedef StarsI16 (*StarsSatisfyTechnologyBinding)(void *, StarsSatisfyFleet *,
    const StarsSatisfyOrder *, StarsI32, StarsI16, StarsI16,
    StarsSatisfyTechnologyOutcome *);
typedef void (*StarsSatisfyShipCopy)(void *, StarsSatisfyFleet *,
    const StarsSatisfyOrder *, StarsI16);
typedef void (*StarsSatisfyDesignMaterialize)(void *, StarsSatisfyFleet *,
    const StarsSatisfyOrder *, StarsI16);
typedef StarsI16 (*StarsSatisfyFleetCopy)(void *, StarsSatisfyFleet *,
    const StarsSatisfyOrder *, StarsI16);
typedef void (*StarsSatisfySalvage)(void *, StarsSatisfyFleet *,
    const StarsSatisfyOrder *, const StarsI32[STARS_SATISFY_RESOURCE_COUNT]);
typedef void (*StarsSatisfyMessageLayout)(void *, StarsI16, StarsI16,
    StarsI16, StarsI16, StarsI32, StarsI16);
/* Exact three-payload form used by FSendPlrMsg2.  Keeping this separate from
 * the generic layout trace prevents a lossy adapter from being mistaken for
 * the original message ABI. */
typedef void (*StarsSatisfyRawMessage2)(void *, StarsI16, StarsI16,
    StarsI16, StarsI16, StarsI16);
typedef void (*StarsSatisfyRawMessage)(void *, StarsI16, StarsI16,
    const StarsI16[8]);
/* Task-5 services run after mineral accounting, in original call order.
 * The salvage callback creates a thing and returns its original object ID. */
typedef StarsI16 (*StarsSatisfyScrapSalvage)(void *, const StarsU8 *,
    const StarsU32[3]);
typedef StarsI16 (*StarsSatisfyScrapLearn)(void *, StarsI16,
    StarsU16 *);
typedef void (*StarsSatisfyScrapRemoveMessage)(void *, StarsI16,
    StarsI16, StarsU16);
typedef void (*StarsSatisfyScrapMarkTechs)(void *, const StarsU8 *,
    StarsI16);
typedef StarsI16 (*StarsSatisfyScrapReprice)(void *, const StarsU8 *,
    StarsU8 *);
typedef StarsU16 (*StarsSatisfyScrapPlanetResources)(void *, StarsI16,
    StarsU16);
/* Caller-resolved fuel state used by the route engine's raw FSendPlrMsg
 * branches. The reduced order record does not own waypoint fuel demand or
 * LGetFleetStat results, so this boundary preserves those values explicitly. */
typedef struct StarsSatisfyFuelFrame {
    StarsI16 variant;
    StarsI16 recipient;
    StarsI16 fleet_id;
    StarsI16 target_word;
    StarsI16 target_x;
    StarsI16 target_id;
    StarsI32 fleet_fuel;
    StarsI32 fuel_stat;
    StarsI32 required_fuel;
    StarsI32 optimal_delta;
} StarsSatisfyFuelFrame;
void stars_satisfy_raw_fuel_message(const StarsSatisfyFuelFrame *,
    StarsSatisfyRawMessage, void *);
/* Caller-resolved failure state used by SatisfyOrders branches whose packed
 * order/action union is not part of the reduced typed order view. */
typedef struct StarsSatisfyFailureFrame {
    StarsI16 variant;
    StarsI16 recipient;
    StarsI16 fleet_id;
    StarsI16 resource;
    StarsI16 requested;
    StarsI16 target_word;
    StarsI16 target_x;
    StarsI16 thing_kind;
} StarsSatisfyFailureFrame;
void stars_satisfy_raw_failure_message(const StarsSatisfyFailureFrame *,
    StarsSatisfyRawMessage, StarsSatisfyRawMessage2, void *);
typedef struct StarsSatisfyOrdersOps {
    StarsSatisfyAvailable available;
    StarsSatisfyFleetStat fleet_stat;
    StarsSatisfyTargetCapacity target_capacity;
    StarsSatisfyTransfer transfer;
    StarsSatisfyMine mine;
    StarsSatisfyMerge merge;
    StarsSatisfyBattlePlan battle_plan;
    StarsSatisfyScrap scrap;
    StarsSatisfyRoute auto_route;
    StarsSatisfyRoute auto_route_at_starbase;
    StarsSatisfyRoute auto_fleet_order;
    StarsSatisfyPlayerTransfer transfer_player;
    StarsSatisfyPlayerTransferMessage transfer_player_message;
    StarsSatisfyLayMines lay_mines;
    StarsSatisfyColonistDrop queue_colonist_drop;
    StarsSatisfyMessage send_message;
    StarsSatisfyOrderDone order_done;
    StarsSatisfyTechnology technology_discovery;
    StarsSatisfyTechnologyBinding technology_binding;
    StarsSatisfyShipCopy ship_copy;
    StarsSatisfyDesignMaterialize materialize_designs;
    StarsSatisfyFleetCopy copy_fleet_state;
    StarsSatisfySalvage create_salvage;
    StarsSatisfyMessageLayout message_layout;
    StarsSatisfyRawMessage2 raw_message2;
    StarsSatisfyRawMessage raw_message;
    StarsSatisfyScrapSalvage scrap_salvage;
    StarsSatisfyScrapLearn scrap_learn;
    StarsSatisfyScrapRemoveMessage scrap_remove_message;
    StarsSatisfyScrapMarkTechs scrap_mark_techs;
    StarsSatisfyScrapReprice scrap_reprice;
    StarsSatisfyScrapPlanetResources scrap_planet_resources;
    void *context;
} StarsSatisfyOrdersOps;
StarsI16 stars_bind_satisfy_technology(StarsTechLearnInput *, StarsI16,
    StarsI16, StarsSatisfyTechnologyOutcome *);
typedef struct StarsSatisfyOrdersInput {
    StarsSatisfyFleet *fleets;
    StarsU16 fleet_count;
    StarsI16 pass;
    StarsI16 lay_mines_race_stat;
    const StarsSatisfyOrdersOps *ops;
} StarsSatisfyOrdersInput;
void stars_satisfy_orders(StarsSatisfyOrdersInput *);
/* Packed turn-engine bridge for the recovered 124-byte _fleet and 18-byte
 * _order records.  The resource vectors and destination facts are supplied
 * by the caller's live turn state; the bridge owns the raw identity, order
 * kind/target-kind, target id, validity bit, and pass-1 turn-flag mapping. */
typedef struct StarsSatisfyPackedInput {
    StarsU8 *fleet_record;
    StarsU16 fleet_size;
    StarsU8 *order_record;
    StarsU16 order_size;
    StarsI32 cargo[STARS_SATISFY_RESOURCE_COUNT];
    StarsI32 capacity[STARS_SATISFY_RESOURCE_COUNT];
    StarsI32 requested[STARS_SATISFY_RESOURCE_COUNT];
    StarsU8 resource_mode[STARS_SATISFY_RESOURCE_COUNT];
    StarsI16 phase_mask;
    StarsI16 pass;
    StarsI16 lay_mines_race_stat;
    StarsI16 target_owner;
    StarsI16 planet_id;
    StarsI16 target_present;
    StarsI16 target_inactive;
    StarsI16 target_starbase_flags;
    StarsI16 source_cord;
    StarsI16 auto_route_message_variant;
    StarsU16 target_planet_flags;
    StarsI16 colonist_race_stat;
    StarsI16 repeat_count;
    StarsI16 technology_flag;
    /* Opt-in raw task-5 scrap inputs. Each referenced design is the
     * original 147-byte SHDEF, with costs at 0x2a/0x2c and gift flag at
     * 0x7b. An orbiting fleet uses the original 60-byte planet record;
     * deep-space scrap requires the scrap_salvage and raw_message2 services. */
    struct StarsSatisfyScrapPackedState *scrap_state;
    const StarsSatisfyOrdersOps *ops;
} StarsSatisfyPackedInput;
typedef struct StarsSatisfyScrapPackedState {
    StarsU8 *planet_record;
    StarsU16 planet_size;
    const StarsU8 *design_records[16];
    StarsU16 design_size;
    StarsI16 fleet_race_reprice;
    StarsI16 planet_race_scrap_bonus;
    StarsU16 *planet_resource_extra;
    /* Live global grBits2 word. Task 5 sets bit 3 while it reprices owner
     * designs, then clears it before posting the result messages. */
    StarsU16 *game_flags;
} StarsSatisfyScrapPackedState;
StarsI16 stars_satisfy_orders_packed(StarsSatisfyPackedInput *);

/* Typed report-row view of DumpPlanets (NE 34:86e4).  Text formatting and
 * stream ownership stay with the report host; this layer performs the same
 * visibility and derived-field selection over named planet state. */
#define STARS_DUMP_PLANET_FLAG_INHABITED 7U
#define STARS_DUMP_PLANET_FLAG_SCANNED 0x0200U
#define STARS_DUMP_PLANET_HEADER_COUNT 2
#define STARS_DUMP_PLANET_HEADER_COUNT_DETAILED 3
typedef struct StarsDumpPlanet {
    StarsI16 id;
    StarsI16 owner;
    StarsU16 flags;
    StarsI16 turn;
    StarsI16 environment[3];
    StarsI16 environment_original[3];
    StarsI32 mineral_concentration[3];
    StarsI32 mineral_rate[3];
    StarsI32 population;
    StarsI32 production;
    StarsI32 survival_percent;
    StarsI32 resources;
    StarsI16 scanner_range;
    StarsI16 scanner_detail;
    StarsI16 starbase_id;
    StarsI16 starbase_level;
    StarsI16 wormhole_id;
    StarsI16 wormhole_distance;
    StarsI16 mine_count;
} StarsDumpPlanet;
typedef struct StarsDumpPlanetRow {
    StarsI16 id;
    StarsI16 owner;
    StarsI16 owner_visible;
    StarsI16 turns_since_visit;
    StarsU16 flags;
    StarsI32 population;
    StarsI32 production;
    StarsI32 survival_percent;
    StarsI32 resources;
    StarsI32 mineral_concentration[3];
    StarsI32 mineral_rate[3];
    StarsI16 environment[3];
    StarsI16 environment_original[3];
    StarsI16 scanner_range;
    StarsI16 scanner_detail;
    StarsI16 starbase_id;
    StarsI16 starbase_level;
    StarsI16 wormhole_id;
    StarsI16 wormhole_distance;
    StarsI16 mine_count;
} StarsDumpPlanetRow;
typedef StarsI16 (*StarsDumpPlanetOpen)(void *, const char *, StarsI16);
typedef StarsI16 (*StarsDumpPlanetHeader)(void *, StarsI16);
typedef StarsI16 (*StarsDumpPlanetRowWriter)(void *,
    const StarsDumpPlanetRow *);
/* The normal original export has twenty localized tab-separated fields.
 * Names, queue text, percentages, and number formatting remain report-host
 * services; the coordinator owns their recovered order and row boundary. */
#define STARS_DUMP_PLANET_TEXT_FIELD_COUNT 20
typedef StarsI16 (*StarsDumpPlanetTextField)(void *, StarsI16,
    const StarsDumpPlanetRow *);
typedef StarsI16 (*StarsDumpPlanetFormatText)(void *, StarsI16,
    const StarsDumpPlanetRow *, StarsU8 *, StarsU16);
typedef StarsI16 (*StarsDumpPlanetWriteText)(void *, StarsI16,
    const StarsU8 *, StarsU16);
typedef StarsI16 (*StarsDumpPlanetTextEnd)(void *);
typedef void (*StarsDumpPlanetClose)(void *);
typedef struct StarsDumpPlanetsInput {
    const char *file_name;
    StarsI16 game_active;
    StarsI16 viewer_player;
    StarsI16 game_turn;
    StarsI16 detailed;
    const StarsDumpPlanet *planets;
    StarsU16 planet_count;
    StarsDumpPlanetOpen open_report;
    StarsDumpPlanetHeader write_header;
    StarsDumpPlanetRowWriter write_row;
    StarsDumpPlanetTextField write_text_field;
    StarsDumpPlanetFormatText format_text_field;
    StarsDumpPlanetWriteText write_text_value;
    StarsDumpPlanetTextEnd write_text_end;
    StarsDumpPlanetClose close_report;
    void *context;
} StarsDumpPlanetsInput;
StarsI16 stars_dump_planets(StarsDumpPlanetsInput *);

/* Values such as resources, survival, scanner detail, and report services
 * are supplied by live game tables rather than the compact _planet record.
 * The production bridge decodes the record first, then gives the caller the
 * same typed view to enrich before the ordinary report coordinator runs. */
typedef void (*StarsDumpPlanetsResolveServices)(void *, const StarsU8 *,
    StarsU16, StarsDumpPlanet *);

/* Production bridge for the recovered 56-byte _planet record.  The original
 * keeps several report-service values outside that record; the sidecar
 * fields below therefore belong to the caller's live report context.  The
 * bridge decodes one raw record, installs those service values, and dispatches
 * the ordinary typed DumpPlanets coordinator. */
#define STARS_DUMP_PLANET_RECORD_SIZE 56
typedef struct StarsDumpPlanetsPackedInput {
    const StarsU8 *planet_record;
    StarsU16 planet_size;
    StarsI32 resources;
    StarsI32 survival_percent;
    StarsI16 scanner_detail;
    StarsDumpPlanetsResolveServices resolve_services;
    StarsDumpPlanetsInput report;
} StarsDumpPlanetsPackedInput;
StarsI16 stars_dump_planets_packed(StarsDumpPlanetsPackedInput *);

/* Typed report-row view of DumpFleets (NE 34:9530). */
typedef struct StarsDumpFleet {
    StarsI16 id;
    StarsI16 owner;
    StarsI16 x;
    StarsI16 y;
    StarsI16 planet_id;
    StarsU16 flags;
    StarsI16 destination_id;
    StarsI16 destination_visible;
    StarsI16 destination_requires_detail;
    StarsI16 plan_id;
    StarsI16 ship_count[16];
    StarsI16 ship_class[16];
    StarsI32 cargo[5];
    StarsI16 player_number;
    StarsI16 eta;
    StarsI16 warp;
    StarsI32 weight;
    StarsI16 cloak_percent;
    StarsI16 scanner_range;
    StarsI16 scanner_detail;
    StarsI16 task_id;
    StarsI32 mines;
    StarsI32 mine_sweep;
    StarsI32 mine_laying;
    StarsI32 terraforming;
} StarsDumpFleet;
typedef struct StarsDumpFleetRow {
    StarsI16 id;
    StarsI16 owner;
    StarsI16 x;
    StarsI16 y;
    StarsI16 planet_id;
    StarsI16 destination_id;
    StarsI16 destination_visible;
    StarsI16 plan_id;
    StarsI32 ship_total;
    StarsI32 cargo[5];
    StarsI32 ship_other_total;
    StarsI32 ship_class_total[4];
    StarsI16 player_number;
    StarsI16 eta;
    StarsI16 warp;
    StarsI32 weight;
    StarsI16 cloak_percent;
    StarsI16 scanner_range;
    StarsI16 scanner_detail;
    StarsI16 task_id;
    StarsI32 mines;
    StarsI32 mine_sweep;
    StarsI32 mine_laying;
    StarsI32 terraforming;
} StarsDumpFleetRow;
typedef StarsI16 (*StarsDumpFleetRowWriter)(void *,
    const StarsDumpFleetRow *);
/* The original serializer emits a localized tab-separated field sequence.
 * These callbacks keep string tables/number formatting at the service edge
 * while the reconstructed coordinator owns the recovered field order. */
typedef StarsI16 (*StarsDumpFleetTextField)(void *, StarsI16,
    const StarsDumpFleetRow *);
typedef StarsI16 (*StarsDumpFleetFormatText)(void *, StarsI16,
    const StarsDumpFleetRow *, StarsU8 *, StarsU16);
typedef StarsI16 (*StarsDumpFleetWriteText)(void *, StarsI16,
    const StarsU8 *, StarsU16);
typedef StarsI16 (*StarsDumpFleetTextEnd)(void *);
typedef struct StarsDumpFleetsInput {
    const char *file_name;
    StarsI16 game_active;
    StarsI16 viewer_player;
    StarsI16 detailed;
    const StarsDumpFleet *fleets;
    StarsU16 fleet_count;
    StarsDumpPlanetOpen open_report;
    StarsDumpPlanetHeader write_header;
    StarsDumpFleetRowWriter write_row;
    StarsDumpFleetTextField write_text_field;
    StarsDumpFleetFormatText format_text_field;
    StarsDumpFleetWriteText write_text_value;
    StarsDumpFleetTextEnd write_text_end;
    StarsDumpPlanetClose close_report;
    void *context;
} StarsDumpFleetsInput;
StarsI16 stars_dump_fleets(StarsDumpFleetsInput *);

/* Route, scanner, task, minefield, and battle-plan values are resolved by
 * live report services outside the compact _fleet record.  This hook runs
 * after the production decoder and before detail-mode gating. */
typedef void (*StarsDumpFleetsResolveServices)(void *, const StarsU8 *,
    StarsU16, StarsDumpFleet *);

/* Production bridge for the recovered 124-byte _fleet record.  Route/task,
 * scanner, and mine-service values are live report state outside the compact
 * record in this narrow boundary, so the caller supplies those sidecars. */
#define STARS_DUMP_FLEET_RECORD_SIZE 124
typedef struct StarsDumpFleetsPackedInput {
    const StarsU8 *fleet_record;
    StarsU16 fleet_size;
    StarsI16 eta;
    StarsI16 cloak_percent;
    StarsI16 scanner_range;
    StarsI16 scanner_detail;
    StarsI16 destination_id;
    StarsI16 destination_visible;
    StarsI16 destination_requires_detail;
    StarsI16 plan_id;
    StarsI32 mines;
    StarsI32 mine_sweep;
    StarsI32 mine_laying;
    StarsI32 terraforming;
    StarsDumpFleetsResolveServices resolve_services;
    StarsDumpFleetsInput report;
} StarsDumpFleetsPackedInput;
StarsI16 stars_dump_fleets_packed(StarsDumpFleetsPackedInput *);

/* Early design/turn orchestration of DoMacintiAiTurn (NE 21:0008).  The
 * original owns large global design and fleet tables; callbacks below keep
 * those tables typed at their integration boundary while this adapter retains
 * the exact turn thresholds, merge-mask order, and design-status groups. */
#define STARS_MACINTI_STARTER_TURN 40
#define STARS_MACINTI_SPLIT_TURN 80
#define STARS_MACINTI_ARMADA_RAMP_TURN 130
#define STARS_MACINTI_SUPPORT_RAMP_TURN 115
#define STARS_MACINTI_ARMADA_MAX 50
#define STARS_MACINTI_SUPPORT_MAX 12
#define STARS_MACINTI_STARBASE_FLAG 0x0200U
#define STARS_MACINTI_THREAT_STEP 250U
#define STARS_MACINTI_THREAT_MAX 6
typedef StarsI16 (*StarsMacintiMergeShdefs)(void *, StarsI16);
typedef StarsI16 (*StarsMacintiCreateStarter)(void *);
typedef StarsI16 (*StarsMacintiDesignStatus)(void *, StarsI16, StarsI16,
    StarsI16, StarsI16 *, StarsU8[16]);
typedef StarsI16 (*StarsMacintiDesignStatusInfo)(void *, StarsI16,
    StarsI16 *, StarsI16 *);
typedef StarsI16 (*StarsMacintiSplitShdefs)(void *, const StarsU8[16]);
typedef StarsI16 (*StarsMacintiEnsure)(void *);
typedef StarsI16 (*StarsMacintiColonizerDecision)(void *, StarsI16 *);
typedef StarsI16 (*StarsMacintiVisit)(void *);
typedef struct StarsMacintiTurnPlanet {
    StarsI16 id;
    StarsI16 owner;
    const StarsU8 *raw_record;
    StarsPoint point;
    StarsU16 starbase_flags;
    StarsU16 packet_strength;
    StarsI16 desirability;
    StarsU8 mineral_concentration[3];
    StarsI32 resources_available[4];
    StarsI32 production_queue_cost[4];
    /* Optional typed view of the hull cost used by the next queue action.
     * A zero batch leaves the broader production-selection boundary intact. */
    StarsI32 hull_cost[4];
    StarsI16 production_design;
    StarsI16 production_batch;
    /* A zero limit means the caller did not expose the packed queue limit;
     * the semantic adapter then treats the queue as available. */
    StarsI16 production_queue_count;
    StarsI16 production_queue_limit;
    /* Optional original 56-byte _planet; the live table path decodes
     * identity, ownership, starbase, population guess and minerals. */
    StarsU16 raw_record_size;
    /* Caller-resolved original lpplprod allocation when raw_record[52..55]
     * is nonzero. The pointer bytes themselves are a guest far address. */
    const StarsU8 *raw_existing_prod_queue;
    StarsU16 raw_existing_prod_queue_size;
    /* Original far address of this _planet in the guest (offset, segment). */
    StarsU8 guest_far_pointer[4];
} StarsMacintiTurnPlanet;
typedef struct StarsMacintiPlanetPolicy {
    StarsU16 mineral_score;
    StarsI16 owned;
    StarsI16 unowned;
    StarsI16 hostile;
    StarsI16 has_starbase;
    StarsI16 viable_minerals;
    StarsI16 production_queue_available;
    StarsI16 production_affordable;
    StarsI16 production_batch_affordable;
    StarsI16 production_batch;
    StarsI16 threat_level;
    StarsI16 threat_marked;
} StarsMacintiPlanetPolicy;
typedef StarsI16 (*StarsMacintiVisitPlanetTyped)(void *,
    const StarsMacintiTurnPlanet *, const StarsMacintiPlanetPolicy *,
    StarsI16);
typedef StarsI16 (*StarsMacintiChoosePlanetProduction)(void *,
    const StarsMacintiTurnPlanet *, const StarsMacintiPlanetPolicy *,
    StarsI16);
#define STARS_MACINTI_PRODUCTION_NONE 0
#define STARS_MACINTI_PRODUCTION_QUEUE 1
#define STARS_MACINTI_PRODUCTION_FINISH 2
typedef struct StarsMacintiProductionPlan {
    StarsI16 action;
    StarsI16 design;
    StarsI16 quantity;
    StarsI16 object_group;
    StarsI16 queue_mode;
    StarsI16 finish_mode;
    StarsI16 reason;
    /* 1 only for the original attack-design AddItemToQueue branch that
     * increments Macinti's iStack_86 before its queue call. */
    StarsI16 attack_pressure_increment;
} StarsMacintiProductionPlan;
typedef StarsI16 (*StarsMacintiPlanProduction)(void *,
    const StarsMacintiTurnPlanet *, const StarsMacintiPlanetPolicy *,
    StarsI16, StarsMacintiProductionPlan *);
typedef StarsI16 (*StarsMacintiQueueProduction)(void *,
    const StarsMacintiTurnPlanet *, const StarsMacintiProductionPlan *);
typedef StarsI16 (*StarsMacintiFinishProduction)(void *,
    const StarsMacintiTurnPlanet *, StarsI16);
typedef StarsI16 (*StarsMacintiSelectPlanet)(void *,
    const StarsMacintiTurnPlanet *);
typedef StarsI16 (*StarsMacintiInitProduction)(void *,
    const StarsMacintiTurnPlanet *);
typedef StarsI16 (*StarsMacintiCurrentProductionQueue)(void *,
    const StarsMacintiTurnPlanet *, const StarsU8 **, StarsU16 *);
typedef struct StarsMacintiTurnFleet {
    StarsI16 id;
    StarsI16 owner;
    const StarsU8 *raw_record;
    /* Optional packed PLORD allocation.  The raw Macinti turn repairs the
     * first order before choosing a typed fleet action; callers that expose
     * the allocation let the semantic adapter preserve that mutation. */
    StarsU8 *raw_plord;
    StarsPoint point;
    StarsI16 planet_id;
    StarsI16 cord;
    StarsI16 order_kind;
    StarsI16 ship_counts[16];
    StarsU8 plan;
    /* Required by the optional bound planet-table fleet-marker sweep. */
    StarsU16 raw_record_size;
    StarsU16 raw_plord_size;
} StarsMacintiTurnFleet;
typedef StarsI16 (*StarsMacintiMineFromFleet)(void *,
    const StarsMacintiTurnFleet *, StarsI32 *);
/* Outer 10a0:0f5b/0ffc route, before the attack-design queue decision.
 * COLONIZER_THEN_ATTACK still requires the caller to execute its colonizer
 * queue/growth effects before entering the attack branch. */
#define STARS_MACINTI_OUTER_SKIP 0
#define STARS_MACINTI_OUTER_ATTACK 1
#define STARS_MACINTI_OUTER_COLONIZER_ONLY 2
#define STARS_MACINTI_OUTER_COLONIZER_THEN_ATTACK 3
typedef StarsI16 (*StarsMacintiShouldPlanetBuildColonizer)(void *,
    const StarsMacintiTurnPlanet *);
typedef struct StarsMacintiAttackOuterInput {
    const StarsMacintiTurnPlanet *planet;
    StarsI16 turn;
    StarsI16 colonizer_count;
    StarsI16 should_build_colonizers;
    StarsMacintiRandom random_value;
    StarsMacintiShouldPlanetBuildColonizer should_planet_build_colonizer;
    void *context;
} StarsMacintiAttackOuterInput;
StarsI16 stars_macinti_attack_outer_route(
    const StarsMacintiAttackOuterInput *, StarsI16 *);
typedef struct StarsMacintiAttackQueueInput {
    const StarsMacintiTurnPlanet *planet;
    const StarsMacintiTurnFleet *fleets;
    StarsI16 fleet_count;
    StarsI16 player_id;
    StarsI16 attack_design;
    StarsI16 attack_pressure;
    StarsU32 design_existing;
    StarsI16 outer_branch_enabled;
    StarsMacintiRandom random_value;
    StarsMacintiMineFromFleet mine_from_fleet;
    void *context;
} StarsMacintiAttackQueueInput;
StarsI16 stars_macinti_should_queue_attack(
    const StarsMacintiAttackQueueInput *, StarsI16 *);
typedef struct StarsMacintiAttackProductionInput {
    StarsMacintiAttackOuterInput outer;
    StarsMacintiAttackQueueInput attack;
    StarsI16 mission_design;
    StarsI16 mission_count;
    StarsI16 player_planet_count;
    StarsI16 colonizer_design;
    StarsI16 resources_at_planet;
    StarsI16 colonizer_ability;
    StarsI16 growth_percent;
    StarsMacintiQueueProduction queue_production;
    void *queue_context;
} StarsMacintiAttackProductionInput;
typedef struct StarsMacintiAttackProductionResult {
    StarsI16 outer_route;
    StarsI16 mission_queued;
    StarsI16 mission_count;
    StarsI16 colonizer_queues;
    StarsI16 attack_queued;
    StarsI16 colonizer_count;
    StarsI16 attack_pressure;
} StarsMacintiAttackProductionResult;
StarsI16 stars_macinti_run_attack_production(
    const StarsMacintiAttackProductionInput *,
    StarsMacintiAttackProductionResult *);
typedef struct StarsMacintiSelectedFleet {
    StarsU8 *fleet_record;
    StarsU16 fleet_record_size;
    StarsU8 *plord;
    StarsU16 plord_size;
} StarsMacintiSelectedFleet;
typedef struct StarsMacintiFleetPolicy {
    StarsI16 total_ships;
    StarsI16 colonizer_ships;
    StarsI16 transport_ships;
    StarsI16 armed_ships;
    StarsI16 mission_design_ships;
    StarsI16 attack_design_ships;
    StarsI16 support_design_ships;
    StarsI16 needs_order_repair;
    StarsI16 recommended_task;
} StarsMacintiFleetPolicy;
#define STARS_MACINTI_TASK_IDLE 0
#define STARS_MACINTI_TASK_REPAIR_ORDER 1
#define STARS_MACINTI_TASK_ATTACK 2
#define STARS_MACINTI_TASK_TRANSPORT 3
#define STARS_MACINTI_TASK_COLONIZE 4
typedef StarsI16 (*StarsMacintiFleetTask)(void *,
    const StarsMacintiTurnFleet *, const StarsMacintiFleetPolicy *, StarsI16);
#define STARS_MACINTI_FLEET_ACTION_NONE 0
#define STARS_MACINTI_FLEET_ACTION_REPAIR_ORDER 1
#define STARS_MACINTI_FLEET_ACTION_RETARGET_MINER 2
#define STARS_MACINTI_FLEET_ACTION_JOIN_BUDDY 3
#define STARS_MACINTI_FLEET_ACTION_COLONIZE 4
#define STARS_MACINTI_FLEET_ACTION_WORMHOLE 5
#define STARS_MACINTI_FLEET_ACTION_MOVE_STARBASE 6
#define STARS_MACINTI_FLEET_ACTION_ATTACK 7
#define STARS_MACINTI_FLEET_ACTION_TARGET_ARMADA 8
#define STARS_MACINTI_FLEET_ACTION_TARGET_FREIGHTER 9
#define STARS_MACINTI_FLEET_ACTION_CLEAR_TASK 10
typedef struct StarsMacintiFleetPlan {
    StarsI16 action;
    StarsI16 target_planet;
    StarsI16 order_kind;
    StarsI16 append;
    StarsI16 reason;
} StarsMacintiFleetPlan;
typedef StarsI16 (*StarsMacintiPlanFleetAction)(void *,
    const StarsMacintiTurnFleet *, const StarsMacintiFleetPolicy *,
    StarsI16, StarsMacintiFleetPlan *);
typedef StarsI16 (*StarsMacintiApplyFleetAction)(void *,
    const StarsMacintiTurnFleet *, const StarsMacintiFleetPlan *);
typedef StarsI16 (*StarsMacintiFleetAction)(void *,
    const StarsMacintiTurnFleet *, const StarsMacintiFleetPlan *);
typedef StarsI16 (*StarsMacintiVisitFleetTyped)(void *,
    const StarsMacintiTurnFleet *, const StarsMacintiFleetPolicy *,
    const StarsU8[4]);
typedef StarsI16 (*StarsMacintiIsAiTransport)(void *,
    const StarsMacintiTurnFleet *, StarsI16 *);
typedef StarsI16 (*StarsMacintiIsAiAttack)(void *,
    const StarsMacintiTurnFleet *, StarsI16 *);
typedef StarsI16 (*StarsMacintiClearCurrentTask)(void *,
    const StarsMacintiTurnFleet *, StarsI16);
typedef StarsI16 (*StarsMacintiFindBuddy)(void *,
    const StarsMacintiTurnFleet *, StarsI16, StarsI16, StarsI32,
    StarsI32, StarsI16 *);
typedef StarsI16 (*StarsMacintiRetargetMinerLive)(void *,
    const StarsMacintiTurnFleet *);
typedef StarsI16 (*StarsMacintiSetTransportPlan)(void *,
    const StarsMacintiTurnFleet *, StarsU8);
typedef StarsI16 (*StarsMacintiSelectFleet)(void *,
    const StarsMacintiTurnFleet *, StarsMacintiSelectedFleet *);
typedef StarsI16 (*StarsMacintiRefreshFleet)(void *,
    const StarsMacintiTurnFleet *, const StarsMacintiSelectedFleet *);
/* Opt-in 10a0:0f5b-1307 slice inputs. The caller supplies original service
 * values and asserts that the earlier/later production branches do not
 * contribute effects for this selected planet. */
typedef StarsI16 (*StarsMacintiBuildAttackProduction)(void *,
    const StarsMacintiTurnPlanet *, const StarsMacintiPlanetPolicy *,
    StarsI16, StarsMacintiAttackProductionInput *);
typedef struct StarsMacintiAiTurnOps {
    StarsMacintiCreateStarter create_starter;
    StarsMacintiMergeShdefs merge_shdefs;
    StarsMacintiDesignStatus check_design_status;
    StarsMacintiDesignStatusInfo design_status_info;
    StarsMacintiSplitShdefs split_shdefs;
    StarsMacintiEnsure ensure_shdefs;
    StarsMacintiEnsure ensure_starbase_designs;
    StarsMacintiColonizerDecision colonizer_decision;
    StarsMacintiVisit visit_planets;
    StarsMacintiVisit visit_fleets;
    StarsMacintiVisitPlanetTyped visit_planet_typed;
    StarsMacintiChoosePlanetProduction choose_planet_production;
    StarsMacintiPlanProduction plan_production;
    StarsMacintiBuildAttackProduction build_attack_production;
    StarsMacintiQueueProduction queue_production;
    StarsMacintiFinishProduction finish_production;
    StarsMacintiSelectPlanet select_planet;
    StarsMacintiInitProduction init_production;
    StarsMacintiCurrentProductionQueue current_production_queue;
    StarsMacintiVisitFleetTyped visit_fleet_typed;
    StarsMacintiFleetTask choose_fleet_task;
    StarsMacintiPlanFleetAction plan_fleet_action;
    StarsMacintiApplyFleetAction apply_fleet_action;
    StarsMacintiFleetAction repair_order;
    StarsMacintiFleetAction retarget_miner;
    StarsMacintiFleetAction join_buddy;
    StarsMacintiFleetAction colonize;
    StarsMacintiFleetAction goto_wormhole;
    StarsMacintiFleetAction move_starbase;
    StarsMacintiFleetAction attack;
    StarsMacintiFleetAction target_armada;
    StarsMacintiFleetAction target_freighter;
    StarsMacintiFleetAction clear_task;
    StarsMacintiSelectFleet select_fleet;
    StarsMacintiRefreshFleet refresh_fleet;
    StarsMacintiIsAiTransport is_ai_transport;
    /* Opt-in original first-sweep service path. Both callbacks and the
     * transport service are required together when is_ai_attack is set. */
    StarsMacintiIsAiAttack is_ai_attack;
    StarsMacintiClearCurrentTask clear_current_task;
    /* Opt-in original attack-design buddy/miner branch. */
    StarsMacintiRandom random_value;
    StarsMacintiFindBuddy find_buddy;
    StarsMacintiRetargetMinerLive retarget_miner_live;
    StarsMacintiSetTransportPlan set_transport_plan;
    StarsMacintiVisitFleetTyped route_transport_fleet;
    StarsMacintiVisitFleetTyped finalize_fleet;
    StarsMacintiVisit handle_basic_tasks;
    StarsMacintiVisit fill_production_queue;
    void *context;
} StarsMacintiAiTurnOps;
typedef struct StarsMacintiAiTurnInput {
    StarsI16 turn;
    StarsI16 advanced_player;
    StarsI16 starter_missing;
    const StarsMacintiAiTurnOps *ops;
    StarsU8 armada_potency[4];
    /* CheckAiShdefStatus fills this shared mask; it is also the first
     * SplitOutShdefs input after the late-turn status pass. */
    StarsU8 split_status_mask[16];
    StarsI16 starter_order;
    StarsI16 colonizer_count;
    StarsI16 player_id;
    StarsI16 planet_count;
    const StarsMacintiTurnPlanet *planets;
    StarsI16 fleet_count;
    const StarsMacintiTurnFleet *fleets;
    StarsI16 typed_planets_visited;
    StarsI16 typed_fleets_visited;
    StarsI16 typed_armed_fleets;
    StarsI16 typed_colonizer_fleets;
    StarsI16 typed_mission_fleets;
    StarsI16 typed_support_design_fleets;
    StarsI16 typed_attack_design_fleets;
    /* Original iStack_86 after the earlier owned-fleet census and queue
     * additions; distinct from game.turn. Required by live buddy routing. */
    StarsI16 first_sweep_attack_pressure;
    StarsI16 first_sweep_attack_pressure_bound;
    StarsI16 first_sweep_attack_census;
    StarsI16 first_sweep_attack_census_bound;
    StarsI16 first_sweep_mission_count;
    /* Caller asserts all pre-fleet attack queue additions are represented
     * by plan_production plans with attack_pressure_increment set. */
    StarsI16 derive_attack_pressure_from_plans;
    /* Execute the reconstructed mission/colonizer/attack slice for each
     * selected production planet instead of the one-plan adapter. */
    StarsI16 bound_attack_production;
    StarsI16 typed_production_candidates;
    StarsI16 typed_production_queued;
    StarsI16 typed_production_finished;
    StarsI16 typed_production_rejected;
    StarsI16 typed_fleet_actions;
    StarsI16 typed_fleet_actions_rejected;
    StarsI16 typed_transport_routes;
    StarsI16 typed_fleet_finalizations;
    StarsI16 basic_tasks_run;
    StarsI16 production_queue_filled;
    /* Bound game.cPlanMax/vlpbAiPlanet and the vrglpplAi ID selection.
     * planets must contain the full cPlanet scan; selected IDs are visited
     * in original pointer-list order, not full-table order. */
    StarsI16 planet_table_bound;
    StarsU16 game_planet_max;
    StarsU8 *raw_ai_planet_table;
    StarsU16 raw_ai_planet_record_count;
    const StarsI16 *selected_ai_planet_ids;
    StarsI16 selected_ai_planet_count;
    /* Optional byte-exact vrglpplAi: one little-endian far pointer per
     * selected planet. When supplied, it replaces the transitional ID list. */
    const StarsU8 *selected_ai_planet_pointers;
    StarsU16 selected_ai_planet_pointers_size;
} StarsMacintiAiTurnInput;
StarsI16 stars_do_macinti_ai_turn(StarsMacintiAiTurnInput *);

typedef struct StarsMinefield {
    StarsI16 id;
    StarsI16 owner;
    StarsI16 x;
    StarsI16 y;
    StarsI16 radius;
    StarsI16 mine_class;
    StarsI32 strength;
    StarsI16 active;
} StarsMinefield;
typedef struct StarsMinefieldFleet {
    StarsI16 id;
    StarsI16 owner;
    StarsI16 x;
    StarsI16 y;
    StarsI16 target_x;
    StarsI16 target_y;
    StarsI16 engine_count;
    StarsI16 engine_class[16];
    StarsI32 engine_count_by_type[16];
    StarsI32 shield_strength[16];
} StarsMinefieldFleet;
typedef StarsI16 (*StarsMinefieldRandom)(void *, StarsI16);
typedef StarsI16 (*StarsMinefieldIntersect)(void *, const StarsMinefieldFleet *,
    const StarsMinefield *, StarsI32 *, StarsI32 *);
typedef StarsI32 (*StarsMinefieldShield)(void *, StarsI16, StarsI16);
typedef StarsI32 (*StarsMinefieldDamage)(void *, StarsMinefieldFleet *,
    const StarsMinefield *, StarsI32);
typedef void (*StarsMinefieldTransfer)(void *, StarsMinefieldFleet *);
typedef void (*StarsMinefieldRemove)(void *, StarsMinefield *);
typedef void (*StarsMinefieldMessage)(void *, StarsI16, StarsI16, StarsI16,
    StarsI16, StarsI16, StarsI16);
typedef struct StarsMinefieldOps {
    StarsMinefieldRandom random_value;
    StarsMinefieldIntersect intersect;
    StarsMinefieldShield shield_value;
    StarsMinefieldDamage apply_damage;
    StarsMinefieldTransfer transfer_cargo;
    StarsMinefieldRemove remove_mine;
    StarsMinefieldMessage send_message;
    void *context;
} StarsMinefieldOps;
typedef struct StarsMinefieldInput {
    StarsMinefieldFleet *fleet;
    StarsMinefield *fields;
    StarsU16 field_count;
    StarsI32 *travel;
    StarsMinefield *hit;
    const StarsMinefieldOps *ops;
} StarsMinefieldInput;
StarsI16 stars_travel_through_mine_fields(StarsMinefieldInput *);

typedef struct StarsProduceItem {
    StarsI16 type;
    StarsI16 object_kind;
    StarsI16 quantity;
    StarsI16 partial; /* Existing partial-build marker passed to the callback. */
    StarsI16 progress; /* Packed partial-build progress percentage, 0..100. */
    StarsI16 active;
    StarsI16 special;
    StarsI32 cost[4];
} StarsProduceItem;
typedef struct StarsProducePlanet {
    StarsI16 id;
    StarsI16 owner;
    StarsI16 cheater;
    StarsI16 research_percent; /* Owning player's pctResearch. */
    StarsI16 research_exempt; /* Original planet bit 23. */
    StarsI32 resources;
    StarsI32 last_year_resources;
    StarsI32 mineral[3];
    StarsI16 population_factor;
    StarsI16 factory_count;
    StarsI16 mine_count;
    StarsI16 defense_count;
    StarsI16 terraform_capacity;
    StarsProduceItem *items;
    StarsU16 item_count;
    StarsU16 item_capacity;
} StarsProducePlanet;
typedef StarsI16 (*StarsProducePlayerCount)(void *);
typedef void (*StarsProduceMineMinerals)(void *);
typedef StarsI32 (*StarsProduceResources)(void *, const StarsProducePlanet *,
    StarsI16);
typedef StarsI16 (*StarsProduceCapacity)(void *, const StarsProducePlanet *,
    StarsI16, StarsI16);
typedef StarsI16 (*StarsProduceTerraform)(void *, const StarsProducePlanet *);
typedef StarsI16 (*StarsProduceBuildItem)(void *, StarsProducePlanet *,
    StarsProduceItem *, StarsI32 *, StarsI16);
typedef StarsI16 (*StarsProduceBuildItemWithResources)(void *,
    StarsProducePlanet *, StarsProduceItem *, StarsProduceItem *,
    StarsI32[4], StarsI16 *);
typedef StarsI16 (*StarsProduceBuildObject)(void *, StarsProducePlanet *,
    StarsI16, StarsI16, StarsI16, StarsI32 *);
typedef void (*StarsProduceMessage)(void *, StarsI16, StarsI16, StarsI16,
    StarsI16, StarsI16);
typedef void (*StarsProducePlanetStage)(void *, StarsProducePlanet *);
typedef void (*StarsProduceResearch)(void *, StarsI16);
typedef void (*StarsProduceRandomEvents)(void *);
typedef void (*StarsProduceRemoveItem)(void *, StarsProducePlanet *, StarsI16);
typedef StarsI16 (*StarsProduceInsertPartial)(void *, StarsProducePlanet *,
    StarsU16, const StarsProduceItem *);
typedef StarsI16 (*StarsProduceExtra)(void *, StarsI16);
typedef struct StarsProduceOps {
    StarsProducePlayerCount player_count;
    StarsProduceMineMinerals mine_minerals;
    StarsProduceResources resources_at_planet;
    StarsProduceCapacity max_capacity;
    StarsProduceTerraform terraform_capacity;
    StarsProduceBuildItem build_item;
    StarsProduceBuildItemWithResources build_item_with_resources;
    StarsProduceBuildObject build_object;
    StarsProduceMessage send_message;
    StarsProducePlanetStage update_populations;
    StarsProduceResearch update_research;
    StarsProduceRandomEvents random_events;
    StarsProduceRemoveItem remove_item;
    StarsProduceInsertPartial insert_partial;
    StarsProduceExtra planet_extra;
    void *context;
} StarsProduceOps;
typedef struct StarsProduceInput {
    StarsProducePlanet *planets;
    StarsU16 planet_count;
    StarsI16 game_flags;
    const StarsProduceOps *ops;
} StarsProduceInput;
void stars_produce(StarsProduceInput *);
StarsI16 stars_write_production_queue_record(const StarsProduceItem *,
    StarsU16, StarsU8 *, StarsU16, StarsU16 *, StarsWriteGameRecord, void *);
typedef StarsI16 (*StarsBuildProductionCosts)(void *,
    const StarsProducePlanet *, const StarsProduceItem *, StarsI32[4]);
typedef StarsI16 (*StarsBuildProductionCapacity)(void *,
    const StarsProducePlanet *, StarsI16, StarsI16);
typedef StarsI16 (*StarsBuildProductionTerraform)(void *,
    const StarsProducePlanet *);
typedef StarsI32 (*StarsBuildProductionPopulation)(void *,
    StarsProducePlanet *, StarsI16);
typedef StarsI16 (*StarsBuildProductionDesirability)(void *,
    const StarsProducePlanet *, StarsI16);
typedef StarsI16 (*StarsBuildProductionWarp)(void *,
    const StarsProducePlanet *);
typedef StarsI16 (*StarsBuildProductionRaceGrbit)(void *, StarsI16, StarsI16);
typedef StarsI16 (*StarsBuildProductionObject)(void *, StarsProducePlanet *,
    StarsI16, StarsI16, StarsI16, StarsI32[3]);
typedef void (*StarsBuildProductionMessage)(void *, StarsI16, StarsI16,
    StarsI16, StarsI16, StarsI16);
typedef struct StarsBuildProductionOps {
    StarsBuildProductionCosts costs;
    StarsBuildProductionCapacity capacity;
    StarsBuildProductionTerraform terraform;
    StarsBuildProductionPopulation population_loss;
    StarsBuildProductionDesirability desirability;
    StarsBuildProductionWarp warp_available;
    StarsBuildProductionRaceGrbit race_grbit;
    StarsBuildProductionObject build_object;
    StarsBuildProductionMessage send_message;
    void *context;
} StarsBuildProductionOps;
typedef struct StarsBuildProductionInput {
    StarsProducePlanet *planet;
    StarsProduceItem *item;
    StarsProduceItem *partial; /* Optional output queue entry for a partial build. */
    StarsI32 resources[4];
    StarsI16 alchemy;
    StarsI16 calculate_only;
    StarsI16 *status;
    const StarsBuildProductionOps *ops;
} StarsBuildProductionInput;
StarsI16 stars_build_production_item(StarsBuildProductionInput *);
typedef struct StarsColonistDrop {
    StarsI16 source_owner;
    StarsI16 target_planet;
    StarsI32 colonists;
    StarsI16 private_drop;
    StarsI16 active;
} StarsColonistDrop;
typedef struct StarsColonistPlanet {
    StarsI16 id;
    StarsI16 owner;
    StarsI32 population;
    StarsI32 capacity;
    StarsI32 growth;
    StarsI32 minerals;
    StarsI16 climate_factor;
    StarsI16 race_stat;
} StarsColonistPlanet;
typedef StarsI16 (*StarsColonistLookupPlanet)(void *, StarsI16,
    StarsColonistPlanet *);
typedef StarsI16 (*StarsColonistSurvival)(void *, const StarsColonistPlanet *,
    StarsI16, StarsI32, StarsI32 *);
typedef StarsI16 (*StarsColonistRaceStat)(void *, StarsI16, StarsI16);
typedef void (*StarsColonistMessage)(void *, StarsI16, StarsI16, StarsI16,
    StarsI16, StarsI16, StarsI16);
typedef void (*StarsColonistClaim)(void *, StarsColonistPlanet *, StarsI16);
typedef void (*StarsColonistUninhabit)(void *, StarsColonistPlanet *);
typedef void (*StarsColonistResearch)(void *, StarsI16, StarsI16);
typedef void (*StarsColonistInitProduction)(void *, StarsColonistPlanet *,
    StarsI16);
typedef StarsI32 (*StarsColonistRandom)(void *, StarsI32);
typedef struct StarsDropColonistsOps {
    StarsColonistLookupPlanet lookup_planet;
    StarsColonistSurvival survival;
    StarsColonistRaceStat race_stat;
    StarsColonistMessage send_message;
    StarsColonistClaim claim_planet;
    StarsColonistUninhabit uninhabit;
    StarsColonistResearch learn_research;
    StarsColonistInitProduction init_production;
    StarsColonistRandom random_value;
    void *context;
} StarsDropColonistsOps;
typedef struct StarsDropColonistsInput {
    StarsColonistDrop *drops;
    StarsU16 drop_count;
    StarsColonistPlanet *planets;
    StarsU16 planet_count;
    const StarsDropColonistsOps *ops;
} StarsDropColonistsInput;
void stars_drop_colonists(StarsDropColonistsInput *);

/* Deterministic galaxy-layout phase of GenerateWorld (NE 16:0136). */
typedef StarsI16 (*StarsGenerateWorldRandom)(void *, StarsI16);
#define STARS_GENERATE_WORLD_SPECIAL_ID_MODE 0x0008U
typedef struct StarsGenerateWorldLayout {
    StarsPoint points[999];
    StarsI16 point_ids[999];
    StarsI16 point_count;
    StarsI16 candidate_count;
    StarsI16 rejected_count;
    StarsI16 galaxy_extent;
    StarsI16 galaxy_inverse;
} StarsGenerateWorldLayout;
typedef struct StarsGenerateWorldInput {
    StarsI16 map_size;
    StarsI16 density;
    StarsI16 minimum_distance;
    StarsI16 smooth_layout;
    StarsU16 world_flags;
    StarsGenerateWorldRandom random_value;
    void *context;
    StarsGenerateWorldLayout *layout;
} StarsGenerateWorldInput;
/* Packed _game view consumed by GenerateWorld.  The raw record is 64 bytes;
 * the offsets below are the recovered Win16 storage boundary, not a host
 * replacement declaration. */
typedef struct StarsGenerateWorldGameState {
    StarsU32 game_id;
    StarsI16 map_size;
    StarsI16 density;
    StarsI16 player_count;
    StarsI16 planet_max;
    StarsI16 start_distance;
    StarsU16 dirty;
    StarsU16 generation_flags;
    StarsI16 turn;
    StarsSeeds random;
} StarsGenerateWorldGameState;
StarsI16 stars_generate_world_read_game_state(const StarsU8 *,
    const StarsSeeds *, StarsGenerateWorldGameState *);
StarsI16 stars_generate_world_random_from_state(void *, StarsI16);
StarsI16 stars_generate_world_layout(StarsGenerateWorldInput *);
typedef struct StarsGenerateWorldPlanet {
    StarsI16 id;
    StarsI16 owner;
    StarsPoint point;
    StarsI16 environment[3];
    StarsI16 environment_original[3];
    StarsI16 mineral_concentration[3];
    StarsI32 mineral_rate[3];
    StarsI32 population;
    StarsU16 flags;
    StarsI16 mine_count;
    StarsI16 factory_count;
    StarsI16 defense_count;
    /* Low-nibble packed isb production slot. */
    StarsI16 production_slot;
    StarsI16 special_world;
} StarsGenerateWorldPlanet;
typedef struct StarsGenerateWorldPlanetInput {
    StarsU16 world_flags;
    StarsI16 planet_count;
    StarsGenerateWorldRandom random_value;
    void *context;
    StarsGenerateWorldPlanet *planets;
} StarsGenerateWorldPlanetInput;
StarsI16 stars_generate_world_init_planets(StarsGenerateWorldPlanetInput *);
/* Selects player start planets from a completed galaxy layout.  The returned
 * indices are shuffled into player order, matching GenerateWorld's final
 * randomization step. */
typedef struct StarsGenerateWorldStartsInput {
    const StarsGenerateWorldLayout *layout;
    StarsI16 player_count;
    StarsI16 start_distance;
    StarsGenerateWorldRandom random_value;
    void *context;
    StarsI16 *start_planets;
} StarsGenerateWorldStartsInput;
StarsI16 stars_generate_world_assign_starts(StarsGenerateWorldStartsInput *);
typedef StarsI16 (*StarsGenerateWorldRaceStat)(void *, StarsI16, StarsI16);
typedef StarsI16 (*StarsGenerateWorldRaceFlag)(void *, StarsI16, StarsI16);
typedef StarsI16 (*StarsGenerateWorldTrueMaxGrowth)(void *, StarsI16);
typedef StarsI16 (*StarsGenerateWorldAdvantagePoints)(void *, StarsI16);
typedef void (*StarsGenerateWorldRandomizeRace)(void *, StarsI16);
typedef struct StarsGenerateWorldPlayer {
    StarsI16 id;
    StarsI16 race_type;
    StarsI16 race_stats[6];
    StarsI16 tech[6];
    StarsI16 environment_min[3];
    StarsI16 environment_max[3];
    StarsI16 homeworld;
    StarsI16 messages_sent;
    StarsI16 ships_created;
    StarsI16 is_dead;
    StarsI16 is_computer;
    StarsI16 computer_type;
    StarsI32 salt;
    StarsI16 research_percent;
    StarsI32 resources_last_year;
    StarsI16 score;
    StarsI16 relations[16];
} StarsGenerateWorldPlayer;
typedef struct StarsGenerateWorldPlayerInput {
    StarsU16 world_flags;
    StarsI16 player_count;
    StarsGenerateWorldPlayer *players;
    StarsGenerateWorldRaceStat race_stat;
    StarsGenerateWorldRaceFlag race_flag;
    StarsGenerateWorldRandomizeRace randomize_race;
    void *context;
} StarsGenerateWorldPlayerInput;
StarsI16 stars_generate_world_init_players(StarsGenerateWorldPlayerInput *);
typedef struct StarsGenerateWorldHomeworldInput {
    StarsU16 world_flags;
    StarsI16 mineral_floor;
    StarsI16 player_count;
    StarsI16 planet_count;
    const StarsI16 *start_planets;
    StarsGenerateWorldPlanet *planets;
    StarsGenerateWorldPlayer *players;
    StarsGenerateWorldRandom random_value;
    StarsGenerateWorldRaceFlag race_flag;
    /* Returns the signed race growth value used by enhanced homeworlds. */
    StarsGenerateWorldTrueMaxGrowth true_max_growth;
    /* Optional typed views of CAdvantagePoints and GetRaceStat. */
    StarsGenerateWorldAdvantagePoints advantage_points;
    StarsGenerateWorldRaceStat race_stat;
    void *context;
} StarsGenerateWorldHomeworldInput;
StarsI16 stars_generate_world_assign_homeworlds(
    StarsGenerateWorldHomeworldInput *);
typedef StarsI16 (*StarsGenerateWorldCreateShip)(void *, StarsI16,
    StarsI16, StarsI16, StarsI16);
typedef void (*StarsGenerateWorldSetStarbase)(void *, StarsI16, StarsI16);
typedef void (*StarsGenerateWorldSetScanner)(void *, StarsI16, StarsI16,
    StarsI16);
typedef StarsI16 (*StarsGenerateWorldCreateSpecialStartup)(void *,
    StarsI16, StarsI16);
typedef struct StarsGenerateWorldDesignPart {
    StarsU16 group;
    StarsU16 subtype;
    StarsU16 part;
} StarsGenerateWorldDesignPart;
typedef struct StarsGenerateWorldDesign {
    StarsU16 part_count;
    StarsGenerateWorldDesignPart parts[16];
} StarsGenerateWorldDesign;
typedef StarsGenerateWorldDesign *(*StarsGenerateWorldGetDesignBank)(void *,
    StarsI16, StarsU16 *);
typedef StarsI16 (*StarsGenerateWorldPartAvailable)(void *, StarsU16,
    StarsU16, StarsU16);
typedef struct StarsGenerateWorldStartupShipsInput {
    StarsI16 player_count;
    StarsI16 map_size;
    StarsGenerateWorldPlayer *players;
    StarsGenerateWorldRaceFlag race_flag;
    StarsGenerateWorldCreateShip create_ship;
    /* Race type 6 sets the homeworld starbase bit after its first ship. */
    StarsGenerateWorldSetStarbase set_starbase;
    /* Race type 8 gives the homeworld the raw maximum scanner value. */
    StarsGenerateWorldSetScanner set_scanner;
    /* Returns the number of nearby-special startup ships created. */
    StarsGenerateWorldCreateSpecialStartup create_special_startup;
    /* Optional typed view of the per-player design bank. */
    StarsGenerateWorldDesign *designs;
    StarsU16 design_count;
    StarsGenerateWorldGetDesignBank get_design_bank;
    StarsI16 environment_special;
    StarsGenerateWorldPartAvailable part_available;
    void *context;
} StarsGenerateWorldStartupShipsInput;
StarsI16 stars_generate_world_create_startup_ships(
    StarsGenerateWorldStartupShipsInput *);
typedef StarsI16 (*StarsGenerateWorldPlanetDesirability)(void *,
    const StarsGenerateWorldPlanet *, StarsI16);
typedef struct StarsGenerateWorldSpecialPlanetInput {
    const StarsGenerateWorldLayout *layout;
    StarsI16 planet_count;
    StarsI16 player;
    StarsI16 homeworld;
    StarsGenerateWorldPlanet *planets;
    StarsGenerateWorldRandom random_value;
    StarsGenerateWorldPlanetDesirability desirability;
    StarsGenerateWorldCreateShip create_ship;
    void *context;
    StarsI16 *special_planet;
} StarsGenerateWorldSpecialPlanetInput;
StarsI16 stars_generate_world_create_special_planet(
    StarsGenerateWorldSpecialPlanetInput *);
typedef StarsI16 (*StarsGenerateWorldOpenFile)(void *, const char *);
typedef StarsI16 (*StarsGenerateWorldWriteRecord)(void *, StarsI16,
    StarsI16, const void *, StarsI16);
typedef void (*StarsGenerateWorldCloseStream)(void *);
typedef StarsI16 (*StarsGenerateWorldWriteDataFile)(void *, const char *,
    StarsI16, StarsI16);
typedef struct StarsGenerateWorldFileInput {
    const StarsGenerateWorldLayout *layout;
    StarsI16 player_count;
    const char *base_name;
    const void *game_state;
    StarsGenerateWorldOpenFile open_file;
    StarsGenerateWorldWriteRecord write_record;
    StarsGenerateWorldCloseStream close_stream;
    StarsGenerateWorldWriteDataFile write_data_file;
    void *context;
} StarsGenerateWorldFileInput;
StarsI16 stars_generate_world_write_files(StarsGenerateWorldFileInput *);
typedef void (*StarsGenerateWorldSetPlayer)(void *, StarsI16);
typedef void (*StarsGenerateWorldResetMessageLog)(void *);
typedef void (*StarsGenerateWorldCreateWindows)(void *);
typedef void (*StarsGenerateWorldDestroyGame)(void *);
typedef StarsI16 (*StarsGenerateWorldFormatLoadName)(void *, StarsI16,
    char *, StarsI16);
typedef StarsI16 (*StarsGenerateWorldLoadGame)(void *, const char *,
    const char *);
typedef void (*StarsGenerateWorldSendMessage)(void *, StarsI16, StarsI16,
    StarsI16);
typedef struct StarsGenerateWorldFinalizeInput {
    StarsI16 batch_mode;
    StarsI16 hosted_game;
    StarsI16 selected_player;
    const char *base_name;
    char *load_name;
    StarsI16 load_name_capacity;
    StarsGenerateWorldSetPlayer set_player;
    StarsGenerateWorldResetMessageLog reset_message_log;
    StarsGenerateWorldCreateWindows create_windows;
    StarsGenerateWorldDestroyGame destroy_game;
    StarsGenerateWorldFormatLoadName format_load_name;
    StarsGenerateWorldLoadGame load_game;
    StarsGenerateWorldSendMessage send_message;
    void *context;
} StarsGenerateWorldFinalizeInput;
StarsI16 stars_generate_world_finalize(
    StarsGenerateWorldFinalizeInput *);
typedef void (*StarsGenerateWorldInitBattlePlan)(void *, StarsU8 *,
    StarsI16, StarsI16);
typedef struct StarsGenerateWorldBattlePlansInput {
    StarsI16 player_count;
    StarsI16 plan_stride;
    StarsI16 plan_size;
    StarsU8 *plan_storage;
    StarsU8 *plan_counts;
    StarsI16 planet_count;
    StarsGenerateWorldPlanet *planets;
    StarsGenerateWorldInitBattlePlan init_plan;
    void *context;
} StarsGenerateWorldBattlePlansInput;
StarsI16 stars_generate_world_init_battle_plans(
    StarsGenerateWorldBattlePlansInput *);
typedef StarsI16 (*StarsGenerateWorldCreateWormhole)(void *, StarsI16);
typedef void (*StarsGenerateWorldSetWormholePoint)(void *, StarsI16,
    StarsPoint);
typedef StarsI16 (*StarsGenerateWorldValidateWormhole)(void *, StarsI16);
typedef StarsI16 (*StarsGenerateWorldLinkWormholes)(void *, StarsI16,
    StarsI16);
typedef struct StarsGenerateWorldWormholesInput {
    StarsI16 pair_count;
    StarsI16 galaxy_extent;
    StarsI16 max_attempts;
    StarsGenerateWorldRandom random_value;
    StarsGenerateWorldCreateWormhole create_wormhole;
    StarsGenerateWorldSetWormholePoint set_point;
    StarsGenerateWorldValidateWormhole validate_wormhole;
    StarsGenerateWorldLinkWormholes link_wormholes;
    void *context;
} StarsGenerateWorldWormholesInput;
StarsI16 stars_generate_world_create_wormholes(
    StarsGenerateWorldWormholesInput *);
typedef void (*StarsGenerateWorldSendPlayerMessage)(void *, StarsI16,
    StarsI16, StarsI16, StarsI16);
typedef struct StarsGenerateWorldPlayerStatusInput {
    StarsI16 player_count;
    StarsGenerateWorldPlayer *players;
    StarsGenerateWorldSendPlayerMessage send_message;
    void *context;
    StarsI16 *selected_player;
    StarsI16 *single_player_mode;
} StarsGenerateWorldPlayerStatusInput;
StarsI16 stars_generate_world_finalize_player_status(
    StarsGenerateWorldPlayerStatusInput *);

/* Typed top-level phase ordering for GenerateWorld.  Each phase keeps its
 * original data ownership and callback boundary; this wrapper only expresses
 * the order in which the original procedure performs the work.  The special
 * planet array may be omitted when the selected races do not request that
 * path. */
typedef struct StarsGenerateWorldPipelineInput {
    StarsGenerateWorldInput *layout;
    StarsGenerateWorldPlanetInput *planets;
    StarsGenerateWorldStartsInput *starts;
    StarsGenerateWorldPlayerInput *players;
    StarsGenerateWorldHomeworldInput *homeworlds;
    StarsGenerateWorldStartupShipsInput *startup_ships;
    StarsGenerateWorldSpecialPlanetInput *special_planets;
    StarsI16 special_planet_count;
    StarsGenerateWorldBattlePlansInput *battle_plans;
    StarsGenerateWorldWormholesInput *wormholes;
    StarsGenerateWorldPlayerStatusInput *player_status;
    StarsGenerateWorldFileInput *files;
    StarsGenerateWorldFinalizeInput *finalize;
} StarsGenerateWorldPipelineInput;
StarsI16 stars_generate_world_pipeline(StarsGenerateWorldPipelineInput *);
#endif
