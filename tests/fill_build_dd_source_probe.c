#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace { StarsI16 resets; StarsI16 adds; StarsI16 enables; StarsI16 positions; StarsI16 last_height; StarsI16 delete_enabled; StarsI16 edit_enabled; StarsI16 ends; StarsI16 reported_count; } Trace;
static void send(void *c, StarsU16 h, StarsU16 m, StarsU16 w, const StarsU8 *s){(void)h;(void)w;(void)s;if(m==0x40b)++((Trace*)c)->resets;if(m==0x403)++((Trace*)c)->adds;if(m==0x40e)++((Trace*)c)->ends;}
static void enable(void *c, StarsU16 h, StarsU16 v){Trace *t=(Trace*)c;++t->enables;if(h==0x821&&v)t->delete_enabled=1;if(h==0x822&&v)t->edit_enabled=1;}
static StarsU16 item(void *c, StarsU16 h, StarsU16 id){(void)c;(void)h;return (StarsU16)(id+10);}
static StarsI16 part(void *c, StarsI16 sb, StarsI16 n, StarsU8 *b, StarsU16 z){(void)c;(void)sb;(void)z;b[0]='P';b[1]=(StarsU8)('0'+n);b[2]=0;return 1;}
static StarsI16 player(void *c, StarsI16 p, StarsU8 *b, StarsU16 z){(void)c;(void)p;(void)z;b[0]='E';b[1]=0;return 1;}
static StarsI16 queued(void *c, StarsI16 n, StarsI16 *q){(void)c;(void)n;*q=0;return 1;}
static StarsI16 count(void *c, StarsU16 h){Trace *t=(Trace*)c;(void)h;return t->reported_count?t->reported_count:(StarsI16)t->adds;}
static const StarsU8 *string(void *c, StarsU16 id){(void)c;(void)id;return (const StarsU8*)"Label";}
static void rect(void *c, StarsU16 h, StarsRect *r){(void)c;(void)h;r->left=4;r->top=0;r->right=100;r->bottom=20;}
static void pos(void *c, StarsU16 h, StarsU16 z, StarsI16 x, StarsI16 y, StarsI16 w, StarsI16 d, StarsU16 f){(void)h;(void)z;(void)x;(void)y;(void)w;(void)f;++((Trace*)c)->positions;((Trace*)c)->last_height=d;}
int main(void)
{
    StarsFillBuildDesign designs[16]; StarsFillBuildInput input; Trace trace;
    memset(designs,0,sizeof designs);memset(&input,0,sizeof input);memset(&trace,0,sizeof trace);
    designs[0].visible=1;designs[0].name[0]='A';designs[0].name[1]=0;
    input.dropdown=3;input.slot_dialog=4;input.mode=0;input.line_height=10;input.parent_width=200;input.designs=designs;input.designs_per_player=16;input.send_message=send;input.enable_window=enable;input.get_dialog_item=item;input.part_name=part;input.player_name=player;input.queued=queued;input.list_count=count;input.compressed_string=string;input.get_window_rect=rect;input.set_window_pos=pos;input.context=&trace;
    stars_fill_build_dropdown(&input);
    if(trace.resets!=1||trace.adds!=1||trace.positions!=1||trace.last_height!=26||
            trace.ends!=1||!trace.delete_enabled||!trace.edit_enabled)return 1;
    input.mode=1;stars_fill_build_dropdown(&input);if(trace.adds!=33||trace.ends!=2)return 2;
    input.mode=3;input.starbase_mode=1;stars_fill_build_dropdown(&input);if(trace.adds!=41||trace.ends!=3)return 3;
    trace.reported_count=99;stars_fill_build_dropdown(&input);if(trace.last_height!=305||trace.ends!=4)return 4;
    puts("FILL_BUILD_DD_CHECKS=8 FAILURES=0 RESULT=PASS");
#ifdef TARGET_NATIVE
    {
        FILE *report = fopen("C:\\FBD\\FBDR.TXT", "w");
        if (!report)
            return 5;
        fputs("FILL_BUILD_DD_CHECKS=8 FAILURES=0 RESULT=PASS\n", report);
        fclose(report);
    }
#endif
    return 0;
}
