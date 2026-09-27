#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 prepares;
    StarsI16 jettisons;
    StarsI16 splits;
    StarsI16 enumerations;
    StarsI16 dialogs;
    StarsI16 balances;
    StarsI16 deletes;
    StarsI16 cancels;
    StarsI16 refreshes;
    StarsI16 invalidates;
    StarsI16 advances;
} Trace;
static StarsI16 prepare(void *context, StarsTransferObject *object,
    StarsI16 mode)
{
    (void)object; (void)mode; ++((Trace *)context)->prepares; return 1;
}
static void jettison(void *context, StarsTransferObject *object)
{
    (void)object; ++((Trace *)context)->jettisons;
}
static StarsI16 split(void *context, StarsTransferObject *source,
    StarsTransferObject *result)
{
    Trace *trace = (Trace *)context;
    (void)source; ++trace->splits; result->object_type = 4;
    result->object_id = 9; result->has_cargo = 1; return 1;
}
static StarsI16 enumerate_hulls(void *context,
    const StarsTransferObject *object, StarsI16 *hulls, StarsI16 capacity)
{
    (void)object; (void)capacity; ++((Trace *)context)->enumerations;
    hulls[0] = 2; return 1;
}
static StarsI16 dialog(void *context)
{
    ++((Trace *)context)->dialogs; return 1;
}
static void balance(void *context, StarsTransferObject *first,
    StarsTransferObject *second)
{
    (void)first; (void)second; ++((Trace *)context)->balances;
}
static void delete_fleet(void *context, StarsTransferObject *object)
{
    (void)object; ++((Trace *)context)->deletes;
}
static void cancel_memory(void *context)
{
    ++((Trace *)context)->cancels;
}
static void refresh(void *context, StarsI16 type)
{
    (void)type; ++((Trace *)context)->refreshes;
}
static void invalidate(void *context, StarsI16 window)
{
    (void)window; ++((Trace *)context)->invalidates;
}
static void advance(void *context)
{
    ++((Trace *)context)->advances;
}
static void install(StarsTransferOps *ops, Trace *trace)
{
    memset(ops, 0, sizeof *ops);
    ops->prepare_object = prepare;
    ops->jettison = jettison;
    ops->split_fleet = split;
    ops->enumerate_hulls = enumerate_hulls;
    ops->show_dialog = dialog;
    ops->balance = balance;
    ops->delete_fleet = delete_fleet;
    ops->cancel_memory = cancel_memory;
    ops->refresh_selection = refresh;
    ops->invalidate_window = invalidate;
    ops->advance_tutor = advance;
    ops->context = trace;
}
int main(void)
{
    StarsTransferInput input;
    StarsTransferOps ops;
    Trace trace;

    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    install(&ops, &trace);
    input.first.object_type = 1;
    input.second.object_type = 2;
    input.second.has_cargo = 1;
    input.mode = 1;
    input.scanner_bits = 1;
    input.scanner_mode = 5;
    input.ops = &ops;
    stars_transfer_stuff(&input);
    if (trace.prepares != 2 || trace.enumerations != 1 ||
            trace.dialogs != 1 || trace.balances != 1 ||
            trace.refreshes != 1 || trace.invalidates != 2 ||
            trace.advances != 1 || input.mode != -1)
        return 1;
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    install(&ops, &trace);
    input.first.object_type = 1;
    input.second.object_type = 4;
    input.mode = 1;
    input.dialog_result = 0;
    ops.show_dialog = (StarsTransferDialog)0;
    input.ops = &ops;
    stars_transfer_stuff(&input);
    if (trace.splits != 1 || trace.deletes != 1 || trace.cancels != 1 ||
            trace.balances != 0)
        return 1;
    puts("TRANSFER_STUFF_CHECKS=20 FAILURES=0 RESULT=PASS");
#ifdef TARGET_NATIVE
    {
        FILE *report = fopen("C:\\TRNS\\TRNSR.TXT", "w");
        if (!report)
            return 4;
        fputs("TRANSFER_STUFF_CHECKS=20 FAILURES=0 RESULT=PASS\n", report);
        fclose(report);
    }
#endif
    return 0;
}
