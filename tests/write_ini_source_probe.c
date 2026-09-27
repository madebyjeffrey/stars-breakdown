#include "config.h"
#include <stdio.h>
#include <string.h>
typedef struct Probe { int writes, serials, tiles, found_prod; char last[256]; } Probe;
static void get_string(void *c, StarsU16 id, StarsU8 *out, StarsU16 n)
{ const char *s=(id==0x93)?"ZIP":(id==0x90)?"MAIN":"K"; (void)c; if(n){strncpy((char *)out,s,n-1);out[n-1]=0;} }
static void write_string(void *opaque,const StarsU8 *sec,const StarsU8 *key,const StarsU8 *value,const StarsU8 *file)
{ Probe *p=(Probe *)opaque; (void)sec;(void)key;(void)file;++p->writes;if(!strcmp((const char *)value,"abagaaP"))p->found_prod=1;strncpy(p->last,(const char *)value,sizeof(p->last)-1);p->last[sizeof(p->last)-1]=0; }
static void format_serial(void *opaque,StarsU32 serial,const StarsU8 *machine,StarsU8 *out,StarsU16 n)
{ Probe *p=(Probe *)opaque;(void)machine;++p->serials;if(n)sprintf((char *)out,"SERIAL%lu",(unsigned long)serial); }
static void format_tiles(void *opaque,StarsU16 which,StarsU8 *out,StarsU16 n)
{ Probe *p=(Probe *)opaque;(void)n;++p->tiles;sprintf((char *)out,"TILE%u",(unsigned)which); }
int main(void)
{
    StarsWriteIniState s; StarsWriteIniOps o; Probe p; int failures=0;
    memset(&s,0,sizeof(s));memset(&o,0,sizeof(o));memset(&p,0,sizeof(p));
    s.frame.left=1;s.frame.top=2;s.frame.right=11;s.frame.bottom=22;s.available_screen_colors=4;
    s.object_mode=1;s.player_id=2;s.object_selection=7;s.message_index=3;s.game_lid=0x1234UL;s.scan_zoom=0;
    s.graphics_bits2_low=0x30;s.zip_valid[0]=1;s.zip_order[0][0]=0x1234;s.zip_name[0][0]='Z';s.production[0].valid=1;s.production[0].count=1;s.production[0].order[0]=6;s.production[0].name[0]='P';s.date_installed=44;s.speed_vcr=2;
    o.get_string=get_string;o.write_string=write_string;o.format_serial=format_serial;o.format_tiles=format_tiles;
    stars_write_ini_settings(&s,&o,&p,(const StarsU8 *)"S.INI");
    if(p.serials!=1||p.tiles!=2||p.writes<25)++failures;
    if(!p.found_prod)++failures;
    printf("WRITE_INI_CHECKS=3 FAILURES=%d RESULT=%s\n",failures,failures?"FAIL":"PASS");
    return failures!=0;
}
