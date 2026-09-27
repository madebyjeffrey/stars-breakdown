#include <stdio.h>
#include <string.h>
#include "core.h"
typedef struct Trace {
    StarsI16 loaded;
    StarsI16 destroyed;
    StarsI16 prepared;
    StarsI16 dispatch[8];
    StarsI16 dispatched_player;
    StarsI16 logs;
    StarsI16 history;
    StarsI16 frees;
    StarsI16 current_player, fail_load, failures;
} Trace;
static unsigned char memory[0x2000];
static unsigned char planets[64], indices[16];
static StarsI16 load(void *c, StarsI16 p, StarsI16 r)
{
    Trace *t;
    t = (Trace *)c;
    (void)p;
    (void)r;
    ++t->loaded;
    t->current_player = p;
    return !t->fail_load;
}
static void simple(void *c)
{
    Trace *t;
    t = (Trace *)c;
    if (t->prepared == 0) {
        unsigned int i;
        for (i = 0; i < sizeof planets; ++i)
            if (planets[i]) ++t->failures;
        if (memory[0] != 2 || memory[1] != 0) ++t->failures;
    }
    ++t->prepared;
}
static void destroy(void *c)
{
    Trace *t;
    t = (Trace *)c;
    ++t->destroyed;
}
static void *allocate(void *c, StarsU32 n, StarsU16 tag)
{
    (void)c;
    (void)tag;
    if (n == sizeof planets) { memset(planets, 0xa5, sizeof planets); return planets; }
    if (n == sizeof indices) return indices;
    if (n == sizeof memory) return memory;
    return 0;
}
static void free_memory(void *c, void *p, StarsU16 tag)
{
    Trace *t;
    t = (Trace *)c;
    (void)p;
    (void)tag;
    ++t->frees;
}
static void personality1(void *c, StarsI16 p, void *d)
{
    Trace *t;
    t = (Trace *)c;
    (void)d;
    ++t->dispatch[1];
    t->dispatched_player = p;
}
static void personality2(void *c, StarsI16 p, void *d)
{
    Trace *t;
    t = (Trace *)c;
    (void)d;
    ++t->dispatch[2];
    t->dispatched_player = p;
}
static void logit(void *c, StarsI16 p)
{
    Trace *t;
    t = (Trace *)c;
    (void)p;
    ++t->logs;
}
int main(void)
{
    StarsDoAiTurnInput input;
    StarsAiTurnPlayer players[2];
    Trace trace;
    memset(&input, 0, sizeof input);
    memset(players, 0, sizeof players);
    memset(&trace, 0, sizeof trace);
    players[0].personality = (StarsU16)(2U << 13);
    players[1].personality = 0x2797U;
    input.players = players;
    input.player_count = 2;
    input.planet_capacity = 4;
    input.load_game = load;
    input.destroy_game = destroy;
    input.allocate = allocate;
    input.free_memory = free_memory;
    input.compute_design_powers = simple;
    input.mark_planets_under_attack = simple;
    input.increase_minefield_sizes = simple;
    input.init_random_planet_list = simple;
    input.personalities[1] = personality1;
    input.personalities[2] = personality2;
    input.write_log = logit;
    input.write_history = logit;
    input.context = &trace;
    input.current_player = &trace.current_player;
    trace.current_player = 19;

    stars_do_ai_turn(0, 0xffff, &input);
    if (trace.loaded != 1 || trace.destroyed != 1 || trace.prepared != 4 ||
            trace.dispatch[2] != 1 || trace.dispatched_player != 0 ||
            trace.logs != 2 || trace.frees != 3 || input.ai_active ||
            trace.current_player != 19 || trace.failures)
        return 1;

    memset(&trace, 0, sizeof trace);
    stars_do_ai_turn(1, 0xffff, &input);
    if (trace.loaded != 1 || trace.destroyed != 1 || trace.prepared != 4 ||
            trace.dispatch[1] != 1 || trace.dispatched_player != 1 ||
            trace.dispatch[2] != 0 || trace.logs != 2 || trace.frees != 3 ||
            input.ai_active || trace.current_player != 0 || trace.failures)
        return 2;

    memset(&trace, 0, sizeof trace);
    players[1].dead = 1;
    stars_do_ai_turn(1, 0xffff, &input);
    if (trace.prepared || trace.dispatch[1] || trace.logs != 2 || trace.frees)
        return 3;
    memset(&trace, 0, sizeof trace);
    trace.current_player = 23; trace.fail_load = 1;
    stars_do_ai_turn(1, 0xffff, &input);
    if (trace.current_player != 23 || input.ai_active || trace.loaded != 1 ||
            trace.destroyed != 1 || trace.prepared || trace.logs || trace.frees)
        return 4;
    puts("DO_AI_TURN_SOURCE=PASS");
    return 0;
}
