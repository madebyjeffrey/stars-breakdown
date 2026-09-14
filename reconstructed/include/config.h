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
#endif
