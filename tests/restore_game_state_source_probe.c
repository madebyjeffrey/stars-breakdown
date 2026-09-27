#include <stdio.h>
#include "core.h"
typedef struct Trace { StarsU16 submenu, menu_flags, refit, invalidated; } Trace;
static StarsU16 sub(void *c, StarsU16 w, StarsU16 n){(void)c;(void)w;(void)n;return 55;}
static void menu(void *c, StarsU16 m, StarsU16 item, StarsU16 flags){Trace*t=(Trace*)c;(void)item;t->submenu=m;t->menu_flags=flags;}
static void refit(void *c){((Trace*)c)->refit=1;}
static void inv(void *c, StarsU16 a, StarsU16 b){Trace*t=(Trace*)c;(void)a;(void)b;t->invalidated=1;}
int main(void){StarsRestoreGameStateInput i;Trace t;StarsU16 n;for(n=0;n<13U;++n){i.zip_prod_current[n][0]=(StarsU8)n;i.zip_prod_current[n][1]=(StarsU8)(n+1U);i.zip_prod_saved[n][0]=0;i.zip_prod_saved[n][1]=0;i.tutor_zpq[n]=(StarsU16)((n<<8)|(n+3U));}t.submenu=0;t.menu_flags=0;t.refit=0;t.invalidated=0;i.grbit_scan=7;i.scan_zoom=2;i.tutor_grbit_scan=9;i.tutor_scan_zoom=-2;i.zip_current_valid=9;i.zip_saved_valid=0;i.tutor_zip_valid=0x0010U;i.game_grbits=0x8000U;i.frame_window=3;i.report_sort=1;i.tutor_report_sort=4;i.get_sub_menu=sub;i.check_menu=menu;i.refit_children=refit;i.invalidate_report=inv;i.context=&t;stars_restore_game_state(&i);if(i.grbit_scan!=9||i.scan_zoom!=-2||i.zip_saved_valid!=9||i.zip_current_valid!=1||i.zip_prod_saved[2][0]!=2U||i.zip_prod_current[2][0]!=5U||i.zip_prod_current[2][1]!=2U||t.submenu!=55U||t.menu_flags!=8U||t.refit!=1||i.report_sort!=4||t.invalidated!=1)return 1;i.tutor_zip_valid=0;i.game_grbits=0;i.report_sort=4;t.refit=0;t.invalidated=0;stars_restore_game_state(&i);if(t.refit!=0||t.invalidated!=0)return 2;puts("RESTORE_GAME_STATE_SOURCE=PASS");return 0;}
