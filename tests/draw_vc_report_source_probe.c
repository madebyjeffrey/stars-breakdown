#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsDrawVcPlayerState players[2];
    StarsI16 ids[96], values[32];
    StarsU16 id_count, value_count, text_count, bitblt_count, pat_count;
    StarsU16 create_count, delete_count, select_count;
    StarsU32 colors[32];
} Trace;
static const StarsU8 *name(void *v, StarsI16 i){(void)v;return i==0?(const StarsU8*)"AB":(const StarsU8*)"C";}
static StarsI16 get_string(void *v, StarsI16 id, StarsU8 *out, StarsU16 cap){Trace*t=(Trace*)v;if(t->id_count<96)t->ids[t->id_count++]=id;if(cap>1){out[0]=(StarsU8)'L';out[1]=0;}return 1;}
static StarsI16 get_value(void *v, StarsI16 id, StarsI16 unused){Trace*t=(Trace*)v;if(t->value_count<32)t->values[t->value_count++]=id;(void)unused;return (StarsI16)(id+10);}
static StarsI16 check(void *v, StarsI16 id){(void)v;(void)id;return 1;}
static StarsI16 format(void *v, StarsI16 value, StarsU8 *out, StarsU16 cap){(void)v;(void)value;if(cap>1){out[0]=(StarsU8)'1';out[1]=0;}return 1;}
static StarsU16 select_object(void *v, StarsU16 dc, StarsU16 object){Trace*t=(Trace*)v;t->select_count++;(void)dc;(void)object;return 66;}
static void color(void *v, StarsU16 dc, StarsU32 value){Trace*t=(Trace*)v;if(t->text_count<32)t->colors[t->text_count++]=value;(void)dc;}
static void mode(void *v, StarsU16 dc, StarsU16 value){(void)v;(void)dc;(void)value;}
static void pat(void *v, StarsU16 dc, StarsI16 x, StarsI16 y, StarsI16 w, StarsI16 h, StarsU32 rop){Trace*t=(Trace*)v;t->pat_count++;(void)dc;(void)x;(void)y;(void)w;(void)h;(void)rop;}
static void text(void *v, StarsU16 dc, StarsI16 x, StarsI16 y, const StarsU8 *s, StarsU16 n){(void)v;(void)dc;(void)x;(void)y;(void)s;(void)n;}
static StarsI16 extent(void *v, StarsU16 dc, const StarsU8 *s, StarsU16 n){(void)v;(void)dc;(void)s;return (StarsI16)(n*3);}
static StarsU16 create_dc(void *v, StarsU16 dc){Trace*t=(Trace*)v;t->create_count++;(void)dc;return 88;}
static void delete_dc(void *v, StarsU16 dc){Trace*t=(Trace*)v;t->delete_count++;(void)dc;}
static void bitblt(void *v, StarsU16 dc, StarsI16 x, StarsI16 y, StarsI16 w, StarsI16 h, StarsU16 src, StarsI16 sx, StarsI16 sy, StarsU32 rop){Trace*t=(Trace*)v;t->bitblt_count++;(void)dc;(void)x;(void)y;(void)w;(void)h;(void)src;(void)sx;(void)sy;(void)rop;}
int main(void){StarsDrawVcReportInput i;Trace t;memset(&t,0,sizeof t);t.players[0].score_flags=0x4055U;t.players[1].detection_flags=0x100U;t.players[1].dead_flags=1;t.players[1].score_flags=0x0040U;memset(&i,0,sizeof i);i.player_count=2;i.plan_max=100;i.line_height=10;i.score_x=100;i.font_arial8_small=1;i.font_arial8_large=4;i.brush_button_shadow=7;i.bitmap_mono=9;i.color_button_shadow=0x12345678UL;i.percent_suffix=(const StarsU8*)"%";i.players=t.players;i.player_name=name;i.get_string=get_string;i.get_value=get_value;i.get_check=check;i.format_value=format;i.select_object=select_object;i.set_text_color=color;i.set_background_color=color;i.set_background_mode=mode;i.pat_blt=pat;i.text_out=text;i.text_extent=extent;i.create_compatible_dc=create_dc;i.delete_dc=delete_dc;i.bit_blt=bitblt;i.context=&t;stars_draw_vc_report(3,&i);if(t.id_count==0||t.ids[0]!=0x3aa||t.create_count!=1||t.delete_count!=1||t.pat_count<3||t.bitblt_count==0)return 1;if(t.value_count==0||t.values[0]!=0)return 2;puts("DRAW_VC_REPORT_SOURCE=PASS");return 0;}
