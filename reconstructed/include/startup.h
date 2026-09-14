#ifndef STARS_STARTUP_H
#define STARS_STARTUP_H
#include "memory.h"
StarsU16 stars_brush_get(StarsRuntime *, StarsU32);
void stars_brush_free(StarsRuntime *, StarsU16);
StarsI16 stars_system_colors(StarsRuntime *);
StarsI16 stars_create_fonts(StarsRuntime *, StarsU16);
void stars_init_tiles(StarsU8 *);
void stars_free_stuff(StarsMemory *);
#endif
