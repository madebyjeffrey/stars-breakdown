#include "core.h"
#include <stdio.h>
#include <string.h>
typedef struct Probe { StarsU8 packet[42]; StarsU16 length; int prepends; } Probe;
static StarsI16 id(void *c,StarsI16 n){(void)c;return n;}
static StarsU16 count(void *c,StarsI16 n){(void)c;(void)n;return 2;}
static StarsU16 flags(void *c,StarsI16 n){(void)c;(void)n;return 0;}
static void append(void *c,const StarsU8 *p,StarsU16 n){Probe *q=(Probe *)c;memcpy(q->packet,p,n);q->length=n;}
static void prepend(void *c,const StarsU8 *p,StarsU16 n){Probe *q=(Probe *)c;memcpy(q->packet,p,n);q->length=n;++q->prepends;}
int main(void)
{
    StarsMessageState s; StarsMessageOps o; Probe p; StarsI16 vals[2]={3,(StarsI16)0x1200}; StarsU8 out[42]; int failures=0;
    memset(&s,0,sizeof(s));memset(&o,0,sizeof(o));memset(&p,0,sizeof(p));s.message_count=3;s.player_message_count=2;s.current_message=0;s.view_filtered=1;o.message_id=id;o.parameter_count=count;o.player_flags=flags;o.append_message=append;o.prepend_message=prepend;
    if(stars_message_next(&s,0,&o,&p)!=1||stars_message_previous(&s,0,&o,&p)!=-1)++failures;
    if(stars_package_player_message(&s,out,2,9,7,vals,&o,&p)!=8||out[0]!=0x32||out[1]!=9||out[2]!=4||out[3]!=7||out[5]!=3||out[6]!=0||out[7]!=0x12)++failures;
    if(stars_package_player_message(&s,out,0,0x141,-2,vals,&o,&p)!=8||out[1]!=0x41||out[2]!=5||out[3]!=0xfe||out[4]!=0xff)++failures;
    if(stars_send_player_message2(&s,2,9,7,3,4,&o,&p)!=1||s.message_count!=4||p.length!=7)++failures;
    if(stars_send_player_message_prepend(&s,2,9,7,vals,&o,&p)!=1||s.message_count!=5||p.prepends!=1||p.length!=8)++failures;
    if(stars_send_player_message2_xgen(&s,1,9,7,3,0x1200,&o,&p)!=1||s.sent[1]!=2||p.prepends!=2||s.current_message!=0)++failures;
    printf("MESSAGES_CHECKS=6 FAILURES=%d RESULT=%s\n",failures,failures?"FAIL":"PASS");return failures!=0;
}
