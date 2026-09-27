#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 values[16];
    StarsI16 random_values[8];
    StarsI16 count;
    StarsI16 random_index;
} Trace;
static StarsI16 random_value(void *context, StarsI16 limit)
{ Trace *t=(Trace *)context; (void)limit; return t->random_values[t->random_index++]; }
static void queue(void *context, StarsI16 item, StarsI16 kind, StarsI16 count,
    StarsI16 flags)
{ Trace *t=(Trace *)context; t->values[t->count++]=item; t->values[t->count++]=kind; t->values[t->count++]=count; t->values[t->count++]=flags; }

int main(void)
{
    StarsAttackFleetPlanet planet;
    StarsAttackFleetInput input;
    StarsU16 flags[12];
    Trace trace;
    memset(&planet, 0, sizeof planet);
    memset(&input, 0, sizeof input);
    memset(flags, 0, sizeof flags);
    memset(&trace, 0, sizeof trace);
    planet.max_mines=200; planet.operating_mines=200;
    planet.max_factories=200; planet.operating_factories=200;
    flags[6]=0U; flags[7]=0U; flags[8]=0U; flags[9]=0x0200U;
    trace.random_values[0]=40; trace.random_values[1]=95;
    input.planet=&planet; input.best_destroyer=2; input.best_battle=0;
    input.best_starbase_defender=3; input.battle_design_flags=flags;
    input.battle_design_count=12U; input.random=random_value;
    input.add_to_queue=queue; input.context=&trace;
    if (stars_add_attack_fleet(&input)!=2 || trace.count != 4 ||
            trace.values[0] != 3 || trace.values[1] != 1)
        return 1;
    memset(&trace, 0, sizeof trace);
    trace.random_values[0]=80; trace.random_values[1]=95; trace.random_values[2]=0;
    input.best_battle=0; input.best_starbase_defender=3;
    if (stars_add_attack_fleet(&input)!=1 || trace.count != 12 ||
            trace.values[0] != 6 || trace.values[4] != 7 ||
            trace.values[8] != 8)
        return 2;
    puts("ADD_ATTACK_FLEET_SOURCE=PASS");
    return 0;
}
