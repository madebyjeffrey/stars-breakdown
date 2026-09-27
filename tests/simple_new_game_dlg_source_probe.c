#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 checked;
    StarsI16 strings;
    StarsI16 ended;
    StarsI16 last_end;
    StarsI16 race_wizards;
    StarsI16 loaded;
    StarsI16 saved;
    StarsI16 focus;
    StarsI16 selected;
    StarsI16 no_checked;
    StarsI16 deletes;
    StarsI16 last_limit;
    StarsU16 density_radio;
    StarsU16 size_radio;
} Trace;
static StarsU16 item(void *c, StarsU16 h, StarsU16 id){(void)c;(void)h;return (StarsU16)(id+100);}
static StarsI16 checked(void *c, StarsU16 h, StarsU16 id){Trace*t=(Trace*)c;(void)h;++t->checked;if(t->no_checked)return 0;return id==200||id==1002;}
static StarsI16 get_string(void *c, StarsU16 id, StarsU8 *b, StarsU16 n){(void)c;(void)id;if(n>1){b[0]='X';b[1]=0;return 1;}return 0;}
static void radio(void *c, StarsU16 h, StarsU16 a, StarsU16 b, StarsU16 d){Trace*t=(Trace*)c;(void)h;if(a==200U){t->density_radio=d;}else if(a==1000U){t->size_radio=d;}(void)b;}
static void send(void *c, StarsU16 h, StarsU16 m, StarsU16 w, StarsU32 l){Trace*t=(Trace*)c;(void)h;(void)l;if(m==0x407)t->selected=6;if(m==0x404)++t->deletes;if(m==0x40e)t->last_limit=(StarsI16)w;}
static void add(void *c, StarsU16 h, StarsU16 i, const StarsU8 *s){(void)h;(void)i;(void)s;++((Trace*)c)->strings;}
static void sticky(void *c, StarsU16 h, StarsPoint *p, StarsI16 save){(void)c;(void)h;(void)p;(void)save;}
static void end(void *c, StarsU16 h, StarsI16 r){(void)h;++((Trace*)c)->ended;((Trace*)c)->last_end=r;}
static StarsI16 wizard(void *c, StarsU16 h){(void)h;++((Trace*)c)->race_wizards;return 1;}
static void load_old(void *c, StarsI16 n){(void)n;++((Trace*)c)->loaded;}
static void load_new(void *c, StarsI16 n){(void)n;++((Trace*)c)->loaded;}
static void save(void *c, StarsI16 n){(void)n;++((Trace*)c)->saved;}
static StarsI16 selection(void *c, StarsU16 h){(void)h;(void)c;return 6;}
static StarsI16 list_count(void *c, StarsU16 h){(void)c;(void)h;return 8;}
static void focus(void *c, StarsU16 h){(void)h;++((Trace*)c)->focus;}

int main(void)
{
    StarsSimpleNewGameState state;
    StarsNewGameDlgOps ops;
    StarsSimpleNewGameInput input;
    Trace trace;
    memset(&state,0,sizeof state);memset(&ops,0,sizeof ops);memset(&input,0,sizeof input);memset(&trace,0,sizeof trace);
    state.density=1;state.map_size=2;state.turn=6;state.button_face_brush=4;state.button_face_color=7;state.arial8_height=8;
    ops.get_dialog_item=item;ops.is_button_checked=checked;ops.get_string=get_string;ops.check_radio_button=radio;ops.send_message=send;ops.sticky_position=sticky;ops.end_dialog=end;ops.context=&trace;
    input.state=&state;input.ops=&ops;input.race_wizard=wizard;input.load_old_race=load_old;input.load_new_race=load_new;input.save_new_race=save;input.add_list_string=add;input.list_count=list_count;input.get_selection=selection;input.set_focus=focus;input.context=&trace;
    if(stars_simple_new_game_dlg(&input,10,272,0,0)!=1||trace.strings!=7||
        trace.density_radio!=201U||trace.size_radio!=1001U)return 1;
    if(stars_simple_new_game_dlg(&input,10,273,0x430,0)!=1||state.density!=0||state.map_size!=2||trace.last_end!=0x430)return 2;
    if(stars_simple_new_game_dlg(&input,10,273,0xd2,0)!=0||trace.race_wizards!=1||trace.loaded!=1||trace.saved!=1||trace.focus!=1||trace.deletes!=1||trace.last_limit!=7)return 3;
    if(stars_simple_new_game_dlg(&input,10,273,0xd4,0)!=1||trace.last_end!=0xd4)return 4;
    trace.no_checked=1;
    if(stars_simple_new_game_dlg(&input,10,273,0x430,0)!=1||
            state.density!=4||state.map_size!=5)return 5;
    puts("SIMPLE_NEW_GAME_DLG_SOURCE=PASS");
#ifdef TARGET_NATIVE
    {
        FILE *report = fopen("C:\\SNGD\\SNGDR.TXT", "w");
        if (!report)
            return 6;
        fprintf(report, "SIMPLE_NEW_GAME_DLG_CHECKS=5 FAILURES=0 RESULT=PASS\n");
        fclose(report);
    }
#endif
    return 0;
}
