#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct BackupState {
    int calls;
    char created[80];
} BackupState;

static void make_directory(void *context, const StarsU8 *path)
{
    BackupState *state = (BackupState *)context;
    ++state->calls;
    strcpy(state->created, (const char *)path);
}

int main(void)
{
    StarsU8 path[80];
    BackupState state;
    int failures = 0;

    memset(&state, 0, sizeof(state));
    stars_create_backup_dir(path, (const StarsU8 *)"C:\\GAME\\STARS.HST",
        1U, 27U, (const StarsU8 *)"backup", (const StarsU8 *)"backup%d",
        (const StarsU8 *)"backup%03d", (const StarsU8 *)".bak",
        make_directory, &state);
    if (strcmp((char *)path, "C:\\GAME\\backup.bak") != 0 || state.calls != 1 ||
        strcmp(state.created, "C:\\GAME\\backup") != 0)
        ++failures;

    memset(&state, 0, sizeof(state));
    stars_create_backup_dir(path, (const StarsU8 *)"STARS.HST", 100U, 327U,
        (const StarsU8 *)"backup", (const StarsU8 *)"backup%d",
        (const StarsU8 *)"backup%03d", (const StarsU8 *)".bak",
        make_directory, &state);
    if (strcmp((char *)path, "backup027.bak") != 0 || state.calls != 1 ||
        strcmp(state.created, "backup027") != 0)
        ++failures;
    printf("CREATE_BACKUP_DIR_CHECKS=2 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
