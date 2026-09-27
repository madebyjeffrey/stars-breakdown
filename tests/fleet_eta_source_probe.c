#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace { StarsU16 id; StarsU16 color; } Trace;
static void get_text(void *context, StarsU16 hdc, StarsU8 *output,
    StarsU16 capacity)
{ Trace *trace=(Trace *)context; (void)hdc; (void)capacity; trace->id=1U; strcpy((char *)output,"12 years"); }
static void set_color(void *context, StarsU16 color)
{ ((Trace *)context)->color=color; }
int main(void)
{
    static const StarsU8 dash[] = "--";
    StarsGetEtaInput input; StarsGetEtaOps ops; StarsU8 output[32];
    StarsI16 years; Trace trace;
    memset(&input,0,sizeof input); memset(&ops,0,sizeof ops); memset(&trace,0,sizeof trace);
    input.cord=2U; input.hdc=1U; input.fuel_use=20UL; input.fuel_available=10UL;
    input.dash_text=dash; ops.get_text=get_text; ops.set_color=set_color;
    if (!stars_get_eta(&input,output,sizeof output,&years,&ops,&trace) ||
            strcmp((char *)output,"12 years")!=0 || years!=12 || trace.color!=0xffU) return 1;
    input.cord=1U;
    if (!stars_get_eta(&input,output,sizeof output,&years,&ops,&trace) ||
            strcmp((char *)output,"--")!=0 || years!=0) return 2;
    input.cord=2U; input.order_flags=0x1001U; input.order_words[0]=0x6000U;
    if (!stars_get_eta(&input,output,sizeof output,&years,&ops,&trace) || years!=0) return 3;
    puts("FLEET_ETA_SOURCE=PASS");
    return 0;
}
