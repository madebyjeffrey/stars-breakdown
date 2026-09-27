#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsDrawScorePlayerState players[2];
    StarsI16 ids[16], formatted[16];
    StarsU16 id_count, format_count, right_count, pat_count, color_count;
    StarsU32 colors[32];
} Trace;
static const StarsU8 *name(void *v, StarsI16 i){(void)v;return i==0?(const StarsU8*)"AB":(const StarsU8*)"C";}
static StarsI16 get_string(void *v, StarsI16 id, StarsU8 *out, StarsU16 cap){Trace*t=(Trace*)v;if(t->id_count<16)t->ids[t->id_count++]=id;if(cap>3){out[0]=(StarsU8)'H';out[1]=(StarsU8)'D';out[2]=(StarsU8)'R';out[3]=0;}return 3;}
static StarsI16 format_long(void *v, StarsI32 value, StarsU8 *out, StarsU16 cap){Trace*t=(Trace*)v;if(t->format_count<16)t->formatted[t->format_count++]=(StarsI16)value;if(cap>1){out[0]=(StarsU8)'V';out[1]=0;}return 1;}
static StarsU16 select_object(void *v, StarsU16 dc, StarsU16 object){(void)v;(void)dc;(void)object;return 0;}
static void color(void *v, StarsU16 dc, StarsU32 value){Trace*t=(Trace*)v;if(t->color_count<32)t->colors[t->color_count++]=value;(void)dc;}
static void mode(void *v, StarsU16 dc, StarsU16 value){(void)v;(void)dc;(void)value;}
static void pat(void *v, StarsU16 dc, StarsI16 x, StarsI16 y, StarsI16 w, StarsI16 h, StarsU32 rop){Trace*t=(Trace*)v;t->pat_count++;(void)dc;(void)x;(void)y;(void)w;(void)h;(void)rop;}
static void text(void *v, StarsU16 dc, StarsI16 x, StarsI16 y, const StarsU8 *s, StarsU16 n){(void)v;(void)dc;(void)x;(void)y;(void)s;(void)n;}
static StarsI16 extent(void *v, StarsU16 dc, const StarsU8 *s, StarsU16 n){(void)v;(void)dc;(void)s;return (StarsI16)(n*4);}
static void right_text(void *v, StarsU16 dc, StarsI16 x, StarsI16 y, const StarsU8 *s, StarsU16 n, StarsI16 flags){Trace*t=(Trace*)v;t->right_count++;(void)dc;(void)x;(void)y;(void)s;(void)n;(void)flags;}
int main(void){StarsDrawScoreReportInput i;Trace t;memset(&t,0,sizeof t);t.players[0].score_flags=0x4020U;t.players[0].planet_count=10;t.players[0].starbase_count=20;t.players[0].ship_counts[0]=3;t.players[0].ship_counts[1]=4;t.players[0].ship_counts[2]=5;t.players[0].tech_levels=6;t.players[0].resources=100;t.players[0].score=500;t.players[0].overlap_value=1;memset(&i,0,sizeof i);i.player_count=2;i.line_height=10;i.score_x=100;i.font_arial8_small=1;i.font_arial8_large=4;i.brush_button_shadow=7;i.color_button_shadow=0x12345678UL;i.players=t.players;i.player_name=name;i.get_string=get_string;i.format_long=format_long;i.select_object=select_object;i.set_text_color=color;i.set_background_mode=mode;i.pat_blt=pat;i.text_out=text;i.text_extent=extent;i.right_text=right_text;i.context=&t;stars_draw_score_report(3,&i);if(t.id_count!=9||t.ids[0]!=0x1b3||t.ids[8]!=0x1bb||t.pat_count!=13||t.format_count!=9||t.formatted[0]!=10||t.right_count!=18)return 1;puts("DRAW_SCORE_REPORT_SOURCE=PASS");return 0;}
