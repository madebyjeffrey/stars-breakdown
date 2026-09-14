#ifndef STARS_RUNTIME_INTERNAL_H
#define STARS_RUNTIME_INTERNAL_H
#include "runtime.h"
#include <string.h>
#include <stdio.h>
StarsU16 stars_word(const StarsU8 *, StarsU16);
StarsU32 stars_dword(const StarsU8 *, StarsU16);
void stars_put_word(StarsU8 *, StarsU16, StarsU16);
void stars_put_dword(StarsU8 *, StarsU16, StarsU32);
StarsI16 stars_signed_word(StarsU16);
StarsU16 stars_scratch(StarsRuntime *, StarsU16);
StarsU32 stars_window(StarsRuntime *, const char *, StarsU16, StarsU16,
    StarsU16, StarsU32, void *, void *);
StarsU16 stars_control(StarsRuntime *, StarsU16, StarsU16);
StarsU32 stars_send_item(StarsRuntime *, StarsU16, StarsU16, StarsU16, StarsU16, StarsU32);
StarsU16 stars_game_string(StarsRuntime *, StarsI16);
StarsI16 stars_runtime_alert(StarsRuntime *, StarsU16, StarsI16);
void stars_runtime_read(void *, StarsU8 *, StarsU16);
void stars_runtime_write(void *, const StarsU8 *, StarsU16);
void stars_runtime_abort(StarsRuntime *, StarsU16, StarsI16);
#endif
