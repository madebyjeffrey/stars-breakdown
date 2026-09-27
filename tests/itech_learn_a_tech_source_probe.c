#include <stdio.h>
#include <string.h>
#include "core.h"
typedef struct Trace { StarsI16 trader; StarsI16 research; StarsI16 events; } Trace;
static StarsI16 random_value(void *c, StarsI16 limit)
{ (void)c; return limit == 100 ? 60 : 0; }
static StarsI16 give(void *c, StarsU32 mask, StarsI16 p, StarsU16 *id)
{ (void)c; (void)mask; (void)p; *id = 42; return 5; }
static StarsI32 cost(void *c, StarsI16 t, StarsI16 l, StarsI16 p)
{ (void)c; (void)t; (void)l; (void)p; return 100L; }
static void trader_message(void *c, StarsI16 p, StarsI16 m, StarsU16 id,
    StarsI16 x, StarsI16 y)
{ Trace *t = (Trace *)c; (void)p; (void)m; (void)id; (void)x; (void)y; ++t->trader; }
static void research_message(void *c, StarsI16 p, StarsI16 m, StarsI16 x,
    StarsI16 y, StarsI16 tech, StarsI32 amount)
{ Trace *t = (Trace *)c; (void)p; (void)m; (void)x; (void)y; (void)tech;
  (void)amount; ++t->research; }
static void mark(void *c, StarsI16 p)
{ Trace *t = (Trace *)c; (void)p; ++t->events; }
int main(void)
{
    StarsTechLearnPlayer player;
    StarsTechLearnInput input;
    Trace trace;
    StarsU16 goto_id;
    memset(&player, 0, sizeof player); memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace); goto_id = 0;
    input.player = &player; input.player_id = 0; input.message_id = -1;
    input.trader_chance[0] = 100; input.goto_id = &goto_id;
    input.random = random_value; input.give_trader_part = give;
    input.tech_level_cost = cost; input.send_trader_message = trader_message;
    input.send_research_message = research_message; input.mark_event = mark;
    input.context = &trace;
    if (stars_itech_learn_a_tech(&input) != -1 || goto_id != 42 ||
            trace.trader != 0 || trace.events != 1)
        return 1;
    puts("ITECH_LEARN_A_TECH_SOURCE=PASS");
    return 0;
}
