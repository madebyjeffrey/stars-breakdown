#include <stdio.h>
#include <string.h>
#include "core.h"
typedef struct Trace { StarsI16 tiles; StarsI16 shown; StarsI16 positions; StarsI16 buttons; StarsI16 texts; StarsI16 enables; StarsI16 tile_result; } Trace;
static const StarsU8 *string(void *c, StarsU16 id){(void)c;(void)id;return (const StarsU8*)"X";}
static StarsI16 tile(void *c, StarsU16 h, StarsPlanetProductionTile *t, const StarsU8 *s, StarsRect *r){(void)h;(void)t;(void)s;++((Trace*)c)->tiles;r->left=4;r->top=4;r->right=180;r->bottom=140;return ((Trace*)c)->tile_result;}
static void show(void *c, StarsU16 h, StarsU16 v){(void)h;(void)v;++((Trace*)c)->shown;}
static void enable(void *c, StarsU16 h, StarsU16 v){(void)h;(void)v;++((Trace*)c)->enables;}
static void pos(void *c, StarsU16 h, StarsU16 z, StarsI16 x, StarsI16 y, StarsI16 w, StarsI16 d, StarsU16 f){(void)h;(void)z;(void)x;(void)y;(void)w;(void)d;(void)f;++((Trace*)c)->positions;}
static StarsI16 selection(void *c, StarsU16 h){(void)c;(void)h;return 0;}
static void font(void *c, StarsU16 h, StarsU16 f){(void)c;(void)h;(void)f;}
static StarsI16 extent(void *c, StarsU16 h, const StarsU8 *s, StarsI16 n){(void)c;(void)h;(void)s;return n;}
static void text(void *c, StarsU16 h, StarsI16 x, StarsI16 y, const StarsU8 *s, StarsI16 n){(void)h;(void)x;(void)y;(void)s;(void)n;++((Trace*)c)->texts;}
static void right(void *c, StarsU16 h, StarsI16 x, StarsI16 y, const StarsU8 *s, StarsI16 n, StarsI16 o){(void)h;(void)x;(void)y;(void)s;(void)n;(void)o;++((Trace*)c)->texts;}
static void button(void *c, StarsU16 h, const StarsRect *r, StarsI16 k, StarsI16 e){(void)h;(void)r;(void)k;(void)e;++((Trace*)c)->buttons;}
int main(void)
{
    StarsPlanetProductionTile tile_state; StarsPlanetProductionData data; StarsPlanetProductionOps ops; StarsPlanetProductionInput input; StarsRect reference; Trace trace;
    memset(&tile_state,0,sizeof tile_state);memset(&data,0,sizeof data);memset(&ops,0,sizeof ops);memset(&input,0,sizeof input);memset(&trace,0,sizeof trace);
    tile_state.flags=0x0800;data.has_production=1;data.queue_count=2;data.planet_name[0]='P';data.planet_name[1]=0;data.eta[0]='E';data.eta[1]=0;
    ops.compressed_string=string;ops.draw_tile=tile;ops.show_window=show;ops.enable_window=enable;ops.set_window_pos=pos;ops.selection=selection;ops.select_font=font;ops.text_extent=extent;ops.text_out=text;ops.right_text=right;ops.draw_button=button;ops.context=&trace;
    trace.tile_result=1;input.hdc=3;input.line_height=10;input.compact_rows=0;input.normal_font=1;input.bold_font=2;input.list_window=9;input.buttons[0]=10;input.buttons[1]=11;input.reference_rect=&reference;input.tile=&tile_state;input.data=&data;input.ops=&ops;
    stars_draw_planet_production(&input);
    if(trace.tiles!=1||trace.shown<3||trace.positions<3||trace.buttons!=1||trace.enables!=1||trace.texts<3||(tile_state.flags&0x0800U)!=0)return 1;
    data.has_production=0;trace.tile_result=0;tile_state.flags=0x0800;stars_draw_planet_production(&input);if(trace.shown<6||reference.left!=(StarsU16)-5)return 2;
    puts("DRAW_PLANET_PRODUCTION_SOURCE=PASS");
#ifdef TARGET_NATIVE
    {
        FILE *report = fopen("C:\\DPROD\\DPR.TXT", "w");
        if (!report)
            return 4;
        fputs("DRAW_PLANET_PRODUCTION_CHECKS=9 FAILURES=0 RESULT=PASS\n",
            report);
        fclose(report);
    }
#endif
    return 0;
}
