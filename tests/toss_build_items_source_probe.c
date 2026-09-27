#include <stdio.h>
#include "core.h"
typedef struct Trace { unsigned freed; } Trace;
static void release(void *c, StarsProductionItem *items)
{ Trace *t=(Trace *)c; (void)items; ++t->freed; }
int main(void)
{
    StarsProductionItem items[4]; StarsTossBuildItemsInput input; Trace trace;
    items[0].packed=0x00000400UL; /* wrong type */
    items[1].packed=(1UL<<17)|(3UL<<10)|0x11UL;
    items[2].packed=(1UL<<17)|(8UL<<10)|0x22UL; /* wrong part */
    items[3].packed=(1UL<<17)|(6UL<<10)|0x33UL;
    trace.freed=0U;input.items=items;input.item_count=4U;input.free_items=release;input.context=&trace;
    stars_toss_non_auto_build_items(&input);
    if(input.item_count!=2U||items[0].packed!=(1UL<<17|3UL<<10|0x11UL)||items[1].packed!=(1UL<<17|6UL<<10|0x33UL))return 1;
    items[0].packed=0x00000400UL;items[1].packed=0x00000800UL;input.item_count=2U;input.items=items;
    stars_toss_non_auto_build_items(&input);
    if(input.items!=0||trace.freed!=1U)return 2;
    puts("TOSS_BUILD_SOURCE=PASS");return 0;
}
