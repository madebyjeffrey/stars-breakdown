#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace { StarsI16 paints,colors,titles,positions,edits,gets,texts,sends,fits,stickies,ends,helps; StarsU16 title_id,end_result,help_id; StarsU32 brush,color; StarsU8 work[250]; } Trace;
static void gr(void*v,StarsU16 h,StarsRect*r){Trace*t=(Trace*)v;++t->paints;(void)h;r->left=1;r->top=2;r->right=3;r->bottom=4;}
static void fr(void*v,StarsU16 h,const StarsRect*r,StarsU32 b){Trace*t=(Trace*)v;++t->paints;t->brush=b;(void)h;(void)r;}
static void co(void*v,StarsU16 h,StarsU32 c){Trace*t=(Trace*)v;++t->colors;t->color=c;(void)h;}
static const StarsU8 *gs(void*v,StarsU16 id){Trace*t=(Trace*)v;++t->titles;t->title_id=id;return (const StarsU8 *)"Rename";}
static void st(void*v,StarsU16 h,const StarsU8*s){Trace*t=(Trace*)v;++t->texts;if(h==9U)strcpy((char*)t->work,(const char*)s);}
static void sp(void*v,StarsU16 h,StarsI16 x,StarsI16 y,StarsU16 f){Trace*t=(Trace*)v;++t->positions;(void)h;(void)x;(void)y;(void)f;}
static void se(void*v,StarsU16 d,StarsU16 i,StarsU16 m,StarsU16 w,StarsU32 l){Trace*t=(Trace*)v;++t->edits;(void)d;(void)i;(void)m;(void)w;(void)l;}
static StarsU16 gi(void*v,StarsU16 d,StarsU16 i){Trace*t=(Trace*)v;++t->gets;(void)d;(void)i;return 77U;}
static void gt(void*v,StarsU16 d,StarsU16 i,StarsU8*o,StarsU16 n){Trace*t=(Trace*)v;++t->texts;(void)d;(void)i;(void)n;o[0]='O';o[1]='K';o[2]=0;}
static void gw(void*v,StarsU16 h,StarsU8*o,StarsU16 n){Trace*t=(Trace*)v;++t->texts;(void)h;(void)n;o[0]='T';o[1]='O';o[2]='O';o[3]=0;}
static StarsU32 sm(void*v,StarsU16 h,StarsU16 m,StarsU16 w,StarsU32 l){Trace*t=(Trace*)v;++t->sends;(void)h;(void)m;(void)w;(void)l;return 123UL;}
static StarsI16 fs(void*v,const StarsU8*s,StarsU16 w){Trace*t=(Trace*)v;++t->fits;(void)s;(void)w;return 0;}
static void sk(void*v,StarsU16 d,StarsPoint*p,StarsI16 save){Trace*t=(Trace*)v;++t->stickies;(void)d;(void)p;(void)save;}
static void ed(void*v,StarsU16 d,StarsU16 r){Trace*t=(Trace*)v;++t->ends;t->end_result=r;(void)d;}
static void hp(void*v,StarsU16 d,const StarsU8*f,StarsU16 i){Trace*t=(Trace*)v;++t->helps;t->help_id=i;(void)d;(void)f;}
int main(void){StarsRenameInput i;Trace t;memset(&i,0,sizeof i);memset(&t,0,sizeof t);i.sticky_position.x=10;i.sticky_position.y=20;i.button_face_brush=55UL;i.button_face_color=66UL;i.work[0]='O';i.work[1]='K';i.work[2]=0;i.help_file=(const StarsU8*)"HELP.HLP";i.get_client_rect=gr;i.fill_rect=fr;i.set_background_color=co;i.get_string=gs;i.set_window_text=st;i.set_window_pos=sp;i.send_dialog_item_message=se;i.get_dialog_item=gi;i.get_dialog_text=gt;i.get_window_text=gw;i.send_message=sm;i.fits_screen=fs;i.sticky_position_call=sk;i.end_dialog=ed;i.win_help=hp;i.context=&t;if(stars_rename_dlg(9,0x14,88,0,&i)!=1||t.colors!=0)return 1;if(stars_rename_dlg(9,0x19,88,0x00060000UL,&i)!=55||t.colors!=1)return 2;if(stars_rename_dlg(9,0x110,0,0,&i)!=1||t.title_id!=0x2dcU||t.positions!=1||t.edits!=1||t.gets!=1)return 3;if(stars_rename_dlg(9,0x111,1,0,&i)!=1||t.end_result!=1||t.fits!=1)return 4;if(stars_rename_dlg(9,0x111,0x10c,0x0400004dUL,&i)!=1||t.sends!=2||i.edit_update!=0)return 5;if(stars_rename_dlg(9,0x111,0x76,0,&i)!=1||t.help_id!=0x447U)return 6;puts("RENAME_DLG_SOURCE=PASS");return 0;}
