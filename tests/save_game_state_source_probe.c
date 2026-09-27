#include <stdio.h>
#include "core.h"
typedef struct Trace { StarsU16 submenu, menu_flags, refit, invalidated, destroyed; } Trace;
static StarsU16 sub(void*c,StarsU16 w,StarsU16 n){(void)w;(void)n;return ((Trace*)c)->submenu=55;}
static void menu(void*c,StarsU16 s,StarsU16 i,StarsU16 f){Trace*t=(Trace*)c;(void)s;(void)i;t->menu_flags=f;}
static void refit(void*c){((Trace*)c)->refit=1;}
static void inv(void*c,StarsU16 a,StarsU16 b){Trace*t=(Trace*)c;(void)a;(void)b;t->invalidated=1;}
static void destroy(void*c){((Trace*)c)->destroyed=1;}
int main(void){StarsSaveGameStateInput i;Trace t;StarsU16 n;for(n=0;n<13U;++n){i.current_queue[n].count=(StarsU8)(n+1U);i.current_queue[n].no_research=(StarsU8)(n+2U);i.saved_queue[n].count=(StarsU8)(n+5U);i.saved_queue[n].no_research=(StarsU8)(n+6U);i.tutor_zpq[n]=0;}t.submenu=0;t.menu_flags=0;t.refit=0;t.invalidated=0;t.destroyed=0;i.current_grbit_scan=9;i.current_scan_zoom=-2;i.tutor_grbit_scan=0;i.tutor_scan_zoom=0;i.tutor_flags=0xffffU;i.display_flags=0;i.frame_window=3;i.current_valid=1;i.tutor_zip_valid=0;i.report_sort=4;i.report_ascending=0;i.tutor_report_sort=0;i.game_lid=1;i.game_flags=0;i.get_sub_menu=sub;i.check_menu=menu;i.refit_children=refit;i.invalidate_report=inv;i.destroy_game=destroy;i.context=&t;stars_save_game_state(&i);if(i.tutor_grbit_scan!=9U||i.tutor_scan_zoom!=-2||i.display_flags!=0x8000U||i.tutor_zpq[2]!=0x0403U||i.current_queue[2].count!=7U||i.current_queue[2].no_research!=8U||t.submenu!=55U||t.menu_flags!=8U||!t.refit||!t.invalidated||!t.destroyed||i.report_sort!=1||!i.report_ascending||i.tutor_report_sort!=4)return 1;puts("SAVE_GAME_STATE_SOURCE=PASS");return 0;}
