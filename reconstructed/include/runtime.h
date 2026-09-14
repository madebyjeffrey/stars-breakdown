/* Race component integration. Only operating-system services are supplied by
 * the caller. Game logic and dialog procedures belong to the component. */
#ifndef STARS_RUNTIME_H
#define STARS_RUNTIME_H
#include "core.h"

typedef struct StarsRuntime StarsRuntime;
typedef StarsI16 (*StarsDialogProc)(StarsRuntime *, StarsU16, StarsU16,
    StarsU16, StarsU32);
/* Arguments retain Win16 widths. p/q carry host addresses for pointer-valued
 * arguments; value carries a DWORD or the original numeric LPARAM. Operation
 * names are Windows API names, not callbacks for unreconstructed game helpers. */
typedef struct StarsWindowCall {
    const char *operation;
    StarsU16 window, a, b;
    StarsU32 value;
    void *p, *q;
} StarsWindowCall;
typedef struct StarsSaveChoice {
    StarsU16 owner, file_offset;
    StarsU32 flags, filter_index;
    StarsU8 *file, *title, *directory, *filter;
    const StarsU8 *extension;
    StarsU16 file_capacity, title_capacity;
} StarsSaveChoice;
typedef struct StarsPlatform {
    StarsI16 (*open)(void *, const StarsU8 *, StarsU16, StarsU16 *);
    StarsI16 (*close)(void *, StarsI16);
    StarsU16 (*read)(void *, StarsI16, StarsU8 *, StarsU16);
    StarsU16 (*write)(void *, StarsI16, const StarsU8 *, StarsU16);
    StarsU32 (*ticks)(void *);
    const StarsU8 *(*memory)(void *, StarsU16, StarsU16);
    StarsI16 (*access)(void *, const StarsU8 *, StarsI16);
    StarsU32 (*seek)(void *, StarsI16, StarsU32, StarsI16);
    void (*date)(void *, StarsU8 *);
    void (*time)(void *, StarsU8 *);
    /* TOOLHELP TimerCount, distinct from USER GetTickCount. */
    StarsU32 (*timer_count)(void *);
    StarsU32 (*window)(void *, const StarsWindowCall *);
    /* Adapter performs MakeProcInstance / DialogBox / FreeProcInstance. */
    StarsI16 (*dialog)(void *, StarsRuntime *, StarsU16, StarsU16, StarsDialogProc);
    StarsI16 (*save_choice)(void *, StarsSaveChoice *);
    /* Unhandled original nonlocal transfer; must not return. */
    void (*abort)(void *, StarsU16, StarsI16);
} StarsPlatform;
struct StarsRuntime {
    StarsU8 *data;
    const StarsU8 *strings;
    const StarsPlatform *platform;
    void *context;
    StarsStreamApi stream;
    StarsAlertApi alerts;
    /* Private near-address workspace [scratch_bottom,scratch_top), disjoint
     * from globals, arguments and far-memory sources. Stack bytes aren't part
     * of the public state comparison. Scopes restore top after returning. */
    StarsU16 scratch_bottom, scratch_top;
    void *recovery;
    /* Read-only NE images, indexed by original segment ordinal. Formatting
     * planet/part names needs 9/2; runtime_init installs 3 and 37. */
    const StarsU8 *segments[38];
};
void stars_runtime_init(StarsRuntime *, StarsU8 *, const StarsU8 *,
    const StarsPlatform *, void *, StarsU16, StarsU16);
StarsU32 stars_password_salt(const StarsU8 *);
void stars_delay(StarsI16, StarsTickCount, void *);
StarsU16 stars_version_string(StarsU8 *, const StarsU8 *);
void stars_output_file_string(StarsRuntime *, const StarsU8 *, const StarsU8 *);
void stars_output_string(StarsRuntime *, StarsI16, const StarsU8 *);
StarsI16 stars_check_password(StarsRuntime *);
StarsI16 stars_password_dialog(StarsRuntime *, StarsU16, StarsU16, StarsU16, StarsU32);
StarsI16 stars_save_race(StarsRuntime *, StarsU16, StarsU16);
StarsI16 stars_was_race_file(StarsRuntime *, StarsU16, StarsI16);
StarsU16 stars_format_id(StarsRuntime *, StarsI16, const StarsU8 *);
StarsU16 stars_format_string(StarsRuntime *, StarsU16, const StarsU8 *);
StarsU16 stars_environment_string(StarsRuntime *, StarsI16, StarsI16);
StarsU16 stars_gravity_string(StarsRuntime *, StarsI16);
#endif
