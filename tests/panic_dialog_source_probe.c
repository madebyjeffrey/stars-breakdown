#include <stdio.h>
#include <string.h>
#include "core.h"
typedef struct Trace { StarsU16 paint_hwnd,paint_dc,brush; StarsU32 color; StarsI16 result; unsigned ended,helped; } Trace;
static void rect(void*c,StarsU16 h,StarsRect*r){Trace*t=(Trace*)c;(void)h;r->left=1U;r->top=2U;r->right=30U;r->bottom=40U;t->paint_hwnd=h;}
static void fill(void*c,StarsU16 d,const StarsRect*r,StarsU16 b){Trace*t=(Trace*)c;(void)r;t->paint_dc=d;t->brush=b;}
static void color(void*c,StarsU16 d,StarsU32 v){Trace*t=(Trace*)c;(void)d;t->color=v;}
static void end(void*c,StarsU16 h,StarsI16 r){Trace*t=(Trace*)c;(void)h;t->result=r;++t->ended;}
static void help(void*c,StarsU16 h,const StarsU8*f,StarsU16 cmd,StarsU16 id){Trace*t=(Trace*)c;(void)h;(void)f;(void)cmd;(void)id;++t->helped;}
int main(void){StarsPanicDialogInput i;Trace t;memset(&t,0,sizeof(t));i.hwnd=10U;i.message=0x14U;i.wparam=20U;i.lparam=0;i.button_face=7U;i.color_button_face=0x12345678UL;i.tutor_help_id=99U;i.help_file=(const StarsU8*)"HELP";i.get_client_rect=rect;i.fill_rect=fill;i.set_background_color=color;i.end_dialog=end;i.win_help=help;i.context=&t;
 if(stars_panic_dialog(&i)!=1||t.paint_hwnd!=10U||t.paint_dc!=20U||t.brush!=7U)return 1;i.message=0x19U;i.lparam=6UL<<16;if(stars_panic_dialog(&i)!=7||t.color!=0x12345678UL)return 2;i.message=0x111U;i.wparam=2U;if(stars_panic_dialog(&i)!=1||t.result!=0)return 3;i.wparam=0x76U;if(stars_panic_dialog(&i)!=1||t.helped!=1U)return 4;i.wparam=0x9caU;if(stars_panic_dialog(&i)!=1||t.result!=(StarsI16)0x9ca)return 5;i.wparam=1U;if(stars_panic_dialog(&i)!=0)return 6;puts("PANIC_DIALOG_SOURCE=PASS");return 0;}
