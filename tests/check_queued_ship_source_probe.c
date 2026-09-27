#include <stdio.h>
#include <string.h>
#include "core.h"
static StarsI16 queued(void *c, StarsU16 id, StarsI16 edit, StarsI16 *progress)
{ (void)c;(void)id;(void)edit;*progress=12;return 2; }
static void format(void *c, StarsU16 id, StarsU32 existing, StarsI16 count,
        StarsI16 progress, StarsU16 hull, StarsI16 group, StarsU8 *text)
{ (void)c;(void)id;(void)existing;(void)count;(void)progress;(void)hull;
  text[0]=(StarsU8)group;text[1]=0; }
static StarsI16 message(void *c, StarsU16 focus, const StarsU8 *text,
        StarsU16 title, StarsU16 style)
{ StarsU16 *result=(StarsU16 *)c;(void)focus;(void)text;(void)title;(void)style;return (StarsI16)*result; }
static StarsU16 get_focus(void *c){(void)c;return 9;}
static void focus(void *c, StarsU16 hwnd){StarsU16 *n=(StarsU16 *)c;(void)hwnd;*n=(*n==99)?100:(StarsU16)(*n+1);}
static void remove_q(void *c, StarsU16 id, StarsI16 edit){StarsU16 *n=(StarsU16 *)c;(void)id;(void)edit;*n=110;}
static void destroy(void *c, StarsU16 id, StarsI16 player){StarsU16 *n=(StarsU16 *)c;(void)id;(void)player;*n=120;}
static void invalidate(void *c, StarsU16 hwnd){StarsU16 *n=(StarsU16 *)c;(void)hwnd;*n=(StarsU16)(*n+1);}
int main(void)
{
    StarsCheckQueuedShipInput i;StarsU16 state;
    memset(&i,0,sizeof(i));i.hwnd=4;i.scanner_window=5;i.message_window=6;
    i.design_id=3;i.existing_count=0;i.player=2;i.edit_mode=1;
    i.queued_count=queued;i.format_warning=format;i.message_box=message;
    i.get_focus=get_focus;i.set_focus=focus;i.remove_from_queues=remove_q;i.context=&state;
    state=6;if(stars_check_queued_ship(&i)!=1||state!=110)return 1;
    state=7;if(stars_check_queued_ship(&i)!=0)return 1;
    i.existing_count=1;state=6;i.destroy_design=destroy;i.invalidate=invalidate;
    if(stars_check_queued_ship(&i)!=1||state!=122)return 1;
    puts("CHECK_QUEUED_SHIP_SOURCE=PASS");return 0;
}
