#include <stdio.h>
#include "common.h"

typedef struct Trace {
    StarsI16 race;
    StarsI16 grbit;
    StarsI16 hidden;
    StarsI16 resolve_calls;
    StarsI16 tech_calls;
    StarsU16 last_group;
    StarsU16 last_item;
} Trace;
static StarsU32 resolve(void *c, StarsU16 group, StarsU16 item)
{ Trace *t=(Trace *)c;++t->resolve_calls;t->last_group=group;t->last_item=item;return ((StarsU32)group<<16)|item; }
static StarsI16 race_stat(void *c, StarsI16 player, StarsI16 kind)
{ (void)player;(void)kind;return ((Trace *)c)->race; }
static StarsI16 race_grbit(void *c, StarsI16 player, StarsI16 bit)
{ (void)player;return ((Trace *)c)->grbit && bit == 0 ? 1 : 0; }
static StarsI16 hidden(void *c, StarsU16 group, StarsU16 item)
{ (void)group;(void)item;return ((Trace *)c)->hidden; }
static StarsI16 tech_status(void *c, StarsU32 component)
{ Trace *t=(Trace *)c;(void)component;++t->tech_calls;return 7; }
static StarsI16 armor_dp(void *c, StarsU32 component)
{ (void)component;return ((Trace *)c)->hidden ? -1 : 1; }

static void initialize(StarsLookupPartInput *i, Trace *t)
{
    i->group=1U;i->item=0U;i->player=0;i->component=0U;
    i->resolve=resolve;i->race_stat=race_stat;i->race_grbit=race_grbit;
    i->should_hide=hidden;i->tech_status=tech_status;i->armor_dp=armor_dp;
    i->context=t;t->race=0;t->grbit=0;t->hidden=0;
    t->resolve_calls=0;t->tech_calls=0;t->last_group=0;t->last_item=0;
}

int main(void)
{
    StarsLookupPartInput input; Trace trace; int failures=0;
    initialize(&input,&trace);
    input.item=15U;
    if(stars_lookup_part(&input)!=-1||trace.tech_calls!=0)++failures;
    trace.grbit=1;
    if(stars_lookup_part(&input)!=7||input.component!=0x0001000fUL||
            trace.tech_calls!=1)++failures;

    input.group=2U;input.item=16U;
    if(stars_lookup_part(&input)!=0)++failures;
    input.group=4U;input.item=3U;trace.race=4;
    if(stars_lookup_part(&input)!=7||trace.last_group!=4U||trace.last_item!=3U)
        ++failures;

    input.group=0x4000U;input.item=20U;trace.grbit=0;trace.race=0;
    if(stars_lookup_part(&input)!=-1)++failures;
    input.group=0x8000U;input.item=2U;trace.grbit=1;trace.hidden=1;
    if(stars_lookup_part(&input)!=-1)++failures;
    input.group=0x8000U;input.item=14U;trace.hidden=0;trace.race=0;
    if(stars_lookup_part(&input)!=7)++failures;
    input.group=0x1234U;
    if(stars_lookup_part(&input)!=0)++failures;
#ifndef TARGET_NATIVE
    printf("LOOKUP_PART_CHECKS=8 FAILURES=%d RESULT=%s\n",failures,
        failures?"FAIL":"PASS");
#endif
    return failures!=0;
}
