#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsU16 tile_calls, text_calls, right_calls, color_calls, reference_calls;
    StarsU16 selected[16], selected_count;
    StarsU32 colors[4];
    StarsI16 right_y[8], right_width[8];
    StarsI16 capacities[2];
    StarsU16 refs[2];
} Trace;

static const StarsU8 *compressed(void *c, StarsU16 id)
{ (void)c; (void)id; return (const StarsU8 *)"Minerals"; }
static StarsI16 get_text(void *c, StarsU16 id, StarsU8 *b, StarsU16 n)
{ const char *s = id == 0x0322 ? "Factory" : "Mineral"; Trace *t=(Trace *)c; (void)t; if(n<2)return 0; strcpy((char *)b,s); return (StarsI16)strlen(s); }
static StarsI16 tile(void *c, StarsU16 h, const StarsPlanetMinSumTile *t,
    StarsRect *r, const StarsU8 *s)
{ Trace *x=(Trace *)c; (void)h;(void)t;(void)s; ++x->tile_calls; r->left=10;r->top=20;r->right=110;r->bottom=80;return 1; }
static StarsU16 select_obj(void *c, StarsU16 h, StarsU16 o)
{ Trace *t=(Trace *)c;(void)h;if(t->selected_count<16)t->selected[t->selected_count++]=o;return 9U; }
static void color(void *c, StarsU16 h, StarsU32 v)
{ Trace *t=(Trace *)c;(void)h;if(t->color_calls<4)t->colors[t->color_calls]=v;++t->color_calls; }
static void text_out(void *c, StarsU16 h, StarsI16 x, StarsI16 y,const StarsU8*s,StarsU16 n)
{ Trace*t=(Trace*)c;(void)h;(void)x;(void)y;(void)s;(void)n;++t->text_calls; }
static void pat(void *c, StarsU16 h, StarsI16 x, StarsI16 y,StarsI16 w,StarsI16 z,StarsU32 r)
{ (void)c;(void)h;(void)x;(void)y;(void)w;(void)z;(void)r; }
static void ref(void *c, StarsU16 i, const StarsRect *r)
{ Trace*t=(Trace*)c;(void)r;++t->reference_calls;if(t->reference_calls<=2)t->refs[t->reference_calls-1]=i; }
static void right(void *c, StarsU16 h, StarsI16 x, StarsI16 y,const StarsU8*s,StarsU16 n,StarsI16 w)
{ Trace*t=(Trace*)c;(void)h;(void)x;(void)s;(void)n;if(t->right_calls<8){t->right_y[t->right_calls]=y;t->right_width[t->right_calls]=w;}++t->right_calls; }
static StarsI16 race(void *c, StarsI16 p, StarsI16 s)
{ (void)c;(void)p;(void)s;return 0; }
static StarsI16 mines(void *c,const StarsPlanetMinSumPlanet*p,StarsI16 i,StarsI16 f)
{ Trace*t=(Trace*)c;(void)p;(void)i;(void)f;t->capacities[0]=12;return 12; }
static StarsI16 factories(void *c,const StarsPlanetMinSumPlanet*p,StarsI16 i,StarsI16 f)
{ Trace*t=(Trace*)c;(void)p;(void)i;(void)f;t->capacities[1]=7;return 7; }
static StarsI16 format_value(void *c,StarsU8*b,StarsU16 n,const StarsU8*f,StarsI32 v,StarsI16 x)
{ (void)c;(void)f;(void)x;if(n<2)return 0;sprintf((char*)b,"%ld",(long)v);return (StarsI16)strlen((char*)b); }

int main(void)
{
    StarsPlanetMinSumInput i;
    StarsPlanetMinSumPlanet p;
    Trace t;
    StarsRect refs[12];
    memset(&i,0,sizeof i); memset(&p,0,sizeof p); memset(&t,0,sizeof t);
    p.minerals[0]=10;p.minerals[1]=20;p.minerals[2]=30;
    p.mineral_capacity_bits=(StarsU32)(0x456U<<8)|(StarsU32)(0x789U<<20);
    i.hdc=3;i.tile.flags=0;i.planet=&p;i.player=2;i.arial8_height=8;
    i.max_mineral_width=11;i.normal_font=1;i.bold_font=2;i.highlight_brush=4;
    i.text_color=0x1234;i.mineral_colors[0]=1;i.mineral_colors[1]=2;i.mineral_colors[2]=3;
    memset(refs,0,sizeof refs);
    i.reference_rects=refs;
    i.ops.compressed_string=compressed;i.ops.get_text=get_text;i.ops.draw_tile=tile;
    i.ops.select_object=select_obj;i.ops.set_text_color=color;i.ops.text_out=text_out;
    i.ops.pat_blt=pat;i.ops.set_reference=ref;i.ops.right_text=right;
    i.ops.race_stat=race;i.ops.max_mines=mines;i.ops.max_factories=factories;
    i.ops.format=format_value;i.context=&t;
    stars_draw_planet_min_sum(&i);
    if(t.tile_calls!=1||t.right_calls!=5||t.text_calls!=5||t.color_calls!=6||
            t.capacities[0]!=12||t.capacities[1]!=7||t.right_y[0]!=20||
            t.right_y[3]!=45||t.right_width[3]!=22)return 1;
    puts("DRAW_PLANET_MIN_SUM_SOURCE=PASS");
    return 0;
}
