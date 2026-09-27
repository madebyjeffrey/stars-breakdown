#include <stdio.h>
#include <string.h>
#include "core.h"
typedef struct Trace { StarsU16 ended, end_result, focus, beep; char set_text[16]; } Trace;
static StarsU16 gi(void*c,StarsU16 h,StarsU16 id){(void)c;(void)h;return id+1000U;}
static void gt(void*c,StarsU16 h,StarsU8*t,StarsU16 n){(void)c;(void)n;if(h==1268U){t[0]='3';t[1]=0;}else if(h==1269U){t[0]='4';t[1]=0;}else{t[0]='x';t[1]=0;}}
static void st(void*c,StarsU16 h,const StarsU8*t){Trace*x=(Trace*)c;(void)h;strcpy(x->set_text,(const char*)t);}
static void sm(void*c,StarsU16 h,StarsU16 m,StarsU16 w,StarsU32 l){(void)c;(void)h;(void)m;(void)w;(void)l;}
static void sk(void*c,StarsU16 h,StarsU16 n){(void)h;(void)n;(void)c;}
static void fc(void*c,StarsU16 h,const StarsRect*r,StarsU16 f){(void)c;(void)h;(void)r;(void)f;}
static void al(void*c,const StarsU8*t,StarsI16 n){(void)t;(void)n;((Trace*)c)->focus=0;}
static const StarsU8*fi(void*c,StarsI16 id){(void)c;(void)id;return (const StarsU8*)"bad";}
static void fo(void*c,StarsU16 h){(void)h;((Trace*)c)->focus=1;}
static void en(void*c,StarsU16 h,StarsI16 n){Trace*x=(Trace*)c;(void)h;x->ended=1;x->end_result=(StarsU16)n;}
static void be(void*c,StarsU16 n){(void)n;((Trace*)c)->beep=1;}
int main(void){StarsPrintMapDlgInput i;Trace t;i.page[0]=1;i.page[1]=2;i.button_face_brush=6;i.font_arial8=9;i.get_dialog_item=gi;i.get_window_text=gt;i.set_window_text=st;i.send_message=sm;i.sticky_dialog=sk;i.fill_rect=fc;i.alert=al;i.format_ids=fi;i.set_focus=fo;i.end_dialog=en;i.message_beep=be;i.context=&t;t.ended=0;t.end_result=0;t.focus=0;t.beep=0;t.set_text[0]=0;if(stars_print_map_dlg(&i,12,0x110,0,0)!=1||strcmp(t.set_text,"2"))return 1;if(stars_print_map_dlg(&i,12,0x111,1,0)!=1||i.page[0]!=3U||i.page[1]!=4U||!t.ended)return 2;if(stars_print_map_dlg(&i,12,0x111,0x10cU,0x040004f6UL)!=0||!t.beep)return 3;puts("PRINT_MAP_DLG_SOURCE=PASS");return 0;}
