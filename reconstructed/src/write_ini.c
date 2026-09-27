/* Genuine C reconstruction of WriteIniSettings (NE 5:7a76-8c87). */
#include "core.h"
#include <stdio.h>
#include <string.h>
typedef struct StarsReadIniWindow { StarsI16 left,top,right,bottom; } StarsReadIniWindow;
typedef struct StarsReadIniPane { StarsI16 x,y,width,height; StarsU16 visible,sort; StarsI16 ascending; } StarsReadIniPane;
typedef struct StarsReadIniProduction { StarsU8 valid,no_research,count; StarsU16 order[12]; StarsU8 name[13]; } StarsReadIniProduction;
typedef struct StarsWriteIniState {
    StarsReadIniWindow frame; StarsReadIniPane pane[4];
    StarsU16 startup_flags,graphics_bits,graphics_bits2,graphics_bits2_low,window_layout,pane_width[6];
    StarsI16 available_screen_colors,scan_zoom,min_graph_max;
    StarsU16 scan_flags,scan_ship,scan_enemy_ship,scan_mines,radar_view;
    StarsU32 serial_number,game_lid,date_installed; StarsU8 machine_config[16],base[256];
    StarsI16 object_mode,player_id,object_selection,message_index,turn; StarsU16 timer_id,speed_vcr;
    StarsU8 zip_valid[4],zip_name[4][13]; StarsU16 zip_order[4][5]; StarsReadIniProduction production[5];
} StarsWriteIniState;
typedef struct StarsWriteIniOps {
    void (*get_string)(void *,StarsU16,StarsU8 *,StarsU16);
    void (*write_string)(void *,const StarsU8 *,const StarsU8 *,const StarsU8 *,const StarsU8 *);
    void (*format_serial)(void *,StarsU32,const StarsU8 *,StarsU8 *,StarsU16);
    void (*format_tiles)(void *,StarsU16,StarsU8 *,StarsU16);
} StarsWriteIniOps;
static void wi_key(const StarsWriteIniOps *o,void *c,StarsU16 id,StarsU8 *p,StarsU16 n)
{ if(o->get_string)o->get_string(c,id,p,n); else if(n)p[0]=0; }
static void wi_put(const StarsWriteIniOps *o,void *c,const StarsU8 *sec,const StarsU8 *key,
    const StarsU8 *value,const StarsU8 *file)
{ if(o->write_string)o->write_string(c,sec,key,value,file); }
static void wi_int(const StarsWriteIniOps *o,void *c,const StarsU8 *sec,StarsU16 id,
    StarsI16 value,const StarsU8 *file)
{ StarsU8 key[32],text[32]; wi_key(o,c,id,key,sizeof(key)); sprintf((char *)text,"%d",value); wi_put(o,c,sec,key,text,file); }
static void wi_order_word(StarsU8 *p,StarsU16 v)
{ p[0]=(StarsU8)('a'+((v>>12)&15U)); p[1]=(StarsU8)('a'+(v&15U)); p[2]=(StarsU8)('a'+((v>>4)&15U)); p[3]=(StarsU8)('a'+((v>>8)&15U)); }
static void wi_tiles(const StarsWriteIniOps *o,void *c,const StarsU8 *sec,StarsU16 id,
    StarsU16 which,StarsU16 count,const StarsU8 *file)
{ StarsU8 key[32],text[80]; wi_key(o,c,id,key,sizeof(key)); if(o->format_tiles)o->format_tiles(c,which,text,sizeof(text)); else text[0]=0; wi_put(o,c,sec,key,text,file); (void)count; }
void stars_write_ini_settings(const StarsWriteIniState *s,const StarsWriteIniOps *o,
    void *c,const StarsU8 *file)
{
    StarsU8 sec[64],key[64],text[256]; StarsU16 i,j; StarsI16 x,y,r,b; StarsU16 packed;
    StarsI16 mode='N';
    wi_key(o,c,0x90,sec,sizeof(sec));
    wi_key(o,c,0xa4,key,sizeof(key));
    if(o->format_serial)o->format_serial(c,s->serial_number,s->machine_config,text,sizeof(text)); else text[0]=0;
    wi_put(o,c,sec,key,text,file);
    wi_key(o,c,0xad,key,sizeof(key)); packed=(StarsU16)(s->available_screen_colors<5 ? 1:0); if((s->graphics_bits&0x4000U)==0U)packed|=2U; sprintf((char *)text,"%d",packed); wi_put(o,c,sec,key,text,file);
    wi_key(o,c,0x94,key,sizeof(key)); sprintf((char *)text,"M%d,%d,%d,%d",s->frame.left,s->frame.top,s->frame.right,s->frame.bottom); wi_put(o,c,sec,key,text,file);
    for(i=0;i<4;++i) { static const StarsU16 ids[4]={0xb2,0xb3,0xb4,0xb1}; x=s->pane[i].x;y=s->pane[i].y;r=(StarsI16)(x+s->pane[i].width);b=(StarsI16)(y+s->pane[i].height); wi_key(o,c,ids[i],key,sizeof(key)); sprintf((char *)text,"M%d,%d,%d,%d",x,y,r,b); wi_put(o,c,sec,key,text,file); }
    wi_int(o,c,sec,0xa3,(StarsI16)s->window_layout,file); for(i=0;i<6;++i)wi_int(o,c,sec,(StarsU16)(0xa5+i),(StarsI16)s->pane_width[i],file); wi_int(o,c,sec,0xab,(StarsI16)((s->graphics_bits>>15)&1U),file);
    wi_tiles(o,c,sec,0x96,0,6,file); wi_tiles(o,c,sec,0x95,1,7,file);
    if(s->object_mode==1)mode='P'; else if(s->object_mode==2)mode='S'; else if(s->object_mode==4)mode='E';
    wi_key(o,c,0x98,key,sizeof(key)); sprintf((char *)text,"%c%c%d",mode,(char)('B'+s->player_id),s->object_selection); wi_put(o,c,sec,key,text,file);
    wi_int(o,c,sec,0xcb,s->message_index,file); wi_key(o,c,0xac,key,sizeof(key)); sprintf((char *)text,"%08lx",(unsigned long)s->game_lid); wi_put(o,c,sec,key,text,file);
    wi_key(o,c,0x97,key,sizeof(key)); text[0]=(StarsU8)('5'+s->scan_zoom);text[1]=0;wi_put(o,c,sec,key,text,file);
    wi_int(o,c,sec,0x9d,0x14f8,file); wi_int(o,c,sec,0x9e,0x14f8,file); wi_int(o,c,sec,0x9f,0x14f8,file); wi_int(o,c,sec,0xa0,0x14f8,file); wi_int(o,c,sec,0xa1,0x14f8,file); wi_int(o,c,sec,0xa2,s->min_graph_max,file);
    if(s->player_id!=-1) { wi_key(o,c,0x99,sec,sizeof(sec)); wi_int(o,c,sec,0x9a,s->timer_id?1:0,file); if((s->graphics_bits2_low&0x0100U)!=0U) { wi_int(o,c,sec,0x9c,s->turn,file); } wi_key(o,c,0x9b,key,sizeof(key)); sprintf((char *)text,"%s%d",s->base,s->player_id+1); wi_put(o,c,sec,key,text,file); }
    wi_key(o,c,0x92,sec,sizeof(sec));
    for(i=0;i<4;++i) { wi_int(o,c,sec,(StarsU16)(0xb5+i),(StarsI16)s->pane[i].visible,file); packed=(StarsU16)(s->pane[i].sort|(s->pane[i].ascending?0x100U:0)); wi_int(o,c,sec,(StarsU16)(0xb6+i),(StarsI16)packed,file); }
    wi_int(o,c,sec,0xbd,(StarsI16)(s->graphics_bits2&15U),file); wi_int(o,c,sec,0xbf,(StarsI16)s->date_installed,file); wi_int(o,c,sec,0xc0,(StarsI16)s->speed_vcr,file);
    if((s->graphics_bits2_low&0x0010U)!=0U) for(i=0;i<4;++i) { wi_key(o,c,0x93,key,sizeof(key)); j=(StarsU16)strlen((char *)key); key[j]=(StarsU8)('1'+i);key[j+1]=0; if(s->zip_valid[i]) { char *p=(char *)text; for(j=0;j<5;++j){wi_order_word((StarsU8 *)p,s->zip_order[i][j]);p+=4;} strcpy(p,(char *)s->zip_name[i]); } else text[0]=0; wi_put(o,c,sec,key,text,file); }
    if((s->graphics_bits2_low&0x0020U)!=0U) for(i=0;i<5;++i) { wi_key(o,c,0x93,key,sizeof(key)); j=(StarsU16)strlen((char *)key);key[j]='P';key[j+1]=(StarsU8)('1'+i);key[j+2]=0; if(s->production[i].valid){text[0]=(StarsU8)('a'+(s->production[i].no_research?1:0));text[1]=(StarsU8)('a'+s->production[i].count);for(j=0;j<s->production[i].count;++j)wi_order_word(text+2+j*4,s->production[i].order[j]);strcpy((char *)(text+2+s->production[i].count*4),(char *)s->production[i].name);}else text[0]=0;wi_put(o,c,sec,key,text,file); }
}
