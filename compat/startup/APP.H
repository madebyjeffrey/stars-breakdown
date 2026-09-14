/* Native Windows integration; original game behavior lives in reconstructed/. */
#ifndef START_APP_H
#define START_APP_H
#include <windows.h>
#include "title.h"
#include "config.h"
#include "runtime_internal.h"
extern StarsRuntime runtime;
extern StarsPlatform platform;
extern HWND frame_window, title_window;
extern HINSTANCE app_instance;
extern char directory[144], ini_file[180];
void log_line(const char *);
void native_platform(void);
StarsU32 native_window(const StarsWindowCall *, int *);
void race_command(HWND, unsigned);
int race_resources(void);
void workshop_show(void);
void workshop_paint(HWND);
void settings_read(void);
void settings_write(void);
int native_selftest(void);
BOOL CALLBACK RaceProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK SettingsProc(HWND, UINT, WPARAM, LPARAM);
void settings_show(HWND);
int settings_fallback(void);
int settings_workshop(void);
#define CMD_RACES 600
#define CMD_NEW_RACE 601
#define CMD_OPEN_RACE 602
#define CMD_RECENT_RACE 603
#define CMD_TITLE 604
#define CMD_SETTINGS 605
#define CMD_EXIT 606
#define CMD_CHECK 607
#define CMD_ABOUT 608
#endif
