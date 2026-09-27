#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsU16 marks;
    StarsU16 balances;
    StarsU16 salvages;
    StarsI16 player;
    StarsI16 survivor_count;
    StarsI16 killed_count;
} Trace;

static void mark(void *c, const void *hull, StarsI16 player)
{ Trace *t = (Trace *)c; (void)hull; ++t->marks; t->player = player; }
static void balance(void *c, StarsKillShipsFleet *source,
    StarsKillShipsFleet *killed)
{ Trace *t = (Trace *)c; ++t->balances; t->survivor_count =
      (StarsI16)source->ship_counts[0]; t->killed_count =
      (StarsI16)killed->ship_counts[0]; source->cargo[0] = 77UL; }
static void salvage(void *c, const StarsKillShipsFleet *fleet)
{ Trace *t = (Trace *)c; (void)fleet; ++t->salvages; }

int main(void)
{
    StarsKillShipsInput input;
    StarsKillShipsFleet fleet;
    StarsKillShipsToken token;
    Trace trace;

    memset(&input, 0, sizeof input);
    memset(&fleet, 0, sizeof fleet);
    memset(&token, 0, sizeof token);
    memset(&trace, 0, sizeof trace);
    fleet.player = 3; fleet.ship_counts[0] = 5; fleet.cargo[0] = 12UL;
    token.player = 7; token.flags = 1U;
    input.token = &token; input.killed_count = 2; input.hull_index = 0;
    input.fleet = &fleet; input.fallout = 1; input.hull_definition = &fleet;
    input.ops.mark_techs = mark; input.ops.balance_cargo = balance;
    input.ops.create_salvage = salvage; input.context = &trace;
    stars_kill_ships(&input);
    if (trace.marks != 1U || trace.balances != 1U || trace.salvages != 1U ||
            trace.player != 7 || trace.survivor_count != 5 ||
            trace.killed_count != 2 || token.ship_count != 3 ||
            fleet.ship_counts[0] != 5U || fleet.cargo[0] != 77UL)
        return 1;

    memset(&trace, 0, sizeof trace);
    memset(&fleet, 0, sizeof fleet);
    token.flags = 1U; token.ship_count = 5;
    fleet.ship_counts[0] = 5;
    input.killed_count = 5; input.fallout = 0;
    stars_kill_ships(&input);
    if (trace.balances != 0U || trace.salvages != 0U ||
            token.ship_count != 0 || (fleet.flags & 0x0400U) == 0U ||
            (token.flags & 1U) != 0U)
        return 2;
    puts("KILL_SHIPS_SOURCE=PASS");
    return 0;
}
