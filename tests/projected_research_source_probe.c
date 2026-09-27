#include <stdio.h>
#include "core.h"
typedef struct Trace { StarsU16 resources, production; StarsI16 last_item; } Trace;
static StarsU32 resources(void *c, StarsU16 n, StarsI16 p){Trace*t=(Trace*)c;(void)p;++t->resources;return n==0U?0x00018000UL:0x00000001UL;}
static StarsU32 production(void *c, StarsU16 n, StarsI16 item){Trace*t=(Trace*)c;(void)n;t->last_item=item;++t->production;return 0x00010001UL;}
int main(void){StarsProjectedResearchInput i;StarsResearchPlanet p[3];Trace t;StarsU8 pct;StarsI32 result;pct=9;t.resources=0;t.production=0;t.last_item=0;p[0].owner=2;p[0].production_active=0;p[1].owner=2;p[1].production_active=1;p[2].owner=1;p[2].production_active=0;i.percentage=255;i.player_percentage=&pct;i.player=2;i.planets=p;i.planet_count=3;i.resources_at_planet=resources;i.production_schedule=production;i.context=&t;result=stars_projected_research_spending(&i);if(result!=0x00058001L||pct!=9||t.resources!=1U||t.production!=1U||t.last_item!=-1)return 1;puts("PROJECTED_RESEARCH_SOURCE=PASS");return 0;}
