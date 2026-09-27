/* Genuine C reconstruction of ReadIniSettings (NE 1:124c-2b89).
 * Profile and resource services are callbacks so the state machine can run
 * under both the host probe and the original Win16 service layer. */
#include "core.h"
#include <string.h>

typedef struct StarsReadIniWindow { StarsI16 left, top, right, bottom; } StarsReadIniWindow;
typedef struct StarsReadIniPane {
    StarsI16 x, y, width, height; StarsU16 visible, sort; StarsI16 ascending;
} StarsReadIniPane;
typedef struct StarsReadIniProduction {
    StarsU8 valid, no_research, count; StarsU16 order[12]; StarsU8 name[13];
} StarsReadIniProduction;
typedef struct StarsReadIniState {
    StarsU16 startup_flags, graphics_bits, graphics_bits2, graphics_bits2_low;
    StarsReadIniWindow frame; StarsReadIniPane pane[4];
    StarsU16 window_layout, pane_width[6];
    StarsI16 screen_colors, available_screen_colors, plan_width, message_height, minimum_height;
    StarsU16 second_plan_width, second_message_height, second_minimum_height;
    StarsU16 scan_ship, scan_enemy_ship, scan_mines, radar_view, scan_flags;
    StarsI16 scan_zoom, min_graph_max; StarsU32 serial_number, lid;
    StarsU8 machine_config[16], base[256], default_password[16];
    StarsI16 message_index, turn, player_id, object_selection; StarsU16 backup_dirs, speed_vcr;
    StarsU32 date_installed; StarsU8 mru[9][256], race_file[4][32]; StarsU16 view_flags;
    StarsU8 zip_valid[4], zip_name[4][13]; StarsU16 zip_order[4][5];
    StarsReadIniProduction production[5];
} StarsReadIniState;
typedef struct StarsReadIniOps {
    void (*get_string)(void *, StarsU16, StarsU8 *, StarsU16);
    StarsI16 (*profile_int)(void *, const StarsU8 *, const StarsU8 *, StarsI16, const StarsU8 *);
    StarsU16 (*profile_string)(void *, const StarsU8 *, const StarsU8 *, const StarsU8 *, StarsU8 *, StarsU16, const StarsU8 *);
    void (*get_window)(void *, StarsU16, StarsReadIniWindow *); void (*read_tiles)(void *, const StarsU8 *, StarsU16);
    void (*parse_serial)(void *, StarsU32 *, StarsU8 *, const StarsU8 *); void (*alert)(void *, StarsU16, StarsI16);
    StarsU32 (*today)(void *);
} StarsReadIniOps;

static StarsI16 ri_int(const StarsReadIniOps *o, void *c, StarsU8 *s,
    StarsU8 *k, StarsI16 d, const StarsU8 *f)
{ return o->profile_int ? o->profile_int(c,s,k,d,f) : d; }
static StarsU16 ri_text(const StarsReadIniOps *o, void *c, StarsU8 *s,
    StarsU8 *k, const StarsU8 *d, StarsU8 *out, StarsU16 n, const StarsU8 *f)
{
    if (!o->profile_string) { if (n) out[0]=0; return 0; }
    return o->profile_string(c,s,k,d,out,n,f);
}
static void ri_key(const StarsReadIniOps *o, void *c, StarsU16 id,
    StarsU8 *out, StarsU16 n)
{ if (o->get_string) o->get_string(c,id,out,n); else if (n) out[0]=0; }
static StarsU16 ri_clamp(StarsI16 v, StarsI16 lo, StarsI16 hi)
{ return (StarsU16)(v<lo ? lo : v>hi ? hi : v); }
static void ri_window(const StarsReadIniOps *o, void *c, StarsU16 id,
    StarsReadIniPane *p)
{
    StarsReadIniWindow w;
    if (!o->get_window) return;
    w.left=-32768; w.top=w.right=w.bottom=0; o->get_window(c,id,&w);
    if (w.left!=-32768) {
        p->x=w.left; p->y=w.top; p->width=(StarsI16)(w.right-w.left);
        p->height=(StarsI16)(w.bottom-w.top);
    }
}
static StarsU32 ri_hex(const StarsU8 *p, StarsU16 n)
{
    StarsU32 v=0; StarsU16 i;
    for (i=0;i<n;++i) {
        v<<=4;
        if (p[i]>='0' && p[i]<='9') v+=(StarsU32)(p[i]-'0');
        else if (p[i]>'`' && p[i]<'g') v+=(StarsU32)(p[i]-'W');
    }
    return v;
}
static StarsU16 ri_order_word(const StarsU8 *p)
{
    StarsU16 v=(StarsU16)(p[0]-'a')<<12;
    v=(StarsU16)(v|(StarsU16)(p[1]-'a'));
    v=(StarsU16)(v|(StarsU16)(p[2]-'a')<<4);
    v=(StarsU16)(v|(StarsU16)(p[3]-'a')<<8);
    if ((v>>6)>0x3fcU) v=(StarsU16)((v&0x3fU)|0x40U);
    if ((v&0x3fU)>6U) v=(StarsU16)(v&0xffc0U);
    return v;
}
static void ri_digit(StarsU8 *p, StarsU16 n, StarsU8 digit)
{ p[n]=(StarsU8)('1'+digit); p[n+1]=0; }
static void ri_compact_mru(StarsU8 mru[9][256])
{
    StarsU16 r,w;
    for (r=0,w=0;r<9;++r) if (mru[r][0]) {
        if (r!=w) { memcpy(mru[w],mru[r],256); mru[r][0]=0; }
        ++w;
    }
    while (w<9) { mru[w][0]=0; ++w; }
}
static StarsU16 ri_zip_valid(const StarsU8 *p, StarsU16 n)
{
    StarsU16 i;
    if (n<=19U || n>=33U) return 0;
    for (i=0;i<20;++i) if (p[i]<='`' || p[i]>='q') return 0;
    return 1;
}
static StarsU16 ri_prod_valid(const StarsU8 *p, StarsU16 n, StarsU16 *count)
{
    StarsI16 c; StarsU16 i,need;
    if (n<=2U || n>=65U) return 0;
    c=(StarsI16)p[1]-'a'; if (c<0 || c>=13) return 0;
    need=(StarsU16)(c*4+2); if (n<need) return 0;
    for (i=0;i<need;++i) if (p[i]<='`' || p[i]>='q') return 0;
    *count=(StarsU16)c; return 1;
}

void stars_read_ini_settings(StarsReadIniState *s, const StarsReadIniOps *o,
    void *c, const StarsU8 *file)
{
    StarsU8 section[64], key[64], text[256], serial_text[64], prefix[64];
    StarsReadIniWindow w;
    StarsI16 v,n,i,j; StarsU16 u,count; StarsU32 today;

    s->startup_flags=(StarsU16)(s->startup_flags&0xffe3U);
    ri_key(o,c,0x90,section,sizeof(section));
    ri_key(o,c,0x91,prefix,sizeof(prefix));
    if (o->get_window) {
        w.left=-32768; w.top=w.right=w.bottom=0; o->get_window(c,0x94,&w); s->frame=w;
    }
    ri_window(o,c,0xb2,&s->pane[1]); ri_window(o,c,0xb3,&s->pane[2]);
    ri_window(o,c,0xb4,&s->pane[3]); ri_window(o,c,0xb1,&s->pane[0]);

    ri_key(o,c,0xad,key,sizeof(key));
    s->screen_colors=ri_int(o,c,prefix,key,0,file);
    if (s->screen_colors==0 && (s->available_screen_colors<5 || (s->graphics_bits&3U)==0U))
        if (o->alert) o->alert(c,0x2c9,0x10);
    ri_key(o,c,0xa3,key,sizeof(key)); s->window_layout=ri_clamp(ri_int(o,c,prefix,key,1,file),0,2);
    for (i=0;i<6;++i) {
        ri_key(o,c,(StarsU16)(0xa5+i),key,sizeof(key));
        v=ri_int(o,c,prefix,key,i%3==0 ? 396 : i%3==1 ? 110 : 192,file);
        s->pane_width[i]=ri_clamp(v,10,2000);
    }
    ri_key(o,c,0xab,key,sizeof(key)); u=(StarsU16)ri_int(o,c,prefix,key,1,file);
    s->graphics_bits=(StarsU16)((s->graphics_bits&0x7fffU)|(u?0x8000U:0));

    ri_key(o,c,0xa4,key,sizeof(key));
    n=(StarsI16)ri_text(o,c,prefix,key,(const StarsU8 *)"",serial_text,40,file);
    if (n==28 && o->parse_serial) o->parse_serial(c,&s->serial_number,s->machine_config,serial_text);
    else { s->serial_number=0; memset(s->machine_config,0,sizeof(s->machine_config)); }
    ri_key(o,c,0x96,key,sizeof(key)); ri_text(o,c,prefix,key,(const StarsU8 *)"",text,20,file);
    if (o->read_tiles) o->read_tiles(c,text,6);
    ri_key(o,c,0x95,key,sizeof(key)); ri_text(o,c,prefix,key,(const StarsU8 *)"",text,20,file);
    if (o->read_tiles) o->read_tiles(c,text,7);

    ri_key(o,c,0x98,key,sizeof(key)); n=(StarsI16)ri_text(o,c,prefix,key,(const StarsU8 *)"",text,20,file);
    if (n<3) s->startup_flags=(StarsU16)(s->startup_flags&0xfe1fU);
    else {
        s->startup_flags=(StarsU16)(s->startup_flags&0xfe1fU);
        if (text[0]=='E') s->startup_flags|=0x80U;
        else if (text[0]=='P') s->startup_flags|=0x20U;
        else if (text[0]=='S') s->startup_flags|=0x40U;
        else if (text[0]!='N') s->startup_flags=(StarsU16)(s->startup_flags&0xfe1fU);
        if (text[1]>='B' && text[1]<='Q') s->player_id=(StarsI16)(text[1]-'B');
        else s->startup_flags=(StarsU16)(s->startup_flags&0xfe1fU);
        if ((s->startup_flags&0x01e0U)!=0U) {
            s->object_selection=0;
            for (i=2;i<n && text[i]>='0' && text[i]<='9';++i)
                s->object_selection=(StarsI16)(s->object_selection*10+text[i]-'0');
        }
    }
    ri_key(o,c,0xcb,key,sizeof(key)); s->message_index=ri_int(o,c,prefix,key,0,file);
    ri_key(o,c,0xac,key,sizeof(key)); n=(StarsI16)ri_text(o,c,prefix,key,(const StarsU8 *)"",text,10,file); s->lid=ri_hex(text,(StarsU16)n);
    ri_key(o,c,0x97,key,sizeof(key)); v=ri_int(o,c,prefix,key,4,file); if (v!=0 && v<10) s->scan_zoom=(StarsI16)(v-5);
    ri_key(o,c,0x9e,key,sizeof(key)); s->scan_ship=(StarsU16)ri_int(o,c,prefix,key,0,file);
    ri_key(o,c,0x9f,key,sizeof(key)); s->scan_enemy_ship=(StarsU16)ri_int(o,c,prefix,key,0,file);
    ri_key(o,c,0xa0,key,sizeof(key)); s->scan_mines=(StarsU16)ri_int(o,c,prefix,key,15,file)&15U;
    ri_key(o,c,0xa1,key,sizeof(key)); u=(StarsU16)ri_int(o,c,prefix,key,100,file); s->radar_view=u>100U?100U:u;
    ri_key(o,c,0x9d,key,sizeof(key)); s->scan_flags=(StarsU16)ri_int(o,c,prefix,key,224,file);
    if ((s->scan_flags&0xc00fU)>5U) { s->scan_flags=0; s->scan_ship=0; }
    ri_key(o,c,0xa2,key,sizeof(key)); s->min_graph_max=ri_int(o,c,prefix,key,s->min_graph_max,file);
    if (s->min_graph_max<100 || s->min_graph_max>30000) s->min_graph_max=5000;

    ri_key(o,c,0x99,section,sizeof(section)); ri_key(o,c,0xcc,key,sizeof(key)); v=ri_int(o,c,section,key,0,file);
    s->startup_flags=(StarsU16)((s->startup_flags&0x7fffU)|((StarsU16)v<<15));
    ri_key(o,c,0x9a,key,sizeof(key)); v=ri_int(o,c,section,key,0,file); s->startup_flags=(StarsU16)((s->startup_flags&0xfffbU)|((StarsU16)v&1U)<<2);
    ri_key(o,c,0x9b,key,sizeof(key)); n=(StarsI16)ri_text(o,c,section,key,(const StarsU8 *)"",text,256,file);
    if (n<4) s->startup_flags=(StarsU16)(s->startup_flags&0xfffeU); else { s->startup_flags|=1U; memcpy(s->base,text,256); }
    for (i=0;i<9;++i) { ri_digit(key,(StarsU16)strlen((char *)key),(StarsU8)i); ri_text(o,c,section,key,(const StarsU8 *)"",s->mru[i],256,file); }
    ri_compact_mru(s->mru);
    ri_key(o,c,0x9c,key,sizeof(key)); s->turn=ri_int(o,c,section,key,s->turn,file);

    ri_key(o,c,0x92,section,sizeof(section)); ri_key(o,c,0xae,key,sizeof(key)); ri_text(o,c,section,key,(const StarsU8 *)"",s->default_password,16,file);
    ri_key(o,c,0xaf,key,sizeof(key)); v=ri_int(o,c,section,key,0,file); s->graphics_bits=(StarsU16)((s->graphics_bits&0xfbffU)|(v?0x400U:0));
    ri_key(o,c,0xc9,key,sizeof(key)); v=ri_int(o,c,section,key,0,file); s->graphics_bits2=(StarsU16)((s->graphics_bits2&0xffdfU)|(v?0x20U:0));
    ri_key(o,c,0xca,key,sizeof(key)); v=ri_int(o,c,section,key,0,file); s->graphics_bits2=(StarsU16)((s->graphics_bits2&0xffbfU)|(v?0x40U:0));
    ri_key(o,c,0xb0,key,sizeof(key)); v=ri_int(o,c,section,key,1,file); s->backup_dirs=(StarsU16)(v<1 || v>999 ? 1 : v);
    for (i=0;i<4;++i) {
        ri_key(o,c,(StarsU16)(0xb5+i),key,sizeof(key)); s->pane[i].visible=(StarsU16)ri_int(o,c,section,key,-1,file);
        ri_key(o,c,(StarsU16)(0xb6+i),key,sizeof(key)); u=(StarsU16)ri_int(o,c,section,key,0,file);
        s->pane[i].ascending=(StarsI16)((u&0x100U)!=0U); s->pane[i].sort=u&0xffU;
    }
    ri_key(o,c,0xbd,key,sizeof(key)); u=(StarsU16)ri_int(o,c,section,key,7,file); s->view_flags=(StarsU16)((s->view_flags&0xfff0U)|(u&15U));
    if ((s->view_flags&15U)>7U) s->view_flags=(StarsU16)((s->view_flags&0xfff0U)|7U);
    ri_key(o,c,0xc0,key,sizeof(key)); s->speed_vcr=(StarsU16)ri_int(o,c,section,key,1,file);
    today=o->today ? o->today(c) : 0UL; ri_key(o,c,0xbf,key,sizeof(key)); s->date_installed=(StarsU32)(StarsI32)ri_int(o,c,section,key,-1,file);
    if (s->date_installed>today) s->date_installed=today;
    s->graphics_bits2_low=(StarsU16)((s->graphics_bits2_low&0xfffdU)|(s->date_installed+0x15UL<=today?2U:0U));

    ri_key(o,c,0xc4,section,sizeof(section));
    for (i=0;i<4;++i) { ri_key(o,c,(StarsU16)(0xc5+i),key,sizeof(key)); n=(StarsI16)ri_text(o,c,section,key,(const StarsU8 *)"",text,80,file); if (n>4 && n<32) { memcpy(s->race_file[i],text,(size_t)n); s->race_file[i][n]=0; } }
    ri_key(o,c,0x93,section,sizeof(section)); memset(s->zip_valid,0,sizeof(s->zip_valid));
    for (i=0;i<4;++i) { ri_key(o,c,0x93,prefix,sizeof(prefix)); n=(StarsI16)strlen((char *)prefix); ri_digit(prefix,(StarsU16)n,(StarsU8)i); n=(StarsI16)ri_text(o,c,section,prefix,(const StarsU8 *)"",text,80,file); if (ri_zip_valid(text,(StarsU16)n)) { for (j=0;j<5;++j) s->zip_order[i][j]=ri_order_word(text+j*4); memcpy(s->zip_name[i],text+20,13); s->zip_valid[i]=1; } }
    memset(s->production,0,sizeof(s->production));
    for (i=0;i<5;++i) { ri_key(o,c,0x93,prefix,sizeof(prefix)); n=(StarsI16)strlen((char *)prefix); prefix[n]='P'; prefix[n+1]=(StarsU8)('1'+i); prefix[n+2]=0; n=(StarsI16)ri_text(o,c,section,prefix,(const StarsU8 *)"",text,80,file); count=0; if (ri_prod_valid(text,(StarsU16)n,&count)) { s->production[i].valid=1; s->production[i].no_research=(StarsU8)(text[0]!='a'); s->production[i].count=(StarsU8)count; memcpy(s->production[i].name,text+2+count*4,13); for (j=0;j<(StarsI16)count;++j) s->production[i].order[j]=ri_order_word(text+2+j*4); } }
    ri_key(o,c,0x230,s->production[0].name,sizeof(s->production[0].name)); s->production[0].valid=1;
}
