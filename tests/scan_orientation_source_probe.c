#include <stdio.h>
#include "core.h"
typedef struct Trace { StarsI16 dx,dy; } Trace;
static void orientation(void *c, StarsI16 dx, StarsI16 dy, StarsPoint *p, StarsPoint *d)
{Trace*t=(Trace*)c;t->dx=dx;t->dy=dy;p->x=dx;p->y=dy;d->x=9;d->y=9;}
int main(void)
{StarsScanFleetOrientationInput i;StarsPoint p,d;Trace t;
 i.fleet_player=2;i.local_player=2;i.fleet_cord=3;i.own_order_flags=0x20U;i.own_order_x=8;i.own_order_y=-4;i.enemy_position=0;i.enemy_flags=0;i.orientation=orientation;i.context=&t;
 stars_get_scan_fleet_orientation(&i,&p,&d);if(t.dx!=8||t.dy!=-4||p.x!=8||p.y!=-4)return 1;
 i.fleet_player=3;i.enemy_position=(StarsU16)(140U<<8|120U);i.enemy_flags=0x0013U;stars_get_scan_fleet_orientation(&i,&p,&d);if(t.dx!=-7||t.dy!=13)return 2;
 i.enemy_flags=0x0010U;stars_get_scan_fleet_orientation(&i,&p,&d);if(t.dx!=0||t.dy!=0)return 3;
 i.fleet_player=2;i.fleet_cord=1;i.own_order_flags=0x00f0U;stars_get_scan_fleet_orientation(&i,&p,&d);if(t.dx!=0||t.dy!=0)return 4;
 puts("SCAN_ORIENTATION_SOURCE=PASS");return 0;}
