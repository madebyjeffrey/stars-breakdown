/* Automatic compiler/output acceptance runner. UI services are deterministic. */
#ifndef RACE_TEST_H
#define RACE_TEST_H
#include "raceui.h"
#include "runtime_internal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#define ACTION_MAX 256
#define DIALOG_MAX 16
typedef struct Action {
    StarsU16 kind, a, b, c, d, e;
    char text[32];
} Action;
typedef struct Dialog {
    StarsU16 resource, first, count;
} Dialog;
extern StarsRuntime runtime;
extern FILE *report;
extern char filename[13];
extern Action *actions;
extern Dialog dialogs[DIALOG_MAX];
extern unsigned dialog_count, dialog_next, alerts, files_created;
extern unsigned long checks, failures;
extern jmp_buf fatal_jump;
void fatal(const char *message);
void check(const char *label, long actual, long expected);
void read_exact(FILE *f, void *p, unsigned count);
StarsU16 read_word(FILE *f);
void world_init(void);
void world_reset(void);
void world_free(void);
StarsU32 test_window(void *context, const StarsWindowCall *call);
StarsI16 test_dialog(void *context, StarsRuntime *r, StarsU16 resource,
    StarsU16 owner, StarsDialogProc proc);
extern const StarsPlatform test_platform;
void close_data_file(void);
#endif
