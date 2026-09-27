#include <stdio.h>
#include <string.h>
#include "core.h"
typedef struct Trace { StarsI16 dcs; StarsI16 strings; StarsI16 texts; StarsI16 rights; StarsI16 buttons; StarsI16 percents; StarsI16 values; StarsI16 last_value; } Trace;
static StarsU16 dc(void *c, StarsU16 h){(void)h;++((Trace*)c)->dcs;return 3;}
static void release(void *c, StarsU16 h, StarsU16 d){(void)c;(void)h;(void)d;}
static void rect(void *c, StarsU16 h, StarsRect *r){(void)c;(void)h;r->left=10;r->top=10;r->right=100;r->bottom=30;}
static void map(void *c, StarsU16 h, StarsRect *r){(void)c;(void)h;(void)r;}
static void font(void *c, StarsU16 h, StarsU16 f){(void)c;(void)h;(void)f;}
static StarsI16 extent(void *c, StarsU16 h, const StarsU8 *s, StarsI16 n){(void)c;(void)h;(void)s;return n*2;}
static StarsI16 string(void *c, StarsU16 id, StarsU8 *b, StarsU16 n){const char *v; ++((Trace*)c)->strings; v=id==0x0af4U?"%":"L"; if(n>1){b[0]=(StarsU8)v[0];b[1]=0;return 1;}return 0;}
static StarsI16 value(void *c, StarsI16 i, StarsI16 j){Trace*t=(Trace*)c;(void)j;++t->values;t->last_value=i;return (StarsI16)(i+2);}
static void text(void *c, StarsU16 h, StarsI16 x, StarsI16 y, const StarsU8 *s, StarsI16 n){(void)h;(void)x;(void)y;(void)s;(void)n;++((Trace*)c)->texts;}
static void right(void *c, StarsU16 h, StarsI16 x, StarsI16 y, const StarsU8 *s, StarsI16 n, StarsI16 w){Trace*t=(Trace*)c;(void)h;(void)x;(void)y;(void)w;++t->rights;if(n>0&&s[n-1]=='%')++t->percents;}
static void button(void *c, StarsU16 h, const StarsRect *r, StarsU16 s){(void)h;(void)r;(void)s;++((Trace*)c)->buttons;}
int main(void)
{
    StarsDrawNewGame3Ops ops; StarsDrawNewGame3Input input; StarsRect refs[36]; StarsI16 count; Trace trace; signed char packed[18]; StarsI16 i;
    memset(&ops,0,sizeof ops);memset(&input,0,sizeof input);memset(refs,0,sizeof refs);memset(&trace,0,sizeof trace);
    ops.get_dc=dc;ops.release_dc=release;ops.get_window_rect=rect;ops.map_window_rect=map;ops.select_font=font;ops.text_extent=extent;ops.get_string=string;ops.get_value=value;ops.text_out=text;ops.right_text=right;ops.draw_button=button;ops.context=&trace;
    input.hwnd=4;input.line_height=10;input.bold_font=1;input.normal_font=2;input.button_style=4;input.reference_rects=refs;input.reference_count=&count;input.ops=&ops;
    input.column_widths[0][0]=1;input.column_widths[0][1]=1;input.column_widths[1][0]=1;input.column_widths[1][1]=1;
    stars_draw_new_game3(&input,-1);
    if(trace.dcs!=1||count<4||trace.buttons!=count||count%2!=0||
        trace.texts==0||trace.rights==0)return 1;
    input.column_widths[0][0] = -1;
    memset(&trace, 0, sizeof trace);
    stars_draw_new_game3(&input,-1);
    if(trace.percents == 0)return 2;
    for(i=0;i<18;++i)packed[i]=1;
    packed[0]=-1; packed[1]=0; packed[2]=2; packed[3]=-1;
    memset(&trace, 0, sizeof trace);
    input.packed_column_widths=packed; input.packed_column_width_count=18;
    memset(refs,0,sizeof refs);
    stars_draw_new_game3(&input,-1);
    if(trace.values!=17||trace.last_value!=16||trace.percents!=2||count!=34)return 3;
    if(refs[0].left==0||refs[32].left==0)return 4;
    puts("DRAW_NEW_GAME3_PACKED_E1_CHECKS=7 FAILURES=0 RESULT=PASS");
#ifdef TARGET_NATIVE
    {
        FILE *report = fopen("C:\\DNG3P\\DNG3PE1.TXT", "w");
        if (!report)
            return 3;
        fprintf(report, "DRAW_NEW_GAME3_PACKED_E1_CHECKS=7 FAILURES=0 RESULT=PASS\n");
        fclose(report);
    }
#endif
    return 0;
}
