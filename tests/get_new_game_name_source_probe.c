#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace { StarsU16 string_id; char filter[32]; } Trace;

static StarsI16 get_string(void *context, StarsU16 id, StarsU8 *output)
{
    Trace *trace = (Trace *)context;
    trace->string_id = id;
    strcpy((char *)output, "Stars files|*.xy");
    return 1;
}

static StarsI16 choose_file(void *context, const StarsU8 *initial,
    const StarsU8 *filter, StarsU8 *output, StarsU16 capacity,
    StarsU16 *extension_offset)
{
    Trace *trace = (Trace *)context;
    (void)initial; (void)capacity;
    strcpy(trace->filter, (const char *)filter);
    strcpy((char *)output, "saved");
    *extension_offset = 0U;
    return 1;
}

int main(void)
{
    StarsGetNewGameNameOps ops;
    Trace trace;
    StarsU8 suggestion[32];
    memset(&ops, 0, sizeof ops);
    memset(&trace, 0, sizeof trace);
    strcpy((char *)suggestion, "old");
    ops.get_string = get_string;
    ops.choose_file = choose_file;
    if (!stars_get_new_game_name(suggestion, (StarsU16)sizeof suggestion,
            &ops, &trace) || strcmp((char *)suggestion, "savedxy") != 0 ||
            trace.string_id != 0x316U || strcmp(trace.filter, "Stars files") != 0)
        return 1;
    puts("GET_NEW_GAME_NAME_SOURCE=PASS");
    return 0;
}
