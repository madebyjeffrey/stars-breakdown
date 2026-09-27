#include <stdio.h>
#include <string.h>
#include "core.h"
typedef struct State{StarsU16 fonts,small,strings,title,centers,wraps;StarsI16 last_id,last_value;StarsU16 title_length;StarsI16 final_y;}State;
static const StarsU8 *str(void*c,StarsU16 id){State*s=(State*)c;s->strings++;s->last_id=(StarsI16)id;return id==0x233U?(const StarsU8*)"Title":(const StarsU8*)"Label";}
static void font(void*c,StarsU16 h,StarsU16 f){State*s=(State*)c;s->fonts+=(h==3);s->small+=(f==1);}
static void center(void*c,StarsU16 h,StarsI16 x,StarsI16 y,const StarsU8*t,StarsU16 n){State*s=(State*)c;s->centers+=(h==3&&x==46&&y==2&&n==5);s->title_length=n;(void)t;}
static void pct(void*c,StarsI16 v,StarsU8*out,StarsU16 n){State*s=(State*)c;s->last_value=v;if(n>4)strcpy((char*)out,"PCT");}
static void wrap(void*c,StarsU16 h,StarsI16*x,StarsI16*y,const StarsU8*t,StarsI16 a,StarsI16 n,StarsI16 w,StarsI16*ht,StarsI16 p){State*s=(State*)c;s->wraps+=(h==3&&a==0&&n==4&&w==92&&p==1);s->final_y=*y;*y=(StarsI16)(*y+10);(void)x;(void)t;(void)ht;}
static void init(StarsDisplayResourceInfoInput*i,State*s){memset(i,0,sizeof*i);i->font_small=1;i->font_large=2;i->width=100;i->print=1;i->line_height=8;i->race_stat=8;i->planet_id=7;i->planet_variable=20;i->planet_value=30;i->resource_text_value=0;i->hdc=3;i->select_font=font;i->get_string=str;i->center_text=center;i->format_percent=pct;i->wrap_text=wrap;i->context=s;}
int main(void){StarsDisplayResourceInfoInput i;State s;StarsPoint p;memset(&s,0,sizeof s);init(&i,&s);p=stars_display_resource_info(&i);if(p.x!=8||p.y!=124||s.centers!=1||s.wraps!=10||s.small!=6||s.last_value!=30){fprintf(stderr,"one %d %d %u %u %u %d\n",p.x,p.y,s.centers,s.wraps,s.small,s.last_value);return 1;}memset(&s,0,sizeof s);init(&i,&s);i.race_stat=0;i.planet_value=20;p=stars_display_resource_info(&i);if(p.x!=8||p.y!=84||s.wraps!=6||s.last_id!=0xef){fprintf(stderr,"two %d %d %u %d\n",p.x,p.y,s.wraps,s.last_id);return 2;}puts("DISPLAY_RESOURCE_INFO_SOURCE=PASS");return 0;}
