#include "config.h"
#include <stdio.h>
#include <string.h>

typedef struct Probe {
    int tiles, alerts, serials;
} Probe;
static void get_string(void *opaque, StarsU16 id, StarsU8 *out, StarsU16 size)
{
    const char *s=""; Probe *p=(Probe *)opaque; (void)p;
    switch (id) {
    case 0x90: s="MAIN"; break; case 0x91: s="STARS.INI"; break;
    case 0x92: s="GAME"; break; case 0x93: s="ZIP"; break;
    case 0x99: s="GAME"; break; case 0x9b: s="MRU"; break;
    case 0xa4: s="SERIAL"; break; case 0x230: s="DEFAULT"; break;
    default: s="KEY"; break;
    }
    if (id==0x9b) s="MRU";
    if (size) { strncpy((char *)out,s,size-1); out[size-1]=0; }
}
static StarsI16 profile_int(void *opaque, const StarsU8 *section, const StarsU8 *key,
    StarsI16 fallback, const StarsU8 *file)
{
    (void)opaque; (void)section; (void)file;
    if (!strcmp((const char *)key,"KEY")) return fallback;
    if (!strcmp((const char *)key,"MAIN")) return 8;
    if (!strcmp((const char *)key,"MRU")) return fallback;
    return fallback;
}
static StarsU16 profile_string(void *opaque, const StarsU8 *section, const StarsU8 *key,
    const StarsU8 *fallback, StarsU8 *out, StarsU16 size, const StarsU8 *file)
{
    Probe *p=(Probe *)opaque; const char *s=""; StarsU16 n;
    (void)section; (void)fallback; (void)file;
    if (!strcmp((const char *)key,"SERIAL")) s="1234567890123456789012345678";
    else if (!strcmp((const char *)key,"MRU1")) s="first.stars";
    else if (!strcmp((const char *)key,"MRU2")) s="second.stars";
    else if (!strcmp((const char *)key,"MRU3")) s="";
    else if (!strcmp((const char *)key,"MRU")) s="BASE";
    else if (!strcmp((const char *)key,"ZIP1")) s="abcdefghijklmnopabcdZipName";
    else if (!strncmp((const char *)key,"ZIPP",4)) s="ababcaProd";
    else if (!strcmp((const char *)key,"KEY")) s="PB123";
    else s="ABCD";
    n=(StarsU16)strlen(s); if (size) { strncpy((char *)out,s,size-1); out[size-1]=0; }
    (void)p;
    return n;
}
static void get_window(void *opaque, StarsU16 id, StarsReadIniWindow *w)
{ (void)opaque; w->left=(StarsI16)id; w->top=2; w->right=(StarsI16)(id+100); w->bottom=102; }
static void read_tiles(void *opaque, const StarsU8 *text, StarsU16 count)
{ Probe *p=(Probe *)opaque; (void)text; (void)count; ++p->tiles; }
static void parse_serial(void *opaque, StarsU32 *serial, StarsU8 *machine, const StarsU8 *text)
{ Probe *p=(Probe *)opaque; ++p->serials; *serial=0x12345678UL; machine[0]=text[0]; }
static void alert(void *opaque, StarsU16 id, StarsI16 type)
{ Probe *p=(Probe *)opaque; (void)id; (void)type; ++p->alerts; }
static StarsU32 today(void *opaque) { (void)opaque; return 1000UL; }
int main(void)
{
    StarsReadIniState s; StarsReadIniOps o; Probe p; int failures=0;
    memset(&s,0,sizeof(s)); memset(&o,0,sizeof(o)); memset(&p,0,sizeof(p));
    s.startup_flags=0xffffU; s.available_screen_colors=8; s.graphics_bits=1; s.min_graph_max=5000;
    o.get_string=get_string; o.profile_int=profile_int; o.profile_string=profile_string;
    o.get_window=get_window; o.read_tiles=read_tiles; o.parse_serial=parse_serial; o.alert=alert; o.today=today;
    stars_read_ini_settings(&s,&o,&p,(const StarsU8 *)"STARS.INI");
    if (p.serials!=1 || p.tiles!=2 || p.alerts!=0) ++failures;
    if (s.frame.left!=148 || s.pane[0].width!=100 || s.window_layout!=1) ++failures;
    if ((s.startup_flags&0x01e0U)!=0x20U || s.player_id!=0) ++failures;
    if (s.mru[0][0]==0 || s.mru[1][0]==0 || s.zip_valid[0]!=1 || s.production[0].valid!=1) ++failures;
    printf("READ_INI_CHECKS=5 FAILURES=%d RESULT=%s\n",failures,failures?"FAIL":"PASS");
    return failures!=0;
}
