#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsU16 dc, selected_font, released_window, released_dc;
    StarsU16 ids[16], items[32], pos_flags[16];
    StarsI16 pos_x[16], pos_y[16], pos_w[16], pos_h[16];
    StarsU16 id_count, item_count, pos_count, maps, offsets, title_id;
    StarsI16 score_x, extent_calls;
    StarsRect window, client, child;
    const StarsU8 *title;
} Trace;

static StarsU16 get_dc(void *v, StarsU16 w){Trace*t=(Trace*)v;t->dc=9;(void)w;return t->dc;}
static void select_font(void *v, StarsU16 d, StarsU16 f){Trace*t=(Trace*)v;t->selected_font=f;(void)d;}
static StarsI16 extent(void *v, StarsU16 d, const StarsU8 *s, StarsU16 n){Trace*t=(Trace*)v;t->extent_calls++;(void)d;(void)s;return n==1?5:20;}
static const StarsU8 *string(void *v, StarsU16 id){Trace*t=(Trace*)v;static const StarsU8 a[]="SCORE";static const StarsU8 b[]="VICTORY";t->ids[t->id_count++]=id;return id==0x1b5U||id==0x3b3U?a:b;}
static void release_dc(void *v, StarsU16 w, StarsU16 d){Trace*t=(Trace*)v;t->released_window=w;t->released_dc=d;}
static void get_rect(void *v, StarsU16 w, StarsRect *r){Trace*t=(Trace*)v;if(w==10U)*r=t->window;else *r=t->child;}
static void get_client(void *v, StarsU16 w, StarsRect *r){Trace*t=(Trace*)v;(void)w;*r=t->client;}
static StarsU16 item(void *v, StarsU16 w, StarsU16 id){Trace*t=(Trace*)v;(void)w;if(t->item_count<32)t->items[t->item_count++]=id;return (StarsU16)(100+id);}
static void map_points(void *v, StarsU16 from, StarsU16 to, StarsRect *r, StarsU16 n){Trace*t=(Trace*)v;t->maps++;r->left=(StarsU16)(r->left+10);(void)from;(void)to;(void)n;}
static void offset(void *v, StarsRect *r, StarsI16 x, StarsI16 y){Trace*t=(Trace*)v;t->offsets++;r->left=(StarsU16)(r->left+x);r->top=(StarsU16)(r->top+y);r->right=(StarsU16)(r->right+x);r->bottom=(StarsU16)(r->bottom+y);}
static void pos(void *v, StarsU16 w, StarsI16 x, StarsI16 y, StarsI16 cx, StarsI16 cy, StarsU16 f){Trace*t=(Trace*)v;StarsU16 n=t->pos_count++;t->pos_flags[n]=f;t->pos_x[n]=x;t->pos_y[n]=y;t->pos_w[n]=cx;t->pos_h[n]=cy;t->items[3]=w;}
static void title(void *v, StarsU16 w, const StarsU8 *s){Trace*t=(Trace*)v;t->title=s;(void)w;}

static void setup(StarsInitScoreDlgInput *i, Trace *t){memset(i,0,sizeof *i);i->victory_mode=0;i->player_count=2;i->line_height=12;i->font_arial8=77;i->score_x=&t->score_x;i->get_dc=get_dc;i->select_object=select_font;i->text_extent=extent;i->get_string=string;i->release_dc=release_dc;i->get_window_rect=get_rect;i->get_client_rect=get_client;i->get_dialog_item=item;i->map_window_points=map_points;i->offset_rect=offset;i->set_window_pos=pos;i->set_window_text=title;i->context=t;}
int main(void){StarsInitScoreDlgInput i;Trace t;memset(&t,0,sizeof t);t.window.left=10;t.window.top=20;t.window.right=210;t.window.bottom=180;t.client.right=180;t.client.bottom=140;t.child.left=30;t.child.top=40;t.child.right=80;t.child.bottom=60;setup(&i,&t);stars_init_score_dlg(10,&i);if(t.score_x!=28||t.ids[0]!=0x1b5U||t.ids[1]!=0x4baU||t.pos_count!=4||t.pos_flags[0]!=6||t.pos_x[2]!=t.pos_x[1]*2+140)return 1;i.victory_mode=2;stars_init_score_dlg(10,&i);if(t.pos_count!=8||t.ids[2]!=0x4bcU)return 2;i.victory_mode=1;stars_init_score_dlg(10,&i);if(t.score_x!=60||t.ids[4]!=0x4bbU)return 3;puts("INIT_SCORE_DLG_SOURCE=PASS");return 0;}
