#ifndef STARS_TITLE_H
#define STARS_TITLE_H
#include "startup.h"
#include "raceui.h"

typedef struct StarsCreateWindow {
    const StarsU8 *class_name, *text;
    StarsRect geometry; /* x, y, width, height */
    StarsU16 instance, menu;
} StarsCreateWindow;
typedef struct StarsProfileString {
    const StarsU8 *section, *key, *fallback, *file;
} StarsProfileString;

StarsI16 stars_read_big_block(StarsRuntime *, StarsU16, StarsU32, StarsU32);
StarsU16 stars_load_big_resource(StarsRuntime *, StarsU16);
StarsU16 stars_palette_from_dib(StarsRuntime *, StarsU16);
void stars_ini_window_rect(StarsRuntime *, StarsU16, StarsU16, StarsI16, StarsU16);
/* The original caller supplies only four of PushRandom's eight argument bytes.
 * stack_seed makes the two saved register words it consumes explicit. */
void stars_draw_stars(StarsRuntime *, StarsU16, const StarsRect *, StarsU32 stack_seed);
/* Recovered TitleWndProc paths. Game commands and shutdown configuration are
 * deliberately outside this interface; these are not a full TitleWndProc. */
void stars_title_create(StarsRuntime *, StarsU16);
void stars_title_paint(StarsRuntime *, StarsU16, StarsU32 stack_seed);
StarsI16 stars_title_palette(StarsRuntime *, StarsU16, StarsU16, StarsU16);
void stars_title_release(StarsRuntime *);
/* Font-only slice of ReadIniSettings, with the filename supplied by the caller. */
void stars_title_read_fonts(StarsRuntime *, const StarsU8 *);
#endif
