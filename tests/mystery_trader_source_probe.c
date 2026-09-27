#include <stdio.h>
#include <string.h>
#include "core.h"
typedef struct Trace { StarsMysteryTraderThing thing; StarsI16 messages; } Trace;
static StarsMysteryTraderThing *allocate(void *c, StarsI16 kind, StarsI16 mode)
{ Trace *t = (Trace *)c; (void)kind; (void)mode; t->thing.id = 7; return &t->thing; }
static StarsI16 random_value(void *c, StarsI16 limit)
{ (void)c; (void)limit; return 0; }
static void message(void *c, StarsI16 p, StarsI16 id, StarsI16 thing)
{ Trace *t = (Trace *)c; (void)p; (void)id; (void)thing; ++t->messages; }
int main(void)
{
    StarsMysteryTraderInput input;
    Trace trace;
    memset(&input, 0, sizeof input); memset(&trace, 0, sizeof trace);
    input.turn = 40; input.map_size = 1; input.player_count = 2;
    input.allocate_thing = allocate; input.random = random_value;
    input.send_message = message; input.context = &trace;
    stars_mystery_trader(&input);
    if (trace.messages != 2 || trace.thing.flags != 8 ||
            trace.thing.x != 1020 || trace.thing.y != 1020 ||
            trace.thing.ability != 0x1000)
        return 1;
    puts("MYSTERY_TRADER_SOURCE=PASS");
    return 0;
}
