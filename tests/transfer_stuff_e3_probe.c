#include "core.h"
#include <stdio.h>
#include <string.h>

#ifdef TARGET_NATIVE
#include <windows.h>
#endif

#define DLG_WM_INIT 0x0110U
#define DLG_WM_COMMAND 0x0111U
#define DLG_ID_OK 1U

typedef struct TransferE3Trace {
    unsigned char records[3][128];
    StarsI16 record_count;
    StarsI16 initial_lookups;
    StarsI16 update_lookups;
    StarsI16 rejected_lookups;
    StarsI16 prepares;
    StarsI16 splits;
    StarsI16 split_source_type;
    StarsI16 split_source_id;
    StarsI16 enumerations;
    StarsI16 dialogs;
    StarsI16 dialog_init;
    StarsI16 dialog_commands;
    StarsI16 dialog_setup;
    StarsI16 dialog_result;
    StarsI16 balances;
    StarsI16 refresh_objects;
    StarsI16 refreshes;
    StarsI16 invalidates;
    StarsI16 advances;
    StarsI16 reject_id;
    StarsI16 last_refresh_type;
    StarsI16 last_refresh_id;
} TransferE3Trace;

static void put16(unsigned char *raw, StarsI16 offset, StarsU16 value)
{
    raw[offset] = (StarsU8)value;
    raw[offset + 1] = (StarsU8)(value >> 8);
}

static StarsU16 get16(const unsigned char *raw, StarsI16 offset)
{
    return (StarsU16)(raw[offset] |
        ((StarsU16)raw[offset + 1] << 8));
}

static StarsI16 find_record(TransferE3Trace *trace, StarsI16 group,
    StarsI16 id)
{
    StarsI16 i;
    for (i = 0; i < trace->record_count; ++i)
        if ((StarsI16)get16(trace->records[i], 2) == group &&
                (StarsI16)get16(trace->records[i], 0) == id)
            return i;
    return -1;
}

static StarsI16 lookup_object(void *opaque, StarsI16 group, StarsI16 id,
    StarsTransferObject *object, StarsI16 update)
{
    TransferE3Trace *trace = (TransferE3Trace *)opaque;
    StarsI16 index;
    if (update)
        ++trace->update_lookups;
    else
        ++trace->initial_lookups;
    if (!update && trace->reject_id != 0 && id == trace->reject_id) {
        ++trace->rejected_lookups;
        return 0;
    }
    index = find_record(trace, group, id);
    if (index < 0 && update && trace->record_count < 3) {
        index = trace->record_count++;
        memset(trace->records[index], 0, sizeof trace->records[index]);
        put16(trace->records[index], 0, (StarsU16)id);
        put16(trace->records[index], 2, (StarsU16)group);
    }
    if (index < 0) {
        ++trace->rejected_lookups;
        return 0;
    }
    if (!update && object) {
        object->object_type = group;
        object->object_id = id;
        object->has_cargo = trace->records[index][16] != 0U;
        object->is_selected_planet = group == 1;
    } else if (update && object) {
        trace->records[index][16] = (StarsU8)(object->has_cargo != 0);
    }
    return 1;
}

static StarsI16 prepare(void *opaque, StarsTransferObject *object,
    StarsI16 mode)
{
    TransferE3Trace *trace = (TransferE3Trace *)opaque;
    ++trace->prepares;
    (void)object;
    (void)mode;
    return 1;
}

static StarsI16 split_fleet(void *opaque, StarsTransferObject *source,
    StarsTransferObject *result)
{
    TransferE3Trace *trace = (TransferE3Trace *)opaque;
    ++trace->splits;
    trace->split_source_type = source->object_type;
    trace->split_source_id = source->object_id;
    result->object_type = 2;
    result->object_id = 55;
    result->has_cargo = 1;
    result->is_selected_planet = 0;
    return 1;
}

static StarsI16 enumerate_hulls(void *opaque,
    const StarsTransferObject *object, StarsI16 *hulls, StarsI16 capacity)
{
    TransferE3Trace *trace = (TransferE3Trace *)opaque;
    ++trace->enumerations;
    if (capacity > 0) hulls[0] = 3;
    (void)object;
    return 1;
}

static void balance(void *opaque, StarsTransferObject *first,
    StarsTransferObject *second)
{
    ++((TransferE3Trace *)opaque)->balances;
    (void)first;
    (void)second;
}

static void refresh_object(void *opaque, StarsI16 group, StarsI16 id)
{
    TransferE3Trace *trace = (TransferE3Trace *)opaque;
    ++trace->refresh_objects;
    trace->last_refresh_type = group;
    trace->last_refresh_id = id;
}

static void refresh(void *opaque, StarsI16 type)
{
    ++((TransferE3Trace *)opaque)->refreshes;
    (void)type;
}

static void invalidate(void *opaque, StarsI16 window)
{
    ++((TransferE3Trace *)opaque)->invalidates;
    (void)window;
}

static void advance(void *opaque)
{
    ++((TransferE3Trace *)opaque)->advances;
}

static void dlg_rect(void *opaque, StarsU16 hwnd, StarsRect *rect)
{
    (void)opaque;
    (void)hwnd;
    rect->left = 0; rect->top = 0; rect->right = 240; rect->bottom = 120;
}

static StarsI16 dlg_string(void *opaque, StarsI16 id, StarsU8 *text,
    StarsU16 capacity)
{
    (void)opaque;
    (void)id;
    if (capacity > 3U) {
        text[0] = 'X'; text[1] = 'F'; text[2] = 'R'; text[3] = 0;
    }
    return 3;
}

static void dlg_title(void *opaque, StarsU16 hwnd, const StarsU8 *text)
{
    (void)opaque;
    (void)hwnd;
    (void)text;
}

static void dlg_setup(void *opaque, const StarsRect *rect)
{
    ++((TransferE3Trace *)opaque)->dialog_setup;
    (void)rect;
}

static void dlg_end(void *opaque, StarsU16 hwnd, StarsI16 result)
{
    TransferE3Trace *trace = (TransferE3Trace *)opaque;
    trace->dialog_result = result;
    (void)hwnd;
}

static StarsI16 show_dialog(void *opaque)
{
    TransferE3Trace *trace = (TransferE3Trace *)opaque;
    StarsTransferDlgOps ops;
    StarsTransferDlgInput input;
    StarsPoint sticky;
#ifdef TARGET_NATIVE
    MSG message;
    HTASK task;
#endif

    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    ++trace->dialogs;
    input.mode = 1U;
    input.valid_hulls = 1;
    input.tutor_active = 1U;
    input.sticky_point = &sticky;
    input.control_ids[0] = 1U;
    input.control_ids[1] = 2U;
    input.control_ids[2] = 3U;
    input.ops = &ops;
    input.context = trace;
    ops.get_client_rect = dlg_rect;
    ops.get_string = dlg_string;
    ops.set_window_text = dlg_title;
    ops.setup_buttons = dlg_setup;
    ops.advance_tutor = advance;
    ops.end_dialog = dlg_end;
#ifdef TARGET_NATIVE
    task = GetCurrentTask();
    if (!PostAppMessage(task, DLG_WM_INIT, 0, 0L) ||
            !PostAppMessage(task, DLG_WM_COMMAND, DLG_ID_OK, 0L))
        return 0;
    while (PeekMessage(&message, (HWND)0, 0, 0, PM_REMOVE)) {
        if (message.message == DLG_WM_INIT) {
            ++trace->dialog_init;
            (void)stars_transfer_dlg((StarsU16)message.hwnd,
                (StarsU16)message.message, (StarsU16)message.wParam,
                (StarsU32)message.lParam, &input);
        } else if (message.message == DLG_WM_COMMAND) {
            ++trace->dialog_commands;
            (void)stars_transfer_dlg((StarsU16)message.hwnd,
                (StarsU16)message.message, (StarsU16)message.wParam,
                (StarsU32)message.lParam, &input);
        }
    }
#else
    ++trace->dialog_init;
    (void)stars_transfer_dlg(100U, DLG_WM_INIT, 0U, 0UL, &input);
    ++trace->dialog_commands;
    (void)stars_transfer_dlg(100U, DLG_WM_COMMAND, DLG_ID_OK, 0UL, &input);
#endif
    return trace->dialog_result;
}

static void install(StarsTransferOps *ops, TransferE3Trace *trace)
{
    memset(ops, 0, sizeof *ops);
    ops->prepare_object = prepare;
    ops->split_fleet = split_fleet;
    ops->enumerate_hulls = enumerate_hulls;
    ops->show_dialog = show_dialog;
    ops->balance = balance;
    ops->refresh_selection = refresh;
    ops->invalidate_window = invalidate;
    ops->advance_tutor = advance;
    ops->lookup_object = lookup_object;
    ops->refresh_object = refresh_object;
    ops->context = trace;
}

static void add_record(TransferE3Trace *trace, StarsI16 group, StarsI16 id,
    StarsI16 has_cargo)
{
    StarsI16 index = trace->record_count++;
    memset(trace->records[index], 0, sizeof trace->records[index]);
    put16(trace->records[index], 0, (StarsU16)id);
    put16(trace->records[index], 2, (StarsU16)group);
    trace->records[index][16] = (StarsU8)(has_cargo != 0);
}

static StarsI16 check(StarsI16 condition, StarsI16 *checks)
{
    ++*checks;
    return condition ? 0 : 1;
}

int main(void)
{
    static TransferE3Trace trace;
    StarsTransferOps ops;
    StarsTransferInput input;
    StarsI16 checks;
    StarsI16 failures;

    memset(&trace, 0, sizeof trace);
    install(&ops, &trace);
    add_record(&trace, 2, 7, 1);
    memset(&input, 0, sizeof input);
    input.first.object_type = 2;
    input.first.object_id = 7;
    input.second.object_type = 4;
    input.second.object_id = -1;
    input.mode = 1;
    input.scanner_bits = 1;
    input.scanner_mode = 5;
    input.ops = &ops;
    failures = 0;
    checks = 0;
    stars_transfer_stuff(&input);
    failures += check(trace.initial_lookups == 1 && trace.prepares == 1,
        &checks);
    failures += check(trace.splits == 1 && trace.split_source_type == 2 &&
        trace.split_source_id == 7 && input.second.object_type == 2 &&
        input.second.object_id == 55, &checks);
    failures += check(trace.enumerations == 1 && trace.dialogs == 1 &&
        trace.dialog_init == 1 && trace.dialog_commands == 1 &&
        trace.dialog_setup == 1, &checks);
    failures += check(trace.balances == 1 && trace.update_lookups == 2 &&
        trace.refresh_objects == 2, &checks);
    failures += check(trace.refreshes == 1 && trace.invalidates == 2 &&
        trace.advances == 3 && input.mode == -1, &checks);

    memset(&trace, 0, sizeof trace);
    install(&ops, &trace);
    add_record(&trace, 1, 10, 1);
    add_record(&trace, 8, 80, 1);
    memset(&input, 0, sizeof input);
    input.first.object_type = 1;
    input.first.object_id = 10;
    input.second.object_type = 8;
    input.second.object_id = 80;
    input.mode = 0;
    input.ops = &ops;
    stars_transfer_stuff(&input);
    failures += check(trace.initial_lookups == 2 && trace.prepares == 2 &&
        trace.splits == 0 && trace.balances == 0, &checks);
    failures += check(trace.update_lookups == 2 && trace.refresh_objects == 2 &&
        input.mode == -1, &checks);

    memset(&trace, 0, sizeof trace);
    install(&ops, &trace);
    trace.reject_id = 99;
    add_record(&trace, 1, 10, 1);
    memset(&input, 0, sizeof input);
    input.first.object_type = 1;
    input.first.object_id = 10;
    input.second.object_type = 8;
    input.second.object_id = 99;
    input.ops = &ops;
    stars_transfer_stuff(&input);
    failures += check(trace.initial_lookups == 2 && trace.dialogs == 0 &&
        trace.rejected_lookups == 1, &checks);

#ifdef TARGET_NATIVE
    {
        FILE *report = fopen("C:\\TRCNE3\\TRCNE3.TXT", "w");
        if (!report) return 4;
        fprintf(report,
            "TRANSFER_STUFF_E3_CHECKS=%d FAILURES=%d RESULT=%s\n",
            (int)checks, (int)failures,
            failures == 0 ? "PASS" : "FAIL");
        fclose(report);
    }
#else
    printf("TRANSFER_STUFF_E3_CHECKS=%d FAILURES=%d RESULT=%s\n",
        (int)checks, (int)failures, failures == 0 ? "PASS" : "FAIL");
#endif
    return failures == 0 ? 0 : 1;
}
