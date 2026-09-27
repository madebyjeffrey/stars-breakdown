#include <stdio.h>
#include <string.h>
#include "core.h"
typedef struct T { StarsU16 stock,filled,mode_calls; StarsI16 x,y,sx,sy,focus; StarsU32 color; } T;
static StarsU16 so(void*c,StarsU16 n){T*t=(T*)c;t->stock=n;return 9;}
static void fi(void*c,StarsU16 h,const StarsRect*r,StarsU16 b){T*t=(T*)c;(void)h;(void)r;t->filled=b;}
static void sm(void*c,StarsU16 w,StarsU16 m,StarsU16 id,StarsU32 l){(void)c;(void)w;(void)m;(void)id;(void)l;}
static void sp(void*c,StarsU16 h,StarsU16 p){(void)c;(void)h;(void)p;}
static void rp(void*c,StarsU16 h){(void)c;(void)h;}
static void db(void*c,StarsU16 h,StarsI16 x,StarsI16 y,StarsI16 w,StarsI16 z,StarsU16 b,StarsI16 sx,StarsI16 sy,StarsI16 sw,StarsI16 sh,StarsU32 rop){T*t=(T*)c;(void)h;(void)w;(void)z;(void)b;(void)sw;(void)sh;(void)rop;t->x=x;t->y=y;t->sx=sx;t->sy=sy;}
static StarsU32 tc(void*c,StarsU16 h,StarsU32 n){T*t=(T*)c;(void)h;if(n!=0x12345678UL)t->color=n;return n==0x12345678UL?0x87654321UL:0;}
static StarsU16 bm(void*c,StarsU16 h,StarsU16 n){T*t=(T*)c;(void)h;++t->mode_calls;return n==1U?7U:8U;}
static void to(void*c,StarsU16 h,StarsI16 x,StarsI16 y,const StarsU8*s,StarsU16 n){(void)c;(void)h;(void)x;(void)y;(void)s;(void)n;}
static void hf(void*c,const StarsRect*r,StarsI16 n){T*t=(T*)c;(void)r;t->focus=n;}
int main(void){StarsDrawDlgLBEntireItemInput i;T t;StarsRect r;r.left=10;r.top=20;r.right=74;r.bottom=84;memset(&t,0,sizeof(t));i.hdc=3;i.hwnd_item=4;i.item_id=5;i.item_state=0x10;i.item_rect=r;i.inflate=1;i.arial8_height=8;i.palette=6;i.window_color=0x12345678UL;i.glyph_x='C';i.glyph_y='B';i.text=(const StarsU8*)"abc";i.get_stock_object=so;i.fill_rect=fi;i.send_message=sm;i.select_palette=sp;i.realize_palette=rp;i.dib_blt=db;i.set_text_color=tc;i.set_bk_mode=bm;i.text_out=to;i.handle_focus=hf;i.context=&t;stars_draw_dlg_lb_entire_item(&i);if(t.stock!=4U||t.filled!=9U||t.x!=12||t.y!=21||t.sx!=256||t.sy!=0||t.color!=0x87654321UL||t.mode_calls!=2U||t.focus!=3)return 1;puts("DRAW_DLG_LB_ITEM_SOURCE=PASS");return 0;}
