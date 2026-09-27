#include "core.h"
#include <stdio.h>
#include <string.h>
typedef struct Trace { StarsPoint point; } Trace;
static StarsI16 find_object(void *c,StarsPoint p,StarsU16 *id)
{ Trace*t=(Trace*)c;t->point=p;*id=2;return 1; }
static StarsI16 random_value(void *c,StarsI16 n){(void)c;(void)n;return 0;}
int main(void)
{
    StarsIdGetBestScannerDestInput input;StarsMacFreighterPlanet planets[1];Trace trace;int failures=0;
    memset(&input,0,sizeof input);memset(planets,0,sizeof planets);memset(&trace,0,sizeof trace);
    input.source_point.x=1000;input.source_point.y=1000;input.map_size=4;input.warp_distance=20;
    planets[0].id=2;planets[0].owner=2;planets[0].point.x=2600;planets[0].point.y=1000;
    input.planets=planets;input.planet_count=1;input.find_object=find_object;input.random_value=random_value;input.context=&trace;
    if(stars_id_get_best_scanner_dest(0,&input)!=2)++failures;
    planets[0].owner=0;
    if(stars_id_get_best_scanner_dest(0,&input)!=-1)++failures;
    printf("ID_GET_BEST_SCANNER_DEST_CHECKS=2 FAILURES=%d RESULT=%s\n",failures,failures?"FAIL":"PASS");return failures!=0;
}
