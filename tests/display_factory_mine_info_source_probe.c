#include <stdio.h>
#include <string.h>
#include "core.h"
typedef struct State{StarsU16 fonts,small,strings,planet_names,titles,values,percents,centers,wraps;StarsU16 last_id,last_suffix;StarsI16 last_value;}State;
static const StarsU8 *str(void*c,StarsU16 id){State*s=(State*)c;s->strings++;s->last_id=id;return (const StarsU8*)"Label";}
static const StarsU8 *planet(void*c,StarsI16 id){State*s=(State*)c;s->planet_names+=(id==7);return (const StarsU8*)"Planet";}
static void font(void*c,StarsU16 h,StarsU16 f){State*s=(State*)c;s->fonts+=(h==3);s->small+=(f==1);}
static void title(void*c,StarsU16 a,StarsU16 b,StarsU8*out,StarsU16 n){State*s=(State*)c;s->titles+=(a==0xc2dU&&b==0xc35U);if(n>5)strcpy((char*)out,"Title");}
static void value(void*c,StarsI16 v,StarsU16 suf,StarsU8*out,StarsU16 n){State*s=(State*)c;s->values++;s->last_value=v;s->last_suffix=suf;if(n>3)strcpy((char*)out,"Val");}
static void pct(void*c,StarsI16 v,StarsU8*out,StarsU16 n){State*s=(State*)c;s->percents+=(v==9);if(n>3)strcpy((char*)out,"Pct");}
static void center(void*c,StarsU16 h,StarsI16 x,StarsI16 y,const StarsU8*t,StarsU16 n){State*s=(State*)c;s->centers+=(h==3&&x==46&&y==2&&n==5);(void)t;}
static void wrap(void*c,StarsU16 h,StarsI16*x,StarsI16*y,const StarsU8*t,StarsI16 a,StarsI16 n,StarsI16 w,StarsI16*ht,StarsI16 p){State*s=(State*)c;s->wraps+=(h==3&&a==0&&n==4&&w==92&&p==1);*y=(StarsI16)(*y+10);(void)x;(void)t;(void)ht;}
static void init(StarsDisplayFactoryMineInfoInput*i,State*s){memset(i,0,sizeof*i);i->font_small=1;i->font_large=2;i->width=100;i->print=1;i->line_height=8;i->race_stat=0;i->planet_id=7;i->planet_variable=2;i->planet_value=1;i->planet_max=9;i->hdc=3;i->select_font=font;i->get_string=str;i->get_planet_name=planet;i->format_title=title;i->format_value=value;i->format_percent=pct;i->center_text=center;i->wrap_text=wrap;i->context=s;}
int main(void){StarsDisplayFactoryMineInfoInput i;State s;StarsPoint p;memset(&s,0,sizeof s);init(&i,&s);p=stars_display_factory_mine_info(&i);if(p.x!=8||p.y!=114||s.titles!=1||s.centers!=1||s.wraps!=9||s.small!=5||s.planet_names!=1||s.values!=2||s.percents!=1||s.last_suffix!=0xc35U){fprintf(stderr,"one %d %d %u %u %u %u %u %u %u\n",p.x,p.y,s.titles,s.centers,s.wraps,s.small,s.planet_names,s.values,s.percents);return 1;}memset(&s,0,sizeof s);init(&i,&s);i.race_stat=8;i.planet_max=0;p=stars_display_factory_mine_info(&i);if(p.x!=8||p.y!=34||s.wraps!=1||s.last_id!=0x4ecU){fprintf(stderr,"two %d %d %u %u\n",p.x,p.y,s.wraps,s.last_id);return 2;}puts("DISPLAY_FACTORY_MINE_INFO_SOURCE=PASS");return 0;}
