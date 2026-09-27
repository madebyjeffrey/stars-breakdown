#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace { StarsI16 dc, release, frames, text, right, parts, buttons,
    invalidated; } Trace;
static StarsU16 dc(void *p, StarsU16 w){Trace*t=(Trace*)p;(void)w;++t->dc;return 7;}
static void rel(void*p,StarsU16 w,StarsU16 h){Trace*t=(Trace*)p;(void)w;(void)h;++t->release;}
static void font(void*p,StarsU16 h,StarsU16 f){(void)p;(void)h;(void)f;}
static StarsU32 color(void*p,StarsU16 h,StarsU32 c){(void)p;(void)h;return c;}
static StarsU16 str(void*p,StarsU16 id,StarsU8*b,StarsU16 n){(void)p;(void)id;if(n){b[0]='x';b[1]=0;}return 1;}
static const StarsU8 *cmp(void*p,StarsU16 id){(void)p;(void)id;return (const StarsU8*)"x";}
static StarsI16 ext(void*p,StarsU16 h,const StarsU8*s,StarsU16 n){(void)p;(void)h;(void)s;(void)n;return 12;}
static void tx(void*p,StarsU16 h,StarsI16 x,StarsI16 y,const StarsU8*s,StarsU16 n){Trace*t=(Trace*)p;(void)h;(void)x;(void)y;(void)s;(void)n;++t->text;}
static void rt(void*p,StarsU16 h,StarsI16 x,StarsI16 y,const StarsU8*s,StarsU16 n,StarsI16 o){Trace*t=(Trace*)p;(void)h;(void)x;(void)y;(void)s;(void)n;(void)o;++t->right;}
static void fr(void*p,StarsU16 h,const StarsRect*r,StarsI16 n){Trace*t=(Trace*)p;(void)h;(void)r;(void)n;++t->frames;}
static void fill(void*p,StarsU16 h,StarsI16 a,StarsI16 b,StarsI16 c,StarsI16 d,StarsU32 e){(void)p;(void)h;(void)a;(void)b;(void)c;(void)d;(void)e;}
static void sr(void*p,StarsRect*r,StarsI16 a,StarsI16 b,StarsI16 c,StarsI16 d){(void)p;r->left=(StarsU16)a;r->top=(StarsU16)b;r->right=(StarsU16)c;r->bottom=(StarsU16)d;}
static void inv(void*p,StarsU16 h,const StarsRect*r,StarsI16 b){Trace*t=(Trace*)p;(void)h;(void)r;(void)b;++t->invalidated;}
static StarsI16 flag(void*p,StarsI16 a,StarsI16 b){(void)p;(void)a;(void)b;return 0;}
static StarsU32 cost(void*p,StarsI16 a,StarsI16 b,StarsI16 c){(void)p;(void)a;(void)b;(void)c;return 100;}
static StarsI16 part(void*p,StarsU16 a,StarsU16 b,StarsU8 r[6],const StarsU8**n){(void)p;(void)a;if(b>0)return 0;memset(r,0,6);*n=(const StarsU8*)"part";return 1;}
static StarsU16 item(void*p,StarsU16 a,StarsU16 b){(void)p;(void)a;(void)b;return 0;}
static void pos(void*p,StarsU16 a,StarsU16 b,StarsI16 c,StarsI16 d,StarsI16 e,StarsI16 f,StarsU16 g){(void)p;(void)a;(void)b;(void)c;(void)d;(void)e;(void)f;(void)g;}
static void btn(void*p,StarsU16 h,const StarsRect*r,StarsU16 i){Trace*t=(Trace*)p;(void)h;(void)r;(void)i;++t->buttons;}
int main(void){StarsDrawResearchOps o;StarsDrawResearchInput i;Trace t;memset(&o,0,sizeof o);memset(&i,0,sizeof i);memset(&t,0,sizeof t);o.get_dc=dc;o.release_dc=rel;o.select_font=font;o.set_text_color=color;o.set_bk_color=color;o.get_string=str;o.compressed=cmp;o.text_extent=ext;o.text_out=tx;o.center_text=tx;o.right_text=rt;o.draw_frame=fr;o.fill_rect=fill;o.set_rect=sr;o.invalidate_rect=inv;o.race_flag=flag;o.level_cost=cost;o.lookup_part=part;o.get_dialog_item=item;o.set_window_pos=pos;o.draw_button=btn;i.hwnd=2;i.draw_flags=0x7f07;i.dy_arial8=10;i.dx_res_left=200;i.dx_res_right=120;i.dx_res_string_right=40;i.current_technology=1;i.technology_level[1]=3;i.resource_budget=100;i.research[1]=20;i.button_face=0xaaaa;i.window_text=0xbbbb;i.font=4;i.ops=&o;i.context=&t;stars_draw_research_dialog(&i);if(t.dc!=1||t.release!=1||t.frames==0||t.text==0||t.right==0||t.buttons!=2)return 1;i.draw_flags=4;i.hdc=9;stars_draw_research_dialog(&i);if(t.release!=1)return 2;puts("DRAW_RESEARCH_SOURCE=PASS");return 0;}
