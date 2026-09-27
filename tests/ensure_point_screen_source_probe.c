#include <stdio.h>
#include "core.h"
typedef struct Trace { StarsRect rect; StarsPoint centered; StarsI16 scroll; } Trace;
static StarsI16 scan(void *c, StarsI16 value){(void)c;return value;}
static StarsI16 inside(void *c, StarsRect r, StarsPoint p){Trace*t=(Trace*)c;t->rect=r;return p.x>=r.left&&p.x<r.right&&p.y>=r.top&&p.y<r.bottom;}
static void center(void *c, StarsPoint p, StarsI16 scroll){Trace*t=(Trace*)c;t->centered=p;t->scroll=scroll;}
int main(void){StarsEnsurePointOnScreenInput i;Trace t;StarsPoint p;t.rect.left=0;t.rect.right=0;t.rect.top=0;t.rect.bottom=0;t.centered.x=0;t.centered.y=0;t.scroll=0;i.dy_sbar=100;i.x_scan_top=100;i.y_scan_top=200;i.d_gal_inv=4000;i.client_right=1500;i.client_bottom=1600;i.scan_to_point=scan;i.point_in_rect=inside;i.center_point=center;i.context=&t;p.x=500;p.y=3200;if(stars_ensure_point_on_screen(p,1,&i)!=1)return 1;p.x=1700;p.y=4000;if(stars_ensure_point_on_screen(p,1,&i)!=0||t.scroll!=1||t.centered.x!=970||t.centered.y!=3260)return 2;p.x=50;p.y=2200;if(stars_ensure_point_on_screen(p,0,&i)!=0||t.centered.x!=790||t.centered.y!=2940)return 3;puts("ENSURE_POINT_SCREEN_SOURCE=PASS");return 0;}
