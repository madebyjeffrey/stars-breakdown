#include <stdio.h>
#include <string.h>
#include "core.h"
typedef struct Trace { StarsI16 dcs; StarsI16 texts; StarsI16 rights; StarsI16 diamonds; StarsI16 strings; StarsI16 modes; StarsI16 fonts; StarsI16 formats; StarsI16 special6; StarsU16 old_mode; StarsU16 last_mode; StarsU8 last_label[8]; } Trace;
static StarsU16 dc(void *c, StarsU16 h){(void)h;++((Trace*)c)->dcs;return 3;}
static void release(void *c, StarsU16 h, StarsU16 d){(void)c;(void)h;(void)d;}
static void font(void *c, StarsU16 h, StarsU16 f){(void)h;(void)f;++((Trace*)c)->fonts;}
static StarsU16 mode(void *c, StarsU16 h, StarsU16 m){Trace *t=(Trace*)c;(void)h;++t->modes;t->last_mode=m;if(t->modes==1){t->old_mode=7;return 7;}return t->old_mode;}
static StarsI16 extent(void *c, StarsU16 h, const StarsU8 *s, StarsI16 n){(void)c;(void)h;(void)s;return n;}
static StarsI16 string(void *c, StarsU16 id, StarsU8 *b, StarsU16 n){Trace*t=(Trace*)c;++t->strings;if(id==0x20b)t->special6=1;if(n>1){b[0]='S';b[1]=0;return 1;}return 0;}
static const StarsU8 *compressed(void *c, StarsU16 id){(void)c;(void)id;return (const StarsU8*)"C";}
static void text(void *c, StarsU16 h, StarsI16 x, StarsI16 y, const StarsU8 *s, StarsI16 n){(void)h;(void)x;(void)y;(void)s;(void)n;++((Trace*)c)->texts;}
static void right(void *c, StarsU16 h, StarsI16 x, StarsI16 y, const StarsU8 *s, StarsI16 n, StarsI16 w){Trace *t=(Trace*)c;(void)h;(void)x;(void)y;(void)w;++t->rights;if(n<(StarsI16)sizeof t->last_label){memcpy(t->last_label,s,(size_t)n);t->last_label[n]=0;}}
static StarsI16 format(void *c, StarsU8 *b, StarsU16 n, StarsU16 id, const StarsU8 *a, const StarsU8 *d){Trace *t=(Trace*)c;(void)id;++t->formats;if(a&&n>1){b[0]=a[0];b[1]=0;return 1;}if(d&&n>1){b[0]=d[0];b[1]=0;return 1;}if(n>0)b[0]=0;return 0;}
static void color(void *c, StarsU16 h, StarsU32 v){(void)c;(void)h;(void)v;}
static void diamond(void *c, StarsU16 h, const StarsRect *r){(void)h;(void)r;++((Trace*)c)->diamonds;}
int main(void)
{
    StarsDrawNewGame2Player players[16]; StarsDrawNewGame2Ops ops; StarsDrawNewGame2Input input; Trace trace;
    memset(players,0,sizeof players);memset(&ops,0,sizeof ops);memset(&input,0,sizeof input);memset(&trace,0,sizeof trace);
    players[0].type=0;players[1].type=25;players[1].active=1;players[1].dead=1;players[1].name[0]='N';players[1].name[1]=0;
    players[2].type=2;players[2].name[0]='A';players[2].file_name[0]='F';
    ops.get_dc=dc;ops.release_dc=release;ops.select_font=font;ops.set_bk_mode=mode;ops.text_extent=extent;ops.get_string=string;ops.compressed_string=compressed;ops.format_text=format;ops.text_out=text;ops.right_text=right;ops.set_text_color=color;ops.draw_diamond=diamond;ops.context=&trace;
    input.hwnd=4;input.line_height=10;input.player_count=2;input.text_color=1;input.dead_color=2;input.bold_font=9;input.players=players;input.ops=&ops;
    stars_draw_new_game2(&input,-1);
    if(trace.dcs!=1||trace.diamonds!=16||trace.texts!=16||trace.rights!=16||trace.modes!=2||trace.last_mode!=7||trace.fonts!=1||trace.formats!=1||!trace.special6||strcmp((const char*)trace.last_label,"16:"))return 1;
    input.read_only=1;stars_draw_new_game2(&input,-1);if(trace.diamonds!=18||trace.texts!=18||trace.rights!=18)return 2;
    puts("DRAW_NEW_GAME2_SOURCE=PASS");
#ifdef TARGET_NATIVE
    {
        FILE *report = fopen("C:\\DNG2\\DNG2R.TXT", "w");
        if (!report)
            return 3;
        fprintf(report, "DRAW_NEW_GAME2_CHECKS=2 FAILURES=0 RESULT=PASS\n");
        fclose(report);
    }
#endif
    return 0;
}
