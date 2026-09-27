#include <stdio.h>
#include "core.h"

typedef struct Trace { StarsI16 values[4]; StarsI16 count; } Trace;
static StarsI16 random_value(void *value, StarsI16 bound)
{ Trace *t=(Trace *)value; (void)bound; return t->values[t->count++]; }

int main(void)
{
    StarsTorpHitToken token;
    Trace trace;
    StarsI32 result;
    token.jam_percent=10U; trace.values[0]=0; trace.values[1]=60;
    trace.values[2]=54; trace.count=0;
    result=stars_c_torp_hit(3L,&token,50,20,random_value,&trace);
    if(result!=2L||trace.count!=3)return 1;
    token.jam_percent=0U; trace.count=0;
    result=stars_c_torp_hit(250L,&token,50,0,random_value,&trace);
    if(result!=125L||trace.count!=0)return 2;
    result=stars_c_torp_hit(5L,&token,0,20,random_value,&trace);
    if(result!=0L)return 3;
    puts("C_TORP_HIT_SOURCE=PASS"); return 0;
}
