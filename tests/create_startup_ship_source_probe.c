#include <stdio.h>
#include <string.h>
#include "core.h"
typedef struct T { StarsI16 player, planet, stat; StarsStartupFleetState fleet; } T;
static StarsStartupFleetState *cf(void*c,StarsI16 p,StarsI16 pl){T*t=(T*)c;t->player=p;t->planet=pl;memset(&t->fleet,0,sizeof(t->fleet));return &t->fleet;}
static StarsI32 fs(void*c,const StarsStartupFleetState*f,StarsU16 s){(void)c;(void)f;(void)s;return 0x12345678L;}
int main(void){StarsCreateStartupShipInput i;StarsU8 count=0,table[294];StarsU16 words[73];T t;StarsU16 n;memset(table,0,sizeof(table));for(n=0;n<73U;++n)words[n]=(StarsU16)(n+1U);i.player=2;i.planet=7;i.design_index=0;i.add_design=1;i.design_count=&count;i.design_table=table;i.design_capacity=2;i.design_words=words;i.design_word_count=73;i.design_hull_id=9;i.create_fleet=cf;i.fleet_stat=fs;i.context=&t;if(stars_create_startup_ship(&i)!=0||count!=1U||table[0]!=1U||table[144]!=73U||table[146]!=9U||t.fleet.bytes[12]!=1U||t.fleet.bytes[92]!=0x78U||t.fleet.bytes[95]!=0x12U||t.fleet.bytes[96]!=0U)return 1;puts("CREATE_STARTUP_SHIP_SOURCE=PASS");return 0;}
