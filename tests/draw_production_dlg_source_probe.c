#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace { StarsI16 dcs; StarsI16 fonts; StarsI16 texts; StarsI16 rights; StarsI16 diamonds; StarsI16 costs; StarsI16 packed_costs; StarsI16 etas; StarsI16 last_cost; StarsI16 last_list; StarsI16 last_packed_index; StarsI16 source_seen; StarsI16 planet_seen; StarsU32 source_packed; StarsU32 planet_packed; StarsU32 last_packed; StarsI16 formats; StarsI16 colors; StarsI32 last_value; } Trace;
static StarsU16 getdc(void *c, StarsU16 h){(void)h;++((Trace*)c)->dcs;return 3;}
static void release(void *c, StarsU16 h, StarsU16 dc){(void)c;(void)h;(void)dc;}
static void font(void *c, StarsU16 dc, StarsU16 f){(void)dc;(void)f;++((Trace*)c)->fonts;}
static void bk(void *c, StarsU16 dc, StarsU32 v){(void)c;(void)dc;(void)v;}
static void color(void *c, StarsU16 dc, StarsU32 v){(void)dc;(void)v;++((Trace*)c)->colors;}
static StarsI16 extent(void *c, StarsU16 dc, const StarsU8 *s, StarsI16 n){(void)c;(void)dc;(void)s;return n;}
static void text(void *c, StarsU16 dc, StarsI16 x, StarsI16 y, const StarsU8 *s, StarsI16 n){(void)dc;(void)x;(void)y;(void)s;(void)n;++((Trace*)c)->texts;}
static void right(void *c, StarsU16 dc, StarsI16 x, StarsI16 y, const StarsU8 *s, StarsI16 n, StarsI16 o){(void)dc;(void)x;(void)y;(void)s;(void)n;(void)o;++((Trace*)c)->rights;}
static const StarsU8 *str(void *c, StarsU16 id){(void)c;(void)id;return (const StarsU8*)"X";}
static void rect(void *c, StarsU16 h, StarsRect *r){(void)c;(void)h;r->left=10;r->top=10;r->right=150;r->bottom=80;}
static StarsU16 item(void *c, StarsU16 h, StarsU16 id){(void)c;(void)h;return id+10;}
static StarsI16 selection(void *c, StarsU16 h){(void)c;(void)h;return 1;}
static StarsI16 costs(void *c, StarsI16 i, StarsI32 v[4]){Trace*t=(Trace*)c;t->last_cost=i;v[0]=1;v[1]=20;v[2]=300;v[3]=4000;++t->costs;return 1;}
static StarsI16 packed_costs(void *c, StarsI16 list, StarsI16 i, StarsU32 packed, StarsI32 v[4]){Trace*t=(Trace*)c;t->last_list=list;t->last_packed_index=i;t->last_packed=packed;if(list==0){++t->source_seen;t->source_packed=packed;}else{++t->planet_seen;t->planet_packed=packed;}v[0]=11;v[1]=220;v[2]=3300;v[3]=44000;++t->packed_costs;return packed!=0UL;}
static StarsI16 eta(void *c, StarsI16 i, StarsI16 y){(void)i;(void)y;++((Trace*)c)->etas;return 1;}
static StarsI16 format_number(void *c, StarsI32 value, StarsU8 *out, StarsU16 cap){Trace*t=(Trace*)c;t->last_value=value;++t->formats;if(cap<2)return 0;if(value==1)strcpy((char*)out,"1");else if(value==20)strcpy((char*)out,"20");else if(value==300)strcpy((char*)out,"300");else strcpy((char*)out,"4000");return (StarsI16)strlen((char*)out);}
static StarsU32 mineral_color(void *c, StarsU16 row){(void)row;++((Trace*)c)->colors;return 0x100UL+row;}
static void diamond(void *c, StarsU16 dc, const StarsRect *r){(void)dc;(void)r;++((Trace*)c)->diamonds;}
int main(void)
{
    StarsDrawProductionDlgOps ops; StarsDrawProductionDlgInput input; StarsRect diamond_rect; Trace trace;
    memset(&ops,0,sizeof ops);memset(&input,0,sizeof input);memset(&trace,0,sizeof trace);
    ops.get_dc=getdc;ops.release_dc=release;ops.select_font=font;ops.set_background_color=bk;ops.set_text_color=color;ops.text_extent=extent;ops.text_out=text;ops.right_text=right;ops.compressed_string=str;ops.get_label=str;ops.get_window_rect=rect;ops.screen_to_client=rect;ops.get_dialog_item=item;ops.selection=selection;ops.get_costs=costs;ops.get_eta=eta;ops.format_number=format_number;ops.get_mineral_color=mineral_color;ops.draw_diamond=diamond;ops.context=&trace;
    input.hwnd=4;input.line_height=10;input.max_mineral_width=12;input.normal_font=1;input.bold_font=2;input.face_color=7;input.diamond_rect=&diamond_rect;input.ops=&ops;
    stars_draw_production_dlg(&input,-1);
    if(trace.dcs!=1||trace.costs!=2||trace.last_cost!=0||trace.etas!=1||trace.formats!=8||trace.colors<16||trace.last_value!=4000||trace.diamonds!=1||trace.texts<6||trace.rights<8)return 1;
    {
        StarsU32 source_items[2];
        StarsU32 planet_items[2];
        source_items[0]=0x00123456UL; source_items[1]=0x0abcdeffUL;
        planet_items[0]=0x00112233UL; planet_items[1]=0x00fedcbaUL;
        memset(&trace,0,sizeof trace);
        ops.get_costs=(StarsDrawProductionDlgCosts)0;
        ops.get_packed_costs=packed_costs;
        input.source_items=source_items; input.source_item_count=2;
        input.planet_items=planet_items; input.planet_item_count=2;
        stars_draw_production_dlg(&input,-1);
        if(trace.packed_costs!=2 || trace.source_seen!=1 ||
            trace.source_packed!=source_items[1] || trace.planet_seen!=1 ||
            trace.planet_packed!=planet_items[0] || trace.last_list!=1 ||
            trace.last_packed_index!=0 || trace.last_packed!=planet_items[0] ||
            trace.formats!=8 || trace.rights<8) return 1;
        input.source_item_count=1; input.planet_item_count=0;
        memset(&trace,0,sizeof trace);
        stars_draw_production_dlg(&input,-1);
        if(trace.packed_costs!=0) return 1;
    }
    puts("DRAW_PRODUCTION_DLG_SOURCE=PASS");return 0;
}
