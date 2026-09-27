#include <stdio.h>
#include <string.h>
#include "core.h"
typedef struct Trace { StarsU16 message, item, hdc; StarsU32 lparam; StarsI16 inflate, selected, listbox, focus; } Trace;
static void send(void *c, StarsU16 w, StarsU16 m, StarsU16 id, StarsU32 l){Trace*t=(Trace*)c;(void)w;t->message=m;t->item=id;t->lparam=l;}
static void draw(void *c, StarsU16 h, StarsRect *r, StarsU8 *s, StarsI16 i, StarsI16 x, StarsI16 l){Trace*t=(Trace*)c;(void)r;(void)s;t->hdc=h;t->inflate=i;t->selected=x;t->listbox=l;}
static void focus(void *c, const StarsDrawCbItemInput *i, StarsI16 v){Trace*t=(Trace*)c;(void)i;t->focus=v;}
int main(void){StarsDrawCbItemInput i;Trace t;StarsU8 text[8];memset(&i,0,sizeof i);memset(&t,0,sizeof t);i.hwnd_item=20;i.item_id=3;i.item_state=1;i.hdc=44;i.fleet_comp_list=20;i.planet_prod_list=21;i.work_text=text;i.send_message=send;i.draw_production=draw;i.handle_focus=focus;i.context=&t;stars_draw_cb_entire_item(&i,2);if(t.message!=0x40aU||t.item!=3||t.lparam!=0x112057a4UL||t.hdc!=44||t.inflate!=-2||t.selected!=1||t.listbox!=1||t.focus!=0)return 1;i.hwnd_item=30;i.item_state=0;memset(&t,0,sizeof t);stars_draw_cb_entire_item(&i,-3);if(t.message!=0x408U||t.inflate!=-3||t.listbox!=0||t.focus!=-1)return 2;puts("DRAW_CB_ITEM_SOURCE=PASS");return 0;}
