#include <stdio.h>
#include <string.h>
#include "core.h"
typedef struct State{StarsU16 ids,extents;StarsU16 last_id,last_len;StarsI16 temporary;}State;
static StarsI16 get_string(void*c,StarsU16 id,StarsU8*out,StarsU16 n){State*s=(State*)c;s->ids++;s->last_id=id;s->last_len=n;if(n>3)strcpy((char*)out,"abcd");return 4;}
static StarsI16 extent(void*c,StarsU16 h,const StarsU8*t,StarsU16 n){State*s=(State*)c;s->extents++;(void)h;(void)t;return (StarsI16)(n==1?3:12);}
int main(void){StarsReportColumnHeaderInput i;State s;StarsU8 text[42];memset(&s,0,sizeof s);i.hdc=4;i.get_string=get_string;i.text_extent=extent;i.context=&s;if(stars_report_column_header(&i,1,2,text)!=36||s.last_id!=0x474U||s.extents!=2)return 1;if(stars_report_column_header(&i,0,1,text)!=50||s.last_id!=0x45aU)return 2;if(stars_report_column_header(&i,0,4,text)!=18||s.last_id!=0x468U)return 3;if(stars_report_column_header(&i,2,2,text)!=36||s.last_id!=0x480U)return 4;if(stars_report_column_header(&i,3,0,text)!=36||s.last_id!=0x48aU)return 5;if(stars_report_column_header(&i,9,0,text)!=0||text[0]!=0)return 6;puts("REPORT_COLUMN_HEADER_SOURCE=PASS");return 0;}
