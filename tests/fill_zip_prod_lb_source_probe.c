#include <stdio.h>
#include <string.h>
#include "core.h"
typedef struct Trace { StarsI16 gets,formats,copies,sends; StarsU16 list,send_codes[8],ids[8],kinds[8]; StarsRect rect; StarsU8 sent[8][40],fallback[12]; } Trace;
static StarsU16 item(void *v,StarsU16 w,StarsU16 id){(void)v;(void)w;(void)id;return 9;}
static void rect(void *v,StarsU16 w,StarsRect *r){Trace*t=(Trace*)v;t->rect.left=w;t->rect.top=1;t->rect.right=10;t->rect.bottom=20;(void)r;}
static void inval(void *v,StarsU16 w,const StarsRect *r,StarsU16 e){Trace*t=(Trace*)v;t->rect=*r;(void)w;(void)e;}
static void send(void *v,StarsU16 w,StarsU16 m,StarsU16 wp,const StarsU8 *s){Trace*t=(Trace*)v;StarsI16 n=t->sends++;t->list=w;t->send_codes[n]=m;if(s&&n<8)strcpy((char*)t->sent[n],(const char*)s);(void)wp;}
static const StarsU8 *compressed(void *v,StarsU16 id){Trace*t=(Trace*)v;if(id!=0x4c4)return (const StarsU8*)0;strcpy((char*)t->fallback,"EMPTY");return t->fallback;}
static void getstr(void *v,StarsU16 id,StarsU8 *out,StarsU16 n){Trace*t=(Trace*)v;StarsI16 k=t->gets++;t->ids[k]=id;if(n>1){out[0]=(StarsU8)('A'+k);out[1]=0;}}
static void copystr(void *v,StarsU8 *out,const StarsU8 *in){Trace*t=(Trace*)v;t->copies++;strcpy((char*)out,(const char*)in);}
static void formatstr(void *v,StarsU8 *out,const StarsU8 *fmt,const StarsU8 *in,StarsU16 kind){Trace*t=(Trace*)v;t->formats++;t->kinds[t->formats-1]=kind;(void)fmt;strcpy((char*)out,(const char*)in);}
int main(void){StarsZipProductionQueue q;StarsFillZipProdInput i;Trace t;memset(&q,0,sizeof q);memset(&i,0,sizeof i);memset(&t,0,sizeof t);i.static_y=30;i.line_height=8;i.get_dialog_item=item;i.get_client_rect=rect;i.invalidate_rect=inval;i.send_message=send;i.compressed_string=compressed;i.get_string=getstr;i.copy_string=copystr;i.format_string=formatstr;i.context=&t;q.valid=0;q.item_count=0;stars_fill_zip_prod_lb(4,&q,&i);if(t.sends!=2||t.send_codes[0]!=0x405||t.send_codes[1]!=0x401||strcmp((char*)t.sent[1],"EMPTY")!=0||t.rect.top!=30||t.rect.bottom!=38)return 1;memset(&t,0,sizeof t);memset(&q,0,sizeof q);q.valid=1;q.item_count=2;q.items[0]=0x4002;q.items[1]=3;stars_fill_zip_prod_lb(4,&q,&i);if(t.sends!=3||t.formats!=1||t.copies!=1||t.ids[0]!=0x4c5||t.ids[1]!=0x80||t.ids[2]!=0x81)return 2;puts("FILL_ZIP_PROD_LB_SOURCE=PASS");return 0;}
