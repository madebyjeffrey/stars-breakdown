#ifndef STARS_CONFIG_H
#define STARS_CONFIG_H
#include "title.h"
void stars_read_ini_tiles(StarsU8 *, StarsU16, StarsU16, StarsI16);
void stars_window_rect(StarsRuntime *, StarsU16, StarsU16);
void stars_window_ini_string(StarsRuntime *, StarsU16, StarsU16);
/* Explicit UI-setting slices; excludes registration, game MRU, orders and
 * other game-session fields of the larger Read/WriteIniSettings routines. */
void stars_read_ui_config(StarsRuntime *, const StarsU8 *);
void stars_write_ui_config(StarsRuntime *, const StarsU8 *, StarsU16);

typedef struct StarsReadIniWindow {
    StarsI16 left, top, right, bottom;
} StarsReadIniWindow;
typedef struct StarsReadIniPane {
    StarsI16 x, y, width, height;
    StarsU16 visible, sort;
    StarsI16 ascending;
} StarsReadIniPane;
typedef struct StarsReadIniProduction {
    StarsU8 valid, no_research, count;
    StarsU16 order[12];
    StarsU8 name[13];
} StarsReadIniProduction;
typedef struct StarsReadIniState {
    StarsU16 startup_flags, graphics_bits, graphics_bits2, graphics_bits2_low;
    StarsReadIniWindow frame;
    StarsReadIniPane pane[4];
    StarsU16 window_layout, pane_width[6];
    StarsI16 screen_colors, available_screen_colors;
    StarsI16 plan_width, message_height, minimum_height;
    StarsU16 second_plan_width, second_message_height, second_minimum_height;
    StarsU16 scan_ship, scan_enemy_ship, scan_mines, radar_view, scan_flags;
    StarsI16 scan_zoom, min_graph_max;
    StarsU32 serial_number, lid;
    StarsU8 machine_config[16], base[256], default_password[16];
    StarsI16 message_index, turn, player_id, object_selection;
    StarsU16 backup_dirs, speed_vcr;
    StarsU32 date_installed;
    StarsU8 mru[9][256], race_file[4][32];
    StarsU16 view_flags;
    StarsU8 zip_valid[4], zip_name[4][13];
    StarsU16 zip_order[4][5];
    StarsReadIniProduction production[5];
} StarsReadIniState;
typedef struct StarsReadIniOps {
    void (*get_string)(void *, StarsU16, StarsU8 *, StarsU16);
    StarsI16 (*profile_int)(void *, const StarsU8 *, const StarsU8 *, StarsI16, const StarsU8 *);
    StarsU16 (*profile_string)(void *, const StarsU8 *, const StarsU8 *, const StarsU8 *,
        StarsU8 *, StarsU16, const StarsU8 *);
    void (*get_window)(void *, StarsU16, StarsReadIniWindow *);
    void (*read_tiles)(void *, const StarsU8 *, StarsU16);
    void (*parse_serial)(void *, StarsU32 *, StarsU8 *, const StarsU8 *);
    void (*alert)(void *, StarsU16, StarsI16);
    StarsU32 (*today)(void *);
} StarsReadIniOps;
void stars_read_ini_settings(StarsReadIniState *, const StarsReadIniOps *, void *,
    const StarsU8 *);
typedef struct StarsWriteIniState {
    StarsReadIniWindow frame;
    StarsReadIniPane pane[4];
    StarsU16 startup_flags, graphics_bits, graphics_bits2, graphics_bits2_low;
    StarsU16 window_layout, pane_width[6];
    StarsI16 available_screen_colors, scan_zoom, min_graph_max;
    StarsU16 scan_flags, scan_ship, scan_enemy_ship, scan_mines, radar_view;
    StarsU32 serial_number, game_lid, date_installed;
    StarsU8 machine_config[16], base[256];
    StarsI16 object_mode, player_id, object_selection, message_index, turn;
    StarsU16 timer_id, speed_vcr;
    StarsU8 zip_valid[4], zip_name[4][13];
    StarsU16 zip_order[4][5];
    StarsReadIniProduction production[5];
} StarsWriteIniState;
typedef struct StarsWriteIniOps {
    void (*get_string)(void *, StarsU16, StarsU8 *, StarsU16);
    void (*write_string)(void *, const StarsU8 *, const StarsU8 *, const StarsU8 *, const StarsU8 *);
    void (*format_serial)(void *, StarsU32, const StarsU8 *, StarsU8 *, StarsU16);
    void (*format_tiles)(void *, StarsU16, StarsU8 *, StarsU16);
} StarsWriteIniOps;
void stars_write_ini_settings(const StarsWriteIniState *, const StarsWriteIniOps *, void *,
    const StarsU8 *);
#endif
