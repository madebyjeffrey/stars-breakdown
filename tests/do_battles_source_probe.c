#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 calls;
    StarsU16 order[16];
    StarsI16 cools;
    StarsI16 frees;
    StarsI16 bombed;
    StarsI16 link_arg;
    StarsU16 alloc_sizes[2];
    StarsU16 alloc_segments[2];
    StarsU16 free_segments[4];
    void *freed[4];
    StarsU8 table[4];
    StarsU8 current[4];
} Trace;
static int memory_a;
static int memory_b;

static void link_fleets(void *value, StarsI16 post_movement)
{ Trace *t=(Trace *)value; t->link_arg=post_movement; t->order[t->calls++]=1U; }
static void *allocate(void *value, StarsU16 size, StarsU16 segment)
{ Trace *t=(Trace *)value; StarsI16 n=(StarsI16)(t->alloc_sizes[0]?1:0); t->alloc_sizes[n]=size; t->alloc_segments[n]=segment; t->order[t->calls++]=(StarsU16)(size==0x1d00U?2U:3U); return size==0x1d00U?(void *)&memory_a:(void *)&memory_b; }
static void free_memory(void *value, void *memory, StarsU16 segment)
{ Trace *t=(Trace *)value; StarsI16 n=t->frees++; t->freed[n]=memory; t->free_segments[n]=segment; }
static StarsI16 cplr(void *value, const StarsDoBattlesFleet *fleet,
    StarsU16 *ids, StarsU16 *token_count, StarsU16 *battle_count)
{ Trace *t=(Trace *)value; (void)fleet; ids[0]=7U; *token_count=1U; *battle_count=2U; t->order[t->calls++]=4U; return 9; }
static void cool(void *value, const StarsDoBattlesFleet *fleet, StarsI16 battle,
    const StarsU16 *ids, StarsU16 token_count, StarsU16 battle_count)
{ Trace *t=(Trace *)value; (void)fleet; (void)battle; (void)ids; (void)token_count; (void)battle_count; t->cools++; t->order[t->calls++]=5U; }
static void bombing(void *value)
{ Trace *t=(Trace *)value; t->bombed=1; t->order[t->calls++]=6U; }

int main(void)
{
    StarsDoBattlesFleet fleets[3];
    StarsDoBattlesInput input;
    Trace trace;
    memset(&trace,0,sizeof trace); memset(&input,0,sizeof input);
    fleets[0].flags=0x1000U; fleets[0].has_next=1U;
    fleets[1].flags=0x0800U; fleets[1].has_next=1U;
    fleets[2].flags=0; fleets[2].has_next=0U;
    input.fleets=fleets; input.fleet_count=3U; input.link_fleets=link_fleets;
    input.allocate=allocate; input.free_memory=free_memory;
    input.cplr_battle=cplr; input.cool_battle=cool; input.do_bombing=bombing;
    input.battle_table=trace.table; input.battle_current=trace.current;
    input.context=&trace;
    trace.table[0]=1U; trace.table[1]=2U; trace.current[0]=3U; trace.current[1]=4U;
    stars_do_battles(1,&input);
    if (trace.link_arg!=1 || fleets[0].flags!=0 || fleets[1].flags!=0x0800U ||
        trace.cools!=1 || trace.frees!=3 || trace.bombed!=1 || trace.table[0]!=0xffU ||
        trace.table[1]!=0xffU || trace.current[0]!=0xffU ||
        trace.current[1]!=0xffU || input.battle_table!=(void *)0)
        return 1;
    if (trace.order[0]!=1U || trace.order[1]!=2U || trace.order[2]!=3U ||
        trace.order[3]!=4U || trace.order[4]!=5U)
        return 2;
    puts("DO_BATTLES_SOURCE=PASS"); return 0;
}
