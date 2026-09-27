#include <stdio.h>
#include "core.h"

static void fill(void *c, StarsU16 hwnd, const StarsRect *rect, StarsU16 brush)
{ StarsI16 *n=(StarsI16 *)c; (void)hwnd;(void)rect;(void)brush;*n=(StarsI16)(*n+1); }
static void draw(void *c, StarsU16 hwnd, StarsU16 flags)
{ StarsI16 *n=(StarsI16 *)c;(void)hwnd;if(flags==3U)*n=(StarsI16)(*n+1); }
static void release(void *c, StarsU16 hwnd, StarsU16 dc)
{ StarsI16 *n=(StarsI16 *)c;(void)hwnd;(void)dc;*n=(StarsI16)(*n+1); }
int main(void)
{
    StarsReflowColumnInput input;
    StarsI16 calls=0;
    input.selected_object=1;input.arial8_height=8;input.hwnd_planet=4;
    input.hdc=5;
    input.button_brush=2;input.fill_rect=fill;input.draw_plan_ship=draw;
    input.release_dc=release;input.context=&calls;
    input.columns[0].column_flags=0;input.columns[0].object_flags=1;
    input.columns[1].column_flags=0x80|0;input.columns[1].height=20;
    input.columns[1].object_flags=2;
    input.columns[2].column_flags=1;input.columns[2].object_flags=4;
    input.columns[3].column_flags=2;input.columns[4].column_flags=3;
    input.columns[5].column_flags=4;
    stars_reflow_column(&input,0,-1,1);
    if(input.columns[0].top!=4||input.columns[1].top!=19||
            (input.columns[0].column_flags&0x800U)==0||calls!=3)return 1;
    puts("REFLOW_COLUMN_SOURCE=PASS");return 0;
}
