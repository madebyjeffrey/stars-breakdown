#include <stdio.h>
#include <string.h>
#include "core.h"
typedef struct Trace { StarsI16 values[32]; StarsI16 random_values[16]; StarsI16 count; StarsI16 random_index; } Trace;
static StarsI16 random_value(void *context, StarsI16 limit) { Trace *t=(Trace *)context; (void)limit; return t->random_values[t->random_index++]; }
static void message(void *context, StarsI16 player, StarsI16 kind, StarsI16 severity, StarsI32 object, StarsI32 value, StarsI16 extra)
{ Trace *t=(Trace *)context; t->values[t->count++]=player; t->values[t->count++]=kind; t->values[t->count++]=severity; t->values[t->count++]=(StarsI16)object; t->values[t->count++]=(StarsI16)value; t->values[t->count++]=extra; }
int main(void)
{
    StarsU8 dead[2]; StarsCheaterFleet fleets[1]; StarsCheaterPlanet planets[1];
    StarsSpankCheatersInput input; Trace trace;
    memset(dead,0,sizeof dead); memset(fleets,0,sizeof fleets); memset(planets,0,sizeof planets);
    memset(&input,0,sizeof input); memset(&trace,0,sizeof trace);
    dead[1]=1; fleets[0].id=7; fleets[0].owner=1; fleets[0].cargo[0]=1000UL;
    planets[0].id=9; planets[0].owner=1; planets[0].mine_count=1000UL;
    trace.random_values[0]=1; trace.random_values[1]=0; trace.random_values[2]=0;
    input.turn=10; input.player_count=2; input.dead_players=dead;
    input.fleets=fleets; input.fleet_count=1; input.planets=planets; input.planet_count=1;
    input.random=random_value; input.send_message=message; input.context=&trace;
    stars_spank_the_cheaters(&input);
    if (fleets[0].cargo[0] != 900UL || trace.values[1] != 0x0105 ||
            planets[0].mine_count != 950UL || trace.values[7] != 0x0106) return 1;
    memset(&trace,0,sizeof trace); fleets[0].flags=0; fleets[0].cargo[0]=1000UL;
    trace.random_values[0]=0; stars_spank_the_cheaters(&input);
    if ((fleets[0].flags & 0x0400U)==0U || trace.values[1] != 0x0104) return 2;
    puts("SPANK_THE_CHEATERS_SOURCE=PASS"); return 0;
}
