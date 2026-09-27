#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsDrawHistoryPoint points[2];
    StarsDrawHistorySeries series[2];
    StarsRect client;
    StarsI16 ids[16], values[16], format_values[64];
    StarsU16 id_count, value_count, format_count, pat_count, text_count;
    StarsU16 center_count, rect_center_count, diamond_count, pen_count;
    StarsU16 delete_count, move_count, line_count, pixel_count;
} Trace;
static void get_client(void *v, StarsU16 w, StarsRect *r){Trace*t=(Trace*)v;(void)w;*r=t->client;}
static void expand(void *v, StarsRect *r, StarsI16 dx, StarsI16 dy){(void)v;r->left=(StarsU16)((StarsI16)r->left-dx);r->right=(StarsU16)((StarsI16)r->right+dx);r->top=(StarsU16)((StarsI16)r->top-dy);r->bottom=(StarsU16)((StarsI16)r->bottom+dy);}
static void set_rect(void *v, StarsRect *r, StarsI16 l, StarsI16 t, StarsI16 rr, StarsI16 b){(void)v;r->left=(StarsU16)l;r->top=(StarsU16)t;r->right=(StarsU16)rr;r->bottom=(StarsU16)b;}
static const StarsU8 *name(void *v, StarsI16 p){(void)v;return p==0?(const StarsU8*)"P0":(const StarsU8*)"P1";}
static StarsI32 value(void *v, StarsI16 p, StarsI16 n){Trace*t=(Trace*)v;if(t->value_count<16)t->values[t->value_count++]=(StarsI16)(p*10+n);return t->series[p].points[n].value;}
static StarsI16 get_string(void *v, StarsI16 id, StarsU8 *out, StarsU16 cap){Trace*t=(Trace*)v;const char*s=id==0x3c6?"Turn ":"Score!";StarsU16 n=(StarsU16)strlen(s);if(t->id_count<16)t->ids[t->id_count++]=id;if(n+1<cap)strcpy((char*)out,s);return (StarsI16)n;}
static StarsI16 fmt(void *v, StarsI32 n, StarsU8 *out, StarsU16 cap){Trace*t=(Trace*)v;if(t->format_count<64)t->format_values[t->format_count++]=(StarsI16)n;if(cap>1){out[0]=(StarsU8)'N';out[1]=0;}return 1;}
static StarsU16 select_object(void *v, StarsU16 d, StarsU16 o){(void)v;(void)d;(void)o;return 77;}
static void color(void *v, StarsU16 d, StarsU32 c){(void)v;(void)d;(void)c;}
static void mode(void *v, StarsU16 d, StarsU16 m){(void)v;(void)d;(void)m;}
static void pat(void *v, StarsU16 d, StarsI16 x, StarsI16 y, StarsI16 w, StarsI16 h, StarsU32 r){Trace*t=(Trace*)v;t->pat_count++;(void)d;(void)x;(void)y;(void)w;(void)h;(void)r;}
static void text(void *v, StarsU16 d, StarsI16 x, StarsI16 y, const StarsU8 *s, StarsU16 n){Trace*t=(Trace*)v;t->text_count++;(void)d;(void)x;(void)y;(void)s;(void)n;}
static void center(void *v, StarsU16 d, StarsI16 x, StarsI16 y, const StarsU8 *s, StarsU16 n){Trace*t=(Trace*)v;t->center_count++;(void)d;(void)x;(void)y;(void)s;(void)n;}
static void center_rect(void *v, StarsU16 d, const StarsRect *r, const StarsU8 *s, StarsI16 n){Trace*t=(Trace*)v;t->rect_center_count++;(void)d;(void)r;(void)s;(void)n;}
static void right_text(void *v, StarsU16 d, StarsI16 x, StarsI16 y, const StarsU8 *s, StarsU16 n, StarsI16 f){(void)v;(void)d;(void)x;(void)y;(void)s;(void)n;(void)f;}
static StarsU16 pen(void *v, StarsU16 style, StarsI16 w, StarsU32 c){Trace*t=(Trace*)v;t->pen_count++;(void)style;(void)w;(void)c;return 55;}
static void del(void *v, StarsU16 o){Trace*t=(Trace*)v;t->delete_count++;(void)o;}
static void move_to(void *v, StarsU16 d, StarsI16 x, StarsI16 y){Trace*t=(Trace*)v;t->move_count++;(void)d;(void)x;(void)y;}
static void line_to(void *v, StarsU16 d, StarsI16 x, StarsI16 y){Trace*t=(Trace*)v;t->line_count++;(void)d;(void)x;(void)y;}
static void pixel(void *v, StarsU16 d, StarsI16 x, StarsI16 y, StarsU32 c){Trace*t=(Trace*)v;t->pixel_count++;(void)d;(void)x;(void)y;(void)c;}
int main(void){StarsDrawHistoryReportInput i;Trace t;memset(&t,0,sizeof t);t.client.right=400;t.client.bottom=300;t.points[0].turn=0;t.points[0].value=2;t.points[1].turn=10;t.points[1].value=8;t.series[0].points=t.points;t.series[0].point_count=2;t.series[0].color=0x12;t.series[1].points=0;t.series[1].point_count=0;memset(&i,0,sizeof i);i.score_dialog=99;i.player_count=2;i.game_turn=30;i.current_player=0;i.report_type=2;i.arial8_height=10;i.arial10_height=20;i.font_arial8=1;i.font_arial10=2;i.brush_blue=3;i.brush_button_hilite=4;i.brush_button_shadow=5;i.color_button_text=6;i.series=t.series;i.player_name=name;i.get_value=value;i.get_string=get_string;i.format_short=fmt;i.format_long=fmt;i.get_client_rect=get_client;i.expand_rect=expand;i.set_rect=set_rect;i.draw_diamond=(StarsDrawHistoryDiamond)0;i.center_rect_text=center_rect;i.select_object=select_object;i.set_text_color=color;i.set_background_mode=mode;i.pat_blt=pat;i.text_out=text;i.center_text=center;i.right_text=right_text;i.create_pen=pen;i.delete_object=del;i.move_to=move_to;i.line_to=line_to;i.set_pixel=pixel;i.context=&t;stars_draw_history_report(7,&i);if(t.id_count<2||t.ids[0]!=0x3c6||t.ids[1]!=0x1b5||t.rect_center_count!=1||t.pat_count<10||t.pen_count!=1||t.delete_count!=1||t.move_count!=1||t.line_count!=1)return 1;puts("DRAW_HISTORY_REPORT_SOURCE=PASS");return 0;}
