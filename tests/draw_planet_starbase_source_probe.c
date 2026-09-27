#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 tiles;
    StarsI16 text;
    StarsI16 right;
    StarsI16 references;
    StarsI16 buttons;
    StarsI16 gauges;
    StarsI16 bars;
    StarsI16 colors;
    StarsI16 last_reference;
    StarsI16 gauge_best;
    StarsI16 no_warp;
} Trace;

static const StarsU8 *compressed(void *c, StarsU16 id)
{
    (void)c;
    return id == 0x343 ? (const StarsU8 *)"No base" :
        (const StarsU8 *)"Base";
}
static StarsI16 get_text(void *c, StarsU16 id, StarsU8 *b, StarsU16 n)
{
    const char *text = id == 0x560 ? "-" : "Label";
    StarsI16 i;
    (void)c;
    for (i = 0; i + 1 < (StarsI16)n && text[i] != 0; ++i)
        b[i] = (StarsU8)text[i];
    if (n != 0U) b[i] = 0;
    return i;
}
static StarsI16 draw_tile(void *c, StarsU16 hdc,
    const StarsPlanetStarbaseTile *tile, const StarsU8 *title, StarsRect *r)
{
    Trace *t = (Trace *)c;
    (void)hdc;(void)tile;(void)title;
    ++t->tiles; r->left=10;r->top=4;r->right=170;r->bottom=130; return 1;
}
static void font(void *c, StarsU16 h, StarsU16 f){(void)c;(void)h;(void)f;}
static void text(void *c, StarsU16 h, StarsI16 x, StarsI16 y,
    const StarsU8 *b, StarsI16 n){(void)h;(void)x;(void)y;(void)b;(void)n;++((Trace*)c)->text;}
static void right(void *c, StarsU16 h, StarsI16 x, StarsI16 y,
    const StarsU8 *b, StarsI16 n, StarsI16 o){(void)h;(void)x;(void)y;(void)b;(void)n;(void)o;++((Trace*)c)->right;}
static StarsI32 extent(void *c, StarsU16 h, const StarsU8 *b, StarsI16 n){(void)c;(void)h;(void)b;return n*2;}
static void color(void *c, StarsU16 h, StarsU32 v){(void)h;(void)v;++((Trace*)c)->colors;}
static void bar(void *c, StarsU16 h, StarsI16 x, StarsI16 y, StarsI16 w,
    StarsI16 z, StarsU32 rop){(void)h;(void)x;(void)y;(void)w;(void)z;(void)rop;++((Trace*)c)->bars;}
static void reference(void *c, StarsU16 n, const StarsRect *r){(void)r;++((Trace*)c)->references;((Trace*)c)->last_reference=(StarsI16)n;}
static void button(void *c, StarsU16 h, const StarsRect *r, StarsI16 k, StarsI16 e){(void)h;(void)r;(void)k;(void)e;++((Trace*)c)->buttons;}
static void gauge(void *c, StarsU16 h, const StarsRect *r, StarsI16 a, StarsI16 b){(void)h;(void)r;(void)a;++((Trace*)c)->gauges;((Trace*)c)->gauge_best=b;}
static StarsI32 shield(void *c, StarsI16 h, StarsI16 p){(void)c;(void)h;(void)p;return 25;}
static StarsI16 warp(void *c, StarsI16 p, StarsI16 *sign){Trace *t=(Trace *)c;(void)p;*sign=0;return t->no_warp ? 0 : 5;}
static StarsI16 format(void *c, StarsU8 *b, StarsU16 n, StarsI32 v, StarsI16 mode){(void)c;(void)n;(void)mode;b[0]=(StarsU8)('0'+(v%10));b[1]=0;return 1;}

int main(void)
{
    StarsPlanetStarbaseTile tile;
    StarsPlanetStarbaseData data;
    StarsPlanetStarbaseOps ops;
    StarsPlanetStarbaseInput input;
    StarsRect references[17];
    Trace trace;

    memset(&tile,0,sizeof tile); memset(&data,0,sizeof data);
    memset(&ops,0,sizeof ops); memset(&input,0,sizeof input);
    memset(references,0,sizeof references); memset(&trace,0,sizeof trace);
    tile.flags=0x0800U; data.has_starbase=1; data.hull_id=2;
    data.cargo_capacity=20; data.defense=15; data.mass_warp_packed=80;
    data.planet_id=4;
    ops.compressed_string=compressed;ops.get_text=get_text;ops.draw_tile=draw_tile;
    ops.select_font=font;ops.text_out=text;ops.right_text=right;ops.text_extent=extent;
    ops.set_text_color=color;ops.pat_blt=bar;ops.set_reference=reference;
    ops.draw_button=button;ops.draw_gauge=gauge;ops.shield_value=shield;
    ops.warp_value=warp;ops.format_value=format;ops.context=&trace;
    input.hdc=3;input.line_height=10;input.normal_font=1;input.bold_font=2;
    input.highlight_color=0x7f;input.tile=&tile;input.data=&data;
    input.reference_rects=references;input.ops=&ops;
    stars_draw_planet_starbase(&input);
    if(trace.tiles!=1 || trace.text<6 || trace.right<4 || trace.buttons!=1 ||
        trace.gauges!=1 || trace.gauge_best!=4 ||
        references[13].left!=14 || references[14].left!=14 ||
        (tile.flags&0x0800U)!=0U) return 1;
    trace.no_warp=1; stars_draw_planet_starbase(&input);
    if(references[15].right!=(StarsU16)-6 ||
        references[16].right!=(StarsU16)-6) return 2;
    tile.flags=0x0800U; data.has_starbase=0; stars_draw_planet_starbase(&input);
    if(trace.tiles!=3 || references[14].left!=(StarsU16)-5 ||
        references[16].right!=(StarsU16)-6) return 3;
    puts("DRAW_PLANET_STARBASE_SOURCE=PASS");
#ifdef TARGET_NATIVE
    {
        FILE *report = fopen("C:\\DPSBN\\DPSBR.TXT", "w");
        if (!report)
            return 4;
        fputs("DRAW_PLANET_STARBASE_CHECKS=14 FAILURES=0 RESULT=PASS\n",
            report);
        fclose(report);
    }
#endif
    return 0;
}
