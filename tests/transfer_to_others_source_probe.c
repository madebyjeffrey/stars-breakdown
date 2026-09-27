#include <stdio.h>
#include <string.h>
#include "core.h"
typedef struct Trace { StarsTransferMessage messages[16]; StarsI16 count; StarsI32 accepted; } Trace;
static StarsI16 lookup(void *context, StarsI16 group, StarsU16 id, StarsI16 *owner)
{ (void)context; if (id==99U) return 0; *owner=(StarsI16)(group==1 ? 1 : 2); return 1; }
static StarsI32 apply(void *context, StarsI16 group, StarsU16 id, StarsI16 supply, StarsI32 requested)
{ Trace *t=(Trace *)context; (void)group; (void)id; (void)supply; return t->accepted < requested ? t->accepted : requested; }
static void message(void *context, const StarsTransferMessage *message)
{ Trace *t=(Trace *)context; t->messages[t->count++]=*message; }
int main(void)
{
    StarsTransferFullRecord record; StarsTransferToOthersInput input; Trace trace;
    memset(&record,0,sizeof record); memset(&input,0,sizeof input); memset(&trace,0,sizeof trace);
    record.source_id=3; record.destination_id=8; record.source_group=2; record.destination_group=1;
    record.quantities[0]=100UL; record.quantities[4]=20UL;
    trace.accepted=100L; input.records=&record; input.record_count=1;
    input.lookup_owner=lookup; input.apply_cargo=apply; input.send_message=message; input.context=&trace;
    stars_transfer_to_others(&input);
    if (trace.count!=4 || trace.messages[0].event!=0x42 ||
            trace.messages[1].event!=0x44 || trace.messages[2].event!=0x43 ||
            trace.messages[0].source_tag!=0x8003U) return 1;
    memset(&trace,0,sizeof trace); trace.accepted=0L; stars_transfer_to_others(&input);
    if (trace.count!=4 || trace.messages[0].event!=0x4a ||
            trace.messages[1].event!=0x48) return 2;
    puts("TRANSFER_TO_OTHERS_SOURCE=PASS"); return 0;
}
