#include <stdio.h>
#include "core.h"

typedef struct Trace { StarsI16 stats, frees; } Trace;
static StarsI16 stat(void *value, StarsI16 player, StarsI16 index)
{ Trace *t=(Trace *)value; t->stats++; (void)player; (void)index; return 0; }
static void free_thing(void *value, StarsPacketDecayThing *thing)
{ Trace *t=(Trace *)value; t->frees++; (void)thing; }

int main(void)
{
    StarsPacketDecayThing thing;
    StarsPacketDecayInput input;
    Trace trace;
    StarsI16 result;

    thing.status_flags=(StarsU16)(2U<<14); thing.owner=3;
    thing.amounts[0]=100; thing.amounts[1]=200; thing.amounts[2]=300;
    trace.stats=0; trace.frees=0; input.race_stat=stat;
    input.free_thing=free_thing; input.context=&trace;
    result=stars_f_packet_decay(&thing,100,&input);
    if(result!=0||thing.amounts[0]!=75||thing.amounts[1]!=150||
       thing.amounts[2]!=225||thing.status_flags!=((2U<<14)|45U)||
       trace.stats!=1||trace.frees!=0)return 1;
    thing.status_flags=(StarsU16)(1U<<14); thing.amounts[0]=1;
    thing.amounts[1]=0; thing.amounts[2]=0; trace.frees=0;
    result=stars_f_packet_decay(&thing,1000,&input);
    if(result!=1||trace.frees!=1||thing.amounts[0]!=0)return 2;
    thing.status_flags=(StarsU16)(3U<<14); thing.amounts[0]=20;
    trace.frees=0; result=stars_f_packet_decay(&thing,100,&input);
    if(result!=0||thing.amounts[0]!=10||thing.status_flags!=((3U<<14)|1U))return 3;
    puts("F_PACKET_DECAY_SOURCE=PASS"); return 0;
}
